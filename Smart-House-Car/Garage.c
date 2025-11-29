// Garage.c
// Runs on TM4C123	
// Solution for CECE346 Project 3.
// by Min He, 7/20/2024

// PD0 connected to driver for stepper motor coil A/In1
// PD1 connected to driver for stepper motor coil A'/In2
// PD2 connected to driver for stepper motor coil B/In3
// PD3 connected to driver for stepper motor coil B'/In4
// PD6 connected to IR sesnsor output

#include "tm4c123gh6pm.h"
#include "Garage.h"
#include <stdint.h>
#include "SysTick.h"

// TODO: bit address definitions for stepper motor interface and three onboard LEDs
#define STEPPER  (*((volatile uint32_t *) 0x4000703C))  // PORT D, pin: 0,1,2,3, used to control garage door
#define LED      (*((volatile uint32_t *) 0x40025038))  // PORTF, pin: 1,2,3
#define SWITCHES (*((volatile uint32_t *) 0x40025044))  // PORTF, pin: 0, 4

// TODO: define value for the three onboard LEDs
#define GREEN 					0x08  // PF3 connects to green LED
#define RED 						0x02  // PF1 connects to red LED
#define BLUE 						0x04  // PF2 connects to blue LED

struct State{
  uint8_t Out;     // Output
  uint8_t Next[2]; // CW/CCW Next[0]->CW, Next[1]->CCW
};
typedef const struct State StateType;

enum Stepper_Dir {CLOCKWISE, COUNTERCLOCKWISE};
enum FSM_STATES {STEP1,STEP2,STEP3,STEP4}; 

// TODO: constant definitions
#define DEGREE_HUNDRED_STEPS 		18  // each step moves 0.18 degree, 100 steps moves 18 degree
#define HALF_SEC      					500*T1ms 	//systick reload value for 0.5s
#define T1ms 										16000UL    // Systick reload value to generate 1ms delay, assumes using 16 MHz Clock.
#define CURRENT_SPEED 					FASTER_SPEED  	// fastest speed the stepper can move
#define HIGHEST_SPEED 					2*T1ms  	// fastest speed the stepper can move
#define FASTER_SPEED						5*T1ms
#define STANDARD_SPEED					10*T1ms  	// stepper motor speed: output every 10ms, frequency for the stepper motor is 100Hz.
#define GARAGE_STEPS      			720				// number of steps to fully open/close the garage door
#define STEPPER_MASK  					0x0F       // bit positions for stepper motor: PD3-0
#define HALF_S_COUNT						50UL     // number of stepper motor steps to generate 0.5s. Ex: HALF_SEC/CURRENT_SPEED = 50
#define PORTF_PRI_BITS 	  			(0)
#define PORTF_INT_PRI     			(0)

StateType fsm[4]={
	// index 0: state 0,state goes from 0 to 3,  output 1100,
	// if next state index is 0: move clockwise, next state for clockwise movement is 1
	// CW state transition is: 0->1->2->3 then repeat, output transition: 3->6->12->9
	// CCW state transition is: 0->3->2->1 then repeat, output transition: 3->9->12->6
	{3,  {STEP2, STEP4}}, // state 0, PD3-0:0011 CW next state->1, CCW next state->3
  {6,  {STEP3,STEP1}}, // state 1, PD3-0:0110
  {12, {STEP4,STEP2}}, // state 2, PD3-0:1100
  {9,  {STEP1,STEP3}}  // state 3, PD3-0:1001
};
	
// Move clockwise, speed is controlled by the systick reload value 
// each step moves 0.18 degree: each 100 steps move 18 degree.
// GARAGE_STEPS determines degrees to open/close the garage door.
// Flash LED here every 0.5 second.
void Open_Garage(void){
  uint8_t s=STEP1; // current state
	uint32_t i,n=(100*GARAGE_STEPS)/DEGREE_HUNDRED_STEPS;

	LED = RED; 
	
	// TODO: make enough steps to fully open the garage door
	// Flash red LED every 0.5 second while move the stepper motor: every 50 steps
	// inverse output for red LED
		for(int i = 0; i < n; ++i){
			STEPPER = fsm[s].Out;
			SysTick_Wait(CURRENT_SPEED);
			s = fsm[s].Next[COUNTERCLOCKWISE];
		}
	
	// Change LED color to indicate the garage door is fully opened
	LED = BLUE;
	
	// wait half second after the garage door is fully opened.
  SysTick_Wait(HALF_SEC);	
}

