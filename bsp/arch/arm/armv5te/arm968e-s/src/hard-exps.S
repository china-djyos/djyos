.globl Init_Cpu
.globl irq_handler
.globl fiq_handler


/*
 *==============================================================================
 */
.section ".vectors", "ax"
.align  3
.global isr_vector
isr_vector:
    b   _start
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
_not_used:    	        .word HardExp_Not_UsedHandler
_irq:                   .word irq_handler
_fiq:                   .word fiq_handler

/*
 * exception handlers
 */
    .align  5
HardExp_UndefHandler:
    B   .
    SUBS    PC,LR,#4

    .align  5
HardExp_SwiHandler:
    B   .

    .align  5
HardExp_PrefetchHandler:
    B   .
    NOP
    NOP
    SUBS        PC,LR,#0

    .align  5
HardExp_DataAbortHandler:
    NOP
    NOP
    B   .
    NOP
    NOP
    SUBS PC,LR,#0

    .align  5
HardExp_Not_UsedHandler:
    B     .

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
.globl _start
.type _start, %function
_start:
    //Disable IRQ and FIQ before starting anything
    MRS   R0, CPSR
    ORR   R0, R0, #0xC0
    MSR   CPSR_c, R0

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
    sub		sp,sp,#0x40        /*IRQ响应到禁止FIQ之前，要用56字节，必须避开，否则会被覆盖*/

    //Clear FIQ banked registers while in FIQ mode
    MOV     R8,  #0
    MOV     R9,  #0
    MOV     R10, #0
    MOV     R11, #0
    MOV     R12, #0

    BOOT_CHANGE_MODE 0x13 0x1F  /*svc mode管理模式*/
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

    ldr     pc, =Init_Cpu

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

.end

