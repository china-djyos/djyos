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
#include "heap.h"
#include "int.h"

#ifdef    DEBUG
#define   HEAP_AEESRT(x)   while(!(x));
#else
#define   HEAP_AEESRT(x)
#endif

static const size_t heap_mem_list[HEAD_LIST_NUM] = {16,32,64,128,256,512,1024,1560,2048,3072,4096,8192};

#define BLOCK_HEADER_OFFSET         (sizeof(tagBlockHeader))
#define BLOCK_SIZE_MIN              (heap_mem_list[0] + sizeof(tagBlockHeader))
#define BLOCK_SIZE_MAX              (heap_mem_list[HEAD_LIST_NUM-1] + sizeof(tagBlockHeader))
#define BLOCK_HRADER_FREE_BIT       (1U<<0)
#define BLOCK_HEADER_PREV_FREE_BIT  (1U<<1)

// =============================================================================
// ���ܣ���ȡblock�Ĵ�С
// ������block��block��ָ��
// ���أ���
// =============================================================================
static size_t BlockGetSize(const tagBlockHeader * block)
{
    return block->size & ~(BLOCK_HRADER_FREE_BIT | BLOCK_HEADER_PREV_FREE_BIT);
}

// =============================================================================
// ���ܣ�����block�Ĵ�С
// ������block��block��ָ��
// ���أ���
// =============================================================================
static void BlockSetSize(tagBlockHeader * block, size_t size)
{
    const size_t oldsize = block->size;
    block->size = size | (oldsize & (BLOCK_HRADER_FREE_BIT | BLOCK_HEADER_PREV_FREE_BIT));
}

// =============================================================================
// ���ܣ����block�Ƿ������һ��
// ������block��block��ָ��
// ���أ���
// =============================================================================
static int BlockIsLast(const tagBlockHeader * block)
{
    return (BlockGetSize(block) == 0);
}

// =============================================================================
// ���ܣ����block���Ƿ��ǿ��е�
// ������block��block��ָ��
// ���أ���
// =============================================================================
static int BlockIsFree(const tagBlockHeader * block)
{
    return (int)(block->size & BLOCK_HRADER_FREE_BIT);
}

// =============================================================================
// ���ܣ�����blockΪ����״̬
// ������block��block��ָ��
// ���أ���
// =============================================================================
static void BlockSetFree(tagBlockHeader * block)
{
    block->size |= BLOCK_HRADER_FREE_BIT;
}

// =============================================================================
// ���ܣ�����blockΪ����ʹ��״̬
// ������block��block��ָ��
// ���أ���
// =============================================================================
static void BlockSetUsed(tagBlockHeader * block)
{
    block->size &= ~BLOCK_HRADER_FREE_BIT;
}

// =============================================================================
// ���ܣ����block��ǰһ���������ڵ�blockΪ���б�־λ
// ������block��block��ָ��
// ���أ���
// =============================================================================
static int BlockIsPrevFree(const tagBlockHeader * block)
{
    return (int)(block->size & BLOCK_HEADER_PREV_FREE_BIT);
}

// =============================================================================
// ���ܣ�����block��ǰһ���������ڵ�blockΪ���б�־λ
// ������block��block��ָ��
// ���أ���
// =============================================================================
static void BlockSetPrevFree(tagBlockHeader * block)
{
    block->size |= BLOCK_HEADER_PREV_FREE_BIT;
}

// =============================================================================
// ���ܣ�����block��ǰһ���������ڵ�blockΪæµ��־λ
// ������block��block��ָ��
// ���أ���
// =============================================================================
static void BlockSetPrevUsed(tagBlockHeader * block)
{
    block->size &= ~BLOCK_HEADER_PREV_FREE_BIT;
}

// =============================================================================
// ���ܣ���ptr����ƫ�Ƶõ�block�ĵ�ַ
// ������ptr�������ַ
// ���أ�block�ĵ�ַ
// =============================================================================
static tagBlockHeader * BlockFromPtr(const void * ptr)
{
    return (tagBlockHeader *)(ptr - BLOCK_HEADER_OFFSET);
}

// =============================================================================
// ���ܣ���block����ƫ�Ƶõ�ptr�ĵ�ַ
// ������block��block�ĵ�ַ
// ���أ�ptr:����ĵ�ַ
// =============================================================================
static void * BlockToPtr(const tagBlockHeader * block)
{
    return (void*)((uint8_t*)block + BLOCK_HEADER_OFFSET);
}

// =============================================================================
// ���ܣ���ptr����ƫ����ƫ��ֵ
// ������ptr�������ַ��size:ƫ�Ƶ�ַ
// ���أ�block�ĵ�ַ
// =============================================================================
static tagBlockHeader * OffsetToBlock(const void * ptr, size_t size)
{
    return (tagBlockHeader *)((uint8_t*)ptr + size);
}

