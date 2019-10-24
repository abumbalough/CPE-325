#include <msp430.h> 


/*
--------------------------------------------------------
*
File: Lab_09/main.c
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
Date: 10/31/2019
*
--------------------------------------------------------
*/
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	return 0;
}
