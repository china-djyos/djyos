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
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。
// 3. 本软件作为都江堰操作系统的组成部分，未获事前取得的书面许可，不允许移植到非
//    都江堰操作系统环境下运行。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <int.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <stat.h>
#include <pool.h>
#include <systime.h>
#include <lock.h>
#include <multiplex.h>
#include <dbug.h>
#include <math.h>
#include <object.h>
#include <objhandle.h>
#include "component_config_objfile.h"

static struct MutexLCB s_tHandleMutex; // 文件系统互斥锁
static struct MemCellPool s_tHandlePool; // 文件预分配池
static struct objhandle s_tHandleInitPool[CFG_HANDLE_LIMIT];

// ============================================================================
// 功能：对象句柄系统上锁；
// 参数：无；
// 返回：无；
// 备注：
// ============================================================================
static inline void __Handle_LockSys(void)
{
    Lock_MutexPend(&s_tHandleMutex, CN_TIMEOUT_FOREVER);
}

// ============================================================================
// 功能：对象句柄系统解锁；
// 参数：无；
// 返回：无；
// 备注：
// ============================================================================
static inline void __Handle_UnlockSys(void)
{
    Lock_MutexPost(&s_tHandleMutex);
}


// ============================================================================
// 功能：对象句柄系统初始化；
// 参数：
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 Handle_ModuleInit(void)
{

    if(!Lock_MutexCreate_s(&s_tHandleMutex, "ohdl sys"))
    {
        return (-1);
    }

    Mb_CreatePool_s(&s_tHandlePool, s_tHandleInitPool,
                        CFG_HANDLE_LIMIT, sizeof(struct objhandle),
                        16, 16384, "handle pool");
    return (0);
}

// 控制文件对象的是指针，但open返回的fd是s32，两者需要有对应关系
// hdl 与 fd 天然能够互相直接转化的条件：
// 1、指针长度小于32
// 2、指针长度==32，但字节位宽＜32
#if(( CN_PTR_BITS < 32)  || ( (CN_PTR_BITS == 32) && (CN_BYTE_BITS < 32)))
// ============================================================================
// 功能：对象句柄转文件描述符；
// 参数：hdl -- 内部句柄；
// 返回：成功（文件描述符）；失败（-1）；
// 备注：
// ============================================================================
s32 Handle2fd(struct objhandle *hdl)
{
    if(!hdl)
        return (-1);

    return (s32)(((u32)hdl) >> 1);
}

// ============================================================================
// 功能：文件描述符转对象句柄
// 参数：fd -- 文件描述符；
// 返回：成功（对象句柄）；失败（NULL）；
// 备注：
// ============================================================================
struct objhandle *fd2Handle(s32 fd)
{
    extern struct objhandle *fd2stdio(s32 fd);

    if(-1 == fd)
        return (NULL);

    if((fd <= 2) && (fd >= 0))
        return (fd2stdio(fd));
    else
        return ((struct objhandle*)(((u32)fd) << 1));
}
#else
// 不能天然相互转换，情况就复杂多了。
// 考虑限制object所使用的内存空间范围，只要限制在2G以内，就可以互转了。
#error FD logic is not supported.
#endif

// ============================================================================
// 功能：是否目录标志；
// 参数：flags -- 使用标志；
// 返回：目录（1）；非目录（0）；
// 备注：INLINE
// ============================================================================
s32 Handle_FlagIsDirectory(u32 flags)
{
    if(flags & O_DIRECTORY)
        return (1);

    return (0);
}

// ============================================================================
// 功能：是否文件标志；
// 参数：flags -- 使用标志；
// 返回：文件（1）；非文件（0）；
// 备注：INLINE
// ============================================================================
s32 Handle_FlagIsRegular(u32 flags)
{
    if(flags & O_DIRECTORY)
        return (0);

    return (1);
}

// ============================================================================
// 功能：是否创建或打开标志；
// 参数：flags -- 使用标志；
// 返回：创建或打开（1）；非创建或打开（0）；
// 备注：INLINE
// ============================================================================
s32 Handle_FlagIsCreate(u32 flags)
{
    if(flags & O_CREAT)
        return (1);

    return (0);
}

// ============================================================================
// 功能：是否仅创建标志；
// 参数：flags -- 使用标志；
// 返回：仅创建（1）；非仅创建（0）；
// 备注：INLINE
// ============================================================================
s32 Handle_FlagIsOnlyCreate(u32 flags)
{
    if((flags & (O_CREAT | O_EXCL)) == (O_CREAT | O_EXCL))
        return (1);

    return (0);
}

// ============================================================================
// 功能：是否追加；
// 参数：flags -- 文件使用标志；
// 返回：追加（1）；非追加（0）；
// 备注：INLINE
// ============================================================================
s32 Handle_FlagIsAppend(u32 flags)
{
    if(flags & O_APPEND)
        return (1);

    return (0);
}

// ============================================================================
// 功能：是否截断标志；
// 参数：flags -- 文件使用标志；
// 返回：截断（1）；非截断（0）；
// 备注：INLINE
// ============================================================================
s32 Handle_FlagIsTrunc(u32 flags)
{
    if(flags & (O_TRUNC))
        return (1);

    return (0);
}

// ============================================================================
// 功能：是否可读标志；
// 参数：flags -- 文件使用标志；
// 返回：可读（1）；非可读（0）；
// 备注：INLINE
// ============================================================================
s32 Handle_FlagIsReadable(u32 flags)
{
    if(flags & O_RDONLY)
        return (1);

    return (0);
}

// ============================================================================
// 功能：是否可写标志；
// 参数：flags -- 文件使用标志；
// 返回：可写（1）；非可写（0）；
// 备注：INLINE
// ============================================================================
s32 Handle_FlagIsWriteable(u32 flags)
{
    return (flags & O_WRONLY);
}

// ============================================================================
// 功能：测试设备写操作是否阻塞直至物理层完成传输
// 参数：flags -- 文件使用标志；
// 返回：（1）阻塞直至物理层完成传输；（0）写到缓冲区即认为完成；
// 备注：用于判断设备的操作模式
// ============================================================================
s32 Handle_FlagIsBlockComplete(u32 flags)
{
    if(flags & O_BLOCK_COMPLETE)
        return (1);

    return (0);
}

// ============================================================================
// 功能：对象句柄是否追加模式；
// 参数：hdl -- 对象句柄；
// 返回：追加（1）；非追加（0）；
// 备注：INLINE
// ============================================================================
s32 Handle_IsAppend(struct objhandle *hdl)
{
    return (Handle_FlagIsAppend(hdl->flags));
}

// ============================================================================
// 功能：对象句柄是否可读模式；
// 参数：hdl -- 对象句柄；
// 返回：可读（1）；不可读（0）；
// 备注：INLINE
// ============================================================================
s32 Handle_IsReadable(struct objhandle *hdl)
{
    return (Handle_FlagIsReadable(hdl->flags));
}

// ============================================================================
// 功能：对象句柄是否可写模式；
// 参数：hdl -- 对象句柄；
// 返回：可读（1）；不可读（0）；
// 备注：INLINE
// ============================================================================
s32 Handle_IsWritable(struct objhandle *hdl)
{
    return (Handle_FlagIsWriteable(hdl->flags));
}

// ============================================================================
// 功能：对象是否存在竞争者，即对象被其他地方同时打开
// 参数：hdl -- 对象句柄；
// 返回：存在（1）；不存在（0）；
// 备注：INLINE
// ============================================================================
s32 iscontender(struct objhandle *hdl)
{
    if(!hdl)
        return (0);

    if(dListGetBefore(&hdl->list)==dListGetAfter(&hdl->list))
        return (0);

    return (1);
}

// ============================================================================
// 功能：新建对象句柄；
// 参数：无；
// 返回：成功（对象句柄）；失败（NULL）；
// 备注：INLINE
// ============================================================================
struct objhandle *Handle_New(void)
{
    struct objhandle *hdl;

