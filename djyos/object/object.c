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
#include <stdio.h>

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


#define CN_OBJ_NAME_LIMIT               255 // 对象名最大长度；
#define DEFAULT_LOCK_TIMEOUT            (360*1000*mS) // 一个小时，不设未FOREVER是为了防止逻辑锁死；
#define BASIC_OBJECTS                   32

static inline s32 __checkname(const char *name);
static inline s32 __lock_objsys(void);
static inline void __unlock_objsys(void);
static inline s32 __init_obj_struct_pool(void);
static inline struct obj *__newobj(void);
static inline void __freeobj(struct obj *ob);

static struct MemCellPool __basic_obj_pool;
static struct obj __basic_obj[BASIC_OBJECTS];
static struct obj *__root_obj, *__current_obj; // 逻辑上，应该是每一个进程一个；
static struct MutexLCB __objsys_mutex;
static const char *__uname_obj = "un_named";
static ptu32_t __objsys_default_ops(enum objops ops, ptu32_t oof, ptu32_t args, ...);

struct __statics
{
    u32 news;
    u32 fress;
    u32 alives;
}__objsys_statics;

// ============================================================================
// 功能：为对象提供一个默认操作函数，当你只是要创建一个对象，并不需要作为文件操作
//      时，就不需要提供 Operation 函数，此时，系统会自动把对象操作函数指向这里。
// 参数：都没有意义。
// 返回：固定返回 OBJUNSUPPORTED
// 备注：
// ============================================================================
static inline ptu32_t __objsys_default_ops(enum objops ops, ptu32_t p_hdl, ptu32_t args, ...)
{
    return (OBJUNSUPPORTED);
}

// ============================================================================
// 功能：锁定对象系统；
// 参数：
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static inline s32 __lock_objsys(void)
{
    if(TRUE==Lock_MutexPend(&__objsys_mutex, DEFAULT_LOCK_TIMEOUT))
        return (0);

    return (-1);
}

// ============================================================================
// 功能：解锁对象系统；
// 参数：
// 返回：
// 备注：
// ============================================================================
static inline void __unlock_objsys(void)
{
    Lock_MutexPost(&__objsys_mutex);
}

// ============================================================================
// 功能：初始化对象池
// 参数：
// 返回：成功（0）；失败（-1）；
// 备注：POOL的设置是可以自动扩展
// ============================================================================
static inline s32 __init_obj_struct_pool(void)
{
    if(Mb_CreatePool_s(&__basic_obj_pool, __basic_obj,
            BASIC_OBJECTS, sizeof(struct obj), 16, 16384, "object"))
        return (0);

    return (-1);
}

// ============================================================================
// 功能：申请新对象
// 参数：无；
// 返回：成功申请（新对象）；失败（NULL）；
// 备注：
// ============================================================================
static inline struct obj *__newobj(void)
{
    struct obj *ob;

    ob = (struct obj*)Mb_Malloc(&__basic_obj_pool, 0);
    if(ob)
        memset(ob, 0x0, sizeof(*ob));

    __objsys_statics.news++;
    __objsys_statics.alives++;
    return (ob);
}

// ============================================================================
// 功能：释放对象
// 参数：ob -- 对象；
// 返回：无；
// 备注：
// ============================================================================
static inline void __freeobj(struct obj *ob)
{
    Mb_Free(&__basic_obj_pool, ob);
    __objsys_statics.fress++;
    __objsys_statics.alives--;
}

// ============================================================================
// 功能：检查对象名的合法性；
// 参数：name -- 对象名；
// 返回：合法（0）；非法（0）；
// 备注：
// ============================================================================
static inline s32 __checkname(const char *name)
{
    u16 i;

    for(i=0; i<CN_OBJ_NAME_LIMIT; i++)
    {
        if(('/' == name[i])||('\\' == name[i]))
            return (-1);

        if('\0' == name[i])
            return (0);
    }

    return (-1);
}

// ============================================================================
// 功能：获取对象的值；
// 参数：ob -- 对象；
// 返回：对象的值；
// 备注：
// ============================================================================
inline ptu32_t obj_GetPrivate(struct obj *ob)
{
    if(ob)
        return (ob->ObjPrivate);

    return (0);
}

// ============================================================================
// 功能：获取对象的集合点；
// 参数：ob -- 对象；
// 返回；集合点；
// 备注：
// ============================================================================
inline struct obj *obj_set(struct obj *ob)
{
    if(ob)
        return (ob->set);

    return (NULL);
}

// ============================================================================
// 功能：获取对象的引用关系；
// 参数：ob -- 对象；
// 返回：对象的引用关系；
// 备注：
// ============================================================================
inline list_t *obj_quote(struct obj *ob)
{
    if(dListIsEmpty(&ob->handles))
        return (NULL);

    return (dListGetAfter(&ob->handles));
}

// ============================================================================
// 功能：获取对象操作；
// 参数：ob -- 对象；
// 返回：成功（对象操作）；失败（NULL）；
// 备注：
// ============================================================================
inline fnObjOps obj_ops(struct obj *ob)
{
    if(!ob)
        return (NULL);

    return (ob->ops);
}

// ============================================================================
// 功能：获取对象名；
// 参数：ob -- 对象；
// 返回：成功（对象名）；失败（NULL）；
// 注意：
// ============================================================================
inline const char *obj_name(struct obj *ob)
{
    if(!ob)
        return (NULL);

    return ((const char*)ob->name);
}

// ============================================================================
// 功能: 获取对象的父关系对象；
// 参数: ob -- 对象；
// 返回: 成功（父节点），失败（NULL）；
// 备注：
// ============================================================================
inline struct obj *obj_parent(struct obj *ob)
{
    if(!ob)
        return (NULL);

    return (ob->parent);
}

// ============================================================================
// 功能：获取对象的子关系对象（同级子对象的链表链表头）
// 参数：ob -- 对象
// 返回：成功（子对象）；失败（NULL）；
// 备注：
// ============================================================================
inline struct obj *obj_child(struct obj *ob)
{
    if(!ob)
        return (NULL);

    return (ob->child);
}

// ============================================================================
// 功能：获取对象的前一个对象（同级对象的链表之上成员）
// 参数：ob -- 对象；
// 返回：成功（前一个对象）；失败（NULL）；
// 备注：
// ============================================================================
inline struct obj *obj_prev(struct obj *ob)
{
    if(!ob)
        return (NULL);

    return (ob->prev);
}

// ============================================================================
// 功能：获取对象的后一个对象（对象链表之上成员）
// 参数：ob -- 对象；
// 返回：成功（后一个对象）；失败（NULL）；
// 备注：
// ============================================================================
inline struct obj *obj_next(struct obj *ob)
{
    if(!ob)
        return (NULL);

    return (ob->next);
}

