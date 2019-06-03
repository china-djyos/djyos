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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
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
// 功能：获取block的大小
// 参数：block：block的指针
// 返回：无
// =============================================================================
static size_t BlockGetSize(const tagBlockHeader * block)
{
    return block->size & ~(BLOCK_HRADER_FREE_BIT | BLOCK_HEADER_PREV_FREE_BIT);
}

// =============================================================================
// 功能：设置block的大小
// 参数：block：block的指针
// 返回：无
// =============================================================================
static void BlockSetSize(tagBlockHeader * block, size_t size)
{
    const size_t oldsize = block->size;
    block->size = size | (oldsize & (BLOCK_HRADER_FREE_BIT | BLOCK_HEADER_PREV_FREE_BIT));
}

// =============================================================================
// 功能：检查block是否是最后一个
// 参数：block：block的指针
// 返回：无
// =============================================================================
static int BlockIsLast(const tagBlockHeader * block)
{
    return (BlockGetSize(block) == 0);
}

// =============================================================================
// 功能：检查block的是否是空闲的
// 参数：block：block的指针
// 返回：无
// =============================================================================
static int BlockIsFree(const tagBlockHeader * block)
{
    return (int)(block->size & BLOCK_HRADER_FREE_BIT);
}

// =============================================================================
// 功能：设置block为空闲状态
// 参数：block：block的指针
// 返回：无
// =============================================================================
static void BlockSetFree(tagBlockHeader * block)
{
    block->size |= BLOCK_HRADER_FREE_BIT;
}

// =============================================================================
// 功能：设置block为正在使用状态
// 参数：block：block的指针
// 返回：无
// =============================================================================
static void BlockSetUsed(tagBlockHeader * block)
{
    block->size &= ~BLOCK_HRADER_FREE_BIT;
}

// =============================================================================
// 功能：检查block的前一个物理相邻的block为空闲标志位
// 参数：block：block的指针
// 返回：无
// =============================================================================
static int BlockIsPrevFree(const tagBlockHeader * block)
{
    return (int)(block->size & BLOCK_HEADER_PREV_FREE_BIT);
}

// =============================================================================
// 功能：设置block的前一个物理相邻的block为空闲标志位
// 参数：block：block的指针
// 返回：无
// =============================================================================
static void BlockSetPrevFree(tagBlockHeader * block)
{
    block->size |= BLOCK_HEADER_PREV_FREE_BIT;
}

// =============================================================================
// 功能：设置block的前一个物理相邻的block为忙碌标志位
// 参数：block：block的指针
// 返回：无
// =============================================================================
static void BlockSetPrevUsed(tagBlockHeader * block)
{
    block->size &= ~BLOCK_HEADER_PREV_FREE_BIT;
}

// =============================================================================
// 功能：从ptr里面偏移得到block的地址
// 参数：ptr：缓存地址
// 返回：block的地址
// =============================================================================
static tagBlockHeader * BlockFromPtr(const void * ptr)
{
    return (tagBlockHeader *)(ptr - BLOCK_HEADER_OFFSET);
}

// =============================================================================
// 功能：从block里面偏移得到ptr的地址
// 参数：block：block的地址
// 返回：ptr:缓存的地址
// =============================================================================
static void * BlockToPtr(const tagBlockHeader * block)
{
    return (void*)((uint8_t*)block + BLOCK_HEADER_OFFSET);
}

// =============================================================================
// 功能：从ptr里面偏加上偏移值
// 参数：ptr：缓存地址，size:偏移地址
// 返回：block的地址
// =============================================================================
static tagBlockHeader * OffsetToBlock(const void * ptr, size_t size)
{
    return (tagBlockHeader *)((uint8_t*)ptr + size);
}

// =============================================================================
// 功能：获取物理相邻的前一个block的地址
// 参数：Block：block的地址
// 返回：前一个block的地址
// =============================================================================
static tagBlockHeader * BlockPrev(const tagBlockHeader * block)
{
    return block->prev_phys_block;
}

// =============================================================================
// 功能：获取物理相邻的后一个block的地址
// 参数：Block：block的地址
// 返回：后一个block的地址
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
// 功能：把物理相邻的后一个block的prev_phys_block参数设成前一个block的地址
// 参数：control：控制块，Block：前一个block的地址
// 返回：后一个block的地值
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
// 功能：把block的标志位设为空闲，并设置后一个Block的prev_phys_block参数
// 参数：control：控制块，Block：block的地址
// 返回：无
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
// 功能：把block的标志位设为忙碌，并设置后一个Block的前一个block为忙碌标志
// 参数：control：控制块，Block：block的地址
// 返回：无
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
// 功能：把size值对应到链表的某个档位(插入时用)
// 参数：size:需要的大小
// 返回：档位
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
// 功能：把size值对应到链表的某个档位（取出时用）
// 参数：size:需要的大小
// 返回：档位
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
// 功能：向上对齐
// 参数：
// 返回：
// =============================================================================
size_t AlignUp(size_t x, size_t align)
{
    HEAP_AEESRT(0 == (align & (align - 1)) && "must align to a power of two");
    return (x + (align - 1)) & ~(align - 1);
}

// =============================================================================
// 功能：向下对齐
// 参数：
// 返回：
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
// 功能：把找到size值所对应的档位
// 参数：
// 返回：
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
// 功能：把block从control控制块中移除
// 参数：control:控制块，block：block的地址
// 返回：
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
// 功能：把block插入control控制块中
// 参数：control:控制块，block：block的地址
// 返回：
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
// 功能：检查该块是否还可以被分割
// 参数：
// 返回：
// =============================================================================
static int BlockCanSplit(tagBlockHeader * block, uint32_t level)
{
    return BlockGetSize(block) >= (BLOCK_SIZE_MIN + heap_mem_list[level]);
}

// =============================================================================
// 功能：分割block
// 参数：
// 返回：
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
// 功能：把block与其前一个block融合，合成一个block
// 参数：
// 返回：
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
// 功能：检查是否可以融合，可以的话把block与其前一个block融合，合成一个block
// 参数：
// 返回：
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
// 功能：检查是否可以融合，可以的话把block与其后一个block融合，合成一个block
// 参数：
// 返回：
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
// 功能：检查该block是否可分割，可以的话执行分割，并把分割出来的block重新插回链表
// 参数：
// 返回：
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
// 功能：把block从链表中取出
// 参数：
// 返回：
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
// 功能：该block准备被使用，设置标志位以及检查分割情况
// 参数：
// 返回：
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
// 功能：初始化control控制块
// 参数：
// 返回：
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