    hdl = (struct objhandle*)Mb_Malloc(&s_tHandlePool, 0);
    if(!hdl)
        return (NULL);

    memset(hdl, 0x0, sizeof(*hdl));
    dListInit(&hdl->list);
    return (hdl);
}

// ============================================================================
// 功能：释放对象句柄；
// 参数：hdl -- 对象句柄；
// 返回：成功（0）；失败（-1）；
// 备注：INLINE
// ============================================================================
s32 Handle_Delete(struct objhandle *hdl)
{
    if(hdl)
    {
        dListRemove(&hdl->list);
        Mb_Free(&s_tHandlePool, hdl);
        return (0);
    }

    return (-1);
}

// ============================================================================
// 功能：对象句柄的初始化；
// 参数：hdl -- 对象句柄；
//      ob -- 关联的对象；
//      flags，文件访问标志，在fcntl.h中定义，如 O_RDONLY
//      context -- 关联的内容；
// 返回：无；
// 备注：
// ============================================================================
void Handle_Init(struct objhandle *hdl, struct Object *ob, u32 flags, ptu32_t context)
{
    //memset(hdl, 0, sizeof(struct objhandle));
    hdl->timeout = CN_TIMEOUT_FOREVER;
    hdl->HostObj = ob;
    hdl->context = context;
    hdl->flags = flags;
    if(ob)
        dListInsertAfter(&ob->handles, &hdl->list);
}

// ============================================================================
// 功能：获取对象名；
// 参数：hdl -- 对象句柄
// 返回：成功（文件名）；失败（NULL）；
// 备注：INLINE
// ============================================================================
const char *Handle_GetName(struct objhandle *hdl)
{
    if(!hdl)
        return (NULL);

    return (OBJ_GetName(hdl->HostObj));
}

// ============================================================================
// 功能：获取对象句柄关联的内容；
// 参数：hdl -- 对象句柄；
// 返回：成功（私有内容）；失败（NULL）;
// 备注：INLINE
// ============================================================================
ptu32_t Handle_GetContext(struct objhandle *hdl)
{
    if(hdl)
        return (hdl->context);

    return (0);
}

// ============================================================================
// 功能：设置对象句柄关联的内容；
// 参数：hdl -- 对象句柄；
//      context -- 内容；
// 返回：无；
// 备注：INLINE
// ============================================================================
void Handle_SetContext(struct objhandle *hdl, ptu32_t context)
{
    if(hdl)
        hdl->context = context;
}

// ============================================================================
// 功能：获取对象句柄关联的对象内容；
// 参数：hdl -- 对象句柄；
// 返回：成功（文件）；失败（NULL）；
// 备注：
// ============================================================================
ptu32_t handle_GetHostObjectPrivate(struct objhandle *hdl)
{
    if(hdl&&hdl->HostObj)
        return (OBJ_GetPrivate(hdl->HostObj));

    return (0);
}

// ============================================================================
// 功能：获取对象句柄的超时时间；
// 参数：hdl -- 对象句柄；
// 返回：超时时间；
// 备注：
// ============================================================================
u32 handle_gettimeout(struct objhandle *hdl)
{
    if(hdl)
        return (hdl->timeout);

    return (0);
}

// ============================================================================
// 功能：设置对象句柄的超时时间；
// 参数：hdl -- 对象句柄；
//      timeout -- 超时时间；
// 返回：无；
// 备注：INLINE
// ============================================================================
void Handle_SetTimeOut(struct objhandle *hdl, u32 timeout)
{
    if(hdl)
        hdl->timeout = timeout;
}

// ============================================================================
// 功能：获取对象句柄的使用标志；
// 参数：hdl -- 对象句柄；
// 返回：使用标志；
// 备注：INLINE
// ============================================================================
u32 Handle_GetMode(struct objhandle *hdl)
{
    if(hdl)
        return (hdl->flags);

    return (0);
}

// ============================================================================
// 功能：设置对象句柄的使用标志；
// 参数：hdl -- 对象句柄；
//      mode -- 使用标志；
// 返回：无；
// 备注：INLINE
// ============================================================================
void Handle_SetMode(struct objhandle *hdl, u32 mode)
{
    if(hdl)
        hdl->flags = mode;
}
// ============================================================================
// 功能：获取对象句柄的关联对象；
// 参数：hdl -- 对象句柄；
// 返回：成功（关联对象）；失败（NULL）；
// 备注：INLINK
// ============================================================================
struct Object *Handle_GetHostObj(struct objhandle *hdl)
{
    if(hdl)
        return (hdl->HostObj);

    return (NULL);
}

#if 0
// ============================================================================
// 功能：遍历文件句柄，遍历一个对象下创建的全部文件，典型用法：
//      struct objhandle *Current = NULL;
//      while(1)
//      {
//          Current = OBJ_TraveFile(Current, s_ptSocketObject);
//          if(Current != NULL)
//              do something;
//          else
//              break;
//      }
// 参数：Current，当前遍历位置，初始为NULL。
// 返回：下一个文件指针，遍历完成则返回NULL
// 备注：当前是SOCKET专用（未见使用）
// ============================================================================
struct objhandle *traverseall(struct objhandle *hdl, struct Object *ob)
{
    struct objhandle *res;
    struct dListNode *list;

    if(!ob)
        return (NULL);

    if(hdl == NULL)
        list = dListGetAfter(&ob->port);
    else
        list = dListGetAfter(&hdl->list);

    if(list == &ob->port)
    {
        res = NULL;
    }
    else
    {
        res = dListEntry(list, struct objhandle, list);
        if(testcontent(reflagsode)) // 越过content
        {
            list = dListGetAfter(&res->list);
            if(list == &ob->port)
                return (NULL);

            res = dListEntry(list, struct __port, list);
        }
    }

    return (res);
}


// ============================================================================
// 功能：设置对象句柄的使用标志；
// 参数：hdl -- 对象句柄；
//      flags -- 使用标志；
// 返回：无；
// 备注：
// ============================================================================
void hdl2objrflag(struct objhandle *hdl, u32 flags)
{
    if(hdl)
        hdl->flags |= flags;
}
#endif
// ============================================================================
// 功能：获取对象句柄的已设置的多路复用触发事件；
// 参数：hdl -- 对象句柄；
// 返回：已设置的多路复用事件；
// 备注：
// ============================================================================
inline u32 Handle_MultiEvents(struct objhandle *hdl)
{
    if(hdl)
        return (hdl->MultiplexEvents);

    return (0);
}

// ============================================================================
// 功能：获取对象句柄的多路复用控制；
// 参数：hdl -- 对象句柄；
// 返回：成功（多路复用控制）；失败（NULL）；
// 备注：
// ============================================================================
struct MultiplexObjectCB *__Handle_GetMultiplexHead(struct objhandle *hdl)
{
    if(hdl)
        return (hdl->pMultiplexHead);

    return (NULL);
}

// ============================================================================
// 功能：设置对象句柄的多路复用控制；
// 参数：hdl -- 对象句柄；
//      cb -- 多路复用控制；
// 返回：无；
// 备注：
// ============================================================================
void __Handle_SetMultiplexHead(struct objhandle *hdl, struct MultiplexObjectCB *cb)
{
    if(hdl)
        hdl->pMultiplexHead = cb;
}

// ============================================================================
// 功能：设置对象句柄的多路复用触发事件；
// 参数：hdl -- 对象句柄；
//      events -- 多路复用触发事件；
// 返回：无；
// 备注：
// ============================================================================
void Handle_SetMultiplexEvent(struct objhandle *hdl, u32 events)
{
//    u32 MultiplexEvents;
    extern bool_t __Multiplex_Set(s32 Fd, u32 dwAccess);
//    Int_SaveAsynSignal();
    __Handle_LockSys();
    if(hdl)
    {
//        MultiplexEvents = hdl->MultiplexEvents;
        hdl->MultiplexEvents |= events;
//        if(MultiplexEvents!=hdl->MultiplexEvents)
            __Multiplex_Set(Handle2fd(hdl), hdl->MultiplexEvents);
    }
//    Int_RestoreAsynSignal();
    __Handle_UnlockSys();
}

