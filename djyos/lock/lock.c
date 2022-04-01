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
//3. ����:2013-2-20
//   ����:  ������.
//   �°汾�ţ�1.1.0
//   �޸�˵��: �޸�mutex��ش��룬ʹ֮��ͬһ���¼��ڣ��ɵݹ���ã�����һ���¼�
//      ӵ��mutex�������¼��ٵ���pend�Ļ������������������ٴε���pend��������
//      ���ü����������������������ͬ��������post�󣬲��������ͷ�mutex��
//2. ����:2013-1-4
//   ����:  ������.
//   �°汾�ţ�1.0.1
//   �޸�˵��: ������mutex_delete������һ�����󣬸�bug������"��ë��"�ύ
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "int.h"
#include "lock.h"
#include "pool.h"
#include "djyos.h"
#include "object.h"
#include "objhandle.h"
#include "systime.h"
#include "dbug.h"

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"lock"//��ģ�飬�����������ź���
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:system              //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:none                //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"             //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //�������������⣬�������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //�������������⣬�������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_LOCK == false )
//#warning  " lock  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_LOCK    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100
#define CFG_LOCK_LIMIT          40      //"��������",�������������������ź����ͻ�����
//%$#@enum,true,false
//%$#@string,1,10
//%$#@select
//%$#@free
#endif
//%$#@end configue  ****�������ý���

//@#$%component end configure


//˵���� CFG_LOCK_LIMIT ���û����õģ������û�����֪������ϵͳ��Ҫ�ö����ź�����
//      ���Բ���ϵͳ����ռ�� CFG_LOCK_LIMIT ָ�꣬�û�ʹ�õ��ź�����
//      tg_semp_pool������ڴ���з��䣬����ϵͳʹ�õ��ź����Լ����壬����
//      ��أ�����û���ͼ���� Lock_SempDelete ɾ������ϵͳ������ź��������������
//      �ڴ�й©��������ز���ϵͳ��ԴҲ��ʧȥ�ź����������������Ԥ��
//�����ź����ṹ��ʼ�ڴ��
static union lock_MCB *g_ptLockMemBlock;
struct MemCellPool *g_ptLockPool;  //�ź����ṹ�ڴ��ͷָ��
//static struct SemaphoreLCB *s_tSempHead;
//static struct MutexLCB *s_tMutexHead;
static struct dListNode s_tSempHead = {&s_tSempHead, &s_tSempHead};
static struct dListNode s_tMutexHead = {&s_tMutexHead, &s_tMutexHead};

extern void __DJY_EventReady(struct EventECB *event_ready);
extern void __DJY_ResumeDelay(struct EventECB *delay_event);
extern void __DJY_AddToDelay(u32 u32l_uS);
extern void __DJY_AddRunningToBlock(struct EventECB **Head,bool_t Qsort,u32 timeout,u32 Status);

void __Lock_ShowSemp(void);
void __Lock_ShowMutex(void);
s32 Lock_SempObjOps(void *opsTarget, u32 cmd, ptu32_t OpsArgs1,
                        ptu32_t OpsArgs2, ptu32_t OpsArgs3);
s32 Lock_MutexObjOps(void *opsTarget, u32 cmd, ptu32_t OpsArgs1,
                        ptu32_t OpsArgs2, ptu32_t OpsArgs3);

//----��ʼ����ģ��ģ��step1----------------------------------------------------
//���ܣ���ʼ���ź���ģ��ĵ�һ�����˺���Ե��ó�semp_create��mutex_create�����
//      ��������������semp_create_s��mutex_createe_s��������������ֹ����
//      Ϊmodule_init_memb��û�е��ã�����mudule_init_memb��Ҫʹ���ź�������ֻ��
//      ��module_init_lock1������á�
//��������
//���أ�1
//-----------------------------------------------------------------------------
//ptu32_t ModuleInstall_Lock1(ptu32_t para)
//{
//    static struct Object semp_root;
//    static struct Object mutex_root;
//    para = para;        //�����������澯
//    s_ptMutexObject = __Lock_RscAddLockTree(&semp_root,sizeof(struct Object),"semaphore");
//    s_ptSempObject = __Lock_RscAddLockTree(&mutex_root,sizeof(struct Object),"mutex");
//    return 1;
//}

