#include "Timer1B.h"

void Timer1B_Wait1us(uint8_t delay, unsigned long period) {
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1;        // Activate TIMER1
    TIMER1_CTL_R &= ~TIMER_CTL_TBEN;                  // Disable TIMER1B during setup
		TIMER1_CFG_R = TIMER_CFG_16_BIT;  	 						  // Configure for 16-bit mode
    TIMER1_TAMR_R = TIMER_TBMR_TBMR_PERIOD;           // Set periodic mode, down-counter
    TIMER1_TAPR_R = PRESCALE_1US;        							//Prescale = desired frequency/number of bits in register = (16 000 000MHz x 0.25s)/2^16 
    TIMER1_TAILR_R = delay*period - 1;                // period // Set load value for 0.25 seconds
    TIMER1_ICR_R = TIMER_ICR_TBTOCINT;                // Clear timeout flag
    //TIMER1_IMR_R = TIMER_IMR_TBTOIM;                  // Arm timeout interrupt
    //NVIC_PRI4_R = (NVIC_PRI4_R & ~NVIC_PRI33_INTD_M) | NVIC_INT_CTRL_NMI_SET; // Set interrupt priority
    //NVIC_EN0_R = 1 << 21;                             // Enable interrupt 21
    TIMER1_CTL_R |= TIMER_CTL_TBEN;                   // Enable TIMER1B
}