//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
//所属模块:调度器
//作者:  罗侍田.
//版本：V1.0.1
//文件描述:调度器中与CPU直接相关的汇编代码，主要是线程切换方面的代码。
//其他说明:
//修订历史:
// 日期: 2010-01-04
//  作者:  罗侍田.
//  新版本号: V1.0.1
//  修改说明: 从gcc移植到keil
// 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//-----------------------------------------------------------------------------
.cpu    arm926ej-s
//.fpu    softvfp
//.file   "cpus.S"

.global __asm_reset_thread
.globl djy_switch_interrupt_flag
.globl djy_interrupt_from_thread
.globl djy_interrupt_to_thread


.equ NOINT,                             0xc0
.equ BOOT_MODE_MASK,                    0x1F
.equ BOOT_MODE_USR,                     0x10
.equ BOOT_MODE_FIQ,                     0x11
.equ BOOT_MODE_IRQ,                     0x12
.equ BOOT_MODE_SVC,                     0x13
.equ BOOT_MODE_ABT,                     0x17
.equ BOOT_MODE_UND,                     0x1B
.equ BOOT_MODE_SYS,                     0x1F
.equ BOOT_FIQ_IRQ_MASK,                 0xC0
.equ BOOT_IRQ_MASK,                     0x80

//因异步信号使能才允许调度，故上下文切换中恢复异步信号寄存器等同于使能异步信号
.macro restore_asyn_signal_reg
    bl Int_ContactAsynSignal
.endm

