// PWM.h
// Runs on TM4C123
// Use PWM0/PB6 and PWM1/PB7 to generate pulse-width modulated outputs.
// Daniel Valvano
// March 28, 2014

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
// Modified by Min He, September 7, 2021
#include <stdint.h>

#define DIRECTION (*((volatile unsigned long *)0x40005330))
#define FORWARD 		0xCC	  //1111, both wheels move forward
#define BACKWARD 		0x44	  //1010, both wheels move backward
#define LEFTPIVOT   0x4C    // left moving backward, right moving fordward
#define RIGHTPIVOT  0xC4    // right moving backward, left moving fordward

#define LIGHT (*((volatile unsigned long *)0x40025038)) // onboard RBG LEDs are used to show car status.
#define RED 0x02
#define GREEN 0x08
#define BLUE 0x04

// duty cycles for different speeds
#define STOP 1
#define SPEED_0  1
#define SPEED_5  800
#define SPEED_10 1600
#define SPEED_15 2400
#define SPEED_20 3200
#define SPEED_35 5600
#define SPEED_60 9600
#define SPEED_80 12800
#define SPEED_98 15680


void PLL_Init(void);

// Wheel PWM connections: on PB6/M0PWM0:Left wheel, PB7/M0PWM0:Right wheel
void PWM_PB45_Init(void);

// Start left wheel
void Start_L(void);

// Start right wheel
void Start_R(void);

// Stop left wheel
void Stop_L(void);

// Stop right wheel
void Stop_R(void);

void Start_Both_Wheels(void);

void Stop_Both_Wheels(void);

void Move_Forward(int L_Wheel, int R_Wheel);
void Move_Backward(int L_Wheel, int R_Wheel);
void Left_Turn(int L_Wheel, int R_Wheel);
void Left_Turn_Wall(int L_Wheel, int R_Wheel);
void Right_Turn_Wall(int L_Wheel, int R_Wheel);
void Right_Turn(int L_Wheel, int R_Wheel);
void Left_Turn_Back(int L_Wheel, int R_Wheel);
void Right_Turn_Back(int L_Wheel, int R_Wheel);
void Left_Pivot(int L_Wheel, int R_Wheel);
void Right_Pivot(int L_Wheel, int R_Wheel);
void Stop_Car(void);
// Change duty cycle of left wheel: PB6
void Set_L_Speed(unsigned long dutyL);

// change duty cycle of right wheel: PB7
void Set_R_Speed(unsigned long dutyR);

// Initialize port E pins PE0-3 for output
// PE0-3 control directions of the two motors: PE3210:L/SLP,L/DIR,R/SLP,R/DIR
// Inputs: None
// Outputs: None
void Car_Dir_Init(void);

// Initilize port F and arm PF4, PF0 for falling edge interrupts


