#ifndef ADC_H
#define	ADC_H

#include <xc.h> 

#define MANUAL 0
#define AUTO 1

#define BAT 11
#define IR 5

#define BIT10 0
#define BIT12 1

/* 
 * ADC setup (single channel), choose: 
 * -10 or 12 bits mode (input: BIT10/BIT12)
 * -Manual/automatic sampling and conversion (input: MANUAL/AUTO)
 * -Analog pin to read from (input: pin number)
*/
void adc_setup(int bit_mode, int sampling_mode, int conversion_mode, int an_read);

// Returns data from ADC, set the parameters according to adc_setup
double adc_read(int sampling_mode, int conversion_mode);

#endif 