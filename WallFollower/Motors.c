// PWM.c
// Runs on TM4C123
// Use PWM0/PB6 and PWM1/PB7 to generate pulse-width modulated outputs.
// Daniel Valvano
// March 28, 2014
// Modified by Min He, September 7, 2021

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014
  Program 6.7, section 6.3.2

 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
#include <stdint.h>
#include "Motors.h"
#include "tm4c123gh6pm.h"

#define PERIOD 16000				// Total PWM period

uint16_t curr_speed_idx = 0;
uint16_t speeds[] = {STOP, SPEED_35, SPEED_60, SPEED_80, SPEED_98};
#define NUM_OF_SPEEDS		5
	

// Wheel PWM connections: on PB6/M0PWM0:Left wheel, PB7/M0PWM0:Right wheel
void PWM_PB45_Init(void){
  // 1) Enable clock to Port B
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;
  while((SYSCTL_RCGC2_R & SYSCTL_RCGC2_GPIOB) == 0){};
  
  // 2) Configure PB4, PB5 for alternate function (PWM)
  GPIO_PORTB_AFSEL_R |= 0x30;   // bits 4,5
  GPIO_PORTB_PCTL_R  &= ~0x00FF0000; // clear PB4,PB5
  GPIO_PORTB_PCTL_R  |=  0x00440000; // PB4 => M0PWM2, PB5 => M0PWM3
	//GPIO_PORTB_PCTL_R  = (GPIO_PORTB_PCTL_R & ~0x00FF0000) | 0x00440000; // Set PB4,PB5 -> M0PWM2, M0PWM3
  GPIO_PORTB_DEN_R   |=  0x30;       
  GPIO_PORTB_AMSEL_R &= ~0x30; 
  
  // 3) Enable clock to PWM0
  SYSCTL_RCGCPWM_R |= 0x01;
  // 4) Disable any PWM clock division (if you want direct SysClk)
  //    If you want to divide the clock, remove this line and configure accordingly
  SYSCTL_RCC_R &= ~0x001E0000;   // clear PWM divider bits

  // 5) Configure PWM0, Generator 1 (for M0PWM2,3)
  //    Re-loading down-count mode
  PWM0_1_CTL_R = 0;         // turn-off generator 1 while configuring
  PWM0_1_GENA_R = 0xC8;     // low on LOAD, high on CMPA down
  PWM0_1_GENB_R = 0xC08;    // low on LOAD, high on CMPB down
  PWM0_1_LOAD_R = PERIOD - 1;  // e.g. 16000 for 1 kHz
  PWM0_1_CMPA_R = 0;        // duty = 0% at init
  PWM0_1_CMPB_R = 0;        // duty = 0% at init
  
  // 6) Start generator
  PWM0_1_CTL_R |= 0x01;     // enable generator 1
  
  // 7) Enable M0PWM2 (bit2) and M0PWM3 (bit3) outputs, but start them OFF
  //    We'll set the bits to 0 first, to ensure not driving until we do duty
  PWM0_ENABLE_R &= ~0x0C;   // disable PWM2,3 initially
  // If you want them always active, you can set them now:
  //PWM0_ENABLE_R |= 0x0C;
}

// Start left wheel
void Start_L(void) {
  PWM0_ENABLE_R |= 0x00000004;          // PB4/M0PWM2
}

// Start right wheel
void Start_R(void) {
  PWM0_ENABLE_R |= 0x00000008;          // enable PB5/M0PWM3
}

// Stop left wheel
void Stop_L(void) {
  PWM0_ENABLE_R &= ~0x00000004;          // PB4/M0PWM2
}

// Stop right wheel
void Stop_R(void) {
  PWM0_ENABLE_R &= ~0x00000008;          // enable PB5/M0PWM3
}

void Start_Both_Wheels(void){
	Start_R();
	Start_L();
}

void Stop_Both_Wheels(void) {
	Stop_L();
	Stop_R();
}

void Move_Forward(int L_Wheel, int R_Wheel){
	Set_L_Speed(L_Wheel);
	Set_R_Speed(R_Wheel);
	DIRECTION = FORWARD;
	Start_Both_Wheels();
}

void Move_Backward(int L_Wheel, int R_Wheel){
	Set_L_Speed(L_Wheel);
	Set_R_Speed(R_Wheel);
	DIRECTION = BACKWARD;
	Start_Both_Wheels();
}

void Left_Turn_Wall(int L_Wheel, int R_Wheel){
	Set_L_Speed(L_Wheel);
	Set_R_Speed(R_Wheel);
	DIRECTION = FORWARD;
	Start_Both_Wheels();
}

void Right_Turn_Wall(int L_Wheel, int R_Wheel){
	Set_L_Speed(L_Wheel);
	Set_R_Speed(R_Wheel);
	DIRECTION = FORWARD;
	Start_Both_Wheels();
}

void Left_Turn(int L_Wheel, int R_Wheel){
	Set_L_Speed(L_Wheel);
	Set_R_Speed(R_Wheel);
	DIRECTION = FORWARD;
	Start_Both_Wheels();
}

