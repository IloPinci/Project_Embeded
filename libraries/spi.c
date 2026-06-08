#include "xc.h"
#include "spi.h"
#include "uart.h"  
#include <math.h>
#include "timer.h"

// Setup the SPI
void spi_setup(void) {

    SPI1STATbits.SPIEN = 0;     // Disable SPI before changing configuration 

    TRISAbits.TRISA1 = 1;       // RA1  / RPI17 - MISO (input)        
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

// Sets up the magnetometer to go from sleep-mode to normal mode
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
    spi_write(0x00);    // Normal mode

    LATDbits.LATD6 = 1;          
}

// Reads data from the accelerometer and computes to result and computes roll and pitch angles
AccelData accel_read() {

    AccelData result;
    uint16_t LSB_part, MSB_part;
    int16_t ax, ay, az;

    LATBbits.LATB3 = 0;             // Select accelerometer
    spi_write(0x02 | 0x80);         // Starting register 0x02 with read flag set 
    
    // Auto-incrementing reads 
    // Combine LSB and MSB, then logical right-shift by 4 to get complete value
    // X axis
    LSB_part = spi_write(0x00); 
    MSB_part = spi_write(0x00); 
    ax = ((int16_t)((MSB_part << 8) | LSB_part)) >> 4;

    // Y axis
    LSB_part = spi_write(0x00); 
    MSB_part = spi_write(0x00); 
    ay = ((int16_t)((MSB_part << 8) | LSB_part)) >> 4;

    // Z axis
    LSB_part = spi_write(0x00); 
    MSB_part = spi_write(0x00); 
    az = ((int16_t)((MSB_part << 8) | LSB_part)) >> 4;

    LATBbits.LATB3 = 1;         // Deselect        

    // Convert raw counts to result
    result.axis_x = (float) ax * 0.00098;
    result.axis_y = (float) ay * 0.00098;
    result.axis_z = (float) az * 0.00098;
    

    //Roll  = atan2(ay, az) - rotation around X axis
    //Pitch = atan2(-ax, sqrt(ay^2+az^2)) - rotation around Y axis
    //atan2 returns radians -> multiply by 180/Pi for degrees
    result.roll  = atan2f(result.axis_y, result.axis_z ) * (180.0 / PI);
    result.pitch = atan2f(-result.axis_x, sqrtf(result.axis_y * result.axis_y + result.axis_z  * result.axis_z )) * (180.0 / PI);

    return result;
}

// Read data from magnetometer
MagData mag_read() {

    MagData result;
    uint16_t LSB_part, MSB_part;
    int16_t ax, ay, az;

    LATDbits.LATD6 = 0;            // Select magnetometer
    spi_write(0x42 | 0x80);        // Starting register 0x42 with read flag set 

    // X axis
    LSB_part = spi_write(0x00) & 0xF8; 
    MSB_part = spi_write(0x00);
    ax = ((int16_t)((MSB_part << 8) | LSB_part)) >> 3;

    // Y axis
    LSB_part = spi_write(0x00) & 0xF8;
    MSB_part = spi_write(0x00);
    ay = ((int16_t)((MSB_part << 8) | LSB_part)) >> 3;

    // Z axis
    LSB_part = spi_write(0x00) & 0xFE; 
    MSB_part = spi_write(0x00);
    az = ((int16_t)((MSB_part << 8) | LSB_part)) >> 1;

    LATDbits.LATD6 = 1;     // deselect
    
    result.axis_x = (float) ax;
    result.axis_y = (float) ay;
    result.axis_z = (float) az;

    return result;
}

GyroData gyro_read(void){

    GyroData result;
    uint16_t lo, hi;
    int16_t gx, gy, gz;

    LATBbits.LATB4 = 0;          // Select gyroscope
    spi_write(0x02 | 0x80);      // Rate data start, auto-increment, read flag set

    lo = spi_write(0x00); hi = spi_write(0x00); gx = (int16_t)((hi << 8) | lo);
    lo = spi_write(0x00); hi = spi_write(0x00); gy = (int16_t)((hi << 8) | lo);
    lo = spi_write(0x00); hi = spi_write(0x00); gz = (int16_t)((hi << 8) | lo);

    LATBbits.LATB4 = 1;          // Deselect

    // +-2000 deg/s default range -> 16.4 LSB per deg/s -> 0.061 deg/s per LSB
    result.x = gx * 0.061f;
    result.y = gy * 0.061f;
    result.z = gz * 0.061f;

    return result;
}