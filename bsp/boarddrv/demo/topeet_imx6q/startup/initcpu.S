//----------------------------------------------------------------
// ARMv7-A Embedded example - Startup Code
//
// Copyright (c) 2005-2018 Arm Limited (or its affiliates). All rights reserved.
// Use, modification and redistribution of this file is subject to your possession of a
// valid End User License Agreement for the Arm Product of which these examples are part of
// and your compliance with all applicable terms and conditions of such licence agreement.
//----------------------------------------------------------------

// Standard definitions of mode bits and interrupt (I & F) flags in PSRs
#include "arm_mode_s_file.h"

#define __irq_stack_size        0x1000
#define __fiq_stack_size        0x1000
#define __usr_stack_size        0x1000
#define __svc_stack_size        0x1000
#define __und_stack_size        0x1000
#define __abt_stack_size        0x1000
#define __mon_stack_size        0x1000
#define __hyp_stack_size        0x1000
//注：sys态的栈在 sys_init.c中定义。

    .section .stack
    .align  4
    .globl  __irq_stack_top
    .globl  __irq_stack_base
    .globl  __irq_stack_limit
__irq_stack_top:
__irq_stack_limit:
    .space  __irq_stack_size
__irq_stack_base:

    .globl  __fiq_stack_top
    .globl  __fiq_stack_base
    .globl  __fiq_stack_limit
__fiq_stack_top:
__fiq_stack_limit:
    .space  __fiq_stack_size
__fiq_stack_base:

    .globl  __usr_stack_top
    .globl  __usr_stack_base
    .globl  __usr_stack_limit
__usr_stack_tp:
__usr_stack_limit:
    .space  __usr_stack_size
__usr_stack_base:

    .globl  __sys_stack_top
    .globl  __sys_stack_base
    .globl  __sys_stack_limit
//注：sys态的栈在 sys_init.c中定义。

    .globl  __svc_stack_top
    .globl  __svc_stack_base
    .globl  __svc_stack_limit
__svc_stack_top:
__svc_stack_limit:
    .space  __svc_stack_size
__svc_stack_base:

    .globl  __und_stack_top
    .globl  __und_stack_base
    .globl  __und_stack_limit
__und_stack_top:
__und_stack_limit:
    .space  __und_stack_size
__und_stack_base:

    .globl  __abt_stack_top
    .globl  __abt_stack_base
    .globl  __abt_stack_limit
__abt_stack_top:
__abt_stack_limit:
    .space  __abt_stack_size
__abt_stack_base:

    .globl  __mon_stack_top
    .globl  __mon_stack_base
    .globl  __mon_stack_limit
__mon_stack_top:
__mon_stack_limit:
    .space  __mon_stack_size
__mon_stack_base:

    .globl  __hyp_stack_top
    .globl  __hyp_stack_base
    .globl  __hyp_stack_limit
__hyp_stack_top:
__hyp_stack_limit:
    .space  __hyp_stack_size
__hyp_stack_base:

    .section  StartupExpTbl, "ax"
    .align 3
    .cfi_sections .debug_frame  // put stack frame info into .debug_frame instead of .eh_frame
//----------------------------------------------------------------
// Entry point for the Reset handler
//----------------------------------------------------------------

    .global _start
//----------------------------------------------------------------
// Exception Vector Table
//----------------------------------------------------------------
// Note: LDR PC instructions are used here, though branch (B) instructions
// could also be used, unless the exception handlers are >32MB away.

    .global __djy_vectors
    .text
    .arm
    .align 12
_start:
__djy_vectors:
    ldr pc, __djy_reset
    ldr pc, __djy_undefined
    ldr pc, __djy_swi_interrupt
    ldr pc, __djy_prefetch_handler
    ldr pc, __djy_abort_handler
    ldr pc, __djy_reserved_handler
    ldr pc, __djy_irq_handler
    ldr pc, __djy_fiq_handler
//----------------------------------------------------------------
// Exception Handlers
//----------------------------------------------------------------
__djy_reset:
    .word   djy_reset
__djy_undefined:
    .word   djy_undefined
__djy_swi_interrupt:
    .word   djy_swi_interrupt
__djy_prefetch_handler:
    .word   djy_prefetch_handler
