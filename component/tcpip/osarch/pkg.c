//-----------------------------------------------------------------------------
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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合以下三条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然:
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。
// 3. 本软件作为都江堰操作系统的组成部分，未获事前取得的书面许可，不允许移植到非
//    都江堰操作系统环境下运行。

// 免责声明:本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
#include <sys/socket.h>
#include "dbug.h"
#include "heap.h"
#include <shell.h>

#include "../component_config_tcpip.h"
//attention than the biggest buffer is 8192,if you need more, please contact the author
//we will add some

//tagNetPkg的原理
//bufsize在申请时指定，使用过程中一直不变;pkgbuf 一直指向buf的起始位置，保持不变
//当向PKG写入数据时，offset不变，从buf的offset+datalen的地方开始写新数据，写完之后
//                  datalen +=len(len为写入数据长度)
//当从PKG读取数据时，从buf的offset开始cpy，cpy完成之后，
//                  offset += len,datalen -= len(len为取出数据长度)
struct NetPkg
{
    struct NetPkg   *partnext;  //单向非循环，该组指针负责数据包在协议栈的传递
    ptu32_t  PkgPrivate;// 用户数据，当前只用于收到乱序包时保存包对应的tcp序号。
//  u8   level;         // PKG的大小等级，冗余
    u8   pkgflag;       // PKG的属性标志，目前只有包结束标志
    u8   refers;        // 缓存的次数
    u16  datalen;       // buf中的有效数据长度
    u16  bufsize;       // buf的长度
    u16  pkgoffset;     // 有效数据偏离pkgbuf的位置，offset之前的数据无效,当分拆数据或者数据对齐的时候很有用
    u8   *pkgbuf;       // pkg的buf（数据缓存区）
};
struct PkgHeapCB;

//#define CN_NETPKG_LEVEL           9                     //how many layers
//static struct NetPkg *sgPkgFreeLst[CN_NETPKG_LEVEL];        //used to list the free pkg
#define CN_PKG_HDRSIZE    (sizeof(struct NetPkg))
//static u32  sgPkgLevlMap[CN_NETPKG_LEVEL]=  {32,64,128,256,512,1024,2048,4096,8192}; //layer size
static u8  *pPkgMemSrc = NULL;                          //the package heap
static u32  gPkgMemOffset = 0;                          //the package use heap offset
static  struct MutexLCB* pPkgQueLock = NULL;                     //protect the heap and the free list
static struct PkgHeapCB *pkg_heap_control;

void PkgInit(struct NetPkg *pkg, u8 flag, u16 offset, u16 datalen, u8* buf)
{
    pkg->partnext = NULL;
    pkg->pkgflag  = flag;
    pkg->pkgoffset   = offset;
    pkg->datalen  = datalen;
    pkg->pkgbuf      = buf;
    return;
}

//typedef struct
//{
//     u32 mallocnum[CN_NETPKG_LEVEL]; //how many pack we have malloc ok from the heap
//     u32 freenum[CN_NETPKG_LEVEL];   //how many pack we have free
//     u32 failnum[CN_NETPKG_LEVEL];   //how many times we failed to malloc the buffer
//     u32 overbufnum;                 //which means it is out of the max buffer size times
//}tagPkgStatistics;
//static tagPkgStatistics gPkgStatistics;

extern struct PkgHeapCB * __PkgMenInit(void *mem,size_t size);
extern void *__PkgMalloc(struct PkgHeapCB *PkgHeap,size_t size);
extern void __PkgFree(struct PkgHeapCB *PkgHeap,void * ptr);

//=============================================================================
//FUNCTION   :use this function to malloc a pkg
//PARAMS IN  :buffersize, the pkg buffer size
//            bufferpro, which means the pkg is the end in the queue
//PARAMS OUT :
//RETURN     :the package malloc or NULL failed
//DESCRIPTION:
//=============================================================================
struct NetPkg *PkgMalloc(u16 bufsize, u8 flags)
{
    u16  pkgsize;
    struct NetPkg *result;

