// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��lp_SR1266.c
// ģ������: SR1266���͹��ķ���Դ���뼰ʵ��
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 6/20.2017
// =============================================================================
#include "stdint.h"
#include "stdio.h"
#include <lowpower.h>
#include <os.h>
#include <cpu_peri.h>
#include <stm32l4xx_ll_rcc.h>
#include <stm32l4xx_ll_pwr.h>
// =============================================================================
#define __HAL_RCC_PERI_CLK_SLEEP_DISABLE()  {	RCC->AHB1SMENR  = 0x0;	\
											RCC->AHB2SMENR  = 0x0;\
											RCC->AHB3SMENR  = 0x0;\
											RCC->APB1SMENR1 = 0x0;\
											RCC->APB1SMENR2 = 0x0;\
											RCC->APB2SMENR  = 0x0;\
											}

// =============================================================================
// ���ܣ���GPIO�������ó�Ϊģ��������Խ��͹��ģ����ݽ���ĵ͹��ĵȼ���������GPIO��������ͬ
// ������
// ���أ�
// =============================================================================
void GPIO_LowPower(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_12;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_6|GPIO_PIN_9|GPIO_PIN_14|GPIO_PIN_15;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
}

// =============================================================================
// ���ܣ�����͹���ǰ������ϵͳ����øú������������׼������
// ������
// ���أ�
// =============================================================================
u32 EntrySleepReCall(u32 SleepLevel)
{
	switch(SleepLevel)
	{
	case CN_SLEEP_L0:		//SLEEP
		break;
	case CN_SLEEP_L1:		//LP SLEEP
		__HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_MSI);
		break;
	case CN_SLEEP_L2:		//STOP
		__HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_MSI);
		break;
	case CN_SLEEP_L3:
		break;
	case CN_SLEEP_L4:
		break;
	default:
		break;
	}
	return 0;
}
// =============================================================================
// ���ܣ��˳��͹��ĺ󣬲���ϵͳ����øú���
// ������
// ���أ�
// =============================================================================
u32 ExitSleepReCall(u32 SleepLevel)
{
	switch(SleepLevel)
	{
	case CN_SLEEP_L0:
		break;
	case CN_SLEEP_L1:
		__HAL_RCC_PWR_CLK_ENABLE();
		break;
	case CN_SLEEP_L2:		//back to run mode
		__HAL_RCC_PWR_CLK_ENABLE();
		break;
	case CN_SLEEP_L3:
		break;
	case CN_SLEEP_L4:
		break;
	default:
		break;
	}
	return 0;
}

// =============================================================================

bool_t __LP_BSP_RestoreRamL3(void)
{
    return true;
}
bool_t __LP_BSP_SaveRamL3(void)
{
   return true;
}

// =============================================================================
// ���ܣ�����͹������ã�
//      1��GPIOû�õ��ģ�������Ϊ����ģ������
//      2���͹���ģʽ�£��رճ�LPUART1��LPTIM1��ʱ�ӣ�
//      3�����ý���ĵ͹��ļ���ΪSTOP2
// ������
// ���أ�
// =============================================================================
void SR1266_LowPowerCfg(void)
{
	GPIO_LowPower();

	__HAL_RCC_PERI_CLK_SLEEP_DISABLE();	//��ֹSLEEP/STOPģʽ����������
	__HAL_RCC_LPTIM1_CLK_SLEEP_ENABLE();
	__HAL_RCC_LPUART1_CLK_SLEEP_ENABLE();

	LP_SetSleepLevel(CN_SLEEP_L2);
}




