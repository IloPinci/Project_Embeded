#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "data_types.h"
#include <xc.h>

#define Max_Tasks 8

// What a task should have
typedef struct{
    int counter;
    int period;      // period = heartbeat * period_multiplier
    int enable;
    void (*task_function) (void *);
    void * params;
}TaskData; 


void task_setup(TaskData schedInfo[], distance_sensing *ir_handle, pwm_ctrl *pwm_handle, uart_send *send_handle, 
                pwm_variables *pwm, AccelData *accel, float *battery, car_state *fsm);
// Run the scheduler
void scheduler_run(TaskData tasks[]);

#endif