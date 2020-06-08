//#include "exp_vector.h"
.globl Iboot_IAP_SelectLoadProgam
.globl irq_handler
.globl fiq_handler
.globl Init_Cpu
.globl Iboot_LoadPreload
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

/*
 * Macro for switching ARM mode
 */
 .align 5
    .macro  BOOT_CHANGE_MODE, mode, mode_mask
    MRS   R0, CPSR
    BIC   R0, R0, #\mode_mask
    ORR   R0, R0, #\mode
    MSR   CPSR_c, R0
    .endm

/* ========================================================================
 *                      Stack and Heap Definitions
 * ========================================================================
 */
/*各种异常不会嵌套，用同一个栈*/
    .global cp15_enable_alignfault
    .section .data
    .space 0x0400
    .align 3
    .global und_stack_top
    .global abt_stack_top
und_stack_top:
abt_stack_top:
    .word 0

.text
.align 5
.globl ExpStackInit
.type ExpStackInit, %function
ExpStackInit:
    BOOT_CHANGE_MODE 0x1F 0x1F  /*System mode 系统模式*/
    ldr     sp,=msp_top
    bic     sp,sp,#0x07

    BOOT_CHANGE_MODE 0x17 0x1F /*Abort mode */
    ldr     sp,=abt_stack_top
    bic     sp,sp,#0x07

    BOOT_CHANGE_MODE 0x1B 0x1F  /*Undefined mode*/
    ldr     sp,=und_stack_top
    bic     sp,sp,#0x07

    BOOT_CHANGE_MODE 0x12 0x1F/*IRQ mode*/
    ldr     sp,=msp_top
    bic     sp,sp,#0x07

    BOOT_CHANGE_MODE 0x11 0x1F /*FIQ mode*/
    ldr     sp,=msp_top
    bic     sp,sp,#0x07
    sub     sp,sp,#0x44        /*IRQ响应到禁止FIQ之前，要用56字节，必须避开，否则会被覆盖*/

    //Clear FIQ banked registers while in FIQ mode
    MOV     R8,  #0
    MOV     R9,  #0
    MOV     R10, #0
    MOV     R11, #0
    MOV     R12, #0

    BOOT_CHANGE_MODE 0x13 0x1F  /*svc mode管理模式*/
    ldr     sp,=msp_top
    bic     sp,sp,#0x07
    bx      lr
    //==================

.text
.align 5
.globl Init_Cpu
.type Init_Cpu, %function
Init_Cpu:
    //Disable IRQ and FIQ before starting anything
    MRS   R0, CPSR
    ORR   R0, R0, #0xC0
    MSR   CPSR_c, R0

    ldr     r1,=0x802044        /*禁止CPU中断控制器的FIQ和IRQ位*/
    mov     r2,#0
    str     r2,[r1]

//以下程序用于bk7251狗叫复位后，从cpu寄存器中获取复位信息
//	ldr     r8,=0x00400000
//	ldr 	r7, [r8]

//	ldr     r6,=0x12345678
//	cmp		r7, r6
//	beq		soft
//	ldr     r6,=0x87654321
//	cmp		r7, r6
//	beq		soft

//	str		r9, [r8]
//	ldr     r8,=0x00400004
//	str		r10, [r8]

//soft:
//	MOV R6,  #0
//	MOV R7,  #0
//	MOV R8,  #0
//	MOV R9,  #0
//	MOV R10,  #0
//
    BL    ExpStackInit
    //Clear Registers
    MOV R0,  #0
    MOV R1,  #0
    MOV R2,  #0
    MOV R3,  #0
    MOV R4,  #0
    MOV R5,  #0
    MOV R6,  #0
    MOV R7,  #0
    MOV R8,  #0
    MOV R9,  #0
    MOV R10, #0
    MOV R11, #0
    MOV R12, #0

    bl  cp15_enable_alignfault
    ldr     pc, =Iboot_IAP_SelectLoadProgam



//注意，Init_Cpu将在Iboot中被连接，AppStart将在APP中被连接。
//ExpStackInit中各状态的栈指针，将由Iboot.lds或app.lds分配，其值可能不一样。
.section ".text.AppStart", "ax"
.align 5
.globl AppStart
.type AppStart, %function
AppStart:
    //Disable IRQ and FIQ before starting anything
    MRS   R0, CPSR
    ORR   R0, R0, #0xC0
    MSR   CPSR_c, R0

    ldr     r1,=0x802044        /*禁止CPU中断控制器的FIQ和IRQ位*/
    mov     r2,#0
    str     r2,[r1]

    BL    ExpStackInit
    //Clear Registers
    MOV R0,  #0
    MOV R1,  #0
    MOV R2,  #0
    MOV R3,  #0
    MOV R4,  #0
    MOV R5,  #0
    MOV R6,  #0
    MOV R7,  #0
    MOV R8,  #0
    MOV R9,  #0
    MOV R10, #0
    MOV R11, #0
    MOV R12, #0

    bl  cp15_enable_alignfault
    bl  Iboot_LoadPreload

.text
.align 5
.globl __asm_reset_msp
.type __asm_reset_msp, %function
__asm_reset_msp:
    MRS   R0, CPSR
    ORR   R0, R0, #0xC0
    MSR   CPSR_c, R0
    BOOT_CHANGE_MODE 0x13 0x1F  /*Undefined mode管理模式*/
    ldr     sp,=msp_top
    bic     sp,sp,#0x07
    //==================
    //Clear Registers
    MOV R0,  #0
    MOV R1,  #0
    MOV R2,  #0
    MOV R3,  #0
    MOV R4,  #0
    MOV R5,  #0
    MOV R6,  #0
    MOV R7,  #0
    MOV R8,  #0
    MOV R9,  #0
    MOV R10, #0
    MOV R11, #0
    MOV R12, #0
    mov pc ,lr

.text
.align 5
.globl GetSvcSP
.type GetSvcSP, %function
GetSvcSP:
    MRS     R1, CPSR
    BOOT_CHANGE_MODE 0x13 0x1F
    mov r0,sp
    msr CPSR, r1
    BX LR

//.text
//.align 5
//.globl WrResetToReg
//.type WrResetToReg, %function
//WrResetToReg:
//	ldr     r8,=0x00400000
//	ldr     r9,[r8]
//	ldr     r8,=0x00400004
//	ldr     r10,[r8]

//    BX      LR
    .end

