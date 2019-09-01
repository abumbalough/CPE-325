#include <msp430.h>
#include <stdio.h>

/*
--------------------------------------------------------
*
File
: Lab_01_Part_01/main.c
*
Description
: Create user-defined power function.
*
Input: Constant values defined in program
* Output: Stdout
*
Author: Austin Bumbalough
* Lab Section: 08
*
Date: 8/27/19
*
--------------------------------------------------------
*/

long int myPower(int, int) ;

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	// Declare parameter values
	int a = 2;
	int p = 11;
	long int c;

	c = myPower(a,p);

	printf("%d raised to the power %d is %ld\n", a, p ,c);

	return 0;
}

long int myPower(int a, int b) {
    long int c = 1;

    // c = 1 for case where b = 0
	// for all other cases, c = a^b
	for (int i=0; i<b; i++) {
        c *= a;
    }

    return c;
}
