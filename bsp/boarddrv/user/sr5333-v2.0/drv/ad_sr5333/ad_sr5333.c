//----------------------------------------------------
// Copyright (c) 2018,Open source team. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ�ɶ����߲���ϵͳ��Դ�Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
// �ļ���     ��ad_sr5333.c
// ģ������: sr5333�����AD��·AD�ֱ�����¶ȼ���Դ��ѹ.
// ģ��汾: V1.00
// ������Ա: zhb
// ����ʱ��: 7/23.2017
// =============================================================================
#include "stdint.h"
#include "stdio.h"
#include <os.h>
#include <cpu_peri.h>
#include <stm32f7xx_hal_adc.h>
#include <stm32f7xx_hal_dma.h>
#include "ad_sr5333.h"

#define R_LEVEL_0      16517
#define R_LEVEL_1      12698
#define R_LEVEL_2      9857
#define R_LEVEL_3      7721
#define R_LEVEL_4      6100
#define R_LEVEL_5      4858
#define R_LEVEL_6      3899
#define R_LEVEL_7      3151
#define R_LEVEL_8      2564
#define R_LEVEL_9      2099
#define R_LEVEL_10      1730
#define R_LEVEL_11      1433
#define R_LEVEL_12      1194
#define R_LEVEL_13      1000
#define R_LEVEL_14      841
#define R_LEVEL_15      711
#define R_LEVEL_16      604
#define R_LEVEL_17      515
#define R_LEVEL_18      441
#define R_LEVEL_19      379
#define R_LEVEL_20      327
#define R_LEVEL_21      283
#define R_LEVEL_22      246
#define R_LEVEL_23      214
#define R_LEVEL_24      187
#define R_LEVEL_25      164

ADC_HandleTypeDef g_hadc;

void ADC_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStruct;
     ADC_ChannelConfTypeDef sConfig;
    __HAL_RCC_ADC3_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
    //   �������ADC3�ĳ�ʼ��
    g_hadc.Instance = ADC3;
    g_hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2; //����Ƶ
    g_hadc.Init.Resolution = ADC_RESOLUTION_12B;                     //12λ����
    g_hadc.Init.ScanConvMode = DISABLE;
    g_hadc.Init.ContinuousConvMode = DISABLE;
    g_hadc.Init.DiscontinuousConvMode = DISABLE;
    g_hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    g_hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    g_hadc.Init.NbrOfConversion = 1;
    g_hadc.Init.DMAContinuousRequests = DISABLE;
    g_hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    if (HAL_ADC_Init(&g_hadc) != HAL_OK)
    {
        printf("ADC Init failed.\r\n");
    }
     sConfig.Channel = ADC_CHANNEL_14;                                      //ͨ��6
     sConfig.Rank = 1;
     sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
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
        sConfig.Channel = ADC_CHANNEL_9;
    }
    else
    {
        sConfig.Channel = ADC_CHANNEL_14;
    }
     sConfig.Rank = 1;
     sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
     if (HAL_ADC_ConfigChannel(&g_hadc,&sConfig)!=HAL_OK)
     {
         printf("ADC Config Channel failed.\r\n");
     }
}

