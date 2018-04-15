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
#ifndef __TIMER_H
#define __TIMER_H




#include <xc.h> // include processor files - each processor file is guarded. 
#include "main.h"

#define TIM_PRESCALAR_1_8   0x30
#define TIM_PRESCALAR_1_4   0x20
#define TIM_PRESCALAR_1_2   0x10
#define TIM_PRESCALAR_1_1   0x00
#define TIM_OSC_EN          0x08
#define TIM_NO_SYNC_EXT     0x04
#define TIM_CS_EXT          0x02
#define TIM_ON              0x01

//enum timerType {
//                    ackTimer = 0,
//                    errorTimer = 1,
//                    okTimer = 2
//                };

static char timerOVFcount;

#define standbyTimeValue 0      // 3036 To get 0.5s time @ 1:8 prescaling and 16bit timer
#define standbyTime 10//20//154//20
#define doorOpenTime 3000
#define ackTimerVal 0//0xF000
#define errorTimerVal 0//0xFF00
#define okTimerVal 0//0xFF10
#define timer2BaseValue 45      // for 10ms time @ prescalar 0f 1:16 and a post scalar 0f 1:3 giving a full period of 12ms.

void setup_timer0();
void state_timer0(unsigned char state);
void updateTimer();
void ackKeyPressTimerTone(unsigned char noSoundFlag);
void errorTone(unsigned char noSoundFlag);
//void okTimerTone(unsigned char noSoundFlag);
void alarmTimerTone();
void initStandbyTimer();
void startTimer(unsigned int time_val);
void initDoorOpenTimer2();
void beginDoorOpenCheck();

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */



#endif  /*  __TIMER_H    */

