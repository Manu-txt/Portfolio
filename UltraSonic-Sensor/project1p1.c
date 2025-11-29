// SysTickTestMain.c
// Runs on LM4F120/TM4C123
// Test the SysTick functions by activating the PLL, initializing the
/*
``Class: CECS 347 Embedded Systems 2
``Group 5
Project 1
Filename: Project1P1.c
Professor: Min He
01/29/2025
 */
#include "tm4c123gh6pm.h"
#include "SysTick.h"
#include "PLL.h"
#define RED_LED (*((volatile unsigned long *)0x40025008))
#define RED_LED_MASK 0x02 // bit pisition for onboard red[BLUE] LED
#define PFTwo 0x00000F00
#define Period250MS 4000000
#define Cleared 0
void PORTF_Init(void);
int main(void){
 PORTF_Init(); // initialize LED pins
 PLL_Init(); // set system clock to 50 MHz
 SysTick_Init(); // initialize SysTick timer with sinterrupts
 SysTick_Start(Period250MS); // start SysTick with 250ms period

 while(1){
 // Check if 250ms has passed
 if(Tick_Flag){
 RED_LED ^= RED_LED_MASK; // toggle red LED
 Tick_Flag = Cleared; // clear the flag
 }
 }
}
void PORTF_Init(void){
 SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5; // activate port F
 while ((SYSCTL_RCGCGPIO_R & SYSCTL_RCGCGPIO_R5) == 0){};

 GPIO_PORTF_DIR_R |= RED_LED_MASK; // make PF1 out
 GPIO_PORTF_AFSEL_R &= ~RED_LED_MASK; // disable alt funct
 GPIO_PORTF_DEN_R |= RED_LED_MASK; // enable digital I/O
 GPIO_PORTF_PCTL_R &= ~PFTwo; // configure as GPIO
 GPIO_PORTF_AMSEL_R &= ~RED_LED_MASK; // disable analog functionality
}