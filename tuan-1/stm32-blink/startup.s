.syntax unified
.cpu cortex-m3
.thumb

.global Reset_Handler
.global _estack

.section .isr_vector, "a", %progbits
.word _estack
.word Reset_Handler
.rept 14
.word Default_Handler
.endr

.section .text.Reset_Handler
.thumb_func
Reset_Handler:
    ldr r0, =_sidata
    ldr r1, =_sdata
    ldr r2, =_edata

copy_data:
    cmp r1, r2
    bcc copy_word
    b zero_bss

copy_word:
    ldr r3, [r0], #4
    str r3, [r1], #4
    b copy_data

zero_bss:
    ldr r1, =_sbss
    ldr r2, =_ebss
    movs r0, #0

zero_word:
    cmp r1, r2
    bcc clear_word
    bl main

hang:
    b hang

clear_word:
    str r0, [r1], #4
    b zero_word

.thumb_func
Default_Handler:
    b Default_Handler
