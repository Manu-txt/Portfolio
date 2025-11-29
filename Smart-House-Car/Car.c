// Car.c
// Runs on TM4C123
// Starter program for CECS 346 Project 3
// 
// November 27, 2024
// by Min HE

// Group 4: Sheesh Dhawan, Kevin Zelada, Emmanuel Velazquez, Zoe Wright
// Description: Autonomous car that will move forard and then left after 
// 							a push of a button. Using FSM to control the direction of 
//							the left and right wheel while also avoiding collision with
//							the garage door that it will be parking in using a sensor to 
//							determine if it is safe.

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
#include "SysTick.h"
#include "Car.h"

// TODO: bit addresses for the two stepper motor control pins
#define LEFT_STEPPER   		(*((volatile uint32_t *)0x4000703C))  // PORT D, pin: 0,1,2,3
#define RIGHT_STEPPER  		(*((volatile uint32_t *)0x4002403C))  // PORT E, pin: 0,1,2,3
#define SW_MASK        		0x11  // bit positions for switch 1&2: PF0, PF4
#define HUNDRED_STEPS     18       // each step moves 0.18 degree, 100 steps moves 18 degree
#define HIGHEST_SPEED 		2*T1ms   // fastest speed the stepper can move
#define FASTER_SPEED			5*T1ms
#define STANDARD_SPEED		10*T1ms  // stepper motor speed: output every 10ms, frequency for the stepper motor is 100Hz.
#define CURRENT_SPEED 		HIGHEST_SPEED  	// fastest speed the stepper can move
#define PORTF_PRI_BITS 	  0UL      // bit positions for PORTF priority setting
#define PORTF_INT_PRI     5UL      // PORTF priority value

enum states {STEP1, STEP2, STEP3, STEP4};
enum Dir {FORWARD,BACKWARD,LEFTTURN, RIGHTTURN};

struct State{
  uint8_t Left;     // Output
  uint8_t Right;    // Output
  uint8_t Next[2];  // CW/CCW Next[0]->CW, Next[1]->CCW
};
typedef const struct State StateType;

StateType fsm[4]={
	{3,  3,  {STEP2, STEP4}}, // state 0, PD3-0:0011 CW next state->1, CCW next state->3
  {9,  6,  {STEP3,STEP1}},  // state 1, PD3-0:0110
  {12, 12, {STEP4,STEP2}},  // state 2, PD3-0:1100
  {6,  9,  {STEP1,STEP3}}   // state 3, PD3-0:1001
};

uint8_t s, l_s, r_s; // current state

// Stop car movement
void Stop_Car(void) {
	LEFT_STEPPER = 0; // step motor
  RIGHT_STEPPER = 0; // step motor
	s = FORWARD;
  l_s = FORWARD; 
	r_s = FORWARD;
}

// Move forward, speed is the systick reload value to control time to wait after each step
// each step moves 0.18 degree: one complete circle is 360 degrees
void Step_Forward(uint16_t degree){
	uint16_t i,n=(100*degree)/HUNDRED_STEPS;
	
	for (i=0;i<n;i++) {
		LEFT_STEPPER = fsm[s].Left; // step motor
		RIGHT_STEPPER = fsm[s].Left; // step motor
		SysTick_Wait(CURRENT_SPEED);
		s = fsm[s].Next[FORWARD];
	}	
}

// Move backward, speed is the systick reload value to control time to wait after each step
// each step moves 0.18 degree: one complete circle is 360 degrees
void Step_Backward(uint16_t degree){
		uint16_t i,n=(100*degree)/HUNDRED_STEPS;
	
	for (i=0;i<n;i++) {
		LEFT_STEPPER = fsm[s].Left; // step motor
		RIGHT_STEPPER = fsm[s].Left; // step motor
		SysTick_Wait(CURRENT_SPEED); // blind-cycle wait
		s = fsm[s].Next[BACKWARD];
	}
}

// Turn right: left wheel move forward(CCW), right wheel move backward(CCW)
void Step_Right(uint16_t degree){	
	uint16_t i,n=(100*degree)/HUNDRED_STEPS;
	
	for (i=0;i<n;i++) {
		LEFT_STEPPER = fsm[l_s].Right; // step motor
		RIGHT_STEPPER = fsm[r_s].Left; // step motor
		SysTick_Wait(CURRENT_SPEED);
		r_s = fsm[r_s].Next[BACKWARD]; // right wheel move backward(CCW)
		l_s = fsm[l_s].Next[BACKWARD];  // left wheel move forward(CCW)
	}
}

// Turn left: left wheel move backward(CW), right wheel move forward(CW)
void Step_Left(uint16_t degree){	
	uint16_t i,n=(100*degree)/HUNDRED_STEPS;
	
	for (i=0;i<n;i++) {
		LEFT_STEPPER = fsm[l_s].Right; // step motor
		RIGHT_STEPPER = fsm[r_s].Left; // step motor
		SysTick_Wait(CURRENT_SPEED);
		r_s = fsm[r_s].Next[FORWARD]; 
		l_s = fsm[l_s].Next[FORWARD];
	}
}

