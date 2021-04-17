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
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <net/osarch.h>
#include "heap.h"
#include "int.h"
#include "align.h"

#ifdef    DEBUG
#define   HEAP_AEESRT(x)   while(!(x));
#else
#define   HEAP_AEESRT(x)
#endif

#define HEAD_LIST_NUM               (12U)
#define BLOCK_ALIGN_SIZE            (0X04U)
//������ÿ���ڵ�ĳߴ磬Ҫ���� block_header_t
static const size_t heap_mem_list[HEAD_LIST_NUM] =
                            {16,32,64,128,256,512,1024,1560,2048,3072,4096,8192};

//�����ͷ�ṹ��
//AddressSortPrev ָ�����ںϲ����ڿ飬�������п鰴�������ڵ�ԭ�򴮽�������
struct block_header_t
{
    struct block_header_t * AddressSortPrev;    //ָ�򰴵�ַ�����ǰһ�飬�����Ƿ����
    size_t size;            //��ߴ磬������ struct block_header_t ����
                            //bit0 ���ڱ�ʾ BLOCK_FREE_BIT
                            //bit1 ���ڱ�ʾ BLOCK_PREV_FREE_BIT
    struct block_header_t * next_free;
    struct block_header_t * prev_free;
}__attribute__((aligned (BLOCK_ALIGN_SIZE)));

#define BLOCK_SIZE_MIN              (heap_mem_list[0] + sizeof(struct block_header_t))
#define BLOCK_SIZE_MAX              (heap_mem_list[HEAD_LIST_NUM-1] + sizeof(struct block_header_t))
#define BLOCK_FREE_BIT       (1U<<0)
#define BLOCK_PREV_FREE_BIT  (1U<<1)

//mem_lst[i]���������� �� heap_mem_list[i] �� �� heap_mem_list[i+1] ���ڴ��
struct PkgHeapCB
{
//  struct block_header_t block_null;
    struct block_header_t *mem_lst[HEAD_LIST_NUM];
    size_t last_addr;       //�ѵ�ĩ��ַ�����һ���ֽڵ�ַ+1��
};


// =============================================================================
// ���ܣ���ȡblock�Ĵ�С
// ������block��block��ָ��
// ���أ���
// =============================================================================
static size_t __BlockGetSize(const struct block_header_t * block)
{
    return block->size & ~(BLOCK_FREE_BIT | BLOCK_PREV_FREE_BIT);
}

// =============================================================================
// ���ܣ�����block�Ĵ�С
// ������block��block��ָ��
// ���أ���
// =============================================================================
static void __BlockSetSize(struct block_header_t * block, size_t size)
{
    const size_t oldsize = block->size;
    block->size = size | (oldsize & (BLOCK_FREE_BIT | BLOCK_PREV_FREE_BIT));
}

// =============================================================================
// ���ܣ����block�Ƿ������һ��
// ������block��block��ָ��
// ���أ���
// =============================================================================
static int BlockIsLast(const struct block_header_t * block)
{
    return (__BlockGetSize(block) == 0);
}

// =============================================================================
// ���ܣ���ȡ�������ڵĺ�һ��block�ĵ�ַ
// ������Block��block�ĵ�ַ
// ���أ���һ��block�ĵ�ַ
// =============================================================================
static struct block_header_t * __BlockPhyNext(struct PkgHeapCB * PkgHeap,
                                    const struct block_header_t * block)
{
    struct block_header_t * next;
    next = (struct block_header_t*)((size_t)(block+1) + __BlockGetSize(block));
    if(!BlockIsLast(block))
    {
        if((size_t)next>=(size_t)(PkgHeap->last_addr))
            return NULL;
        else
            return next;
    }
    else
        printf("--------------------------------------------------\r\n");
    return NULL;
}

