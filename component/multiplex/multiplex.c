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
//����ģ��: multiplexģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����:ʵ�ֶ�·���õĹ���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2014-06-24
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "stddef.h"
#include "systime.h"
#include "object.h"
#include "objhandle.h"
#include "int.h"
#include "pool.h"
#include "lock.h"
#include <djyos.h>
#include "multiplex.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_Multiplex(void);
//    ModuleInstall_Multiplex ();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"multiplex"//��·����
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:system              //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"lock"  //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_MULTIPLEX == false )
//#warning  " multiplex  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_MULTIPLEX    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure


struct MultiplexSetsCB
{
    struct MultiplexObjectCB *ObjectQ;   // ָ��δ�����Ķ�������
    struct MultiplexObjectCB *ActiveQ;   // ָ���Ѵ����Ķ�������
    u32 ObjectSum;          // �����а����Ķ�������
    u32 ActiveLevel;        // ����ˮƽ�����������������ﵽActiveLevel��������
                            // ���󼯣�����ObjectSum ��ʾȫ����
    u32 Actived;            // �Ѿ������Ķ�����
    bool_t SetsActived;     // �����Ƿ��Ѿ�������
    struct SemaphoreLCB Lock;       // �ȴ�Sets�������õ�����
};

struct MultiplexObjectCB
{
    struct MultiplexObjectCB *NextObject;//����˫��������������һ��MultiplexSets
                                         //�����Ķ��object
    struct MultiplexObjectCB *PreObject;
    struct MultiplexObjectCB *NextSets; //��������������һ��object�����
                                        //MultiplexSets���������
    struct MultiplexSetsCB *MySets;     //ָ�������ƿ�
    s32 Fd;                             //��MultiplexSets�ȴ����ļ�
    ptu32_t ObjectID;                   //��MultiplexSets�ȴ��Ķ���
    u32 ET_SaveBit;                     // ETģʽ���� wait �������صĴ���λ��
    u32 PendingBit;                     //bit0~23���������Ѿ�������bit��
                                        //bit31��1=�����Ѽ��
    u32 SensingBit;                     //bit0~23������λ��־
                                        //bit24~31��ʾ����λ������ͣ��μ�
                                        //CN_MULTIPLEX_SENSINGBIT_MODE����
};

static struct MemCellPool *g_ptMultiplexSetsPool;
static struct MemCellPool *g_ptMultiplexObjectPool;
static struct MutexLCB MultiplexMutex;

//��Object��*SrcList����ȡ�����ŵ�*TarList����ͷ��ȥ
static void __Multiplex_ChangeList(struct MultiplexObjectCB **SrcList,
                        struct MultiplexObjectCB **TarList,
                        struct MultiplexObjectCB *Object)
{
    Object->NextObject->PreObject = Object->PreObject;
    Object->PreObject->NextObject = Object->NextObject;
    if (*SrcList == Object)
    {
        if (Object->NextObject == Object)
        {
            *SrcList = NULL;
        } else
        {
            *SrcList = Object->NextObject;
        }
    }
    //��Object�ŵ�Active������
    if (*TarList == NULL)
    {
        *TarList = Object;
        Object->NextObject = Object;
        Object->PreObject = Object;
    } else
    {
        Object->PreObject = (*TarList)->PreObject;
        Object->NextObject = *TarList;
        (*TarList)->PreObject->NextObject = Object;
        (*TarList)->PreObject = Object;

        *TarList = Object;
    }
}

//�ж�һ��Object�Ƿ��Ѿ�����
bool_t __Multiplex_ObjectIsActived(u32 Pending, u32 Sensing, u32 Mode)
{
    if (Mode & CN_MULTIPLEX_SENSINGBIT_OR)
    {
        if ((Pending & Sensing) != 0)
            return true;
        else
            return false;
    }
    else
    {
        if ((Pending & Sensing) == Sensing)
            return true;
        else
            return false;
    }
}

