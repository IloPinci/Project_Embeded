#include <xc.h>
#include "tasks.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "uart.h"
#include "pwm.h"
#include "adc.h"
#include "spi.h"
#include "parser.h"

//TODO delete if button interrupt works
/*static volatile int button1_flag = 0;
static volatile int button2_flag = 0;*/

// Helper functions and state machine logic

//! Functions
// Obstacle avoidance state machine handler
static void obstacle_avoidance_step(car_state *fsm, float *distance){

    switch (fsm->avoid.state){

        // INIT: start clockwise rotation
        case INIT:
            fsm->avoid.swept = 0.0f;                // Reset integrated angle
            fsm->avoid.state = ROT_CLOCKWISE;
            pwm_move(0, -70);                       // Rotate clockwise
            break;

        // ROT_CLOCKWISE: integrate gyro z until 90 deg swept
        case ROT_CLOCKWISE: {
            GyroData g = gyro_read();
            fsm->avoid.swept += g.axis_z * CTRL_DT; // deg/s * s = deg
            if (fabsf(fsm->avoid.swept) >= 90.0f) {
                fsm->avoid.state = MOVE_FORWARD;
                fsm->avoid.two_sec_counter = 0;
                pwm_move(50, 0);                    // Forward at low speed
            }
            break;
        }

        // MOVE_FORWARD: 2 s at 500 Hz = 1000 ticks
        case MOVE_FORWARD:
            if (++fsm->avoid.two_sec_counter >= 1000) {
                fsm->avoid.state = ROT_COUNTERCLOCKWISE;
                fsm->avoid.swept = 0.0f;            // Reset for the return turn
                pwm_move(0, 70);                    // Rotate anticlockwise
            }
            break;

        // ROT_COUNTERCLOCKWISE: integrate gyro z until 90 deg back
        case ROT_COUNTERCLOCKWISE: {
            GyroData g = gyro_read();
            fsm->avoid.swept += g.axis_z * CTRL_DT;
            if (fabsf(fsm->avoid.swept) >= 90.0f) {
                pwm_stop_all();
                fsm->avoid.state = INIT;

                // We restart the obstacle avoidance execution for a maximum of three times in a row 
                if (*distance <= (IR_THRESHOLD + 5)) {
                    //? we do a +10 cm here to compesate for the fact that when turning the robot does not turn fully 90 degrees. Thus it moves diagonally and it gets away from the target. This means that it will move forward just a bit and will get stuck in a infinite loop.

                    fsm->avoid.rep++;
                    // After the third time the car moves to HALT state
                    if (fsm->avoid.rep >= 3) {
                        fsm->avoid.rep = 0;
                        fsm->state = HALT;
                    }
                } else {
                    // If no obstacle is detected, move back to MOVE state
                    fsm->avoid.rep = 0;
                    fsm->state = MOVE;
                }
            }
            break;
        }
    }
}

//TODO remove if it works
/*/ISRs
//! Interrupts
void __attribute__((interrupt, no_auto_psv)) _INT1Interrupt(void) {

    button1_flag = 1;
    
    // Clear the flag and disable the interrupt. The disabing is done to combat bounces. 
    // The interrupt enable is activated after 200ms which should be sufficient time to allow for it
    IFS1bits.INT1IF = 0; 
    IEC1bits.INT1IE = 0;
}

void __attribute__((interrupt, no_auto_psv)) _INT2Interrupt(void) {

    button2_flag = 1;
    
    // The buffer sizes are now read by button_handler through uart_rx_count() /
    // uart_tx_count(), so the ISR no longer reaches into uart.c's buffers.

    // Clear the flag and disable the interrupt. The disabing is done to combat bounces. 
    // The interrupt enable is activated after 200ms which should be sufficient time to allow for it
    IFS1bits.INT2IF = 0; 
    IEC1bits.INT2IE = 0;
}*/

//! Tasks

