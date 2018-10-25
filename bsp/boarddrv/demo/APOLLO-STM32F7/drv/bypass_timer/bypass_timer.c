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
#include "cpu_peri.h"
#include "int.h"

static u64 g_bypass_timer_cnt = 0;

static u32  Bypadd_Timer_Int(ptu32_t handle)
{
    static u8 fi = 0;
    TIM5->SR = 0;//���жϱ�־
    if(fi!=0)
        g_bypass_timer_cnt += 1080000000;
    fi = 1;
    return 0;
}

u64 Bypass_GetCnt(void)
{
    static u32 temp_per = 0;
    static u32 temp = 0;

    if(TIM5->ARR==0)
        return 0;

    if(temp_per==0 && temp==0)
    {
        temp = TIM5->CNT;
        temp_per = temp;
        return (g_bypass_timer_cnt + TIM5->ARR - TIM5->CNT);
    }
    else
    {
        temp = TIM5->CNT;
        if( temp_per < temp)
        {
            temp_per = temp;
            g_bypass_timer_cnt += 1080000000;
            return (g_bypass_timer_cnt + TIM5->ARR - TIM5->CNT);
        }
        else
        {
            temp_per = temp;
            return (g_bypass_timer_cnt + TIM5->ARR - TIM5->CNT);
        }
    }
}

u64 Bypass_Cnt_to_Systick(void)
{
    return (Bypass_GetCnt()<<1);
    //return (Bypass_GetCnt()/108)*(CN_CFG_TIME_BASE_HZ/Mhz);
}

void Bypass_Timer_PerInit(void)
{
    g_bypass_timer_cnt = 0;
    TIM5->ARR = 0;
    TIM5->CNT = 0;
}

void Bypass_TimerInit(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    TIM5->CR1 &= ~(TIM_CR1_CEN); //��ֹTIMER
    //TIM5->CR1 &= ~(TIM_CR1_DIR);
    TIM5->CR1 |= TIM_CR1_ARPE | TIM_CR1_DIR;//�Զ���װ

    //TIM5->DIER |= TIM_DIER_UIE;//ʹ�ܸ����ж�
    TIM5->PSC = 0;//��Ƶϵ�� Ϊ�� ����Ƶ(1/108)1uS
    TIM5->ARR = 1080000000;//��ʱ��Ԥװ��ֵ:10S

//  Int_Register(CN_INT_LINE_TIM5);
//  Int_SetClearType(CN_INT_LINE_TIM5,CN_INT_CLEAR_AUTO);
//  Int_IsrConnect(CN_INT_LINE_TIM5,Bypadd_Timer_Int);
//  Int_SettoAsynSignal(CN_INT_LINE_TIM5);
//  Int_ClearLine(CN_INT_LINE_TIM5);
//  Int_RestoreAsynLine(CN_INT_LINE_TIM5);

    //TIM5->CNT = 0;
    TIM5->SR = 0;//���жϱ�־
    TIM5->CR1 |= TIM_CR1_CEN;
}

