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
#include "tasks.h"
#include "scheduler.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//! Data structures

// OBSTACLE AVOIDANCE STATE MACHINE STRUCT
typedef struct{
    int  state;                 // Sub-state
    int  two_sec_counter;       // Two seconds counter for the movement after rotation
    int  rep;                   // After three rep goes in HALT state
    float swept;                // Integrated turn angle in degrees
}Obs_avoid; 

// AS SIMETTI SUGGESTED, PUT SPEED AND YAWRATE IN A STRUCT
typedef struct{
    int speed;
    int yawRate;
}pwm_variables; 

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
    AccelData accel_data;
    MagData mag_data;
    float yaw;

    // uart recieve
    pwm_variables pwm;

    // handling of buttons
    volatile int button_1_original;     // RE8
    volatile int button_2_original;     // RE9
    int button1_confirmed;
    int button2_confirmed;
    int receive_size;
    int transmit_size;

    car_state current_car_state;

    // NEW
    Obs_avoid obs_avoid_var;

    int led_toggle;
    int adc_ready;
}shared_data;

TaskData schedInfo[Max_Tasks];
shared_data global_system_state = {0};

// Get the buffer that is found in the uart.c so we don't have to declare it again
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

    //route the RPI88 and 89 to the interrupt
    RPINR0bits.INT1R = 0x58;
    RPINR1bits.INT2R = 0x59;

    // clear the flags
    IFS1bits.INT1IF = 0;
    IFS1bits.INT2IF = 0;
    // enable the interrupts
    IEC1bits.INT1IE = 1;
    IEC1bits.INT2IE = 1;
    
    
    TRISDbits.TRISD1 = 0;       // -Left PWM (VCPcon/RP65/RD1)
    TRISDbits.TRISD2 = 0;       // Left PWM (DPH/RP66/RD2)
    TRISDbits.TRISD3 = 0;       // -Right PWM (PMBE/RP67/RD3)
    TRISDbits.TRISD4 = 0;       // Right PWM (PMWR/RP68/RD4)
    
    // Pin remapping
    RPOR1bits.RP66R = 0b010000;     // Map left PWM to OC1
    RPOR0bits.RP65R = 0b010001;     // Map -left PWM to OC2
    RPOR2bits.RP68R = 0b010010;     // Map right PWM to OC3
    RPOR1bits.RP67R = 0b010011;     // Map -right PWM to OC4
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
    
    // Clear the flag and disable the interrupt. The disabing is done to combat bounces. 
    // The interrupt enable is activated after 200ms which should be sufficient time to allow for it
    IFS1bits.INT1IF = 0; 
    IEC1bits.INT1IE = 0;
}

void __attribute__((interrupt, no_auto_psv)) _INT2Interrupt(void) {

    global_system_state.button_2_original = 1;
    
    // Find the amount of data as the difference 
    global_system_state.receive_size = (receive_buffer.head + R_BUF_SIZE - receive_buffer.tail ) % R_BUF_SIZE;

    global_system_state.transmit_size = (transmit_buffer.head + T_BUF_SIZE - transmit_buffer.tail ) % T_BUF_SIZE;

    // Clear the flag and disable the interrupt. The disabing is done to combat bounces. 
    // The interrupt enable is activated after 200ms which should be sufficient time to allow for it
    IFS1bits.INT2IF = 0; 
    IEC1bits.INT2IE = 0;
}

int main(void) {

    port_setup();
    library_setup();
    task_setup();
    
    while(1){
        scheduler_run(schedInfo);
        if(tmr_wait_period(TIMER1)){
            //uart_transmit("$MISS*");
        }
    }

    return 0;
}
