#include "xc.h"
#include "timer.h"
#include "adc.h"

void adc_setup(int bit_mode, int sampling_mode, int conversion_mode, int an_read){
    
    //TODO make this reconfigurable
    AD1CON1bits.ADON = 0;       // turn ADC off
    
    AD1CON1bits.AD12B = bit_mode;
    if(bit_mode == BIT10){AD1CON3bits.ADCS = 8;}        // clock prescaler to 8 for 10-bit Tad; must ensure a minimum Tad time of 76 nsec
    else{ AD1CON3bits.ADCS = 10; }
    
    if(sampling_mode == MANUAL && conversion_mode == MANUAL){
        AD1CON1bits.ASAM = 0;       // manual sampling
        AD1CON1bits.SSRC = 0;       // manual end/ begin conversion
    }else if(sampling_mode == MANUAL && conversion_mode == AUTO){
        AD1CON1bits.ASAM = 0;       // manual start
        AD1CON3bits.SAMC = 16;      // sample time 16 Tad
        AD1CON1bits.SSRC = 7;       // conversion starts after time specified by SAMC
    }else if(sampling_mode == AUTO && conversion_mode == MANUAL){
        AD1CON1bits.ASAM = 1;       // automatic sampling
        AD1CON1bits.SSRC = 0;       // manual end/ begin conversion
    }else if(sampling_mode == AUTO && conversion_mode == AUTO){
        AD1CON1bits.ASAM = 1;       // automatic start
        AD1CON3bits.SAMC = 16;      // sample time 16 Tad TODO maybe make this adjustable???
        AD1CON1bits.SSRC = 7;       // conversion starts after time specified by SAMC
    }
    
    AD1CON2bits.CHPS = 0;       // select 1 channel
    
    AD1CHS0bits.CH0SA = an_read;     // chose battery (AN011) as +V/ reading for the channel
    
    AD1CON1bits.ADON = 1;       // turn ADC on

}

// helper function fully manual
float adc_manual_read(){
    
    AD1CON1bits.DONE = 0;          // put done to 0 to prevent issues with sampling
    
    AD1CON1bits.SAMP = 1;     // start sampling
    tmr_wait_ms(TIMER1,1);           // TODO not sure what time appropriate

    AD1CON1bits.SAMP = 0;     // stop sampling -> conversion starts

    while(!AD1CON1bits.DONE); // wait for conversion

    return ADC1BUF0;          // return result
    
}

// helper function Manual Start automatic conversion
float adc_manual_start_only_read(){
    
    AD1CON1bits.DONE = 0;      // put done to 0 to prevent issues with sampling
    
    AD1CON1bits.SAMP = 1;      // start sampling

    while(!AD1CON1bits.DONE);  // wait for conversion

    return ADC1BUF0;
}

// helper function automatic start Manual Conversion
float adc_manual_stop_only_read(){
    
    AD1CON1bits.DONE = 0;       // Put DONE = 0 to prevent sampling issues

    // sampling is already active automatically after previous conversion
    tmr_wait_ms(TIMER1, 1);     // allow sampling time

    AD1CON1bits.SAMP = 0;       // manually stop sampling -> start conversion

    while (!AD1CON1bits.DONE);  // Wait for conversion

    return ADC1BUF0;            // Return result
}

// helper function fully automatic
float adc_auto_read(){
    
    AD1CON1bits.DONE = 0;       // Put DONE = 0 to prevent sampling issues
    
    // ADC runs automatically -> wait for one completed conversion
    while (!AD1CON1bits.DONE);
    
    return ADC1BUF0; 
}

//TODO we can make it so the global variables save the sampling and conversion mode from setup so we dont need parameters
float adc_read(int sampling_mode, int conversion_mode){
    
    if(sampling_mode == MANUAL && conversion_mode == MANUAL){
        return adc_manual_read();        
    }
    if(sampling_mode == MANUAL && conversion_mode == AUTO){
        return adc_manual_start_only_read();
    }
    if(sampling_mode == AUTO && conversion_mode == MANUAL){
        return adc_manual_stop_only_read();
    }
    if(sampling_mode == AUTO && conversion_mode == AUTO){
        return adc_auto_read();
    }
    
    // Default in the case of wrong parameters values
    return -1;
}

// Scan Mode

void adc_scan_setup(int bit_mode) {

    AD1CON1bits.ADON = 0;           // turn ADC off 

    AD1CON1bits.AD12B = bit_mode;   // 10 or 12 bit mode

    // setting the clock prescaler
    if (bit_mode == BIT10) {
        AD1CON3bits.ADCS = 8;       // Tad >= 76ns for 10-bit
    } else {
        AD1CON3bits.ADCS = 10;
    }

    // fully automatic: auto sample + auto convert
    AD1CON1bits.ASAM = 1;           // automatic sampling starts after conversion
    AD1CON3bits.SAMC = 16;          // sample time = 16 Tad // TODO need to figure out if this time is ok 
    AD1CON1bits.SSRC = 7;           // auto convert after SAMC

    // scan mode
    AD1CON2bits.CSCNA = 1;          // enable channel scan on CH0
    AD1CON2bits.CHPS  = 0;          // use CH0 only (scan fills it sequentially)
    AD1CON2bits.SMPI  = 1;          // flag/interrupt after every 2 conversions (SMPI = N-1)

    // select which AN pins to scan: AN5 (IR) and AN11 (battery)
    AD1CSSLbits.CSS5  = 1;          // scan AN5
    AD1CSSLbits.CSS11 = 1;          // scan AN11

    // clear CHOSA, not sure if obsolete because of scan anyways??
    AD1CHS0bits.CH0SA = 0;

    AD1CON1bits.ADON = 1;           // turn ADC on
}

// reads latest scan results from the two buffers
// ADC fills BUF0 = first scanned channel (AN5), BUF1 = second (AN11)
int adc_scan_read(float *ch0, float *ch1) {
    
    // we check if it is ready or not. If it isn't we skip we don't busy wait
    if (!AD1CON1bits.DONE){
        return 0;
    }     

    AD1CON1bits.DONE = 0;           // we clear the done 

    *ch0 = ADC1BUF0;                // IR sensor data
    *ch1 = ADC1BUF1;                // battery data
    
    return 1;
}