//----ģ���ʼ��---------------------------------------------------------------
//����: ��ʼ��Multiplexģ�顣��������������ʼ���ڴ�ء�
//������para����Ч��
//���أ�true=�ɹ���false=ʧ�ܡ�
//-----------------------------------------------------------------------------
bool_t ModuleInstall_Multiplex(void)
{
    Lock_MutexCreate_s(&MultiplexMutex, "Multiplex control block");
    //����MultiplexSets���ƿ��ڴ�أ��ڴ�صĳ�ʼ״̬�ǿյġ�
    g_ptMultiplexSetsPool = Mb_CreatePool(NULL, 0, sizeof(struct MultiplexSetsCB),
        5, CN_MULTIPLEX_MAX_SETS, "Multiplex control block");
    if (g_ptMultiplexSetsPool == NULL)
        return false;
    //����Multiplex������ƿ��ڴ�أ��ڴ�صĳ�ʼ״̬�ǿյġ�
    g_ptMultiplexObjectPool = Mb_CreatePool(NULL, 0, sizeof(struct MultiplexObjectCB),
        5, CN_MULTIPLEX_MAX_OBJECTS, "Multiplex control block");
    if (g_ptMultiplexObjectPool == NULL)
    {
        Mb_DeletePool(g_ptMultiplexObjectPool);
        return false;
    }
    return true;
}

//----����MultiplexSets----------------------------------------------------------
//���ܣ�����һ��MultiplexSets�������趨���ԡ�
//������ActiveLevel, ����ˮƽ,�ü��ϱ������Ķ���ﵽ������,��������.
//���أ��´�����MultiplexSetsָ��
//----------------------------------------------------------------------------
struct MultiplexSetsCB* Multiplex_Create(u32 ActiveLevel)
{
    struct MultiplexSetsCB *SetsQ;
    if (ActiveLevel == 0)       //����ˮƽ�������0
        return NULL;
    SetsQ = (struct MultiplexSetsCB *)Mb_Malloc(g_ptMultiplexSetsPool,
        CN_TIMEOUT_FOREVER);
    if (SetsQ != NULL)
    {
        SetsQ->ObjectQ = NULL;
        SetsQ->ActiveQ = NULL;
        SetsQ->ObjectSum = 0;
        SetsQ->ActiveLevel = ActiveLevel;
        SetsQ->Actived = 0;
        SetsQ->SetsActived = false;
        Lock_SempCreate_s(&SetsQ->Lock, 1, 0, CN_BLOCK_FIFO, NULL);
        return SetsQ;
    } else
        return NULL;
}

//----��Ӷ���MultiplexSets--------------------------------------------------
//����: MultiplexSets�����һ�����������Object�ĳ�ʼ״̬���Ѿ�����������뵽
//      ActiveQ���У��������ObjectQ���С�
//����: Sets����������MultiplexSetsָ��
//      Fd��������Sets���ļ���������
//      SensingBit��bit0~23��24������bit����Ϊ1��ʾ����������bit��־���У��μ�
//                          CN_MULTIPLEX_SENSINGBIT_READ �Ķ���
//                  bit24~31��ʾģʽ���μ� CN_MULTIPLEX_SENSINGBIT_OR �Ķ���
//����: true=�ɹ���false=ʧ�ܡ�
//-----------------------------------------------------------------------------
bool_t Multiplex_AddObject(struct MultiplexSetsCB *Sets,s32 Fd, u32 SensingBit)
{
    struct objhandle *Kfp;
    struct MultiplexObjectCB *temp;
    struct MultiplexObjectCB **TargetQ;
    u32 InitStatus;
    bool_t repeat = false;
    bool_t IsActived = 0;

    Kfp = fd2Handle(Fd);
    if ((NULL == Sets) || (NULL == Kfp) )
        return false;

    Lock_MutexPend(&MultiplexMutex, CN_TIMEOUT_FOREVER);
    temp = __Handle_GetMultiplexHead(Kfp);
    InitStatus = Handle_MultiEvents(Kfp);
    //ѭ�����һ��Object�Ƿ��ظ�����ͬһ��MultiplexSets
    //���ObjectHead=NULL,������ǲ��ظ������������ܹ���ȷ���С�
    while (temp != NULL)
    {
        if (temp->MySets != Sets)
            temp = temp->NextSets;
        else
        {
            repeat = true;
            break;
        }
    }
    Lock_MutexPost(&MultiplexMutex);

    if (repeat == false)
    {
        InitStatus &= ~CN_MULTIPLEX_MODEMSK;      //���ģʽλ
        //�������¼����Object�Ƿ��Ѿ��������Ծ������뵽MultiplexSets���ĸ�������
        if (__Multiplex_ObjectIsActived(InitStatus,
                SensingBit & CN_MULTIPLEX_STATUSMSK,
                SensingBit & CN_MULTIPLEX_MODEMSK))
        {
            TargetQ = &Sets->ActiveQ;
            IsActived = true;
        }
        else
            TargetQ = &Sets->ObjectQ;

        temp = (struct MultiplexObjectCB *)Mb_Malloc(g_ptMultiplexObjectPool, CN_TIMEOUT_FOREVER);
        if (temp != NULL)
        {
            Sets->ObjectSum++;
            temp->ET_SaveBit = 0;
            temp->SensingBit = SensingBit;
            temp->PendingBit = InitStatus & CN_MULTIPLEX_STATUSMSK;
            temp->Fd = Fd;
//          temp->ObjectID = ObjectID;
            Lock_MutexPend(&MultiplexMutex, CN_TIMEOUT_FOREVER);
            temp->MySets = Sets;            //�趨��������MultiplexSets
                                            //ͬһ��MultiplexSets�����������NextObject����Щ��������������
            if (*TargetQ == NULL)
            {
                *TargetQ = temp;
                temp->NextObject = temp;
                temp->PreObject = temp;
            } else
            {
                //�¼���MultiplexSets�Ķ���������ͷ��
                temp->PreObject = (*TargetQ)->PreObject;
                temp->NextObject = *TargetQ;
                (*TargetQ)->PreObject->NextObject = temp;
                (*TargetQ)->PreObject = temp;

                (*TargetQ) = temp;
            }
            //ͬһ�����󱻶��MultiplexSets��������NextSets���ӡ�
            //NextSets�ǵ��������¶����������ͷ��
            temp->NextSets = __Handle_GetMultiplexHead(Kfp);
            __Handle_SetMultiplexHead(Kfp, temp);
            Lock_MutexPost(&MultiplexMutex);
            if (IsActived)
            {
                Sets->Actived += 1;
                temp->PendingBit |= CN_MULTIPLEX_OBJECT_ACTIVED;
                if ((Sets->Actived >= Sets->ActiveLevel)
                    || (Sets->Actived >= Sets->ObjectSum))
                {
                    if (false == Sets->SetsActived)
                    {
                        Sets->SetsActived = true;
                        Lock_SempPost(&Sets->Lock);
                    }
                }
            }
        } else
            return false;
    } else
    {
        //�ظ����룬�������κδ���
    }
    return true;
}

