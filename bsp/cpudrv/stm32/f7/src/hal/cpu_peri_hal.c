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
#include <stm32f7xx_hal_tim.h>


#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern void HAL_TickInit(void);
//    HAL_TickInit();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu_peri_hal" //stm32��hal�ⰲȫ����
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp���             //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                   //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//grade:init                    //��ʼ��ʱ������ѡֵ��none��init��main��none��ʾ�����ʼ����
                                //init��ʾ�ڵ���main֮ǰ��main��ʾ��main�����г�ʼ��
//dependence:"none"             //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������������������������none����ʾ�������������
                                //����������������������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
//%$#@end configue  ****�������ý���
//@#$%component end configure

//==============================================================================
//HAL_RCC���õ���������ΪDJYOSʹ���Լ���ʱ�ӳ�ʼ�����̣���ˣ���ʹ��HAL���ʱ����ЩHAL
//�ڲ�����ø����������˴���ֵ
//����ETH���������У�stm32f7xx_hal_eth.c�л����
uint32_t SystemCoreClock = CN_CFG_MCLK;         //HAL_RCC���õ�����

//HAL�������øú�����ʱ���������ˣ����õ�HAL�⣬����Ҫ�ṩ�ú����ṩ��ʱ
//return ms
uint32_t HAL_GetTick(void)
{
    static u32 sTicks = 0;
    static u16 sTimCntLast = 0;
    u16 TimCntCurrent;

    TIM_HandleTypeDef TimHandle;

    TimHandle.Instance = TIM6;
    TimCntCurrent = (__HAL_TIM_GET_COUNTER(&TimHandle))&0xFFFF;//0.5ms
    if( sTimCntLast > TimCntCurrent)
    {
        sTicks += 32768;    //ʱ�ӷֱ���0.5mS��16bit��32768mS
    }
    sTimCntLast = TimCntCurrent;
    return sTicks + (TimCntCurrent>>1);
}

void HAL_SuspendTick(void)
{
    TIM_HandleTypeDef TimHandle;
    TimHandle.Instance = TIM6;
    HAL_TIM_Base_Stop(&TimHandle);
}
void HAL_ResumeTick(void)
{
    TIM_HandleTypeDef TimHandle;
    TimHandle.Instance = TIM6;
    HAL_TIM_Base_Start(&TimHandle);
}


//�˴���TIM6
void HAL_TickInit(void)
{
    u32 uwPrescalerValue;
    TIM_HandleTypeDef TimHandle;

    __HAL_RCC_TIM6_CLK_ENABLE();

    uwPrescalerValue = ((CN_CFG_MCLK/4) / 1000) - 1;    //Counter Clock = 2K
    TimHandle.Instance = TIM6;
    TimHandle.Init.Period        = 0xFFFFFFFF;
    TimHandle.Init.Prescaler     = uwPrescalerValue;
    TimHandle.Init.ClockDivision = 0;
    TimHandle.Init.CounterMode   = TIM_COUNTERMODE_UP;
    TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    HAL_TIM_Base_DeInit(&TimHandle);
    HAL_TIM_Base_Init(&TimHandle);
    HAL_TIM_Base_Start(&TimHandle);
}


