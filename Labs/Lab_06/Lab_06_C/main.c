#include <msp430.h> 

#define SW1 BIT0&P1IN
#define SW2 BIT1&P1IN
/*
--------------------------------------------------------
*
File: Lab_06/Lab_06_C/main.c
*
Description: Change blinking LED frequency by altering clock
				speed using FLL+ module control registers
*
Input: MSP430 Experimenter Board SW1 and SW2
*
Output: MSP430 Experimenter Board LED1 and LED2
*
Author: Austin Bumbalough
*
Lab Section: 8
*
Date: 10/07/2019
*

SW1 - P1.0 active low
SW2 - P1.1 active low
LED1 - P2.2
LED2 - P2.1
--------------------------------------------------------
*/
int clkFreq = 1;

int main(void)
{
	// Setup Ports and Registers
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P1DIR = 0x00; // P1 set to input
	P2DIR |= BIT2+BIT1; // P2.1 and P2.2 set to output
	P2OUT = BIT2; // LED 1 on, LED 2 off
	P1IES |= BIT1+BIT0; // Interrupt triggered on falling edge for P1.1 and P1.0
	P1IE |= BIT1+BIT0; // Interrupts enabled on P1.1 and P1.0
	P1IFG &= ~(BIT1+BIT0); // Clear any pending interrupts
	_EINT(); // Enable global interrupts
	
	while(1) {
		for (long int i=50000;i>0;i--);
		P2OUT ^= BIT2+BIT1;
	}
	
	return 0;
}

#pragma vector = PORT1_VECTOR
__interrupt void P1ISR(void) {
	switch (P1IFG & (BIT1+BIT0)) {
		case (BIT0): // SW1 pressed, frequency increases
			switch (clkFreq) {
				case 1:
					clkFreq = 2;
					SCFQCTL = 60; // f = (60+1) * 32768 = 1.99 MHz
					break;
				case 2:
					clkFreq = 4;
					SCFQCTL = 121; // f = (121+1) * 32768 = 3.99 MHz
					break;
				case 4:
					clkFreq = 8;
					SCFI0 |= BIT2; // Adjust DCO range up to 1.3-12.1 MHz
					FLL_CTL0 |= DCOPLUS; // Enable FLL+ loop divider, doubles frequency
					break;
				case 8:
					// Do nothing, frequency is at maximum value
					break;
			}
			P1IFG &= ~BIT0;
			break;
		case(BIT1): // SW2 pressed, frequency decreases
			switch (clkFreq) {
				case 1:
					// Do nothing, frequency is at minimum value
					break;
				case 2:
					clkFreq = 1;
					SCFQCTL = 30; // f = (30+1) * 32768 = 1.01 MHz
					break;
				case 4:
					clkFreq = 2;
					SCFQCTL = 60; // f = (60+1) * 32768 = 1.99 MHz
					break;
				case 8:
					clkFreq = 4;
					SCFI0 &= ~BIT2; // Adjust DCO range down to 0.65-6.1 MHz
					FLL_CTL0 &= ~DCOPLUS; // Disable FLL+ loop divider, halves frequency
					break;
			}
			P1IFG &= ~BIT1;
			break;	
	}
}
