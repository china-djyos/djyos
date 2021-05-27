//#include "exp_vector.h"
.globl irq_handler
.globl fiq_handler
.globl Init_Cpu
/*
 *==============================================================================
 */
.section ".vectors", "ax"
.align  3
.global isr_vector
isr_vector:
    b   Init_Cpu
    ldr pc, _undefined_instruction
    ldr pc, _software_interrupt
    ldr pc, _prefetch_abort
    ldr pc, _data_abort
    ldr pc, _not_used
    ldr pc, _irq
    ldr pc, _fiq


_undefined_instruction: .word HardExp_UndefHandler
_software_interrupt:    .word HardExp_SwiHandler
_prefetch_abort:        .word HardExp_PrefetchHandler
_data_abort:            .word HardExp_DataAbortHandler
_not_used:              .word HardExp_Not_UsedHandler
_irq:                   .word irq_handler
_fiq:                   .word fiq_handler

 .align 5
    .macro  SPI_TO_JTAG
    push    {r0-r3}

    ldr     r0,=(0x0802800 + 46*4)
    ldr     r1,[r0]
    ldr     r2,=0xFFFFFCFF
    and     r1,r2
    ldr     r2,=0x100
    orr     r1,r2
    str     r1,[r0]

    ldr     r0,=(0x0802800 + 46*4)
    ldr     r1,[r0]
    ldr     r2,=0xFFFFF3FF
    and     r1,r2
    ldr     r2,=0x400
    orr     r1,r2
    str     r1,[r0]

    ldr     r0,=(0x0802800 + 46*4)
    ldr     r1,[r0]
    ldr     r2,=0xFFFFCFFF
    and     r1,r2
    ldr     r2,=0x1000
    orr     r1,r2
    str     r1,[r0]

    ldr     r0,=(0x0802800 + 46*4)
    ldr     r1,[r0]
    ldr     r2,=0xFFFF3FFF
    and     r1,r2
    ldr     r2,=0x4000
    orr     r1,r2
    str     r1,[r0]

    pop     {r0-r3}
    .endm

/*
 * exception handlers
 */
.equ     CN_VECTOR_RESET,     0
.equ     CN_VECTOR_UND,       1
.equ     CN_VECTOR_SWI,       2
.equ     CN_VECTOR_PABT,      3
.equ     CN_VECTOR_DABT,      4
.equ     CN_VECTOR_NOTUSE,    5
.equ     CN_VECTOR_IRQ,       6
.equ     CN_VECTOR_FIQ,       7
    .align  5
HardExp_UndefHandler:
    push  {r0-r12,lr}
    mrs r0, cpsr
    orr r0, r0, #0xc0          @; disable interrupt
    msr cpsr_c, r0
    mov r0,sp
    ldr r1,=g_u32ExpTable
    ldr r1,[r1,#CN_VECTOR_UND * 4]
    mov lr, pc
    bx  r1
    B   .
    pop  {r0-r12,lr}
    SUBS    PC,LR,#4

    .align  5
HardExp_SwiHandler:
    push  {r0-r12,lr}
    mrs r0, cpsr
    orr r0, r0, #0xc0          @; disable interrupt
    msr cpsr_c, r0
    mov r0,sp
    ldr r1,=g_u32ExpTable
    ldr r1,[r1,#CN_VECTOR_SWI * 4]
    mov lr, pc
    bx  r1
    B   .
    pop  {r0-r12,lr}
    SUBS    PC,LR,#4

    .align  5
HardExp_PrefetchHandler:
    push  {r0-r12,lr}
    mrs r0, cpsr
    orr r0, r0, #0xc0          @; disable interrupt
    msr cpsr_c, r0
    mov r0,sp
    ldr r1,=g_u32ExpTable
    ldr r1,[r1,#CN_VECTOR_PABT * 4]
    mov lr, pc
    bx  r1
    B   .
    pop  {r0-r12,lr}
    SUBS    PC,LR,#4

    .align  5
HardExp_DataAbortHandler:
    push  {r0-r12,lr}
    mrs r0, cpsr
    orr r0, r0, #0xc0          @; disable interrupt
    msr cpsr_c, r0
    mov r0,sp
    ldr r1,=g_u32ExpTable
    ldr r1,[r1,#CN_VECTOR_DABT * 4]
    mov lr, pc
    bx  r1
    B   .
    pop  {r0-r12,lr}
    SUBS    PC,LR,#4

    .align  5
HardExp_Not_UsedHandler:
    push  {r0-r12,lr}
    mrs r0, cpsr
    orr r0, r0, #0xc0          @; disable interrupt
    msr cpsr_c, r0
    mov r0,sp
    ldr r1,=g_u32ExpTable
    ldr r1,[r1,#CN_VECTOR_NOTUSE * 4]
    mov lr, pc
    bx  r1
    B   .
    pop  {r0-r12,lr}
    SUBS    PC,LR,#4

