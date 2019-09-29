;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
; *
; File: Lab_05/part2_solution.asm
; *
; Description: Computes the first 5 powers of a given base. Valid range for inputs
;				is 2 < n < 8 where n = 2, 3, 4, ...
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
											
			.bss power_sw, 20, 2			; Allocate unitialized space for outputs
			.bss power_hw, 20, 2			;
;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer
;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
			mov.w #power_sw, R14	; Load result address to register
			mov.w #power_hw, R15	;
			mov.w #5, R4			; Power
			mov.w #7, R5			; Base low word
			mov.w #0, R6 			; Base high word
			add.w #20, R14	
main_loop:	push R4					; Pass exponent on stack
			push R5					; Pass base on stack
			push R6					; -
			push R7					; Pass space for return value on stack
			push R8					; -
			call power_sp			;
			pop R8					; Pop return value from stack
			pop R7					; -
			add.w #6, SP			; Collapse stack after subroutine return
			mov.w R7, 0(R14)		; Store result in memory
			mov.w R8, 2(R14)		; -	
			add.w #-4, R14			; Decrement result pointer
;			mov.w R7, 0(R15)		; Store result in memory
;			mov.w R8, 2(R15)		; -
;			add.w #-4, R15			; Decrement result pointer
			dec.w R4				; Decrement exponent
			jnz main_loop			;
loop_end:	jmp $					; End of program
;-------------------------------------------------------------------------------
; Power Subroutine
;-------------------------------------------------------------------------------
power_sp:	push R4				; Save program state
			push R5				;
			push R6				;
			push R7				;
			push R8				;
			mov.w 20(SP), R4	; Retrieve loop counter from stack
			mov.w 18(SP), R5	; Retrieve OP1 LB
			mov.w 16(SP), R6	; Retrieve OP1 UB
			mov.w #1, R7		; Retrieve OP2 LB
			mov.w #0, R8		; Retrieve OP2 UB
power_loop:	push R5				; Pass OP1 on stack
			push R6				; -
			push R7				; Pass OP2 on stack
			push R8				; -
			call swMult			;
			pop R8				; Pop return value from stack
			pop R7				; -
			add.w #4, SP		; Collapse stack after subroutine return
;			call hwMult			;
;			pop R8				; Pop return value from stack
;			pop R7				; -
;			add.w #4, SP		; Collapse stack after subroutine return
			dec.w R4			; Decrement loop counter
			jnz power_loop		;
power_end:	mov.w R7, 14(SP)	; Return result on stack
			mov.w R8, 12(SP)	; -
			pop R8				; Restore program state
			pop R7				;
			pop R6				;
			pop R5				;
			pop R4				;
			ret					;
;-------------------------------------------------------------------------------
; SW_Multiply Subroutine - Only supports up to 16-bit multiplication
;-------------------------------------------------------------------------------
swMult:		push R4				; Save program state
			push R5				;
			push R6				;
			push R7				;
			push R8				;
			push R9				;
			push R10			;
			mov.w #16, R4		; 
			mov.w 22(SP), R5	; Retrieve OP1 LB from stack
			mov.w 20(SP), R6	; Retrieve OP1 UB
			mov.w 18(SP), R7	; Retrieve OP2 LB
			mov.w 16(SP), R8	; Retrieve OP2 UB
mul_loop:	bit.w BIT0, R7		; Test R7 bit 0
			jz shift			;
			add.w R5, R9		; 16-bit add
			addc.w R6, R10		; -
shift:		rla.w R5			; 16-bit left shift
			rlc.w R6			; -
			rra.w R7			; 16-bit right shift
			rrc.w R8			; -
			dec.w R4			;
			jnz mul_loop		;
			bit.w BIT0, R7		; Test R7 bit 0
			jz mul_end			;
			inv.w R5			; 16-bit 2's complement
			inv.w R6			; -
			add.w #1, R5		; -
			addc.w #0, R6		; -
			add.w R5, R9		; 16-bit add
			addc.w R6, R10		; -
mul_end:	mov.w R9, 18(SP)	; Return result on stack
			mov.w R10, 16(SP)	; -
			pop R10				; Restore program state
			pop R9				;
			pop R8				;
			pop R7				;
			pop R6				;
			pop R5				;
			pop R4				;
			ret					;
;-------------------------------------------------------------------------------
; HW_Multiply Subroutine
;-------------------------------------------------------------------------------
hwMult:		mov.w 8(SP), &MPYS	;
			mov.w 4(SP), &OP2	;
			mov.w &RESLO, 4(SP)	;
			mov.w &RESHI, 2(SP)	;
			ret
;----------------------------------------------------------------;
;	 _ )               |            |                     |      ;
;	 _ \  |  |   ` \    _ \   _` |  |   _ \  |  |   _` |    \    ;
;	___/ \_,_| _|_|_| _.__/ \__,_| _| \___/ \_,_| \__, | _| _|   ;
;	                                              ____/          ;
;----------------------------------------------------------------;
