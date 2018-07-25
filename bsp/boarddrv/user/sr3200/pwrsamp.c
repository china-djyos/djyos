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
// �ļ���     ��cpu-ad.c
// ģ������: SR3200��CPU�ɼ�����
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 1/11.2016
// =============================================================================

#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
#include "lpc17xx_adc.h"

#define ADC_CLK_RATE			200000   //10K�Ĳ�����
#define ACD_TIME_OUT 			1000	//��ʱʱ�䣬us

#define PWR10_CHANNEL			ADC_CHANNEL_2
#define P24V10_CHANNEL			ADC_CHANNEL_3
#define LOCK10_CHANNEL    		ADC_CHANNEL_4

extern void Board_AdcGpioInit(void);
// =============================================================================
// ���ܣ��弶��CPU��ADCģ���ʼ������Ӧ��ͨ�����ã�gpio��ʼ��
// ����: ��
// ���أ���
// =============================================================================
void PwrSamp_Init(void)
{
	Board_AdcGpioInit();

	ADC_Init( LPC_ADC, ADC_CLK_RATE);                	/*���� ADC��ʱ��*/
	ADC_ChannelCmd( LPC_ADC, PWR10_CHANNEL, ENABLE);    /* ʹ��ADCͨ��  */
	ADC_ChannelCmd( LPC_ADC, P24V10_CHANNEL, ENABLE);    /* ʹ��ADCͨ��  */
	ADC_ChannelCmd( LPC_ADC, LOCK10_CHANNEL, ENABLE);    /* ʹ��ADCͨ��  */

//	ADC_BurstCmd(LPC_ADC, ENABLE);
}

// =============================================================================
// ���ܣ���ȡCPU AD������ֵ������һ��AD����������ѯ�����Ƿ���ɣ������ɣ����ȡ����ֵ��
//      ���򷵻ز��ɹ������Խ�����Ϊ400us�����ܹ����
// ����: ���������û��ṩ�����ڴ洢��ȡ�����������飬ֻ��ָ����ͨ����������Ч
// ���أ�true,ADC�ɼ��ɹ���false������ʧ��
// =============================================================================
bool_t PwrSamp_GetValue(u32 *Buf)
{
	u32 TimeOut = 1000;
	FlagStatus Status = RESET;
	bool_t Result = false;

	if(NULL == Buf)
		return false;

	//���־λ
	ADC_ChannelGetStatus(LPC_ADC,PWR10_CHANNEL,ADC_DATA_BURST);
	ADC_ChannelGetStatus(LPC_ADC,P24V10_CHANNEL,ADC_DATA_BURST);
	ADC_ChannelGetStatus(LPC_ADC,LOCK10_CHANNEL,ADC_DATA_BURST);

	ADC_BurstCmd(LPC_ADC, ENABLE);

	//�ȴ����µ�һ��ת�����
	while(1)
	{
		if(LPC_ADC->ADSTAT & (1<<(LOCK10_CHANNEL + 8)))
			Status = SET;
		if( (Status == SET) || (TimeOut == 0))
			break;
		Djy_DelayUs(1);
		TimeOut -= 1;
	}

	//��������
	if(Status == SET)
	{
		Buf[PWR10_CHANNEL] = ADC_ChannelGetData(LPC_ADC,PWR10_CHANNEL);
		Buf[P24V10_CHANNEL] = ADC_ChannelGetData(LPC_ADC,P24V10_CHANNEL);
		Buf[LOCK10_CHANNEL] = ADC_ChannelGetData(LPC_ADC,LOCK10_CHANNEL);
		Result = true;
	}
	else
	{
		Result = false;
	}
	ADC_BurstCmd(LPC_ADC, DISABLE);
	return Result;
}






