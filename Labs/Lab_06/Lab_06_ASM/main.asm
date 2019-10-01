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

Setup:		bic.w BIT1|BIT0, P1DIR 		; Configure P1.0 and P1.1 as inputs
			bis.w BIT2|BIT1, P2DIR 		; Configure P2.1 and P2.2 as ouputs
			bis.w #GIE, SR 				; Enable global interrupts
			bis.w BIT1|BIT0, P1IES 		; Trigger interrupts on falling edge since SW1 and SW2 have 100K pullup resistor
			bic.w BIT1|BIT0, P1IFG 		; Clear any pending interrupts
			bis.w BIT1|BIT0, P1IE 		; Enable interrupts on P1.0 and P1.1


;-------------------------------------------------------------------------------
; Main loop here
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
            