void Right_Turn(int L_Wheel, int R_Wheel){
	Set_L_Speed(L_Wheel);
	Set_R_Speed(R_Wheel);
	DIRECTION = FORWARD;
	Start_Both_Wheels();
}

void Left_Turn_Back(int L_Wheel, int R_Wheel){
	Set_L_Speed(SPEED_35);
	Set_R_Speed(R_Wheel);
	DIRECTION = BACKWARD;
	Start_Both_Wheels();
}

void Right_Turn_Back(int L_Wheel, int R_Wheel){
	Set_L_Speed(L_Wheel);
	Set_R_Speed(R_Wheel);
	DIRECTION = BACKWARD;
	Start_Both_Wheels();
}

void Left_Pivot(int L_Wheel, int R_Wheel){
	Set_L_Speed (L_Wheel);
	Set_R_Speed (R_Wheel);
	DIRECTION = LEFTPIVOT;
	Start_Both_Wheels();
}

void Right_Pivot(int L_Wheel, int R_Wheel){
	Set_L_Speed (L_Wheel);
	Set_R_Speed (R_Wheel);
	DIRECTION = RIGHTPIVOT;
	Start_Both_Wheels();
}

void Stop_Car(void){
	Stop_Both_Wheels();
}
// Set duty cycle for Left Wheel: PB6
void Set_L_Speed(unsigned long dutyL){
  PWM0_1_CMPA_R = dutyL - 1;             // 6) count value when output rises
}
// Set duty cycle for Right Wheel: PB7
void Set_R_Speed(unsigned long dutyR){
  PWM0_1_CMPB_R = dutyR - 1;             // 6) count value when output rises
}

// Initialize port E pins PE0-3 for output
// PE0-3 control directions of the two motors: PE3210:L/SLP,L/DIR,R/SLP,R/DIR
// Inputs: None
// Outputs: None
//void Dir_Init(void){
//	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE; //activate E clock
//	while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOE)!= SYSCTL_RCGC2_GPIOE){} //wait for clk
//	
//	GPIO_PORTE_AMSEL_R &= ~0x0F; //disable analog function
//	GPIO_PORTE_PCTL_R &= ~0x0000FFFF; //GPIO clear bit PCTL
//	GPIO_PORTE_DIR_R |= 0x0F; //PE0-3 output
//	GPIO_PORTE_AFSEL_R &= ~0x0F; //no alternate function
//	GPIO_PORTE_DEN_R |= 0x0F; //enable digital pins PE0-3
//}
void Car_Dir_Init(void){
	if ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0) {
		SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;	// Activate B clocks
		while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0){};
	}

	GPIO_PORTB_AMSEL_R &= ~(0xCC);   // PB7,6,3,2 => 1100_1100 = 0xCC
  GPIO_PORTB_AFSEL_R &= ~(0xCC);
  GPIO_PORTB_PCTL_R  &= ~0xFF00FF00; // Clear relevant bits for PB7,6,3,2
  GPIO_PORTB_DEN_R   |= 0xCC;     
  GPIO_PORTB_DIR_R   |= 0xCC;       // PB7,6,3,2 => outputs
  
  // Optionally set them all low to begin with:
  //WHEEL_DIR = 0x00; 
}
// Initilize port F and arm PF4, PF0 for falling edge interrupts
void SwitchLED_Init(void){  
	unsigned long volatile delay;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF; // (a) activate clock for port F
  delay = SYSCTL_RCGC2_R;
  GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY; // unlock GPIO Port F
  GPIO_PORTF_CR_R |= 0x1F;         // allow changes to PF4,0
  GPIO_PORTF_DIR_R &= ~0x11;    // (c) make PF4,0 in (built-in button)
  GPIO_PORTF_DIR_R |= 0x0E;     // make PF1-3 out
  GPIO_PORTF_AFSEL_R &= ~0x1F;  //     disable alt funct on PF4,0
  GPIO_PORTF_DEN_R |= 0x1F;     //     enable digital I/O on PF4,0
  GPIO_PORTF_PCTL_R &= ~0x000FFFFF; //  configure PF4,0 as GPIO
  GPIO_PORTF_AMSEL_R &= ~0x1F;  //     disable analog functionality on PF4,0
  GPIO_PORTF_PUR_R |= 0x11;     //     enable weak pull-up on PF4,0
  GPIO_PORTF_IS_R &= ~0x11;     // (d) PF4,PF0 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x11;    //     PF4,PF0 is not both edges
  GPIO_PORTF_IEV_R |= 0x11;    //     PF4,PF0 rising edge event
  GPIO_PORTF_ICR_R = 0x11;      // (e) clear flags 4,0
  GPIO_PORTF_IM_R |= 0x11;      // (f) arm interrupt on PF4,PF0
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF1FFFFF)|0x00400000; // (g) bits:23-21 for PORTF, set priority to 5
  NVIC_EN0_R |= 0x40000000;      // (h) enable interrupt 30 in NVIC
  GPIO_PORTF_DATA_R &= ~0x0E;   	// make PF1-3 low
	GPIO_PORTF_DATA_R |= RED; 		//make LED red
}




