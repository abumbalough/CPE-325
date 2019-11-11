#include <msp430.h> 
#include "lut.h"


/*
--------------------------------------------------------
*
File: Lab_10/part3_solution.c
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

unsigned int HALF = 1;

unsigned int WAVE_SEL = 0;

int* WAVE_TABLE[2] = {sine_512_lut,saw_512_lut};

void DAC12_Setup(void);

void TimerA_Setup(void);

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P1DIR &= ~(BIT1+BIT0); // Configure P1.0 (SW1) and P1.1 (SW2) as inputs
    P1IES |= BIT1+BIT0; // Trigger interrupt on falling edge
    P1IE |= BIT1+BIT0; // Enable interrupts for P1.0 and P1.1
    TimerA_Setup();
    DAC12_Setup();
    _BIS_SR(LPM0_bits + GIE);
}

void DAC12_Setup(void) {
    ADC12CTL0 = REF2_5V + REFON;
    for (unsigned int i = 50000;i>0;i--); // Delay to allow Vref to stabilize
    DAC12_0CTL = DAC12IR + DAC12AMP_5;
}

void TimerA_Setup(void) {
    TA0CTL = TASSEL_2 + MC_1;
    TA0CCR0 = 82; // 25 Hz signal
    TA0CCTL0 = CCIE;
}

#pragma vector=PORT1_VECTOR
__interrupt void P1_ISR(void) {
    if (P1IFG & BIT0) {
        for (unsigned int i = 20952;i>0;i--); // 20 ms debounce using SMCLK = 2^20 Hz
        if ((P1IN & BIT0) == 0) { // Check if switch is still pressed
            WAVE_SEL = (WAVE_SEL + 1) % 2; // Toggle between sine and saw wave
            P1IES ^= BIT0; // Toggle interrupt edge select to catch switch release
        }
        P1IFG &= ~BIT0; // Clear IFG
    } else if (P1IFG & BIT1) {
        for (unsigned int i = 20952;i>0;i--); // 20 ms debounce using SMCLK = 2^20 Hz
        if ((P1IN & BIT0) == 0) { // Check if switch is still pressed   
            if (HALF == 1) HALF = 2;
            else HALF = 1;
            P1IES ^= BIT1; // Toggle interrupt edge select to catch switch release
        }
        P1IFG &= ~BIT1; // Clear IFG
    }
}

#pragma vector=TIMERA0_VECTOR
__interrupt void TimerA0_ISR(void) {
    static unsigned int j = 0;
    DAC12_0DAT = WAVE_TABLE[WAVE_SEL][j++ % 512] / HALF;
}


