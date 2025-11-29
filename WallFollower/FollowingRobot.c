// FollowingRobot.c
// Runs TM4C123
// Starter program CECS 347 project 2 - A Follwoing Robot
// by Min He, 03/17/2024

#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "ADC1SS1.h"  
#include "Motors.h"
#include "SW_LED.h"
#include "PLL.h"
#include "Timer0A.h"

volatile enum robot_modes {INACTIVE, OBJECT_FOLLOWER, WALL_FOLLOWER};

// Function prototypes
// external functions
extern void DisableInterrupts(void);
extern void EnableInterrupts(void);  // Enable interrupts
extern void WaitForInterrupt(void);  // low power mode
volatile uint16_t FrontADC, LeftADC, RightADC;
volatile uint8_t robot_active = 0;

// functions defined in this file
void System_Init(void);
void object_follower(void);
void wall_follower(void);
void GPIOPortF_Handler(void);
uint16_t median(uint16_t u1, uint16_t u2, uint16_t u3);
void ReadADCMedianFilter( uint16_t *ain3, uint16_t *ain2, uint16_t *ain1);

volatile enum robot_modes mode=INACTIVE;

int main(void){	
	System_Init();
	robot_active = 0;
  while(1){
			//object_follower();
		switch (mode) {
			case OBJECT_FOLLOWER:
				object_follower();
			  break;
			case WALL_FOLLOWER:
				wall_follower();
				break;
			case INACTIVE:
				Stop_Car();
			  break;
			default:
				WaitForInterrupt();
			  break;
					
		}
  }
}

void System_Init(void){
	DisableInterrupts();
  PLL_Init();               // set system clock to 16 MHz 
	ADC_Init321();       // Initialize ADC1 Sample sequencer 1 
  SW_LED_Init();          // inititlize switches and LEDs in SW_LED module
	Car_Dir_Init();
	PWM_PB45_Init();
  EnableInterrupts();	\
	Timer0A_Init();
}

void object_follower(void) {
    uint8_t inactive = 0;

    // Wait for an object to be within range before starting
    do {
        ReadADCMedianFilter(&FrontADC, &LeftADC, &RightADC);
    } while ((FrontADC < TOO_FAR) || (FrontADC > TOO_CLOSE));

    while (robot_active) {
			if (mode == WALL_FOLLOWER) {
						Stop_Car();
            return;  // Exit the function to switch to wall_follower mode
        }
			
			for(int i = 0; i < 10; ++i){
				ReadADCMedianFilter(&FrontADC, &LeftADC, &RightADC);
			}

        // **Enter inactive state if object is too close (<10cm)**
        if (FrontADC > TOO_CLOSE || LeftADC > TOO_CLOSE || RightADC > TOO_CLOSE) { 
          mode = INACTIVE;
					Stop_Car();
					LED = RED;
					return;
					//LED = RED;
        }

        // **If object is out of range, stop**
        else if (FrontADC < TOO_FAR && LeftADC < TOO_FAR && RightADC < TOO_FAR) { 
            //mode = INACTIVE;
						Stop_Car();
        }
				
				
        // **Turn left if the left sensor detects an object first**
        else if (LeftADC < FOLLOW_DIST && LeftADC > RightADC && LeftADC > FrontADC) { 
            Left_Turn(SPEED_20, SPEED_10);
        }
        // **Turn right if the right sensor detects an object first**
        else if (RightADC < FOLLOW_DIST && RightADC > LeftADC && RightADC > FrontADC) { 
            Right_Turn(SPEED_10, SPEED_20);
        }
				
				else if (LeftADC > FOLLOW_DIST && LeftADC > RightADC && LeftADC > FrontADC) { 
            Right_Turn_Back(SPEED_20, SPEED_35);
        }
        // **Turn right if the right sensor detects an object first**
        else if (RightADC > FOLLOW_DIST && RightADC > LeftADC && RightADC > FrontADC) { 
            Left_Turn_Back(SPEED_35, SPEED_20);
        }
        // **Move forward if the object is too far (ADC lower than FOLLOW_DIST but still in range)**
        else if (FrontADC > FOLLOW_DIST && LeftADC < TOO_FAR && RightADC < TOO_FAR) { 
            Move_Forward(SPEED_15, SPEED_15);
        }
        // **Move backward if the object is too close (but not too close to stop)**
        else if (FrontADC < FOLLOW_DIST && LeftADC < TOO_FAR && RightADC < TOO_FAR) { 
            Move_Backward(SPEED_15, SPEED_15);
        }
    }
}

