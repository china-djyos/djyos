// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��PWM_SR1266.c
// ģ������: �����ز��ֳ�ʼ�������õ�
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 6/20.2017
// =============================================================================
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <os.h>
#include <cpu_peri.h>
#include <lowpower.h>


TIM_HandleTypeDef TimHandle;
static bool_t sPwmInit = false;
// =============================================================================
// ���ܣ�LPTIM2��ʱ����ʼ������ʱ���������ģʽ������̶�PWM����������ϣ��ò��δ���ADC
//		���������ԵĲ���
// ��������
// ���أ�true or false
// =============================================================================
bool_t PWM_Init(void)
{
	TIM_OC_InitTypeDef sConfig;
	__HAL_RCC_TIM2_CLK_ENABLE();

	TimHandle.Instance = TIM2;
	TimHandle.Init.Prescaler         = ((CN_CFG_MCLK) / 1000) - 1;	//1K
	TimHandle.Init.Period            = 20-1;
	TimHandle.Init.ClockDivision     = 0;
	TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
	TimHandle.Init.RepetitionCounter = 0;
	if (HAL_TIM_PWM_Init(&TimHandle) == HAL_OK)
	{
		sConfig.OCMode       = TIM_OCMODE_PWM1;
		sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
		sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
		sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
		sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;

		sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
		sConfig.Pulse = 14;
		if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_4) == HAL_OK)
		{
			sPwmInit = true;
			return true;
		}
	}
	return false;
}

// =============================================================================
// ���ܣ�����PWM��,�����ʱ��ص���PWU_Stop����,�������Իָ���STOP2
// ������cycle,PWM���ڣ���λΪus
//       duty��ռ�ձȣ�0~100
// ���أ���
// =============================================================================
void PWM_Start(void)
{
	if(sPwmInit)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();
		HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_4);
	}
}
// =============================================================================
// ���ܣ�����PWM��
// ������cycle,PWM���ڣ���λΪms
//       duty��ռ�ձȣ�0~100
// ���أ���
// =============================================================================
void PWM_Stop(void)
{
	if(sPwmInit)
	{
		HAL_TIM_PWM_Stop(&TimHandle, TIM_CHANNEL_4);
		__HAL_RCC_TIM2_CLK_DISABLE();
	}
}
