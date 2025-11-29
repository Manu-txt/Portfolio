// switches.h
// Runs on TM4C123, starter file for space invader game
// Min He
// November 15, 2022

// Initialize the onboard two switches.

#ifndef SWITCH_H
#define SWITCH_H

#include "tm4c123gh6pm.h"

// Define SW1 (Button PF4) for start/stop control
#define SW1                 (*((volatile unsigned long *)0x40025040)) //PF4
#define SW1_MASK 0x10
#define SW2                 (*((volatile unsigned long *)0x40025004))	//PF0
#define SW2_MASK 0x01
#define SW_MASK 0x11


void Switch_Init(void);

#endif 