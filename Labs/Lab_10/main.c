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

void ADC_Setup(void);

void TimerB_Setup(void);

volatile unsigned int ADCX, ADCY, ADCZ;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    TimerB_Setup(); // Initialize Timer B for SH timer source
    ADC_Setup(); // Initialize ADC
    _BIS_SR(GIE);

    while(1) {
//        ADC12CTL0 |= ADC12SC;
    }
}

void ADC_Setup(void) {
    P6DIR &= ~(BIT2+BIT1+BIT0);             // Configure P6.0, P6.1, and P6.2 as input pins
    P6SEL |= BIT2+BIT1+BIT0;                // Configure P6.0, P6.1, and P6.2 as analog pins
    ADC12CTL0 = ADC12ON + SHT0_6 + MSC;     // Enable conversion, 128 clk. cycle SHT, multiple sample and conversion enabled
    ADC12CTL1 = ADC12SSEL_0 + SHP + SHS_2 + CONSEQ_3;         // Use internal oscillator, use sampling timer, Timer B0 SH source, Repeat sequence sample mode
//    ADC12CTL1 = ADC12SSEL_0 + SHP + CONSEQ_3;
    ADC12MCTL0 = INCH_0;                    // ADC A0 pin - Acc. X-axis
    ADC12MCTL1 = INCH_1;                    // ADC A1 pin - Acc. Y-axis
    ADC12MCTL2 = INCH_2 + EOS;              // ADC A2 pin - Acc. Z-axis, EOS - End of Sequence for Conversions
    ADC12IE |= BIT2;                        // Enable ADC12IFG.2, interrupt triggers after three conversions
//    for (unsigned int i = 0; i < 0x3600; i++);           // Delay for reference start-up
    ADC12CTL0 |= ENC;                       // Enable conversions
}

void TimerB_Setup(void) {
    TB0CTL |= CNTL_3 + TBSSEL_2 + MC_2; // SMCLK source, continuous mode
    TB0CCTL0 = OUTMOD_4;
}

#pragma vector = ADC12_VECTOR
__interrupt void ADC12ISR(void) {
    ADCX = ADC12MEM0;                  // Move results, IFG is cleared
    ADCY = ADC12MEM1;
    ADCZ = ADC12MEM2;
//    __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
}

