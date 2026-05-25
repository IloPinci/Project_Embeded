#ifndef TIMER_CONFIG_H
#define TIMER_CONFIG_H

#include <xc.h>
#include <stdint.h>

#define TIMER1 1
#define TIMER2 2
#define TIMER3 3

void tmr_setup_period(int timer, int ms);
int tmr_wait_period(int timer);
void tmr_wait_ms(int timer, int ms);

#endif