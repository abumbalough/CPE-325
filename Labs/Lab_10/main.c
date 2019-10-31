#include <msp430.h> 


/*
--------------------------------------------------------
*
File: Lab_10/part1_solution.c
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
Date: 11/11/2019
*
--------------------------------------------------------
*/

volatile unsigned int ADCX, ADCY, ADCZ;

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	return 0;
}

void ADC_setup(void) {
    unsigned int i = 0;

    P6DIR &= ~(BIT2+BIT1+BIT0);             // Configure P6.3 and P6.7 as input pins
    P6SEL |= BIT2+BIT1+BIT0;               // Configure P6.3 and P6.7 as analog pins
    ADC12CTL0 = ADC12ON + SHT0_6 + MSC; // configure ADC converter
    ADC12CTL1 = ADC12SSEL_0 + SHP + CONSEQ_1;         // Use sample timer, single sequence
    ADC12MCTL0 = INCH_0;                // ADC A3 pin - Acc. X-axis
    ADC12MCTL1 = INCH_1;          // ADC A7 pin - Acc. Y-axis
    ADC12MCTL2 = INCH_2 + EOS;        // EOS - End of Sequence for Conversions
    ADC12IE |= 0x02;                    // Enable ADC12IFG.1
    for (i = 0; i < 0x3600; i++);       // Delay for reference start-up
    ADC12CTL0 |= ENC;                   // Enable conversions
}

#pragma vector = ADC12_VECTOR
__interrupt void ADC12ISR(void) {
    ADCX = ADC12MEM0;                  // Move results, IFG is cleared
    ADCY = ADC12MEM1;
    ADCZ = ADC12MEM2;
    __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
}