//----��ʼ����ģ��ģ��step2----------------------------------------------------
//���ܣ���ʼ���ź���ģ��ĵڶ������˺���Ե��ú�semp_create��mutex_createe���ڵ�
//      ������������
//��������
//���أ�1
//-----------------------------------------------------------------------------
ptu32_t __InitLock(void)
{
    static struct MemCellPool lock_pool;

    g_ptLockMemBlock = M_Malloc(CFG_LOCK_LIMIT* sizeof(union lock_MCB),0);
    if(g_ptLockMemBlock == NULL)
        return 0;

    //�ر���ʾ����װlockģ��ʱ���ļ�ϵͳ��û�а�װ��ɣ������������Ĺ��̴���
    //�ʹ�mutex��semaphore�ļ���
//    s_ptSempObject = OBJ_SearchChild(OBJ_GetRoot( ), "semaphore");
//    OBJ_SetOps(s_ptSempObject,Lock_SempObjOps);
//  s_ptSempFp = OBJ_GetFirstFile(s_ptSempObject);
#if 0
    dListInit(&s_tSempHead);

//    OBJ_SetPrivate(s_ptSempObject, (ptu32_t)&s_tSempHead);

//    s_ptMutexObject = OBJ_SearchChild(OBJ_GetRoot( ), "mutex");
//    OBJ_SetOps(s_ptMutexObject,Lock_MutexObjOps);
//  s_ptMutexFp = OBJ_GetFirstFile(s_ptMutexObject);
    dListInit(&s_tMutexHead);
#endif
//    OBJ_SetPrivate(s_ptMutexObject, (ptu32_t)&s_tMutexHead);

    //��ʼ�������ƿ��ڴ��
    g_ptLockPool = Mb_CreatePool_s( &lock_pool,
                                    (void*)g_ptLockMemBlock,
                                    CFG_LOCK_LIMIT,
                                    sizeof(union lock_MCB),
                                    16, 16384,
                                    "�����ƿ��");
    return 1;
}


// ============================================================================
// ���ܣ���װLOCK�ļ�ϵͳ
// ������
// ���أ�
// ��ע��
// ============================================================================
s32 Lock_CreateObject(void)
{
    if(!OBJ_NewChild(OBJ_GetRoot(), (fnObjOps)Lock_MutexObjOps, 0, "mutex"))
        return (-1);

    if(!OBJ_NewChild(OBJ_GetRoot(), (fnObjOps)Lock_SempObjOps, 0, "semaphore"))
        return (-1);

    return (0);
}


//----����һ���ź���-----------------------------------------------------------
//���ܣ�����һ���ź������Ժ�Ϳ���ʹ������ź����ˡ��ź������ƿ���ڴ���з��䡣
//������semp_limit,�źŵƵ�������-1=�����޶���źŵơ�
//      init_lamp,��ʼ�źŵ�����
//      sync_order,���������¼��Ŷ�˳��ȡֵΪ CN_BLOCK_FIFO
//          �� CN_BLOCK_PRIO��ע�⣬���������������ȼ��Ŷӡ�
//      name���ź��������֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//���أ��½������ź���ָ��
//-----------------------------------------------------------------------------
struct SemaphoreLCB *Lock_SempCreate(s32 lamps_limit,s32 init_lamp,
                                     u32 sync_order,const char *name)
{
    struct SemaphoreLCB *semp;
    if(init_lamp > lamps_limit)
        return NULL;
    semp = Mb_Malloc(g_ptLockPool,0);
    if(semp == NULL)
        return NULL;
    semp->sync_order = sync_order;
    semp->lamps_limit = lamps_limit;
    semp->lamp_counter = init_lamp;
    semp->semp_sync = NULL;
    semp->name = name;
    //���½ڵ�ҵ��ź���������
    dListInsertAfter(&s_tSempHead, &semp->List);
//  OBJ_AddChild(s_ptMutexObject,&semp->node,
//                      sizeof(struct SemaphoreLCB),RSC_SEMP,name);
    return semp;
}

//----�����ź���---------------------------------------------------------------
//���ܣ�����һ���ź��������������ṩ�ź������ƿ飬�߿ɿ��Ե�Ӧ���У���Ӧ��ʹ�ö�
//      ̬����ķ�ʽ����̬������ɿ���Ȼ���ָ�봫�ݹ������ں���ʹ�õ��ź�������
//      ��ʹ�þ�̬����ġ�Ӧ�ó�������cfg_locks_limitʱ��ֻ��Ҫ�����Լ��������
//      �����ˡ�
//������semp,Ŀ���ź���ָ��
//      semp_limit,�źŵƵ�������-1=�����޶���źŵơ�
//      init_lamp,��ʼ�źŵ�����
//      sync_order,���������¼��Ŷ�˳��ȡֵΪ CN_BLOCK_FIFO
//          �� CN_BLOCK_PRIO��ע�⣬���������������ȼ��Ŷӡ�
//      name���ź��������֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//���أ��½������ź���ָ��
//-----------------------------------------------------------------------------
struct SemaphoreLCB *Lock_SempCreate_s( struct SemaphoreLCB *semp,
                       s32 lamps_limit,s32 init_lamp,u32 sync_order,const char *name)
{
    if(semp == NULL)
        return NULL;
    semp->sync_order = sync_order;
    semp->lamps_limit = lamps_limit;
    semp->lamp_counter = init_lamp;
    semp->semp_sync = NULL;
    semp->name = name;
    //���½ڵ�ҵ��ź���������
    dListInsertAfter(&s_tSempHead, &semp->List);

    return semp;
}

