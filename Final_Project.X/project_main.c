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

//! Task structure definition
typedef struct{
    int counter;
    int period;      // period = heartbeat * period_multiplier
    int enable;
    void (*task_function) (void *);
    void * params;
}TaskData; 
//! Shared data structure
typedef struct{
    float battery_voltage;
    float ir_distance;
    int loop;
    int current_state;      // 1 -> halted; 2 -> obs avoid; 3 -> moving
    Sensor_DataStruct accel_data;
    Sensor_DataStruct mag_data;
}shared_data;

TaskData schedInfo[Max_Tasks];
shared_data global_system_state = {0};


void port_setup(){
    //! Disable analog inputs 
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

    // lights
    TRISAbits.TRISA0 = 0;   // LED1 output
    TRISBbits.TRISB8 = 0;   // left side lights
    TRISFbits.TRISF1 = 0;   // right side lights
    TRISFbits.TRISF0 = 0;   // brakes
    TRISGbits.TRISG1 = 0;   // low intensity
    TRISAbits.TRISA7 = 0;   // high intensity 


    LATAbits.LATA0 = 1;     // LED1 on in the initial state
    LATBbits.LATB8 = 0;     
    LATFbits.LATF1 = 0;     
    LATFbits.LATF0 = 0;     
    LATGbits.LATG1 = 0;     
    LATAbits.LATA7 = 0;     


    // buttons
    TRISEbits.TRISE8 = 1;   // Button 1 input
    TRISEbits.TRISE9 = 1;   // Button 2 input
}
//! library setups
void library_setup(){
    tmr_setup_period(TIMER1, 2);    // 500Hz -> 2ms
    uart_setup();
    spi_setup();        
    mag_setup();            
    adc_setup(AUTO, AUTO, BIT10, BATTERY);    // battery
}

//! Scheduler execution
void scheduler_run(TaskData tasks[]){

    for (int i = 0; i < Max_Tasks; i++){
        // should the task be executed
        if (tasks[i].enable == 0){
            tasks[i].counter = 0;   //if not then we clear its counter
            continue;
        } 

        if (++tasks[i].counter == tasks[i].period){
            tasks[i].task_function(tasks[i].params);    // we execute the task
            tasks[i].counter = 0;
        }
    }
}


void led_blink(void* param){
    shared_data *sd = (shared_data *) param; 

    LATAbits.LATA0 =  !LATAbits.LATA0;

    // halted
    if (sd->current_state == 1){
        LATBbits.LATB8 = !LATBbits.LATB8;   // left side    
        LATFbits.LATF1 = !LATFbits.LATF1;   // right side
    }
    // ob avoid
    else if (sd->current_state == 2){
        LATFbits.LATF1 = !LATFbits.LATF1;
    } 
}


void uart_sending(void* param){
    shared_data *dat = (shared_data *) param;
    char buffer[32];

    sprintf(buffer, "$MDIST,%.2f*", dat->ir_distance);
    uart_transmit(buffer);

    sprintf(buffer, "$MANGLE,%.2f,%.2f,%.2f*",
         dat->mag_data.axis_x, 
         dat->mag_data.axis_y, 
         dat->mag_data.axis_z);
    uart_transmit(buffer);

    // every 1 hz we transmit what we have read. We enter uart_sending every 50 loops. And we want to send the the voltage every 500 loops. So we have to send it if we enter in the uart_sending 10 times
    if (++dat->loop % 10 == 0){
        sprintf(buffer, "$MBATT,%.2f*", dat->battery_voltage);
        uart_transmit(buffer);
        dat->loop = 0;
    }
}


//TODO: We should read the IR and transmit the value every 100ms
void ir_read(void* param){
    shared_data *sd = (shared_data *) param;

    sd->ir_distance = adc_auto_read();
}


void pwd_update(void* param){
    int x;
}


void accelerometer(void* param){
    shared_data *sd = (shared_data *) param;
    sd->accel_data = accel_read();
}

void magnetometer(void* param){
    shared_data *sd = (shared_data *) param;
    sd->mag_data = mag_read();
}


//! TASK SETUP
void task_setup(){
    /*
        period = 1   (2ms       = 500Hz)        
        period = 50  (100ms     = 10Hz)     
        period = 500 (1000ms    = 1Hz)
    */

    //? IR - read
    schedInfo[0].counter = 0;
    schedInfo[0].period = 1;
    schedInfo[0].enable = 0;
    schedInfo[0].task_function = ir_read;
    schedInfo[0].params = (void*)&global_system_state;


    //? PWD
    schedInfo[1].counter = 0;
    schedInfo[1].period = 1;
    schedInfo[1].enable = 0;
    schedInfo[1].task_function = pwd_update;
    schedInfo[1].params = (void*)&global_system_state;


    //? Accelerometer - read
    schedInfo[2].counter = 0;
    schedInfo[2].period = 50;
    schedInfo[2].enable = 0;
    schedInfo[2].task_function = accelerometer;
    schedInfo[2].params = (void*)&global_system_state;


    //? Magnetometer - read
    schedInfo[3].counter = 15;
    schedInfo[3].period = 50;
    schedInfo[3].enable = 1;
    schedInfo[3].task_function = magnetometer;
    schedInfo[3].params = (void*)&global_system_state;


    //? Light Control
    schedInfo[4].counter = 5;
    schedInfo[4].period = 500;
    schedInfo[4].enable = 1;
    schedInfo[4].task_function = led_blink;
    schedInfo[4].params = (void*)&global_system_state;


    //? Uart transmitting
    schedInfo[5].counter = 30;
    schedInfo[5].period = 50;
    schedInfo[5].enable = 1;
    schedInfo[5].task_function = uart_sending;
    schedInfo[5].params = (void*)&global_system_state;


    // //? Accelerometer
    // schedInfo[6].counter = 0;
    // schedInfo[6].period = 50;
    // schedInfo[6].enable = 1;
    // schedInfo[6].task_function = accelerometer;
    // schedInfo[6].params = (void*)&global_system_state;


    // //? Magnetometer
    // schedInfo[7].counter = 0;
    // schedInfo[7].period = 50;
    // schedInfo[7].enable = 1;
    // schedInfo[7].task_function = magnetometer;
    // schedInfo[7].params = (void*)&global_system_state;
}


int main(void) {

    port_setup();
    library_setup();
    task_setup();

    while(1){
        scheduler_run(schedInfo);
        if(tmr_wait_period(TIMER1)){
            uart_transmit("!!!!!!!!!!!!!!!!!!!!");
        }
    }

    return 0;
}
