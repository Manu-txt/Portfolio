// Systick.c
// Runs on TM4C123
// CECS 347 Project 2 - Space Invaders
// Group number:
// Group members:
// By Min He
// November 15, 2022
#include "Systick.h"
#include "tm4c123gh6pm.h"

void SysTick_Init(void){
		NVIC_ST_CTRL_R = 0;                                    // disable SysTick during setup
	///10Hz screen refresh timing control
		NVIC_ST_RELOAD_R = (8000000) - 1;            // number of counts to wait 100ms (assuming 80MHz clock)
    NVIC_ST_CURRENT_R = 0;                            // any write to CURRENT clears
    NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x1FFFFFFF)|0x40000000;    // priority 1
    NVIC_ST_CTRL_R = 0x07;                            // enable SysTick with core and interrupts
}

void SysTick_Wait3s(void){
		NVIC_ST_CTRL_R = 0;                                    // disable SysTick during setup
	///10Hz screen refresh timing control
		NVIC_ST_RELOAD_R = 8000000 - 1;            // number of counts to wait 100ms (assuming 80MHz clock)
    NVIC_ST_CURRENT_R = 0;                            // any write to CURRENT clears
    NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x1FFFFFFF)|0x40000000;    // priority 1
    NVIC_ST_CTRL_R = 0x07;	// enable SysTick with core and interrupts
	for (int i = 0; i < 30; i++) {
        // Wait for the COUNTFLAG to be set, which indicates the timer has expired
        while ((NVIC_ST_CTRL_R & 0x00010000) == 0) {
            // Wait until COUNTFLAG is set (indicating timer finished)
        }
        
    }
}