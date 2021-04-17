//-----------------------------------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

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


#ifndef GD5F1GQ5X_H_
#define GD5F1GQ5X_H_


#ifdef __cplusplus
extern "C" {
#endif

#define Gd5f1g_WriteEnable                   0x06
#define Gd5f1g_WriteDisable                  0x04

#define Gd5f1g_ReadFeaturesReg               0x0F
#define Gd5f1g_RdFeaturesReg_Protection      0xA0
#define Gd5f1g_RdFeaturesReg_Feature1        0xB0
#define Gd5f1g_RdFeaturesReg_Status1         0xC0
#define Gd5f1g_RdFeaturesReg_Feature2        0xD0
#define Gd5f1g_RdFeaturesReg_Status2         0xF0

#define Gd5f1g_WriteFeaturesReg              0x1F
#define Gd5f1g_WrFeaturesReg_Protection      0xA0
#define Gd5f1g_WrFeaturesReg_Feature1        0xB0
#define Gd5f1g_WrFeaturesReg_Feature2        0xD0

#define Gd5f1g_ReadData                      0x13
#define Gd5f1g_ReaddaraCache                 0x03
#define Gd5f1g_ReaddaraCachex4               0x6B
#define Gd5f1g_PageProgram                   0x02
#define Gd5f1g_PageProgramx4                 0x32
#define Gd5f1g_PageProgramExecute            0x10
#define Gd5f1g_BlockErase                    0xD8
#define Gd5f1g_PowerDown                     0x66
#define Gd5f1g_ReleasePowerDown              0x99
#define Gd5f1g_DeviceID                      0x9F

#define Gd5f1g_SoftReset                     0xFF

#define ParameterPage_Addr                   4

enum GetFeatures
{
    Get_Protection = 0,
    Get_Feature1,
    Get_Status1,
    Get_Feature2,
    Get_Status2,
};
enum SetFeatures
{
    Set_Protection = 0,
    Set_Feature1,
    Set_Feature2,
};

s32 Gd5f1g_Ctrl(enum ucmd cmd, ptu32_t args, ...);
s32 Gd5f1g_EraseBlock(u32 block_no);
u32 Gd5f1g_ReadSpare(u32 PageNo, u8* buf);
u32 Gd5f1g_WriteSpare(u32 PageNo, u8* buf);
u32 Gd5f1g_ReadPage(u32 PageNo, u8* buf, u32 Flags);
u32 Gd5f1g_WritePage(u32 PageNo, u8* buf, u32 Flags);
s32 Gd5f1g_ReadPage_BadBlk(s32 page, void *data, struct uopt opt);
s32 Gd5f1g_WritePage_BadBlk(s32 page, void *data, struct uopt opt);
s32 Gd5f1g_Erase_BadBlk(s32 block);
bool_t Gd5f1g_IsInstall(void);
s32 __Gd5f1g_FsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv);
#ifdef __cplusplus
}
#endif

#endif /* GD5F1GQ5X_H_ */
