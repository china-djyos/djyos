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
#include <stm32f7xx_hal_rcc.h>
#include <stm32f7xx_hal_rtc.h>
#include <stm32f7xx_hal_rtc.h>
#include <stm32f7xx_hal_rtc_ex.h>
#include <stm32f7xx_hal_pwr.h>


// =============================================================================


extern void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);

// =============================================================================



// =============================================================================
// ���ܣ�����͹���ǰ������ϵͳ����øú������������׼������
// ������
// ���أ�
// =============================================================================
__attribute__((weak)) void EntrySleepReCall(u32 SleepLevel)
{
	switch(SleepLevel)
	{
	case CN_SLEEP_L0:		//SLEEP
		break;
	case CN_SLEEP_L1:		//LP SLEEP
		break;
	case CN_SLEEP_L2:		//STOP
		break;
	case CN_SLEEP_L3:
		break;
	case CN_SLEEP_L4:
		break;
	default:
		break;
	}
	return ;
}

// =============================================================================
// ���ܣ��˳��͹��ĺ󣬲���ϵͳ����øú���
// ������
// ���أ�
// =============================================================================
__attribute__((weak)) void ExitSleepReCall(u32 SleepLevel)
{
	switch(SleepLevel)
	{
	case CN_SLEEP_L0:
		break;
	case CN_SLEEP_L1:
		break;
	case CN_SLEEP_L2:		//back to run mode
		break;
	case CN_SLEEP_L3:
		break;
	case CN_SLEEP_L4:
		break;
	default:
		break;
	}
	return ;
}

ptu32_t EXTI_WakeUpIrq(u8 irqline)
{
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_13) != RESET)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);
	}
	Int_ClearLine(CN_INT_LINE_EXTI15_10);
	return 1;
}

// =============================================================================
// ���ܣ�����͹������ã�
//      1��GPIOû�õ��ģ�������Ϊ����ģ������
//      2���͹���ģʽ�£��رճ�LPUART1��LPTIM1��ʱ�ӣ�
//      3�����ý���ĵ͹��ļ���ΪSTOP2
// ������
// ���أ�
// =============================================================================
void SR2711_LowPowerCfg(void)
{
	LP_SetSleepLevel(CN_SLEEP_L1);

}
