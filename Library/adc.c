#include "xc.h"
#include "timer.h"
#include "adc.h"

void adc_setup(){
    
    // Pins setup
    ANSELBbits.ANSB14 = 1;      // Turn on analog mode for pin RB14 (AN14, IR sensor - mikrobus 2)
    ANSELBbits.ANSB11 = 1;      // Turn on analog mode for pin RB11 (AN11, BAT_VSENSE)

    TRISBbits.TRISB14 = 1;      // AN14 input
    TRISBbits.TRISB11 = 1;      // AN11 Input
    TRISBbits.TRISB9 = 0;       // B9 output
    LATBbits.LATB9 = 1;         // B9 high -> enable IR reading

    // ADC1 setup
    AD1CON1bits.ADON = 0;       // Turn ADC off
    
    AD1CON1bits.AD12B = 0;      // Select 10/12-bits mode
    AD1CON3bits.ADCS = 8;       // Clock prescaler to 8 for 10-bit Tad

    // Manual sampling - automatic conversion mode
    AD1CON1bits.ASAM = 0;       // Manual start
    AD1CON3bits.SAMC = 16;      // Sample time 16 Tad
    AD1CON1bits.SSRC = 7;       // Conversion starts after time specified by SAMC
    
    AD1CON2bits.CHPS = 0;       // Select one channel (CH0)

    // Note: CH0SA bit is set inside adc_read in order to select the desired pin to read from
    
    AD1CON1bits.ADON = 1;       // Turn ADC on
}

// ADC1 read
double adc_read(int pin){

    AD1CHS0bits.CH0SA = pin;        // Set the pin we want to read from
    
    AD1CON1bits.DONE = 0;           // Put DONE = 0 to prevent sampling issues
    AD1CON1bits.SAMP = 1;           // Start sampling

    while (!AD1CON1bits.DONE);      // Wait for conversion

    // Return converted result
    if(pin == BAT){
        return battery_conversion(ADC1BUF0);
    } else {
        return IR_conversion(ADC1BUF0);
    }

}

// Value conversion for battery
double battery_conversion(int read_value){
    BAT_VSENSE = 3.3 * (double )read_value / 1024.0;    // Convert raw value to analog voltage
    return (R49 + R51 + R54)/R54 * BAT_VSENSE;          // Convert voltage from voltage divider
}

// Value conversion for IR
double IR_conversion(int read_value){
    V_IR = 3.3 * (double) read_value / 1024.0;     // Convert raw value to analog voltage
    double V2 = V_IR * V_IR;            // Manually compute the powers, since pow(x,y) is CPU heavy
    double V3 = V2 * V_IR;
    double V4 = V3 * V_IR;
    return 100.0 * (2.34 - 4.74 * V_IR + 4.06 * V2 - 1.60 * V3 + 0.24 * V4);    // Converting the voltage into distance (cm)
}