//----��MultiplexSetsɾ������-----------------------------------------------------
//����: MultiplexAdd���溯��
//����: Sets����������MultiplexSetsָ��
//      Fd����ɾ���Ķ���
//����: true=�ɹ���false=ʧ�ܡ�
//-----------------------------------------------------------------------------
bool_t Multiplex_DelObject(struct MultiplexSetsCB *Sets,s32 Fd)
{
    struct objhandle *Kfp;
    struct MultiplexObjectCB *Object,*following;

    Kfp = fd2Handle(Fd);
    if ((Sets == NULL) || (Kfp == NULL))
        return false;
    Lock_MutexPend(&MultiplexMutex, CN_TIMEOUT_FOREVER);
    Object = __Handle_GetMultiplexHead(Kfp);
    following = NULL;
    while (Object != NULL)
    {       //���ұ�ɾ���Ķ�����ƿ�
        if (Object->MySets != Sets)
        {
            following = Object;
            Object = Object->NextSets;
        }
        else
        {
            break;
        }
    }

    if (Object != NULL)
    {
        //�����鱻ɾ����Object�Ƿ��Ѿ�������
        if (__Multiplex_ObjectIsActived(Object->PendingBit,
                Object->SensingBit & CN_MULTIPLEX_STATUSMSK,
                Object->SensingBit & CN_MULTIPLEX_MODEMSK))
        {
            if (Sets->Actived != 0)
            {
                Sets->Actived--;
                if (Sets->Actived == 0)
                    Sets->SetsActived = false;
            }
        }

        Sets->ObjectSum--;
        if (Object->NextObject != Object)
        {    //�����в�ֹһ�����
            Object->NextObject->PreObject = Object->PreObject;
            Object->PreObject->NextObject = Object->NextObject;
            if (Object == Sets->ObjectQ)
            {
                Sets->ObjectQ = Object->NextObject;
            }
            if (Object == Sets->ActiveQ)
            {
                Sets->ActiveQ = Object->NextObject;
            }
        } else {                                //����ֻ��һ�����
            if (Object == Sets->ObjectQ)
            {
                Sets->ObjectQ = NULL;
            }
            if (Object == Sets->ActiveQ)
            {
                Sets->ActiveQ = NULL;
            }
        }
        if(following == NULL)       // Fd������ͷ
            __Handle_SetMultiplexHead(Kfp, Object->NextSets);
        else
            following->NextSets = Object->NextSets;
        Mb_Free(g_ptMultiplexObjectPool, Object);
    }
    Lock_MutexPost(&MultiplexMutex);
    return true;
}

