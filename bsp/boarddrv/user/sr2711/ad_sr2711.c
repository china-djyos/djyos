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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 文件名     ：ad_sr2711.c
// 模块描述: sr2711板板上AD两路AD分别采样温度及电源电压.
// 模块版本: V1.00
// 创建人员: zhb
// 创建时间: 7/23.2017
// =============================================================================
#include <stdint.h>
#include <stdio.h>
#include <os.h>
#include <cpu_peri.h>
#include <stm32f7xx_hal_adc.h>
#include "powerctrl.h"
#include "ad_sr2711.h"


extern void Peri_LowPowerCfg(enum BoardPeriCode PeriIdx,bool_t Flag);

ADC_HandleTypeDef g_hadc;
void ADC_Init(void)
{
	 GPIO_InitTypeDef GPIO_InitStruct;
	 ADC_ChannelConfTypeDef sConfig;
	__HAL_RCC_ADC1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_3|GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    //	 下面进行ADC1的初始化
	g_hadc.Instance = ADC1;
	g_hadc.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV8; //二分频
	g_hadc.Init.Resolution = ADC_RESOLUTION_12B;                     //12位精度
	g_hadc.Init.ScanConvMode = DISABLE;
	g_hadc.Init.ContinuousConvMode = DISABLE;
	g_hadc.Init.DiscontinuousConvMode = DISABLE;
	g_hadc.Init.NbrOfDiscConversion   = 0;
	g_hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	g_hadc.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
	g_hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	g_hadc.Init.NbrOfConversion = 1;
	g_hadc.Init.DMAContinuousRequests = DISABLE;
	g_hadc.Init.EOCSelection = DISABLE;
    if (HAL_ADC_Init(&g_hadc) != HAL_OK)
    {
        printf("ADC Init failed.\r\n");
    }
    sConfig.Channel = ADC_CHANNEL_0;                                      //通道6
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
    if (HAL_ADC_ConfigChannel(&g_hadc,&sConfig)!=HAL_OK)
    {
        printf("ADC Config Channel failed.\r\n");
    }

}

void __ConfigCh(uint8_t idx)
{
	ADC_ChannelConfTypeDef sConfig;
	if(idx==0)
	{
		sConfig.Channel = ADC_CHANNEL_0;
	}
	else if(idx==1)
	{
		sConfig.Channel = ADC_CHANNEL_3;
	}
	else
	{
		sConfig.Channel = ADC_CHANNEL_4;
	}
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
	if (HAL_ADC_ConfigChannel(&g_hadc,&sConfig)!=HAL_OK)
	{
		 printf("ADC Config Channel failed.\r\n");
	}
}



#define CN_VOLT_CNT    20
static uint32_t Volt[CN_VOLT_CNT];

static void Delete_Max_Min(u32 *Buf)
{
	u8 idx1,idx2,idx3,idx4;
	u32 max = 0,min=0xffffffff;
	u32 i,t,j;
	for(j=0;j<(CN_VOLT_CNT-1);j++)
	{
		for(i=0;i<((CN_VOLT_CNT-1))-j;i++)
		{
			if(Buf[i]>Buf[i+1])
			{
				t=Buf[i];
				Buf[i]=Buf[i+1];
				Buf[i+1]=t;
			}
		}
	}
	for(i=(CN_VOLT_CNT-1); i>=CN_VOLT_CNT-5; i--)
	{
		Buf[i] = 0;
	}
	for(i=0; i<5; i++)
	{
		Buf[i] = 0;
	}
}


uint32_t ADC_GetBatVolt(void)
{
	 uint32_t i=0,q=0;
	 uint32_t sum=0;
     uint32_t coe=0;
	 uint32_t volt=0,val=0;
	 float temp=0.0;
	 Peri_LowPowerCfg(EN_PERI_BATTERY_SAMPLE,true);
	 Djy_EventDelay(50*mS);
	 __ConfigCh(0);
	 for(i=0;i<CN_VOLT_CNT;i++)
	 {
		 HAL_ADC_Start(&g_hadc);
		 Djy_DelayUs(100);
		 Volt[i] = HAL_ADC_GetValue(&g_hadc);
	     HAL_ADC_Stop(&g_hadc);
	 }
	 Delete_Max_Min(Volt);
	 for(i=0;i<CN_VOLT_CNT;i++)
	 {
		 sum+=Volt[i];
		 if(Volt[i] != 0)
		 {
			 q++;
		 }

	 }
	 if(q!=0)
	 {
		 val=sum/q;
		 coe=140*val/2296;   //补偿系数
		 temp=3300*val/4095;
		 volt=(uint32_t)(1200*temp/185)+coe;
	 }
	 Peri_LowPowerCfg(EN_PERI_BATTERY_SAMPLE,false);
	 return volt;
}

// =============================================================================
// 函数功能：AD_GetVolt
//        根据采样的数字值转换电压值
// 输入参数：AD采样的12位的数字值
// 输出参数：无
// 返回值  ：电源电压
// =============================================================================
uint32_t AD_GetVDD12Volt(void)
{
   uint32_t volt=0,val=0;
   RCC->APB2ENR |=(1<<8);
   __ConfigCh(1);
   HAL_ADC_Start(&g_hadc);
   Djy_DelayUs(100);
   val = HAL_ADC_GetValue(&g_hadc);
   HAL_ADC_Stop(&g_hadc);
   volt=3300*(val+1)/4096;
   volt=volt*4;
   RCC->APB2ENR &=~(1<<8);
   return volt;
}

uint32_t  ADC_GetVDD12CheckVolt(void)
{
	 uint32_t volt=0,val=0;
	 RCC->APB2ENR |=(1<<8);
	 __ConfigCh(2);
	 HAL_ADC_Start(&g_hadc);
	 Djy_DelayUs(100);
	 val = HAL_ADC_GetValue(&g_hadc);
	 HAL_ADC_Stop(&g_hadc);
	 volt=3300*(val+1)/4096;
	 volt=volt*4;
	 RCC->APB2ENR &=~(1<<8);
	 return volt;
}


void ADC_LowPowerCfg(bool_t Flag)
{
	if(Flag==InLowPower)
	{
		__HAL_RCC_ADC1_CLK_DISABLE();
	}
	else
	{
		ADC_Init();
	}
}


