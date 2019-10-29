#include <msp430.h> 


/*
--------------------------------------------------------
*
File: Lab_09/slave.c
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

void initLED(void);

void SPI_setup(void);

void sysInit(void);

unsigned char pause;

unsigned char spiRx;

unsigned char spiTx;

void main(void) {
	sysInit();
    SPI_setup();                  // Setup USI module in SPI mode
	
	while(1) {
		_BIS_SR(LPM0_bits + GIE); // LPM0 w/ Interrupt
		if (spiRx == 0) {
			USISRL = pause;
		} else if (spiRx < 101) {
			pause = spiRx;
		} else if (spiRx == 0xff) {
			P1DIR &= ~BIT0; // Turn LED3 off
		}
	}
} 

void initLED(void) {
    P1DIR |= BIT0;                // P1.0 as output - LED3
    P1OUT |= BIT0; // Initialize LED3 on
} 

void SPI_setup(void) {
    USICTL0 |= USISWRST;          // Set UCSWRST -- needed for re-configuration process
    USICTL0 |= USIPE5 + USIPE6 + USIPE7 + USIOE; // SCLK-SDO-SDI port enable,MSB first
    USICTL1 = USIIE;              // USI Counter Interrupt enable
    USICTL0 &= ~USISWRST;         // **Initialize USCI state machine**
    USICNT = 8;                   // Load bit counter, clears IFG
}

void sysInit(void) {
    WDTCTL = WDT_MDLY_32;     // 32 ms interval mode
    BCSCTL1 = CALBC1_1MHZ;        // Set DCO
    DCOCTL = CALDCO_1MHZ;
}

#pragma vector = USI_VECTOR
__interrupt void USI_ISR(void) {
	P1DIR |= BIT0;
    spiRx = USISRL;           // Read new command
    USICNT = 8;                   // Load bit counter for next TX
    _BIC_SR_IRQ(LPM0_bits);       // Exit from LPM0 on RETI
}

#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void) {
	static int i = 0;
	if (i == 1) {
		P1OUT &= ~BIT0;
	} else if (i == pause+1) {
		P1OUT |= BIT0;
		i = 0;
	} else i++;
	_BIS_SR(LPM0_bits + GIE);
}
