.syntax unified
.cpu cortex-m3
.thumb
.global g_pfnVectors
.global Reset_Handler
.section .isr_vector,"a",%progbits
g_pfnVectors:
 .word _estack
 .word Reset_Handler
 .rept 14
 .word Default_Handler
 .endr
 .word SysTick_Handler
 .rept 68
 .word Default_Handler
 .endr
.section .text.Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
 ldr r0, =_estack
 mov sp, r0
 ldr r0, =_sidata
 ldr r1, =_sdata
 ldr r2, =_edata
1: cmp r1, r2
 bcs 2f
 ldr r3, [r0]
 str r3, [r1]
 adds r0, r0, #4
 adds r1, r1, #4
 b 1b
2: ldr r1, =_sbss
 ldr r2, =_ebss
 movs r0, #0
3: cmp r1, r2
 bcs 4f
 str r0, [r1]
 adds r1, r1, #4
 b 3b
4: bl SystemInit
 bl main
5: b 5b
.weak Default_Handler
.type Default_Handler, %function
Default_Handler: b .