//----����һ���źŵ�------------------------------------------------------------
//���ܣ��ͷ�һ���ź����������ź�����1��
//������semp,�ź���ָ��
//���أ���
//-----------------------------------------------------------------------------
void Lock_SempPost(struct SemaphoreLCB *semp)
{
    struct EventECB *event;

    if(semp == NULL)
        return;
    Int_SaveAsynSignal();
    if(semp->semp_sync == NULL)     //�ȴ����пգ�����һյ�źŵ�
    {
        if(semp->lamps_limit == -1)        //�źŵ���������
        {
            if(semp->lamp_counter != CN_LIMIT_SINT32)   //�źŵ�����δ������
                semp->lamp_counter ++;                  //����һյ�źŵ�
        }else if(semp->lamp_counter < semp->lamps_limit )//�źŵ�����δ������
            semp->lamp_counter++;                       //����һյ�źŵ�
    } else      //���¼��ڵȴ������źŵ�ֱ��ת�������������ȼ���ߵ��¼�
    {
        event = semp->semp_sync;
        event->sync_head = NULL;
        if(event->multi_next == event)
        {//������ֻ��һ���¼�
            semp->semp_sync = NULL;
        }else
        {//�������ж���¼���ȡ������ͷ�����¼�
            semp->semp_sync = event->multi_next;
            event->multi_next->multi_previous = event->multi_previous;
            event->multi_previous->multi_next = event->multi_next;
            event->multi_next = NULL;
            event->multi_previous = NULL;
        }
        if(event->event_status & CN_STS_SYNC_TIMEOUT)
            __DJY_ResumeDelay(event);    //����¼��ڳ�ʱ�ȴ������У�ȡ��
        event->wakeup_from = CN_STS_WAIT_SEMP;
        event->event_status = CN_STS_EVENT_READY;
        __DJY_EventReady(event);
    }
    Int_RestoreAsynSignal();
}

//----�ź�������---------------------------------------------------------------
//���ܣ������ź�����������ͬʱ��������Ҳ��Ӧ����
//������semp���ź���ָ��
//      Num�����ݵ�����
//���أ���
//-----------------------------------------------------------------------------
void Lock_SempExpand(struct SemaphoreLCB *semp, s32 Num)
{
    struct EventECB *event,*EventHead;
    s32 BlockingNum = Num;

    if((semp == NULL) || (Num <= 0) )
        return;
    Int_SaveAsynSignal();

    if(semp->lamps_limit != -1)             //�źŵ���������
    {
        if(CN_LIMIT_SINT32 - semp->lamps_limit >= Num)
        {
            semp->lamps_limit += Num;           //�źŵ���������
        }
        else
        {
            BlockingNum = CN_LIMIT_SINT32 - semp->lamps_limit;
            semp->lamps_limit = CN_LIMIT_SINT32;
        }
    }

    if(semp->semp_sync != NULL)     //�ȴ����зǿգ����ȵ����źŵ�
    {
        EventHead = semp->semp_sync;
        event = EventHead;
        while(1)
        {
            if(event->multi_next == event)
            {//������ֻʣ��һ���¼�
                semp->semp_sync = NULL;
            }else
            {//�������ж���¼���ȡ������ͷ�����¼�
                semp->semp_sync = event->multi_next;
                event->multi_next->multi_previous = event->multi_previous;
                event->multi_previous->multi_next = event->multi_next;
                event->multi_next = NULL;
                event->multi_previous = NULL;
            }
            if(event->event_status & CN_STS_SYNC_TIMEOUT)
                __DJY_ResumeDelay(event);    //����¼��ڳ�ʱ�ȴ������У�ȡ��
            event->wakeup_from = CN_STS_WAIT_SEMP;
            event->event_status = CN_STS_EVENT_READY;
            __DJY_EventReady(event);
            BlockingNum--;
            if((BlockingNum == 0) || (semp->semp_sync == NULL) )
            {
                break;
            }
        }
    }
    semp->lamp_counter += BlockingNum;              //������Ӧ�����źŵ�
    if(semp->lamp_counter < 0 )
        semp->lamp_counter = CN_LIMIT_SINT32;

    Int_RestoreAsynSignal();
}

