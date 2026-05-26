#ifndef ADC_H
#define	ADC_H

#include <xc.h> 

#define MANUAL 0
#define AUTO 1

#define BAT 11
#define IR 14

// ADC setup function
void adc_setup();

// Returns data from ADC, select the pin to read from (AN11 - battery, AN14 - IR)
double adc_read(int pin);

// Value conversion for battery
double battery_conversion(int read_value);

// Value conversion for IR
double IR_conversion(int read_value);

#endif 