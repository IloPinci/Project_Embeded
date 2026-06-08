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
#define CTRL_DT 0.002f   // 500 Hz control loop -> 2 ms per tick

//! CONSTANTS FOR OBSTACLE AVOIDANCE STATE MACHINE------
// to put them in the "general" library
#define INIT 0
#define ROT_CLOCKWISE 1
#define MOVE_FORWARD 2
#define ROT_COUNTERCLOCKWISE 3
//------------------------------------------------------

//! Data structures
// what a task should have
typedef struct{
    int counter;
    int period;      // period = heartbeat * period_multiplier
    int enable;
    void (*task_function) (void *);
    void * params;
}TaskData; 

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

// here we get the buffer that is found in the uart.c so we don't have to declare it again
extern Circular_Buffer receive_buffer;
extern Circular_Buffer transmit_buffer;

//! Setups
void port_setup(){
    //! Disable analog inputs 
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

    // removed the adc bits setup, since they are stup inside adc_setup()

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

//! Scheduler execution
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

//! Function
static int parser(const char *msg, int *speed, int *yawRate) {

    if (strncmp(msg, "$PCREF,", 7) != 0) return 0;

    const char *ptr = msg + 7;
    char *end;

    int parsed_speed = (int)strtol(ptr, &end, 10);
    if (*end != ',') return 0;

    int parsed_yaw = (int)strtol(end + 1, &end, 10);
    if (*end != '*') return 0;

    *speed   = parsed_speed;
    *yawRate = parsed_yaw;
    return 1;
}

//! Tasks

// Handles all LEDs blink
void led_blink(void* param){

    shared_data *sd = (shared_data *) param; 

    LATAbits.LATA0 =  !LATAbits.LATA0;          // Always blink the DSP led 

   switch (sd->current_car_state){
        case HALT:
            LATBbits.LATB8 = !LATBbits.LATB8;   // Left side blink 
            LATFbits.LATF1 = LATBbits.LATB8     // Right side blink
            LATGbits.LATG1 = 0;                 // Low off
            break;

        case MOVE:
            LATBbits.LATB8 = 0;                 // Left  off     
            LATFbits.LATF1 = 0;                 // Right off     
            LATGbits.LATG1 = 1;                 // Low on      
            break;

        case AVOID:
            LATBbits.LATB8 = 0;                 // Left side blink 
            LATFbits.LATF1 = !LATFbits.LATF1;   // Right side blink
            LATGbits.LATG1 = 1;                 // Low on  
            break;

        default:
            LATBbits.LATB8 = 0;
            LATFbits.LATF1 = 0;
            LATGbits.LATG1 = 0;
            break;
   }
}

// Send required values to UART
void uart_sending(void* param){

    shared_data *sd = (shared_data *) param;
    char buffer[32];

    // IR
    sprintf(buffer, "$MDIST,%d*\n", (int)(sd->ir_distance + 0.5f));
    uart_transmit(buffer);

    // Magnetometer
    sprintf(buffer, "$MANGLE,%.2f,%.2f,%.2f*\n",
         sd->accel_data.roll, 
         sd->accel_data.pitch, 
         sd->yaw);
    uart_transmit(buffer);

    // Every 1 hz we transmit what we have read. We enter uart_sending every 50 loops.
    // We want to send the the voltage every 500 loops. So we have to send it if we enter in the uart_sending 10 times
    if (++sd->led_toggle % 10 == 0){
        sprintf(buffer, "$MBATT,%.2f*\n", sd->battery_voltage);
        uart_transmit(buffer);
        sd->led_toggle = 0;
    }
}

// Read IR value
void ir_read(void* param){
    
    shared_data *sd = (shared_data *) param;
    
    float raw_ir_data = adc_read(IR);       // Read IR from channel 14
    
    // Compute voltage
    float voltage = 3.3 * raw_ir_data / 1024.0;     // For 10 bit adc and 3.3 voltage range
    
    // Convert into distance (cm)
    float converted_distance = 100 * (2.34f
                         - 4.74f * voltage
                         + 4.06f * voltage * voltage
                         - 1.60f * voltage * voltage * voltage
                         + 0.24f * voltage * voltage * voltage * voltage);

    sd->ir_distance = converted_distance;

    // Handle all the car states according to the distance threshold
    if (converted_distance <= ir_threshold){
        if(sd->current_car_state == MOVE){
            sd->current_car_state = AVOID;
        } else if(sd->current_car_state == AVOID && sd->obs_avoid_var.state == MOVE_FORWARD){
            sd->current_car_state = HALT;
        }
    }     
}

// Read
void battery_read(void* param){

    shared_data *sd = (shared_data *) param;
    
    float raw_bat_data = adc_read(BATTERY);       // Read battery from channel 11

    // Compute voltage
    float voltage = 3.3 * raw_bat_data / 1024.0;  // For 10 bit adc and 3.3 voltage range

    // Convert voltage divider value to actual battery value
    float result = 3 * voltage;

    sd->battery_voltage = result; 
}

// PWM control: handles the car movement for all the states
void pwm_control(void* param){

    shared_data *sd = (shared_data *) param;
    switch (sd->current_car_state){
        case HALT:
            pwm_stop_all();     // Stop all the motors

            // Reset obstacle avoidance variables 
            sd->obs_avoid_var.state = INIT;
            sd->obs_avoid_var.rep = 0;
            sd->obs_avoid_var.two_sec_counter = 0;        
            break;

        case MOVE:
            // Move the buggy according to the speed and yaw_rate values received by UART
            pwm_move(sd->pwm.speed,sd->pwm.yawRate);
            break;

        case AVOID:
            // INIT: start clockwise rotation
            if (sd->obs_avoid_var.state == INIT) {
                sd->obs_avoid_var.swept = 0.0f;             // Reset integrated angle
                sd->obs_avoid_var.state = ROT_CLOCKWISE;
                pwm_move(0, -70);                           // Rotate clockwise
            }

            // ROT_CLOCKWISE: integrate gyro z until 90 deg swept
            if (sd->obs_avoid_var.state == ROT_CLOCKWISE) {
                GyroData g = gyro_read();
                sd->obs_avoid_var.swept += g.z * CTRL_DT;   // deg/s * s = deg
                if (fabs(sd->obs_avoid_var.swept) >= 90.0f) {
                    sd->obs_avoid_var.state = MOVE_FORWARD;
                    sd->obs_avoid_var.two_sec_counter = 0;
                    pwm_move(50, 0);                        // Forward at low speed
                }
            }

            // MOVE_FORWARD: 2 s at 500 Hz = 1000 ticks
            if (sd->obs_avoid_var.state == MOVE_FORWARD) {
                if (++sd->obs_avoid_var.two_sec_counter >= 1000) {
                    sd->obs_avoid_var.state = ROT_COUNTERCLOCKWISE;
                    sd->obs_avoid_var.swept = 0.0f;       // Reset for the return turn
                    pwm_move(0, 70);                      // Rotate anticlockwise
                }
            }

            // ROT_COUNTERCLOCKWISE: integrate gyro z until 90 deg back
            if (sd->obs_avoid_var.state == ROT_COUNTERCLOCKWISE) {
                GyroData g = gyro_read();
                sd->obs_avoid_var.swept += g.z * CTRL_DT;
                if (fabs(sd->obs_avoid_var.swept) >= 90.0f) {
                    pwm_stop_all();
                    sd->obs_avoid_var.state = INIT;

                    // We restart the obstacle avoidance execution for a maximum of three times in a row
                    if (sd->ir_distance <= ir_threshold) {
                        sd->obs_avoid_var.rep++;
                        // After the third time the car moves to HALT state
                        if (sd->obs_avoid_var.rep >= 3) {
                            sd->obs_avoid_var.rep = 0;
                            sd->current_car_state = HALT;
                        }
                    } else {
                        // If no obstacle is detected, move back to MOVE state
                        sd->obs_avoid_var.rep = 0;
                        sd->current_car_state = MOVE;
                    }
                }
            }
            break;

        default:
            break;
    }
}

// Parser
void parse_uart(void* param){

    shared_data *sd = (shared_data *) param;
    char buffer[32];

    if (!uart_receive_line(buffer, sizeof(buffer))) return;

    int spd = 0, yaw = 0;
    if (parser(buffer, &spd, &yaw)) {
        if (spd >= -100 && spd <= 100 && yaw >= -100 && yaw <= 100){
            sd->pwm.speed   = spd;
            sd->pwm.yawRate = yaw;
        }
    }
}

// Handles the buttons
void button_handler(void* param){

    shared_data *sd = (shared_data *) param;

    // Handle the buffer sizes
    if (sd->button_2_original == 1){
        char buffer[32];

        sprintf(buffer, "$MBUF,%d,%d\n*", sd->transmit_size, sd->receive_size);
        uart_transmit(buffer);

        sd->button_2_original = 0;
        sd->button2_confirmed = 1;
    }

    // Handle the state transitions
    if (sd->button_1_original == 1){
        
        if(sd->current_car_state == HALT){
            sd->current_car_state = MOVE;
        }
        else{
            sd->current_car_state = HALT;
        }

        sd->button_1_original = 0;
        sd->button1_confirmed = 1;
    }

    // After 300ms the button can be pressed again in order to avoid bounces
    if (sd->button1_confirmed > 0) {
        if(++sd->button1_confirmed >=3 ){
            sd->button1_confirmed = 0;
            IFS1bits.INT1IF = 0;
            IEC1bits.INT1IE = 1;
        }
    }

    if (sd->button2_confirmed > 0) {
        if(++sd->button2_confirmed >=3){
            sd->button2_confirmed = 0;
            IFS1bits.INT2IF = 0;
            IEC1bits.INT2IE = 1;
        }
    }
}

// Read accelerometer and magnetometer values
void accel_mag_read(void* param){

    shared_data *sd = (shared_data *) param;

    sd->accel_data = accel_read();
    sd->mag_data = mag_read();

    // Convert to radians for better calculation
    float roll_rad  = sd->accel_data.roll  * (PI / 180.0f);
    float pitch_rad = sd->accel_data.pitch * (PI / 180.0f);

    // Tilt compensation in case that the car is in a slope
    float x = sd->mag_data.axis_x * cosf(pitch_rad) 
            + sd->mag_data.axis_y * sinf(roll_rad) * sinf(pitch_rad) 
            + sd->mag_data.axis_z * cosf(roll_rad) * sinf(pitch_rad);

    float y = sd->mag_data.axis_y * cosf(roll_rad) 
            - sd->mag_data.axis_z * sinf(roll_rad);

    // Compute yaw value after tilt compensation
    sd->yaw = atan2f(-y, x) * (180.0f / PI);
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
    schedInfo[0].enable = 1;
    schedInfo[0].task_function = ir_read;
    schedInfo[0].params = (void*)&global_system_state;


    //? PWM control
    schedInfo[1].counter = 0;
    schedInfo[1].period = 1;
    schedInfo[1].enable = 1;
    schedInfo[1].task_function = pwm_control;
    schedInfo[1].params = (void*)&global_system_state;


    // We offset the tasks that have the same period so they don't run in the same tick. 
    // Also the sensors have a lower offset than the consumers so the data is updated.

    //? We parse the receiving messages 
    schedInfo[2].counter = 0;
    schedInfo[2].period = 50;
    schedInfo[2].enable = 1;
    schedInfo[2].task_function = parse_uart;
    schedInfo[2].params = (void*)&global_system_state;


    //? Accelerometer & Magnetometer
    schedInfo[3].counter = 10;
    schedInfo[3].period = 50;
    schedInfo[3].enable = 1;
    schedInfo[3].task_function = accel_mag_read;
    schedInfo[3].params = (void*)&global_system_state;


    //? Button handler
    schedInfo[4].counter = 20;
    schedInfo[4].period = 50;
    schedInfo[4].enable = 1;
    schedInfo[4].task_function = button_handler;
    schedInfo[4].params = (void*)&global_system_state;


    //? Uart Transmit (all the messages at once)
    schedInfo[5].counter = 30;
    schedInfo[5].period = 50;
    schedInfo[5].enable = 1;
    schedInfo[5].task_function = uart_sending;
    schedInfo[5].params = (void*)&global_system_state;


    // //? Led blinking
    schedInfo[6].counter = 0;
    schedInfo[6].period = 250;
    schedInfo[6].enable = 1;
    schedInfo[6].task_function = led_blink;
    schedInfo[6].params = (void*)&global_system_state;


    //? Button debounce
    schedInfo[7].counter = 0;
    schedInfo[7].period = 500;
    schedInfo[7].enable = 1;
    schedInfo[7].task_function = battery_read;
    schedInfo[7].params = (void*)&global_system_state;
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
