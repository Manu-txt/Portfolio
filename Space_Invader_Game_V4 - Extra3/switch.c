// switches.c
// Runs on TM4C123, starter file for space invader game
// Min He
// November 15, 2022

#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "switch.h"

#define PORTF_UNLOCK						0x4C4F434B
#define EN_PORTF_INTR (1 << 30)  // Enable interrupt for Port F in NVIC

// Initialize the onboard two switches.
void Switch_Init(void){
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;	// Activate F clocks
	while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOF)==0){};
	GPIO_PORTF_LOCK_R = PORTF_UNLOCK; 	// unlock GPIO Port F
  GPIO_PORTF_CR_R |= SW_MASK;         		// allow changes to PF4,0
  GPIO_PORTF_DIR_R &= ~SW_MASK;    			// (c) make PF4,0 in (built-in button)
  GPIO_PORTF_AFSEL_R &= ~SW_MASK;  			//     disable alt funct on PF4,0
  GPIO_PORTF_DEN_R |= SW_MASK;     			//     enable digital I/O on PF4,0
  GPIO_PORTF_PCTL_R &= ~0x000F000F; 	//  configure PF4,0 as GPIO
  GPIO_PORTF_AMSEL_R &= ~SW_MASK;  			//     disable analog functionality on PF4,0
  GPIO_PORTF_PUR_R |= SW_MASK;     			//     enable weak pull-up on PF4,0
  GPIO_PORTF_IS_R &= ~SW_MASK;     			// (d) PF4,PF0 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~SW_MASK;    			//     PF4,PF0 is not both edges
  GPIO_PORTF_IEV_R |= SW_MASK;    			//     PF4,PF0 rising edge event
  GPIO_PORTF_ICR_R = SW_MASK;      			// (e) clear flags 4,0
  GPIO_PORTF_IM_R |= SW_MASK;      			// (f) arm interrupt on PF4,PF0
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00400000; 
  NVIC_EN0_R |= EN_PORTF_INTR;      // (h) enable interrupt 30 in NVIC
}