//----����һյ�źŵ�-----------------------------------------------------------
//���ܣ�����Ȼ��Ϩ��һյ�źŵƣ���ʾ������Դ����1��
//������semp,�ź���ָ��
//      timeout����ʱ����,��λ��΢�룬CN_TIMEOUT_FOREVER=���޵ȴ���0��������
//      ��ʱ���ء���0ֵ�������ϵ���ΪCN_CFG_TICK_US��������
//���أ�true=ȡ���źŷ��ػ���Դ�����źŵƱ���(semp == NULL)��semp==NULLʱ����
//      true��������ģ������ϣ�������ڲ����Ƿ����źŵƱ���������һ�£���semp
//      ��ΪNULL�����ǵ�ѡ�񣬱�����Ҫ����һ�������źŵƱ������豸���ɰѸ��豸
//      struct pan_device��semp��Ա��ΪNULL��
//      false=û��ȡ���ź�(��ʱ���ػ�����ԭ��)
//-----------------------------------------------------------------------------
bool_t Lock_SempPend(struct SemaphoreLCB *semp,u32 timeout)
{
    bool_t lamp,sch;

    if(semp == NULL)
    {
        g_ptEventRunning->error_no = EN_LOCK_PARA_ERROR;
        return false;
    }
    sch = DJY_QuerySch();
    Int_SaveAsynSignal();
    if(semp->lamps_limit == -1)   //���ź��������޶��źŵ�
    {
        if(semp->lamp_counter != 0) //�źŵ�������Ϊ0
            semp->lamp_counter--;   //Ϩ��һյ�źŵ�
        lamp = true;
    }else                           //���ź������źŵ���������
    {
        if(semp->lamp_counter > 0)  //�е������źŵ�
        {
            lamp = true;            //�����Դ����
            semp->lamp_counter--;   //Ϩ��һյ�źŵ�
        }else                       //û�е������źŵ�
            lamp = false;           //�����Դ������
    }

    if(lamp == true)
    {
        Int_RestoreAsynSignal();  //�ָ��жϣ��������������л�
        return true;    //ȡ���źŵƷ���
    }
    else if(!sch || (timeout == 0))//��ֹ����ʱ,��������.��timeoutΪ0
    {
        Int_RestoreAsynSignal();  //�ָ��жϣ��������������л�
        return false;   //û��ȡ���źŵƷ���
    }

    __DJY_AddRunningToBlock(&(semp->semp_sync),semp->sync_order,timeout,CN_STS_WAIT_SEMP);

    Int_RestoreAsynSignal();  //�ָ��жϣ��������������л�
    //�������ﷵ�أ��ǳ�ʱ����ͬ���¼���ɡ�
    if(g_ptEventRunning->wakeup_from & CN_STS_SYNC_TIMEOUT)
    {//˵��ͬ������δ�����ӳ�ʱ���أ�Ӧ��Ŀ���¼���ͬ��������ȡ���¼���
     //��ʱ����ͬ�����¼��϶���û����ɡ�
        return false;
    }else
    {//˵���ǵõ��ź�������
        return true;
    }
}

//----ɾ���ں��ź���-----------------------------------------------------------
//���ܣ�ɾ��һ���ź�������semp_create_s������Ӧ
//������semp����ɾ�����ź���
//���أ���
//-----------------------------------------------------------------------------
bool_t Lock_SempDelete_s(struct SemaphoreLCB *semp)
{
    bool_t result;
    if(semp == NULL)    //��������
        return false;
    Int_SaveAsynSignal();
    if(semp->semp_sync == NULL)     //û���¼��ڵȴ��źŵ�
    {
        dListRemove(&semp->List);  //ɾ���ź������
        result = true;
    }else
        result = false;
    Int_RestoreAsynSignal();
    return result;
}

//----ɾ��һ���ź���-----------------------------------------------------------
//���ܣ�ɾ��һ���ź���
//������semp����ɾ�����ź���
//���أ���
//-----------------------------------------------------------------------------
bool_t Lock_SempDelete(struct SemaphoreLCB *semp)
{
    bool_t result;
    if(semp == NULL)                //��������
        return false;
    Int_SaveAsynSignal();
    if(semp->semp_sync == NULL)     //û���¼��ڵȴ��źŵ�
    {
        dListRemove(&semp->List);  //ɾ���ź������
        Mb_Free(g_ptLockPool,semp); //�ͷ��ڴ�
        result = true;
    }else
        result = false;
    Int_RestoreAsynSignal();
    return result;
}

//----��ѯ�ź�������-----------------------------------------------------------
//���ܣ���ѯһ���ź��������źŵƵ�����
//������semp������ѯ���ź���
//���أ��źŵ�����
//-----------------------------------------------------------------------------
u32 Lock_SempQueryCapacital(struct SemaphoreLCB *semp)
{
    u32 result;
    if(semp == NULL)            //��������
        return 0;
    else
    {
#if (32 > CN_CPU_BITS)
        atom_low_t atom_low;
        atom_low = Int_LowAtomStart();
#endif
        result = semp->lamps_limit;     //������ʹ�õ��źŵ�����
#if (32 > CN_CPU_BITS)
        Int_LowAtomEnd();
#endif
    }
    return result;
}

