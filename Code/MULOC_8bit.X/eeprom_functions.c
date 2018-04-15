
#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000
#endif
#include <xc.h>
#include "eeprom_functions.h"

void eepromWriteCode(unsigned char eepromWriteAddr, unsigned char codeLenght, char *codeBuffer)
{
    writeEEPROM(eepromWriteAddr, codeLenght);    // Store no. of chars in code
    for(unsigned char i=0; i <= codeLenght; i++)
    {
        eepromWriteAddr++;
        writeEEPROM(eepromWriteAddr, *(codeBuffer + i));
    }
}

void writeEEPROM(unsigned char address, unsigned char datas)
{
    unsigned char gie_Status;

    while(EECON1bits.WR);        // check the WR bit to see if a WR is in progress
    EEADR = address;             // Write the address to EEADR.
    EEDATA = datas;              // load the 8-bit data value to be written in the EEDATA register.
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.WREN = 1;         // Set the WREN bit to enable eeprom operation.
    gie_Status = INTCONbits.GIE; // Copy the current Interrupt state
    INTCONbits.GIE = 0;          // Disable the interrupts
    EECON2 = 0x55;               // Execute the special instruction sequence
    EECON2 = 0xaa;               // Refer the datasheet for more info
    EECON1bits.WR = 1;           // Set the WR bit to trigger the eeprom write operation.
    INTCONbits.GIE = gie_Status; // Restore the interrupts
    EECON1bits.WREN = 0;         // Disable the EepromWrite
}

unsigned char readEEPROM(unsigned char address)
{
  EEADR = address;      //Address to be read
  EECON1bits.EEPGD = 0; //Selecting EEPROM Data Memory
  EECON1bits.RD = 1;    //Initialise read cycle
  return EEDATA;        //Returning data
}
