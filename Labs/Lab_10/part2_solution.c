#include <msp430.h> 
#include <math.h>


/*
--------------------------------------------------------
*
File: Lab_10/part2_solution.c
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

volatile unsigned int crashDetected = 0;

volatile float X_ACC, Y_ACC, Z_ACC;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P2DIR |= BIT2; // Configure P2.2 as output
    P1DIR &= ~BIT0; // Configure P1.0 as input
	P1IES |= BIT0; // Select falling edge for interrupt trigger
    P1IE |= BIT0; // Enable interrupts for P1.0 (SW1)
    TimerB_Setup(); // Initialize Timer B
    ADC_Setup(); // Initialize ADC
    UART_Setup();
    _EINT();

    while(1) {
        ADC12CTL0 |= ADC12SC;
        _BIS_SR(LPM0_bits + GIE);
        if (crashDetected) P2OUT |= BIT2;
    }
}

void ADC_Setup(void) {
    P6DIR &= ~(BIT2+BIT1+BIT0);             // Configure P6.0, P6.1, and P6.2 as input pins
    P6SEL |= BIT2+BIT1+BIT0;                // Configure P6.0, P6.1, and P6.2 as analog pins
    ADC12CTL0 = ADC12ON + SHT0_6 + MSC;     // Enable conversion, 128 clk. cycle SHT, multiple sample and conversion enabled
    ADC12CTL1 = SHP + CONSEQ_1;             // Use sample timer, single sequence
    ADC12MCTL0 = INCH_0;                    // ADC A0 pin - Acc. X-axis
    ADC12MCTL1 = INCH_1;                    // ADC A1 pin - Acc. Y-axis
    ADC12MCTL2 = INCH_2 + EOS;              // ADC A2 pin - Acc. Z-axis, EOS - End of Sequence for Conversions
    ADC12IE |= BIT2;                        // Enable ADC12IFG.2, interrupt triggers after three conversions
    ADC12CTL0 |= ENC;                       // Enable conversions
}

void sendData(void) {
    unsigned int i;
    
    X_ACC = ((ADCX / 4095.0 * 10.0) - 5);    // Calculate output x acceleration
    Y_ACC = ((ADCY / 4095.0 * 10.0) - 5);    // Calculate output y acceleration
    Z_ACC = ((ADCZ / 4095.0 * 10.0) - 5);    // Calculate output z acceleration
    
    // Use character pointers to send one byte at a time
    char *X_ACC_pnt = (char *)&X_ACC;
    char *Y_ACC_pnt = (char *)&Y_ACC;
    char *Z_ACC_pnt = (char *)&Z_ACC;

    UART_PutChar(0x55);            // Send header
    for(i = 0;i<4;i++) {            // Send x acceleration - one byte at a time
        UART_PutChar(X_ACC_pnt[i]);
    }
    
    for(i = 0;i<4;i++) {            // Send y acceleration - one byte at a time
        UART_PutChar(Y_ACC_pnt[i]);
    }
    
    for(i = 0;i<4;i++) {            // Send z acceleration - one byte at a time
        UART_PutChar(Z_ACC_pnt[i]);
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

#pragma vector=PORT1_VECTOR
__interrupt void P1_ISR(void) {
    for (unsigned long int i=20972;i>0;i--); // 20 ms debounce delay
    if ((P1IN & BIT0) == 0) {
        P2OUT &= ~BIT2; // Turn off led
        crashDetected = 0; // reset flag
    }
    P1IFG &= ~BIT0;
}

#pragma vector = TIMERB0_VECTOR
__interrupt void TimerB_ISR(void) {
    sendData();
    if (crashDetected == 0) {
        float net_acc = sqrt( pow(X_ACC,2) + pow(Y_ACC,2)); // + pow(Z_ACC,2) );
        if (net_acc > 2) crashDetected = 1; // Set flag if net acceleration exceeds 2g
    }
    __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
}

