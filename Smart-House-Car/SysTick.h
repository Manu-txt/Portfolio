// SysTick.h
// Runs on TM4C123
// Starter program for CECS 346 Project 3
// 
// November 27, 2024
// by Min HE

#include <stdint.h>

#define T1ms 							16000U   // Systick reload value to generate 1ms delay, assumes using 16 MHz Clock.
#define HALF_SEC      		500*T1ms 	//systick reload value for 0.5s

// Initialize SysTick with core clock.
void SysTick_Init(void);

// Time delay using busy wait.
// The delay parameter is in units of the core clock:1/16MHz 
void SysTick_Wait(uint32_t delay);