//----��ѯ�����źŵ�����-------------------------------------------------------
//���ܣ���ѯһ���ź��������źŵƵ�����
//������semp������ѯ���ź���
//���أ��źŵ�����
//-----------------------------------------------------------------------------
u32 Lock_SempQueryFree(struct SemaphoreLCB *semp)
{
    u32 result;
    if(semp == NULL)            //��������
        return 0;
    else
    {
#if (32 > CN_CPU_BITS)
        atom_low_t atom_low;
        atom_low = Int_LowAtomStart();
#endif
        result = semp->lamp_counter;     //������ʹ�õ��źŵ�����
#if (32 > CN_CPU_BITS)
        Int_LowAtomEnd();
#endif
    }
    return result;
}

//----��ѯ����-----------------------------------------------------------
//���ܣ���ѯ���ź����Ƿ����¼�������
//������mutex������ѯ�Ļ�����
//���أ�true = �У�false = û��
//-----------------------------------------------------------------------------
bool_t Lock_SempCheckBlock(struct SemaphoreLCB *Semp)
{
    if(Semp == NULL)    //��������
        return false;
    if(NULL != Semp->semp_sync)
        return true;
    else
        return false;
}

//----����ͬ����������ʽ-----------------------------------------------------
//����: ���õȴ��ź������¼�������ʽ������Ϊ�����������ʱ�����򣬻��߰�������
//      ���¼������ȼ������ź�����������ʱ��Ĭ���ǰ����ȼ�����ġ�
//      ������ֻӰ����ñ������������е��¼���ԭ���ڶ����е��¼�����Ӱ�졣
//����: semp: �����õ��ź���
//      order: CN_BLOCK_PRIO=���ȼ��Ŷӣ�sort_time=�Ⱥ�˳���Ŷ�
//����: ��
//-----------------------------------------------------------------------------
void Lock_SempSetSyncSort(struct SemaphoreLCB *semp,u32 order)
{
    if(NULL == semp)
        return;
    semp->sync_order = order;
}
//-----------------------�����ǻ�����------------------------------------------

//----����һ���ź���-----------------------------------------------------------
//���ܣ�����һ����������
//������name�������������֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//���أ��½����Ļ�����ָ��
//-----------------------------------------------------------------------------
struct MutexLCB *Lock_MutexCreate(const char *name)
{
    struct MutexLCB *mutex;
    mutex = Mb_Malloc(g_ptLockPool,0);
    if(mutex == NULL)
        return NULL;
    mutex->enable = 0;   //�������ĳ�ʼ״̬�ǿ��õ�,�������ָ��onwer
    mutex->mutex_sync = NULL;
//  mutex->prio_bak = CN_PRIO_INVALID;
    mutex->owner = NULL;
    mutex->name = name;
    //���½ڵ�ҵ�������������
    dListInsertAfter(&s_tMutexHead, &mutex->List);
    //���½ڵ�ҵ��ź������ڵ���
//  OBJ_AddChild(s_ptSempObject,&mutex->node,
//                      sizeof(struct MutexLCB),RSC_MUTEX,name);
    return mutex;
}

//----����������---------------------------------------------------------------
//���ܣ�����һ�������������������ṩ���������ƿ飬�߿ɿ��Ե�Ӧ���У���Ӧ��ʹ�ö�
//      ̬����ķ�ʽ����̬������ɿ���Ȼ���ָ�봫�ݹ������ں���ʹ�õĻ���������
//      ��ʹ�þ�̬����ġ�Ӧ�ó�������cfg_locks_limitʱ��ֻ��Ҫ�����Լ��������
//      �����ˡ�
//������mutex,������ָ��
//      name�������������֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//���أ��½����Ļ�����ָ��
//-----------------------------------------------------------------------------
struct MutexLCB *Lock_MutexCreate_s( struct MutexLCB *mutex,const char *name)
{
    if(mutex == NULL)
        return NULL;
    mutex->enable = 0;   //�������ĳ�ʼ״̬�ǿ��õ�,�������ָ��onwer
    mutex->mutex_sync = NULL;
//  mutex->prio_bak = CN_PRIO_INVALID;
    mutex->owner = NULL;
    mutex->name = name;
    //���½ڵ�ҵ�������������
    dListInsertAfter(&s_tMutexHead, &mutex->List);
    //���½ڵ�ҵ��ź������ڵ���
//  OBJ_AddChild(s_ptSempObject,&mutex->node,
//                      sizeof(struct MutexLCB),RSC_MUTEX,name);
    return mutex;
}

