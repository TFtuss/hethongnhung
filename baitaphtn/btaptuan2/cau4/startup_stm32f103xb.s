.syntax unified
.cpu cortex-m3
.thumb

.global g_pfnVectors
.global Reset_Handler
.extern SystemInit
.extern __libc_init_array
.extern main

.section .isr_vector, "a", %progbits
.type g_pfnVectors, %object
g_pfnVectors:
    .word _estack
    .word Reset_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word 0
    .word 0
    .word 0
    .word 0
    .word Default_Handler
    .word Default_Handler
    .word 0
    .word Default_Handler
    .word Default_Handler
    .rept 68
    .word Default_Handler
    .endr

.section .text.Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
    ldr r0, =_sidata
    ldr r1, =_sdata
    ldr r2, =_edata
    subs r2, r2, r1
    ble init_bss
copy_data:
    ldr r3, [r0], #4
    str r3, [r1], #4
    subs r2, r2, #4
    bgt copy_data
init_bss:
    ldr r1, =_sbss
    ldr r2, =_ebss
    movs r3, #0
clear_bss:
    cmp r1, r2
    bge call_main
    str r3, [r1], #4
    adds r1, r1, #4
    b clear_bss
call_main:
    bl SystemInit
    bl __libc_init_array
    bl main
hang:
    b hang

.section .text.Default_Handler, "ax", %progbits
.type Default_Handler, %function
Default_Handler:
    b Default_Handler