;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
; *
; File: Lab_04/part1_solution.asm
; *
; Description: Count the number of mathematical operators
;              in a string
; *
; Input: Data stored in memory
; *
; Output: Debug data browser
; *
; Author: Austin Bumbalough
; *
; Lab Section: 8
; *
; Date: 09/17/2019
; *
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            .align 2						; Align to words
            
testStr:	.cstring "Do 42+53/76%8=2*8-32+71 & you can sleep." ; Program input string

opChars:	.cstring "%&*+-/<>=^~"          ; Allocates 11 bytes for math operator characters
			.data
charCount:	.word 0

;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
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
            clr.w R8 				; Clear R8 to store the matched character count
            mov.w #charCount, R10	; Move memory address of counter variable to R10

            mov.w #testStr, R4 		; Move address of first test string character into R4
getNext:    mov.w #opChars, R5 		; Move address of first operator character into R5
          	mov.b @R4+, R6 			; Move next test character into R6 and increment R4 to next character address
			cmp.b #0, R6 			; Check if the current test character is the null character
			jeq stringEnd 			; If the current test character is null character, jump to program end
testOp:		mov.b @R5+, R7 			; Move next operator charcter into R7 and increment R5 to next operator character
			cmp.b #0, R7 			; Check if all operator characters have been tested
			jeq getNext 			; All operator characters have been compared to current test character
			cmp.b R6, R7 			; Check if current test character matches current operator character
			jne testOp 				; If not match, get next operator character and repeat
			inc.w charCount			; If the characters match, increment the counter register and
			jmp getNext 			; Get next operator charcter and repeat
stringEnd:	jmp $ 					; End of program, unconditional jump to current location



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
            
