/****************************************************************************
 * Project Name:    Light controlled light
 * Author:          RUST
 * Filename:        lightControlledLight.c
 * Purpose:         Class exercise
 * ------------------------------------------------------------------------
 * Version history:
 * 01-03-15: Initial version
 *
 * =========================================================================
 * Description:
 * This exercise is used to show how the comparator works in the MSP430
 * The program turns on an LED when the voltage on P1.4 is below 0.5*Vcc and
 * turns off the LED when the voltage is above 0.5Vcc.
 *
 * Requires:
 * LaunchPad
 * Homemade circuit
 *
 * P1.4: input - Comparator input
 * P1.6: Output - Green LED on LaunchPad
 ****************************************************************************/
#include <msp430.h> 

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    /**
     * Setup CACTL1 register
     * CAON:    Comparator ON
     * CAREF_2: Reference voltage set to Vcc/2
     * CAIE:    Enable Comparator Interrupts
     * CAIES:   Initially enable falling edge interrupt
     */
    CACTL1 = CAON + CAREF_2 + CAIE + CAIES;
    /**
     * Setup CACTL2 register
     * P2CA3:   Enable input on pin P1.4
     * P2CA0:   Enable pin P1.0 (measure the reference voltage on this pin)
     * CAF:     Enable output filter.
     */
    CACTL2 = P2CA3 + P2CA0 + CAF;

    /**
     * Setup P1.6 as output (green LED on P1.6)
     */
    P1DIR = BIT6;

    /**
     * Enable interrupts globally, go into low power mode 0 (datasheet p.38)
     */
    __bis_SR_register(GIE + LPM0_bits);

	return 0;
}

/**
 * Interrupt service routing for the comparator
 */
#pragma vector=COMPARATORA_VECTOR
__interrupt void isr_comparator()
{
    if (CACTL1 & CAIES)     // Interrupt Falling Ege - it's dark.
    {
        P1OUT |= BIT6;      // Turn on LED at P1.6
        CACTL1 &= ~CAIES;   // Change interrupt edge to rising
    }
    else
    {
        P1OUT &= ~BIT6;     // Turn off LED at P1.6
        CACTL1 |= CAIES;    // Change interrupt edge to falling
    }
    CACTL1 &= ~CAIFG;       // Clear interrupt flag
}
