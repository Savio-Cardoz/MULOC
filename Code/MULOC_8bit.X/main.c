/* 
 * File:   main.c
 * Author: Cardoz
 *
 * Created on December 13, 2016, 11:40 PM
 */
// CONFIG1L
#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = INTOSCIO_EC// Oscillator Selection bits (Internal oscillator, port function on RA6, EC used by USB (INTIO))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = ON         // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = OFF     // CCP2 MUX bit (CCP2 input/output is multiplexed with RB3)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config ICPRT = OFF       // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <htc.h>
#include "main.h"
#include "Keypad4x4.h"
#include "timer.h"
#include "eeprom_functions.h"

#define MAX_CODE_SIZE 10
#define MAX_NUM_OF_LOCKS 21
#define True 1
#define False 0

typedef enum {
    NORMAL = 0,
    UID_ENTERED = 1,
    PSWD_ENTERED
}systemState_t;

/* Convert elements in a array to a 8bit number 
   Arguments: arr - Array of elements to be made to a number
              num_of_elements - A maximum of 3 elements can be used to make the number
              number - the converted 8bit number. Maximum 255
 
   Returns: True - Conversion passed
            False - Conversion failed
*/
unsigned char arr_to_num(unsigned char* arr, unsigned char num_of_elements, unsigned char* number)
{
    unsigned char validity = False;
    *number = 0;
    /* only three elements of the array will be used */
    if(num_of_elements > 3)
        return validity;
    /* Checking elements less than 255 */
    if((num_of_elements == 3) && (arr[0] == 2) && (arr[1] == 5) && (arr[2] == 6))
            return validity;
    for(unsigned char a = 0; a < num_of_elements; a++)
    {
        *number = (*number * 10) + arr[a];
    }
    validity = True;
    return validity;
}

/* Check if the locker number entered is within valid range */
unsigned char check_lock_num_validity(unsigned char lock_num)
{
    unsigned char validity = False;
    if(lock_num)
    {
        lock_num <= MAX_NUM_OF_LOCKS ? validity = True : validity = False;
    }
    return validity;
}

unsigned char check_password_presence(unsigned char lock_num)
{
    unsigned char validity = False;
    /* Interpret lock number into address in eeprom */
    unsigned char addr = (lock_num - 1) * 10;
    /* Read for a length value at the address. If a value, representing length of password is present, a password is present */
    char a = readEEPROM(addr);
    if(readEEPROM(addr))
        validity = True;
    return validity;
}

