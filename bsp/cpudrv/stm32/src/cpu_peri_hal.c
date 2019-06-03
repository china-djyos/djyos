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
//����ģ��: bsp
//����:  ������.
//�汾��V1.0.0
//�ļ�����: HAL��Ҫʹ�õ��Ľӿں���,stm32��hal�����ʱ���������⣬�����ISR�е��ã�����
//         �ڹ��жϣ��ص��ȣ�������µ��ã�����ѭ�����������������ʵ�֡�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#include <stdio.h>
#include <stddef.h>
#include "cpu_peri.h"
#include <os.h>
#include <board-config.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

#ifdef USE_HAL_DRIVER

//==============================================================================
//HAL_RCC���õ�SystemCoreClock����ΪDJYOSʹ���Լ���ʱ�ӳ�ʼ�����̣��û�ʹ��hal��ʱ��
//�ͻ�û�ж��壬�����˴�����͸�ֵ
//����ETH���������У�stm32f7xx_hal_eth.c�л����
uint32_t SystemCoreClock = CN_CFG_MCLK;         //HAL_RCC���õ�����

#if(CFG_STM32_HAL_TIMER == 1)
    TIM_TypeDef* const STM32_HAL_TIMER = TIM1;
#elif(CFG_STM32_HAL_TIMER == 2)
    TIM_TypeDef* const STM32_HAL_TIMER = TIM2;
#elif(CFG_STM32_HAL_TIMER == 3)
    TIM_TypeDef* const STM32_HAL_TIMER = TIM3;
#elif(CFG_STM32_HAL_TIMER == 4)
    TIM_TypeDef* const STM32_HAL_TIMER = TIM4;
#elif(CFG_STM32_HAL_TIMER == 5)
    TIM_TypeDef* const STM32_HAL_TIMER = TIM5;
#elif(CFG_STM32_HAL_TIMER == 6)
    TIM_TypeDef* const STM32_HAL_TIMER = TIM6;
#elif(CFG_STM32_HAL_TIMER == 7)
    TIM_TypeDef* const STM32_HAL_TIMER = TIM7;
#elif(CFG_STM32_HAL_TIMER == 8)
    TIM_TypeDef* const STM32_HAL_TIMER = TIM8;
#else
#error "error �� û�����ö�ʱ������";
#endif

//HAL�������øú�����ʱ���������ˣ����õ�HAL�⣬����Ҫ�ṩ�ú����ṩ��ʱ
//return ms

//------------------------------------------------------------------------------
//���ܣ��滻hal���б�weak���ε�ͬ����������djyos��ʹ��stm32��hal���е���ʱ���ܣ����ڴ�
//     ����ʵ��.
//------------------------------------------------------------------------------
uint32_t HAL_GetTick(void)
{
    static u32 sTicks = 0;
    static u16 sTimCntLast = 0;
    u16 TimCntCurrent;

//    TimCntCurrent = (__HAL_TIM_GET_COUNTER(&HalTimHandle))&0xFFFF;//0.5ms
    TimCntCurrent = (STM32_HAL_TIMER->CNT)&0xFFFF;
    if( sTimCntLast > TimCntCurrent)
    {
        sTicks += 32768;    //ʱ�ӷֱ���0.5mS��16bit��32768mS
    }
    sTimCntLast = TimCntCurrent;
    return sTicks + (TimCntCurrent>>1);
}

//------------------------------------------------------------------------------
//���ܣ��滻hal���б�weak���ε�ͬ����������djyos��ʹ��stm32��hal���е���ʱ���ܣ����ڴ�
//     ����ʵ��.
//------------------------------------------------------------------------------
void HAL_SuspendTick(void)
{
    TIM_HandleTypeDef HalTimHandle;
    HalTimHandle.Instance = STM32_HAL_TIMER;
    HAL_TIM_Base_Stop(&HalTimHandle);
}
//------------------------------------------------------------------------------
//���ܣ��滻hal���б�weak���ε�ͬ����������djyos��ʹ��stm32��hal���е���ʱ���ܣ����ڴ�
//     ����ʵ��.
//------------------------------------------------------------------------------
void HAL_ResumeTick(void)
{
    TIM_HandleTypeDef HalTimHandle;
    HalTimHandle.Instance = STM32_HAL_TIMER;
    HAL_TIM_Base_Start(&HalTimHandle);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
//------------------------------------------------------------------------------
//���ܣ��滻hal���б�weak���ε�ͬ����������djyos��ʹ��stm32��hal���е���ʱ���ܣ����ڴ�
//     ��������Ϊ�պ���.
//------------------------------------------------------------------------------
HAL_StatusTypeDef HAL_InitTick (uint32_t TickPriority)
{
    return HAL_OK;
}
#pragma GCC diagnostic pop

//------------------------------------------------------------------------------
//���ܣ�
void Hal_Timer_Clk_Enable(void)
{
    //�˴��ĺ�����board.c��ʵ�֣�����ʹ�ܶ�Ӧ��TIM��ʱ��
    //������TIM6Ϊ��
#if(CFG_STM32_HAL_TIMER == 1)
    __HAL_RCC_TIM1_CLK_ENABLE();
#elif(CFG_STM32_HAL_TIMER == 2)
    __HAL_RCC_TIM2_CLK_ENABLE();
#elif(CFG_STM32_HAL_TIMER == 3)
    __HAL_RCC_TIM3_CLK_ENABLE();
#elif(CFG_STM32_HAL_TIMER == 4)
    __HAL_RCC_TIM4_CLK_ENABLE();
#elif(CFG_STM32_HAL_TIMER == 5)
    __HAL_RCC_TIM5_CLK_ENABLE();
#elif(CFG_STM32_HAL_TIMER == 6)
    __HAL_RCC_TIM6_CLK_ENABLE();
#elif(CFG_STM32_HAL_TIMER == 7)
    __HAL_RCC_TIM7_CLK_ENABLE();
#elif(CFG_STM32_HAL_TIMER == 8)
    __HAL_RCC_TIM8_CLK_ENABLE();
#else
#error "error �� û�����ö�ʱ������";
#endif
}
//�˴���TIM6
void HAL_TickInit(void)
{
    u32 uwPrescalerValue;
    TIM_HandleTypeDef HalTimHandle;
    Hal_Timer_Clk_Enable();

    uwPrescalerValue = ((CN_CFG_MCLK/4) / 1000) - 1;    //Counter Clock = 2K
    HalTimHandle.Instance = STM32_HAL_TIMER;
    HalTimHandle.Init.Period        = 0xFFFFFFFF;
    HalTimHandle.Init.Prescaler     = uwPrescalerValue;
    HalTimHandle.Init.ClockDivision = 0;
    HalTimHandle.Init.CounterMode   = TIM_COUNTERMODE_UP;

#if defined(TIM_AUTORELOAD_PRELOAD_ENABLE)
    HalTimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
#endif
    HAL_TIM_Base_DeInit(&HalTimHandle);
    HAL_TIM_Base_Init(&HalTimHandle);
//    MODIFY_REG(STM32_HAL_TIMER->CR1, TIM_CR1_ARPE, TIM_CR1_ARPE);
    HAL_TIM_Base_Start(&HalTimHandle);
}
#endif