// Move counterclockwise, speed is the systick reload value to control time to wait after each step
// Each step moves 0.18 degree: one complete circle is 360 degrees
void Close_Garage(void){
  uint8_t s=STEP1; // current state
	uint16_t i,n=(100*GARAGE_STEPS)/DEGREE_HUNDRED_STEPS;

	LED = RED; 
	
	// TODO: make enough steps to fully open the garage door
	// Flash red LED every 0.5 second while move the stepper motor.
		for(int i = 0; i < n; ++i){
			STEPPER = fsm[s].Out;
			SysTick_Wait(CURRENT_SPEED);
			s = fsm[s].Next[CLOCKWISE];
		}
	
	// Change LED color to indicate the garage door is fully closed
	LED = GREEN;
	
	// wait half second after the garage door is fully closed.
  SysTick_Wait(HALF_SEC);	
}

// Initialize Stepper interface: including
// Four pins to drive the stepper motor driver
// One pin to collect information from the sensor: enable both edge interrupt for this pin.
void Stepper_Sensor_Init(void){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_PORTD;
	
	while((SYSCTL_RCGCGPIO_R & SYSCTL_RCGCGPIO_PORTD) != SYSCTL_RCGCGPIO_PORTD);
	
	// Stepper Init
	GPIO_PORTD_AMSEL_R &= ~STEPPER_MASK;	//	Disable analog function
	GPIO_PORTD_PCTL_R &= ~0x0000FFFF;			// Configure GPIO 
	GPIO_PORTD_DEN_R |= STEPPER_MASK;			// Enable digital I/O
	GPIO_PORTD_DIR_R |= STEPPER_MASK;			// Output
	GPIO_PORTD_DR8R_R |= STEPPER_MASK; 		// Enable 8mA drive
	GPIO_PORTD_AFSEL_R &= ~STEPPER_MASK;	// Disable alternative functions
	
	// Sensor Init
	GPIO_PORTD_AMSEL_R |= SENSOR_MASK;		
	GPIO_PORTD_PCTL_R &= ~0x0F000000;
	GPIO_PORTD_DIR_R &= ~ SENSOR_MASK;
	GPIO_PORTD_AFSEL_R &= ~SENSOR_MASK;
	GPIO_PORTD_DEN_R |= SENSOR_MASK;
	
	GPIO_PORTD_IS_R &= ~SENSOR_MASK;
	GPIO_PORTD_IBE_R |= SENSOR_MASK;
	GPIO_PORTD_ICR_R |= SENSOR_MASK;
	GPIO_PORTD_IM_R |= SENSOR_MASK;
	NVIC_PRI0_R = (NVIC_PRI0_R & 0x1FFFFFFF) | 0x60000000;
	NVIC_EN0_R |= EN_PORTD_INTR;
}

// Initialize PORTF for three LEDs and two switches.
// Enable interrupt for sw1 & sw2
// Initialize LED to red.
void SwLed_Init(void){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_PORTF;
	while ((SYSCTL_RCGCGPIO_R & SYSCTL_RCGCGPIO_PORTF) != SYSCTL_RCGCGPIO_PORTF);
	
	// Unlocking PortF
	GPIO_PORTF_LOCK_R = PORTF_UNLOCK;	
	GPIO_PORTF_CR_R |= SWITCH_MASK | LED_MASK;
	
	// LEDS
	GPIO_PORTF_AMSEL_R &= ~LED_MASK;	// disable analog function
	GPIO_PORTF_PCTL_R &= ~0x0000F0F0; // Configure GPIO
	GPIO_PORTF_DIR_R |= LED_MASK;			// Output
	GPIO_PORTF_AFSEL_R &= ~LED_MASK;	// No alternative funtions
	GPIO_PORTF_DEN_R |= LED_MASK;			// Enable digital pins
	
	LED = RED;	
	
	// Switches
	GPIO_PORTF_AMSEL_R &= ~SWITCH_MASK;
	GPIO_PORTF_PCTL_R &= ~0x000F000F;
	GPIO_PORTF_DEN_R |= SWITCH_MASK;
	GPIO_PORTF_DIR_R &= ~SWITCH_MASK;
	GPIO_PORTF_AFSEL_R &= ~SWITCH_MASK;
	GPIO_PORTF_IS_R &= ~SWITCH_MASK;
	GPIO_PORTF_IBE_R &= ~SWITCH_MASK;
	GPIO_PORTF_IEV_R |= SWITCH_MASK;
	GPIO_PORTF_ICR_R |= SWITCH_MASK;
	GPIO_PORTF_IM_R |= SWITCH_MASK;
	NVIC_PRI7_R = (NVIC_PRI7_R&0xFF1FFFFF) | 0x00800000;
	NVIC_EN0_R |= EN_PORTF_INTR;
}

void Garage_Init(void) {
	Stepper_Sensor_Init();
	SwLed_Init();
}

