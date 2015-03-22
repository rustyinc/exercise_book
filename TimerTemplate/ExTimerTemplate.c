/****************************************************************************
 * Project Name:    Timer Template
 * Author:          RUST
 * Filename:        ExTimerTemplate.c
 * Purpose:         Class exercise
 * ------------------------------------------------------------------------
 * Version history:
 * 19-03-15: Initial version
 *
 * =========================================================================
 * Description:
 * This file implements a timer exercise. The timer times out continously
 * and executes the corresponding interrupt service routine. The timeout
 * frequency can be adjusted.
 *
 * Required:
 * -> LaunchPad
 *
 * Optional:
 * -> Clock xtal
 *
 ****************************************************************************/
#include <msp430.h> 

/* Write:
 * #undef USE_EXTERNAL_XTAL to route the build-in VLOCLK to ACLK.
 * #define USE_EXTERNAL_XTAL to route the external clock xtal to ACLK.
 */
#undef USE_EXTERNAL_XTAL

int main(void)
{
    /* Stop Watchdog Timer */
    WDTCTL = WDTPW | WDTHOLD;

#ifdef USE_EXTERNAL_XTAL
    /***
     * Route the external clock xtal to ACLK.
     * The ACLK is as default connected to the external clock xtal.
     *
     * XCAP_3   : Set xtal capacitors to 12.5 pF.
     *            (lookup the datasheet for the xtal for details)
     */
    BCSCTL3 |= XCAP_3;
#else
    /***
     * Route the VLOCLK (source) to ACLK (signal)
     */
    BCSCTL3 |= LFXT1S_2;
#endif

    /***
     * Setup the timer functions (first the control register)
     * TASSEL_1 : Use ACLK as clock signal
     * MC_1     : Clock counts in up mode.
     *            (Generates an interrupt when value in TACCR is reaced)
     */
    TA0CTL |= TASSEL_1 | MC_1;

    /***
     * Setup the timer capture/compare (CC) register
     * CCIE     : Enable interrupts when timer reaches value in TACCR
     */
	TA0CCTL0 |= CCIE;
	
	/***
	 * The TA0CCR0 register contains the value, the timer has to count to
	 * before generating an interrupt.
	 * The TA0CCR0 register is continously compared with the TAR register,
	 * which the timer modeule automatically increments at every clock pulse.
	 */
	TA0CCR0 = 10000;

    /**
     * Enable interrupts globally - and go to sleep, zzzzzzz
     * (Timer interrupt function will still work)
     */
	__bis_SR_register(GIE + LPM3_bits);

	return 0;
}

/**
 * Interrupt service routine for the Timer timeout
 */
#pragma vector = TIMER0_A0_VECTOR
__interrupt void timer_isr(void)
{
    /* IMPLEMENT YOUR MAGIC HERE... */
}

