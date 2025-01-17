//----------------------------------------------------
// Copyright (c) 2018, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2018，著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
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
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
//所属模块:
//作者：
//版本：V1.0.0
//文件描述:
//其他说明:
//修订历史:
//2. ...
//1. 日期:
//   作者:
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------

#ifndef CPU_PERI_ADC_H
#define CPU_PERI_ADC_H

#define  CN_AD_FAILSE           ( 0 )
#define  CN_AD_SUCCESS      ( 1 )
#define  CN_ARG_ERR             ( 0x38  )
#define  CN_RAED_ERR_DAT    (0xffffffff)  //这里用前面16位,标识读出错

/*channel define*/

#define CN_AD_CHANNEL0  IO_CONFIG_PA0
#define CN_AD_CHANNEL1  IO_CONFIG_PA1
#define CN_AD_CHANNEL2  IO_CONFIG_PA2
#define CN_AD_CHANNEL3  IO_CONFIG_PA3
#define CN_AD_CHANNEL4  IO_CONFIG_PA4
#define CN_AD_CHANNEL5  IO_CONFIG_PA5
#define CN_AD_CHANNEL6  IO_CONFIG_PA6
#define CN_AD_CHANNEL7  IO_CONFIG_PA7

/*AD pin config choose from below when read .match with channel config*/

#define CN_AD_CFG_PA0  IO_CONFIG_PA0
#define CN_AD_CFG_PA1  IO_CONFIG_PA1
#define CN_AD_CFG_PA2  IO_CONFIG_PA2
#define CN_AD_CFG_PA3  IO_CONFIG_PA3
#define CN_AD_CFG_PA4  IO_CONFIG_PA4
#define CN_AD_CFG_PA5  IO_CONFIG_PA5
#define CN_AD_CFG_PA6  IO_CONFIG_PA6
#define CN_AD_CFG_PA7  IO_CONFIG_PA7

//comand
#define  CN_ADC_CHANNEL_SET             (0x11)
#define  CN_ADC_CHANNEL_RETSET      (0x12)
#define  CN_ADC_CHANNEL_PINCFG       (0x13)
#define  CN_ADC_CHANNEL_RD_DAT       (0x14)     //read data

//vef select
typedef enum{
    EN_ADC_VREF_EXT33 = 0,
    EN_ADC_VREF_IN24,
}tagVef;

int vbat_voltage_get(void);
void ModuleInstall_ADC(void);
//extern void appADC_Ctrl(u32 handle,u32 cmd, u32 data1,u32 data2);


#endif
