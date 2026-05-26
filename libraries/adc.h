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

float adc_read(int sampling_mode, int conversion_mode);

float adc_manual_read();

//void adc_ir_setup();

float adc_auto_read();

float adc_manual_stop_only_read();

float adc_manual_start_only_read();

int adc_scan_read(float *ch0, float *ch1);

void adc_scan_setup(int bit_mode);

#endif