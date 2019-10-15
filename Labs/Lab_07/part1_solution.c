#include <msp430.h> 

#define SW1 (P1IN & BIT0)
#define SW2 (P1IN & BIT1)

/*
--------------------------------------------------------
*
File: Lab_07/part1_solution.c
*
Description: Use timers to generate a PWM signal for controlling LED brightness
*
Input: On-board switches SW1 and SW2
*
Output: On-board LEDs LED1 and LED2
*
Author: Austin Bumbalough
*
Lab Section: 8
*
Date: 10/16/2019
*

SW1 - P1.0
SW2 - P1.1
LED1 - P2.2
LED2 - P2.1
--------------------------------------------------------
*/

int main(void) {
    // Setup Ports and Registers
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P1DIR &= ~(BIT1+BIT0); // Set P1.0 and P1.1 to input
    P2DIR |= BIT2; // Set P2.2 to output
    P2SEL |= BIT2; // Select special function for P2.2 (TB1 Output)
    TBCTL = MC_0; // Stop timer B while configuring
    TBCTL |= TBSSEL_2; // Timer B Clock Source: SMCLK
    TBCCTL1 |= OUTMOD_7; // Set Timer B1 to toggle reset/set output mode
    TBCCR1 = 500; // Set Timer B1 compare value
    TBCCR0 = 1000;
    TBCTL |= MC_1; // Start Timer B in up mode
    P1IES |= BIT1+BIT0; // Falling edge triggers interrupt
    P1IFG &= ~(BIT1+BIT0); // Clear any pending interrupts
    P1IE |= BIT1+BIT0; // Enable interrupts on P1.0 and P1.1
    _EINT(); // Set GIE bit in SR
    LPM0; // Put system in low power mode 3

    return 0;
}

#pragma vector = PORT1_VECTOR
__interrupt void P1ISR(void) {
	static int level = 3;
	switch (P1IFG & (BIT1+BIT0)) {
		case BIT0: // SW1 pressed, brightness increases
			for (int i=10000;i>0;i--);
			if (SW1 == 0) {
				if (level < 5) level++;
			}
			P1IFG &= ~BIT0; // Clear P1.0 interrupt flag
			break;
		case BIT1: // SW2 pressed, brightness decreases
			for (int i=10000;i>0;i--);
			if (SW2 == 0) {
				if (level > 1) level--;
			}
			P1IFG &= ~BIT1; // Clear P1.1 interrupt flag
			break;
		case (BIT1+BIT0):
			break;
	}
	TBCCR1 = (200*level) - 1;
}
