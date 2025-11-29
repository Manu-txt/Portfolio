// CECS346 Lab 3: FSM
// Project Group: Group 5
// Team members: Emmanuel Velazquez, Kevin Zelada, Sheesh Dhawan, Zoe
// Lab description: Design a traffic light controller to maximize traffic
// flow, minimize wait time at a red light, and avoid accidents. Applying 
// indexed data structures, creating a segmented software system, designing 
// a finite state machine controller, creating fixed-time delays using 
// Systick Timer, and debugging real-time systems. 

// Hardware Design
// 1)	Port B will be used to control 6 LEDs: Red West(PB5), Yellow West(PB4)
//    Green West(PB3), Red South(PB2), Yellow South(PB1), Green South(PB0)
// 2)	Port E will be used for the 3 switches: South Sensor(PE2), 
//		West Sensor(PE1), Pedestrain Walk button(PE0)
// 3) Port F will be used for the onboard LED lights: Red Pedestrian(PF1),
//		Green Pedestrian(PF3)

#include <stdint.h>   // for data type alias


// Registers for switches
// Complete the following register definitions

//// Register for Sensors (Port E)
#define SENSOR								 (*((volatile unsigned long *)0x4002401C)) // bit addresses for bits PE0-PE2
#define GPIO_PORTE_DATA_R				 (*((volatile unsigned long *)0x400243FC))        
#define GPIO_PORTE_DIR_R         (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R       (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_DEN_R         (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_AMSEL_R       (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R        (*((volatile unsigned long *)0x4002452C))


//// Registers for Traffic LEDs (Port B)
#define T_LIGHTS               	 (*((volatile unsigned long *)0x400050FC)) // bit addresses for bits PB0-PB5
#define GPIO_PORTB_DIR_R         (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_AFSEL_R       (*((volatile unsigned long *)0x40005420))
#define GPIO_PORTB_DEN_R         (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_AMSEL_R       (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R        (*((volatile unsigned long *)0x4000552C))
	
//// Registers for Pedestrian LEDs (Port F)
#define P_LIGHTS								 (*((volatile unsigned long *)0x40025028)) // bit addresses for bit PF1 & PF3
#define GPIO_PORTF_DIR_R         (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_CR_R					 (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AFSEL_R       (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_DEN_R         (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_AMSEL_R       (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R        (*((volatile unsigned long *)0x4002552C))
	
// Constants definitions
#define SYSCTL_RCGC2_GPIOE        0x10 // port E Clock Gating Control
#define SYSCTL_RCGC2_GPIOB        0x02 // port A Clock Gating Control
#define SYSCTL_RCGC2_GPIOF				0x20 // port F Clock Gating Control
#define SYSCTL_RCGC2_R            (*((volatile unsigned long *)0x400FE108))
#define NVIC_ST_CTRL_R						(*((volatile uint32_t *)0xE000E010))
#define NVIC_ST_RELOAD_R					(*((volatile uint32_t *)0xE000E014))
#define NVIC_ST_CURRENT_R					(*((volatile uint32_t *)0xE000E018))
#define NVIC_ST_CTRL_COUNT				0x00010000
#define NVIC_ST_CTRL_CLK_SRC			0x00000004
#define NVIC_ST_CTRL_ENABLE				0x00000001
#define MILLI_SEC									4000000
#define P_LIGHT_MASK              0x0A
#define T_LIGHT_MASK              0x3F
#define SENSOR_MASK								0x07
#define S_GREEN                   0x01
#define S_YELLOW                  0x02
#define S_RED                     0x04
#define W_GREEN                   0x08
#define W_YELLOW                  0x10
#define W_RED                     0x20 
#define S_SW                      0x04
#define W_SW                      0x02
#define P_SW                      0x01

void Delay(uint8_t n);
void P_Lights_Init(void);
void T_Lights_Init(void);
void Sensor_Init(void);
void SysTick_init(void);
void Wait_HalfSecond(void);
void SysTick_N_HalfSec(unsigned char n_half_s);

// FSM state data structure
struct State {
  uint8_t Out_T;
	uint8_t Out_P;
  uint8_t Time;  
  uint8_t Next[9];
}; 

typedef const struct State STyp;
// Constants definitions
#define GOS       0
#define WAITS     1  
#define GOW       2  
#define WAITW     3  
#define GOP       4  
#define WAITPON1  5  
#define WAITPOFF1 6  
#define WAITPON2  7  
#define WAITPOFF2 8 

// FSM states

