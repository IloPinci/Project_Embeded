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
#include "system.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {

    //! all the data that will need to be shared is declared as local variables in the stack
    pwm_variables pwm = {40, 0};    // we want to to move forward even if there isn't a command from the uart, so we can ust press the move button and it will go
    AccelData accel = {0};
    float distance = 0;
    float battery  = 0;
    car_state fsm = {HALT, { INIT, 0, 0, 0.0f }};

    //! structures that are made up from other data structs so we can pass to the tasks
    distance_sensing ir_handle = { &distance, &fsm };
    pwm_ctrl pwm_handle = { &pwm, &distance, &fsm };
    uart_send send_handle = { &distance, &accel, &battery };

    TaskData schedInfo[Max_Tasks] = {0};    // we initialize to 0 the array that will be used by the scheduler (so the enable the period ect)

    port_setup();
    library_setup();
    task_setup(schedInfo, &ir_handle, &pwm_handle, &send_handle, &pwm, &accel, &battery, &fsm);     // we pass to each task the pointer to the data structure that it needs
    
    while(1){
        scheduler_run(schedInfo);
        if(tmr_wait_period(TIMER1)){
            //uart_transmit("$MISS*");
        }
    }

    return 0;
}