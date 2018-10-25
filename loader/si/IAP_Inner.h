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
#ifndef __IAP_INNER_H__
#define __IAP_INNER_H__

#if __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <version.h>

#define CN_IBOOT_FLAG               "RunIboot"
#define CN_APP_CTRL_APP_FLAG        0x61707000 // �ַ���"app"
#define CN_APP_CTRL_DBG_FLAG        0x00676264 // �ַ���"dbg"
#define CN_APP_CTRL_RELEASE_FLAG    0x72656173 // �ַ���"reas"
#define CN_IAP_MODE_IBOOT           0x12345678 // iboot mode
#define CN_IAP_MODE_APP             0x56781234 // app mode


struct IbootCtrl
{
   u32 AppSize;
   u32 Iap_crc;
   u32 Reserved;
   u32 flag;
   char filename[240];
};

struct AppInfo
{
    u32 RomStartAddr;
    u32 AppSize;
};

enum _IBOOT_ERR_STATUS_
{
    EN_NO_ERR=0,
    EN_FORCE_IBOOT,
    EN_RAM_IBOOT_FLAG,
    EN_LOAD_FROM_DATA_MODE,
    EN_APP_UPDATE_MODE,
    EN_CRC_ERR,
    EN_APP_FLAG_ERR,
    EN_FILE_NO_EXSIT_ERR,
    EN_FILE_SIZE_INVALID_ERR,
    EN_BIN_INCOMPLETED_ERR,
    EN_lDS_MISMATCH,
};

typedef struct __Message{
    union __U{
        u8 data[60]; // ��ʾ����ռ���
        struct __i2a{
            struct __version{
                char *pIBOOT;
                char *pIBOOT_COMPILE_DATE;
                char *pIBOOT_COMPILE_TIME;
                char *pBOARD;
            } version;
        }i2a; // IBOOT���ݸ�APP������

        struct __a2i{
            struct __message{
                u32 dwMethod;
                char path[56];
            } update;
        }a2i; // APP���ݸ�IBOOT������
    }u;
    s32 (*actions)(u8 bArgc, ...); // ����ԭ���ǣ�IBOOT��APP���߱����ʱ�򣬲��õ��öԷ��ĺ���(����);
}tagMessage;

typedef struct _IAP_Var
{
    u32 IbootStatus;
    u32 RunMode;
    char UpdateFlag[8];
    char IbootFlag[16];
    tagMessage message;
}tagIapVar;


#if __cplusplus
}
#endif
#endif  /*__IAP_H__*/
