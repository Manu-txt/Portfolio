// ADC1SS3.c
// Runs on TM4C123, starter file for space invader game
// Min He
// November 15, 2022

#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "ADC1SS3.h"
// TODO
#define ADC1_PSSI_SS3 0x0008    // start sample sequencer 3
#define ADC1_ISC_SS3  0x0008    // acknowledge sample sequencer 3 interrupt
#define ADC1_RIS_SS3  0x08

// This initialization function sets up the ADC2 Sample Sequencer 3 
// Max sample rate: <=125,000 samples/second
// SS3 triggering event: software trigger
// SS3 1st sample source: Ain8 (PE5)
// SS3 interrupts: flag set on completion but no interrupt requested
void ADC1SS3_Init(void){
volatile unsigned long delay;
  //SYSCTL_RCGC0_R |= 0x00020000; // 1) activate ADC1 (legacy code)
  SYSCTL_RCGCADC_R |= 0x00000002; // 1) activate ADC1
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4; // 1) activate clock for Port E
  delay = SYSCTL_RCGC2_R;      // 2) allow time for clock to stabilize
  delay = SYSCTL_RCGCADC_R;
  GPIO_PORTE_DIR_R &= ~0x20;      // 3) make PE5 input
  GPIO_PORTE_AFSEL_R |= 0x20;     // 4) enable alternate function on PE5
  GPIO_PORTE_DEN_R &= ~0x20;      // 5) disable digital I/O on PE5
                                  // 5a) configure PE4 as ?? (skip this line because PCTL is for digital only)
  //GPIO_PORTE_PCTL_R = GPIO_PORTE_PCTL_R&0xFFFFF000;
  GPIO_PORTE_AMSEL_R |= 0x20;     // 6) enable analog functionality on PE5
	
	//ADC Configuration
  ADC1_PC_R &= ~0xF;              // 8) clear max sample rate field
  ADC1_PC_R |= 0x1;               //    configure for 125K samples/sec
  ADC1_SSPRI_R = 0x0123;          // 9) Sequencer 3 is highest priority
  ADC1_ACTSS_R &= ~0x0008;        // 10) disable sample sequencer 3
  ADC1_EMUX_R &= ~0xF000;         // 11) seq3 is software trigger
  ADC1_SSMUX3_R = (ADC1_SSMUX3_R&0xFFFFFFF0) +8;	// 12) set channels for SS3 ain8 PE5
  ADC1_SSCTL3_R = 0x0006;         // no TS0 D0, yes IE0 END0
  ADC1_IM_R &= ~0x0008;           // 14) disable SS3 interrupts
  ADC1_ACTSS_R |= 0x0008;         // 15) enable sample sequencer 3	
}

//------------ADC1SS3_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint16_t ADC1SS3_In(void){ 
	uint16_t ADC_Value=0;
	// TODO: start ADC1 SS3 and read the ditigal output.
	ADC1_PSSI_R = ADC1_PSSI_SS3;
	while((ADC1_RIS_R&ADC1_RIS_SS3)==0){};
		ADC_Value = ADC1_SSFIFO3_R&0xFFF;
		ADC1_ISC_R = ADC1_ISC_SS3;
	return 	ADC_Value;
}

// Convert a ADC value to a value between [0, max_x_axis]
// convert a value range [0,4095] to a range of [0,83]
uint8_t ADCValue_To_X_AXIS(uint16_t ADCValue,uint8_t max_x_axis) {
	uint8_t x_position=0;
	x_position = (ADCValue * max_x_axis) / 4095;
	return x_position;
}
