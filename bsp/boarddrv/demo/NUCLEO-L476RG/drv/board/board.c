// =============================================================================
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 文件名     ：board.c
// 模块描述: 板件相关部分初始化或配置等
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 3/8.2017
// =============================================================================

#include <stdint.h>
#include <stddef.h>
#include <board-config.h>
#include <stm32L4xx.h>
#include <stm32l4xx_hal_rcc.h>
#include <stm32l4xx_hal_gpio.h>
#include <stm32l4xx_hal_gpio_ex.h>

#include "int.h"
#include "djyos.h"
#include "hard-exp.h"
#include "cpu_peri_lptimer.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern void Board_GpioInit(void);
//    Board_GpioInit();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"nucleo_l476rg_board"      //板件特性配置
//parent:"none"                             //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp组件                         //选填“第三方组件、核心组件、bsp组件、用户组件”，本属性用于在IDE中分组
//select:必选                               //选填“必选、可选、不可选”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                            //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early                          //初始化时机，可选值：early，medium，later。
                                            //表示初始化时间，分别是早期、中期、后期
//dependence:"cpu_peri_gpio","cpu_peri_lowpower"                //该组件的依赖组件名（可以是none，表示无依赖组件），
                                            //选中该组件时，被依赖组件将强制选中，
                                            //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"                     //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                            //选中该组件时，被依赖组件不会被强制选中，
                                            //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                              //该组件的依赖组件名（可以是none，表示无依赖组件），
                                            //如果依赖多个组件，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
//%$#@target = header                       //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***定义无值的宏，仅用于第三方组件
//%$#@free,
//%$#@end configue  ****参数配置结束
//@#$%component end configure


extern u32 SystemCoreClock;
#if CN_CFG_USE_USERTIMER
extern struct IntMasterCtrl  tg_int_global;          //?¨ò?2￠3?ê??ˉ×ü?D???????á11
extern void __Djy_ScheduleAsynSignal(void);
static void __DjyIsrTimeBase(u32 param);
#define CN_TIME_ROUNDING    (32768U)//四舍五入的值
#define TIME_GLUE           (0x1E849CU)
#define FAST_TIME_GLUE      (0x863U)
#define TIME_BASE_MIN_GAP   (CN_CFG_TIME_BASE_HZ>Mhz?(100*TIME_GLUE):((200*CN_CFG_TIME_BASE_HZ)/Mhz))
static u64 g_time_base_tick=0;
static u64 g_per_sys_cnt = 0;
extern void HardExp_ConnectSystick(void (*tick)(u32 inc_ticks));

static void Null_Tick(u32 inc_ticks)
{
    return;
}

void __InitTimeBase(void)
{
    HardExp_ConnectSystick(Null_Tick);
    pg_systick_reg->ctrl &=   ~((1<<bo_systick_ctrl_enable)    //使能
                                |(1<<bo_systick_ctrl_tickint)   //允许产生中断
                                |(1<<bo_systick_ctrl_clksource));//用内核时钟
    pg_systick_reg->reload = 0;
    pg_systick_reg->current = 0;
    /*以上几行是为了兼容旧版本的IBOOT，因为旧版本的IBOOT里面使用了SYSTICK定时器，打开了该定时器，所以这里要关掉*/
    Lptimer1_PreInit();
}

void __DjyStartTimeBase(void)
{
    Lptimer1_Init(CN_LIMIT_UINT16,__DjyIsrTimeBase);
}

//获取可以定时的最大值
u32 __Djy_GetDelayMaxCnt(void)
{
    return (CN_LIMIT_UINT16>>1);
}

u32 __Djy_GetTimeBaseGap(void)
{
    return TIME_BASE_MIN_GAP;
}

void __Djy_SetTimeBaseCnt(u32 cnt)
{
    if(cnt>CN_LIMIT_UINT16 || cnt==0)
    {
        //àí??é?2??é?ü3???′?ê??t
        return;
    }
    Lptimer1_set_period((u16)cnt);
}

static u32 __Djy_GetTimeBaseRealCnt(void)
{
    return Lptimer1_read_cnt();
}

u32 __Djy_GetTimeBaseReload(void)
{
    return Lptimer1_read_reload();
}

u64 __Djy_TimeBaseUsToCnt(u64 us)
{
    u64 temp = 0;
    temp = ((CN_CFG_TIME_BASE_HZ>Mhz)?
            (us*TIME_GLUE):
            ((us*FAST_TIME_GLUE + CN_TIME_ROUNDING))>>16);
    if( temp < TIME_BASE_MIN_GAP )
        temp = TIME_BASE_MIN_GAP;
    return temp;
}

u32 __Djy_TimeBaseCntToUs(u64 cnt)
{
    return ((CN_CFG_TIME_BASE_HZ>Mhz)?
            (cnt/(u32)TIME_GLUE):
            (u32)(((u64)(cnt*TIME_GLUE))>>16));
}

u64 __Djy_GetTimeBaseCnt(u32 cnt)
{
    return (g_time_base_tick + cnt);
}

u64 __DjyGetSysCnt(void)
{
    u64 temp = 0;
    atom_low_t atom_low;
    atom_low = Int_LowAtomStart();
    temp = g_time_base_tick + __Djy_GetTimeBaseRealCnt();
    if(temp < g_per_sys_cnt)
    {
        temp += CN_LIMIT_UINT16;
    }
    else
        g_per_sys_cnt = temp;
    Int_LowAtomEnd(atom_low);
    return temp;
}

u64 __DjyGetSysTime(void)
{
    u64 time;
    u64 temp=0;
    temp = __DjyGetSysCnt();
    time = ((CN_CFG_TIME_BASE_HZ>Mhz)?
            (temp/(u32)TIME_GLUE)://这里没有办法，只能直接使用除法，否则将引入累计误差，不能容忍  --chj
            ((u64)(temp*TIME_GLUE))>>16);
    return time;
}

static void __DjyIsrTimeBase(u32 param)
{
    u8 flag = 0;
    u32 tick=0;
    g_bScheduleEnable = false;
    tg_int_global.en_asyn_signal_counter = 1;
    tg_int_global.nest_asyn_signal = 1;
    flag = Lptimer1_ClearISR();
    switch(flag)
    {
        case CN_LPTIMER_NONE:
            break;
        case CN_LPTIMER_RELOAD:
            g_time_base_tick += CN_LIMIT_UINT16;
            g_per_sys_cnt = g_time_base_tick;
            break;
        case CN_LPTIMER_CMP:
            tick=__Djy_GetTimeBaseRealCnt();
            g_per_sys_cnt = g_time_base_tick + tick;
            Djy_IsrTimeBase(tick);
            break;
        case CN_LPTIMER_RELOAD_AND_CMP:
            g_time_base_tick += CN_LIMIT_UINT16;
            g_per_sys_cnt = g_time_base_tick;
            //tick=__Djy_GetTimeBaseRealCnt();
            Djy_IsrTimeBase(0);
            break;
    }

    tg_int_global.nest_asyn_signal = 0;
    tg_int_global.en_asyn_signal_counter = 0;
    if(g_ptEventReady != g_ptEventRunning)
        __Djy_ScheduleAsynSignal();       //?′DD?D???úμ÷?è
    g_bScheduleEnable = true;
}

#endif


void Board_GpioInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    //USART2:PA2,PA3
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    SystemCoreClock = CN_CFG_MCLK;
}

