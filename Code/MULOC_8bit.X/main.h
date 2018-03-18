/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef _MAIN_H
#define _MAIN_H

#include <xc.h> // include processor files - each processor file is guarded. 
#include <htc.h>
#include "Keypad4x4.h"

#define MAXCODESIZE 24
#define MAXWRONGCODEENTRIES 3
#define ON 1
#define OFF 0
#define DOOR_SW_OPEN 1
#define DOOR_SW_CLOSED 0
#define CODE1ADDR 0x01      /* The preset seek address in EEPROM for code 1 */
#define CODE2ADDR 0x1B      /*  The preset seek address in EEPROM for code 2.
                                Codes can have be maximum 25 Chars long */
#define FACTORYRESETCODEADDR 0x36
#define WRNGCODEENTRIESADDR 0x7D
#define SYSTEMSTATEADDR 0x7E

// 0x7F is the first flash check byte.

enum timerType  {
                    ackTimer = 1,
                    errorTimer = 2,
                    okTimer = 3
                };
                
enum codeType   {
                    general = 0,
                    code1 = 1,
                    code2 = 2,
                    master1 = 3
                };

struct configSound{
    char timerVal;
    int duration;
    char pitch;
    enum timerType playSeq ;
    char noSoundFlag : 1;
};

typedef struct			// Using to access individual bits/pins of a register/port
{
	volatile unsigned char bit0:1;
	volatile unsigned char bit1:1;
	volatile unsigned char bit2:1;
	volatile unsigned char bit3:1;
	volatile unsigned char bit4:1;
	volatile unsigned char bit5:1;
	volatile unsigned char bit6:1;
	volatile unsigned char bit7:1;
} _io_reg;

#define REGISTER_BIT(rg,bt) ((volatile _io_reg*)&rg)->bit##bt

#define SETBIT(ADDRESS,BIT) (ADDRESS |= (1<<BIT))
#define CLEARBIT(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT))
#define CHECKBIT(ADDRESS,BIT) (ADDRESS & (1<<BIT))
#define BIT_FLIP(ADDRESS,BIT) ((ADDRESS)^=(BIT))

#define SET_LED REGISTER_BIT(PORTA, 0)
#define POWER_LED REGISTER_BIT(PORTA, 1)
#define ACCESS_LED REGISTER_BIT(PORTA, 2)
#define BUZZER REGISTER_BIT(PORTA, 3)
#define DOORSWITCH REGISTER_BIT(PORTA, 5)
#define LED_CONTROL REGISTER_BIT(PORTA, 6)
#define ERROR_LED REGISTER_BIT(PORTA, 7)

#define BATT_INP REGISTER_BIT(PORTA, 4)

volatile unsigned char systemStateReg;           // Flags that have to be saved to EEPROM
unsigned char systemStateReg2;          // Flags that dont need to be save to EEPROM
#define DOORSTATE REGISTER_BIT(systemStateReg, 0)       // 0-Door Closed, 1-Door Open
//#define LASTPASSCODE REGISTER_BIT(systemStateReg, 1)    // 0-Code 1 entered, 1-Code 2 entered
#define KEYPADSILENT REGISTER_BIT(systemStateReg, 2)    // 0-Sound on Keypress, 1-No Sound on Keypress
#define KEYPADLOCK REGISTER_BIT(systemStateReg, 3)      // 0-Keypad unlocked, 1-Keypad Locked
#define SYSTAMPERED REGISTER_BIT(systemStateReg, 4)     // 0-Normal State, 1-Attempt to tamper. Wrong codes were entered 3 times.
#define STATE_SLEEP REGISTER_BIT(systemStateReg, 5)
#define PINCHANGEINTERRUPT REGISTER_BIT(systemStateReg, 6)
#define TIMERINTERRUPT REGISTER_BIT(systemStateReg, 7)

#define DOTLSILENT REGISTER_BIT(systemStateReg2, 0)
#define PASSCODECONFIRM REGISTER_BIT(systemStateReg2, 1)
#define VALIDCODEINDICATOR REGISTER_BIT(systemStateReg2, 2)     // This flag is used while resetting the system to factory default. It remains SET for the 'systemWaitTime' only.
#define LEDSTATUS REGISTER_BIT(systemStateReg2, 3)              // Flag indicating if LED light should be switched ON or OFF, 1-ON, 0-OFF

#define ON_BUZZER 0
#define OFF_BUZZER 1

unsigned char keyPressCount;
unsigned char keyBuffer[25];     // Buffer that will hold the values of the keys pressed
volatile unsigned char timer1OVFcount;
unsigned int timer2OVFcount;
unsigned char wrongCodeEnteredCount;
unsigned char bufferPtr = 0;

struct configSound soundStuff;
unsigned char *eepromCode;
//extern enum timerType currentTimerState;
enum codeType LASTPASSCODE;

const unsigned char defaultPassCode[9] = {9,8,7,6,5,4,3,2,1};
const unsigned char defaultPassCode2[3] = {1,2,3};
unsigned char factoryResetCode[4] = {8,0,8,0};
unsigned char factoryResetCodeLenght = 4;
const unsigned char defaultPassCodeLenght = 9;
const unsigned char defaultPassCode2Lenght = 3;

unsigned char tempCodeBuffer[25];
unsigned char tempCodeKeyPressCount;

void checkFirstFlash();
void ackKeyPress();
unsigned char passcodeCompare(char *keybuffer1, char compareLenght, unsigned char compareAddr);
void handleSleep();
void disableInterrupts();
void interrupt ISR(void);
void okTimerTone();
void doorTimerTone();
void errorTimerTone();
void doorOpenAlarm(unsigned char alarmMute);
void startTimer0();
void updateTempCodeBuffer(char *keybuffer, char keycount);
char confirmationCodeCheck(char *keybuffer1, char *keybuffer2, char keycount1, char keycount2);
void doorOpenedTone(unsigned char tuneNumber);
void setDefaultUserCodes();
void setDefaultFactoryCodes();
void soundConfigParam(unsigned int timerStartValue, unsigned int timerDuration, unsigned char soundONOFF, unsigned char soundType);
char monitorBattery();

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif

