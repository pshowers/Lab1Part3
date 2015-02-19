#include "p24fj64ga002.h"
#include "leds.h"

#define DIGITAL 1;
#define ANALOG 0;

/*****************************************************************
 * This function initializes SW1 to be an input pin and enables
 * the change notification for the switch.
*****************************************************************/
void initSW1()
{
    //TODO: Initialize the pin connected to the switch as an input.
    TRISBbits.TRISB5 = 1;  //Pin for SW1

    //Put the overall CN Interrupt flag down (set it to 0)
    IFS1bits.CNIF = 0;

    //Enable the overall change notification interrupt
    IEC1bits.CNIE = 1;

    /* Enable the change notification interrupt for the switch 1 connected to
     * the development board. This corresponds to CN27 according to the PIC24F
     * datasheet. The internal pull-up should not be used for the switch
     * on the development board because one is already there.
     */
    CNEN2bits.CN27IE = 1;
}

void initSW2()
{
    //TODO: Initialize the appropriate pins to work with the external switch using a
    //change notification interrupt.

    //Put the overall CN Interrupt flag down (set it to 0)
    IFS1bits.CNIF = 0;

    TRISBbits.TRISB2 = 1; //Sets the pin for external switch to input
    AD1PCFGbits.PCFG4 = DIGITAL;    //Set the analog pin to digital

    //Enable the overall change notification interrupt
    IEC1bits.CNIE = 1;
    CNEN1bits.CN6IE = 1;
//Enable the pull up resistors for the external components
    CNPU1bits.CN6PUE = 1;   //Pull up resistor for external switch
}