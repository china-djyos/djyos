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
//����ģ�飺ϵͳ���ܲ���ģ��
//����:
//�汾��V1.0.0
//�ļ�����:
//����˵��:
//�޶���ʷ:
//
#include "cpu_peri.h"
#include <int.h>
#include <systime.h>
#include "djyos.h"

typedef struct
{
    volatile u32 FSR;
    volatile u32 CR;
    volatile u32 TC;
    volatile u32 PR;
    volatile u32 PC;
    volatile u32 MR;
    volatile u32 MCR;
    volatile u32 EMR;
    volatile u32 EWCR;
    volatile u32 CAPTR;
    volatile u32 CCR;
    volatile u32 INTIF;
    volatile u32 INTIE;
}TIM_TypeDef;

TIM_TypeDef *TimerReg;
bool_t TimerStarted = false;
extern u32 IntLine;

void Test_PushTimeISR(u32 time_counter);
void Test_PushTimeHook(u32 time_counter);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
//��������ʵ��TimerIsr���ڶ�ʱ�������У��붨��� weak ����
u32 __SCM6xxTimer_isr(ptu32_t TimerHandle)
{
    u32 cnt1,cnt2;
    u32 cnt16L1 = 0,cnt16H1 = 0;
    u32 cnt16L2 = 0,cnt16H2 = 0;

    TIM_TypeDef* Reg_Tim16L = (TIM_TypeDef*) TimerReg;
    TIM_TypeDef* Reg_Tim16H = (TIM_TypeDef*)((u32)TimerReg + 0x40);

    cnt16L1 = Reg_Tim16L->TC;
    cnt16H1 = Reg_Tim16H->TC;
    cnt1   = (cnt16H1 << 16) | cnt16L1;

    cnt16L2 = Reg_Tim16L->TC;
    cnt16H2 = Reg_Tim16H->TC;
    cnt2   = (cnt16H2 << 16) | cnt16L2;

    Reg_Tim16L->INTIF |= (1<<0);//���жϱ�־
    Reg_Tim16H->INTIF |= (1<<0);
    Int_ClearLine(IntLine);

    cnt1 = cnt1 - (cnt2-cnt1);
    Test_PushTimeISR(cnt1);     //��¼����
    return 0;
}
#pragma GCC diagnostic pop

u32 __Test_TimerStart(void)
{
    TimerStarted = true;
    TIM_TypeDef* Reg_Tim16L = (TIM_TypeDef*) TimerReg;
    TIM_TypeDef* Reg_Tim16H = (TIM_TypeDef*)((u32)TimerReg + 0x40);

    //���жϱ��
    Reg_Tim16L->INTIF |= 0xf;
    Reg_Tim16H->INTIF |= 0xf;

    Reg_Tim16L->TC  = 0;
    Reg_Tim16H->TC  = 0;

    //��λ
    Reg_Tim16L->CR |= (1<<1);
    Reg_Tim16H->CR |= (1<<1);

    //ֹͣ��λ
    Reg_Tim16L->CR &= ~(1<<1);
    Reg_Tim16H->CR &= ~(1<<1);

    //��������
    Reg_Tim16L->CR = 0x1;
    Reg_Tim16H->CR = 0x1;


    return 0;
}

bool_t __Test_TimerEnd(u32 *val)
{
    u32 valueL = 0,valueH = 0;
    if(TimerStarted)
    {
        TIM_TypeDef* Reg_Tim16L = (TIM_TypeDef*) TimerReg;
        TIM_TypeDef* Reg_Tim16H = (TIM_TypeDef*)((u32)TimerReg + 0x40);

        TimerStarted = false;
        valueL = Reg_Tim16L->TC;
        valueH = Reg_Tim16H->TC;
        *val   = (valueH<<16) | valueL;
        return true;
    }
    else
        return false;
}

void Test_IntSyncHook(ucpu_t SchType)
{
    u32 cnt1,cnt2;
    u32 valueL = 0,valueH = 0;

    //�����ʱ��Ż�ȡʱ��,�г���������
    if(SchType == EN_SWITCH_IN)
    {
        TIM_TypeDef* Reg_Tim16L = (TIM_TypeDef*) TimerReg;
        TIM_TypeDef* Reg_Tim16H = (TIM_TypeDef*)((u32)TimerReg + 0x40);
        valueL = Reg_Tim16L->TC;
        valueH = Reg_Tim16H->TC;
        cnt1 = (valueH<<16) | valueL;

        valueL = Reg_Tim16L->TC;
        valueH = Reg_Tim16H->TC;
        cnt2   = (valueH<<16) | valueL;

        cnt1 = cnt1 - (cnt2-cnt1);
        Test_PushTimeHook(cnt1);//��¼����
    }
}