// =============================================================================
// ���ܣ����������ڵĺ�һ��block��prev_phys_block�������ǰһ��block�ĵ�ַ
// ������control�����ƿ飬Block��ǰһ��block�ĵ�ַ
// ���أ���һ����������block�ĵ�ַ
// =============================================================================
static struct block_header_t * __BlockLinkNext(struct PkgHeapCB * PkgHeap,
                                        struct block_header_t * block)
{
    struct block_header_t * next = __BlockPhyNext(PkgHeap,block);
    if(next==NULL)
        return NULL;
    next->AddressSortPrev = block;
    return next;
}

// =============================================================================
// ���ܣ���block�ı�־λ��Ϊ���У������ú�һ��Block��prev_phys_block����
// ������PkgHeap������ heap��
//       Block���������� block
// ���أ���
// =============================================================================
static void __BlockMarkAsFree(struct PkgHeapCB * PkgHeap,struct block_header_t * block)
{
    struct block_header_t * next = NULL;
//  __BlockSetFree(block);                  //�ѱ� block ��Ϊ����
    block->size |= BLOCK_FREE_BIT;          //�ѱ� block ��Ϊ����
    next = __BlockLinkNext(PkgHeap,block);  //�� block ���ڵ���һ��� phy prev ָ��ָ���Լ�
    if(next==NULL)
        return;
//  __BlockSetPrevFree(next);                 //���� block ���ڵ���һ��ġ�ǰһ����С���־
    next->size |= BLOCK_PREV_FREE_BIT;       //���� block ���ڵ���һ��ġ�ǰһ����С���־
}

// =============================================================================
// ���ܣ���block�ı�־λ��Ϊæµ�������ú�һ��Block��ǰһ��blockΪæµ��־
// ������control�����ƿ飬Block��block�ĵ�ַ
// ���أ���
// =============================================================================
static void __BlockMarkAsUsed(struct PkgHeapCB * PkgHeap,struct block_header_t * block)
{
    struct block_header_t * next = NULL;
//  BlockSetUsed(block);
    block->size &= ~BLOCK_FREE_BIT;
    block->next_free = block;
    block->prev_free = block;
    next = __BlockPhyNext(PkgHeap,block);
    if(next==NULL)
        return;
//  BlockSetPrevUsed(next);
    next->size &= ~BLOCK_PREV_FREE_BIT;
}

// =============================================================================
// ���ܣ�����sizeֵ��Ӧ�������ĳ����λ(����ʱ��)
// ������size:��Ҫ�Ĵ�С
// ���أ���λ
// =============================================================================
static u16 __BlockFindLevel(size_t size)
{
    u16 i = 0;
    for(i=0;i<HEAD_LIST_NUM;i++)
    {
        if(heap_mem_list[i]>size)
            break;
    }
    i--;
    return i;
}

// =============================================================================
// ���ܣ����� size ֵ�ҵ���Ӧ�����п����ڴ�ĵ�λ
// ������
// ���أ�
// =============================================================================
static u32 __GetLevel(size_t size, size_t align)
{
    u32 scale = 0;
//  u16 i = 0;
    if (size)
    {
        const size_t aligned = align_up(align, size);
//      scale = BlockFindFitLevel(aligned);
        for(scale=0; scale<HEAD_LIST_NUM; scale++)
        {
            if(heap_mem_list[scale] >= aligned)
                break;
        }
    }
    return scale;
}

// =============================================================================
// ���ܣ���block��control���ƿ����Ƴ�
// ������control:���ƿ飬block��block�ĵ�ַ
// ���أ���
// =============================================================================
static void __BlockRemove(struct PkgHeapCB * PkgHeap, struct block_header_t * block)
{
    u16 id = 0;
//  HEAP_AEESRT(!((block->next_free==NULL)&&(block->prev_free!=NULL)));
//  HEAP_AEESRT(!((block->next_free!=NULL)&&(block->prev_free==NULL)));
    if(block->next_free != NULL)
    {
        block->prev_free->next_free = block->next_free;
        block->next_free->prev_free = block->prev_free;
    }
    id = __BlockFindLevel(block->size);
    if(PkgHeap->mem_lst[id]==block)
    {
        if(block->next_free != block)
            PkgHeap->mem_lst[id] = block->next_free;
        else
            PkgHeap->mem_lst[id] = NULL;
    }
    block->prev_free = block;
    block->next_free = block;
}