// ============================================================================
// 功能：获取对象链表的首成员
// 参数：ob -- 对象；
// 返回：成功（后一个对象）；失败（NULL）；
// 备注：
// ============================================================================
inline struct obj *obj_head(struct obj *ob)
{
    if(!ob)
        return (NULL);

    if(!ob->parent)
        return (ob); // 没有父对象，其本身就是单独的，那它也就是首成员了；

    return (ob->parent->child);
}

// ============================================================================
// 功能：获取对象在对象体系中的等级（深度）；
// 参数：ob -- 对象；
// 返回：成功（深度）；失败（-1）；
// 备注：根表示0级，根的子关系对象表示1级，根的子关系对象的子关系对象表示2级；......
//      目前限定深度不大于256，考虑的原因是，大于256时，实际上路径也是可能表示不出来的；
// ============================================================================
s32 obj_level(struct obj *ob)
{
    u32 level = 0;
    struct obj *pl_node = ob;

    if(pl_node==NULL)
        return (-1);

    if(__lock_objsys())
        return (-1);

    while(pl_node->parent!=__root_obj)
    {
        if(NULL!=pl_node->parent)
        {
            pl_node = pl_node->parent;
        }
        else
        {
            break; // 如果没有父节点，也认为是到头了；
        }

        if(level++>256) // 防止数据被破坏，死锁；
        {
            __unlock_objsys();
            return (-1);
        }
    }

    __unlock_objsys();
    return (level);
}

// ============================================================================
// 功能：获取对象在同级链表中的位置；
// 参数：ob -- 对象；
// 返回：对象的位置；出错（-1）；
// 备注：链表头为0；链表的第二个成员为1；......
// ============================================================================
u32 obj_order(struct obj *ob)
{
    u32 order;
    struct obj *tmp;

    if(!ob)
        return (-1);

    if(ob==__root_obj)
        return (0);

    if(__lock_objsys())
        return (-1);

    order = 0;
    tmp = ob->parent->child;
    while(tmp!=ob)
    {
        order +=1;
        tmp = tmp->next;
    }

    __unlock_objsys();
    return (order);
}

// ============================================================================
// 功能：设置对象的值；
// 参数：ob -- 对象；
//      represent -- 对象的值；
// 返回：无；
// 备注：
// ============================================================================
inline void obj_setval(struct obj *ob, ptu32_t Private)
{
    if(ob)
        ob->ObjPrivate = Private;
}

// ============================================================================
// 功能：设置对象操作；
// 参数：ob -- 对象；
//      ops -- 对象操作；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
inline s32 obj_setops(struct obj *ob, fnObjOps ops)
{
    if(!ob)
        return (-1);

    ob->ops = ops;
    return (0);
}

// ============================================================================
// 功能：设置对象的引用关系；
// 参数：ob -- 对象；
//      quote -- 引用关系；
// 返回：无；
// 备注：
// ============================================================================
inline void obj_setquote(struct obj *ob, list_t *quote)
{
    if(ob&&quote)
        dListInsertBefore(&ob->handles, quote);
}

// ============================================================================
// 功能：初始化对象系统；
// 参数：无
// 返回：成功（0）；失败（-1）。
// 备注：
// ============================================================================
s32 raw_objsys_init(void)
{
    s32 res;

    if(!Lock_MutexCreate_s(&__objsys_mutex, "obj sys"))
        return (-1);

    res = __init_obj_struct_pool();
    if(res)
    {
        Lock_MutexDelete_s(&__objsys_mutex);
        return (-1);
    }

    __root_obj = __newobj();
    if(!__root_obj)
    {
        Lock_MutexDelete_s(&__objsys_mutex);
        return (-1);
    }

    __OBJ_LIST_INIT(__root_obj);
    __root_obj->name = "";
    __root_obj->parent = __root_obj;
    __root_obj->set = __root_obj;
    __root_obj->seton = __root_obj;
    __root_obj->ops = (fnObjOps)__objsys_default_ops;
    __current_obj = __root_obj;
    return (0);
}

// ============================================================================
// 功能：对象是否是同级链表的头成员；
// 参数：ob -- 对象；
// 返回：是（1）；否（0）；
// 备注：
// ============================================================================
inline s32 obj_ishead(struct obj *ob)
{
    if(!ob)
        return (0);

    if((ob==ob->parent->child)||(ob==__root_obj))
        return (1);
    else
        return (0);
}

// ============================================================================
// 功能：对象是否是同级链表的尾成员；
// 参数：ob -- 对象；
// 返回：是（1）；否（0）；
// 备注：
// ============================================================================
inline s32 obj_islast(struct obj *ob)
{
    if(!ob)
        return (0);

    if((ob->next==ob->parent->child)||(ob==__root_obj))
        return (1);
    else
        return (0);
}

// ============================================================================
// 功能：对象是否被使用中；
// 参数：ob -- 对象；
// 返回：文件使用中（1）；文件未使用（0）
// 备注：
// ============================================================================
inline s32 obj_isonduty(struct obj *ob)
{
    if(!ob)
        return (0);

    if((!dListIsEmpty(&ob->handles)) // 未被引用
       ||(ob->rights.b.muses)
       ||(ob->child))
        return (1);

    return (0);
}

// ============================================================================
// 功能：对象是否是对象集合点；
// 参数：ob -- 对象；
// 返回：是（1）；否（0）；
// 备注：
// ============================================================================
inline s32 obj_isset(struct obj *ob)
{
    if(!ob)
        return (0);

    return (ob==ob->set);
}

// ============================================================================
// 功能：对象是否被引用；
// 参数：ob -- 对象；
// 返回：是（1）；否（0）；
// 备注：
// ============================================================================
inline s32 obj_isquoted(struct obj *ob)
{
    if(!ob)
        return (0);

    if(dListIsEmpty(&ob->handles))
        return (0);

    return (1);
}

// ============================================================================
// 功能：对象是否是临时建立的；
// 参数：ob -- 对象；
// 返回：是（1）；否（0）；
// 备注：用于构建路径等；
// ============================================================================
inline s32 obj_istmp(struct obj *ob)
{
    if((!obj_isset(ob)) // 不是对象集合点；
       &&(!ob->ObjPrivate)) // 对象无内容；
        return (1);

    return (0);
}

// ============================================================================
// 功能：是否允许建立新的对象集合；
// 参数：ob -- 对象；
// 返回：允许（1）；不允许（0）；
// 备注：
// ============================================================================
inline s32 obj_testset(struct obj *ob)
{
    if(ob&&(-1==(u32)(ob->seton)))
        return (0);

    return (1);
}

// ============================================================================
// 功能：允许在对象上建立新的对象集合；
// 参数：ob -- 对象；
// 返回：成功允许（0）；不能允许（-1）；
// 备注：
// ============================================================================
inline s32 obj_allow2set(struct obj *ob)
{
    if(!ob)
        return (-1);

    if(-1==(u32)ob->seton)
        ob->seton = (struct obj*)0;

    return (0);
}

