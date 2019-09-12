#include <msp430.h> 
#include <stdio.h>

#define SW1 P1IN & BIT0 // SW1 is bit 0 of P1IN
#define SW2 P1IN & BIT1 // SW2 is bit 1 of P1IN
#define LED1 BIT2 // LED 1 is bit 2 of P2OUT
#define LED2 BIT1 // LED 2 is bit 1 of P2OUT
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
    P2OUT |= LED1 + LED2;

    // Infinite loop
    for(EVER) {

		unsigned long int i;
		switch (P1IN & (BIT1 + BIT0)) {
		    case (BIT1 + BIT0): // No switches pressed, bit 1 = bit 0 = 1
                P2OUT |= (LED1 + LED2); // LED1 and LED2 are on when no switch is pressed
		        break;
			case BIT1: // SW1 pressed, bit 1 = 1, bit 0 = 0
				for(i=0;i<10000;i++); // 100ms delay --> 5 Hz
				P2OUT ^= LED1;
				break;
			case BIT0: // SW2 pressed, bit 1 = 0, bit 0 = 1
				for(i=0;i<25000;i++); // 250ms delay --> 2 Hz
				P2OUT ^= LED2;
				break;
			case 0: // SW1 and SW2 pressed, bit 1 = bit 0 = 0
				for(i=0;i<50000;i++); // 500ms delay --> 1 Hz
				P2OUT &= 0;
                for(i=0;i<50000;i++);
                P2OUT |= (LED1 + LED2);
				break;
		}

    }

    return 0;
}
