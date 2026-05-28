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

#ifndef UART_H
#define UART_H

#define R_BUF_SIZE  64
#define PI  3.14159265358979323846
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
    volatile char *data;    //buffer array
    volatile int head;      // write index  
    volatile int tail;      // read index
    int buf_size;     // buffer length
}Circular_Buffer;

// initialize uart 
void uart_setup();

// add char to buffer
int cb_produce(Circular_Buffer *cb, char c);

// get char from buffer
int cb_consume(Circular_Buffer *cb, char *out);

// send string over uart
void uart_transmit(const char* message);

// read received char
int uart_receive_char(char *out);

// change frequency according to input
int uart_frequency_change(int value, int current);

// reads one line
int uart_receive_line(char *out, int max_len);

#endif