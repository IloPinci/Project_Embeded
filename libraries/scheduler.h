#ifndef SCHEDULER_H
#define SCHEDULER_H

#define Max_Tasks 8

// What a task should have
typedef struct{
    int counter;
    int period;      // period = heartbeat * period_multiplier
    int enable;
    void (*task_function) (void *);
    void * params;
}TaskData; 

// Run the scheduler
void scheduler_run(TaskData tasks[]);

#endif