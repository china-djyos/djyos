//-----------------------------------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
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
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <stat.h>
#include <pool.h>
#include <systime.h>
#include <lock.h>
#include <multiplex.h>
#include "__object.h"
#include "../include/object.h"
#include "../include/objfile.h"
#include "dbug.h"
#define CN_BASIC_OFILES               32 // 文件预分的数量；（动态扩展）

static struct MutexLCB *ofmutex; // 文件系统互斥锁
static struct MemCellPool ofpool; // 文件预分配池

// 控制文件对象的是指针，但open返回的fd是s32，两者需要有对应关系
// of 与 fd 天然能够互相直接转化的条件：
// 1、指针长度小于32
// 2、指针长度==32，但字节位宽＜32
#if(( CN_PTR_BITS < 32)  || ( (CN_PTR_BITS == 32) && (CN_BYTE_BITS < 32)))
// ============================================================================
// 功能：内部文件描述转文件句柄
// 参数：
// 返回：
// 备注：
// ============================================================================
s32 ofno(tagOFile *of)
{
    if(!of)
        return (-1);

    return (s32)(((u32)of) >> 1);
}

// ============================================================================
// 功能：文件句柄转文件描述
// 参数：
// 返回：
// 备注：
// ============================================================================
inline tagOFile *of(s32 fd)
{
    extern tagOFile *STDIO_Get(u32 dwType);

    if(-1 == fd)
        return (NULL);

    if((fd <= 2) && (fd >= 0))
        return (STDIO_Get(fd));
    else
        return ((tagOFile*)(((u32)fd) << 1));
}
#else
// 不能天然相互转换，情况就复杂多了。
// 考虑限制object所使用的内存空间范围，只要限制在2G以内，就可以互转了。
#error FD logic is not supported.
#endif

// ============================================================================
// 功能：文件上下文空间池初始化
// 参数：
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 __of_initbasicpool(void)
{
    static tagOFile ofbasicspace[CN_BASIC_OFILES];

    if(Mb_CreatePool_s(&ofpool, ofbasicspace,
                CN_BASIC_OFILES, sizeof(tagOFile), 16, 16384, "object file"))
        return (0);

    return (-1);
}

// ============================================================================
// 功能：新建文件上下文
// 参数：
// 返回：成功（文件上下文）；失败（NULL）；
// 备注：
// ============================================================================
tagOFile *of_new(void)
{
    tagOFile *of;

    of = (tagOFile*)Mb_Malloc(&ofpool, 0);
    if(!of)
        return (NULL);

    memset(of, 0x0, sizeof(*of));
    dListInit(&of->list);
    return (of);
}

// ============================================================================
// 功能：释放文件上下文
// 参数：of -- 文件上下文；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 of_free(tagOFile *of)
{
    if(of)
    {
        dListRemove(&of->list);
        Mb_Free(&ofpool, of);
        return (0);
    }

    return (-1);
}

// ============================================================================
// 功能：文件上下文初始化
// 参数：of -- 文件上下文；
//      obj -- 对象；
//      flags -- 文件使用标志；
//      context -- 文件实际上下文；
// 返回：
// 备注：TODO :INLINE
// ============================================================================
inline void of_init(tagOFile *of, struct Object *obj, u32 flags, ptu32_t context)
{
    of->obj = obj;
    of->context = context;
    of->flags = flags;
}

// ============================================================================
// 功能：
// 参数：target -- 安装点；
//      pTargetRoot -- 安装点的根路径；
//      ops -- 操作方式；
//      pPrivate -- 安装节点的表示；
// 返回：
// 备注：
// ============================================================================
struct Object *__mounto(const char *target, char *pTargetRoot, tagObjOps ops, ptu32_t represent)
{
    struct Object *obj;
    char *unfinish; //

    obj = OBJ_MatchPath(pTargetRoot, &unfinish, 0);
    if(unfinish)
    {
        return (NULL); // 安装点的根路径不能完成
    }

    if(__OBJ_NameSantiy(target))
    {
        return (NULL);
    }

    obj = OBJ_AddChild(obj, ops, represent, target);
    __OBJ_SetMountPoint(obj);

    return (obj);
}

// ============================================================================
// 功能：获取文件名
// 参数：of -- 文件下文；
// 返回：存在文件（1）；不存在文件（0）；
// 备注：INLINE
// ============================================================================
inline const char *of_name(tagOFile *of)
{
    if(!of)
        return (NULL);

    return (__OBJ_Name(of->obj));
}

// ============================================================================
// 功能：是否内置文件
// 参数：flags -- 文件使用标志；
// 返回：内置文件（1）；非内置文件（0）；
// 备注：INLINE
// ============================================================================
inline s32 testcontent(u32 flags)
{
    if(flags & O_FILE_BASIC)
        return (1);

    return (0);
}

// ============================================================================
// 功能：是否目录
// 参数：flags -- 文件使用标志；
// 返回：目录（1）；非目录（0）；
// 备注：INLINE
// ============================================================================
inline s32 testdirectory(u32 flags)
{
    if(flags & O_DIRECTORY)
        return (1);

    return (0);
}

// ============================================================================
// 功能：是否文件
// 参数：flags -- 文件使用标志；
// 返回：文件（1）；非文件（0）；
// 备注：INLINE
// ============================================================================
inline s32 testregular(u32 flags)
{
    if(flags & O_DIRECTORY)
        return (0);

    return (1);
}