void interrupt ISR(void)
{   
    if(RBIF)
    {
        RBIF = 0;
        TIMERINTERRUPT = 0;
//        STATE_SLEEP = 0;
        timer1OVFcount = 0;
        T1CONbits.TMR1ON = 1;
//        INTCONbits.TMR0IE = 1;
        if(PORTB)
            NOP();
    }
    if(PIR1bits.TMR1IF)    
    {
        timer1OVFcount++;
        TMR1 = standbyTimeValue;
        if(timer1OVFcount == standbyTime)
        {
            TIMERINTERRUPT = 1;
            T1CONbits.TMR1ON = 0;
            STATE_SLEEP = 1;
//            INTCONbits.TMR0IE = 0;
        }
        PIR1bits.TMR1IF = 0;
    }
//    if(TMR2IF)
//    {
////        static unsigned int doorOpenTimeCount = doorOpenTime;
////        TMR2IF = 0;
////        if(DOORSWITCH == DOOR_SW_OPEN)
////        {
////            T2CONbits.TMR2ON = 0;           // if door has been closed stop the timer.
////            timer2OVFcount = 0;
////            DOORSTATE = 0;
////            DOTLSILENT = 0;
////            doorOpenTimeCount = doorOpenTime;   // Change time to initial allowance
////        }
////        else{
////            TMR2 = timer2BaseValue;
////            timer2OVFcount++;
////            if(timer2OVFcount == doorOpenTimeCount)
////            {
////                doorOpenTimeCount = 1000;   // After the inital door open time allowance, system should default to a 10sec door open alarm.
////                timer2OVFcount = 0;
////                T2CONbits.TMR2ON = 0;
////                DOORSTATE = 1;              // Door open Flag set
////                //INTCONbits.T0IE = 1;
////            }
////        }
//    }
//    if(T0IF)
//    {
//        T0IF = 0;
//        if(soundStuff.playSeq == okTimer)
//        {
//            if(soundStuff.duration)      // i.e. duration of playing sound track is not over
//            {
//                TMR0 = soundStuff.timerVal;
//                if(!soundStuff.noSoundFlag)
//                PORTAbits.RA3 = ~PORTAbits.RA3;
//                soundStuff.duration--;
//            }
//            else
//            {
//                //OPTION_REGbits.T0CS = 1; // stop timer0
//                BUZZER = OFF_BUZZER;
//            }
//        }
//        else if(soundStuff.playSeq == ackTimer)
//        {
//            if(soundStuff.duration)      // i.e. duration of playing sound track is not over
//            {
//                TMR0 = soundStuff.timerVal;
//                if(!soundStuff.noSoundFlag)
//                PORTAbits.RA3 = ~PORTAbits.RA3;
//                soundStuff.duration--;
//            }
//            else
//            {
//                //OPTION_REGbits.T0CS = 1; // stop timer0
//                BUZZER = OFF_BUZZER;
//            }
//        }
//        else if(soundStuff.playSeq == errorTimer)
//        {
//            if(soundStuff.duration)      // i.e. duration of playing sound track is not over
//            {
//                static char tempSoundVal;
//
//                TMR0 = ++tempSoundVal;
//                if(!soundStuff.noSoundFlag)
//                PORTAbits.RA3 = ~PORTAbits.RA3;
//                soundStuff.duration--;
//            }
//            else
//            {
//                //OPTION_REGbits.T0CS = 1; // stop timer0
//                BUZZER = OFF_BUZZER;
//            }
//        }
//        if(!soundStuff.duration)
//        {
//            //OPTION_REGbits.T0CS = 1; // stop timer0
//            BUZZER = OFF_BUZZER;
////            ERROR_LED = 0;
//        }
//    }
}

void initControllerIO()
{
    ADCON1 = 0x0F;      // All digital pins
    CMCON = 0x07;       // No comparator output on the pins
    TRISAbits.RA0 = 0;  // Buzzer output
}

void beep()
{
    BUZZER_ON;
    __delay_ms(10);
    BUZZER_OFF;
}

void beep_ok()
{
    BUZZER_ON;
    __delay_ms(2);
    BUZZER_OFF;
    __delay_ms(1);
    BUZZER_ON;
    __delay_ms(2);
    BUZZER_OFF;
}
//
//void handleSleep()
//{
//    /********   Intentional introduction of BUG   Remove @ final release  *************/
//    // static sleepCounter;
//    // if(sleepCounter++ > 80)
//    // {
//    //     POWER_LED = ON;
//    //     while(1);
//    // }
//    /*********************************************************************************/
//
//    CMCON = 0x07;       // Comparators switched OFF
//    //VRCON = 0x00;       // Voltage Ref disabled
//    TRISA = 0x10;   // Set as outputs
//    PORTA = 0x38;       // 0x38 to keep LED light also OFF ,0x78;
//    TRISB = 0xF0;
//    ////OPTION_REG = 0x7F;   //0x7F;// activate pull ups
//    
//    PORTB = 0xF0;  
//    INTCON = 0x68;  //0x48;
//    PIE1bits.TMR1IE = 1;    // Enabling Timer1 interrupt for standby countdown
//    PIE1bits.TMR2IE = 1;    // Enabling Timer2 interrupt for door open alarm indication.
//    PIR1 = 0x00;            // Clearing any flags already SET
//    
//    for(unsigned char i = 0; i <= 24; i++)   // Flush the keyBuffer after timeout.
//    {
//        keyBuffer[i] = 0;
//    }
//    keyPressCount = 0;
//    bufferPtr = 0;              // Solution to Defect no. 12
//    VALIDCODEINDICATOR = 0;         // Timeout clear the flag. System cannot be used to reset after period has passed.
//    PASSCODECONFIRM = 0;
//    LEDSTATUS = 0;
//    
//    ei();
//    SLEEP();
//    NOP();
//}
void clear_keyArray(unsigned char* arr)
{
    for(unsigned char a = 0; a < MAX_CODE_SIZE; a++)
        arr[a] = 0;
}

