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


#define Max_Tasks 9
#define ir_threshold 30

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

// PWM struct
typedef struct{
    int speed;
    int yaw_rate;
}pwm_state;

// a data structure which is shared by the tasks
typedef struct{
    // ADC sensor readings
    float battery_voltage;
    float ir_distance;

    // IMU
    Sensor_DataStruct accel_data;
    Sensor_DataStruct mag_data;

    // handling of buttons
    volatile int button_1_original;     // RE8
    volatile int button_2_original;     // RE9
    int button1_confirmed;
    int button2_confirmed;
    int receive_size;
    int transmit_size;

    // TODO: PWM TO READ FROM UART
    pwm_state pwm;

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
    adc_setup();
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
    
    // we find the ammount of data as athe difference 
    global_system_state.receive_size = (receive_buffer.head + R_BUF_SIZE - receive_buffer.tail ) % R_BUF_SIZE;

    global_system_state.transmit_size = (transmit_buffer.head + T_BUF_SIZE - transmit_buffer.tail ) % T_BUF_SIZE;

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


//! Tasks

//* finished ??
void led_blink(void* param){
    LATAbits.LATA0 =  !LATAbits.LATA0;
    // I would put it inside the main
    // as the requirements state:
    // The LED A0 and should blink at 1 Hz frequency at all times, to indicate the functioning of the main loop.
}

//* finished ??
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


// Read the IR and transmit the value every 100ms
void ir_read(void* param){
    shared_data *sd = (shared_data *) param;
    sd->ir_distance = adc_read(IR);
}

// Read battery level
void battery_read(void* param){
    shared_data *sd = (shared_data *) param;
    sd->battery_voltage = adc_read(BAT);
}

// TODO FSM
void finite_state_machine(void* param){
    shared_data *sd = (shared_data *) param;
    switch (sd->current_car_state){
        case HALT:
            LATBbits.LATB8 = !LATBbits.LATB8;   // left side blink 
            LATFbits.LATF1 = !LATFbits.LATF1;   // right side blink
            LATGbits.LATG1 = 0;                 // low off
            pwm_stop_all();                     // stop all the motors
            break;

        case MOVE:
            LATBbits.LATB8 = 0;                 //left  off     
            LATFbits.LATF1 = 0;                 // right off     
            LATGbits.LATG1 = 1;                 // low   on 
            // Move the buggy according to the speed and yaw_rate values received by UART
            pwm_control(sd->pwm.speed,sd->pwm.yaw_rate);     
            break;

        case AVOID:
            LATBbits.LATB8 = 0;                 // left side blink 
            LATFbits.LATF1 = !LATFbits.LATF1;   // right side blink
            LATGbits.LATG1 = 1;                 // low   on  
            break;

        default:
            LATBbits.LATB8 = 0;
            LATFbits.LATF1 = 0;
            LATGbits.LATG1 = 0;
            break;
    }
}

// TODO
void parse_uart(void* param){

}

//* finished ??
void button_handler(void* param){
    shared_data *data = (shared_data *) param;

    // handle the buffer sizes
    if (data->button_2_original == 1){
        char buffer[32];

        sprintf(buffer, "$MBUF,%d,%d*", data->transmit_size, data->receive_size);
        uart_transmit(buffer);

        data->button_2_original = 0;
        data->button2_confirmed = 1;
    }

    // handle the state transitions
    if (data->button_1_original == 1){
        
        if(data->current_car_state == HALT){
            data->current_car_state = MOVE;
        }
        else{
            data->current_car_state = HALT;
        }

        data->button_1_original = 0;
        data->button1_confirmed = 1;
    }

    // after 300ms the button can be pressed again this way we avoid bounces
    if (data->button1_confirmed > 0) {
        if(++data->button1_confirmed >=3 ){
            data->button1_confirmed = 0;
            IFS1bits.INT1IF = 0;
            IEC1bits.INT1IE = 1;
        }
    }

    if (data->button2_confirmed > 0) {
        if(++data->button2_confirmed >=3){
            data->button2_confirmed = 0;
            IFS1bits.INT2IF = 0;
            IEC1bits.INT2IE = 1;
        }
    }
}

//TODO
void accelerometer(void* param){
    shared_data *sd = (shared_data *) param;
    sd->accel_data = accel_read();
}

//TODO
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


    //? Handler of the FSM
    schedInfo[2].counter = 0;
    schedInfo[2].period = 50;
    schedInfo[2].enable = 0;
    schedInfo[2].task_function = finite_state_machine;
    schedInfo[2].params = (void*)&global_system_state;


    //? We parse the receiving messages 
    schedInfo[3].counter = 15;
    schedInfo[3].period = 50;
    schedInfo[3].enable = 1;
    schedInfo[3].task_function = parse_uart;
    schedInfo[3].params = (void*)&global_system_state;


    //? Light Control
    schedInfo[4].counter = 5;
    schedInfo[4].period = 250;
    schedInfo[4].enable = 1;
    schedInfo[4].task_function = led_blink;
    schedInfo[4].params = (void*)&global_system_state;


    //* Shifted tasks
    //? Uart transmitting
    schedInfo[5].counter = 5;
    schedInfo[5].period = 50;
    schedInfo[5].enable = 1;
    schedInfo[5].task_function = uart_sending;
    schedInfo[5].params = (void*)&global_system_state;


    //? Accelerometer
    schedInfo[6].counter = 15;
    schedInfo[6].period = 50;
    schedInfo[6].enable = 1;
    schedInfo[6].task_function = accelerometer;
    schedInfo[6].params = (void*)&global_system_state;


    //? Magnetometer
    schedInfo[7].counter = 25;
    schedInfo[7].period = 50;
    schedInfo[7].enable = 1;
    schedInfo[7].task_function = magnetometer;
    schedInfo[7].params = (void*)&global_system_state;


    //? Button debounce
    schedInfo[8].counter = 40;
    schedInfo[8].period = 50;
    schedInfo[8].enable = 1;
    schedInfo[8].task_function = button_handler;
    schedInfo[8].params = (void*)&global_system_state;

    //TODO: add battery read task or insert it inside another task
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
