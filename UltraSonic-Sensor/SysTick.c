// SysTick.c
// Runs on LM4F120/TM4C123
/*
``Class: CECS 347 Embedded Systems 2
``Group 5
Project 1
Filename: Systick.c
Objective: provide functions that initialize the systick module
Professor: Min He
01/29/2025
 */
#include "tm4c123gh6pm.h"
#include "SysTick.h"
#include <stdint.h>
#define Clear 0
#define Triggered 1

volatile unsigned long Tick_Flag = Clear; // Global flag

// Initialize SysTick with interrupt enabled and running at bus clock.
void SysTick_Init(void){
 NVIC_ST_CTRL_R = 0; // disable SysTick during setup
 NVIC_ST_CTRL_R = NVIC_ST_CTRL_CLK_SRC | NVIC_ST_CTRL_INTEN; // enable interrupts
}
// Start periodic interrupts with given period
void SysTick_Start(unsigned long period){
 NVIC_ST_RELOAD_R = period - 1; // reload value for desired period
 NVIC_ST_CURRENT_R = Clear; // clear current counter
 NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE;// start counting
}
// Stop SysTick interrupts
void SysTick_Stop(void){
 NVIC_ST_CTRL_R &= ~NVIC_ST_CTRL_ENABLE; // disable counter
}
// SysTick ISR - just sets the flag


//void SysTick_Wait1us(uint8_t delay){
//  NVIC_ST_CTRL_R = 0;
//  NVIC_ST_RELOAD_R = delay*ONE_MICRO_SECOND-1; // number of counts to wait
//  NVIC_ST_CURRENT_R = 0; // any value written to CURRENT clears
//	NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE+NVIC_ST_CTRL_CLK_SRC;
//  while((NVIC_ST_CTRL_R&NVIC_ST_CTRL_COUNT)==0); // wait for count flag
//  NVIC_ST_CTRL_R = 0;
//}