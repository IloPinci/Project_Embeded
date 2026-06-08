#include "xc.h"
#include "timer.h"
#include "uart.h"  
#include "spi.h"
#include "adc.h"
#include "pwm.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static int parser(const char *msg, int *speed, int *yawRate){

    if (strncmp(msg, "$PCREF,", 7) != 0) return 0;

    const char *ptr = msg + 7;
    char *end;

    int parsed_speed = (int)strtol(ptr, &end, 10);
    if (*end != ',') return 0;

    int parsed_yaw = (int)strtol(end + 1, &end, 10);
    if (*end != '*') return 0;

    *speed   = parsed_speed;
    *yawRate = parsed_yaw;
    return 1;
}

void led_blink(void* param){

    shared_data *sd = (shared_data *) param; 

    LATAbits.LATA0 =  !LATAbits.LATA0;          // Always blink the DSP led 

   switch (sd->current_car_state){
        case HALT:
            LATBbits.LATB8 = !LATBbits.LATB8;   // Left side blink 
            LATFbits.LATF1 = LATBbits.LATB8     // Right side blink
            LATGbits.LATG1 = 0;                 // Low off
            break;

        case MOVE:
            LATBbits.LATB8 = 0;                 // Left  off     
            LATFbits.LATF1 = 0;                 // Right off     
            LATGbits.LATG1 = 1;                 // Low on      
            break;

        case AVOID:
            LATBbits.LATB8 = 0;                 // Left side blink 
            LATFbits.LATF1 = !LATFbits.LATF1;   // Right side blink
            LATGbits.LATG1 = 1;                 // Low on  
            break;

        default:
            LATBbits.LATB8 = 0;
            LATFbits.LATF1 = 0;
            LATGbits.LATG1 = 0;
            break;
   }
}

void uart_sending(void* param){

    shared_data *sd = (shared_data *) param;
    char buffer[32];

    // IR
    sprintf(buffer, "$MDIST,%d*\n", (int)(sd->ir_distance + 0.5f));
    uart_transmit(buffer);

    // Magnetometer
    sprintf(buffer, "$MANGLE,%.2f,%.2f,%.2f*\n",
         sd->accel_data.roll, 
         sd->accel_data.pitch, 
         sd->yaw);
    uart_transmit(buffer);

    // Every 1 hz we transmit what we have read. We enter uart_sending every 50 loops.
    // We want to send the the voltage every 500 loops. So we have to send it if we enter in the uart_sending 10 times
    if (++sd->led_toggle % 10 == 0){
        sprintf(buffer, "$MBATT,%.2f*\n", sd->battery_voltage);
        uart_transmit(buffer);
        sd->led_toggle = 0;
    }
}

void ir_read(void* param){
    
    shared_data *sd = (shared_data *) param;
    
    float raw_ir_data = adc_read(IR);       // Read IR from channel 14
    
    // Compute voltage
    float voltage = 3.3 * raw_ir_data / 1024.0;     // For 10 bit adc and 3.3 voltage range
    
    // Convert into distance (cm)
    float converted_distance = 100 * (2.34f
                         - 4.74f * voltage
                         + 4.06f * voltage * voltage
                         - 1.60f * voltage * voltage * voltage
                         + 0.24f * voltage * voltage * voltage * voltage);

    sd->ir_distance = converted_distance;

    // Handle all the car states according to the distance threshold
    if (converted_distance <= ir_threshold){
        if(sd->current_car_state == MOVE){
            sd->current_car_state = AVOID;
        } else if(sd->current_car_state == AVOID && sd->obs_avoid_var.state == MOVE_FORWARD){
            sd->current_car_state = HALT;
        }
    }     
}

void battery_read(void* param){

    shared_data *sd = (shared_data *) param;
    
    float raw_bat_data = adc_read(BATTERY);       // Read battery from channel 11

    // Compute voltage
    float voltage = 3.3 * raw_bat_data / 1024.0;  // For 10 bit adc and 3.3 voltage range

    // Convert voltage divider value to actual battery value
    float result = 3 * voltage;

    sd->battery_voltage = result; 
}

