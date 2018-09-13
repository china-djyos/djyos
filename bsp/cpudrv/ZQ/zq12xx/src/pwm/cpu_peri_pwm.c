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
// =============================================================================
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 文件名     ：
// 模块描述:
// 模块版本: V1.00
// 创建人员:
// 创建时间:
// =============================================================================

#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
#include "cpu_peri_pwm.h"
#include "djyos.h"
#include "stdio.h"

//#include "silan_pwm_regs.h"
//#include "silan_types.h"
#include "silan_syscfg.h"
#include "silan_iomux.h"

//Reg define
typedef struct
{
	vu32  PWMPSC;    //0x00
	vu32  PWM0D;      //0x04
	vu32  PWM1D;     //0x08
	vu32  PWM2D;     //0x0c
	vu32  PWM3D;     //0x10
	vu32  PWM4D;     //0x14
	vu32  PWM5D;  	 //0x18
	vu32  PWM6D;		 //0x1c
	vu32  PWM7D;		 //0x20
	vu32  PWMP01;	 //0x24
	vu32  PWMP23;	 //0x28
	vu32  PWMP45;	 //0x2c
	vu32  PWMP67;	 //0x30
	vu32  PWMCON;	 //0x34
}tagPWMReg;

#define CN_PWM_CHANNEL_START	     ( CN_PWM_CHANNEL0 )
#define CN_PWM_CHANNEL_END		 ( CN_PWM_CHANNEL7 )

//channel number select
#define CHANNEL_NUM_GET(channelNum)     (1<< (channelNum+4) )

//base PWM base addr
#define BASE_PWM_ADDR  ( 0x410D0000 )

static tagPWMReg volatile *const sPWMReg = (tagPWMReg*) BASE_PWM_ADDR;

//通道使能选择
static void __CPU_PWM_ChannelOpen(u8 channelNum,u8 onOff)
{
	if(onOff == EN_CHANNEL_ON)
	{
		sPWMReg->PWMCON  |= CHANNEL_NUM_GET(channelNum);
		return ;
	}else if(onOff == EN_CHANNEL_OFF){

		sPWMReg->PWMCON  &= ~(CHANNEL_NUM_GET(channelNum));
		return ;
	}

	printk("__CPU_PWM_ChannelOpen func argc onOff Err\r\n");
}

static void __CPU_PWM_ChannelParaSet(u8 channelNum,u32 para)
{
	switch(channelNum)
	{
			case CN_PWM_CHANNEL0: sPWMReg->PWM0D = (para-1);break;
			case CN_PWM_CHANNEL1: sPWMReg->PWM1D = (para-1);break;
			case CN_PWM_CHANNEL2: sPWMReg->PWM2D = (para-1);break;
			case CN_PWM_CHANNEL3: sPWMReg->PWM3D = (para-1);break;
			case CN_PWM_CHANNEL4: sPWMReg->PWM4D = (para-1);break;
			case CN_PWM_CHANNEL5: sPWMReg->PWM5D = (para-1);break;
			case CN_PWM_CHANNEL6: sPWMReg->PWM6D = (para-1);break;
			case CN_PWM_CHANNEL7: sPWMReg->PWM7D = (para-1);break;
			default:printk("__CPU_ChannelParaSet func arg channelNum Err\r\n");
			return;
	}

}

static void __CPU_PWM_ChannelPeriodSet(u8 channelNum,u32 para)
{
	u8 channelSet;
	channelSet = (u8)(channelNum/2);

    switch(channelSet)
    {
			case CN_PWM_CHANNEL0: sPWMReg->PWMP01 = (para-1);break;
			case CN_PWM_CHANNEL1: sPWMReg->PWMP23 = (para-1);break;
			case CN_PWM_CHANNEL2: sPWMReg->PWMP45 = (para-1);break;
			case CN_PWM_CHANNEL3: sPWMReg->PWMP67 = (para-1);break;
			default:printk("__CPU_ChannelPeriodSet func arg channelNum Err\r\n");
			return;
    }
}

static void __CPU_PWM_PreDivSet(u8 preDiv)
{
	sPWMReg->PWMPSC = preDiv -1;
}

static void __CPU_PWM_IO_Config(u32 io, u32 x_para, u32 x_period,u8 onOff)
{
		u32 ioNumber =  io;

		ioNumber = ioNumber % 8; 								//make sure to be :0~7

		if(x_para == 0 || x_period == 0)
		{
			printk("__CPU_PWM_IO_Config func arg x_para or x_period Err\r\n");
			return;
		}else{

			__CPU_PWM_ChannelParaSet(ioNumber,x_para);
			__CPU_PWM_ChannelPeriodSet(ioNumber,x_period);
			__CPU_PWM_ChannelOpen(ioNumber,onOff);
			if(onOff == EN_CHANNEL_ON)
				silan_io_func_config(ioNumber, IO_FUNC_PWM);
		}

}

//app 调用 set pwm channel number
void appPWM_ChannelOpen(u8 channelNum,u8 onOff)
{
	if(onOff == EN_CHANNEL_OFF || onOff ==  EN_CHANNEL_ON)
	{
		if(channelNum >=CN_PWM_CHANNEL_START  && channelNum <= CN_PWM_CHANNEL_END)
		{
			__CPU_PWM_ChannelOpen(channelNum,onOff);
		}
		else{
			  printk("appPWM_ChannelOpen func arg channelNum Err\r\n");
			  return;
		}
	}else{

          printk("appPWM_ChannelOpen func arg onOff Err\r\n");
          return;
	}
}

//app 调用
void appPWM_ChannelParaSet(u8 channelNum,u32 para)
{
	 if(channelNum >=CN_PWM_CHANNEL_START  && channelNum <= CN_PWM_CHANNEL_END)
	 {
		 __CPU_PWM_ChannelParaSet(channelNum,para);
	 }
	else{
			  printk("appPWM_ChannelOpen func arg channelNum Err\r\n");
			  return;
	 }

}

//app 调用
void appPWM_ChannelPeriodSet(u8 channelNum,u32 para)
{
	if(channelNum >= CN_PWM_CHANNEL_START || channelNum <= 	CN_PWM_CHANNEL_END)
	{
		__CPU_PWM_ChannelPeriodSet(channelNum,para);
	}else{
        printk("appPWM_ChannelPeriodSet Func channelNum Err\r\n");
        return ;
	}

}


//app 调用
void appPWM_ChannelPreDivSet(u32 preDiv)
{
	u32 tmpPreDiv;
	tmpPreDiv = preDiv;

	//暂时不做其他处理，若参数过大只取前面8byte
	if(preDiv > 256)
	{
		printk("appPWM_ChannelPreDivSet Func arg para out of range\r\n");
	}
	CPU_PWM_PreDivSet(tmpPreDiv& 0xff);

}

void appPWM_IO_Config(u32 io, u32 x_para, u32 x_period,u8 onOff)
{
	 __CPU_PWM_IO_Config(io,x_para,  x_period,onOff);
}




































































































































































































































