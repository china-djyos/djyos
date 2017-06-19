//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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
/*
 * osarch.c
 *
 *  Created on: 2016��11��25��
 *      Author: zhangqf
 */

#include "osarch.h"

//mutex
mutex_t mutex_init(const char *name)
{
	return Lock_MutexCreate(name);
}
bool_t mutex_lock(mutex_t mutex)
{
	return Lock_MutexPend(mutex,CN_TIMEOUT_FOREVER);
}
bool_t mutex_locktimeout(mutex_t mutex,u32 timeout)
{
	return Lock_MutexPend(mutex,CN_TIMEOUT_FOREVER);
}
bool_t mutex_unlock(mutex_t mutex)
{
	Lock_MutexPost(mutex);
	return 0;
}

void mutex_del(mutex_t mutex)
{
	Lock_MutexDelete(mutex);
	return;
}

//-----------------------------------------------------------------------------
//����:use this function to get a random data, which got from the system time
//����:NULL
//����:the random data
//��ע:
//-----------------------------------------------------------------------------
u32 get_random(void)
{
	return (u32)DjyGetSysTime();
}


//-----------------------------------------------------------------------------
//����:use this function to create a semaphore
//����:
//����:the semaphore if success and NULL when failed
//��ע:
//-----------------------------------------------------------------------------
semp_t semp_init()
{
	return Lock_SempCreate(0x1000,0,CN_BLOCK_FIFO,NULL);
}
//-----------------------------------------------------------------------------
//����:use this function to pend a semaphore
//����:semp, the semaphore we will pend later
//����:true when got the semaphore else false
//��ע:never return if not got the semaphore and the condition permit waiting
//-----------------------------------------------------------------------------
bool_t semp_pend(semp_t semp)
{
	return Lock_SempPend(semp,CN_TIMEOUT_FOREVER);
}

bool_t semp_pendtimeout(semp_t semp, unsigned int timeout)
{
	return Lock_SempPend(semp,timeout);
}

bool_t semp_post(semp_t semp)
{
	Lock_SempPost(semp);
	return true;
}

void semp_del(semp_t semp)
{
	Lock_SempDelete(semp);
	return;
}

void semp_reset(semp_t semp)
{
	semp->lamp_counter = 0;
	return ;
}

//-----------------------------------------------------------------------------
//����:use this function to create a task
//����:
//����:
//��ע:
//����:zhangqf@����4:38:04/2017��1��5��
//-----------------------------------------------------------------------------
bool_t taskcreate(const char *name,u16 stacksize,u8 prior,ptu32_t (*fnTask)(void),ptu32_t para)
{
	u16 evttID,eventID;
    evttID= Djy_EvttRegist(EN_INDEPENDENCE, prior, 0, 1,fnTask,NULL, stacksize,(char *)name);
    if(evttID == CN_EVTT_ID_INVALID)
    {
        goto EXIT_EVTTFAILED;
    }
    eventID = Djy_EventPop(evttID, NULL, 0, para, 0, 0);
    if(eventID == CN_EVENT_ID_INVALID)
    {
        goto EXIT_EVENTFAILED;
    }
    return true;

EXIT_EVENTFAILED:
	Djy_EvttUnregist(evttID);
EXIT_EVTTFAILED:
	return false;
}



