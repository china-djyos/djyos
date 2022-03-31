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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
//����ģ��: Iboot
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ��Ӧ�ñ�̵ĺ����ļ�
//����˵��:
//�޶���ʷ:
//1. ����: 2016-06-17
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __XIP_H__
#define __XIP_H__

#if __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

//
// IAP�ļ���Ϣ
//
struct __ifile{
    u32 cxbase; // �ļ�ʵ�����ݵ�ƫ�ã��ļ�ͷ����Ϣ����ڿ�ʼ������һ��ƫ�ã���
    u32 sz; // �ļ���С
    u32 status; // �ļ�״̬��
    struct MutexLCB *lock; // �ļ�����
};

//
// IAP�ļ������ģ�����δ����Ԥ���߼�����ֻ���д�����˻��棬��δ���ǻ��棻
//
struct __icontext{
    u32 pos; // �ļ��ĵ�ǰλ�ã�
    s16 bufed; // �����ڻ����е����ݣ�
    u8 *buf; // �������һ�����棬�߼����Ƕ���ģ�

    u8 *apphead;
    u32 Wappsize;
};

//
// IAP�ļ�ϵͳ������Ϣ
//
struct __icore{
    void *vol; // �ļ�ϵͳ�ײ����volume��
    s16 bufsz; // �ļ����壬�����APP���ļ�ͷ��
    u32 inhead; // �ļ�buffer size ������ļ�ͷ����inhead=�������ֳߴ�
    s64 MStart;             // ��ý���е���ʼunit,unitΪ��λ��
    s64 ASize;               // ����������ܴ�С��ByteΪ��λ��
    struct Object *root; // IAP�ļ�ϵͳ������ļ�ϵͳ�ĸ���
    struct MutexLCB *lock; // ϵͳ����
    struct __xip_drv *drv;
};
//
// iap�ļ�ϵͳ��flash�Ĳ���������
//
struct __xip_drv{
    s32 (*xip_write_media) (struct __icore *core, u8 *data, u32 bytes, u32 pos);
    s32 (*xip_read_media) (struct __icore *core, u8 *data, u32 bytes, u32 pos);
    s32 (*xip_erase_media) (struct __icore *core, u32 bytes, u32 pos);
};

//���ļ��м��ؿ�ִ���ļ����ڴ����У�����������iboot��iboot�м����ļ�ϵͳ
#define CN_RUN_FORM_FILE  0
//ԭ�� EN_DIRECT_RUN����ģʽ�£���ʵ���ƹ�iboot��ֱ������APP���ٶȷǳ���
#define CN_DIRECT_RUN     1

#define CN_XIP_APP_MOUNT        "xip-app"
#define CN_XIP_APP_PATH         "/xip-app"
#define CN_XIP_IBOOT_MOUNT      "xip-iboot"
#define CN_XIP_IBOOT_PATH       "/xip-iboot"

enum _ENUM_USE_CRC_
{
    EN_NO_APP_CRC=0,
    EN_USE_APP_CRC,
};

u32 IAP_GetAPPCRC(void);
u32 IAP_GetAPPStartCodeRamAddr(void);
u32 IAP_GetAPPCodeRamSize(void);
bool_t IAP_LoadAPPFromFile(void);
s32 IAP_Update(u8 bArgC, ...);
s32 IAP_SetMethod(u32 dwMethod);
u32 IAP_GetMethod(void);
char *IAP_GetPath(void);
s32 IAP_SetPath(char *pPath);


#if __cplusplus
}
#endif
#endif  /*__IAP_H__*/
