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
//����ģ��: SR2711���ģ���Դ����
//����:  zhb.
//�汾��V1.0.0
//�ļ�����: Զ������
//����˵��:
//�޶���ʷ:
//1. ����: 2017-08-12
//   ����:  zhb.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "cpu_peri.h"
#include "board-config.h"
#include <stm32f7xx_hal_lptim.h>
#include <shell.h>

 LPTIM_HandleTypeDef             LptimHandle;
static RCC_PeriphCLKInitTypeDef        RCC_PeriphCLKInitStruct;

typedef void (*funTimer2_ISR)(void);
typedef u32 (*timer_isr)(u32 param);
funTimer2_ISR pTimer_ISR = NULL;
static timer_isr LP_TimerIsr;
static uint64_t gs_LpTimerCnt=0;


static bool_t Sh_LpStart(char *param);
static bool_t Sh_LpStop(char *param);
static bool_t Sh_LpStat(char *param);
static bool_t Sh_LpRst(char *param);
static bool_t Sh_LpReadCounter(char *param);
ptu32_t LP_TIM_Shell_Module_Install(void);
void Lptimer_PreInit(void)
{
	LPTIM1->ARR = 0;
	LPTIM1->CNT = 0;
}

bool_t Lptimer_GetIntFlag(void)
{
	if(LPTIM1->ISR&LPTIM_ISR_ARRM)
		return true;
	else
		return false;
}
// =============================================================================
// ���ܣ��Ƚ�ƥ��ʱ�Ļص����������������л�ȡ�絽,��ͨ��Start��Stop��������ֹͣ
// ������hlptim
// ���أ���
// =============================================================================
void HAL_LPTIM_CompareMatchCallback(LPTIM_HandleTypeDef *hlptim)
{
	Int_ClearLine(CN_INT_LINE_LPTIM1);
}


// =============================================================================
// ���ܣ��Զ��ؼ��صĻص��������˴���ΪAD�����Ķ�ʱʱ�䣬�������л���ȡ��ģʽ
// ������hlptim
// ���أ���
// =============================================================================
void HAL_LPTIM_AutoReloadMatchCallback(LPTIM_HandleTypeDef *hlptim)
{
	Int_ClearLine(CN_INT_LINE_LPTIM1);
	gs_LpTimerCnt+=LP_TimerIsr(0);
}


/**
  * @brief  Enable External Low Speed Clock (LSE)
  * @param  None
  * @retval None
  */
static void LSI_ClockEnable(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable LSI clock */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
      printf("LSI Enable failed.\r\n");
  }
}



/**
* @brief  LPTIM MSP Init
* @param  hlptim : LPTIM handle
* @retval None
*/
void HAL_LPTIM_MspInit(LPTIM_HandleTypeDef *hlptim)
{

  /* ## - 1 - Enable LPTIM clock ############################################ */
  __HAL_RCC_LPTIM1_CLK_ENABLE();

  /* ## - 2 - Force & Release the LPTIM Periheral Clock Reset ############### */
  /* Force the LPTIM Periheral Clock Reset */
  __HAL_RCC_LPTIM1_FORCE_RESET();

  /* Release the LPTIM Periheral Clock Reset */
  __HAL_RCC_LPTIM1_RELEASE_RESET();

}


//------------------------------------------------------------------------
//����: ����L2���͹���״̬,������lowpower.h������,��lowpower.c�ļ�����
//����: ��
//����: ��
//-------------------------------------------------------------------------
bool_t LP_TimerInit(u32 (*isr)(u32 param))
{
	LP_TIM_Shell_Module_Install();
	LSI_ClockEnable();
	 /* ### - 1 - Re-target the LSI to Clock the LPTIM Counter ################# */
	  /* Select the LSI clock as LPTIM peripheral clock */
	 RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1;
	 RCC_PeriphCLKInitStruct.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_LSI;
	 HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);

	  /* ### - 2 - Initialize LPTIM peripheral ################################## */
	    /*
	     *  Instance        = LPTIM1.
	     *  Clock Source    = APB or LowPowerOSCillator
	     *  Counter source  = Internal event.
	     *  Clock prescaler = 1 (No division).
	     *  Counter Trigger = Trigger1: PG.14
	     *  Active Edge     = Rising edge.
	     */

	LptimHandle.Instance = LPTIM1;
	LptimHandle.Init.Clock.Source       = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
	LptimHandle.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;
	LptimHandle.Init.Trigger.Source  = LPTIM_TRIGSOURCE_SOFTWARE;
	LptimHandle.Init.OutputPolarity  = LPTIM_OUTPUTPOLARITY_HIGH;
	LptimHandle.Init.UpdateMode      = LPTIM_UPDATE_IMMEDIATE;
	LptimHandle.Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL;


	/* Initialize LPTIM peripheral according to the passed parameters */
	if (HAL_LPTIM_Init(&LptimHandle) == HAL_OK)
	{
		Int_Register(CN_INT_LINE_LPTIM1);
		Int_SetClearType(CN_INT_LINE_LPTIM1,CN_INT_CLEAR_AUTO);
		Int_IsrConnect(CN_INT_LINE_LPTIM1,(u32 (*)(ptu32_t))HAL_LPTIM_IRQHandler);
		Int_SetIsrPara(CN_INT_LINE_LPTIM1,(ptu32_t)&LptimHandle);
		Int_SettoAsynSignal(CN_INT_LINE_LPTIM1);
		Int_ClearLine(CN_INT_LINE_LPTIM1);
		Int_RestoreAsynLine(CN_INT_LINE_LPTIM1);
		return true;
	}
	LP_TimerIsr = isr;
	printf("LPTIM init failed.\r\n");
	return false;


}



