#include "UltrasonicSensor.h"
#include "Timer1A.h"

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
