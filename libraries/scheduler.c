#include "xc.h"
#include "timer.h"
#include "uart.h"  
#include "spi.h"
#include "adc.h"
#include "pwm.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Scheduler execution
void scheduler_run(TaskData tasks[]){

    for (int i = 0; i < Max_Tasks; i++){
        // Should the task be executed
        if (tasks[i].enable == 0){
            tasks[i].counter = 0;   // If not then clear its counter
            continue;
        } 

        if (++tasks[i].counter == tasks[i].period){
            tasks[i].task_function(tasks[i].params);    // Execute the task
            tasks[i].counter = 0;
        }
    }
}

// Tasks setup
void task_setup(){
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
    schedInfo[0].params = (void*)&global_system_state;

    //? PWM control
    schedInfo[1].counter = 0;
    schedInfo[1].period = 1;
    schedInfo[1].enable = 1;
    schedInfo[1].task_function = pwm_control;
    schedInfo[1].params = (void*)&global_system_state;

    // We offset the tasks that have the same period so they don't run in the same tick. 
    // Also the sensors have a lower offset than the consumers so the data is updated.

    //? We parse the receiving messages 
    schedInfo[2].counter = 0;
    schedInfo[2].period = 50;
    schedInfo[2].enable = 1;
    schedInfo[2].task_function = parse_uart;
    schedInfo[2].params = (void*)&global_system_state;

    //? Accelerometer & Magnetometer
    schedInfo[3].counter = 10;
    schedInfo[3].period = 50;
    schedInfo[3].enable = 1;
    schedInfo[3].task_function = accel_mag_read;
    schedInfo[3].params = (void*)&global_system_state;

    //? Button handler
    schedInfo[4].counter = 20;
    schedInfo[4].period = 50;
    schedInfo[4].enable = 1;
    schedInfo[4].task_function = button_handler;
    schedInfo[4].params = (void*)&global_system_state;

    //? Uart Transmit (all the messages at once)
    schedInfo[5].counter = 30;
    schedInfo[5].period = 50;
    schedInfo[5].enable = 1;
    schedInfo[5].task_function = uart_sending;
    schedInfo[5].params = (void*)&global_system_state;

    // //? Led blinking
    schedInfo[6].counter = 0;
    schedInfo[6].period = 250;
    schedInfo[6].enable = 1;
    schedInfo[6].task_function = led_blink;
    schedInfo[6].params = (void*)&global_system_state;

    //? Button debounce
    schedInfo[7].counter = 0;
    schedInfo[7].period = 500;
    schedInfo[7].enable = 1;
    schedInfo[7].task_function = battery_read;
    schedInfo[7].params = (void*)&global_system_state;
}