// =============================================================================
// ���ܣ���ȡ�������ڵ�ǰһ��block�ĵ�ַ
// ������Block��block�ĵ�ַ
// ���أ�ǰһ��block�ĵ�ַ
// =============================================================================
static tagBlockHeader * BlockPrev(const tagBlockHeader * block)
{
    return block->prev_phys_block;
}

// =============================================================================
// ���ܣ���ȡ�������ڵĺ�һ��block�ĵ�ַ
// ������Block��block�ĵ�ַ
// ���أ���һ��block�ĵ�ַ
// =============================================================================
static tagBlockHeader * BlockNext(tagHeadControl * control,const tagBlockHeader * block)
{
    tagBlockHeader * next = OffsetToBlock(BlockToPtr(block), BlockGetSize(block));
    HEAP_AEESRT(!BlockIsLast(block));
    if((size_t)next>=(size_t)(control->last_addr))
          return NULL;
    return next;
}

// =============================================================================
// ���ܣ����������ڵĺ�һ��block��prev_phys_block�������ǰһ��block�ĵ�ַ
// ������control�����ƿ飬Block��ǰһ��block�ĵ�ַ
// ���أ���һ��block�ĵ�ֵ
// =============================================================================
static tagBlockHeader * BlockLinkNext(tagHeadControl * control,tagBlockHeader * block)
{
    tagBlockHeader * next = BlockNext(control,block);
    if(next==NULL)
        return NULL;
    next->prev_phys_block = block;
    return next;
}

// =============================================================================
// ���ܣ���block�ı�־λ��Ϊ���У������ú�һ��Block��prev_phys_block����
// ������control�����ƿ飬Block��block�ĵ�ַ
// ���أ���
// =============================================================================
static void BlockMarkAsFree(tagHeadControl * control,tagBlockHeader * block)
{
    tagBlockHeader * next = NULL;
    BlockSetFree(block);
    next = BlockLinkNext(control,block);
    if(next==NULL)
        return;
    BlockSetPrevFree(next);
}

// =============================================================================
// ���ܣ���block�ı�־λ��Ϊæµ�������ú�һ��Block��ǰһ��blockΪæµ��־
// ������control�����ƿ飬Block��block�ĵ�ַ
// ���أ���
// =============================================================================
static void BlockMarkAsUsed(tagHeadControl * control,tagBlockHeader * block)
{
    tagBlockHeader * next = NULL;
    BlockSetUsed(block);
    block->next_free = block;
    block->prev_free = block;
    next = BlockNext(control,block);
    if(next==NULL)
        return;
    BlockSetPrevUsed(next);
}

// =============================================================================
// ���ܣ���sizeֵ��Ӧ�������ĳ����λ(����ʱ��)
// ������size:��Ҫ�Ĵ�С
// ���أ���λ
// =============================================================================
static uint16_t BlockFindLevel(size_t size)
{
    uint16_t i = 0;
    for(i=0;i<HEAD_LIST_NUM;i++)
    {
        if(heap_mem_list[i]>size)
            break;
    }
    i--;
    return i;
}

// =============================================================================
// ���ܣ���sizeֵ��Ӧ�������ĳ����λ��ȡ��ʱ�ã�
// ������size:��Ҫ�Ĵ�С
// ���أ���λ
// =============================================================================
static uint16_t BlockFindFitLevel(size_t size)
{
    uint16_t i = 0;
    for(i=0;i<HEAD_LIST_NUM;i++)
    {
        if(heap_mem_list[i]>=size)
            break;
    }
    return i;
}

// =============================================================================
// ���ܣ����϶���
// ������
// ���أ�
// =============================================================================
size_t AlignUp(size_t x, size_t align)
{
    HEAP_AEESRT(0 == (align & (align - 1)) && "must align to a power of two");
    return (x + (align - 1)) & ~(align - 1);
}

// =============================================================================
// ���ܣ����¶���
// ������
// ���أ�
// =============================================================================
size_t AlignDown(size_t x, size_t align)
{
    HEAP_AEESRT(0 == (align & (align - 1)) && "must align to a power of two");
    return x - (x & (align - 1));
}

void * AlignPtr(const void * ptr, size_t align)
{
    const size_t *aligned = (size_t*)(((size_t)ptr + (align - 1)) & ~(align - 1));
    HEAP_AEESRT(0 == (align & (align - 1)) && "must align to a power of two");
    return (void*)aligned;
}

// =============================================================================
// ���ܣ����ҵ�sizeֵ����Ӧ�ĵ�λ
// ������
// ���أ�
// =============================================================================
static uint32_t AdjustRequestSize(size_t size, size_t align)
{
    uint32_t adjust_id = 0;
    if (size)
    {
        const size_t aligned = AlignUp(size, align);
        adjust_id = BlockFindFitLevel(aligned);
    }
    return adjust_id;
}

