#include <stdio.h>


/*
--------------------------------------------------------
*
File
: Lab_01_Part_02/main.c
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


int main(void)
{
	
	char operators[11] = {'%', '&', '*', '+', '-', '/', '<', '>', '=', '^', '~'};
	char testString[] = "Do 42+53/76%8=2*8-32+71 & you can sleep.";
	size_t opSize = sizeof(operators);
	size_t stringSize = sizeof(testString);

	int mathCharCount = 0;

    char currentChar;
	for (int i=0;i<stringSize;i++) {
	    currentChar = testString[i];

	    for (int j=0;j<opSize;j++) {
	            if (currentChar == operators[j]) {
	                mathCharCount++;
	            }
	        }
	}

	printf("String: %s\n", testString);
	printf("Contains: The string contains %d symbols that represent mathematical operations.\n", mathCharCount);

	return 0;
}
