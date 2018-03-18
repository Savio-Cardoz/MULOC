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
#ifndef __KEYPAD4X4_H
#define __KEYPAD4X4_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define KEYPAD_PORT_IN PORTB
#define KEYPAD_PORT_OUT LATB
#define KEYPAD_PORT_DIR TRISB
#define ROW_1 RB7
#define ROW_2 RB6
#define ROW_3 RB5
#define ROW_4 RB4
#define COL_1 RB3
#define COL_2 RB2
#define COL_3 RB1
#define COL_4 RB0

#define SCAN_ROW_1 0x7F
#define SCAN_ROW_2 0xBF
#define SCAN_ROW_3 0xDF
#define SCAN_ROW_4 0xEF

#define keyLocked 1
#define keyUnlocked 0
#define True 1
#define False 0

enum button {   button_0 = 0,
                button_1,
                button_2,
                button_3,
                button_4,
                button_5,
                button_6,
                button_7,
                button_8,
                button_9,
                NO_KEY_TUNE = 10,
                SP_FUNC_A = 11,
                SP_FUNC_B = 12,
                SP_FUNC_ON = 13,
                SP_FUNC_OFF = 14,
                SP_FUNC_ENTER = 15,
                NONE = 16,
            };

typedef struct{
    char keyPressIndicator:1;  // Indicates if a key was pressed
    char keyPressSound:1;      // Flag for audible key press indication
    char keyLock:1;           // keypad locked/unlocked status indicator. 1 locked, 0 unlocked
    char keyPressed;        // Holds the value of the key pressed        
}keypadStatus;

void initKeypad(void);
void ackKeyPress();
void getKey(keypadStatus *KeypadStatus);
char buttonPressedIndicator();
void delay_ms(unsigned int time);

#endif  /* KEYPAD4X4_H */

