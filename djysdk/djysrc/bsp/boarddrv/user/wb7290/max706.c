// =============================================================================
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 文件名     ：brdwdt.c
// 模块描述: SR3200的看门狗，包括初始化和喂狗函数
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 09/04.2015
// =============================================================================
#include "stdint.h"
#include "stddef.h"
#include "djyos.h"
#include "cpu_peri.h"
#include "int.h"

#include <shell.h>
#include <sys/time.h>

// BOARD WDT
// =============================================================================
static const Pin wdt_pin[] = {
		{PIO_PA26, PIOA, ID_PIOA, PIO_OUTPUT_1,    PIO_DEFAULT}
};
#define WDT_FEED_CYCLE  (300*mS) //0.3s
static u32 sBootDogFeedTime = 0;
// =============================================================================
// 功能：板上看门狗喂狗函数,max706的喂狗时间是1.6s,根据手册，喂狗是边沿触发
// 参数：无
// 返回：无
// =============================================================================

static u32 gTimeUsedMax = 0;
static u32 gTimeUsedMin = 0;
static s64 gFeedTimeBak = 0;
bool_t BrdWdt_FeedDog(void)
{
    s64  timenow = 0;
    if( PIO_Get(wdt_pin) )
    {
        PIO_Clear(wdt_pin);
    }
    else
    {
        PIO_Set(wdt_pin);
    }

    timenow = DjyGetSysTime();
    int timeused = 0;
    timeused = timenow - gFeedTimeBak;
    gFeedTimeBak = timenow;

    if((gTimeUsedMax == 0)||(timeused > gTimeUsedMax))
    {
    	gTimeUsedMax = timeused;
    }
    if((gTimeUsedMin == 0)||(timeused < gTimeUsedMin))
    {
    	gTimeUsedMin = timeused;
    }
    return true;
}

static bool_t wdttime(char *param)
{

	printf("WDT:CYCLE:%d ms MAX:%d.%d ms Min:%d.%d ms  lastime:%llx\n\r",\
			WDT_FEED_CYCLE/mS,gTimeUsedMax/1000,gTimeUsedMax%1000,gTimeUsedMin/1000,gTimeUsedMin%1000,gFeedTimeBak);
	return true;
}

static bool_t wdtstop(char *param)
{
	vu32 value = 0;
	Int_SaveAsynSignal();
	while(1)
	{
		value++;
	}
	Int_RestoreAsynSignal();
	return true;
}


bool_t BrdBoot_FeedEnd(void);
u32 RttFeedDog_Isr(ptu32_t intline)
{
	static u32 cnt = 0;

	if(RTT->RTT_SR & RTT_SR_RTTINC)
	{
		BrdWdt_FeedDog();
//		if(cnt * 1000*1000 > sBootDogFeedTime)
//		{
//			BrdBoot_FeedEnd();
//		}
//		else
//		{
//			cnt ++;
//		}
	}
	Int_ClearLine(intline);
	return 1;
}
bool_t BrdBoot_FeedStart(u32 bootfeedtime)
{
	u8 irqline = CN_INT_LINE_RTT;

	sBootDogFeedTime = bootfeedtime;

	PMC_EnablePeripheral(ID_RTT);
	RTT->RTT_MR = (9000 |  RTT_MR_RTTRST | RTT_MR_RTTINCIEN);

    Int_Register(irqline);
    Int_IsrConnect(irqline,RttFeedDog_Isr);
    Int_SettoReal(irqline);
    Int_ClearLine(irqline);
    Int_RestoreRealLine(irqline);
	return true;
}

bool_t BrdBoot_FeedEnd(void)
{
	u8 irqline = CN_INT_LINE_RTT;


	BrdWdt_FeedDog();
	RTT->RTT_MR = RTT_MR_RTTDIS;

	Int_SaveRealLine(irqline);
    Int_IsrDisConnect(irqline);
    Int_UnRegister(irqline);
    PMC_DisablePeripheral(ID_RTT);
	return true;
}




struct ShellCmdTab  gMax706Shell[] =
{
    {
        "wdttime",
		wdttime,
        "usage:wdttime",
        "usage:wdttime",
    },
    {
        "wdtstop",
		wdtstop,
        "usage:wdtstop",
        "usage:wdtstop",
    },
};

#define CN_MAX706SHELL_NUM  ((sizeof(gMax706Shell))/(sizeof(struct ShellCmdTab)))
static struct ShellCmdRsc gMax706CmdRsc[CN_MAX706SHELL_NUM];

// =============================================================================
// 功能：板上看门狗芯片初始化
// 参数：无
// 返回：无
// =============================================================================
void ModuleInstall_BrdWdt(void)
{

	Sh_InstallCmd(gMax706Shell,gMax706CmdRsc,CN_MAX706SHELL_NUM);
	PIO_Configure(wdt_pin, PIO_LISTSIZE(wdt_pin));

    BrdWdt_FeedDog();

    WdtHal_RegisterWdtChip("Wdt_MAX706",WDT_FEED_CYCLE,BrdWdt_FeedDog,
    		BrdBoot_FeedStart,BrdBoot_FeedEnd);
}







