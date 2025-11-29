// PLL.h
// Runs on TM4C123
// CECS 347 Project 2 - Space Invaders
// Group number:
// Group members:

// TODO:
// Determine the #define statement SYSDIV initializes
// the PLL to the desired frequency.
#define SYSDIV2  4 
// bus frequency is 400MMHz/(SYSDIV+1)

void PLL_Init(void);
