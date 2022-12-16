#include <stdint.h>
#include <stddef.h>

#include "cpu_peri.h"
#include "hard-exp.h"
#include "arch_feature.h"
#include "at32f435_437.h"
#include "arm32_feature.h"

#include "project_config.h"

#include "core_cm4.h"

extern   uint32_t   msp_top[ ];

struct ScbReg volatile * const startup_scb_reg
                        = (struct ScbReg *)0xe000ed00;
void Startup_NMI(void)
{
    while(1);
}
void Startup_Hardfault(void)
{
    while(1);
}

void Init_Cpu(void);

const u32 gc_u32StartupExpTable[128] __attribute__ ((section(".StartupExpTbl")))=
{
    (u32) msp_top,
    (u32) Init_Cpu,
    (u32) Startup_NMI,
    (u32) Startup_Hardfault
};

void Init_Cpu(void)
{
    __set_PSP((uint32_t)msp_top);
    __set_PRIMASK(1);
    __set_FAULTMASK(1);
    __set_CONTROL(0);

    #if (_D_FPU_USED == 1)
    startup_scb_reg->CPACR = (3UL << 20)|(3UL << 22);    //使能FPU
    startup_scb_reg->FPCCR = (1UL << 31);                //关闭lazy stacking
    #endif
    switch(startup_scb_reg->CPUID)
    {
    }

    extern void SysClockInit(void);
    SysClockInit();

    extern void SRAM_Init(void);
    SRAM_Init();

#if (CFG_RUNMODE_BAREAPP == 1)
    Iboot_LoadPreload();
#else
    Iboot_IAP_SelectLoadProgam();
#endif
}

extern void Iboot_LoadPreload(void);
void AppStart(void)
{
    __set_MSP((uint32_t)msp_top);
    __set_PSP((uint32_t)msp_top);
    Iboot_LoadPreload();
}

//-----------------------------------------------------------------
//功能：IAP组件控制运行模式所需的GPIO引脚初始化，由于此时系统还没有加载，只能使
//      用直接地址操作，不能调用gpio相关的库函数。
//      如果不是使用gpio做标志，本函数不是必须，可删掉。
//参数：无
//返回：无。
//-----------------------------------------------------------------
void IAP_GpioPinInit(void)
{
    /* adapt to your project. */
}

//-----------------------------------------------------------------
//功能：由硬件决定是否强制进入Iboot，若此函数返回TRUE，则强制运行Iboot。通常会使
//      用一个gpio，通过跳线决定。
//      正常情况下，如果正在运行APP，是可以用runiboot命令切换到Iboot状态的，设置
//      此硬件的目的有二：
//     1、在严重异常错误，不能用shell切换时，提供一个补救措施。
//     2、出于安全考虑，APP中没有包含切换代码，或者由于资源的关系，裁掉了shell。
//参数：无
//返回：无。
//说明：本函数所涉及到的硬件，须在本文件中初始化，特别需要注意的是，不允许调用未
//      加载的函数，特别是库函数。
//      本函数必须提供，如果没有设置相应硬件，可以简单返回false。
//-----------------------------------------------------------------
bool_t Iboot_IAP_IsForceIboot(void)
{
    u32 flag;
    IAP_GpioPinInit( );

    /* TODO: detect the real hardware. */
    if (flag == 0)
        return true;
    return false;

}




