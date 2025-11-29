// DragRace.c
// Starter file for CECS346 Project 2
// By Dr. Min He
// 3/21/2024

#include "tm4c123gh6pm.h"
#include <stdint.h> // C99 data types
#include <stdbool.h> // provides boolean data type
#include "PLL.h"
#include "SysTick.h"
#include "Sensors_Lights.h"

// Function Prototypes
// External functions from startup.s
extern void DisableInterrupts(void); // Disable interrupts
extern void EnableInterrupts(void);  // Enable interrupts
extern void WaitForInterrupt(void);  // Go to low power mode while waiting for the next interrupt

// Functions defined in this file.
void System_Init(void);

//TODO: Bit addresses for two sensors, 8 color lights, and one reset button 
#define SENSORS 					(*((volatile unsigned long *) 0x40004300)) // bit addresses for 2 sensors, PortA: PA7-PA6
#define LIGHTS_PORTE      (*((volatile unsigned long *) 0x40024078)) // bit addresses for 4 Race Lights, PortE: PE4-PE1
#define LIGHTS_PORTD			(*((volatile unsigned long *) 0x4000703C)) // bit addresses for 4 Race Lights, PortD: PD3-PD0
#define RESET             (*((volatile unsigned long *) 0x40005004)) // bit address for one reset button, PortB: PB4
	
// FSM 
struct State { 
	uint8_t Out;
	uint8_t Time;     // multiple of 0.5 second
	uint8_t Next[4];
};

typedef const struct State STyp;

// Constant definitions for FSM outputs

// TODO: define reload value for half second
#define HALF_SEC   12500000

//TODO: assign a value to all states in Drag Race FSM
enum DragRace_states {INIT, WAITFORSTAGE, COUNTY_1, COUNTY_2, GO, FSL, FSR,
											FSB, WINLEFT, WINRIGHT, WINBOTH};

//TODO: Define Drag Race FSM
STyp DragRace_FSM[] = {
	{0xFF, 2, {WAITFORSTAGE, WAITFORSTAGE, WAITFORSTAGE, WAITFORSTAGE}},
	{0x00, 1, {WAITFORSTAGE, WAITFORSTAGE, WAITFORSTAGE, COUNTY_1}},
	{0x88, 1, {FSB, FSL, FSR, COUNTY_2}},
	{0x44, 1, {FSB, FSL, FSR, GO}},
	{0x22, 1, {WINBOTH, WINLEFT, WINRIGHT}},
	{0x10, 2, {WAITFORSTAGE, WAITFORSTAGE, WAITFORSTAGE, WAITFORSTAGE}},
	{0x01, 2, {WAITFORSTAGE, WAITFORSTAGE, WAITFORSTAGE, WAITFORSTAGE}},
	{0x11, 2, {WAITFORSTAGE, WAITFORSTAGE, WAITFORSTAGE, WAITFORSTAGE}},
	{0x20, 2, {WAITFORSTAGE, WAITFORSTAGE, WAITFORSTAGE, WAITFORSTAGE}},
	{0x02, 2, {WAITFORSTAGE, WAITFORSTAGE, WAITFORSTAGE, WAITFORSTAGE}},
	{0x22, 2, {WAITFORSTAGE, WAITFORSTAGE, WAITFORSTAGE, WAITFORSTAGE}},
};

uint8_t S;  // current state index
uint8_t Input;
bool timesup;
bool reset;  // flag to reset the system, set by the reset button located at breadboard, not the launchpad reset button.
	
	
int main(void){
	System_Init();
		
  while(1){
		
		S = ;
		reset = ;
		input = ;
		
		while (!reset) {
			// TO Do: take care of FSM outputs and time in state.
			LIGHTS_PORTE = DragRace_FSM[S].Out & 0x1E;
			LIGHTS_PORTD = (DragRace_FSM[S].Out & 0x0A) >> 8;
			SysTick_Start(DragRace_FSM[S].Time*HALF_SEC);
			while((!timesup)&&(!reset)){
			  WaitForInterrupt();
			}
			timesup=false;
			S = DragRace_FSM[S].Next[Input];
		}
  }
}

// Initialize the following hardware modules: PLL, Sensors, Reset button, Lights, SysTick timer
// Initialize the following golbal variables: timesup for FSM state time, reset for reset the whole system
void System_Init(void) {
	DisableInterrupts();
	PLL_Init();
  Sensors_Init(); 
	Reset_Init(); 
	Lights_Init();
	SysTick_Init(); 
  // TODO: assign Initial state, reset global variables: timesup and reset 
  S = DragRace_FSM[INIT].Out; 
	timesup = false;
	reset = false;
	Input = WAITFORSTAGE;
	EnableInterrupts();
}

// Interrupt handler for the two sensors: take care of Input here 
void GPIOPortA_Handler(void){
		GPIO_PORTA_ICR_R = SENSORS; // acknowledge flag
		Input = WAITFORSTAGE;
}

// Interrupt handler for reset button: Resets the board to the initalization state: 
// take care of global variable reset, timesup and S 
// stop systick timer
void GPIOPortB_Handler(void){
	GPIO_PORTB_ICR_R = RESET; // acknowledge flag
		reset = true;
		S = DragRace_FSM[INIT].Out;
		timesup = false;
}

// Systick interrupt handler: stop systick timer and take care of timesup 
void SysTick_Handler(void){
	NVIC_ST_CTRL_R &= ~NVIC_ST_CTRL_ENABLE; 
	timesup = true;
}

