#include "xc.h"
#include "system.h"
#include "timer.h"
#include "timer.h"
#include "uart.h"
#include "spi.h"
#include "adc.h"
#include "pwm.h"

//! Setups
void port_setup(){
    
    // Disable analog inputs 
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

    // Lights
    TRISAbits.TRISA0 = 0;   // LED1 output
    TRISBbits.TRISB8 = 0;   // Left side lights
    TRISFbits.TRISF1 = 0;   // Right side lights
    TRISFbits.TRISF0 = 0;   // Brakes
    TRISGbits.TRISG1 = 0;   // Low intensity light
    TRISAbits.TRISA7 = 0;   // High intensity light

    LATAbits.LATA0 = 1;     // LED1 on in the initial state
    LATBbits.LATB8 = 0;     
    LATFbits.LATF1 = 0;     
    LATFbits.LATF0 = 0;     
    LATGbits.LATG1 = 0;     
    LATAbits.LATA7 = 0;     


    // Buttons
    TRISEbits.TRISE8 = 1;   // Button 1 input
    TRISEbits.TRISE9 = 1;   // Button 2 input

    // ISRs for the buttons
    INTCON2bits.INT1EP = 1; 
    INTCON2bits.INT2EP = 1;

    // Route RPI88 and RPI89 to the interrupts
    RPINR0bits.INT1R = 0x58;
    RPINR1bits.INT2R = 0x59;

    // Clear the flags
    IFS1bits.INT1IF = 0;
    IFS1bits.INT2IF = 0;
    
    // Enable the interrupts
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

