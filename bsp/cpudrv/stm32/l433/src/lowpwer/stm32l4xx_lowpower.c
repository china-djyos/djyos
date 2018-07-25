//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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
//����ģ��: CPU��ص͹��Ĺ���
//����:  ������.
//�汾��V1.0.0
//�ļ�����: cpu��صĵ͹��Ĺ���,ʵ��lowpower.h�ļ��й涨bsp��Ҫ�ṩ�ĺ���,����
//          ��ʵ��CPU��صĺ���, �������ص�,�ɵ�boarddrvĿ¼��ȥ��
//          ʹ����stm32��42�����ݼĴ��������һ��.
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

//stm32�ϵĵ͹���Level����(�ο�lowpower.h�ļ��еĶ���)
//L0:��ִ��wfeָ��,����������.��L1һ��,������Ǵ�������˯��ģʽ.
//L1:wfeָ��,�ں�ִ�еĲ�����L0һ��,���û��Ļص���������ͣ������ʱ��
//L2:������stm32��ֹͣģʽ,����SRAM�ͼĴ���ֵ,�˳����������
//L3/L4:����stm32�Ĵ���ģʽ,SRAM�ͼĴ���ֵ����ʧֻ�б��ݼĴ���ά�ֹ���.��������
//�˳���,L3����ʱҪ����SRAM�ͼĴ���,�˳�ʱҪ�ָ������˳�������ִ��.

#include "stdint.h"
#include "lowpower.h"
#include "cpu_peri.h"
#include <stm32l4xx_hal_rcc.h>
#include <stm32l4xx_hal_rtc.h>
#include <stm32l4xx_hal_rtc.h>
#include <stm32l4xx_hal_rtc_ex.h>
#include <stm32l4xx_hal_pwr.h>

//----��ʼ���͹���Ӳ��--------------------------------------------------------
//����: ��ʼ���͹��Ĺ���Ӳ��,�������Ҫ��ʼ��,����ֱ��return true.��Ϊstm32�͹���
//      ������Ҫʹ�ñ������������, ����Ҫ�򿪱���ģ���Դ��ʹ�ܱ���������.
//����: ��
//����: ��
//----------------------------------------------------------------------------
bool_t __LP_BSP_HardInit(void)
{
	__HAL_RCC_PWR_CLK_ENABLE();//PWRģ��ʹ��

//    HAL_PWR_EnableBkUpAccess();//����ʹ��
    return true;
}

//----��ȡ���߼���------------------------------------------------------------
//����: ����ϵͳ�����߻��Ѳ�����,���ñ��������Ի�û���ǰ������״̬,����ֵ��
//      ��lowpower.h�ļ��ж����CN_SLEEP_L3��CN_SLEEP_L4��CN_SLEEP_NORMAL֮һ,
//      CN_SLEEP_NORMAL��ʾ�ϵ����Ӳ��λ,�������ָ�λ.ע��,����ֵ������
//      CN_SLEEP_L0~2֮һ.
//����: ��
//����: CN_SLEEP_L3��CN_SLEEP_L4��CN_SLEEP_NORMAL֮һ
//----------------------------------------------------------------------------
u32 __LP_BSP_GetSleepLevel(void)
{
    u32 bkt_DR;
    RTC_HandleTypeDef RTC_Handler;  //RTC���
    RTC_Handler.Instance=RTC;
    if(__HAL_PWR_GET_FLAG(PWR_FLAG_WU+PWR_FLAG_SB)& PWR_FLAG_WU)
    {
        bkt_DR = HAL_RTCEx_BKUPRead(&RTC_Handler,RTC_BKP_DR0);
        return bkt_DR;
    }
    else
        return CN_SLEEP_NORMAL;
}

//----�������߼���-------------------------------------------------------------
//����: �Ѽ�����������߼��𱣴浽��Ӧ��Ӳ����,ע��,ֻ��L3��L4��Ҫ.
//����: SleepLevel, ��������ĵ͹��ļ���
//����true=�ɹ�,false=ʧ��
//-----------------------------------------------------------------------------
bool_t __LP_BSP_SaveSleepLevel(u32 SleepLevel)
{
    RTC_HandleTypeDef RTC_Handler;  //RTC���
    RTC_Handler.Instance=RTC;

    if((SleepLevel!= CN_SLEEP_L3) && (SleepLevel!= CN_SLEEP_L4))
        return false;
    HAL_RTCEx_BKUPWrite(&RTC_Handler,RTC_BKP_DR0,SleepLevel);
    return true;

}

//----����L0���͹���-----------------------------------------------------------
//����: ����L0���͹���״̬,������lowpower.h������,��lowpower.c�ļ�����
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void __LP_BSP_EntrySleepL0(void)
{
	//Sleep
//    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFE);
//	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

//----����L1���͹���-----------------------------------------------------------
//����: ����L1���͹���״̬,������lowpower.h������,��lowpower.c�ļ�����.��stm32��
//      L0��L1��ʵ��һ����,���û��Ļص��������ܲ�һ��.
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void __LP_BSP_EntrySleepL1(void)
{
	//Mode : Run --> lprun--->lpsleep
//	HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
	HAL_PWREx_EnterSTOP1Mode(PWR_SLEEPENTRY_WFI);
}

//----����L2���͹���-----------------------------------------------------------
//����: ����L2���͹���״̬,������lowpower.h������,��lowpower.c�ļ�����
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void __LP_BSP_EntrySleepL2(void)
{
	//stop2 mode : exit this mode by exti_line int or wakeup
	HAL_PWREx_EnterSTOP2Mode(PWR_SLEEPENTRY_WFI);
}

//----����L3���͹���-----------------------------------------------------------
//����: ����L3���͹���״̬,������lowpower.h������,��lowpower.c�ļ�����
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void __LP_BSP_EntrySleepL3(void)
{
	//STOP
//    HAL_PWR_EnterSTANDBYMode();
}

//----����L4���͹���-----------------------------------------------------------
//����: ����L4���͹���״̬,������lowpower.h������,��lowpower.c�ļ�����,stm32��,
//      L3��L4��cpudrv����,��һ�µ�.
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void __LP_BSP_EntrySleepL4(void)
{
	//ShutDown/Standby
//    HAL_PWR_EnterSTANDBYMode( );
//	HAL_PWREx_EnterSHUTDOWNMode();
}

