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