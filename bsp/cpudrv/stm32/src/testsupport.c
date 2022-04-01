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
#include <djyos.h>
#include <int.h>
#include <systime.h>

TIM_TypeDef *TimerReg;
bool_t TimerStarted = false;    //��Ҫ�޸ı�������������ں���SysPerformTest��Ҫ��
extern u32 IntLine;     //��ʹ�ö�ʱ�����жϺţ��ڲ�����ں���SysPerformTest�и�ֵ

void Test_PushTimeISR(u32 time_counter);
void Test_PushTimeHook(u32 time_counter);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
//��������ʵ��TimerIsr���ڶ�ʱ�������У��붨��� weak ����
//���������ǣ��������жϷ��������뱾������ʱ������������Test_PushTimeISR����
u32 __STM32Timer_isr(ptu32_t TimerHandle)
{
    u32 cnt1,cnt2;
    //cnt1��Ҫ��ȡ���ǽ���ISR��ʱ�䣬�����ֱ�Ӷ�ȡ�����ȡCNT�Ĵ�����ʱ��Ҳ�������ڣ������Σ�
    //��ȥ��ֵ������ȷ��
    cnt1 = ((TIM_TypeDef *)TimerReg)->CNT;
    cnt2 = ((TIM_TypeDef *)TimerReg)->CNT;
    ((TIM_TypeDef *)TimerReg)->SR = 0;//���жϱ�־
    Int_ClearLine(IntLine);

    cnt1 = cnt1 - (cnt2-cnt1);
    Test_PushTimeISR(cnt1);     //��¼����
    return 0;
}
#pragma GCC diagnostic pop

//ʵ�����ܼ��ģ���е� weak ����
//���ò����ö�ʱ��������־��
u32 __Test_TimerStart(void)
{
    TimerStarted = true;
    ((TIM_TypeDef *)TimerReg)->CNT = 0;
    return 0;
}

//ʵ�����ܼ��ģ���е� weak ����
//�����ʱ����������״̬����ȡ���ϴε��� __Test_TimerStart ������ʱ��������ֹͣ��ʱ����
bool_t __Test_TimerEnd(u32 *val)
{
    if(TimerStarted)
    {
        TimerStarted = false;
        *val = ((TIM_TypeDef *)TimerReg)->CNT;
        return true;
    }
    else
        return false;
}

//ʵ�����ܼ��ģ���е� weak ����
//�����ж�ͬ�����ԣ��������жϷ��������뱾������ʱ������������Test_PushTimeHook����
void Test_IntSyncHook(ucpu_t SchType)
{
    u32 cnt1,cnt2;

    //�����ʱ��Ż�ȡʱ��,�г���������
    if(SchType == EN_SWITCH_IN)
    {
        cnt1 = ((TIM_TypeDef *)TimerReg)->CNT;
        cnt2 = ((TIM_TypeDef *)TimerReg)->CNT;
        cnt1 = cnt1 - (cnt2-cnt1);
        Test_PushTimeHook(cnt1);        //��¼����
    }
}

