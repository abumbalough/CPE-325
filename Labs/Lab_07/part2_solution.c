#include <msp430.h> 

/*
--------------------------------------------------------
*
File: Lab_07/part2_solution.c
*
Description: Use timers to pulse a 1 KHz tone at 0.5 Hz
*
Input: N/A
*
Output: On-board LEDs and buzzer
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
Buzzer - P3.5 - Active Low
--------------------------------------------------------
*/

int main(void) {
    // Setup Ports and Registers
    WDTCTL = WDT_ADLY_1000;   // set watchdog timer to interval mode 1000 ms
	P2DIR |= BIT2; // Set P2.2 (LED1) to output
	P2OUT |= BIT2; // Initialize LED1 to on
	IE1 |= WDTIE; // Enable WDT interrupts
    P3DIR |= BIT5; // Set P3.5 (Buzzer) to output
    P3SEL |= BIT5; // Select special function for P2.2 (TB4 Output)
    TBCTL = MC_0; // Stop timer B while configuring
    TBCTL |= TBSSEL_2; // Timer B Clock Source: SMCLK
    TBCCTL4 |= OUTMOD_4; // Set Timer B4 to toggle output mode
    TBCCR0 = 523; // Sets buzzer frequency to 1 KHz
    TBCTL |= MC_1; // Start Timer B in up mode
    _EINT(); // Set GIE bit in SR
    LPM0; // Put system in low power mode 0

    return 0;
}

#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void) {
	P2OUT ^= BIT2; // Toggle LED1
	P3SEL ^= BIT5; // Toggle function of P3.5
}
