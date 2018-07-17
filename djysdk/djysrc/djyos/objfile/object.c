//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
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
//所属模块: 对象管理
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 创建、添加、移动、删除、搜索对象节点的服务
//其他说明:
//修订历史:
//4. 日期:20130220
//   作者:  罗侍田.
//   新版本号：v1.3.0
//   修改说明:
//      1.修正了rsc操作可能导致死锁的问题，bug是由王毛磊提交的。
//3. 日期:20090518
//   作者:  罗侍田.
//   新版本号：v1.2.0
//   修改说明:
//      1.在概念上，分开了树根和对象根，函数名也相应地调整了
//      2.增加rsc_get_root函数，原rsc_get_root改名为rsc_get_tree，且不再兼职
//      3.因有了rsc_get_root，取消1.1.0版本的"修改3"
//      4.补全了NULL参数检查
//2. 日期:20090131
//   作者:  罗侍田.
//   新版本号：v1.1.0
//   修改说明:
//      1.加了个总根节点，添加了模块初始化函数
//      2.用信号量保护并发访问安全
//      3.改进了遍历函数，允许用NULL代表遍历整个队列
//      4.修正了一些bug
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           对象树算法说明
// 各同级节点连成双向循环链表
// 每个节点的child指针直接指向的节点称为该节点的所在队列的队列头，它的previous节点
// 是队列尾。
// 所有节点的Parent指针均指向父节点

// 特别提示，对象队列的并发访问安全由对象管理模块负责，对象节点自身的并发访问
//          保护由使用者负责。
// 对链表的修改都会使用原子操作保证链表的完备性
// 对链表的读操作则不使用任何安全机制，由对象所属模块来保证对象竞争的安全性
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <int.h>
#include <pool.h>
#include <list.h>
#include <object.h>
#include <systime.h>
#include <lock.h>
#include "__object.h"
#include "dbug.h"
// OBJ双向链表初始化
#define __OBJ_LIST_INIT(l)              (l)->next = (l)->prev = (l)

// 在双向循环链表的l位置后面插入节点o
#define __OBJ_LIST_INS_AFTER(l, o)      \
                                        do \
                                        {  \
                                            (l)->next->prev = (o); \
                                            (o)->next = (l)->next; \
                                            (l)->next = (o); \
                                            (o)->prev = (l); \
                                        }while(0)

// 在双向循环链表的l位置前面插入节点o
#define __OBJ_LIST_INS_BEFORE(l, o)     \
                                        do \
                                        { \
                                            (l)->prev->next = (o); \
                                            (o)->prev = (l)->prev; \
                                            (l)->prev = (o); \
                                            (o)->next = (l); \
                                        }while(0)

// 在双向循环链表中移除, NOTE: 这里未考虑上下级链表的处理
#define __OBJ_LIST_REMOVE(l)            \
                                        do \
                                        { \
                                            (l)->next->prev = (l)->prev; \
                                            (l)->prev->next = (l)->next; \
                                            (l)->next = (l)->prev = (l); \
                                        }while(0)

// 是否是最后一个链表成员；
#define __OBJ_LIST_LAST(l)              (((l)->next == (l)) ? 1 : 0)

// 循环查询兄弟链；
#define __OBJ_LIST_FOR_EACH_BROTHERS(l, h)            \
                                        for((l) = (h)->next; (l) != (h); (l) = (l)->next)

#define DEFAULT_LOCK_TIMEOUT        (360*1000*mS)   // 一个小时，不设未FOREVER是为了防止逻辑锁死
#define BASIC_OBJECTS               32
static struct MemCellPool basicObjPool;
static struct Object basicObjSpace[BASIC_OBJECTS];
static struct MutexLCB * pObjMutex;
static struct Object *pRootObj, *pCurObj;

ptu32_t OBJ_DefaultOps(u32 dwCMD, ptu32_t context, ptu32_t args, ...);

struct __statics
{
    u32 news;
    u32 fress;
    u32 alives;
}objStatics;

