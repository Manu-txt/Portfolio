// SmartHouse.c
// Runs on TM4C123
// Starter for CECE346 Project 3 Smart House with a stepper motor controlled garage door.
// by Min He, 7/20/2024

// Group 4: Sheesh Dhawan, Kevin Zelada, Emmanuel Velazquez, Zoe Wright
// Description: A garage door that will full open when it senses a car
//							approaching. As it opens it will warn the user with a
//							blinking LED as well as notify the user when the door is
//							opened or closed. A FSM will determine the rotating of 
//							the stepper while and a sensor will be used to determine
//							if a car is approaching with the option of also opening 
//							and closing the garage with a switch.

// Garage door stepper motor/sensor connections
// PD0 connected to driver for stepper motor coil A/In1
// PD1 connected to driver for stepper motor coil A'/In2
// PD2 connected to driver for stepper motor coil B/In3
// PD3 connected to driver for stepper motor coil B'/In4
// PD6 connected to IR sesnsor output

#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "Garage.h"
#include "SysTick.h"
#include <stdbool.h>

extern void DisableInterrupts(void);
extern void EnableInterrupts(void);
extern void WaitForInterrupt(void);

// Constances for garage door status
enum Garage_Status {CLOSED,OPEN};
enum Garage_Status garage_status;

// Constances for car status:
// SAME means does not move across the 15cm distance.
// MOVING_OUT means moving from <15cm to >15cm
// MOVING_IN means moving from >15cm to <15cm
enum Car_Status {SAME, MOVING_OUT,MOVING_IN};

// flag shared with main() and PORTF handler
bool button_pressed = false;

enum Car_Status car_status;

int main(void){
	DisableInterrupts();
  Garage_Init();
  SysTick_Init();
	garage_status = CLOSED;
	car_status = SAME;
	EnableInterrupts();
	
  while(1){	
		if (garage_status == CLOSED) {
			// If a car is moving into the garage or the gragage button (sw2) is pressed
			// open the garage door
			// reset flags: car_status, button_pressed, garage_status
			if ((car_status == MOVING_IN) | button_pressed) {
				car_status = SAME;
		    Open_Garage();
				button_pressed = false;
				garage_status = OPEN;
			}
		}
		else {
			//If a car is moving out of the garage or the gragage button (sw2) is pressed
			//close the garage door
			//reset flags: car_status, button_pressed, garage_status
			if ((car_status == MOVING_OUT) | button_pressed) {
				car_status = SAME;
		    Close_Garage();
				button_pressed = false;
				garage_status = CLOSED;
			}
		}
	}
}

void GPIOPortF_Handler(void){
	// simple debouncing code: generate 20ms to 30ms delay
	for (uint32_t time=0;time<800000;time++) {}

	// take care of acknowledge and button_pressed flag here
	GPIO_PORTF_ICR_R = SW1_MASK | SW2_MASK;
		
	button_pressed = true;
}

void GPIOPortD_Handler(void) 
{
	// simple debouncing code: generate 20ms to 30ms delay
	for (uint32_t time=0;time<800000;time++) {}

	// take care of acknowledge and car_status flag here
	GPIO_PORTD_ICR_R |= SENSOR_MASK;
		
	if (SENSOR_MASK & SENSOR){
		car_status = MOVING_OUT;
	}
	else{
		car_status = MOVING_IN;
	}
}
