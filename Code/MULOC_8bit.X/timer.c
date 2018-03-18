#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif
#include<htc.h>
#include<xc.h>
#include"timer.h"

enum timerType currentTimerState;
struct configSound soundOut;
void initTimer();
void startTimer(unsigned int time_val);

//void updateTimer()
//{
//    timerOVFcount++;
//    if(timerOVFcount == 1)
//        BUZZER = ON_BUZZER;
//
//    if(timerOVFcount == 2)
//    {
//        BUZZER = OFF_BUZZER;
//    }
//
//    switch(currentTimerState)
//    {
//        case 0: TMR0 = ackTimerVal;
//        break;
//
//        case 1: TMR0 = errorTimerVal;
//        break;
//
//        case 2: TMR0 = okTimerVal;
//        break;
//    }
//    T0IF = 0;
//}

//void ackKeyPressTimerTone(unsigned char noSoundFlag)
//{
//    currentTimerState = ackTimer;
//    TMR0 = ackTimerVal;
//    OPTION_REGbits.T0CS = 0;    // Starts the timer0 clock
//    for(unsigned char i = 0; i<=100; i++)
//    {
//        while(!T0IF);
//        T0IF = 0;
//        if(!(i%2))
//            if(!noSoundFlag)
//        PORTAbits.RA3 = ~PORTAbits.RA3;
//    }
//}

//void errorTone(unsigned char noSoundFlag)
//{
//    currentTimerState = errorTimer;
//    TMR0 = errorTimerVal;
//    OPTION_REGbits.T0CS = 0;    // Starts the timer0 clock
//    for(unsigned int i = 0; i<=5000; i++)   // 100 for ack
//    {
//        while(!T0IF);
//        TMR0 = errorTimerVal;
//        T0IF = 0;
//        if(!(i%2))                          // %2 for keypress ack
//        {
//            if(!noSoundFlag)
//                PORTAbits.RA3 = ~PORTAbits.RA3;
//        }
//    }
//    OPTION_REGbits.T0CS = 1;    // Stops the timer0 clock
//}

//void okTimerTone(unsigned char noSoundFlag)
//{
//    currentTimerState = okTimer;
//    TMR0 = okTimerVal;
//    OPTION_REGbits.T0CS = 0;    // Starts the timer0 clock
//    for(unsigned int i = 0; i<=300; i++)   // 300 for ok
//    {
//        while(!T0IF);
//        T0IF = 0;
//        if(!(i%2))                          // %2 for ok
//            if(!noSoundFlag)
//        PORTAbits.RA3 = ~PORTAbits.RA3;
//    }
//    __delay_ms(60);
//    for(unsigned int i = 0; i<=300; i++)   
//    {
//        while(!T0IF);
//        T0IF = 0;
//        if(!(i%2))        
//            if(!noSoundFlag)
//        PORTAbits.RA3 = ~PORTAbits.RA3;
//    }
//}

void initStandbyTimer()
{
    T1CONbits.T1CKPS0 = 1;
    T1CONbits.T1CKPS1 = 1;
    TMR1 = standbyTimeValue;
    PIE1bits.TMR1IE = 1;        // Enable Timer1 interrupt
}

//void initDoorOpenTimer2()
//{
//    INTCONbits.PEIE = 1;
//    PIE1bits.TMR2IE = 1;
//    PIR1bits.TMR2IF = 0;
//    T2CON = 0x13;       //Prescalar 1:16 and Postscalar 1:3 for 12ms
//    TMR2 = timer2BaseValue;          // for 10ms
//}

//void beginDoorOpenCheck()
//{
//    T2CON = 0x13;       //Prescalar 1:16 and Postscalar 1:3 for 12ms
//    TMR2 = timer2BaseValue;          // for 10ms
//    T2CONbits.TMR2ON = 1;       // Start timer2 of 30secs to give a door open timeout alarm.
//}
//void initTimer0()
//{
//    
//}

//void alarmTimerTone()
//{
//    currentTimerState = errorTimer;
//    TMR0 = errorTimerVal;
//    OPTION_REGbits.T0CS = 0;    // Starts the timer0 clock
//    while(1)
//    {
//        for(unsigned int i = 0; i<=500; i++)   // 100 for ack
//        {
//            while(!T0IF);
//            T0IF = 0;
//            if(!(i%8))                          // %2 for keypress ack
//            PORTAbits.RA3 = ~PORTAbits.RA3;
//        }
//        for(unsigned int i = 0; i<=500; i++)   // 100 for ack
//        {
//            while(!T0IF);
//            T0IF = 0;
//            if(!(i%7))                          // %2 for keypress ack
//            PORTAbits.RA3 = ~PORTAbits.RA3;
//        }
//        for(unsigned int i = 0; i<=500; i++)   // 100 for ack
//        {
//            while(!T0IF);
//            T0IF = 0;
//            if(!(i%6))                          // %2 for keypress ack
//            PORTAbits.RA3 = ~PORTAbits.RA3;
//        }
//        for(unsigned int i = 0; i<=500; i++)   // 100 for ack
//        {
//            while(!T0IF);
//            T0IF = 0;
//            if(!(i%5))                          // %2 for keypress ack
//            PORTAbits.RA3 = ~PORTAbits.RA3;
//        }
//        for(unsigned int i = 0; i<=500; i++)   // 100 for ack
//        {
//            while(!T0IF);
//            T0IF = 0;
//            if(!(i%4))                          // %2 for keypress ack
//            PORTAbits.RA3 = ~PORTAbits.RA3;
//        }
//        for(unsigned int i = 0; i<=500; i++)   // 100 for ack
//        {
//            while(!T0IF);
//            T0IF = 0;
//            if(!(i%3))                          // %2 for keypress ack
//            PORTAbits.RA3 = ~PORTAbits.RA3;
//        }
//        for(unsigned int i = 0; i<=500; i++)   // 100 for ack
//        {
//            while(!T0IF);
//            T0IF = 0;
//            if(!(i%2))                          // %2 for keypress ack
//            PORTAbits.RA3 = ~PORTAbits.RA3;
//        }
//        __delay_ms(500);
//    }
//    
//}