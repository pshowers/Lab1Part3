/*
 * File:   lcd.c
 * Authors:
 *
 * Created on December 31, 2014, 1:39 PM
 */

#include "p24fj64ga002.h"
#include "lcd.h"
#include "timer.h"

#define LCD_DATA   LATB
#define LCD_RS  LATBbits.LATB7
#define LCD_E   LATBbits.LATB6
#define LCD_WRITE_DATA    1
#define LCD_WRITE_CONTROL 0

#define TRIS_D7  TRISBbits.TRISB15
#define TRIS_D6  TRISBbits.TRISB14
#define TRIS_D5  TRISBbits.TRISB13
#define TRIS_D4  TRISBbits.TRISB12
#define TRIS_RS  TRISBbits.TRISB7
#define TRIS_E   TRISBbits.TRISB6

#define UPPER 0
#define LOWER 1

/* This function should take in a two-byte word and writes either the lower or upper
 * byte to the last four bits of LATB. Additionally, according to the LCD data sheet
 * It should set LCD_RS and LCD_E with the appropriate values and delays.
 * After these commands are issued, there should be a delay.
 * The command type is a simplification. From the data sheet, the RS is '1'
 * when you are simply writing a character. Otherwise, RS is '0'.
 */
void writeFourBits(unsigned char word, unsigned int commandType, unsigned int delayAfter, unsigned int lower)
{
    if(lower)
    {
        LATBbits.LATB15 = (word & 0x08) >> 3;
        LATBbits.LATB14 = (word & 0x04) >> 2;
        LATBbits.LATB13 = (word & 0x02) >> 1;
        LATBbits.LATB12 = (word & 0x01);
    }
    else
    {


        LATBbits.LATB15 = (word & 0x80) >> 7;
        LATBbits.LATB14 = (word & 0x40) >> 6;
        LATBbits.LATB13 = (word & 0x20) >> 5;
        LATBbits.LATB12 = (word & 0x10) >> 4;
    }
    LCD_RS = commandType; delayUs(1);
    LCD_E = 1;  delayUs(1); //minimum 230 ns
    LCD_E = 0;  delayUs(1);
    delayUs(delayAfter);
}

/* Using writeFourBits, this function should write the two bytes of a character
 * to the LCD.
 */
void writeLCD(unsigned char word, unsigned int commandType, unsigned int delayAfter)
{
    //TODO:
    writeFourBits(word, commandType, delayAfter, UPPER);
    writeFourBits(word, commandType, delayAfter, LOWER);
}

/* Given a character, write it to the LCD. RS should be set to the appropriate value.
 */
void printCharLCD(char c)
{
    //TODO:
    writeLCD(c, LCD_WRITE_DATA, 46);
}
/*Initialize the LCD
 */
void initLCD(void)
{
    // Setup D, RS, and E to be outputs (0).

    TRIS_RS = 0;    //Sets RS to be output
    TRIS_E = 0;     //Sets Enable to be output
    TRIS_D7 = 0;
    TRIS_D6 = 0;
    TRIS_D5 = 0;
    TRIS_D4 = 0;
    // Initilization sequence utilizes specific LCD commands before the general configuration
    // commands can be utilized. The first few initilition commands cannot be done using the
    // WriteLCD function. Additionally, the specific sequence and timing is very important.

    delayUs(1500);

    writeFourBits(0x03, LCD_WRITE_CONTROL, 4100, LOWER);

    writeFourBits(0x03, LCD_WRITE_CONTROL, 100, LOWER);

    writeLCD(0x32,LCD_WRITE_CONTROL, 40);
    // 4-bit mode initialization is complete. We can now configure the various LCD
    // options to control how the LCD will function.

    writeLCD(0x28,LCD_WRITE_CONTROL, 40);

    // TODO: Display On/Off Control
        // Turn Display (D) Off

    writeLCD(0x08,LCD_WRITE_CONTROL, 40);

    // TODO: Clear Display (The delay is not specified in the data sheet at this point.
    //You really need to have the clear display delay here.

    writeLCD(0x01,LCD_WRITE_CONTROL, 1640);

    // TODO: Entry Mode Set
        // Set Increment Display, No Shift (i.e. cursor move)
    writeLCD(0x06,LCD_WRITE_CONTROL, 40);

// TODO: Display On/Off Control
  // Turn Display (D) On, Cursor (C) Off, and Blink(B) Off
    writeLCD(0x0C,LCD_WRITE_CONTROL, 40);
}


/*
 * You can use printCharLCD here. Note that every time you write a character
 * the cursor increments its position automatically.
 * Since a string is just a character array, try to be clever with your use of pointers.
 */
void printStringLCD(const char* s)
{
    //TODO:
    
    int i = 0;

    while( s[i] != 0)
    {
       printCharLCD(s[i]);
       i ++;
    }
}

/*
 * Clear the display.
 */
void clearLCD()
{
    writeLCD(0x01,LCD_WRITE_CONTROL, 1640);
}

/*
 Use the command for changing the DD RAM address to put the cursor somewhere.
 */
void moveCursorLCD(unsigned char x, unsigned char y)
{
unsigned char ddAddress = 0;

if( y == 1)
{
    ddAddress = 0x40 | x;
}
 writeLCD(ddAddress,LCD_WRITE_CONTROL, 40);
/*NOTE: If the above IF statement does not work, the commented section below will work for where to set the cursor.*/
//    writeLCD(0xC0,LCD_WRITE_CONTROL, 40); //This will move the cursor to the second row for the Timer display.
}

/*
 * This function displays Running: on the first row of the LCD display.
 */
void lcdRunState()
{
    printStringLCD("Running:");
}

/*
 * This function displays Stopped: on the first row of the LCD display.
 */
void lcdStopState()
{
    printStringLCD("Stopped:");
}

/*
 * This function will move the cursor to the second row of the LCD
 * and display the time of the stopwatch on the LCD. Then it will increment the correct
 * time after each .01 second.
 */
void getTimeString()
{
    moveCursorLCD(1,0);
    printStringLCD("%d:%d:%d%d");
}