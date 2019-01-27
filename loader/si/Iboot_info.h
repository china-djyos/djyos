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
// �ļ���     ��
// ģ������:
// ģ��汾:
// ������Ա: czz
// ����ʱ��:
// =============================================================================

#ifndef __SI_INFO_H__
#define __SI_INFO_H__
#include "stdio.h"

enum runibootmode{
    HEARD_SET_RUN_IBOOT,
    RAM_SET_RUN_IBOOT,
    UPDATE_APP_RUN_IBOOT,
    CHACK_ERROR,
};

enum runappmode{
    RUN_APP_FROM_FILE,
    RUN_APP_FROM_DIRECT,
    RUN_APP_FROM_UPDATE,
};

enum headflag
{
    POWER_ON_FLAG,
    HEAD_RESET_FLAG,
    HEAD_WDT_RESET,
    LOW_POWER_WAKEUP,

};
//==============================================================================
//˵�����ú���__attribute__((weak))  u8  Get_Headflag(enum headflag flag)ʵ��
//      ���Ӳ��֧����Ӧ��initcpuc.c�ļ���ʵ�ָú���
//���ܣ���ȡӲ���ϵı�־
//������POWER_ON_FLAG����ȡ�ϵ縴λӲ����־��0=�޴�Ӳ����1=�д�Ӳ����
//                  ���ޱ�־��2=�б�־���ĺ󼴷٣�3=�У����ĺ󼴷�
//     HEAD_RESET_FLAG�� ��ȡӲ����λ��־û��/��֧�ַ���0
//     HEAD_WDT_RESE�� ��ȡӲ�����Ź���λ��־û��/��֧�ַ���0
//     LOW_POWER_WAKEUP�� �͹��Ļ���û��/��֧�ַ���0
//
//==============================================================================

bool_t Run_Iboot(enum runibootmode mode);
bool_t Run_App(enum runappmode mode);

bool_t Set_UpdateRunModet(u8 mode);
bool_t Set_RunIbootUpdateIboot();
bool_t Set_RunIbootUpdateApp();
bool_t Set_RunAppFlag();
bool_t Set_RunIbootFlag();
bool_t clear_resentflag();
bool_t Update_ToRun();
bool_t Si_IbootAppInfoInit();
bool_t IAP_IsRamIbootFlag();


u32  IAP_GetAPPSize(void * apphead);
u32  Get_AppHeadSize(void);
u32  IAP_GetAPPStartAddr(void * apphead);
char*  Get_AppName(void * apphead);
bool_t IAP_APPIsDebug(void * apphead);
bool_t Iap_AppFileChack(void * apphead);
bool_t Rewrite_AppHead(void * apphead,const char*name,u32 filesize);

#endif /* __IICBUS_H__ */
