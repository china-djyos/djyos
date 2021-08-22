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
#include "core_ck803.h"
#include "arch_feature.h"
#include "align.h"
#include "int.h"
#include "stdint.h"
#include "djyos.h"
#include "hard-exp.h"
#include "board-config.h"
#include "cpu_peri.h"

extern s64  g_s64OsTicks;             //����ϵͳ����ticks��
extern void Init_Cpu(void);
extern void Iboot_LoadPreload(void);

static uint64_t gRunTicks = 0;
static bool_t gResumeTickFlag = false;

__attribute__((weak)) uint64_t __DjyGetTicks(void)
{
    return gRunTicks;
}

//�ɵ����߱�֤���ð�ȫ
__attribute__((weak)) void DjySetUpdateTickFlag(bool_t flag)
{
    gResumeTickFlag = flag;
}

__attribute__((weak)) bool_t DjyGetUpdateTickFlag(void)
{
    return gResumeTickFlag;
}

//�ɵ����߱�֤ԭ�Ӳ���
__attribute__((weak)) void DjyUpdateTicks(uint32_t ticks)
{
    gRunTicks += ticks;
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
    HardExp_ConnectSystick(DJY_ScheduleIsr);
    CORET->LOAD = CN_CFG_FCLK/CN_CFG_TICK_HZ;
    CORET->VAL =CN_CFG_FCLK/CN_CFG_TICK_HZ;
    CORET->CTRL = CORET_CTRL_CLKSOURCE_Msk |
               CORET_CTRL_TICKINT_Msk |
               CORET_CTRL_ENABLE_Msk;                               /* Enable CORET IRQ and CORET Timer */

    csi_vic_enable_irq(PIC_IRQID_CTIMER);

    djybsp_isr_hdl_register(CN_INT_LINE_CORET,0,Exp_SystickTickHandler,CN_INT_LINE_CORET);
}

// =============================================================================
// ���ܣ���ȡϵͳʱ��
// ��������
// ���أ���ǰus��
// =============================================================================
__attribute__((weak))   uint64_t __DjyGetSysTime(void)
{
    s64 time;
    static s64 BakTime = 0;
    u32 temp;
    atom_low_t atom_low;
    temp =CN_CFG_FCLK/CN_CFG_TICK_HZ - CORET->VAL;

    atom_low = Int_LowAtomStart();
    time = g_s64OsTicks;
    Int_LowAtomEnd(atom_low);
    time = time*CN_CFG_TICK_US + (temp*CN_CFG_FINE_US >>16);
    if(time < BakTime)
        time += CN_CFG_TICK_US;
    BakTime = time;

    return (uint64_t)time;
}

extern void Iboot_LoadPreload(void);
#include <blackbox.h>
#include <Iboot_info.h>
//extern tagIapVar pg_IapVar;

#define CN_BOOT_SOFTREBOOT_FLAG         (CN_BOOT_LEGALKEY <<1)
#define CN_BOOT_SOFTRELOAD_FLAG         (CN_BOOT_LEGALKEY <<2)
#define CN_BOOT_HARDRST_FLAG         (CN_BOOT_LEGALKEY <<3)
#define CN_BOOT_SOFTRESET_FLAG          (CN_BOOT_LEGALKEY <<4)

// =============================================================================
// ���ܣ����е�ѡ��ϵͳ���з�ʽǰ������M3/M4��CPU����PC��ת��Init_CPU()
// ��������
// ���أ���
// =============================================================================
void CPU_Reboot(void)
{


        u32 InitCpu_Addr;
        InitCpu_Addr = *(u32*)0x02160000;
        ((void (*)(void))(InitCpu_Addr))();
}

void CPU_Reset(void)
{
    u32 InitCpu_Addr;
    InitCpu_Addr = *(u32*)0x02160000;
    ((void (*)(void))(InitCpu_Addr))();

}

void CPU_RestartSystem(void)
{
    Iboot_LoadPreload();
}


