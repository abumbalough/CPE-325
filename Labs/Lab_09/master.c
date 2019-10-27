#include <msp430.h> 
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


/*
--------------------------------------------------------
*
File: Lab_09/master.c
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
Date: 10/30/2019
*
--------------------------------------------------------
*/

void SPI_setup(void);

char SPI_getChar(void);

void SPI_putChar(char);

void SPI_getState(void);

void SPI_setState(char);

char UART_getChar(void);

void UART_getStr(char*, int);

void UART_putChar(char);

void UART_putStr(char*);

void UART_setup(void);

char m1[] = "Beacon pause: ";

char uartRx[10];

char spiRx;

void main(void) {
    WDTCTL = WDTPW + WDTHOLD;     // Stop watchdog timer
	unsigned char pause;
	
	UART_setup();
	SPI_setup();
	
	while(1) {
		UART_putStr(m1); // Send prompt
		UART_getStr(uartRx,10);
		if (strcmp(uartRx,"?")) {
			SPI_getState();
		} else if (strcmp(uartRx,"-")) {
			SPI_putChar(0xFF);
		} else {
			pause = (unsigned char)atoi(uartRx);
			if ((pause>0) & (pause<101)) {
				SPI_setState(pause);
			} else {
				UART_putStr("Invalid pause entered.\r\n");
			}
		}
	}
}

char SPI_getChar(void) {
    while(!(IFG2 & UCB0RXIFG)); // Wait until character is ready to be received
    IFG2 &= ~UCB0RXIFG;
    UCB0TXBUF = 0x80;     // Dummy write to start SPI
    while (!(IFG2 & UCB0RXIFG));   // USCI_B0 TX buffer ready?
    return UCB0RXBUF;
}

void SPI_getState(void) {
	char pauseStr[20];
	SPI_putChar(0x00);
	spiRx = SPI_getChar();
	sprintf(pauseStr,"Current pause: %u\r\n",spiRx);
	UART_putStr(pauseStr);
}

void SPI_putChar(char c) {
    while (!(IFG2 & UCB0TXIFG));  // Wait for previous character to transmit
    UCB0TXBUF = c;                // Put character into tx buffer
}

void SPI_setState(char state) {
	SPI_putChar(state);
}

void SPI_setup(void) {
    UCB0CTL0 = UCMSB + UCMST + UCSYNC;// Sync. mode, 3-pin SPI, Master mode, 8-bit data
    UCB0CTL1 = UCSSEL_2 + UCSWRST; // SMCLK and Software reset
    UCB0BR0 = 0x02;                // Data rate = SMCLK/2 ~= 500kHz
    UCB0BR1 = 0x00;
    P3SEL |= BIT1 + BIT2 + BIT3;   // P3.1,P3.2,P3.3 option select
    UCB0CTL1 &= ~UCSWRST;          // **Initialize USCI state machine**
}

char UART_getChar(void) {
	while (!(IFG2 & UCA0RXIFG)); // Wait until a character is ready to be read from Rx buffer
	IFG2 &= ~UCA0RXIFG;
	return UCA0RXBUF;
}

void UART_getStr(char* rxBuf, int limit) {
	char c;
	unsigned int i = 0;
	
	c = UART_getChar();
	while ((c != '\r') & (i < limit-1)) {
		rxBuf[i++] = c; // Store received character in receive buffer
		UART_putChar(c); // Echo character back
		c = UART_getChar(); // Get next character
	}	
	rxBuf[i] = (char)0x00; // Terminate string with null character
}

void UART_putChar(char c) {
    while (!(IFG2 & UCA0TXIFG));  // Wait for previous character to transmit
    UCA0TXBUF = c;                // Put character into tx buffer
}

void UART_putStr(char* message) {
    int i;
    for(i = 0; message[i] != 0; i++) {
        UART_putChar(message[i]);
    }
}

void UART_setup(void) {
    P2SEL |= BIT4 + BIT5;          // Set UC0TXD and UC0RXD to transmit and receive data
    UCA0CTL1 |= UCSWRST;           // Software reset
    UCA0CTL0 = 0;                  // USCI_A0 control register
    UCA0CTL1 |= UCSSEL_2;          // Clock source SMCLK - 1048576 Hz
    UCA0BR0 = 54;                  // Baud rate - 1048576 Hz / 19200
    UCA0BR1 = 0;
    UCA0MCTL = 0x0A;               // remainder 10
    UCA0CTL1 &= ~UCSWRST;          // Clear software reset
}
