// Garage.h
// Runs on TM4C123
// Solution for CECE346 Project 3.
// by Min He, 7/20/2024

#include <stdint.h>

// Sensor port defenition
#define SENSOR 						(*((volatile uint32_t *)0x40007100)) // PD6
#define SENSOR_MASK  			0x40  // bit position for SENSOR: PD6
#define SW1_MASK  			0x10  // bit position for switch1: PF4
#define SW2_MASK  			0x01  // bit position for switch2: PF0

#define SYSCTL_RCGCGPIO_PORTD 	0x08
#define SYSCTL_RCGCGPIO_PORTF		0x20
#define EN_PORTF_INTR						0x40000000
#define EN_PORTD_INTR						0x08
#define LED_MASK 								0x0E
#define SWITCH_MASK							0x11
#define PORTF_UNLOCK						0x4C4F434B

// Initialize Stepper interface
void Garage_Init(void);
void Open_Garage(void);
void Close_Garage(void);

