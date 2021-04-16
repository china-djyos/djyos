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
//链表中每个节点的尺寸，要加上 block_header_t
static const size_t heap_mem_list[HEAD_LIST_NUM] =
                            {16,32,64,128,256,512,1024,1560,2048,3072,4096,8192};

//块控制头结构。
//AddressSortPrev 指针用于合并相邻块，它把所有块按物理相邻的原则串接起来。
struct block_header_t
{
    struct block_header_t * AddressSortPrev;    //指向按地址排序的前一块，不管是否空闲
    size_t size;            //块尺寸，不包含 struct block_header_t 本身；
                            //bit0 用于表示 BLOCK_FREE_BIT
                            //bit1 用于表示 BLOCK_PREV_FREE_BIT
    struct block_header_t * next_free;
    struct block_header_t * prev_free;
}__attribute__((aligned (BLOCK_ALIGN_SIZE)));

#define BLOCK_SIZE_MIN              (heap_mem_list[0] + sizeof(struct block_header_t))
#define BLOCK_SIZE_MAX              (heap_mem_list[HEAD_LIST_NUM-1] + sizeof(struct block_header_t))
#define BLOCK_FREE_BIT       (1U<<0)
#define BLOCK_PREV_FREE_BIT  (1U<<1)

//mem_lst[i]所串联的是 ≥ heap_mem_list[i] 且 ＜ heap_mem_list[i+1] 的内存块
struct PkgHeapCB
{
//  struct block_header_t block_null;
    struct block_header_t *mem_lst[HEAD_LIST_NUM];
    size_t last_addr;       //堆的末地址（最后一个字节地址+1）
};


// =============================================================================
// 功能：获取block的大小
// 参数：block：block的指针
// 返回：无
// =============================================================================
static size_t __BlockGetSize(const struct block_header_t * block)
{
    return block->size & ~(BLOCK_FREE_BIT | BLOCK_PREV_FREE_BIT);
}

// =============================================================================
// 功能：设置block的大小
// 参数：block：block的指针
// 返回：无
// =============================================================================
static void __BlockSetSize(struct block_header_t * block, size_t size)
{
    const size_t oldsize = block->size;
    block->size = size | (oldsize & (BLOCK_FREE_BIT | BLOCK_PREV_FREE_BIT));
}

// =============================================================================
// 功能：检查block是否是最后一个
// 参数：block：block的指针
// 返回：无
// =============================================================================
static int BlockIsLast(const struct block_header_t * block)
{
    return (__BlockGetSize(block) == 0);
}

// =============================================================================
// 功能：获取物理相邻的后一个block的地址
// 参数：Block：block的地址
// 返回：后一个block的地址
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
// 功能：把物理相邻的后一个block的prev_phys_block参数设成前一个block的地址
// 参数：control：控制块，Block：前一个block的地址
// 返回：后一个物理相邻block的地址
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
// 功能：把block的标志位设为空闲，并设置后一个Block的prev_phys_block参数
// 参数：PkgHeap：所属 heap，
//       Block：被操作的 block
// 返回：无
// =============================================================================
static void __BlockMarkAsFree(struct PkgHeapCB * PkgHeap,struct block_header_t * block)
{
    struct block_header_t * next = NULL;
//  __BlockSetFree(block);                  //把本 block 设为空闲
    block->size |= BLOCK_FREE_BIT;          //把本 block 设为空闲
    next = __BlockLinkNext(PkgHeap,block);  //把 block 相邻的下一块的 phy prev 指针指向自己
    if(next==NULL)
        return;
//  __BlockSetPrevFree(next);                 //设置 block 相邻的下一块的“前一块空闲”标志
    next->size |= BLOCK_PREV_FREE_BIT;       //设置 block 相邻的下一块的“前一块空闲”标志
}

// =============================================================================
// 功能：把block的标志位设为忙碌，并设置后一个Block的前一个block为忙碌标志
// 参数：control：控制块，Block：block的地址
// 返回：无
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
// 功能：计算size值对应到链表的某个档位(插入时用)
// 参数：size:需要的大小
// 返回：档位
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
// 功能：根据 size 值找到对应的且有可用内存的档位
// 参数：
// 返回：
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
// 功能：把block从control控制块中移除
// 参数：control:控制块，block：block的地址
// 返回：无
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
//功能：把block插入 PkgHeap 相应尺寸 level 的链表中
//参数：PkgHeap:所属堆，
//      block：block的地址
//返回：无
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
// 功能：检查该块是否还可以被分割，块尺寸＞ (level 对应的尺寸 + BLOCK_SIZE_MIN)说明
//      可以分割，BLOCK_SIZE_MIN=最小块尺寸+sizeof(struct block_header_t)
// 参数：
// 返回：
// =============================================================================
static int __BlockCanSplit(struct block_header_t * block, u32 level)
{
    return __BlockGetSize(block) >= (BLOCK_SIZE_MIN + heap_mem_list[level]);
}

