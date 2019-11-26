#include <msp430.h> 

#define PASSTHROUGH 1
#define DISTORTION 2
#define FUZZ 3
#define DIST_UPPR_THR 3500
#define DIST_LWR_THR 600
#define FUZZ_UPPR_THR 3000
#define FUZZ_LWR_THR 1100



/**
 * main.c
 */

unsigned int SIGNAL;

unsigned int VOLUME = 4095;

unsigned char CURRENT_EFFECT = PASSTHROUGH;

void OA_Setup(void);

void FLL_Setup(void);

void ADC12_Setup(void);

void DAC12_Setup(void);

//void DMA_Setup(void);

void TimerA_Setup(void);

void main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
//	P5DIR |= 0x02;                            // P5.1 output
	P1DIR &= ~BIT0;
	P1IES |= BIT0;
	P1IE |= BIT0;
	P2DIR |= BIT2 + BIT1; // Configure P2.2 (LED1) and P2.1(LED2) as output
	P2OUT = BIT2;
	P6SEL |= BIT4 + BIT0;
	FLL_Setup();
	TimerA_Setup();
	ADC12_Setup();
	DAC12_Setup();
	OA_Setup();

	ADC12CTL0 |= ADC12SC;
	DAC12_0CTL |= DAC12ENC;

	while(1) {
	    _BIS_SR(GIE);
	    ADC12CTL0 |= ADC12SC;
	}
}

void OA_Setup(void) {
//    OA0CTL0 = OAN_0 + OAP_2 + OAPM_3 + OAADC1;        // OA0I0 to inverting input, DAC0 (1.25 Vbias) to non-inverting input, fast slew rate (better audio performance)
//    OA0CTL1 = OAFC_6 + OAFBR_0;      // Inverting PGA mode, amplifier gain is -4.3 (500mV p-p --> 2.15 V p-p), rail-to-rail input
    OA0CTL0 = OAPM_3 + OAADC1;        // OA0I0 to inverting input, DAC0 (1.25 Vbias) to non-inverting input, fast slew rate (better audio performance)
    OA0CTL1 = OAFC_1;
}

void FLL_Setup(void) {
    FLL_CTL0 |= XCAP18PF; // Set load capacitance for xtal
    SCFQCTL = 122; // (122 * 32768) = 4 MHz
}

void ADC12_Setup(void) {
    ADC12CTL0 = MSC + REFON + REF2_5V + ADC12ON;              // Reference = 2.5V, also used by DAC0
    for (unsigned int i=34000;i>0;i--); // 17 ms delay to allow Vref to settle
    for (unsigned int i=34000;i>0;i--);
    ADC12CTL1 = SHP + CONSEQ_3; // Repeated single-channel conversion mode
    ADC12MCTL0 = SREF_1 + INCH_1; // Map Channel 1 to MEM0
    ADC12MCTL1 = SREF_1 + INCH_4 + EOS; // Map Channel 4 to MEM1
    ADC12IE |= BIT1;
    ADC12CTL0 |= ENC; // Enable conversion
}

void DAC12_Setup(void) {
    DAC12_1CTL = DAC12IR + DAC12AMP_2 + DAC12ENC; // Internal Vref (2.5 V from ADC),
    DAC12_1DAT = (unsigned int)2048;          // 1.25V DC offset for Op Amp

    DAC12_0CTL = DAC12LSEL_3 + DAC12IR + DAC12AMP_7;
}

//void DMA_Setup(void) {
//}

void TimerA_Setup(void) {
    TACTL = TASSEL_2 + MC_1; // CLK = SMCLK = 4 MHz, Up mode, Enable interrupts
    TACCR0 = 95; // 4,194,304/95 = 44.1 KHz
    TACCTL0 = CCIE + OUTMOD_4;
}


// Timer A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMERA0_VECTOR
__interrupt void TimerA_ISRB (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMERA0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
    _NOP();
    switch (CURRENT_EFFECT) {
        case DISTORTION:
            if (SIGNAL > DIST_UPPR_THR) DAC12_0DAT = DIST_UPPR_THR;
            else if (SIGNAL < DIST_LWR_THR) DAC12_0DAT = DIST_LWR_THR;
            else DAC12_0DAT = SIGNAL;
            break;
        case FUZZ:
            if (SIGNAL > FUZZ_UPPR_THR) DAC12_0DAT = FUZZ_UPPR_THR;
            else if (SIGNAL < FUZZ_LWR_THR) DAC12_0DAT = FUZZ_LWR_THR;
            else DAC12_0DAT = SIGNAL;
            break;
        default: // Passthrough, no effect on signal
//            DAC12_0DAT = (int)(((double)SIGNAL*VOLUME)/4095.0);
            DAC12_0DAT = SIGNAL;
            break;

    }
//    P5OUT ^= 0x02;                            // Toggle P5.1 using exclusive-OR
//    _BIC_SR_IRQ(LPM0_bits);
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC12_VECTOR))) ADC12_ISR (void)
#else
#error Compiler not supported!
#endif
{
    SIGNAL = ADC12MEM0;
    VOLUME = ADC12MEM1;
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT1_VECTOR))) PORT1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    for (unsigned int i=42000;i>0;i--);
    for (unsigned int i=42000;i>0;i--); // 20 ms debounce delay
    if ((P1IN & BIT0) == 0) { // Check if switch is still pressed
        if (CURRENT_EFFECT == PASSTHROUGH) CURRENT_EFFECT = DISTORTION;
        else if (CURRENT_EFFECT == DISTORTION) CURRENT_EFFECT = FUZZ;
        else if (CURRENT_EFFECT == FUZZ) CURRENT_EFFECT = PASSTHROUGH;
        P2OUT = CURRENT_EFFECT << 1;
    } P1IFG = 0;
}
