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
//����ģ��:�ѹ���ģ��
//����:  ������.
//�汾��V1.0.1
//�ļ�����: �Ӷ�����׼��̬�ڴ����
//����˵��:
//�޶���ʷ:
//4. ����:2014-9-1
//   ����:  ������.
//   �°汾�ţ�2.0.0
//   �޸�˵��: ֧�ֶ���ѣ�ÿ�����ɶ��Ƭ�����
//      1������ɾ���������¼ÿ��ʱ��������ڴ�Ĵ��롣
//      2��������Ҫ��config_prj.h������heap��ص���Ŀ������lds�ṩ��
//      3��������ʾheap������Ϣ�ĺ���
//      4��ʵ�����׺�����һ�״�Ĭ�϶��з��䣬һ�״�ָ�����з��䡣
//      5������ʱ�Ȳ�������ʵ�Ƭ��
//4. ����:2013-1-18
//   ����:  ������.
//   �°汾�ţ�1.0.3
//   �޸�˵��: ������m_free������һ�����󣬸�bug������"��ë��"�ύ
//3. ����:2009-10-31
//   ����:  ������.
//   �°汾�ţ�1.0.2
//   �޸�˵��: ������__m_check_memory��__m_wait_memory������һ�����󣬸�bug��
//   ����"�ٻ���"�ύ
//2. ����:2009-03-03
//   ����:  ������.
//   �°汾�ţ�1.0.1
//   �޸�˵��: ������__m_check_memory������һ�����󣬸�bug������sniper�ύ
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "board-config.h"
#include "stddef.h"
#include "stdlib.h"
#include "string.h"
#include "list.h"
#include "align.h"
#include "int.h"
#include "lock.h"
#include "systime.h"
#include "heap-in.h"
#include "heap.h"
#include "component_config_heap.h"
extern void *pHeapList;             //�ڽű��ж���
struct HeapCB *tg_pHeapList=NULL;   //����ָ�룬ϵͳ�����еĶѱ�������һ��
struct HeapCB *tg_pSysHeap=NULL;    //����ָ�룬ϵͳ�����еĶѱ�������һ��
ptu32_t g_uFreeMin = 0;             //����heap�����������ڴ����Сֵ

void *__Heap_StaticMallocHeap(ptu32_t size,struct HeapCB *Heap,u32 Timeout);
void *__Heap_StaticMalloc(ptu32_t size,u32 timeout);
void __Heap_StaticFreeHeap(void *pl_mem,struct HeapCB *Heap);
void __Heap_StaticFree(void *pl_mem);
void *__Heap_StaticRealloc(void *, ptu32_t NewSize);
void *__Heap_StaticMallocStack(struct EventECB *event, u32 size);

void Heap_StaticModuleInit(void);

ptu32_t __Heap_StaticFormatSizeHeap(ptu32_t size,struct HeapCB *Heap);
ptu32_t __Heap_StaticFormatSize(ptu32_t size);
ptu32_t __Heap_StaticGetMaxFreeBlockHeap(struct HeapCB *Heap);
ptu32_t __Heap_StaticGetMaxFreeBlock(void);
ptu32_t Heap_GetHeapSizeHeap(struct HeapCB *Heap);
ptu32_t Heap_GetHeapSize(void);
struct HeapCB *Heap_FindHeap(const char *HeapName);       //��heap-dynamic.c��
ptu32_t __Heap_StaticGetFreeMem(void);
ptu32_t __Heap_StaticGetFreeMemHeap(struct HeapCB *Heap);
ptu32_t __Heap_StaticCheckSize(void * mp);