@----重置线程-----------------------------------------------------------------
@功能:  初始化线程的栈，并挂上线程执行函数，新创建线程时将会调用本函数
@参数:  thread_routine，被重置的线程函数
@               vm，线程虚拟机指针
@返回:  初始化结束后的当前栈指针
@函数原型:void __asm_reset_thread(void (*thread_routine)(void),
@                                   struct  tagThreadVm  *vm);
@-----------------------------------------------------------------------------
.section .text.__asm_reset_thread, "ax", %progbits
.global __asm_reset_thread
__asm_reset_thread:
    LDR     R2,[R1,#4]          @取虚拟机栈顶指针
    LDR     R3,=__DJY_VmEngine  @取虚拟机引擎指针
    STMFD   R2!,{R3}            @存pc，
    stmfd   r2!,{r3}            @给LR一个可访问的地址，复位状态下，LR值并无意义
    mov     r3,#0
    mov     r4,#0
    mov     r5,#0
    mov     r6,#0
    stmfd   r2!,{r3,r4,r5,r6}    @清零 r9~r12
    stmfd   r2!,{r3,r4,r5,r6}    @清零 r5~r8
    stmfd   r2!,{r3,r4,r5,r6}    @清零 r1~r4
@    SUB     R2,R2,#13*4         @后退13个寄存器,初始状态r1-r12随意，__vm_engine
                                @函数不返回，lr也无意义
                                @__vm_engine的参数，切换上下文时，service_routine将恢复到r0中，根据调用约
                                @定，r0的值就是__vm_engine函数的参数。这只在创建并初始化线程上下文时需要这
                                @样做，线程启动以后线程切换要保存上下文时，保存r0的实际值即可
    STMFD   R2!,{R0}            @保存 thread_routine指针到r0的位置.
    LDR     R3,=BOOT_MODE_SVC   @取cpsr
    TST     R0, #0X01
    BEQ     __ASM_ARM
    ORR     R3, R3, #0X20           @; it's thumb code set T=1
__ASM_ARM:
    STMFD   R2!,{R3}                    @保存cpsr
    STR     R2,[R1]             @保存vm的当前栈指针到vm->stack中
    BX      LR
@ end of __asm_reset_thread
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
    MRS     R4,CPSR
    ORR     R4,R4,#0xc0
    MSR     CPSR_cxsf,R4
    LDMFD   SP!, {R4}               @; pop new task cpsr
    AND     R4,  R4, #0Xffffff3f
    MSR     SPSR_cxsf, R4
    restore_asyn_signal_reg
    LDMFD   SP!, {R0-R12, LR, PC}^  @; pop new task r0-r12, lr & pc

/*
@----启动多事件调度-----------------------------------------------------------
@功能: 系统初始化完成后，启动多事件调度并切入第一个事件.
@参数: new_vm，首次启动的线程控制块指针
@返回: 无
@函数原型: void __asm_start_thread(struct  tagThreadVm  *new_vm)@
@-----------------------------------------------------------------------------
*/
//cpu.h中有#define __asm_start_thread __asm_turnto_context
//.align 2
//.global __asm_start_thread
//.type   __asm_start_thread, %function
//__asm_start_thread:
//    LDR     SP, [R0]                @; get new task stack pointer
//    LDMFD   SP!, {R4}               @; pop new task cpsr
//    AND     R4,  R4, #0Xffffff3f
//    MSR     SPSR_cxsf, R4
//    LDMFD   SP!, {R0-R12, LR, PC}^  @; pop new task r0-r12, lr & pc

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
    MRS     R4,CPSR
    ORR     R4,R4,#0xc0
    MSR     CPSR_cxsf,R4
    LDMFD   SP!, {R4}               @; pop new task cpsr
    AND     R4,  R4, #0Xffffff3f
    MSR     SPSR_cxsf, R4
    restore_asyn_signal_reg
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
    MRS     R4,CPSR
    ORR     R4,R4,#0xc0
    MSR     CPSR_cxsf,R4
    LDMFD   SP!, {R4}               @; pop new task spsr
    AND     R4,  R4, #0Xffffff3f
    MSR     SPSR_cxsf, R4
    restore_asyn_signal_reg

    LDMFD   SP!, {R0-R12, LR, PC}^  @; pop new task r0-r12, lr & pc

/*
@----中断中的上下文切换-------------------------------------------------------
@功能:  保存被中断线程的上下文，切换到新线程的上下文中继续执行。本函数虽然在中
@       断服务函数(非用户用int_isr_connect函数连接的中断服务函数)中，但在ARM
@       中，却运行在svc态
@参数:  new_vm，切入线程的控制块指针
@参数:  old_vm，被中断线程的控制块指针
@返回:  无
@函数原型: void __asm_switch_context_int(struct tagThreadVm *new_vm,struct tagThreadVm *old_vm);
@-----------------------------------------------------------------------------
*/
.align 2
.global __asm_switch_context_int
.type   __asm_switch_context_int, %function
__asm_switch_context_int:
    LDR     R2, =djy_switch_interrupt_flag
    MOV     R3, #1                          @; set flag to 1
    STR     R3, [R2]
    LDR     R2, =djy_interrupt_from_thread   @; set djy_interrupt_from_thread
    /*LDR     R1, [R1]*/
    STR     R1, [R2]
    LDR     R2, =djy_interrupt_to_thread     @; set djy_interrupt_to_thread
    /*LDR     R0, [R0]*/
    STR     R0, [R2]

    BX      LR

.section .text.__asm_reset_cpu, "ax", %progbits
.global __asm_reset_cpu
.global __asm_reset_cpu_hot
__asm_reset_cpu:
__asm_reset_cpu_hot:
    mov     r0,#0
    mov     pc,r0


.equ    SF_MODEMASK,    0x1f
.equ    SF_IRQ_BIT,     0x80
.equ    SF_FIQ_BIT,     0x40
.equ    SF_USERMODE,    0x10
.equ    SF_SYSMODE,     0x1f
.equ    SF_FIQMODE,     0x11
.equ    SF_IRQMODE,     0x12
.equ    SF_SVCMODE,     0x13
.equ    SF_ABORTMODE,   0x17
.equ    SF_UNDEFMODE,   0x1b


/* cpsr的IF位清0，允许全部中断 */
.section .text.int_enable_arm_int, "ax", %progbits
.align 2
.global int_enable_arm_int
int_enable_arm_int:
        mrs r0, cpsr
        bic r0, r0, #SF_IRQ_BIT|SF_FIQ_BIT
        msr cpsr_cxsf, r0
        bx      lr

/* cpsr的IF位置1，禁止全部中断 */
.section .text.int_disable_arm_int, "ax", %progbits
.align 2
.global int_disable_arm_int
int_disable_arm_int:
        mrs  r0, cpsr
        orr  r0, r0, #SF_IRQ_BIT|SF_FIQ_BIT
        msr  cpsr_cxsf, r0
        bx      lr

/* cpsr的I位置1，禁止irq中断 */
.section .text.__int_disable_irq, "ax", %progbits
.align 2
.global __int_disable_irq
__int_disable_irq:
        mrs  r0, cpsr
        orr  r0, r0, #SF_IRQ_BIT
        msr  cpsr_cxsf, r0
        bx      lr

/* cpsr的I位清0，允许irq中断 */
.section .text.__int_enable_irq, "ax", %progbits
.align 2
.global __int_enable_irq
__int_enable_irq:
        mrs  r0, cpsr
        bic  r0, r0, #SF_IRQ_BIT
        msr  cpsr_cxsf, r0
        bx      lr

/* cpsr的F位置1，禁止fiq中断 */
.section .text.__int_disable_fiq, "ax", %progbits
.align 2
.global __int_disable_fiq
__int_disable_fiq:
        mrs r0,cpsr
        orr r0,r0,#SF_FIQ_BIT
        msr cpsr_cxsf,r0
        bx      lr

/* cpsr的F位清0，允许fiq中断 */
.section .text.__int_enable_fiq, "ax", %progbits
.align 2
.global __int_enable_fiq
__int_enable_fiq:
        mrs r0, cpsr
        bic r0, r0, #SF_FIQ_BIT
        msr cpsr_cxsf, r0
        bx      lr


.text
.align 2
.global __asm_delay_cycle
.type   __asm_delay_cycle, %function
__asm_delay_cycle:
    subs    r0 ,#6      /*减去运算的周期数*/
    lsr     r0 ,r0, #2  /*每次循环4个机械周期 每个机械周期 1个内核时钟*/
 loop:
    subs    r0 ,#1
    bne     loop
    subs    r1 ,#1
    bne     end
    mov     r0 ,#0xffffffff
    b       __asm_delay_cycle
end:
    bx      lr

.global __asm_bl_fun
.type   __asm_bl_fun, %function
__asm_bl_fun:
    BX   R0


/*
@----保存上下文---------------------------------------------------------------
@功能: 保存当前线程的上下文，但不切换,专用于进入CN_SLEEP_L3之前调用.
@参数: running_vm，当前线程的控制块指针，即&vm->stack。
@参数: SaveRamL3, 把执行过step1的内存保存到flash中.
@参数: EntrySleep, 进入低功耗.
@返回:  无
@函数原型: void __LP_BSP_AsmSaveReg(struct tagThreadVm *running_vm，
                    bool_t (*SaveRamL3)(void),
                    void (*EntrySleep)(void))
@-----------------------------------------------------------------------------
*/
.global __LP_BSP_AsmSaveReg
.type   __LP_BSP_AsmSaveReg, %function
__LP_BSP_AsmSaveReg:
    push    {lr}                /* 保存PC,退出L3低功耗后,将从本函数返回地址处继续运行*/
    push    {r0-r3,r12,lr}      /* 保存r0-r3,r12,lr */
    push    {r4-r11}
    str     sp,[r0]             /* 保存旧上下文栈指针到vm->stack */
    push    {r2}
    blx      r1                  /* 调用保存RAM到非易失存储器的函数*/
    pop     {r2}
    mov     r0, #0x4            /* 选择休眠方式:L3 */
    blx      r2                  /* 调用进入SLEEP3的函数 */
    bx      lr

    .global __asm_ExecuteCmd
__asm_ExecuteCmd:
    bx  lr


/*
; * void WFI(void);
; */
    .text
    .code 32
    .globl WFI
    .type WFI, %function
WFI:
    MOV R0, #0
    MCR p15, 0, R0, c7, c0, 4
    BX LR

;/*
; * void cp15_enable_alignfault(void)
; */
    .text
    .globl cp15_enable_alignfault
    .type cp15_enable_alignfault, %function
cp15_enable_alignfault:
    MRC p15, 0, R0, c1, c0, 0
    ORR R0, R0, #0x02
    MCR p15, 0, R0, c1, c0, 0
    BX LR

;/*
; * void cp15_disable_alignfault(void)
; */
    .text
    .globl cp15_disable_alignfault
    .type cp15_disable_alignfault, %function
cp15_disable_alignfault:
    MRC p15, 0, R0, c1, c0, 0
    BIC R0, R0, #0x02
    MCR p15, 0, R0, c1, c0, 0
    BX LR

.text
.globl Int_GetPreStatus
.type Int_GetPreStatus, %function
Int_GetPreStatus:
    MRS     R0, SPSR
    and     R0,R0,#0x1f
    BX      LR
.text
.globl Int_GetStatus
.type Int_GetStatus, %function
Int_GetStatus:
    MRS     R0, CPSR
    BX      LR

.text
.globl GetLR
.type GetLR, %function
GetLR:
    mov     R0, LR
    BX      LR
.end