    result = NULL;
    pkgsize = bufsize+CN_PKG_HDRSIZE;
    if(Lock_MutexPend(pPkgQueLock,CN_TIMEOUT_FOREVER))
    {
        result = (struct NetPkg *)malloc(pkgsize);
//      result = (struct NetPkg *)__PkgMalloc(pkg_heap_control,pkgsize);
        if(result!=NULL)
        {
            result->partnext = NULL;
            result->bufsize = pkgsize - CN_PKG_HDRSIZE;
            result->pkgbuf = (u8 *)result + CN_PKG_HDRSIZE;
            result->datalen = 0;
            result->refers = 0;
            result->pkgflag =flags;
//          result->level = i;
            result->pkgoffset = 0;
        }
        Lock_MutexPost(pPkgQueLock);
    }
    return result;
}

struct NetPkg *PkgGetNextUnit(struct NetPkg *NextUnit)
{
    return NextUnit->partnext;
}

void PkgSetNextUnit(struct NetPkg *pkg,struct NetPkg *NextUnit)
{
    if(pkg != NextUnit)
        pkg->partnext = NextUnit;
}

u8* PkgGetCurrentBuffer(struct NetPkg *pkg)
{
    return pkg->pkgbuf + pkg->pkgoffset;
}
u8* PkgGetBuffer(struct NetPkg *pkg)
{
    return pkg->pkgbuf;
}
void PkgSetBuffer(struct NetPkg *pkg, u8 *buf)
{
    pkg->pkgbuf = buf;
}
u16 PkgGetDataLen(struct NetPkg *pkg)
{
    return pkg->datalen;
}
void PkgSetDataLen(struct NetPkg *pkg,u16 len)
{
    pkg->datalen = len;
}
u16 PkgGetOffset(struct NetPkg *pkg)
{
    return pkg->pkgoffset;
}
void PkgSetOffset(struct NetPkg *pkg,u16 offset)
{
    pkg->pkgoffset = offset;
}
ptu32_t PkgGetPrivate(struct NetPkg *pkg)
{
    return pkg->PkgPrivate;
}
void PkgSetPrivate(struct NetPkg *pkg,ptu32_t PkgPrivate)
{
    pkg->PkgPrivate = PkgPrivate;
}
bool_t PkgIsBufferEnd(struct NetPkg *pkg)
{
    return (pkg->pkgflag&CN_PKLGLST_END);
}

void PkgMoveOffsetUp(struct NetPkg *pkg, u16 len)
{
    pkg->pkgoffset += len;
    pkg->datalen -= len;
}
void PkgMoveOffsetDown(struct NetPkg *pkg, u16 len)
{
    pkg->pkgoffset -= len;
    pkg->datalen += len;
}
u16 PkgListDatastatistics(struct NetPkg *pkg)
{
    u16 result = 0;
    while(pkg != NULL)
    {
        result += pkg->datalen;
        pkg = PkgGetNextUnit(pkg);
    }
    return result;
}
u16 PkgFrameDatastatistics(struct NetPkg *pkg)
{
    u16 result = 0;
    while(pkg != NULL)
    {
        result += pkg->datalen;
        if(PkgIsBufferEnd(pkg))
            break ;
        else
            pkg = PkgGetNextUnit(pkg);
    }
    return result;
}

//-----------------------------------------------------------------------------
//功能：把 pkg 包链中的全部数据 copy 到 dst 缓冲区中
//参数：pkg，被copy的包链
//     dst，接收数据的缓冲区。
//返回：copy的数据长度
//注意：注意与 PkgFrameDataCopy 的细微区别
//-----------------------------------------------------------------------------
u16 PkgListDataCopy(struct NetPkg *pkg,u8 *dst)
{
    u8 *src;
    u16 cpylen = 0;
    while (NULL != pkg)
    {
        src = (u8 *) (pkg->pkgbuf + pkg->pkgoffset);
        memcpy(dst, src, pkg->datalen);
        dst += pkg->datalen;
        cpylen += pkg->datalen;
        pkg = PkgGetNextUnit(pkg);
    }
    return cpylen;
}

