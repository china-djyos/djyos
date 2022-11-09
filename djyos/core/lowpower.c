#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include <systick.h>
#include "string.h"
#include "lowpower.h"
#include "int.h"
#include "int_hard.h"
#include "cpu_peri.h"
#include <shell.h>
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//#define LP_DEFAULT_TRIGGER_TICK         (2U)
struct LowPowerCtrl
{
    u32 SleepLevel;                             //当前低功耗级别
//  u32 TriggerTick;
    u32 DisableCounter;     //禁止低功耗次数计数,0才可以进入L1及以上级别低功耗

    u32 (*EntrySleepReCall)(u32 SleepLevel);    //进入低功耗状态前的回调函数
                                                //返回值表示是否允许进入相应级别低功耗
    u32 (*ExitSleepReCall)(u32 SleepLevel);     //从低功耗状态唤醒后的回调函数

    void (*EntrySleep)(u8 sleep_level, u32 pend_ticks);    //进入低功耗的函数
    bool_t (*SaveSleepLevel)(u32 SleepLevel);                        //保存休眠等级的函数
    bool_t (*SaveRamL3)(void);              //把执行过AsmSaveReg的内存保存到flash中

    void (*AsmSaveReg)(struct ThreadVm *running_vm,
                                bool_t (*SaveRamL3)(void),
                                void (*EntrySleep)(u8 sleep_level, u32 pend_ticks));  //当系统从L3模式唤醒时，系统应从__LP_BSP_AsmSaveReg的返回地址，即下一行代码处继续运行。要实现以下顺序指定：

};

struct LowPowerCtrl g_tLowPower;

