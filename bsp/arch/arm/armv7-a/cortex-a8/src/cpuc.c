#include "cpu.h"
#include "arch_feature.h"
#include "align.h"
#include "int.h"
#include "hard-exp.h"
#include "board-config.h"

uint32_t djy_switch_interrupt_flag __attribute__ ((section(".data.interrupt")));
uint32_t *djy_interrupt_from_thread __attribute__ ((section(".data.interrupt")));
uint32_t *djy_interrupt_to_thread __attribute__ ((section(".data.interrupt")));

//// =============================================================================
//// 功能：在系统起来以后需把各种标志复位
//// 参数：无
//// 返回：无
//// =============================================================================
//__attribute__((weak)) void __InitTimeBase(void)
//{
//    djy_switch_interrupt_flag = 0;
//    djy_interrupt_from_thread = NULL;
//    djy_interrupt_to_thread = NULL;
//}
//


//__attribute__((weak)) uint64_t __DjyGetTicks(void)
//{
//    return gRunTicks;
//}

//??????????
//__attribute__((weak)) void DjySetUpdateTickFlag(bool_t flag)
//{
//    gResumeTickFlag = flag;
//}
//
//__attribute__((weak)) bool_t DjyGetUpdateTickFlag(void)
//{
//    return gResumeTickFlag;
//}
//
////??????????
//__attribute__((weak)) void DjyUpdateTicks(uint32_t ticks)
//{
//    gRunTicks += ticks;
//}

// ============================================================================
// ARM9的reboot、reset和restart_system，在对应的cpudrv里实现，不放在这里实现
// ============================================================================
#if 0
#include <Iboot_Info.h>

extern void Iboot_LoadPreload(void);
//void CPU_Reboot(u32 key)
//{
//
//}

void CPU_Reset(u32 key)
{
    Iboot_SetSoftResetFlag();
#if (CFG_RUNMODE != CN_RUNMODE_BOOTSELF)
    Iboot_SetPreviouResetFlag();
#endif
    void (*fn_start)();
    fn_start = 0x0;
    fn_start();
//  _start();
}
void CPU_RestartSystem(u32 key)
{
    Iboot_LoadPreload();
}
#endif
