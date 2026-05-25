/* 
 * Group ID: 5
 * 
 * Board number: 14
 * 
 * Authors: 
 * Joel TOPULLI (8663382)
 * Edda Kulle (10217725)
 * Giacomo Nogarin (8654515)
 */

#include "xc.h"
#include "uart.h"

static volatile char receive_data [R_BUF_SIZE];
static volatile char transmit_data[T_BUF_SIZE];

// Initializing circular buffers
Circular_Buffer receive_buffer  = {receive_data,  0, 0, R_BUF_SIZE};
Circular_Buffer transmit_buffer = {transmit_data, 0, 0, T_BUF_SIZE};

// RX interrupt
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void) {
    IFS0bits.U1RXIF = 0;  

    while (U1STAbits.URXDA == 1) {
        cb_produce(&receive_buffer, U1RXREG);       // If full we drop the data
    }
}

//TX interrupt
void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void) {
    IFS0bits.U1TXIF = 0;  
    char c;

    if (cb_consume(&transmit_buffer, &c)) {
        U1TXREG = c;
    } else {
        // if the buffer is empty we disavle it until next transmit
        IEC0bits.U1TXIE = 0;    
        // the disabling is done so we don't have a loop
    }
}


/* The baud rate that we have chosen is 115200
 */
void uart_setup() {

    TRISDbits.TRISD11 = 1;      // RD11 as input 
    TRISDbits.TRISD0  = 0;      // RD0  as output 

    // Pin remapping
    RPINR18bits.U1RXR = 75;     // Map U1RX input  to RPI75 (RD11) 
    RPOR0bits.RP64R   = 0x01;   // Map U1TX output to RP64  (RD0) 

    // 72000000 / (4 � 115200) - 1 = 155.25
    U1MODEbits.BRGH = 1;
    // a baud rate of 115200 allows us to send 11.5 bytes/ms. Better than the 1 byte/ms of the 9600
    
    U1BRG = 155;            
    
    // Enable UART module and transmitter 
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN   = 1;

    // Enable RX interrupt
    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 1;
}

// Queues stings in buffer and begins totransmit
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
        IEC0bits.U1TXIE = 0;
        U1TXREG = c;
        IEC0bits.U1TXIE = 1;    // Enable the uart so the rest is sent by interrupt
    }
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

// Converting a received frequency value into the corresponding loop period
int uart_frequency_change(int value, int current) {

    switch (value) {
        case  0: return 0;          //disable
        case  1: return 100;        //1Hz = 1000ms thus 100 loops 
        case  2: return 50;         //2Hz = 500ms thus 50 loops
        case  5: return 20;         //5Hz = 200ms thus 20 loops
        case 10: return 10;         //10Hz = 100ms thus 10 loops
        default:
            // If a value outside the scope is received, send error message 2 and leave the frequency unchanged 
            uart_transmit("$ERR,2*");
            return current;
    }
}
