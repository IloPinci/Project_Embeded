#include "xc.h"
#include "timer.h"
#include "adc.h"

void adc_setup(int bit_mode, int sampling_mode, int conversion_mode, int an_read){
    
    AD1CON1bits.ADON = 0;           // turn ADC off
    
    AD1CON1bits.AD12B = bit_mode;   // Select 10/12-bits mode
    if(bit_mode == BIT10){
        AD1CON3bits.ADCS = 8;       // clock prescaler to 8 for 10-bit Tad; must ensure a minimum Tad time of 76 nsec
    } else {
        AD1CON3bits.ADCS = 10;      // clock prescaler to 10 for 10-bit Tad; must ensure a minimum Tad time of 117.6 nsec
    }
    
    // Sampling and conversion modes setup (I removed the "else" as Cannata suggested)
    if(sampling_mode == MANUAL && conversion_mode == MANUAL){
        AD1CON1bits.ASAM = 0;       // manual sampling
        AD1CON1bits.SSRC = 0;       // manual end/ begin conversion
    }
    if(sampling_mode == MANUAL && conversion_mode == AUTO){
        AD1CON1bits.ASAM = 0;       // manual start
        AD1CON3bits.SAMC = 16;      // sample time 16 Tad
        AD1CON1bits.SSRC = 7;       // conversion starts after time specified by SAMC
    }
    if(sampling_mode == AUTO && conversion_mode == MANUAL){
        AD1CON1bits.ASAM = 1;       // automatic sampling
        AD1CON1bits.SSRC = 0;       // manual end/ begin conversion
    }
    if(sampling_mode == AUTO && conversion_mode == AUTO){
        AD1CON1bits.ASAM = 1;       // automatic start
        AD1CON3bits.SAMC = 16;      // sample time 16 Tad
        AD1CON1bits.SSRC = 7;       // conversion starts after time specified by SAMC
    }
    
    AD1CON2bits.CHPS = 0;           // select one channel (CH0)
    
    AD1CHS0bits.CH0SA = an_read;    // chose battery (AN011) as +V/ reading for the channel8
    
    AD1CON1bits.ADON = 1;           // turn ADC on
}


// TODO instead of making a function for everything make one function adc_red_channel
// or something that chooses where to read from??/

double adc_read(int sampling_mode, int conversion_mode){
    
    if(sampling_mode == MANUAL && conversion_mode == MANUAL){
        AD1CON1bits.DONE = 0;       // Put DONE = 0 to prevent sampling issues
        AD1CON1bits.SAMP = 1;       // Start sampling
        tmr_wait_ms(TIMER1,1);      // TODO not sure what time appropriate
        AD1CON1bits.SAMP = 0;       // Stop sampling -> conversion starts

        while(!AD1CON1bits.DONE);   // Wait for conversion

        return ADC1BUF0;            // Return result
    }
    if(sampling_mode == MANUAL && conversion_mode == AUTO){
        AD1CON1bits.DONE = 0;       // Put DONE = 0 to prevent sampling issues
        AD1CON1bits.SAMP = 1;       // Start sampling

        while (!AD1CON1bits.DONE);  // Wait for conversion

        return ADC1BUF0;            // Return result
    }
    if(sampling_mode == AUTO && conversion_mode == MANUAL){
        AD1CON1bits.DONE = 0;       // Put DONE = 0 to prevent sampling issues

        // sampling is already active automatically after previous conversion
        tmr_wait_ms(TIMER1, 1);     // allow sampling time

        AD1CON1bits.SAMP = 0;       // manually stop sampling -> start conversion

        while (!AD1CON1bits.DONE);  // Wait for conversion

        return ADC1BUF0;            // Return result
    }
    if(sampling_mode == AUTO && conversion_mode == AUTO){
        AD1CON1bits.DONE = 0;       // Put DONE = 0 to prevent sampling issues

        // ADC runs automatically -> wait for one completed conversion
        while (!AD1CON1bits.DONE);

        return ADC1BUF0;
    }
    
    // Default in the case of wrong parameters values
    return -1;
}

/*void adc_ir_setup(){
    
    //TODO not sure if this should be here
    TRISBbits.TRISB4 = 0;       // set as output
    TRISBbits.TRISB5 = 1;       // set as input
    LATBbits.LATB4 = 1;         // enable
    
    AD1CON1bits.ADON = 0;       // turn ADC off
    
    AD1CON3bits.ADCS = 8;       // clock prescaler to 8 for 10-bit Tad; must ensure a minimum Tad time of 76 nsec
    //AD1CON3bits.SAMC = 10;      // sample time 16 Tad
    
    AD1CON1bits.ASAM = 0;       // manual start
    AD1CON3bits.SAMC = 16;      // sample time 16 Tad
    AD1CON1bits.SSRC = 7;       // conversion starts after time specified by SAMC 

    
    AD1CON2bits.CHPS = 0;       // select 1 channel
    AD1CHS0bits.CH0SA = 5;      // chose battery (AN5) as +V/ reading for the channel
    
    ANSELBbits.ANSB5 = 1;       // set AN5 -> RB5 battery pin to analog (as we want to read voltages)
    
    AD1CON1bits.ADON = 1;       // turn ADC on
}*/
