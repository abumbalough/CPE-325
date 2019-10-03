;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
; *
; File: Lab_06/Lab_06_ASM/main.asm
; *
; Description: Use interrupt service routines to blink LEDs according to pressed switches
; *
; Input: On-board switches SW1 and SW2
; *
; Output: On-board LEDs LED1 and LED2
; *
; Author: Austin Bumbalough
; *
; Lab Section: 8
; *
; Date: 10/08/2019
; *
;
; SW1 - P1.0
; SW2 - P1.1
; LED1 - P2.2
; LED2 - P2.1
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

;-------------------------------------------------------------------------------
RESET:		mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT:	mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer

Setup:		bic.b #03h, &P1DIR 		; Configure P1.0 and P1.1 as inputs
			bis.b #06h, &P2DIR 		; Configure P2.1 and P2.2 as ouputs
			bic.b #06h, &P2OUT		; Initialize LEDs to off
			bis.w #GIE, &SR 		; Enable global interrupts
			bis.b #03h, &P1IES 		; Trigger interrupts on falling edge since SW1 and SW2 have 100K pullup resistor
			bic.b #03h, &P1IFG 		; Clear any pending interrupts
			bis.b #03h, &P1IE 		; Enable interrupts on P1.0 and P1.1
;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------

;-------------------------------------------------------------------------------
; Port 1 Interrupt Service Routine
;-------------------------------------------------------------------------------
P1_ISR:		push R15

			bic.w #GIE, SR				; Disable interrupts while executing subroutine
			bit.b #01h, &P1IFG			; Check for SW1 interrupt
			jnz SW1
			bit.b #02h, &P1IFG			; Check for SW2 interrupt
			jnz SW2

SW1:		bic.b #01, &P1IFG	; Clear interrupt flag for SW1
			bit.b #01, &P1IN	; Check if SW1 is pressed
			jnz ISR_Exit

			mov.w #2000, R15	; 2000 * 10 cc per loop
Debounce:	dec.w R15	;
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			jnz Debounce
			bit.b #01, &P1IN	;


SW2:		bic.b #02, &P1IFG
			xor.b #02h, &P2OUT			; Toggle LED 2

                                            

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
            .sect 	".int20"
            .short 	P1_ISR
            .end
            
