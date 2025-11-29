// PLL.c
// Runs on TM4C123
// Starter file for CECS346 Project 2
// By Dr. Min He
 
#include "PLL.h"
#include "tm4c123gh6pm.h"

// The #define statement SYSDIV2 in PLL.h
// initializes the PLL to the desired frequency.

#define SYSDIV2 15
// bus frequency is 400MHz/(SYSDIV2+1) = 400MHz/(15+1) = 25 MHz
// see the table at the end of this file

// configure the system to get its clock from the PLL
void PLL_Init(void){
	// 0) User RCC2
	SYSCTL_RCC2_R |= SYSCTL_RCC2_USERCC2;
	// 1) bypass PLL
	SYSCTL_RCC2_R |= SYSCTL_RCC2_BYPASS2;
	// 2) selet the crystal value and oscillator source
	SYSCTL_RCC_R &= ~SYSCTL_RCC_XTAL_M;
	SYSCTL_RCC_R += SYSCTL_RCC_XTAL_16MHZ;
	SYSCTL_RCC2_R &= ~SYSCTL_RCC2_OSCSRC2_M;
	SYSCTL_RCC2_R += SYSCTL_RCC2_OSCSRC2_MO;
	// 3) activate PLL be clearing PWRDN
	SYSCTL_RCC2_R &= ~SYSCTL_RCC2_PWRDN2;
	// 4) set desired system divider and systems dividers least significant bit
	SYSCTL_RCC2_R |= SYSCTL_RCC2_DIV400;
	SYSCTL_RCC2_R = (SYSCTL_RCC2_R & ~0x1FC00000) + (SYSDIV2 << 22);
	// 5) wait for the PLL to lock by polling PLLRIS
	while((SYSCTL_RIS_R & SYSCTL_RIS_PLLLRIS) == 0){};
	// 6) enable use of PLL by clearing BYPASS
	SYSCTL_RCC2_R &= ~SYSCTL_RCC2_BYPASS2;
}
