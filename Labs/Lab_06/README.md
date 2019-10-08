Austin Bumbalough  
CPE 325-08  
Lab 06  
10/08/2019  
# Lab 6 Solution  
## Part 1 (Assembly)
In lab 6 part 1, I wrote an assembly program that uses interrupts triggered by on-board switches to manipulate two on-board LEDs.

When SW2 is pressed while LED1 is blinking nothing happens. Pressing SW2 does not disrupt the blinking LED. SW2 is functional, however, and this is not a program error. By default, when the MSP 430 enters an interrupt service routine (ISR) all other interrupts are disabled for the duration of the subroutine. The programmer has the option to explicitly enable interrupts during an ISR if such functionality is required.

### Source Code
```Assembly
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
RESET:      mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT:    mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer
;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
Setup:      bic.b #0x03, &P1DIR         ; Configure P1.0 and P1.1 as inputs
            bis.b #0x06, &P2DIR         ; Configure P2.1 and P2.2 as ouputs
            bic.b #0xFF, &P2OUT         ; Initialize LEDs to off
            bis.w #GIE, SR              ; Enable global interrupts
            bis.b #0x03, &P1IES         ; Trigger interrupts on falling edge since SW1 and SW2 have 100K pullup resistor
            bic.b #0x03, &P1IFG         ; Clear any pending interrupts
            bis.b #0x03, &P1IE          ; Enable interrupts on P1.0 and P1.1
            jmp $
;-------------------------------------------------------------------------------
; Port 1 Interrupt Service Routine
;-------------------------------------------------------------------------------
P1_ISR:     push R15
            bic.w #GIE, SR              ; Disable interrupts while executing subroutine
            bit.b #0x01, &P1IFG         ; Check for SW1 interrupt
            jnz S1                      ;
            bit.b #0x02, &P1IFG         ; Check for SW2 interrupt
            jnz S2                      ;
            bic.b #0xFF, &P1IFG         ; Clear P1IFG
            jmp ISR_exit
S1:         bit.b #0x01, &P1IN          ; Check if SW1 is pressed
            jnz ISR_exit                ;
            mov.w #2000, R15            ; (2 * 100) * 10 cc per loop
DbncSW1:    dec.w R15                   ; 2 ms debounce
            nop                         ;
            nop                         ;
            nop                         ;
            nop                         ;
            nop                         ;
            nop                         ;
            nop                         ;
            jnz DbncSW1                 ;
            bit.b #0x01, &P1IN          ; Check if SW1 is still pressed
            jnz ISR_exit                ;
            call #SW1                   ;
            xor.b #0x02, &P2OUT         ;
            bic.b #0x01, &P1IFG         ; Clear interrupt flag for SW1
            jmp ISR_exit                ;
S2:         bit.b #0x02, &P1IN          ; Check if SW2 is pressed
            jnz ISR_exit                ;
            mov.w #2000, R15            ; 
DbncSW2:    dec.w R15                   ;
            nop                         ;
            nop                         ;
            nop                         ;
            nop                         ;
            nop                         ;
            nop                         ;
            nop                         ;
            jnz DbncSW2                 ;
            bit.b #0x02, &P1IN          ; Check if SW2 is still pressed
            jnz ISR_exit                ;
            xor.b #0x02, &P2OUT         ; Toggle LED 2
            bic.b #0x02, &P1IFG         ;
ISR_exit:   pop R15                     ;
            reti                        ;
;-------------------------------------------------------------------------------
; Switch 1 Subroutine
;-------------------------------------------------------------------------------
SW1:        push R14                    ;
            push R15                    ;
            mov.w #6, R14               ; LED 1 toggle counter
sw1_loop:   mov.w #16667, R15           ;
sw1_delay:  dec.w R15                   ; 167 ms delay
            nop                         ;
            nop                         ;
            nop                         ;
            nop                         ;
            nop                         ;
            nop                         ;
            nop                         ;
            jnz sw1_delay               ;
            xor.b #0x04, &P2OUT         ; Toggle LED 1
            dec.w R14                   ;
            jnz sw1_loop                ;
            pop R15                     ;
            pop R14                     ;
            ret                         ;
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
            .sect   ".int20"
            .short  P1_ISR
            .end
```

## Part 2 (C)
In lab 6 part 1, I wrote a C program that uses interrupts triggered by on-board switches to manipulate two on-board LEDs.

Since the delay loop is a constant 50,000 iterations, the delay time is a function of the processor clock speed. The delay in milliseconds is [equation]http://www.sciweavers.org/tex2img.php?eq=D%28f%29%3D%5Cfrac%7B5%2A10%5E6%7D%7Bf%7D&bc=White&fc=Black&im=jpg&fs=12&ff=arev&edit=0[/img]
