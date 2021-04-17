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
// Copyright (c) 2018，著作权由都江堰操作系统�?源开发团队所有�?�著作权人保留一切权利�??
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然�?
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明�?
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或�?�其他附
//    于散播包装中的媒介方式，重制上述之版权宣告�?�本条件列表，以及下�?
//    的免责声明�??

// 免责声明：本软件是本软件版权持有人以及贡献�?�以现状�?"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就�?�售性以及特定目
// 的的适用性为默示性担保�?�版权持有人及本软件之贡献�?�，无论任何条件�?
// 无论成因或任何责任主义�?�无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生�?
// 任何直接性�?�间接�?��?�偶发�?��?�特殊�?��?�惩罚�?�或任何结果的损害（包括但不�?
// 于替代商品或劳务之购用�?�使用损失�?�资料损失�?�利益损失�?�业务中断等等）�?
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然�?
//-----------------------------------------------------------------------------
#include "stdio.h"
#include "djyos.h"
#include "int.h"
#include "blackbox.h"
#include "csi_core.h"
#include "silan_irq.h"

#include "board-config.h"
#if (CN_USE_TICKLESS_MODE)
#include "tickless.h"
#endif

#undef  VIC_TSPR
#define VIC_TSPR  0xE000EC10

extern struct IntMasterCtrl  tg_int_global;          
extern void __DJY_ScheduleAsynSignal(void);
extern   void Init_Cpu(void);
extern   void HardExp_HardfaultHandler(void);
extern   void CK_Trap0ISR(void);
extern u32 __Vectors_Tbl[];

void (*user_systick)(u32 inc_ticks);

#define readl(addr) \
    ({ unsigned int __v = (*(volatile unsigned int *) (addr)); __v; })

static void __Exp_TableSet(void)
{
    __set_VBR((uint32_t) & (__Vectors_Tbl));
    /* Here we may setting exception vector, MGU, cache, and so on. */
    csi_cache_set_range(0, 0x00000000, CACHE_CRCR_4M, 1);
    csi_cache_set_range(1, 0x22000000, CACHE_CRCR_8M, 1);
    csi_dcache_enable();
    csi_icache_enable();

    //*((uint32_t*)VIC_TSPR) = 0xb00;
}


void HardExp_ConnectSystick(void (*tick)(u32 inc_ticks))
{
    user_systick = tick;
}

void Exp_SystickTickHandler(void)
{
#if (CN_USE_TICKLESS_MODE)
    u32 tick=0;
#endif
    g_bScheduleEnable = false;
    csi_vic_clear_pending_irq(PIC_IRQID_CTIMER);
    readl(0xE000E010);
//    tg_int_global.en_asyn_signal = false;
    tg_int_global.en_asyn_signal_counter = 1;
    tg_int_global.nest_asyn_signal = 1;
    if(!DjyGetUpdateTickFlag())
        DjyUpdateTicks(1);
    else
        DjySetUpdateTickFlag(false);
    user_systick((CN_USE_TICKLESS_MODE==1)?0:1);

    tg_int_global.nest_asyn_signal = 0;
//    tg_int_global.en_asyn_signal = true;
    tg_int_global.en_asyn_signal_counter = 0;
//  if(g_ptEventReady != g_ptEventRunning)
//      __DJY_ScheduleAsynSignal();       
    g_bScheduleEnable = true;
}

void HardExp_Init(void)
{
    __Exp_TableSet();
}

enum EN_BlackBoxAction Exp_MemManageFaultHandler(u32 fpu_used,u32 *core_info)
{
    return EN_BLACKBOX_DEAL_IGNORE;
}

enum EN_BlackBoxAction Exp_HardFaultHandler(u32 fpu_used,u32 *core_info)
{
    return EN_BLACKBOX_DEAL_IGNORE;
}

enum EN_BlackBoxAction Exp_DebugFaultHandler(u32 fpu_used,u32 *core_info)
{
    return EN_BLACKBOX_DEAL_IGNORE;
}

enum EN_BlackBoxAction Exp_BusFaultHandler(u32 fpu_used,u32 *core_info)
{
    return EN_BLACKBOX_DEAL_IGNORE;
}

enum EN_BlackBoxAction Exp_UsageFaultHandler(u32 fpu_used,u32 *core_info)
{
    return EN_BLACKBOX_DEAL_IGNORE;
}

bool_t  HardExp_Analysis(struct ExpThrowPara *parahead, u32 endian)
{
    return true;
}

bool_t  HardExp_Decoder(struct ExpThrowPara *parahead, u32 endian)
{
    return true;
}

bool_t HardExp_InfoDecoderInit(void)
{
    return true;
}

bool_t HardExp_Trap_c(uint32_t *regs)
{
    int i = 0, j = 0;
    int32_t inc = 0;
    uint32_t vec = 0;
    asm volatile(
        "mfcr    %0, psr \n"
        "lsri    %0, 16 \n"
        "sextb   %0 \n"
        :"=r"(vec):);

    printk("CPU Exception : %d", vec);
    printk("\n");

    for (; i < (16 + inc); i++, j++) {
        printk("r%d: %08x\t", j, regs[i]);

        if ((i % 5) == 4) {
            printk("\n");
        }
    }
    inc += 16;

    for (; i < (16 + inc); i++, j++) {
        printk("r%d: %08x\t", j, regs[i]);

        if ((i % 5) == 4) {
            printk("\n");
        }
    }

    printk("\n");
    printk("epsr: %8x\n", regs[i]);
    printk("epc : %8x\n", regs[i + 1]);

    while (1);
    return true;
}