//=============================================================================
//���ܣ���block���� PkgHeap ��Ӧ�ߴ� level ��������
//������PkgHeap:�����ѣ�
//      block��block�ĵ�ַ
//���أ���
//=============================================================================
static void __BlockInsert(struct PkgHeapCB * PkgHeap, struct block_header_t * block)
{
    struct block_header_t *tmp_block = NULL;
    u16 id = 0;
    id = __BlockFindLevel(block->size);
    tmp_block = PkgHeap->mem_lst[id];
    if(tmp_block==NULL)
    {
        PkgHeap->mem_lst[id] = block;
        block->next_free = block;
        block->prev_free = block;
    }
    else
    {
        tmp_block->prev_free->next_free = block;
        block->next_free = tmp_block;
        block->prev_free = tmp_block->prev_free;
        tmp_block->prev_free = block;
    }
}

// =============================================================================
// ���ܣ����ÿ��Ƿ񻹿��Ա��ָ��ߴ磾 (level ��Ӧ�ĳߴ� + BLOCK_SIZE_MIN)˵��
//      ���ԷָBLOCK_SIZE_MIN=��С��ߴ�+sizeof(struct block_header_t)
// ������
// ���أ�
// =============================================================================
static int __BlockCanSplit(struct block_header_t * block, u32 level)
{
    return __BlockGetSize(block) >= (BLOCK_SIZE_MIN + heap_mem_list[level]);
}

//=============================================================================
//���ܣ��ָ� block��ԭ block ָ�뱣�ֲ��䣬��ߴ籣�� Level ���𡣲����������������
//      ��������һ��� phy prev ָ��ָ���Լ��������䡰ǰһ��ա���־
//������PkgHeap��������
//      block�����ָ�Ŀ飬�����ж�����ڴ��г�ȥ��
//      level��block����Ҫ������ level
//���أ��ָ�����Ŀ�ָ�룬�����ָܷ�򷵻�NULL
//=============================================================================
static struct block_header_t *__BlockSplit(struct PkgHeapCB * PkgHeap,
                                    struct block_header_t * block, u32 level)
{
    size_t size = heap_mem_list[level];
    size_t remain_size;
    struct block_header_t* remaining;
    //�� block �вó� level ����ߴ磬ȡ���ڵ�ַ
    remaining = (struct block_header_t*)((size_t)(block+1) + size);
//  remaining = OffsetToBlock(__BlockToPtr(block), size);

    //�����������ڴ�ߴ磬�����˿����ͷ�ĳߴ�
    remain_size = __BlockGetSize(block) - (size + sizeof(struct block_header_t));

    if(remain_size >= heap_mem_list[0])
    {
        __BlockSetSize(remaining, remain_size);
        __BlockSetSize(block, size);
        __BlockMarkAsFree(PkgHeap,remaining);
        return remaining;
    }
    else
        return NULL;
}

// =============================================================================
// ���ܣ����������ڵ�ǰ������block�ںϣ��ϳ�һ��block������ǰ��ȷ������block�ǿ��е�
// ������PkgHeap��������
//      prev�����ںϵ�����block����ǰ���һ������ַ��С��
//      block�����ںϵ�����block���ź����һ������ַ�ϴ�
// ���أ�
// =============================================================================
static struct block_header_t *__BlockMerge(struct PkgHeapCB * PkgHeap,
                   struct block_header_t * prev, struct block_header_t * block)
{
//  HEAP_AEESRT(!BlockIsLast(prev) && "previous block can't be last!");
    if(!BlockIsLast(prev))
    {
        prev->size += __BlockGetSize(block) + sizeof(struct block_header_t);
        __BlockLinkNext(PkgHeap,prev);
        return prev;
    }
    else
    {
        printf("--------------------------------------------------\r\n");
        return NULL;
    }
}

