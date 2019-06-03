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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
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
// ���ܣ���ϵͳ�����Ժ���Ѹ��ֱ�־��λ
// ��������
// ���أ���
// =============================================================================
__attribute__((weak)) void __InitTimeBase(void)
{}

// =============================================================================
// ���ܣ���ʼ��systick
// ��������
// ���أ���
// =============================================================================
__attribute__((weak)) void __DjyInitTick(void)
{
    HardExp_ConnectSystick(Djy_ScheduleIsr);
}

// =============================================================================
// ���ܣ���ȡϵͳʱ��
// ��������
// ���أ���ǰus��
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

