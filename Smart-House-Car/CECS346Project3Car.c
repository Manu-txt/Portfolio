// CECS346Project3Car.c
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

// IR distance sensor
// PD6 connected to IR sesnsor output

#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "Car.h"
#include "SysTick.h"
#include "stdbool.h"	

extern void DisableInterrupts(void);
extern void EnableInterrupts(void);
extern void WaitForInterrupt(void);

bool trip_active; // flag for trip status
bool safe_to_move;// flag for whether the car can move forward or not
bool leaving_home;// flag for the car to know whether time to leave home or not.

int main(void){
	DisableInterrupts();
  Car_Init();
  SysTick_Init();
	EnableInterrupts();
	trip_active = false;
	
  while(1){
		WaitForInterrupt();
		while (!trip_active) {}
		
		// TODO: Initial flags 
		safe_to_move = true;
		leaving_home = false;
			
		// Start a trip with moving forward a fixed distance: wheels turn 720 degrees		
		Step_Forward(720); // 720  
		Stop_Car();
		SysTick_Wait(HALF_SEC);  // wait for 0.5s
		
		// Car turn left 90 degrees: left wheel move backward, right wheel move forward
		Step_Left(200);   
		Stop_Car();
		SysTick_Wait(HALF_SEC);  // wait for 0.5s
			
		// Move forward until the car sees the garage door
		while (safe_to_move) {
		  Step_Forward(5);   // number of steps here can be adjusted
		}
		
		// Stop the car, wait until the garage door is fully opened 
		// and out of the view of the car.
		Stop_Car();
		SysTick_Wait(HALF_SEC);  // wait for 0.5s	
    while (!safe_to_move) {}
		
    // Move forward 18 degree at a time 
		// until fully into the garage and sense the back wall.			
		while (safe_to_move) {
		  Step_Forward(5);   // number of steps here can be adjusted
		}
		
		Stop_Car();
		SysTick_Wait(HALF_SEC);
	
		// wait for sw2 to be pressed to start leaving home
		while (!leaving_home){}
						
		// Move backward 180 degrees
		Step_Backward(180);   // output every 10ms
		Stop_Car();
		SysTick_Wait(HALF_SEC);  // wait for 0.5s
	
		// Turn right 90 degrees: left wheel move forward, right wheel move backward
		Step_Right(200);   // output every 10ms
		Stop_Car();
		SysTick_Wait(HALF_SEC);  // wait for 0.5s		
			
		// Move forward for a fixed distance and end the trip.		
		Step_Forward(720);   
		Stop_Car();
		SysTick_Wait(HALF_SEC);  // wait for 0.5s

		// end of current trip
		trip_active = false;
  }
}

// TODO: Taking care of two flags: trip_active & leaving_home
void GPIOPortF_Handler(void){
	// simple debouncing code: generate 20ms to 30ms delay
	for (uint32_t time=0;time<800000;time++) {}
		
	if ((GPIO_PORTF_RIS_R & SW1_MASK) == SW1_MASK){
		trip_active = true;
		//leaving_home = false;
	}
	
	if((GPIO_PORTF_RIS_R & SW2_MASK) == SW2_MASK){
		//trip_active = false;
		leaving_home = true;
	}
	
	GPIO_PORTF_ICR_R = SW1_MASK | SW2_MASK; // Acknowledgement
}

// TODO: Taking care of one flag: safe_to_move
void GPIOPortD_Handler(void) 
{
	// simple debouncing code: generate 20ms to 30ms delay
	for (uint32_t time=0;time<800000;time++) {}
	
	if(!(SENSOR & SENSOR_MASK)){
		safe_to_move = false;
	}
	if(SENSOR & SENSOR_MASK){
		safe_to_move = true;
	}
	
	GPIO_PORTD_ICR_R = SENSOR_MASK; // Acknowledgement	
}