struct EventECB *__DJY_GetIdle(void);
//----�ͷ�һ��������-----------------------------------------------------------
//���ܣ��ͷŻ�������ֻ�л�������ӵ���߲����ͷŻ�������
//������mutex,������ָ��
//���أ���
//-----------------------------------------------------------------------------
void Lock_MutexPost(struct MutexLCB *mutex)
{
    struct EventECB *event;

    if(mutex == NULL)
        return;
    if(Int_GetRunLevel() >0)      //�첽�ź����ͷŻ�����
    {
        if(mutex->enable == -1)
        {
            mutex->enable = 0;
        }
        return;
    }
    else
    {
        if(mutex->enable == -1)
        {
            return;                         //����Ӧ�ò����ߵ�����ݴ�һ��
        }
    }
    if((mutex->owner != g_ptEventRunning)   //������ֻ����ӵ�����ͷ�
        &&(mutex->owner != __DJY_GetIdle( ))) //���Ƕ��¼����ȿ�ʼǰ pend �Ļ�����
        return;
    Int_SaveAsynSignal();
    if(mutex->enable > 0)
        mutex->enable--;
    else
        event = NULL;
    if(mutex->enable == 0)
    {
        if(mutex->mutex_sync == NULL)     //�ȴ����пգ����û�����Ϊ����״̬
        {
            mutex->owner = NULL;
        } else      //���¼��ڵȴ����ѻ�����ֱ��ת�������������ȼ���ߵ��¼�
        {
            event = mutex->mutex_sync;
            event->sync_head = NULL;
            mutex->owner = event;
            mutex->enable = 1;
            if(event->multi_next == event)
            {//������ֻ��һ���¼�
                mutex->mutex_sync = NULL;
            }else
            {//�������ж���¼���ȡ������ͷ�����¼�
                mutex->mutex_sync = event->multi_next;
                event->multi_next->multi_previous = event->multi_previous;
                event->multi_previous->multi_next = event->multi_next;
                event->multi_next = NULL;
                event->multi_previous = NULL;
            }
            if(event->event_status & CN_STS_SYNC_TIMEOUT)
                __DJY_ResumeDelay(event);    //����¼��ڳ�ʱ�ȴ������У�ȡ��
            event->event_status = CN_STS_EVENT_READY;
            event->wakeup_from = CN_STS_WAIT_MUTEX;
//          if( (mutex->prio_bak != CN_PRIO_INVALID)  //�û��������������ȼ��̳�
//             ||(!Djy_IsEventPrioChanged(event->event_id))) //���������ı����ȼ�
            DJY_RestorePrio( );
            __DJY_EventReady(event);
        }
    }
    Int_RestoreAsynSignal();
}

//----���󻥳���-----------------------------------------------------------
//���ܣ����󻥳��������ɹ���ӵ�иû�������������owner��Ա�����������ʺ����¼�֮
//      ����¼����첽�ź�֮��ͬ����ֻ�д���owner��Ա������ʵ�����ȼ��̳С�
//      ������첽�ź������󻥳������򲻻������������ƣ�Ҳ���������ȼ��̳С�
//������mutex,������ָ��
//      timeout����ʱ����,��λ��΢�룬CN_TIMEOUT_FOREVER=���޵ȴ���0��������
//      ��ʱ���ء���0ֵ�������ϵ���ΪCN_CFG_TICK_US��������
//���أ�true=ȡ���źŷ��ػ���Դ�����źŵƱ���(mutex == NULL)��mutex==NULLʱ����
//      true��������ģ������ϣ�������ڲ����Ƿ����źŵƱ���������һ�£���semp
//      ��ΪNULL�����ǵ�ѡ�񣬱�����Ҫ����һ�������źŵƱ������豸���ɰѸ��豸
//      struct pan_device��semp��Ա��ΪNULL��
//      false=û��ȡ���ź�(��ʱ���ػ�����ԭ��)
//-----------------------------------------------------------------------------
bool_t Lock_MutexPend(struct MutexLCB *mutex,u32 timeout)
{
    struct EventECB *pl_ecb;
    bool_t lamp,sch;

    if(DJY_IsMultiEventStarted() == false)
    {
        return true;        //������Ȼ�δ��ʼ��ֱ�ӷ���true
    }
    if(mutex == NULL)
    {
        if(Int_GetRunLevel() == 0)
            g_ptEventRunning->error_no = EN_LOCK_PARA_ERROR;
        return false;
    }
    //�첽�ź������󻥳�����������������
    if(Int_GetRunLevel() >0)
    {
        if(mutex->enable == 0)
        {
            mutex->enable = -1;
            return true;
        }
        else
        {
            g_ptEventRunning->error_no = EN_LOCK_BLOCK_IN_INT;
            return false;
        }
    }
    else
    {
        if(mutex->enable == -1)
        {
            return false;                  //����Ӧ�ò����ߵ�����ݴ�һ��
        }
    }
    if(g_ptEventRunning == mutex->owner)
    {
        mutex->enable++;
        return true;
    }
    sch = DJY_QuerySch();
    Int_SaveAsynSignal();
    if(mutex->enable == 0)   //�źŵƿ���
    {
        lamp = true;
        mutex->enable = 1;
//      mutex->prio_bak = CN_PRIO_INVALID;
        mutex->owner = g_ptEventRunning;
    }else                       //�źŵƲ�����
    {
        lamp = false;
    }

    if(lamp == true)
    {
        Int_RestoreAsynSignal();  //�ָ��жϣ��������������л�
        return true;    //ȡ�û���������
    }
    else if(!sch || (timeout == 0))//��ֹ����ʱ,��������.��timeout=0
    {
        Int_RestoreAsynSignal();  //�ָ��жϣ��������������л�
        return false;   //û��ȡ�û���������
    }

    __DJY_AddRunningToBlock(&(mutex->mutex_sync),CN_BLOCK_PRIO,timeout,CN_STS_WAIT_MUTEX);

    //���濴���Ƿ�Ҫ�����ȼ��̳�
    pl_ecb = mutex->owner;
    DJY_RaiseTempPrio(pl_ecb->event_id);
    Int_RestoreAsynSignal();  //�ָ��жϣ��������������л�
    //�������ﷵ�أ��ǳ�ʱ����ͬ���¼���ɡ�
    if(g_ptEventRunning->wakeup_from & CN_STS_SYNC_TIMEOUT)
    {//˵��ͬ������δ�����ӳ�ʱ���أ�Ӧ��Ŀ���¼���ͬ��������ȡ���¼���
     //��ʱ����ͬ�����¼��϶���û����ɡ�
        return false;
    }else
    {//˵���ǵõ�����������
        return true;
    }
}

