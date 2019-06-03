//------------------------------------------------------------------------------
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
// Copyright (c) 2014 ����Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������¶������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�����������У���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�����������У��Լ�����
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
/*
 * app.c
 *  Created on: 2014-5-28
 *      Author: Administrator
 */
#include <stdint.h>
#include <djyos.h>
#include <stdio.h>
#include "ymodem.h"
#include "shell.h"


//д֮ǰ��������
#define CN_REASE_RISC_START (0x100000)
#define CN_REASE_RISC_LEN   (0x80000)

#define CN_REASE_DSP_START  (0x100000 + CN_REASE_RISC_LEN)
#define CN_REASE_DSP_LEN    (0x100000 + 0x80000)

#define CN_REASE_APP_START  (0x40000)
#define CN_REASE_APP_LEN    (1024*256)////����256bytes �ļ�ͷ

/*============================================================================
            M0IBOOT     DSP          CK         M0APP
blackstart  0           2            11
blackEnd    2           11           16
AddRStart   0           0x20000     0xb0000
AddEnd      0x20000     0xb0000     0x100000
SIZE        128K        576K        320K
============================================================================*/

//BLOCK_SIZE
/* M0 0--0x20000  dsp 0x20000 -- 0xb0000   CK  0xb0000 -- 0xb0000     1.mp3 0x100000 2.mp3 0x110000      */
/*64KB BLOCK_SIZE*/
struct iap_config
{
    char* fs_name;
    u32 bstart;
    u32 bend;
};

const struct iap_config iap_ck =
{
        .fs_name = "xip-ck",
        .bstart  =  11,
        .bend    =  16,
};
const struct iap_config iap_dsp =
{
        .fs_name = "xip-dsp",
        .bstart  = 2,
        .bend    = 11,
};

//==============================================================================
// m0 ��App����������CK����DSP����
//==============================================================================
bool_t ModuleInstall_App()
{
    extern s32 ModuleInstall_XIP_FS(u32 opt, void *data,char *xip_target);
    s32 ModuleInstall_EmbededFlash(const char *TargetFs,u32 bstart, u32 bend, u32 doformat);

    ModuleInstall_XIP_FS(0,NULL,iap_ck.fs_name);
    ModuleInstall_EmbededFlash(iap_ck.fs_name,iap_ck.bstart, iap_ck.bend, 0);

    ModuleInstall_XIP_FS(0,NULL,iap_dsp.fs_name);
    ModuleInstall_EmbededFlash(iap_dsp.fs_name,iap_dsp.bstart, iap_dsp.bend, 0);

    return true;
}

//==============================================================================
// ����CK��ymode��ں���
//==============================================================================
bool_t downloadym_ck(char *Param)
{
    Ymodem_PathSet("/xip-ck/");
    return downloadym(Param);
}

//==============================================================================
// ����DSP��ymode�ӿ�
//==============================================================================
bool_t downloadym_dsp(char *Param)
{
    Ymodem_PathSet("/xip-dsp/");
    return downloadym(Param);
}

ADD_TO_ROUTINE_SHELL(downrisc ,downloadym_ck,"����CK��ִ���ļ�       �����ʽ: downrisc");
ADD_TO_ROUTINE_SHELL(downdsp ,downloadym_dsp,"����DSP��ִ���ļ�    �����ʽ: downdsp");

