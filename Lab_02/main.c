#include <msp430.h> 
#include <stdio.h>
#include <float.h>
#include <limits.h>

#define TABLE_WIDTH 91
#define NUM_TYPES 12
#define COL_1_WIDTH -23
#define COL_2_WIDTH -16
#define COL_3_WIDTH -20
#define COL_4_WIDTH -23

#define ARR_LEN 7


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
Notes: Part 1 solution adapted from Stack Overflow user Paul Hankin https://stackoverflow.com/a/14418659
*
--------------------------------------------------------
*/


void part_1();

void part_2();

void print_data_type_info(int);

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	part_1();
	
	part_2();
	
	return 0;
}

void part_1() {
	// Print table header row
	int i;
	for (i=0;i<TABLE_WIDTH;i++) printf("-");
	printf("\n");
	printf("| %-*s| %-*s| %-*s| %-*s|\n", COL_1_WIDTH, "Data Type", COL_2_WIDTH, "Size (in bytes)", COL_3_WIDTH, "Minimum", COL_4_WIDTH, "Maximum");
	for (i=0;i<TABLE_WIDTH;i++) printf("-");
	printf("\n");
	
	// Print information for each data type
	for (i=0;i<NUM_TYPES;i++) {
		print_data_type_info(i);
	}
	
	// Print last row of table
	for (i=0;i<TABLE_WIDTH;i++) printf("-");
	printf("\n");
}

void part_2() {
	int x[ARR_LEN] = {-3, -2, -1, 0, 1, 2, 3};
	int m = 5;
	int c = -2;
	int y[7];
	
	int i;
	for (i=0;i<ARR_LEN;i++) {
		y[i] = (m * x[i]) + c;
	}
	
	printf("Input Array X: [");
	for (i=0;i<ARR_LEN;i++) {
		if (!(i == ARR_LEN -1)) {
			printf("%d ", x[i]);
		} else {
			printf("%d", x[i]);
		}
	}
	printf("]\n");
	printf("Input Constants m: %d c: %d\n", m, c);
	printf("Output Array Y: [");
	for (i=0;i<ARR_LEN;i++) {
		if (!(i == ARR_LEN -1)) {
			printf("%d ", y[i]);
		} else {
			printf("%d", y[i]);
		}
	}
	printf("]\n");
}

void print_data_type_info(int i) {

	switch(i) {
		case 0: // Signed char
			printf("| %*s| %*ld| %*d| %*d|\n", COL_1_WIDTH, "char", COL_2_WIDTH, sizeof(char), COL_3_WIDTH, SCHAR_MIN, COL_4_WIDTH, SCHAR_MAX);
			break;
		case 1: // Signed short int
		    printf("| %*s| %*ld| %*d| %*d|\n", COL_1_WIDTH, "short int", COL_2_WIDTH, sizeof(short int), COL_3_WIDTH, SHRT_MIN, COL_4_WIDTH, SHRT_MAX);
			break;
		case 2: // Signed int
		    printf("| %*s| %*ld| %*d| %*d|\n", COL_1_WIDTH, "int", COL_2_WIDTH, sizeof(int), COL_3_WIDTH, INT_MIN, COL_4_WIDTH, INT_MAX);
			break;
		case 3: // Signed long int
		    printf("| %*s| %*ld| %*ld| %*ld|\n", COL_1_WIDTH, "long int", COL_2_WIDTH, sizeof(long int), COL_3_WIDTH, LONG_MIN, COL_4_WIDTH, LONG_MAX);
			break;
		case 4: // Signed long long int
		    printf("| %*s| %*ld| %*lld| %*lld|\n", COL_1_WIDTH, "long long int", COL_2_WIDTH, sizeof(long long int), COL_3_WIDTH, LLONG_MIN, COL_4_WIDTH, LLONG_MAX);
			break;
		case 5: // Unsigned char
		    printf("| %*s| %*ld| %*d| %*d|\n", COL_1_WIDTH, "unsigned char", COL_2_WIDTH, sizeof(unsigned char), COL_3_WIDTH, 0, COL_4_WIDTH, CHAR_MAX);
			break;
		case 6: // Unsigned short int
			printf("| %*s| %*ld| %*d| %*u|\n", COL_1_WIDTH, "unsigned short int", COL_2_WIDTH, sizeof(unsigned short int), COL_3_WIDTH, 0, COL_4_WIDTH, USHRT_MAX);
			break;
		case 7: // Unsigned int
			printf("| %*s| %*ld| %*d| %*u|\n", COL_1_WIDTH, "unsigned int", COL_2_WIDTH, sizeof(unsigned int), COL_3_WIDTH, 0, COL_4_WIDTH, UINT_MAX);
			break;
		case 8: // Unsigned long int
			printf("| %*s| %*ld| %*d| %*lu|\n", COL_1_WIDTH, "unsigned long int", COL_2_WIDTH, sizeof(unsigned long int), COL_3_WIDTH, 0, COL_4_WIDTH, ULONG_MAX);
			break;
		case 9: // Unsigned long long int
			printf("| %*s| %*ld| %*d| %*llu|\n", COL_1_WIDTH, "unsigned long long int", COL_2_WIDTH, sizeof(unsigned long long int), COL_3_WIDTH, 0, COL_4_WIDTH, ULLONG_MAX);
			break;
		case 10: // Float
			printf("| %*s| %*ld| %*e| %*e|\n", COL_1_WIDTH, "float", COL_2_WIDTH, sizeof(float), COL_3_WIDTH, FLT_MIN, COL_4_WIDTH, FLT_MAX);
			break;
		case 11: // Double
			printf("| %*s| %*ld| %*e| %*e|\n", COL_1_WIDTH, "double", COL_2_WIDTH, sizeof(double), COL_3_WIDTH, DBL_MIN, COL_4_WIDTH, DBL_MAX);
			break;
		default:
			return;
	}
}
