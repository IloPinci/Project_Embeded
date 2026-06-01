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

#ifndef SPI_H
#define	SPI_H
    
#include <stdlib.h>
    
#define ACC 1
#define GYR 2
#define MAG 3
    
typedef struct {
    float axis_x;
    float axis_y;
    float axis_z;
    float roll;  // Computed roll angle in degrees
    float pitch; // Computed pitch angle in degrees
    float yaw
} AccelData;

typedef struct {
    float axis_x;
    float axis_y;
    float axis_z;
} MagData;
     
// Sets up SPI configuration and configures pins
void spi_setup();

// Sends byte over SPI and returns receives byte
unsigned int spi_write(unsigned int data);

// sets accellerometer bandwidth
void accel_bw(int bw);

void mag_setup(void);

// Reads accelerometer data and returns Sensor Datastruct
AccelData accel_read();

// Reads magnetometer data and returns Sensor Datastruct
MagData mag_read();



#endif