void pwm_control(void* param){

    shared_data *sd = (shared_data *) param;
    switch (sd->current_car_state){
        case HALT:
            pwm_stop_all();     // Stop all the motors

            // Reset obstacle avoidance variables 
            sd->obs_avoid_var.state = INIT;
            sd->obs_avoid_var.rep = 0;
            sd->obs_avoid_var.two_sec_counter = 0;        
            break;

        case MOVE:
            // Move the buggy according to the speed and yaw_rate values received by UART
            pwm_move(sd->pwm.speed,sd->pwm.yawRate);
            break;

        case AVOID:
            // INIT: start clockwise rotation
            if (sd->obs_avoid_var.state == INIT) {
                sd->obs_avoid_var.swept = 0.0f;             // Reset integrated angle
                sd->obs_avoid_var.state = ROT_CLOCKWISE;
                pwm_move(0, -70);                           // Rotate clockwise
            }

            // ROT_CLOCKWISE: integrate gyro z until 90 deg swept
            if (sd->obs_avoid_var.state == ROT_CLOCKWISE) {
                GyroData g = gyro_read();
                sd->obs_avoid_var.swept += g.z * CTRL_DT;   // deg/s * s = deg
                if (fabs(sd->obs_avoid_var.swept) >= 90.0f) {
                    sd->obs_avoid_var.state = MOVE_FORWARD;
                    sd->obs_avoid_var.two_sec_counter = 0;
                    pwm_move(50, 0);                        // Forward at low speed
                }
            }

            // MOVE_FORWARD: 2 s at 500 Hz = 1000 ticks
            if (sd->obs_avoid_var.state == MOVE_FORWARD) {
                if (++sd->obs_avoid_var.two_sec_counter >= 1000) {
                    sd->obs_avoid_var.state = ROT_COUNTERCLOCKWISE;
                    sd->obs_avoid_var.swept = 0.0f;       // Reset for the return turn
                    pwm_move(0, 70);                      // Rotate anticlockwise
                }
            }

            // ROT_COUNTERCLOCKWISE: integrate gyro z until 90 deg back
            if (sd->obs_avoid_var.state == ROT_COUNTERCLOCKWISE) {
                GyroData g = gyro_read();
                sd->obs_avoid_var.swept += g.z * CTRL_DT;
                if (fabs(sd->obs_avoid_var.swept) >= 90.0f) {
                    pwm_stop_all();
                    sd->obs_avoid_var.state = INIT;

                    // We restart the obstacle avoidance execution for a maximum of three times in a row
                    if (sd->ir_distance <= ir_threshold) {
                        sd->obs_avoid_var.rep++;
                        // After the third time the car moves to HALT state
                        if (sd->obs_avoid_var.rep >= 3) {
                            sd->obs_avoid_var.rep = 0;
                            sd->current_car_state = HALT;
                        }
                    } else {
                        // If no obstacle is detected, move back to MOVE state
                        sd->obs_avoid_var.rep = 0;
                        sd->current_car_state = MOVE;
                    }
                }
            }
            break;

        default:
            break;
    }
}

void parse_uart(void* param){

    shared_data *sd = (shared_data *) param;
    char buffer[32];

    if (!uart_receive_line(buffer, sizeof(buffer))) return;

    int spd = 0, yaw = 0;
    if (parser(buffer, &spd, &yaw)) {
        if (spd >= -100 && spd <= 100 && yaw >= -100 && yaw <= 100){
            sd->pwm.speed   = spd;
            sd->pwm.yawRate = yaw;
        }
    }
}

void button_handler(void* param){

    shared_data *sd = (shared_data *) param;

    // Handle the buffer sizes
    if (sd->button_2_original == 1){
        char buffer[32];

        sprintf(buffer, "$MBUF,%d,%d\n*", sd->transmit_size, sd->receive_size);
        uart_transmit(buffer);

        sd->button_2_original = 0;
        sd->button2_confirmed = 1;
    }

    // Handle the state transitions
    if (sd->button_1_original == 1){
        
        if(sd->current_car_state == HALT){
            sd->current_car_state = MOVE;
        }
        else{
            sd->current_car_state = HALT;
        }

        sd->button_1_original = 0;
        sd->button1_confirmed = 1;
    }

    // After 300ms the button can be pressed again in order to avoid bounces
    if (sd->button1_confirmed > 0) {
        if(++sd->button1_confirmed >=3 ){
            sd->button1_confirmed = 0;
            IFS1bits.INT1IF = 0;
            IEC1bits.INT1IE = 1;
        }
    }

    if (sd->button2_confirmed > 0) {
        if(++sd->button2_confirmed >=3){
            sd->button2_confirmed = 0;
            IFS1bits.INT2IF = 0;
            IEC1bits.INT2IE = 1;
        }
    }
}

void accel_mag_read(void* param){

    shared_data *sd = (shared_data *) param;

    sd->accel_data = accel_read();
    sd->mag_data = mag_read();

    // Convert to radians for better calculation
    float roll_rad  = sd->accel_data.roll  * (PI / 180.0f);
    float pitch_rad = sd->accel_data.pitch * (PI / 180.0f);

    // Tilt compensation in case that the car is in a slope
    float x = sd->mag_data.axis_x * cosf(pitch_rad) 
            + sd->mag_data.axis_y * sinf(roll_rad) * sinf(pitch_rad) 
            + sd->mag_data.axis_z * cosf(roll_rad) * sinf(pitch_rad);

    float y = sd->mag_data.axis_y * cosf(roll_rad) 
            - sd->mag_data.axis_z * sinf(roll_rad);

    // Compute yaw value after tilt compensation
    sd->yaw = atan2f(-y, x) * (180.0f / PI);
}