// =============================================================================
// ���ܣ���block��control���ƿ����Ƴ�
// ������control:���ƿ飬block��block�ĵ�ַ
// ���أ�
// =============================================================================
static void BlockRemove(tagHeadControl * control, tagBlockHeader * block)
{
    uint16_t id = 0;
    HEAP_AEESRT(!((block->next_free==NULL)&&(block->prev_free!=NULL)));
    HEAP_AEESRT(!((block->next_free!=NULL)&&(block->prev_free==NULL)));
    if(block->next_free!=NULL)
    {
        block->prev_free->next_free = block->next_free;
        block->next_free->prev_free = block->prev_free;
    }
    id = BlockFindLevel(block->size);
    if(control->mem_lst[id]==block)
    {
        if(block->next_free!=block)
            control->mem_lst[id] = block->next_free;
        else
            control->mem_lst[id] = NULL;
    }
    block->prev_free = block;
    block->next_free = block;
}

// =============================================================================
// ���ܣ���block����control���ƿ���
// ������control:���ƿ飬block��block�ĵ�ַ
// ���أ�
// =============================================================================
static void BlockInsert(tagHeadControl * control, tagBlockHeader * block)
{
    tagBlockHeader *tmp_block = NULL;
    uint16_t id = 0;
    id = BlockFindLevel(block->size);
    tmp_block = control->mem_lst[id];
    if(tmp_block==NULL)
    {
        control->mem_lst[id] = block;
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
// ���ܣ����ÿ��Ƿ񻹿��Ա��ָ�
// ������
// ���أ�
// =============================================================================
static int BlockCanSplit(tagBlockHeader * block, uint32_t level)
{
    return BlockGetSize(block) >= (BLOCK_SIZE_MIN + heap_mem_list[level]);
}

// =============================================================================
// ���ܣ��ָ�block
// ������
// ���أ�
// =============================================================================
static tagBlockHeader *
BlockSplit(tagHeadControl * control,tagBlockHeader * block, uint32_t level)
{
    size_t size = heap_mem_list[level];
    tagBlockHeader* remaining = OffsetToBlock(BlockToPtr(block), size);
    const size_t remain_size = BlockGetSize(block) - (size + BLOCK_HEADER_OFFSET);

    HEAP_AEESRT(BlockToPtr(remaining) ==
            AlignPtr(BlockToPtr(remaining), BLOCK_ALIGN_SIZE) && "remaining block not aligned properly");
    HEAP_AEESRT(BlockGetSize(block) == remain_size + size + BLOCK_HEADER_OFFSET);
    BlockSetSize(remaining, remain_size);
    HEAP_AEESRT(BlockGetSize(remaining) >= heap_mem_list[0] && "block split with invalid size");

    BlockSetSize(block, size);
    BlockMarkAsFree(control,remaining);

    return remaining;
}

// =============================================================================
// ���ܣ���block����ǰһ��block�ںϣ��ϳ�һ��block
// ������
// ���أ�
// =============================================================================
static tagBlockHeader *
BlockAbsorb(tagHeadControl * control,tagBlockHeader * prev, tagBlockHeader * block)
{
    HEAP_AEESRT(!BlockIsLast(prev) && "previous block can't be last!");
    prev->size += BlockGetSize(block) + BLOCK_HEADER_OFFSET;
    BlockLinkNext(control,prev);
    return prev;
}

// =============================================================================
// ���ܣ�����Ƿ�����ںϣ����ԵĻ���block����ǰһ��block�ںϣ��ϳ�һ��block
// ������
// ���أ�
// =============================================================================
static tagBlockHeader * BlockMergePrev(tagHeadControl * control, tagBlockHeader * block)
{
    if (BlockIsPrevFree(block))
    {
        tagBlockHeader* prev = BlockPrev(block);
        HEAP_AEESRT(prev && "prev physical block can't be null");
        HEAP_AEESRT(BlockIsFree(prev) && "prev block is not free though marked as such");
        BlockRemove(control, prev);
        block = BlockAbsorb(control,prev, block);
    }

    return block;
}

// =============================================================================
// ���ܣ�����Ƿ�����ںϣ����ԵĻ���block�����һ��block�ںϣ��ϳ�һ��block
// ������
// ���أ�
// =============================================================================
static tagBlockHeader * BlockMergeNext(tagHeadControl * control, tagBlockHeader * block)
{
    tagBlockHeader* next = BlockNext(control,block);
    if(next==NULL)
        return NULL;
    if (BlockIsFree(next))
    {
        HEAP_AEESRT(!BlockIsLast(block) && "previous block can't be last!");
        BlockRemove(control, next);
        block = BlockAbsorb(control,block, next);
    }
    return block;
}

// =============================================================================
// ���ܣ�����block�Ƿ�ɷָ���ԵĻ�ִ�зָ���ѷָ������block���²������
// ������
// ���أ�
// =============================================================================
static void BlockTrimFree(tagHeadControl * control, tagBlockHeader * block, uint32_t level)
{
    HEAP_AEESRT(BlockIsFree(block) && "block must be free");
    if (BlockCanSplit(block, level))
    {
        tagBlockHeader* remaining_block = BlockSplit(control,block, level);
        BlockLinkNext(control,block);
        BlockSetPrevFree(remaining_block);
        BlockInsert(control, remaining_block);
    }
}

// =============================================================================
// ���ܣ���block��������ȡ��
// ������
// ���أ�
// =============================================================================
static tagBlockHeader * BlockLocateFree(tagHeadControl * control, uint32_t level)
{
    tagBlockHeader * block = NULL;
    HEAP_AEESRT(level<HEAD_LIST_NUM);
SEARCH_MEM:
    if(level>=HEAD_LIST_NUM)
        return NULL;
    if(control->mem_lst[level]!=NULL)
    {
        block = control->mem_lst[level];
        if(block->next_free==block)
            control->mem_lst[level] = NULL;
        else
        {
            block->next_free->prev_free = block->prev_free;
            block->prev_free->next_free = block->next_free;
            control->mem_lst[level] = block->next_free;
        }
        block->next_free = NULL;
        block->prev_free = NULL;
    }
    else
    {
        level++;
        goto SEARCH_MEM;
    }
    return block;
}

// =============================================================================
// ���ܣ���block׼����ʹ�ã����ñ�־λ�Լ����ָ����
// ������
// ���أ�
// =============================================================================
static void *
BlockPrepareUsed(tagHeadControl * control, tagBlockHeader * block, uint32_t level)
{
    void* p = 0;
    if (block)
    {
        BlockTrimFree(control, block, level);
        BlockMarkAsUsed(control,block);
        p = BlockToPtr(block);
    }
    return p;
}

// =============================================================================
// ���ܣ���ʼ��control���ƿ�
// ������
// ���أ�
// =============================================================================
static void ControlConstruct(tagHeadControl * control)
{
    uint16_t i=0;
    control->block_null.next_free = &control->block_null;
    control->block_null.prev_free = &control->block_null;
    for(i=0;i<HEAD_LIST_NUM;i++)
        control->mem_lst[i] = NULL;
}

static void * __Malloc(tagHeadControl * control, size_t size)
{
    const uint32_t adjust_id = AdjustRequestSize(size, BLOCK_ALIGN_SIZE);
    if(adjust_id==HEAD_LIST_NUM)
        return NULL;
    tagBlockHeader * block = BlockLocateFree(control, adjust_id);
    return BlockPrepareUsed(control, block, adjust_id);
}

static void __Free(tagHeadControl * control, void * ptr)
{
    if (ptr)
    {
        tagBlockHeader * block = BlockFromPtr(ptr);
        HEAP_AEESRT(!BlockIsFree(block) && "block already marked as free");
        BlockMarkAsFree(control,block);
        block = BlockMergePrev(control, block);
        block = BlockMergeNext(control, block);
        BlockInsert(control, block);
    }
}

void *DjyMalloc(tagHeadControl *control,size_t size)
{
    void *mem = NULL;
    atom_low_t low;
    if(control==NULL || size==0)
        return NULL;
    low = Int_LowAtomStart();
    mem = __Malloc(control,size);
    Int_LowAtomEnd(low);
    return mem;
}

void DjyFree(tagHeadControl *control,void * ptr)
{
    atom_low_t low;
    if(control==NULL || ptr==NULL)
         return;
    low = Int_LowAtomStart();
    __Free(control,ptr);
    Int_LowAtomEnd(low);
}

bool_t DjyMemInit(tagHeadControl *control,void *mem,size_t size)
{
    tagBlockHeader  *block;
    atom_low_t low;
    if(mem==NULL || size<=sizeof(tagBlockHeader) || control==NULL)
        return false;
    low = Int_LowAtomStart();
    ControlConstruct(control);
    block = (tagBlockHeader*)AlignUp((size_t)mem,BLOCK_ALIGN_SIZE);
    HEAP_AEESRT((size_t)block<=(size_t)mem);
    if((size_t)block<(size_t)mem)
        return false;
    block->size = size +  (size_t)block - (size_t)mem - sizeof(tagBlockHeader);
    block->prev_phys_block = NULL;
    block->next_free = block;
    block->prev_free = block;
    control->last_addr = block->size +  (size_t)block + sizeof(tagBlockHeader);
    BlockInsert(control,block);
    BlockSetFree(block);
    Int_LowAtomEnd(low);
    return true;
}
