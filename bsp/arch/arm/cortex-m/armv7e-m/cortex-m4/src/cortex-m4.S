/*
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
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
@所属模块:  中断模块
@作者：     lst
@版本：      V1.00
@初始版本完成日期：2009-08-10
@文件描述:  中断模块中的汇编部分
@其他说明:  无
@修订历史:
@2. ...
@1. 日期:
@   作者:
@   新版本号：
@   修改说明:
@------------------------------------------------------
*/
.syntax         unified
.thumb
.file   "cortex-m4.S"


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
.text
.align 2
.global __LP_BSP_AsmSaveReg
.thumb
.thumb_func
__LP_BSP_AsmSaveReg:
    mrs     r3,xpsr
    orr     r3,#0x01000000      /* xpsr的T标志读不出来，得手工置位。 */
    push    {r3}                /* 保存xpsr */
    push    {lr}                /* 保存PC,退出L3低功耗后,将从本函数返回地址处继续运行*/
    push    {r0-r3,r12,lr}      /* 保存r0-r3,r12,lr */
    push    {r4-r11}
    str     sp,[r0]             /* 保存旧上下文栈指针到vm->stack */
    push    {r2}
    blx      r1                  /* 调用保存RAM到非易失存储器的函数*/
    pop     {r2}
    mov 	r0, #0x4			/* 选择休眠方式:L3 */
    blx      r2                  /* 调用进入SLEEP3的函数 */
    bx      lr

/*
@----指令操作实现延时函数-------------------------------------------------------
@功能:  指令延时
@参数:  timeCoreClock。时间所对应的内核时钟数
@返回:  无
@函数原型:  void __asm_delay_cycle(u64 timeCoreClock)
@-----------------------------------------------------------------------------
*/
.text
.align 2
.global __asm_delay_cycle
.thumb
.thumb_func
__asm_delay_cycle:
    subs    r0 ,#6      /*减去运算的周期数*/
    lsr     r0 ,r0, #2
 loop:
    subs    r0 ,#1
    nop
    bne     loop
    cmp    r1 ,#0
    beq    end
    subs    r1 ,#1
    mov     r0 ,#0xffffffff
    b       __asm_delay_cycle
end:
    bx      lr

.end
