//----------------------------------------------------
// Copyright (c) 2018,Open source team. All rights reserved.

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
#ifndef __BOARD_H__
#define __BOARD_H__

#include "./include/ad_sr5333.h"
#include "./include/kiko.h"
#include "./include/max706.h"
#include "./include/harddrv_ad.h"
#include "./include/pwm.h"

#ifdef __cplusplus
extern "C" {
#endif

void Board_UartHalfDuplexSend(u8 SerialNo);
void Board_UartHalfDuplexRecv(u8 SerialNo);
void CAN_HalInit(void);
bool_t Max31865_DataReady(void);
void Board_GpioInit(void);
bool_t Board_SpiCsCtrl(u8 SPIPort,u8 cs,u8 level);
void M4G_PowerSet(void);
void M4G_PowerClr(void);

bool_t ModuleInstall_ExpBkpsram(ptu32_t para);
bool_t ModuleInstall_ExpBkpsramTest(ptu32_t para);

enum EN_ExpDealResult BrdExp_RecordSave(struct BlackBoxRecordPara  *recordpara);
bool_t BrdExp_RecordClean(void);
bool_t BrdExp_RecordCheckNum(u32 *recordnum);
bool_t BrdExp_RecordCheckLen(u32 assignedno, u32 *recordlen);
bool_t BrdExp_RecordGet(u32 assignedno,u32 buflen,u8 *buf,\
                            struct BlackBoxRecordPara *recordpara);
void BrdExp_RecordScan(void);
bool_t ModuleInstall_BrdExp(ptu32_t para);

bool_t ModemReset(void);

void USB_IOs(void);
void USB_IAP_TurnOnLED(void);
void USB_IAP_TurnOffLED(void);
void USB_UDiskReset(void);
void USB_DeviceReset(u8 bHostID, u8 bDevID);
void USB_UserInstall(u8 bHostID);
void USB_FS_GPIO_Init(void);
void USB_HS_GPIO_Init(void);




#ifdef __cplusplus
}
#endif
#endif
