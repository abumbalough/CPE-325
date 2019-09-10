#include <msp430.h> 
#include <stdio.h>


/*
--------------------------------------------------------
* 
File: 
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
Date:
*
--------------------------------------------------------
*/

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	return 0;
}
