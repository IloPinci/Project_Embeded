/*
 * File:   project_main.c
 * Author: boli
 *
 * Created on May 25, 2026, 3:55 PM
 */


#include "xc.h"
#include "timer.h"
#include "spi.h"
#include "uart.h"
#include "adc.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#define Max_Tasks 6

typedef struct{
    int counter;
    int period;      // period = heartbeat * period_multiplier
    int enable;
    void (*task_function) (void *, int);
    void * params;
}TaskData; 

typedef struct{
    float mag_data[1];
    float accel_data[2];
    int current_state;      // 1 -> halted; 2 -> obs avoid; 3 -> moving
}shared_data;


TaskData schedInfo[Max_Tasks];

void scheduler_run(TaskData tasks[], int loops){

    for (int i = 0; i < Max_Tasks; i++){
        // should the task be executed
        if (tasks[i].enable == 0){
            tasks[i].counter = 0;   //if not then we clear its counter
            continue;
        } 

        if (++tasks[i].counter == tasks[i].period){
            tasks[i].task_function(tasks[i].params, loops);    // we execute the task
            tasks[i].counter = 0;
        }
    }
}

void led_blink(void* param, int loops){
    shared_data *sd = (shared_data *) param; 

    LATAbits.LATA0 !=  LATAbits.LATA0;

    // halted
    if (sd->current_state == 1){
        LATRbits.LATB8 != LATRbits.LATB8;   // left side    
        LATRbits.LATF1 != LATRbits.LATF1;   // right side
    }
    // ob avoid
    else if (sd->current_state == 2){
        LATRbits.LATF1 != LATRbits.LATF1;
    } 
}

//TODO: We should read the IR and transmit the value every 100ms
void ir_read(void* param, int loops){
    //TODO: get the correct reading
    float distance = adc_auto_read()

    // every 1 hz we transmit what we have read
    if (loops % 50 == 0){
        char buffer[11] = sprintf(buffer, "$MDIST,%.2f*", distance);
        uart_transmit(buffer);
    }
}

void initial_setup(){
    //! Disable analog inputs 
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

    // lights
    TRISAbits.TRISA0 = 0;   // LED1 output
    TRISRbits.TRISB8 = 0;   // left side lights
    TRISRbits.TRISF1 = 0;   // right side lights
    TRISRbits.TRISF0 = 0;   // brakes
    TRISRbits.TRISG1 = 0;   // low intensity
    TRISRbits.TRISA7 = 0;   // high intensity 


    LATAbits.LATA0 = 1;     // LED1 on in the initial state
    LATRbits.LATB8 = 0;     
    LATRbits.LATF1 = 0;     
    LATRbits.LATF0 = 0;     
    LATRbits.LATG1 = 0;     
    LATRbits.LATA7 = 0;     


    // buttons
    TRISEbits.TRISE8 = 1;   // Button 1 input
    TRISEbits.TRISE9 = 1;   // Button 2 input

    

    //! library setups
    tmr_setup_period(TIMER1, 2);    // 500Hz -> 2ms
    uart_setup();
    spi_setup();
    mag_setup();
    adc_setup(AUTO, AUTO, BIT12, 2);    //Todo: what the fuck should be the last argument


    //! TASK SETUP
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
    schedInfo[0].params = NULL;


    //? PWD
    schedInfo[1].counter = 0;
    schedInfo[1].period = 1;
    schedInfo[1].enable = 1;
    schedInfo[1].task_function = pwd_update;
    schedInfo[1].params = NULL;


    //? Accelerometer - read
    schedInfo[2].counter = 0;
    schedInfo[2].period = 50;
    schedInfo[2].enable = 1;
    schedInfo[2].task_function = accel_read;
    schedInfo[2].params = NULL;


    //? Magnetometer - read
    schedInfo[3].counter = 0;
    schedInfo[3].period = 50;
    schedInfo[3].enable = 1;
    schedInfo[3].task_function = mag_read;
    schedInfo[3].params = NULL;


    //? Board LED
    schedInfo[4].counter = 0;
    schedInfo[4].period = 500;
    schedInfo[4].enable = 1;
    schedInfo[4].task_function = led_blink;
    schedInfo[4].params = NULL;
}




int main(void) {

    initial_setup();
    
    int loops = 0;

    while(1){
        scheduler_run(++loops);
        tmr_wait_period(TIMER1);
    }

    return 0;
}
