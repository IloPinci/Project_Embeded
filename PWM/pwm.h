#ifndef PWM_H
#define PWM_H

#include <xc.h>

#define FCY 72000000UL
#define PWM_FREQ 10000UL
#define PERIOD_TICKS ((FCY / PWM_FREQ) - 1U)

// Define motor number
#define LEFT_FORWARD 1
#define LEFT_BACKWARD 2
#define RIGHT_FORWARD 3
#define RIGHT_BACKWARD 4

// Setup single output compare module (from OC1 to OC4)
void pwm_setup(int oc_select);

// Setup all output compare modules from 1 to 4
void pwm_setup_all(void);

// Stop the selected motor 
void pwm_stop(int oc_select);

// Stop all the motors
void pwm_stop_all(void);

// Control motors through speed and yaw_rate
void pwm_control(int speed, int yaw_rate);

// Helping function to convert duty cycles to valid registers values
unsigned int pwm_to_ticks(int pwm_percent);

#endif