void *  (*M_Malloc)(ptu32_t size,u32 timeout);
void *  (*M_Realloc) (void *, ptu32_t NewSize);
void    (*M_Free)(void * pl_mem);
void *  (*M_MallocHeap)(ptu32_t size,struct HeapCB *Heap,u32 timeout);
void *  (*M_MallocLc)(ptu32_t size,u32 timeout);
void *  (*M_MallocLcHeap)(ptu32_t size,struct HeapCB *Heap, u32 timeout);
void    (*M_FreeHeap)(void * pl_mem,struct HeapCB *Heap);
void *  (*__MallocStack)(struct EventECB *pl_ecb,u32 size);
ptu32_t (*M_FormatSizeHeap)(ptu32_t size,struct HeapCB *Heap);
ptu32_t (*M_FormatSize)(ptu32_t size);
ptu32_t (*M_GetMaxFreeBlockHeap)(struct HeapCB *Heap);
ptu32_t (*M_GetMaxFreeBlock)(void);
ptu32_t (*M_GetFreeMem)(void);
ptu32_t (*M_GetFreeMemHeap)(struct HeapCB *Heap);
ptu32_t (*M_CheckSize)(void * mp);

//u32 __Heap_MemStrLen(char *addr)
//{
//    u32 result = 0;
//    while(*addr != '\0')
//    {
//        result ++;
//        addr++;
//    }
//    return result;
//}