void wall_follower(void){
	uint8_t followWall = 0;

	while (robot_active) {
        // Check if mode has changed back to OBJECT_FOLLOWER and stop current operation
        if (mode == OBJECT_FOLLOWER) {
            Stop_Car();  // Stop the car before switching back to object follower mode
            return;  // Exit the function to allow switching to object_follower mode
        }
				if(LeftADC > TOO_CLOSE){
					followWall = 1; // Follow left wall
				} else if (RightADC > TOO_CLOSE){
					followWall = 2; // Follow right wall
				}
				
				for(int i = 0; i < 10; ++i){
					ReadADCMedianFilter(&FrontADC, &LeftADC, &RightADC);
				}	
				
				switch (followWall){
					case 1:
						if (LeftADC > TOO_CLOSE){
							Right_Turn(SPEED_10, SPEED_20);
						}
						else if (LeftADC < TOO_CLOSE){
							Left_Turn(SPEED_20, SPEED_10);
						}
						else if (LeftADC < FOLLOW_DIST){
								Left_Turn_Wall(SPEED_35, SPEED_20);
							} 
							
						
						if (FrontADC > TOO_CLOSE){
							Right_Pivot(SPEED_20, SPEED_20);
						}
						
						break;
					case 2:
						if (RightADC > TOO_CLOSE){
							Left_Turn(SPEED_20, SPEED_10);
						}
						else if (RightADC< TOO_CLOSE){
							Right_Turn(SPEED_10, SPEED_20);
						}
						else if (RightADC < FOLLOW_DIST){
								Right_Turn_Wall(SPEED_20, SPEED_35);
							} 
							
						
						if (FrontADC > TOO_CLOSE){
							Left_Pivot(SPEED_20, SPEED_20);
						}
						
						break;
					}
	}
    }




uint16_t median(uint16_t u1, uint16_t u2, uint16_t u3){
  uint16_t result;
	
  if(u1>u2)
    if(u2>u3)   result=u2;     // u1>u2,u2>u3       u1>u2>u3
      else
        if(u1>u3) result=u3;   // u1>u2,u3>u2,u1>u3 u1>u3>u2
        else      result=u1;   // u1>u2,u3>u2,u3>u1 u3>u1>u2
  else
    if(u3>u2)   result=u2;     // u2>u1,u3>u2       u3>u2>u1
      else
        if(u1>u3) result=u1;   // u2>u1,u2>u3,u1>u3 u2>u1>u3
        else      result=u3;   // u2>u1,u2>u3,u3>u1 u2>u3>u1
  return(result);
}

void ReadADCMedianFilter(uint16_t *ain3, uint16_t *ain2, uint16_t *ain1){
  static uint16_t oldest3 = 0, middle3 = 0;
  static uint16_t oldest2 = 0, middle2 = 0;
  static uint16_t oldest1 = 0, middle1 = 0;
  
  uint16_t  newest3, newest2, newest1;
  uint16_t NewValue3, NewValue2, NewValue1;

  // Read ADC values
  ADC_In321(&newest3, &newest2, &newest1);
	
	*ain3 = median(newest3, middle3, oldest3);
	*ain2 = median(newest2, middle2, oldest2);
	*ain1 = median(newest1, middle1, oldest1);
	
	oldest3 = middle3; oldest2 = middle2; oldest1 = middle1;
	middle3 = newest3; middle2 = newest2; middle1 = newest1;

}

void GPIOPortF_Handler(void) {
	int i;
	
	for (i = 0; i < 100000; i++){}; // Simple debounce delay (adjust as needed)
    
	if (GPIO_PORTF_RIS_R & SW1_MASK) {		// Check if PF4 caused the interrupt
			GPIO_PORTF_ICR_R = SW1_MASK;  // Clear interrupt flag
			//mode = OBJECT_FOLLOWER;
			robot_active = !robot_active; // Toggle activation state
      mode = robot_active ? OBJECT_FOLLOWER : INACTIVE;
       
	}
	if ((GPIO_PORTF_RIS_R & SW2_MASK) && robot_active) {  // Check if PF0 (SW2) caused the interrupt
			  GPIO_PORTF_ICR_R = SW2_MASK;  // Clear interrupt flag
        //mode = WALL_FOLLOWER;
			//mode = (mode == OBJECT_FOLLOWER) ? WALL_FOLLOWER : OBJECT_FOLLOWER; // Toggle mode]
		if (mode == OBJECT_FOLLOWER) {
            mode = WALL_FOLLOWER;  // Switch to wall follower mode
        } else {
            mode = OBJECT_FOLLOWER;  // Switch to object follower mode
        }
    }

	GPIO_PORTF_DATA_R &= ~(Red | Blue | Green);  // Turn off all LEDs
		if (!robot_active) {
        GPIO_PORTF_DATA_R |= Red;   // RED for inactive
    } else {
        if (mode == OBJECT_FOLLOWER) {
            GPIO_PORTF_DATA_R |= Blue;  // BLUE for Object Follower
        } else if (mode == WALL_FOLLOWER) {
            GPIO_PORTF_DATA_R |= Green; // GREEN for Wall Follower
        }
}
		}