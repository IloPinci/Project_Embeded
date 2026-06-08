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
    float roll;     // Computed roll angle
    float pitch;    // Computed pitch angle
    float yaw;      // Computed yaw angle
} AccelData;

typedef struct {
    float axis_x;
    float axis_y;
    float axis_z;
} MagData;

typedef struct {
    float x;
    float y;
    float z;
} GyroData;
     
// Sets up SPI configuration and configures pins
void spi_setup(void);

// Sends byte over SPI and returns receives byte
unsigned int spi_write(unsigned int data);

// Setup the magnetometer
void mag_setup(void);

// Reads accelerometer data and returns Sensor Datastruct
AccelData accel_read(void);

// Reads magnetometer data and returns Sensor Datastruct
MagData mag_read(void);

// Reads magnetometer data and returns Sensor Datastruct
GyroData gyro_read(void);



#endif