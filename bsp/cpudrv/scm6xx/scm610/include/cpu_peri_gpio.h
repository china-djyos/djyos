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
//所属模块:IO初始化
//作者：lst
//版本：V1.0.0
//文件描述:IO初始化
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-03-10
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __cpu_peri_gpio_h_
#define __cpu_peri_gpio_h_

#ifdef __cplusplus
extern "C" {
#endif

enum EN_GPIO_MODE
{
    EN_INPUT_MODE = 0,   //配置为输入模式
    EN_OUTPUT_MODE,      //配置为输出模式
};

enum EN_GPIO_LEV
{
    EN_GPIO_LEV_L,       //配置输出为低电平
    EN_GPIO_LEV_H,       //配置输出为高电平
};

//端口读取电平失败

#define CN_RORT_READ_FAILED (-1)

enum EN_GPIO_MASK
{
    EN_GPIO_UMASK = 0,
    EN_GPIO_MASK,
};



enum EN_GPIO_INT_MODE
{
    EN_MODE_BEGIN = 0,
    EN_MODE_HIGHT_LEVEL = EN_MODE_BEGIN,  //高电平触发
    EN_MODE_LOW_LEVEL,                    //低电平触发
    EN_MODE_RISINGEDGE,                   //上升沿触发
    EN_MODE_FALLINGEDGE,                  //下降沿触发
    EN_MODE_BOTHEDGE,                     //双边沿触发
    EN_MODE_END = EN_MODE_BOTHEDGE,
};


void GPIO_CfgPinFunc(u32 port,u32 Pinx,u32 Mode,
                        u32 OutType,u32 OutSpeed,u32 PUPD);
u32 GPIO_AFSet(u32 port,u32 pinnum,u32 af_no);
u32 GPIO_GetData(u32 port);
void GPIO_OutData(u32 port,u32 data);
void GPIO_SettoHigh(u32 port,u32 msk);
void GPIO_SettoLow(u32 port,u32 msk);
void GPIO_PowerOn(u32 port);
void GPIO_PowerOff(u32 port);

bool_t Gpio_DirConfig(const u8 Mode,const u8 PortNum);
char Gpio_ReadLevel(u8 PortNum);
bool_t Gpio_ExIntEnable(u8 PortNum);
bool_t Gpio_ExIntDisable(u8 PortNum);
bool_t Gpio_IntMask(u8 PortNum,u8 MaskCode);
bool_t Gpio_ClearIntFlag(u8 PortNum);
bool_t Gpio_IntModeConfig(u8 PortNum,u8 IntMode);

#ifdef __cplusplus
}
#endif

#endif /*__cpu_peri_gpio_h_*/
