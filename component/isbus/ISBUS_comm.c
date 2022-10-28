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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <djyos.h>
#include <shell.h>
#include "component_config_isbus.h"

bool_t ISBUS_SlaveInit(u32 StackSize);
void ISBUS_HostInit(u32 HostStackSize);
bool_t debug_ctrl = true;
u32 dbg_stopack = 0;
u32 testresend = 0;
// ============================================================================
// �������ܣ���װ�ڲ�����ͨ��ģ�顣��ģ��������ڴӻ����֡�
// ���������StackSize��ģ����Ҫ���ڴ�ߴ磬���ڴ���ͨ��Э�������Ҫ�����û�
//              �ṩ�Ļص������������ݣ���ģ��������Ҫ���ڴ���٣���಻������
//              �ֽڣ����û������Լ��Ļص�����������
//       CN_PRIO_RLYMAIN���¼����ȼ������û��Լ�ѡȡ��һ��ѡ��ϸ����ȼ�
// ����ֵ�� ���ǳɹ������ɹ��ͽ�����ѭ����
// ============================================================================
bool_t ModuleInstall_ISBUS(u32 HostStackSize,u32 SlaveStackSize)
{
#if(CFG_HOST_ENABLE == true)
    ISBUS_HostInit(HostStackSize);
#endif
#if(CFG_SLAVE_ENABLE == true)
    ISBUS_SlaveInit(SlaveStackSize);
#endif
    return true;
}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

bool_t ndbgisbus(char *param)
{
    if(debug_ctrl == false)
        debug_ctrl = true;
    else
        debug_ctrl = false;
    return true;
}
#pragma GCC diagnostic pop

bool_t stopack(char *param)
{
    u32 times;
    times = atoi(param);
    dbg_stopack = times;
    return true;
}
bool_t resend(char *param)
{
    testresend = atoi(param);
    return true;
}

ADD_TO_ROUTINE_SHELL(ndbgisbus,ndbgisbus,"");
ADD_TO_ROUTINE_SHELL(stopack,stopack,"");
ADD_TO_ROUTINE_SHELL(resend,resend,"");

