#ifndef TASKS_H
#define	TASKS_H

#include <xc.h> 
#include "data_types.h"
#include "uart.h"
#include "pwm.h"
#include "adc.h"
#include "spi.h"
#include "scheduler.h"

//TODO ISRs?

// Handles all LEDs blink
void led_blink(void* param);

// Send required values to UART
void uart_sending(void* param);

// Read IR value
void ir_read(void* param);

// Read battery level
void battery_read(void* param);

// PWM control: handles the car movement for all the states
void pwm_control(void* param);

// Parser
void parse_uart(void* param);

// Handles the buttons
void button_handler(void* param);

// Read accelerometer and magnetometer values
void accel_mag_read(void* param);

#endif