//----Multiplex����״̬--------------------------------------------------------
//���ܣ���֪MultiplexSets�б���ض���ĵ�ǰ״̬��
//����: ObjectHead����������Object����ͷָ��
//      Status��Object�ĵ�ǰ״̬
//����: true=�ɹ���false=ʧ�ܡ�
//-----------------------------------------------------------------------------
bool_t __Multiplex_Set(s32 Fd, u32 Status)
{
    struct objhandle *Kfp;
    struct MultiplexObjectCB *Object;
    struct MultiplexSetsCB *Sets;
    u32 Sensing, Type, NewPendsing,MaskPending;
    u32 OldPend;
    bool_t ActivedFlag;
    Kfp = fd2Handle(Fd);
    if (Kfp == NULL)
        return false;
//  Lock_MutexPend(&MultiplexMutex, CN_TIMEOUT_FOREVER);
    Object = __Handle_GetMultiplexHead(Kfp);
    while (Object != NULL)
    {
        Int_SaveAsynSignal();
        OldPend = Object->PendingBit;
        Sets = Object->MySets;
        ActivedFlag = Sets->SetsActived;
        Sensing = Object->SensingBit & CN_MULTIPLEX_STATUSMSK;
        Type = Object->SensingBit & CN_MULTIPLEX_MODEMSK;
        NewPendsing = Status & Sensing;
        Object->PendingBit = (OldPend & CN_MULTIPLEX_OBJECT_ACTIVED)|NewPendsing;
        if (Object->SensingBit & CN_MULTIPLEX_SENSINGBIT_ET)    //ET�����أ�ģʽ
        {
            MaskPending = (~(OldPend&Object->ET_SaveBit)) & NewPendsing;
            //����߼��Ƚϸ��ӣ��ο��������ĵ�
            Object->ET_SaveBit = Object->ET_SaveBit & OldPend & NewPendsing;
        }
        else
            MaskPending = NewPendsing;
        if (Object->PendingBit & CN_MULTIPLEX_OBJECT_ACTIVED)
        {                                           //����ǰ��Object�Ѵ���
            if (!__Multiplex_ObjectIsActived(MaskPending, Sensing, Type))
            {
                //����Multiplex_Set���¶�����δ����
                //��Object��Sets->ActiveQ�����ó����ŵ�ObjectQ������
                __Multiplex_ChangeList(&(Sets->ActiveQ), &(Sets->ObjectQ), Object);
                Object->PendingBit &= ~CN_MULTIPLEX_OBJECT_ACTIVED;
                if (Sets->Actived != 0)
                    Sets->Actived--;
                if (Sets->Actived == 0)
                    ActivedFlag = false;
            }
        } else
        {                                           //����ǰ��Objectδ����
            if (__Multiplex_ObjectIsActived(MaskPending, Sensing, Type))
            {
                //����Multiplex_Set���¶��󱻴���

                //��Object��Sets->ObjectQ�����ó����ŵ�ActiveQ������
                __Multiplex_ChangeList(&(Sets->ObjectQ), &(Sets->ActiveQ), Object);
                Object->PendingBit |= CN_MULTIPLEX_OBJECT_ACTIVED;
                if (Sets->Actived < Sets->ObjectSum)
                    Sets->Actived++;
                //���ͷ��ź���
                if ((Sets->Actived >= Sets->ActiveLevel)
                    || (Sets->Actived >= Sets->ObjectSum))
                {
                    ActivedFlag = true;
                }
            }
        }

        if (ActivedFlag == true)
        {
            if(false == Sets->SetsActived)
            {
                Sets->SetsActived = ActivedFlag;
                Int_RestoreAsynSignal();
                Lock_SempPost(&Sets->Lock);
            }
            else
            {
                Sets->SetsActived = ActivedFlag;
                Int_RestoreAsynSignal();
            }
        }
//      else if((ActivedFlag == false) && (true == Sets->SetsActived))
//      {
//          Sets->SetsActived = ActivedFlag;
//          Lock_SempPend(&Sets->Lock,0);
//          Int_RestoreAsynSignal();
//      }
        else
        {
            if(true == Sets->SetsActived)
            {
                Sets->SetsActived = ActivedFlag;
                Lock_SempPend(&Sets->Lock, 0);
                Int_RestoreAsynSignal();
            }
            else
            {
                Sets->SetsActived = ActivedFlag;
                Int_RestoreAsynSignal();
            }
        }

        Object = Object->NextSets;
    }
//  Lock_MutexPost(&MultiplexMutex);
    return true;
}

