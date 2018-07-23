// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��AD_SR1266.c
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

#define CN_AD_CHANNEL_NUM	4
// =============================================================================
DMA_HandleTypeDef g_hdma;
ADC_HandleTypeDef g_hadc;
LPTIM_HandleTypeDef g_hlptim2;
// =============================================================================
// ���ܣ�ADC������DMA��ʼ��������DMAͨ�������ò�ѯ�ķ�ʽ��������DMA�ж�
// ��������
// ���أ���
// =============================================================================
static HAL_StatusTypeDef ADC_DmaInit(DMA_HandleTypeDef *hdma)
{
	u8 DmaIntLine;

	DmaIntLine = CN_INT_LINE_DMA1_CH1;
	__HAL_RCC_DMA1_CLK_ENABLE();

	hdma->Instance = DMA1_Channel1;						//ADC1(DMA1,Req0)
	hdma->Init.Request = DMA_REQUEST_0;
	hdma->Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma->Init.PeriphInc = DMA_PINC_DISABLE;
	hdma->Init.MemInc = DMA_MINC_ENABLE;
	hdma->Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma->Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma->Init.Mode = DMA_NORMAL;
	hdma->Init.Priority = DMA_PRIORITY_HIGH;
	hdma->Parent = (void*)&g_hadc;

	if(HAL_DMA_DeInit(hdma) == HAL_OK)
	{
		if(HAL_DMA_Init(hdma) == HAL_OK)
		{
			Int_Register(DmaIntLine);
			Int_SetClearType(DmaIntLine,CN_INT_CLEAR_AUTO);
			Int_IsrConnect(DmaIntLine,(u32 (*)(ptu32_t))HAL_DMA_IRQHandler);
			Int_SetIsrPara(DmaIntLine,(ptu32_t)hdma);
			Int_SettoAsynSignal(DmaIntLine);
			Int_ClearLine(DmaIntLine);
			Int_RestoreAsynLine(DmaIntLine);
			return HAL_OK;
		}
	}
	return HAL_ERROR;
}
// =============================================================================
// ���ܣ�ADC������ʼ����SR1263����ʹ��һ��START�ɼ�4��ͨ����
// ������mode,Ϊ1ʱ��ȡ��ģʽ��Ϊ0ʱ�����ģʽ
// ���أ���
// =============================================================================
bool_t AD_Init(void)
{
	ADC_ChannelConfTypeDef sConfig;

	__HAL_RCC_ADC_CLK_ENABLE();

	g_hadc.Instance = ADC1;
	g_hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
	g_hadc.Init.Resolution = ADC_RESOLUTION_12B;
	g_hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	g_hadc.Init.ContinuousConvMode = DISABLE;//ENABLE;
	g_hadc.Init.DMAContinuousRequests = DISABLE;
	g_hadc.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
	g_hadc.Init.NbrOfConversion = 4;
	g_hadc.Init.ScanConvMode = ADC_SCAN_ENABLE;
	g_hadc.Init.DiscontinuousConvMode = ENABLE;
	g_hadc.Init.NbrOfDiscConversion = 4;
	g_hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	g_hadc.Init.LowPowerAutoWait = ENABLE;
	g_hadc.Init.OversamplingMode = DISABLE;
	g_hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;//ADC_EXTERNALTRIG_EXT_IT11;//ADC_EXTERNALTRIG_T2_TRGO;
	g_hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;//ADC_EXTERNALTRIGCONVEDGE_FALLING;//ADC_EXTERNALTRIGCONVEDGE_RISING;
	g_hadc.DMA_Handle = &g_hdma;

	if(HAL_OK != ADC_DmaInit(&g_hdma))
	{
		printf("ADC_DmaInit failed  !\r\n");
		return false;
	}

	if(HAL_ADC_DeInit(&g_hadc))
	{
		printf("HAL_ADC_DeInit failed!\r\n");
		return false;
	}
	if(HAL_OK != HAL_ADC_Init(&g_hadc))
	{
		printf("HAL_ADC_Init failed!\r\n");
		return false;
	}

	sConfig.Channel = ADC_CHANNEL_5;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.Offset = 0;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
	if(HAL_OK != HAL_ADC_ConfigChannel(&g_hadc,&sConfig))
	{
		printf("ADC_CHANNEL_1 Config failed!\r\n");
		return false;
	}

	sConfig.Channel = ADC_CHANNEL_6;
	sConfig.Rank = ADC_REGULAR_RANK_2;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.Offset = 0;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
	if(HAL_OK != HAL_ADC_ConfigChannel(&g_hadc,&sConfig))
	{
		printf("ADC_CHANNEL_2 Config failed!\r\n");
		return false;
	}

	sConfig.Channel = ADC_CHANNEL_7;
	sConfig.Rank = ADC_REGULAR_RANK_3;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.Offset = 0;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
	if(HAL_OK != HAL_ADC_ConfigChannel(&g_hadc,&sConfig))
	{
		printf("ADC_CHANNEL_3 Config failed!\r\n");
		return false;
	}

	sConfig.Channel = ADC_CHANNEL_10;
	sConfig.Rank = ADC_REGULAR_RANK_4;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.Offset = 0;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
	if(HAL_OK != HAL_ADC_ConfigChannel(&g_hadc,&sConfig))
	{
		printf("ADC_CHANNEL_4 Config failed!\r\n");
		return false;
	}

	if(HAL_OK != HAL_ADCEx_Calibration_Start(&g_hadc, ADC_SINGLE_ENDED))
	{
		printf("ADC Calibration failed!\r\n");
		return false;
	}
	return true;
}

// =============================================================================
// ���ܣ���ȡADC����ֵ,�˴�������CN_AD_DMA_BUF_NUMʱ���ڣ��ض�Ҫ��ȡADֵ����DMA���䲻��
//       �������
// ������buf,AD���ݻ�����
//      len,��������Ҫ��ȡ����������WORD��
// ���أ�ʵ�ʶ�ȡ����������WORD��
// =============================================================================
bool_t AD_ValueRead(u16 *buf)
{
	if(NULL != buf)
	{
		HAL_ADC_Start_DMA(&g_hadc,(u32*)buf,CN_AD_CHANNEL_NUM);
		while(g_hdma.Instance->CNDTR)
		{

		}
		return true;
	}
	return false;
}