//=============================================================================
// ���ܣ����block����������ǰ�����ڵ�һ��block�Ƿ���У��������ں�
// ������PkgHeap��������
//       block�������Ŀ�
// ���أ��ںϺ��¿��ַ���������ں��򷵻�NULL
//=============================================================================
static struct block_header_t * __BlockMergePrev(struct PkgHeapCB * PkgHeap,
                                        struct block_header_t * block)
{
    if ((block->size & BLOCK_PREV_FREE_BIT))
    {
        struct block_header_t* prev = block->AddressSortPrev;
        if(prev == NULL)
            return NULL;
//      HEAP_AEESRT(prev && "prev physical block can't be null");
//      if( ! __BlockIsFree(prev))
        if(! (prev->size & BLOCK_FREE_BIT))
            return NULL;
//      HEAP_AEESRT(__BlockIsFree(prev) && "prev block is not free though marked as such");
        __BlockRemove(PkgHeap, prev);
        block = __BlockMerge(PkgHeap,prev, block);
    }

    return block;
}

//=============================================================================
// ���ܣ����block���������Ϻ������ڵ�һ��block�Ƿ���У��������ں�
// ������PkgHeap��������
//       block�������Ŀ�
// ���أ��ںϺ��¿��ַ���������ں��򷵻�NULL
//=============================================================================
static struct block_header_t * __BlockMergeNext(struct PkgHeapCB * PkgHeap,
                                        struct block_header_t * block)
{
    struct block_header_t* next = __BlockPhyNext(PkgHeap,block);
    if(next==NULL)
        return NULL;
//  if (__BlockIsFree(next))
    if((next->size & BLOCK_FREE_BIT))
    {
//      HEAP_AEESRT(!BlockIsLast(block) && "previous block can't be last!");
        if(BlockIsLast(block))
        {
            printf("--------------------------------------------------\r\n");
            return NULL;
        }
        __BlockRemove(PkgHeap, next);
        block = __BlockMerge(PkgHeap,block, next);
    }
    return block;
}

//=============================================================================
//���ܣ��޼�block������Ҫ��� level ������block�Ƿ�ɷָ���ԵĻ�ִ�зָ����
//      �ָ������block���²������ԭ block ָ�뱣�ֲ��䣬��ߴ籣�� Level ����
//������PkgHeap��������
//      block�����ָ�Ŀ飬�����ǿ��еģ������ж�����ڴ��г�ȥ��
//      level��block����Ҫ������ level
//���أ�
//=============================================================================
static void __BlockTrim(struct PkgHeapCB * PkgHeap, struct block_header_t * block,
                            u32 level)
{
    struct block_header_t* remaining_block;
    if (__BlockCanSplit(block, level))
    {
        remaining_block = __BlockSplit(PkgHeap,block, level);
        __BlockLinkNext(PkgHeap,block);
//      __BlockSetPrevFree(remaining_block);
        remaining_block->size |= BLOCK_PREV_FREE_BIT;   //���� block ���ڵ���һ��ġ�ǰһ����С���־
        __BlockInsert(PkgHeap, remaining_block);
    }
}

//=============================================================================
//���ܣ��� PkgHeap ��ȡ��һ��level����� block������� level �Ķ����� NULL �ģ���
//      ȡ��һ���ģ��������ơ����ȡ�������ϼ��飬�����ָ
//������PkgHeap����ָ��
//     level���ߴ�׼�
//���أ�����ƿ�ͷָ��
//=============================================================================
static struct block_header_t * __GetOneBlock(struct PkgHeapCB * PkgHeap, u32 level)
{
    struct block_header_t * block = NULL;
//  HEAP_AEESRT(level<HEAD_LIST_NUM);
    if(!(level<HEAD_LIST_NUM))
        return NULL;
//SEARCH_MEM:
    while(1)
    {
        if(PkgHeap->mem_lst[level] != NULL)
        {
            block = PkgHeap->mem_lst[level];
            if(block->next_free==block)
                PkgHeap->mem_lst[level] = NULL;
            else
            {
                block->next_free->prev_free = block->prev_free;
                block->prev_free->next_free = block->next_free;
                PkgHeap->mem_lst[level] = block->next_free;
            }
            block->next_free = NULL;
            block->prev_free = NULL;
            break;
        }
        else
        {
            level++;
            if(level>=HEAD_LIST_NUM)
                return NULL;
        }
    }
    return block;
}