//----�ȴ�MultiplexSets-----------------------------------------------------------
//����: ���ú�MultiplexSets��Ӧ�ó�����ñ������ȴ�MultiplexSets������������
//      Typeֵ���ȴ���ʽ����ѯ���첽�������֡�
//����: Sets����������MultiplexSetsָ��
//      Status�����ض���ǰ״̬��ָ�룬���Ӧ�ó��򲻹�����״̬�����Ը�NULL��
//      Timeout�������ȴ����ʱ�䣬uS��
//���أ����MultiplexSets���������򷵻�MultiplexSets��һ�������������Fd��
//      �ڹ涨ʱ�����޴�����Ϊ������������ȶ��᷵��-1.
//ע��: ���ñ�������,Sets�Ĵ���״̬��û�иı�,ֻ��Multiplex_Set���޸�Sets�Ĵ���
//      ״̬,Multiplex�����й�����:
//      1.Ӧ�ó������Multiplex_Wait,ȡ��ObjectID����
//      2.����ObjectID������Ӧ�Ķ���.
//      3.�����ʵĶ����ڲ�,������η��ʵ�����Sending Bit �仯,����Multiplex_Set
//          "��֪"Multiplex���.
//      4.Multiplex_Set�����޸�Sets��״̬.
//      5.����1~4��,ֱ��ȫ����Ķ��������,Ȼ����� Multiplex_Wait ���������״̬.
//      ��,������Ա�select�Ķ�������д
//-----------------------------------------------------------------------------
s32 Multiplex_Wait(struct MultiplexSetsCB *Sets, u32 *Status, u32 Timeout)
{
    struct MultiplexObjectCB *Object;
    s32 ret = -1;
    if (Sets == NULL)
    {
        return ret;
    }
//    if ((Sets->ActiveQ == NULL) && (Sets->ObjectQ == NULL))
//        return -1;
//     ����������ע�͵���ԭ�����£�
//     1,�����Ǵ���sets��ʱ�򣬿�����Щ�����Ѿ������˵ȴ���Ȼ����ʱ��û���κεĵȴ��������
//     2,���ȴ������1��Ϊ0ʱ�������ڴ�����ѭ����ʹ��sets wait�ĳ���һ��ʹ��while(1)��
//       �жϻ�ȴ�ֱ��MultiplexSets��������

    //�жϻ�ȴ�ֱ��MultiplexSets��������
    if (!Sets->SetsActived)
    {
        Lock_SempPend(&Sets->Lock, Timeout);
        //���д���ע��ԭ������Ѿ���fd������������û�ﵽActiveLevel���������ҲҪ�ڳ�ʱ
        //֮ʱ����fd
//      if(false ==Lock_SempPend(&Sets->Lock, Timeout))
//      {
//          return ret; //û�д�����Ϊ��ԭ����δ����״̬
//      }
    }
    Int_SaveAsynSignal();
    if (Sets->ActiveQ != NULL)
    {   //�����ȴ�������Actived�ǿռ���Ϊ������Lock_SempPend��ʱ���أ���ʱ
        //ActiveQ����NULL���޶��󱻴�������Ҳ���ܷǿգ��ж��󴥷�����δ�ﵽActiveLevel��
        //��ʱҪ�˹���Ϊ�Ѵ�����
        Sets->SetsActived = true;
        Object = Sets->ActiveQ;
        ret = Object->Fd;
        if(Object->SensingBit & CN_MULTIPLEX_SENSINGBIT_ET)
        {
            //��Object��Sets->ActiveQ�����ó����ŵ�ObjectQ������
            //ȡ���ϴδ���ʱδ�����������α�λΪ�Ѵ�����λ
//            Object->PendingBit &= CN_MULTIPLEX_STATUSMSK;
//            Object->ET_SaveBit = (~Object->ET_SaveBit) & Object->PendingBit;
            if (Status != NULL)
                *Status = Object->PendingBit;
            Object->PendingBit = 0;
            __Multiplex_ChangeList(&(Sets->ActiveQ), &(Sets->ObjectQ), Object);
            if (Sets->Actived != 0)
                Sets->Actived--;
            if (Sets->Actived == 0)
            {
                Sets->SetsActived = false;
                Lock_SempPend(&Sets->Lock, 0);  //������Ѿ�post���ź�
            }
        }
        else
        {
            Sets->SetsActived = true;
            if (Status != NULL)
                *Status = Object->PendingBit;
        }
    }
    Int_RestoreAsynSignal();

    return ret;
}

