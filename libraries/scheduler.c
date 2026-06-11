#include "scheduler.h"
#include "tasks.h"

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