// ============================================================================
// 功能：清除对象句柄的多路复用触发事件；
// 参数：hdl -- 对象句柄；
//      events -- 多路复用触发事件；
// 返回：无；
// 备注：
// ============================================================================
void Handle_ClrMultiplexEvent(struct objhandle *hdl, u32 events)
{
//    u32 MultiplexEvents;
    extern bool_t __Multiplex_Set(s32 Fd, u32 dwAccess);
//    Int_SaveAsynSignal();
    __Handle_LockSys();
    if(hdl)
    {
//        MultiplexEvents = hdl->MultiplexEvents;
        hdl->MultiplexEvents &= ~events;
//        if(MultiplexEvents != hdl->MultiplexEvents)
            __Multiplex_Set(Handle2fd(hdl), hdl->MultiplexEvents);
    }
//  Int_RestoreAsynSignal();
    __Handle_UnlockSys();
}

// ============================================================================
// 功能：设置对象的所有句柄的多路复用触发事件；
// 参数：ob -- 被操作的文件对象；
//      events -- 多路复用触发事件；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 OBJ_SetMultiplexEvent(struct Object *ob, u32 events)
{
    struct objhandle *hdl;
//    u32 MultiplexEvents;
    list_t *head, *cur;
    extern bool_t __Multiplex_Set(s32 Fd, u32 dwAccess);

    if(!ob)
        return (-1);

    head = &ob->handles; // head是不需要设置的；
    dListForEach(cur, head)
    {
        hdl = dListEntry(cur, struct objhandle, list);
//        MultiplexEvents = hdl->MultiplexEvents;
//        Int_SaveAsynSignal();
        __Handle_LockSys();
        hdl->MultiplexEvents |= events;
//        if(MultiplexEvents!=hdl->MultiplexEvents)
        __Multiplex_Set(Handle2fd(hdl), hdl->MultiplexEvents);
//      Int_RestoreAsynSignal();
        __Handle_UnlockSys();
    }
    return (0);
}

// ============================================================================
// 功能：清除对象的所有句柄的多路复用触发事件；
// 参数：ob -- 被操作的文件对象；
//      events -- 多路复用触发事件；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 OBJ_ClrMultiplexEvent(struct Object *ob, u32 events)
{
    struct objhandle *hdl;
//    u32 MultiplexEvents;
    list_t *head, *cur;
    extern bool_t __Multiplex_Set(s32 Fd, u32 dwAccess);

    if(!ob)
        return (-1);

    head = &ob->handles;             // 头部是不需要设置的；
    dListForEach(cur, head)
    {
        hdl = dListEntry(cur, struct objhandle, list);
//        MultiplexEvents = hdl->MultiplexEvents;
//      Int_SaveAsynSignal();
        __Handle_LockSys();
        hdl->MultiplexEvents &= ~events;
//        if(MultiplexEvents != hdl->MultiplexEvents)
        __Multiplex_Set(Handle2fd(hdl),hdl->MultiplexEvents);
//      Int_RestoreAsynSignal();
        __Handle_UnlockSys();
    }

    return (0);
}

//----IO状态--------------------------------------------------------------------
//功能：这是一个为 select 函数准备的基础函数，为了兼容一些开源软件而设，编程时不建议使用
//      select函数，应该使用更高效更便捷的 Multiplex_Wait 系列函数
//参数：Fd，待判断的文件
//     mode，所关注的状态，可读、可写、或者其他
//返回：1=actived，0=unactive
//------------------------------------------------------------------------------
s32 issocketactive(s32 Fd, s32 mode)
{
    struct objhandle *hdl;

    hdl = fd2Handle(Fd);
    if(mode & hdl->MultiplexEvents & CN_MULTIPLEX_STATUSMSK)
    {
        return  1;
    }
    return  0;
}


// ============================================================================
// 功能：打开文件
// 参数：pathname -- 文件路径;
//      flags -- 操作标志;fcntl.h的 O_RDONLY 等
//      mode -- 操作模式;flags中有有create标志才有意义，stat.h的 S_IRUSR 等
// 返回：文件描述符，失败==NULL
// 备注：O_RDONLY、O_WRONLY、O_RDWR 须符合POSIX（2016）的最新定义
// ============================================================================
struct objhandle *__open(char *path, u32 flags, u32 mode)
{
    struct objhandle *hdl = NULL;
    struct Object *ob;
    char *uncached;
    s32 run;
    u64 OpenMode;
//  struct stat statbuf;
//  ptu32_t StatResult;
    OpenMode = ( (u64)mode << 32) | flags;
    __Handle_LockSys();
    ob = OBJ_MatchPath(path, &uncached);
    if(ob == NULL)
    {
        __Handle_UnlockSys();
        return NULL;
    }
    OBJ_DutyUp(ob);        // 防止文件操作过程中，被删除了；

//  __Handle_UnlockSys();
        //todo:权限管理暂未实现。框架：调用stat，再判断当前st_mode是否满足flags权限
//      StatResult = ob->ObjOps(CN_OBJ_CMD_STAT, (ptu32_t)ob, &statbuf, uncached, full);
//  if(权限满足要求)
    run = ob->ObjOps((void *)ob, CN_OBJ_CMD_OPEN,
                                (ptu32_t)&hdl,(ptu32_t)&OpenMode,(ptu32_t)uncached);

    if( (run == CN_OBJ_CMD_EXECUTED) && (hdl != NULL) )
    {
      OBJ_DutyUp(hdl->HostObj);
    }
//  else
    OBJ_DutyDown(ob);

    __Handle_UnlockSys();
    return (hdl);
}

// ============================================================================
// 功能：关闭文件
// 参数：hdl -- 对象句柄;
// 返回：成功（0）;失败（-1）；
// 备注：
// ============================================================================
s32 __close(struct objhandle *hdl)
{
    s32 res;
    struct Object *ob = hdl->HostObj;

    res = ob->ObjOps((void *)hdl, CN_OBJ_CMD_CLOSE, 0, 0, 0);
    if(res == CN_OBJ_CMD_TRUE)
    {
        Handle_Delete(hdl);
        OBJ_DutyDown(ob);
        OBJ_ReleaseTempPath(ob); // 释放对象临时路径
        return (0);
    }
    else
        return (-1);
}

// ============================================================================
// 功能：打开文件
// 参数：pathname -- 文件路径;
//      flags -- 操作标志;fcntl.h的 O_RDONLY 等
//      mode -- 操作模式;flags中有有create标志才有意义，stat.h的 S_IRUSR 等
// 返回：失败（-1）; 成功（文件描述符（非-1））;
// 备注：O_RDONLY、O_WRONLY、O_RDWR 须符合POSIX（2016）的最新定义
// ============================================================================
s32 open(const char *pathname, s32 flags, ...)
{
    struct objhandle *hdl;
    va_list args;
    s32 mode;

    if((!pathname) || ('\0' == *pathname))
        return (-1);

    va_start(args, flags);
    mode = va_arg(args, s32);
    va_end(args);

    hdl = __open((char*)pathname, (u32)flags, mode);
    return (Handle2fd(hdl));
}

// ============================================================================
// 功能：关闭文件
// 参数：fd -- 文件描述符;
// 返回：成功（0）;失败（-1）；
// 备注：
// ============================================================================
s32 close(s32 fd)
{
    struct objhandle *hdl;

    hdl = fd2Handle(fd);
    if(!hdl)
        return (-1);

    return (__close(hdl));
}

