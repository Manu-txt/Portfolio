/*
``Class: CECS 347 Embedded Systems 2
``Group 5
Project 1
Filename: Project1P2.c
Professor: Min He
01/29/2025
 */

#include <stdint.h>
#include <stdio.h>
#include "tm4c123gh6pm.h"
#include "Timer1A.h"
#include "UltrasonicSensor.h"
#include "SysTick.h"
#include "UART0.h"
#include "PLL.h"

#define SW1        (*((volatile uint32_t *) 0x40025040))  // PF4
#define LED        (*((volatile uint32_t *) 0x40025038))  // PORTF, pin: 1,2,3

#define SW1_MASK   0x10
#define LED_MASK   0x0E
#define PORTF_UNLOCK  0x4C4F434B
#define EN_PORTF_INTR 0x40000000
#define SYSCTL_RCGCGPIO_PORTF 0x20

#define MAX_STR_LEN  50
#define MAX_TICKS    65535

#define GREEN       0x08  // PF3 connects to green LED
#define RED         0x02  // PF1 connects to red LED
#define BLUE        0x04  // PF2 connects to blue LED

extern void EnableInterrupts(void);

void PortB_Init(void);
void GPIOPortB_Handler(void);
void SW_LED_Init(void);
void GPIOPortF_Handler(void);

extern volatile uint32_t distance;
extern volatile uint8_t done;
volatile uint8_t start_measurement = 0;  // Flag to start measurement

int main(void) {
    PortB_Init();
    Timer1A_Init();
    SysTick_Init();
    SW_LED_Init();
    PLL_Init();
    UART0_Init();
    EnableInterrupts();
    UART0_OutCRLF();

    // Everything stays OFF at startup
    LED = 0;       
    distance = OUT_OF_RANGE;  
    done = 0;      
    start_measurement = 0;  
		

    while (1) {

	uint8_t str[MAX_STR_LEN];  

        // ? Wait for SW1 press before measuring distance
        while (!start_measurement);

        start_measurement = 0;  // Reset flag

        done = 0;
        distance = 0;

        // ? Start distance measurement
        TRIGGER_PIN &= ~TRIGGER_VALUE;
        GPTM_1_n_US(2);
        TRIGGER_PIN |= TRIGGER_VALUE;
        GPTM_1_n_US(10);
        TRIGGER_PIN &= ~TRIGGER_VALUE;

        while (!done);  // Wait for echo response

		if(distance >= OUT_OF_RANGE){
			sprintf((char *)str, "The current distance is OUT OF RANGE\n\r");
			UART0_OutString(str); 
			//done = 1;
		}
		else{
			sprintf((char *)str, "The current distance is	 %d\n\r", distance);
			UART0_OutString(str); 
			//done = 1;
		}	

        // ? LED control logic based on distance
        if (distance <= 10) {
            SysTick_Start(4000000);  // Start RED LED blinking
            LED &= ~GREEN;  // Turn OFF Green
            LED &= ~BLUE;   // Turn OFF Blue
        } else {
            SysTick_Stop();
            LED &= ~RED;  // Ensure RED LED stops blinking
        }

        if ((distance > 10) && (distance <= 70)) {
            LED = (LED & ~BLUE) | GREEN;  // Keep Green ON, turn Blue OFF
        } else if ((distance > 70) && (distance <= 100)) {
            LED = (LED & ~GREEN) | BLUE;  // Keep Blue ON, turn Green OFF
        } else {
            LED = 0;  // Turn OFF all LEDs if no valid reading
        }
    }
}

void SW_LED_Init(void){
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_PORTF;
    while ((SYSCTL_RCGCGPIO_R & SYSCTL_RCGCGPIO_PORTF) != SYSCTL_RCGCGPIO_PORTF);

    GPIO_PORTF_LOCK_R = PORTF_UNLOCK;  
    GPIO_PORTF_CR_R |= SW1_MASK | LED_MASK;

    // Configure LEDs as output
    GPIO_PORTF_AMSEL_R &= ~LED_MASK;
    GPIO_PORTF_PCTL_R &= ~0x0000F0F0;
    GPIO_PORTF_DIR_R |= LED_MASK;
    GPIO_PORTF_AFSEL_R &= ~LED_MASK;
    GPIO_PORTF_DEN_R |= LED_MASK;

    // Configure Switch 1 (SW1) on PF4 as input with interrupt
    GPIO_PORTF_AMSEL_R &= ~SW1_MASK;
    GPIO_PORTF_PCTL_R &= ~0x000F0000;
    GPIO_PORTF_DEN_R |= SW1_MASK;
    GPIO_PORTF_DIR_R &= ~SW1_MASK;
    GPIO_PORTF_AFSEL_R &= ~SW1_MASK;
    GPIO_PORTF_PUR_R |= SW1_MASK;

    GPIO_PORTF_IS_R &= ~SW1_MASK;   // Edge-sensitive
    GPIO_PORTF_IBE_R &= ~SW1_MASK;  // Single edge
    GPIO_PORTF_IEV_R &= ~SW1_MASK;  // Falling edge trigger
    GPIO_PORTF_ICR_R = SW1_MASK;    // Clear interrupt flag
    GPIO_PORTF_IM_R |= SW1_MASK;    // Enable SW1 interrupt

    NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF1FFFFF) | 0x00800000;  // Priority 4
    NVIC_EN0_R |= EN_PORTF_INTR;  // Enable interrupt
}

void GPIOPortB_Handler(void) {
    if (GPIO_PORTB_MIS_R & ECHO_VALUE) {  // Check if PB4 caused the interrupt
        if (ECHO_PIN == ECHO_VALUE) {     // Rising edge detected
            GPTM_START(MAX_TICKS);
        } else {                          // Falling edge detected
            GPTM_STOP();
            uint32_t elapsed_time = GPTM_ELAPSED();

            // ? Calculate new distance
            distance = (uint32_t)(elapsed_time * MC_LEN * SOUND_SPEED) / 2;



            done = 1;  // ? Mark measurement as complete **after** distance updates
        }

        GPIO_PORTB_ICR_R = ECHO_VALUE;  // Acknowledge interrupt on PB4
    }
}

void GPIOPortF_Handler(void) {
    if ((GPIO_PORTF_RIS_R & SW1_MASK) == SW1_MASK) {  // Check if SW1 was pressed
        start_measurement = 1;  // ? Set flag to trigger distance measurement
    }
    GPIO_PORTF_ICR_R = SW1_MASK; // Acknowledge SW1 press
}

void SysTick_Handler(void) {
    if (distance <= 10) {
        LED ^= RED;  // Toggle RED LED
    }
}
