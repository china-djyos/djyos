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
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 文件名     ：powerctrl.h
// 模块描述: SR2711板件电源管理控制
// 模块版本: V1.00
// 创建人员: zhb
// 创建时间: 8/14.2017
// =============================================================================
#ifndef __POWER_CTRL_H__
#define __POWER_CTRL_H__

#include "stdint.h"


#ifdef __cplusplus
extern "C" {
#endif

//#define In_LowerPower       1
//#define Out_LowerPower      0

enum BoardPeriCode
{
	EN_PERI_SDRAM=0,             //SDRAM
	EN_PERI_EMMC,              //EMMC
	EN_PERI_EMAC,              //EMAC
	EN_PERI_IMAGE_PROCESS,     //模拟图像处理芯片
	EN_PERI_CAMERA_HEADER,     //摄像头球机
	EN_PERI_30W_POWER,         //30W电源
	EN_PERI_10W_POWER,         //10W电源
	EN_PERI_SENSOR1,           //传感器组1
	EN_PERI_SENSOR2,           //传感器组2
	EN_PERI_SENSOR3,           //传感器组3
	EN_PERI_SENSOR4,           //传感器组4
	EN_PERI_SENSOR5,           //传感器组5
	EN_PERI_SENSOR6,           //传感器组6
	EN_PERI_SENSOR7,           //传感器组7
	EN_PERI_SENSOR8,           //传感器组8
	EN_PERI_SENSOR9,           //传感器组9
	EN_PERI_SENSOR10,          //传感器组10
	EN_PERI_SENSOR11,          //传感器组11
	EN_PERI_SENSOR12,          //传感器组12
	EN_PERI_SENSOR13,          //传感器组13
	EN_PERI_RF1,               //RF模块
	EN_PERI_RTC,               //RTC
	EN_PERI_ENCRYPTION,        //国网加密芯片
	EN_PERI_RF2,               //无线射频模块
	EN_PERI_BATTERY_SAMPLE,    //电池采样
	EN_PERI_5V_SAMPLE,         //5V采样
	EN_PERI_SAMPLE_RESERVED,   //预留采样
	EN_DATA_EXPORT_USB,        //USB数据导出
	EN_PERI_4G_USB,            //4G全网通USB
	EN_PERI_4G_UART,           //4G全网通串口
	EN_PERI_DEBUG_UART,        //
	EN_PERI_END,
};
// =============================================================================
// 功能：板件模块各独立电源控制函数
// 参数：PeriIdx,外设编号;
//     Flag,使能标识符,true，进入低功耗状态;false,退出低功耗状态.
// 返回：无
// =============================================================================
void Peri_LowPowerCfg(enum BoardPeriCode PeriIdx,bool_t Flag);
void Board_PowerCtrlInit(void);
void Peri_NameInit(void);
ptu32_t Power_Ctrl_Shell_Module_Install(void);
void Module_PeriCtrlInit(void);


#ifdef __cplusplus
}
#endif

#endif
