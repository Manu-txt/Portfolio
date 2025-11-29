    /*
``Class: CECS 347 Embedded Systems 2
``Group 5
Project 1
Filename: Project1P2.c
Professor: Min He
01/29/2025
 */
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Timer1A.h"

#define OUT_OF_RANGE     101
#define TRIGGER_PIN      (*((volatile uint32_t *)0x40005080))  // PB5 is the trigger pin
#define ECHO_PIN         (*((volatile uint32_t *)0x40005040))  // PB4 is the echo pin
#define TRIGGER_VALUE    0x20  // PB5 mask for trigger pin
#define ECHO_VALUE       0x10  // PB4 mask for echo pin
#define MC_LEN           0.0625  // Machine cycle length in microseconds for 16MHz clock
#define SOUND_SPEED      0.0343  // Speed of sound in cm per microsecond

static volatile uint8_t done = 0;
static volatile uint32_t distance = 0;

extern void EnableInterrupts(void);

void PortB_Init(void);
void GPIOPortB_Handler(void);

int main(void) {
    PortB_Init();
    Timer1A_Init();
    EnableInterrupts();

    while (1) {
        done = 0;
        distance = 0;

        // Send trigger signal to the ultrasonic sensor
        TRIGGER_PIN &= ~TRIGGER_VALUE;  // Set trigger pin low
           GPTM_1_n_US(2);                    // 2 microsecond delay
        TRIGGER_PIN |= TRIGGER_VALUE;   // Set trigger pin high
        GPTM_1_n_US(10);                   // 10 microsecond delay
        TRIGGER_PIN &= ~TRIGGER_VALUE;  // Set trigger pin low

        // Wait for the echo response to complete
        while (!done);
    }
}

void PortB_Init(void) {
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;           // 1) activate clock for Port A
	while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)!=SYSCTL_RCGC2_GPIOB){}; // wait for clock to start
 
    GPIO_PORTB_PCTL_R &= ~0x00FF0000;               // Configure PB4 and PB5 as GPIO
    GPIO_PORTB_AMSEL_R &= (uint32_t)~(TRIGGER_VALUE | ECHO_VALUE);  // Disable analog on PB4 and PB5
    GPIO_PORTB_DIR_R &= ~ECHO_VALUE;                // PB4 (echo) as input
    GPIO_PORTB_DIR_R |= TRIGGER_VALUE;              // PB5 (trigger) as output
    GPIO_PORTB_AFSEL_R &= ~(TRIGGER_VALUE | ECHO_VALUE);  // Disable alternate functions
    GPIO_PORTB_DEN_R |= (TRIGGER_VALUE | ECHO_VALUE);     // Enable digital functionality

    // Configure edge-triggered interrupt on PB4 (echo)
    GPIO_PORTB_IS_R &= ~ECHO_VALUE;                 // PB4 is edge-sensitive
    GPIO_PORTB_IBE_R |= ECHO_VALUE;                 // Trigger interrupt on both edges
    GPIO_PORTB_ICR_R = ECHO_VALUE;                  // Clear any previous interrupt
    GPIO_PORTB_IM_R |= ECHO_VALUE;                  // Enable interrupt on PB4

    // Set interrupt priority and enable interrupt
    NVIC_PRI0_R = (NVIC_PRI0_R & 0xFFFF1FFF) | 0x00006000;  // Set priority 3
//		NVIC_PRI0_R = (NVIC_PRI0_R & 0xFFFF1FFF) | (0x3 << 13);  // Correct priority for IRQ 1

		NVIC_EN0_R = 0x00000002;                                // Enable Port B interrupt in NVIC
//		NVIC_EN0_R = (1 << 1);  // Enable IRQ 1 (Port B)

}

void GPIOPortB_Handler(void) {
    if (GPIO_PORTB_MIS_R & ECHO_VALUE) {            // Check if PB4 caused the interrupt
        if (ECHO_PIN == ECHO_VALUE) {               // Rising edge detected
//				if ((GPIO_PORTB_DATA_R & ECHO_VALUE) == ECHO_VALUE) {
				GPTM_START(65535);                      // Start timer with large period
        } else {                                    // Falling edge detected
            GPTM_STOP();                            // Stop timer
            uint32_t elapsed_time = GPTM_ELAPSED(); // Get elapsed time

            // Calculate distance in centimeters
					distance = (uint32_t)(GPTM_ELAPSED()*MC_LEN*SOUND_SPEED)/2;	 // (tickk to time, timer returns ) number of clock cycle * mc_len * sound speed ) /2 
            if (distance > 100) {
                distance = OUT_OF_RANGE; 							// Out-of-range check
            }
						else if (distance < 10) {}
						else if ((distance >= 70)&&	(distance <= 100)){}
						else if ((distance >= 10)&&	(distance <= 70)){}
							
            done = 1;                               // Signal that measurement is done
        }

        GPIO_PORTB_ICR_R = ECHO_VALUE;              // Acknowledge interrupt on PB4
    }
}


//RELOad value explain 