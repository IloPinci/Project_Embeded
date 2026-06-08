#include "xc.h"
#include "timer.h"
#include "uart.h"
#include "spi.h"
#include "adc.h"
#include "pwm.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define Max_Tasks 9

// What a task should have
typedef struct{
    int counter;
    int period;      // period = heartbeat * period_multiplier
    int enable;
    void (*task_function) (void *);
    void * params;
}TaskData; 

// Scheduler execution
void scheduler_run(TaskData tasks[]);

// Tasks setup
void task_setup();