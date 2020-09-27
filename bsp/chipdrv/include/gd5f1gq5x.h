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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
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
