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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��
// ģ������:
// ģ��汾: V1.00
// ������Ա:
// ����ʱ��:
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

//ͨ��ʹ��ѡ��
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

//app ���� set pwm channel number
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

//app ����
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

//app ����
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


//app ����
void appPWM_ChannelPreDivSet(u32 preDiv)
{
	u32 tmpPreDiv;
	tmpPreDiv = preDiv;

	//��ʱ����������������������ֻȡǰ��8byte
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




































































































































































































































