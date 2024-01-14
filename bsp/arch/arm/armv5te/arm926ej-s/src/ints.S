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
//所属模块:  中断模块
//作者：     lst
 //版本：      V1.0.1
//文件描述:  中断模块中的汇编部分
 //其他说明:  无
 //修订历史:
 //3. 日期:2010-3-06
 //   作者:lst
 //  新版本号：1.0.2
 //   修改说明:     stmfd   sp!,{r0-r3,lr}         保存lr_svc,
 //           改为  stmfd   sp!,{r0,lr}             保存lr_svc,
 //2. 日期:2010-01-04
 //   作者:lst
 //  新版本号：1.0.1
 //  修改说明: 从gcc移植到keil，并无修改代码
 //1. 日期:2009-03-10
 //   作者:lst
 //  新版本号：1.0.0
 //   修改说明: 原始版本

.cpu    arm926ej-s
.fpu    softvfp
.file   "ints.S"

/*#include "cpu.h"*/

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

.equ    INTSTATUS , (0x00802000 + 19 * 4)


/*.global rt_interrupt_enter*/
/*.global rt_interrupt_leave*/
.global djy_switch_interrupt_flag
.global djy_interrupt_from_thread
.global djy_interrupt_to_thread

.macro PUSH_SVC_REG
    SUB     SP, SP, #17 * 4
    STMIA   SP, {R0 - R12}
    MOV     R0, SP
    MRS     R6, SPSR
    STR     LR, [R0, #15*4]
    STR     R6, [R0, #16*4]
    STR     SP, [R0, #13*4]
    STR     LR, [R0, #14*4]
.endm

/* Interrupt */
.align  5
.globl boot_undefined
.type boot_undefined, %function
boot_undefined:
    PUSH_SVC_REG
    BL boot_undefined
    B       .

        .align  5
.globl boot_swi
.type boot_swi, %function
boot_swi:
    PUSH_SVC_REG
    BL boot_swi
    B       .

        .align  5
.globl boot_pabort
.type boot_pabort, %function
boot_pabort:
    PUSH_SVC_REG
    BL boot_pabort
    B       .

        .align  5
.globl boot_dabort
.type boot_dabort, %function
boot_dabort:
    PUSH_SVC_REG
    BL boot_dabort
    B       .

        .align  5
.globl boot_reserved
.type boot_reserved, %function
boot_reserved:
    PUSH_SVC_REG
    BL boot_reserved
    B       .


 .align  5
.globl HardExp_IrqHandler
.globl tg_int_global
.globl SaveExp
.type HardExp_IrqHandler, %function
HardExp_IrqHandler:
    /*把R0-R12,LR存到SP栈里面*/
    STMFD   SP!, {R0-R12,LR}

//本条件编译专门为博通的bk725x系列CPU设置，该CPU部分中断被硬连接到fiq，部分被硬连接到irq，
//软件不能修改。这样，就必然部分实时中断在irq，部分在fiq，异步信号依然，故称为hybrid。

#if (FIQ_IRQ_HYBRID == 1)
    mrs r2, cpsr
    orr r1, r2, #NOINT          @; disable interrupt
    msr cpsr_c, r1
#endif

    BL      __irq_Int_Engine

    LDR     R0, =djy_switch_interrupt_flag
    LDR     R1, [R0]
    CMP     R1, #1
    /*BEQ是个IF语句，和下面是互斥的，也就是说，等于1就跳到 djy_context_switch_interrupt_do，否则就执行下面语句*/
    BEQ     djy_context_switch_interrupt_do

    /*从栈里面弹出R0-R12,LR*/
    /*如果跳到 djy_context_switch_interrupt_do函数后就回不来了*/
    LDMFD   SP!, {R0-R12,LR}
    SUBS    PC,  LR, #4

djy_context_switch_interrupt_do:
    /*整个栈帧按地址从大到小排列是：pc,lr,r12-r4,r3-r0,spsr*/
    /*SP向下生长*/
    /*ARM9里面没有两个栈指针，这里的SP实际上是在用的是被中断打断的线程的栈*/

    MOV     R1,  #0
    /*这里是把djy_switch_interrupt_flag清0*/
    STR     R1,  [R0]

    MOV     R1, SP
    /*R0-R3共四个寄存器*/
    ADD     SP, SP, #4*4
    /*弹出R4-R12,LR寄存器*/
    LDMFD   SP!, {R4-R12,LR}
    /*把SPSR的值传送到R0*/
    MRS     R0,  SPSR
    SUB     R2,  LR, #4

    MSR     CPSR_c, #BOOT_FIQ_IRQ_MASK|BOOT_MODE_SVC
    /*压R2,实际上R2现在等于PC的值*/
    STMFD   SP!, {R2}

    /*压R4-R12,LR*/
    STMFD   SP!, {R4-R12,LR}
    /*R1是原来SP的值，弹出R1-R4*/
    /*这里是个坑，我还以为弹出的是R1到R4的值，实际上是R0-R3的值，只不过是保存在R1-R4里面而已*/
    /*这里SP指针已经切换到线程的栈指针了，之前还是IRQ自己的*/
    LDMFD   R1,  {R1-R4}
    /*实际压入的是被切出事件的R0-R3*/
    STMFD   SP!, {R1-R4}
    /*把SPSR的值压入新的栈里*/
    STMFD   SP!, {R0}

    LDR     R4,  =djy_interrupt_from_thread
    LDR     R5,  [R4]


    /*把栈顶数值存到rt_interrupt_from_thread里*/
    STR     SP,  [R5]

    LDR     R6,  =djy_interrupt_to_thread
    LDR     R6,  [R6]
    /*把新的栈顶的值存到SP上面*/
    LDR     SP,  [R6]

    LDMFD   SP!, {R4}
    AND     R4,  R4, #0Xffffff3f
    MSR     SPSR_cxsf, R4
    /*从栈里面弹出r0-r12,lr,pc，弹出pc后是立即跳转到线程里面执行了*/
    LDMFD   SP!, {R0-R12,LR,PC}^


.align  5
.globl HardExp_FiqHandler
.type HardExp_FiqHandler, %function
HardExp_FiqHandler:
    STMFD   SP!,{R0-R7,LR}

    BL     __fiq_Int_Engine

    LDR     R0, =djy_switch_interrupt_flag
    LDR     R1, [R0]
    CMP     R1, #1
    BEQ     djy_context_switch_interrupt_fiq_do

    LDMFD   SP!,{R0-R7,LR}
    SUBS    PC, LR, #4

djy_context_switch_interrupt_fiq_do:

    MOV     R1,  #0
    /*这里是把djy_switch_interrupt_flag置1*/
    STR     R1,  [R0]

    MOV     R1, SP             @; pop {R0-R7,LR} but skip R0-R3
    ADD     SP, SP, #4*4
    /*这里只弹R4-R7,LR，不弹R0-R3*/
    LDMFD   SP!, {R4-R7,LR}

    /*取得SPSR的值*/
    MRS     R0,  SPSR
    /*取得PC的值*/
    SUB     R2,  LR, #4        @; Save old task's PC to R2

    MSR     CPSR_c, #BOOT_FIQ_IRQ_MASK|BOOT_MODE_SVC
    /*上面一句切换了SP的值，从FIQ的栈切换到线程的栈*/
    STMFD   SP!, {R2}          @; Push old task's PC
    STMFD   SP!, {R4-R12,LR}   @; Push old task's LR,R12-R4
    LDMFD   R1,  {R1-R4}       @; pop old thread R0-R3 to R1-R4
    STMFD   SP!, {R1-R4}       @; Push old thread R0-R3
    STMFD   SP!, {R0}          @; Push old task's CPSR

    LDR     R4,  =djy_interrupt_from_thread
    LDR     R5,  [R4]

    ldr r6,[r5,#4]
    cmp sp,r5
    ble  .
    cmp sp,r6
    bge  .

    STR     SP,  [R5]

    LDR     R6,  =djy_interrupt_to_thread
    LDR     R6,  [R6]
    LDR     SP,  [R6]

    ldr r7,[r6,#4]
    cmp sp,r6
    ble  .
    cmp sp,r7
    bge  .


    LDMFD   SP!, {R4}
    AND     R4,  R4, #0Xffffff3f
    MSR     SPSR_cxsf, R4

    LDMFD   SP!, {R0-R12,LR,PC}^


.end

