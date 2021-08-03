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
//�ļ�����:�ṩ�������ڴ�������
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
#include "stdlib.h"
#include "string.h"
#include "list.h"
#include "align.h"
#include "int.h"
#include "heap-in.h"
#include "heap.h"
#include "lock.h"
#include "djyos.h"
#include "stdio.h"
#include "dbug.h"
#include "systime.h"
#include <shell.h>

#include "component_config_heap.h"
//���ó�������
//��nλΪ1�������λΪ0�ĳ���
#define CN_HIGH_1BIT_1      (0x80000000)
#define CN_HIGH_2BIT_1      (0xc0000000)
#define CN_HIGH_3BIT_1      (0xe0000000)
#define CN_HIGH_4BIT_1      (0xf0000000)
#define CN_HIGH_5BIT_1      (0xf8000000)
#define CN_HIGH_6BIT_1      (0xfc000000)
#define CN_HIGH_7BIT_1      (0xfe000000)
#define CN_HIGH_8BIT_1      (0xff000000)
#define CN_HIGH_9BIT_1      (0xff800000)
#define CN_HIGH_10BIT_1     (0xffc00000)
#define CN_HIGH_11BIT_1     (0xffe00000)
#define CN_HIGH_12BIT_1     (0xfff00000)
#define CN_HIGH_13BIT_1     (0xfff80000)
#define CN_HIGH_14BIT_1     (0xfffc0000)
#define CN_HIGH_15BIT_1     (0xfffe0000)
#define CN_HIGH_16BIT_1     (0xffff0000)
#define CN_HIGH_17BIT_1     (0xffff8000)
#define CN_HIGH_18BIT_1     (0xffffc000)
#define CN_HIGH_19BIT_1     (0xffffe000)
#define CN_HIGH_20BIT_1     (0xfffff000)
#define CN_HIGH_21BIT_1     (0xfffff800)
#define CN_HIGH_22BIT_1     (0xfffffc00)
#define CN_HIGH_23BIT_1     (0xfffffe00)
#define CN_HIGH_24BIT_1     (0xffffff00)
#define CN_HIGH_25BIT_1     (0xffffff80)
#define CN_HIGH_26BIT_1     (0xffffffc0)
#define CN_HIGH_27BIT_1     (0xffffffe0)
#define CN_HIGH_28BIT_1     (0xfffffff0)
#define CN_HIGH_29BIT_1     (0xfffffff8)
#define CN_HIGH_30BIT_1     (0xfffffffc)
#define CN_HIGH_31BIT_1     (0xfffffffe)
#define CN_ALLBIT_1         (0xffffffff)

//��nλΪ1������λΪ0�ĳ���
#define CN_LOW_1BIT_1      (0x00000001)
#define CN_LOW_2BIT_1      (0x00000003)
#define CN_LOW_3BIT_1      (0x00000007)
#define CN_LOW_4BIT_1      (0x0000000f)
#define CN_LOW_5BIT_1      (0x0000001f)
#define CN_LOW_6BIT_1      (0x0000003f)
#define CN_LOW_7BIT_1      (0x0000007f)
#define CN_LOW_8BIT_1      (0x000000ff)
#define CN_LOW_9BIT_1      (0x000001ff)
#define CN_LOW_10BIT_1     (0x000003ff)
#define CN_LOW_11BIT_1     (0x000007ff)
#define CN_LOW_12BIT_1     (0x00000fff)
#define CN_LOW_13BIT_1     (0x00001fff)
#define CN_LOW_14BIT_1     (0x00003fff)
#define CN_LOW_15BIT_1     (0x00007fff)
#define CN_LOW_16BIT_1     (0x0000ffff)
#define CN_LOW_17BIT_1     (0x0001ffff)
#define CN_LOW_18BIT_1     (0x0003ffff)
#define CN_LOW_19BIT_1     (0x0007ffff)
#define CN_LOW_20BIT_1     (0x000fffff)
#define CN_LOW_21BIT_1     (0x001fffff)
#define CN_LOW_22BIT_1     (0x003fffff)
#define CN_LOW_23BIT_1     (0x007fffff)
#define CN_LOW_24BIT_1     (0x00ffffff)
#define CN_LOW_25BIT_1     (0x01ffffff)
#define CN_LOW_26BIT_1     (0x03ffffff)
#define CN_LOW_27BIT_1     (0x07ffffff)
#define CN_LOW_28BIT_1     (0x0fffffff)
#define CN_LOW_29BIT_1     (0x1fffffff)
#define CN_LOW_30BIT_1     (0x3fffffff)
#define CN_LOW_31BIT_1     (0x7fffffff)

