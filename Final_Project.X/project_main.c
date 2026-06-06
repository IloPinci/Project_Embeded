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
    double obstacle_yaw;        // Store current yaw
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

// ------ OBSTACLE AVOIDANCE HELPER FUNCTION--------
// to put in the "general" library
// Compute yaw difference
double angle_diff(double a, double b) {
    double diff = a - b;
    // Wrap into [-180, +180]
    while (diff > 180.0f)  diff -= 360.0f;
    while (diff < -180.0f) diff += 360.0f;
    return diff;
}

//------------------------------------------------------

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
    
    // we use scan mode so both the battery and the IR ca be read in a non blocking way
    //adc_scan_setup(BIT10);    // battery
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

//?? finished ??
void led_blink(void* param){
    shared_data *data = (shared_data *) param; 

    LATAbits.LATA0 =  !LATAbits.LATA0;

   switch (data->current_car_state){
        case HALT:
            LATBbits.LATB8 = !LATBbits.LATB8;    // left side blink 
            LATFbits.LATF1 = !LATFbits.LATF1;    // right side blink
            LATGbits.LATG1 = 0;                  // low off
            break;

        case MOVE:
            LATBbits.LATB8 = 0;                 //left  off     
            LATFbits.LATF1 = 0;                 // right off     
            LATGbits.LATG1 = 1;                 // low   on      
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

//?? We send correctly the values. 
void uart_sending(void* param){
    shared_data *dat = (shared_data *) param;
    char buffer[32];

    // IR
    sprintf(buffer, "$MDIST,%d*\n", (int)(dat->ir_distance + 0.5f));
    uart_transmit(buffer);

    // magnetometer
    sprintf(buffer, "$MANGLE,%.2f,%.2f,%.2f*\n",
         dat->accel_data.roll, 
         dat->accel_data.pitch, 
         dat->yaw);
    uart_transmit(buffer);

    // every 1 hz we transmit what we have read. We enter uart_sending every 50 loops. And we want to send the the voltage every 500 loops. So we have to send it if we enter in the uart_sending 10 times
    if (++dat->led_toggle % 10 == 0){
        sprintf(buffer, "$MBATT,%.2f*\n", dat->battery_voltage);
        uart_transmit(buffer);
        dat->led_toggle = 0;
    }
}

//* finished ??
void ir_read(void* param){
    shared_data *sd = (shared_data *) param;
    
    float raw_ir_data = adc_read(14);       // read ir from channel 14
    
    // calculate voltage
    float voltage = 3.3 * raw_ir_data / 1024.0;     // for 10 bit adc and 3.3 voltage range
    
    // convert into distance (cm)
    float converted_distance = 100 * (2.34f
                         - 4.74f * voltage
                         + 4.06f * voltage * voltage
                         - 1.60f * voltage * voltage * voltage
                         + 0.24f * voltage * voltage * voltage * voltage);


    sd->ir_distance = converted_distance;


    //! This should handle all the requirements
    if (converted_distance <= ir_threshold){
        if(sd->current_car_state == MOVE){
            sd->current_car_state = AVOID;
        } else if(sd->current_car_state == AVOID && sd->obs_avoid_var.state == MOVE_FORWARD){
            sd->current_car_state = HALT;
        }
    }     
}

//* finished ??
void battery_read(void* param){
    shared_data *sd = (shared_data *) param;
    
    float raw_bat_data = adc_read(11);       // read battery from channel 11
    // calculate voltage
    float voltage = 3.3 * raw_bat_data / 1024.0;     // for 10 bit adc and 3.3 voltage range
    // multiply to account for whole battery
    float result = 3 * voltage;
    sd->battery_voltage = result; 
}


//TODO the pwm control
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
            //pwm_move(sd->pwm.speed,sd->pwm.yawRate);     
            pwm_move(100,0); 
            break;

        case AVOID:
            // INIT -> ROT_CLOCKWISE 
            if (sd->obs_avoid_var.state == INIT) {
                sd->obs_avoid_var.obstacle_yaw = sd->yaw;   // Store current yaw
                sd->obs_avoid_var.state = ROT_CLOCKWISE;
                pwm_move(0, -50);    // Start rotating clockwise
            }

            // Sub-state: ROT_CLOCKWISE 
            // Wait until ~90 degrees have been swept
            if (sd->obs_avoid_var.state == ROT_CLOCKWISE) {
                if (fabs(angle_diff(sd->yaw, sd->obs_avoid_var.obstacle_yaw)) >= 90.0) {
                    sd->obs_avoid_var.state = MOVE_FORWARD;
                    sd->obs_avoid_var.two_sec_counter = 0;
                    pwm_move(10, 0);     // Move forward at low speed
                }
            }

            // Sub-state: MOVE_FORWARD
            // pwm_update called at 500 Hz -> 2 s = 1000 ticks
            if (sd->obs_avoid_var.state == MOVE_FORWARD) {
                if (++sd->obs_avoid_var.two_sec_counter >= 1000) {
                    sd->obs_avoid_var.state = ROT_COUNTERCLOCKWISE;
                    sd->obs_avoid_var.obstacle_yaw = sd->yaw;   // Store current yaw
                    pwm_move(0, 50);     // Start rotating anticlockwise
                }
            }

            // Sub-state: ROT_COUNTERCLOCKWISEs
            // Return to the previous heading (~90 deg back)
            if (sd->obs_avoid_var.state == ROT_COUNTERCLOCKWISE) {
                if (fabs(angle_diff(sd->yaw, sd->obs_avoid_var.obstacle_yaw)) >= 90.0) {
                    pwm_stop_all();
                    sd->obs_avoid_var.state = INIT;

                    if (sd->ir_distance <= ir_threshold) {
                        // Obstacle still there: try again or give up
                        sd->obs_avoid_var.rep++;
                        if (sd->obs_avoid_var.rep >= 3) {
                            sd->obs_avoid_var.rep = 0;
                            sd->current_car_state = HALT;
                        }
                        // else: loop back — next call will re-enter INIT
                    } else {
                        // Clear path: go back to MOVE
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



//* finished ??
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

//?? finished 
void button_handler(void* param){
    shared_data *data = (shared_data *) param;

    // handle the buffer sizes
    if (data->button_2_original == 1){
        char buffer[32];

        sprintf(buffer, "$MBUF,%d,%d\n*", data->transmit_size, data->receive_size);
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

//* finished ??
void accel_mag_read(void* param){
    shared_data *sd = (shared_data *) param;

    // we separated the data structures so they do not feed garbage to each other
    sd->accel_data = accel_read();
    sd->mag_data = mag_read();

    // we have to convert to radiant for better calculation
    float roll_rad  = sd->accel_data.roll  * (PI / 180.0f);
    float pitch_rad = sd->accel_data.pitch * (PI / 180.0f);

    // tiilt compesation in case that the car is in a slope
    float x = sd->mag_data.axis_x * cosf(pitch_rad) 
            + sd->mag_data.axis_y * sinf(roll_rad) * sinf(pitch_rad) 
            + sd->mag_data.axis_z * cosf(roll_rad) * sinf(pitch_rad);

    float y = sd->mag_data.axis_y * cosf(roll_rad) 
            - sd->mag_data.axis_z * sinf(roll_rad);

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


    //* We offset the tasks that have the same period so they don't run in the same tick. Also the sensors have a lower offset than the consumers so the data is updated.

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
