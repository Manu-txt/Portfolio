// Timer0.c
// Runs on LM4F120/TM4C123
// Use TIMER0 in 32-bit periodic mode to request interrupts at a periodic rate
// Daniel Valvano
// March 20, 2014

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013
  Program 7.5, example 7.6

 Copyright 2013 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

//#include "tm4c123gh6pm.h"
//#include "Timer0A.h"

//void (*PeriodicTask1)(void);   // user function

// ***************** Timer1A_Init ****************
// Activate Timer1A interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq) and prescaler
// Outputs: none
#include "tm4c123gh6pm.h"
#include "Timer0A.h"

void Timer1A_Init(unsigned long period) {
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1;        // Activate TIMER1
    TIMER1_CTL_R &= ~TIMER_CTL_TAEN;                  // Disable TIMER0A during setup
		TIMER1_CFG_R = TIMER_CFG_16_BIT;  	 						  //  configure for 16-bit mode
    TIMER1_TAMR_R = TIMER_TAMR_TAMR_PERIOD;           // Set periodic mode, down-counter
    TIMER1_TAPR_R = prescale;        								  //For the prescale = desired frequency/number of bits in register = (16 000 000MHz x 0.25s)/2^16 
    TIMER1_TAILR_R = period - 1;                      // period // Set load value for 0.25 seconds
    TIMER1_ICR_R = TIMER_ICR_TATOCINT;                // Clear timeout flag
    TIMER1_IMR_R = TIMER_IMR_TATOIM;                  // Arm timeout interrupt
    NVIC_PRI4_R = (NVIC_PRI4_R & ~NVIC_PRI33_INTD_M) | NVIC_INT_CTRL_NMI_SET; // Set interrupt priority
    NVIC_EN0_R = 1 << 21;                             // Enable interrupt 21
    TIMER1_CTL_R |= TIMER_CTL_TAEN;                   // Enable TIMER0A
}

void Timer1A_Handler(void) {
    TIMER1_ICR_R = TIMER_ICR_TATOCINT;  // Clear the timeout flag
    RED_LED ^= RED_LED_MASK; // Toggle the red LED on PF1
}
