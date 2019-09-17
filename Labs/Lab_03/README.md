Austin Bumbalough  
CPE 325-08  
Lab 03  
09/11/2019  
# Lab 03 Solution  
## Timing Calculations 
There are 10 clock cycles per for-loop iteration, 
and the processor frequency is 1 MHz (1 million clock cycles per second). 
This means that there are ![eqn1](http://www.sciweavers.org/tex2img.php?eq=%5Cfrac%7B10%5E%7B6%7D%5Cfrac%7Bclk.%5C%20cycles%7D%7Bsecond%7D%7D%7B10%5Cfrac%7Bclk.%5C%20cycles%7D%7Bloop%5C%20iteration%7D%7D%3D10%5E%7B5%7D%20%5Cfrac%7Bloop%5C%20iterations%7D%7Bsecond%7D%3D10%5E%7B2%7D%5Cfrac%7Bloop%5C%20iterations%7D%7Bmillisecond%7D&bc=White&fc=Black&im=png&fs=12&ff=arev&edit=0)
so the number of iterations in _s_ milliseconds is _d(s) = 10<sup>2</sup>s_.  
An on/off period of 5 Hz means the led must toggle at 10 Hz. 10 Hz means the delay should be 100 ms.  
Thus, _d(100) = 10<sup>2</sup>(100)=10,000_ loop iterations.  
Similarly, a 2 Hz blinking frequency means the LED must toggle at 4 Hz (250 ms), and a 1 Hz blinking
frequency means the LED must toggle at 2 Hz (500 ms).  
_d(250) = 10<sup>2</sup>(250) = 25,000_ loop iterations and _d(500) = 10<sup>2</sup>(500) = 50,000_ loop iterations.  

## Source Code  

```C
#include <msp430.h> 
#include <stdio.h>

#define SW1 P1IN & BIT0 // SW1 is bit 0 of P1IN
#define SW2 P1IN & BIT1 // SW2 is bit 1 of P1IN
#define LED1 BIT2 // LED 1 is bit 2 of P2OUT
#define LED2 BIT1 // LED 2 is bit 1 of P2OUT
#define EVER ;;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    // Initialize Pins
    P2DIR |= BIT2 + BIT1;

    // Turn LEDs on
    P2OUT |= LED1 + LED2;

    // Infinite loop
    for(EVER) {

		unsigned long int i;
		switch (P1IN & (BIT1 + BIT0)) {
		    case (BIT1 + BIT0): // No switches pressed, bit 1 = bit 0 = 1
                P2OUT |= (LED1 + LED2); // LED1 and LED2 are on when no switch is pressed
		        break;
			case BIT1: // SW1 pressed, bit 1 = 1, bit 0 = 0
				for(i=0;i<10000;i++); // 100ms delay --> 5 Hz
				P2OUT ^= LED1;
				break;
			case BIT0: // SW2 pressed, bit 1 = 0, bit 0 = 1
				for(i=0;i<25000;i++); // 250ms delay --> 2 Hz
				P2OUT ^= LED2;
				break;
			case 0: // SW1 and SW2 pressed, bit 1 = bit 0 = 0
				for(i=0;i<50000;i++); // 500ms delay --> 1 Hz
				P2OUT &= 0;
                for(i=0;i<50000;i++);
                P2OUT |= (LED1 + LED2);
				break;
		}

    }

    return 0;
}
```
