#ifndef UART_H
#define UART_H

#define R_BUF_SIZE  128
#define PI  3.14159265358979323846f
/*
    For the receive buffer we have to consider the frequency (we changed this from a period of 50 to a period of 5) that we empty it and the baud rate itself. We know that almost 11.52 bytes come every ms. And our parse UART fires every 10 ms which means that we need to save at least 116 bytes before the parse uart can drain it. 

    But we want to make sure that everything is handled correctly, hence we need a bit more space in the case that some process takes more that it is required. Coming to the value: 128 which is a power of 2 but also allows enough free space. 
 */

#define T_BUF_SIZE  128
/*
    $MANGLE,%.2f,%.2f,%.2f*\n -> 33 bytes every 100 ms
    $MDIST,%d*\n -> 12 bytes every 100 ms
    $MBATT,%.2f*\n -> 13 bytes every 500ms
    $MBUF,%d,%d*\n -> 15 bytes every 100 ms

    33+12+13+15 = 73 characters (to transmit all data we need 7 ms if we round up)

    Since the time to transmit all the data is less that the period of the tasks we need to worry only about the maximum we we have to send in one loop. Hence we need a buffer size which is larger than 73 characters. 
    
    !We can choose 80, however we would prefer 128 as it is a power of two which makes it easier for the head and the tail to wrap around it (in cpu cycles). It also has more space in case the sensor precision is turns to mm for the IR read. 
*/

typedef struct{
    volatile char *data;    // Buffer array
    volatile int head;      // Write index (volatiole bc they are accessed by the uart ISR)
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

// Number of bytes currently queued in the RX / TX buffers
int uart_rx_count(void);

int uart_tx_count(void);

#endif