int main() 
{   
    keypadStatus currentKeypadStatus;
    systemState_t currentSystemState = NORMAL;
    unsigned char keypressArray[MAX_CODE_SIZE];
    unsigned char numOfKeysPressed = 0;
    unsigned char locker_num = 0;
    unsigned char addr = 0;
    initControllerIO();
    initKeypad();
    initStandbyTimer();
    
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;     // Enable interrupts
////    initDoorOpenTimer2();
//    systemStateReg2 = 0;
//
//    checkFirstFlash();
//    wrongCodeEnteredCount = eeprom_read(WRNGCODEENTRIESADDR);
//    systemStateReg = eeprom_read(SYSTEMSTATEADDR);
//    
//    KEYPADLOCK = 0;
//
//    handleSleep();
//    
    /* Clear the keypressArray */
    clear_keyArray(keypressArray);
    
    writeEEPROM(0, 0x01);
    writeEEPROM(1, 0x01);
    char a = readEEPROM(0);
    
    while(1)
    {
        // Scan the keypad for keys pressed
        getKey(&currentKeypadStatus);
        // Identify and store any numeric key pressed. And for any special key perform the necessary actions
        if(currentKeypadStatus.keyPressIndicator == True)
        {
            beep();
            if(currentKeypadStatus.keyPressed == SP_FUNC_A)
            {
                
            }
            else if(currentKeypadStatus.keyPressed == SP_FUNC_B)
            {
                
            }
            else if(currentKeypadStatus.keyPressed == SP_FUNC_ENTER)
            {
                /* Check state of system */
                if(currentSystemState == NORMAL)
                {
                    /* convert the array to a number*/
                    if(False == arr_to_num(keypressArray, numOfKeysPressed, &locker_num))
                    {
                        /* Send error code */
                        break;
                    }
                    /* User has entered number of locker. Check validity of the locker number entered */
                    else if(False == check_lock_num_validity(locker_num))
                    {
                        /* Send error code */
                        break;
                    }
                    else
                    {
                        beep_ok();
                        currentSystemState = UID_ENTERED;
                    }
                }
                else if(currentSystemState == UID_ENTERED)
                {
                    /* Compare the entered code with that saved previously */
                    if(False == check_password_presence(locker_num))
                    {
                        /* No password exists. Send error code */
                        break;
                    }
                    else 
                    {
                        addr = (locker_num - 1) * 10;
                        if(False == passcodeCompare(keypressArray, numOfKeysPressed, addr))
                        {
                            /* Send error code */
                            break;
                        }
                        else
                        {
                            /* Access OK */
                        }
                    }
                }
                clear_keyArray(keypressArray);
                numOfKeysPressed = 0;
            }
            else if(currentKeypadStatus.keyPressed == SP_FUNC_OFF)
            {
                
            }
            else if(currentKeypadStatus.keyPressed == SP_FUNC_ON)
            {
                if(currentSystemState == UID_ENTERED)
                {
                    /* Check for presence of a password */
                    if(False == check_password_presence(locker_num))
                    {
                        /* Save the entered code as the password */
                        addr = (locker_num - 1) * 10;
                        eepromWriteCode(addr, numOfKeysPressed, keypressArray);
                        break;
                    }
                    else
                    {
                        /* Use the array to save the password */
                        
                    }
                }
                clear_keyArray(keypressArray);
                numOfKeysPressed = 0;
            }
            else 
            {
                keypressArray[numOfKeysPressed++] = currentKeypadStatus.keyPressed;
                if(numOfKeysPressed > MAX_CODE_SIZE)
                {
                    numOfKeysPressed = 0;
                    // Give a audible indication
                }
            }
            
            while(buttonPressedIndicator());    // Wait for release of button
        }
      
////        if(STATE_SLEEP && (DOORSWITCH == DOOR_SW_OPEN)) // Not to enter sleep mode if Safe Door is open. Due to Door open timer running.
//        if(STATE_SLEEP)         // Modified as requirement changed to remove Door_Open_Alarm
//        {
//            handleSleep();
//            STATE_SLEEP = 0;
//        }
//        if(DOORSTATE)
//        {
////            doorOpenAlarm(DOTLSILENT);
//        }
//        /*if((DOORSWITCH == DOOR_SW_CLOSED) && VALIDCODEINDICATOR)						// edited for Hotel Application 
//        {
//            if(LASTPASSCODE == 3)
//            {
//                eepromWriteCode(CODE1ADDR, defaultPassCodeLenght, defaultPassCode);
////                eepromWriteCode(CODE2ADDR, defaultPassCode2Lenght, defaultPassCode2);
//                LASTPASSCODE = 0;
//            }
//            if(prevCodeUsed == LASTPASSCODE)
//                doorOpenedTone(1);                   // Solution to defect no. 1
//            else{
//                doorOpenedTone(2);
//                prevCodeUsed = LASTPASSCODE;
//            }
////            beginDoorOpenCheck();
//            //ACCESS_LED = OFF;															// edited for Hotel Application
//            VALIDCODEINDICATOR = OFF;
//            LEDSTATUS = ON;
//            timer1OVFcount = 0;
//        }
//        
//        if(LEDSTATUS && (DOORSWITCH == DOOR_SW_CLOSED))
//            LED_CONTROL = ON;           // Switch ON the LED when Door is opened
//        
////        if(DOORSWITCH == DOOR_SW_OPEN)
////            LED_CONTROL = OFF;*/
//        
//        if(monitorBattery())
//        {
//            POWER_LED = 1;
//        }
//
//        if((DOORSWITCH == DOOR_SW_CLOSED) && keyPressCount)
//        {
//            SET_LED = ON;
//        	while(DOORSWITCH == DOOR_SW_CLOSED);
//        	if(DOORSWITCH == DOOR_SW_OPEN)
//        	{
//                if(LASTPASSCODE == 1)
//                    eepromWriteCode(CODE1ADDR, keyPressCount, keyBuffer);
//                if(LASTPASSCODE == 2)
//                    eepromWriteCode(CODE2ADDR, keyPressCount, keyBuffer);
//
//        		keyPressCount = 0;
//                bufferPtr = 0; 
//        		soundConfigParam(200, 1000, KEYPADSILENT, okTimer);
//                __delay_ms(100);
//                soundConfigParam(200, 1000, KEYPADSILENT, okTimer);
//                __delay_ms(100);
//                soundConfigParam(200, 1000, KEYPADSILENT, okTimer);
//                __delay_ms(100);
//                soundConfigParam(200, 1000, KEYPADSILENT, okTimer);
//        	}
//            SET_LED = OFF;
//
//        }
//        
//        currentKeypadStatus = getKey();                     // Scan for any button presses
//        
//        if(currentKeypadStatus.keyPressIndicator)           // 
//        {
//            if((currentKeypadStatus.keyPressed == SP_FUNC_ON) && (!KEYPADLOCK))
//            {
//                if((DOORSWITCH == DOOR_SW_OPEN) && keyPressCount)               // iF DOOR IS CLOSED ENTER ACCESS MODE
//                {
//                    if(passcodeCompare(keyBuffer, keyPressCount, CODE1ADDR))
//                        LASTPASSCODE = code1;
//                    else if(passcodeCompare(keyBuffer, keyPressCount, CODE2ADDR))
//                       LASTPASSCODE = code2;
//                    else if(passcodeCompare(keyBuffer, keyPressCount, FACTORYRESETCODEADDR))
//                    {
//                        LASTPASSCODE = master1;
//                        eepromWriteCode(CODE1ADDR, defaultPassCodeLenght, defaultPassCode);
//                        eepromWriteCode(CODE2ADDR, defaultPassCode2Lenght, defaultPassCode2);
//                    }
//                    else if(!VALIDCODEINDICATOR)
//                    {
//                        wrongCodeEnteredCount++;
//                        ACCESS_LED = OFF;
//                        ERROR_LED = 1;
//                        if(wrongCodeEnteredCount >= MAXWRONGCODEENTRIES)
//                        {
//                            SYSTAMPERED = 1;
//                            soundConfigParam(0, 5000, 0, errorTimer);
//                        }
//                        //errorTimerTone();
//                        else soundConfigParam(0, 5000, KEYPADSILENT, errorTimer);
//                    }
//                    eeprom_write(WRNGCODEENTRIESADDR, wrongCodeEnteredCount);
//                    keyPressCount = 0;
//                    bufferPtr = 0;          // Flushing of buffer to be better implemented
//                }
//            }
//
//            else if((currentKeypadStatus.keyPressed == NO_KEY_TUNE) && (!KEYPADLOCK))
//            {
//                KEYPADSILENT =~ KEYPADSILENT;            // check if any visual confirmation is required ????
//                ackKeyPress();
//                while(buttonPressedIndicator());
//            }
//            
//            else if((currentKeypadStatus.keyPressed == SP_FUNC_A) && (!KEYPADLOCK))
//            {
//                // pressing 'A' clears the buffer of any keys pressed before and also clears the count;
//                ackKeyPress();
//                keyPressCount = 0;
//                for(unsigned char z = 0; z <= 24; z++)   // Flush the keyBuffer after timeout.
//                keyBuffer[z] = 0;
//                while(buttonPressedIndicator());
//                bufferPtr = 0;
//            }
//            
//            else if(currentKeypadStatus.keyPressed == SP_FUNC_B)
//            {
//                // if(!KEYPADLOCK)
//                // ackKeyPress();
//                // while(((PORTB & 0x80) == 0x00) && !TIMERINTERRUPT);
//                //     if(TIMERINTERRUPT)
//                //     {
//                //         KEYPADLOCK =~ KEYPADLOCK;
//                //         POWER_LED = 1;
//                //         __delay_ms(500);
//                //         POWER_LED = 0;
//                //     }
//                while(buttonPressedIndicator());
//                SET_LED = 0;
//            }
//
//            else if((currentKeypadStatus.keyPressed == SP_FUNC_OFF) && (!KEYPADLOCK))
//            {
//                ACCESS_LED = 0;
////                DOTLSILENT = 1;         // No alarm for Door Open TOO Long indication
//                SET_LED = 1;
////                 while(((PORTB & 0x80) == 0x00) && !TIMERINTERRUPT);   //Solution to defect no. 6
////                 if(VALIDCODEINDICATOR && TIMERINTERRUPT)
////                 {
////                     eepromWriteCode(CODE1ADDR, defaultPassCodeLenght, defaultPassCode);
//// //                    eepromWriteCode(CODE2ADDR, defaultPassCode2Lenght, defaultPassCode2);
////                     systemStateReg = 0;
////                     okTimerTone();
////                     __delay_ms(50);
////                     okTimerTone();
////                     __delay_ms(100);
////                     okTimerTone();
//// //                    DOTLSILENT = 0;
////                 }
//                while(buttonPressedIndicator());
//                SET_LED = 0;
//                LEDSTATUS = 1;
//            }
//            
//            else if((currentKeypadStatus.keyPressed == SP_FUNC_ENTER) && (!KEYPADLOCK))
//            {
//                ACCESS_LED = 0;
//                if(!PASSCODECONFIRM && VALIDCODEINDICATOR && keyPressCount)                        // Solution to defect 3
//                {
//                    updateTempCodeBuffer(keyBuffer, keyPressCount);
//                    PASSCODECONFIRM = 1;
//                }
//                else if(PASSCODECONFIRM  && VALIDCODEINDICATOR && keyPressCount)
//                {
//                    if(confirmationCodeCheck(tempCodeBuffer, keyBuffer, tempCodeKeyPressCount, keyPressCount))
//                    {
////                        if(LASTPASSCODE == 1)
////                            eepromWriteCode(CODE1ADDR, keyPressCount, keyBuffer);
////                        if(LASTPASSCODE == 2)
////                            eepromWriteCode(CODE2ADDR, keyPressCount, keyBuffer);
//                        if(LASTPASSCODE == 3)
//                            eepromWriteCode(FACTORYRESETCODEADDR, keyPressCount, keyBuffer);
//                        okTimerTone();
//                        __delay_ms(50);
//                        okTimerTone();
//                    }
//                    else {
//                        ERROR_LED = 1;
//                        errorTimerTone();
//                    }
//                    PASSCODECONFIRM = 0;
//                    VALIDCODEINDICATOR = 0;
//                }
//                    __delay_ms(500);
//                    SET_LED = 0;              // SET_LED is switched OFF which was ON when keys are pressed and Door is open(DOORSWITCH is closed)
//                    keyPressCount = 0;
//                    bufferPtr = 0;          // Flushing Buffer, need better implementation...
//            }
//            else if(!KEYPADLOCK)
//                {
//                    if(bufferPtr <= 24)
//                    {
//                        keyBuffer[bufferPtr] = currentKeypadStatus.keyPressed;
//                        ackKeyPress();
//                        keyPressCount++;
//                        bufferPtr++;
//                    }
//                    else if(bufferPtr >= 25)             // Incrementing the buffer and checking overflow
//                    {
//                        bufferPtr = 0;
//                        keyPressCount = 0;
//                        ERROR_LED = 1;
//                        __delay_ms(60);
//                        ERROR_LED = 0;
//                    }
//                    while(buttonPressedIndicator());
//                }
//        }
//        currentKeypadStatus.keyPressIndicator = False;
//        eeprom_write(SYSTEMSTATEADDR, systemStateReg);
    }
    return (EXIT_SUCCESS);
}