// ============================================================================
// 功能：删除文件；
// 参数：path -- 文件路径；
// 返回：成功（0）； 失败（-1）；
// 备注：
// ============================================================================
s32 remove(const char *path)
{
    struct Object *ob;
    char *uncached;
    s32 res;

    __Handle_LockSys();
    ob = OBJ_MatchPath(path, &uncached);
    if(!uncached) // 文件已经在系统中
    {
        if(OBJ_IsOnDuty(ob))  // 文件正在被使用中
        {
            __Handle_UnlockSys();
            return (-1);
        }
    }

    OBJ_Lock();
    __Handle_UnlockSys();
    res = ob->ObjOps((void *)ob, CN_OBJ_CMD_DELETE, 0, 0, (ptu32_t)uncached);
    OBJ_Unlock();
    if(res == CN_OBJ_CMD_TRUE)
    {
        OBJ_ReleaseTempPath(ob); // 释放对象临时路径
        return (0);
    }
    else
        return (-1);

}

// ============================================================================
// 功能：读文件；
// 参数：fd -- 文件描述符；
//      buf -- 读缓冲；
//      count -- 需要读的数据大小（Byte计）；
// 返回：成功（实际读取到的长度）；错误（-1）；
// 备注：
// ============================================================================
ssize_t read(s32 fd, void *buf, size_t size)
{
    size_t result;
    struct objhandle *hdl;
    s32 res = -1;

    if(NULL == buf)
        return (-1); // 参数错误

    if(0 == size)
        return (0);

    hdl = fd2Handle(fd);
    if(!hdl)
        return (-1);

    if(Handle_IsReadable(hdl))
    {
        res = hdl->HostObj->ObjOps((void *)hdl, CN_OBJ_CMD_READ, (ptu32_t)&result,
                                (ptu32_t)buf, (ptu32_t)size);
        if(res == CN_OBJ_CMD_EXECUTED)
            return (result);
        else
            return (-1);
    }
    else
        return -1;
}

// ============================================================================
// 功能：写文件；
// 参数：fd -- 文件描述符；
//      buf -- 写缓冲；
//      count -- 需要写的数据大小（Byte计）；
// 返回： 成功（实际写入的长度）；错误（-1）；
// 备注：
// ============================================================================
ssize_t write(s32 fd, const void *buf, size_t count)
{
    size_t result;
    struct objhandle *hdl;
    s32 res = -1;

    if(!buf)
        return (-1); // 参数错误

    if(0 == count)
        return (0);

    hdl = fd2Handle(fd);
    if(!hdl)
        return (-1);

    if(Handle_IsWritable(hdl))
    {
        res = hdl->HostObj->ObjOps((void *)hdl, CN_OBJ_CMD_WRITE, (ptu32_t)&result,
                                (ptu32_t)buf, (ptu32_t)count);
        if(res == CN_OBJ_CMD_EXECUTED)
            return (result);
        else
            return (-1);
    }
    else
        return -1;
}

// ============================================================================
// 功能：文件重定位
// 参数：fd -- 文件描述符；
//      offset -- 文件偏置；
//      whence -- 文件偏置的起点；
// 返回：成功（重定位后的位置）；失败（-1）；
// 备注：NOTE: lseek不支持文件大小拓展
// ============================================================================
off_t lseek(s32 fd, off_t offset, s32 whence)
{
    struct objhandle *hdl;
    s32 res;
    off_t result = -1;

    if((SEEK_SET != whence) && (SEEK_CUR != whence) && (SEEK_END != whence))
        return (-1); // 错误的参数

    if((SEEK_SET == whence) && (offset < 0))
        return (-1); // 错误的逻辑

    hdl = fd2Handle(fd);
    if(!hdl)
        return (-1);

    if(Handle_FlagIsDirectory(hdl->flags))
        return (-1); // 目录不可以seek

    res = hdl->HostObj->ObjOps((void *)hdl, CN_OBJ_CMD_SEEK, (ptu32_t)&result,
                             (ptu32_t)&offset, whence);
    if(res == CN_OBJ_CMD_EXECUTED)
        return (result);
    else
        return (-1);
}

// ============================================================================
// 功能：文件同步；
// 参数：fd -- 文件描述符；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 fsync(s32 fd)
{
    struct objhandle *hdl;
    s32 res;

    hdl = fd2Handle(fd);
    if(!hdl)
    {
        errno = EBADF; // 参数错误
        return (-1);
    }

    res = hdl->HostObj->ObjOps((void *)hdl, CN_OBJ_CMD_SYNC, 0, 0, 0);
    if(res == CN_OBJ_CMD_TRUE)
        return (0);
    else
        return (-1);
}

// ============================================================================
// 功能：获取文件的当前位置；
// 参数：fd -- 文件描述符；
// 返回：成功（文件当前位置）；失败（-1）；
// 备注：
// ============================================================================
off_t tell(s32 fd)
{
    struct objhandle *hdl;
    off_t offset;
    s32 res;

    hdl = fd2Handle(fd);
    if(!hdl)
    {
        errno = EBADF; // 参数错误
        return (-1);
    }

    res = hdl->HostObj->ObjOps((void *)hdl, CN_OBJ_CMD_TELL, (ptu32_t)&offset,0,0);
    if(res == CN_OBJ_CMD_EXECUTED)
        return (offset);
    else
        return -1;
}

// ============================================================================
//功能：查询文件状态；
//参数：fd -- 文件描述符；
//     buf -- 文件状态;
//返回：成功（0）; 失败（-1）;
//备注：
// ============================================================================
s32 fstat(s32 fd, struct stat *buf)
{
    s32 res;
    struct objhandle *hdl;

    if(NULL == buf)
        return(-1);

    hdl = fd2Handle(fd);
    if(!hdl)
        return (-1);

    res = hdl->HostObj->ObjOps((void *)(hdl->HostObj), CN_OBJ_CMD_STAT,
                                    (ptu32_t)buf, 0, 0);
    if(res == CN_OBJ_CMD_TRUE)
        return 0;
    else
        return (-1);
}

// ============================================================================
// 功能：查询文件状态；
// 参数：path -- 文件路径；
//      buf -- 文件状态;
// 返回：成功（0）; 失败（-1）;
// 备注：
// ============================================================================
s32 stat(const char *path, struct stat *buf)
{
    struct Object *ob;
    char *uncache;
    s32 res;

    if((!buf) || (!path))
        return(-1);

    __Handle_LockSys();// 防止操作过程文件被删除了
    ob = OBJ_MatchPath((char*)path, &uncache);
    OBJ_DutyUp(ob);
    __Handle_UnlockSys();

    if(!uncache)
        uncache = ""; // 全部路径都已经缓存时，设置为空字符串（即'\0'），用于与fstat逻辑区分；

    res = (s32)ob->ObjOps((void *)ob, CN_OBJ_CMD_STAT, (ptu32_t)buf, 0,
                                    (ptu32_t)uncache);
    OBJ_DutyDown(ob);
    if(res == CN_OBJ_CMD_TRUE)
        return 0;
    else
        return (-1);
}

// ============================================================================
// 功能：设定文件长度；
// 参数：fd -- 文件描述符；
//      length -- 文件长度；
// 返回：成功（0）; 失败（-1）;
// 备注：the file offset is not changed.
// ============================================================================
s32 ftruncate(s32 fd, off_t length)
{
    struct objhandle *hdl;
    s32 res;

    if(0 == length)
        return (0);

    hdl = fd2Handle(fd);
    if(!hdl)
        return (-1);

    res = hdl->HostObj->ObjOps((void *)hdl, CN_OBJ_CMD_TRUNCATE, 0,
                                (ptu32_t)&length, 0);
    if(res == CN_OBJ_CMD_TRUE)
        return (0);
    else
        return -1;
}

// ============================================================================
//功能：删除文件或目录
//参数：pathname -- 文件路径；
//返回：0 -- 成功; -1 -- 失败;
//备注:
// ============================================================================
s32 unlink(const char *pathname)
{
    if(remove(pathname))
        return (-1);

    return (0);
}

