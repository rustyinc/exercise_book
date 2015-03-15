/****************************************************************************
 * Project Name:    ADC sample Knob (Trim resistor)
 * Author:          RUST
 * Filename:        ADC_SampleKnob.c
 * Purpose:         Class exercise
 * ------------------------------------------------------------------------
 * Version history:
 * 10-03-15: Initial version
 *
 * =========================================================================
 * Description:
 * This program samples the input from the trimresistor on the LEDpad, and
 * outputs a number between 0 and 255 according to the position of the
 * trimresistor
 *
 * Required:
 * -> LaunchPad
 * -> LED Pad
 *
 * Port 2.x Output
 * Port 1.1 Input (trim resisor)
 ****************************************************************************/

#include <msp430.h> 

int main(void)
{
    /* Stop the watchdog timer */
    WDTCTL = WDTPW | WDTHOLD;

    /**
     * Setup entire port 2 as output
     * BIT6 and BIT7 cleared in select for enabling GPIO
     */
    P2DIR  |= (BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0);
    P2SEL  &= ~(BIT7 | BIT6);
    P2SEL2 &= ~(BIT7 | BIT6);

    /**
     * Setup the ADC10CTL0
     * ADC10SHT_2: 16xsample and hold
     * ADC10ON   : Turn on ADC
     * ADC10IE   : Enable ADC interrupts
     */
    ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE;
    /* Set ADC input to P1.4 */
    ADC10CTL1 = INCH_4;
    /* Enable analog input on P1.4 */
    ADC10AE0 |= BIT4;

    /* Enable and start conversion */
    ADC10CTL0 |= ENC + ADC10SC;

    /**
     * Enable interrupts globally - and go to sleep, zzzzzzz
     * (ADC interrupt function will still work
     */
    __bis_SR_register(GIE + LPM0_bits);
	
	return 0;
}

/**
 *  ADC10 interrupt service routine
 */
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
    /* What happens here? and why? */
    P2OUT = ADC10MEM >> 2;

    /* Restart conversion */
    ADC10CTL0 |= ENC + ADC10SC;
}