//----ɾ��������-----------------------------------------------------------
//���ܣ�ɾ��һ���ź�������mutex_create_s������Ӧ
//������mutex����ɾ���Ļ�����
//���أ�true=�ɹ�ɾ����false=ʧ��
//-----------------------------------------------------------------------------
bool_t Lock_MutexDelete_s(struct MutexLCB *mutex)
{
    bool_t result;
    if(mutex == NULL)    //��������
        return false;
    Int_SaveAsynSignal();
    if(mutex->mutex_sync == NULL)     //û���¼��ڵȴ�������
    {
        dListRemove(&mutex->List);  //ɾ�����������
        result = true;
    }else
        result = false;
    Int_RestoreAsynSignal();
    return result;
}

//----ɾ��һ��������-----------------------------------------------------------
//���ܣ�ɾ��һ��������
//������mutex����ɾ���Ļ�����
//���أ���
//-----------------------------------------------------------------------------
bool_t Lock_MutexDelete(struct MutexLCB *mutex)
{
    bool_t result;
    if(mutex == NULL)    //��������
        return false;
    Int_SaveAsynSignal();
    if(mutex->mutex_sync == NULL)     //û���¼��ڵȴ�������
    {
        dListRemove(&mutex->List);  //ɾ�����������
        Mb_Free(g_ptLockPool,mutex); //�ͷ��ڴ�
        result = true;
    }else
        result = false;
    Int_RestoreAsynSignal();
    return result;
}

//----��ѯһ��������-----------------------------------------------------------
//���ܣ���ѯ�û������Ƿ����
//������mutex������ѯ�Ļ�����
//���أ�true = ���ã�false = ������
//-----------------------------------------------------------------------------
bool_t Lock_MutexQuery(struct MutexLCB *mutex)
{
    if(mutex == NULL)    //��������
        return false;
    return ! mutex->enable;
}

//----��ѯ����-----------------------------------------------------------
//���ܣ���ѯ�û������Ƿ����¼�������
//������mutex������ѯ�Ļ�����
//���أ�true = �У�false = û��
//-----------------------------------------------------------------------------
bool_t Lock_MutexCheckBlock(struct MutexLCB *mutex)
{
    if(mutex == NULL)    //��������
        return false;
    if(NULL != mutex->mutex_sync)
        return true;
    else
        return false;
}

//----��ѯһ��������-----------------------------------------------------------
//���ܣ���ѯ�û�������ӵ����
//������mutex������ѯ�Ļ�����
//���أ�ӵ���ߵ��¼�ID
//-----------------------------------------------------------------------------
u16 Lock_MutexGetOwner(struct MutexLCB *mutex)
{
    if(mutex == NULL)    //��������
        return CN_EVENT_ID_INVALID;
    if( NULL != mutex->owner)
        return mutex->owner->event_id;
    else
        return CN_EVENT_ID_INVALID;
}