// ============================================================================
// 功能：打开目录；
// 参数：name -- 目录路径；
// 返回：成功（非NULL）；失败（NULL）；
// 备注：
// ============================================================================
DIR *opendir(const char *name)
{
    tagDIR *directory; // 即DIR结构体
    struct objhandle *hdl;

    if((NULL == name)  || ('\0' == *name))
        return (NULL);

    hdl = __open((char*)name, (O_DIRECTORY | O_RDWR), 0);
    if(!hdl)
    {
        return (NULL);
    }

    directory = malloc(sizeof(*directory));
    if(NULL == directory)
    {
        __close(hdl);
        return NULL;
    }
    else
    {
        memset(directory, 0, sizeof(*directory));
        directory->__fd = (void*)hdl;
        return (directory);
    }
}

// ============================================================================
// 功能：读目录项；
// 参数：dir -- 已打开的目录；
// 返回：目录项；全部已读返回NULL；
// 备注：每次的返回值，都是同一个空间，这样处理是为了省空间；
// ============================================================================
struct dirent *readdir(DIR *dir)
{
    struct objhandle *hdl;
    struct Object *ob;
    s32 res;

    if(!dir)
        return (NULL);

    hdl = (struct objhandle*)(dir->__fd); // 目录的上下文

    ob = hdl->HostObj;      // 目录的节点
    res = ob->ObjOps((void *)ob, CN_OBJ_CMD_READDIR, (ptu32_t)&dir->__ptr, 0, (ptu32_t)hdl);
    if(res == CN_OBJ_CMD_TRUE)
        return (struct dirent*)&(dir->__ptr);
    else
        return (NULL); // 已全部遍历完
}

// ============================================================================
// 功能：关闭目录
// 参数：dir -- 目录；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 closedir(DIR *dir)
{
    s32 res;

    res = __close(dir->__fd);
    if(res)
        return (-1);

    free(dir);

    return (0);
}

// ============================================================================
// 功能：创建目录
// 参数：filename -- 目录名（路径）；
//      mode -- 模式；
// 返回：成功（0）；失败（-1）；
// 备注：如果目录已存在，返回失败；
// ============================================================================
s32 mkdir(const char *filename, mode_t mode)
{
    struct objhandle *dir;

    dir = __open((char*)filename, (O_DIRECTORY | O_RDWR | O_CREAT | O_EXCL), mode);
    if(dir)
    {
        __close(dir);
        return (0);
    }

    return (-1);
}

// ============================================================================
// 功能：对象句柄处理的文件控制；
// 参数：hdl：对象句柄；
//      cmd：控制命令;
//      argspace：控制命令参数空间；
// 返回：未支持（-1）；成功（0）
// 备注：
// ============================================================================
static s32 __Handle_Cntl(struct objhandle *hdl, s32 cmd, va_list argspace)
{
    va_list args = argspace;

    switch(cmd)
    {
        case F_SETCONTEXT:
        {
            ptu32_t context;

            context = (ptu32_t)va_arg(args, u32);
            Handle_SetContext(hdl, context);
            return (CN_OBJ_CMD_EXECUTED);
        }

        case F_GETCONTEXT:
        {
            ptu32_t *ret;

            ret = (ptu32_t*)va_arg(args, u32);
            *ret =  Handle_GetContext(hdl);
            return (CN_OBJ_CMD_EXECUTED);
        }

        case F_GETFILE:
        {
            ptu32_t *ret;

            ret = (ptu32_t*)va_arg(args, u32);
            *ret = (ptu32_t)handle_GetHostObjectPrivate(hdl);
            return (CN_OBJ_CMD_EXECUTED);
        }

        case F_SETEVENT:
        {
            u32 events;

            events = va_arg(args, u32);
            Handle_SetMultiplexEvent(hdl, events);
            return (CN_OBJ_CMD_EXECUTED);
        }

        case F_CLREVENT:
        {
            u32 events;

            events = va_arg(args, u32);
            Handle_ClrMultiplexEvent(hdl, events);
            return (CN_OBJ_CMD_EXECUTED);
        }

        case F_SETTIMEOUT:
        {
            u32 timeout = va_arg(args, u32);
            hdl->timeout = timeout;
            return (CN_OBJ_CMD_EXECUTED);
        }

        case F_GETTIMEOUT:
        {
            u32 *timeout = va_arg(args, u32 *);
            *timeout = hdl->timeout;
//          *(u32*)OpsArgs1 = handle_gettimeout(hdl);
            return (CN_OBJ_CMD_EXECUTED);
        }

        default: break;
    }

    return (CN_OBJ_CMD_UNSUPPORT);
}

// ============================================================================
// 功能：文件控制；
// 参数：fd，文件描述符；
//      cmd，控制命令，如 fcntl.h 中的 F_DUPFD 等常量定义。
//      ...，控制命令参数；
// 返回：cmd = F_DUPFD：返回新的文件描述符。
//      cmd = F_GETFD、F_GETFL：返回相应标志
//      cmd = F_GETOWN：返回一个正的进程ID或负的进程组ID。
//      cmd = 其他：-1 = 出错，其他值 = 成功。
// ============================================================================
s32 fcntl(s32 fd, s32 cmd, ...)
{
    struct objhandle *hdl;
    s32 result;
    va_list args;
    s32 res;

    hdl = fd2Handle(fd);
    if(!hdl)
        return (-1);

    va_start(args, cmd);
    res = __Handle_Cntl(hdl, cmd, args);
    if(CN_OBJ_CMD_UNSUPPORT == res)
    {
        if((cmd == F_DUPFD)||(cmd == F_GETFD)||(cmd == F_GETFL)||(cmd == F_GETOWN))
        {
            res = hdl->HostObj->ObjOps(
                                    (void *)hdl, CN_OBJ_FCNTL,0,
                                    (ptu32_t)cmd,(ptu32_t)&result);
            if(res == CN_OBJ_CMD_UNSUPPORT)
                result = -1;
        }
        else
        {
            res = hdl->HostObj->ObjOps((void *)hdl, CN_OBJ_FCNTL,0,
                                    (ptu32_t)cmd,(ptu32_t)&args);
            if((res == CN_OBJ_CMD_UNSUPPORT) || (res == CN_OBJ_CMD_FALSE))
                result = -1;
            else
                result = 0;
        }
    }
    else
        result = res;

    va_end (args);

    return result;
}

// ============================================================================
// 功能：IO控制；
// 参数：fd，文件描述符；
//      request，控制命令码，POSIX也没有规定，Linux格式为：方向(2bit)，第三个参数
//      （若有）数据尺寸(14bit)，设备类型 (8it) ，命令编码(8bit)），命令码如 stropts.h
//      中的 I_ATMARK 等常量定义
//      ...,控制命令参数；
// 返回：0 = 成功，-1 = 出错
// 备注：
//      最新的POSIX已经废弃了ioctl函数，而然并卵，大量开源项目仍然在用。
//      注：奇怪的POSIX，ioctl相关的常量，头文件名是 stropts.h 而不是 ioctl.h
//      可变参数写法，完全是为了兼容POSIX，事实上从来没有人用过第四个参数，三个参数不
//      够用的，就用结构指针。
//      POSIX并没有规定参数 request 如何实现，只定义了一些必须的符号名。
//      实际实现中，有3类。
//      A：Linux及newlib的实现，request参数是个组合，其方法如下：
//      1、如果第三个参数是简单类型（指针除外），则在request中体现了该类型的尺寸。
//      2、如果第三个参数是结构指针，则在request中体现了该结构的尺寸。
//      3、如果有第四个参数，对不起，懵逼。
//      故，Linux和newlib实际上是不支持第四个参数的。
//      B：VxWorks，采用了单一常数而非组合的方式。
//      C：有些开源项目，如三星的iotjs，自定义了一些ioctl命令常量，A和B方案都有。
//      djyos自有组件实现方案：
//      1、使用单一常数方案；
//      2、系统自带的模块，不使用第四个参数，三个参数不够用的，用结构指针
//      3、目前尚未见到使用第四个参数的开源代码，djyos实际上不考虑支持。
//      4、djyos的IO系统只提供接口，实际是否支持ioctl的功能，由组件实现者自行决定。
// ============================================================================
s32 ioctl(s32 fd,s32 request, ... )
{
    struct objhandle *hdl;
    va_list args;
    s32 res;

    hdl = fd2Handle(fd);
    if(!hdl)
        return (0);
    va_start(args, request);

    res = hdl->HostObj->ObjOps((void *)hdl, CN_OBJ_IOCTL,0,
                                    (ptu32_t)request,(ptu32_t)&args );
    va_end (args);
    if((res == CN_OBJ_CMD_TRUE) || (res == CN_OBJ_CMD_EXECUTED))
        return 0;
    else
        return -1;

}

