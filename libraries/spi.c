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
#include "spi.h"
#include "uart.h"  
#include <math.h>
#include "timer.h"

#define PI  3.14159265358979323846

void spi_setup(void) {

    SPI1STATbits.SPIEN = 0;     // Disable SPI before changing configuration 

    TRISAbits.TRISA1 = 1;      // RA1  / RPI17 - MISO (input)        
    TRISFbits.TRISF12 = 0;      // RF12 / RP108 - SCK  (output)
    TRISFbits.TRISF13 = 0;      // RF13 / RP109 - MOSI (output) 

    // Pin remapping
    RPINR20bits.SDI1R = 0b0010001;  // SDI1 (MISO) - RPI17 [RA1
    RPOR12bits.RP109R = 0b000101;   // SDO1 (MOSI) - RP109 [RF13]
    RPOR11bits.RP108R = 0b000110;   // SCK1        - RP108 [RF12]

    SPI1CON1bits.MSTEN  = 1;    // Master mode 
    SPI1CON1bits.MODE16 = 0;    // 8-bit transfer width
    SPI1CON1bits.CKP = 1;       // Set the clock idle to 1

    SPI1CON1bits.PPRE = 3;      // Primary prescaler 1:1
    SPI1CON1bits.SPRE = 3;      // Secondary prescaler 3:1

    SPI1STATbits.SPIROV = 0;    // Clear any overflow flag from previous use 
    SPI1STATbits.SPIEN  = 1;    // Enable SPI 

    TRISDbits.TRISD6 = 0;       // RD6 - CS3: Magnetometer
    TRISBbits.TRISB4 = 0;       // RB4 - CS2: Gyroscope
    TRISBbits.TRISB3 = 0;       // RB3 - CS1: Accelerometer

    LATDbits.LATD6 = 1;         // Deselect magnetometer
    LATBbits.LATB4 = 1;         // Deselect gyroscope
    LATBbits.LATB3 = 1;         // Deselect accelerometer
}

// Does SPI Byte transfer
unsigned int spi_write(unsigned int data) {

    while (SPI1STATbits.SPITBF == 1);  // Wait until TX buffer is free
    SPI1BUF = data;                    // Load byte, this also triggers a clock pulse
    while (SPI1STATbits.SPIRBF == 0);  // Wait until the full transfer completes 

    return SPI1BUF;     // Return the data obtained from the slave
}

// Sets the accelerometer bandwidth filter with register 0x10
// Valid values 8-15
void accel_bw(int bw) {

    LATBbits.LATB3 = 0;            // Assert CS, select accelerometer
    spi_write(0x10 & 0x7F);        // Select the register and specify that we will write to it

    switch (bw) {
        case  8: spi_write(0x08); break;
        case  9: spi_write(0x09); break;
        case 10: spi_write(0x0A); break;
        case 11: spi_write(0x0B); break;
        case 12: spi_write(0x0C); break;
        case 13: spi_write(0x0D); break;
        case 14: spi_write(0x0E); break;
        case 15: spi_write(0x0F); break;
        default:
            // If a number which is out of bounds is received, send error to uart
            LATBbits.LATB3 = 1;
            uart_transmit("$ERR,1*");
            return;
    }

    LATBbits.LATB3 = 1;     //deselect when write is complete
}


// Sets up the magnetometer to go from sleep-mode to normal mode
// Not needed in this assignment, just for future usability
void mag_setup(void) {

    // Make sure other SPI devices are deselected
    LATBbits.LATB3 = 1;   
    LATBbits.LATB4 = 1;   
    LATDbits.LATD6 = 1;   

    // Suspend mode -> sleep mode
    LATDbits.LATD6 = 0;          

    spi_write(0x4B & 0x7F);      
    spi_write(0x01);             

    LATDbits.LATD6 = 1;          

    // Wait required startup time
    tmr_wait_ms(TIMER3, 3);

    // Sleep mode -> normal mode
    LATDbits.LATD6 = 0;          

    spi_write(0x4C & 0x7F);      
    spi_write(0x00);    // normal mode

    LATDbits.LATD6 = 1;          
}

// Reads data from the accelerometer and computes to g and computes roll and pitch angles
Sensor_DataStruct accel_read() {

    Sensor_DataStruct result;
    uint16_t LSB_part, MSB_part;
    int16_t ax, ay, az;

    LATBbits.LATB3 = 0;             // select accelerometer
    spi_write(0x02 | 0x80);         // Starting register 0x02 with read flag set 
    
    // Auto-incrementing reads 
    // Combine LSB and MSB, then logical right-shift by 4 to get complete value
    // x axis
    LSB_part = spi_write(0x00); 
    MSB_part = spi_write(0x00); 
    ax = ((int16_t)((MSB_part << 8) | LSB_part)) >> 4;

    // y axis
    LSB_part = spi_write(0x00); 
    MSB_part = spi_write(0x00); 
    ay = ((int16_t)((MSB_part << 8) | LSB_part)) >> 4;

    // z axis
    LSB_part = spi_write(0x00); 
    MSB_part = spi_write(0x00); 
    az = ((int16_t)((MSB_part << 8) | LSB_part)) >> 4;

    LATBbits.LATB3 = 1;             // deselect        

    // Convert raw counts to g
    result.axis_x = (float)ax * 0.00098;
    result.axis_y = (float)ay * 0.00098;
    result.axis_z = (float)az * 0.00098;
    

    //roll  = atan2(ay, az) - rotation around X axis
    //pitch = atan2(-ax, sqrt(ay^2+az^2)) - rotation around Y axis
    //atan2 returns radians -> multiply by 180/Pi for degrees
    result.roll  = atan2f(result.axis_y, result.axis_z ) * (180.0 / PI);
    result.pitch = atan2f(-result.axis_x, sqrtf(result.axis_y * result.axis_y + result.axis_z  * result.axis_z )) * (180.0 / PI);

    return result;
}

// Read from Magnetometer (Not needed for this Exercise but for later modularity)
Sensor_DataStruct mag_read() {

    Sensor_DataStruct result;
    uint16_t LSB_part, MSB_part;
    int16_t ax, ay, az;

    // read from the magnetometer starting at register 0x42
    // CS3 (RD6) is the magnetometer chip select
    LATDbits.LATD6 = 0;            // Assert CS3 - select magnetometer
    spi_write(0x42 | 0x80);        // Address byte, read flag set 

    // x axis
    LSB_part = spi_write(0x00) & 0xF8; 
    MSB_part = spi_write(0x00);
    ax = ((int16_t)((MSB_part << 8) | LSB_part)) >> 3;

    // y axis
    LSB_part = spi_write(0x00) & 0xF8;
    MSB_part = spi_write(0x00);
    ay = ((int16_t)((MSB_part << 8) | LSB_part)) >> 3;

    // z axis
    LSB_part = spi_write(0x00) & 0xFE; 
    MSB_part = spi_write(0x00);
    az = ((int16_t)((MSB_part << 8) | LSB_part)) >> 1;

    LATDbits.LATD6 = 1;
    
    result.axis_x = ax;
    result.axis_y = ay;
    result.axis_z = az;

    return result;
}

// Allows to choose whether to read from the magnetometer or the accelerometer
Sensor_DataStruct sensor_read(int select) {
    if (select == ACC) {
        return accel_read();
    }
    return mag_read();
}