//------------------------------------------------------------------------------
//���ܣ�����һ���ڴ棬���裺
//      1���� heap_mem_list �����ҵ���sizeƥ�䵵λ level
//      2���� level ��λ������ȡ��һ��block�����޿��У��������ߴ�levelȡ��
//      3����ȡ���size���ܴ������裬�Ѷ��ಿ�ֲü����������¼��������С�
//������PkgHeap��������
//      size����������ڴ�
//���أ����䵽���ڴ�ָ��
//------------------------------------------------------------------------------
void *__PkgMalloc(struct PkgHeapCB *PkgHeap,size_t size)
{
    void *mem = NULL;
    u32 level;
    struct block_header_t * block;
    atom_low_t low;
    if(PkgHeap==NULL || size==0)
        return NULL;
    low = Int_LowAtomStart();
//    mem = __Malloc(PkgHeap,size);
    level = __GetLevel(size, BLOCK_ALIGN_SIZE);
    if(level==HEAD_LIST_NUM)    //size�����飬�޷�����
        mem = NULL;
    else
    {
        block = __GetOneBlock(PkgHeap, level);      //�ӿ���������ȡ��һ�飬���ܱ�����Ĵ�
        if(block != NULL)
        {
            __BlockTrim(PkgHeap, block, level);     //�޼��飬�Ѷ���Ĳ��ֻ���
            __BlockMarkAsUsed(PkgHeap,block);
//          mem = __BlockToPtr(block);
            mem = (void*)(block+1);
        }
        else
            mem = NULL;
    }
    Int_LowAtomEnd(low);
    return mem;
}

void __PkgFree(struct PkgHeapCB *PkgHeap,void * ptr)
{
    atom_low_t low;
    struct block_header_t * block;
    if(PkgHeap==NULL || ptr==NULL)
         return;
    low = Int_LowAtomStart();
//  __Free(PkgHeap,ptr);
//  block = BlockFromPtr(ptr);
    block = (struct block_header_t*)ptr-1;
//  if( ! __BlockIsFree(block))
    if(! (block->size & BLOCK_FREE_BIT))
    {
        __BlockMarkAsFree(PkgHeap,block);
        block = __BlockMergePrev(PkgHeap, block);
        block = __BlockMergeNext(PkgHeap, block);
        __BlockInsert(PkgHeap, block);
    }
    Int_LowAtomEnd(low);
}

struct PkgHeapCB * __PkgMenInit(void *mem,size_t size)
{
    struct block_header_t  *block;
    atom_low_t low;
    struct PkgHeapCB *PkgHeap;
    u16 i=0;
    if(mem==NULL || size<=sizeof(struct block_header_t))
        return NULL;
    PkgHeap =  (struct PkgHeapCB *)net_malloc(sizeof(struct PkgHeapCB) );
    if(PkgHeap == NULL)
        return NULL;
    low = Int_LowAtomStart();
//  __ControlConstruct(PkgHeap);
    for(i=0;i<HEAD_LIST_NUM;i++)
        PkgHeap->mem_lst[i] = NULL;

    block = (struct block_header_t*)align_up(BLOCK_ALIGN_SIZE,(size_t)mem);
//  HEAP_AEESRT((size_t)block<=(size_t)mem);
//  if((size_t)block<(size_t)mem)
//      return false;
    //size Ҫ�۳� sizeof(struct block_header_t)
    block->size = size +  (size_t)block - (size_t)mem - sizeof(struct block_header_t);
    block->AddressSortPrev = NULL;
    block->next_free = block;
    block->prev_free = block;
    PkgHeap->last_addr = block->size +  (size_t)block + sizeof(struct block_header_t);
    __BlockInsert(PkgHeap,block);
//  __BlockSetFree(block);
    block->size |= BLOCK_FREE_BIT;          //�ѱ� block ��Ϊ����
    Int_LowAtomEnd(low);
    return PkgHeap;
}
