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

void UART_PutChar(char);

void UART_Setup(void);

volatile unsigned int ADCX, ADCY, ADCZ;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    TimerB_Setup(); // Initialize Timer B
    ADC_Setup(); // Initialize ADC
	_EINT();

    while(1) {
		ADC12CTL0 |= ADC12SC;
		_BIS_SR(LPM0_bits + GIE);
	}
}

void ADC_Setup(void) {
    P6DIR &= ~(BIT2+BIT1+BIT0);             // Configure P6.0, P6.1, and P6.2 as input pins
    P6SEL |= BIT2+BIT1+BIT0;                // Configure P6.0, P6.1, and P6.2 as analog pins
    ADC12CTL0 = ADC12ON + SHT0_6 + MSC;     // Enable conversion, 128 clk. cycle SHT, multiple sample and conversion enabled
    ADC12CTL1 = SHP + CONSEQ_1;         	// Use sample timer, single sequence
    ADC12MCTL0 = INCH_0;                    // ADC A0 pin - Acc. X-axis
    ADC12MCTL1 = INCH_1;                    // ADC A1 pin - Acc. Y-axis
    ADC12MCTL2 = INCH_2 + EOS;              // ADC A2 pin - Acc. Z-axis, EOS - End of Sequence for Conversions
    ADC12IE |= BIT2;                        // Enable ADC12IFG.2, interrupt triggers after three conversions
    ADC12CTL0 |= ENC;                       // Enable conversions
}

void sendData(void) {
	volatile float x_acc, y_acc, z_acc;
    unsigned int i;
	
    x_acc = ((ADCX / 4095 * 10) - 5);    // Calculate output x acceleration
    y_acc = ((ADCY / 4095 * 10) - 5);    // Calculate output y acceleration
    z_acc = ((ADCZ / 4095 * 10) - 5);    // Calculate output z acceleration
	
    // Use character pointers to send one byte at a time
    char *x_acc_pnt = (char *)&x_acc;
    char *y_acc_pnt = (char *)&y_acc;
    char *z_acc_pnt = (char *)&z_acc;

    UART_PutChar(0x55);            // Send header
    for(i = 0; i < 4; i++) {            // Send x acceleration - one byte at a time
        UART_PutChar(x_acc_pnt[i]);
    }
	
    for(i = 0; i < 4; i++) {            // Send y acceleration - one byte at a time
        UART_PutChar(y_acc_pnt[i]);
    }
	
    for(i = 0; i < 4; i++) {            // Send z acceleration - one byte at a time
        UART_PutChar(z_acc_pnt[i]);
    }
}

void TimerB_Setup(void) {
	TB0CCR0 = 3277; // 5 Hz 
    TB0CTL |= TBSSEL_1 + MC_1; // ACLK source, up mode
    TB0CCTL0 = CCIE; // Enable interrupts
}

void UART_PutChar(char c) {
    while(!(IFG2 & UCA0TXIFG));         // Wait for previous character to be sent
    UCA0TXBUF = c;                      // Send byte to the buffer for transmitting
}

void UART_Setup(void) {
    P2SEL |= BIT4 + BIT5;               // Set up Rx and Tx bits
    UCA0CTL0 = 0;                       // Set up default RS-232 protocol
    UCA0CTL1 |= BIT0 + UCSSEL_2;        // Disable device, set clock
    UCA0BR0 = 27;                       // 1048576 Hz / 38400
    UCA0BR1 = 0;
    UCA0MCTL = 0x94;
    UCA0CTL1 &= ~BIT0;                  // Start UART device
}

#pragma vector = ADC12_VECTOR
__interrupt void ADC12ISR(void) {
    ADCX = ADC12MEM0;                  // Move results, IFG is cleared
    ADCY = ADC12MEM1;
    ADCZ = ADC12MEM2;
	__bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
}

#pragma vector = TIMERB0_VECTOR
__interrupt void TimerB_ISR(void) {
	sendData();
	__bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
}