// ============================================================================
// 功能：将当前工作目录的绝对路径复制到参数buf；
// 参数：buf -- 存放路径的空间；
//      size -- buf的大小；
// 返回：成功（当前工作目录的绝对路径）；失败（NULL）
// 备注：buf为 NULL并使 size为零，则本函数会动态申请一个空间来返回给用户，但是用户需要注意将空间释放；
// ============================================================================
char *getcwd(char *buf, size_t size)
{
    u32 len = 0, offset;
    struct Object *ob = OBJ_GetCurrent();

    if(((buf) && (!size)) || ((!buf) && (size)))
        return (NULL);

    while(ob)
    {
        len += strlen((char*)OBJ_GetName(ob)) + 1;
        if(ob==OBJ_GetRoot())
            break;

        ob = OBJ_GetParent(ob);
    }

    len += 1; // 绝对路径的长度
    if((size) && (size < len))
        return (NULL);

    if(!buf)
    {
        buf = malloc(len);
        if(!buf)
            return (NULL);
    }

    offset = len -1;
    ob = OBJ_GetCurrent();
    buf[offset] = '\0';
    while(ob)
    {
        len = strlen((char*)OBJ_GetName(ob));
        offset -= len;
        memcpy(buf+offset, (char*)OBJ_GetName(ob), len);
        buf[--offset] = '/';
        if(ob==OBJ_GetRoot())
            break;

        ob = OBJ_GetParent(ob);
    }

    return (buf);
}

// ============================================================================
// 功能：改变当前工作目录；
// 参数：path -- 新的当前工作目录
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 chdir(const char *path)
{
    path = path;
    return (-1); // TODO; NOTE:设置的路径是一个目录
}

#if 0
// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
#define __CONTENT_SIZE          512
struct __context
{
    u16 cpos; // 如果大于__CONTENT_SIZE，表示由SEEEK引起的尺寸扩大；
    u16 cindex; // 从零计算；
    u8 *cdata;
    struct __content *content;
};

struct __content
{
    u8 *data;
    u32 owners;
    list_t list;
};

enum __ramtype
{
    RAM_REG,
    RAM_DIR,
};

struct __ramfile
{
    enum __ramtype type;
    void *u; // 指向具体文件类型；只有文件才有内容，其他为NULL
};

struct __ramreg
{
    u32 sz;
    struct __content *content;
};


// ============================================================================
// 功能：增加引用文件内容；
// 参数：content -- 文件；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 __rf_up_content(struct __content *content)
{
    if(-1==content->owners)
        return (-1);

    content->owners ++;
    return (0);
}


// ============================================================================
// 功能：减少引用文件内容；
// 参数：content -- 文件；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 __rf_down_content(struct __content *content)
{
    if(!content->owners)
        return (-1);

    content->owners --;
    return (0);
}

// ============================================================================
// 功能：减少引用文件内容；
// 参数：content -- 文件；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 __rf_del_content(struct __content *content)
{
    if(content->owners)
        return (-1);

    free(content);
    return (0);
}

// ============================================================================
// 功能：扩展文件内容空间；
// 参数：reg -- 文件；
//      num -- 文件内容空间数量；
// 返回：成功（最后一个内容空间）；失败（NULL）；
// 备注：
// ============================================================================
struct __content *__rf_extend(struct __ramreg *reg, u16 num)
{
    struct __content *content, *extend;

    while(num)
    {
        extend = malloc(sizeof(*content)+__CONTENT_SIZE);
        if(!extend)
        {
            return (NULL);
        }

        extend->data = (u8*)extend + sizeof(*content);
        memset(extend->data, 0x0, __CONTENT_SIZE);
        if(reg->content)
        {
            dListInsertBefore(&(reg->content->list), &extend->list);
        }
        else
        {
            dListInit(&extend->list);
            reg->content = extend;
        }

        num --;
    }

    if(__rf_up_content(content))
        return (NULL);
    else
        return (extend);
}

// ============================================================================
// 功能：缩减文件内容空间；
// 参数：file -- 文件；
//      size -- 缩减到的尺寸；
// 返回：成功（最后一个内容空间,缩减为零，则为NULL）；失败（-1）；
// 备注：
// ============================================================================
struct __content *__rf_reduce(struct __ramreg *reg, u32 size)
{
    struct __content *tmp;
    list_t *last;
    u32 delsz = reg->sz - size;; // 需要缩减的尺寸；

    if(delsz<=0)
    {
        return (NULL); // 逻辑不对；
    }

    last = dListGetBefore(&(reg->content->list)); // 获取最后一个content
    tmp = dListEntry(last, struct __content, list);
    if(delsz<(reg->sz%__CONTENT_SIZE))
    {
        // 缩减的尺寸小于最后一个content的空间；
        reg->sz -= delsz;
        memset(tmp->data+(reg->sz%__CONTENT_SIZE), 0x0, delsz);
        return (tmp);
    }
    else
    {
        // 缩减的尺寸大于最后一个content的空间；
        last = dListGetBefore(last);
        if(__rf_del_content(tmp))
            return ((struct __content*)-1);

        delsz -= reg->sz % __CONTENT_SIZE;
        reg->sz -= (reg->sz % __CONTENT_SIZE);
    }

    while(delsz)
    {
        tmp = dListEntry(last, struct __content, list);
        if(delsz<__CONTENT_SIZE)
        {
            reg->sz -= delsz;
            memset(tmp->data+(reg->sz%__CONTENT_SIZE), 0x0, delsz);
            break;
        }
        else
        {
            last = dListGetBefore(last);
            if(__rf_del_content(tmp))
                return ((struct __content*)-1);

            delsz -= __CONTENT_SIZE;
            reg->sz -= __CONTENT_SIZE;
        }
    }

    if(reg->sz)
    {
        return (dListEntry(last, struct __content, list));
    }

    return (NULL);
}


// ============================================================================
// 功能：获取文件内容；
// 参数：file -- 文件；
//      index -- 文件内容序列；从零计；
// 返回：成功（文件内容）；失败（NULL）；
// 备注：
// ============================================================================
struct __content *__rf_content(struct __ramreg *reg, s16 index)
{
    struct __content *content;
    list_t *nxt, *fst;

    if(!index)
        return (reg->content); // 查找的是第0个content

    fst = &(reg->content->list);
    nxt = dListGetAfter(fst);
    while(1)
    {

        if(nxt==fst)
            return (NULL); // 未找到需要的index

        index --;
        if(!index)
        {
            content = dListEntry(nxt, struct __content, list);
            if(__rf_up_content(content))
                return (content); // 找到需要的content
            else
                return (NULL);
        }

        nxt = dListGetAfter(nxt);
    }

    // 不合法的逻辑，查找的content不存在
    return (NULL);
}


