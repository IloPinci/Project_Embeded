#ifndef TIMER_H
#define TIMER_H

#include <xc.h>
#include <stdint.h>

#define TIMER1 1
#define TIMER2 2
#define TIMER3 3

// Setup the desired timer
void tmr_setup_period(int timer, int ms);

// Wait for the period specified in the setup
int tmr_wait_period(int timer);

// Wait for the desired amount of milliseconds
void tmr_wait_ms(int timer, int ms);

#endif