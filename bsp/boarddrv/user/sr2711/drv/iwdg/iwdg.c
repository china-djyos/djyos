// =============================================================================
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 文件名     ：brdwdt.c
// 模块描述: SR2711-V1.1.0的看门狗，包括初始化和喂狗函数
// 模块版本: V2.00
// 创建人员: zhb
// 创建时间: 09/30.2017
// 说明        ：使用MCU上片上独立看门狗
// =============================================================================
#if 0
#include "stdint.h"
#include "stddef.h"
#include "djyos.h"
#include "cpu_peri.h"
#include "wdt_hal.h"
#include "shell.h"

static bool_t Sh_IwdgCounter(char *param);
ptu32_t Iwdg_Shell_Module_Install(void);

// BOARD WDT
// =============================================================================
static IWDG_HandleTypeDef   IwdgHandle;
#define WDT_FEED_CYCLE  (2000*1000) //2s

#define WDT_TIM    TIM12    //启动加载用到的定时器TIM12

static u32 sBootDogFeedTime = 0;
static u32 FeedCnt1=0;
static u64 FeedCnt2=0;
static bool_t bSoftFeedDog=false;


static bool_t Sh_IwdgCounter(char *param)
{
    uint32_t data[2];//used to print the s64 type
    u64 Cnt;
    Cnt=FeedCnt2-FeedCnt1;
    memcpy(data,&Cnt,sizeof(data));
    printf("BootLoader Period Counter:%d.\r\n",FeedCnt1);
    printf("Normal Period Counter:0x%08x 0x%08x \r\n",data[1],data[0]);
    return true;
}

//**************************************************************************
struct shell_debug const shell_cmd_iwdg_table[]=
{
    {"wdgc",(bool_t(*)(char*))Sh_IwdgCounter,    "看门狗喂狗次数",     NULL},
};
//**************************************************************************
#define CN_IWDG_SHELL_NUM  sizeof(shell_cmd_iwdg_table)/sizeof(struct shell_debug)
static struct ShellCmdRsc tg_iwdg_shell_cmd_rsc[CN_IWDG_SHELL_NUM];

/*******************************************************************************
功能:IWDG模块shell命令加载
参数:无.
返回值:1。
*********************************************************************************/
ptu32_t Iwdg_Shell_Module_Install(void)
{
    Sh_InstallCmd(shell_cmd_iwdg_table,tg_iwdg_shell_cmd_rsc,CN_IWDG_SHELL_NUM);
    return 1;
}

// =============================================================================
// 功能：板上看门狗喂狗函数,max706的喂狗时间是1.6s,根据手册，喂狗是边沿触发
// 参数：无
// 返回：无
// =============================================================================
bool_t BrdWdt_FeedDog(void)
{
    /* Refresh IWDG: reload counter */
    if(HAL_IWDG_Refresh(&IwdgHandle) == HAL_OK)
    {
//      return false;
        if(bSoftFeedDog)
        {
//          printk("Feed Dog.\r\n");   //todo for test by zhb20171025
        }
        FeedCnt2++;
        return true;
    }
    else
    {
//      printk("Feed Dog failed.\r\n");    //todo for test by zhb20171025
        return false;
    }
}


bool_t BrdBoot_FeedEnd(void);
u32 FeedDog_Isr(ptu32_t intline)
{
    WDT_TIM->CNT = 0;
    WDT_TIM->SR = 0;//清中断标志
    BrdWdt_FeedDog();
    Int_ClearLine(intline);
    FeedCnt1++;
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
    bSoftFeedDog=true;
    return true;
}


void IWDG_Init(void)
{
    /*##-1- Check if the system has resumed from IWDG reset ####################*/
      if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) != RESET)
      {
          /* Clear reset flags */
             __HAL_RCC_CLEAR_RESET_FLAGS();
      }

   /*##-2- Configure the IWDG peripheral ######################################*/
      /*##-3- Configure the IWDG peripheral ######################################*/
      IwdgHandle.Instance = IWDG;
      IwdgHandle.Init.Prescaler = IWDG_PRESCALER_32;
      IwdgHandle.Init.Reload    = 32000/16;
      IwdgHandle.Init.Window    = IWDG_WINDOW_DISABLE;

    if (HAL_IWDG_Init(&IwdgHandle) != HAL_OK)
    {
        printf("IWDG Init failed.\r\n");
    }
    printf("IWDG Init success.\r\n");
}

// =============================================================================
// 功能：板上看门狗芯片初始化
// 参数：无
// 返回：无
// =============================================================================
void ModuleInstall_BrdWdt(void)
{
    IWDG_Init();
    WdtHal_RegisterWdtChip("Wdt_IWDG",WDT_FEED_CYCLE/4,BrdWdt_FeedDog,\
            BrdBoot_FeedStart,BrdBoot_FeedEnd);
    Iwdg_Shell_Module_Install();
}
#endif


