
#include "Includes.h"


// CCP1 module is used here to generate the required PWM
// Timer2 module is used to generate the PWM
// This PWM has 10bit resolution


void InitPWM(void)
{
	TRISA3  = 0;            // Make CCP1 pin as output
	CCP1CON = 0x0C;         // Configure CCP1 module in PWM mode

	PR2   = 0xFF;           // Configure the Timer2 period
	T2CON = 0x01;           // Set Prescaler to be 4, hence PWM frequency is set to 4.88KHz.

	SetPWMDutyCycle(0);     // Intialize the PWM to 0 duty cycle

	T2CON |= 0x04;          // Enable the Timer2, hence enable the PWM.
}



void SetPWMDutyCycle(unsigned int DutyCycle) // Give a value in between 0 and 1024 for DutyCycle
{
	CCPR1L   = DutyCycle>>2;        	// Put MSB 8 bits in CCPR1L
	CCP1CON &= 0xCF;                	// Make bit4 and 5 zero
	CCP1CON |= (0x30&(DutyCycle<<4));   // Assign Last 2 LSBs to CCP1CON
}