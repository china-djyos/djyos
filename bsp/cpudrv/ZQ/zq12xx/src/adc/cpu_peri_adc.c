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
// Copyright (c) 2018£¬Öø×÷È¨ÓÉ¶¼½­Ñß²Ù×÷ÏµÍ³¿ªÔ´¿ª·¢ÍÅ¶ÓËùÓÐ¡£Öø×÷È¨ÈË±£ÁôÒ»ÇÐÈ¨Àû¡£
//
// Õâ·ÝÊÚÈ¨Ìõ¿î£¬ÔÚÊ¹ÓÃÕß·ûºÏÒÔÏÂÈýÌõ¼þµÄÇéÐÎÏÂ£¬ÊÚÓèÊ¹ÓÃÕßÊ¹ÓÃ¼°ÔÙÉ¢²¥±¾
// Èí¼þ°ü×°Ô­Ê¼Âë¼°¶þ½øÎ»¿ÉÖ´ÐÐÐÎÊ½µÄÈ¨Àû£¬ÎÞÂÛ´Ë°ü×°ÊÇ·ñ¾­¸Ä×÷½ÔÈ»£º
//
// 1. ¶ÔÓÚ±¾Èí¼þÔ´´úÂëµÄÔÙÉ¢²¥£¬±ØÐë±£ÁôÉÏÊöµÄ°æÈ¨Ðû¸æ¡¢±¾Ìõ¼þÁÐ±í£¬ÒÔ
//    ¼°ÏÂÊöµÄÃâÔðÉùÃ÷¡£
// 2. ¶ÔÓÚ±¾Ì×¼þ¶þ½øÎ»¿ÉÖ´ÐÐÐÎÊ½µÄÔÙÉ¢²¥£¬±ØÐëÁ¬´øÒÔÎÄ¼þÒÔ¼°£¯»òÕßÆäËû¸½
//    ÓÚÉ¢²¥°ü×°ÖÐµÄÃ½½é·½Ê½£¬ÖØÖÆÉÏÊöÖ®°æÈ¨Ðû¸æ¡¢±¾Ìõ¼þÁÐ±í£¬ÒÔ¼°ÏÂÊö
//    µÄÃâÔðÉùÃ÷¡£
// 3. ±¾Èí¼þ×÷Îª¶¼½­Ñß²Ù×÷ÏµÍ³µÄ×é³É²¿·Ö£¬Î´»ñÊÂÇ°È¡µÃµÄÊéÃæÐí¿É£¬²»ÔÊÐíÒÆÖ²µ½·Ç
//    ¶¼½­Ñß²Ù×÷ÏµÍ³»·¾³ÏÂÔËÐÐ¡£

// ÃâÔðÉùÃ÷£º±¾Èí¼þÊÇ±¾Èí¼þ°æÈ¨³ÖÓÐÈËÒÔ¼°¹±Ï×ÕßÒÔÏÖ×´£¨"as is"£©Ìá¹©£¬
// ±¾Èí¼þ°ü×°²»¸ºÈÎºÎÃ÷Ê¾»òÄ¬Ê¾Ö®µ£±£ÔðÈÎ£¬°üÀ¨µ«²»ÏÞÓÚ¾ÍÊÊÊÛÐÔÒÔ¼°ÌØ¶¨Ä¿
// µÄµÄÊÊÓÃÐÔÎªÄ¬Ê¾ÐÔµ£±£¡£°æÈ¨³ÖÓÐÈË¼°±¾Èí¼þÖ®¹±Ï×Õß£¬ÎÞÂÛÈÎºÎÌõ¼þ¡¢
// ÎÞÂÛ³ÉÒò»òÈÎºÎÔðÈÎÖ÷Òå¡¢ÎÞÂÛ´ËÔðÈÎÎªÒòºÏÔ¼¹ØÏµ¡¢ÎÞ¹ýÊ§ÔðÈÎÖ÷Òå»òÒò·ÇÎ¥
// Ô¼Ö®ÇÖÈ¨£¨°üÀ¨¹ýÊ§»òÆäËûÔ­ÒòµÈ£©¶øÆð£¬¶ÔÓÚÈÎºÎÒòÊ¹ÓÃ±¾Èí¼þ°ü×°Ëù²úÉúµÄ
// ÈÎºÎÖ±½ÓÐÔ¡¢¼ä½ÓÐÔ¡¢Å¼·¢ÐÔ¡¢ÌØÊâÐÔ¡¢³Í·£ÐÔ»òÈÎºÎ½á¹ûµÄËðº¦£¨°üÀ¨µ«²»ÏÞ
// ÓÚÌæ´úÉÌÆ·»òÀÍÎñÖ®¹ºÓÃ¡¢Ê¹ÓÃËðÊ§¡¢×ÊÁÏËðÊ§¡¢ÀûÒæËðÊ§¡¢ÒµÎñÖÐ¶ÏµÈµÈ£©£¬
// ²»¸ºÈÎºÎÔðÈÎ£¬¼´ÔÚ¸ÃÖÖÊ¹ÓÃÒÑ»ñÊÂÇ°¸æÖª¿ÉÄÜ»áÔì³É´ËÀàËðº¦µÄÇéÐÎÏÂÒàÈ»¡£
//-----------------------------------------------------------------------------

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
    vu32 ADC_RES[0x39];     //57->((0x100 - 0x018)/4)-1 æ·‡æ¿ˆæš€
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

    sADCReg->ADC_CFG &= ~(1 << 9);          //éšîˆšå§©AD æžî„å´²

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






















