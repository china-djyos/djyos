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
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------

#ifndef __BOARD__H__
#define __BOARD__H__

//#include "./include/ctiic.h"
#include "cpu_peri.h"
#include "IoIicBus.h"

#ifdef __cplusplus
extern "C" {
#endif

#if CFG_ENABLE_OV2640 == true       //摄像头控制占用了TP的PIN
/**********************************ov2640 SCCB*********************************/
//IO方向设置
#define SCCB_SCL_OUT() GPIO_Congif(GPIOE, GPIO_Pin_11, GPIO_Mode_OUT, GPIO_PuPd_UP)  //PA0输出模式
#define SCCB_SDA_IN()  GPIO_Congif(GPIOE, GPIO_Pin_12, GPIO_Mode_IN,  GPIO_PuPd_UP)  //PA1输入模式
#define SCCB_SDA_OUT() GPIO_Congif(GPIOE, GPIO_Pin_12, GPIO_Mode_OUT, GPIO_PuPd_UP)  //PA1输出模式
//IO操作函数
#define SCCB_IIC_SCL(n) (n?GPIO_SettoHigh(GPIOE,GPIO_Pin_11):GPIO_SettoLow(GPIOE,GPIO_Pin_11))//SCL
#define SCCB_IIC_SDA(n) (n?GPIO_SettoHigh(GPIOE,GPIO_Pin_12):GPIO_SettoLow(GPIOE,GPIO_Pin_12))//SDA
#define SCCB_READ_SDA   (GPIO_READ(GPIOE,GPIO_Pin_12))//输入SDA
/******************************************************************************/
#else
/**********************************gt911**************************************/
//IO方向设置
#define CT_SCL_OUT() GPIO_Congif(GPIOA, GPIO_Pin_0, GPIO_Mode_OUT, GPIO_PuPd_UP)  //PA0输出模式
#define CT_SDA_IN()  GPIO_Congif(GPIOA, GPIO_Pin_2, GPIO_Mode_IN,  GPIO_PuPd_UP)  //PA1输入模式
#define CT_SDA_OUT() GPIO_Congif(GPIOA, GPIO_Pin_2, GPIO_Mode_OUT, GPIO_PuPd_UP)  //PA1输出模式
//IO操作函数
#define CT_IIC_SCL(n) (n?GPIO_SettoHigh(GPIOA,GPIO_Pin_0):GPIO_SettoLow(GPIOA,GPIO_Pin_0))//SCL
#define CT_IIC_SDA(n) (n?GPIO_SettoHigh(GPIOA,GPIO_Pin_2):GPIO_SettoLow(GPIOA,GPIO_Pin_2))//SDA
#define CT_READ_SDA   (GPIO_READ(GPIOA,GPIO_Pin_2))//输入SDA

/*触摸屏芯片接口IO*/
#define FT_RST(n)  (n?GPIO_SettoHigh(GPIOA,GPIO_Pin_3):GPIO_SettoLow(GPIOA,GPIO_Pin_3))
#define FT_INT      (GPIO_READ(GPIOA,GPIO_Pin_1))
/******************************************************************************/
#endif

void Board_Init(void);
void FT6236_Pin_Init(void);
u32 IIC_IoCtrlFunc(enum IIc_Io IO,u32 tag);
void OpenScreen();
void CloseScreen();
void ClosePower();
void CloseSpeaker();
void OpenSpeaker();
enum SpeakerState GetSpeakerState();
ptu32_t Iboot_GetResetAddr(void);

#ifdef __cplusplus
}
#endif
#endif






