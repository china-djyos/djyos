#include "arm_mode_s_file.h"
    .equ    INTOFFSET, 0x4a000014

         .arch armv7-a
         .arm
         .text
/* cpsr的IF位清0，允许全部中断 */
        .text
        .align 2
        .global int_enable_arm_int
int_enable_arm_int:
    cpsie   if
    bx  lr

/* cpsr的IF位置1，禁止全部中断 */
        .text
        .align 2
        .global int_disable_arm_int
int_disable_arm_int:
    cpsid   if
        bx      lr

/* cpsr的I位置1，禁止irq中断 */
    .text
        .align 2
        .global __int_disable_irq
__int_disable_irq:
    cpsid   i
    bx  lr

/* cpsr的I位清0，允许irq中断 */
    .text
        .align 2
        .global __int_enable_irq
__int_enable_irq:
    cpsie   i
    bx  lr

/* cpsr的F位置1，禁止fiq中断 */
    .text
    .align 2
    .global __int_disable_fiq
__int_disable_fiq:
    cpsid   f
    bx  lr

/* cpsr的F位清0，允许fiq中断 */
    .text
    .global __int_enable_fiq
__int_enable_fiq:
    cpsie   f
    bx  lr

#if 0
@ ----进入高级原子操作---------------------------------------------------------
@ 功能：读出当前总中断状态，然后禁止总中断。高级原子操作是指期间不容许任何原因打断的操作。
@      Int_HighAtomStart--int_high_atom_end必须配套使用，在被他们套住的代码块内不
@      允许调用用任何系统调用。
@  参数：无
@  返回：原来的原子状态
@ 备注：移植敏感
@ -----------------------------------------------------------------------------//
.section .text.Int_HighAtomStart, "ax", %progbits
.global Int_HighAtomStart
Int_HighAtomStart:
        mrs     r0, cpsr
        mov r1, r0
        orr     r1, r1, #SF_IRQ_BIT|SF_FIQ_BIT
        msr     cpsr_c, r1

@ ----离开高级原子操作---------------------------------------------------------
@ 功能：恢复进入离开原子操作前的总中断状态，本函数应与int_high_atom_start函数配套
@       Int_HighAtomStart--int_high_atom_end必须配套使用，在被他们套住的代码块内不
@      允许调用用任何系统调用。
@ 参数：high，int_high_atom_start保存的原子状态
@ 返回：无
@ 备注：移植敏感
.section .text.Int_HighAtomEnd, "ax", %progbits
.global Int_HighAtomEnd
Int_HighAtomEnd:
        msr cpsr_c, r0
@  *中断相关的栈安排：
@  *1、irq_stack 放被中断的上下文，顺序:lr，r12-r0，cpsr，共14字
@  *2、svc_stack 运行中断服务函数，包括用户编写的中断服务函数。
@  *3、SYS_stack 除非要在中断里切换上下文，否则无关
#endif

    .section .text.start_int, "ax", %progbits
    .global notused__HardExp_IrqHandler
notused__HardExp_IrqHandler:
@   .equ    INTOFFSET, 0x4a000014   /* [猜]中断寄存器地址，应该是硬件的接口。 */
    stmfd   sp!,{r0-r12,lr}     /* 保护寄存器,以及返回地址 */
    ldr r0,=INTOFFSET
    ldr r0,[r0]

    mrs     r1,spsr
    stmfd   sp!,{r1}        /* 保护SPSR_irq，以支持中断嵌套 */


    msr     cpsr_c,#SF_SVCMODE|SF_IRQ_BIT   /* 进入SVCMODE,以便允许中断嵌套 */
    stmfd   sp!,{r0,lr}             /* 保存lr_svc,r0是保持栈8字节对齐的填充物 */

    /*ldr     r2,=irq_bottom                     取异步信号地址 */
//    ldr         r2, =engine_irq         /* 取异步信号地址 */
    ldr r2, =0

    mov     lr,pc               /* 绝对地址调用 */
    ldr     pc,[r2]             /* intn用r0传递 */

    ldmfd   sp!,{r0,lr}     /* 恢复lr_svc, */
    msr     cpsr_c,#SF_IRQMODE|SF_IRQ_BIT|SF_FIQ_BIT   /* 更新cpsr,进入IRQ模式并禁止中断,由于lr寄存
                                        器的问题,进入irq或者fiq模式，均要禁止中断以防嵌套，
                                        只有在进入svc状态才允许嵌套 */
    ldmfd   sp!,{r0}        /* spsr->r0 */
    msr     spsr_cxsf,r0    /* 恢复spsr */
    ldmfd   sp!,{r0-r12,lr}
    subs    pc,lr,#4        /* 此后，中断被重新打开 */


/* fiq */

        .section .text.start_fiq, "ax", %progbits
        .global notused__HardExp_FiqHandler
notused__HardExp_FiqHandler:
        b       .
.end

