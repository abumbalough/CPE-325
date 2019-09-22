;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
; *
; File: Lab_04/part3_solution.asm
; *
; Description: Convert a mixed-case string to upper-case
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

			.data
testString:	.cstring "This is a Mixed-Case string."
											
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

			mov.w #testString, R4 	; Move string start address to R4
getNext:	mov.b @R4, R5 			; Move first character into R5
			cmp.b #0, R5 			; Check for string end
			jeq stringEnd
			cmp #97, R5 			; Check if R5 contains a lower case character
			jl incString
			cmp #123, R5
			jge incString
			sub.b #32, 0(R4) 		; Subtract 32 to convert lower case to upper case
			jmp incString
incString:	inc.w R4
			jmp getNext
stringEnd:	jmp $

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

