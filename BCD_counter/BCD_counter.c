/****************************************************************************
 * Project Name:    BCD counter
 * Project Created: 06.11.14
 * Author:          Ruben Sternke
 * Contact:         rust@eal.dk
 * ------------------------------------------------------------------------
 * Version history:
 * 06-11-2014: Initial version
 * 06-03-2014: E-mail address in header updated.
 *
 * =========================================================================
 * Description:
 * Using the LEDs as output for a BCD counter
 *
 * Hardware setup:
 * P2.x : LED Output
 * P1.5 : Button input
 *
 ****************************************************************************/

#include <msp430.h> 

// Forward declaration
void BCDtoLED(unsigned char);


void main(void)
{
    unsigned char counter;

    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    // Setup P2.x as output
    P2DIR  |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7;
    P2SEL  &= ~(BIT6 + BIT7);
    P2SEL2 &= ~(BIT6 + BIT7);
    P2OUT  = 0x00;  // Reset the output

    // Setup P1.5 as an interrupt enabled input
    P1DIR &= ~BIT5;
    P1IE  |=  BIT5; // Enable interrupt on p1.5
    P1IFG &= ~BIT5; // Clear pending interrupt flag
    P1IES &= ~BIT5; // Enable low to high transistion
	
    _BIS_SR(GIE + LPM4_bits);  // interrupts and low power mode enabled

    counter = 0;
    while(1)
    {
        counter++;
        if(counter >= 100)
            counter = 0;
        BCDtoLED(counter);
        LPM4;
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void port1_int(void)
{
    P1IFG &= ~BIT5;
    LPM4_EXIT;
}

void BCDtoLED(unsigned char counter)
{
    unsigned char output;

    output  = (counter / 10) << 4;
    output |= (counter % 10);
    P2OUT = output;
}
