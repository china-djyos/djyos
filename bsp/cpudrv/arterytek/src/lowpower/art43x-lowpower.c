#include <stdint.h>
#include <stddef.h>
#include <lowpower.h>
#include <cpu_peri.h>
#include <core_cm4.h>
#include <dbug.h>

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    void ModuleInstall_LowPower (void)
//    ModuleInstall_LowPower();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip peripheral lowpower control"//低功耗组件外设驱动
//parent:"none"                      //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                          //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                       //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                         //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:none                         //初始化时机，可选值：early，medium，later, pre-main。
                                         //表示初始化时间，分别是早期、中期、后期
//dependence:"none"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                         //选中该组件时，被依赖组件将强制选中，
                                         //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"                  //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                         //选中该组件时，被依赖组件不会被强制选中，
                                         //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                         //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_PERIPHERAL_LOWPOWER_CONTROL == false )
//#warning  " cpu_onchip_peripheral_lowpower_control  组件参数未配置，使用默认配置"
//%$#@target = header              //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_PERIPHERAL_LOWPOWER_CONTROL    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束

//@#$%component end configure


//----初始化低功耗硬件--------------------------------------------------------
//功能: 初始化低功耗管理硬件,如果不需要初始化,可以直接return true.
//参数: 无
//返回: 无
//----------------------------------------------------------------------------
bool_t __LP_BSP_HardInit(void)
{

    HAL_PWR_DeInit();               //PWR模块使能
    HAL_PWR_EnableBkUpAccess();//后备区使能
    return true;
}

//----读取休眠级别------------------------------------------------------------
//功能: 当个系统从休眠唤醒并重新,调用本函数可以获得唤醒前的休眠状态,返回值是
//      在lowpower.h文件中定义的CN_SLEEP_L3、CN_SLEEP_L4、CN_SLEEP_NORMAL之一,
//      CN_SLEEP_NORMAL表示上电或者硬复位,或者软件指令复位.注意,返回值不会是
//      CN_SLEEP_L0~2之一.
//参数: 无
//返回: CN_SLEEP_L3、CN_SLEEP_L4、CN_SLEEP_NORMAL之一
//----------------------------------------------------------------------------
u32 __LP_BSP_GetSleepLevel(void)
{
    u32 bkt_DR;
    RTC_HandleTypeDef RTC_Handler;  //RTC句柄
    RTC_Handler.Instance=RTC;
    if(__HAL_PWR_GET_FLAG(PWR_FLAG_WU+PWR_FLAG_SB)& PWR_FLAG_WU)
    {
        bkt_DR = HAL_RTCEx_BKUPRead(&RTC_Handler,RTC_BKP_DR0);//todo
        if( (bkt_DR == CN_SLEEP_L3) || (bkt_DR == CN_SLEEP_L4) )
            return bkt_DR;
        else
            return CN_SLEEP_NORMAL;
    }
    else
        return CN_SLEEP_NORMAL;
}

//----保存休眠级别-------------------------------------------------------------
//功能: 把即将进入的休眠级别保存到相应的硬件中,注意,只有L3和L4需要.
//参数: SleepLevel, 即将进入的低功耗级别
//返回true=成功,false=失败
//-----------------------------------------------------------------------------
bool_t __LP_BSP_SaveSleepLevel(u32 SleepLevel)
{
    RTC_HandleTypeDef RTC_Handler;  //RTC句柄
    RTC_Handler.Instance=RTC;

    if((SleepLevel!= CN_SLEEP_L3) && (SleepLevel!= CN_SLEEP_L4))
        return false;
    HAL_RTCEx_BKUPWrite(&RTC_Handler,RTC_BKP_DR0,SleepLevel);
    return true;

}

//-----------------------------------------------------------------------------
//功能: 进入休眠
//参数: sleep_level,休眠等级
//      pend_ticks, 休眠tick数
//返回: 无意义
//-----------------------------------------------------------------------------
void __LP_BSP_EntrySleep(u8 sleep_level, u32 pend_ticks)
{
    switch(sleep_level)
    {
        case CN_SLEEP_L0:
            SCB->SCR |= SCB_SCR_SEVONPEND_Msk;
            SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
            __WFE();
            break;
        case CN_SLEEP_L1:
            SCB->SCR |= SCB_SCR_SEVONPEND_Msk;
            SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
            __WFE();
            break;
        case CN_SLEEP_L2:
            //禁止中断

            //清所有外部中断标志和RTC闹钟标志
            EXTI->PR = 0xFFFFF;
            HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
            break;
        case CN_SLEEP_L3:
            HAL_PWR_EnterSTANDBYMode();
            break;
        case CN_SLEEP_L4:
            HAL_PWR_EnterSTANDBYMode( );
            break;
    }
}

bool_t __LP_BSP_RestoreRamL3(void)
{
    return true;
}
bool_t __LP_BSP_SaveRamL3(void)
{
   return true;
}

//-----------------------------------------------------------------------------
//功能: 安装低功耗组件，要把一些低功耗需要使用到的函数，注册到系统中
//参数: __LP_BSP_EntrySleep：进入休眠；__LP_BSP_SaveSleepLevel：保存休眠等级；__LP_BSP_SaveRamL3：保存进入休眠等级3之前的内存，
//		__LP_BSP_AsmSaveReg：获取含自己的返回地址在内的上下文并保存到栈中
//返回: 无
//-----------------------------------------------------------------------------
void ModuleInstall_LowPower (void)
{
    Register_LowPower_Function(__LP_BSP_EntrySleep, __LP_BSP_SaveSleepLevel, __LP_BSP_SaveRamL3, __LP_BSP_AsmSaveReg);
}
