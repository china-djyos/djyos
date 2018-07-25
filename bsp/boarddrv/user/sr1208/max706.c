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
#include "wdt_hal.h"


#define WDT_TIM    TIM12    //启动加载用到的定时器TIM12
// BOARD WDT
// =============================================================================
const Pin wdt_pin[] = {
{GPIO_H,PIN12,GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE,AF_NUll},
};

#define WDT_FEED_CYCLE  (1600*1000) //1.6S
static u32 sBootDogFeedTime = 0;
// =============================================================================
// 功能：板上看门狗喂狗函数,max706的喂狗时间是1.6s,根据手册，喂狗是边沿触发
// 参数：无
// 返回：无
// =============================================================================
bool_t BrdWdt_FeedDog(void)
{
    if( PIO_Get(wdt_pin) )
    {
        PIO_Clear(wdt_pin);
    }
    else
    {
        PIO_Set(wdt_pin);
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
	return true;
}

// =============================================================================
// 功能：板上看门狗芯片初始化
// 参数：无
// 返回：无
// =============================================================================
void ModuleInstall_BrdWdt(void)
{
//在initcpuc.c中初始化，由于usb检测耗时长，需要在usb检测过程中不停喂狗.by zhb20170314
	PIO_Configure(wdt_pin, PIO_LISTSIZE(wdt_pin));

    BrdWdt_FeedDog();

    WdtHal_RegisterWdtChip("Wdt_MAX706",WDT_FEED_CYCLE/4,BrdWdt_FeedDog,\
    		BrdBoot_FeedStart,BrdBoot_FeedEnd);
}







