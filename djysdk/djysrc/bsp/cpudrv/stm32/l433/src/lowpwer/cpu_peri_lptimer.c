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
// �����Ȩ�����ʹ���߷������¶������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�����������У���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�����������У��Լ�����
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
// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��cpu_peri_lptimer.c
// ģ������: �͹��Ķ�ʱ���ײ�Ӳ������ģ��
// ģ��汾:V1.0.0
// ������Ա:chj
// ����ʱ��:2018/6/14
// =============================================================================
#include "cpu_peri_lptimer.h"
#include "cpu_peri_int_line.h"
#include "stm32l433xx.h"
#include "int.h"
#include "board-config.h"

static u16 g_lptimer_reload = 0;
static u16 g_lptimer_pre_cnt = 0;
void Lptimer1_ClearAllInt(void)
{
	LPTIM1->ICR = 0x7f;
}

bool_t Lptimer1_GetIntFlag(void)
{
	return ((LPTIM1->ISR&LPTIM_ISR_CMPM) ? true : false);
}

u8 Lptimer1_ClearISR(void)
{
	u8 temp = CN_LPTIMER_NONE;
	if(LPTIM1->ISR & LPTIM_ISR_CMPM)
	{
		if(LPTIM1->ISR & LPTIM_ISR_ARRM)
			temp =  CN_LPTIMER_RELOAD_AND_CMP;
		else
			temp =  CN_LPTIMER_CMP;
	}
	else
	{
		if(LPTIM1->ISR & LPTIM_ISR_ARRM)
			temp =  CN_LPTIMER_RELOAD;
		else
			temp =  CN_LPTIMER_NONE;
	}
	Lptimer1_ClearAllInt();
	return temp;
}

void Lptimer1_set_period(u16 period)
{
	while(LPTIM1->CNT==g_lptimer_pre_cnt);
	g_lptimer_pre_cnt = LPTIM1->CNT;
	LPTIM1->CMP = (period + g_lptimer_pre_cnt > CN_LIMIT_UINT16) ? \
			(period + g_lptimer_pre_cnt - CN_LIMIT_UINT16) : (period + g_lptimer_pre_cnt);
	g_lptimer_reload = period;
}

u16 Lptimer1_read_reload(void)
{
	return g_lptimer_reload;
}

u16 Lptimer1_read_cnt(void)
{
	return LPTIM1->CNT;
}

void Lptimer1_PreInit(void)
{
	g_lptimer_reload = 0;
	g_lptimer_pre_cnt = 0;
}

void Lptimer1_Init(u16 period,void (*isr)(ptu32_t param))
{
	u32 presc=0;
	presc = 32768/CN_CFG_TIME_BASE_HZ;
	switch(presc)
	{
		case 1:
			presc = LPTIM_CFGR_PRESC;
			break;
		case 2:
			presc = LPTIM_CFGR_PRESC_0;
			break;
		case 4:
			presc = LPTIM_CFGR_PRESC_1;
			break;
		case 8:
			presc = (0x03 << (LPTIM_CFGR_PRESC_Pos));
			//presc = LPTIM_CFGR_PRESC_2;
			break;
		default:
			return;
	}

	RCC->APB1ENR1 = RCC_APB1ENR1_LPTIM1EN | RCC_APB1ENR1_PWREN;
//	RCC->CSR |= RCC_CSR_LSION; // ��LSI
//	while ((RCC->CSR & RCC_CSR_LSIRDY) == 0); // �ȴ�LSI�ȶ�
	RCC->CCIPR = RCC_CCIPR_LPTIM1SEL; // LPTIM1ʱ��ѡLSI

	LPTIM1->CFGR = presc; // 4��Ƶ
	LPTIM1->IER = LPTIM_IER_CMPMIE | LPTIM_IER_ARRMIE; // ����ʱ���ж�

	Int_Register(CN_INT_LINE_LPTIM1);
	Int_SetClearType(CN_INT_LINE_LPTIM1,CN_INT_CLEAR_AUTO);
	Int_IsrConnect(CN_INT_LINE_LPTIM1,isr);
	Int_SettoAsynSignal(CN_INT_LINE_LPTIM1);
	Int_ClearLine(CN_INT_LINE_LPTIM1);
	Int_RestoreAsynLine(CN_INT_LINE_LPTIM1);

	Lptimer1_ClearAllInt();
	LPTIM1->CR = LPTIM_CR_ENABLE; // ��LPTIM, ���ݲ���ʼ��ʱ
	LPTIM1->ARR = period;
	LPTIM1->CMP = period;
	g_lptimer_reload = period;
	g_lptimer_pre_cnt = 0;
	LPTIM1->CR |= LPTIM_CR_CNTSTRT;
	while(LPTIM1->CNT==0);
}
