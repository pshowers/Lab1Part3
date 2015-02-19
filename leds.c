/*
* File:   leds.c
* Author: Phillip Showers
*
* Created on February 9 2015
*/

#include "p24fj64ga002.h"
#include "leds.h"

#define STOP_LED LATAbits.LATA0
#define RUN_LED LATAbits.LATA1

#define STOP 0
#define RUN 1

#define RELEASED 1
#define PRESSED 0

#define OFF 1;
#define ON 0;

void initLEDs()
{
//TODO: Initialize the appropriate pins to work with the LEDs
    TRISAbits.TRISA0 = 0; //Sets IO1 to an output
    TRISAbits.TRISA1 = 0; //Sets IO2 to an output

    //Enable the ODCs for the LEDs
    ODCAbits.ODA0 = 1;  //ODC for pin 2
    ODCAbits.ODA1 = 1;  //ODC for pin 3
    
    //Initialize the LEDs
    STOP_LED = OFF;
    RUN_LED = OFF;
}

void TurnOnLED(int led)
{
    if( led == STOP)
    {
    RUN_LED = OFF;  //Illuminates STOP LED
    STOP_LED = ON;
    }
    else if( led == RUN) 
    {
    RUN_LED = ON;  //Illuminates RUN LED
    STOP_LED = OFF;
    }
}