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
.file   "cpus.S"

#define __stack       0
#define __stack_top   4
#define __stack_used  8
#define __next        12
#define __stack_size  16
#define __host_vm     20

.equ    MODEMASK, 0x1f
.equ    NOIRQ, 0x80
.equ    NOFIQ, 0x40
.equ    NOINT, 0xC0
//移植敏感，使用FIQ做实时中断的系统，应改为0x0000001f
.equ    CPSR_sys, 0x0000001f
.equ    CPSR_svc, 0x00000013

//因异步信号使能才允许调度，故上下文切换中恢复异步信号寄存器等同于使能异步信号
.macro restore_asyn_signal_reg
    bl Int_ContactAsynSignal
.endm

@注: sp并不保存在栈中，而是在vm->stack中!!

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
    stmfd   sp!,{lr}
    ldr     r2,[r1,#4]          @取虚拟机栈顶指针
    ldr     r3,=__DJY_VmEngine     @取虚拟机引擎指针
    stmfd   r2!,{r3}            @存pc，
    stmfd   r2!,{r3}            @给LR一个可访问的地址，复位状态下，LR值并无意义
    mov     r3,#0
    mov     r4,#0
    mov     r5,#0
    mov     r6,#0
    stmfd   r2!,{r3-r6}    @清零 r9~r12
    stmfd   r2!,{r3-r6}    @清零 r5~r8
    stmfd   r2!,{r3-r6}    @清零 r1~r4
@    sub     r2,r2,#13*4         @后退13个寄存器,初始状态r1-r12随意，__vm_engine
                                                        @函数不返回，lr也无意义
    @__vm_engine的参数，切换上下文时，service_routine将恢复到r0中，根据调用约
    @定，r0的值就是__vm_engine函数的参数。这只在创建并初始化线程上下文时需要这
    @样做，线程启动以后线程切换要保存上下文时，保存r0的实际值即可
    stmfd   r2!,{r0}            @保存 thread_routine指针到r0的位置.
    ldr     r3,=(CPSR_sys)   @取cpsr
    TST     R0, #0X01
    BEQ     __ASM_ARM
    ORR     R3, R3, #0X20           @; it's thumb code set T=1
__ASM_ARM:
    stmfd   r2!,{r3}                    @保存cpsr
    str     r2,[r1]             @保存vm的当前栈指针到vm->stack中
    ldmfd       sp!,{pc}
@ end of __asm_reset_thread

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
    MOV     R5,SP
    ADD     SP, SP, #60
    AND     R4,  R4, #0Xffffff3f
@切换到svc的目标，是为了使得恢复中断使能和恢复pc能在一条指令中执行
    mrs     r0,cpsr
    bic     r0,#MODEMASK
    orr     r0,#(CPSR_svc+NOIRQ)            @关闭中断，并切换到svc模式
    msr     cpsr_cxsf,r0
    restore_asyn_signal_reg     @此时使用的是svc模式的lr
    msr     spsr_cxsf,r4    @新cpsr_sys-->spsr_svc，弹栈时恢复到cpsr_sys(^符号)
    ldmfd   r5,{r0-r12,lr,pc}^  @恢复寄存器,lr是svc的lr，线程的lr前面已经恢复

@----复位老线程，切换到新线程-------------------------------------------------
@功能:  把old_vm的上下文重新初始化到新创建的状态，然后切换到新线程的
@               上下文中继续执行。
@               当一个在常驻内存的虚拟机中处理的事件完成后，不能删除该虚拟机，必须复位
@               该虚拟机，然后切换到其他就绪线程，这时候就需要调用本函数。因此本函数在
@               被复位的线程上下文里面执行。
@参数:  thread_routine，被重置的线程函数
@               new_vm，目标线程的虚拟机
@               old_vm，被复位的虚拟机
@返回:  无
@函数原型:void __asm_reset_switch(void (*thread_routine)(void),
@                           struct  tagThreadVm *new_vm,struct  tagThreadVm *old_vm);
@调用本函数前，必先调用int_half_enable_asyn_signal
@-----------------------------------------------------------------------------
.section .text.__asm_reset_switch, "ax", %progbits
.global __asm_reset_switch
__asm_reset_switch:
    .global __asm_reset_switch
    ldr     sp,[r2,#4]          @取老虚拟机栈顶指针
    ldr     r3,=__DJY_VmEngine    @取虚拟机引擎指针
    stmfd   sp!,{r3}           @存pc
    stmfd   sp!,{r3}            @给LR一个可访问的地址，复位状态下，LR值并无意义
    mov     r3,#0
    mov     r4,#0
    mov     r5,#0
    mov     r6,#0
    stmfd   sp!,{r3-r6}    @清零 r9~r12
    stmfd   sp!,{r3-r6}    @清零 r5~r8
    stmfd   sp!,{r3-r6}    @清零 r1~r4
//    sub     sp,sp,#13*4         @后退13个寄存器,初始状态r1-r12随意.__DJY_VmEngine
                                                        @函数不返回，lr也无意义
                                @至此，完成老虚拟机复位
    stmfd   sp!,{r0}            @保存 thread_routine指针.
    ldr     r5,=(CPSR_sys)      @取cpsr
    TST     R0, #0X01
    BEQ     __ASM_ARM1
    ORR     R5, R5, #0X20           @; it's thumb code set T=1
__ASM_ARM1:
    stmfd   sp!,{r5}            @保存cpsr,至此，完成老线程重置
    str     sp,[r2]             @保存当前栈指针到old_vm->stack中

    ldr     sp,[r1]             @取得新上下文指针
@    bl      Int_HalfEnableAsynSignal
    ldmfd   sp!,{r4}            @新线程的cpsr-->r4
    ldr     lr,[sp,#52]         @恢复lr
    mov     r5,sp
    add     sp,sp,#60           @即将切换到svc，sp不再可用

@切换到svc的目标，是为了使得恢复中断使能和恢复pc能在一条指令中执行
    mrs     r0,cpsr
    bic     r0,#MODEMASK
    orr     r0,#(CPSR_svc+NOIRQ)            @关闭中断，并切换到svc模式
    msr     cpsr_cxsf,r0
    restore_asyn_signal_reg     @此时使用的是svc模式的lr
    msr     spsr_cxsf,r4    @新cpsr_sys-->spsr_svc，弹栈时恢复到cpsr_sys(^符号)
    ldmfd   r5,{r0-r12,lr,pc}^  @恢复寄存器,lr是svc的lr，线程的lr前面已经恢复
@ end of __asm_reset_switch

@----切入上下文---------------------------------------------------------------
@功能:  不保存原上下文，直接切入新的上下文执行
@参数:  new_sp，新上下文的栈指针
@返回:  无
@函数原型: void __asm_turnto_context(struct  tagThreadVm  *new_vm);
@说明:  当事件完成,就没有必要保存旧事件的上下文,直接切换到新事件即可.
@调用本函数前，必先调用int_half_enable_asyn_signal
@-----------------------------------------------------------------------------
.section .text.__asm_turnto_context, "ax", %progbits
.global __asm_turnto_context
__asm_turnto_context:
    .global __asm_turnto_context
    ldr     sp,[r0]                     @取得新上下文指针
@    bl      Int_HalfEnableAsynSignal @对应done函数开头的 int_save_asyn_signal调用
    ldmfd   sp!,{r4}            @新线程的cpsr-->r4
    ldr     lr,[sp,#52]         @恢复lr
    mov     r5,sp
    add     sp,sp,#60           @即将切换到svc，sp不再可用

@切换到svc的目标，是为了使得恢复中断使能和恢复pc能在一条指令中执行
    mrs     r0,cpsr
    bic     r0,#MODEMASK
    orr     r0,#(CPSR_svc+NOIRQ) @关闭中断，并切换到svc模式
    msr     cpsr_cxsf,r0
    restore_asyn_signal_reg     @此时使用的是svc模式的lr
    msr     spsr_cxsf,r4    @新cpsr_sys-->spsr_svc，弹栈时恢复到cpsr_sys(^符号)
    ldmfd   r5,{r0-r12,lr,pc}^  @恢复寄存器,lr是svc的lr，线程的lr前面已经恢复
@ end of __asm_turnto_context

@----上下文切换---------------------------------------------------------------
@功能:  保存当前线程的上下文，切换到新线程的上下文中继续执行。
@参数:  new_sp，新上下文的栈指针
@参数:  old_sp，旧上下文的栈指针的指针，即&vm->stack。无需提供旧上下文栈指针，
@               sp寄存器的当前值就是
@返回:  无
@函数原型: void __asm_switch_context(struct  tagThreadVm *new_vm,struct  tagThreadVm *old_vm);
@调用本函数前，必先调用int_half_enable_asyn_signal
@-----------------------------------------------------------------------------
.section .text.__asm_switch_context, "ax", %progbits
.global __asm_switch_context
__asm_switch_context:
    .global __asm_switch_context
    stmfd   sp!,{lr}            @保存PC
    stmfd   sp!,{r0-r12,lr}     @保存寄存器和LR
    mrs     r5,CPSR
    stmfd   sp!,{r5}            @保存cpsr
    str     sp,[r1]             @保存旧上下文栈指针到old_vm->stack

    ldr     sp,[r0]             @取得新上下文指针
@    bl      Int_HalfEnableAsynSignal
    ldmfd   sp!,{r4}            @新线程的cpsr-->r4
    ldr     lr,[sp,#52]         @恢复lr
    mov     r5,sp
    add     sp,sp,#60           @即将切换到svc，sp不再可用

@切换到svc的目标，是为了使得恢复中断使能和恢复pc能在一条指令中执行
    mrs     r0,cpsr
    bic     r0,#MODEMASK
    orr     r0,#(CPSR_svc+NOIRQ)  @关闭中断，并切换到svc模式
    msr     cpsr_cxsf,r0
    restore_asyn_signal_reg     @此时使用的是svc模式的lr
    msr     spsr_cxsf,r4    @新cpsr_sys-->spsr_svc，弹栈时恢复到cpsr_sys(^符号)
    ldmfd   r5,{r0-r12,lr,pc}^  @恢复寄存器,lr是svc的lr，线程的lr前面已经恢复
@ end of __asm_switch_context

@中断上下文切换函数,
@本函数由中断函数调用,由于中断发生时间是随机的,编译器没有任何办法保护lr,若发生
@上下文切换,则必需手动把LR_sys保存到上下文中,恢复时才不会出错.
@本函数必需由C语言调用,如果汇编调用,调用前要保存LR,见switch_context函数的注释.
@只有在中断没有嵌套时才可能调用本函数,中断前的状态是sys态.

@----中断中的上下文切换-------------------------------------------------------
@功能:  保存被中断线程的上下文，切换到新线程的上下文中继续执行。本函数虽然在中
@               断服务函数(非用户用int_isr_connect函数连接的中断服务函数)中，但在ARM
@               中，却运行在svc态
@参数:  new_sp，切换目标虚拟机
@参数:  old_sp，被中断线程虚拟机
@返回:  无
@函数原型: void __asm_switch_context_int(struct tagThreadVm *new_vm,struct tagThreadVm *old_vm);
@-----------------------------------------------------------------------------
.section .text.__asm_switch_context_int, "ax", %progbits
.global __asm_switch_context_int
__asm_switch_context_int:
    .global __asm_switch_context_int
// 现在（入irq时制造的） -> old_vm
    push    {r4-r7}

    ldr r3, =__irq_stack_limit
//    sub r2, r3, #68
    ldr r2, [r1]

    ldmdb   r3!, {r4-r7}
    stmdb   r2!, {r4-r7}
    ldmdb   r3!, {r4-r7}
    stmdb   r2!, {r4-r7}
    ldmdb   r3!, {r4-r7}
    stmdb   r2!, {r4-r7}
    ldmdb   r3!, {r4-r7}
    stmdb   r2!, {r4-r7}
//    ldmdb   r3!, {r4}
    str     r2, [r1, #__stack]
@       vm                      irq_stack
@   +----------+             +----------+
@ r3 -> | pc       |       r2 -> | pc       |
@   | lr       |             | lr       |
@   | r0 - r12 |  = copy =>  | r0 - r12 |
@   | spsr     |             | spsr     |
@   +----------+             +----------+
@              vm.stack -->  | sp_sys   |
@                            +----------+
// new_vm(r0) --> irq
    ldr r2, = __irq_stack_limit
    ldr r3, [r0, #__stack]
    add r3, r3, #64

    ldmdb   r3!, {r4-r7}
    stmdb   r2!, {r4-r7}
    ldmdb   r3!, {r4-r7}
    stmdb   r2!, {r4-r7}
    ldmdb   r3!, {r4-r7}
    stmdb   r2!, {r4-r7}
    ldmdb   r3!, {r4-r7}
    stmdb   r2!, {r4-r7}
    add r3, r3, #64
    stmdb   r2!, {r3}

    pop {r4-r7}
    bx  lr

.section .text.__asm_reset_cpu, "ax", %progbits
.global __asm_reset_cpu
.global __asm_reset_cpu_hot
__asm_reset_cpu:
__asm_reset_cpu_hot:
    mov     r0,#0
    mov     pc,r0



.text
.align 2
.global __asm_delay_cycle
//.thumb
//.thumb_func
__asm_delay_cycle:
    subs    r0 ,#6      /*减去运算的周期数*/
    lsr     r0 ,r0, #0  /*每次循环2个机械周期 每个机械周期 1个内核时钟*/
 loop:
    subs    r0 ,#1
    bne     loop
    subs    r1 ,#1
    bne     end
    mov     r0 ,#0xffffffff
    b       __asm_delay_cycle
end:
    bx      lr


    .global __asm_ExecuteCmd
__asm_ExecuteCmd:
#warning "TODO: __asm_ExecuteCmd()"
    bx  lr

.global __asm_bl_fun
.type   __asm_bl_fun, %function
__asm_bl_fun:
    BX   R0


.text
.align 2
.global __LP_BSP_AsmSaveReg

.thumb
.thumb_func
__LP_BSP_AsmSaveReg:
    nop
    bx  lr

.end