// ============================================================================
// 功能：不允许的本对象上建立新的对象集合；
// 参数：ob -- 对象；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
inline s32 obj_ban2set(struct obj *ob)
{
    if(!ob)
        return (-1);

    if(0==ob->seton)
        ob->seton = (struct obj*)-1;

    return (0);
}

// ============================================================================
// 功能：将对象转为对象集合;
// 参数：ob -- 对象；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
inline s32 obj_change2set(struct obj *ob)
{
    if((!ob)
       ||(!obj_testset(ob)) // 对象不允许新建集合
       ||(obj_isset(ob))) // 对象已经是对象集合
        return (-1);

    ob->set = ob;
    ob->seton = ob;
    return (0);
}


// ============================================================================
// 功能：对象上锁，禁止新访问；
// 参数：ob -- 对象；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 obj_lock(struct obj *ob)
{
    if(__lock_objsys())
        return (-1);

    if(ob->rights.b.lock) // 已经上锁
    {
        __unlock_objsys();
        return (-1);
    }

    ob->rights.b.lock = 1;
    __unlock_objsys();
    return (0);
}

// ============================================================================
// 功能：对象解锁；
// 参数：ob -- 对象；
// 返回：成功（0）；失败（-1）；
// 备注：INLINE
// ============================================================================
s32 obj_unlock(struct obj *ob)
{
    if(__lock_objsys())
        return (-1);

    ob->rights.b.lock = 0;
    __unlock_objsys();
    return (0);
}

// ============================================================================
// 功能：对象增引用计数；
// 参数：ob -- 对象；
// 返回：成功（0）；失败（-1）；
// 备注：有引用后则不可删除
// ============================================================================
s32 obj_up(struct obj *ob)
{
    if(!ob)
        return (-1);

    if(__lock_objsys())
        return (-1);

    if((ob->rights.b.lock)||(-1==ob->rights.b.muses)) // 对象被禁止访问（将被删除），或者同步使用者达到上限；
    {
        __unlock_objsys();
        return (-1);
    }

    ob->rights.b.muses++;
    __unlock_objsys();
    return (0);
}

// ============================================================================
// 功能：对象减引用计数；
// 参数：ob -- 对象；
// 返回：成功（0）；失败（-1）；
// 备注：有引用后则不可删除
// ============================================================================
inline s32 obj_down(struct obj *ob)
{
    if(__lock_objsys())
        return (-1);

    if(ob->rights.b.muses)
        ob->rights.b.muses--;

    __unlock_objsys();
    return (0);
}

// ============================================================================
// 功能：删除对象
// 参数：ob -- 对象；
// 返回：成功（0），失败（-1）；
// 备注：
// ============================================================================
s32 obj_del(struct obj *ob)
{
    if(!ob)
        return (-1);

    if(__lock_objsys())
        return (-1);

    if(obj_isonduty(ob))
    {
        __unlock_objsys();
        return (-1); // 正在使用
    }

    if(ob->next==ob) // 说明该节点没有兄弟节点.
    {
        ob->parent->child = NULL;
    }
    else
    {
        if(ob->parent->child==ob)
            ob->parent->child = ob->next; // 说明该节点是队列头节点,需要改变队列头节点

        __OBJ_LIST_REMOVE(ob);
    }

    __freeobj(ob);
    __unlock_objsys();
    return (0);
}

// ============================================================================
// 功能：将对象及其拓展（子对象等）从对象树关系上分离；
// 参数：branch -- 被删除的分支;
// 返回：成功（被分离的对象分支）;失败（NULL）；
// 备注：
// ============================================================================
struct obj *obj_detach(struct obj *branch)
{
    struct obj *ob;

    if(!branch)
        return (NULL);

    if(__lock_objsys())
        return (NULL);

    if(branch->next==branch)   //说明该节点没有兄弟节点.
    {
        branch->parent->child = NULL;
    }
    else
    {
        if(branch->parent->child == branch)
        {   //说明该节点是队列头节点,需要改变队列头节点
            branch->parent->child = branch->next;
        }

        __OBJ_LIST_REMOVE(branch);
    }

    ob = branch;
    __unlock_objsys();
    return (ob);
}

// ============================================================================
// 功能：检查对象名的合法性；
// 参数：name -- 对象名；
// 返回：合法（0）；非法（-1）；
// 备注：检查规则：不允许存在'/'、'\'字符，长度不允许超过规定；
// ============================================================================
inline s32 obj_checkname(const char *name)
{
    if(name)
        return (-1);

    return (__checkname(name));
}

// ============================================================================
// 功能：匹配路径
// 参数：pPath -- 需匹配的路径；
//      left -- 完全匹配，为NULL；不完全匹配，则返回不匹配部分（保证不以'/'开头）；
//      full -- 从最近的安装点开始的匹配路径（同一个文件系统下的路径）；
// 返回：匹配路径所能检索到的结束对象。
// 备注：
// ============================================================================
struct obj *obj_matchpath(const char *match, char **left, char **full)
{
    char *path = (char*)match;
    char *sfull = NULL;
    char *segment;
    struct obj *current, *head;
    u32 i;

    if(__lock_objsys())
        return (NULL);

    if('/' == *path)
    {
        current = objsys_root(); // 绝对路径
    }
    else
    {
        current = objsys_current();// 其他都是相对路径
    }

    //if(GROUP_POINT == __OBJ_Type(current)) // 安装点
    if(obj_isset(current))
        sfull = path;

    while(current)
    {
        while('/' == *path)
            path++; // 过滤多余的'/'

        if('\0' == *path)
            break; // 遍历路径结束

        if(('.' == path[0]) && ('.' == path[1]) &&
            (('\\' == path[3]) || ('/' == path[3]) || ('\0' == path[3])))
        {
            //if(GROUP_POINT == __OBJ_Type(current))
            if(obj_isset(current))
                sfull = NULL; // 从安装点向上检索，其全路径逻辑就失效了

            current = obj_parent(current);
            path += 2; // ".."字符，表示上一级
            continue;
        }
        else if(('.' == path[0]) &&
            (('\\' == path[1]) || ('/' == path[1]) || ('\0' == path[1])))
        {
            path += 1; // "."字符，表示当前级
            continue;
        }

        head = current->child; // 遍历子节点
        if(!head)
            break;

        segment = (char*)obj_name(head);
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
            segment = (char*)obj_name(current);
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

        current = obj_parent(current); // 当前节点与路径不匹配，退回上上以及节点
        break; // 未找到节点

__MATCH_NEXT:
        if(obj_isset(current))
            sfull = path;
    }

__MATCH_DONE:
    if(full)
    {
        if(obj_isset(current))
            *full = path; // // 匹配的最末节点是安装点
        else
            *full = sfull;
    }

