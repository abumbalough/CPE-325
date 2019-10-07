#include <msp430.h> 

#define SW1 P1IN & BIT0
#define SW2 P1IN & BIT1

/*
--------------------------------------------------------
*
File: Lab_06/Lab_06_C_2/main.asm
*
Description: Use interrupt service routines to blink LEDs according to pressed switches
*
Input: On-board switches SW1 and SW2
*
Output: On-board LEDs LED1 and LED2
*
Author: Austin Bumbalough
*
Lab Section: 8
*
Date: 10/08/2019
*

SW1 - P1.0
SW2 - P1.1
LED1 - P2.2
LED2 - P2.1
--------------------------------------------------------
*/
int bothSW = 0;

int main(void) {
    // Setup Ports and Registers
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P1DIR = 0x00; // P1 set to input
    P2DIR |= BIT2+BIT1; // P2.1 and P2.2 set to output
    P2OUT &= ~(BIT2+BIT1); // LED 1 and LED 2 off initially
    P1IES |= BIT1+BIT0; // Interrupt triggered on falling edge for P1.1 and P1.0
    P1IE |= BIT1+BIT0; // Interrupts enabled on P1.1 and P1.0
    P1IFG &= ~(BIT1+BIT0); // Clear any pending interrupts
    _EINT(); // Enable global interrupts

    while(1);
    return 0;
}

#pragma vector = PORT1_VECTOR
__interrupt void P1ISR(void) {
    // _EINT();
    switch (P1IFG & (BIT1+BIT0)) {
        case (BIT0): // SW1 pressed
                for (int i=10000;i>0;i--);
                if ((SW1) == 0) {
                    for (int j=3;j>0;j--) {
                        for (int k=16667;k>0;k--);
                        P2OUT ^= BIT2;
                        for (int k=16667;k>0;k--);
                        P2OUT ^= BIT2;
                    }
                    P2OUT ^= BIT1;
                }
                P1IFG &= ~BIT0; // Clear interrupt flag
                break;
        case(BIT1): // SW2 pressed
                for (int i=10000;i>0;i--);
                if ((SW2) == 0) {
                    P2OUT ^= BIT1;
                }
                P1IFG &= ~BIT1; // Clear interrupt flag
                break;
        case(BIT1+BIT0):
                for (int i=10000;i>0;i--);
                if (((SW1) == 0) & ((SW2) == 0)) {
                    P2OUT |= BIT2+BIT1;
                }
                while (((SW1) == 0) & ((SW2) == 0));
                P2OUT &= ~(BIT2+BIT1);
                P1IFG &= ~(BIT1+BIT0);
                break;
    }
}
