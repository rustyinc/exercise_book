/****************************************************************************
 * Project Name:    PIR sensor handling from interrupt
 * Author:          RUST
 * Filename:        interruptPIRsensor.c
 * Purpose:         Class exercise
 * ------------------------------------------------------------------------
 * Version history:
 * 29-01-15: Initial version
 *
 * =========================================================================
 * Description:
 * This exercise handles interrupts from an PIR sensor. When the PIR sensor
 * interrupts, a LED is turned on for 5 sek.
 *
 * Requires:
 * LaunchPad
 *
 * P1.0: output - Red LED on LaunchPad
 * P1.4: intput - Input from the PIR sensor
 ****************************************************************************/
#include <msp430.h> 

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    /* Set P1.0 as input */
    P1OUT &= ~BIT0;     // Initially clear bit 0 (turn off LED)
    P1DIR |= BIT0;      // Set P1.0 to output

    /* Set P1.4 to input and enable interrupt */
    P1DIR &= ~BIT4;     // Set P1.4 to input
    P1IES |=  BIT4;     // Set high-to-low transistion
    P1OUT |=  BIT4;     // Prepare the internal resistor to Vcc
    P1REN |=  BIT4;     // Enable transistor on P1.4
    P1IFG &= ~BIT4;     // Clear interrupt flag
    P1IE  |=  BIT4;     // Enable interrupt on P1.4

    __bis_SR_register(GIE); // Enable interrupts

    for(;;)
    {
        LPM4;                       // Enter lowpower mode 4
        P1OUT |= BIT0;              // Turn on LED
        __delay_cycles(5000000);    // Wait for 5 seconds
        P1OUT &= ~BIT0;             // Clear bit again
    }
}

/***
 * Interrupt service routing for port 1
 */
#pragma vector=PORT1_VECTOR
__interrupt void port1_ISR(void)
{
    if (P1IFG & BIT4)
    {
        LPM4_EXIT;
        P1IFG &= ~BIT4;
    }
}