// =============================================================================
// �������ܣ�AD_GetTemp
//        ���ݲ���������ֵת���¶�ֵ
// ���������AD������12λ������ֵ
// �����������
// ����ֵ  ��true ע��ɹ�  falseע��ʧ��
// ˵�����¶�����ֵ��Ӧ��ϵ���£�
//R: 16.517  12.698   9.857   7.7212   6.1005   4.8587  3.899  3.1513  2.5642  2.0999
//T: -40     -35      -30     -25      -20      -15     -10    -5      0       5
//R: 1.7300  1.4335   1.1942   1       0.84148  0.71139  0.60407 0.5151 0.441  0.379
//T: 10      15       20       25      30       35       40      45     50     55
//R:0.3269  0.28295   0.24572  0.21408 0.18708  0.16396
//T:60       65       70       75      80       85
// =============================================================================
sint8_t AD_GetTemp(void)
{
   uint32_t R=0;
   sint8_t val;
   uint32_t adval=0;
   __ConfigCh(0);
   HAL_ADC_Start(&g_hadc);
   Djy_DelayUs(2);
   adval = HAL_ADC_GetValue(&g_hadc);
   HAL_ADC_Stop(&g_hadc);
   if(adval!=0)
   {
       R=(4096000/adval)-1000;
//     printf("R AD:%d....R:%d.\r\n",adval,R);
       if(R>R_LEVEL_1)
       {
           val=-35-(5*(R-R_LEVEL_1))/(R_LEVEL_0-R_LEVEL_1);
       }
       else if(R>R_LEVEL_2)
       {
           val=-30-(5*(R-R_LEVEL_2))/(R_LEVEL_1-R_LEVEL_2);
       }
       else if(R>R_LEVEL_3)
       {
           val=-25-(5*(R-R_LEVEL_3))/(R_LEVEL_2-R_LEVEL_3);
       }
       else if(R>R_LEVEL_4)
       {
           val=-20-(5*(R-R_LEVEL_4))/(R_LEVEL_3-R_LEVEL_4);
       }
       else if(R>R_LEVEL_5)
       {
           val=-15-(5*(R-R_LEVEL_5))/(R_LEVEL_4-R_LEVEL_5);
       }
       else if(R>R_LEVEL_6)
       {
           val=-10-(5*(R-R_LEVEL_6))/(R_LEVEL_5-R_LEVEL_6);
       }
       else if(R>R_LEVEL_7)
       {
           val=-5-(5*(R-R_LEVEL_7))/(R_LEVEL_6-R_LEVEL_7);
       }
       else if(R>R_LEVEL_8)
       {
           val=-(5*(R-R_LEVEL_8))/(R_LEVEL_7-R_LEVEL_8);
       }
       else if(R>R_LEVEL_9)
       {
           val=5-(5*(R-R_LEVEL_9))/(R_LEVEL_8-R_LEVEL_9);
       }
       else if(R>R_LEVEL_10)
       {
           val=10-(5*(R-R_LEVEL_10))/(R_LEVEL_9-R_LEVEL_10);
       }
       else if(R>R_LEVEL_11)
       {
           val=15-(5*(R-R_LEVEL_11))/(R_LEVEL_10-R_LEVEL_11);
       }
       else if(R>R_LEVEL_12)
       {
           val=20-(5*(R-R_LEVEL_12))/(R_LEVEL_11-R_LEVEL_12);
       }
       else if(R>R_LEVEL_13)
       {
           val=25-(5*(R-R_LEVEL_13))/(R_LEVEL_12-R_LEVEL_13);
       }
       else if(R>R_LEVEL_14)
       {
           val=30-(5*(R-R_LEVEL_14))/(R_LEVEL_13-R_LEVEL_14);
       }
       else if(R>R_LEVEL_15)
       {
           val=35-(5*(R-R_LEVEL_15))/(R_LEVEL_14-R_LEVEL_15);
       }
       else if(R>R_LEVEL_16)
       {
           val=40-(5*(R-R_LEVEL_16))/(R_LEVEL_15-R_LEVEL_16);
       }
       else if(R>R_LEVEL_17)
       {
           val=45-(5*(R-R_LEVEL_17))/(R_LEVEL_16-R_LEVEL_17);
       }

       else if(R>R_LEVEL_18)
       {
           val=50-(5*(R-R_LEVEL_18))/(R_LEVEL_17-R_LEVEL_18);
       }
       else if(R>R_LEVEL_19)
       {
           val=55-(5*(R-R_LEVEL_19))/(R_LEVEL_18-R_LEVEL_19);
       }
       else if(R>R_LEVEL_20)
       {
           val=60-(5*(R-R_LEVEL_20))/(R_LEVEL_19-R_LEVEL_20);
       }

       else if(R>R_LEVEL_21)
       {
           val=65-(5*(R-R_LEVEL_21))/(R_LEVEL_20-R_LEVEL_21);
       }
       else if(R>R_LEVEL_22)
       {
           val=70-(5*(R-R_LEVEL_22))/(R_LEVEL_21-R_LEVEL_22);
       }
       else if(R>R_LEVEL_23)
       {
           val=75-(5*(R-R_LEVEL_23))/(R_LEVEL_22-R_LEVEL_23);
       }
       else if(R>R_LEVEL_24)
       {
           val=80-(5*(R-R_LEVEL_24))/(R_LEVEL_23-R_LEVEL_24);
       }
       else
       {
           val=85-(5*(R-R_LEVEL_25))/(R_LEVEL_24-R_LEVEL_25);
       }
//     printf("Temp:%d.\r\n",val);
   }
   __ConfigCh(1);
   return val;
}


// =============================================================================
// �������ܣ�AD_GetVolt
//        ���ݲ���������ֵת����ѹֵ
// ���������AD������12λ������ֵ
// �����������
// ����ֵ  ����Դ��ѹ
// =============================================================================
uint32_t AD_GetVolt(void)
{
   uint32_t volt=0,val=0;
//   __ConfigCh(1);
   HAL_ADC_Start(&g_hadc);
   Djy_DelayUs(2);
   val = HAL_ADC_GetValue(&g_hadc);
//   printf("volt ad:%d.\r\n",val);
   HAL_ADC_Stop(&g_hadc);
   volt=3300*(val+1)/4096;
   volt=volt*4;
//   printf("12V check val:%d.\r\n",volt);
   return volt;
}









