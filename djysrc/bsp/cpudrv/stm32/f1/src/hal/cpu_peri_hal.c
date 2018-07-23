// =============================================================================
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 文件名     ：cpu_peri_hal.c
// 模块描述: HAL需要使用到的接口函数
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 2/20.2017
// =============================================================================

#include <stdio.h>
#include <stddef.h>
#include "cpu_peri.h"
#include <os.h>
#include <board-config.h>
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_myname.h****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu_peri_hal" //填写该组件的名字
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp组件             //选填“第三方组件、核心组件、bsp组件、用户组件”，本属性用于在IDE中分组
//select:可选                //选填“必选、可选、不可选”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//grade:none                    //初始化时机，可选值：none，init，main。none表示无须初始化，
                                //init表示在调用main之前，main表示在main函数中初始化
//dependence:"none"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //如果依赖多个组件，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***定义无值的宏，仅用于第三方组件
//%$#@free,
//%$#@end configue  ****参数配置结束
//@#$%component end configure


//==============================================================================
//HAL_RCC会用到此量，因为DJYOS使用自己的时钟初始化过程，因此，在使用HAL库的时候，有些HAL
//内部会调用该量，因此需此处赋值
//例如ETH网络驱动中，stm32f7xx_hal_eth.c中会调用
uint32_t SystemCoreClock = CN_CFG_MCLK;            //HAL_RCC会用到此量

const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t APBPrescTable[8] =  {0, 0, 0, 0, 1, 2, 3, 4};
//HAL里面会调用该函数作时间操作，因此，若用到HAL库，则需要提供该函数提供延时
//return ms
uint32_t HAL_GetTick(void)
{
    static u32 sTicks = 0;
    static u16 sTimCntLast = 0;
    u16 TimCntCurrent;
    TIM_HandleTypeDef TimHandle;


    TimHandle.Instance = TIM5;
    TimCntCurrent = (__HAL_TIM_GET_COUNTER(&TimHandle))&0xFFFF;//1ms
    if( sTimCntLast > TimCntCurrent)
    {
        sTicks += 65536;    //时钟分辨率1mS，16bit是65536mS
    }
    sTimCntLast = TimCntCurrent;
    return sTicks + (TimCntCurrent);
}

void HAL_SuspendTick(void)
{
    TIM_HandleTypeDef TimHandle;
    TimHandle.Instance = TIM5;
    HAL_TIM_Base_Stop(&TimHandle);
}
void HAL_ResumeTick(void)
{
    TIM_HandleTypeDef TimHandle;
    TimHandle.Instance = TIM5;
    HAL_TIM_Base_Start(&TimHandle);
}


//此处用TIM5
void HAL_TickInit(void)
{
    u32 uwPrescalerValue;
    TIM_HandleTypeDef TimHandle;

    __HAL_RCC_TIM5_CLK_ENABLE();

    uwPrescalerValue = ((SystemCoreClock/4) / 1000) - 1;    //Counter Clock = 1000
    TimHandle.Instance = TIM5;
    TimHandle.Init.Period        = 0xFFFFFFFF;
    TimHandle.Init.Prescaler     = uwPrescalerValue;
    TimHandle.Init.ClockDivision = 0;
    TimHandle.Init.CounterMode   = TIM_COUNTERMODE_UP;
    TimHandle.Init.RepetitionCounter = 0;

    HAL_TIM_Base_DeInit(&TimHandle);
    HAL_TIM_Base_Init(&TimHandle);
    HAL_TIM_Base_Start(&TimHandle);
}
