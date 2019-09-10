#include <msp430.h> 
#include <stdio.h>

#define SW1 P1IN & BIT0
#define SW2 P1IN & BIT1
#define EVER ;;

/*
--------------------------------------------------------
*
File:
*
Description:
*
Input:
*
Output:
*
Author: Austin Bumbalough
*
Lab Section: 8
*
Date:
*
--------------------------------------------------------
*/

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    // Initialize Pins
    P2DIR |= BIT2 + BIT1;

    // Turn LEDs on
    P2OUT |= BIT2 + BIT1;

    // Infinite loop
    for(EVER) {
		
		unsigned int i;
		switch (P1IN & (BIT1 + BIT0)) {
			case BIT0:
				for(i=0;i<10000;i++); // 100ms delay --> 5 Hz
				P2OUT ^= BIT2;
				break;
			case BIT1:
				for(i=0;i<25000;i++); // 250ms delay --> 2 Hz
				P2OUT ^= BIT2;
				break;
			case (BIT1 + BIT0):
				for(i=0;i<50000;i++); // 500ms delay --> 1 Hz
				P2OUT ^= (BIT2 + BIT1);
				break;
			default:
				P2OUT |= (BIT2 + BIT1); // LED1 and LED2 are on when no switch is pressed
		}
		
    }

    return 0;
}