//=============================================================================
//功能：分割 block，原 block 指针保持不变，其尺寸保留 Level 级别。并设置物理空闲链表，
//      把物理下一块的 phy prev 指针指向自己并设置其“前一块空”标志
//参数：PkgHeap，所属堆
//      block，待分割的块，把其中多余的内存切出去。
//      level，block中需要保留的 level
//返回：分割出来的块指针，若不能分割，则返回NULL
//=============================================================================
static struct block_header_t *__BlockSplit(struct PkgHeapCB * PkgHeap,
                                    struct block_header_t * block, u32 level)
{
    size_t size = heap_mem_list[level];
    size_t remain_size;
    struct block_header_t* remaining;
    //从 block 中裁出 level 所需尺寸，取相邻地址
    remaining = (struct block_header_t*)((size_t)(block+1) + size);
//  remaining = OffsetToBlock(__BlockToPtr(block), size);

    //计算多出来的内存尺寸，减掉了块控制头的尺寸
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
// 功能：把物理相邻的前后两个block融合，合成一个block。调用前以确保两个block是空闲的
// 参数：PkgHeap，所属堆
//      prev，待融合的两个block中排前面的一个（地址较小）
//      block，带融合的两个block中排后面的一个（地址较大）
// 返回：
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
// 功能：检查block与其物理上前面相邻的一个block是否空闲，空闲则融合
// 参数：PkgHeap，所属堆
//       block，待检查的块
// 返回：融合后新块地址，若不能融合则返回NULL
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
// 功能：检查block与其物理上后面相邻的一个block是否空闲，空闲则融合
// 参数：PkgHeap，所属堆
//       block，待检查的块
// 返回：融合后新块地址，若不能融合则返回NULL
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
//功能：修剪block，根据要求的 level ，检查该block是否可分割，可以的话执行分割，并把
//      分割出来的block重新插回链表，原 block 指针保持不变，其尺寸保留 Level 级别。
//参数：PkgHeap，所属堆
//      block，待分割的块，必须是空闲的，把其中多余的内存切出去。
//      level，block中需要保留的 level
//返回：
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
        remaining_block->size |= BLOCK_PREV_FREE_BIT;   //设置 block 相邻的下一块的“前一块空闲”标志
        __BlockInsert(PkgHeap, remaining_block);
    }
}

//=============================================================================
//功能：从 PkgHeap 中取出一个level级别的 block，如果该 level 的队列是 NULL 的，则
//      取上一级的，依次类推。如果取出的是上级块，并不分割。
//参数：PkgHeap，堆指针
//     level，尺寸阶级
//返回：块控制块头指针
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
//功能：分配一块内存，步骤：
//      1、从 heap_mem_list 表中找到与size匹配档位 level
//      2、从 level 档位链表中取出一个block，若无空闲，则向更大尺寸level取。
//      3、所取块的size可能大于所需，把多余部分裁剪下来，重新加入链表中。
//参数：PkgHeap，所属块
//      size，欲分配的内存
//返回：分配到的内存指针
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
    if(level==HEAD_LIST_NUM)    //size＞最大块，无法分配
        mem = NULL;
    else
    {
        block = __GetOneBlock(PkgHeap, level);      //从空闲链表中取出一块，可能比所需的大
        if(block != NULL)
        {
            __BlockTrim(PkgHeap, block, level);     //修剪块，把多余的部分回收
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
    //size 要扣除 sizeof(struct block_header_t)
    block->size = size +  (size_t)block - (size_t)mem - sizeof(struct block_header_t);
    block->AddressSortPrev = NULL;
    block->next_free = block;
    block->prev_free = block;
    PkgHeap->last_addr = block->size +  (size_t)block + sizeof(struct block_header_t);
    __BlockInsert(PkgHeap,block);
//  __BlockSetFree(block);
    block->size |= BLOCK_FREE_BIT;          //把本 block 设为空闲
    Int_LowAtomEnd(low);
    return PkgHeap;
}
