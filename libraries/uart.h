#ifndef UART_H
#define UART_H

#define R_BUF_SIZE  64
#define PI  3.14159265358979323846f
/*
 * Allows the user to send up to 9 commands since they are 7bytes each.
 * We chose this value just for extreme test cases. However even 16 is an acceptable number
 */

#define T_BUF_SIZE  64
/*
 ACC transmits in total 26 bytes, in the worst case
 ANG transmits in total 19 bytes, in the worst case
 If we also want to debug and see the period misses we have to consider another 11 bytes at max
 If we also consider an Error which can come in an asynch way we have 7 more bytes
 
 As a result at the worst case 26+19+11+7 = 63
 We use the size 64 bc it close to the theoretical maximum and is a power of 2. (helps with %)
 */


typedef struct{
    volatile char *data;    // Buffer array
    volatile int head;      // Write index  
    volatile int tail;      // Read index
    int buf_size;           // Buffer length
}Circular_Buffer;

// Setup UART
void uart_setup();

// Send string over UART
void uart_transmit(const char* message);

// Add char to buffer
int cb_produce(Circular_Buffer *cb, char c);

// Get char from buffer
int cb_consume(Circular_Buffer *cb, char *out);

// Read received char
int uart_receive_char(char *out);

// Read one line
int uart_receive_line(char *out, int max_len);

// Number of bytes currently queued in the RX / TX buffers
int uart_rx_count(void);

int uart_tx_count(void);

#endif