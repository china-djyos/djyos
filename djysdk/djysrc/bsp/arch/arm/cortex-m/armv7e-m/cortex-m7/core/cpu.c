//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
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
//所属模块: 调度器
//作者：lst
//版本：V1.0.1
//文件描述: 调度器中与CPU直接相关的代码。
//其他说明:
//修订历史:
//2. 日期: 2009-04-24
//   作者: lst
//   新版本号: V1.0.1
//   修改说明: 删除了一些为dlsp版本准备的东西
//1. 日期: 2009-01-04
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------

#include "align.h"
#include "stdint.h"
#include "stdlib.h"
#include "int.h"
#include "hard-exp.h"
#include "string.h"
#include "arch_feature.h"
#include "cpu.h"
#include "djyos.h"
#include "cpu_peri.h"
#include <board-config.h>

#include "core_cm7.h"

extern   uint32_t   msp_top[ ];
extern void __set_PSP(uint32_t topOfProcStack);
extern void __set_MSP(uint32_t topOfProcStack);
// void __start_systick(void);


extern s64  g_s64OsTicks;             //操作系统运行ticks数
u32 g_u32CycleSpeed; //for(i=j;i>0;i--);每循环纳秒数*1.024
u32 g_u32HundreUsfor;

//todo 该函数是不是不需要了.
//----测量指令指令延时常数-----------------------------------------------------
//功能: 设置指令延时常数,使不管用何种编译器和编译优化选项,djy_delay_us函数准确延时，
//参数：无
//返回: 无
//备注: 本函数移植敏感
//-----------------------------------------------------------------------------
void __DjySetDelay(void)
{
    uint32_t counter,u32_fors=10000,clksum;
    volatile uint32_t u32loops;

    clksum = 0xFFFFFF;  //此处建议取计数器最大值，M3、M4内核SYSTICK是24位计数器

    pg_systick_reg->reload = clksum;
    pg_systick_reg->current = 0;
    pg_systick_reg->ctrl =   (1<<bo_systick_ctrl_enable)    //使能
                            |(1<<bo_systick_ctrl_tickint)   //允许产生中断
                            |(1<<bo_systick_ctrl_clksource);//用内核时钟
    for(u32loops=u32_fors;u32loops>0;u32loops--);       //循环u32_fors次

    //此处调试阶段需确定，当递减计数器到达0时，说明一个计数周期内无法完成上述循环
    //需减小u32_fors值，或采用systick中断服务函数计算有多少个计数周期
    if(pg_systick_reg->ctrl & bm_systick_ctrl_countflag)
    {
        while(1);
    }
    //停止定时器
    pg_systick_reg->ctrl =   (0<<bo_systick_ctrl_enable)    //不使能
                            |(1<<bo_systick_ctrl_tickint)   //允许产生中断
                            |(1<<bo_systick_ctrl_clksource);//用内核时钟
    counter = pg_systick_reg->current;      //读取循环u32_fors次所需时间

    counter = clksum - counter;    //取实际脉冲数。
    g_u32CycleSpeed = ((uint64_t)counter * (u64)10E8)/CN_CFG_FCLK/u32_fors;//防溢出，用64位
    g_u32CycleSpeed = (g_u32CycleSpeed << 10) / 1000;     //扩大1.024倍

    g_u32HundreUsfor = (1024<<7)/g_u32CycleSpeed;
}

//----初始化tick---------------------------------------------------------------
//功能: 初始化定时器,并连接tick中断函数,启动定时器.
//参数: 无
//返回: 无
//备注: 本函数是移植敏感函数.
//-----------------------------------------------------------------------------
__attribute__((weak)) void __DjyInitTick(void)
{
    HardExp_ConnectSystick(Djy_IsrTick);
    pg_systick_reg->reload = CN_CFG_FCLK/CN_CFG_TICK_HZ;
    pg_systick_reg->current =CN_CFG_FCLK/CN_CFG_TICK_HZ;
    pg_systick_reg->ctrl =   (1<<bo_systick_ctrl_enable)    //使能
                                |(1<<bo_systick_ctrl_tickint)   //允许产生中断
                                |(1<<bo_systick_ctrl_clksource);//用使用外部时钟

}