    if(left)
    {
        if('\0' == *path)
            *left = NULL;
        else
            *left = path; // 逻辑上需保证不会以'/'开头
    }

    __unlock_objsys();
    return (current);
}

// ============================================================================
// 功能：构建基于对象的路径
// 参数：begin -- 开始对象节点；
//      path -- 需建立的临时路径；
// 返回：目标对象；
// 备注：这里新建时，有对重名做判断；
// ============================================================================
struct obj *obj_buildpath(struct obj *begin, char *path)
{
    char *segst, *name;
    struct obj *current, *find;
    u16 i, seglen;

    if(!path)
        return (begin);

    if(__lock_objsys())
        return (NULL);

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
            printf("\r\n: dbug : object : memory out(%s). ", __FUNCTION__);
            __unlock_objsys();
            return (NULL);
        }

        for(i = 0 ; i < seglen; i++)
            name[i] = segst[i];

        name[seglen] = '\0';
        find = obj_search_child(current, name);
        if(find)
        {
            current = find; // child已经存在；
            free(name);
        }
        else
        {
            // 这里的ops和represent是一个继承关系，即继承父节点的
            // current = OBJ_AddChild(current, current->ops, current->represent, (const char*)name);
            current = obj_newchild(current, NULL, 0, 0, (const char*)name);
            if(!current)
            {
                printf("\r\n: dbug : object :  memory out(%s). ", __FUNCTION__);
                __unlock_objsys();
                return (NULL);
            }
        }
    }

    __unlock_objsys();
    return (current);
}

// ============================================================================
// 功能：释放对象构建的路径；
// 参数：start -- 路径开始的对象；
// 返回：释放了的对象数量；
// 备注：
// ============================================================================
s32 obj_releasepath(struct obj *start)
{
    s32 dels = 0;
    struct obj *parent, *current = start;

    if(__lock_objsys())
        return (0);

    while(1)
    {
        if((obj_istmp(current)) // 临时对象
           &&(!obj_isquoted(current)) // 未被引用
           &&(!current->rights.b.muses)) // 无人使用
        {
            parent = obj_parent(current);
            obj_del(current);
            __freeobj(current);
            current = parent;
            dels++;
        }
        else
        {
            break;
        }
    }

    __unlock_objsys();
    return (dels);
}

// ============================================================================
// 功能：用一个对象集合替代对象
// 参数：ob -- 对象（被替换）；
//      ops -- 对象集合的操作；
//      val -- 对象集合的值；
// 返回：成功（对象集合）；失败（NULL）；
// 备注：对象被新对象（集合点）代替后，就会从对象树上消失；正常的接口就不会查找到它；
// ============================================================================
struct obj *obj_replacebyset(struct obj *ob, fnObjOps ops, ptu32_t ObjPrivate)
{
    struct obj *setob, *baseob;
    u8 i = 0;

    setob = __newobj();
    if(!setob)
    {
        printf("\r\n: dbug : object : can not create for new set object.");
        return (NULL);
    }

    if(__lock_objsys())
        return (NULL);

    if(obj_testset(ob)) // 是否允许建立set
    {
        setob->child = NULL;
        setob->parent = ob->parent;
        setob->next = ob->next;
        setob->prev = ob->prev;
        setob->name = ob->name;

        ob->next->prev = setob;
        ob->prev->next = setob;
        if(ob->parent->child == ob)
            ob->parent->child = setob;

        setob->seton = ob;
        if(!ob->seton)
        {
            ob->seton = setob;
        }
        else
        {
            baseob = ob;
            while(baseob->seton!=ob)
            {
                baseob = baseob->seton;
                if(++i==0) // 防止数据被破坏，造成死循环；（原理：目前的逻辑不会在同一个节点上新建那么多次对象集合；）
                {
                    printf("\r\n: erro : object : can not create for new set object, data corrupted.");
                    __freeobj(setob);
                    __unlock_objsys();
                    return (NULL);
                }
            }

            baseob->seton = setob;
        }

        if(!ops)
            setob->ops = ops;
        else
            setob->ops = (fnObjOps)__objsys_default_ops;

        setob->ObjPrivate = ObjPrivate;
        setob->set = setob;
    }
    else
    {
        __freeobj(setob);
        setob = NULL;
    }

    __unlock_objsys();
    return (setob);
}

// ============================================================================
// 功能：释放对象集合；
// 参数：setob -- 对象集合；
// 返回：被重构（被替代）的对象；
// 备注：
// ============================================================================
struct obj *obj_destoryset(struct obj *setob)
{
    struct obj *ob = NULL;

    if(!obj_isset(setob))
        return (NULL); // 输入的参数不是对象集合点；

    if(__lock_objsys())
        return (NULL);

    if(setob->seton==setob) // 对象集合就是对象本身
    {
        if(dListIsEmpty(&setob->handles)&&(!setob->child))
            __freeobj(setob);
    }
    else // 对象集合构建与其他对象之上
    {
        ob = setob->seton;
        ob->parent = setob->parent;
        ob->next = setob->next;
        ob->prev = setob->prev;
        if(setob->parent->child == setob)
            setob->parent->child = ob;

        setob->prev->next = ob;
        setob->next->prev = ob;
        if(dListIsEmpty(&setob->handles))
            __freeobj(setob); // 原节点没有被引用到，就释放
    }

    __unlock_objsys();
    return (ob);
}

// ============================================================================
// 功能：获取对象系统的当前对象；
// 参数：
// 返回：对象系统的当前对象；
// 备注：
// ============================================================================
struct obj * objsys_current(void)
{
    return (__current_obj);
}

// ============================================================================
// 功能：设置对象系统的当前对象；
// 参数：ob -- 对象；
// 返回：无；
// 备注：
// ============================================================================
void objsys_setcurrent(struct obj *ob)
{
    __current_obj = ob;
}

// ============================================================================
// 功能：获取对象系统的根对象；
// 参数：无；
// 返回：对象系统的根对象；
// 备注：
// ============================================================================
struct obj *objsys_root(void)
{
    return (__root_obj);
}

// ============================================================================
// 功能：设置对象系统的根对象；
// 参数：ob -- 对象；
// 返回：无；
// 备注：
// ============================================================================
void objsys_setroot(struct obj *ob)
{
    __root_obj = ob;
}

