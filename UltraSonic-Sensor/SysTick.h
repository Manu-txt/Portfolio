// SysTick.h
// Runs on LM4F120/TM4C123
/*
``Class: CECS 347 Embedded Systems 2
``Group 5
Project 1
Filename: Systick.h
Objective: provide functions that initialize the systick module
Professor: Min He
01/29/2025
 */
 #include <stdint.h>
	
	#define LED      (*((volatile uint32_t *) 0x40025038))  // PORTF, pin: 1,2,3
  #define BLINK_FREEQUENCY          4000000     // number of machine cycles to generate 1us delay for 16MHz system clock

// Initialize SysTick with interrupt enabled and running at bus clock.
void SysTick_Init(void);
// Start periodic interrupts with given period (in bus clock cycles)
void SysTick_Start(unsigned long period);
// Stop SysTick interrupts
void SysTick_Stop(void);
// Global flag to indicate when 250ms has passed
extern volatile unsigned long Tick_Flag;
void SysTick_Wait1us(uint8_t delay);
