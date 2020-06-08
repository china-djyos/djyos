//----------------------------------------------------
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
//author:zhangqf
//date  :����3:20:48/2017��5��15��
//usage :
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#include <os.h>

extern unsigned int client_test(void* args);
extern int wolfcrypt_test(void* args);
extern int server_test(void* args);
typedef struct func_args
{
    int    argc;
    char** argv;
    int    return_code;
} func_args;

static u16 gClientEvttID = CN_EVTT_ID_INVALID;
static u16 gServerEvttID = CN_EVTT_ID_INVALID;
static u16 gCryptEvttID = CN_EVTT_ID_INVALID;
static ptu32_t clienttask(void)
{
    const char *argv[]={"clienttask",};
    struct func_args arg;
    while(1)
    {
        arg.argc = 1;
        arg.argv = argv;
        client_test(&arg);
        DJY_WaitEvttPop(gClientEvttID,NULL,CN_TIMEOUT_FOREVER);
    }

    return 0;
}
static ptu32_t servertask(void)
{
    const char *argv[]={"servertask",};
    struct func_args arg;
    while(1)
    {
        arg.argc = 1;
        arg.argv = argv;
//      server_test(&arg);
        DJY_WaitEvttPop(gServerEvttID,NULL,CN_TIMEOUT_FOREVER);
    }

    return 0;
}


static ptu32_t  crypttask(void)
{
    func_args funcarg;
    wolfcrypt_test(&funcarg);

    const char *argv[]={"crypttask",};
    struct func_args arg;
    while(1)
    {
        arg.argc = 1;
        arg.argv = argv;
        server_test(&arg);
        DJY_WaitEvttPop(gCryptEvttID,NULL,CN_TIMEOUT_FOREVER);
    }

    return 0;
}


static bool_t __ClientStart(char *param)
{
    if(gClientEvttID == CN_EVTT_ID_INVALID)
    {
        gClientEvttID = DJY_EvttRegist(EN_CORRELATIVE,200,1,1,clienttask,NULL,0x4000,"sslclient");
    }
    if(gClientEvttID != CN_EVTT_ID_INVALID)
    {
        DJY_EventPop(gClientEvttID,NULL,0,NULL,NULL,0);
    }
    return true;
}

static bool_t __ServerStart(char *param)
{
    if(gServerEvttID == CN_EVTT_ID_INVALID)
    {
        gServerEvttID = DJY_EvttRegist(EN_CORRELATIVE,200,1,1,servertask,NULL,0x1000,"sslserver");
    }
    if(gServerEvttID != CN_EVTT_ID_INVALID)
    {
        DJY_EventPop(gServerEvttID,NULL,0,NULL,NULL,0);
    }
    return true;
}

static bool_t __CryptStart(char *param)
{
    if(gCryptEvttID == CN_EVTT_ID_INVALID)
    {
        gCryptEvttID = DJY_EvttRegist(EN_CORRELATIVE,200,1,1,crypttask,NULL,0x2000,"sslclient");
    }
    if(gCryptEvttID != CN_EVTT_ID_INVALID)
    {
        DJY_EventPop(gCryptEvttID,NULL,0,NULL,NULL,0);
    }
    return true;
}

#include <shell.h>

ADD_TO_ROUTINE_SHELL(wolfclient,__ClientStart, "usage:wolfclient");
ADD_TO_ROUTINE_SHELL(wolfserver,__ServerStart, "usage:wolfserver");
ADD_TO_ROUTINE_SHELL(wolfcypt,__CryptStart,"usage:wolfcypt");
