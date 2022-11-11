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
#include "stdio.h"
#include "djyos.h"
#include "int.h"
#include "blackbox.h"
#include "csi_core.h"
#include "silan_irq.h"

#include "board-config.h"
#if (CN_USE_TICKLESS_MODE)
#include "tickless.h"
#endif

#undef  VIC_TSPR
#define VIC_TSPR  0xE000EC10

extern struct IntMasterCtrl  tg_int_global;
extern void __DJY_ScheduleAsynSignal(void);
extern   void Init_Cpu(void);
extern   void HardExp_HardfaultHandler(void);
extern   void CK_Trap0ISR(void);
extern u32 __Vectors_Tbl[];

void (*user_systick)(u32 inc_ticks);

#define readl(addr) \
    ({ unsigned int __v = (*(volatile unsigned int *) (addr)); __v; })

static void __Exp_TableSet(void)
{
    __set_VBR((uint32_t) & (__Vectors_Tbl));
    /* Here we may setting exception vector, MGU, cache, and so on. */
    csi_cache_set_range(0, 0x00000000, CACHE_CRCR_4M, 1);
    csi_cache_set_range(1, 0x22000000, CACHE_CRCR_8M, 1);
    csi_dcache_enable();
    csi_icache_enable();

    //*((uint32_t*)VIC_TSPR) = 0xb00;
}


void HardExp_ConnectSystick(void (*tick)(u32 inc_ticks))
{
    user_systick = tick;
}

void Exp_SystickTickHandler(void)
{
#if (CN_USE_TICKLESS_MODE)
    u32 tick=0;
#endif
    g_bScheduleEnable = false;
    csi_vic_clear_pending_irq(PIC_IRQID_CTIMER);
    readl(0xE000E010);
//    tg_int_global.en_asyn_signal = false;
    tg_int_global.en_asyn_signal_counter = 1;
    tg_int_global.nest_asyn_signal = 1;
    if(!DjyGetUpdateTickFlag())
        DjyUpdateTicks(1);
    else
        DjySetUpdateTickFlag(false);
    user_systick((CN_USE_TICKLESS_MODE==1)?0:1);

    tg_int_global.nest_asyn_signal = 0;
//    tg_int_global.en_asyn_signal = true;
    tg_int_global.en_asyn_signal_counter = 0;
//  if(g_ptEventReady != g_ptEventRunning)
//      __DJY_ScheduleAsynSignal();
    g_bScheduleEnable = true;
}

void HardExp_Init(void)
{
    __Exp_TableSet();
}

enum EN_BlackBoxAction Exp_MemManageFaultHandler(u32 fpu_used,u32 *core_info)
{
    return EN_BLACKBOX_DEAL_IGNORE;
}

enum EN_BlackBoxAction Exp_HardFaultHandler(u32 fpu_used,u32 *core_info)
{
    return EN_BLACKBOX_DEAL_IGNORE;
}

enum EN_BlackBoxAction Exp_DebugFaultHandler(u32 fpu_used,u32 *core_info)
{
    return EN_BLACKBOX_DEAL_IGNORE;
}

enum EN_BlackBoxAction Exp_BusFaultHandler(u32 fpu_used,u32 *core_info)
{
    return EN_BLACKBOX_DEAL_IGNORE;
}

enum EN_BlackBoxAction Exp_UsageFaultHandler(u32 fpu_used,u32 *core_info)
{
    return EN_BLACKBOX_DEAL_IGNORE;
}

bool_t  HardExp_Analysis(struct ExpThrowPara *parahead, u32 endian)
{
    return true;
}

bool_t  HardExp_Decoder(struct ExpThrowPara *parahead, u32 endian)
{
    return true;
}

bool_t HardExp_InfoDecoderInit(void)
{
    return true;
}

bool_t HardExp_Trap_c(uint32_t *regs)
{
    int i = 0, j = 0;
    int32_t inc = 0;
    uint32_t vec = 0;
    asm volatile(
        "mfcr    %0, psr \n"
        "lsri    %0, 16 \n"
        "sextb   %0 \n"
        :"=r"(vec):);

    printk("CPU Exception : %d", vec);
    printk("\n");

    for (; i < (16 + inc); i++, j++) {
        printk("r%d: %08x\t", j, regs[i]);

        if ((i % 5) == 4) {
            printk("\n");
        }
    }
    inc += 16;

    for (; i < (16 + inc); i++, j++) {
        printk("r%d: %08x\t", j, regs[i]);

        if ((i % 5) == 4) {
            printk("\n");
        }
    }

    printk("\n");
    printk("epsr: %8x\n", regs[i]);
    printk("epc : %8x\n", regs[i + 1]);

    while (1);
    return true;
}