//------------------------------------------------------------------------
//����: ����L2���͹���״̬,������lowpower.h������,��lowpower.c�ļ�����
//����: cycle,��λms
//����: ��
//-------------------------------------------------------------------------
void LP_TimerStart(uint32_t cycle)
{
	if(cycle==0)
	{
		printf("%s para invaild.\r\n",__FUNCTION__);
		return;
	}
	__HAL_RCC_LPTIM1_CLK_ENABLE();
	if (HAL_LPTIM_Counter_Start_IT(&LptimHandle, cycle) != HAL_OK)
	{
		printf("LPTIM Counter Start failed.\r\n");
	}
	else
	{
		printf("LPTIM Counter Start OK.\r\n");
	}
}

//------------------------------------------------------------------------
//����: ����L2���͹���״̬,������lowpower.h������,��lowpower.c�ļ�����
//����: cycle,��λms
//����: ��
//-------------------------------------------------------------------------
void LP_TimerStop(void)
{
	HAL_LPTIM_Counter_Stop_IT(&LptimHandle);
	__HAL_RCC_LPTIM1_CLK_DISABLE();
}

u32 LP_TimerReadCnt(void)
{
	return HAL_LPTIM_ReadCounter(&LptimHandle);
}

u32 LP_TimerReadReload(void)
{
	return HAL_LPTIM_ReadAutoReload(&LptimHandle);
}

void LP_TimerSetReload(u16 period)
{
	LptimHandle.Instance->ARR = period;
}

//**************************************************************************
struct ShellCmdTab const shell_cmd_lptim_table[]=
{
	{"lpstart",(bool_t(*)(char*))Sh_LpStart,    "ʹ�ܵ͹��Ķ�ʱ��",             NULL},
	{"lpstat",(bool_t(*)(char*))Sh_LpStat,    "�͹��Ķ�ʱ���жϼ���",            NULL},
	{"lpstop",(bool_t(*)(char*))Sh_LpStop,    "���ܵ͹��Ķ�ʱ��",              NULL},
	{"lprst",(bool_t(*)(char*))Sh_LpRst,      "����͹��Ķ�ʱ���жϼ���",         NULL},
	{"lpcnt",(bool_t(*)(char*))Sh_LpReadCounter, "��ǰ����ֵ���Զ��ظ�����ֵ",     NULL},

};
//**************************************************************************
#define CN_LP_TIM_SHELL_NUM  sizeof(shell_cmd_lptim_table)/sizeof(struct ShellCmdTab)
static struct ShellCmdRsc tg_lp_tim_shell_cmd_rsc[CN_LP_TIM_SHELL_NUM];

/*******************************************************************************
����:CAN����������shellģ��
����:��.
����ֵ:1��
*********************************************************************************/
ptu32_t LP_TIM_Shell_Module_Install(void)
{
	Sh_InstallCmd(shell_cmd_lptim_table,tg_lp_tim_shell_cmd_rsc,CN_LP_TIM_SHELL_NUM);
	return 1;
}


//**************************************************************************
static bool_t Sh_LpStart(char *param)
{
	char *word_Cycle,*word_trail,*next_param;
	uint32_t byCycle;
	//��ȡ1������
	extern char *Sh_GetWord(char *buf,char **next);
	word_Cycle = Sh_GetWord(param,&next_param);
	word_trail = Sh_GetWord(next_param,&next_param);
	if((word_Cycle == NULL)||(word_trail != NULL))
	{
	  printf("\r\n��������\r\n");
	  return false;
	}
	byCycle = strtol(word_Cycle, (char **)NULL, 0);
	//LP_TimerStart(byCycle);
	return true;
}

static bool_t Sh_LpStop(char *param)
{
	//LP_TimerStop();
    printf("LP_Tim stop.\r\n");
    return true;
}

static bool_t Sh_LpStat(char *param)
{
	uint32_t data[2];//used to print the s64 type
	memcpy(data,&gs_LpTimerCnt,sizeof(data));
	printf("LP_Tim Int CNT:0x%08x 0x%08x.\r\n",data[1],data[0]);
    return true;
}

static bool_t Sh_LpRst(char *param)
{
	gs_LpTimerCnt=0;
    return true;
}

static bool_t Sh_LpReadCounter(char *param)
{
	uint32_t cnt,val;
	cnt=HAL_LPTIM_ReadCounter(&LptimHandle);
	val=HAL_LPTIM_ReadAutoReload(&LptimHandle);
	printf("Auto-reload value:%d,current counter:%d.\r\n",val,cnt);
	return true;
}
