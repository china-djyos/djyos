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
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��ad_sr2711.c
// ģ������: sr2711�����AD��·AD�ֱ�����¶ȼ���Դ��ѹ.
// ģ��汾: V1.00
// ������Ա: zhb
// ����ʱ��: 7/23.2017
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
    //	 �������ADC1�ĳ�ʼ��
	g_hadc.Instance = ADC1;
	g_hadc.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV8; //����Ƶ
	g_hadc.Init.Resolution = ADC_RESOLUTION_12B;                     //12λ����
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
    sConfig.Channel = ADC_CHANNEL_0;                                      //ͨ��6
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
		 coe=140*val/2296;   //����ϵ��
		 temp=3300*val/4095;
		 volt=(uint32_t)(1200*temp/185)+coe;
	 }
	 Peri_LowPowerCfg(EN_PERI_BATTERY_SAMPLE,false);
	 return volt;
}

// =============================================================================
// �������ܣ�AD_GetVolt
//        ���ݲ���������ֵת����ѹֵ
// ���������AD������12λ������ֵ
// �����������
// ����ֵ  ����Դ��ѹ
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