unsigned char passcodeCompare(char *keybuffer1, char compareLenght, unsigned char compareAddr)
{
    unsigned char validity = False;
    unsigned char storedCodeLenght = readEEPROM(compareAddr);      // The addr provided points to the lenght of the stored code.
    if(compareLenght != storedCodeLenght)
    {
        VALIDCODEINDICATOR = False;
        validity = False;
        /* Send error */
    }
    else
    {
        for(unsigned char i = 0; i <= (compareLenght - 1); i++)
        {
            if(*keybuffer1 == readEEPROM(++compareAddr))
            {
                keybuffer1++;
                validity = True;
            }
            else 
            {
                VALIDCODEINDICATOR = 0;
                validity = False;
                break;
            }              
        }
    }
    return validity;
}

///**************************************************************************************/
///*          Function to give a visual feedback of a key pressed           
//            Arguments Taken: None
//            Returns: None
// */
//void ackKeyPress()
//{
//    ACCESS_LED = OFF;                   // After granting access if any keypressed
//    if(SYSTAMPERED)
//    {
//        ERROR_LED = 1;
//        SET_LED = 1;                    // Solution to Defect no. 4
//        soundConfigParam(0, 5000, 0, errorTimer);
////        soundStuff.timerVal = 0;
////        soundStuff.duration = 5000;
////        soundStuff.noSoundFlag = 0;
////        soundStuff.playSeq = errorTimer;
////        //OPTION_REG = 0x50;
//    }
//    else{
//        SET_LED = 1;
//        if(wrongCodeEnteredCount)
//            ERROR_LED = 1;
//        soundConfigParam(200, 500, KEYPADSILENT, ackTimer);
////        soundStuff.timerVal = 200;
////        soundStuff.duration = 500;
////        soundStuff.noSoundFlag = KEYPADSILENT;
////        soundStuff.playSeq = ackTimer;
////        //OPTION_REG = 0x50; 
//        }
//    //if(DOORSWITCH == DOOR_SW_OPEN)                // As per requirement, if door is closed, SET LED blinks.
//        {                                         // If Door is open, new code is being entered for which LED should
//            __delay_ms(100);                      // remain ON until either timeout or ENTER is pressed.
//            SET_LED = 0;                          //---- Solution to Defect no. 4. This if condition shifted out
//        }                                         //---- of else, to always clear SET LED.  
//}
//
//void okTimerTone()
//{
//    soundConfigParam(200, 500, KEYPADSILENT, okTimer);
////    soundStuff.timerVal = 200;
////    soundStuff.duration = 1000;
////    soundStuff.noSoundFlag = KEYPADSILENT;
////    soundStuff.playSeq = okTimer;
////    //OPTION_REG = 0x50;
//}
//
//
//void doorTimerTone()
//{
//    soundConfigParam(200, 500, KEYPADSILENT, errorTimer);
//    soundStuff.timerVal = 200;
//    soundStuff.duration = 500;
//    soundStuff.noSoundFlag = KEYPADSILENT;
//    soundStuff.playSeq = okTimer;
//    //OPTION_REG = 0x50;
//}
//
//void errorTimerTone()
//{
//    soundConfigParam(0, 5000, KEYPADSILENT, errorTimer);
////    soundStuff.timerVal = 0;
////    soundStuff.duration = 5000;
////    soundStuff.noSoundFlag = KEYPADSILENT;
////    soundStuff.playSeq = errorTimer;
////    //OPTION_REG = 0x50;
//}
//
//void soundConfigParam(unsigned int timerStartValue, unsigned int timerDuration, unsigned char soundONOFF, unsigned char soundType)
//{
//    soundStuff.timerVal = timerStartValue;
//    soundStuff.duration = timerDuration;
//    soundStuff.noSoundFlag = soundONOFF;
//    soundStuff.playSeq = soundType;
//    //OPTION_REG = 0x50;
//}

