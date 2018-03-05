﻿//----------------------------------------------------
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

#include <stdint.h>
#include <cpu_peri.h>
#include <stddef.h>
#include <cpu_peri_gpio.h>


#define INSERTED						(s32)1 // USB设备插入
#define USB_DM							&USB_PINS[0]
#define USB_DP							&USB_PINS[1]
#define USB_POWER						&USB_PINS[2]

static const Pin USB_PINS[] = {
		{GPIO_B, PIN14, GPIO_MODE_IN, GPIO_OTYPE_PP, GPIO_SPEED_VH, GPIO_PUPD_PD, AF_NUll},
		{GPIO_B, PIN15, GPIO_MODE_IN, GPIO_OTYPE_PP, GPIO_SPEED_VH, GPIO_PUPD_PD, AF_NUll},
		{GPIO_H, PIN3, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH, GPIO_PUPD_NONE, AF_NUll},
};

//-----------------------------------------------------------------------------
//功能: 本地延时
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
static void Wait(u32 dwTime)
{
	volatile u32 i, j;

	for(j = 0; j < dwTime; j++)
		for(i = 0; i < 60000; i++);
}
//-----------------------------------------------------------------------------
//功能: USB检测IO口初始化
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
static void USB_DetectorInit(void)
{
	PIO_Configure(USB_PINS, PIO_LISTSIZE(USB_PINS));
}

//-----------------------------------------------------------------------------
//功能: USB上电
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
void USB_PowerOn(void)
{
	PIO_Clear(USB_POWER); // USB供电
	Wait(300); // 等待USB完全上电，有些USB设备的上电速度会慢些
}

//-----------------------------------------------------------------------------
//功能: USB断电
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
void USB_PowerOff(void)
{
	PIO_Set(USB_POWER);
}
//-----------------------------------------------------------------------------
//功能: 查询USB是否被插入
//参数:
//返回: 1 -- 插入； 0 -- 未插入
//备注:
//-----------------------------------------------------------------------------
static s32 USB_State(void)
{
	s32 dm, dp;

	USB_PowerOn();
	dm = PIO_Get(USB_DM);
	dp = PIO_Get(USB_DP);

	if(dm || dp)
		return (1); // USB已经插入，不考虑是否
	
	USB_PowerOff(); // USB未插入，USB断电
	return (0);
}

//-----------------------------------------------------------------------------
//功能: 查询USB是否被插入
//参数:
//返回: 1 -- 插入； 0 -- 未插入
//备注:
//-----------------------------------------------------------------------------
s32 USB_Detector(void)
{
	USB_DetectorInit();
		
	if(INSERTED == USB_State())
		return (1);
		
	return (0);	
}
