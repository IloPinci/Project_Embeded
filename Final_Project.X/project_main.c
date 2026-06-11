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
#include "tasks.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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