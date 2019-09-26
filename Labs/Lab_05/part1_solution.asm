;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
; *
; File: Lab_05/part1_solution.asm
; *
; Description: Perform element-wise multiplication of two one-dimensional
;				arrays and store the result in a third array
; *
; Input: Data stored in memory
; *
; Output: Debug memory window
; *
; Author: Austin Bumbalough
; *
; Lab Section: 8
; *
; Date: 10/01/2019
; *
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

arr1:		.int 1, 2, 3, 4, 5, 6, 7, 8		; Declare input array 1
arr2:		.int 3, 4, 5, 6, 7, 8, 9, 10	; Declare input array 2

			.bss arrOut, 16, 2				; Allocate unitialized space for output array

;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------

			mov.b #8, R4 		; Number of array elements
			mov.w #arr1, R5 	; Starting address of input array 1
			mov.w #arr2, R6 	; Starting address of input array 2
			mov.w #arrOut, R10	; Starting address of output array
			push R4 ;
			push R5 ;
			push R6 ;
			push R7 ;
progEnd:	jmp $ 				; End of program

;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
            
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            