//-----------------------------------------------------------------------------
//功能：把 pkg 包链中的一帧数据 copy 到 dst 缓冲区中
//参数：pkg，被copy的包链
//     dst，接收数据的缓冲区。
//返回：copy的数据长度
//注意：注意与 PkgListDataCopy 的细微区别
//-----------------------------------------------------------------------------
u16 PkgFrameDataCopy(struct NetPkg *pkg,u8 *dst)
{
    u8 *src;
    u16 cpylen = 0;
    while (NULL != pkg)
    {
        src = (u8 *) (pkg->pkgbuf + pkg->pkgoffset);
        memcpy(dst, src, pkg->datalen);
        dst += pkg->datalen;
        cpylen += pkg->datalen;
//          if (pkg->pkgflag & CN_PKLGLST_END)
        if(PkgIsBufferEnd(pkg))
        {
            pkg = NULL;
        }
        else
        {
            pkg = PkgGetNextUnit(pkg);
        }
    }
    return cpylen;
}

//-----------------------------------------------------------------------------
//功能：把 pkg 包链中的全部数据 copy 到 dst 包中，dst是一个单包
//参数：pkg，被copy的包链
//     dst，目标包，是个单包。
//返回：无
//注意：注意与 PkgCopyFrameToPkg 的细微区别
//-----------------------------------------------------------------------------
void PkgCopyListToPkg(struct NetPkg *pkg,struct NetPkg *dst)
{
    u8 *dstbuf,*src;
    u16 pkglen;
    dstbuf = (u8 *)(dst->pkgbuf + dst->pkgoffset);
    while(NULL != pkg)
    {
        pkglen = pkg->datalen;
        src = pkg->pkgbuf + pkg->pkgoffset;
        memcpy(dstbuf, src, pkglen);
        dstbuf += pkglen;
        dst->datalen += pkglen;
        pkg = PkgGetNextUnit(pkg);
    }
}

//-----------------------------------------------------------------------------
//功能：把 pkg 包链中的一帧数据 copy 到 dst 包中，dst是一个单包
//参数：pkg，被copy的包链
//     dst，目标包，是个单包。
//返回：无
//注意：注意与 PkgCopyListToPkg 的细微区别
//-----------------------------------------------------------------------------
void PkgCopyFrameToPkg(struct NetPkg *pkg,struct NetPkg *dst)
{
    u8 *dstbuf,*src;
    u16 pkglen;
    dstbuf = (u8 *)(dst->pkgbuf + dst->pkgoffset);
    while(NULL != pkg)
    {
        pkglen = pkg->datalen;
        src = pkg->pkgbuf + pkg->pkgoffset;
        memcpy(dstbuf, src, pkglen);
        dstbuf += pkglen;
        dst->datalen += pkglen;
        if(PkgIsBufferEnd(pkg))
        {
            break;
        }
        else
        {
            pkg = PkgGetNextUnit(pkg);
        }
    }
}

// =============================================================================
// FUNCTION   :use this function to free an package
// PARAMS IN  :pkg, the package to free
// PARAMS OUT :
// RETURN     :true
// DESCRIPTION:if the package's refers is zero then we will free it else sub the refers
//             if its matches the heap's offset, then will free it to the heap
// =============================================================================
bool_t PkgTryFreePart(struct NetPkg *pkg)
{
    if(NULL != pkg)
    {
        if(Lock_MutexPend(pPkgQueLock,CN_TIMEOUT_FOREVER))
        {
            if(pkg->refers == 0)
            {
                free(pkg);
//              __PkgFree(pkg_heap_control,pkg);
            }
            else
            {
                pkg->refers--;
            }
            Lock_MutexPost(pPkgQueLock);
        }
    }
    return true;
}