// ============================================================================
// 功能：是否创建或打开；
// 参数：flags -- 文件使用标志；
// 返回：创建或打开（1）；非创建或打开（0）；
// 备注：INLINE
// ============================================================================
inline s32 testcreat(u32 flags)
{
    if(flags & O_CREAT)
        return (1);

    return (0);
}

// ============================================================================
// 功能：是否仅创建；
// 参数：flags -- 文件使用标志；
// 返回：仅创建（1）；非仅创建（0）；
// 备注：INLINE
// ============================================================================
inline s32 testonlycreat(u32 flags)
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
inline s32 testappend(u32 flags)
{
    if(flags & O_APPEND)
        return (1);

    return (0);
}

// ============================================================================
// 功能：是否截断
// 参数：flags -- 文件使用标志；
// 返回：截断（1）；非截断（0）；
// 备注：INLINE
// ============================================================================
inline s32 testtrunc(u32 flags)
{
    if(flags & (O_TRUNC))
        return (1);

    return (0);
}

// ============================================================================
// 功能：是否可读
// 参数：flags -- 文件使用标志；
// 返回：可读（1）；非可读（0）；
// 备注：INLINE
// ============================================================================
inline s32 testreadable(u32 flags)
{
    if(flags & O_RDONLY)
        return (1);

    return (0);
}

// ============================================================================
// 功能：是否可写
// 参数：flags -- 文件使用标志；
// 返回：可写（1）；非可写（0）；
// 备注：INLINE
// ============================================================================
inline s32 testwriteable(u32 flags)
{
    return (flags & O_WRONLY);
}

// ============================================================================
// 功能：文件是否是目录
// 参数：
// 返回：目录（1）；非目录（0）；
// 备注：
// ============================================================================
inline s32 isdirectory(tagOFile *of)
{
    return(testdirectory(of->flags));
}

// ============================================================================
// 功能：文件节点是否是目录
// 参数：
// 返回：目录（1）；非目录（0）；
// 备注：
// ============================================================================
s32 isdirectoryo(struct Object *obj)
{
    struct __portBasic *basic;

    basic = dListEntry(obj->port.next, struct __portBasic, list);
    return (testdirectory(basic->flags));
}

// ============================================================================
// 功能：文件是否追加模式；
// 参数：flags -- 文件使用标志；
// 返回：追加（1）；非追加（0）；
// 备注：INLINE
// ============================================================================
inline s32 isappend(tagOFile *of)
{
    return (testappend(of->flags));
}

// ============================================================================
// 功能：文件是否可读
// 参数：
// 返回：
// 备注：
// ============================================================================
inline s32 isreadable(tagOFile *of)
{
    return (testreadable(of->flags));
}

// ============================================================================
// 功能：文件是否可写
// 参数：
// 返回：
// 备注：
// ============================================================================
inline s32 iswritable(tagOFile *of)
{
    return (testwriteable(of->flags));
}

// ============================================================================
// 功能：测试文件是否正在被使用
// 参数：
// 返回：文件使用中（1）；文件未使用（0）
// 备注：
// ============================================================================
s32 isondutyo(struct Object *obj)
{
    s32 links;
    struct __portBasic *basic;

    if(!obj)
        return (0);

    if(__OBJ_IsProtected(obj))
        return (1);

    links = __OBJ_LinksCheck(obj, 2);
    if(-1 == links)
        return (1);

    if(1 == links)
    {
        basic = of_basiclinko(obj);
        if(testcontent(basic->flags))
            return (0);

        return (1);
    }

    return (0);
}

// ============================================================================
// 功能：文件是否存在竞争者，即文件被其他地方同时打开
// 参数：of -- 文件；
// 返回：存在（1）；不存在（0）；
// 备注：INLINE
// ============================================================================
s32 iscontender(tagOFile *of)
{
    s32 links;
    struct __portBasic *basic;

    links = __OBJ_LinksCheck(of->obj, 2);
    if(-1 == links)
        return (1);

    if(2 == links)
    {
        basic = of_basiclinko(of->obj);
        if(testcontent(basic->flags))
            return (0);

        return (1);
    }

    return (0);
}

// ============================================================================
// 功能：获取文件私有上下文；
// 参数：of -- 文件；
// 返回：文件私有上下文；
// 备注：内部调用，不判断参数；
// ============================================================================
inline ptu32_t of_context(tagOFile *of)
{
    if(of)
        return (of->context);

    return (0);
}

// ============================================================================
// 功能：设置文件私有上下文；
// 参数：of -- 文件；
//      context -- 私有上下文；
// 返回：失败（-1）；成功（0）
// 备注：
// ============================================================================
inline void of_setcontext(tagOFile *of, ptu32_t context)
{
    if(of)
        of->context = context;
}

// ============================================================================
// 功能：获取文件表示；
// 参数：of -- 文件；
// 返回：文件表示
// 备注：内部调用，不判断参数；
// ============================================================================
inline ptu32_t of_represent(tagOFile *of)
{
    if(of&&of->obj)
        return (of->obj->represent);

    return (0);
}

// ============================================================================
// 功能：设置文件表示；
// 参数：of -- 文件；
// 返回：文件表示
// 备注：内部调用，不判断参数；
// ============================================================================
inline void of_setrepresent(tagOFile *of, ptu32_t represent)
{
    if(of&&of->obj)
        of->obj->represent = represent;
}

