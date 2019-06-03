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
// =============================================================================
#include "cpu.h"
#include "arch_feature.h"
#include "align.h"
#include "int.h"
#include "hard-exp.h"
#include "board-config.h"

extern void Load_Preload(void);

extern void __Djy_VmEngine(ptu32_t (*thread_routine)(void));

uint32_t djy_switch_interrupt_flag = 0;
uint32_t *djy_interrupt_from_thread = NULL;
uint32_t *djy_interrupt_to_thread = NULL;
static uint64_t gRunTicks = 0;
static bool_t gResumeTickFlag = false;
/*??????????????:pc,lr,r12-r4,r3-r0,spsr*/
void *__asm_reset_thread(ptu32_t (*thread_routine)(void),
                            struct ThreadVm  *vm)
{
    uint32_t *stk = vm->stack_top;
    *(--stk) = (uint32_t)__Djy_VmEngine;         /* entry point */
    *(--stk) = (uint32_t)__Djy_VmEngine;          /* lr */
    stk -=12;
    *(--stk) = (uint32_t)thread_routine;           /*R0*/
    /* cpsr */
	if ((uint32_t)thread_routine & 0x01)
		*(--stk) = SVCMODE | 0x20;			/* thumb mode */
	else
		*(--stk) = SVCMODE;					/* arm mode   */
    vm->stack = stk;
    return stk;
}

// =============================================================================
// 功能：在系统起来以后需把各种标志复位
// 参数：无
// 返回：无
// =============================================================================
__attribute__((weak)) void __InitTimeBase(void)
{}

// =============================================================================
// 功能：初始化systick
// 参数：无
// 返回：无
// =============================================================================
__attribute__((weak)) void __DjyInitTick(void)
{
    HardExp_ConnectSystick(Djy_ScheduleIsr);
}

// =============================================================================
// 功能：获取系统时间
// 参数：无
// 返回：当前us数
// =============================================================================
__attribute__((weak))   uint64_t __DjyGetSysTime(void)
{
    s64 time = 0;
    time = gRunTicks*CN_CFG_TICK_US;
    return (uint64_t)time;
}

__attribute__((weak)) uint64_t __DjyGetTicks(void)
{
    return gRunTicks;
}

//??????????
__attribute__((weak)) void DjySetUpdateTickFlag(bool_t flag)
{
    gResumeTickFlag = flag;
}

__attribute__((weak)) bool_t DjyGetUpdateTickFlag(void)
{
    return gResumeTickFlag;
}

//??????????
__attribute__((weak)) void DjyUpdateTicks(uint32_t ticks)
{
    gRunTicks += ticks;
}



//void reboot(u32 key)
//{
//
//}

void reset(u32 key)
{

}

void restart_system(u32 key)
{
    Load_Preload();
}

