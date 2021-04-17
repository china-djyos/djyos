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

//����ģ��: GDD
//����:  LiuWei.
//�汾��V1.0.0
//�ļ�����: ���ڶ�ʱ��ϵͳ���ƺ͹���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2014-11-10
//   ����:  LiuWei.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include <stdint.h>
#include <djyos.h>
#include    "gdd_private.h"
#include <gui\gdd_timer.h>

static  list_t  sg_GddTimerList;
static struct MutexLCB     *s_ptGddTimerQSync =NULL;  //��ʱ��������
static struct SemaphoreLCB *s_ptGddTimerRun =NULL;

//----���䶨ʱ���ڴ�-------------------------------------------------------------
//����: ��
//��������
//���أ���ʱ���ڴ�ָ��
//------------------------------------------------------------------------------
static  struct WinTimer*  __GDD_TimerAlloc(void)
{
    struct WinTimer *tmr;

    tmr =malloc(sizeof(struct WinTimer));
    //todo:�˴��ӿ�ָ���ж�
    memset(tmr, 0, sizeof(struct WinTimer));
    return tmr;
}

//----�ͷŶ�ʱ���ڴ�-------------------------------------------------------------
//����: �ͷ�һ����__gdd_TimerAlloc����Ķ�ʱ���ڴ�.
//��������ʱ���ڴ�ָ��
//���أ���
//------------------------------------------------------------------------------
static  void    __GDD_TimerFree(struct WinTimer *ptmr)
{
    free(ptmr);
}

//----������ʱ��----------------------------------------------------------------
//����: ������ʱ��,���ڶԶ�ʱ���������,�ú�������TRUEʱ,�������__gdd_TimerUnlock����;
//      �����ú�������FALSE,���������__gdd_TimerUnlock.
//��������ʱ������ָ��
//���أ��ɹ�:TRUE; ʧ��:FLASE;
//------------------------------------------------------------------------------
bool_t    __GDD_TimerLock(struct WinTimer *ptmr)
{
    if(NULL == ptmr)
        return FALSE;
    if(__GDD_Lock())
        return TRUE;

    __GDD_Unlock();
    return  FALSE;
}

//----������ʱ��----------------------------------------------------------------
//����: ����ʱ�������ɹ���,�ɸú������н�������.
//��������ʱ������ָ��
//���أ���
//------------------------------------------------------------------------------
void    __GDD_TimerUnlock(struct WinTimer *ptmr)
{
    __GDD_Unlock();
}

// =============================================================================
// �������ܣ���Ӷ�ʱ������ʱ�����У��ö�������Alarmʱ���ɽ���Զ���еġ�
// ���������timer,����ӵĶ�ʱ��
// �����������
// ����ֵ  ����
// ˵��    ��
// =============================================================================
static void __GDD_TimerAdd(struct WinTimer *timer)
{
    list_t *tmp;
    struct WinTimer *t;
    //�ҵ�һ��Alarmʱ�����timer->Alarm�Ľڵ㣬timer�Ͳ���ýڵ�ǰ�档
    //����Ҳ�����������sg_GddTimerListǰ�棬Ҳ����ȷ�ġ�
    //��һ��û��  ;  ��
    dListForEach(tmp, &sg_GddTimerList)
    {
        t = dListEntry(tmp,struct WinTimer,node_sys);
        if(t->Alarm >= timer->Alarm)
            break;
    }
    dListInsertBefore(tmp,&timer->node_sys);
}

//----GDD��ʱ��ģ���ʼ��------------------------------------------------------
//����: �ú�����GDD�ڲ�����
//��������
//���أ���
//------------------------------------------------------------------------------
bool_t GDD_TimerInit(void)
{
    dListInit(&sg_GddTimerList);
    s_ptGddTimerQSync = Lock_MutexCreate("Gdd Timer");
    s_ptGddTimerRun = Lock_SempCreate(1, 0, CN_BLOCK_FIFO, "Gdd Timer");
    if( (s_ptGddTimerQSync == NULL) || (s_ptGddTimerRun == NULL))
    {
        Lock_MutexDelete(s_ptGddTimerQSync);
        Lock_SempDelete(s_ptGddTimerRun);
        return false;
    }
    else
        return true;
}

//----������ʱ��----------------------------------------------------------------
//����: ����һ����ʱ��������ֹͣ����״̬��
//������hwnd: ��ʱ����������.��ʱ������ô��ڷ���ʱ����Ϣ��
//      Id:   ��ʱ��Id.
//      Flag: ��ʱ�����.
//      IntervalMS: ��ʱ���ʱ��(������). 0��ʾ��ͣ������
//���أ���ʱ������ָ��
//------------------------------------------------------------------------------
struct WinTimer*  GDD_CreateTimer(HWND hwnd,u16 Id,u32 IntervalMS)
{
    struct WinTimer *ptmr=NULL;