// Initialize both stepper motors and the IR sensor
// Setup IR sensor with both edge interrupt enabled
void Stepper_Sensor_Init(void){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_PORTD;
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_PORTE;
	
	while((SYSCTL_RCGCGPIO_R & SYSCTL_RCGCGPIO_PORTE) != SYSCTL_RCGCGPIO_PORTE); // activate port E clock
	while((SYSCTL_RCGCGPIO_R & SYSCTL_RCGCGPIO_PORTD) != SYSCTL_RCGCGPIO_PORTD); // activate port D clock
	
	// Stepper Init, LEFT_STEPPER
	GPIO_PORTD_AMSEL_R &= ~STEPPER_MASK;	//	Disable analog function
	GPIO_PORTD_PCTL_R &= ~0x0000FFFF;			// Configure GPIO 
	GPIO_PORTD_DEN_R |= STEPPER_MASK;			// Enable digital I/O
	GPIO_PORTD_DIR_R |= STEPPER_MASK;			// Output
	GPIO_PORTD_DR8R_R |= STEPPER_MASK; 		// Enable 8mA drive
	GPIO_PORTD_AFSEL_R &= ~STEPPER_MASK;	// Disable alternative functions
	
	// Stepper Init, RIGHT_STEPPER
	GPIO_PORTE_AMSEL_R &= ~STEPPER_MASK;	//	Disable analog function
	GPIO_PORTE_PCTL_R &= ~0x0000FFFF;			// Configure GPIO 
	GPIO_PORTE_DEN_R |= STEPPER_MASK;			// Enable digital I/O
	GPIO_PORTE_DIR_R |= STEPPER_MASK;			// Output
	GPIO_PORTE_DR8R_R |= STEPPER_MASK; 		// Enable 8mA drive
	GPIO_PORTE_AFSEL_R &= ~STEPPER_MASK;	// Disable alternative functions
	
	// Sensor Init
	GPIO_PORTD_AMSEL_R |= SENSOR_MASK;		
	GPIO_PORTD_PCTL_R &= ~0x0F000000;
	GPIO_PORTD_DIR_R &= ~ SENSOR_MASK;
	GPIO_PORTD_AFSEL_R &= ~SENSOR_MASK;
	GPIO_PORTD_DEN_R |= SENSOR_MASK;
	
	// Setting up Interrupt for sensor
	GPIO_PORTD_IS_R &= ~SENSOR_MASK; // Edge triggered
	GPIO_PORTD_IBE_R |= SENSOR_MASK;	// Both edges
	GPIO_PORTD_ICR_R |= SENSOR_MASK;	// clear register
	GPIO_PORTD_IM_R |= SENSOR_MASK;	// arm register
	NVIC_PRI0_R = (NVIC_PRI0_R & 0x1FFFFFFF) | 0x60000000;
	NVIC_EN0_R |= EN_PORTD_INTR;
}

// Initialize onboard two switches with interrupt enabled
void Sw_Init(void){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_PORTF;
	while ((SYSCTL_RCGCGPIO_R & SYSCTL_RCGCGPIO_PORTF) != SYSCTL_RCGCGPIO_PORTF);
	
	// Unlocking PortF
	GPIO_PORTF_LOCK_R = PORTF_UNLOCK;	
	GPIO_PORTF_CR_R |= SWITCH_MASK;
		
	// Switches
	GPIO_PORTF_AMSEL_R &= ~SWITCH_MASK; // Disable analog functions
	GPIO_PORTF_PCTL_R &= ~0x000F000F; 	// Configure GPIO
	GPIO_PORTF_DEN_R |= SWITCH_MASK;		// Enable GPIO ports
	GPIO_PORTF_DIR_R &= ~SWITCH_MASK;		// Input
	GPIO_PORTF_AFSEL_R &= ~SWITCH_MASK;	// Disable alternavtive functions
	GPIO_PORTF_PUR_R |= SWITCH_MASK;
	
	// Setting up Interrupt for Switches
	GPIO_PORTF_IS_R &= ~SWITCH_MASK;		// Edge interrupt
	GPIO_PORTF_IBE_R &= ~SWITCH_MASK;		// Use GPIOIEV
	GPIO_PORTF_IEV_R |= SWITCH_MASK;		// Falling edge triggered
	GPIO_PORTF_ICR_R |= SWITCH_MASK;		// Clear flag
	GPIO_PORTF_IM_R |= SWITCH_MASK;			// arm intterrupt
	NVIC_PRI7_R = (NVIC_PRI7_R&0xFF1FFFFF) | 0x00800000;
	NVIC_EN0_R |= EN_PORTF_INTR; // Enable interrupt 30 in NVIC
}

void Car_Init(void) {
	Stepper_Sensor_Init();
	Sw_Init();
}