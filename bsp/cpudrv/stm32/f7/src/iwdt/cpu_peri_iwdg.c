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
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip iwdt"//组件名
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"kernel","stm32f7"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
//%$#@num,0,100000000,
#define CFG_FEED_CYCLE  (2000*1000)     //"feed dog cycle"
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
//%$#@end configue  ****参数配置结束
//@#$%component end configure


// BOARD WDT
// =============================================================================
static IWDG_HandleTypeDef   IwdgHandle;

#define WDT_TIM    TIM12    //启动加载用到的定时器TIM12

static u32 sBootDogFeedTime = 0;
static u32 FeedCnt1=0;
static u64 FeedCnt2=0;
static bool_t bSoftFeedDog=false;


static bool_t Sh_IwdgCounter(char *param);
ADD_TO_ROUTINE_SHELL(wdgc,Sh_IwdgCounter,"看门狗喂狗次数");
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
    WdtHal_RegisterWdtChip("Wdt_IWDG",CFG_FEED_CYCLE/4,BrdWdt_FeedDog,\
            BrdBoot_FeedStart,BrdBoot_FeedEnd);
}