//----读取当前时间(uS)---------------------------------------------------------
//功能：读取当前时间(uS),从计算机启动以来经历的us数，64位，默认不会溢出
//      g_s64OsTicks 为64位变量，非64位系统中，读取 g_s64OsTicks 需要超过1个
//      周期,需要使用原子操作。
//参数：无
//返回：当前时钟
//说明: 这是一个桩函数,被systime.c文件的 DjyGetSysTime 函数调用。
//      如果systime不使用ticks作为时基，本函数可保持空函数。
//-----------------------------------------------------------------------------
__attribute__((weak)) s64 __DjyGetSysTime(void)
{
    s64 time;
    static s64 BakTime = 0;
    u32 temp;
    atom_low_t atom_low;
    temp =CN_CFG_FCLK/CN_CFG_TICK_HZ - pg_systick_reg->current;

    atom_low = Int_LowAtomStart();
    time = g_s64OsTicks;
    Int_LowAtomEnd(atom_low);
    time = time*CN_CFG_TICK_US + (temp*CN_CFG_FINE_US >>16);
    if(time < BakTime)
        time += CN_CFG_TICK_US;
    BakTime = time;

    return time;
}



extern void Load_Preload(void);
#include <exp.h>
#include <osboot.h>
#include <IAP_Inner.h>
extern tagIapVar pg_IapVar;

#define CN_BOOT_SOFTREBOOT_FLAG         (CN_BOOT_LEGALKEY <<1)
#define CN_BOOT_SOFTRELOAD_FLAG         (CN_BOOT_LEGALKEY <<2)
#define CN_BOOT_HARDRST_FLAG         (CN_BOOT_LEGALKEY <<3)
#define CN_BOOT_SOFTRESET_FLAG          (CN_BOOT_LEGALKEY <<4)

// =============================================================================
// 功能：运行到选择系统运行方式前，对于M3/M4的CPU，即PC跳转到Init_CPU()
// 参数：无
// 返回：无
// =============================================================================
void reboot(u32 key)
{
    vu32 *addr;
    vu32 value;

    if(key != CN_BOOT_LEGALKEY)
    {
        printf("IllegalKey:0x%08x Recorded\n\r",key);
        ThrowOsBootInfo(EN_BOOT_REBOOT);
    }
    else
    {
        addr = (vu32 *)&pg_IapVar.IbootFlag[12];
        value = CN_BOOT_SOFTREBOOT_FLAG;
        *addr = value;
        value = *addr;
        Djy_DelayUs(10);
        SCB_DisableDCache();
        u32 InitCpu_Addr;
        InitCpu_Addr = *(u32*)0x00000004;
        ((void (*)(void))(InitCpu_Addr))();
    }
    return;
}
// =============================================================================
// 功能：Reset硬件CPU，相当于上电重新启动，硬件软件都得到复位
// 参数：无
// 返回：无
// =============================================================================
void reset(u32 key)
{
    vu32 *addr;
    vu32 value;

    if(key != CN_BOOT_LEGALKEY)
    {
        printf("IllegalKey:0x%08x Recorded\n\r",key);
        ThrowOsBootInfo(EN_BOOT_SRST);
    }
    else
    {
        addr = (vu32 *)&pg_IapVar.IbootFlag[12];
        value = CN_BOOT_SOFTRESET_FLAG;
        *addr = value;
        value = *addr;
        Djy_DelayUs(10);
        pg_scb_reg->AIRCR = (0x05FA << 16)|(0x01 << bo_scb_aircr_sysresetreq);
    }
    return;
}
// =============================================================================
// 功能：运行到CPU加载代码前，即pre_load()前
// 参数：无
// 返回：无
// =============================================================================
void restart_system(u32 key)
{
    vu32 *addr;
    vu32 value;

    if(key != CN_BOOT_LEGALKEY)
    {
        printf("IllegalKey:0x%08x Recorded\n\r",key);
        ThrowOsBootInfo(EN_BOOT_RELOAD);
    }
    else
    {
        addr = (vu32 *)&pg_IapVar.IbootFlag[12];
        value = CN_BOOT_SOFTRELOAD_FLAG;
        *addr = value;
        value = *addr;
        Djy_DelayUs(10);
        __set_PSP((uint32_t)msp_top);
        __set_MSP((uint32_t)msp_top);
        Load_Preload();
    }
    return;
}

enBootMode GetBootMethodSoft(void)
{
    vu32 *addr;
    vu32 value;
    enBootMode result;
    addr = (u32*)&pg_IapVar.IbootFlag[12];
    value = *addr;
    if(value == CN_BOOT_SOFTRESET_FLAG)
    {
        result = EN_BOOT_SRST;
    }
    else if(value == CN_BOOT_SOFTREBOOT_FLAG)
    {
        result = EN_BOOT_REBOOT;
    }
    else if(value == CN_BOOT_SOFTRELOAD_FLAG)
    {
        result = EN_BOOT_RELOAD;
    }
    else if(value == CN_BOOT_HARDRST_FLAG)
    {
        result = EN_BOOT_HRST;
    }
    else
    {
        result = EN_BOOT_POWERDOWN;
    }
    value = CN_BOOT_HARDRST_FLAG;
    *addr = value;
    return result;
}
