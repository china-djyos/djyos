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
// 这份授权条款，在使用者符合以下三条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。
// 3. 本软件作为都江堰操作系统的组成部分，未获事前取得的书面许可，不允许移植到非
//    都江堰操作系统环境下运行。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
#ifdef CFG_CPU_ZQ12XX_M0
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
#include "cpu_peri_adc.h"
#include "djyos.h"
#include "stdio.h"
#include "int.h"


#include "silan_adc_regs.h"
#include "silan_adc.h"
#include "silan_syscfg.h"
#include "silan_syscfg_regs.h"
#include "silan_pmu.h"
#include "silan_pmu_regs.h"



typedef struct{

    vu32 ADC_CFG;           //0x000
    vu32 ADC_STS;           //0x004
    vu32 ADC_DATA;          //0x008
    vu32 ADC_INT_RSTS;      //0x00c
    vu32 ADC_INT_MSTS;      //0x010
    vu32 ADC_INT_MSK;       //0x014
    vu32 ADC_INT_CLR;       //0x018
    vu32 ADC_RES[0x39];     //57->((0x100 - 0x018)/4)-1 娣囨繄鏆?
    vu32 ADC_SEQ_CFG;       //0x100
    vu32 ADC_SEQ_RES1;      //0x104
    vu32 ADC_SEQ_PERIOD;    //0x108
    vu32 ADC_SEQ_THR;       //0x10c
    vu32 ADC_SEQ_DATA;      //0x110

}tagADCReg;

#define BASE_ADC_ADDR       (0x410C0000)

static   tagADCReg volatile *const sADCReg = (tagADCReg volatile *)BASE_ADC_ADDR;

static void __CPU_ADC_Init(tagVef vref)
{
    silan_adc_cclk_config(ADC_CCLK_PLLREF_DIV2, CLK_ON);   // sar adc fs   3:40kHz   2:6MHz
    silan_pmu_wakeup_disable(PMU_WAKEUP_KEY);
    silan_sar_adc_vref_select(vref);

    silan_soft_rst(SOFT_RST_ADC);
}

static bool_t __CPU_ADC_Reset(void)
{
    __REG32(SILAN_SYSCFG_REG9)&=~(0x3<<24);
    __REG32(SILAN_SYSCFG_REG9)|=0x3<<24;
  return true;
}

static bool_t __CPU_ADC_SetChannel(u8 channel)
{
    bool_t retVal =  false;

    if(channel > CN_AD_CHANNEL7)
    {
        printk("__CPU_ADC_SetChannel func arg channel Err\r\n");
        return retVal;
    }
    retVal = true;
    //channel config bit 4~6
    sADCReg->ADC_CFG &= ~(0x7 << 4);
    sADCReg->ADC_CFG |= ((channel & 0x7) << 4);

    return retVal;
}

static bool_t __CPU_ADC_ReadDat(u8 channel,u32 dat)
{
    u32 time_out;
    volatile u32 wait_loop;
    u32 readDat      = CN_RAED_ERR_DAT;
    u32 *ptReadDat = NULL;
    bool_t retVal = false;

    ptReadDat =  (u32*)dat;
    //reset
    __CPU_ADC_Reset();

    //ADC module Power Down -->1 effect
    sADCReg->ADC_CFG  = 1;
    //ADC stop module Power Down
    sADCReg->ADC_CFG &= ~(1<<0);
    //set ADC control register contrl derectly .bit  ADC_START contrl
    sADCReg->ADC_CFG  =  (1<<7);

    //channel config bit 4~6
    __CPU_ADC_SetChannel(channel);

    //enter lowfreq mode ,wait_loop= 1024/current_freq*low_freq
//  atom_high_t  adatom;
//  adatom = Int_HighAtomStart();
    for(wait_loop=1024;wait_loop>0;wait_loop--);//delay 1024 cycles
//  Int_HighAtomEnd(adatom);

    sADCReg->ADC_CFG |= (1 << 8);
    sADCReg->ADC_CFG |= (1 << 9);

//  adatom = Int_HighAtomStart();
    for(wait_loop=1024;wait_loop>0;wait_loop--);
//  Int_HighAtomEnd(adatom);

    sADCReg->ADC_CFG &= ~(1 << 9);          //閸氼垰濮〢D 鏉烆剚宕?

    time_out = 0x1000;

//  adatom = Int_HighAtomStart();
    while(!(sADCReg->ADC_INT_RSTS & (0x2)))
    {
        time_out--;
        if(time_out == 0)//out time
        {
            printk("adc read data outtime\r\n");
//          Int_HighAtomEnd(adatom);  //
            return retVal;
        }
    }

    //get ad data
    readDat           = sADCReg->ADC_DATA;
//  Int_HighAtomEnd(adatom);  //

    ptReadDat [0] = readDat;
    sADCReg->ADC_INT_CLR = 0x2;
    retVal            =true;

    return retVal;
}

static bool_t __CPU_ADC_PinConfig(u8 pin)
{
    bool_t retVal = false;
    u8 cfgPin;

    //support for PA0~PA7
    if(pin > CN_AD_CFG_PA7)
    {
        printk("__CPU_ADC_PinConfig func arg pin Err\r\n");
        return retVal;
    }else{

        retVal = true;
        cfgPin = pin;
    }

    silan_iomux_adc_open(cfgPin);

    return retVal;
}


void __CPU_ADC_Ctrl(tagADCReg volatile *const Reg,u32 cmd, u32 data1,u32 data2)
{
    data1 = data1;
    data2 = data2;

    switch(cmd)
    {
        case CN_ADC_CHANNEL_SET:       __CPU_ADC_SetChannel(data1);         break;
        case CN_ADC_CHANNEL_RETSET: __CPU_ADC_Reset();                          break;
        case CN_ADC_CHANNEL_PINCFG:__CPU_ADC_PinConfig( data1);         break;
        case CN_ADC_CHANNEL_RD_DAT:__CPU_ADC_ReadDat(data1,data2);  break;
        default:printk("not exist the cmd \r\n");break;
    }

}

void appADC_Ctrl(u32 handle,u32 cmd, u32 data1,u32 data2)
{
    printk("call appADC_Ctrl\r\n");
    __CPU_ADC_Ctrl(NULL,cmd, data1, data2);
}

void ModuleInstall_ADC(tagVef vref)
{
    __CPU_ADC_Init(vref);
}
#endif






