//-----------------------------------------------------------------------------
//���ܣ�ɨ�����еĶѣ������ѹ���ṹ����δ��ʼ�����������ݽṹ�����з������׼��̬��ʽ
//      ���У�ֱ������ Heap_DynamicModuleInit ������
//      ɨ��ʱ��Ϊÿ���Ѵ���һ���ѿ��ƽṹ���ýṹ��struct HeapCB�ṹ�Լ� n ��struct
//      HeapCession �ṹ��ɣ�n�Ǹöѵ�cession�����ѿ��ƽṹ���ڸöѵ�һ��size����Ҫ��
//      ��cession�ĸߵ�ַ��
//��������
//���أ���
//-----------------------------------------------------------------------------
void __Heap_MemHeapScan(void)
{
    struct HeapCB *HeapTemp;
    struct HeapCession *Cession;
    u32 CessionNum,Ctrlsize,NameLen,n;
    u8  *Offset;
    u32 *u32Offset;
//  u32 HeapNo=0;
    u32 AlignSize;
    u32 Property;
    ptu32_t HeapSizeSum;
    Offset = (u8*)&pHeapList;
    CessionNum = *(u32*)Offset;
    while(CessionNum != 0)     //ɨ�����жѣ������ѿ��ƿ�ṹ
    {
        u32Offset = (u32*)Offset + 1;
        AlignSize = *u32Offset++;
        Property = *u32Offset++;
        //���¼��㹫ʽ���ο�lds�ļ���ʽ
        Offset += sizeof(u32)*3*CessionNum + 3*sizeof(u32);
//      NameLen = __Heap_MemStrLen((char*)Offset);
        NameLen = strlen((char*)Offset);
        //����heap���ƿ��session���ƿ�����Ҫ���ڴ�ߴ硣
//      Ctrlsize = NameLen + 1 + sizeof( struct HeapCB )
//                  + CessionNum * sizeof(struct HeapCession);
        Ctrlsize = sizeof( struct HeapCB )+ CessionNum * sizeof(struct HeapCession);
        Ctrlsize = align_up_sys(Ctrlsize);
        Cession = NULL;
        //����forѭ������:
        //1��ɨ������session���ҵ���һ������Ctrlsize�����session��֮ǰ��session������
        //2���ڸ�session�����������ڿ��ƽṹ���ڴ档
        //3����׼��̬�����Ҫ���ʼ��heap���ƿ��session���ƿ顣
        //4�������е�session���ӳ�����
        for(n = 0; n < CessionNum; n++)
        {
            if(Cession != NULL) //�ҵ�����Ҫ���cession�������ű���ֵ
            {
                Cession->static_bottom = (void*)(*u32Offset);
                Cession->heap_bottom = (void*)(*u32Offset);
                Cession->heap_top = (void*)(*(u32Offset+1));
                Cession->last = (list_t *)(*u32Offset);
                HeapSizeSum += Cession->heap_top - Cession->heap_bottom;
                dListInit(Cession->last);
#if ((CFG_DYNAMIC_MEM == true))
                Cession->PageSize = *(u32Offset+2);
#endif
                if(n == CessionNum -1)
                    Cession->Next = NULL;
                else
                    Cession->Next = Cession+1;
                Cession++;
            }
            //�ҵ��˵�һ������Ҫ���session���ڴ�֮ǰ��session��sizeС��Ctrlsize������֮��
            if((Cession == NULL) && ((*(u32Offset+1) - *u32Offset) >=Ctrlsize))
            {
                HeapTemp = (struct HeapCB *)(*(u32Offset+1) - Ctrlsize);
                HeapTemp = (struct HeapCB *)align_down_sys(HeapTemp);
                if(tg_pHeapList == NULL)
                {
                    tg_pHeapList = HeapTemp;
                    tg_pHeapList->NextHeap = HeapTemp;
                    tg_pHeapList->PreHeap = HeapTemp;
                    //��һ���ѿ���ϵͳ�ѡ�
//                  if(HeapNo == 0)
                        tg_pSysHeap = tg_pHeapList;
                }
                else
                {
                    HeapTemp->NextHeap = tg_pHeapList;
                    HeapTemp->PreHeap = tg_pHeapList->PreHeap;
                    tg_pHeapList->PreHeap->NextHeap = HeapTemp;
                    tg_pHeapList->PreHeap = HeapTemp;
                }
                Cession = (struct HeapCession*)(HeapTemp + 1);
                HeapTemp->Cession = Cession;
                HeapTemp->CessionNum = CessionNum - n;
                if(AlignSize == 0)
                    HeapTemp->AlignSize = CN_ALIGN_SIZE;
                else
                    HeapTemp->AlignSize = AlignSize;
                HeapTemp->HeapProperty = Property;
                HeapTemp->HeapName = (char *)Offset;
#if ((CFG_DYNAMIC_MEM == true))
                HeapTemp->mem_sync = NULL;
#endif
                Cession->static_bottom = (void*)(*u32Offset);
                Cession->heap_bottom = (void*)(*u32Offset);
                Cession->heap_top = (void*)HeapTemp;
                Cession->last = (list_t *)(*u32Offset);
                HeapSizeSum = Cession->heap_top - Cession->heap_bottom;
                dListInit(Cession->last);
#if ((CFG_DYNAMIC_MEM == true))
                Cession->PageSize = *(u32Offset+2);
#endif
                if(n == CessionNum -1)
                    Cession->Next = NULL;
                else
                    Cession->Next = Cession+1;
                Cession++;
            }
//          Ctrlsize -= sizeof(struct HeapCession);
            u32Offset += 3;
        }
        HeapTemp->FreeMin = HeapSizeSum;
        g_uFreeMin += HeapSizeSum;
        Offset += NameLen + 1;      //+1�����������
        Offset = (u8*)align_up(sizeof(u32),Offset);
        CessionNum = *(u32*)Offset;
//      HeapNo++;
    }
}
//----�ڴ�ṹ��ʼ��-----------------------------------------------------------
//����: ɨ��ȫ��heap������Heap���ƿ��Cession���ƿ�������ڴ棬����ʼ����
//����: ��
//����: ����1.
//��ע: 1.�������ڳ�ʼ�����֮ǰ����,�ж���δ����,���迼�ǹر��жϵ�����.
//      2.������������ȵ��ñ�����,������̬���书��,�˺󻹲��߱���̬���书��.
//      3.�����ù���ȷ���Ѳ����������ﲻ����顣
//-----------------------------------------------------------------------------
void Heap_StaticModuleInit(void)
{
    __Heap_MemHeapScan();

    M_Malloc = __Heap_StaticMalloc;
    M_Realloc = __Heap_StaticRealloc;
    M_Free = __Heap_StaticFree;
    M_MallocHeap = __Heap_StaticMallocHeap;
    M_MallocLc = __Heap_StaticMalloc;
    M_MallocLcHeap = __Heap_StaticMallocHeap;
    M_FreeHeap = __Heap_StaticFreeHeap;
    __MallocStack = __Heap_StaticMallocStack;
    M_FormatSizeHeap = __Heap_StaticFormatSizeHeap;
    M_FormatSize = __Heap_StaticFormatSize;
    M_GetMaxFreeBlockHeap = __Heap_StaticGetMaxFreeBlockHeap;
    M_GetMaxFreeBlock = __Heap_StaticGetMaxFreeBlock;
    M_GetFreeMem = __Heap_StaticGetFreeMem;
    M_GetFreeMemHeap = __Heap_StaticGetFreeMemHeap;
    M_CheckSize = __Heap_StaticCheckSize;
}