// ============================================================================
// 功能：初始化对象池
// 参数：
// 返回：成功（0）；失败（-1）；
// 备注：POOL的设置是可以自动扩展
// ============================================================================
s32 __OBJ_InitBasicPool(void)
{
    if(Mb_CreatePool_s(&basicObjPool, basicObjSpace,
            BASIC_OBJECTS, sizeof(struct Object), 16, 16384, "object"))
        return (0);

    return (-1);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：TODO: INLINE
// ============================================================================
struct Object *__OBJ_New(void)
{
    struct Object *obj;

    obj = (struct Object*)Mb_Malloc(&basicObjPool, 0);
    objStatics.news++;
    objStatics.alives++;
    return (obj);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：INLINE
// ============================================================================
inline void __OBJ_Free(struct Object *pObj)
{
    Mb_Free(&basicObjPool, pObj);
    objStatics.fress++;
    objStatics.alives--;
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：INLINE
// ============================================================================
inline struct Object *__OBJ_Child(struct Object *pObj)
{
    return (pObj->child);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：INLINE
// ============================================================================
inline struct Object *__OBJ_Parent(struct Object* pObj)
{
    return (pObj->parent);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：INLINE
// ============================================================================
inline struct Object *__OBJ_Next(struct Object *pObj)
{
    return (pObj->next);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：INLINE
// ============================================================================
inline struct Object *__OBJ_Prev(struct Object *pObj)
{
    return (pObj->prev);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：INLINE
// ============================================================================
inline void __OBJ_SetRepresent(struct Object *pObj, ptu32_t new)
{
    pObj->represent = new;
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：INLINE
// ============================================================================
inline s32 __OBJ_SetMountPoint(struct Object *pObj)
{
    if(!pObj->reload)
        pObj->reload = (struct Object *)(-1);

    return (0);
}

// ============================================================================
// 功能：
// 参数：
// 返回：成功（0）；
// 备注：INLINE
// ============================================================================
inline s32 __OBJ_ClearMountPoint(struct Object *pObj)
{
    if(pObj->reload && (-1 == (s32)pObj->reload))
        pObj->reload = (struct Object *)(0);

    return (0);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：INLINE
// ============================================================================
inline ptu32_t __OBJ_Represent(struct Object *pObj)
{
    return (pObj->represent);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：INLINE
// ============================================================================
inline fnObjOps __OBJ_Ops(struct Object *pObj)
{
    return (pObj->ops);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：INLINE
// ============================================================================
inline void __OBJ_Init(struct Object *pObj)
{
    memset(pObj, 0x0, sizeof(*pObj));
    dListInit(&pObj->port);
}

// ============================================================================
// 功能：初始化对象系统
// 参数：无
// 返回：成功（1），失败（0）。
// 备注：
// ============================================================================
s32 __OBJ_InitSystem(void)
{
    s32 res;

    pObjMutex = Lock_MutexCreate("object");
    if(!pObjMutex)
        return (-1);

    res = __OBJ_InitBasicPool();
    if(res)
    {
        Lock_MutexDelete(pObjMutex);
        return (-1);
    }

    pRootObj = __OBJ_New();
    if(!pRootObj)
    {
        Lock_MutexDelete(pObjMutex);
        return (-1);
    }

    pCurObj = pRootObj;
    __OBJ_Init(pRootObj);
    __OBJ_LIST_INIT(pRootObj);
    __OBJ_SetName(pRootObj, "");
    pRootObj->ops = (fnObjOps)OBJ_DefaultOps;

    return (0);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
inline s32 __OBJ_Del(struct Object *pObj)
{
    if(pObj->next == pObj)   //说明该节点没有兄弟节点.
    {
        pObj->parent->child = NULL;
    }
    else
    {
        if(pObj->parent->child == pObj)
            pObj->parent->child = pObj->next; // 说明该节点是队列头节点,需要改变队列头节点

        __OBJ_LIST_REMOVE(pObj);
    }

    return (0);
}

// ============================================================================
// 功能：对象的类型
// 参数：pObj -- 对象；
// 返回：对象类型；
// 备注：
// ============================================================================
s32 __OBJ_Type(struct Object *pObj)
{
    if(pObj->reload)
        return (GROUP_POINT);

    if(pObj->child)
    {
        return (DIR_POINT); // DIR_POINT;
    }
    else // 没有子节点
    {
        if(dListIsEmpty(&(pObj->port))) // 没有上下文
        {
            return (TEMPORARY_POINT);
        }
        else
        {
            extern s32 isdirectoryo(struct Object *pObj);
            if(isdirectoryo(pObj))
                return (DIR_POINT);
        }
    }

    return (REG_POINT);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：INLINE
// ============================================================================
inline const char *__OBJ_Name(struct Object *pObj)
{
    return((const char*)(pObj->name));
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：INLINE
// ============================================================================
inline void __OBJ_SetName(struct Object *pObj, const char *pName)
{
    pObj->name = (char*)pName;
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：INLINE
// ============================================================================
inline void __OBJ_SetOps(struct Object *pObj, fnObjOps ops)
{
    pObj->ops = ops;
}

// ============================================================================
// 功能：链一个成员给对象；
// 参数：pObj -- 对象；
//      pNew -- 新的链成员；
// 返回：成功（0）；
// 备注：
// ============================================================================
inline s32 __OBJ_Link(struct Object *pObj, list_t *pNew)
{
    dListInsertAfter(&pObj->port, pNew);
    return (0);
}

// ============================================================================
// 功能：对象链接了的成员数量
// 参数：pObj -- 对象；
//      dwLinks -- 数量极限，到达极限就不再统计；
// 返回：大于dwLinks,则返回（-1）,小于等于dwLinks，则返回实际link数
// 备注：TODO:互斥
// ============================================================================
inline s32 __OBJ_LinksCheck(struct Object *pObj, u32 dwLinks)
{
    list_t *head = &pObj->port;
    list_t *cur;
    u32 link;

    cur = dListGetAfter(head);
    if(cur == head)
        return (0);

    for(link = 0; link < dwLinks; link++)
    {
        cur = dListGetAfter(cur);
        if(cur == head)
        {
            break;
        }
    }

    if((++link) <= dwLinks)
        return (link);

    return (-1);
}

// ============================================================================
// 功能：对象进入保护；（不可删除）
// 参数：pObj -- 对象；
// 返回：成功（0）；失败（-1）；
// 备注：INLINE
// ============================================================================
inline s32 __OBJ_Protect(struct Object *pObj)
{
    if(FALSE == Lock_MutexPend(pObjMutex, DEFAULT_LOCK_TIMEOUT))
        return (-1);

    if(pObj->duty != CN_LIMIT_UINT32)
    {
        pObj->duty++;
        Lock_MutexPost(pObjMutex);
        return (0);
    }

    Lock_MutexPost(pObjMutex);
    return (-1);
}

// ============================================================================
// 功能：禁止操作对象；
// 参数：pObj -- 对象；
// 返回：成功（0）；失败（-1）；
// 备注：INLINE，将要删除对象执行；
// ============================================================================
inline s32 __OBJ_Forbid(struct Object *pObj)
{
    if(FALSE == Lock_MutexPend(pObjMutex, DEFAULT_LOCK_TIMEOUT))
        return (-1);

    if(!pObj->duty)
    {
        pObj->duty = CN_LIMIT_UINT32;
        Lock_MutexPost(pObjMutex);
        return (0);
    }

    Lock_MutexPost(pObjMutex);
    return (-1);
}

// ============================================================================
// 功能：对象是否被保护或被禁止；
// 参数：pObj -- 对象；
// 返回：被保护（其他）；未被保护（0）；
// 备注：INLINE
// ============================================================================
inline s32 __OBJ_IsProtected(struct Object *pObj)
{
    return (pObj->duty);
}

// ============================================================================
// 功能：对象退出保护或禁止；
// 参数：pObj -- 对象；
// 返回：合法（0）；非法（0）；
// 备注：INLINE
// ============================================================================
inline s32 __OBJ_Release(struct Object *pObj)
{
    if(FALSE == Lock_MutexPend(pObjMutex, DEFAULT_LOCK_TIMEOUT))
        return (-1);

    if(CN_LIMIT_UINT32 != pObj->duty)
        pObj->duty = 0; // forbid
    else if(pObj->duty)
        pObj->duty--;

    Lock_MutexPost(pObjMutex);
    return (0);
}

// ============================================================================
// 功能：检查对象名的合法性；
// 参数：pName -- 对象名；
// 返回：合法（0）；非法（0）；
// 备注：INLINE
// ============================================================================
inline s32 __OBJ_NameSantiy(const char *pName)
{
    u16 i;

    for(i=0; i<CN_OBJ_NAME_LIMIT; i++)
    {
        if(('/' == pName[i])||('\\' == pName[i]))
            return (-1);

        if('\0' == pName[i])
            return (0);
    }

    return (-1);
}

// ============================================================================
// 功能：匹配路径
// 参数：pPath -- 需匹配的路径；
//      pLeft -- 完全匹配，为NULL；不完全匹配，则返回不匹配部分（保证不以'/'开头）；
//      pFull -- 从最近的安装点开始的匹配路径（同一个文件系统下的路径）；
// 返回：匹配路径所对应的对象。
// 备注：
// ============================================================================
struct Object *OBJ_MatchPath(const char *pPath, char **pLeft, char **pFull)
{
    char *path = (char*)pPath;
    char *full = NULL;
    char *segment;
    struct Object *current, *head;
    u32 i;

    // 增加互斥
    if('/' == *path)
    {
        current = OBJ_Root(); // 绝对路径
    }
    else
    {
        current = OBJ_Current();// 其他都是相对路径
    }

    if(GROUP_POINT == __OBJ_Type(current)) // 安装点
        full = path;

    while(current)
    {
        while('/' == *path)
            path++; // 过滤多余的'/'

        if('\0' == *path)
            break; // 遍历路径结束

        if(('.' == path[0]) && ('.' == path[1]) &&
            (('\\' == path[3]) || ('/' == path[3]) || ('\0' == path[3])))
        {
            if(GROUP_POINT == __OBJ_Type(current))
                full = NULL; // 从安装点向上检索，其全路径逻辑就失效了

            current = __OBJ_Parent(current);
            path += 2; // ".."字符，表示上一级
            continue;
        }
        else if(('.' == path[0]) &&
            (('\\' == path[1]) || ('/' == path[1]) || ('\0' == path[1])))
        {
            path += 1; // "."字符，表示当前级
            continue;
        }

        head = __OBJ_Child(current); // 遍历子节点
        if(!head)
            break;

        segment = (char*)__OBJ_Name(head);
        for(i=0; i<CN_OBJ_NAME_LIMIT; i++) // 对象名长度限制
        {
            if('\0' == segment[i])
            {
                if('\0' == path[i])
                {
                    path += i; // path为'\0'
                    current = head;
                    goto __MATCH_DONE; // 匹配结束，整个遍历结束
                }
                else if(('\\' == path[i]) || ('/' == path[i]))
                {
                    path += i;
                    current = head;
                    goto __MATCH_NEXT; // 匹配当前节点，继续匹配一下一个节点
                }
                else
                {
                    break; // 不与当前节点存在不匹配的字符
                }
            }
            else if(segment[i] == path[i])
            {
                continue;
            }
            else
            {
                break; // 不与当前节点存在不匹配的字符
            }
        }

        __OBJ_LIST_FOR_EACH_BROTHERS(current, head)
        {
            segment = (char*)__OBJ_Name(current);
            for(i=0; i<CN_OBJ_NAME_LIMIT; i++) // 对象名长度限制
            {
                if('\0' == segment[i])
                {
                    if('\0' == path[i])
                    {
                        path += i; // path为'\0'
                        goto __MATCH_DONE; // 匹配结束，整个遍历结束
                    }
                    else if(('\\' == path[i]) || ('/' == path[i]))
                    {
                        path += i;
                        goto __MATCH_NEXT; // 匹配当前节点，继续匹配一下一个节点
                    }
                    else
                    {
                        break; // 不与当前节点存在不匹配的字符
                    }
                }
                else if(segment[i] == path[i])
                {
                    continue;
                }
                else
                {
                    break;
                }
            }
        }

        current = __OBJ_Parent(current); // 当前节点与路径不匹配，退回上上以及节点
        break; // 未找到节点

__MATCH_NEXT:

        if(GROUP_POINT == __OBJ_Type(current)) // 匹配的节点是安装点
            full = path;
    }

__MATCH_DONE:
    if(pFull)
        *pFull = full;

    if(pLeft)
    {
        if('\0' == *path)
            *pLeft = NULL;
        else
            *pLeft = path; // 逻辑上需保证不会以'/'开头
    }

    return (current);
}

// ============================================================================
// 功能：建立临时object路径
// 参数：pBegin -- 开始对象节点；
//      pPath -- 需建立的临时路径；
// 返回：目标对象；
// 备注：这里新建时，有对重名做判断；
// ============================================================================
struct Object *OBJ_BufferPath(struct Object *begin, char *path)
{
    char *segst, *name;
    struct Object *current, *find;
    u16 i, seglen;

    if(!path)
        return (begin);

    current = begin;
    while('\0' != *path)
    {
        while('/' == *path)
            path++; // 过滤多余的'/'

        segst = path;
        while(('/' != *path) && ('\\' != *path) && ('\0' != *path))
            path++; // 查找直至'/'或'\0'

        seglen = path - segst;
        name = malloc(seglen + 1);
        if(!name)
        {
            debug_printf("object","memory out(%s). ", __FUNCTION__);
            return (NULL);
        }

        for(i = 0 ; i < seglen; i++)
            name[i] = segst[i];

        name[seglen] = '\0';
        find = OBJ_SearchChild(current, name);
        if(find)
        {
            current = find; // child已经存在；
            free(name);
        }
        else
        {
            // 这里的ops和represent是一个继承关系，即继承父节点的
            current = OBJ_AddChild(current, current->ops, current->represent, (const char*)name);
            if(!current)
            {
                debug_printf("object","memory out(%s). ", __FUNCTION__);
                return (NULL);
            }
        }
    }

    return (current);
}

// ============================================================================
// 功能：释放临时object路径
// 参数：
// 返回：
// 备注：TODO:互斥
// ============================================================================
void OBJ_UnBufferPath(struct Object *pBegin)
{
    struct Object *parent, *current = pBegin;

    while(1)
    {
        if((TEMPORARY_POINT == __OBJ_Type(current)) &&
           (!__OBJ_IsProtected(current)))
        {
            parent = __OBJ_Parent(current);
            __OBJ_Del(current);
            __OBJ_Free(current);
            current = parent;
        }
        else
        {
            break;
        }
    }
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
struct Object *OBJ_Replace(struct Object *pObj)
{
    struct Object *replacer;
    atom_low_t lock;

    replacer = __OBJ_New();
    if(!replacer)
    {
        debug_printf("object","can not create for replace.");
        return (NULL);
    }

    __OBJ_Init(replacer);

    lock = Int_LowAtomStart();
    replacer->child = NULL;
    replacer->parent = pObj->parent;
    replacer->next = pObj->next;
    replacer->prev = pObj->prev;
    replacer->name = pObj->name;

    pObj->next->prev = replacer;
    pObj->prev->next = replacer;
    if(pObj->parent->child == pObj)
        pObj->parent->child = replacer;

    replacer->reload = pObj;
    Int_LowAtomEnd(lock);

    return (replacer);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
struct Object *OBJ_Restore(struct Object *pObj)
{
    if(-1 == (u32)pObj->reload)
        return (pObj);

    if(pObj->reload->parent != pObj->parent)
    {
        debug_printf("object","replacement changed \"%s\"'s parent relation", pObj->name);
        pObj->reload->parent = pObj->parent;
    }

    if(pObj->reload->next != pObj->next)
    {
        debug_printf("object","replacement changed \"%s\"'s next relation", pObj->name);
        pObj->reload->next = pObj->next;
    }

    if(pObj->reload->prev != pObj->prev)
    {
        debug_printf("object","replacement changed \"%s\"'s previous relation", pObj->name);
        pObj->reload->prev = pObj->prev;
    }

    if(pObj->parent->child == pObj)
        pObj->parent->child = pObj->reload;

    if(dListIsEmpty(&(pObj->port)))
        __OBJ_Free(pObj); // 原节点没有被引用到，就释放

    return (pObj->reload);
}

// ============================================================================
// 功能：检查对象名的合法性；
// 参数：pName -- 对象名；
// 返回：合法（0）；非法（0）；
// 备注：
// ============================================================================
s32 OBJ_NameSantiy(const char *pName)
{
    if(pName)
        return (__OBJ_NameSantiy((char*)pName));

    return (-1);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
struct Object * OBJ_Current(void)
{
    return (pCurObj);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
void OBJ_SetCurrent(struct Object *pObj)
{
    pCurObj = pObj;
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
struct Object * OBJ_Root(void)
{
    return (pRootObj);
}

// ============================================================================
// 功能：是否是根对象；
// 参数：对象；
// 返回：合法（0）；非法（0）；
// 备注：
// ============================================================================
s32 OBJ_IsRoot(struct Object *pObj)
{
    return (pObj == pRootObj);
}

// ============================================================================
// 功能：为对象提供一个默认操作函数，当你只是要创建一个对象，并不需要作为文件操作
//      时，就不需要提供 Operation 函数，此时，系统会自动把对象操作函数指向这里。
// 参数：都没有意义。
// 返回：固定返回 CN_OBJ_CMD_UNSUPPORT
// 备注：
// ============================================================================
inline ptu32_t OBJ_DefaultOps(u32 dwCMD, ptu32_t context, ptu32_t args, ...)
{
    return (CN_OBJ_CMD_UNSUPPORT);
}
#if 0
// ============================================================================
// 功能：
// 参数：
// 返回：成功（对象指针），失败（NULL）。
// 备注：
// ============================================================================
void *OBJ_GetOBJ(char *pPath)
{
    struct Object *start = pRootObj;

    if(('/' != *pPath) && (!g_ptCWD))
        start = g_ptCWD;

    return (OBJ_Search(start, pPath));
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
struct Object *OBJ_Backup(struct Object *pBackup, struct Object *pNew)
{
    if((!pBackup) || (!pNew))
        return (NULL);

    pBackup->prev->next = pNew;
    pBackup->next->prev = pNew;
    pNew->next = pBackup->next;
    pNew->prev = pBackup->prev;
    __OBJ_LIST_INIT(pBackup);
    pNew->reload = pBackup;
    return (pNew);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
struct Object *OBJ_Restore(struct Object *pObj)
{
    struct Object *restore;

    if(!pObj)
        return (NULL);

    restore = pObj->reload;
    pObj->prev->next = restore;
    pObj->next->prev = restore;
    restore->next = pObj->next;
    restore->prev = pObj->prev;
    __OBJ_LIST_INIT(pObj);
    return (pObj);
}
#endif

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
s32 OBJ_SetOps(struct Object *pObj, fnObjOps ops)
{
    if(!pObj)
        return (-1);

    __OBJ_SetOps(pObj, ops);
    return (0);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
fnObjOps OBJ_Ops(struct Object *pObj)
{
    if(!pObj)
        return (NULL);

    return (__OBJ_Ops(pObj));
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
s32 OBJ_SetRepresent(struct Object *pObj, ptu32_t represent)
{
    if(!pObj)
        return (-1);

    __OBJ_SetRepresent(pObj, represent);

    return (0);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
ptu32_t OBJ_Represent(struct Object *pObj)
{
    if(!pObj)
        return (0);

    return (__OBJ_Represent(pObj));
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
list_t *OBJ_PortList(struct Object *pObj)
{
    if(pObj != NULL)
        return (&pObj->port);
    else
        return  (NULL);
}

#if 0
// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
struct Object *__OBJ_NewChild(struct Object *pParent, fnObjOps ops,
                            ptu32_t represent, const char *pName)
{
    struct Object *child;
    atom_low_t lock;

    if(pParent == NULL)
        return (NULL);

#if 0
    if(NULL != OBJ_SearchChild(pParent, pName))
        return (NULL); // child已经存在；
#endif

    child = __OBJ_New();
    if(child == NULL)
        return (NULL);

    lock = Int_LowAtomStart();

    child->parent = pParent;
    child->child = NULL;
    if(represent)
        child->represent = represent;
    else
        child->represent = pParent->represent;

    if(ops)
        child->ops = ops;
    else
        child->ops = pParent->ops;

    child->duty = 0;
    child->name = (char *)pName;
    child->reload = NULL;
    dListInit(&child->port);
    if(pParent->child == NULL)
    {
        pParent->child = child;
        __OBJ_LIST_INIT(child);
    }
    else
    {
        __OBJ_LIST_INS_BEFORE(pParent->child, child);
    }

    Int_LowAtomEnd(lock);

    return (child);
}
#endif

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：pName是否可以是NULL？
// ============================================================================
struct Object *OBJ_NewSanityChild(struct Object *pParent, fnObjOps ops,
                            ptu32_t represent, const char *pName)
{
    struct Object *child;
    atom_low_t lock;
    char *name;

    if(pParent == NULL)
        return (NULL);

    if(OBJ_NameSantiy(pName))
        return (NULL);

    if(NULL != OBJ_SearchChild(pParent, pName))
        return (NULL); // child已经存在；

    name = malloc(strlen(pName)+1);
    if(!name)
        return (NULL);

    strcpy(name, pName);

    child = __OBJ_New();
    if(child == NULL)
        return (NULL);

    lock = Int_LowAtomStart();

    child->parent = pParent;
    child->child = NULL;
    if(represent)
        child->represent = represent;
    else
        child->represent = pParent->represent;

    if(ops)
        child->ops = ops;
    else
        child->ops = pParent->ops;

    child->duty = 0;
    child->name = (char *)name;
    child->reload = NULL;
    dListInit(&child->port);
    if(pParent->child == NULL)
    {
        pParent->child = child;
        __OBJ_LIST_INIT(child);
    }
    else
    {
        __OBJ_LIST_INS_BEFORE(pParent->child, child);
    }

    Int_LowAtomEnd(lock);

    return (child);
}

// ============================================================================
// 功能：在对象树中插入一个节点，新节点在原节点的previous位置；
// 参数：pLoc -- 在此节点前面插入节点；
//      ops -- 对象操作方法；
//      represent -- 对象表示；
//      pName -- 对象名字，所指定的字符串内存区不能释放，但可以是空。
// 返回：新加入的节点
// 备注：
// ============================================================================
struct Object *OBJ_AddToPrevious(struct Object *pLoc, fnObjOps ops,
                                 ptu32_t represent, const char *pName)
{
    struct Object *new;
    atom_low_t low_atom;

    if(pLoc == NULL)
        return (NULL);

    new = __OBJ_New();
    if(new == NULL)
        return (NULL);

    low_atom = Int_LowAtomStart();
    new->parent = pLoc->parent;
    new->child = NULL;
    new->represent = represent;
    if(ops)
        new->ops = ops;
    else
        new->ops = (fnObjOps)OBJ_DefaultOps;

    new->duty = 0;
    new->name = (char *)pName;
    new->reload = NULL;
    dListInit(&new->port);
    __OBJ_LIST_INS_BEFORE(pLoc, new);
    Int_LowAtomEnd(low_atom);
    return (new);
}

// ============================================================================
// 功能：在对象树中增加一个节点，新节点在原节点的next位置
// 参数：pLoc -- 在此节点后面插入节点；
//      ops -- 对象操作方法；
//      represent -- 对象表示；
//      pName -- 对象名字，所指定的字符串内存区不能释放，但可以是空。
// 返回：新加入的节点
// 备注：
// ============================================================================
struct Object *OBJ_AddToNext(struct Object *pLoc, fnObjOps ops,
                             ptu32_t represent, const char *pName)
{
    struct Object *new;
    atom_low_t   low_atom;

    if(pLoc == NULL)
        return (NULL);

    new = __OBJ_New();
    if(new == NULL)
        return (NULL);

    low_atom = Int_LowAtomStart();
    new->parent = pLoc->parent;
    new->child = NULL;
    new->represent = represent;
    if(ops != NULL)
        new->ops = ops;
    else
        new->ops = (fnObjOps)OBJ_DefaultOps;

    new->duty = 0;
    new->name = (char *)pName;
    new->reload = NULL;
    dListInit(&new->port);
    __OBJ_LIST_INS_AFTER(pLoc, new);
    Int_LowAtomEnd(low_atom);
    return (new);
}

// ============================================================================
// 功能：给指定节点增加一个子节点,新节点将在队列尾位置；
// 参数：pLoc -- 新节点的父节点；
//      ops -- 对象操作方法；
//      represent -- 对象表示；
//      pName -- 对象名字，所指定的字符串内存区不能释放，但可以是空；
// 返回：新加入的节点；
// 备注：
// ============================================================================
struct Object *OBJ_AddChild(struct Object *pLoc, fnObjOps ops,
                            ptu32_t represent, const char *pName)
{
    struct Object *child;
    atom_low_t low_atom;

    if(pLoc == NULL)
        return (NULL);

    child = __OBJ_New();
    if(child == NULL)
        return (NULL);

    __OBJ_Init(child);

    low_atom = Int_LowAtomStart();
    child->parent = pLoc;
    child->child = NULL;
    child->represent = represent;

    if(ops != NULL)
        child->ops = ops;
    else
        child->ops = (fnObjOps)OBJ_DefaultOps;

    child->duty = 0;
    child->name = (char *)pName;
    child->reload = NULL;
    dListInit(&child->port);
    if(pLoc->child == NULL)
    {
        pLoc->child = child;
        __OBJ_LIST_INIT(child);
    }
    else
    {
        __OBJ_LIST_INS_BEFORE(pLoc->child, child);
    }

    Int_LowAtomEnd(low_atom);

    return (child);
}

// ============================================================================
// 功能：给指定节点增加一个子节点,新节点将在队列头位置；
// 参数：pLoc -- 新节点的父节点；
//      ops -- 对象操作方法；
//      represent -- 对象表示；
//      pName -- 对象名字，所指定的字符串内存区不能释放，但可以是空；
// 返 回： 新加入的节点
// 备注：
// ============================================================================
struct Object *OBJ_AddChildHead(struct Object *pLoc,fnObjOps ops,
                                ptu32_t represent,const char *pName)
{
    struct Object *child;
    atom_low_t low_atom;

    if(pLoc == NULL)
        return (NULL);

    child = __OBJ_New();
    if(child == NULL)
        return (NULL);

    low_atom = Int_LowAtomStart();
    child->parent = pLoc;
    child->child = NULL;
    child->represent = represent;
    if(ops != NULL)
        child->ops = ops;
    else
        child->ops = (fnObjOps)OBJ_DefaultOps;

    child->duty = 0;
    child->name = (char *)pName;
    child->reload = NULL;
    dListInit(&child->port);
    if(pLoc->child == NULL)
    {
        pLoc->child = child;
        __OBJ_LIST_INIT(child);
    }
    else
    {
        __OBJ_LIST_INS_BEFORE(pLoc->child, child);
        pLoc->child = child;
    }

    Int_LowAtomEnd(low_atom);
    return (child);
}

// ============================================================================
// 功能：把一个对象插入到另一个对象的子节点位置，新节点将位于队列头位置，注意，
//      新节点可能有子节点。
// 参数：pLoc -- 父节点；
//      pObj -- 待插入的子节点，可能有下级节点；
// 返回：新节点指针；
// 备注：
// ============================================================================
struct Object *OBJ_InsertChild(struct Object *pLoc, struct Object *pObj)
{
    atom_low_t low_atom;

    if( (pLoc == NULL) || (pObj == NULL))
        return (NULL);

    pObj->parent = pLoc;
    low_atom = Int_LowAtomStart();
    if(pLoc->child == NULL)
    {
        pLoc->child = pObj;
        __OBJ_LIST_INIT(pObj);
    }
    else
    {
        __OBJ_LIST_INS_BEFORE(pLoc->child, pObj);
        pLoc->child = pObj;
    }

    Int_LowAtomEnd(low_atom);
    return (pObj);
}

#if 0
// ============================================================================
// 功能：用一个新节点替换掉对象队列中的原有节点，新节点原来必须不是队列中的节点
// 参数：pOld -- 被替换的节点；
//      pNew -- 新节点；
// 返回：成功（TRUE）;
// 备注：
// ============================================================================
bool_t OBJ_Displace(struct Object *pOld, struct Object *pNew)
{
    struct Object *temp1, *temp2;
    atom_low_t low_atom;

    if((NULL == pOld) || (NULL == pNew))
        return (FALSE);

    low_atom = Int_LowAtomStart();
    temp1 = pOld->child;
    if(temp1 != NULL)   //把oldnode所有子节点的父指针指向New
    {
        temp2 = temp1;
        do
        {
            temp2->parent = pNew;
            temp2 = temp2->next;
        }while(temp2 != temp1);
    }

    pNew->child = temp1;
    pNew->next = pOld->next;
    pNew->prev = pOld->prev;
    pNew->parent = pOld->parent;
    pOld->next->prev = pNew;
    pOld->prev->next = pNew;
    if(pOld->parent->child == pOld) //如果是队列头节点
        pOld->parent->child = pNew; //父节点的子节点指向队列头节点
    Int_LowAtomEnd(low_atom);

    return (TRUE);
}
#endif

// ============================================================================
// 功能：把一个树枝从对象队列中删除
// 参数：pBranch -- 被删除的分支。
// 返回：被删除分支指针，NULL表示分支不存在
// 备注：
// ============================================================================
struct Object *OBJ_TakeOutBranch(struct Object *pBranch)
{
    struct Object *result;
    atom_low_t low_atom;

    if(pBranch == NULL)
        return (NULL);

    low_atom = Int_LowAtomStart();
    if(pBranch->next == pBranch)   //说明该节点没有兄弟节点.
    {
        pBranch->parent->child = NULL;
    }
    else
    {
        if(pBranch->parent->child == pBranch)
        {   //说明该节点是队列头节点,需要改变队列头节点
            pBranch->parent->child = pBranch->next;
        }
        __OBJ_LIST_REMOVE(pBranch);
    }

    result = pBranch;
    Int_LowAtomEnd(low_atom);

    return (result);
}
// ============================================================================
// 功能：把一个节点从对象树中断开节点,该节点不能有子节点!
// 参数：pObj -- 被删除的节点,如该节点有子节点或正在使用则不删除
// 返回：成功（0），失败（-1）；
// 备注：
// ============================================================================
s32 OBJ_Del(struct Object *pObj)
{
    atom_low_t low_atom;

    if(pObj == NULL)
        return (-1);

    if((pObj->child != NULL) || __OBJ_IsProtected(pObj)) // 有子节点，或正在使用，不操作
        return (-1);

    low_atom = Int_LowAtomStart();
    __OBJ_Del(pObj);
    Int_LowAtomEnd(low_atom);
    __OBJ_Free(pObj);

    return (0);
}

// ============================================================================
// 功能：移动一个对象树枝到别的节点下面成为其子树；
// 参数：pParent -- 新父节点；
//      pObj，待插入的子节点，可能有下级节点；
// 返回：成功（0），失败（-1）；
// 备注：
// ============================================================================
s32 OBJ_MoveToBranch(struct Object *pParent,struct  Object *pObj)
{
    atom_low_t low_atom;
    if( (pParent == NULL) || (pObj == NULL))
        return (-1);

    low_atom = Int_LowAtomStart();
    OBJ_TakeOutBranch(pObj);
    pObj->parent = pParent;
    if(pParent->child == NULL)
    {
        pParent->child = pObj;
        __OBJ_LIST_INIT(pObj);
    }
    else
    {
        __OBJ_LIST_INS_BEFORE(pParent->child, pObj);
        pParent->child = pObj;
    }
    Int_LowAtomEnd(low_atom);
    return (0);
}

// ============================================================================
// 功能：朝next指针方向移动对象队列中的一个节点，到同级队列头的前一个节点位置；
// 参数：pObj -- 被移动的节点指针；
// 返回：成功（0），失败（-1）；
// 备注：
// ============================================================================
s32 OBJ_MoveToLast(struct Object *pObj)
{
    struct Object *eldest;
    atom_low_t low_atom;

    if(pObj == NULL)
        return (-1);

    low_atom = Int_LowAtomStart();
    if(pObj->parent != NULL)    //根节点不能移动
    {
        eldest = pObj->parent->child;
        if(eldest == pObj)
        //如果node是头节点,则直接移动父节点的子指针到下一个节点就可以了.
            pObj->parent->child = pObj->next;
        else if(eldest->prev != pObj)    //不是最后一个节点
        {
            __OBJ_LIST_REMOVE(pObj);
            __OBJ_LIST_INS_BEFORE(eldest, pObj);
        }
    }
    Int_LowAtomEnd(low_atom);

    return (0);
}

// ============================================================================
// 功能：朝previous指针方向移动对象队列中的一个节点，成为同级队列头；
// 参数：pObj -- 被移动的节点指针；
// 返回：成功（0），失败（-1）；
// 备注：
// ============================================================================
s32 OBJ_MoveToHead(struct Object *pObj)
{
    struct Object *eldest;
    atom_low_t low_atom;

    if(pObj == NULL)
        return (-1);

    low_atom = Int_LowAtomStart();
    if(pObj->parent != NULL)    //根节点不能移动
    {
        eldest = pObj->parent->child;
        if(eldest->prev == pObj)
        //如果node是last节点,则直接移动父节点的子指针到下一个节点就可以了.
            pObj->parent->child = pObj->prev;
        else if(eldest != pObj)    //不是头节点
        {
            __OBJ_LIST_REMOVE(pObj);
            __OBJ_LIST_INS_BEFORE(eldest, pObj);
            pObj->parent->child = pObj;
        }
    }

    Int_LowAtomEnd(low_atom);
    return (0);
}

// ============================================================================
// 功能：移动对象队列中的一个节点到另一个节点的next位置，被移动的节点可能有子节点；
// 参数：pLoc -- 目标位置，Obj移动到本节点后面；
//      pObj -- 目标节点,它可能原本就在Object队列中；
// 返回：成功（0），失败（-1）；
// 备注：
// ============================================================================
s32 OBJ_MoveToNext(struct Object *pLoc, struct Object *pObj)
{
    atom_low_t low_atom;

    if((pLoc == NULL) || (pObj == NULL) || (pLoc == pObj))
        return (-1);

    low_atom = Int_LowAtomStart();
    if(pLoc->next != pObj)    //看是否已经符合要求
    {
        //先把目标节点从队列中取出
        if(pObj->next == pObj)   //说明该节点没有兄弟节点.
        {
            pObj->parent->child = NULL;
        }
        else
        {
            if(pObj->parent->child == pObj)
            {   //说明该节点是队列头节点,需要改变队列头节点
                pObj->parent->child = pObj->next;
            }
            __OBJ_LIST_REMOVE(pObj);
        }

        __OBJ_LIST_INS_AFTER(pLoc, pObj);
        pObj->parent = pLoc->parent;
    }
    Int_LowAtomEnd(low_atom);

    return (0);
}

// ============================================================================
// 功能：移动对象队列中的一个节点NewPre到另一个节点Obj的previous位置，被移动的节点
//      可能有子节点
// 参数：pLoc -- 目标位置，Obj移动到本节点前面
//      pObj -- 目标节点，它原本就在Object队列中。
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 OBJ_MoveToPrevious(struct Object *pLoc, struct Object *pObj)
{
    atom_low_t low_atom;

    if((pLoc == NULL) || (pObj == NULL) || (pLoc == pObj))
        return (-1);

    low_atom = Int_LowAtomStart();
    if(pLoc->prev != pObj) // 看是否已经符合要求
    {
        //先把目标节点从队列中取出
        if(pObj->next == pObj)   //说明该节点没有兄弟节点.
        {
            pObj->parent->child = NULL;
        }
        else
        {
            if(pObj->parent->child == pObj)
            {   //说明该节点是队列头节点,需要改变队列头节点
                pObj->parent->child = pObj->next;
            }
            __OBJ_LIST_REMOVE(pObj);
        }

        __OBJ_LIST_INS_BEFORE(pLoc, pObj);
        pObj->parent = pLoc->parent;
    }
    Int_LowAtomEnd(low_atom);

    return (0);
}

// ============================================================================
// 功能: Parent的子节点的相对位置不变,队列头朝previous方向移动一格。
// 参数：pParent -- 父节点指针
// 返回：成功（0），失败（-1）；
// 备注：
// ============================================================================
s32 OBJ_RoundPrevious(struct Object *pParent)
{
    atom_low_t low_atom;

    if(pParent == NULL)
        return (-1);

    low_atom = Int_LowAtomStart();
    if(pParent->child != NULL)
    {
        pParent->child = pParent->child->prev;
    }
    Int_LowAtomEnd(low_atom);

    return (0);
}

// ============================================================================
// 功能：Parent的子节点的相对位置不变,队列头朝next方向移动一格。
// 参数：pParent -- 父节点指针
// 返回：成功（0），失败（-1）；
// 备注：
// ============================================================================
s32 OBJ_RoundNext(struct Object *pParent)
{
    atom_low_t low_atom;

    if(pParent == NULL)
        return (-1);

    low_atom = Int_LowAtomStart();
    if(pParent->child != NULL)
    {
        pParent->child = pParent->child->next;
    }

    Int_LowAtomEnd(low_atom);

    return (0);
}

// ============================================================================
// 功能：修改对象名；
// 参数：pObj -- 被修改的对象节点指针；
//      pName -- 新名字；
// 返回：成功（0），失败（-1）;
// 备注：
// ============================================================================
s32 OBJ_Rename(struct Object *pObj, const char *pName)
{
    if((pObj == NULL) || (pName == NULL))
        return (-1);

    pObj->name = (char *)pName;  //无需判断新名字字符串的长度，对象允许名字是空串

    return (0);
}

// ============================================================================
// 功能：返回某节点的名字；
// 参数：pObj,待操作的对象节点；
// 返回：该节点的名字；
// 注意：该节点可能没有名字，因此返回的即使是成功也可能为NULL，因为确实没有名字
//      成功失败不应该据此做判断
// ============================================================================
const char *OBJ_Name(struct Object *pObj)
{
    if(!pObj)
        return (NULL);

    return (__OBJ_Name(pObj));
}

// ============================================================================
// 功能: 返回某节点的父节点,
// 参数: pObj -- 需要查找的树枝的任意一个子节点
// 返回: 成功（父节点），失败（NULL）；
// 备注：
// ============================================================================
struct Object *OBJ_Parent(struct Object *pObj)
{
    if(!pObj)
        return (NULL);

    return (__OBJ_Parent(pObj));
}

// ============================================================================
// 功能：返回子节点，因父节点的child指针指向子节点的队列头，实际返回子节点的队列头；
// 参数：pObj -- 需要查找的树枝的父节点；
// 返回：成功（子节点），失败（NULL）；
// 备注：
// ============================================================================
struct Object *OBJ_Child(struct Object *pObj)
{
    if(!pObj)
        return (NULL);

    return (__OBJ_Child(pObj));
}

// ============================================================================
// 功能：返回Obj节点的previous节点
// 参数：pObj -- 需要查找的树枝的兄节点中的一个节点
// 返回：previous节点.
// 备注：
// ============================================================================
struct Object *OBJ_Previous(struct Object *pObj)
{
    if(!pObj)
        return (NULL);

    return (__OBJ_Prev(pObj));
}

// ============================================================================
// 功能：返回Obj节点的next节点；
// 参数：pObj -- 需要查找的树枝的兄节点中的一个节点；
// 返回：next节点；
// 备注：
// ============================================================================
struct Object *OBJ_Next(struct Object *pObj)
{
    if(!pObj)
        return (NULL);

    return (__OBJ_Next(pObj));
}

// ============================================================================
// 功能：返回Obj节点所在队列的队列头；
// 参数：pObj,需要查找的树枝的任意节点；
// 返回：队列头指针；
// 备注：
// ============================================================================
struct Object *OBJ_Head(struct Object *pObj)
{
    if(!pObj || !pObj->parent)
        return (NULL);

    return (pObj->parent->child);
}

// ============================================================================
//功能：返回某树枝的一个末梢节点,末梢节点是指没有子节点的节点.
//参数：pObj,需要搜索的树枝的父节点
//返回：树枝的一个末梢节点.当没有子节点时，返回NULL
//备注：当需要删除整个树枝时,本函数很有用,结合rsc_del_node函数,反复调用本函数,
//     并把返回的节点删除,直到本函数返回NULL.需要删除一个文件夹或者删除一个存
//     在子窗口的gui窗口时,就需要用到删除整个树枝的操作.
// ============================================================================
struct Object *OBJ_Twig(struct Object *pObj)
{
    struct Object *result = NULL, *current;

    if(pObj == NULL)
        return (NULL);

    current = pObj;
    while(current->child != NULL)
    {
        current = current->child;
    }

    if(current == pObj)
        result = NULL;
    else
        result = current;

    return (result);
}

// ============================================================================
// 功能：查看某节点是根节点的第几级子节点；
// 参数：pObj -- 被查的节点；
// 返回：子节点级数，0代表根节点。cn_limit_uint32代表出错。
// 备注：
// ============================================================================
u32 OBJ_Level(struct Object *pObj)
{
    u32 n = 0;
    struct Object *pl_node = pObj;

    if(pl_node == NULL)
        return CN_LIMIT_UINT32;

    while(pl_node->parent !=  pRootObj)
    {
        if(NULL != pl_node->parent)
            pl_node = pl_node->parent;
        else
        {
            n = CN_LIMIT_UINT32;
            break;
        }
        n++;
    }

    return (n);
}

#if 0
//----取节点的路径-------------------------------------------------------------
//功能: 查看某节点是根节点的第几级子节点，
//参数: pObj,待提取路径的节点
//      path，保存路径的缓冲区
//      limit，缓冲区长度
//返回: path地址，出错则返回NULL
//-----------------------------------------------------------------------------
char *OBJ_GetPath(struct Object *pObj, char *path, u32 limit)
{
    struct Object *Current,*root;
    char *pos;
    u32 count = 0;
    if((pObj == NULL)||(path == NULL) || (limit < 2))
        return (NULL);
    Current = pObj;
    root = pRootObj;
    pos = path + limit - 2;     //留出最后一个 '\0'字符位置
    while(1)
    {
        if(Current != root )
        {
            count = strlen(Current->name);
            pos -= count;
            if(pos <= path)     //path[0]也要保留。
                return (NULL);
            memcpy(pos, Current->name, count);
            pos--;
            *pos = '/';
            Current = Current->parent;
        }
        else
        {
            count = limit - 2 - (pos - path);
            memcpy(path+1, pos, count);
            path[0] = '/';
            path[count +1] = '\0';
            break;
        }
    }
    return path;
}
#endif

// ============================================================================
// 功能：从当前节点开始,获取下一个节点的指针,直到遍历完全部子节点
// 参数：pParent -- 需要搜索的树枝的祖先节点
//      pChild -- 当前搜索位置,起始位置应该设为与parent相等，才能搜索全部子节点。
// 返回：当前搜索位置的下一个节点指针,如果已经搜索完成,则返回NULL.
// 备注：
// ============================================================================
struct Object *OBJ_TraveChild(struct Object *pParent, struct Object *pChild)
{
    if((pParent == NULL) || (pChild == NULL))
        return (NULL);

    if(pChild == pParent)
    {
        pChild = pParent->child;
    }
    else
    {
        pChild = pChild->next;
        if(pChild == pParent->child)
            pChild = NULL;
    }

    return (pChild);
}

// ============================================================================
// 功能：从当前节点开始,获取下一个节点的指针,沿着搜索路线,直到搜索完整个树枝.搜索
//      路线为:当前节点的子节点,如果子节点为空则搜索next节点,next节点又为空则搜
//      索父节点的next节点,直到搜索完成.
//      本函数只返回子孙节点，不返回ancestor_node本身
// 参数：pAncestor,需要搜索的树枝的祖先节点
//      Current,当前搜索位置,
// 返回：当前搜索位置的下一个节点指针,如果已经搜索完成,则返回NULL.
// 备注：当需要对对象树中某一个树枝或者整个链表中的节点逐一进行某种操作时,可
//      反复调用本函数,第一次调用current_node = parent_node,其后current_node
//      = 上次返回值，直到返回空.
//      本函数按父、子、孙、曾孙....的顺序搜索，先搜直系，再搜旁系，确保所有子孙
//      节点都能够访问到，如果对访问顺序有特殊要求，不能使用本函数
// ============================================================================
struct Object *OBJ_TraveScion(struct Object *pAncestor, struct Object *pCurrent)
{
    struct  Object *result = NULL,*current_copy;
    bool_t up = FALSE;

    if((pAncestor == NULL) || (pCurrent == NULL))
        return (NULL);

    if((pCurrent != pAncestor) || pAncestor->child)
    {
        current_copy = pCurrent;
        do
        {
            if((up == FALSE) && (current_copy->child != NULL))
            {   //子节点非空,返回子节点
                result = current_copy->child;
                break;
            }
            else if(current_copy->next != current_copy->parent->child)
            {   //子节点空,但本节点不是队列尾节点,返回next节点
                result = current_copy->next;
                break;
            }
            else
            {   //无子节点,且本节点已经是队列尾节点,需要判断当前父节点的next节点
                current_copy = current_copy->parent;
                up = true;
            }
        }while(current_copy != pAncestor);
    }

    return (result);
}

// ============================================================================
// 功能：在某一个对象节点的所有兄弟节点中搜索给定名字的对象
// 参数：pBrother,兄弟节点中任意一个对象指针.
//      pName,需要搜索的对象名
// 返回：如果搜索到对象返回对象节点指针,否则返回NULL
// 备注：
// ============================================================================
struct Object *OBJ_SearchSibling(struct Object *pBrother, const char *pName)
{
    struct Object *temp, *result = NULL;

    if((pBrother == NULL) || (pName == NULL))
        return (NULL);

    temp = pBrother;
    do
    {
        if(strcmp(temp->name, pName) == 0)
        {
            result = temp;
            break;
        }
        temp = temp->next;
    }while (temp != pBrother);

    return (result);
}

// ============================================================================
// 功能：在某一个对象节点的所有子节点中搜索给定名字的对象；
// 参数：pParent -- 父节点对象指针；
//      pName -- 需要搜索的对象名,纯对象名,不包含路径；
// 返回：如果搜索到对象返回对象节点指针,否则返回NULL；
// 备注：
// ============================================================================
struct Object *OBJ_SearchChild(struct Object *pParent, const char *pName)
{
    struct Object *temp, *result = NULL;

    if((pName == NULL) || (pParent == NULL))
        return (NULL);

    temp = pParent->child;
    if(temp == NULL)
    {
        result = NULL;
    }
    else
    {
        do
        {
            if(strcmp(temp->name, pName)==0)
            {
                result = temp;
                break;
            }
            temp = temp->next;
        }while (temp != pParent->child);
    }

    return (result);
}

// ============================================================================
// 功能：与OBJ_SearchChild类似,搜索Ancestor的所有后代节点,直到找到一个名称
//      匹配对象。
// 参数：pAncestor,树枝的父节点
//      pName,需要搜索的对象名,纯对象名,不包含路径.
// 返回：如果搜索到对象返回对象节点指针,否则返回NULL
// 备注：
// ============================================================================
struct Object *OBJ_SearchScion(struct Object *pAncestor, const char *pName)
{
    struct Object *current, *temp, *result = NULL;

    if((pAncestor == NULL) || (pName == NULL))
        return (NULL);

    current = pAncestor;
    //在rsc_trave_scion中已经有信号量保护，此处无须保护
    while((temp = OBJ_TraveScion(pAncestor, current)) != NULL)
    {
        if(strcmp(temp->name, pName) == 0)
        {
            result = temp;
            break;
        }
        current = temp;
    }

    return (result);
}

// ============================================================================
// 功能：与OBJ_SearchScion类似,不同的是，path是包含路径名的字符串，执行精确搜索。
// 参数：pStart -- 树枝的父节点；
//      pPath -- 包含路径名的对象名；
// 返回：如果搜索到对象返回对象节点指针，否则返回NULL；
// 备注：
// ============================================================================
struct Object *OBJ_Search(struct Object *pStart, const char *pPath)
{
    const char *path_name;
    char *ObjName;
    bool_t end = FALSE;
    u32 i;
    struct Object *current, *result = NULL;

    if((pPath == NULL) || (pStart == NULL))
        return (NULL);

    current = pStart->child;
    if(current == NULL)
    {
        return (NULL);
    }

    path_name = pPath;
    while(1)
    {
        result = current;
        ObjName = current->name;
        for(i=0;i<CN_OBJ_NAME_LIMIT;i++)        //对象名长度限制
        {
            if(ObjName[i] == path_name[i])
            {
                if(path_name[i] == '\0')        //匹配，且path结束
                {
                    result = current;
                    end = true;
                }
            }
            else if( (ObjName[i] == '\0')
                    && ((path_name[i] == '\\') || (path_name[i] == '/')))
            {   //对象名字结束，path到达名字分隔符
                if(path_name[i+1] == '\0')      //匹配，且path结束
                {
                    result = current;
                    end = true;
                }
                else                        //匹配，但path未结束,继续查找子节点
                {
                    path_name += i+1;
                    current = current->child;
                    if(current == NULL)     //没有子节点了，但path未结束
                    {
                        result = NULL;
                        end = true;
                        break;
                    }
                }
            }
            else // 字符串不相等
            {
                current = current->next;
                if(current == current->parent->child)
                {
                    //本级的所有节点已经搜索完,没有找到匹配的.
                    result = NULL;
                    end = true;
                }

                break;
            }
        }
        if(end)
            break;
    }

    return result;
}
#if 0
//----沿路径匹配对象名---------------------------------------------------------
//功能: 与OBJ_Search类似,不同的是，找到第一个匹配不上的就返回。例如，对象树中有
//      "obj1\obj2\"，path="obj1\obj2\obj3\obj4"，将返回obj2的指针。
//参数: Start,树枝的父节点
//      Path,包含路径名的对象名
//      pos，第一个不匹配的，NULL表示全匹配。
//返回: 最后一个匹配的对象指针，完全不匹配则返回NULL
//-----------------------------------------------------------------------------
struct Object *OBJ_MatchPath(struct Object *Start, const char *Path,char **pos)
{
    char *path_name;
    char *ObjName;
    bool_t end = FALSE;
    u32 i;
    struct Object *current, *result = NULL;

    if((Path == NULL) || (Start == NULL))
        return NULL;

    current = Start->child;
    if(current == NULL)
    {
        return NULL;
    }
    path_name = (char *)Path;
    while(1)
    {
        result = current;
        ObjName = current->name;
        for(i=0;i<CN_OBJ_NAME_LIMIT;i++)        //对象名长度限制
        {
            if(ObjName[i] == path_name[i])
            {
                if(path_name[i] == '\0')        //匹配，且path结束
                {
                    result = current;
                    *pos = NULL;
                    end = true;
                }
            }
            else if( (ObjName[i] == '\0')
                    && ((path_name[i] == '\\') || (path_name[i] == '/')))
            {   //对象名字结束，path到达名字分隔符
                result = current;
                if(path_name[i+1] == '\0')      //匹配，且path结束
                {
                    *pos = NULL;
                    end = true;
                }
                else                        //匹配，但path未结束,继续查找子节点
                {
                    path_name += i+1;
                    *pos = path_name;
                    current = current->child;
                    if(current == NULL)
                    {
                        end = true;
                        break;
                    }
                }
            }
            else
            {   //字符串不相等
                current = current->next;
                if(current == current->parent->child)
                {
                //本级的所有节点已经搜索完,没有找到匹配的.
                    end = true;
                }
                break;
            }
        }
        if(end)
            break;
    }

    return result;
}
#endif

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
void OBJ_DelChild(struct Object *pChild)
{
    __OBJ_LIST_REMOVE(pChild);
    __OBJ_Free(pChild);
}

#if 0
// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
u32 OBJ_IncUseOne(struct Object *pObj)
{
    if(pObj == NULL)
    {
        return (0);
    }
    else
    {
        if(pObj->duty != CN_LIMIT_UINT32)
            pObj->duty++;

        return (pObj->duty);
    }
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
void OBJ_IncUseAlongPath(struct Object *pObj)
{
    while(pObj != NULL)
    {
        if(pObj->duty != CN_LIMIT_UINT32)
            pObj->duty++;

        pObj = pObj->parent;
    }
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
u32 OBJ_DecUseOne(struct Object *pObj)
{
    if(!pObj)
        return (0);

    if(pObj->duty != 0)
        pObj->duty--;

    return (pObj->duty);

}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
void OBJ_DecUseAlongPath(struct Object *pObj)
{
    while(pObj != NULL)
    {
        if(pObj->duty != 0)
            pObj->duty++;
        pObj = pObj->parent;
    }
    return ;
}
#endif

//----是否父子关系-------------------------------------------------------------
//功能：判断两个对象是否父子关系
//参数：pParent，可能的父对象。
//      pChild， 可能的子对象
//返回：true = 父子关系成立，FALSE = 不成立
//-----------------------------------------------------------------------------
bool_t OBJ_IsParentChild(struct Object *pParent, struct Object *pChild)
{
    struct Object *temp;
    bool_t result = FALSE;

    if((pChild == NULL) || (pParent == NULL))
        return (FALSE);

    temp = pParent->child;
    if((temp == NULL) || (pChild->parent != pParent))
        result = FALSE;
    else
    {
        do
        {
            if(pParent->child == pChild)
            {
                result = true;
                break;
            }
            temp = temp->next;
        }while (temp != pParent->child);
    }

    return result;
}

//----是否队列头-------------------------------------------------------------
//功能: 判断给定节点是否兄弟节点中的队列头节点
//参数: pObj，被测试节点
//返回: true=是队列头，FALSE=不是队列头
//-----------------------------------------------------------------------------
bool_t OBJ_IsHead(struct Object *pObj)
{
    if(NULL == pObj)
        return (FALSE);

    if(pObj == pRootObj)
        return (TRUE);

    if((pObj) && (pObj  == pObj->parent->child))
        return (TRUE);
    else
        return (FALSE);
}

//----是否队列尾---------------------------------------------------------------
//功能: 判断给定节点是否兄弟节点中的最后一个节点
//参数: pObj，被测试节点
//返回: true=是最后一个节点，FALSE=不是
//-----------------------------------------------------------------------------
bool_t OBJ_IsLast(struct Object *pObj)
{
    if(NULL == pObj)
        return (FALSE);

    if(pObj == pRootObj)
        return (TRUE);

    if(pObj->next  == pObj->parent->child)
        return (TRUE);
    else
        return (FALSE);
}

//----取节点顺序-------------------------------------------------------------
//功能: 取给定节点在兄弟节点中的位置，head=0，下一个是1，类推之
//参数: pObj，被测试节点
//返回: 节点位置号
//-----------------------------------------------------------------------------
u32 OBJ_Sequencing(struct Object *pObj)
{
    u32 ranking;
    struct Object *temp;

    if(NULL == pObj)
        return (-1);

    if(pObj == pRootObj)
        return (0);
        
    ranking = 0;
    temp = pObj->parent->child;
    while(temp != pObj)
    {
        ranking +=1;
        temp = temp->next;
    }
    
    return (ranking);
}

