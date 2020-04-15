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
// ģ������: �����ʱ�����ݽṹ�Լ��ӿ�����
// ģ��汾: V1.00
// ������Ա: zhangqf_cyg
// ����ʱ��: 2:44:27 PM/Apr 1, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ��ע

#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

struct Timer;
typedef struct Timer tagTimer;
typedef void (*fnTimerRecall)(struct Timer *timer);

enum TimerCmdCode
{
    EN_TIMER_SOFT_START,     //������ʱ����ʱ�������¼��㣬inoutpara�����壬���Ѿ���������Ϊ�ղ���
    EN_TIMER_SOFT_PAUSE,     //ֹͣ��ʱ��inoutpara�����壬������Ϊ�˼���
    EN_TIMER_SOFT_STOP = EN_TIMER_SOFT_PAUSE,   //ֹͣ��ʱ��inoutpara������
    EN_TIMER_SOFT_SETCYCLE,  //�������ڣ�inoutparaΪu32,�����õ����ڣ�uS����
    EN_TIMER_SOFT_SETRELOAD, //reloadģʽ,true�����Զ�reload
};

tagTimer*  Timer_Create_s(tagTimer *timer,const char *name,
                                  u32 cycle, fnTimerRecall isr);
tagTimer*  Timer_Delete_s(tagTimer* timer);
tagTimer*  Timer_Create(const char *name,u32 cycle,fnTimerRecall isr);
bool_t Timer_IsRunning(tagTimer *Timer);
bool_t Timer_Delete(tagTimer* timer);
bool_t Timer_Ctrl(tagTimer* timer,u32 opcode, ptu32_t para);
char *Timer_GetName(tagTimer* timer);
bool_t Timer_SetTag(tagTimer* timer,ptu32_t Tag);
ptu32_t Timer_GetTag(tagTimer* timer);
bool_t ModuleInstall_Timer(void);

#ifdef __cplusplus
}
#endif

#endif /* __TIMER_H__ */
