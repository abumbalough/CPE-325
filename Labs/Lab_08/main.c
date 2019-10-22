#include <msp430.h> 
#include <stdio.h>

/**
 * main.c
 */
void UART_initialize(void);

void main(void) {
	WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer
	P5DIR |= BIT1; // LED4
	UART_initialize();
	UCA0CTL1 &= ~UCSWRST;
	_BIS_SR(LPM0_bits + GIE); // Low-power mode 0, interrupts enabled
}

void UART_initialize(void) {
    UCA0CTL1 |= UCSWRST; // USCI SW Reset
    P2SEL |= BIT5 + BIT4; // Enable UART pins
    UCA0CTL0 = 0;
    UCA0CTL1 |= UCSSEL_2; // Set clock to SMCLK
    UCA0BR0 = 54; // Sets Baud rate to 19200 bps
    UCA0BR1 = 0; // 2^20/19200 = 54r5
    UCA0MCTL = 0x0A; // UCBRS0 = 5, UCOS16 = 0
    IE2 |= UCA0RXIE; // Rx Interrupt enable
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCIA0RX_ISR(void) {
    UCA0TXBUF = UCA0RXBUF; // Echo the character back
    P5OUT ^= BIT1; // Toggle LED4
}
