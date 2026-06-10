#include "xc.h"
#include "scheduler.h"

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