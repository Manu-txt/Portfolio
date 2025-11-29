// Car.h
// Runs on TM4C123
// Starter program for CECS 346 Project 3
// 
// November 27, 2024
// by Min HE

// Left stepper motor connections
// PD0 connected to driver for stepper motor coil A/In1
// PD1 connected to driver for stepper motor coil A'/In2
// PD2 connected to driver for stepper motor coil B/In3
// PD3 connected to driver for stepper motor coil B'/In4

// right stepper motor connections
// PE0 connected to driver for stepper motor coil A/In1
// PE1 connected to driver for stepper motor coil A'/In2
// PE2 connected to driver for stepper motor coil B/In3
// PE3 connected to driver for stepper motor coil B'/In4

#include <stdint.h>

// TODO: Bit address for sensor data register pin
#define SENSOR 						(*((volatile uint32_t *)0x40007100)) // PD6
#define SENSOR_MASK  			0x40 	  // bit position for SENSOR: PD6
#define SW1_MASK  			  0x10  // bit position for switch1: PF4
#define SW2_MASK  			  0x01   // bit position for switch2: PF0

#define STEPPER_MASK  					0x0F       // bit positions for stepper motor: PD3-0
#define SYSCTL_RCGCGPIO_PORTD 	0x08
#define SYSCTL_RCGCGPIO_PORTE		0x10
#define SYSCTL_RCGCGPIO_PORTF		0x20
#define EN_PORTF_INTR						0x40000000
#define EN_PORTD_INTR						0x08
#define LED_MASK 								0x0E
#define SWITCH_MASK							0x11
#define PORTF_UNLOCK						0x4C4F434B

// Initialize the stepper motor car
void Car_Init(void);

// Stop car movement
void Stop_Car(void);

// Move forward, speed is the systick reload value to control time to wait after each step
// each step moves 0.18 degree: one complete circle is 360 degrees
void Step_Forward(uint16_t degree);

// Move backward, speed is the systick reload value to control time to wait after each step
// each step moves 0.18 degree: one complete circle is 360 degrees
void Step_Backward(uint16_t degree);

// Turn right, speed is the systick reload value to control time to wait after each step
// Parameter degree: angles the car turns.
void Step_Right(uint16_t degree);

// Turn left, speed is the systick reload value to control time to wait after each step
// Parameter degree: angles the car turns.
void Step_Left(uint16_t degree);

