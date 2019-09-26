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

evalString:	.cstring "1+2-4"
											
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

			clr.w SR 				; Clear status register
			clr.w R7 				; Clear result register R7
			clr.w R8 				; Clear operator flag register R8
			bis.b #0xFF, P2DIR 		; Set port 2 to output
			mov.w #0, P2OUT
			mov.w #evalString, R4 	; Move string start address to R4
			jmp getOperand

getOperator:mov.b @R4+, R6 			; Get next Operator
			cmp.b #0, R6 			; Check for string end
			jeq printResult
			cmp.b #'+', R6 			; Check if operator is +
			jeq setAdd
			cmp.b #'-', R6 			; Check if operator is -
			jeq setSub
convOp:		cmp.b #'+', R6 			; Compare operator and set operation flag
			jeq setAdd
			cmp.b #'-', R6
			jeq setSub
setAdd:		mov.b #0, R8 			; Set operation flag to 0
			jmp getOperand
setSub:		mov.b #1, R8 			; Set operation flag to 1
			jmp getOperand
getOperand:	mov.b @R4+, R5 			; Move next operand to R5
charToDec:	cmp.b #'0', R5 			; Convert operand from ascii to decimal
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
			jmp evaluate
ascii1:		mov.b #1, R5
			jmp evaluate
ascii2:		mov.b #2, R5
			jmp evaluate
ascii3:		mov.b #3, R5
			jmp evaluate
ascii4:		mov.b #4, R5
			jmp evaluate
ascii5:		mov.b #5, R5
			jmp evaluate
ascii6:		mov.b #6, R5
			jmp evaluate
ascii7:		mov.b #7, R5
			jmp evaluate
ascii8:		mov.b #8, R5
			jmp evaluate
ascii9:		mov.b #9, R5
			jmp evaluate
evaluate:	cmp #0, R8 				; Check operation flag and perform operation
			jeq regAdd
			cmp #1, R8
			jeq regSub
regAdd:		add.b R5, R7 			; Add R5 to R7
			jmp getOperator
regSub:		sub.b R5, R7 			; Subtract R5 from R7
			jmp getOperator
printResult:mov.b R7, P2OUT 		; Display R7 on Port 2
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

