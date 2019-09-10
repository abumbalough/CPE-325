#include <msp430.h> 
#include <stdio.h>

#define SW1 P1IN & BIT0
#define SW2 P1IN & BIT1

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
    for(;;) {

        // If Switch 1 is pressed
        if ((SW1) == 0) {

            // 20 ms debounce
            int i;
            for(i=0;i<20000;i++);

            // Toggle LED using XOR
            P2OUT ^= BIT2;
        }
    }

    return 0;
}
