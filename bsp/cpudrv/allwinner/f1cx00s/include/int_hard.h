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
//所属模块:中断模块
//作者：liuwei
//版本：V1.0.0
//文件描述: 与中断相关的代码，包含异步信号与实时中断
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2015-04-22
//   作者: liuwei
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __INT_HARD_H__
#define __INT_HARD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

typedef ucpu_t atom_high_t;
typedef u64 atom_low_t;

//以下配置中断模块的常量，不但跟硬件有关，也跟软件策略有关。本实现异步信号和
//实时中断均使用irq，irq作为总中断开关，而异步信号则没有独立的使能开关，禁止异步
//信号是通过禁止所有被配置为异步信号的中断号来间接实现的，故没有独立的异步信号
//使能开关。如果移植者决定用fiq实现实时中断，用irq实现异步信号，则可以有独立的
//异步信号使能开关。
#define CN_CFG_ASYN_TURNKEY    0   //是否支持独立的异步信号使能开关
#define CN_CFG_TRUNK_TURNKEY   1   //是否支持独立的总中断使能开关

#define CN_CFG_REG_ATOM   0     //操作异步信号开关寄存器的原子性，即读或者写
                                //异步信号开关时，是否可以一条指令操作完。
                                //举例:arm9的s3c2410,INTMSK是一个32位寄存器，
                                //      在32位cpu上，一条指令可执行读或写，
                                //      可认为具有原子性，本常量设为1
                                //     arm9的s3c2416，INTMSK有64位，需两个
                                //      32位寄存器，至少两条指令才能读或写，
                                //      故不具原子性，本常量设为0
                                //     在cortex-m3上，直接读写BASEPRI寄存器，
                                //      也是一套指令操作完，具备原子性，设为1

struct INTC_REG                 //0x01c20400
{

    vu32 VECTOR;                // 00 当前响应的中断向量，DJYOS中用来存中断号
    vu32 BASE_ADDR;             // 04 中断向量表基址寄存器
    vu32 PAD0;
    vu32 NMICTRL;               // 0c NMI 控制寄存器

    vu32 INTPND1;               // 10 interrupt pending reg
    vu32 INTPND2;               // 14 interrupt pending reg
    vu32 PAD1;
    vu32 PAD2;

    //特别注意，INTEN1.0任何时候都不能置1
    //当INTEN1.0=1，产生x号中断，将响应IRQ+x中断一次，然后持续响应FIQ+x中断，即使撤掉
    //x中断，依然会持续响应FIQ+x中断。若x中断信号不撤除，修改INTEN1.0的值为0，将用IRQ
    //响应x中断，否则在FIQ
    vu32 INTEN1;                // 20 interrupt enable reg
    vu32 INTEN2;                // 24 interrupt enable reg
    vu32 PAD3;
    vu32 PAD4;

    vu32 INTMSK1;               // 30 interrupt mask reg
    vu32 INTMSK2;               // 34 interrupt mask reg
    vu32 PAD5;
    vu32 PAD6;

    vu32 INTRESPONSE1;          // 40 interrupt response reg
    vu32 INTRESPONSE2;          // 44 interrupt response reg
    vu32 PAD7;
    vu32 PAD8;

    vu32 INTFORCE1;             // 50 interrupt trig reg
    vu32 INTFORCE2;             // 54 interrupt trig reg
    vu32 PAD9;
    vu32 PAD10;

    vu32 PRIORITY1;             // 60 priority reg
    vu32 PRIORITY2;             // 64 priority reg
    vu32 PRIORITY3;             // 68 priority reg
    vu32 PRIORITY4;             // 6c priority reg

};

#define cn_noirq    0x80
#define cn_nofiq    0x40
#define cn_noint    0xc0

void __Int_ClearAllLine(void);
void __Int_InitHard(void);
void __Int_EngineReal(ufast_t ufl_line);
void __Int_EngineAsynSignal(ufast_t ufl_line);
void __Int_EngineAll(ufast_t ufl_line);

void int_enable_arm_int(void);
void int_disable_arm_int(void);
void __int_enable_irq(void);
void __int_disable_irq(void);
void __int_enable_fiq(void);
void __int_disable_fiq(void);

#ifdef __cplusplus
}
#endif

#endif //__INT_HARD_H__
