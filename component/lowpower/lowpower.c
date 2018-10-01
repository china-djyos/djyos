#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include "string.h"
#include "lowpower.h"
#include "int.h"
#include "int_hard.h"

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
// extern void ModuleInstall_LowPower (void);
//    ModuleInstall_LowPower();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"lowpower"      //低功耗模块
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:核心组件               //选填“第三方组件、核心组件、bsp组件、用户组件”，本属性用于在IDE中分组
//select:可选                //选填“必选、可选、不可选”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"cpu_peri_lowpower"//该组件的依赖组件名（可以是none，表示无依赖组件），
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


struct LowPowerCtrl g_tLowPower;

//----进入低功耗状态----------------------------------------------------------
//功能: 调用本函数后,进入设定的低功耗状态,进入前调用相关回调函数.
//参数: 无
//返回: CN_SLEEP_NORMAL,CN_SLEEP_L0-L3之一, 不可能返回CN_SLEEP_L4
//说明：使用L3要特别注意，许多设备，进入如此深的休眠后，其配置会丢失，唤醒时往往
//      不能正常工作，或者中作状态不正确。因此，必须充分利用EntrySleepReCall和
//      ExitSleepReCall两个函数。
//----------------------------------------------------------------------------
u32 LP_EntryLowPower(struct ThreadVm *vm)
{
    atom_low_t  atom_bak;

    if(g_tLowPower.DisableCounter != 0)
    {
        g_tLowPower.EntrySleepReCall(CN_SLEEP_L0);
        __LP_BSP_EntrySleepL0( );
        g_tLowPower.ExitSleepReCall(CN_SLEEP_L0);
        return CN_SLEEP_L0;
    }
    switch(g_tLowPower.SleepLevel)
    {
       case CN_SLEEP_L0:
           Int_CutTrunk();
           g_tLowPower.EntrySleepReCall(CN_SLEEP_L0);
           __LP_BSP_EntrySleepL0( );
           g_tLowPower.ExitSleepReCall(CN_SLEEP_L0);
           Int_ContactTrunk();
           return CN_SLEEP_L0;
           break;
       case CN_SLEEP_L1:
           Int_CutTrunk();
           g_tLowPower.EntrySleepReCall(CN_SLEEP_L1);
           __LP_BSP_EntrySleepL1( );
           g_tLowPower.ExitSleepReCall(CN_SLEEP_L1);
           Int_ContactTrunk();
           return CN_SLEEP_L1;
           break;
       case CN_SLEEP_L2:
           g_tLowPower.EntrySleepReCall(CN_SLEEP_L2);
           __LP_BSP_EntrySleepL2( );
           g_tLowPower.ExitSleepReCall(CN_SLEEP_L2);
           return CN_SLEEP_L2;
           break;
       case CN_SLEEP_L3:
           g_tLowPower.EntrySleepReCall(CN_SLEEP_L3);
           __LP_BSP_SaveSleepLevel(CN_SLEEP_L3);
           atom_bak = Int_LowAtomStart();
           __LP_BSP_AsmSaveReg(vm,__LP_BSP_SaveRamL3,__LP_BSP_EntrySleepL3);
           Int_LowAtomEnd(atom_bak);
           g_tLowPower.ExitSleepReCall(CN_SLEEP_L3);
           return CN_SLEEP_L3;
           break;
       case CN_SLEEP_L4:
           g_tLowPower.EntrySleepReCall(CN_SLEEP_L4);
           __LP_BSP_SaveSleepLevel(CN_SLEEP_L4);
           __LP_BSP_EntrySleepL4( );    //进入低功耗,并且不会返回这里
           break;

       default:
           break;
    }
    return 0;
}

//----禁止进入低功耗状态------------------------------------------------------
//功能: 调用本函数后,系统不会进入L1及以上级别的低功耗状态.并且使禁止次数计数器
//      +1,须调用同等次数的LP_EnableSleep使计数器减至0后,才允许进入L1及以上级别
//      低功耗状态.
//参数: 无
//返回: 禁止计数器当前值
//----------------------------------------------------------------------------
u32 LP_DisableSleep(void)
{
    if(g_tLowPower.DisableCounter != CN_LIMIT_UINT32)
        g_tLowPower.DisableCounter++;
    return g_tLowPower.DisableCounter;
}

//----使能进入低功耗状态------------------------------------------------------
//功能: 调用本函数后,禁止次数计数器减1,减至0后,允许进入L1及以上级别低功耗状态.
//参数: 无
//返回: 禁止计数器当前值
//----------------------------------------------------------------------------
u32 LP_EnableSleep(void)
{
    if(g_tLowPower.DisableCounter != 0)
        g_tLowPower.DisableCounter--;
    return g_tLowPower.DisableCounter;
}

//----设置低功耗级别----------------------------------------------------------
//功能: 设置当前低功耗级别,CN_SLEEP_L0---CN_SLEEP_L4
//参数: Level, CN_SLEEP_L0---CN_SLEEP_L4中的一个
//返回: 当前低功耗级别
//----------------------------------------------------------------------------
u32 LP_SetSleepLevel(u32 Level)
{
    if(Level <= CN_SLEEP_L4)
        g_tLowPower.SleepLevel = Level;
    return g_tLowPower.SleepLevel;
}


u32 LP_GetSleepLevel(void)
{
    return g_tLowPower.SleepLevel;
}

void LP_SetHook(u32 (*EntrySleepReCall)(u32 SleepLevel),
                u32 (*ExitSleepReCall)(u32 SleepLevel))
{
    if(EntrySleepReCall != NULL)
        g_tLowPower.EntrySleepReCall = EntrySleepReCall;
    if(ExitSleepReCall != NULL)
        g_tLowPower.ExitSleepReCall = ExitSleepReCall;
}

u32 EmptyReCall(u32 SleepLevel)
{
    return 0;
}
//----初始化-------------------------------------------------------------------
//功能: 初始化低功耗组件
//参数: EntrySleepReCall,进入低功耗状态前调用的回调函数
//      ExitSleepReCall, 离开低功耗状态时调用的回调函数
//返回: 无意义
//-----------------------------------------------------------------------------
extern u32 (*g_fnEntryLowPower)(struct ThreadVm *vm);  //进入低功耗状态的函数指针。
void ModuleInstall_LowPower (void)
{
    g_tLowPower.EntrySleepReCall = EmptyReCall;
    g_tLowPower.ExitSleepReCall = EmptyReCall;
    g_tLowPower.SleepLevel = CN_SLEEP_NORMAL;
    g_tLowPower.DisableCounter = 0;
    g_fnEntryLowPower = LP_EntryLowPower;
    return ;
}

