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
// 这份授权条款，在使用者符合以下二条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、此三条件表列，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、此三条件表列，以及下述
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
// =============================================================================

// 文件名     ：cpu_peri_spi.c
// 模块描述: SPI模块底层硬件驱动模块
// 模块版本: V1spi
// 创建人员:
// 创建时间:
// =============================================================================
#ifdef CFG_CPU_ZQ12XX_M0
#include "stdint.h"
#include "stddef.h"
#include "systime.h"
#include "wdt_hal.h"
#include "cpu_peri.h"

typedef  bool_t (*fnWdtChip_Feed)(void);

//定义喂狗看门狗周期
#define CN_WDT_WDTCYCLE (0xffffffff)

//系统配置相关
#define SYS_CTR2        *((vu32*)(0x4204000c))
#define CTR_REG2        *((vu32*)(0x42010000 + 0x08))

typedef struct _WatDog_{

    vu32 WDOGLOAD;
    vu32 WDOGVALUE;
    vu32 WDOGCONTROL;
    vu32 WDOGINTCLR;
    vu32 WDOGRIS;
    vu32 WDOGMIS;

}tagWatDogReg;

#define  BASE_WDT_ADDR (0x41870000)

static tagWatDogReg* WatReg[1] = {
    (tagWatDogReg*)BASE_WDT_ADDR,
};

//watDog lock reg
typedef struct _WatDog_Lock_{
    vu32 WDOGLOCK;
}tagWatDogLock;

#define WDT_LOCK_ADDR   (BASE_WDT_ADDR + 0xC00)

static tagWatDogLock* WatLockReg[1] = {

    (tagWatDogLock*)WDT_LOCK_ADDR,
};


// =============================================================================
//功能: 启动看门狗
//参数：无
//返回：无
// =============================================================================

void WDT_Start(void)
{
    tagWatDogReg  *Reg     = NULL;
    tagWatDogLock *LockReg = NULL;
    u32 tmp;
    printk("start watDog\r\n");

    Reg     = (tagWatDogReg  *)WatReg[0];
    LockReg = (tagWatDogLock *)WatLockReg[0];

    //first to open the lock
    LockReg->WDOGLOCK = 0x1ACCE551;

    //enable counter and interrupt
    Reg->WDOGCONTROL |= (1<<0);

    //enable reset
    Reg->WDOGCONTROL |= (1<< 1);

    //read diable again
    LockReg->WDOGLOCK = 0x38;
    tmp = LockReg->WDOGLOCK;
    printk("WDT_Start: %d\r\n",tmp);
    //wait for lock again
    //while(!(tmp & 0x00000001 == 0x00000001));

    printk("start dog:%x\r\n",Reg->WDOGVALUE);

}

// =============================================================================
//功能: 软件复位。
//参数：无
//返回：无
//注意: 通过调用复位函数实现，真正的复位函数，不是在这里实现
// =============================================================================
void WDT_Reboot(void)
{
    while(1);//到时候，可以调用复位函数，这里先用while(1)代替
}

// =============================================================================
//功能: 看门狗喂狗函数
//参数：无
//返回：无
// =============================================================================

bool_t WDT_WdtFeed(void)
{
    tagWatDogReg  *Reg     = NULL;
    tagWatDogLock *LockReg = NULL;
    u32 tmp = 0;
    u32 setVal;

    if(setVal == 0){

        setVal  = 0xffffffff;
    }

    //first open the lock and then to write
    LockReg->WDOGLOCK = 0x1ACCE551;
    printk("feed dog %x\r\n",LockReg->WDOGLOCK);

    Reg     = WatReg[0];
    LockReg = WatLockReg[0];

    printk("WDT FEED!\r\n");

    //reset the reload value
    Reg->WDOGLOAD   = setVal;

    //write anything to clear int flag
    Reg->WDOGINTCLR = 0x38;
    //read diable again
    tmp = LockReg->WDOGLOCK;
    //wait for lock again
    //while(!(tmp & 0x00000001 == 0x00000001));

    printk("feed dog %x\r\n",setVal);
    printk("---->%d\r\n",tmp);

    return true;
}


// =============================================================================
//功能: 看门狗硬件初始化
//参数：无
//返回：无
//注意: 根据用户的看门狗初始化，然后和djyos提供的条件连接
// =============================================================================

static void WDT_HardInit(void)
{
    u8 WatDogCnt;
    tagWatDogReg  *Reg     = NULL;
    tagWatDogLock *LockReg = NULL;
    u32 tmp;

    //enable watDog clock
    CTR_REG2 |= (1<<26);
    //enable watDog to reset system
    SYS_CTR2 |= (1<<28);

    //disable all the WatDog

    Reg      =   WatReg[0];
    LockReg  =   WatLockReg[0];

    //first open the lock and then to write
    LockReg->WDOGLOCK = 0x1ACCE551;

    Reg->WDOGLOAD = 0x00000f00;
    printk("WatDog%d WDT_HardInit set wdtLoad %x\r\n",WatDogCnt,Reg->WDOGLOAD);
    printk("WatDog%d WDT_HardInit current watDog count value %x\r\n",WatDogCnt,Reg->WDOGVALUE);

    //diable watdog
    Reg->WDOGCONTROL &= ~((1<<0) | (1<<1));

    //read diable again
    LockReg->WDOGLOCK = 0x38;
    tmp = LockReg->WDOGLOCK;
    //wait for lock again
    while(!(tmp & 0x00000001 == 0x00000001));
    printk("WatDog%d WDT_HardInit: %d\r\n",WatDogCnt,tmp);

}

//**************************************************//
//todo：这三个函数用于实现保护加载过程，使用的时候需补全
u32 __FeedDog_Isr(ptu32_t intline)
{
    return false;
}
bool_t __BrdBoot_FeedStart(u32 bootfeedtime)
{
    return false;
}

bool_t __BrdBoot_FeedEnd(void)
{
    return false;
}
//**************************************************//

// =============================================================================
// 函数功能: 看门狗注册
// 输入参数:
// 返回值  :true成功false失败
// =============================================================================

bool_t WatDog_Install(void)
{
    bool_t result;

    WDT_HardInit();

//  result = WdtHal_RegisterWdtChip(pgWatDogName[0],CN_WDT_WDTCYCLE,g_WatDogFeedHandle[0],NULL,NULL);
    result = WdtHal_RegisterWdtChip("zqM0WatDog1", CN_WDT_WDTCYCLE, WDT_WdtFeed);
#if(CFG_DEFEND_ON_BOOT == true)
    __BrdBoot_FeedEnd();
#endif

   return result;

}

#endif














































































































