// ============================================================================
// 功能：给对象新建一个prev关系对象；
// 参数：loc -- 对象；为空，则为当前设置对象；
//      ops -- 对象操作方法；（NULL，继承父方法；-1，使用缺省方法；）
//      rights -- 对象权限；为零，则继承；
//      represent -- 对象表示；
//      name -- 对象名字；为空，则使用系统默认名；
// 返回：成功（对象）；失败（NULL）；
// 备注：
// ============================================================================
struct obj *obj_newprev(struct obj *loc, fnObjOps ops, u32 rights,
                            ptu32_t represent, const char *name)
{
    struct obj *prev;
    char *cname;

    if(!loc)
        loc = __current_obj; // 如果未指定父节点，则指向当前工作节点；

    if(name)
    {
        if(__checkname(name))
          return (NULL);

        if(loc->parent)
        {
            if(obj_search_child(loc->parent, name))
              return (NULL); // child已经存在；
        }

        cname = malloc(strlen(name)+1);
        if(!cname)
          return (NULL);

        strcpy(cname, name);
    }
    else
    {
        cname = (char*)__uname_obj; // 系统默认名；
    }

    prev = __newobj();
    if(!prev)
        return (NULL);

    if(__lock_objsys())
    {
        __freeobj(prev);
        return (NULL);
    }

    prev->parent = loc->parent;
    prev->child = NULL;
    prev->ObjPrivate = represent;
    if(ops)
    {
        if(-1==(s32)ops)
            prev->ops = (fnObjOps)__objsys_default_ops;
        else
            prev->ops = ops;
    }
    else
    {
        if(prev->parent)
            prev->ops = prev->parent->ops;
        else
            prev->ops = (fnObjOps)__objsys_default_ops;
    }

    if((!rights)&&(loc->parent))
        prev->rights.a = loc->parent->rights.a & 0x7;
    else
        prev->rights.a = rights & 0x07;

    prev->name = cname;
    prev->seton = (struct obj*)-1; // 默认对象之上不允许建立对象集合；
    if(loc->parent)
        prev->set = loc->parent->set;
    else
        prev->set = NULL;

    dListInit(&prev->handles);
    __OBJ_LIST_INS_BEFORE(loc, prev);
    __unlock_objsys();
    return (prev);
}

// ============================================================================
// 功能：为对象新建一个next关系对象；
// 参数：loc -- 对象；为空，则为当前设置对象；
//      ops -- 对象操作方法；（NULL，继承父方法；-1，使用缺省方法；）
//      rights -- 对象权限；为零，则继承；
//      represent -- 对象表示；
//      name -- 对象名字。为空，则使用系统默认名；
// 返回：成功（对象）；失败（NULL）；
// 备注：
// ============================================================================
struct obj *obj_newnext(struct obj *loc, fnObjOps ops, u32 rights,
                        ptu32_t represent, const char *name)
{
    struct obj *next;
    char *cname;

    if(!loc)
        loc = __current_obj; // 如果未指定父节点，则指向当前工作节点；

    if(name)
    {
        if(__checkname(name))
            return (NULL);

        if(loc->parent)
        {
            if(NULL != obj_search_child(loc->parent, name))
                return (NULL); // child已经存在；
        }

        cname = malloc(strlen(name)+1);
        if(!cname)
            return (NULL);

        strcpy(cname, name);
    }
    else
    {
        cname = (char*)__uname_obj; // 系统默认名；
    }

    next = __newobj();
    if(!next)
        return (NULL);

    if(__lock_objsys())
    {
        __freeobj(next);
        return(NULL);
    }

    next->parent = loc->parent;
    next->child = NULL;
    next->ObjPrivate = represent;
    if(ops)
    {
        if(-1==(s32)ops)
            next->ops = (fnObjOps)__objsys_default_ops;
        else
            next->ops = ops;
    }
    else
    {
        if(next->parent)
            next->ops = next->parent->ops;
        else
            next->ops = (fnObjOps)__objsys_default_ops;
    }

    if((!rights)&&(loc->parent))
        next->rights.a = loc->parent->rights.a & 0x7;
    else
        next->rights.a = rights & 0x7;

    next->name = (char *)name;
    next->seton = (struct obj*)-1; // 默认对象之上不允许建立对象集合；
    dListInit(&next->handles);
    if(loc->parent)
        next->set = loc->parent->set;
    else
        next->set = NULL;

    __OBJ_LIST_INS_AFTER(loc, next);
    __unlock_objsys();
    return (next);

}

// ============================================================================
// 功能：新建子对象；
// 参数：parent -- 父对象；如果未设置，默认为当前设置对象；
//      ops -- 对象方法;（NULL，继承父方法；-1，使用缺省方法；）
//      rights -- 对象权限
//      represent -- 对象内容（表示）；
//      name -- 对象名；不可为系统默认名；（系统默认名用于未设置对象名的逻辑）
// 返回：成功（新建子对象）；失败（NULL）；
// 备注：新建的子对象，放置在子对象链的末尾；
// ============================================================================
struct obj *obj_newchild(struct obj *parent, fnObjOps ops, u32 rights,
                            ptu32_t ObjPrivate, const char *name)
{
    struct obj *child;
    char *cname;

    if(!parent)
        parent = __current_obj; // 如果未指定父节点，则指向当前工作节点；

    if(name)
    {
        if(__checkname(name))
            return (NULL);

        if(NULL != obj_search_child(parent, name))
            return (NULL); // child已经存在；

        cname = malloc(strlen(name)+1);
        if(!cname)
            return (NULL);

        strcpy(cname, name);
    }
    else
    {
        cname = (char*)__uname_obj; // 系统默认名；
    }

    child = __newobj();
    if(!child)
        return (NULL);

    if(__lock_objsys())
    {
        __freeobj(child);
        return (NULL);
    }

    child->parent = parent;
    child->child = NULL;
    if(ObjPrivate)
        child->ObjPrivate = ObjPrivate;
    else
        child->ObjPrivate = parent->ObjPrivate;

    if(ops)
    {
        if(-1==(s32)ops)
            child->ops = (fnObjOps)__objsys_default_ops;
        else
            child->ops = ops;
    }
    else
    {
        child->ops = parent->ops;
    }

    if(rights)
        child->rights.a = rights & 0x7;
    else
        child->rights.a = parent->rights.a & 0x7;

    child->name = cname;
    child->seton = (struct obj*)-1; // 默认对象之上不允许建立对象集合；
    child->set = parent->set; // 集合逻辑必须继承父；
    dListInit(&child->handles);
    if(!parent->child)
    {
        parent->child = child;
        __OBJ_LIST_INIT(child);
    }
    else
    {
        __OBJ_LIST_INS_BEFORE(parent->child, child);
    }

    __unlock_objsys();
    return (child);
}
// ============================================================================
// 功能：新建一个对象，其处于loc对象所处队列的首；
// 参数： ops -- 对象方法；（NULL，继承父方法；-1，使用缺省方法；）
//      rights -- 对象权限
//      represent -- 对象内容（表示）；
//      name -- 对象名；不可为系统默认名；（系统默认名用于未设置对象名的逻辑）
// 返回：成功（新建子对象）；失败（NULL）；
// 备注：
// ============================================================================
struct obj *obj_newhead(struct obj *loc, fnObjOps ops, u32 rights,
                           ptu32_t ObjPrivate, const char *name)
{
    struct obj *head;
    char *cname;

    if(!loc)
        loc = __current_obj; // 如果未指定父节点，则指向当前工作节点；

    if(name)
    {
        if(__checkname(name))
            return (NULL);

        if(loc->parent)
        {
            if(NULL != obj_search_child(loc->parent, name))
                return (NULL); // child已经存在；
        }

        cname = malloc(strlen(name)+1);
        if(!cname)
            return (NULL);

        strcpy(cname, name);
    }
    else
    {
        cname = (char*)__uname_obj; // 系统默认名；
    }

    head = __newobj();
    if(!head)
        return (NULL);

    if(__lock_objsys())
    {
        __freeobj(head);
        return (NULL);
    }

    head->parent = loc->parent;
    head->child = NULL;
    head->ObjPrivate = ObjPrivate;
    if(ops)
    {
        if(-1==(s32)ops)
            head->ops = (fnObjOps)__objsys_default_ops;
        else
            head->ops = ops;
    }
    else
    {
        if(head->parent)
            head->ops = head->parent->ops;
        else
            head->ops = (fnObjOps)__objsys_default_ops;
    }

    if((!rights)&&(loc->parent))
        head->rights.a = loc->parent->rights.a & 0x7;
    else
        head->rights.a = rights & 0x7;

    head->name = (char *)name;
    head->seton = (struct obj*)-1;
    if(loc->parent)
        head->set = loc->parent->set;
    else
        head->set = NULL;

    if(loc->parent)
    {
        if(loc->parent->child)
        {
            __OBJ_LIST_INS_BEFORE(loc->parent->child, head);
        }
        else
        {
            loc->parent->child = head;
            __OBJ_LIST_INIT(head);
        }
    }
    else
    {
        __OBJ_LIST_INS_BEFORE(loc, head); // 不存在父对象，则就将新对象放到当前对象前
    }

    __unlock_objsys();
    return (head);
}

