#ifndef ULTRASONICSENSOR_H // do not include more than once
#define ULTRASONICSENSOR_H

#include "tm4c123gh6pm.h"
#include "UltrasonicSensor.h"
#include <stdint.h>

#define TRIGGER_PIN      (*((volatile uint32_t *)0x40005080))  // PB5 is the trigger pin
#define ECHO_PIN         (*((volatile uint32_t *)0x40005040))  // PB4 is the echo pin
#define TRIGGER_VALUE    0x20  // PB5 mask for trigger pin
#define ECHO_VALUE       0x10  // PB4 mask for echo pin
#define MC_LEN           0.0625  // Machine cycle length in microseconds for 16MHz clock
#define SOUND_SPEED      0.0343  // Speed of sound in cm per microsecond
#define OUT_OF_RANGE     101

static volatile uint8_t done=0;
static volatile uint32_t distance=0;

void PortB_Init(void);
extern void GPIOPortB_Handler(void);



#endif // ULTRASONICSENSOR_H