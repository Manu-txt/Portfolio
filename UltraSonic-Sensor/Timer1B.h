#ifndef TIMER1BINTS_H // do not include more than once
#define TIMER1BINTS_H
#include <stdint.h>
#include "tm4c123gh6pm.h"

#define PRESCALE_1US 0
// ** Timer1B_Init **
// Activate Timer1B interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
  void Timer1B_Wait1us(uint8_t delay, unsigned long period);
	//void Timer1B_Handler(void);


#endif // TIMER1BINTS_H