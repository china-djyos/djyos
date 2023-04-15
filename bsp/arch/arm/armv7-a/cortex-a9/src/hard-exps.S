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
    cpsid   i
@   +----------+
@   | pc       | <-- __irq_stack_base
@   | lr       |
@   | r0 - r12 |
@   | spsr     |
@   +----------+
@   | sp_sys   |
@   +----------+
@   | sp_irq   | (not used)
@       +----------+
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

    mov r0, sp
    push    {r0}

    readIntAck r0

    cmp r0, #29
    bne 1f

/* do djyos tick schedule */
    bl  Exp_SystickTickHandler

/* Private Timer */
    mrc p15, 4, r0, c15, c0, 0
/* Clear the interrupt by writing 0x1 to the Timer's Interrupt Status register */
    mov r1, #1
    str r1, [r0, #0x60C]
    dsb

1:
    mov r0, #29
    mrc p15, 4, r1, c15, c0, 0  // Read periph base address
    str r0, [r1, #0x0110]       // Write ID to the End of Interrupt register

    add sp, #4      /* skip sp_irq */

    ldmfd   sp, {sp}^
    add sp, #4

    pop {r0}
    msr spsr, r0
    pop {r0-r12}
    ldmfd   sp, {lr}^
    add sp, #4
    pop {lr}

    add lr, #4
    /* retrun from irq. */
    cpsie   i
    subs    pc, r14, #4

djy_fiq_handler:
    bkpt
    subs    pc, r14, #4
    .end

// Ref.
// DEN0013D_cortex_a_series_PG.pdf Page. 11-9

