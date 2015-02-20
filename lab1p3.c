// ******************************************************************************************* //
//
// File:         lab1p3.c
// Date:
// Authors: Phillip Showers, Miguel Garcia, Angel Lopez
//
// Description: Part 3 for lab 1. This shows the status of a process (Running: or Stopped: and displays the amount
//                                of time that has elapsed.
// ******************************************************************************************* //

#include "p24fj64ga002.h"
#include "leds.h"
#include "timer.h"
#include "lcd.h"
#include "initSW.h"
#include <stdio.h>

#define STOP 0
#define RUN 1

#define RELEASED 1
#define PRESSED 0

#define RESET_SW PORTBbits.RB5
#define TOGGLE_SW PORTBbits.RB2

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & COE_OFF & ICS_PGx1 &
          FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )

_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
          IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )

typedef enum stateTypeEnum
{
    //TODO: Define states by name
    run,
    stop
} stateType;

/*These variables are used to count time in the T1_ISR. These will be updated and displayed on the lcd*/
volatile int minute = 0;
volatile int minuteTens = 0;
volatile int second = 0;
volatile int secondTens = 0;
volatile int tenthSecond = 0;
volatile int hundredthSecond = 0;

volatile stateType CurrState = 0;
volatile int displayOnce = 0;   //This variable is used for limiting the amount of times
                                //the lcd is continually displaying the Runnind:/Stopped: and the time.


// ******************************************************************************************* //

int main(void)
{
    initLCD();
    initLEDs();
    initSW1();
    initSW2();
    initTimer1();
    CurrState = run;
//    lcdRunState();

    while(1)
    {
        //TODO: Using a finite-state machine, define the behavior of the LEDs
        //Debounce the switch
        switch(CurrState)
        {
            case run:
            TurnOnLED(RUN);
            if (displayOnce < 10){
                lcdRunState();
                displayOnce++;
            }
            getTimeString(minuteTens, minute, secondTens, second, tenthSecond, hundredthSecond);
            break;

            case stop:
            if (displayOnce < 10){
            lcdStopState();
            displayOnce++;
            getTimeString(minuteTens, minute, secondTens, second, tenthSecond, hundredthSecond);
            }
            TurnOnLED(STOP);
            break;
        }
    }

    return 0;
}

void _ISR _CNInterrupt(void)
{
    IFS1bits.CNIF = 0;  //Put flag down

    /*This function will toggle the LEDs between the RUN and STOP state.
     There is a 5 msecond delay on every press and release.
     */
    if(TOGGLE_SW == PRESSED)
    {
        displayOnce = 0;
        if(CurrState == run)
        {
            CurrState = stop;
        }
        else if(CurrState == stop)
        {
            CurrState = run;
        }
    }

    /*
     * This IF statement will reset the timer if the switch on the microcontroller is pressed.
     * It will only reset if the LED toggle switch is also released.
     */
    if((RESET_SW == PRESSED) && (TOGGLE_SW == RELEASED))
    {

        if(CurrState == stop)
        {
/*This resets all of the values for the time if the reset button is pressed.*/
            minuteTens = 0;
            minute = 0;
            secondTens = 0;
            second = 0;
            tenthSecond = 0;
            hundredthSecond = 0;
            CurrState == stop;
            displayOnce = 0;
        }
    }

}

void _ISR _T1Interrupt(void)
{
    //TODO: Put down the timer 1 flag first!
    IFS0bits.T1IF = 0;  //Put the flag down
    /*
     * This function will increment the volatile variables used to count time.
     * When the circuit is in the run state.
     */
    if(CurrState == run)    //Will display the Running: state.
    {
        hundredthSecond = hundredthSecond + 1;

        if(hundredthSecond > 9)
        {
        tenthSecond = tenthSecond + 1;
        hundredthSecond = 0;
        }
        if(tenthSecond > 9)
        {
        second = second + 1;
        tenthSecond = 0;
        }
        if(second > 9)
        {
        secondTens = secondTens + 1;
        second = 0;
        }
        if(secondTens > 5)
        {
            minute = minute + 1;
            secondTens = 0;
        }
        if(minute > 9)
        {
            minuteTens = minuteTens + 1;
            minute = 0;
        }
       
    }
}
