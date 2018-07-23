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
//所属模块: SR2711板件模块电源控制
//作者:  zhb.
//版本：V1.0.0
//文件描述: 远程升级
//其他说明:
//修订历史:
//1. 日期: 2017-08-12
//   作者:  zhb.
//   新版本号: V1.0.0
//   修改说明: 原始版本
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
// 功能：比较匹配时的回调函数，它被用来切换取电到,可通过Start和Stop来启动和停止
// 参数：hlptim
// 返回：无
// =============================================================================
void HAL_LPTIM_CompareMatchCallback(LPTIM_HandleTypeDef *hlptim)
{
	Int_ClearLine(CN_INT_LINE_LPTIM1);
}


// =============================================================================
// 功能：自动重加载的回调函数，此处作为AD采样的定时时间，采样完切换到取电模式
// 参数：hlptim
// 返回：无
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
//功能: 进入L2级低功耗状态,函数在lowpower.h中声明,供lowpower.c文件调用
//参数: 无
//返回: 无
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
//功能: 进入L2级低功耗状态,函数在lowpower.h中声明,供lowpower.c文件调用
//参数: cycle,单位ms
//返回: 无
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
//功能: 进入L2级低功耗状态,函数在lowpower.h中声明,供lowpower.c文件调用
//参数: cycle,单位ms
//返回: 无
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
	{"lpstart",(bool_t(*)(char*))Sh_LpStart,    "使能低功耗定时器",             NULL},
	{"lpstat",(bool_t(*)(char*))Sh_LpStat,    "低功耗定时器中断计数",            NULL},
	{"lpstop",(bool_t(*)(char*))Sh_LpStop,    "禁能低功耗定时器",              NULL},
	{"lprst",(bool_t(*)(char*))Sh_LpRst,      "清零低功耗定时器中断计数",         NULL},
	{"lpcnt",(bool_t(*)(char*))Sh_LpReadCounter, "当前计数值及自动重复加载值",     NULL},

};
//**************************************************************************
#define CN_LP_TIM_SHELL_NUM  sizeof(shell_cmd_lptim_table)/sizeof(struct ShellCmdTab)
static struct ShellCmdRsc tg_lp_tim_shell_cmd_rsc[CN_LP_TIM_SHELL_NUM];

/*******************************************************************************
功能:CAN控制器操作shell模块
参数:无.
返回值:1。
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
	//提取1个参数
	extern char *Sh_GetWord(char *buf,char **next);
	word_Cycle = Sh_GetWord(param,&next_param);
	word_trail = Sh_GetWord(next_param,&next_param);
	if((word_Cycle == NULL)||(word_trail != NULL))
	{
	  printf("\r\n参数错误。\r\n");
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
