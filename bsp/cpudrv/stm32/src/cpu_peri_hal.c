// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��cpu_peri_hal.c
// ģ������: HAL��Ҫʹ�õ��Ľӿں���
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 2/20.2017
// =============================================================================

#include <stdio.h>
#include <stddef.h>
#include "cpu_peri.h"
#include <os.h>
#include <board-config.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

#ifdef USE_HAL_DRIVER
//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������
//%$#@describe      ****���������ʼ
//component name:"cpu_peri_hal" //stm32��hal�ⲹ��
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp���             //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                   //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//grade:none                    //��ʼ��ʱ������ѡֵ��none��init��main��none��ʾ�����ʼ����
                                //init��ʾ�ڵ���main֮ǰ��main��ʾ��main�����г�ʼ��
//dependence:"none"             //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������������������������none����ʾ�������������
                                //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header              //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef CFG_STM32_HAL_TIMER   //****�������Ƿ��Ѿ����ú�
#warning    hal�ⲹ��ģ�����δ���ã�ʹ��Ĭ��ֵ
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#define CFG_STM32_HAL_TIMER     TIM6//"HAL��ʹ�õĶ�ʱ��",����HAL��ʹ�õĶ�ʱ��
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

//==============================================================================
//HAL_RCC���õ���������ΪDJYOSʹ���Լ���ʱ�ӳ�ʼ�����̣���ˣ���ʹ��HAL���ʱ����ЩHAL
//�ڲ�����ø����������˴���ֵ
//����ETH���������У�stm32f7xx_hal_eth.c�л����
uint32_t SystemCoreClock = CN_CFG_MCLK;         //HAL_RCC���õ�����
//static TIM_HandleTypeDef HalTimHandle;
//HAL�������øú�����ʱ���������ˣ����õ�HAL�⣬����Ҫ�ṩ�ú����ṩ��ʱ
//return ms
uint32_t HAL_GetTick(void)
{
    static u32 sTicks = 0;
    static u16 sTimCntLast = 0;
    u16 TimCntCurrent;

//    TimCntCurrent = (__HAL_TIM_GET_COUNTER(&HalTimHandle))&0xFFFF;//0.5ms
    TimCntCurrent = (CFG_STM32_HAL_TIMER->CNT)&0xFFFF;
    if( sTimCntLast > TimCntCurrent)
    {
        sTicks += 32768;    //ʱ�ӷֱ���0.5mS��16bit��32768mS
    }
    sTimCntLast = TimCntCurrent;
    return sTicks + (TimCntCurrent>>1);
}

void HAL_SuspendTick(void)
{
    TIM_HandleTypeDef HalTimHandle;
    HalTimHandle.Instance = CFG_STM32_HAL_TIMER;
    HAL_TIM_Base_Stop(&HalTimHandle);
}
void HAL_ResumeTick(void)
{
    TIM_HandleTypeDef HalTimHandle;
    HalTimHandle.Instance = CFG_STM32_HAL_TIMER;
    HAL_TIM_Base_Start(&HalTimHandle);
}
HAL_StatusTypeDef HAL_InitTick (uint32_t TickPriority)
{
    return HAL_OK;
}

__attribute__((weak)) void Hal_Timer_Clk_Enable(void)
{
    //�˴��ĺ�����board.c��ʵ�֣�����ʹ�ܶ�Ӧ��TIM��ʱ��
    //������TIM6Ϊ��
    __HAL_RCC_TIM6_CLK_ENABLE();
}
//�˴���TIM6
void HAL_TickInit(void)
{
    u32 uwPrescalerValue;
    TIM_HandleTypeDef HalTimHandle;
    Hal_Timer_Clk_Enable();

    uwPrescalerValue = ((CN_CFG_MCLK/4) / 1000) - 1;    //Counter Clock = 2K
    HalTimHandle.Instance = CFG_STM32_HAL_TIMER;
    HalTimHandle.Init.Period        = 0xFFFFFFFF;
    HalTimHandle.Init.Prescaler     = uwPrescalerValue;
    HalTimHandle.Init.ClockDivision = 0;
    HalTimHandle.Init.CounterMode   = TIM_COUNTERMODE_UP;
    HalTimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    HAL_TIM_Base_DeInit(&HalTimHandle);
    HAL_TIM_Base_Init(&HalTimHandle);
    HAL_TIM_Base_Start(&HalTimHandle);
}
#endif