__djy_abort_handler:
    .word   djy_abort_handler
__djy_reserved_handler:
    .word   djy_reserved_handler
__djy_irq_handler:
    .word   djy_irq_handler
__djy_fiq_handler:
    .word   djy_fiq_handler
    b   .

djy_reset:
    mov r4, r0           @ save r0 for cores 1-3, r0 arg field passed by ROM
                         @ r0 is a function pointer for secondary cpus
    mrc p15,0,r2,c0,c0,5 @ read multiprocessor affinity register
    and r2, r2, #3       @ mask off, leaving CPU ID field

   //// 初始化栈指针
    mov     r1,#SF_UNDEFMODE|SF_IRQ_BIT|SF_FIQ_BIT
    msr     cpsr_cxsf,r1        //UndefMode
    ldr     sp,=__und_stack_limit
    bic     sp,sp,#0x07

    mov     r1,#SF_ABORTMODE|SF_IRQ_BIT|SF_FIQ_BIT
    msr     cpsr_cxsf,r1        //AbortMode
    ldr     sp,=__abt_stack_limit
    bic     sp,sp,#0x07

    mov     r1,#SF_IRQMODE|SF_IRQ_BIT|SF_FIQ_BIT
    msr     cpsr_cxsf,r1        //IRQMode
    ldr     sp,=__irq_stack_limit
    bic     sp,sp,#0x07

    mov     r1,#SF_FIQMODE|SF_IRQ_BIT|SF_FIQ_BIT
    msr     cpsr_cxsf,r1        //FIQMode
    ldr     sp,=__fiq_stack_limit
    bic     sp,sp,#0x07

    mov     r1,#SF_SVCMODE|SF_IRQ_BIT|SF_FIQ_BIT
    msr     cpsr_cxsf,r1        //SVCMode
    ldr     sp,=__svc_stack_limit
    bic     sp,sp,#0x07

    mov     r1,#SF_SYSMODE|SF_IRQ_BIT|SF_FIQ_BIT
    msr     cpsr_cxsf,r1        //SYSMode
    ldr     sp,=__sys_stack_limit
    bic     sp,sp,#0x07
    nop
    nop
//----------------------------------------------------------------
// Set Vector Base Address Register (VBAR) to point to this application's vector table
//----------------------------------------------------------------
    ldr r0, =__djy_vectors
    mcr p15, 0, r0, c12, c0, 0

	@ Invalidate caches
	bl      invalidateCaches

	@ Clear Branch Prediction Array
	mov     r0, #0x0
	mcr     p15, 0, r0, c7, c5, 6     @ BPIALL - Invalidate entire branch predictor array

	BL	enableCaches

    MRC   p15, 0, r0, c0 ,c0, 5 // Read Multiprocessor ID register (MPIDR)
    and   r0, #3
    cmp   r0, #0
    bne   123f

start_secondary_cpus:
    ldr   r3, =0x20D8000
    ldr   r1, =0x40000       // iboot text size, see memory.lds
    ldr   r2, =0x10200000    //  =_start core 0

    add   r2, r2, r1         // entry for core 1
    str   r2, [r3, #0x28]    // Holds entry function for core1

    add   r2, r2, r1         // =_start of core 2
    str   r2, [r3, #0x30]    // Holds entry function for core2

    add   r2, r2, r1
    str   r2, [r3, #0x38]    // core 3

    ldr   r2, =29360128      // start cores 1-3
    str   r2, [r3, #0]

123:
    ldr r0, __djy_iboot
    bx  r0
    b   .   /* stop */

__djy_iboot:
#if (CFG_RUNMODE_BAREAPP == 1)
.word   Iboot_LoadPreload
#else
.word   Iboot_LoadPreload
.word   Iboot_IAP_SelectLoadProgam
#endif

    .section .text.AppStart, "ax", %progbits
    .align  2
    .global AppStart
AppStart:
    nop
    /* setup vector table */
appcopy_isr_table:
    /* prepare isr table */
    /* setup stack for modes */
    ldr r0, __djy_app
    blx r0

    bx  lr  /* return to iboot? */
    b   .   /* stop */
__djy_app:
.word   Iboot_LoadPreload
    .end

