#include "xc.h"
#include "pwm.h"
#include <stdlib.h>

void pwm_setup(int oc_select){
    
    switch(oc_select){
        case 1:
            OC1CON1bits.OCM = 0;            // Disable before setup
            OC1CON1bits.OCTSEL = 7;         // Peripheral clock as source
            OC1CON2bits.SYNCSEL = 0x1F;     // Peripheral clock -> no sync source
            OC1RS = PERIOD_TICKS;           // One tick = 1 / 72 MHz = 13.9 ns -> 72 MHz / 10 KHz = 7200 (-1?)
            OC1R = 0;                       // Initialize duty cycle at 0 (no movement)
            OC1CON1bits.OCM = 6;            // Output compare edge aligned mode (turn on OC1)
            break;
        case 2:
            OC2CON1bits.OCM = 0;            // Disable before setup
            OC2CON1bits.OCTSEL = 7;         // Peripheral clock as source
            OC2CON2bits.SYNCSEL = 0x1F;     // Peripheral clock -> no sync source
            OC2RS = PERIOD_TICKS;           // One tick = 1 / 72 MHz = 13.9 ns -> 72 MHz / 10 KHz = 7200 (-1?)
            OC2R = 0;                       // Initialize duty cycle at 0 (no movement)
            OC2CON1bits.OCM = 6;            // Output compare edge aligned mode (turn non OC2)
            break;
        case 3:
            OC3CON1bits.OCM = 0;            // Disable before setup
            OC3CON1bits.OCTSEL = 7;         // Peripheral clock as source
            OC3CON2bits.SYNCSEL = 0x1F;     // Peripheral clock -> no sync source
            OC3RS = PERIOD_TICKS;           // One tick = 1 / 72 MHz = 13.9 ns -> 72 MHz / 10 KHz = 7200 (-1?)
            OC3R = 0;                       // Initialize duty cycle at 0 (no movement)
            OC3CON1bits.OCM = 6;            // Output compare edge aligned mode (turn on OC1)
            break;
        case 4:
            OC4CON1bits.OCM = 0;            // Disable before setup
            OC4CON1bits.OCTSEL = 7;         // Peripheral clock as source
            OC4CON2bits.SYNCSEL = 0x1F;     // Peripheral clock -> no sync source
            OC4RS = PERIOD_TICKS;           // One tick = 1 / 72 MHz = 13.9 ns -> 72 MHz / 10 KHz = 7200 (-1?)
            OC4R = 0;                       // Initialize duty cycle at 0 (no movement)
            OC4CON1bits.OCM = 6;            // Output compare edge aligned mode (turn non OC2)
            break;
        default:
            break;
    }
}

void pwm_setup_all(void){
    pwm_setup(1);
    pwm_setup(2);
    pwm_setup(3);
    pwm_setup(4);
}

void pwm_stop(int oc_select){
    
    switch(oc_select){
        case 1:
            OC1R = 0;
            break;
        case 2:
            OC2R = 0;
            break;
        case 3:
            OC3R = 0;
            break;
        case 4:
            OC4R = 0;
            break;
        default:
            break;
    }
}

void pwm_stop_all(void){
    OC1R = 0;
    OC2R = 0;
    OC3R = 0;
    OC4R = 0;
}

void pwm_move(int speed, int yaw_rate){
    
    int left_pwm  = speed - yaw_rate;
    int right_pwm = speed + yaw_rate;
    
    // Saturation
    if (left_pwm > 100) left_pwm = 100;
    if (left_pwm < -100) left_pwm = -100;

    if (right_pwm > 100) right_pwm = 100;
    if (right_pwm < -100) right_pwm = -100;
    
    // Convert PWM values to duty cycle
    unsigned int duty_ticks_left = pwm_to_ticks(left_pwm);
    unsigned int duty_ticks_right = pwm_to_ticks(right_pwm);
    
    // Set left wheel PWMs
    if(left_pwm > 0){
        // Enable forward movement, disable backward
        OC1R = duty_ticks_left;
        OC2R = 0;
    } else if(left_pwm < 0){
        // Enable backward movement, disable forward
        OC1R = 0;
        OC2R = duty_ticks_left;
    } else{
        // Stop the motor
        OC1R = 0;
        OC2R = 0;
    }
    
    // Set right wheel PWMs
    if(right_pwm > 0){
        // Enable forward movement, disable backward
        OC3R = duty_ticks_right;
        OC4R = 0;
    } else if(right_pwm < 0){
        // Enable backward movement, disable forward
        OC3R = 0;
        OC4R = duty_ticks_right;
    } else{
        // Stop the motor
        OC3R = 0;
        OC4R = 0;
    }
     
}

unsigned int pwm_to_ticks(int pwm_percent) {

    if (pwm_percent == 0) {
        return 0;
    }

    unsigned int abs_pwm = abs(pwm_percent);

    // map 1-100 command to 40-100 duty cycle
    unsigned int duty_percent = 40U + (abs_pwm * 60U) / 100U;

    return ((PERIOD_TICKS + 1U) * duty_percent) / 100U;
}