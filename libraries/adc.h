#ifndef ADC_H
#define	ADC_H

#include <xc.h> 

#define MANUAL 0
#define AUTO 1

#define BIT10 0
#define BIT12 1

#define BATTERY 11
#define IR 5

void adc_setup();

double adc_read(int channel);

double adc_scan_read(float *ch0, float *ch1);

#endif