// ============================================================================
// 功能：将文件与文件节点关联
// 参数：
// 返回：
// 备注： TODO:互斥
// ============================================================================
s32 of_linko(tagOFile *of, struct Object *obj)
{
    if(dListIsEmpty(&of->list))
    {
        // obj含有content链成员（第三方文件系统没有），obj上下文链接在尾部
        dListInsertBefore(&obj->port, &of->list);
    }
    else if(dListIsEmpty(&obj->port))
    {
        // pOF含有content链成员，obj是新建的，其list的第一个成员必须是ocntent
        dListInsertAfter(&of->list, &obj->port);
    }
    else
    {
        return (-1); // 不符合设计逻辑
    }

    of->obj = obj;
    return (0);
}

// ============================================================================
// 功能：获取文件的多路复用；
// 参数：
// 返回：
// 备注：
// ============================================================================
inline struct MultiplexObjectCB *of_mulitplex(tagOFile *of)
{
    if(of)
        return (of->mphead);

    return (NULL);
}

// ============================================================================
// 功能：获取文件的多路复用；
// 参数：
// 返回：
// 备注：
// ============================================================================
inline void of_setmulitplex(tagOFile *of, struct MultiplexObjectCB *cb)
{
    if(of)
        of->mphead = cb;
}

// ============================================================================
// 功能：获取文件的超时设置；
// 参数：
// 返回：
// 备注：
// ============================================================================
inline u32 of_timeout(tagOFile *of)
{
    if(of)
        return (of->timeout);

    return (0);
}

// ============================================================================
// 功能：设置文件的超时；
// 参数：
// 返回：
// 备注：
// ============================================================================
inline void of_settimeout(tagOFile *of, u32 timeout)
{
    of->timeout = timeout;
}


// ============================================================================
// 功能：获取文件的使用标志；
// 参数：
// 返回：
// 备注：
// ============================================================================
inline u32 of_mode(tagOFile *of)
{
    if(of)
        return (of->flags);

    return (0);
}