// Handles all LEDs blink
void led_blink(void* param){

    car_state *fsm = (car_state *) param; 

    LATAbits.LATA0 =  !LATAbits.LATA0;          // Always blink the DSP led 

   switch (fsm->state){
        case HALT:
            LATBbits.LATB8 = !LATBbits.LATB8;   // Left side blink 
            LATFbits.LATF1 = LATBbits.LATB8;    // Right side blink
            LATGbits.LATG1 = 0;                 // Low off
            break;

        case MOVE:
            LATBbits.LATB8 = 0;                 // Left off     
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

// Send required values to UART
void uart_sending(void* param){

    uart_send *ctx = (uart_send *) param;
    static int batt_div = 0;    // counts entries to divide 10 Hz down to 1 Hz for battery
    char buffer[48];            // wide enough for the worst-case $MANGLE

    // IR
    sprintf(buffer, "$MDIST,%d*\n", (int)(*ctx->distance + 0.5f));
    uart_transmit(buffer);

    // Magnetometer
    sprintf(buffer, "$MANGLE,%.2f,%.2f,%.2f*\n",
         ctx->accel->roll, 
         ctx->accel->pitch, 
         ctx->accel->yaw);
    uart_transmit(buffer);

    // Every 1 hz we transmit what we have read. We enter uart_sending every 50 loops.
    // We want to send the the voltage every 500 loops. So we have to send it if we enter in the uart_sending 10 times
    if (++batt_div % 10 == 0){
        sprintf(buffer, "$MBATT,%.2f*\n", *ctx->battery);
        uart_transmit(buffer);
        batt_div = 0;
    }
}

// Read IR value
void ir_read(void* param){
    
    distance_sensing *ctx = (distance_sensing *) param;
    
    float raw_ir_data = adc_read(IR);       // Read IR from channel 14
    
    // Compute voltage
    float voltage = 3.3f * raw_ir_data / 1024.0f;     // For 10 bit adc and 3.3 voltage range
    
    // Convert into distance (cm)
    float converted_distance = 100.0f * (2.34f
                         - 4.74f * voltage
                         + 4.06f * voltage * voltage
                         - 1.60f * voltage * voltage * voltage
                         + 0.24f * voltage * voltage * voltage * voltage);

    *ctx->distance = converted_distance;

    // Handle all the car states according to the distance threshold
    if (converted_distance <= IR_THRESHOLD){
        if(ctx->fsm->state == MOVE){
            ctx->fsm->state = AVOID;
        } else if(ctx->fsm->state == AVOID && ctx->fsm->avoid.state == MOVE_FORWARD){
            ctx->fsm->state = HALT;
        }
    }     
}

// Read battery level
void battery_read(void* param){

    float *battery = (float *) param;
    
    float raw_bat_data = adc_read(BATTERY);       // Read battery from channel 11

    // Compute voltage
    float voltage = 3.3f * raw_bat_data / 1024.0f;  // For 10 bit adc and 3.3 voltage range

    // Convert voltage divider value to actual battery value
    float result = 3 * voltage;

    *battery = result; 
}

// PWM control: handles the car movement for all the states
void pwm_control(void* param){

    pwm_ctrl *ctx = (pwm_ctrl *) param;
    switch (ctx->fsm->state){
        case HALT:
            pwm_stop_all();     // Stop all the motors

            // Reset obstacle avoidance variables 
            ctx->fsm->avoid.state = INIT;
            ctx->fsm->avoid.rep = 0;
            ctx->fsm->avoid.two_sec_counter = 0;        
            break;

        case MOVE:
            // Move the buggy according to the speed and yaw_rate values received by UART
            pwm_move(ctx->pwm->speed, ctx->pwm->yawRate);
            break;

        case AVOID:
            // The avoidance manoeuvre lives in its own helper (see above)
            obstacle_avoidance_step(ctx->fsm, ctx->distance);
            break;

        default:
            break;
    }
}

// Parser
void parse_uart(void* param){

    pwm_variables *pwm = (pwm_variables *) param;
    static parser_state pstate = { STATE_DOLLAR, {0}, {0}, 0, 0 };
    char c;

     while (uart_receive_char(&c)) {

        if (parse_byte(&pstate, c) == NEW_MESSAGE) {

            if (strncmp(pstate.msg_type, "PCREF", 5) == 0) {

                int spd = extract_integer(pstate.msg_payload);
                int i   = next_value(pstate.msg_payload, 0);
                int yaw = extract_integer(pstate.msg_payload + i);

                if (spd >= -100 && spd <= 100 && yaw >= -100 && yaw <= 100) {
                    pwm->speed   = spd;
                    pwm->yawRate = yaw;
                }
            }
        }
    }
}

//TODO delete if it works
/*// Handles the buttons
void button_handler(void* param){

    car_state *fsm = (car_state *) param;
    static int button1_confirmed = 0;   // debounce counters: only this task uses them
    static int button2_confirmed = 0;

    // Handle the buffer sizes
    if (button2_flag == 1){
        char buffer[32];

        sprintf(buffer, "$MBUF,%d,%d*\n", uart_tx_count(), uart_rx_count());
        uart_transmit(buffer);

        button2_flag = 0;
        button2_confirmed = 1;
    }

    // Handle the state transitions
    if (button1_flag == 1){
        
        if(fsm->state == HALT){
            fsm->state = MOVE;
        }
        else{
            fsm->state = HALT;
        }

        button1_flag = 0;
        button1_confirmed = 1;
    }

    // After 300ms the button can be pressed again in order to avoid bounces
    if (button1_confirmed > 0) {
        if(++button1_confirmed >=3 ){
            button1_confirmed = 0;
            IFS1bits.INT1IF = 0;
            IEC1bits.INT1IE = 1;
        }
    }

    if (button2_confirmed > 0) {
        if(++button2_confirmed >=3){
            button2_confirmed = 0;
            IFS1bits.INT2IF = 0;
            IEC1bits.INT2IE = 1;
        }
    }
}*/

// Handles the buttons
void button_handler(void* param){

    car_state *fsm = (car_state *) param;

    static int re8_prev = 1;
    static int re9_prev = 1;

    // Current value of the buttons
    int re8_now = PORTEbits.RE8;
    int re9_now = PORTEbits.RE9;

    // Chenge car state only when button is pressed
    if (re8_prev == 1 && re8_now == 0) {
        fsm->state = (fsm->state == HALT) ? MOVE : HALT;
    }
    re8_prev = re8_now;

    // Send to UART only when button is pressed 
    if (re9_prev == 1 && re9_now == 0) {
        char buffer[32];
        sprintf(buffer, "$MBUF,%d,%d*\n", uart_tx_count(), uart_rx_count());
        uart_transmit(buffer);
    }
    re9_prev = re9_now;
}

// Read accelerometer and magnetometer values
void accel_mag_read(void* param){

    AccelData *accel = (AccelData *) param;

    *accel = accel_read();
    MagData mag = mag_read();   // only used inside this task -> plain local

    // Convert to radians for better calculation
    float roll_rad  = accel->roll  * (PI / 180.0f);
    float pitch_rad = accel->pitch * (PI / 180.0f);

    // Compute yaw value with tilt compensation
    accel->yaw = yaw_compute(roll_rad, pitch_rad, mag.axis_x, mag.axis_y, mag.axis_z);
}