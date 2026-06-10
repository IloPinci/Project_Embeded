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
#include "timer.h"
#include "spi.h"
#include "uart.h"
#include "adc.h"
#include "pwm.h"
#include "scheduler.h"
#include "data_types.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define ir_threshold 30
#define CTRL_DT 0.002f   // 500 Hz control loop -> 2 ms per tick

static volatile int button1_flag = 0;
static volatile int button2_flag = 0;

//! Setups
void port_setup(){
    
    // Disable analog inputs 
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

    // Lights
    TRISAbits.TRISA0 = 0;   // LED1 output
    TRISBbits.TRISB8 = 0;   // Left side lights
    TRISFbits.TRISF1 = 0;   // Right side lights
    TRISFbits.TRISF0 = 0;   // Brakes
    TRISGbits.TRISG1 = 0;   // Low intensity light
    TRISAbits.TRISA7 = 0;   // High intensity light

    LATAbits.LATA0 = 1;     // LED1 on in the initial state
    LATBbits.LATB8 = 0;     
    LATFbits.LATF1 = 0;     
    LATFbits.LATF0 = 0;     
    LATGbits.LATG1 = 0;     
    LATAbits.LATA7 = 0;     


    // Buttons
    TRISEbits.TRISE8 = 1;   // Button 1 input
    TRISEbits.TRISE9 = 1;   // Button 2 input

    // ISRs for the buttons
    INTCON2bits.INT1EP = 1; 
    INTCON2bits.INT2EP = 1;

    // Route RPI88 and RPI89 to the interrupts
    RPINR0bits.INT1R = 0x58;
    RPINR1bits.INT2R = 0x59;

    // Clear the flags
    IFS1bits.INT1IF = 0;
    IFS1bits.INT2IF = 0;
    
    // Enable the interrupts
    IEC1bits.INT1IE = 1;
    IEC1bits.INT2IE = 1;
}

void library_setup(){
    tmr_setup_period(TIMER1, 2);    // 500Hz -> 2ms
    uart_setup();
    spi_setup();        
    mag_setup(); 
    pwm_setup_all();   
    adc_setup();
}

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
}

//! Functions
// Parser
static int parser(const char *msg, int *speed, int *yawRate) {

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
                if (*distance <= ir_threshold) {
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
    if (converted_distance <= ir_threshold){
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
    char buffer[32];

    if (!uart_receive_line(buffer, sizeof(buffer))) return;

    int spd = 0, yaw = 0;
    if (parser(buffer, &spd, &yaw)) {
        if (spd >= -100 && spd <= 100 && yaw >= -100 && yaw <= 100){
            pwm->speed = spd;
            pwm->yawRate = yaw;
        }
    }
}

// Handles the buttons
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

//! TASK SETUP
void task_setup(TaskData schedInfo[], distance_sensing *ir_handle, pwm_ctrl *pwm_handle, uart_send *send_handle, 
                pwm_variables *pwm, AccelData *accel, float *battery, car_state *fsm){
    /*
        period = 1   (2ms       = 500Hz)        
        period = 50  (100ms     = 10Hz)     
        period = 500 (1000ms    = 1Hz)
    */

    //? IR - read
    schedInfo[0].counter = 0;
    schedInfo[0].period = 1;
    schedInfo[0].enable = 1;
    schedInfo[0].task_function = ir_read;
    schedInfo[0].params = (void*)ir_handle;

    //? PWM control
    schedInfo[1].counter = 0;
    schedInfo[1].period = 1;
    schedInfo[1].enable = 1;
    schedInfo[1].task_function = pwm_control;
    schedInfo[1].params = (void*)pwm_handle;

    // Offset the tasks that have the same period so they don't run in the same tick. 
    // Also the sensors have a lower offset than the consumers so the data is updated.

    //? We parse the receiving messages 
    schedInfo[2].counter = 0;
    schedInfo[2].period = 50;
    schedInfo[2].enable = 1;
    schedInfo[2].task_function = parse_uart;
    schedInfo[2].params = (void*)pwm;

    //? Accelerometer & Magnetometer
    schedInfo[3].counter = 10;
    schedInfo[3].period = 50;
    schedInfo[3].enable = 1;
    schedInfo[3].task_function = accel_mag_read;
    schedInfo[3].params = (void*)accel;

    //? Button handler
    schedInfo[4].counter = 20;
    schedInfo[4].period = 50;
    schedInfo[4].enable = 1;
    schedInfo[4].task_function = button_handler;
    schedInfo[4].params = (void*)fsm;

    //? Uart Transmit (all the messages at once)
    schedInfo[5].counter = 30;
    schedInfo[5].period = 50;
    schedInfo[5].enable = 1;
    schedInfo[5].task_function = uart_sending;
    schedInfo[5].params = (void*)send_handle;

    // //? Led blinking
    schedInfo[6].counter = 0;
    schedInfo[6].period = 250;
    schedInfo[6].enable = 1;
    schedInfo[6].task_function = led_blink;
    schedInfo[6].params = (void*)fsm;

    //? Battery read
    schedInfo[7].counter = 0;
    schedInfo[7].period = 500;
    schedInfo[7].enable = 1;
    schedInfo[7].task_function = battery_read;
    schedInfo[7].params = (void*)battery;
}


int main(void) {

    // Data setups, updated through pointers
    pwm_variables pwm = {0};
    AccelData accel = {0};
    float distance = 0;
    float battery  = 0;
    car_state fsm = {HALT, { INIT, 0, 0, 0.0f }};

    // Per-task views: pointer bundles naming exactly what each task may touch
    distance_sensing ir_handle = { &distance, &fsm };
    pwm_ctrl pwm_handle = { &pwm, &distance, &fsm };
    uart_send send_handle = { &distance, &accel, &battery };

    TaskData schedInfo[Max_Tasks] = {0};    // Zero-init

    port_setup();
    library_setup();
    task_setup(schedInfo, &ir_handle, &pwm_handle, &send_handle, &pwm, &accel, &battery, &fsm);
    
    while(1){
        scheduler_run(schedInfo);
        if(tmr_wait_period(TIMER1)){
            //uart_transmit("$MISS*");
        }
    }

    return 0;
}