// Output pins are:3(white), 2(red), 1(yellow), 0(green)
// Input pins are: 1:sw2, 0:sw1 
 STyp FSM[9]={
  {0x21, 0x02, 8, {GOS, WAITS, WAITS, WAITS, GOS, WAITS, WAITS, WAITS}},
  {0x22, 0x02, 4, {GOW, GOP, GOW, GOW, GOP, GOP,GOW, GOP}},
  {0x0C, 0x02, 8, {GOW, WAITW, GOW, WAITW, WAITW, WAITW, WAITW, WAITW}},
  {0x14, 0x02, 4, {GOS, GOP, GOP, GOP, GOS, GOP, GOS, GOS}},         
  {0x24, 0x08, 8, {GOP, GOP, WAITPON1, WAITPON1, WAITPON1, WAITPON1, WAITPON1, WAITPON1}},
  {0x24, 0x08, 1, {WAITPOFF1, WAITPOFF1, WAITPOFF1, WAITPOFF1, WAITPOFF1, WAITPOFF1, WAITPOFF1, WAITPOFF1}},  
	{0x24, 0x00, 1, {WAITPON2, WAITPOFF1, WAITPON2, WAITPON2, WAITPON2, WAITPON2, WAITPON2, WAITPON2}},         
	{0x24, 0x08, 1, {WAITPOFF2, WAITPOFF2, WAITPOFF2, WAITPOFF2, WAITPOFF2, WAITPOFF2, WAITPOFF2, WAITPOFF2}},         
	{0x24, 0x00, 1, {GOS, GOP, GOW, GOW, GOS, GOS, GOS, GOW}}
             };    

int main(void){ 
  uint8_t S;  // index to the current states
  uint8_t Input; 
	
	P_Lights_Init();
	T_Lights_Init();
	Sensor_Init();
	SysTick_init();
	
  S = GOS;                     // FSM start with green on south 
    
  while(1){
		// Put your FSM engine here
    T_LIGHTS = FSM[S].Out_T; 							// set Traffic lights
		P_LIGHTS = FSM[S].Out_P;							// Set pedestrain lights
		SysTick_N_HalfSec(FSM[S].Time);								
		Input = SENSOR;												// read sensors(switches)
		S = FSM[S].Next[Input];
  }
}

void T_Lights_Init(void){
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB; // Activate Port B clocks
	while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB) != SYSCTL_RCGC2_GPIOB){} // wait for clock to be active
		
  GPIO_PORTB_AMSEL_R  &= T_LIGHT_MASK; // Disable analog function on PB5-0
  GPIO_PORTB_PCTL_R &= ~0x00FFFFFF; // Enable regular GPIO
  GPIO_PORTB_DIR_R  |= T_LIGHT_MASK;  // Outputs on PB5-0
  GPIO_PORTB_AFSEL_R &= ~T_LIGHT_MASK; // Regular function on PB5-0
  GPIO_PORTB_DEN_R |= T_LIGHT_MASK; // Enable digital on PB5-0
}

void Sensor_Init(void){
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;  // Activate Port E clocks
	while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOE) != SYSCTL_RCGC2_GPIOE){} // wait for clock to be active
		
	GPIO_PORTE_AMSEL_R &= ~SENSOR_MASK; // Disable analog function on PE2-0
  GPIO_PORTE_PCTL_R  &= ~0x00000FFF;// Enable regular GPIO
  GPIO_PORTE_DIR_R &= ~SENSOR_MASK;   // Input on PE2-0
  GPIO_PORTE_AFSEL_R &= ~SENSOR_MASK; // Regular function on P2-0
  GPIO_PORTE_DEN_R |= SENSOR_MASK;    // Enable digital signals on PE2-0
	//GPIO_PORTA_PDR_R |= SW_MASK;    // Optional: Enable pull-down resistors for PE2-0
}

void P_Lights_Init(void){
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;  // Activate Port F clocks
	while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOF) != SYSCTL_RCGC2_GPIOF){} // wait for clock to be active
	
	GPIO_PORTF_AMSEL_R &= ~P_LIGHT_MASK; // Disable analog function on PF3&1
	GPIO_PORTF_CR_R |= P_LIGHT_MASK;
  GPIO_PORTF_PCTL_R  &= ~0x0000F0F0;// Enable regular GPIO
  GPIO_PORTF_DIR_R |= P_LIGHT_MASK;   // Output on PF3&1
  GPIO_PORTF_AFSEL_R &= ~P_LIGHT_MASK; // Regular function on PF3&1
  GPIO_PORTF_DEN_R |= P_LIGHT_MASK;    // Enable digital signals on PF3&1
	//GPIO_PORTA_PDR_R |= SW_MASK;    // Optional: Enable pull-down resistors for PF3&1
}

// initialize the systick timer with maximum reload value,
// enable SysTick timer with system clock.
void SysTick_init(void)
{
	NVIC_ST_CTRL_R &= ~NVIC_ST_CTRL_ENABLE;
	NVIC_ST_CTRL_R |= NVIC_ST_CTRL_CLK_SRC;
}


// Use busy waiting approach to generate n*0.5s delay,
// n is specified by the parameter of this function.
void Wait_HalfSecond(void)
{
	NVIC_ST_RELOAD_R = MILLI_SEC-1;
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it                                        
  NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE; // enable SysTick timer
	
	// wait for COUNT bit in control register to be raised.
	while ((NVIC_ST_CTRL_R&NVIC_ST_CTRL_COUNT)==0) {} 
  NVIC_ST_CTRL_R &= ~NVIC_ST_CTRL_ENABLE; // disable SysTick timer
}

void SysTick_N_HalfSec(unsigned char n_half_s)
{
	for(int i = 0; i < n_half_s; ++i)
	{
		Wait_HalfSecond();
	}
}