// ============================================================================
// 功能：将队列child插入到对象loc关系，成为其子关系；
// 参数：loc -- 被插入对象；
//      child -- 将插入的对象；
// 返回：成功（被插入的对象）；失败（NULL）；
// 备注：
// ============================================================================
struct obj *obj_insert2child(struct obj *loc, struct obj *child)
{
    if((loc==NULL)||(child==NULL))
        return (NULL);

    if(__lock_objsys())
        return (NULL);

    obj_detach(child); // 就对象从原关系树中分离；
    child->parent = loc;
    if(loc->child==NULL)
    {
        loc->child = child;
        __OBJ_LIST_INIT(child);
    }
    else
    {
        __OBJ_LIST_INS_BEFORE(loc->child, child);
        loc->child = child;
    }

    __unlock_objsys();
    return (child);
}

#if 0
// ============================================================================
// 功能：移动一个对象树枝到别的节点下面成为其子树；
// 参数：pParent -- 新父节点；
//      ob，待插入的子节点，可能有下级节点；
// 返回：成功（0），失败（-1）；
// 备注：
// ============================================================================
s32 OBJ_MoveToBranch(struct obj *pParent,struct  obj *ob)
{
    if((pParent==NULL)||(ob==NULL))
        return (-1);

    low_atom = Int_LowAtomStart();
    obj_detach(ob);
    ob->parent = pParent;
    if(pParent->child == NULL)
    {
        pParent->child = ob;
        __OBJ_LIST_INIT(ob);
    }
    else
    {
        __OBJ_LIST_INS_BEFORE(pParent->child, ob);
        pParent->child = ob;
    }
    Int_LowAtomEnd(low_atom);
    return (0);
}
#endif

// ============================================================================
// 功能：将对象移动到队列尾；
// 参数：ob -- 对象；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 obj_move2last(struct obj *ob)
{
    struct obj *head;

    if((!ob)||(!ob->parent))
        return (-1);

    if(__lock_objsys())
        return (-1);

    head = ob->parent->child;
    if(head==ob) // 本对象时队列头
    {
        ob->parent->child = ob->next;
    }
    else if(head->prev!=ob) // 本对象也不是队列尾
    {
        __OBJ_LIST_REMOVE(ob);
        __OBJ_LIST_INS_BEFORE(head, ob);
    }

    __unlock_objsys();
    return (0);
}

// ============================================================================
// 功能：将对象移动到队列头；
// 参数：ob -- 对象；
// 返回：成功（0），失败（-1）；
// 备注：
// ============================================================================
s32 obj_move2head(struct obj *ob)
{
    struct obj *head;

    if((!ob)||(!ob->parent))
        return (-1);

    if(__lock_objsys())
        return (-1);

    head = ob->parent->child;
    if(head->prev==ob)
    {
        ob->parent->child = ob->prev;
    }
    else if(head!=ob)    //不是头节点
    {
        __OBJ_LIST_REMOVE(ob);
        __OBJ_LIST_INS_BEFORE(head, ob);
        ob->parent->child = ob;
    }

    __unlock_objsys();
    return (0);
}

// ============================================================================
// 功能：将对象（next）插入到对象(loc)的后（next）位置；
// 参数：loc -- 目标对象；
//      next -- 目标节点,它可能原本就在Object队列中；
// 返回：成功（0），失败（-1）；
// 备注：
// ============================================================================
s32 obj_insert2next(struct obj *loc, struct obj *next)
{

    if((loc==NULL)||(next==NULL)||(loc==next))
        return (-1);

    if(__lock_objsys())
        return (-1);

    if(loc->next!=next) // 看是否已经符合要求
    {
        // 先把目标节点从队列中取出
        if(next->next == next) // 说明该节点没有兄弟节点.
        {
            next->parent->child = NULL;
        }
        else
        {
            if(next->parent->child == next)
            {   //说明该节点是队列头节点,需要改变队列头节点
                next->parent->child = next->next;
            }
            __OBJ_LIST_REMOVE(next);
        }

        __OBJ_LIST_INS_AFTER(loc, next);
        next->parent = loc->parent;
    }

    __unlock_objsys();
    return (0);
}

// ============================================================================
// 功能：将对象（prev）插入到对象(loc)的前（previous）位置；
// 参数：loc -- 被插入的对象；
//      next -- 需插入的对象；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 obj_insert2prev(struct obj *loc, struct obj *prev)
{
    if((loc==NULL)||(prev==NULL)||(loc==prev))
        return (-1);

    if(__lock_objsys())
        return (-1);

    if(loc->prev!=prev) // 看是否已经符合要求
    {
        //先把目标节点从队列中取出
        if(prev->next == prev)   //说明该节点没有兄弟节点.
        {
            prev->parent->child = NULL;
        }
        else
        {
            if(prev->parent->child == prev)
            {   //说明该节点是队列头节点,需要改变队列头节点
                prev->parent->child = prev->next;
            }
            __OBJ_LIST_REMOVE(prev);
        }

        __OBJ_LIST_INS_BEFORE(loc, prev);
        prev->parent = loc->parent;
    }

    __unlock_objsys();
    return (0);
}

