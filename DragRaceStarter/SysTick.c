// SysTick.c
// Runs on TM4C123
// Starter file for CECS346 Project 2
// By Dr. Min He

// oscilloscope or LED connected to PF2 for period measurement

#include "tm4c123gh6pm.h"
#include <stdint.h> // C99 data types

// Constants
#define NVIC_ST_CTRL_ENABLE				0x00000001
#define NVIC_ST_CTRL_CLK					0x07

void SysTick_Init(void) {	
	NVIC_ST_CTRL_R &= ~NVIC_ST_CTRL_ENABLE; // disabled SysTick during setup
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x1FFFFFFF) | 0x60000000; // Priority 3
	NVIC_ST_CTRL_R |= NVIC_ST_CTRL_CLK | NVIC_ST_CTRL_INTEN;
}

void SysTick_Start(uint32_t period) {	
	NVIC_ST_RELOAD_R = period - 1; // reload value
	NVIC_ST_CTRL_R |= NVIC_ST_CTRL_CLK;
}
