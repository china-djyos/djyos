/*#include "cpu.h"*/

.text

.global __asm_reset_thread
.globl djy_switch_interrupt_flag
.globl djy_interrupt_from_thread
.globl djy_interrupt_to_thread

/*
@----复位老线程，切换到新线程-------------------------------------------------
@功能:  把old_vm的上下文重新初始化到新创建的状态，然后切换到新线程的
@       上下文中继续执行。
@       当一个在常驻内存的线程中处理的事件完成后，不能删除该线程，必须复位
@       该线程，然后切换到其他就绪线程，这时候就需要调用本函数。因此本函数在
@       被复位的线程上下文里面执行。
@参数:  thread_routine，被重置的线程函数
@       new_vm，目标线程的控制块指针
@       old_vm，被复位线程的控制块指针
@返回:  无
@函数原型:void __asm_reset_switch(void (*thread_routine)(void),
@                           struct  tagThreadVm *new_vm,struct  tagThreadVm *old_vm)@
@-----------------------------------------------------------------------------
*/
.align 2
.global __asm_reset_switch
.type   __asm_reset_switch, %function
__asm_reset_switch:
    MOV     R5,  R1
    MOV     R1,  R2
    BL      __asm_reset_thread
    LDR     SP, [R5]                @; get new task stack pointer
    LDMFD   SP!, {R4}               @; pop new task cpsr
    AND     R4,  R4, #0Xffffff3f
    MSR     SPSR_cxsf, R4
    LDMFD   SP!, {R0-R12, LR, PC}^  @; pop new task r0-r12, lr & pc

/*
@----启动多事件调度-----------------------------------------------------------
@功能: 系统初始化完成后，启动多事件调度并切入第一个事件.
@参数: new_vm，首次启动的线程控制块指针
@返回: 无
@函数原型: void __asm_start_thread(struct  tagThreadVm  *new_vm)@
@-----------------------------------------------------------------------------
*/
.align 2
.global __asm_start_thread
.type   __asm_start_thread, %function
__asm_start_thread:
    LDR     SP, [R0]                @; get new task stack pointer
    LDMFD   SP!, {R4}               @; pop new task cpsr
    AND     R4,  R4, #0Xffffff3f
    MSR     SPSR_cxsf, R4
    LDMFD   SP!, {R0-R12, LR, PC}^  @; pop new task r0-r12, lr & pc
    
/*
@----切入上下文---------------------------------------------------------------
@功能:  不保存原上下文，直接切入新的上下文执行
@参数:  new_vm，待切入的线程控制块指针
@返回:  无
@函数原型: void __asm_turnto_context(struct  tagThreadVm  *new_vm)@
@说明:  当事件完成,就没有必要保存旧事件的上下文,直接切换到新事件即可.
@-----------------------------------------------------------------------------
*/
.align 2
.global __asm_turnto_context
.type   __asm_turnto_context, %function
__asm_turnto_context:
    LDR     SP, [R0]                @; get new task stack pointer
    LDMFD   SP!, {R4}               @; pop new task cpsr
    AND     R4,  R4, #0Xffffff3f
    MSR     SPSR_cxsf, R4
    LDMFD   SP!, {R0-R12, LR, PC}^  @; pop new task r0-r12, lr & pc

/*
@----上下文切换---------------------------------------------------------------
@功能:  保存当前线程的上下文，切换到新线程的上下文中继续执行。
@参数:  new_vm，切入线程的控制块指针
@参数:  old_vm，切离线程的控制块指针，即&vm->stack。无需提供旧上下文栈指针，sp寄存器的当前值就是
@返回:  无
@函数原型: void __asm_switch_context(struct  tagThreadVm *new_vm,struct  tagThreadVm *old_vm)@
@-----------------------------------------------------------------------------
*/
.align 2
.global __asm_switch_context
.type   __asm_switch_context, %function
__asm_switch_context:
 /*保存旧线程，切出新线程*/
    STMFD   SP!, {LR}               @; push pc (lr should be pushed in place of pc)
    STMFD   SP!, {R0-R12, LR}       @; push lr & register file
    MRS     R4, CPSR
    tst     lr, #0x01
    beq     _ARM_MODE
    orr     r4, r4, #0x20           @; it's thumb code set T=1

_ARM_MODE:
    STMFD   SP!, {R4}               @; push cpsr
    STR     SP, [R1]                @; store sp in preempted tasks tcb
    LDR     SP, [R0]                @; get new task stack pointer
    LDMFD   SP!, {R4}               @; pop new task spsr
    AND     R4,  R4, #0Xffffff3f
    MSR     SPSR_cxsf, R4
    LDMFD   SP!, {R0-R12, LR, PC}^  @; pop new task r0-r12, lr & pc
    
/*
@----中断中的上下文切换-------------------------------------------------------
@功能:  保存被中断线程的上下文，切换到新线程的上下文中继续执行。本函数虽然在中
@       断服务函数(非用户用int_isr_connect函数连接的中断服务函数)中，但在ARM
@       中，却运行在svc态
@参数:  new_vm，切入线程的控制块指针
@参数:  old_vm，被中断线程的控制块指针
@返回:  无
@函数原型: void __asm_switch_context_int(struct tagThreadVm *new_vm,struct tagThreadVm *old_vm)@
@-----------------------------------------------------------------------------
*/    
.align 2
.global __asm_switch_context_int
.type   __asm_switch_context_int, %function
__asm_switch_context_int:
    LDR     R2, =djy_switch_interrupt_flag
    MOV     R3, #1                          @; set flag to 1
    STR     R3, [R2]
    LDR     R2, =djy_interrupt_from_thread   @; set rt_interrupt_from_thread
    /*LDR     R1, [R1]*/
    STR     R1, [R2]
    LDR     R2, =djy_interrupt_to_thread     @; set rt_interrupt_to_thread
    /*LDR     R0, [R0]*/
    STR     R0, [R2]
    BX      LR
    
.align 2
.global __asm_delay_cycle
.type   __asm_delay_cycle, %function
__asm_delay_cycle:
    BX      LR

.global __asm_bl_fun
.type   __asm_bl_fun, %function
__asm_bl_fun:
    BX   R0
.end
