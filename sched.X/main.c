#include "xc.h"
#include "scheduler.h"
#include "timer.h"
#include "adc.h"
#include "control.h"
#include "task.h"

#define MAX_TASKS 4     //TODO change to actual number of tasks

/*
 * TODO probs need new file just for this so everyone can include it???
 * typedef struct {
    // ADC raw values (written by acquisition, read by processing)
    float raw_ir;
    float raw_bat;
    // processed results (written by processing)
    float ir_distance;
    float battery_v;
    // existing buggy fields
    bool  button_prev;
    bool  pwm_on;
    int   speed;
    int   yaw_rate;
} control_data;*/

heartbeat schedInfo[MAX_TASKS];
control_data controlData = {0};

void setup_scheduler(){
    // button task - always on
    schedInfo[0].n      = 0;
    schedInfo[0].N      = 10;   //TODO what frequency??
    schedInfo[0].f      = task_button;
    schedInfo[0].params = (void*)(&controlData);
    schedInfo[0].enable = 1;    // starts enabled

    // motor task - only when button pressed TODO obstacel???
    schedInfo[1].n      = 0;
    schedInfo[1].N      = 1;
    schedInfo[1].f      = task_motor;
    schedInfo[1].params = (void*)(&controlData);
    schedInfo[1].enable = 0;    // starts disabled
    
    // LED task - start disabled TODO later
    schedInfo[0].n      = 0;
    schedInfo[0].N      = 250;   
    schedInfo[0].f      = task_LED;
    schedInfo[0].params = (void*)(&controlData);
    schedInfo[0].enable = 0;    // starts enabled
    
    /*// task 3 - ADC acquire, every 100ms
    schedInfo[2].n      = 0;
    schedInfo[2].N      = 100;
    schedInfo[2].f      = task_adc_acquire;
    schedInfo[2].params = (void*)(&controlData);
    schedInfo[2].enable = 1;

    // task 4 - ADC process + UART send, every 100ms
    schedInfo[3].n      = 0;
    schedInfo[3].N      = 100;
    schedInfo[3].f      = task_adc_process;
    schedInfo[3].params = (void*)(&controlData);
    schedInfo[3].enable = 1;*/
}

int main(void) {
    
    //TODO where to update enable ad disable??
    tmr_setup_period(TIMER1,2); //for 2ms/500hz heartbeat
    setup_scheduler();
    
    while(1) {
        scheduler(schedInfo, MAX_TASKS);
        tmr_wait_period(TIMER1);
    }
}