//----׼��̬�ڴ����-----------------------------------------------------------
//���ܣ�ִ��׼��̬����ķ�������ָ���Ķ��ڷ���һ���ڴ棬�÷���ʵ������ģ�����
//      ��������Ϊ���ټ��϶��롣
//������size,��������ڴ��ߴ�
//      Heap,ָ���Ķ�
//���أ�������ڴ�ָ�룬NULL��ʾû���ڴ���Է���
//��ע: 1.׼��̬�����뾲̬�ڴ��������,���ַ��䷽��ʹ��ԭ�Ӳ�����ȷ������һ�£�
//      ������������,�ڴ治����ֱ�ӷ��ء�
//      ��ִ��module_init_heap_dynamic֮ǰ,���е��ڴ���������׼��̬����
//------------
//���¼�¼:
// 2.����: 2015/4/25
//   ˵��: �����ж�ͨ�öѻ���ר�ö�
//   ����: ������
// 1.����: 2015/3/16
//   ˵��: ���ӶԲ���size����ֵ�ж�
//   ����: ������
//-----------------------------------------------------------------------------
void *__Heap_StaticMallocHeapIn(ptu32_t size,struct HeapCB *Heap)
{
    void *temp,*result = NULL;
    list_t *pStaticList;
    struct HeapCession *Cession;
    atom_low_t atom_m;

    if((Heap == NULL) || (0 == size))
        return NULL;

    Cession = Heap->Cession;
    atom_m = Int_LowAtomStart();
    while(Cession != NULL)
    {
        temp = Cession->heap_bottom;   //���浱ǰ�ѵ�
        //���±����ߴ�Ŀռ�����
        Cession->heap_bottom = (u8 *)align_up(Heap->AlignSize,
                            Cession->heap_bottom + sizeof(list_t));
        if(((ptu32_t)Cession->heap_top > (ptu32_t)Cession->heap_bottom)&&\
           ((ptu32_t)(Cession->heap_top-Cession->heap_bottom) >= size))
        {
            //�·�����ڴ�ߴ籣���ڵ�ǰ�ѵ�ǰ��
            pStaticList = (list_t *)Cession->heap_bottom;
            pStaticList -= 1;
            dListInsertAfter(Cession->last,pStaticList);
            Cession->last = pStaticList;
            result = Cession->heap_bottom;
            Cession->heap_bottom += align_up(Heap->AlignSize,size);
            break;
        }
        else
        {
            Cession->heap_bottom = temp;   //�ָ���ǰ�ѵ�
        }
        Cession = Cession->Next;
    }
    Int_LowAtomEnd(atom_m);
    return(result);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

//----׼��̬�ڴ����-----------------------------------------------------------
//���ܣ�ִ��׼��̬����ķ�������ָ���Ķ��ڷ���һ���ڴ棬�÷���ʵ������ģ�����
//      ��������Ϊ���ټ��϶��롣���ָ���Ķ���ͨ�ö�,�����ȴӸöѷ���,����ʧ��
//      ���������ͨ�����ԵĶ��з���;���ָ���Ķ���ר�ö�,��ֻ�Ӹöѷ���.
//������size,��������ڴ��ߴ�
//      Heap,ָ���Ķ�
//���أ�������ڴ�ָ�룬NULL��ʾû���ڴ���Է���
//��ע: 1.׼��̬�����뾲̬�ڴ��������,���ַ��䷽��ʹ��ԭ�Ӳ�����ȷ������һ�£�
//      ������������,�ڴ治����ֱ�ӷ��ء�
//      ��ִ��module_init_heap_dynamic֮ǰ,���е��ڴ���������׼��̬����
//------------
//���¼�¼:
// 1.����: 2015/4/25
//   ˵��: �����ж�ͨ�öѻ���ר�ö�
//   ����: ������
//-----------------------------------------------------------------------------
void *__Heap_StaticMallocHeap(ptu32_t size,struct HeapCB *Heap,u32 Timeout)
{
    struct HeapCB *Work;
    void *result = NULL;

    if((Heap == NULL) || (0 == size))
        return NULL;
    if((Heap->HeapProperty & CN_HEAP_PRIVATE) == CN_HEAP_GENERAL)
    {
        Work = Heap;
        do
        {
            if((Work->HeapProperty & CN_HEAP_PRIVATE) == CN_HEAP_GENERAL )
            {
                result = __Heap_StaticMallocHeapIn(size,Work);
                if(result != NULL)
                    break;
            }
            Work = Work->NextHeap;
        }while(Work != Heap);

    }
    else
    {
        result = __Heap_StaticMallocHeapIn(size,Heap);
    }
    return(result);
}
//----�����߳�ջ---------------------------------------------------------------
//���ܣ�׼��̬������, ��malloc��һ�µ�.
//������size,��������ڴ��ߴ�
//���أ�������ڴ�ָ�룬NULL��ʾû���ڴ���Է���
//-----------------------------------------------------------------------------
void *__Heap_StaticMallocStack(struct EventECB *event, u32 size)
{
    struct HeapCB *Work;
    void *result = NULL;
    Work = tg_pHeapList;
    do
    {
        if((Work->HeapProperty & CN_HEAP_PRIVATE) == CN_HEAP_GENERAL )
        {
            result = __Heap_StaticMallocHeapIn(size,Work);
            if(result != NULL)
                break;
        }
        Work = Work->NextHeap;
    }while(Work != tg_pHeapList);

    return result;
}

void *__Heap_StaticMalloc(ptu32_t size,u32 timeout)
{
    return __Heap_StaticMallocHeap(size,tg_pSysHeap,0);
}

#pragma GCC diagnostic pop

//----���·���һ���ڴ�---------------------------------------------------------
//����: ���µĳߴ�,��pָ�����·���һ���ڴ�.
//      �������,���ԭ������ȫ��copy���·�����ڴ���.
//      �����С,���³ߴ�copyԭ������,���������ݽ�����.
//      ���p=NULL,���൱��__M_Malloc
//      ���NewSize = 0,���൱�� __Heap_Free
//      ������ڴ����ʧ��,ԭ�����ڴ�Ҳ�����ͷ�,����NULL
//      ������������ڴ�,��ʹ��free�����ͷ�.
//����: p, ���޸ĳߴ��ָ��
//      NewSize,�µ��ڴ�ߴ�
//����: �³ߴ���ڴ�ָ��,ע�������ԭָ��.
//------------
//���¼�¼:
// 1.����: 2015/3/16
//   ˵��: ���ȶԲ���NewSize���ж��߼�
//   ����: ������
//-----------------------------------------------------------------------------
void *__Heap_StaticRealloc(void *p, ptu32_t NewSize)
{
    ptu32_t OldSize;
    void *NewP = NULL;

    if(NewSize == 0)
    {
        __Heap_StaticFree(p);
        return NULL;
    }

    if(p == NULL)
    {
        //��NewSize = 0,����NULL
        NewP = __Heap_StaticMalloc(NewSize,0);
        return NewP;
    }

    OldSize = __Heap_StaticCheckSize(p);
    if((NewSize > (OldSize - tg_pSysHeap->AlignSize)) && (NewSize <= OldSize))
        return p;
    else
    {
        NewP = __Heap_StaticMalloc(NewSize,0);
        if(NewP)
        {
            memcpy(NewP,p,NewSize < OldSize ? NewSize:OldSize);
        }
        __Heap_StaticFree(p);
    }
    return NewP;
}

//----�ͷ�׼��̬������ڴ��---------------------------------------------------
//���ܣ��ͷ�׼��̬�����һ���ڴ�飬��heap��cession�в��ң�������ͷŵ��ڴ��ǴӸ�
//      session��������һ���ڴ棬�ͷ�֮��
//������pl_mem�����ͷŵ��ڴ��ָ��
//      Heap��ָ������������ͷš�
//���أ�true = �ɹ��ͷţ�false =  �ͷ�ʧ��
//��ע: 1.׼��̬�����뾲̬�ڴ��������,û�б�����ʩ,��ȷ��Ҫ����Ա�Լ���֤.����
//      ���䷽��Ҳ������������,��ִ��module_init_heap_dynamic֮ǰ,���е��ڴ����
//      ������׼��̬����
//      2.�������ڳ�ʼ�����֮ǰ����,�ж���δ����,���迼�ǹر��жϵ�����.
//-----------------------------------------------------------------------------
void __Heap_StaticFreeHeap(void * pl_mem,struct HeapCB *Heap)
{
    atom_low_t atom_m;
    struct HeapCession *Cession;
    list_t *pStaticList;

    if(pl_mem == NULL)
        return ;
    if(pl_mem != (void *)align_up(Heap->AlignSize,pl_mem))//�����϶���Ҫ���ָ��
        return ;
    Cession = Heap->Cession;
    atom_m = Int_LowAtomStart();
    while(Cession != NULL)
    {
        if((pl_mem >=(void*)Cession->static_bottom) && (pl_mem <= (void*)Cession->heap_bottom))
        {
            pStaticList = (list_t*)pl_mem;
            pStaticList--;
            if(pStaticList == Cession->last)        //�������һ�飬ֱ�ӻ��ա�
            {
//                Cession->last = pStaticList->prev;
                Cession->last = dListGetBefore(pStaticList);
                Cession->heap_bottom = (u8*)pStaticList;
            }
            dListRemove(pStaticList);               //�����Ƿ����һ�飬�����սڵ�
            break;
        }
//      psize = (ptu32_t *)pl_mem;
//      psize--;
//      if( (*psize + (u8*)pl_mem) == Cession->heap_bottom)
//      {
//          //pl_mem�ǴӸ�cession��������ڴ棬�����ͷ�
//          Cession->heap_bottom = (void*)align_down(Heap->AlignSize,psize);
//          break;
//      }
        Cession = Cession->Next;
    }
    Int_LowAtomEnd(atom_m);

    return ;
}
void __Heap_StaticFree(void * pl_mem)
{
    __Heap_StaticFreeHeap(pl_mem,tg_pSysHeap);
}

//----��ѯ�ѿռ��С---------------------------------------------------------
//���ܣ����ضѿռ��С,�˺������־�̬��׼��̬,��һ��.
//��������
//���أ��ڴ���ֽ���
//-----------------------------------------------------------------------------
ptu32_t  Heap_GetHeapSizeHeap(struct HeapCB *Heap)
{
    struct HeapCession *Cession;
    ptu32_t result=0;
    Cession = Heap->Cession;
    while(Cession != NULL)
    {
        result += Cession->heap_top - Cession->static_bottom;
        Cession = Cession->Next;
    }
    return result;
}

ptu32_t  Heap_GetHeapSize(void)
{
    if(tg_pSysHeap != NULL)
        return Heap_GetHeapSizeHeap(tg_pSysHeap);
    else
        return 0;
}

//----��ȡ��ʽ���ߴ�-----------------------------------------------------------
//����: ��ȻӦ�ó���������������ߴ��ڴ棬���ڴ������������û����ڴ�ȴ������
//      �û�����Ĺ�񻯳ߴ���ڴ�飬�����������㣬������������size�ֽ��ڴ棬
//      ϵͳ�����������ڴ���ʵ�ʳߴ磬��������ִ�з����ڴ�Ĳ�����Ҳ����֤��
//      ��ķ�������ܷ�ɹ���
//����: size��Ӧ�ó���������ĳߴ�
//����: ��񻯳ߴ�
//-----------------------------------------------------------------------------
ptu32_t __Heap_StaticFormatSizeHeap(ptu32_t size,struct HeapCB *Heap)
{
    return align_up(Heap->AlignSize,size);  //׼��̬���䣬���뼴��
}
ptu32_t __Heap_StaticFormatSize(ptu32_t size)
{
    return align_up_sys(size);  //׼��̬���䣬���뼴��
}

ptu32_t __Heap_StaticGetMaxFreeBlockHeapIn(struct HeapCB *Heap)
{
    struct HeapCession *Cession;
    ptu32_t result = 0;
    Cession = Heap->Cession;
    while(Cession != NULL)
    {
        if(result < (ptu32_t)(Cession->heap_top - Cession->heap_bottom))
            result = Cession->heap_top - Cession->heap_bottom;
        Cession = Cession->Next;
    }
    return result;
}
//----��ѯ�������ڴ��-------------------------------------------------------
//���ܣ�����ָ���ѵ��������ڴ��ĳߴ�
//������Heap,����ѯ�Ķ�,������Ǹ�ר�ö�,��ֻ��ѯ�����,�����ѯ����ͨ�ö�.
//���أ��ڴ���ֽ���
//-----------------------------------------------------------------------------
ptu32_t  __Heap_StaticGetMaxFreeBlockHeap(struct HeapCB *Heap)
{
    struct HeapCB *Work;
    ptu32_t result = 0,temp;
    if(Heap== NULL)
        return 0;
    if((Heap->HeapProperty & CN_HEAP_PRIVATE) == CN_HEAP_PRIVATE)
    {
        result = __Heap_StaticGetMaxFreeBlockHeapIn(Heap);
    }
    else
    {
        Work = Heap;
        do
        {
            if((Work->HeapProperty & CN_HEAP_PRIVATE) == CN_HEAP_GENERAL )
            {
                temp = __Heap_StaticGetMaxFreeBlockHeapIn(Work);
                if(result < temp)
                    result = temp;
            }
            Work = Work->NextHeap;
        }while(Work != Heap);
    }

    return result;
}

//----��ѯ�������ڴ��-------------------------------------------------------
//���ܣ�������ͨ�ö��м���������ڴ��ĳߴ�
//��������
//���أ��ڴ���ֽ���
//-----------------------------------------------------------------------------
ptu32_t  __Heap_StaticGetMaxFreeBlock(void)
{
    struct HeapCB *Work;
    ptu32_t result = 0,temp;
    Work = tg_pHeapList;
    do
    {
        if((Work->HeapProperty & CN_HEAP_PRIVATE) == CN_HEAP_GENERAL )
        {
            temp = __Heap_StaticGetMaxFreeBlockHeapIn(Work);
            if(result < temp)
                result = temp;
        }
        Work = Work->NextHeap;
    }while(Work != tg_pHeapList);

    return result;
}

//----��ѯ�����ڴ��С-------------------------------------------------------
//���ܣ�������ָ���ѵĿ����ڴ��ֽ���
//������Heap,����ѯ�Ķ�
//���أ������ڴ��ֽ���
//-----------------------------------------------------------------------------
ptu32_t  __Heap_StaticGetFreeMemHeapIn(struct HeapCB *Heap)
{
    struct HeapCession *Cession;
    ptu32_t result=0;
    Cession = Heap->Cession;
    while(Cession != NULL)
    {
        result += Cession->heap_top - Cession->heap_bottom;
        Cession = Cession->Next;
    }
    return result;
}

//----��ѯ�����ڴ��С-------------------------------------------------------
//���ܣ�������ָ���ѵĿ����ڴ��ֽ���
//������Heap,����ѯ�Ķ�,������Ǹ�ר�ö�,��ֻ��ѯ�����,�����ѯ����ͨ�ö�.
//���أ������ڴ��ֽ���
//-----------------------------------------------------------------------------
ptu32_t  __Heap_StaticGetFreeMemHeap(struct HeapCB *Heap)
{
    struct HeapCB *Work;
    ptu32_t result = 0;
    if(Heap== NULL)
        return 0;
    if((Heap->HeapProperty & CN_HEAP_PRIVATE) == CN_HEAP_PRIVATE)
    {
        result = __Heap_StaticGetFreeMemHeapIn(Heap);
    }
    {
        Work = Heap;
        do
        {
            if((Work->HeapProperty & CN_HEAP_PRIVATE) == CN_HEAP_GENERAL )
            {
                result += __Heap_StaticGetFreeMemHeapIn(Work);
            }
            Work = Work->NextHeap;
        }while(Work != Heap);

    }
    return result;
}
//----��ѯ�ܿ����ڴ��С-------------------------------------------------------
//���ܣ���������ͨ�öѵĿ����ڴ��ֽ���֮��
//��������
//���أ������ڴ��ֽ���
//-----------------------------------------------------------------------------
ptu32_t  __Heap_StaticGetFreeMem(void)
{
    struct HeapCB *Work;
    ptu32_t result = 0;
    Work = tg_pHeapList;
    do
    {
        if((Work->HeapProperty & CN_HEAP_PRIVATE) == CN_HEAP_GENERAL )
        {
            result += __Heap_StaticGetFreeMemHeap(Work);
        }
        Work = Work->NextHeap;
    }while(Work != tg_pHeapList);

    return result;
}

//----��ѯ�ڴ�ߴ�-------------------------------------------------------------
//����: ���ݸ�����ָ��,��ѯ��ָ�����ڵ��ڴ��ĳߴ�.
//����: mp,��̬������ڴ�ָ��.
//����: �ڴ��ߴ�,����0�м��ֺ���:1.�Ƿ�ָ��,2.mp����׼��̬�����ָ��.
//-----------------------------------------------------------------------------
ptu32_t __Heap_StaticCheckSize(void * pl_mem)
{
    atom_low_t atom_m;
    struct HeapCB *Work;
    struct HeapCession *Cession;
    list_t *NextMem,*LocMem;
    ptu32_t result = 0;
    if(pl_mem == NULL)
        return 0;
    if(pl_mem != (void *)align_up(tg_pHeapList->AlignSize,pl_mem))//�����϶���Ҫ���ָ��
        return 0;

    Work = tg_pHeapList;
    atom_m = Int_LowAtomStart();
    do
    {
        Cession = Work->Cession;
        while(Cession != NULL)
        {
            if((pl_mem >=(void*)Cession->static_bottom) && (pl_mem <= (void*)Cession->heap_bottom))
            {
                LocMem = (list_t*)pl_mem;
                LocMem--;
                NextMem = LocMem->next;
                if(NextMem > LocMem)
                    result = (ptu32_t)NextMem - (ptu32_t)LocMem;
                else
                {
                    result = (ptu32_t)Cession->heap_bottom - (ptu32_t)LocMem;
                }
                break;
            }
            Cession = Cession->Next;
        }
        if(result != 0)
            break;
        Work = Work->NextHeap;
    }while(Work != tg_pHeapList);

    if(result != 0)
        result -= sizeof(list_t);
    Int_LowAtomEnd(atom_m);
    return result;
}