// ============================================================================
// 功能：打开文件；
// 参数：ob -- 文件对象；
//      flags -- 打开标志；
//      name -- 文件名；
// 返回：成功（文件句柄）；失败（NULL）；
// 备注：
// ============================================================================
static struct objhandle *__rf_open(struct Object *ob, u32 flags, char *name)
{
    struct objhandle *hdl;
    struct __ramfile *file = NULL;
    struct __ramreg *reg = NULL;
    struct __context *cx = NULL;

    if(name) // 需要新建文件或目录；
    {
        if(!Handle_FlagIsCreate(flags))
            return (NULL); // 逻辑上不要求创建，但实际文件却不存在；

        if(Handle_FlagIsRegular(flags)) // 创建文件；
        {
            file = malloc(sizeof(*file)+sizeof(struct __ramreg));
            if(!file)
            {
                debug_printf("ramfs", "open failed(memory out).");
                return (NULL);
            }

            file->type = RAM_REG;
            file->u = (u8*)file + sizeof(*file);
            memset(file->u, 0x0, sizeof(struct __ramreg));
            cx = malloc(sizeof(*cx));
            if(!cx)
            {
                free(file);
                debug_printf("ramfs", "open failed(memory out).");
                return (NULL);
            }

            memset(cx, 0x0, sizeof(*cx));
        }
        else // 创建非文件；非文件没有提供上下文；
        {
            file = malloc(sizeof(*file));
            if(!file)
            {
                debug_printf("ramfs", "open failed(memory out).");
                return (NULL);
            }

            file->type = RAM_DIR;
            file->u = NULL;
        }

        ob = OBJ_NewChild(ob, NULL, (ptu32_t)file, name);
        if(!ob)
        {
            debug_printf("ramfs", "open failed(object create).");
            free(file);
            free(cx);
            return (NULL);
        }

        if(!Handle_FlagIsRegular(flags))
        {
//          if(obj_allow2set(ob)) // 允许目录对象之上建立对象集合；比如在目录之上挂载文件系统；
            {
                free(reg);
                OBJ_Delete(ob);
                return (NULL);
            }
        }
    }
    else // 文件或目录已存在；
    {
        if(Handle_FlagIsOnlyCreate(flags))
            return (NULL); // 逻辑上不合要求；必须创建，但已存在；

        file = (struct __ramfile*)OBJ_GetPrivate(ob);
        if(Handle_FlagIsRegular(flags)) // 打开文件；
        {
            if(file->type!=RAM_REG)
                return (NULL);
            reg = (struct __ramreg*)file->u;
            cx = malloc(sizeof(*cx));
            if(!cx)
            {
                debug_printf("ramfs", "open failed(memory out).");
                return (NULL);
            }

            memset(cx, 0x0, sizeof(*cx));
            if(Handle_FlagIsAppend(flags))
            {
                cx->cpos = reg->sz;
                cx->content = __rf_content(reg, (reg->sz/__CONTENT_SIZE));
                if(!cx->content)
                {
                    debug_printf("ramfs", "open failed(no content).");
                    free(cx);
                    return (NULL);
                }
            }
            else if(Handle_FlagIsTrunc(flags))
            {
                cx->content = __rf_reduce(reg, reg->sz);
                if(!cx->content)
                {
                    debug_printf("ramfs", "open failed(truncate).");
                    free(cx);
                    return (NULL);
                }
            }
        }
        else // 打开非文件；
        {
            if(file != NULL)
            {
                if((Handle_FlagIsDirectory(flags))&&(file->type!=RAM_DIR))
                    return (NULL);
            }
        }
    }

    hdl = Handle_New();
    if(!hdl)
    {
        printf("\r\n: dbug : ramfs  : open failed(context).");
        if(reg)
           free(reg);
    }

    Handle_Init(hdl, ob, flags, (ptu32_t)cx);
    return (hdl);
}

// ============================================================================
// 功能：关闭文件；
// 参数：hdl -- 文件对象句柄；
// 返回：
// 备注：
// ============================================================================
static s32 __rf_close(struct objhandle *hdl)
{
    struct __context *cx = (struct __context*)Handle_GetContext(hdl);

    if(cx) // 目录没有__context;
    {
        __rf_down_content(cx->content);
        free(cx);
    }

    Handle_Delete(hdl);
    return (0);
}

// ============================================================================
// 功能：删除文件；
// 参数：
// 返回：
// 备注：删除的时候，对象已经被锁住，不会有新的访问；
// ============================================================================
static s32 __rf_remove(struct Object *ob)
{
    struct __ramfile *file = (struct __ramfile*)OBJ_GetPrivate(ob);

    if(RAM_REG==file->type)
    {
        if(__rf_reduce((struct __ramreg*)(file->u), 0))
        {
            return (-1);
        }

        free(file);
    }

    OBJ_Delete(ob);
    return (0);
}

// ============================================================================
// 功能：写文件；
// 参数：hdl -- 文件对象句柄；
//      data -- 写数据；
//      size  -- 写尺寸；
// 返回：成功写入内容的尺寸；
// 备注：
// ============================================================================
static s32 __rf_write(struct objhandle *hdl, u8 *data, u32 size)
{
    struct __ramfile *file = (struct __ramfile*)handle_GetHostObjectPrivate(hdl);
    struct __context *cx = (struct __context*)Handle_GetContext(hdl);
    struct __ramreg *reg;
    s32 once, extend, left = size;
    struct __content *tmp;

    if(RAM_REG!=file->type)
        return (0); // 目录不可写；

    reg = (struct __ramreg*)file->u;
    if(Handle_IsAppend(hdl))
    {
        if((cx->cindex*__CONTENT_SIZE+cx->cpos)!=reg->sz)
        {
            if(cx->cindex!=(reg->sz/__CONTENT_SIZE))
            {
                tmp = __rf_content(reg, (reg->sz/__CONTENT_SIZE));
                if(!tmp)
                {
                    return (0);
                }

                cx->content = tmp;
                cx->cindex = reg->sz/__CONTENT_SIZE;
            }

            cx->cpos = reg->sz % __CONTENT_SIZE;
        }
    }

    if(cx->cpos>__CONTENT_SIZE) // 由seek引起的扩展；
    {
        extend = cx->cpos / __CONTENT_SIZE;
        tmp = __rf_extend(reg, extend);
        if(tmp)
        {
            return (0);
        }

        if(__rf_down_content(cx->content))
        {
            __rf_down_content(tmp);
            return (0);
        }

        cx->content = tmp;
        cx->cpos = cx->cpos % __CONTENT_SIZE;
    }

    while(left)
    {
        once = __CONTENT_SIZE - cx->cpos; // 剩余可写空间；
        if(left<once)
            once = left;

        if(once)
        {
            memcpy(cx->content->data+cx->cpos, data, once);
            left -= once;
            cx->cpos += once;
        }

        if(left)
        {
            tmp = __rf_extend(reg, 1);
            if(!tmp)
                break;

            if(__rf_down_content(cx->content))
            {
                __rf_down_content(tmp);
                break;
            }

            cx->content = tmp;
            cx->cpos = 0;
            cx->cindex ++;
        }
    }

    return ((s32)(size-left));
}