// ============================================================================
// 功能: 父对象的子对象的相对位置不变，将子对象链表的队列头朝前（previous）方向移动一格。
// 参数：parent -- 父对象；
// 返回：成功（0），失败（-1）；
// 备注：
// ============================================================================
s32 obj_child_move2prev(struct obj *parent)
{
    if(!parent)
        return (-1);

    if(__lock_objsys())
        return (-1);

    if(parent->child)
        parent->child = parent->child->prev;

    __unlock_objsys();
    return (0);
}

// ============================================================================
// 功能: 父对象的子对象的相对位置不变，将子对象链表的队列头朝后（next）方向移动一格。
// 参数：parent -- 父对象；
// 返回：成功（0），失败（-1）；
// 备注：
// ============================================================================
s32 obj_child_move2next(struct obj *parent)
{
    if(!parent)
        return (-1);

    if(__lock_objsys())
        return (-1);

    if(parent->child)
        parent->child = parent->child->next;

    __unlock_objsys();
    return (0);
}

// ============================================================================
//功能：返回某树枝的一个末梢节点,末梢节点是指没有子节点的节点.
//参数：ob,需要搜索的树枝的父节点
//返回：树枝的一个末梢节点.当没有子节点时，返回NULL
//备注：当需要删除整个树枝时,本函数很有用,结合rsc_del_node函数,反复调用本函数,
//     并把返回的节点删除,直到本函数返回NULL.需要删除一个文件夹或者删除一个存
//     在子窗口的gui窗口时,就需要用到删除整个树枝的操作.
//     TODO: 旧的逻辑，在概念和逻辑上都不完整；不建议使用；
// ============================================================================
struct obj *obj_twig(struct obj *ob)
{
    struct obj *result = NULL, *current;

    if(!ob)
        return (NULL);

    if(__lock_objsys())
       return (NULL);

    current = ob;
    while(current->child != NULL)
    {
        current = current->child;
    }

    if(current == ob)
        result = NULL;
    else
        result = current;

    __unlock_objsys();
    return (result);
}

#if 0
//----取节点的路径-------------------------------------------------------------
//功能: 查看某节点是根节点的第几级子节点，
//参数: ob,待提取路径的节点
//      path，保存路径的缓冲区
//      limit，缓冲区长度
//返回: path地址，出错则返回NULL
//-----------------------------------------------------------------------------
char *obj_getpath(struct obj *ob, char *path, u32 limit)
{
    struct obj *Current,*root;
    char *pos;
    u32 count = 0;
    if((ob == NULL)||(path == NULL) || (limit < 2))
        return (NULL);
    Current = ob;
    root = __root_obj;
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
// 功能：遍历对象（parent）的所有子对象链表，每调用一次，返回一个子对象(current)，直至完成；
// 参数：parent -- 对象；
//      child -- 当前子对象。
// 返回：成功（子对象）；失败（NULL）
// 备注：data必须先初始化；
//      如果初始化为父对象，则遍历全部队列成员；
//      如果data初始化为是某个子对象链表成员，则只遍历其到队列结束，而不是全部队列成员；
// ============================================================================
struct obj *obj_foreach_child(struct obj *parent, struct obj *child)
{
    if((parent==NULL)||(child==NULL))
        return (NULL);

    if(__lock_objsys())
       return (NULL);

    if((child==parent)||(child->parent!=parent))
    {
        child = parent->child; // data不是某个子对象，则从首子对象开始；
    }
    else
    {
        child = child->next;
        if(child == parent->child)
            child = NULL;
    }

    __unlock_objsys();
    return (child);
}

// ============================================================================
// 功能：遍历对象的所有子对象及其拓展对象（对象的子对象关系上延展的所有对象）；
// 参数：ancester -- 对象；
//      scion -- 当前遍历到的对象,
// 返回：当前搜索位置的下一个节点指针,如果已经搜索完成,则返回NULL.
// 备注：当需要对对象树中某一个树枝或者整个链表中的节点逐一进行某种操作时,可
//      反复调用本函数,第一次调用current_node = parent_node,其后current_node
//      = 上次返回值，直到返回空.
//      本函数按父、子、孙、曾孙....的顺序搜索，先搜直系，再搜旁系，确保所有子孙
//      节点都能够访问到，如果对访问顺序有特殊要求，不能使用本函数；
// ============================================================================
struct obj *obj_foreach_scion(struct obj *ancester, struct obj *scion)
{
    struct  obj *result = NULL, *current_copy;
    bool_t up = FALSE;

    if((ancester==NULL)||(scion==NULL))
        return (NULL);

    if(__lock_objsys())
        return (NULL);

    if((scion != ancester)||ancester->child)
    {
        current_copy = scion;
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
        }while(current_copy != ancester);
    }

    __unlock_objsys();
    return (result);
}

// ============================================================================
// 功能：查找兄弟（同级链表上的）对象；
// 参数：ob -- 对象;.
//      name -- 需要搜索的对象名;
// 返回：成功（名为name的对象）；失败（NULL）；
// 备注：
// ============================================================================
struct obj *obj_search_sibling(struct obj *ob, const char *name)
{
    struct obj *tmp, *result = NULL;

    if((ob==NULL)||(name==NULL))
        return (NULL);

    if(__lock_objsys())
        return (NULL);

    tmp = ob;
    do
    {
        if(strcmp(tmp->name, name) == 0)
        {
            result = tmp;
            break;
        }
        tmp = tmp->next;
    }while (tmp != ob);

    __unlock_objsys();
    return (result);
}

// ============================================================================
// 功能：查找子对象；
// 参数：parent -- 对象；
//      name -- 需要搜索的对象名
// 返回：成功（名为name的对象）；失败（NULL）；
// 备注：
// ============================================================================
struct obj *obj_search_child(struct obj *parent, const char *name)
{
    struct obj *tmp, *result = NULL;

    if((name == NULL) || (parent == NULL))
        return (NULL);

    if(__lock_objsys())
        return (NULL);

    tmp = parent->child;
    if(tmp == NULL)
    {
        result = NULL;
    }
    else
    {
        do
        {
            if(strcmp(tmp->name, name)==0)
            {
                result = tmp;
                break;
            }
            tmp = tmp->next;
        }while (tmp != parent->child);
    }

    __unlock_objsys();
    return (result);
}

// ============================================================================
// 功能：查找对象的子关系及其拓展关系下的对象；
// 参数：ancester -- 对象；
//      name -- 需查找的对象名；
// 返回：成功（名为name的对象）；失败（NULL）；
// 备注：
// ============================================================================
struct obj *obj_search_scion(struct obj *ancester, const char *name)
{
    struct obj *current, *tmp, *result = NULL;

