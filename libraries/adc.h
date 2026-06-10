#ifndef ADC_H
#define	ADC_H

#include <xc.h> 

#define MANUAL 0
#define AUTO 1

#define BIT10 0
#define BIT12 1

#define BATTERY 11
#define IR 14

// Setup the ADC
void adc_setup();

// Read from the desired channel
double adc_read(int channel);

#endif