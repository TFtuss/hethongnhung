.syntax unified
.cpu cortex-m3
.thumb

.section .isr_vector, "a", %progbits
.global vector_table
.type vector_table, %object
vector_table:
    .word _estack
    .word Reset_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler

.section .text.Reset_Handler, "ax", %progbits
.global Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
    ldr r0, =_sidata
    ldr r1, =_sdata
    ldr r2, =_edata
copy_data:
    cmp r1, r2
    bcs zero_bss
    ldr r3, [r0]
    str r3, [r1]
    adds r0, #4
    adds r1, #4
    b copy_data
zero_bss:
    ldr r1, =_sbss
    ldr r2, =_ebss
    movs r3, #0
zero_loop:
    cmp r1, r2
    bcs call_main
    str r3, [r1]
    adds r1, #4
    b zero_loop
call_main:
    bl main
hang:
    b hang

.section .text.Default_Handler, "ax", %progbits
.global Default_Handler
.type Default_Handler, %function
Default_Handler:
    b .
