/*#include "cpu.h"*/

    .globl _vector_start
	.globl	_undefined_instruction
	.globl	_software_interrupt
	.globl	_prefetch_abort
	.globl	_data_abort
	.globl	_not_used
	.globl	_irq
	.globl	_fiq
	.globl	msp_top
	
	.extern boot_reset
	.extern boot_swi
	.extern boot_undefined
	.extern boot_pabort
	.extern boot_dabort
	.extern boot_reserved
	.extern irq_handler
	.extern fiq_handler
    .extern IAP_SelectLoadProgam
    

#define _FIQ_STACK_SIZE_                  0xFF0
#define _IRQ_STACK_SIZE_                  0xFF0
#define _SVC_STACK_SIZE_                  0x3F0
#define _SYS_STACK_SIZE_                  0x3F0
#define _UND_STACK_SIZE_                  0x010
#define _ABT_STACK_SIZE_                  0x010

#define BOOT_MODE_MASK                    0x1F
#define BOOT_MODE_USR                     0x10
#define BOOT_MODE_FIQ                     0x11
#define BOOT_MODE_IRQ                     0x12
#define BOOT_MODE_SVC                     0x13
#define BOOT_MODE_ABT                     0x17
#define BOOT_MODE_UND                     0x1B
#define BOOT_MODE_SYS                     0x1F
#define BOOT_FIQ_IRQ_MASK                 0xC0
#define BOOT_IRQ_MASK                     0x80
    /* ========================================================================
                    restore macro definitions
 * ======================================================================== */

/* ========================================================================
/**
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
/**
 * Macro for setting the stack
 */
 .align 5
    .macro  BOOT_SET_STACK, stackStart, stackLen, color
    LDR   R1, \stackStart
    LDR   R0, \stackLen

    SUB   R0, R1, R0
    MOV   SP, R1        //Set stack pointer

    LDR   R2, =\color

3:
    CMP   R0, R1        //End of stack?
    STRLT R2, [r0]      //Colorize stack word
    ADDLT R0, R0, #4
    BLT   3b           //branch to previous local label
    .endm
/* ========================================================================
 *                      Stack and Heap Definitions
 * ========================================================================
 */
    .section .data
    .space 0x010
    .align 3
    .global und_stack_start
und_stack_start:

    .space 0x010
    .align 3
    .global abt_stack_start
abt_stack_start:

    .space 0xFF0
    .align 3
    .global fiq_stack_start
fiq_stack_start:

    .space 0xFF0
    .align 3
    .global irq_stack_start
irq_stack_start:

    .skip 0x3F0
    .align 3
    .global sys_stack_start
sys_stack_start:

    .space 0x3F0
    .align 3
    .global svc_stack_start
svc_stack_start:
	.word 0x00000000

.section ".rom1", "ax"
 .align 5
 .globl boot_reset
.type boot_reset, %function
boot_reset:
    //Disable IRQ and FIQ before starting anything
    MRS   R0, CPSR
    ORR   R0, R0, #0xC0
    MSR   CPSR_c, R0

    //Setup all stacks //Note: Abt and Usr mode are not used
    BOOT_CHANGE_MODE 0x1F 0x1F
    //BOOT_SET_STACK   boot_stack_base_SYS 0x3F0 0xee
	ldr     sp,=sys_stack_start
    bic     sp,sp,#0x07

    BOOT_CHANGE_MODE 0x17 0x1F
    //BOOT_SET_STACK   boot_stack_base_UNUSED boot_stack_len_UNUSED 0xAAAAAAAA
    ldr     sp,=und_stack_start
    bic     sp,sp,#0x07

    BOOT_CHANGE_MODE 0x1B 0x1F
    //BOOT_SET_STACK   boot_stack_base_UNUSED boot_stack_len_UNUSED 0xAAAAAAAA
	ldr     sp,=und_stack_start
    bic     sp,sp,#0x07

    BOOT_CHANGE_MODE 0x12 0x1F
    //BOOT_SET_STACK   boot_stack_base_IRQ boot_stack_len_IRQ 0xCCCCCCCC
	ldr     sp,=irq_stack_start
    bic     sp,sp,#0x07

    BOOT_CHANGE_MODE 0x11 0x1F
    //BOOT_SET_STACK   boot_stack_base_FIQ boot_stack_len_FIQ 0xDDDDDDDD
	ldr     sp,=fiq_stack_start
    bic     sp,sp,#0x07

    //Clear FIQ banked registers while in FIQ mode
    MOV     R8,  #0
    MOV     R9,  #0
    MOV     R10, #0
    MOV     R11, #0
    MOV     R12, #0

    BOOT_CHANGE_MODE 0x13 0x1F
    //BOOT_SET_STACK   boot_stack_base_SVC boot_stack_len_SVC BOOT_COLOR_SVC
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

    ldr     pc, =IAP_SelectLoadProgam

    
