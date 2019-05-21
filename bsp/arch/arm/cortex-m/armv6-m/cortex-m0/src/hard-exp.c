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
//����ģ��:�ж�ģ��
//���ߣ�lst
//�汾��V1.1.0
//�ļ�����: cm3�쳣����ģ��
//����˵��:
//�޶���ʷ:
//2. ����: 2010-01-20
//   ����: lst
//   �°汾��: V1.1.0
//   �޸�˵��: �޸� exp_esr_tick������������tick�ж��е����첽�źŵ�ʹ�ܺͽ�ֹ
//      ����ʱ������en_asyn_signal_counter�Ȳ���δ��ȷ���ã�������⡣
//1. ����: 2009-08-30
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��:
//------------------------------------------------------
#include "stdint.h"
#include "stddef.h"
#include "arch_feature.h"
#include "djyos.h"
#include "int.h"
#include "cpu.h"
//#include "core_cm0.h"
#include "hard-exp.h"
#include "board-config.h"
#if (CN_USE_TICKLESS_MODE)
#include "tickless.h"
#endif
#include "dbug.h"

//�쳣��������ⲿ����
extern void HardExp_SystickHandler(void);
extern void HardExp_SvcHandler(void);
extern   uint32_t   msp_top[ ];
extern   void Init_Cpu(void);
extern   void HardExp_HardfaultHandler(void);
extern   void HardExp_NmiHandler(void);

extern struct IntLine *tg_pIntLineTable[];       //�ж��߲��ұ�
extern struct IntMasterCtrl  tg_int_global;          //���岢��ʼ�����жϿ��ƽṹ
extern void __Djy_ScheduleAsynSignal(void);
void (*user_systick)(u32 inc_ticks);

struct SystickReg volatile * const pg_systick_reg
                        = (struct SystickReg *)0xE000E010;
struct ScbReg volatile * const pg_scb_reg
                        = (struct ScbReg *)0xe000ed00;

u32 g_u32ExpTable[16] __attribute__((section(".table.exceptions")));

bool_t HardExp_Init(void)
{
    pg_scb_reg->pri12_15 |=0xff000000;    //systick��Ϊ������ȼ�,=�첽�ź�
    pg_scb_reg->pri8_11 |= 0xff000000;    //svc�����ȼ����첽�ź���ͬ��
    /*����ҳ���ʼ��*/
    g_u32ExpTable[0]                            = (uint32_t)msp_top;
    g_u32ExpTable[CN_SYSVECT_RESET]             = (uint32_t)Init_Cpu;
    g_u32ExpTable[CN_SYSVECT_NMI]               = (uint32_t) HardExp_NmiHandler;
    g_u32ExpTable[CN_SYSVECT_HARD_FAULT]        = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[CN_SYSVECT_MEMMANAGE_FAULT]   = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[CN_SYSVECT_BUS_FAULT]         = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[CN_SYSVECT_USAGE_FAULT]       = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[7]                            = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[8]                            = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[9]                            = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[10]                           = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[CN_SYSVECT_SVC]               = (u32)HardExp_SvcHandler;
    g_u32ExpTable[CN_SYSVECT_DEBUG]             = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[13]                           = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[CN_SYSVECT_PENDSV]            = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[CN_SYSVECT_SYSTICK]           = (u32)HardExp_SystickHandler;

    return true;
}
void HardExp_ConnectNmi(void (*esr)(void))
{
}

void HardExp_ConnectSystick(void (*tick)(u32 inc_ticks))
{
    user_systick = tick;
}

void HardExp_EsrTick(void)
{
    if((pg_systick_reg->ctrl & bm_systick_ctrl_tickint) == 0)
        return;
    g_bScheduleEnable = false;
    tg_int_global.nest_asyn_signal++;
    if(!DjyGetUpdateTickFlag())
        DjyUpdateTicks(1);
    else
        DjySetUpdateTickFlag(false);
    user_systick((CN_USE_TICKLESS_MODE==1)?0:1);
    tg_int_global.nest_asyn_signal--;
    if(g_ptEventReady != g_ptEventRunning)
        __Djy_ScheduleAsynSignal();       //ִ���ж��ڵ���
    g_bScheduleEnable = true;
}


//�쳣����

void Exp_HardHandle(u32 spValue ,u8 intNum,u32 pcVal)
{
    printk("��ǰ�쳣��Ϊ:%u\r\n",intNum);
    printk("pc:%x\r\n",pcVal);
    printk("sp:%x\r\n",spValue);

    while(1);
}
