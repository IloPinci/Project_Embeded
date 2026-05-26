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
double adc_read(int channel){

    AD1CHS0bits.CH0SA = channel;    // Set the channel we want to read from
    
    AD1CON1bits.DONE = 0;           // Put DONE = 0 to prevent sampling issues
    AD1CON1bits.SAMP = 1;           // Start sampling

    while (!AD1CON1bits.DONE);      // Wait for conversion

    return ADC1BUF0;                // Return result
}