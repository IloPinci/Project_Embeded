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
#include "pwm.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#define Max_Tasks 8
#define ir_threshold 30
#define debounce_ticks 3

//! Data structures
// what a task should have
typedef struct{
    int counter;
    int period;      // period = heartbeat * period_multiplier
    int enable;
    void (*task_function) (void *);
    void * params;
}TaskData; 

// a finite state machine for the car
typedef enum{
    HALT = 0,
    MOVE = 1,
    AVOID = 2
}car_state;

// a data structure which is shared by the tasks
typedef struct{
    // ADC sensor readings
    float battery_voltage;
    float ir_distance;

    // IMU
    Sensor_DataStruct accel_data;
    Sensor_DataStruct mag_data;

    // uart recieve
    volatile int speed;
    volatile int yawRate;

    // handling of buttons
    volatile int button_1_original;     // RE8
    volatile int button_2_original;     // RE9
    int button1_confirmed;
    int button2_confirmed;

    car_state current_car_state;

    int led_toggle;
    int adc_ready;
}shared_data;

TaskData schedInfo[Max_Tasks];
shared_data global_system_state = {0};

// here we get the buffer that is found in the uart.c so we don't have to declare it again
extern Circular_Buffer receive_buffer;
extern Circular_Buffer transmit_buffer;

//! Setups
void port_setup(){
    //! Disable analog inputs 
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    // enable AN5 and AN11 (the ir and the battery)
    ANSELBbits.ANSB5  = 1;  
    ANSELBbits.ANSB11 = 1;


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

    // the ISR for the buttons
    INTCON2bits.INT1EP = 1; 
    INTCON2bits.INT2EP = 1;
    // clear the flags
    IFS1bits.INT1IF = 0;
    IFS1bits.INT2IF = 0;
    // enable the interrupts
    IEC1bits.INT1IE = 1;
    IEC1bits.INT2IE = 1;
}

void library_setup(){
    tmr_setup_period(TIMER1, 2);    // 500Hz -> 2ms
    uart_setup();
    spi_setup();        
    mag_setup(); 
    pwm_setup_all();   
    
    // we use scan mode so both the battery and the IR ca be read in a non blocking way
    adc_scan_setup(BIT10);    // battery
}


//! Interrupts
void __attribute__((interrupt, no_auto_psv)) _INT1Interrupt(void) {
    global_system_state.button_1_original = 1;
    
    // clear the flag and disable the interrupt. The disabing is done to combat bounces. The interrupt enable is activated after 200ms which should be sufficient time to allow for it
    IFS1bits.INT1IF = 0; 
    IEC1bits.INT1IE = 0;
}

void __attribute__((interrupt, no_auto_psv)) _INT2Interrupt(void) {
    global_system_state.button_2_original = 1;
    
    // clear the flag and disable the interrupt. The disabing is done to combat bounces. The interrupt enable is activated after 200ms which should be sufficient time to allow for it
    IFS1bits.INT2IF = 0; 
    IEC1bits.INT2IE = 0;
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
//    if (sd->car_state->current_car_state == 1){
//        LATBbits.LATB8 = !LATBbits.LATB8;   // left side    
//        LATFbits.LATF1 = !LATFbits.LATF1;   // right side
//    }
//    // ob avoid
//    else if (sd->car_state->current_car_state == 2){
//        LATFbits.LATF1 = !LATFbits.LATF1;
//    } 
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
    if (++dat->led_toggle % 10 == 0){
        sprintf(buffer, "$MBATT,%.2f*", dat->battery_voltage);
        uart_transmit(buffer);
        dat->led_toggle = 0;
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
    schedInfo[5].counter = 0;
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
            uart_transmit("$MISS*");
        }
    }

    return 0;
}
