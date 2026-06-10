#include "timer.h"

#define Fcy 72000000UL
#define Prescaler 256
#define Ticks_per_ms (Fcy / (Prescaler * 1000UL))

void tmr_setup_period(int timer, int ms){
    
    // The value of the Fcy = 72MHz
    // To support up to 200ms a 1:256 prescaler is needed
    uint16_t pr = (Ticks_per_ms * ms);

    if (timer == TIMER1){
        T1CONbits.TON = 0;      // Turn off the timer
        T1CONbits.TCS = 0;      // Specify Fcy as the clock source
        T1CONbits.TCKPS = 3;    // Define prescaler as 1:256

        TMR1 = 0;               // Reset the timer 
        IFS0bits.T1IF = 0;      // Reset the flag
        
        PR1 = pr - 1;           // As it needs one cycle to see that it has reached the desired number
        T1CONbits.TON = 1;      // Start the timer
    }
    else if(timer == TIMER2){
        T2CONbits.TON = 0; 
        T2CONbits.TCS = 0;  
        T2CONbits.TCKPS = 3;

        TMR2 = 0;     
        IFS0bits.T2IF = 0;      
        
        PR2 = pr - 1;
        T2CONbits.TON = 1;
    }
    
    else if(timer == TIMER3){
        T3CONbits.TON = 0; 
        T3CONbits.TCS = 0;  
        T3CONbits.TCKPS = 3;

        TMR3 = 0;     
        IFS0bits.T3IF = 0;      
        
        PR3 = pr - 1;
        T3CONbits.TON = 1;
    }
}


int tmr_wait_period(int timer){

    if (timer == TIMER1){
        if(IFS0bits.T1IF == 1){
            IFS0bits.T1IF = 0;          // Clear the flag before exiting early
            return 1;
        }
        while(IFS0bits.T1IF == 0);      // Busy waiting
        IFS0bits.T1IF = 0;              // Reset it (perhaps it is redundant)
    }

    else if (timer == TIMER2){
        if(IFS0bits.T2IF == 1){
            IFS0bits.T2IF = 0;
            return 1;
        }
        while(IFS0bits.T2IF == 0);
        IFS0bits.T2IF = 0;
    }
    
    else if (timer == TIMER3){
        if(IFS0bits.T3IF == 1){
            IFS0bits.T3IF = 0;
            return 1;
        }
        while(IFS0bits.T3IF == 0);
        IFS0bits.T3IF = 0;
    }

    return 0;
}


void tmr_wait_ms(int timer, int ms){
   
    // Define the lowest unit allowed -> 1ms
    tmr_setup_period(timer, 1);

    // Iterate until the value inputed is reached
    for (int i = 0; i < ms; i++){
        tmr_wait_period(timer);
    }

    if (timer == TIMER1){
        T1CONbits.TON = 0; 
    }
    else if (timer == TIMER2){
        T2CONbits.TON = 0; 
    }
    else if (timer == TIMER3){
        T3CONbits.TON = 0; 
    }
}