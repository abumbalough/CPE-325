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

			.bss arrOut_sw, 16, 2			; Allocate unitialized space for output arrays
			.bss arrOut_hw, 16, 2			;

;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer
;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
			mov.b #8, R4 			; Number of array elements
			mov.w #arr1, R5 		; Starting address of input array 1
			mov.w #arr2, R6 		; Starting address of input array 2
			mov.w #arrOut_sw, R7	; Starting address of output arrays
			mov.w #arrOut_hw, R8	;
			push R4 ;
			push R5 ;
			push R6 ;
			push R7 ;
			call swMult
			add.w #8, SP			; Collapse stack after subroutine execution
			mov.b #8, R4
			mov.b #arr1, R5
			mov.b #arr2, R6
			push R4
			push R5
			push R6
			push R8
			call hwMult
			add.w #, SP
			
progEnd:	jmp $ 					; End of program
;-------------------------------------------------------------------------------
; SW_Multiply Subroutine - Only supports up to 8-bit multiplication
;-------------------------------------------------------------------------------
swMult:		push R9
			push R10
			push R11
			push R12
			clr.w R12
								; Retrieve parameters from stack
			mov.w 14(SP), R4 	; Array length
			mov.w 12(SP), R5	; arr1 address
			mov.w 10(SP), R6	; arr2 address
			mov.w 8(SP), R7	; arrOut address
getNext:	mov.b @R5, R9 ; Get next operands
			mov.b @R6, R10
			sxt R5 ; Sign extend the operands
			sxt R6
			mov.b R10, R11 ; Use R11 as loop counter
mul_loop:	bit.w BIT0, R10
			jz shift
			add.w R9, R12
shift:		rla.w R9
			rra.w R10
			dec.w R11
			jnz mul_loop
			bit.w BIT0, R10
			jz mul_end
			inv.w R9
			add.w #1, R9
			add.w R9, R12
mul_end:	mov.w R12, 0(R7)
			inc.w R5 ; Point arr1 to the next element
			inc.w R6 ; Point arr2 to the next element
			inc.w R7 ; Point arrOut to the next element
			dec.w R4 ; Decrement loop counter
			jnz getNext
			pop R12
			pop R11
			pop R10
			pop R9
			ret			
;-------------------------------------------------------------------------------
; HW_Multiply Subroutine
;-------------------------------------------------------------------------------
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
            