//void doorOpenAlarm(unsigned char alarmMute)
//{
//    if(DOORSWITCH == DOOR_SW_CLOSED)    // Door is open
//    {
//        INTCONbits.T0IE = 0;
//        ERROR_LED = 1;
//        errorTone(alarmMute);
//        INTCONbits.T0IE = 1;        // Solution to defect no. 13
//        BUZZER = OFF_BUZZER;
//        ERROR_LED = 0;
//        DOORSTATE = 0;          //Door is assumed closed to start dotl timer for 30secs.
//        T2CONbits.TMR2ON = 1;   // start DOTL check timer2
//    }
//}

//void setDefaultUserCodes()
//{
//    eepromWriteCode(CODE1ADDR, defaultPassCodeLenght, defaultPassCode);
//    eepromWriteCode(CODE2ADDR, defaultPassCode2Lenght, defaultPassCode2);
//}

//void setDefaultFactoryCodes()
//{
//    eepromWriteCode(FACTORYRESETCODEADDR, factoryResetCodeLenght, factoryResetCode);
//}

//void checkFirstFlash()
//{
//    if(eeprom_read(0x7F) != 0xaa)
//    {
//        eepromWriteCode(CODE1ADDR, defaultPassCodeLenght, defaultPassCode);
//        eepromWriteCode(CODE2ADDR, defaultPassCode2Lenght, defaultPassCode2);
//        eepromWriteCode(FACTORYRESETCODEADDR, factoryResetCodeLenght, factoryResetCode);
//        wrongCodeEnteredCount = 0;
//        systemStateReg = 0;
//        eeprom_write(WRNGCODEENTRIESADDR, wrongCodeEnteredCount);
//        eeprom_write(SYSTEMSTATEADDR, systemStateReg);
//        eeprom_write(0x7F, 0xaa);
//    }
//}

