#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "data_types.h"
#include <xc.h>

#define Max_Tasks 8

// What a task should have
typedef struct{
    int counter;        // incremented every tick and reset to 0 when it reaches the vaue of the period
    int period;         // period = heartbeat * period_multiplier
    int enable;         // 0 skip 1 run
    void (*task_function) (void *);     // the pointer of the function that the task calls 
    void * params;      // the pointer of the data structures that the task will need to execute properly
}TaskData; 


void task_setup(TaskData schedInfo[], distance_sensing *ir_handle, pwm_ctrl *pwm_handle, uart_send *send_handle, 
                pwm_variables *pwm, AccelData *accel, float *battery, car_state *fsm);
// Run the scheduler
void scheduler_run(TaskData tasks[]);

#endif