    if((IntervalMS == 0)||(GDD_FindTimer(hwnd,Id)!=NULL))
        return ptmr;
    if(__HWND_Lock(hwnd))
    {
        ptmr =__GDD_TimerAlloc();;
        if(NULL!=ptmr)
        {
            dListInit(&ptmr->node_hwnd);
            dListInit(&ptmr->node_msg_timer);

            ptmr->hwnd     =hwnd;
            ptmr->Id       =Id;
//            ptmr->Flag     =Flag;
            ptmr->Interval =IntervalMS;
            ptmr->Alarm    = CN_LIMIT_SINT64;       //�´����Ķ�ʱ������ͣ���е�
//          ptmr->HoldTime =GUI_GetTickMS();

            dListInsertBefore(&hwnd->list_timer,&ptmr->node_hwnd);
            Lock_MutexPend(s_ptGddTimerQSync,CN_TIMEOUT_FOREVER);
            __GDD_TimerAdd(ptmr);
            Lock_MutexPost(s_ptGddTimerQSync);
        }
        __HWND_Unlock(hwnd);
    }
    return  ptmr;
}

//----���Ҷ�ʱ��----------------------------------------------------------------
//����: ��Id����ָ�����ڵĶ�ʱ��
//������hwnd: Ҫ���ҵĶ�ʱ�����ڴ���.
//      Id: Ҫ���ҵĶ�ʱ��Id
//���أ��ɹ����ض�ʱ������ָ��,���򷵻�NULL
//------------------------------------------------------------------------------
struct WinTimer*  GDD_FindTimer(HWND hwnd,u16 Id)
{
    list_t *n;
    struct WinTimer *ptmr=NULL;
    if(hwnd==NULL)
        return ptmr;
    if(__GDD_Lock())
    {
        Lock_MutexPend(s_ptGddTimerQSync,CN_TIMEOUT_FOREVER);
        dListForEach(n, &hwnd->list_timer)
        {
            ptmr =(struct WinTimer*)dListEntry(n,struct WinTimer,node_hwnd);
            if(ptmr->Id==Id)
            {
                break;
            }
            ptmr=NULL;
        }
        Lock_MutexPost(s_ptGddTimerQSync);
        __GDD_Unlock();
    }
    return ptmr;
}

//----���ö�ʱ��----------------------------------------------------------------
//����: �������ö�ʱ����������ʱ��������������״̬
//������ptmr: ��ʱ������.
//      IntervalMS: ��ʱ���ʱ��(������). 0��ʾ��ͣ������
//���أ���
//------------------------------------------------------------------------------
void GDD_ResetTimer(struct WinTimer *ptmr,u32 IntervalMS)
{
    if(ptmr==NULL)
        return;
    Lock_MutexPend(s_ptGddTimerQSync,CN_TIMEOUT_FOREVER);
    if(IntervalMS != 0)
    {
        ptmr->Interval  =IntervalMS;
        ptmr->Alarm      = DJY_GetSysTime( ) + IntervalMS * 1000;
    }
    else
    {
        ptmr->Alarm      = CN_LIMIT_SINT64;     //�����޳�����ʱ����ʾ��ͣ
    }
    dListRemove(&ptmr->node_sys);
    __GDD_TimerAdd(ptmr);
    if(&ptmr->node_sys == dListGetAfter(&sg_GddTimerList))
    {
        //����Ƕ���ͷ���������ͷ��ź�����ʹ��ʱ��ɨ�躯�����¶�ʱʱ��
        Lock_SempPost(s_ptGddTimerRun);
    }
    Lock_MutexPost(s_ptGddTimerQSync);
    return ;
}

//----ֹͣ��ʱ��----------------------------------------------------------------
//����: ֹͣһ����ʱ����ʵ���Ͼ��ǰ��ֵ���ʱ����Ϊ�����޳���
//������ptmr: ��ʱ������.
//���أ���
//------------------------------------------------------------------------------
void GDD_StopTimer(struct WinTimer *ptmr)
{
    if(ptmr==NULL)
        return;
    Lock_MutexPend(s_ptGddTimerQSync,CN_TIMEOUT_FOREVER);
    ptmr->Alarm      = CN_LIMIT_SINT64;     //�����޳�����ʱ����ʾ��ͣ
    dListRemove(&ptmr->node_sys);
    __GDD_TimerAdd(ptmr);
    Lock_MutexPost(s_ptGddTimerQSync);
    return ;
}