//�ֽ�����ǰ��0������,256�ֽ�,���ڿ��ٲ�Ѱ�׸���0λ��λ��
u8 const cn_leading_zero[]=
{
    8, 7, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

const ucpu_t cn_low_xbit_msk_exp2 []=
{
    CN_LOW_1BIT_1,
    CN_LOW_2BIT_1,
    CN_LOW_4BIT_1,
    CN_LOW_8BIT_1,
    CN_LOW_16BIT_1,
    CN_ALLBIT_1,
};

const ucpu_t cn_low_xbit_msk []=
{
    0,
    CN_LOW_1BIT_1,
    CN_LOW_2BIT_1,
    CN_LOW_3BIT_1,
    CN_LOW_4BIT_1,
    CN_LOW_5BIT_1,
    CN_LOW_6BIT_1,
    CN_LOW_7BIT_1,
    CN_LOW_8BIT_1,
    CN_LOW_9BIT_1,
    CN_LOW_10BIT_1,
    CN_LOW_11BIT_1,
    CN_LOW_12BIT_1,
    CN_LOW_13BIT_1,
    CN_LOW_14BIT_1,
    CN_LOW_15BIT_1,
    CN_LOW_16BIT_1,
    CN_LOW_17BIT_1,
    CN_LOW_18BIT_1,
    CN_LOW_19BIT_1,
    CN_LOW_20BIT_1,
    CN_LOW_21BIT_1,
    CN_LOW_22BIT_1,
    CN_LOW_23BIT_1,
    CN_LOW_24BIT_1,
    CN_LOW_25BIT_1,
    CN_LOW_26BIT_1,
    CN_LOW_27BIT_1,
    CN_LOW_28BIT_1,
    CN_LOW_29BIT_1,
    CN_LOW_30BIT_1,
    CN_LOW_31BIT_1,
    CN_ALLBIT_1,
};

struct EventECB;
//ͨ�öѵ�ͬ��ָ�룬ר�ö�ʹ�öԿ��ƿ� struct HeapCB �� mem_sync ��Ա
static struct EventECB *s_ptGenMemSync;
//����ջ��ͬ��ָ�룬����߳��ڷ���ջʱ�������������������뱾���С�
static struct EventECB *s_ptStackSync;

//0~16383����event ID�������ID�¼�����ĵ�ҳ�ֲ��ڴ档
#define CN_MEM_DOUBLE_PAGE_LOCAL    0xffff
#define CN_MEM_MANY_PAGE_LOCAL      0xfffe
//16384~32767��ȥ�����λ����event ID�������ID�¼�����ĵ�ҳȫ���ڴ档
//#define CN_MEM_SINGLE_PAGE_GLOBAL   0xfffd
#define CN_MEM_DOUBLE_PAGE_GLOBAL   0xfffd
#define CN_MEM_MANY_PAGE_GLOBAL     0xfffc
#define CN_MEM_FREE_PAGE            0xfffb

extern void *pHeapList;             //�ڽű��ж���
extern struct HeapCB *tg_pHeapList;   //����ָ�룬ϵͳ�����еĶѱ�������һ��
extern struct HeapCB *tg_pSysHeap;   //����ָ�룬ϵͳ�����еĶѱ�������һ��

void *__Heap_Malloc(ptu32_t size,u32 timeout);
void  __Heap_Free(void * pl_mem);
void *__Heap_Realloc(void *p, ptu32_t NewSize);
void *__Heap_MallocHeap(ptu32_t size,struct HeapCB *Heap,u32 timeout);
void *__Heap_MallocLc(ptu32_t size,u32 timeout);
void *__Heap_MallocLcHeap(ptu32_t size,struct HeapCB *Heap, u32 timeout);
void  __Heap_FreeHeap(void * pl_mem,struct HeapCB *Heap);
void *__Heap_MallocStack(struct EventECB *event, u32 size);

//void __M_ShowHeap(void);
//void __M_ShowHeapSpy(void);

ptu32_t __Heap_FormatSizeHeap(ptu32_t size,struct HeapCB *Heap);
ptu32_t __Heap_FormatSize(ptu32_t size);
ptu32_t __Heap_GetMaxFreeBlockHeap(struct HeapCB *Heap);
ptu32_t __Heap_GetMaxFreeBlock(void);
ptu32_t Heap_GetHeapSizeHeap(struct HeapCB *Heap);  //��heap-static.c��
ptu32_t Heap_GetHeapSize(void);  //��heap-static.c��
struct HeapCB *Heap_FindHeap(const char *HeapName);
ptu32_t __Heap_GetFreeMem(void);
ptu32_t __Heap_GetFreeMemHeap(struct HeapCB *Heap);
ptu32_t __Heap_CheckSize(void * mp);
void __Heap_CheckSTackSync(void);

extern void *  (*M_Malloc)(ptu32_t size,u32 timeout);
extern void *  (*M_Realloc) (void *, ptu32_t NewSize);
extern void  (*M_Free)(void * pl_mem);
extern void *  (*M_MallocHeap)(ptu32_t size,struct HeapCB *Heap,u32 timeout);
extern void *  (*M_MallocLc)(ptu32_t size,u32 timeout);
extern void *  (*M_MallocLcHeap)(ptu32_t size,struct HeapCB *Heap, u32 timeout);
extern void    (*M_FreeHeap)(void * pl_mem,struct HeapCB *Heap);
extern void *  (*__MallocStack)(struct EventECB *pl_ecb,u32 size);
extern ptu32_t (*M_FormatSizeHeap)(ptu32_t size,struct HeapCB *Heap);
extern ptu32_t (*M_FormatSize)(ptu32_t size);
extern ptu32_t (*M_GetMaxFreeBlockHeap)(struct HeapCB *Heap);
extern ptu32_t (*M_GetMaxFreeBlock)(void);
extern ptu32_t (*M_GetFreeMem)(void);
extern ptu32_t (*M_GetFreeMemHeap)(struct HeapCB *Heap);
extern ptu32_t (*M_CheckSize)(void * mp);


//�ڲ���������
void *__Heap_MallocBlock(ufast_t grade,struct HeapCession *Cession);

extern void __DJY_EventReady(struct EventECB *event_ready);
extern void __DJY_CutReadyEvent(struct EventECB *event);
extern void __DJY_ResumeDelay(struct EventECB *delay_event);
extern void __DJY_AddToDelay(u32 u32l_uS);
//extern void __Djy_CutEventFromEvttMarked(struct EventECB *event);


//----���Ҷ�-------------------------------------------------------------------
//����: ͨ�����֣����Ҷѿ��ƿ�ָ��
//����: HeapName���ѵ�����
//����: ��ָ�룬�Ҳ���������ԭ�򷵻�NULL
//-----------------------------------------------------------------------------
struct HeapCB *Heap_FindHeap(const char *HeapName)
{
    struct HeapCB *Heap = NULL,*Work;
    if((tg_pHeapList == NULL) || (HeapName == NULL))
        return NULL;
    Work = tg_pHeapList;
    do
    {
        if( ! strcmp(HeapName,Work->HeapName))
        {
            Heap = Work;
            break;
        }
        Work = Work->NextHeap;
    }while(Work != tg_pHeapList);
    return Heap;
}

//void __M_ShowHeap(void)
bool_t heap(void)
{
    struct HeapCB *Heap;
    struct HeapCession *Cession;
    struct EventECB *ECB;
    u32 n,m,k;

    if(tg_pHeapList == NULL)
        return (FALSE);

    Heap = tg_pHeapList;
    printf("�г�ϵͳ�����е�Heap\r\n\r\n");
    do
    {
        printf("Heap name: %s���� %d �� Cessions\r\n",
                    Heap->HeapName,Heap->CessionNum);
#if ((CFG_DYNAMIC_MEM == true))
        ECB = Heap->mem_sync;
        n = 0;
        if(ECB != NULL)
        {
            do
            {
                n++;
                ECB = ECB->multi_next;
            }while(ECB != Heap->mem_sync);
        }
#endif
        printf("�� %d ���¼��������ȴ��� Heap �����ڴ�\r\n\r\n",n);
        Cession = Heap->Cession;
        n = 0;
        m = 0;
        k = 0;
        while(Cession != NULL)
        {
            printf("Cession %d:\r\n",n+1);

            printf("׼��̬����ռ�:  0x%08x\r\n",
                    (ptu32_t)(Cession->heap_bottom - Cession->static_bottom));
#if ((CFG_DYNAMIC_MEM == true))
            printf("��̬����ռ�:    0x%08x\r\n",
                        Cession->ua_pages_num * Cession->PageSize);
#endif
            printf("��̬��ʼ��ַ:    0x%08x,    ��̬��ʼ��ַ:  0x%08x\r\n",
                        (ptu32_t)Cession->static_bottom,
                        (ptu32_t)Cession->heap_bottom);
#if ((CFG_DYNAMIC_MEM == true))
            printf("��ҳ��:          %10d��   ҳ�ߴ�:        0x%08x\n\r",
                        Cession->ua_pages_num,Cession->PageSize);
            printf("����ҳ��:        %10d��   ���пռ�:      0x%08x\r\n",
                        Cession->free_pages_num,
                        Cession->free_pages_num * Cession->PageSize);
            printf("����ߴ�:      0x%08x��   �����п�ߴ�:0x%08x\r\n\r\n",
                        Cession->ua_block_max,
                        Cession->ua_free_block_max);
#else
            printf("��cession���У�  %08x\n\r",(ptu32_t)(Cession->heap_top - Cession->heap_bottom));
#endif
#if ((CFG_DYNAMIC_MEM == true))
            m += Cession->ua_pages_num * Cession->PageSize;
            k += Cession->free_pages_num * Cession->PageSize;
#else
            k += Cession->heap_top - Cession->heap_bottom;
#endif
            n++;
            Cession = Cession->Next;
        }
        if(n > 1)
        {
            printf("%d��Cession�ܶ�̬����ռ� %d bytes\r\n",n,m);
            printf("%d��Cession�ܿ����ڴ�     %d bytes\r\n",n,k);
            printf("\r\n");
        }
        Heap = Heap->NextHeap;
    }while(Heap != tg_pHeapList);
    return true;
}

//void __M_ShowHeapSpy(void)
bool_t heap_spy(void)
{
    struct HeapCB *Heap;
    struct HeapCession *Cession;
    u16 *pl_eid;
    u32 n,m,k;

    if(tg_pHeapList == NULL)
        return (TRUE);

    Heap = tg_pHeapList;
    printf("�г�����Heap�ķ������\r\n");
    do
    {
        printf("Heap name: %s���� %d �� Cessions\r\n",
                    Heap->HeapName,Heap->CessionNum);
        Cession = Heap->Cession;
        n = 0;
        k = 0;
        while(Cession != NULL)
        {
            printf("Cession %d:\r\n",n+1);
            printf("         ҳ��           ����   �ߴ�       �¼�\r\n");
            m = 0;
#if ((CFG_DYNAMIC_MEM == true))
            pl_eid = Cession->index_event_id;
            while(m < Cession->ua_pages_num)
            {
                switch(pl_eid[m])
                {
                    case CN_MEM_DOUBLE_PAGE_LOCAL:
                    {//˫ҳ�ֲ�����,CN_MEM_DOUBLE_PAGE_LOCAL+event id
                        printf("%10d = %-10d �ֲ�  0x%08x %05d\n\r", m, m + 1,
                                    Cession->PageSize*2,pl_eid[m + 1]);
                        m += 2;
                    }break;
                    case CN_MEM_MANY_PAGE_LOCAL:
                    {//��ҳ�ֲ�����:CN_MEM_MANY_PAGE_LOCAL+event id+�׺�
                        printf("%10d = %-10d �ֲ�   0x%08x %05d\n\r",
                                    m,m+(1<<pl_eid[m+2])-1, Cession->PageSize*(1<<pl_eid[m+2]),
                                    pl_eid[m+1]);
                        m += 1<<pl_eid[m+2];
                    }break;
//                  case CN_MEM_SINGLE_PAGE_GLOBAL:
//                  {//��ҳȫ���ڴ�:-3
//                      printf("%10d - %-10d ȫ��   0x%08x\n\r",m,m,Cession->PageSize);
//                      m++;
//                  }break;
                    case CN_MEM_DOUBLE_PAGE_GLOBAL:
                    {//˫ҳȫ���ڴ�:CN_MEM_DOUBLE_PAGE_GLOBAL + event id
                        printf("%10d - %-10d ȫ��   0x%08x %05d\n\r",m,m+1,
                                    Cession->PageSize*2,pl_eid[m + 1]);
                        m+=2;
                    }break;
                    case CN_MEM_MANY_PAGE_GLOBAL:
                    {//��ҳȫ���ڴ�:CN_MEM_MANY_PAGE_GLOBAL+event id+�׺�
                        printf("%10d - %-10d ȫ��   0x%08x %05d\n\r",m,m+(1<<pl_eid[m+2])-1,
                                    Cession->PageSize*(1<<pl_eid[m+2]),pl_eid[m + 1]);
                        m += 1<<pl_eid[m+2];
                    }break;
                    case CN_MEM_FREE_PAGE:
                    {//�����ڴ�
                        for(k = m+1; k < Cession->ua_pages_num; k++)
                        {
                            if(pl_eid[k] != CN_MEM_FREE_PAGE)
                                break;
                        }
                        printf("%10d ~ %-10d ����   0x%08x\n\r",m,k-1,Cession->PageSize*(k-m));
                        m = k;
                    }break;
                    default :
                    {
                        //��ҳ�ֲ��ڴ�:event id
                        //��ҳȫ���ڴ�:event id + CN_EVTT_ID_MASK
                        if(pl_eid[m] < CN_EVTT_ID_MASK)
                        {
                            printf("%10d = %-10d �ֲ�   0x%08x %05d\n\r",m,m,
                                            Cession->PageSize,pl_eid[m]);
                        }else
                        {
                            printf("%10d = %-10d ȫ��   0x%08x %05d\n\r",m,m,
                                   Cession->PageSize,pl_eid[m] - CN_EVTT_ID_MASK);
                        }
//                      printf("%10d = %-10d �ֲ�   0x%08x %05d\n\r",m,m,Cession->PageSize,pl_eid[m]);
                        m++;
                    }break;
                }
            }
#endif
            n++;
            Cession = Cession->Next;
        }
        Heap = Heap->NextHeap;
    }while(Heap != tg_pHeapList);
    return true;
}

#if ((CFG_DYNAMIC_MEM == true))

//----��ѯ�ڴ�ߴ�-------------------------------------------------------------
//����: ���ݸ�����ָ��,��ѯ��ָ�����ڵ��ڴ��ĳߴ�.
//����: mp,��̬������ڴ�ָ��.
//����: �ڴ��ߴ�,����0�м��ֺ���:1.�Ƿ�ָ��,2.mp����׼��̬�����ָ��.
//-----------------------------------------------------------------------------
ptu32_t __Heap_StaticCheckSize(void * mp);
ptu32_t __Heap_CheckSize(void * mp)
{
    struct HeapCB *Heap;
    struct HeapCession *Cession;
    ptu32_t ua_pages_number;
    uint16_t *pl_id;
    ufast_t uf_free_grade_th;

    if(tg_pHeapList == NULL)
        return 0;
    Heap = tg_pHeapList;
    do
    {
        Cession = Heap->Cession;

        while(Cession != NULL)
        {

            if(((u8*)mp >= Cession->static_bottom)
                        && ((u8*)mp < Cession->heap_top))
            {
                if((u8*)mp < Cession->heap_bottom)    //��ָ���ھ�̬������
                {
                    return __Heap_StaticCheckSize(mp);
                }else                   //��ָ���ڿ�������̬������
                {
                #if ((CFG_DYNAMIC_MEM == true))
                    //�����ͷŵ��ڴ��ҳ��
                    ua_pages_number=(ptu32_t)((u8*)mp - Cession->heap_bottom)
                                                /Cession->PageSize;

                    //�����ͷŵ��ڴ��Ľ׺�,��0���.ͨ���׺�Ҳ����ȷ���ڴ���
                    //��С.ȷ���ڴ��������,�ֲ�������Ҫ֪��ӵ�и��ڴ���¼�id��
                    //ȫ�ַ�����������ڴ������ٱ�,����֪��ӵ�и��ڴ���¼�id
                    pl_id = &Cession->index_event_id[ua_pages_number];
                    switch(pl_id[0])
                    {
                        case CN_MEM_DOUBLE_PAGE_LOCAL :
                        {//˫ҳ�ֲ�����,CN_MEM_DOUBLE_PAGE_LOCAL+event id
                            uf_free_grade_th = 1;
                        }break;
                        case CN_MEM_MANY_PAGE_LOCAL :
                        {//��ҳ�ֲ�����:CN_MEM_MANY_PAGE_LOCAL+event id+�׺�
                            uf_free_grade_th = (ufast_t)pl_id[2];
                        }break;
//                      case CN_MEM_SINGLE_PAGE_GLOBAL :
//                      {//��ҳȫ���ڴ�:-3
//                          uf_free_grade_th = 0;
//                      }break;
                        case CN_MEM_DOUBLE_PAGE_GLOBAL :
                        {//˫ҳȫ���ڴ�:CN_MEM_DOUBLE_PAGE_GLOBAL + event id
                            uf_free_grade_th = 1;
                        }break;
                        case CN_MEM_MANY_PAGE_GLOBAL :
                        {//��ҳȫ���ڴ�:CN_MEM_MANY_PAGE_GLOBAL+(event id)|CN_EVTT_ID_MASK+�׺�.
                            uf_free_grade_th = (ufast_t)pl_id[2];
                        }break;
                        case CN_MEM_FREE_PAGE :
                        {
                            uf_free_grade_th = CN_LIMIT_UFAST;
                        }break;
                        default :
                        {
                            //��ҳ�ֲ��ڴ�:event id
                            //��ҳȫ���ڴ�:event id + CN_EVTT_ID_MASK
                            uf_free_grade_th = 0;       //��ҳ�ֲ�����:id,

//                          if(pl_id[0] <= CN_EVENT_ID_LIMIT)
//                              uf_free_grade_th = 0;       //��ҳ�ֲ�����:id,
//                          else
//                          {
//                              DJY_SaveLastError(EN_MEM_ERROR);
//                              return 0;
//                          }
                        }break;
                    }
                    if(uf_free_grade_th == CN_LIMIT_UFAST)
                        return 0;
                    else
                        return Cession->PageSize << uf_free_grade_th;
                #endif
                }
            }
            Cession = Cession->Next;
        }
        Heap = Heap->NextHeap;
    }while(Heap != tg_pHeapList);

    //mpָ�벻���κ�heap�ķ�Χ֮�ڣ������ߵ����
    return 0;
}

//----������������λ1λ�ã�ǰ��0�ĸ�����----------------------------------------
//���ܣ������������ֽڵ����λ��ʼ����ǰ��0��������
//������data,��������ָ��
//      len,����Ĵ�С
//���أ�ǰ��0�ĸ���
//------------------------------------------------------------------------------
ucpu_t __Heap_LeadingZero(u8 *data,u8 len)
{
    u8 u8_i;
    ucpu_t  uc_j=0;
#if CN_CFG_BYTE_ORDER == CN_CFG_LITTLE_ENDIAN
    u8_i=len-1;
    do
    {
        if(cn_leading_zero[data[u8_i]] != 8)
        {
            uc_j+=cn_leading_zero[data[u8_i]];
            break;
        }else
            uc_j +=8;
    }while(u8_i-- !=0);
#else
    u8_i=0;
    do
    {
        if(cn_leading_zero[data[u8_i]] != 8)
        {
            uc_j+=cn_leading_zero[data[u8_i]];
            break;
        }else
            uc_j +=8;
    }while(u8_i++ != len-1);
#endif
    return uc_j;
}

//----������Ʊ�������ֽ���---------------------------------------------------
//���ܣ�������Ʊ�������ֽ���
//������u32_pages,�ڴ�ҳ��
//���أ��ֽ���
//ע��: ���ֺ������ܵĵ�һ��,��Ҫͬʱ������ƽṹ�����ֵ��ֽ���
//      ��ֲ����:����������Ļ����Ķ��뷽ʽ�й�,��ֲʱ������˽�����Ķ��뷽ʽ.
//      ��������������󲿷�cpu�Ķ���Ҫ��,�����ų�������.��������ֲʱ����ö���
//      ��������Ȼ�����Ƿ���Ҫ�޸�
//-----------------------------------------------------------------------------
u32 __Heap_CalculateExpense(ptu32_t ua_pages)
{
    ptu32_t ua_bits,u32_return;
    ufast_t uf_grades,uf_grade_th;

    //�����������һ���ж�����ҳ�ߴ�,����:��ַ���ȼ�ȥua_pages��ǰ��0����������
    //����,����,ua_pages=5,���λλ����bit2,��29��ǰ��0��ҳ�ߴ���1,2,4ҳ3��,����=3
    uf_grades=CN_PTR_BITS-__Heap_LeadingZero((u8 *)&ua_pages,sizeof(ptu32_t));

    //ÿҳ����һ��16λ�ּ�¼ӵ�б�ҳ�ڴ���¼���ID��
    //������߽��ڴ��ʱ��ֻ��¼��ҳ��freeʱ������������ñ�����Ϊ�˷�ֹ��
    //�¼��佻�������ͷ��ڴ�ʱ�����ڳ��������������û�����ʹ�á�
    u32_return = ua_pages * sizeof(u16);

    //��һ����������ufast_t���͵�,��Ҫ��ufast_t���Ͷ���.
    u32_return = align_up(sizeof(ufast_t),u32_return);

    //ÿ�׻�Ҫ����һ��ufast_t���͵�����,��¼���׵ļ���
    u32_return += uf_grades*(sizeof(ufast_t));

    //��һ����������ָ��,��Ҫ��ָ�����
    u32_return = align_up(sizeof(void*),u32_return);

    //ÿ��һ��ָ��,ָ��λͼ������
    u32_return += uf_grades*sizeof(void*);

    for(uf_grade_th = 0; uf_grade_th < uf_grades; uf_grade_th++)
    {
        ua_bits = ua_pages>>uf_grade_th;   //���㱾����λ��
        do
        {
            ua_bits=(ua_bits+CN_CPU_BITS-1)>>CN_CPU_BITS_SUFFIX_ZERO;
            u32_return += ua_bits*sizeof(ucpu_t);   //�ü�λͼ����
            u32_return += sizeof(void*);            //��Ҫһ��ָ��ָ���λͼ����
        }while(ua_bits>1); //ֱ�����м���������.
    }
    return u32_return;
}

//----����ucpu_t������ǰ��0�ĸ���----------------------------------------------
//���ܣ���ucpu_t�����ݵ����λ��ʼ����ǰ��0��������
//������data,������
//���أ�ǰ��0�ĸ���
//-----------------------------------------------------------------------------
ucpu_t __Heap_LeadingUcpuZero(ucpu_t data)
{
    return(__Heap_LeadingZero((u8*)&data,sizeof(ucpu_t)));
}

//----���ع�񻯽�-------------------------------------------------------------
//���ܣ����������ݹ��Ϊ��񻯳ߴ硣����ǰ����֪sizeС��Cession������ߴ�
//������size,��������ڴ��ߴ�
//      Heap��Ŀ��ѡ�
//���أ��׺š�
//-----------------------------------------------------------------------------
ufast_t __Heap_GetGrade(ptu32_t size,struct HeapCession *Cession)
{
    ucpu_t  grade;
    u32 pages;

    pages = (size + Cession->PageSize -1)/Cession->PageSize;
    grade = __Heap_LeadingZero((u8 *)&pages,sizeof(u32));
    if((CN_HIGH_1BIT_1>>grade) != pages)
    //���grade���ǹ�񻯵���,���������������С������С
    //����ά��size�Ĵ�С,��������
        grade--;
    grade = CN_PTR_BITS-1-grade;
    //���ؽ���,��0���,����Сҳ�ߴ�ǰ��0����ߴ�ǰ��0.
    return grade;
}


//----���ؿ��еĹ�񻯽�-------------------------------------------------------
//���ܣ����������ݹ��Ϊ���п����ڴ�Ĺ�񻯡��ý׺ź�cession��ʾ��ͬ���Ľ�
//      �ţ���ͬcession����ߴ��ǲ�һ���ġ�
//      ���ñ�����ǰ���Ѿ�ȷ��heap�����㹻�Ŀ����ڴ档
//������size,��������ڴ��ߴ�
//      Heap��Ŀ��ѡ�
//      result���������ʵĿ����ڴ���Cession��
//���أ��׺š�
//-----------------------------------------------------------------------------
ufast_t __Heap_GetFreeGrade(ptu32_t size,struct HeapCB *Heap,
                         struct HeapCession **result)
{
    ucpu_t  grade,result_grade = 0;
    struct HeapCession *Cession;
    struct HeapCB *Work;
    ptu32_t tsize,fsize = CN_LIMIT_PTU32;
    u32 pages;

    if((Heap->HeapProperty & CN_HEAP_PRIVATE) == CN_HEAP_PRIVATE )
    {
        Cession = Heap->Cession;
        while(Cession != NULL)
        {
            if(Cession->ua_free_block_max >= size)
            {
                pages = (size + Cession->PageSize -1)/Cession->PageSize;
                grade = __Heap_LeadingZero((u8 *)&pages,sizeof(u32));
                if((CN_HIGH_1BIT_1>>grade) != pages)
                //���grade���ǹ�񻯵���,���������������С������С
                //����ά��size�Ĵ�С,��������
                    grade--;
                grade = CN_PTR_BITS-1-grade;
                tsize = Cession->PageSize << grade;
                if(tsize < fsize)
                {
                    *result = Cession;
                    fsize = tsize;
                    result_grade = grade;
                 }
            }
            Cession = Cession->Next;
        }
    }
    else
    {
        Work = Heap;
        do
        {
            if((Work->HeapProperty & CN_HEAP_PRIVATE) == CN_HEAP_GENERAL )
            {
                Cession = Work->Cession;
                while(Cession != NULL)
                {
                    if(Cession->ua_free_block_max >= size)
                    {
                        pages = (size + Cession->PageSize -1)/Cession->PageSize;
                        grade = __Heap_LeadingZero((u8 *)&pages,sizeof(u32));
                        if((CN_HIGH_1BIT_1>>grade) != pages)
                        //���grade���ǹ�񻯵���,���������������С������С
                        //����ά��size�Ĵ�С,��������
                            grade--;
                        grade = CN_PTR_BITS-1-grade;
                        tsize = Cession->PageSize << grade;
                        if(tsize < fsize)
                        {
                            *result = Cession;
                            fsize = tsize;
                            result_grade = grade;
                         }
                    }
                    Cession = Cession->Next;
                }
            }
            Work = Work->NextHeap;
        }while(Work != Heap);
    }

    //���ؽ���,��0���,����Сҳ�ߴ�ǰ��0����ߴ�ǰ��0.
    return result_grade;
}
//----�鿴���ȴ������ڴ�----------------------------------------------------
//���ܣ��ȴ�������һ�����������˫��ѭ������,����������ڴ�������,С����ǰ,
//      ���ڴ����ʱ,����ʹ������С���¼��õ�����,��������ʹ�ڴ����㾡�����¼�,
//      ��ʹ�����ȼ�����,��Ϊʵʱ�¼��ǲ�����ʹ�ö�̬�����,���Բ���Ӱ��ʵʱ��.
//      ���Heap��������ͨ�ö�,��ʹ��ͨ�öѵĵȴ�ָ��,�������,ֻҪ����һ��ͨ��
//      ���п����ڴ�,�ͷ���true.
//������size����Ҫ������ڴ�ߴ�
//      Heap��Ŀ��heap
//      timeout����ʱ����,��λ��΢�룬CN_TIMEOUT_FOREVER=���޵ȴ���
//      ��0ֵ�������ϵ���ΪCN_CFG_TICK_US��������
//����: true = �п����ڴ湩���䣬false = ���ڴ�ɷ���
//��ע: �������ɲ���ϵͳ����,�û����ܵ���.
//���¼�¼:
// 1.����: 2015/4/25
//   ˵��: �����ж�ͨ�öѻ���ר�ö�
//   ����: ������
//------------------------------------------------------------------------------
bool_t __Heap_CheckMemory(ptu32_t size,struct HeapCB *Heap,u32 timeout)
{
    struct EventECB *event;
    struct EventECB **MemSyncHead;
    u32 u32l_start_time,u32l_rest_time = 0;
    bool_t   wait;
    ptu32_t FreeSize;

    u32l_start_time = (u32)DJY_GetSysTime();
    if( (Heap->HeapProperty & CN_HEAP_PRIVATE) == CN_HEAP_PRIVATE)
        MemSyncHead = &Heap->mem_sync;      //ʹ��ר�öѵ�ͬ��ָ��
    else
        MemSyncHead = &s_ptGenMemSync;         //ʹ��ͨ�öѵ�ͬ��ָ��

    do
    {
        wait=false;

        FreeSize = __Heap_GetMaxFreeBlockHeap(Heap );
        if(FreeSize < size)
        {//���Ŀɷ����ߴ�С��������ڴ�,û���ڴ���Է���
            Int_SaveAsynSignal();
            wait=true;  //����Ǿ���������������ִ��,whileѭ����Ҫ�ٴ��ж�
                        //�Ƿ����㹻���ڴ����.
            __DJY_CutReadyEvent(g_ptEventRunning);

            g_ptEventRunning->next = NULL;
            g_ptEventRunning->previous = NULL;

            g_ptEventRunning->wait_mem_size = size; //���ȴ����ֽ���
            event = *MemSyncHead;       //��ȡ�ڴ�ȴ���ָ��

            //���°��¼������ڴ�ȴ�������
            g_ptEventRunning->sync_head = MemSyncHead;
            if(event == NULL)    //�ȴ����п�
            {
                g_ptEventRunning->multi_next = g_ptEventRunning;
                g_ptEventRunning->multi_previous = g_ptEventRunning;
                Heap->mem_sync = g_ptEventRunning;
            }else
            {
                do
                {//��ѭ���ҵ���һ�������ڴ�������¼����¼�.
                    if(event->wait_mem_size < size)
                        event = event->multi_next;
                    else
                        break;
                }while(event != *MemSyncHead);
                //���û���ҵ������ڴ�����¼������¼�,���¼��������β,��
                //����β������event_wait��ǰ��,��ʱeventǡ�õ���event_wait
                //����ҵ�ʣ����ʱʱ�䳤�����¼����¼�,���¼�������¼�ǰ��.
                //��������ǰ��ѭ���ҵ����,����ʹ�����д���
                g_ptEventRunning->multi_next = event;
                g_ptEventRunning->multi_previous = event->multi_previous;
                event->multi_previous->multi_next = g_ptEventRunning;
                event->multi_previous = g_ptEventRunning;
            }

            if(timeout != CN_TIMEOUT_FOREVER)
            {
                g_ptEventRunning->event_status= CN_STS_WAIT_MEMORY
                                                + CN_STS_SYNC_TIMEOUT;
                u32l_rest_time = timeout-((u32)(u32)DJY_GetSysTime() - u32l_start_time);
                __DJY_AddToDelay(timeout-u32l_rest_time);
            }else
            {
                g_ptEventRunning->event_status = CN_STS_WAIT_MEMORY;
            }

            Int_RestoreAsynSignal();  //���жϲ���ʵ�ֵ���
            //�������ﷵ�أ��ǳ�ʱ�������¼��ͷ��ڴ�
            if(g_ptEventRunning->wakeup_from & CN_STS_SYNC_TIMEOUT)
            {//����ӳ�ʱ���أ���Ӧ�÷���false��˵��û���ڴ����
             //����ǳ�ʱ���أ�˵�����߳��ͷ��ڴ棬��������һ��whileѭ��
             //����Ƿ����㹻���ڴ�,
                return false;   //��ʱ����
            }
        }
    }while(wait==true);
    return true;    //return trueǰ�ǲ����жϵ�,�����ڷ����ڴ�֮ǰ�����ж�.
}

//----����running�¼�������ڴ�------------------------------------------------
//����: ���ڴ������¼�id���в�����event_id�¼�����ľֲ��ڴ棬����У���Ǽ�
//      enum_mem_leak���󣬲�ǿ���ͷ�֮��
//����: event_id��Ŀ���¼�id
//����: ��
//-----------------------------------------------------------------------------
void __Heap_CleanUp(uint16_t event_id)
{
    struct HeapCB *Heap;
    struct HeapCession *Cession;
    uint16_t *pl_id;
    u32 n;

    if(M_Malloc != __Heap_Malloc)
        return;             //��̬�ڴ����δ��ʼ����
    Heap = tg_pHeapList;
    do
    {
        Cession = Heap->Cession;

        while(Cession != NULL)
        {
            pl_id = Cession->index_event_id;
            n = 0;
            while(n < Cession->ua_pages_num)
            {
            //�Ķ���������������struct HeapCession��index_event_id��ע��.
                if(pl_id[n] == event_id)
                {
                    __Heap_Free(Cession->PageSize * n + Cession->heap_bottom);
                    n++;
                }
                else if( (pl_id[n] == CN_MEM_DOUBLE_PAGE_LOCAL)
                          && (pl_id[n+1] == event_id))
                {
                    __Heap_Free(Cession->PageSize * n + Cession->heap_bottom);
                    n += 2;
                }
                else if( (pl_id[n] == CN_MEM_MANY_PAGE_LOCAL)
                          && (pl_id[n+1] == event_id))
                {
                    __Heap_Free(Cession->PageSize * n + Cession->heap_bottom);
                    n += 1<<pl_id[n+1];
                }
            }
            Cession = Cession->Next;
        }
        Heap = Heap->NextHeap;
    }while(Heap != tg_pHeapList);

    //mpָ�벻���κ�heap�ķ�Χ֮�ڣ������ߵ����
    return ;
}


bool_t __Heap_InitCession(struct HeapCession *Cession)
{
    ptu32_t ua_temp,ua_temp1=0,ua_temp2,ua_temp3 = 0,ua_faction;
    ptu32_t ua_pages,ua_table_size=0;
    ufast_t uf_classes,uf_grades;
    ucpu_t  ***pppl_bitmap,**ppl_bitmap,*pl_bitmap;
    ufast_t *pl_classes;

    ua_faction=(ptu32_t)(Cession->heap_top-Cession->heap_bottom);
    //����ѵ���ҳ����ȡ��ҳ����ȥ��������
    ua_pages = ua_faction / Cession->PageSize;
    ua_faction = ua_faction % Cession->PageSize;
    //�����ҳ����������������������ڴ���ƽṹʹ�á�
    Cession->heap_top -=ua_faction;

    if(ua_pages < 2) //�������ҳ��С��2ҳ,���ƽṹ���ܻ�Ҫռ��1ҳ,����������?
    {
        Cession->ua_pages_num = 0;
        Cession->free_pages_num = 0;
        Cession->ua_block_max = 0;
        Cession->ua_free_block_max = 0;
        Cession->uf_grades = 0;
        Cession = Cession->Next;
        return false;
    }

    ua_temp = __Heap_CalculateExpense(ua_pages); //�������ƽṹ��Ҫ���ֽ���
    if(ua_faction < ua_temp)
    {
        //�������ƽṹ��ua_faction�⻹��Ҫ��ҳ��
        ua_temp = (ua_temp-ua_faction+Cession->PageSize-1)
                    /Cession->PageSize;
        for(ua_temp1 = ua_temp; ua_temp1 > 0; ua_temp1--)
        {//������ҳ���϶����ڻ����ʵ����Ҫ��ҳ��,�������ڴ�ҳ���϶�ʱ.
         //����ֵ�ӳ�����ҳ����ʼ��һ��С,ֱ������Ϊֹ.
            //�ع����ƽṹ�ֽ���
            ua_temp2 = __Heap_CalculateExpense(ua_pages-ua_temp1);
            ua_temp3 = (ua_temp2-ua_faction+Cession->PageSize-1)
                    /Cession->PageSize;         //���¼�����ƽṹҳ��
            if(ua_temp3 >= ua_temp1)
            //������ҳ���պô��ڻ���ڹ���ֵʱ,ѭ����ֹ
                break;
        }
        //ʵ�ʿɷ����ҳ��,u32_temp3Ϊ���ƽṹҳ��
        ua_pages = ua_pages -ua_temp3;
        Cession->heap_top -= ua_temp3 * Cession->PageSize;  //�����Ѷ�
    }
    Cession->ua_pages_num = ua_pages;
    Cession->free_pages_num = ua_pages;
    //�ڴ���ƽṹ����
    memset(Cession->heap_top,0,ua_faction+(ua_temp3 *Cession->PageSize));

    //�����ܽ�������һ���ж�����ҳ�ߴ�,����:��ַ���ȼ�ȥua_pages��ǰ��0
    //����������,ua_pages=5,���λλ����bit2,��29��ǰ��0��ҳ�ߴ���1,2,4
    //ҳ3��,����=3
    uf_grades=CN_PTR_BITS-__Heap_LeadingZero((u8 *)&ua_pages,sizeof(ptu32_t));

    Cession->uf_grades = uf_grades;    //�ܽ���
    Cession->ua_block_max=Cession->PageSize<<(uf_grades-1);//��߽׿�ߴ�
    Cession->ua_free_block_max=Cession->ua_block_max; //�����ÿ�ߴ�

    //����λͼָ��������ָ����
    ua_table_size=0;
    for(ua_temp=0;ua_temp<uf_grades;ua_temp++)
    {//��������
        ua_temp1 = ua_pages>>ua_temp;   //����ua_temp��0��λͼλ��
        do
        {
            //������һ��λͼ��λ��
            ua_temp1=(ua_temp1+CN_CPU_BITS-1)>>CN_CPU_BITS_SUFFIX_ZERO;
            ua_table_size++;        //��Ҫһ��ָ��ָ��ü�λͼ�׵�ַ
        //��һ��λͼ����1λ(��������cn_cpu_bitsλ),����ѭ��
        }while(ua_temp1>1);
    }

    //�¼�ָ��id��,��ϸ˵����mem_global_t���Ͷ��岿��.
    Cession->index_event_id=(uint16_t*)Cession->heap_top;
    for(ua_temp = 0; ua_temp<ua_pages; ua_temp++)
        Cession->index_event_id[ua_temp] = CN_MEM_FREE_PAGE;
    //���׵�λͼ������ָ�룬ÿ����һ��ufast_t�͵�����ʾ�ý׵�λͼ����
    ua_temp = (ptu32_t)(Cession->index_event_id+ua_pages);
    ua_temp = (ua_temp+sizeof(ufast_t)-1)&(~(sizeof(ufast_t)-1)); //����
    pl_classes=(ufast_t *)ua_temp;
    Cession->p_classes=pl_classes;     //���׿��н�������������ָ��

    //����Ǹ��������ָ���ָ����ַ
    ua_temp = (ptu32_t)(pl_classes+uf_grades);
    ua_temp = (ua_temp+sizeof(void *)-1)&(~(sizeof(void *)-1));  //����
    pppl_bitmap=(void*)ua_temp;
    //���׽�����λͼָ����ָ������ָ��
    Cession->ppp_bitmap=pppl_bitmap;

    //������λͼָ����ָ��
    ppl_bitmap=(ucpu_t **)(pppl_bitmap+uf_grades);
    pl_bitmap=(ucpu_t *)(ppl_bitmap+ua_table_size); //������λͼָ���

    //����������ͷ����
    for(ua_temp=0;ua_temp<uf_grades;ua_temp++)  //��������
    {
        ua_temp1 = ua_pages>>ua_temp;       //ua_temp��λͼ��λ��
        uf_classes=0;                       //ua_temp�׵�λͼ����
        pppl_bitmap[ua_temp]=ppl_bitmap;    //ua_temp��λͼ���ָ��
        do
        {
            ucpu_t  msk=~0;     //����λ��δ֪,�ô˷�����ȫ1
            ua_temp3=ua_temp1>>CN_CPU_BITS_SUFFIX_ZERO; //������Ĵ�С(��������)
            ua_temp1=ua_temp1%CN_CPU_BITS;    //��·������Ĵ�С(��������)
            if(ua_temp1==0)
            //����û�ж����λ,���账��
                ua_temp1=ua_temp3;  //����λͼ��ռ��������һ����λ��
            else
            {//�����ж����λ,��1,ʹ�����λ��ͬ���ѷ����λ.
                msk=msk<<ua_temp1;              //ʹ�����λ��1
                *(pl_bitmap+ua_temp3) |=msk;    //�޸�λͼ����Ӧ��λ
                ua_temp1=ua_temp3+1;    //·��������,������������
            }
            *ppl_bitmap++ = pl_bitmap;  //��¼����λͼָ��
            pl_bitmap +=ua_temp1;       //λͼָ��ָ����һ��λͼ��ʼ��ַ
            uf_classes++;                //ua_temp�׵ļ�������
        }while(ua_temp1>1);             //ֱ��������������λͼ��
        pl_classes[ua_temp]=uf_classes; //�ü�·�������
    }
    return true;
}

//-----------------------------------------------------------------------------
//���ܣ����heap��ϵͳ�У�����malloc�Ⱥ������䡣һ����˵��������lds�ļ��ж���heap����
//     ��Щ���������Ҫ�ֶ���Ӷѡ��ڼ�����ϵͳ֮ǰ���͵�����Heap_StaticModuleInit
//     �����������ô˺���ǰ��lds�����õ�heap���ڴ�Ӧ���ǿɶ�д�ġ�����Щram���ر���qspi�ӿ�
//     ��ram����ʱ�������ɷ��ʣ��ʲ�����lds�����ã���Ӧ���ں������Heap_Add������ӡ�
//     djyos���������heap�������������һ����heap������������heap������ڴ档
//������bottom���½�heap����ʼ��ַ��
//     size��heap�ĳߴ硣
//     PageSize��ҳ�ߴ�
//     AlignSize��Ҫ��Ķ���ߴ磬0��ʾ��ϵͳ�Ķ���ߴ�
//     proper��true��ʾר��heap��false��ʾͨ��heap
//     name���½���heap���֡�
//���أ��½���heapָ�룬����heapʧ�ܵķ���NULL
//�ر�ע�⣺����ӵ�heap��������������heap�������غϣ����Բ��������һ��heap��mallocһ��
//         ���ڴ棬Ȼ������Ϊheap��ӵ�ϵͳ�С�
//------------------------------------------------------------------------------
struct HeapCB *Heap_Add(void* bottom, u32 size, u32 PageSize,u32 AlignSize,
                        bool_t proper,char *name)
{
    struct HeapCB *HeapTemp;
    struct HeapCession *Cession;
    void* HeapBottom;
    u32 ctrlsize;

    ctrlsize = sizeof( struct HeapCB ) + sizeof(struct HeapCession);
    if(bottom == NULL)
       return NULL;
    HeapTemp = (struct HeapCB *)(bottom + size - ctrlsize);
    HeapTemp = (struct HeapCB *)align_down_sys(HeapTemp);
    HeapBottom = (void*)align_up_sys(bottom);
    if((u32)((void*)HeapTemp - HeapBottom) < PageSize)
        return NULL;
    if(tg_pHeapList == NULL)
    {
        tg_pHeapList = HeapTemp;
        tg_pHeapList->NextHeap = HeapTemp;
        tg_pHeapList->PreHeap = HeapTemp;
        //��һ���ѿ���ϵͳ�ѡ�
        tg_pSysHeap = tg_pHeapList;
    }
    else
    {
        HeapTemp->NextHeap = tg_pHeapList;
        HeapTemp->PreHeap = tg_pHeapList->PreHeap;
        tg_pHeapList->PreHeap->NextHeap = HeapTemp;
        tg_pHeapList->PreHeap = HeapTemp;
    }

    Cession = (struct HeapCession *)(HeapTemp+1);
    HeapTemp->Cession = Cession;
    HeapTemp->CessionNum = 1;
    if(AlignSize == 0)
        HeapTemp->AlignSize = CN_ALIGN_SIZE;
    else
        HeapTemp->AlignSize = AlignSize;
    HeapTemp->HeapProperty = proper;
    HeapTemp->HeapName = name;
#if ((CFG_DYNAMIC_MEM == true))
    HeapTemp->mem_sync = NULL;
#endif
    Cession->static_bottom = HeapBottom;
    Cession->heap_bottom = HeapBottom;
    Cession->heap_top = (void*)HeapTemp;
    Cession->last = (list_t *)HeapBottom;
    dListInit(Cession->last);
#if ((CFG_DYNAMIC_MEM == true))
    Cession->PageSize = PageSize;
#endif
    Cession->Next = NULL;
    if(__Heap_InitCession(Cession))
    {
        Lock_MutexCreate_s(&(HeapTemp->HeapMutex),(const char*)(HeapTemp->HeapName));
        return(HeapTemp);
    }
    else
        return NULL;
}

//----��ʼ���ڴ��-------------------------------------------------------------
//���ܣ�1.�Ӷ��з�����ʿ��ƽṹ������ڴ棬λ���ڶѵĶ�����ռ��������
//      2.��ʼ���ڴ������ƽṹ,��ʼ��ȫ���ڴ�ṹ struct MemGlobal
//      3.��ʼ���ڴ�����
//���أ�1=�ɹ���0=ʧ��
//��ע: 1.�������ڳ�ʼ�����֮ǰ����,�첽�ź���δ����,���迼�ǹر��жϵ�����.
//      2.���ñ�����ǰ,ֻ����ִ�о�̬���书��,����ִ�ж�̬���书��.
//      3.Ŀǰֻ�����ڲ���mmu���������mmu�ر���֧�ֶ���̵������û����á�
//-----------------------------------------------------------------------------
bool_t Heap_DynamicModuleInit(void)
{

    struct HeapCB *Heap;
    struct HeapCession *Cession;

    if(tg_pHeapList == NULL)
        return false;
    Heap = tg_pHeapList;
    do
    {
        Cession = Heap->Cession;
        Lock_MutexCreate_s(&(Heap->HeapMutex),(const char*)(Heap->HeapName));

        while(Cession != NULL)
        {
            __Heap_InitCession(Cession);
            Cession = Cession->Next;
        }       //for while(Cession != NULL)
        Heap = Heap->NextHeap;
    }while(Heap != tg_pHeapList);

    M_Malloc = __Heap_Malloc;
    M_Realloc = __Heap_Realloc;
    M_Free = __Heap_Free;
    M_MallocHeap = __Heap_MallocHeap;
    M_MallocLc = __Heap_MallocLc;
    M_MallocLcHeap = __Heap_MallocLcHeap;
    M_FreeHeap = __Heap_FreeHeap;
    __MallocStack = __Heap_MallocStack;
    M_FormatSizeHeap = __Heap_FormatSizeHeap;
    M_FormatSize = __Heap_FormatSize;
    M_GetMaxFreeBlockHeap = __Heap_GetMaxFreeBlockHeap;
    M_GetMaxFreeBlock = __Heap_GetMaxFreeBlock;
    M_GetFreeMem = __Heap_GetFreeMem;
    M_GetFreeMemHeap = __Heap_GetFreeMemHeap;
    M_CheckSize = __Heap_CheckSize;

    return true;
}

//lst debug
//void *event11m[2000];
//u32 moff = 0,foff = 0;
//void save11(void *p)
//{
//    s32 loop;
//    if((g_ptEventRunning->event_id == 10) && (p != NULL))
//    {
//        for(loop = moff; loop >0; loop--)
//        {
//            if(p == event11m[loop-1])
//                printk("error = %x\r\n", p);
//        }
//        if(moff < 2000)
//        {
//            event11m[moff] = p;
//            moff++;
//        }
//    }
//}
//void free11(void *p)
//{
//    s32 loop;
//    if((p != NULL))
//    {
//        for(loop = moff - 1; loop >= 0; loop--)
//        {
//            if(event11m[loop] == p)
//            {
//                event11m[loop] += 1;
//            }
//        }
//    }
//}

//----���ڴ���з����ڴ�-------------------------------------------------------
//���ܣ�1.����ڴ�ߴ磬��������Ҫ�����С�ڴ�ߴ磬����ÿ��ڴ�ߴ�Ľ���
//      2.��ȡ�ü�����·����ȣ��ط���·���𼶲��ң�ֱ���ҵ������ڴ�Ϊֹ��
//      3.�����ڴ�����
//      4.���ڴ������Ϣ����������,�Ա������ڴ���.
//      5.����ڴ治�㣬����¼�����tg_mem_global.event_wait��,�������¼��л�.
//������size,��������ڴ��ߴ�
//      Heap, Ŀ���,������Ǹ�ר�ö�,���޶�������ѷ���,���������ͨ�öѷ���
//      timeout����ʱ����,��λ��΢�룬CN_TIMEOUT_FOREVER=���޵ȴ���0��������
//      ��ʱ���ء���0ֵ�������ϵ���ΪCN_CFG_TICK_US��������
//���أ�������ڴ�ָ�룬NULL��ʾû���ڴ���Է���
//��ע: ����ڶ��¼���������ǰ���ñ���������¼ӵ����ʱȫ������ϵͳ�����¼��С�
//      ϵͳ�����¼������������ʵ�ͬ��ȫ�ַ��䡣
//-----------------------------------------------------------------------------
void *__Heap_MallocLcHeap(ptu32_t size,struct HeapCB *Heap,u32 timeout)
{
    struct HeapCession *Cession;
    void *ua_address;
    ufast_t  uf_grade_th;
    bool_t   en_scheduler;
    void *result;
    uint16_t *pl_id,id;
    u32 page;

    if( (size == 0) || (Heap == NULL) )
        return NULL;

    //�����ڴ�ֱ���ж�size�Ƿ�����,��Ϊȡ�û�����ǰ����������ж���Ч.
    if(Lock_MutexPend(&Heap->HeapMutex, timeout) == false)
        return NULL;
    en_scheduler = DJY_QuerySch( );
    //��ֹ���������£������ǰû���㹻�Ŀ����ڴ棬�ԹԵ��߰ɡ�
    if((__Heap_GetMaxFreeBlockHeap(Heap) < size)
                && ((timeout == 0) || !en_scheduler))
    {
        result = NULL;
    }else
    {
        if( !__Heap_CheckMemory(size,Heap,timeout) )
        {
            result = NULL;        //û�к��ʵĿ����ڴ��
        }else
        {    //�к��ʵĿ����ڴ��
            uf_grade_th=__Heap_GetFreeGrade(size,Heap,&Cession);    //ȡ�׺�
            ua_address=__Heap_MallocBlock(uf_grade_th,Cession);     //�����ڴ�
            g_ptEventRunning->local_memory++;
            g_ptEventRunning->HeapSize += Cession->PageSize * (1<<uf_grade_th);
            if(g_ptEventRunning->HeapSize > g_ptEventRunning->HeapSizeMax)
                g_ptEventRunning->HeapSizeMax = g_ptEventRunning->HeapSize;

            //�Ķ���������������struct HeapCession��index_event_id��ע��.
            pl_id = Cession->index_event_id;
            id = g_ptEventRunning->event_id;
            page = (ptu32_t)((u8*)ua_address-Cession->heap_bottom)/Cession->PageSize;
            if(uf_grade_th==0)
            {//����1ҳ
                pl_id[page] = id;
            }else if(uf_grade_th==1)
            {//����2ҳ
                pl_id[page] = CN_MEM_DOUBLE_PAGE_LOCAL;
                pl_id[page+1] = id;
            }else
            {   //�����ҳ
                pl_id[page] = CN_MEM_MANY_PAGE_LOCAL;
                pl_id[page+1] = id;
                pl_id[page+2] = uf_grade_th;
            }
            result = ua_address;
        }
    }
    Lock_MutexPost(&Heap->HeapMutex);
    __Heap_CheckSTackSync( );
    return result;
}

void *__Heap_MallocLc(ptu32_t size,u32 timeout)
{
    return ( __Heap_MallocLcHeap(size,tg_pSysHeap,timeout) );
}

//----����ȫ���ڴ�-------------------------------------------------------------
//���ܣ�1.����ڴ�ߴ磬��������Ҫ�����С�ڴ�ߴ磬����ÿ��ڴ����ڵڼ��ס�
//      2.��ȡ�ü�����·����ȣ��ط���·���𼶲��ң�ֱ���ҵ������ڴ�Ϊֹ��
//      3.�����ڴ�����
//      4.����ڴ治�㣬����¼�����tg_mem_global.event_wait��,�������¼��л�.
//������size,��������ڴ��ߴ�
//      Heap, Ŀ���,������Ǹ�ר�ö�,���޶�������ѷ���,���������ͨ�öѷ���
//      timeout����ʱ����,��λ��΢�룬CN_TIMEOUT_FOREVER=���޵ȴ���0��������
//      ��ʱ���ء���0ֵ�������ϵ���ΪCN_CFG_TICK_US��������
//���أ�������ڴ�ָ�룬NULL��ʾû���ڴ���Է���
//��ע: �ô˺���������ڴ�,���������¼����ʱ���ջ�.
//-----------------------------------------------------------------------------
//#include "stm32f7xx.h" lst debug
//#include <core_cm7.h>
//struct recstack
//{
//    void * p;
//    u32 ev;
//    u32 who;
//    u32 tm;
//};
//struct recstack rec[2000];
//u32 recoffset = 0;
//void recordwho(void * m)
//{
//    u32 *st;
//    st = __get_PSP() + 25*4;
//    if(recoffset < 2000)
//    {
//        rec[recoffset].p = m;
//        rec[recoffset].ev = DJY_GetMyEventId();
//        rec[recoffset].who = *st -1;
//        rec[recoffset].tm = (u32)(DJY_GetSysTime()/1000);
//        recoffset++;
//    }
//}
//void recordfree(void * m)
//{
//    u32 i,j;
//    u32 *st;
//    st = __get_PSP() + 9*4;
//    for(i = recoffset-1; i>0; i--)
//    {
//        if(rec[i].p == m)
//        {
//            memcpy(&rec[i],&rec[i+1],sizeof(struct recstack)*(recoffset -i-1));
//            recoffset--;
//        }
//    }
//}
//bool_t pr(char *param)
//{
//    u32 i;
//    for(i = recoffset; i>0; i--)
//        printf("time = %10d; p = %8x; ev = %2d; call = %8x;\r\n", rec[i].tm,rec[i].p,
//                    rec[i].ev, rec[i].who);
//}

void *__Heap_MallocHeap(ptu32_t size,struct HeapCB *Heap, u32 timeout)
{
    struct HeapCession *Cession;
    void *ua_address;
    ufast_t  uf_grade_th;
    uint16_t    *pl_id,id;
    bool_t   en_scheduler;
    void *result;

    if( (size == 0) || (Heap == NULL) )
        return NULL;
    //�����ڴ�ֱ���ж�size�Ƿ�����,��Ϊ��ȡ�û�����ǰ���ܷ����л����ж���Ч.
    if(Lock_MutexPend(&Heap->HeapMutex,timeout) == false)
        return NULL;
    en_scheduler = DJY_QuerySch();
    //��ֹ���������£������ǰû���㹻�Ŀ����ڴ棬�ԹԵ��߰ɡ�
    if((__Heap_GetMaxFreeBlockHeap(Heap) < size)
                && ((timeout == 0) || !en_scheduler))
    {
        result = NULL;
    }else
    {
        if( ! __Heap_CheckMemory(size,Heap,timeout))
        {
            result = NULL;
        }else
        {
            uf_grade_th=__Heap_GetFreeGrade(size,Heap,&Cession);    //ȡ�׺�
            ua_address=__Heap_MallocBlock(uf_grade_th,Cession);     //�����ڴ�
            g_ptEventRunning->HeapSize += Cession->PageSize * (1<<uf_grade_th);
            if(g_ptEventRunning->HeapSize > g_ptEventRunning->HeapSizeMax)
                g_ptEventRunning->HeapSizeMax = g_ptEventRunning->HeapSize;

            //������id���м�¼���η��������
            //�Ķ����δ������� struct HeapCession ��index_event_id��Ա�����ע��.
            pl_id = &Cession->index_event_id
                    [(ptu32_t)((u8*)ua_address-Cession->heap_bottom)
                            /Cession->PageSize];
            id = g_ptEventRunning->event_id;
//          if(0==uf_grade_th)
//              *pl_id = CN_MEM_SINGLE_PAGE_GLOBAL;
//          else
//          {
//              *pl_id++ = CN_MEM_MANY_PAGE_GLOBAL;
//              *pl_id = uf_grade_th;
//          }

            if(uf_grade_th==0)
            {//����1ҳ
                *pl_id = id + CN_EVTT_ID_MASK;
            }else if(uf_grade_th==1)
            {//����2ҳ
                *pl_id++ = CN_MEM_DOUBLE_PAGE_GLOBAL;
                *pl_id = id;
            }else
            {   //�����ҳ
                *pl_id++ = CN_MEM_MANY_PAGE_GLOBAL;
                *pl_id++ = id;
                *pl_id = uf_grade_th;
            }

            result = ua_address;
        }
    }
    Lock_MutexPost(&Heap->HeapMutex);
    __Heap_CheckSTackSync( );
//  if(result != NULL) recordwho(result);
    return result;
}
void *__Heap_Malloc(ptu32_t size,u32 timeout)
{
    return (__Heap_MallocHeap(size,tg_pSysHeap,timeout));
}

//----���stack�ȴ�����--------------------------------------------------------
//���ܣ������heap�Ļ����������ڼ䴴���̣߳��ͻᵼ���¼��ҵ� s_ptStackSync ����
//      �У�post����������Ҫ���ö��У������ͷ������������ڲ����á�
//��������
//���أ���
//-----------------------------------------------------------------------------
void __Heap_CheckSTackSync(void)
{
    struct EventECB *event = s_ptStackSync,*next_sync;
    atom_low_t atom_low;
    atom_low = Int_LowAtomStart();
    if(event != NULL)
    {
        next_sync = event;
        do
        {
            next_sync = event->multi_next;
            event->wakeup_from = CN_STS_WAIT_MEMORY + CN_STS_EVENT_NORUN;
            event->event_status = CN_STS_EVENT_READY;
            __DJY_EventReady(event);
            event = next_sync;
        }while(next_sync != s_ptStackSync);
        s_ptStackSync = NULL;
    }
    Int_LowAtomEnd(atom_low);
    return;
}

//----���¼�����ȴ�����-------------------------------------------------------
//����: ���¼�ֱ�ӷŽ��ȴ�����,������.�ȴ�������һ����������ڴ��С�������е�
//      ˫��ѭ������.���ڴ�ߴ���������������ȼ��Ŷӣ��������㿼��:1���������
//      �̶���������2����Ӧ�ó�������У�һ�㲻���ڷǳ����������ȼ��ǳ��ߵ�
//      �¼�������ʹ�ö�̬���䷽����
//      ������ֻ�ṩ��djyos.c�д����߳�ʱʹ�ã���ͨ�öѲ���
//����: event,������ȴ����е��¼�ָ��
//����: ��
//���¼�¼:
// 1.����: 2015/4/25
//   ˵��: ��Ϊ���¼��Ž�ͨ�öѵȴ�������
//   ����: ������
//-----------------------------------------------------------------------------
void __Heap_WaitMemoryStack(struct EventECB *event,u32 size)
{
    struct EventECB *pl_event;


    event->wait_mem_size = size;
    pl_event = s_ptGenMemSync;     //��ȡ�ڴ�ȴ���ָ��
    event->sync_head = &s_ptGenMemSync;
    __DJY_CutReadyEvent(event);
    if(pl_event == NULL)            //�ȴ����п�
    {
        event->next = NULL;
        event->previous = NULL;

        event->multi_next = event;
        event->multi_previous = event;

        s_ptGenMemSync = event;
    }else
    {
        do
        {//��ѭ���ҵ���һ�������ڴ�������¼����¼�.
            if(pl_event->wait_mem_size < size)
                pl_event = pl_event->multi_next;
            else
                break;
        }while(pl_event != s_ptGenMemSync);
        //���û���ҵ������ڴ�����¼������¼�,���¼��������β,������
        //β������event_wait��ǰ��,��ʱeventǡ�õ���event_wait
        //����ҵ�ʣ����ʱʱ�䳤�����¼����¼�,���¼�������¼�ǰ��.
        //��������ǰ��ѭ���ҵ����,����ʹ�����д���
        event->multi_next = pl_event;
        event->multi_previous = pl_event->multi_previous;
        pl_event->multi_previous->multi_next = event;
        pl_event->multi_previous = event;
    }
    event->event_status = CN_STS_WAIT_MEMORY + CN_STS_EVENT_NORUN;
    return;
}

//----�����߳�ջ---------------------------------------------------------------
//���ܣ��� malloc ���ƣ���Ҫ���ڹ��첽�źŵ������µ��ã����������ṩ������
//      �̺߳���__CreateThreadʹ��
//������size,��������ڴ��ߴ�
//���أ�������ڴ�ָ�룬NULL��ʾû���ڴ���Է���
//���¼�¼:
// 1.����: 2015/4/25
//   ˵��: �Ĵ�tg_pSysHeap���з���Ϊ������ͨ�ö��з���,���ȴ�tg_pSysHeap�з��䡣
//   ����: ������
//-----------------------------------------------------------------------------
extern void __Heap_WaitMemoryStack(struct EventECB *event,u32 size);
bool_t __DJY_RaiseTempPrioForStack(u16 event_id);
void __DJY_AddToBlockForStack(struct EventECB **Head,bool_t Qsort,u32 Status);
void *__Heap_MallocStack(struct EventECB *event, u32 size)
{
    struct HeapCession *Cession;
    void *ua_address;
    ufast_t  uf_grade_th;
    u16    *pl_id,id;
    void *result;

    if( (size == 0) || (tg_pSysHeap == NULL))
        return NULL;

    //�˴��Ѿ����˵��ȣ����ᱻ��ռ��
    if(__Heap_GetMaxFreeBlockHeap(tg_pSysHeap) < size)
    {
        __Heap_WaitMemoryStack(event,size);
        result = NULL;
    }
    else
    {
        if(Lock_MutexQuery(&tg_pSysHeap->HeapMutex) == false)
        {
            __DJY_RaiseTempPrioForStack(tg_pSysHeap->HeapMutex.owner->event_id);
            __DJY_AddToBlockForStack(&s_ptStackSync,CN_BLOCK_PRIO,CN_STS_WAIT_MUTEX);
            result = NULL;
        }
        else
        {
            uf_grade_th=__Heap_GetFreeGrade(size,tg_pSysHeap,&Cession);    //ȡ�׺�
            ua_address=__Heap_MallocBlock(uf_grade_th,Cession);     //�����ڴ�
            event->HeapSize += Cession->PageSize * (1<<uf_grade_th);
            if(event->HeapSize > event->HeapSizeMax)
                event->HeapSizeMax = event->HeapSize;
            //������id���м�¼���η��������
            //�Ķ����δ�������mem_global_t��index_event_id��Ա�����ע��.
            pl_id = &Cession->index_event_id
                    [(ptu32_t)((u8*)ua_address-Cession->heap_bottom)
                              /Cession->PageSize];
            id = event->event_id;
//          if(0==uf_grade_th)
//              *pl_id = CN_MEM_SINGLE_PAGE_GLOBAL;
//          else
//          {
//              *pl_id++ = CN_MEM_MANY_PAGE_GLOBAL;
//              *pl_id = uf_grade_th;
//          }
            if(uf_grade_th==0)
            {//����1ҳ
                *pl_id = id + CN_EVTT_ID_MASK;
            }else if(uf_grade_th==1)
            {//����2ҳ
                *pl_id++ = CN_MEM_DOUBLE_PAGE_GLOBAL;
                *pl_id = id;
            }else
            {   //�����ҳ
                *pl_id++ = CN_MEM_MANY_PAGE_GLOBAL;
                *pl_id++ = id;
                *pl_id = uf_grade_th;
            }

            result = ua_address;
        }
    }
//    save11(result);     //lst debug
    return result;
}

//----����1���ڴ�--------------------------------------------------------------
//����: ���ڴ���з���һ���ڴ��,�ڲ�����,�����Ÿ��û�
//����: grade,������Ŀ�Ľ׺�,0=1��,1=2��,����֮
//      Cession����Cession���з���
//����: ��õ��ڴ��ָ��,������ܷ����򷵻�NULL
//-----------------------------------------------------------------------------
void *__Heap_MallocBlock(ufast_t grade,struct HeapCession *Cession)
{
    ptu32_t ua_temp1;
    ptu32_t ua_pages_number;
    ufast_t uf_grade_th,uf_classes;
    u32 u32WordShift;
    ufast_t *pl_classes;
    ucpu_t  ***pppl_bitmap;
    ucpu_t  **ppl_bitmap;
    ucpu_t  *pl_bitmap;

    ptu32_t ua_word_offset,ua_bit_num,ua_loop;
    ucpu_t  uc_msk;               //����ƫ����

    pppl_bitmap=Cession->ppp_bitmap; //���н�����λͼָ���ָ������ָ��
    ppl_bitmap=pppl_bitmap[grade];        //�ý׿��н�����λͼָ������ַ
    pl_classes=Cession->p_classes;   //��ȡ���׿��н�����������ָ��.
    uf_classes=pl_classes[grade]-1;       //�ý׿��н��������һ��ָ����
                                          //ppl_bitmap�е�ƫ����
    Cession->free_pages_num -= (1<<grade); //ʣ�����ҳ��
    ua_pages_number=0;
    do
    {//��ѭ�����ҵ�һ���ɷ����(��bit=0)��ƫ��λ��,�Ӹߵ��Ͳ���,����:
     //����,�ҳ��ý׿���λͼ��������߼�λͼ�е�һ��bit=0��λƫ����.
     //Ȼ��,�������ƫ��������θ߼���0λ����ƫ��,�������ֲ��ҳ�bit=0��λƫ��.
     //��˷��������ҵ����һ����һ��0λ��ƫ��,����Ǳ��η����Ŀ���.

        pl_bitmap=ppl_bitmap[uf_classes];   //��ȡ����λͼ��ָ��

        //ua_pages_number����һ����λƫ��,ua_temp1�Ǳ������һ�ֵ�����λƫ��
        ua_temp1=CN_CPU_BITS-1-__Heap_LeadingUcpuZero(~pl_bitmap[ua_pages_number]);

        //��һ������ƫ�ơ��ֳ�+��������λƫ�Ƶõ�������λƫ��
        ua_pages_number =(ua_pages_number<<CN_CPU_BITS_SUFFIX_ZERO)+ua_temp1;
    }while(uf_classes-- >0);

    ua_pages_number <<= grade;  //Ŀ������ҳ��ҳ��

    //���濪ʼ��bitmap���Ǳ��η������
    for(uf_grade_th=0;uf_grade_th<=grade;uf_grade_th++)
    {//��0��ֱ����ǰ��(������),����׺ŵ��ڵ���������ڴ��λͼ.
        //�����ʼҳ�����ƽ׺ż��õ��ڸý�0��λͼ�е�ƫ��
        ua_word_offset = ua_pages_number>>uf_grade_th;
        ppl_bitmap=pppl_bitmap[uf_grade_th];    //ȡ�øý׷����ָ������ַ
        ua_bit_num=1<<(grade-uf_grade_th);      //��������λ��
        for(ua_temp1=0;ua_temp1<pl_classes[uf_grade_th];ua_temp1++)
        {//�ӵͼ����߼������ý׸���λͼ
            pl_bitmap=ppl_bitmap[ua_temp1]; //��Ӧ����Ӧ·���������ַ
            //��������λƫ��,������ʽ��ȡģ������
            u32WordShift=ua_word_offset&cn_low_xbit_msk[CN_CPU_BITS_SUFFIX_ZERO];
            ua_word_offset=ua_word_offset>>CN_CPU_BITS_SUFFIX_ZERO;  //��ƫ��
            if(ua_bit_num>=CN_CPU_BITS)   //ua_bit_num���������Ҫ��1��λ
            {//λ�����ڻ����1���ֿ�,��Ϊ����2��n�η��ֽ�,����Ҫ�����λ���϶�
             //����������,������������1�Ϳ�����.

                //��������,Ҳ����һ��λͼ������1��λ��
                ua_bit_num >>=CN_CPU_BITS_SUFFIX_ZERO;
                for(ua_loop =0; ua_loop < ua_bit_num; ua_loop++)
                    pl_bitmap[ua_word_offset+ua_loop]=CN_ALLBIT_1;
            }else if(ua_bit_num>0)
            //λ��С��һ���ֿ�,�Ҵ���0
            {
                uc_msk=cn_low_xbit_msk[ua_bit_num]<<u32WordShift;
                pl_bitmap[ua_word_offset] |=uc_msk;
                if(pl_bitmap[ua_word_offset] == CN_ALLBIT_1)
                //����1��λ���ڵ��ֱ�Ϊȫ1ʱ�����ֶ�Ӧ����һ��λͼ��Ӧ��λ����1
                    ua_bit_num=0;
                else
                    break;
            }else
            {//ǰһ�β���ʹһ���ִӷ�ȫ1���ȫ1����λ���ֶ�Ӧ����һ��·����Ӧλ
                pl_bitmap[ua_word_offset] |= 1<<u32WordShift;
                if(pl_bitmap[ua_word_offset] == CN_ALLBIT_1)
                {
                //����1��λ���ڵ��ֱ�Ϊȫ1ʱ�����ֶ�Ӧ����һ��λͼ��Ӧ��λ����1
                }
                else
                    break;
            }
        }
    }

    for(; uf_grade_th < Cession->uf_grades; uf_grade_th++)
    {//��ǰ��(����)����߽�,����׺Ÿ��ڻ���ڵ���������ڴ��λͼ.ÿ��ֻ��1λ��1

        //�����ʼҳ�����ƽ׺ż��õ��ڸý�0��λͼ�е�ƫ��
        ua_word_offset = ua_pages_number >> uf_grade_th;
        if(ua_word_offset >= Cession->ua_pages_num>>uf_grade_th)
        //�ý��޶�Ӧ��,���ǿ��ܵ�,����һ����10ҳ���ڴ�,��9ҳ�͵�10ҳ���ڵ�3����û��
        //ӳ��.����ʽ�ұ��Ǳ����ܿ���,����Ǵ�0��Ƶ�ƫ����,���ֻ�����ܿ���-1
            break;
        ppl_bitmap=pppl_bitmap[uf_grade_th]; //ȡ�øý׷����ָ������ַ

        for(ua_temp1=0;ua_temp1<pl_classes[uf_grade_th];ua_temp1++)
        {
            pl_bitmap = ppl_bitmap[ua_temp1];
            //��������λƫ��,������ʽ��ȡģ������
            u32WordShift=ua_word_offset&cn_low_xbit_msk[CN_CPU_BITS_SUFFIX_ZERO];
            ua_word_offset=ua_word_offset>>CN_CPU_BITS_SUFFIX_ZERO;  //��ƫ��
            pl_bitmap[ua_word_offset] |= 1<<u32WordShift;
            if(pl_bitmap[ua_word_offset] == CN_ALLBIT_1)
            {
            //����ģ��λ���ڵ��ֱ�Ϊȫ1ʱ�����ֶ�Ӧ����һ��λͼ��Ӧ��λ����1
            }
            else
                break;
        }
    }

    //ˢ���������ڴ��
    if(Cession->free_pages_num != 0)
    {
        uf_grade_th=Cession->uf_grades-1;     //��߽׺�
        do{
            uf_classes= pl_classes[uf_grade_th];        //��uf_grade_th���ܼ���
            ppl_bitmap   = pppl_bitmap[uf_grade_th];    //λͼ������ָ��
            pl_bitmap    = ppl_bitmap[uf_classes-1];    //��߼�λͼָ��
            if(*pl_bitmap != (u32)(~0))
            {//·������ֻҪ����ȫ1�ͱ�ʾ�ý׿϶��п��п�.
                //���ݽ׺ż��������п�ĳߴ�.
                Cession->ua_free_block_max=Cession->PageSize<<uf_grade_th;
                break;
            }
        }while(uf_grade_th-- >0);   //����߽�(����)ɨ�赽0��
    }
    else
        Cession->ua_free_block_max = 0;
    return Cession->heap_bottom
                    +(ua_pages_number * Cession->PageSize);
}

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
void *__Heap_Realloc(void *p, ptu32_t NewSize)
{
    ptu32_t OldSize;
    void *NewP = NULL;

    if(NewSize == 0)
    {
        __Heap_Free(p);
        return NULL;
    }

    if(p == NULL)
    {
        //��NewSize = 0,����NULL
        NewP = __Heap_Malloc(NewSize,0);
        return NewP;
    }

    OldSize = __Heap_CheckSize(p);
    if((NewSize > (OldSize>>1)) && (NewSize <= OldSize))
        return p;
    else
    {
        NewP = __Heap_Malloc(NewSize,0);
        if(NewP)
        {
            memcpy(NewP,p,NewSize < OldSize ? NewSize:OldSize);
        }
        __Heap_Free(p);
    }
    return NewP;
}

//----��ȡ��ʽ���ߴ�-----------------------------------------------------------
//����: ��ȻӦ�ó���������������ߴ��ڴ棬���ڴ������������û����ڴ�ȴ������
//      �û�����Ĺ�񻯳ߴ���ڴ�飬�����������㣬������������size�ֽ��ڴ棬
//      ϵͳ�����������ڴ���ʵ�ʳߴ磬��������ִ�з����ڴ�Ĳ�����Ҳ����֤��
//      ��ķ�������ܷ�ɹ���
//����: size��Ӧ�ó���������ĳߴ�
//      Heap, Ŀ���,���ö���ͨ�ö�,�����ȼ��ö�
//����: ��񻯳ߴ�
//------------
//���¼�¼:
// 1.����: 2015/3/16
//   ˵��: ���ӶԲ���size����ֵ�ж�
//   ����: ������
//-----------------------------------------------------------------------------
ptu32_t __Heap_FormatSizeHeapIn(ptu32_t size,struct HeapCB *Heap)
{
    ucpu_t  grade;
    struct HeapCession *Cession;
    ptu32_t tsize,fsize = CN_LIMIT_PTU32;
    u32 pages;

    Cession = Heap->Cession;
    while(Cession != NULL)
    {
        pages = (size + Cession->PageSize -1)/Cession->PageSize;
        grade = __Heap_LeadingZero((u8 *)&pages,sizeof(u32));
        if((CN_HIGH_1BIT_1>>grade) != pages)
        //���grade���ǹ�񻯵���,���������������С������С
        //����ά��size�Ĵ�С,��������
            grade--;
        grade = CN_PTR_BITS-1-grade;
        tsize = Cession->PageSize << grade;
        if( (tsize < fsize) && (tsize <= Cession->ua_block_max) )
        {
            fsize = tsize;
        }
        Cession = Cession->Next;
    }
    return fsize;
}
ptu32_t __Heap_FormatSizeHeap(ptu32_t size,struct HeapCB *Heap)
{
    struct HeapCB *Work;
    ptu32_t result;

    if((0 == size) || (Heap == NULL))
        return (0);
    if((Heap->HeapProperty & CN_HEAP_PRIVATE) == CN_HEAP_GENERAL)
    {
        Work = Heap;
        do
        {
            if((Work->HeapProperty & CN_HEAP_PRIVATE) == CN_HEAP_GENERAL )
            {
                result = __Heap_FormatSizeHeapIn(size,Work);
                if(result != 0)
                    break;
            }
            Work = Work->NextHeap;
        }while(Work != Heap);

    }
    else
    {
        result = __Heap_FormatSizeHeapIn(size,Heap);
    }
    return result;
}
ptu32_t __Heap_FormatSize(ptu32_t size)
{
    if(tg_pSysHeap != NULL)
        return __Heap_FormatSizeHeapIn(size,tg_pSysHeap);
    else
        return 0;
}

//----�ͷŴӶѷ�����ڴ�-------------------------------------------------------
//����: �ͷŴӶ��з����һ���ڴ棬�����ʱ��̬���仹û�г�ʼ������ִ��׼��̬
//      �ڴ��ͷš��ͷ��ڴ�����¼��������п�ĳߴ磬������¼����ڴ�ͬ��
//      �����У���ѵȴ����ڴ�ߴ�С���µ������п�ߴ���¼����
//������pl_mem,���ͷŵ��ڴ�ָ��
//      Heap,���ͷŵ��ڴ����ڵĶѡ������˽�ж�,��ֱ��ʹ��.
//           �����ͨ�ö�,�������ѿ�ʼ������ͨ�ö��в���pl_mem���ڵĶ�
//���أ����󷵻�flase,��ȷʱ����true
//-----------------------------------------------------------------------------
extern struct EventECB g_tECB_Table[];
void __Heap_FreeHeap(void * pl_mem,struct HeapCB *Heap)
{
    struct HeapCession *Cession;
    struct HeapCB *CurHeap;
    struct EventECB **MemSyncHead;
    ptu32_t ua_temp1;
    ptu32_t ua_word_offset;
    u32 u32WordShift;          //����ƫ����
    ptu32_t ua_pages_no;
    ufast_t uf_grade_th,uf_free_grade_th;
    ufast_t uf_classes;
    ufast_t *pl_classes;
    ucpu_t  ***pppl_bitmap;
    ucpu_t  **ppl_bitmap;
    ucpu_t  *pl_bitmap;
    uint16_t *pl_id;                //id;
    struct EventECB  *event;
    ptu32_t ua_bit_num;
    ucpu_t  uc_msk;
    bool_t Lc;

    u8    event_mem_stage = 0;

    if( (pl_mem == NULL) || (Heap == NULL) )
        return;

    if( (Heap->HeapProperty & CN_HEAP_PRIVATE) == CN_HEAP_PRIVATE)
    {
        CurHeap = Heap;
        MemSyncHead = &Heap->mem_sync;      //ʹ��ר�öѵ�ͬ��ָ��
        Cession = CurHeap->Cession;
        //����ѭ���ҳ����ͷŵ��ڴ����ĸ�Cession��
        while(Cession != NULL)
        {
            if(((u8*)pl_mem < Cession->heap_top) && ((u8*)pl_mem >= Cession->heap_bottom) )
                break ;
            Cession = Cession->Next;
        }
    }
    else
    {
        MemSyncHead = &s_ptGenMemSync;         //ʹ��ͨ�öѵ�ͬ��ָ��
        CurHeap = Heap;
        do
        {
            if((CurHeap->HeapProperty & CN_HEAP_PRIVATE) == CN_HEAP_GENERAL )
            {
                Cession = CurHeap->Cession;
                //����ѭ���ҳ����ͷŵ��ڴ����ĸ�Cession��
                while(Cession != NULL)
                {
                    if(((u8*)pl_mem < Cession->heap_top) && ((u8*)pl_mem >= Cession->heap_bottom) )
                        break ;
                    Cession = Cession->Next;
                }
            }
            if(Cession == NULL)
                CurHeap = CurHeap->NextHeap;
            else
                break;
        }while(CurHeap != Heap);
    }


    if(Cession == NULL)
    {
        DJY_SaveLastError(EN_MEM_ERROR);  //���ͷŵ��ڴ治���ڴ����,ֱ���˳�.
        return;
    }

    ua_temp1 = (ptu32_t)((ptu32_t)pl_mem-(ptu32_t)Cession->heap_bottom);
    //��鱻�ͷ��ڴ��Ƿ�ҳ�߽����
    if(ua_temp1 % Cession->PageSize != 0)
    {
        DJY_SaveLastError(EN_MEM_ERROR);  //���ͷŵ��ڴ��ҳ�߽����,ֱ���˳�.
        return;
    }
    //�����ͷŵ��ڴ�����ҳҳ��
    ua_pages_no=ua_temp1 / Cession->PageSize;

    Lock_MutexPend(&(CurHeap->HeapMutex),CN_TIMEOUT_FOREVER);
    //�����ͷŵ��ڴ��Ľ׺�,��0���.ͨ���׺�Ҳ����ȷ���ڴ��Ĵ�С.
    //ȷ���ڴ�������,�ֲ��ڴ���Ҫ֪��ӵ�и��ڴ���¼�id��
    //ȫ���ڴ����������ڴ������ٱ�,����֪��ӵ�и��ڴ���¼�id
    pl_id = &Cession->index_event_id[ua_pages_no];
    switch(pl_id[0])
    {
        case CN_MEM_DOUBLE_PAGE_LOCAL :
        {   //˫ҳ�ֲ��ڴ�,CN_MEM_DOUBLE_PAGE_LOCAL + event id
            event = &g_tECB_Table[pl_id[1]];
            pl_id[1] = CN_MEM_FREE_PAGE;
            uf_free_grade_th = 1;
            Lc = true;
        }break;
        case CN_MEM_MANY_PAGE_LOCAL :
        {   //��ҳ�ֲ��ڴ�:CN_MEM_MANY_PAGE_LOCAL+event id+�׺�
            event = &g_tECB_Table[pl_id[1]];
            uf_free_grade_th = (ufast_t)pl_id[2];
            pl_id[1] = CN_MEM_FREE_PAGE;
            pl_id[2] = CN_MEM_FREE_PAGE;
            Lc = true;
        }break;
//      case CN_MEM_SINGLE_PAGE_GLOBAL :
//      {   //��ҳȫ���ڴ�:-3
//          uf_free_grade_th = 0;
//      }break;
        case CN_MEM_DOUBLE_PAGE_GLOBAL :
        {//˫ҳȫ���ڴ�:CN_MEM_DOUBLE_PAGE_GLOBAL + event id
            event = &g_tECB_Table[pl_id[1]];
            pl_id[1] = CN_MEM_FREE_PAGE;
            uf_free_grade_th = 1;
            Lc = false;
        }break;
        case CN_MEM_MANY_PAGE_GLOBAL :
        {//��ҳȫ���ڴ�:CN_MEM_MANY_PAGE_GLOBAL+(event id)+�׺�.
            event = &g_tECB_Table[pl_id[1]];
            uf_free_grade_th = (ufast_t)pl_id[2];
            pl_id[1] = CN_MEM_FREE_PAGE;
            pl_id[2] = CN_MEM_FREE_PAGE;
            Lc = false;
        }break;
        default :
        {
            //��ҳ�ֲ��ڴ�:event id
            //��ҳȫ���ڴ�:event id + CN_EVTT_ID_MASK
//          pl_id[0] = CN_MEM_FREE_PAGE;
            uf_free_grade_th = 0;
            event = &g_tECB_Table[pl_id[0] & (~CN_EVTT_ID_MASK)];
            //����¼��ż�¼�Ƿ�Ϸ�
            if((pl_id[0] & (~CN_EVTT_ID_MASK)) >= CFG_EVENT_LIMIT)
            {
//              pl_id[0] = CN_MEM_FREE_PAGE;
                DJY_SaveLastError(EN_MEM_ERROR);   //ָ���д�,ֱ���˳�
                Lock_MutexPost(&(CurHeap->HeapMutex) );
                __Heap_CheckSTackSync( );
                return;
            }
            if(pl_id[0] & CN_EVTT_ID_MASK)      //����Ǿֲ��ڴ滹��ȫ���ڴ�
                Lc = false;
            else
                Lc = true;
        }break;
    }
    ua_temp1 = Cession->PageSize * (1 << uf_free_grade_th);
//  if(g_ptEventRunning->HeapSize > ua_temp1)   //�������������ڲ�����
    {
        event->HeapSize -= ua_temp1;
    }
    pl_id[0] = CN_MEM_FREE_PAGE;
    pppl_bitmap=Cession->ppp_bitmap; //���н�����λͼָ���ָ������ָ��
//    ppl_bitmap=pppl_bitmap[uf_free_grade_th];//�ý׿��н�����λͼָ������ַ
//    pl_bitmap=ppl_bitmap[0];                 //���н�����λͼָ��
    pl_classes=Cession->p_classes;    //��ȡ���׿��н�����������ָ��.
    uf_classes=pl_classes[uf_free_grade_th]-1;  //�ý׿��н��������һ��ָ����
                                                //ppl_bitmap�е�ƫ����

    //����λƫ��
//    ua_word_offset =ua_pages_no>>uf_free_grade_th;
    //��������λƫ��,������ʽ��ȡģ������
//    u32WordShift=ua_word_offset&cn_low_xbit_msk[CN_CPU_BITS_SUFFIX_ZERO];
//    ua_word_offset=ua_word_offset>>CN_CPU_BITS_SUFFIX_ZERO;  //������ƫ��

    //����ҳ������
    Cession->free_pages_num += 1<<uf_free_grade_th;

    //���濪ʼ��bitmap���Ǳ����ͷ����
    for(uf_grade_th = 0; uf_grade_th <= uf_free_grade_th; uf_grade_th++)
    {//��0��ֱ����ǰ��(������),����׺ŵ��ڵ���������ڴ��λͼ.
        //�����ʼҳ�����ƽ׺ż��õ��ڸý�0��λͼ�е�ƫ��
        ua_word_offset =ua_pages_no>>uf_grade_th;
        ppl_bitmap=pppl_bitmap[uf_grade_th];        //ȡ�øü������ָ������ַ
        ua_bit_num=1<<(uf_free_grade_th-uf_grade_th);       //��������λ��
        for(ua_temp1=0;ua_temp1<pl_classes[uf_grade_th];ua_temp1++)
        {//�ӵͼ����߼������ý׸���λͼ
            pl_bitmap = ppl_bitmap[ua_temp1]; //��Ӧ����Ӧ·���������ַ
            //��������λƫ��,������ʽ��ȡģ������
            u32WordShift=ua_word_offset&cn_low_xbit_msk[CN_CPU_BITS_SUFFIX_ZERO];
            ua_word_offset = ua_word_offset>>CN_CPU_BITS_SUFFIX_ZERO;  //��ƫ��
            if(ua_bit_num>=CN_CPU_BITS)   //ua_bit_num���������Ҫ��0��λ
            {//λ�����ڻ����1���ֿ�,��Ϊ����2��n�η��ּ�,����Ҫ�����λ���϶�
             //����������,������������0�Ϳ�����.

                //��������,Ҳ����һ��λͼ������0��λ��
                ua_bit_num >>=CN_CPU_BITS_SUFFIX_ZERO;
                //uc_word_shift�����ﵱ��ʱ����ʹ��
                for(u32WordShift =0; u32WordShift < ua_bit_num; u32WordShift++)
                    pl_bitmap[ua_word_offset+u32WordShift]=0;
            }else if(ua_bit_num > 0)
            {//λ��С��һ���ֿ�,�Ҵ���0
                uc_msk=~(cn_low_xbit_msk[ua_bit_num]<<u32WordShift);
                pl_bitmap[ua_word_offset] &=uc_msk;
                if(pl_bitmap[ua_word_offset] == uc_msk)
                //����0��λ���ڵ���ȫ1ȫ1->��0�����ֶ�Ӧ����һ��λͼ��Ӧλ��0
                    ua_bit_num=0;
                else
                    break;
            }else
            {//ǰһ�β���ʹһ���ִ�ȫ1->��0�����ֶ�Ӧ����һ��·����Ӧλ��0
                pl_bitmap[ua_word_offset] &= ~(1<<u32WordShift);
                if(pl_bitmap[ua_word_offset] == (u32)~(1<<u32WordShift))
                {
                //����0��λ���ڵ���ȫ1->��0�����ֶ�Ӧ����һ��λͼ��Ӧλ��0
                }
                else
                    break;
            }
        }
    }

    for(;uf_grade_th<Cession->uf_grades;uf_grade_th++)
    {//��ǰ��(����)����߽�,����׺Ÿ��ڻ���ڵ���������ڴ��λͼ.ÿ��ֻ����1λ
        //ȡ��һ�׶�Ӧ���������нϵ͵�ַ��ƫ����
        ua_word_offset = (ua_pages_no >> uf_grade_th)<<1;
        //��������λƫ��,������ʽ��ȡģ������
        u32WordShift=ua_word_offset&cn_low_xbit_msk[CN_CPU_BITS_SUFFIX_ZERO];
        ua_word_offset=ua_word_offset>>CN_CPU_BITS_SUFFIX_ZERO;  //��ƫ��
        ppl_bitmap=pppl_bitmap[uf_grade_th-1]; //ȡ�øü������ָ������ַ
        pl_bitmap = ppl_bitmap[0];
        if( (pl_bitmap[ua_word_offset] & (3<<u32WordShift)))
            break;      //����λͼ�������㣬���ߵĽ�Ҳ��������

        //�����ʼҳ�����ƽ׺ż��õ��ڸý�0��λͼ�е�ƫ��
        ua_word_offset=ua_pages_no>>uf_grade_th;
        if(ua_word_offset>=Cession->ua_pages_num>>uf_grade_th)
        //�ý��޶�Ӧ��,���ǿ��ܵ�,����һ����10ҳ���ڴ�,��9ҳ�͵�10ҳ���ڵ�3����
        //��ӳ��.����ʽ�ұ��Ǳ����ܿ���,����Ǵ�0��Ƶ�ƫ����,���ֻ�����ܿ���-1
            break;
        ppl_bitmap=pppl_bitmap[uf_grade_th]; //ȡ�øü������ָ������ַ

        for(ua_temp1=0;ua_temp1<pl_classes[uf_grade_th];ua_temp1++)
        {
            pl_bitmap = ppl_bitmap[ua_temp1];
            //��������λƫ��,������ʽ��ȡģ������
            u32WordShift=ua_word_offset&cn_low_xbit_msk[CN_CPU_BITS_SUFFIX_ZERO];
            ua_word_offset=ua_word_offset>>CN_CPU_BITS_SUFFIX_ZERO;  //��ƫ��
            pl_bitmap[ua_word_offset] &= ~(1<<u32WordShift);
            if(pl_bitmap[ua_word_offset] == (u32)~(1<<u32WordShift))
            {
            //����0��λ���ڵ���ȫ1->��0�����ֶ�Ӧ����һ��λͼ��Ӧλ��0
            }
            else
                break;
        }
    }

    //ˢ���������ڴ��
    uf_grade_th=Cession->uf_grades-1;     //ȡ��߽׵Ľ׺�
    do{
        uf_classes   = pl_classes[uf_grade_th];     //��uf_grade_th�׵��ܼ���
        ppl_bitmap   = pppl_bitmap[uf_grade_th];    //λͼ������ָ��
        pl_bitmap    = ppl_bitmap[uf_classes-1];    //��߼�λͼָ��
        if(*pl_bitmap != (u32)(~0))
        {//·������ֻҪ����ȫ1�ͱ�ʾ�ý׿϶��п��п�.
            //���ݽ׺ż��������п�ĳߴ�.
            Cession->ua_free_block_max=Cession->PageSize<<uf_grade_th;
            break;
        }
    }while(uf_grade_th-- >0);   //����߽�(����)ɨ�赽0��

    Lock_MutexPost( &(CurHeap->HeapMutex) );
    __Heap_CheckSTackSync( );
    if((g_ptEventRunning->local_memory > 0) && (Lc))
    {
        g_ptEventRunning->local_memory--;
    }
    //���ڴ�ȴ������������ڴ�֮��С�ڵ�ǰ��������ڴ�ļ����¼��ŵ�ready����
    //�ȴ�������˫��ѭ������
    Int_SaveAsynSignal();
    if(*MemSyncHead != NULL)
    {
        event = *MemSyncHead;   //ȡ�ȴ�����ͷ
        ua_temp1 = Cession->ua_free_block_max;
        while(1)
        {
            //��ϸ�����ﲢû�и���event����������Ϊevent�Ƴ��������д������
            //���Ӧ������������һ�䣺
            event = *MemSyncHead;   //ȡ�ȴ�����ͷ
            //ua_word_offset�ڴ�Ȩ����ʱ����ʹ��,��񻯵�ǰ�¼���Ҫ���ڴ�.
            event_mem_stage = __Heap_GetGrade(event->wait_mem_size,Cession);
            ua_word_offset = Cession->PageSize<<(event_mem_stage);
            if(ua_word_offset <= ua_temp1)
            {//�¼��ȴ����ڴ���С���������ڴ��,���¼��ŵ���������
                //ʣ����óߴ���ٵ�ǰ������¼���������ڴ�,������ʵ��ִ������
                ua_temp1 -= ua_word_offset;
                event->sync_head = NULL;
                if(event->event_status & CN_STS_SYNC_TIMEOUT)
                    __DJY_ResumeDelay(event);
                event->event_status = CN_STS_EVENT_READY;
                event->wakeup_from = CN_STS_WAIT_MEMORY;
//                if(event->multi_next == event->multi_previous)
//                {//�����һ���¼�
//                    tg_MCB.mem_sync  = NULL;
//                    break;
//                }else   //bug
                if(event->multi_next == event)
                {//�����һ���¼�
                    *MemSyncHead  = NULL;
                    break;
                }else

                {
                    *MemSyncHead = event->multi_next;
                    event->multi_next->multi_previous = event->multi_previous;
                    event->multi_previous->multi_next = event->multi_next;
                }
                //������    ECB�ǲ�����VM�����û��������Ϊ����VM���ã��ŷ��ڴˣ�
                //�����Ҫ��maked�������Ƴ���
//                if(NULL == event->vm)//����VM���ã���marked����ɾ��
//                {
//                    __Djy_CutEventFromEvttMarked(event);
//                }
//                else//������������ʱ�����ڴ�����õ��µ�
//                {
//                    //Ŀǰ����ʲô��������
//                }
                __DJY_EventReady(event);
            }else
                break;
        }
    }
    Int_RestoreAsynSignal();  //�ָ��ж�״̬
    return;
}

void __Heap_Free(void * pl_mem)
{
    if( tg_pSysHeap == NULL)
        return;
//  recordfree(pl_mem);
    __Heap_FreeHeap(pl_mem,tg_pSysHeap);
    return;
}

ptu32_t __Heap_GetMaxFreeBlockHeapIn(struct HeapCB *Heap)
{
    struct HeapCession *Cession;
    ptu32_t result = 0;
    Cession = Heap->Cession;
    while(Cession != NULL)
    {
        if(result < Cession->ua_free_block_max)
            result = Cession->ua_free_block_max ;
        Cession = Cession->Next;
    }
    return result;
}
//----��ѯ�������ڴ��-------------------------------------------------------
//���ܣ������������ڴ��ĳߴ�.
//������Heap,����ѯ�Ķ�,������Ǹ�ר�ö�,��ֻ��ѯ�����,�����ѯ����ͨ�ö�.
//���أ��ڴ���ֽ���
//-----------------------------------------------------------------------------
ptu32_t __Heap_GetMaxFreeBlockHeap(struct HeapCB *Heap)
{
    struct HeapCB *Work;
    ptu32_t result = 0,temp;
    if(Heap== NULL)
        return 0;
    if((Heap->HeapProperty & CN_HEAP_PRIVATE) == CN_HEAP_PRIVATE)
    {
        result = __Heap_GetMaxFreeBlockHeapIn(Heap);
    }
    else
    {
        Work = Heap;
        do
        {
            if((Work->HeapProperty & CN_HEAP_PRIVATE) == CN_HEAP_GENERAL )
            {
                temp = __Heap_GetMaxFreeBlockHeapIn(Work);
                if(result < temp)
                    result = temp;
            }
            Work = Work->NextHeap;
        }while(Work != Heap);
    }

    return result;
}


//----��ѯ�������ڴ��-------------------------------------------------------
//���ܣ�����ͨ�öѵ��������ڴ��ĳߴ�.
//��������
//���أ��ڴ���ֽ���
//-----------------------------------------------------------------------------
ptu32_t  __Heap_GetMaxFreeBlock(void)
{
    struct HeapCB *Work;
    ptu32_t result = 0,temp;
    Work = tg_pHeapList;
    do
    {
        if((Work->HeapProperty & CN_HEAP_PRIVATE) == CN_HEAP_GENERAL )
        {
            temp = __Heap_GetMaxFreeBlockHeapIn(Work);
            if(result < temp)
                result = temp;
        }
        Work = Work->NextHeap;
    }while(Work != tg_pHeapList);

    return result;
}

ptu32_t  __Heap_GetFreeMemHeapIn(struct HeapCB *Heap)
{
    struct HeapCession *Cession;
    ptu32_t result=0;
    Cession = Heap->Cession;
    while(Cession != NULL)
    {
        result += Cession->free_pages_num * Cession->PageSize;
        Cession = Cession->Next;
    }
    return result;
}

//----��ѯ�����ڴ��С-------------------------------------------------------
//���ܣ�������ָ���ѵĿ����ڴ��ֽ���
//������Heap,����ѯ�Ķ�,������Ǹ�ר�ö�,��ֻ��ѯ�����,�����ѯ����ͨ�ö�.
//���أ������ڴ��ֽ���
//-----------------------------------------------------------------------------
ptu32_t  __Heap_GetFreeMemHeap(struct HeapCB *Heap)
{
    struct HeapCB *Work;
    ptu32_t result = 0;
    if(Heap== NULL)
        return 0;
    if((Heap->HeapProperty & CN_HEAP_PRIVATE) == CN_HEAP_PRIVATE)
    {
        result = __Heap_GetMaxFreeBlockHeapIn(Heap);
    }
    else
    {
        Work = Heap;
        do
        {
            if((Work->HeapProperty & CN_HEAP_PRIVATE) == CN_HEAP_GENERAL )
            {
                result += __Heap_GetFreeMemHeapIn(Heap);
            }
            Work = Work->NextHeap;
        }while(Work != Heap);
    }

    return result;
}

//----��ѯ�ܿ����ڴ��С-------------------------------------------------------
//���ܣ�����ͨ�öѵĿ����ڴ��ֽ���
//��������
//���أ������ڴ��ֽ���
//-----------------------------------------------------------------------------
ptu32_t  __Heap_GetFreeMem(void)
{
    struct HeapCB *Work;
    ptu32_t result = 0;
    Work = tg_pHeapList;
    do
    {
        if((Work->HeapProperty & CN_HEAP_PRIVATE) == CN_HEAP_GENERAL )
        {
            result += __Heap_GetFreeMemHeapIn(Work);
        }
        Work = Work->NextHeap;
    }while(Work != tg_pHeapList);

    return result;
}

#endif

ADD_TO_ROUTINE_SHELL(heapdetail,heap_spy,"��ʾ��̬�ڴ���ϸ�������");
ADD_TO_ROUTINE_SHELL(heap,heap,"��ʾ��ʹ�����");
//ADD_TO_ROUTINE_SHELL(pr,pr,"�����ڴ�й©");