// ============================================================================
// 功能：文件对应的文件节点；
// 参数：
// 返回：
// 备注：INLINK
// ============================================================================
inline struct Object *of_o(tagOFile *of)
{
    if(of)
        return (of->obj);

    return (NULL);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
struct __portBasic *of_basiclinko(struct Object *obj)
{
    struct __portBasic *basic;

    basic = dListEntry(dListGetAfter(&obj->port), struct __portBasic, list);
    if(!testcontent(basic->flags))
        return (NULL);

    return (basic);
}

// ============================================================================
// 功能：
// 参数：
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
inline void *of_core(tagOFile *of)
{
    extern void *FS_Core(struct Object *obj);

    return (FS_Core(of->obj));
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：INLINK
// ============================================================================
inline struct __portBasic *of_basic(tagOFile *of)
{
    if(of)
        return (of_basiclinko(of->obj));

    return (NULL);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：INLINK
// ============================================================================
inline struct Object *basic2obj(struct __portBasic *basic)
{
    struct Object *obj;

    obj = dListEntry(dListGetBefore(&basic->list), struct Object, port);
    return (obj);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：对于自研的文件系统，文件的数据保存于PortHead的第一个，后续用于存放上下文
// ============================================================================
static inline s32 __basiclinko(struct __portBasic *basic, struct Object *obj)
{
    if(-1 == __OBJ_LinksCheck(obj, 0))
        return (-1); // 不符合逻辑

    __OBJ_Link(obj, &basic->list);
    return (0);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：对于自研的文件系统，文件的数据保存于PortHead的第一个，后续用于存放上下文
// ============================================================================
inline void of_initbasic(struct __portBasic *basic, u32 flags)
{
    dListInit(&basic->list);
    basic->flags = flags | O_FILE_BASIC;
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
inline void of_unlinkbasic(struct __portBasic *basic)
{
    dListRemove(&basic->list);
}

// ============================================================================
// 功能：在文件系统中parent节点下虚拟一个文件
// 参数：
// 返回：
// 备注：INLINK
// ============================================================================
inline struct Object *of_virtualizeo(struct Object *parent, struct __portBasic *basic, char *name)
{
    struct Object *new;

    new = OBJ_NewSanityChild(parent, NULL, 0, name);
    if(!new)
        return (NULL);

    if(__basiclinko(basic, new))
    {
        OBJ_Del(new);
        return (NULL);
    }

    return (new);
}

// ============================================================================
// 功能：在文件系统中parent节点下虚拟一个文件
// 参数：
// 返回：
// 备注：INLINK
// ============================================================================
s32 of_virtualize(struct Object *parent, struct __portBasic *basic, char *name)
{
    if(of_virtualizeo(parent, basic, name))
        return (0);

    return (-1);
}


// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：TODO:当时文件时，其context指向文件结构，是目录时，则为空；
// ============================================================================
struct __ramfile
{
    struct __portBasic basic;
};

static tagOFile *__rf_open(struct Object *obj, u32 flags, char *new)
{
    tagOFile *cx;
    struct __ramfile *file;

    if((new) && (testcreat(flags)) && __OBJ_NameSantiy(new))
        return (NULL); // 新建文件逻辑校验

    if(testregular(flags)) // 目前不支持创建文件；
    {
        debug_printf("objfile","ramfs open failed<do not support file related operation>.");
        return (NULL);
    }

    if(new) // 需要新建文件或目录
    {
        file = malloc(sizeof(*file));
        if(!file)
        {
            debug_printf("objfile","ramfs open failed<memory out>.");
            return (NULL);
        }

        of_initbasic(&file->basic, flags);
        if(of_virtualize(obj, &file->basic, new))
        {
            debug_printf("objfile","ramfs open failed<virtual>.");
            free(file);
            return (NULL);
        }
    }

    cx = of_new();
    if(!cx)
    {
        debug_printf("objfile","ramfs open failed<context>.");
        if(file)
           free(file);
    }

    of_init(cx, NULL, flags, 0);

    return (cx);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 __rf_close(tagOFile *of)
{
    of_free(of);
    return (0);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 __rf_remove(struct Object *obj)
{
    debug_printf("objfile","ramfs do not support file now.");
    return (0);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 __rf_write(tagOFile *of, u8 *pBuf, u32 dwSize)
{
    debug_printf("objfile","ramfs do not support file now.");
    return (0);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 __rf_read(tagOFile *of, u8 *pBuf, u32 dwSize)
{
    debug_printf("objfile","ramfs do not support file now.");
    return (0);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 __rf_readdentry(tagOFile *pDirectory, struct dirent *pDirent)
{
    debug_printf("objfile","ramfs do not support file now.");
    return (0);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 __rf_stat(struct Object *obj, struct stat *data)
{
#if 0
    struct __ramfile *file;
    file = dListEntry(of_basiclinko(obj), struct __ramfile, basic);
#endif
    memset(data, 0x0, sizeof(struct stat));
    data->st_size = 0 ; // 当前还未实现文件；
    data->st_mode = S_IFDIR; // 当前还未实现文件；
    return (0);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static ptu32_t __rf_operations(u32 cmd, ptu32_t oof, ptu32_t args, ...)
{
    va_list list;

    switch(cmd)
    {
        case CN_OBJ_CMD_OPEN:
        {
            char *new;
            struct Object *obj = (struct Object*)oof;
            u32 flags = (u32)args;

            va_start(list, args);
            new = (char*)va_arg(list, u32);
            va_end(list);

            return ((ptu32_t)__rf_open(obj, flags, new));
        }

        case CN_OBJ_CMD_CLOSE:
        {
            tagOFile *of = (tagOFile*)oof;
            return ((ptu32_t)__rf_close(of));
        }

        case CN_OBJ_CMD_READDIR:
        {
            tagOFile *of = (tagOFile*)oof;
            struct dirent *ret = (struct dirent *)args;

            return(__rf_readdentry(of, ret));
        }

        case CN_OBJ_CMD_READ:
        {
            u32 len;
            tagOFile *of = (tagOFile*)oof;
            u8 *buf = (u8*)args;

            va_start(list, args);
            len = va_arg(list, u32);
            va_end(list);

            return((ptu32_t)__rf_read(of, buf, len));
        }

        case CN_OBJ_CMD_WRITE:
        {
            u32 len;
            tagOFile *of = (tagOFile*)oof;
            u8 *buf = (u8*)args;

            va_start(list, args);
            len = va_arg(list, u32);
            va_end(list);
            return((ptu32_t)__rf_write(of, buf, len));
        }

        case CN_OBJ_CMD_STAT:
        {
            struct stat *data;
            struct Object *obj = (struct Object*)oof;
            u32 notfound = (u32)args;

            if(notfound)
                return (-1); // 查询的文件不存在；

            va_start(list, args);
            data = (struct stat*)args;
            va_end(list);
            return ((ptu32_t)__rf_stat(obj, data));
        }

        case CN_OBJ_CMD_DELETE:
        {
            struct Object *obj = (struct Object*)oof;
            u32 notfound = (u32)args;

            if(notfound)
                return (-1);

            return((ptu32_t)__rf_remove(obj));
        }

        default:
        {
            debug_printf("objfile","ramfs do not support this operation now.");
            break;
        }
    }

    return (-1);
}

// ============================================================================
// 功能：
// 参数：
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 __InitFileSystem(void)
{
    s32 res;

    res = __OBJ_InitSystem();
    if(res)
    {
        return (-1);
    }

    ofmutex = Lock_MutexCreate("file system");
    if(!ofmutex)
    {
        return (-1);
    }

    res = __of_initbasicpool();
    if(res)
    {
        return (-1);
    }

    res = __OBJ_SetMountPoint(OBJ_Root());
    if(res)
        return (-1);

    __OBJ_SetOps(OBJ_Root(), __rf_operations); // 缺省安装的是基于RAM的文件系统

    return (0);
}

#if 0
// ============================================================================
// 功能：遍历文件句柄，遍历一个对象下创建的全部文件，典型用法：
//      tagOFile *Current = NULL;
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
tagOFile *traverseall(tagOFile *of, struct Object *obj)
{
    tagOFile *res;
    struct dListNode *list;

    if(!obj)
        return (NULL);

    if(of == NULL)
        list = dListGetAfter(&obj->port);
    else
        list = dListGetAfter(&of->list);

    if(list == &obj->port)
    {
        res = NULL;
    }
    else
    {
        res = dListEntry(list, tagOFile, list);
        if(testcontent(reflagsode)) // 越过content
        {
            list = dListGetAfter(&res->list);
            if(list == &obj->port)
                return (NULL);

            res = dListEntry(list, struct __port, list);
        }
    }

    return (res);
}
#endif

// ============================================================================
// 功能：设置文件当前标志成员；；
// 参数：
// 返回：
// 备注：
// ============================================================================
void of_setmode(tagOFile *of, u32 flags)
{
    if(of)
        of->flags |= flags;
}

// ============================================================================
// 功能：获取文件access成员；
// 参数：
// 返回：
// 备注：
// ============================================================================
inline u32 of_access(tagOFile *of)
{
    if(of)
        return (of->access);

    return (0);
}

// ============================================================================
// 功能：设置文件多路复用触发事件
// 参数：
// 返回：
// 备注：TODO:互斥
// ============================================================================
inline void of_setmevent(tagOFile *of, u32 events)
{
    u32 check;
    extern bool_t __Multiplex_Set(s32 Fd, u32 dwAccess);

    if(of)
    {
        check = of->access;
        of->access |= events;
        if(check!=of->access)
            __Multiplex_Set(ofno(of), of->access);
    }
}

// ============================================================================
// 功能：清文件的多路复用触发事件；
// 参数：
// 返回：
// 备注：TODO:互斥
// ============================================================================
inline void of_clrmevent(tagOFile *of, u32 events)
{
    u32 check;
    extern bool_t __Multiplex_Set(s32 Fd, u32 dwAccess);

    if(of)
    {
        check = of->access;
        of->access &= ~events;
        if(check != of->access)
            __Multiplex_Set(ofno(of), of->access);
    }
}

// ============================================================================
// 功能：置所有已被打开文件（即用一个文件被多个用户打开）的多路复用的触发条件；
// 参数：obj -- 被操作的文件对象；
//      events -- 文件当前状态，只有低24bit有效。bitmask，置1的位在AccessStatus中相应
//          的bit被置1，参见 CN_FDFLAG_IN 等定义
// 返回：无
// 备注：
// ============================================================================
void setmevento(struct Object *obj, u32 events)
{
    tagOFile *of;
    struct dListNode *head, *cur;
    u32 access;
    extern bool_t __Multiplex_Set(s32 Fd, u32 dwAccess);

    if(NULL == obj)
        return ;

    head = &obj->port;
    dListForEach(cur, head)
    {
        of = dListEntry(cur, tagOFile, list);
        if(testcontent(of->flags))
            continue; // 越过content

        access = of->access;
        of->access |= events;
        if(access != of->access)
            __Multiplex_Set(ofno(of), of->access);
    }
}

// ============================================================================
// 功能：清所有已被打开文件（即用一个文件被多个用户打开）的多路复用的触发条件；
// 参数：Object，见SetObjectAccessStatus函数
//      events，见SetObjectAccessStatus函数
// 返回：无
// 备注：
// ============================================================================
void clrmevento(struct Object *obj, u32 events)
{
    tagOFile *of;
    struct dListNode *head, *cur;
    u32 access;
    extern bool_t __Multiplex_Set(s32 Fd, u32 dwAccess);

    if(NULL == obj)
        return ;

    head = &obj->port;
    dListForEach(cur, head)
    {
        of = dListEntry(cur, tagOFile, list);
        if(testcontent(of->flags))
            continue;

        access = of->access;
        of->access &= ~events;
        if(access != of->access)
            __Multiplex_Set(ofno(of),of->access);
    }
}

// ============================================================================
// 功能：打开文件
// 参数：pathname -- 文件路径;
//      flags -- 操作标志;
//      mode -- 操作模式;
// 返回：失败（-1）; 文件句柄号（非-1）;
// 备注：O_RDONLY、O_WRONLY、O_RDWR 须符合POSIX（2016）的最新定义
// ============================================================================
tagOFile *__open(char *pPath, s32 flags, u32 mode)
{
    tagOFile *of;
    struct Object *obj;
    char *full, *uncached;

    Lock_MutexPend(ofmutex, CN_TIMEOUT_FOREVER);
    obj = OBJ_MatchPath(pPath, &uncached, &full);
    __OBJ_Protect(obj); // 防止文件操作过程中，被删除了
    Lock_MutexPost(ofmutex); // 出互斥

    of = (tagOFile*)obj->ops(CN_OBJ_CMD_OPEN, (ptu32_t)obj, (ptu32_t)flags, uncached, mode, full);

    Lock_MutexPend(ofmutex, CN_TIMEOUT_FOREVER);
    __OBJ_Release(obj);
    if(of)
    {
        obj = OBJ_BufferPath(obj, uncached); // 对于文件系统而言，存在需要缓冲的目录路径,继承操作方法等.
        of_linko(of, obj);
    }

    Lock_MutexPost(ofmutex); // 出互斥

    return (of);
}


// ============================================================================
// 功能：直接将对象作为一个文件打开；
// 参数：obj -- 对象；
// 返回：成功（文件号）；失败（-1）；
// 备注：
// ============================================================================
s32 openo(struct Object *obj)
{
    tagOFile *of;

    if(!obj)
        return (-1);

    Lock_MutexPend(ofmutex, CN_TIMEOUT_FOREVER);
    of = of_new();
    if(!of)
    {
        Lock_MutexPost(ofmutex);
        return (-1);
    }

    of->timeout = CN_TIMEOUT_FOREVER;
    of_linko(of, obj);
    Lock_MutexPost(ofmutex);

    return (ofno(of));
}

// ============================================================================
// 功能：关闭文件
// 参数：fd -- 文件句柄号;
// 返回：成功（0）;失败（-1）；
// 备注：
// ============================================================================
s32 __close(tagOFile *of)
{
    s32 res;
    struct Object *obj = of->obj;

    res = (s32)obj->ops(CN_OBJ_CMD_CLOSE, (ptu32_t)of, 0);
    if(!res)
    {
        OBJ_UnBufferPath(obj);
    }

    return (res);
}

// ============================================================================
// 功能：打开文件
// 参数：pathname -- 文件路径;
//      flags -- 操作标志;
//      mode -- 操作模式;
// 返回：失败（-1）; 文件句柄号（非-1）;
// 备注：O_RDONLY、O_WRONLY、O_RDWR 须符合POSIX（2016）的最新定义
// ============================================================================
int open(const char *pathname, int flags, ...)
{
    tagOFile *of;

    if((!pathname) || ('\0' == *pathname))
        return (-1);

#if 0 // TODO
    va_start(args, flags);
    mode = va_arg(args, s32);
    va_end(args);
#endif

    of = __open((char*)pathname, flags, 0);
    return (ofno(of));
}

// ============================================================================
// 功能：关闭文件
// 参数：fd -- 文件句柄号;
// 返回：成功（0）;失败（-1）；
// 备注：
// ============================================================================
int close(int fd)
{
    tagOFile *ofile;

    ofile = of(fd);
    if(!ofile)
        return (-1);

    return (__close(ofile));
}

// ============================================================================
// 功能：删除文件；
// 参数：path -- 文件路径；
// 返回：成功（0）； 失败（-1）；
// 备注：
// ============================================================================
int remove(const char *path)
{
    struct Object *obj;
    char *uncached, *full;
    s32 res;

    Lock_MutexPend(ofmutex, CN_TIMEOUT_FOREVER);
    obj = OBJ_MatchPath(path, &uncached, &full);

    if(!uncached) // 文件已经在系统中
    {
        if((GROUP_POINT == __OBJ_Type(obj)) || // 安装点不可删除
           (isondutyo(obj)))  // 文件正在被使用中
        {
            Lock_MutexPost(ofmutex); // 出互斥
            return (-1);
        }
    }

    __OBJ_Protect(obj);
    Lock_MutexPost(ofmutex); // 出互斥

    res = (s32)obj->ops(CN_OBJ_CMD_DELETE, (ptu32_t)obj, (ptu32_t)uncached, full);

    Lock_MutexPend(ofmutex, CN_TIMEOUT_FOREVER);
    __OBJ_Release(obj);
    if(!res)
    {
        OBJ_UnBufferPath(obj);
    }

    Lock_MutexPost(ofmutex); // 出互斥
    return (res);
}

// ============================================================================
// 功能：读文件；
// 参数：fd -- 文件句柄号；
//      buf -- 读缓冲；
//      count -- 需要读的数据大小（Byte计）；
// 返回：成功（实际读取到的长度）；错误（-1）；
// 备注：
// ============================================================================
ssize_t read(int fd, void *buf, size_t size)
{
    tagOFile *ofile;
    s32 res = -1;

    if(NULL == buf)
        return (-1); // 参数错误

    if(0 == size)
        return (0);

    ofile = of(fd);
    if(!ofile)
        return (-1);

    if(isreadable(ofile))
    {
        res = (s32)ofile->obj->ops(CN_OBJ_CMD_READ, (ptu32_t)ofile, (ptu32_t)buf, size);
    }

    return (res);
}

// ============================================================================
// 功能：写文件；
// 参数：fd -- 文件句柄号；
//      buf -- 写缓冲；
//      count -- 需要写的数据大小（Byte计）；
// 返回： 成功（实际写入的长度）；错误（-1）；
// 备注：
// ============================================================================
ssize_t write(int fd, const void *buf, size_t count)
{
    tagOFile *ofile;
    s32 res = -1;

    if(!buf)
        return (-1); // 参数错误

    if(0 == count)
        return (0);

    ofile = of(fd);
    if(!ofile)
        return (-1);

    if(iswritable(ofile))
    {
        res = (s32)ofile->obj->ops(CN_OBJ_CMD_WRITE, (ptu32_t)ofile, (ptu32_t)buf, count);
    }

    return (res);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：NOTE: lseek不支持文件大小拓展
// ============================================================================
off_t lseek(int fd, off_t offset, int whence)
{
    tagOFile *ofile;
    s32 res;

    if((SEEK_SET != whence) && (SEEK_CUR != whence) && (SEEK_END != whence))
        return (-1); // 错误的参数

    if((SEEK_SET == whence) && (offset < 0))
        return (-1); // 错误的逻辑

    ofile = of(fd);
    if(!ofile)
        return (-1);

    if(testdirectory(ofile->flags))
        return (-1); // 目录不可以seek

    res = (s32)ofile->obj->ops(CN_OBJ_CMD_SEEK, (ptu32_t)ofile, (ptu32_t)&offset, whence);
    if(res)
        return (-1);

    return (offset);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
int fsync(int fd)
{
    tagOFile *ofile;
    s32 res;

    ofile = of(fd);
    if(!ofile)
    {
        errno = EBADF; // 参数错误
        return (-1);
    }

    res = (s32)ofile->obj->ops(CN_OBJ_CMD_SYNC, (ptu32_t)ofile, 0);

    return (res);

}

// ============================================================================
// 功能：获取文件指针的当前位置
// 参数：
// 返回：
// 备注：
// ============================================================================
off_t tell(int fd)
{
    tagOFile *ofile;
    s64 offset;
    s32 res;

    ofile = of(fd);
    if(!ofile)
    {
        errno = EBADF; // 参数错误
        return (-1);
    }

    res = (s32)ofile->obj->ops(CN_OBJ_CMD_TELL, (ptu32_t)ofile, (ptu32_t)&offset);
    if(!res)
        return (offset);

    return (-1);
}

//-----------------------------------------------------------------------------
//功能：查询文件状态
//参数：file_name -- 文件路径；
//     buf -- 文件状态;
//返回：0 -- 成功; -1 -- 失败;
//备注：
//-----------------------------------------------------------------------------
int fstat(int fd, struct stat *buf)
{
    s32 res;
    tagOFile *ofile;


    if(NULL == buf)
        return(-1);

    ofile = of(fd);
    if(!ofile)
        return (-1);

    res = (s32)ofile->obj->ops(CN_OBJ_CMD_STAT, (ptu32_t)(ofile->obj), 0, buf);

    return (res);
}

// ============================================================================
// 功能：查询文件状态
// 参数：path -- 文件路径；buf -- 文件状态;
// 返回：0 -- 成功; -1 -- 失败;
// 备注：
// ============================================================================
int stat(const char *path, struct stat *buf)
{
    struct Object *obj;
    char *uncached, *full;
    s32 res;

    if((!buf) || (!path))
        return(-1);

    Lock_MutexPend(ofmutex, CN_TIMEOUT_FOREVER); // 防止操作过程文件被删除了
    obj = OBJ_MatchPath((char*)path, &uncached, &full);
    __OBJ_Protect(obj);
    Lock_MutexPost(ofmutex);

    res = (s32)obj->ops(CN_OBJ_CMD_STAT, (ptu32_t)obj, (ptu32_t)uncached, buf, full);
    __OBJ_Release(obj);
    return (res);
}

// ============================================================================
// 功能：设定文件长度
// 参数：fd -- 文件句柄；length -- 文件长度；
// 返回：成功（0）; 失败（-1）;
// 备注：the file offset is not changed.
// ============================================================================
int ftruncate(int fd, off_t length)
{
#if 0
    tagOFile *of;
    s32 res;

    if(0 == length)
        return (0);

    of = of(fd);
    if(!of)
        return (-1);

    res = (s32)of->obj->ops(CN_OBJ_CMD_TRUNCATE, (ptu32_t)of, (ptu32_t)&length);
    return (res);
#endif
    return (-1);
}

//-----------------------------------------------------------------------------
//功能: 删除文件或目录
//参数:
//返回: 0 -- 成功; -1 -- 失败;
//备注:
//-----------------------------------------------------------------------------
int unlink(const char *pathname)
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
    tagOFile *of;

    if((NULL == name)  || ('\0' == *name))
        return (NULL);

    of = __open((char*)name, (O_DIRECTORY | O_RDWR), 0);
    if(!of)
    {
        return (NULL);
    }

    directory = malloc(sizeof(*directory));
    if(NULL == directory)
    {
        __close(of);
    }

    memset(directory, 0, sizeof(*directory));
    directory->__fd = (void*)of;

    return (directory);
}

// ============================================================================
// 功能：读目录项；
// 参数：dir -- 已打开的目录；
// 返回：目录项；全部已读返回NULL；
// 备注：每次的返回值，都是同一个空间，这样处理是为了省空间；
// ============================================================================
struct dirent *readdir(DIR *dir)
{
    tagOFile *of;
    struct Object *obj;
    s32 res;

    if(!dir)
        return (NULL);

    if(!dir->__ptr)
    {
        dir->__ptr = malloc(sizeof(struct dirent));
        if(!dir->__ptr)
            return (NULL);
    }

    of = (tagOFile*)(dir->__fd); // 目录的上下文
    obj = of->obj; // 目录的节点

    res = (s32)obj->ops(CN_OBJ_CMD_READDIR, (ptu32_t)of, (ptu32_t)dir->__ptr);
    if(!res)
        return (struct dirent*)(dir->__ptr);
    else
        return (NULL); // 已全部遍历完
}

// ============================================================================
// 功能：关闭目录
// 参数：dir -- 已打开的目录；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
int closedir(DIR *dir)
{
    s32 res;

    res = __close(dir->__fd);
    if(res)
        return (-1);

    if(dir->__ptr)
        free(dir->__ptr);

    free(dir);

    return (0);
}

// ============================================================================
// 功能：创建目录
// 参数：
// 返回：成功（0）；失败（-1）；
// 备注：如果目录已存在，返回失败；
// ============================================================================
int mkdir(const char *filename, mode_t mode)
{
    tagOFile *dir;

    dir = __open((char*)filename, (O_DIRECTORY | O_RDWR | O_CREAT | O_EXCL), mode);
    if(dir)
    {
        __close(dir);
        return (0);
    }

    return (-1);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：cmd 加上偏移 CN_OBJ_FCNTL_START 后就是调用 ObjOps 的cmd 码
// ============================================================================
static s32 __of_cntl(tagOFile *of, s32 dwCmd, va_list *pArgs)
{
    va_list args = *pArgs;

    switch(dwCmd)
    {
        case F_OF_GET:
        {
            tagOFile **ret;
            ret = (tagOFile **)va_arg(args, u32);

            if(!*ret)
                return (-1);

            *ret = of;
            return (0);
        }

        case F_OF_SETCONTEXT:
        {
            ptu32_t context;

            context = (ptu32_t)va_arg(args, u32);
            of_setcontext(of, context);
            return (0);
        }

        case F_OF_GETCONTEXT:
        {
            ptu32_t *ret;

            ret = (ptu32_t*)va_arg(args, u32);
            *ret =  of_context(of);
            return (0);
        }

        case F_SETREPRESENT:
        {
            ptu32_t represent;

            represent = (ptu32_t)va_arg(args, u32);
            of_setrepresent(of, represent);
            return (0);
        }

        case F_GETREPRESENT:
        {
            ptu32_t *ret;

            ret = (ptu32_t*)va_arg(args, u32);
            *ret =  of_represent(of);
            return (0);
        }

        case F_SETEVENT:
        {
            u32 access;

            access = va_arg(args, u32);
            of_setmevent(of, access);
            return (0);
        }

        case F_CLREVENT:
        {
            u32 access;

            access = va_arg(args, u32);
            of_clrmevent(of, access);
            return (0);
        }

        default: break;
    }

    return (-1);
}

// ============================================================================
// 功能：文件控制
// 参数：
// 返回：
// 备注：cmd 加上偏移 CN_OBJ_FCNTL_START 后就是调用 ObjOps 的cmd 码
// ============================================================================
int fcntl(int fd, int cmd, ...)
{
    tagOFile *ofile;
    va_list args;
    s32 res;

    if((cmd < CN_OBJ_IOCTL_START) &&
       ((cmd + CN_OBJ_FCNTL_START) >= CN_OBJ_IOCTL_START))
        return (0); // 命令越界

    ofile = of(fd);
    if(!ofile)
        return (0);

    va_start (args, cmd);
    res = __of_cntl(ofile, cmd, &args);
    if(!res)
        return (0); // 仅对文件的操作

    if(cmd < CN_OBJ_IOCTL_START)
       cmd += CN_OBJ_FCNTL_START;

    res = (s32)ofile->obj->ops(cmd, (ptu32_t)ofile, (ptu32_t)&args);
    va_end (args);

    return (res);
}

// ============================================================================
// 功能：io控制
// 参数：
// 返回：
// 备注：
//      最新的POSIX已经废弃了ioctl函数，而然并卵，大量开源项目仍然在用。
//      注：奇怪的POSIX，ioctl相关的常量，头文件名是 stropts.h 而不是 ioctl.h
//      request 加上偏移 CN_OBJ_IOCTL_START 后就是调用 ObjOps 的cmd 码
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
//      C：有些开源项目，如三星的iotjs，自定义了一些ioctl命令，A和B方案都有。
//      djyos实现方案：
//      1、使用单一常数方案；
//      2、系统自带的模块，不使用第四个参数，三个参数不够用的，用结构指针
//      3、目前尚未见到使用第四个参数的开源代码，djyos实际上不考虑支持。
// ============================================================================
int ioctl(int fd, int cmd, ...)
{
    tagOFile *ofile;
    va_list args;
    u32 opcode;
    s32 res;

    if((cmd < CN_OBJ_CMD_USER) &&
       ((cmd + CN_OBJ_IOCTL_START) >= CN_OBJ_CMD_USER))
        return (0); // 命令越界

    ofile = of(fd);
    if(!ofile)
        return (0);

    va_start (args, cmd);

    opcode = va_arg(args, u32);

    if(cmd < CN_OBJ_CMD_USER)
        cmd += CN_OBJ_IOCTL_START;

    res = (s32)ofile->obj->ops(cmd, (ptu32_t)ofile, (ptu32_t)opcode, &args);

    va_end (args);

    return (res);
}

// ============================================================================
// 功能：将当前工作目录的绝对路径复制到参数buf
// 参数：buf -- 存放路径的空间；
//      size -- buf的大小；
// 返回：成功（当前工作目录的绝对路径）；失败（NULL）
// 备注：buf为 NULL并使 size为零，则本函数会动态申请一个空间来返回给用户，但是用户需要注意将空间释放；
// ============================================================================
char *getcwd(char *buf, size_t size)
{
    u32 len = 0, offset;
    struct Object *obj = OBJ_Current();

    if(((buf) && (!size)) || ((!buf) && (size)))
        return (NULL);

    while(obj)
    {
        len += strlen((char*)__OBJ_Name(obj)) + 1;
        if(OBJ_IsRoot(obj))
            break;

        obj = __OBJ_Parent(obj);
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
    obj = OBJ_Current();
    buf[offset] = '\0';
    while(1)
    {
        len = strlen((char*)__OBJ_Name(obj));
        offset -= len;
        memcpy(buf+offset, (char*)__OBJ_Name(obj), len);
        buf[--offset] = '/';
        if(OBJ_IsRoot(obj))
            break;

        obj = __OBJ_Parent(obj);
    }

    return (buf);
}

// ============================================================================
// 功能：改变当前工作目录
// 参数：path -- 新的当前工作目录
// 返回：
// 备注：
// ============================================================================
int chdir(const char *path)
{
    return (-1);// TODO; NOTE:设置的路径是一个目录
}