//----������ʱ��----------------------------------------------------------------
//����: ����һ����ʱ���������ʱ���Ѿ���������ʱʱ�����¼���
//������ptmr: ��ʱ������.
//���أ���
//------------------------------------------------------------------------------
void GDD_StartTimer(struct WinTimer *ptmr)
{
    if(ptmr==NULL)
        return;
    Lock_MutexPend(s_ptGddTimerQSync,CN_TIMEOUT_FOREVER);
    ptmr->Alarm      = DJY_GetSysTime( ) + ptmr->Interval * 1000;
    dListRemove(&ptmr->node_sys);
//  dListRemove(&ptmr->node_msg_timer);
    __GDD_TimerAdd(ptmr);
    Lock_MutexPost(s_ptGddTimerQSync);
    //���������Ķ�ʱ���Ƿ���ڶ���ͷ��
    if(&ptmr->node_sys == dListGetAfter(&sg_GddTimerList))
    {
        //����Ƕ���ͷ���������ͷ��ź�����ʹ��ʱ��ɨ�躯�����¶�ʱʱ��
        Lock_SempPost(s_ptGddTimerRun);
    }
    return ;
}
//----ɾ����ʱ��----------------------------------------------------------------
//����: ɾ��һ����ʱ��
//������ptmr: ��ʱ������.
//���أ���
//------------------------------------------------------------------------------
void GDD_DeleteTimer(struct WinTimer*ptmr)
{
    if(ptmr==NULL)
        return;
    Lock_MutexPend(s_ptGddTimerQSync,CN_TIMEOUT_FOREVER);
    dListRemove(&ptmr->node_sys);
    Lock_MutexPost(s_ptGddTimerQSync);

    if(__HWND_Lock(ptmr->hwnd))
    {
        dListRemove(&ptmr->node_hwnd);
        dListRemove(&ptmr->node_msg_timer);
        __HWND_Unlock(ptmr->hwnd);
    }
    __GDD_TimerFree(ptmr);
    return;
}

//----ɾ���������ж�ʱ��-------------------------------------------------------
//����: �ú���ΪGDD�ڲ�����
//������hwnd: ���ھ��.
//���أ���.
//------------------------------------------------------------------------------
void __GDD_RemoveWindowTimer(HWND hwnd)
{
    list_t *n;
    struct WinTimer *ptmr;

    n = (&hwnd->list_timer)->next;
    for (;n != (&hwnd->list_timer); n = (&hwnd->list_timer)->next)
    {
        ptmr =(struct WinTimer*)dListEntry(n,struct WinTimer,node_hwnd);

        Lock_MutexPend(s_ptGddTimerQSync,CN_TIMEOUT_FOREVER);
        dListRemove(&ptmr->node_sys);
        Lock_MutexPost(s_ptGddTimerQSync);

        if(__HWND_Lock(hwnd))
        {
            dListRemove(&ptmr->node_hwnd);
            dListRemove(&ptmr->node_msg_timer);
            __HWND_Unlock(hwnd);
        }
        __GDD_TimerFree(ptmr);
    }
}

//----���ڶ�ʱ��ɨ��-----------------------------------------------------------
//���ܣ�ɨ�贰�ڶ�ʱ�����У��ö������� sg_GddTimerList����������ʱ���Ⱥ�����
//������
//���أ�������
//------------------------------------------------------------------------------

ptu32_t GDD_TimerScan(void)
{
    list_t *tmp;
    struct WinTimer *timer;
    s64 NowTime;
    while(1)
    {
        if(dListIsEmpty(&sg_GddTimerList))
        {
            //��ʱ�����пգ�ֻ�д�����ʱ�����ͷ��ź���
            Lock_SempPend(s_ptGddTimerRun, CN_TIMEOUT_FOREVER);
        }
        else
        {
            NowTime = DJY_GetSysTime( );
            Lock_MutexPend(s_ptGddTimerQSync,CN_TIMEOUT_FOREVER);
            tmp = (&sg_GddTimerList)->next;
            timer = dListEntry(tmp,struct WinTimer,node_sys);
            if(timer->Alarm > NowTime)
            {
                Lock_MutexPost(s_ptGddTimerQSync);
                //��Ϊ�ȴ��ź�����������ʱ��ʱ�������ź���������ͷ���Ķ�ʱ����ʱʱ�䵽
                //��ʱ���ء�
                Lock_SempPend( s_ptGddTimerRun, timer->Alarm - NowTime);
            }
            else
            {
                timer->Alarm += timer->Interval * mS;
                dListRemove(tmp);
                __GDD_TimerAdd(timer);
                Lock_MutexPost(s_ptGddTimerQSync);
                GDD_PostMessage(timer->hwnd, MSG_TIMER, (ptu32_t)timer->Id, (ptu32_t)timer);
            }
        }
    }
}