//void updateTempCodeBuffer(char *keybuffer, char keycount)
//{
//    tempCodeKeyPressCount = keycount;
//    for(char i = 0; i <= keycount; i++)
//    {
//        tempCodeBuffer[i] = *keybuffer;
//        keybuffer++;
//    }
//    okTimerTone();
//}
//
//char confirmationCodeCheck(char *keybuffer1, char *keybuffer2, char keycount1, char keycount2)
//{
//    if(keycount1 == keycount2)
//    {
//        for(unsigned char i=0; i <= (keycount2 - 1); i++)
//        {
//            if(*keybuffer1 == *keybuffer2)
//            {
//                keybuffer1++;
//                keybuffer2++;
//            }
//            else return 0; 
//        }
//        return 1;
//    }
//    else return 0;
//}

//void doorOpenedTone(unsigned char tuneNumber)
//{
//    DOTLSILENT = 0;
//    switch(tuneNumber){
//        case 1: okTimerTone();
//                __delay_ms(90);
//                okTimerTone();
//                break;
//                    
//        case 2: doorTimerTone();
//                __delay_ms(100);
//                doorTimerTone();
//                __delay_ms(40);
//                doorTimerTone();
//                __delay_ms(100);
//                doorTimerTone();
//                break;
//    }
//}

//char monitorBattery()
//{
//    PORTBbits.RB3 = 1;          // Set the Pin to allow voltage at input of MC340 voltage sense IC.
//    __delay_ms(50);
//    if(BATT_INP == 0)            // Batt is below 4.5v as per the specifications of MC340
//        return 1;
////    else if(BATT_INP == 1)
//        return 0;
//}