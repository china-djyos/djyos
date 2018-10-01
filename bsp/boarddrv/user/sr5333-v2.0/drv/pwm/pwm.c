//----------------------------------------------------
// Copyright (c) 2018,Open source team. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ�ɶ����߲���ϵͳ��Դ�Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������¶������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�����������У���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�����������У��Լ�����
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
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��cpu_peri_pwm.c
// ģ������: PWM����
// ģ��汾: V1.00
// ������Ա: zhb
// ����ʱ��: 8/24.2016
// =============================================================================

#include "string.h"
#include "cpu_peri.h"
#include "pwm.h"


// =========================================================================
// �������ܣ�����PWM
// ���������idx,PWM���,������·�����Ϊ0��1.�ֱ��ӦPWMC0_PWML3/PWMC0_PWML2.
//          cycle,����,��λus
//          duty,ռ�ձ�,25%��ռ�ձȣ�ֱ��25����
// �����������
// ����ֵ  ����
// =========================================================================
void PWM_Config(uint8_t idx,uint32_t cycle,uint32_t duty)
{
    uint32_t psc,arr;



    if(cycle > 20000)       //����20ms
    {
        //���綨ʱ��ʱ��ԴΪ108M�����Ƶ��PSC = 10800
        //������Ϊ100us��CYCLE��ΧΪ[100us,6.5535��]
        psc = 100 * ((CN_CFG_PCLK1*2)/1000000) - 1;
        arr = cycle/100;
    }
    else
    {
        //����Ϊ1us��cycle[1us,0.065535��]
        psc = 1 * (CN_CFG_PCLK1*2)/1000000 - 1;
        arr = cycle/1;
    }

    if(idx == 0)
    {
        RCC->APB1ENR    |= RCC_APB1ENR_TIM3EN;
        TIM3->CR1       &= ~(TIM_CR1_CEN);                      //DISABLE
        TIM3->CR1       |= TIM_CR1_ARPE;                        //ENABLE RELOAD
        TIM3->PSC       = psc;                                  //PRESCALER value
        TIM3->ARR       = arr;                                  //reload value
        TIM3->CCMR2     = TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE;    //PWM MODE 1
        TIM3->CCR3      = duty * arr/100;                       //
        TIM3->CCER      = TIM_CCER_CC3E;                        //ENABLE CH 3
        TIM3->CR1       |= TIM_CR1_CEN;                         //ENABLE
    }
    else if(idx == 1)
    {
        RCC->APB1ENR    |= RCC_APB1ENR_TIM4EN;
        TIM4->CR1       &= ~(TIM_CR1_CEN);                      //DISABLE
        TIM4->CR1       |= TIM_CR1_ARPE;                        //ENABLE RELOAD
        TIM4->PSC       = psc;                                  //PRESCALER value
        TIM4->ARR       = arr;                                  //reload value
        TIM4->CCMR1     = TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2PE;    //PWM MODE 1
        TIM4->CCR2      = duty * arr/100;                       //
        TIM4->CCER      = TIM_CCER_CC2E;                        //ENABLE CH 4
        TIM4->CR1       |= TIM_CR1_CEN;                         //ENABLE
    }
    else
    {

    }
}

// =========================================================================
// �������ܣ�Ӧ�ò�����PWM��Ӧ�ò����õ�ռ�ձ���ײ�ռ�ձ���Ҫȡ����
// ���������idx,PWM���,������·�����Ϊ0��1.
//          cycle,����,��λus
//          duty,ռ�ձ�,25%��ռ�ձȣ�ֱ��25����
// �����������
// ����ֵ  ����
// =========================================================================
void PWM_Set(uint8_t idx,uint32_t cycle,uint32_t duty)
{
    if(duty>100)
    {
        return ;
    }
    if((cycle < 100) || (cycle > 6000000))
    {
        return;
    }

    PWM_Config(idx,cycle,duty);
}