    if((ancester == NULL)||(name == NULL))
        return (NULL);

    if(__lock_objsys())
        return (NULL);

    current = ancester;
    while((tmp = obj_foreach_scion(ancester, current)) != NULL)
    {
        if(strcmp(tmp->name, name) == 0)
        {
            result = tmp;
            break;
        }
        current = tmp;
    }

    __unlock_objsys();
    return (result);
}

// ============================================================================
// 功能：从对象（start）开始依据路径查找对象；
// 参数：start -- 路径的起始对象；
//      path -- 检索的对象路径；
// 返回：成功（路径结束的对象）；失败（NULL）；
// 备注：
// ============================================================================
struct obj *obj_search_path(struct obj *start, const char *path)
{
#if 0
    const char *path_name;
    char *ObjName;
    bool_t end = FALSE;
    u32 i;
    struct obj *current, *result = NULL;

    if((path == NULL) || (start == NULL))
        return (NULL);

    if(__lock_objsys())
        return (NULL);


    current = start->child;
    if(current == NULL)
    {
        __unlock_objsys();
        return (NULL);
    }

    path_name = path;
    while(1)
    {
        result = current;
        ObjName = current->name;
        for(i=0;i<CN_OBJ_NAME_LIMIT;i++) // 对象名长度限制
        {
            if(ObjName[i] == path_name[i])
            {
                if(path_name[i] == '\0') // 匹配，且path结束
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
                else // 匹配，但path未结束,继续查找子节点
                {
                    path_name += i+1;
                    current = current->child;
                    if(current == NULL) // 没有子节点了，但path未结束
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

    __unlock_objsys();
    return result;
#else
    char *segment;
    struct obj *current = start, *head;
    u32 i;

    if((!current)||(!path))
        return (NULL);

    if(__lock_objsys())
        return (NULL);

    while(current)
    {
__SEARCH_NEXT:
        while('/' == *path)
            path++; // 过滤多余的'/'

        if('\0' == *path)
            break; // 遍历路径结束

        if(('.' == path[0]) && ('.' == path[1])
           && (('\\' == path[3]) || ('/' == path[3]) || ('\0' == path[3])))
        {
            current = obj_parent(current);
            path += 2; // ".."字符，表示上一级
            continue;
        }
        else if(('.' == path[0])
                && (('\\' == path[1]) || ('/' == path[1]) || ('\0' == path[1])))
        {
            path += 1; // "."字符，表示当前级
            continue;
        }

        head = current->child; // 遍历子节点
        if(!head)
            break;

        segment = (char*)obj_name(head);
        for(i=0; i<CN_OBJ_NAME_LIMIT; i++) // 对象名长度限制
        {
            if('\0' == segment[i])
            {
                if('\0' == path[i])
                {
                    path += i; // path为'\0'
                    current = head;
                    goto __SEARCH_DONE; // 匹配结束，整个遍历结束
                }
                else if(('\\' == path[i]) || ('/' == path[i]))
                {
                    path += i;
                    current = head;
                    goto __SEARCH_NEXT; // 匹配当前节点，继续匹配一下一个节点
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
            segment = (char*)obj_name(current);
            for(i=0; i<CN_OBJ_NAME_LIMIT; i++) // 对象名长度限制
            {
                if('\0' == segment[i])
                {
                    if('\0' == path[i])
                    {
                        path += i; // path为'\0'
                        goto __SEARCH_DONE; // 匹配结束，整个遍历结束
                    }
                    else if(('\\' == path[i]) || ('/' == path[i]))
                    {
                        path += i;
                        goto __SEARCH_NEXT; // 匹配当前节点，继续匹配一下一个节点
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

        current = obj_parent(current); // 当前节点与路径不匹配，退回上上以及节点
        break; // 未找到节点
    }

__SEARCH_DONE:
   __unlock_objsys();
   return (current);
#endif
}

// ============================================================================
// 功能：在对象parent之下新建子对象集合
// 参数：parent -- 对象；
//      name -- 对象集合点名 ；
//      ops -- 对象集合点的操作方式（NULL，继承父方法；-1，使用缺省方法；）；
//      represent -- 对象集合点的值；
//      rights -- 对象集合点的权限；
// 返回：成功（新建的子对象集合）；失败（NULL）；
// 备注：
// ============================================================================
struct obj *obj_newchild_set(struct obj *parent, const char *name, fnObjOps ops,
                             ptu32_t ObjPrivate, u32 rights)
{
    struct obj *child_set;
    char *cname;

    if(!parent)
        parent = __current_obj; // 如果未指定父节点，则指向当前工作节点；

    if(name)
    {
        if(__checkname(name))
            return (NULL);

        if(NULL != obj_search_child(parent, name))
            return (NULL); // child已经存在；

        cname = malloc(strlen(name)+1);
        if(!cname)
            return (NULL);

        strcpy(cname, name);
    }
    else
    {
        cname = (char*)__uname_obj; // 系统默认名；
    }

    child_set = __newobj();
    if(!child_set)
        return (NULL);

    if(__lock_objsys())
    {
        __freeobj(child_set);
        return (NULL);
    }

    child_set->parent = parent;
    child_set->child = NULL;
    child_set->ObjPrivate = ObjPrivate;
    if(ops)
    {
        if(-1==(s32)ops)
            child_set->ops = (fnObjOps)__objsys_default_ops;
        else
            child_set->ops = ops;
    }
    else
    {
        child_set->ops = parent->ops;
    }

    child_set->rights.a = rights & 0x7;
    child_set->name = cname;
    child_set->seton = child_set; // 默认对象之上不允许建立对象集合；
    child_set->set = child_set; // 集合逻辑必须继承父；
    dListInit(&(child_set->handles));
    if(!parent->child)
    {
        parent->child = child_set;
        __OBJ_LIST_INIT(child_set);
    }
    else
    {
        __OBJ_LIST_INS_BEFORE(parent->child, child_set);
    }

    __unlock_objsys();
    return (child_set);
}

// ============================================================================
// 功能：设置对象的所有句柄的多路复用触发事件；
// 参数：ob -- 被操作的文件对象；
//      events -- 多路复用触发事件；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 obj_set_handles_multievent(struct obj *ob, u32 events)
{
    extern s32 handle_multievent_setall(list_t *all, u32 events);

    if(!ob)
        return (-1);

    return (handle_multievent_setall(&ob->handles, events));
}

// ============================================================================
// 功能：清除对象的所有句柄的多路复用触发事件；
// 参数：ob -- 被操作的文件对象；
//      events -- 多路复用触发事件；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 obj_unset_handles_multievent(struct obj *ob, u32 events)
{
    extern s32 handle_multievent_unsetall(list_t *all, u32 events);

    if(!ob) // 对象未被引用；
        return (-1);

    return (handle_multievent_unsetall(&ob->handles, events));
}
