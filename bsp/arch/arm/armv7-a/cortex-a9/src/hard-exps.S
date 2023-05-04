#include "arm_mode_s_file.h"

.macro readIntAck ret
    mrc p15, 4, \ret, c15, c0, 0  // Read periph base address
    ldr \ret, [\ret, #0x010C]       // Read the Interrupt Acknowledge Register
.endm

    .text
    .global djy_reset2
    .global djy_undefined
    .global djy_swi_interrupt
    .global djy_prefetch_handler
    .global djy_abort_handler
    .global djy_reserved_handler
    .global djy_irq_handler
    .global djy_fiq_handler
    .align  2
djy_reset2:
    b   .
djy_undefined:
    nop
    nop
    nop
    b   .
    movs    pc, r14
djy_swi_interrupt:
    b   .
    movs    pc, r14
djy_prefetch_handler:
    nop
    nop
    nop
    b   .
    subs    pc, r14, #4
djy_abort_handler:
    nop
    nop
    nop
    b   .
    subs    pc, r14, #8
djy_reserved_handler:
    b   .
djy_irq_handler:
@    cpsid   i
@   +----------+
@   | pc       | <-- __irq_stack_base
@   | lr       |
@   | r0 - r12 |
@   | spsr     |
@   +----------+
@   +----------+
    sub lr, lr, #4
    push    {lr}        /* pc */
    stmfd   sp, {lr}^   /* lr */
    sub sp, #4
    push   {r0-r12}          /* 保护寄存器,以及返回地址 */

    mrs     r1,spsr
    push   {r1}                 /* 保护SPSR_irq，以支持中断嵌套 */

    mov r0, sp
    stmfd   r0, {r0,sp}^   /* sp_sys,r0 用于填充至8字节对齐 */
    sub sp, r0, #8

    readIntAck r0

    msr     cpsr_c,#SF_SVCMODE|SF_IRQ_BIT   /* 进入SVCMODE,以便允许中断嵌套 */
    push   {r0,lr}             /* 保存lr_svc,r0是保持栈8字节对齐的填充物 */

    ldr         r2, =__Int_EngineAll         /* 取异步信号地址 */

    mov     lr,pc               /* 这执行4G空间内的函数调用 */
    mov     pc,r2             /* 参数intn已经在r0中 */

    pop     {r0,lr}     /* 恢复lr_svc, */
    msr     cpsr_c,#SF_IRQMODE|SF_IRQ_BIT|SF_FIQ_BIT   /* 更新cpsr,进入IRQ模式并禁止中断,由于lr寄存
                                       器的问题,进入irq或者fiq模式，均要禁止中断以防嵌套，
                                        只有在进入svc状态才允许嵌套 */
    mrc p15, 4, r1, c15, c0, 0 /* Get base address of private perpherial space */
    str r0, [r1, #0x0110] /* Write ID to the End of Interrupt register (ICCEOIR) */

    ldmfd   sp, {r0,sp}^        //r0是填充物
    add     sp,#8
    pop     {r2}
    msr     spsr_cxsf,r2        /* 恢复spsr */

    pop     {r0-r12}
    ldmfd   sp, {lr}^
    add     sp, #4
    pop     {lr}
    movs    pc,lr           /* 此后，中断被重新打开 */

djy_irq_handler1:
    sub lr, lr, #4
    push    {lr}        /* pc */
    stmfd   sp, {lr}^   /* lr */
    sub sp, #4
    push    {r0-r12}    /* r0-r12 */
    mrs r0, spsr
    push    {r0}        /* cpsr */

    mov r0, sp
    stmfd   r0, {sp}^   /* sp_sys */
    sub sp, r0, #4

    readIntAck r0
    push {r0}


    cmp r0, #29
//    ldreq lr, =Exp_SystickTickHandler
//    ldrne lr, =__Int_EngineReal /* 这里应该调用实时/异步信号总入口。 */
//    blx lr
/* Private Timer */
    mrc p15, 4, r0, c15, c0, 0
/* Clear the interrupt by writing 0x1 to the Timer's Interrupt Status register */
    mov r1, #1
    str r1, [r0, #0x60C]
    dsb

    pop {r0} /* restore Interrupt ID */
    mrc p15, 4, r1, c15, c0, 0 /* Get base address of private perpherial space */
    str r0, [r1, #0x0110] /* Write ID to the End of Interrupt register (ICCEOIR) */


    ldmfd   sp, {sp}^
    add sp, #4

    pop {r0}
    msr spsr, r0
    pop {r0-r12}
    ldmfd sp, {lr}^
    add sp, #4
    pop {lr}
    add lr, #4
    /* retrun from irq. */
    cpsie   i
    subs    pc, r14, #4

djy_fiq_handler:
    sub lr, lr, #4      // return address adjustment
    srsfd   sp!, #17    // save registers
    stmfd   sp!, {r3, ip}

    mrc p15, 4, r1, c15, c0, 0  /* Read periph base address */
    ldr r0, [r1, #0x010C]       /* Read the Interrupt Acknowledge Register */
    stmfd   sp!, {r0, r1}

    /* call your real fiq handler */
    ldr lr, =panic
    blx lr

    ldmfd   sp!, {r0, r1}
    str r0, [r1, #0x110]        /* Write ID to the End of Interrupt register (ICCEOIR) */

    ldmfd   sp!, {r3, ip}
    rfefd   sp!

@    bkpt
@    subs    pc, r14, #4
    .end

// Ref.
// DEN0013D_cortex_a_series_PG.pdf Page. 11-9

