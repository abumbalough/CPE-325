#include <msp430.h> 
#include <stdio.h>
#include <string.h>

#define NULL 0

/*
--------------------------------------------------------
*
File: Lab_08/main.c
*
Description: Create a serial communication chatbot
*
Input: Data from USCI Peripheral
*
Output: Write data to USCI Peripheral
*
Author: Austin Bumbalough
*
Lab Section: 8
*
Date: 10/24/2019
*
--------------------------------------------------------
*/

void UART_initialize(void);

void UART_sendCharacter(char);

char UART_getCharacter(void);

void UART_sendString(char*);

void UART_getLine(char*, int);

void main(void) {
	WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer
	P5DIR |= BIT1; // LED4
	UART_initialize();
	UCA0CTL1 &= ~UCSWRST;
	// _BIS_SR(LPM0_bits + GIE); // Low-power mode 0, interrupts enabled
	
	// char key[] = "Hey, Bot!";
	// char meStr[] = "Me: ";
	// char botStr[] = "Bot: ";
	char wakeStr[25];
	while(1) {
		UART_sendString("Me: ");
		UART_getLine(wakeStr, 25); // Get line
		while(!(strcmp("Hey, Bot!",wakeStr))) {
			UART_sendString("\r\n"); // send carriage return and newline
			UART_sendString("Me: ");
			UART_getLine(wakeStr, 25);
		} UART_sendString("\r\n");
		
		char ageStr[10];
		UART_sendString("Bot: Hi! How old are you?\r\n"); // Send chatbot reply to key phrase
		UART_sendString("Me: ");
		UART_getLine(ageStr, 10); // Get user's age
		UART_sendString("Bot: ");
		if (!(strcmp("1000",ageStr))) { // If user enters "1000"
			UART_sendString("That cannot be true!\r\n");
		} else {
			UART_sendString("You are so young! I am 1"); // Respond to user age
			UART_sendString(ageStr);
			UART_sendString("\r\n");
		}
	}
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

void UART_sendCharacter(char c) {
	while (!(IFG2 & UCA0TXIFG)); // Wait until Tx buffer is ready to receive char
	UCA0TXBUF = c; // Move c into Tx buffer
}

char UART_getCharacter(void) {
	while (!(IFG2 & UCA0RXIFG)); // Wait until a character is ready to be read from Rx buffer
	return UCA0RXBUF;
}

void UART_sendString(char* string) {
	int i = 0;
	while (string[i] != (char) NULL) {
		UART_sendCharacter(string[i++]); // Send each character in string 
	}
}

void UART_getLine(char* buffer, int limit) {
	char c = UART_getCharacter();
	int i = 0;
	
	while ((c != '\n') & (i < limit-1)) {
		buffer[i++] = c; // Store received character in receive buffer
		UART_sendCharacter(c); // Echo character back
		c = UART_getCharacter(); // Get next character
	}	
	buffer[i] = (char) NULL; // Terminate string with null character
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCIA0RX_ISR(void) {
    UCA0TXBUF = UCA0RXBUF; // Echo the character back
    P5OUT ^= BIT1; // Toggle LED4
}