//----进入低功耗状态----------------------------------------------------------
//功能: 调用本函数后,进入设定的低功耗状态,进入前调用相关回调函数.
//参数: 无
//返回: CN_SLEEP_NORMAL,CN_SLEEP_L0-L3之一, 不可能返回CN_SLEEP_L4
//说明：使用L3要特别注意，许多设备，进入如此深的休眠后，其配置会丢失，唤醒时往往
//      不能正常工作，或者中作状态不正确。因此，必须充分利用EntrySleepReCall和
//      ExitSleepReCall两个函数。
//----------------------------------------------------------------------------
u32 LP_EntryLowPower(struct ThreadVm *vm,u32 pend_ticks)
{
    atom_low_t  atom_bak;

    if(g_tLowPower.DisableCounter == 0)
    {
        Tick_SetNextTimeTick(pend_ticks);       //具体设置为多少时间，由BSP决定
        switch(g_tLowPower.SleepLevel)
        {
           case CN_SLEEP_L0:
               if(g_tLowPower.EntrySleepReCall(CN_SLEEP_L0))
               {
                   g_tLowPower.EntrySleep(CN_SLEEP_L0, pend_ticks);
                   g_tLowPower.ExitSleepReCall(CN_SLEEP_L0);
               }
               return CN_SLEEP_L0;
               break;
           case CN_SLEEP_L1:
               atom_bak = Int_LowAtomStart();
               if(g_tLowPower.EntrySleepReCall(CN_SLEEP_L1))
               {
                   g_tLowPower.EntrySleep(CN_SLEEP_L1, pend_ticks);
                   g_tLowPower.ExitSleepReCall(CN_SLEEP_L1);
               }
               Int_LowAtomEnd(atom_bak);
               return CN_SLEEP_L1;
               break;
           case CN_SLEEP_L2:
               atom_bak = Int_LowAtomStart();
               if(g_tLowPower.EntrySleepReCall(CN_SLEEP_L2))
               {
                   g_tLowPower.EntrySleep(CN_SLEEP_L2, pend_ticks);
                   g_tLowPower.ExitSleepReCall(CN_SLEEP_L2);
               }
               Int_LowAtomEnd(atom_bak);
               return CN_SLEEP_L2;
               break;
           case CN_SLEEP_L3:
               atom_bak = Int_LowAtomStart();
               if(g_tLowPower.EntrySleepReCall(CN_SLEEP_L3))
               {
                   g_tLowPower.SaveSleepLevel(CN_SLEEP_L3);
                   //当系统从L3模式唤醒时，系统应从__LP_BSP_AsmSaveReg的返回地址，即
                   //下一行代码处继续运行。要实现以下顺序指定：
                   //step1：__LP_BSP_AsmSaveReg获取含自己的返回地址在内的上下文并保存到栈中。
                   //step2：__LP_BSP_SaveRamL3把执行过step1的内存保存到flash中。
                   //step3：g_tLowPower.EntrySleep进入低功耗
                   //step4：从低功耗唤醒后，恢复内存和上下文，程序继续运行
                   //似乎只有把__LP_BSP_SaveRamL3和g_tLowPower.EntrySleep函数按参数
                   //传入才能实现。
                   g_tLowPower.AsmSaveReg(vm, g_tLowPower.SaveRamL3, g_tLowPower.EntrySleep);
                   g_tLowPower.ExitSleepReCall(CN_SLEEP_L3);
               }
               Int_LowAtomEnd(atom_bak);
               return CN_SLEEP_L3;
               break;
           case CN_SLEEP_L4:
               atom_bak = Int_LowAtomStart();
               if(g_tLowPower.EntrySleepReCall(CN_SLEEP_L4))
               {
                   g_tLowPower.SaveSleepLevel(CN_SLEEP_L4);
                   g_tLowPower.EntrySleep(CN_SLEEP_L4, 0);    //进入低功耗,并且不会返回这里
                   //若正确调用，上面的函数并不返回，而是等待复位，下面的代码，是为了在上述
                   //函数执行不正常而返回时，不使系统紊乱。
                   g_tLowPower.ExitSleepReCall(CN_SLEEP_L3);
               }
               Int_LowAtomEnd(atom_bak);
               return CN_SLEEP_ERROR;
               break;

           default:
               return CN_SLEEP_NORMAL;
               break;
        }
    }
    else
        return CN_SLEEP_NORMAL;
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
//参数: Level, CN_SLEEP_NORMAL---CN_SLEEP_L4中的一个
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

//void LP_SetTriggerTick(u32 tick)
//{
//    atom_low_t atom_bak;
//    atom_bak = Int_LowAtomStart();
//    if(tick<LP_DEFAULT_TRIGGER_TICK)
//        return;
//    else
//        g_tLowPower.TriggerTick = tick;
//    Int_LowAtomEnd(atom_bak);
//}
//
//u32 LP_GetTriggerTick(void)
//{
//    return g_tLowPower.TriggerTick;
//}
//
void LP_SetHook(u32 (*EntrySleepReCall)(u32 SleepLevel),
                u32 (*ExitSleepReCall)(u32 SleepLevel))
{
    if(EntrySleepReCall != NULL)
        g_tLowPower.EntrySleepReCall = EntrySleepReCall;
    if(ExitSleepReCall != NULL)
        g_tLowPower.ExitSleepReCall = ExitSleepReCall;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

u32 LP_EmptyReCall(u32 SleepLevel)
{
    return 1;
}

//-----------------------------------------------------------------------------
//功能: 注册低功耗相关的功能函数
//参数: EntrySleep,进入低功耗状态前调用的回调函数
//      SaveSleepLevel, 保持休眠等级
//      SaveRamL3, 保存在进入休眠等级3之前的内存
//      AsmSaveReg, 获取含自己的返回地址在内的上下文并保存到栈中
//返回: 无意义
//-----------------------------------------------------------------------------
void Register_LowPower_Function (void (*EntrySleep)(u8 sleep_level, u32 pend_ticks),
                                 bool_t (*SaveSleepLevel)(u32 SleepLevel),
                                 bool_t (*SaveRamL3)(void),
                                 void (*AsmSaveReg)(struct ThreadVm *running_vm,
                                 bool_t (*SaveRamL3)(void),
                                 void (*EntrySleep)(u8 sleep_level, u32 pend_ticks)))
{
    g_tLowPower.EntrySleep = EntrySleep;
    g_tLowPower.SaveSleepLevel = SaveSleepLevel;
    g_tLowPower.SaveRamL3 = SaveRamL3;
    g_tLowPower.AsmSaveReg = AsmSaveReg;

    g_tLowPower.EntrySleepReCall = LP_EmptyReCall;
    g_tLowPower.ExitSleepReCall = LP_EmptyReCall;
    g_tLowPower.SleepLevel = CN_SLEEP_L0;
    g_tLowPower.DisableCounter = 0;
    return ;
}

__attribute__((weak)) bool_t __LP_BSP_SetSleepMode(ptu32_t param)
{
    return false;
}

#pragma GCC diagnostic pop
