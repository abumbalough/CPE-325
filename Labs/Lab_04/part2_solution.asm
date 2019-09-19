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
			clr.w R7
			bis.b #0xFF, P2DIR
			mov.b #'+', R6

getNext:	mov.b @R4+, R5 ; Move next operand to R7
			cmp.b #0, R5 ; Check for string end
			jeq printResult
			jmp charToDec
charToDec:	cmp.b #'0', R5
			jeq ascii0
			cmp.b #'1', R5
			jeq ascii1
			cmp.b #'2', R5
			jeq ascii2
			cmp.b #'3', R5
			jeq ascii3
			cmp.b #'4', R5
			jeq ascii4
			cmp.b #'5', R5
			jeq ascii5
			cmp.b #'6', R5
			jeq ascii6
			cmp.b #'7', R5
			jeq ascii7
			cmp.b #'8', R5
			jeq ascii8
			cmp.b #'9', R5
			jeq ascii9
ascii0:		mov.b #0, R5
			jmp getOp
ascii1:		mov.b #1, R5
			jmp getOp
ascii2:		mov.b #2, R5
			jmp getOp
ascii3:		mov.b #3, R5
			jmp getOp
ascii4:		mov.b #4, R5
			jmp getOp
ascii5:		mov.b #5, R5
			jmp getOp
ascii6:		mov.b #6, R5
			jmp getOp
ascii7:		mov.b #7, R5
			jmp getOp
ascii8:		mov.b #8, R5
			jmp getOp
ascii9:		mov.b #9, R5
			jmp getOp
eval:		cmp.b #0, R8
			jeq setAdd
			cmp.b #1, R8
			jeq setSub
getOp:		mov.b @R4+, R6 ; Get next Operator
			cmp.b #'+', R6 ; Check if operator is +
			jeq addOps
			cmp.b #'-', R6 ; Check if operator is -
			jeq subOps
addOps:		add.b R5, R7 ; Add R7 to R5
			jmp getNext
subOps:		sub.b R5, R7 ; Subtract R7 from R5
			jmp getNext
setAdd:		mov.b #0, R8
setSub:		mov.b #1, R8
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