void __Lock_ShowMutex(void)
{
    struct dListNode *current;
    struct MutexLCB *Mutex;
    struct EventECB *pl_ecb;

    printf("������ mutex �б�\r\n");
    printf("״̬  ӵ����  ԭ���ȼ�  �����ȼ�  �����¼�  ����\r\n");
    dListForEach(current, &s_tMutexHead)
    {
        Mutex = dListEntry(current, struct MutexLCB,List);
        if(Mutex->enable == 0)
        {
            printf("����                              ");
        }
        else
        {
            printf("ռ��  ");
            pl_ecb = Mutex->owner;
            if(pl_ecb->prio_base != pl_ecb->prio)
            {
                printf("%05d   %03d       ",pl_ecb->event_id,
                        pl_ecb->prio_base);
                printf("%03d       ",pl_ecb->prio);
            }
            else
            {
                printf("%05d   δ�������ȼ��̳�    ",pl_ecb->event_id);
            }
        }

        pl_ecb = Mutex->mutex_sync;
        if(pl_ecb)
            printf("%05d     ",pl_ecb->event_id);
        else
            printf("��        ");

        if((Mutex->name != NULL) && (Mutex->name[0] != '\0'))
        {
            printf("%s\r\n", Mutex->name);
        }else
        {
            printf("����������\r\n");
        }
    }
            printf("�ź����ͻ���������:%d��������: %d \r\n",
            CFG_LOCK_LIMIT,Mb_QueryFree(g_ptLockPool));
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

//----�������ļ���������-------------------------------------------------------
//���ܣ�ֻʵ����һ�����ܣ������г�ȫ���������Ĳ�����״̬��
//������Target���ο�����ָ������ fnObjOps �Ķ���
//      cmd�������룬ֻ֧�� CN_OBJ_CMD_SHOW
//      para������
//���أ�true
//-----------------------------------------------------------------------------
s32 Lock_MutexObjOps(void *opsTarget, u32 objcmd, ptu32_t OpsArgs1,
                        ptu32_t OpsArgs2, ptu32_t OpsArgs3)
{
    s32 result = CN_OBJ_CMD_EXECUTED;

    switch(objcmd)
    {
        case CN_OBJ_CMD_SHOW:
        {
            __Lock_ShowMutex( );
            result = CN_OBJ_CMD_TRUE;
            break;
        }

        default:
        {
            result = CN_OBJ_CMD_UNSUPPORT;
            break;
        }
    }

    return (result);
}

void __Lock_ShowSemp(void)
{
    struct dListNode *current;
    struct SemaphoreLCB *Semp;
    struct EventECB *pl_ecb;

    printf("�ź��� semaphore �б�\r\n");
    printf("��������  �ź�������  �����ź���  �����¼�  ����\r\n");
    dListForEach(current, &s_tSempHead)
    {
        Semp = dListEntry(current, struct SemaphoreLCB,List);
        if(Semp->sync_order == CN_BLOCK_PRIO)
        {
            printf("���ȼ�  ");
        }
        else
        {
            printf("   fifo   ");
        }
            printf("%8d    %8d    ",Semp->lamps_limit,Semp->lamp_counter);
        pl_ecb = Semp->semp_sync;
        if(pl_ecb)
            printf("%05d     ",pl_ecb->event_id);
        else
            printf("��        ");

        if((Semp->name != NULL) && (Semp->name[0] != '\0'))
        {
            printf("%s\r\n", Semp->name);
        }
        else
        {
            printf("�����ź���\r\n");
        }
    }
    printf("\n\r�ź����ͻ���������:%d��������: %d \r\n",\
                    CFG_LOCK_LIMIT,Mb_QueryFree(g_ptLockPool));
}

//----�ź����ļ���������-------------------------------------------------------
//���ܣ�ֻʵ����һ�����ܣ������г�ȫ���ź����Ĳ�����״̬��
//������Target���ο�����ָ������ fnObjOps �Ķ���
//      cmd�������룬ֻ֧�� CN_OBJ_CMD_SHOW
//      para������
//���أ�true
//-----------------------------------------------------------------------------
s32 Lock_SempObjOps(void *opsTarget, u32 objcmd, ptu32_t OpsArgs1,
                        ptu32_t OpsArgs2, ptu32_t OpsArgs3)
{
    s32 result = CN_OBJ_CMD_EXECUTED;

    switch(objcmd)
    {
        case CN_OBJ_CMD_SHOW:
        {
            __Lock_ShowSemp( );
            result = CN_OBJ_CMD_TRUE;
            break;
        }
        default:
        {
            result = CN_OBJ_CMD_UNSUPPORT;
            break;
        }
    }

    return result;
}
#pragma GCC diagnostic pop

