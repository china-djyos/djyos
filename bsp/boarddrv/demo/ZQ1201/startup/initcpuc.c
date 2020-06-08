//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
//所属模块:  CPU初始化
//作者：     lst
//版本：      V1.0.0
//初始版本完成日期：2009-08-05
//文件描述:  cpu时钟初始化
//其他说明:  无
//修订历史:
//2. ...
//1. 日期: 2009-08-05
//   作者: lst
//   新版本号：V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifdef CFG_CPU_ZQ12XX_M0
#include "arch_feature.h"
#include "stdint.h"
#include "int.h"
#include "cpu.h"
#include "cpu_peri.h"
#include "hard-exp.h"
#include "lowpower.h"
#include "djyos.h"
//#include "core_cmFunc.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

extern   uint32_t   msp_top[ ];
//extern void __set_PSP(uint32_t topOfProcStack);
//extern void __set_PRIMASK(uint32_t priMask);
//extern void __set_CONTROL(uint32_t control);

extern void Iboot_IAP_SelectLoadProgam(void);
//static void __set_PSP(uint32_t topOfProcStack)
//{
//  __asm volatile ("MSR psp, %0\n" : : "r" (topOfProcStack) : "sp");
//}
//static void __set_MSP(uint32_t topOfMainStack)
//{
//  __asm volatile ("MSR msp, %0\n" : : "r" (topOfMainStack) : "sp");
//}
//
//static void __set_PRIMASK(uint32_t priMask)
//{
//  __asm volatile ("MSR primask, %0" : : "r" (priMask) : "memory");
//}
//static void __set_CONTROL(uint32_t control)
//{
//  __asm volatile ("MSR control, %0" : : "r" (control) : "memory");
//}
void Startup_NMI(void)
{
    while(1);
}
void Startup_Hardfault(void)
{
    while(1);
}

#include <silan_pmu.h>
#include <silan_syscfg.h>
#include <silan_m0_cache.h>
#include <silan_bootspi.h>
#include <silan_sdram.h>
#include "silan_syscfg_regs.h"


static void SystemInit(void)
{
    silan_pmu_wdt_reset_disable();
    silan_dsp_ldo_config(LDO_OUT_12V);
    silan_pwr_iso_disable();

    silan_vlsp_cclk_config(CLK_ON);
    silan_soc_ldo_config(LDO_OUT_12V);

    silan_dsp_dcache_cfg(DCACHE_MODE_DC);

    // LQFP64 Close MCU DEBUG
    //silan_mcu_debug_close();
    silan_risc_debug_close();
    silan_dsp_debug_close();
}

static void ResetInit(void)
{
     __REG32(SILAN_SYSCFG_REG11) &= ~(0x1);
    __REG32(SILAN_SYSCFG_REG11) &= ~(0x1<<2);
    silan_audiopll_init();
    silan_syspll_init(SYSPLL_SSCG_OFF);
    silan_sysclk_change(SYSCLK_FROM_PLL, SYSPLL_160M);
    silan_timer_cclk_config(TIMER_CCLK_CLKSYS_DIV2, CLK_ON);

//  iomux_init();

//  silan_module_clkon(CLK_UART2, CLK_ON);
/// silan_uart_sys_init(UART_2, BAUDRATE_115200);

//  SL_LOG("\n\n\n");
//  SL_LOG("CLK_SYS = %dM", get_sysclk_val_settled()/1000000);

    m0_cache_init(CACHE_ENABLE, 0, 0, 0);
//  m0_cache_init(CACHE_DISABLE, 0, 0, 0);
//  cache_spi_cmd_sel(SF_CMD_FRD);

    silan_mcu_cclksel(MCU_CCLK_DIV1);

//  int5ms_f = 0;
//  int1s_f = 0;
    silan_pic_init();
}

static void SDRAM_Init(void)
{
    silan_sdram_init();
}
void Init_Cpu(void)
{
    __set_PSP((uint32_t)msp_top);
    __set_PRIMASK(1);
    __set_CONTROL(0);

    SystemInit();
    ResetInit();

    SDRAM_Init();

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
//    u32 flag;
//    IAP_GpioPinInit( );
//    flag=pg_gpio_regc->IDR&(1<<10);
//    if(flag==0)
//        return false;
//    return true;

    return false;

}
#endif

#ifdef CFG_CPU_ZQ12XX_CK
#include "csi_core.h"
#include "arch_feature.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

extern void Iboot_LoadPreload(void);

void Init_Cpuc(void)
{
    /* Here we may setting exception vector, MGU, cache, and so on. */
//  csi_cache_set_range(0, 0x00000000, CACHE_CRCR_4M, 1);
//  csi_cache_set_range(1, 0x22000000, CACHE_CRCR_8M, 1);
//  csi_dcache_enable();

#if (CFG_RUNMODE_BAREAPP == 1)
    Iboot_LoadPreload();
#else
    Iboot_IAP_SelectLoadProgam();
#endif
}

void IAP_GpioPinInit(void)
{

}

bool_t Iboot_IAP_IsForceIboot(void)
{
    return false;
}

void epc_test(uint32_t r_epc)
{
    if(r_epc>=0x021e0100)
    {
        return;
    }
    else if(r_epc<=0x02160000)
    {
        return;
    }
}

#endif

