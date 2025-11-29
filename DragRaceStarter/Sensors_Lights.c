// Sensors_Lights.c
// Starter file for CECS346 Project 2
// By Dr. Min He
#include "tm4c123gh6pm.h"
#include <stdint.h> // C99 data types
#include "Sensors_Lights.h"

// Constants
#define SYSCTL_RCGC2_PORTA			0x01
#define SYSCTL_RCGC2_PORTE			0x10
#define SYSCTL_RCGC2_PORTD			0x08
#define SYSCTL_RCGC2_PORTB			0x02
#define SENSOR_MASK						  0xA0
#define LIGHTS_PORTE_MASK				0x1E
#define LIGHTS_PORTD_MASK				0x0A
#define RESET_MASK							0x10

// Initialize the two sensors, enable both edge edge-triggered interrupt for both sensors, PortA: PA7-PA6
void Sensors_Init(void){ 
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_PORTA; // Activate clock A
	GPIO_PORTA_AMSEL_R &= ~SENSOR_MASK; // disable analog function
	GPIO_PORTA_DIR_R &= ~SENSOR_MASK;	// input
	GPIO_PORTA_DEN_R |= SENSOR_MASK; // enable digital pins
	GPIO_PORTA_PCTL_R  &= ~0xFF000000; // GPIO clear bit PCTL
	GPIO_PORTA_AFSEL_R &= ~SENSOR_MASK; // No alternative functions
	GPIO_PORTA_IS_R &= ~SENSOR_MASK; // Edge-sensitive
	GPIO_PORTA_IBE_R |= SENSOR_MASK; // Both edges
	GPIO_PORTA_ICR_R |= SENSOR_MASK; // Clear flag
	GPIO_PORTA_IM_R |= SENSOR_MASK; // Arm interrupt
	NVIC_PRI0_R  = (NVIC_PRI0_R & 0xFFFFFF1F) | 0x00000040; // Priority 2
	NVIC_EN0_R |= 0x01; // Enable interrupt 1
}

// Initialize the reset button: use level triggered interrupt, PortB: PB4
void Reset_Init(void){
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_PORTB; // Activate clock B
	GPIO_PORTB_AMSEL_R &= ~RESET_MASK; // disable analog function
	GPIO_PORTB_DIR_R &= ~RESET_MASK; // input
	GPIO_PORTB_DEN_R |= RESET_MASK; // enable digital pins
	GPIO_PORTB_PCTL_R &= ~0x000F0000; // GPIO clear bit PCTL
	GPIO_PORTB_AFSEL_R &= ~RESET_MASK; // No alternative functions
	
	GPIO_PORTB_IS_R |= RESET_MASK; // level sensitive
	GPIO_PORTB_IBE_R &= ~RESET_MASK; // user GPIOIEV
	GPIO_PORTB_IEV_R |= RESET_MASK; // high level triggered
	GPIO_PORTB_ICR_R |= RESET_MASK; // clear flag
	GPIO_PORTB_IM_R |= RESET_MASK; // arm interrupt
	NVIC_PRI0_R = (NVIC_PRI0_R & 0xFFFF1FFF) | 0x00002000; // Priority 1
	NVIC_EN0_R |= 0x02;
}

// Initialize 8 LEDs, PortE: PE4-PE1 & PortD:PD3-PD0
void Lights_Init(void){
	// LIGHTS_1, PortE: PE4-PE1 
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_PORTE; // activate clock E
	GPIO_PORTE_AMSEL_R &= ~LIGHTS_PORTE_MASK; // disable analog functions
	GPIO_PORTE_DIR_R |= LIGHTS_PORTE_MASK; // output
	GPIO_PORTE_DEN_R |= LIGHTS_PORTE_MASK; // enable digital pins
	GPIO_PORTE_PCTL_R &= ~0x000FFFF0; // GPIO clear bit PCTL
	GPIO_PORTE_AFSEL_R &= ~LIGHTS_PORTE_MASK; // no alternative functions
	
	// LIGHTS_2, PortD:PD3-PD0
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_PORTD; // activate clock D
	GPIO_PORTD_AMSEL_R &= ~LIGHTS_PORTD_MASK; // disable analog functions
	GPIO_PORTD_DIR_R |= LIGHTS_PORTD_MASK; // output
	GPIO_PORTD_DEN_R |= LIGHTS_PORTD_MASK; // enable digital pins
	GPIO_PORTD_PCTL_R &= ~0x0000FFFF; // GPIO clear bit PCTL
	GPIO_PORTD_AFSEL_R &= ~LIGHTS_PORTD_MASK; // no alternative functions
}
