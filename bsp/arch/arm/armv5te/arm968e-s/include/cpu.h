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
//所属模块:调度器
//作者：lst
//版本：V1.0.0
//文件描述:内核中与CPU直接相关的代码的函数声明,OS内部使用
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-03-10
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------


#ifndef __CPU_H__
#define __CPU_H__
#include "stdint.h"
#include "djyos.h"
#ifdef __cplusplus
extern "C" {
#endif

#define ADD_TO_TEST_DEBUG                __attribute__ ((section(".test_debug")))

#define _FIQ_STACK_SIZE_                  0xFF0
#define _IRQ_STACK_SIZE_                  0xFF0
#define _SVC_STACK_SIZE_                  0x3F0
#define _SYS_STACK_SIZE_                  0x3F0
#define _UND_STACK_SIZE_                  0x010
#define _ABT_STACK_SIZE_                  0x010

#define NOINT                             0xc0

#define BOOT_MODE_MASK                    0x1F
#define BOOT_MODE_USR                     0x10
#define BOOT_MODE_FIQ                     0x11
#define BOOT_MODE_IRQ                     0x12
#define BOOT_MODE_SVC                     0x13
#define BOOT_MODE_ABT                     0x17
#define BOOT_MODE_UND                     0x1B
#define BOOT_MODE_SYS                     0x1F
#define BOOT_FIQ_IRQ_MASK                 0xC0
#define BOOT_IRQ_MASK                     0x80

#define BOOT_COLOR_UNUSED                 0xAAAAAAAA      //Pattern to fill UNUSED stack
#define BOOT_COLOR_SVC                    0xBBBBBBBB      //Pattern to fill SVC stack
#define BOOT_COLOR_IRQ                    0xCCCCCCCC      //Pattern to fill IRQ stack
#define BOOT_COLOR_FIQ                    0xDDDDDDDD      //Pattern to fill FIQ stack
#define BOOT_COLOR_SYS                    0xEEEEEEEE      //Pattern to fill SYS stack

/*****************************/
/* CPU Mode                  */
/*****************************/
#define USERMODE        0x10
#define FIQMODE         0x11
#define IRQMODE         0x12
#define SVCMODE         0x13
#define ABORTMODE       0x17
#define UNDEFMODE       0x1b
#define MODEMASK        0x1f
#define NOINT           0xc0

void *__asm_reset_thread(ptu32_t (*thread_routine)(void),
                           struct ThreadVm  *vm);
void __asm_reset_switch(ptu32_t (*thread_routine)(void),
                           struct ThreadVm *new_vm,struct ThreadVm *old_vm);
void __asm_start_thread(struct ThreadVm  *new_vm);
void __asm_turnto_context(struct ThreadVm  *new_vm);
void __asm_switch_context(struct ThreadVm *new_vm,struct ThreadVm *old_vm);
void __asm_switch_context_int(struct ThreadVm *new_vm,struct ThreadVm *old_vm);

uint64_t __DjyGetTicks(void);
void DjyUpdateTicks(uint32_t ticks);
void DjySetUpdateTickFlag(bool_t flag);
bool_t DjyGetUpdateTickFlag(void);

#ifdef __cplusplus
}
#endif

#endif /*__CPU_H__*/

