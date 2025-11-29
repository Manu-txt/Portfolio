/*
``Class: CECS 347 Embedded Systems 2
``Group 5
Project 1
Filename: Project1P2.c
Professor: Min He
01/29/2025
 */
#include "tm4c123gh6pm.h"
#include "Timer1A.h"
#include <stdint.h>

#define ONE_MICRO_SECOND 16
#define SYSCTL_RCGCTIMER_TIMER1 SYSCTL_RCGCTIMER_R1
#define PRESCALE 5	//For the prescale = desired frequency/number of bits in register = (16 000 000MHz x 0.25s)/2^16 

volatile uint8_t Timer1A_Flag = 0;  // Flag set by the interrupt handler

void Timer1A_Init(void) {
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_TIMER1;   // Activate TIMER1 clock

    TIMER1_CTL_R &= ~TIMER_CTL_TAEN;                 // Disable TIMER1A during setup
    TIMER1_CFG_R = TIMER_CFG_16_BIT;                 // Configure for 16-bit timer mode
    TIMER1_TAMR_R = TIMER_TAMR_TAMR_PERIOD;          // Periodic mode
    TIMER1_TAPR_R = PRESCALE;                        // Set prescaler for timer
    TIMER1_IMR_R = TIMER_IMR_TATOIM;                 // Enable timeout interrupt
    TIMER1_ICR_R = TIMER_ICR_TATOCINT;               // Clear any pending timeout interrupt
    NVIC_EN0_R = 1 << 21;                            // Enable IRQ 21 in NVIC for TIMER1A
}


void Timer1A_Handler(void) {
    TIMER1_ICR_R = TIMER_ICR_TATOCINT;  // Acknowledge TIMER1A timeout
    Timer1A_Flag = 1;                   // Set the flag to indicate timer completion
}
//       Maximum distance for HC-SR04 = 400 cm
//       Round-trip time for 400 cm = (2 * 400) / 0.0343 cm/?s ? 23368 ?s
//       Timer configuration:
//          - Clock = 16 MHz
//          - Prescaler = 5 (divides clock by 6)
//          - Tick time = 6 / 16,000,000 = 0.375 ?s per tick
//          - Required ticks for 23368 ?s = 23368 / 0.375 = 62355 ticks
//          - ILR  can handle to 65536 ticks 
// 			 Period = desired frequency/(period +1)
void GPTM_START(uint32_t period) {
    TIMER1_TAILR_R = period - 1;        // Load period value (counts down from this value)
    TIMER1_TAPR_R = PRESCALE;           // Set prescaler
    Timer1A_Flag = 0;                   // Clear flag before starting
  	TIMER1_CTL_R |= TIMER_CTL_TAEN;			// Enable TIMER1A
}

void GPTM_STOP(void) {
    TIMER1_CTL_R &= ~TIMER_CTL_TAEN;    // Disable TIMER1A
}

void GPTM_1_n_US(uint8_t delay) {
    TIMER1_TAILR_R = (delay * ONE_MICRO_SECOND) - 1;  // (ILR) Reload value for delay in microseconds/ 1 delay = 16 cycles 
    TIMER1_TAPR_R = 0;                      // Prescale value
    Timer1A_Flag = 0;                       // Reset flag before starting
    TIMER1_CTL_R |= TIMER_CTL_TAEN;         // Enable TIMER1A

    while (Timer1A_Flag == 0) {             // Wait for the interrupt to set the flag
    }

    TIMER1_CTL_R &= ~TIMER_CTL_TAEN;        // Disable TIMER1A after timeout
}


uint32_t GPTM_ELAPSED(void) {
    return (uint32_t)(TIMER1_TAILR_R - TIMER1_TAR_R) * (TIMER1_TAPR_R + 1); 
	// Initial reload value - timer value * prescale -1
}
