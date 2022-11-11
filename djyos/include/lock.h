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
//����ģ��: ��ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����: �����ź����ͻ�����
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __semp_h__
#define __semp_h__

#include "errno.h"
#include "list.h"
#ifdef __cplusplus
extern "C" {
#endif

#include "stddef.h"

struct EventECB;

//---------�������źŵ�����--------------
//1���������źſ��ṩ�ܱ�������Դ��ʹ�ü���
//2��Ϊ������Ҳ�ɲ���������Դ�ṩһ�µĴ��룬�������ṩ������ʱ���õĿ�����
struct SemaphoreLCB
{
    struct dListNode List;
    u32 sync_order;
    s32 lamps_limit;   //�źŵ��������ޣ�cn_limit_uint32��ʾ��������
    s32 lamp_counter;  //�����źŵ�������
    struct EventECB *semp_sync;    //�Ⱥ��źŵ��¼�����
    const char *name;
};

//�������ǿ��Եݹ�����ģ���ͬһ���¼����������󣬵�Ҫ���ͷŵĴ���������Ĵ���
//��ͬ���������������һ���¼������������Σ���Ҫ�ͷ����β��������ͷš�
struct MutexLCB
{
    struct dListNode List;
    s32  enable;                        //0=���ã�>0 = ���߳�ռ�ã�<0 = �ж�ռ��
    ufast_t  prio_bak;                  //���ȼ��̳��б���ԭ���ȼ�
    struct EventECB *owner;         //ռ�û��������¼��������ж�ռ�ã�����Ч
    struct EventECB *mutex_sync;    //�Ⱥ򻥳������¼����У����ȼ��Ŷ�
    const char *name;
};

//�����ź����ͻ����������ڴ��
union lock_MCB
{
   struct SemaphoreLCB sem;
   struct MutexLCB  mut;
};

//������Ϣ����
enum _LOCK_ERROR_CODE_
{
    EN_LOCK_NO_ERROR = CN_LOCK_NO_ERROR,      //û�д���
    EN_LOCK_PARA_ERROR,             //��������������
    EN_LOCK_RANGE_ERROR,            //�ź�����Χ��������
    EN_LOCK_BLOCK_IN_INT,           //�ж�ISR����������
};

ptu32_t ModuleInstall_Lock(void);
struct SemaphoreLCB *Lock_SempCreate(s32 lamps_limit,s32 init_lamp,
                                        u32 sync_order,const char *name);
struct SemaphoreLCB *Lock_SempCreate_s( struct SemaphoreLCB *semp,
                       s32 lamps_limit,s32 init_lamp,u32 sync_order,const char *name);
void Lock_SempExpand(struct SemaphoreLCB *semp, s32 Num);
void Lock_SempPost(struct SemaphoreLCB *semp);
bool_t Lock_SempPend(struct SemaphoreLCB *semp,u32 timeout);
bool_t Lock_SempDelete_s(struct SemaphoreLCB *semp);
bool_t Lock_SempDelete(struct SemaphoreLCB *semp);
u32 Lock_SempQueryCapacital(struct SemaphoreLCB *semp);
bool_t Lock_SempCheckBlock(struct SemaphoreLCB *Semp);
u32 Lock_SempQueryFree(struct SemaphoreLCB *semp);
void Lock_SempSetSyncSort(struct SemaphoreLCB *semp,u32 order);
struct MutexLCB *Lock_MutexCreate(const char *name);
struct MutexLCB *  Lock_MutexCreate_s( struct MutexLCB *mutex,const char *name);
void Lock_MutexPost(struct MutexLCB *mutex);
bool_t Lock_MutexPend(struct MutexLCB *mutex,u32 timeout);
bool_t Lock_MutexDelete(struct MutexLCB *mutex);
bool_t Lock_MutexDelete_s(struct MutexLCB *mutex);
bool_t Lock_MutexQuery(struct MutexLCB *mutex);
bool_t Lock_MutexCheckBlock(struct MutexLCB *mutex);
u16 Lock_MutexGetOwner(struct MutexLCB *mutex);

#ifdef __cplusplus
}
#endif

#endif //__semp_h__

