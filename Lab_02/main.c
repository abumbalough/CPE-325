#include <msp430.h> 
#include <stdio.h>
#include <float.h>
#include <limits.h>


/*
--------------------------------------------------------
*
File: Lab_02/main.c
*
Description: Print size and range of char, short int,
             int, long int, long long int, unsigned
             char, unsigned short int, unsigned int,
             unsigned long int, unsigned long long int,
             float, and double data types.
*
Input: N/A
*
Output: Print to stdout
*
Author: Austin Bumbalough
*
Lab Section: 8
*
Date: 8/29/19
*
--------------------------------------------------------
*/


/*
------------
   PART 1
------------
*/
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	// Print table header row
	printf("-----------------------------------------------------------------------\n");
	printf("| %-16s| %-17s| %-16s| %-13s|\n", "Data Type", "Size (in bytes)", "Minimum", "Maximum");
	printf("-----------------------------------------------------------------------\n");

	// Print size and range for char data type
	printf("| %-16s| %-17d| %-16d| %-13d|\n", "char", sizeof(char), SCHAR_MIN, SCHAR_MAX);

    // Print size and range for short int data type
    printf("| %-16s| %-17d| %-16d| %-13d|\n", "short int", sizeof(short int), SHRT_MIN, SHRT_MAX);

    // Print size and range for int data type
    printf("| %-16s| %-17d| %-16d| %-13d|\n", "int", sizeof(int), INT_MIN, INT_MAX);

    // Print size and range for long int data type
    printf("| %-16s| %-17d| %-16ld| %-13ld|\n", "long int", sizeof(long int), LONG_MIN, LONG_MAX);

    // Print size and range for long long int data type
    printf("| %-16s| %-17d| %-16lld| %-13lld|\n", "long long int", sizeof(long long int), LLONG_MIN, LLONG_MAX);

    // Print size and range for unsigned char data type
    printf("| %-16s| %-17d| %-16d| %-13d|\n", "unsigned char", sizeof(unsigned char), CHAR_MIN, CHAR_MAX);

	return 0;
}
/*
------------
 END PART 1
------------
*/


// /*
// ------------
//    PART 2
// ------------
// */
// int main(void)
// {
//     WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
//
//     return 0;
// }
// /*
// ------------
//  END PART 2
// ------------
// */
