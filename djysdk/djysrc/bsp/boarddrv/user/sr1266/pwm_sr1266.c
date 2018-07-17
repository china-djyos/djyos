// =============================================================================
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 文件名     ：PWM_SR1266.c
// 模块描述: 板件相关部分初始化或配置等
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 6/20.2017
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
// 功能：LPTIM2定时器初始化，定时器采用输出模式，输出固定PWM到输出引脚上，该波形触发ADC
//		进行周期性的采样
// 参数：无
// 返回：true or false
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
// 功能：产生PWM波,请结束时务必调用PWU_Stop（）,那样可以恢复到STOP2
// 参数：cycle,PWM周期，单位为us
//       duty，占空比，0~100
// 返回：无
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
// 功能：结束PWM波
// 参数：cycle,PWM周期，单位为ms
//       duty，占空比，0~100
// 返回：无
// =============================================================================
void PWM_Stop(void)
{
	if(sPwmInit)
	{
		HAL_TIM_PWM_Stop(&TimHandle, TIM_CHANNEL_4);
		__HAL_RCC_TIM2_CLK_DISABLE();
	}
}