// =============================================================================
// FUNCTION   :we use this function to free the queue which terminated by partnxt is NULL
// PARAMS IN  :pkglst, the package to free
// PARAMS OUT :
// RETURN     :true
// DESCRIPTION:
// =============================================================================
bool_t PkgTryFreeQ(struct NetPkg  *pkglst)
{
    struct NetPkg  *pkg;
    struct NetPkg  *pkgnxt;
    if(Lock_MutexPend(pPkgQueLock,CN_TIMEOUT_FOREVER))
    {
        pkg = pkglst;
        while(NULL != pkg)
        {
            pkgnxt = pkg->partnext;
            if(pkg->refers==0)
            {
                free(pkg);
//              __PkgFree(pkg_heap_control,pkg);
            }
            else
            {
                pkg->refers--;
            }
            pkg = pkgnxt;
        }
        Lock_MutexPost(pPkgQueLock);
    }
    return true;
}
// =============================================================================
// FUNCTION   :we use this function to cache a part, which add the refers
// PARAMS IN  :pkg, the package to cache
// PARAMS OUT :
// RETURN     :true
// DESCRIPTION:
// =============================================================================
bool_t PkgCachedPart(struct NetPkg  *pkg)
{
    if(NULL != pkg)
    {
        pkg->refers++;
    }
    return true;
}
// =============================================================================
// FUNCTION   :we use this function to cache a package list, which add the refers
// PARAMS IN  :pkglst, the package list to cache
// PARAMS OUT :
// RETURN     :true
// DESCRIPTION:
// =============================================================================
bool_t PkgCachedLst(struct NetPkg   *pkglst)
{
    struct NetPkg *pkg;
    pkg = pkglst;
    while(NULL != pkg)
    {
        pkg->refers++;
        if(PkgIsBufferEnd(pkg))
        {
            pkg = NULL;// this is the end of the lst
        }
        else
        {
            pkg = pkg->partnext;
        }
    }
    return true;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// =============================================================================
// FUNCTION   :we use this function browse the package module status
// PARAMS IN  :param,NO USE NOW
// PARAMS OUT :
// RETURN     :true
// DESCRIPTION:
// =============================================================================
//bool_t PkgMemShow(char *param)
bool_t pkgmem(char *param)
{
    debug_printf("pkg","PkgMem:HdrSize:%d\n\r",CN_PKG_HDRSIZE);
    debug_printf("pkg","PkgMem:    TOTAL:%08d   USE:%08d  USAGE:%02d%%\n\r",\
            CFG_NETPKG_MEMSIZE,gPkgMemOffset,gPkgMemOffset*100/(CFG_NETPKG_MEMSIZE+1));
//    debug_printf("pkg","OverLayer:%d\n\r",gPkgStatistics.overbufnum);
//  for(i = 0; i<CN_NETPKG_LEVEL;i++)
//  {
//      debug_printf("pkg","layer:%04d:TOTAL:0x%08x   Free:0x%08x  Failed:0x%08x\n\r",
//              sgPkgLevlMap[i],gPkgStatistics.mallocnum[i],gPkgStatistics.freenum[i],gPkgStatistics.failnum[i]);
//  }
    return true;
}
#pragma GCC diagnostic pop

// =============================================================================
// FUNCTION:this function is used to initialize the package memory
// PARA  IN:
// PARA OUT:
// RETURN  :true success while false failed
// INSTRUCT:
// =============================================================================
bool_t PkgModuleInit(void)
{
    bool_t result = false;
    //FIRST MALLOC THE MEM FROM THE HEAP
    gPkgMemOffset = 0;
    pPkgQueLock = Lock_MutexCreate(NULL);
    if(NULL == pPkgQueLock)
    {
        error_printf("pkg","%s:create memory sync failed\r\n",__FUNCTION__);
        goto EXIT_SYNC;
    }
//    pPkgMemSrc = net_malloc(CFG_NETPKG_MEMSIZE);
//    if(NULL == pPkgMemSrc)
//    {
//        error_printf("pkg","%s:create memory block failed\r\n",__FUNCTION__);
//        goto EXIT_MEM;
//    }
//    pkg_heap_control = __PkgMenInit(pPkgMemSrc,CFG_NETPKG_MEMSIZE);
//    if(pkg_heap_control == NULL)
//    {
//        error_printf("pkg","%s:create memory block failed\r\n",__FUNCTION__);
//        goto EXIT_MEM;
//    }

    result = true;
    return result;

//EXIT_MEM:
//    Lock_MutexDelete_s(pPkgQueLock);
//    pPkgQueLock = NULL;
EXIT_SYNC:
    result = false;
    return result;
}
ADD_TO_ROUTINE_SHELL(pkgmem,pkgmem,"usage:pkgmem,  show the pkg module statistics");




