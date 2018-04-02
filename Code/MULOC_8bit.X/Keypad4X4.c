#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000
#endif 

#include <htc.h>
#include "Keypad4x4.h"

/**************************************************************************************/
/*          Function to Initialize the controller port to which the 4X4 keypad is connected
 *          Rows are set as outputs and columns as inputs           
            Arguments Taken: None
            Returns:    None
 */
void initKeypad(void)
{
	KEYPAD_PORT_DIR = 0xF0;	// Last 4 pins (ROW) output, First 4 pins (COLOUMN) input
    KEYPAD_PORT_OUT = 0xF0;     // Initial value to wake up controller on Pin-change at the column inputs.
    INTCON2bits.RBPU = 0;   // Enabling pullups on PORTB specifically
    INTCONbits.RBIE = 1;    // Enable pin change interrupt on the keypad lines
}

/**************************************************************************************/
/*          Function to check key pressed           
            Arguments Taken: None
            Returns:    Key pressed indicator bit   member of structure keypadStatus
                        Key pressed value           member of structure keypadStatus
 */
void getKey(keypadStatus *KeypadStatus)                
{	
    enum button buttonPressed;
    
    KEYPAD_PORT_OUT = SCAN_ROW_1;
    if ((KEYPAD_PORT_IN & ROW_BUTTON_1_MASK) == 0x00)
    { 
        __delay_ms(5);
        if ((KEYPAD_PORT_IN & ROW_BUTTON_1_MASK) == 0x00)
        {
            buttonPressed = button_1;
            KeypadStatus->keyPressIndicator = True;
            KeypadStatus->keyPressed = buttonPressed;
            return;
        }
    }
    
    else if ((KEYPAD_PORT_IN & ROW_BUTTON_2_MASK) == 0x00)
    { 
        __delay_ms(5);
        if ((KEYPAD_PORT_IN & ROW_BUTTON_2_MASK) == 0x00)
        {
            buttonPressed = button_4;
            KeypadStatus->keyPressIndicator = True;
            KeypadStatus->keyPressed = buttonPressed;
            return;
        }
    }
    
    else if ((KEYPAD_PORT_IN & ROW_BUTTON_3_MASK) == 0x00)
    { 
        __delay_ms(5);
        if ((KEYPAD_PORT_IN & ROW_BUTTON_3_MASK) == 0x00)
        {
            buttonPressed = button_7;
            KeypadStatus->keyPressIndicator = True;
            KeypadStatus->keyPressed = buttonPressed;
            return;
        }
    }
    
    else if ((KEYPAD_PORT_IN & ROW_BUTTON_4_MASK) == 0x00)
    { 
        __delay_ms(5);
        if ((KEYPAD_PORT_IN & ROW_BUTTON_4_MASK) == 0x00)
        {
            buttonPressed = SP_FUNC_ON;
            KeypadStatus->keyPressIndicator = True;
            KeypadStatus->keyPressed = buttonPressed;
            return;
        }
    }

    KEYPAD_PORT_OUT = SCAN_ROW_2;
    if ((KEYPAD_PORT_IN & ROW_BUTTON_1_MASK) == 0x00)
    { 
        __delay_ms(5);
        buttonPressed = button_2;
        KeypadStatus->keyPressIndicator = True;
        KeypadStatus->keyPressed = buttonPressed;
        return;
    }
    
    else if ((KEYPAD_PORT_IN & ROW_BUTTON_2_MASK) == 0x00)
    { 
        __delay_ms(5);
        buttonPressed = button_5;
        KeypadStatus->keyPressIndicator = True;
        KeypadStatus->keyPressed = buttonPressed;
        return;
    }
    
    else if ((KEYPAD_PORT_IN & ROW_BUTTON_3_MASK) == 0x00)
    { 
        __delay_ms(5);
        buttonPressed = button_8;
        KeypadStatus->keyPressIndicator = True;
        KeypadStatus->keyPressed = buttonPressed;
        return;
    }
    
    else if ((KEYPAD_PORT_IN & ROW_BUTTON_4_MASK) == 0x00)
    { 
        __delay_ms(5);
        buttonPressed = button_0;
        KeypadStatus->keyPressIndicator = True;
        KeypadStatus->keyPressed = buttonPressed;
        return;
    }

    KEYPAD_PORT_OUT = SCAN_ROW_3;
    if ((KEYPAD_PORT_IN & ROW_BUTTON_1_MASK) == 0x00)
    { 
        __delay_ms(5);
        buttonPressed = button_3;
        KeypadStatus->keyPressIndicator = True;
        KeypadStatus->keyPressed = buttonPressed;
        return;
    }
    
    else if ((KEYPAD_PORT_IN & ROW_BUTTON_2_MASK) == 0x00)
    { 
        __delay_ms(5);
        buttonPressed = button_6;
        KeypadStatus->keyPressIndicator = True;
        KeypadStatus->keyPressed = buttonPressed;
        return;
    }
    
    else if ((KEYPAD_PORT_IN & ROW_BUTTON_3_MASK) == 0x00)
    { 
        __delay_ms(5);
        buttonPressed = button_9;
        KeypadStatus->keyPressIndicator = True;
        KeypadStatus->keyPressed = buttonPressed;
        return;
    }
    
    else if ((KEYPAD_PORT_IN & ROW_BUTTON_4_MASK) == 0x00)
    { 
        __delay_ms(5);
        buttonPressed = SP_FUNC_ENTER;
        KeypadStatus->keyPressIndicator = True;
        KeypadStatus->keyPressed = buttonPressed;
        return;
    }

    KEYPAD_PORT_OUT = SCAN_ROW_4;
    if ((KEYPAD_PORT_IN & ROW_BUTTON_1_MASK) == 0x00)
    { 
        __delay_ms(5);
        buttonPressed = SP_FUNC_A;
        KeypadStatus->keyPressIndicator = True;
        KeypadStatus->keyPressed = buttonPressed;
        return;
    }
    
    else if ((KEYPAD_PORT_IN & ROW_BUTTON_2_MASK) == 0x00)
    { 
        __delay_ms(5);
        buttonPressed = SP_FUNC_B;
        KeypadStatus->keyPressIndicator = True;
        KeypadStatus->keyPressed = buttonPressed;
        return;
    }
    
    else if ((KEYPAD_PORT_IN & ROW_BUTTON_3_MASK) == 0x00)
    { 
        __delay_ms(5);
        buttonPressed = NO_KEY_TUNE;
        KeypadStatus->keyPressIndicator = True;
        KeypadStatus->keyPressed = buttonPressed;
        return;
    }
    
    else if ((KEYPAD_PORT_IN & ROW_BUTTON_4_MASK) == 0x00)
    { 
        __delay_ms(5);
        buttonPressed = SP_FUNC_OFF;
        KeypadStatus->keyPressIndicator = True;
        KeypadStatus->keyPressed = buttonPressed;
        return;
    }
    
    else {
            KeypadStatus->keyPressIndicator = False;
            KeypadStatus->keyPressed = NONE;
            return;
    }
    
}

char buttonPressedIndicator()
{
    keypadStatus keypad_state;
    getKey(&keypad_state);
    return keypad_state.keyPressIndicator;
}
