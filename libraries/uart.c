#include "xc.h"
#include "uart.h"

static volatile char receive_data [R_BUF_SIZE];
static volatile char transmit_data[T_BUF_SIZE];

// Initializing circular buffers
static Circular_Buffer receive_buffer  = {receive_data,  0, 0, R_BUF_SIZE};
static Circular_Buffer transmit_buffer = {transmit_data, 0, 0, T_BUF_SIZE};

// RX interrupt which fires when a byte arrives
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void) {
    IFS0bits.U1RXIF = 0;  

    while (U1STAbits.URXDA == 1) {  // the while is to drain all of the bytes in the queue and not just one at a time
        cb_produce(&receive_buffer, U1RXREG);       // If full, drop the data
    }
}

// TX interrupt that fires when the tx register is empty
void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void) {
    IFS0bits.U1TXIF = 0;  
    char c;

    if (cb_consume(&transmit_buffer, &c)) {
        U1TXREG = c;
    } else {
        // if the buffer is empty we disavle it until next transmit
        IEC0bits.U1TXIE = 0;    
        // the disabling is done so we don't have a loop that executes forever
    }
}

void uart_setup() {

    TRISDbits.TRISD11 = 1;      // RD11 as input 
    TRISDbits.TRISD0  = 0;      // RD0  as output 

    // Pin remapping
    RPINR18bits.U1RXR = 75;     // Map U1RX input  to RPI75 (RD11) 
    RPOR0bits.RP64R   = 0x01;   // Map U1TX output to RP64  (RD0) 

    // 72000000 / (4 * 115200) - 1 = 155.25
    U1MODEbits.BRGH = 1;
    // a baud rate of 115200 allows to send 11.5 bytes/ms. Better than the 1 byte/ms of the 9600
    
    U1BRG = 155;                    //  if we want to change to the 9600 baud replace with 1874 bc 72000000 / (4 * 9600) - 1 = 1874
    

    U1MODEbits.UARTEN = 1;      // Enable UART module 
    U1STAbits.UTXEN   = 1;      // enable transmitter 


    IFS0bits.U1RXIF = 0;        // clear rx flag
    IEC0bits.U1RXIE = 1;        // enable rx interrupt
}

void uart_transmit(const char *message) {
    
    char c;

    // Fill the circular buffer with every character of the string
    while (*message) {
        while (!cb_produce(&transmit_buffer, *message)){
            break;  // if the buffer is full we drop the extra bytes so it doesn't spin indefinetly 
        } 
        message++;
    }

    // Manually send the first byte to kick off the interrupt chain
    if (cb_consume(&transmit_buffer, &c)) {
        IEC0bits.U1TXIE = 0;    // disable just to be 100% sure
        U1TXREG = c;            // we sent the first byte manually
        IEC0bits.U1TXIE = 1;    // Enable the uart so the rest is sent by interrupt
    }
    //? the uart irs fires only when the tx register becomes empty. So we send one byte to the register and enable the IRS. the byte is consumed -> the irs is called bc now the register is empty and the whole message is pulled from the buffer until it become empty agin and the irs is disabled
}

// Attempts to add byte to buffer, returns 1 if successful
int cb_produce(Circular_Buffer *cb, char c) {
    
    int next = (cb->head + 1) % cb->buf_size;

    if (next == cb->tail) {
        return 0;           // Buffer full, byte dropped
    }

    cb->data[cb->head] = c;
    cb->head = next;
    return 1;
}

// Attempts to remove one byte from the buffer, returns 1 on success
int cb_consume(Circular_Buffer *cb, char *out) {
    
    if (cb->tail == cb->head) {
        return 0;           // Buffer empty 
    }

    *out = cb->data[cb->tail];
    cb->tail = (cb->tail + 1) % cb->buf_size;
    return 1;
}


// Reads one character from the UART RX circular buffer into *out
int uart_receive_char(char *out) {
    return cb_consume(&receive_buffer, out);
}

// Bytes currently queued, computed from head/tail with wrap-around
int uart_rx_count(void) {
    return (receive_buffer.head + R_BUF_SIZE - receive_buffer.tail) % R_BUF_SIZE;
}

int uart_tx_count(void) {
    return (transmit_buffer.head + T_BUF_SIZE - transmit_buffer.tail) % T_BUF_SIZE;
}
