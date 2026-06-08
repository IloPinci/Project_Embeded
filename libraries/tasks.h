#include "xc.h"
#include "timer.h"
#include "uart.h"  
#include "spi.h"
#include "adc.h"
#include "pwm.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ir_threshold 30  // IR distance threshold

// Constants for obstacle avoidance state machine
#define CTRL_DT 0.002f   // 500 Hz control loop -> 2 ms per tick
#define INIT 0
#define ROT_CLOCKWISE 1
#define MOVE_FORWARD 2
#define ROT_COUNTERCLOCKWISE 3

// Helper function for UART parsing
static int parser(const char *msg, int *speed, int *yawRate);

// Handles all LEDs blink
void led_blink(void* param);

// Send required values to UART
void uart_sending(void* param);

// Read IR value
void ir_read(void* param);

// Read battery value
void battery_read(void* param);

// PWM control: handles the car movement for all the states
void pwm_control(void* param);

// Parser
void parse_uart(void* param);

// Handles the buttons
void button_handler(void* param);

// Read accelerometer and magnetometer values
void accel_mag_read(void* param);