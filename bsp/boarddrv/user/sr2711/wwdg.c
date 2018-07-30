// =============================================================================
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 文件名     ：brdwdt.c
// 模块描述: SR2711-V1.1.0的看门狗，包括初始化和喂狗函数
// 模块版本: V2.00
// 创建人员: zhb
// 创建时间: 09/30.2017
// 说明        ：使用MCU上片上独立看门狗
// =============================================================================
#include "stdint.h"
#include "stddef.h"
#include "djyos.h"
#include "cpu_peri.h"
#include "wdt_hal.h"
#include "shell.h"
#include "newshell.h"
//static bool_t Sh_WwdgCounter(char *param);
//ptu32_t Wwdg_Shell_Module_Install(void);

// BOARD WDT
// =============================================================================
static WWDG_HandleTypeDef   WwdgHandle;
#define WDT_FEED_CYCLE  (30*1000) //30ms

#define WDT_TIM    TIM12    //启动加载用到的定时器TIM12

static u32 sBootDogFeedTime = 0;
static u32 FeedCnt1=0;
static u64 FeedCnt2=0;
static bool_t bLoadEndFlag=false;




//static bool_t Sh_WwdgCounter(char *param)
ADD_TO_SHELL_HELP(wdgc,"看门狗喂狗次数");
ADD_TO_IN_SHELL static bool_t wdgc(char *param)
{
	uint32_t data[2];//used to print the s64 type
	memcpy(data,&FeedCnt2,sizeof(data));
	printf("BootLoader Period Counter:%d.\r\n",FeedCnt1);
	printf("Normal Period Counter:0x%08x 0x%08x \r\n",data[1],data[0]);
	return true;
}

//**************************************************************************
//struct ShellCmdTab const shell_cmd_wwdg_table[]=
//{
//	{"wdgc",(bool_t(*)(char*))Sh_WwdgCounter,    "看门狗喂狗次数",     NULL},
//};
////**************************************************************************
//#define CN_WWDG_SHELL_NUM  sizeof(shell_cmd_wwdg_table)/sizeof(struct ShellCmdTab)
//static struct ShellCmdRsc tg_wwdg_shell_cmd_rsc[CN_WWDG_SHELL_NUM];

/*******************************************************************************
功能:IWDG模块shell命令加载
参数:无.
返回值:1。
*********************************************************************************/
//ptu32_t Wwdg_Shell_Module_Install(void)
//{
//	Sh_InstallCmd(shell_cmd_wwdg_table,tg_wwdg_shell_cmd_rsc,CN_WWDG_SHELL_NUM);
//	return 1;
//}

// =============================================================================
// 功能：板上看门狗喂狗函数,max706的喂狗时间是1.6s,根据手册，喂狗是边沿触发
// 参数：无
// 返回：无
// =============================================================================
bool_t BrdWdt_FeedDog(void)
{
	/* Refresh IWDG: reload counter */
	if(HAL_WWDG_Refresh(&WwdgHandle) != HAL_OK)
	{
		return false;
	}
    return true;
}


bool_t BrdBoot_FeedEnd(void);
u32 FeedDog_Isr(ptu32_t intline)
{
	WDT_TIM->CNT = 0;
	WDT_TIM->SR = 0;//清中断标志
	BrdWdt_FeedDog();
	Int_ClearLine(intline);
	if(!bLoadEndFlag)
	{
		FeedCnt1++;
	}
	else
	{
		FeedCnt2++;
	}
	return 1;
}
bool_t BrdBoot_FeedStart(u32 bootfeedtime)
{
	u8 irqline = CN_INT_LINE_TIM8_BRK_TIM12;
	sBootDogFeedTime = bootfeedtime;
	RCC->APB1ENR |=RCC_APB1ENR_TIM12EN;
	WDT_TIM->CR1 &= ~(TIM_CR1_CEN); //禁止TIMER
	WDT_TIM->CR1 |= TIM_CR1_ARPE | TIM_CR1_DIR;//自动重装
	WDT_TIM->DIER |= TIM_DIER_UIE;//使能更新中断
	WDT_TIM->PSC = 4000-1;//分频系数 为零 不分频(1/108)1uS
	WDT_TIM->ARR = 27000;//定时器预装初值
    Int_Register(irqline);
    Int_IsrConnect(irqline,FeedDog_Isr);
    Int_SettoReal(irqline);
    Int_ClearLine(irqline);
    Int_RestoreRealLine(irqline);
    WDT_TIM->CR1 |= (TIM_CR1_CEN); //使能TIMER
	return true;
}

bool_t BrdBoot_FeedEnd(void)
{
	u8 irqline = CN_INT_LINE_TIM8_BRK_TIM12;
	BrdWdt_FeedDog();
	WDT_TIM->CR1 &=~(TIM_CR1_CEN); //禁止TIMER
	Int_SaveRealLine(irqline);
    Int_IsrDisConnect(irqline);
    Int_UnRegister(irqline);
    RCC->APB1ENR &=(~RCC_APB1ENR_TIM12EN);
    bLoadEndFlag=true;
	return true;
}


void WWDG_Init(void)
{
	/*##-1- Check if the system has resumed from WWDG reset ####################*/
	  if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST) != RESET)
	  {
		  /* Clear reset flags */
		     __HAL_RCC_CLEAR_RESET_FLAGS();
	  }

   /*##-2- Configure the WWDG peripheral ######################################*/
	 /* WWDG clock counter = (PCLK1 (54MHz)/4096)/8) = 1648 Hz (~607 us)
		WWDG Window value = 120 means that the WWDG counter should be refreshed only
		when the counter is below 120 (and greater than 64/0x40) otherwise a reset will
		be generated.
		WWDG Counter value = 127, WWDG timeout = ~607 us * 64 = 38.8 ms */
	 WwdgHandle.Instance = WWDG;

	 WwdgHandle.Init.Prescaler = WWDG_PRESCALER_8;
	 WwdgHandle.Init.Window    = 120;
	 WwdgHandle.Init.Counter   = 127;

	 WwdgHandle.Init.EWIMode = WWDG_EWI_DISABLE;

	if (HAL_WWDG_Init(&WwdgHandle) != HAL_OK)
	{
		printf("WWDG Init failed.\r\n");
	}
	printf("WWDG Init success.\r\n");
}

// =============================================================================
// 功能：板上看门狗芯片初始化
// 参数：无
// 返回：无
// =============================================================================
void ModuleInstall_BrdWdt(void)
{
//在initcpuc.c中初始化，由于usb检测耗时长，需要在usb检测过程中不停喂狗.by zhb20170314
	WWDG_Init();
    WdtHal_RegisterWdtChip("Wdt_IWDG",WDT_FEED_CYCLE,BrdWdt_FeedDog,\
    		BrdBoot_FeedStart,BrdBoot_FeedEnd);
//    Wwdg_Shell_Module_Install();
}