// ============================================================================
// 功能：读文件；
// 参数：hdl -- 文件对象句柄；
//      data -- 读数据；
//      size  -- 读尺寸；
// 返回：成功读出内容的尺寸；
// 备注：
// ============================================================================
static s32 __rf_read(struct objhandle *hdl, u8 *data, u32 size)
{
    struct __ramfile *file = (struct __ramfile*)handle_GetHostObjectPrivate(hdl);
    struct __context *cx = (struct __context*)Handle_GetContext(hdl);
    struct __ramreg *reg;
    s32 left, once, cfree;
    u8 *tmp;

    if(RAM_REG!=file->type)
        return (0); // 目录不可写；

    reg = (struct __ramreg*)file->u;
    left = reg->sz - ((cx->cindex * __CONTENT_SIZE) + cx->cpos);
    if(size>left)
        size = left;

    left = size;
    while(left)
    {
        if((reg->sz/__CONTENT_SIZE)>cx->cindex)
        {
            once = __CONTENT_SIZE - cx->cpos;
        }
        else
        {
            once = reg->sz % __CONTENT_SIZE - cx->cpos;
        }

        if(once<left)
            once = left;

        memcpy(data, (cx->content->data + cx->cpos), once);
        left -= once;
        data += once;
        if(left)
        {
            tmp = __rf_content(reg, (cx->cindex+1)); // 获取下一部分内容；
            if(!tmp)
            {
                break;
            }

            if(__rf_down_content(cx->content))
            {
                __rf_down_content(tmp);
                break;
            }
            cx->content = tmp;
            cx->cpos = 0;
            cx->cindex ++;
        }
        else
        {
            cx->cpos += once;
        }
    }

    return (size-left);
}

// ============================================================================
// 功能：读目录项；
// 参数：
// 返回：全部读完（1）；失败（-1）；读了一项（0）；
// 备注：
// ============================================================================
static s32 __rf_readdentry(struct objhandle *hdl, struct dirent *dentry)
{
    struct Object *ob = (struct Object *)dentry->d_ino;

    if(!ob) // 第一次读；
    {
        ob = OBJ_GetChild(Handle_GetHostObj(hdl));
        if(!ob)
            return (1); // 没有子项目；
    }
    else // 后续读；
    {
        ob = OBJ_GetNext(ob);
        if(ob==OBJ_GetChild(Handle_GetHostObj(hdl)))
            return (1); // 全部读完；
    }

    if(!OBJ_GetPrivate(ob))
        dentry->d_type = DIRENT_IS_DIR;
    else
        dentry->d_type = DIRENT_IS_REG;

    strcpy(dentry->d_name, OBJ_GetName(ob));
    dentry->d_ino = (long)ob;
    return (0);
}

// ============================================================================
// 功能：查文件状态；
// 参数：ob -- 文件对象；
//      data -- 文件对象状态；
// 返回：成功（0）；
// 备注：
// ============================================================================
static s32 __rf_stat(struct Object *ob, struct stat *data)
{
    struct __ramfile *file = (struct __ramfile*)OBJ_GetPrivate(ob);
    struct __ramreg *reg;

    if(RAM_REG==file->type) // 文件
    {
        reg = (struct __ramreg*)file->u;
        data->st_size = reg->sz; //
        data->st_mode = S_IFREG|S_IRWXUGO; //
    }
    else // 目录
    {
        data->st_size = 0 ; //
        data->st_mode = S_IFDIR; //
    }

    return (0);
}

// ============================================================================
// 功能：设置文件当前位置；
// 参数：hdl -- IAP文件对象句柄；
//      offset -- 移动位置的量；
//      whence -- 移动位置的起点；
// 返回：成功（0，*pOffset指向新的当前位置）；失败（-1）；
// 备注：
// ============================================================================
static s32 __rf_seek(struct objhandle *hdl, s64 *offset, s32 whence)
{
    struct __ramfile *file = (struct __ramfile*)handle_GetHostObjectPrivate(hdl);
    struct __ramreg *reg;
    struct __context *cx;
    s32 npos, movs, pos, bufed;
    s16 nindex;
    u8 *tmp;

    if(RAM_REG!=file->type)
        return (-1); // 非文件无法seek；

    reg = (struct __ramreg*)file->u;
    cx = (struct __context*)Handle_GetContext(hdl);
    pos = cx->cindex * __CONTENT_SIZE + cx->cpos;
    bufed = reg->sz - pos;
    switch(whence)
    {
        case SEEK_END: // 转为从头SEEK逻辑
        {
            *offset = reg->sz + *offset;
            if(*offset<0)
                *offset = 0; // 新位置越界了
        }

        case SEEK_SET: // 转为当前位置的SEEK的逻辑
        {
            *offset = *offset - pos;
            if((*offset+pos)<0)
                *offset = pos; // 新位置越界了
        }

        case SEEK_CUR:
        {
            npos = pos + *offset;
            if(npos<0)
                npos = 0;

            movs = npos - pos;
            if(movs>0)
            {
                if(movs<=bufed)
                {
                    if(movs>(__CONTENT_SIZE-cx->cpos))
                    {
                        nindex = npos / __CONTENT_SIZE;
                        if(npos % __CONTENT_SIZE)
                            nindex += 1;

                        tmp = __rf_content(reg, nindex);
                        if(tmp)
                        {
                            return (-1);
                        }

                        cx->cindex = nindex;
                        cx->cpos = npos % __CONTENT_SIZE;
                        cx->cdata = tmp;
                    }
                    else
                    {
                        cx->cpos += movs;
                    }
                }
                else // 超过当前文件尺寸
                {
                    cx->cpos += movs;
                }
            }
            else if(movs<0) //
            {
                movs = abs(movs);
                if(cx->cpos<movs)
                {
                    nindex = npos / __CONTENT_SIZE;
                    if(npos % __CONTENT_SIZE)
                        nindex += 1;

                    tmp = __rf_content(reg, nindex);
                    if(tmp)
                    {
                        return (-1);
                    }

                    cx->cindex = nindex;
                    cx->cpos = npos % __CONTENT_SIZE;
                    cx->cdata = tmp;
                }
            }

        }
    }

    *offset = pos;
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 __rf_operations(void *opsTarget, u32 objcmd, ptu32_t OpsArgs1,
                            ptu32_t OpsArgs2, ptu32_t OpsArgs3)
{
    va_list list;
    s32 result = CN_OBJ_CMD_EXECUTED;

    switch(objcmd & CN_OBJ_CMD_MSK)
    {
        case CN_OBJ_CMD_OPEN:
        {
            struct objhandle *hdl;
            hdl = __rf_open((struct Object *)opsTarget, (u32)(*(u64*)OpsArgs2), (char*)OpsArgs3);
            (struct objhandle *)OpsArgs1 = hdl;
            break;
        }

//      case OBJ_TRAVERSE:
//      {
//          struct objhandle *hdl = (struct objhandle*)opsTarget;
//          struct dirent *ret = (struct dirent *)OpsArgs1;
//
//          return(__rf_readdentry(hdl, ret));
//      }

        case CN_OBJ_CMD_READ:
        {
            ssize_t len;
            struct objhandle *devfile = (struct objhandle*)opsTarget;

            len = __rf_read(devfile, (u8*)OpsArgs2, (ssize_t)OpsArgs3);
            *(ssize_t *)OpsArgs1 = len;
            break;
        }

        case CN_OBJ_CMD_WRITE:
        {
            ssize_t len;
            struct objhandle *devfile = (struct objhandle*)opsTarget;

            len = __rf_write(devfile, (u8*)OpsArgs2, (ssize_t)OpsArgs3);
            *(ssize_t *)OpsArgs1 = len;
            break;
        }

        case CN_OBJ_CMD_CLOSE:
        {
            __rf_close((struct objhandle*)opsTarget);
            result = CN_OBJ_CMD_TRUE;
            break;
        }

        case CN_OBJ_CMD_STAT:
        {
            char *path = (char*)OpsArgs2;
            if(path&&('\0'!=*path))
                return (-1);    // 查询的文件不存在；ramfs的文件都是缓存了的；
            __rf_stat((struct Object*)opsTarget, (struct stat *)OpsArgs1);
            result = CN_OBJ_CMD_TRUE;
            break;
        }

        case CN_OBJ_CMD_DELETE:
        {
            *(s32*)OpsArgs1 = __rf_remove((struct Object*)opsTarget);
            result = CN_OBJ_CMD_TRUE;
            break;
        }

        default:
        {
            return CN_OBJ_CMD_UNSUPPORT;
            break;
        }
    }

    return result;
}
#endif
