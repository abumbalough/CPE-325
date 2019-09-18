;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
; *
; File: Lab_04/part2_solution.asm
; *
; Description: Evaluate a string representation of a mathematical
;			   expression and display the result on a parallel data port
; *
; Input: Data stored in memory
; *
; Output: Debug register window
; *
; Author: Austin Bumbalough
; *
; Lab Section: 8
; *
; Date: 09/23/2019
; *
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            .align 2						; Align to words
			
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
											
;-------------------------------------------------------------------------------
; Program variables stored in RAM (.data)
;-------------------------------------------------------------------------------

evalString:	.cstring "1+2+13"
											
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
RESET:       mov.w   #__STACK_END,SP         ; Initialize stackpointer
             mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------

			clr.w SR
			clr.w R5
			bis.w #0xFF, P2DIR
			
			mov.w #evalString, R4
			mov.b @R4+, R5 ; R5 stores expression result
			cmp.b #0, R5
			jeq printResult
			mov.b @R4+, R6 ; R6 stores current operator
			cmp.b #0, R6
			jeq printResult
			mov.b @R4+, R7 ; R7 stores operand
			cmp.b #0, R7
			jeq printResult
			cmp.b #'+', R6 ; Check if operator is +
			jeq addOps
			cmp.b #'-', R6 ; Check if operator is -
			jeq subOps
getNext:	mov.b @R4+, R7 ; Move next operand to R7
			cmp.b #0, R7 ; Check for string end
			jeq printResult
			mov.b @R4+, R6 ; Get next Operator
			cmp.b #0, R6 ; Check for string end
			jeq printResult
			cmp.b #'+', R6 ; Check if operator is +
			jeq addOps
			cmp.b #'-', R6 ; Check if operator is -
			jeq subOps
addOps:		add.w R7, R5 ; Add R7 to R5
			jmp getNext
subOps:		sub.w R7, R5 ; Subtract R7 from R5
			jmp getNext
printResult:mov.b R5, P2OUT
			jmp $

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

