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
#include <dirent.h>
#include <objhandle.h>
#include <shell.h>
#include "component_config_objfile.h"

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

static inline s32 __OBJ_CheckName(const char *name);
static inline struct Object *__OBJ_NewObj(void);
static inline void __OBJ_FreeObj(struct Object *ob);

static struct MemCellPool s_tObjectPool;
static struct Object s_tObjectInitPool[CFG_OBJECT_LIMIT];
static struct Object *s_ptRootObject, *s_ptCurrentObject; // 逻辑上，应该是每一个进程一个；
static struct MutexLCB s_tObjectMutex;
static const char *__uname_obj = "un_named";
inline static s32 __OBJ_DefaultOps(void *opsTarget, u32 cmd, ptu32_t OpsArgs1,
                                ptu32_t OpsArgs2, ptu32_t OpsArgs3);
static struct objhandle *__OBJ_Open(struct Object *ob, u32 flags, char *uncached);
static s32 __OBJ_ReadDentry(struct objhandle *directory, struct dirent *dentry);
static s32 __OBJ_Close(struct objhandle *hdl);

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
// 返回：固定返回 CN_OBJ_CMD_UNSUPPORT
// 备注：
// ============================================================================
inline static s32 __OBJ_DefaultOps(void *opsTarget, u32 cmd, ptu32_t OpsArgs1,
                                ptu32_t OpsArgs2, ptu32_t OpsArgs3)
{
    s32 result = CN_OBJ_CMD_EXECUTED;
    switch(cmd)
    {
        case CN_OBJ_CMD_OPEN:
        {
            struct objhandle *hdl = NULL;
            if(OpsArgs3 != 0)
                hdl = __OBJ_Open((struct Object *)opsTarget,
                                (u32)(*(u64*)OpsArgs2), (char*)OpsArgs3);
            *(struct objhandle **)OpsArgs1 = hdl;
            break;
        }

        case CN_OBJ_CMD_READDIR:
        {
            struct objhandle *hdl = (struct objhandle*)OpsArgs3;
            struct dirent *ret = (struct dirent *)OpsArgs1;

            if((ptu32_t)__OBJ_ReadDentry(hdl, ret) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }

        case CN_OBJ_CMD_CLOSE:
        {
            if(__OBJ_Close((struct objhandle*)opsTarget) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }
//      ftp是通过能否获取到该obj的状态来决定该obj是否要通过ftp显示出来，所以这里把获取状态的功能注释了，不然所有节点都会显示出来


        default:
        {
            result = CN_OBJ_CMD_UNSUPPORT;
            break;
        }
    }

    return (result);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
// ============================================================================
// 功能：默认的打开对象函数，根对象使用，如果创建对象时，参数ops == -1，亦使用此函数。
// 参数：ob -- 根目录对象(可能不是需要打开的文件)；
//      flags -- 文件标记；
//      uncached -- 未匹配的部分；
// 返回：成功申请（新对象）；失败（NULL）；
// 备注：
// ============================================================================
static struct objhandle *__OBJ_Open(struct Object *ob, u32 flags, char *uncached)
{
    struct objhandle *hdl;

    hdl = Handle_New();
    if(!hdl)
    {
        printf("\r\n : erro : efs    : open failed(memory out).");
        return (NULL);
    }
    Handle_Init(hdl, ob, flags, (ptu32_t)NULL);     //将obj和hdl关联起来
    return (hdl);
}
#pragma GCC diagnostic pop

// ============================================================================
// 功能：默认的读对象函数，根对象使用，如果创建对象时，参数ops == -1，亦使用此函数。
// 参数：directory -- 根目录的对象句柄；；
//      dentry -- 目录项；
// 返回：全部读完（1）；读了一项（0）；
// 备注：
// ============================================================================
static s32 __OBJ_ReadDentry(struct objhandle *directory, struct dirent *dentry)
{
    struct Object *ob = (struct Object *)dentry->d_ino;
    if(!ob) // 第一次读；
    {
        ob = OBJ_GetChild(Handle_GetHostObj(directory));
        if(!ob)
            return (1); // 没有子项目；
    }
    else // 后续读；
    {
        ob = OBJ_GetNext(ob);
        if(ob==OBJ_GetChild(Handle_GetHostObj(directory)))
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
// 功能：默认的关闭对象函数，根对象使用，如果创建对象时，参数ops == -1，亦使用此函数。
// 参数：hdl -- 根目录的对象句柄；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 __OBJ_Close(struct objhandle *hdl)
{
    return Handle_Delete(hdl);
}

// ============================================================================
// 功能：锁定对象系统；
// 参数：
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
bool_t OBJ_Lock(void)
{
    return Lock_MutexPend(&s_tObjectMutex, CN_TIMEOUT_FOREVER);

}

// ============================================================================
// 功能：解锁对象系统；
// 参数：
// 返回：
// 备注：
// ============================================================================
void OBJ_Unlock(void)
{
    Lock_MutexPost(&s_tObjectMutex);
}

// ============================================================================
// 功能：申请新对象
// 参数：无；
// 返回：成功申请（新对象）；失败（NULL）；
// 备注：
// ============================================================================
static inline struct Object *__OBJ_NewObj(void)
{
    struct Object *ob;

    ob = (struct Object*)Mb_Malloc(&s_tObjectPool, 0);
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
static inline void __OBJ_FreeObj(struct Object *ob)
{
    Mb_Free(&s_tObjectPool, ob);
    __objsys_statics.fress++;
    __objsys_statics.alives--;
}

// ============================================================================
// 功能：检查对象名的合法性；
// 参数：name -- 对象名；
// 返回：合法（0）；非法（-1）；
// 备注：
// ============================================================================
static inline s32 __OBJ_CheckName(const char *name)
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
// 功能：获取对象的私有成员值；
// 参数：ob -- 对象；
// 返回：对象的值；
// 备注：
// ============================================================================
ptu32_t OBJ_GetPrivate(struct Object *ob)
{
    if(ob)
        return (ob->ObjPrivate);

    return (0);
}

// ============================================================================
// 功能：设置对象的私有成员值；
// 参数：ob -- 对象；
//      represent -- 对象的值；
// 返回：无；
// 备注：
// ============================================================================
void OBJ_SetPrivate(struct Object *ob, ptu32_t Private)
{
    if(ob)
        ob->ObjPrivate = Private;
}

// ============================================================================
// 功能：获取对象操作；
// 参数：ob -- 对象；
// 返回：成功（对象操作）；失败（NULL）；
// 备注：
// ============================================================================
fnObjOps OBJ_GetOps(struct Object *ob)
{
    if(!ob)
        return (NULL);

    return (ob->ObjOps);
}

// ============================================================================
// 功能：设置对象操作；
// 参数：ob -- 对象；
//      ObjOps -- 对象操作；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 OBJ_SetOps(struct Object *ob, fnObjOps ObjOps)
{
    if(!ob)
        return (-1);

    ob->ObjOps = ObjOps;
    return (0);
}

// ============================================================================
// 功能：设置对象为临时对象，临时对象在close时，只要引用数是0，即被删除。注意，如果子孙
//      对象中存在非临时对象，则不能设置为临时对象。
// 参数：ob -- 对象；
// 返回：成功（0）；失败（-1）；
// 备注：次函数没有逆操作，临时对象属性不能清除
// ============================================================================
s32 OBJ_SetToTemp(struct Object *ob)
{
    struct Object *current;
    bool_t temp = true;
    if(ob != NULL)
    {
        current = ob;
        current = OBJ_ForeachScion(ob,current);
        while(current)
        {
            if(!current->BitFlag.temporary)
            {
                temp = false;
                break;
            }
            else
                current = OBJ_ForeachScion(ob,current);
        }
    }
    if(temp == true)
    {
        ob->BitFlag.temporary = 1;
        return 0;
    }
    else
        return -1;
}

// ============================================================================
// 功能：获取对象名；
// 参数：ob -- 对象；
// 返回：成功（对象名）；失败（NULL）；
// 注意：
// ============================================================================
char *OBJ_GetName(struct Object *ob)
{
    if(!ob)
        return (NULL);

    return (ob->name);
}

// ============================================================================
// 功能: 获取对象的父关系对象；
// 参数: ob -- 对象；
// 返回: 成功（父节点），失败（NULL）；
// 备注：
// ============================================================================
struct Object *OBJ_GetParent(struct Object *ob)
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
struct Object *OBJ_GetChild(struct Object *ob)
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
struct Object *OBJ_GetPrev(struct Object *ob)
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
struct Object *OBJ_GetNext(struct Object *ob)
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
struct Object *OBJ_GetHead(struct Object *ob)
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
s32 OBJ_GetLevel(struct Object *ob)
{
    u32 level = 0;
    struct Object *pl_node = ob;

    if(pl_node==NULL)
        return (-1);

    OBJ_Lock();

    while(pl_node->parent!=s_ptRootObject)
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
            OBJ_Unlock();
            return (-1);
        }
    }

    OBJ_Unlock();
    return (level);
}

// ============================================================================
// 功能：获取对象在同级链表中的位置；
// 参数：ob -- 对象；
// 返回：对象的位置；出错（-1）；
// 备注：链表头为0；链表的第二个成员为1；......
// ============================================================================
u32 OBJ_GetOrder(struct Object *ob)
{
    u32 order;
    struct Object *tmp;

    if(!ob)
        return (-1);

    if(ob==s_ptRootObject)
        return (0);

    OBJ_Lock();

    order = 0;
    tmp = ob->parent->child;
    while(tmp!=ob)
    {
        order +=1;
        tmp = tmp->next;
    }

    OBJ_Unlock();
    return (order);
}

//----遍历文件句柄-------------------------------------------------------------
//功能：遍历一个对象下创建的全部文件，典型用法：
//      struct objhandle *Current = NULL;
//      while(1)
//      {
//          Current = OBJ_TraveFile(Current, s_ptSocketObject);
//          if(Current != NULL)
//              do something;
//          else
//              break;
//      }
//参数：Current，当前遍历位置，初始为NULL。
//返回：下一个文件指针，遍历完成则返回NULL
//-----------------------------------------------------------------------------
struct objhandle* OBJ_ForeachHandle(struct objhandle *Current, struct Object *Object)
{
    struct objhandle *result;
    struct dListNode *List;
    if(Object == NULL)
        return NULL;
    if(Current == NULL)
        List = Object->handles.next;
    else
        List = Current->list.next;
    if(List == &Object->handles)
        result = NULL;
    else
        result = dListEntry(List, struct objhandle, list);
    return result;
}

// ============================================================================
// 功能：将对象句柄与对象关联；
// 参数：hdl -- 对象句柄；
//      ob -- 对象；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 OBJ_LinkHandle(struct objhandle *hdl, struct Object *ob)
{
    if((!ob)||(!hdl))
        return (-1);

    hdl->HostObj = ob;
    dListInsertBefore(&ob->handles,&hdl->list);
    return (0);
}

// ============================================================================
// 功能：初始化对象系统；
// 参数：无
// 返回：成功（0
// 备注：
// ============================================================================
s32 OBJ_ModuleInit(void)
{
    Lock_MutexCreate_s(&s_tObjectMutex, "obj sys");

    Mb_CreatePool_s(&s_tObjectPool, s_tObjectInitPool,
            CFG_OBJECT_LIMIT, sizeof(struct Object), 16, 16384, "object");
    s_ptRootObject = __OBJ_NewObj();

    __OBJ_LIST_INIT(s_ptRootObject);
    s_ptRootObject->name = "";
//  s_ptRootObject->rights = S_IRWXUGO;       //根的默认权限是拥有所有权限
    s_ptRootObject->parent = NULL;
    s_ptRootObject->ObjOps = (fnObjOps)__OBJ_DefaultOps;
    s_ptRootObject->BitFlag.temporary = 0;
    s_ptRootObject->BitFlag.inuse = 1;  //被设为当前目录，故初始化为1
    s_ptCurrentObject = s_ptRootObject;
    return (0);
}

// ============================================================================
// 功能：对象是否是同级链表的头成员；
// 参数：ob -- 对象；
// 返回：是（1）；否（0）；
// 备注：
// ============================================================================
s32 OBJ_IsHead(struct Object *ob)
{
    if(!ob)
        return (0);

    if((ob==ob->parent->child)||(ob==s_ptRootObject))
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
s32 OBJ_IsLast(struct Object *ob)
{
    if(!ob)
        return (0);

    if((ob->next==ob->parent->child)||(ob==s_ptRootObject))
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
s32 OBJ_IsOnDuty(struct Object *ob)
{
    if(!ob)
        return (0);

    if((ob->BitFlag.inuse) || (ob->child))
        return (1);
    else
        return (0);
}

//
//// ============================================================================
//// 功能：对象上锁，禁止新访问；
//// 参数：ob -- 对象；
//// 返回：成功（0）；失败（-1）；
//// 备注：
//// ============================================================================
//s32 OBJ_Lock(struct Object *ob)
//{
//    if(OBJ_Lock())
//        return (-1);
//
//    if(ob->rights.lock) // 已经上锁
//    {
//        OBJ_Unlock();
//        return (-1);
//    }
//
//    ob->rights.lock = 1;
//    OBJ_Unlock();
//    return (0);
//}
//
//// ============================================================================
//// 功能：对象解锁；
//// 参数：ob -- 对象；
//// 返回：成功（0）；失败（-1）；
//// 备注：INLINE
//// ============================================================================
//s32 OBJ_Unlock(struct Object *ob)
//{
//    if(OBJ_Lock())
//        return (-1);
//
//    ob->rights.lock = 0;
//    OBJ_Unlock();
//    return (0);
//}

//// ============================================================================
//// 功能：对象增引用计数；
//// 参数：ob -- 对象；
//// 返回：成功（0）；失败（-1）；
//// 备注：有引用后则不可删除
//// ============================================================================
//s32 obj_up(struct Object *ob)
//{
//    if(!ob)
//        return (-1);
//
//    if(__lock_objsys())
//        return (-1);
//
//    if((ob->rights.b.lock)||(-1==ob->rights.b.muses)) // 对象被禁止访问（将被删除），或者同步使用者达到上限；
//    {
//        __unlock_objsys();
//        return (-1);
//    }
//
//    ob->rights.b.muses++;
//    __unlock_objsys();
//    return (0);
//}
//
//// ============================================================================
//// 功能：对象减引用计数；
//// 参数：ob -- 对象；
//// 返回：成功（0）；失败（-1）；
//// 备注：有引用后则不可删除
//// ============================================================================
//inline s32 obj_down(struct Object *ob)
//{
//    if(__lock_objsys())
//        return (-1);
//
//    if(ob->rights.b.muses)
//        ob->rights.b.muses--;
//
//    __unlock_objsys();
//    return (0);
//}

// ============================================================================
// 功能：增对象引用计数；
// 参数：ob -- 对象；
// 返回：增加后的对象引用次数；
// 备注：有引用后则不可删除
// ============================================================================
//u32 obj_InuseUp(struct Object *Obj)
//{
//    u32 result;
//    OBJ_Lock();
//    if(Obj == NULL)
//        result = 0;
//    else
//    {
//        if(Obj->inuse != CN_LIMIT_UINT32)
//            Obj->inuse++;
//        result = Obj->inuse;
//    }
//    OBJ_Unlock();
//    return result;
//}

// ============================================================================
// 功能：在路径范围内增对象引用计数；例如，对路径"/a/b/c/d/e/f/g"，start对应"c",end
//      对应"f"，则"d","e","f"三个对象的引用次数将增加
// 参数：start，路径起始点，不包含在内。
//      end，路径终点，包含在内
// 返回：无
// 备注：有引用后则不可删除
// ============================================================================
//void obj_InuseUpRange(struct Object *start, struct Object *end)
//{
//    struct Object *temp = end;
//    OBJ_Lock();
//    if((start != NULL) && (end != NULL))
//    {
//        while((temp != start) && (temp != OBJ_GetRoot()))
//        {
//            if(temp->inuse != CN_LIMIT_UINT32)
//                temp->inuse++;
//            temp = temp->parent;
//        }
//    }
//    OBJ_Unlock();
//    return;
//}

// ============================================================================
// 功能：沿路径增对象引用计数；打开一次文件，则从根开始沿到被打开的文件，沿途所有对象
//      的引用次数均增 1
// 参数：ob -- 对象；
// 返回：无
// 备注：有引用后则不可删除
// ============================================================================
void OBJ_DutyUp(struct Object *Obj)
{
    while(Obj != NULL)
    {
        if(Obj->BitFlag.inuse != 0xffffff)
            Obj->BitFlag.inuse++;
        Obj = Obj->parent;
    }
    return;
}

// ============================================================================
// 功能：在路径范围内减对象引用计数；例如，对路径"/a/b/c/d/e/f/g"，start对应"c",end
//      对应"f"，则"d","e","f"三个对象的引用次数将减少
// 参数：start，路径起始点，不包含在内。
//      end，路径终点，包含在内
// 返回：无
// 备注：有引用后则不可删除
// ============================================================================
//void obj_InuseDownRange(struct Object *start, struct Object *end)
//{
//    struct Object *temp = end;
//    OBJ_Lock();
//    if((start != NULL) && (end != NULL))
//    {
//        while((temp != start) && (temp != OBJ_GetRoot()))
//        {
//            if(temp->inuse != 0)
//                temp->inuse--;
//            temp = temp->parent;
//        }
//    }
//    OBJ_Unlock();
//    return ;
//}

// ============================================================================
// 功能：减少对象引用计数，关闭文件时，将减少引用次数。
// 参数：ob -- 对象；
// 返回：减少后的对象引用次数；
// 备注：有引用后则不可删除
// ============================================================================
//u32 obj_InuseDown(struct Object *Obj)
//{
//    u32 result;
//    OBJ_Lock();
//    if(Obj == NULL)
//        result = 0;
//    else
//    {
//        if(Obj->inuse != 0)
//            Obj->inuse--;
//        result = Obj->inuse;
//    }
//
//    OBJ_Unlock();
//    return result;
//}
//
// ============================================================================
// 功能：沿路径减对象引用计数；关闭一次文件，则从根开始沿到被打开的文件，沿途所有对象
//      的引用次数均减 1
// 参数：ob -- 对象；
// 返回：无
// 备注：有引用后则不可删除
// ============================================================================
void OBJ_DutyDown(struct Object *Obj)
{
    while(Obj != NULL)
    {
        if(Obj->BitFlag.inuse != 0)
            Obj->BitFlag.inuse--;
        Obj = Obj->parent;
    }
    return ;
}

// ============================================================================
// 功能：删除对象
// 参数：ob -- 对象；
// 返回：成功（0），失败（-1）；
// 备注：
// ============================================================================
s32 OBJ_Delete(struct Object *Obj)
{
    if(!Obj)
        return (-1);

    OBJ_Lock();

    if(OBJ_IsOnDuty(Obj))
    {
        OBJ_Unlock();
        return (-1); // 正在使用
    }

    if(Obj->next==Obj) // 说明该节点没有兄弟节点.
    {
        Obj->parent->child = NULL;
    }
    else
    {
        if(Obj->parent->child==Obj)
            Obj->parent->child = Obj->next; // 说明该节点是队列头节点,需要改变队列头节点

        __OBJ_LIST_REMOVE(Obj);
    }
    if(Obj->name != __uname_obj)
       free(Obj->name);
    __OBJ_FreeObj(Obj);
    OBJ_Unlock();
    return (0);
}

// ============================================================================
// 功能：将对象子树从对象树关系上分离；
// 参数：branch -- 被删除的分支;
// 返回：成功（被分离的对象分支）;失败（NULL）；
// 备注：
// ============================================================================
struct Object *OBJ_Detach(struct Object *branch)
{
    struct Object *ob;

    if(!branch)
        return (NULL);

    OBJ_Lock();

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
    OBJ_Unlock();
    return (ob);
}

// ============================================================================
// 功能：检查对象名的合法性；
// 参数：name -- 对象名；
// 返回：合法（0）；非法（-1）；
// 备注：检查规则：不允许存在'/'、'\'字符，长度不允许超过规定；
// ============================================================================
inline s32 OBJ_CheckName(const char *name)
{
    if(name)
        return (-1);

    return (__OBJ_CheckName(name));
}

//----沿路径匹配对象名---------------------------------------------------------
//功能: 与OBJ_Search类似,不同的是，找到第一个匹配不上的就返回。例如，对象树中有
//      "obj1\obj2\"，path="obj1\obj2\obj3\obj4"，将返回obj2的指针。
// 参数：match -- 需匹配的路径；
//      left -- 完全匹配，为NULL；不完全匹配，则返回不匹配部分（保证不以'/'开头）；
// 返回：匹配路径所能检索到的最终对象。
// 备注：如果对应路径是磁盘文件路径，则只匹配打开的文件或路径。
//-----------------------------------------------------------------------------
struct Object *OBJ_MatchPath(const char *match, char **left)
{
    char *path = (char*)match;
    char *ResultName;
    struct Object *Base, *current, *result = NULL;
    u32 i;

    *left = path;
    if(path == NULL)
        return NULL;
    if('\0' == *path)
        return NULL;
    OBJ_Lock();

    if(('/' == *path) || ('\\' == *path))
    {
        Base = OBJ_GetRoot();          // 绝对路径
        result = Base;              //根目录是第一个匹配项
    }
    else
    {
        Base = OBJ_GetCurrent();   // 其他都是相对路径
        result = Base;              //根目录是第一个匹配项
    }
    current = Base;
    *left = (char*)match;
    while(Base)
    {
        while(('/' == *path) || ('\\' == *path))
            path++; // 过滤多余的'/'
        if('\0' == *path)
        {
            break; // 遍历路径结束
        }

        if('.' == path[0])
        {
            if('.' == path[1])  //看是否要返回上一级目录
            {
                if(Base == OBJ_GetRoot())
                {
                    result = NULL;
                    *left = path;
                    return NULL;
                }
                Base = OBJ_GetParent(Base);        // ".."字符，表示上一级目录
                result = Base;
                current = Base;
                if(('\\' == path[2])||('/' == path[2]))
                {
                    path++;
                    continue;
                }
                else if('\0' != path[2])        //完成path路径匹配
                {
                    *left = NULL;
                    return current;
                }
                else                            //".."后不是合法的分隔符，非法
                {
//                  result = current;
                    *left = NULL;
                    return NULL;
                }
            }
            else if(('\\' == path[1])||('/' == path[1]))
            {
                path++;
                continue;
            }
            else if('\0' != path[1])        //完成path路径匹配
            {
                *left = NULL;
                return result;
            }
            else                            //"."后不是合法的分隔符，非法
            {
                *left = NULL;
                return NULL;
            }
        }

        while(current)
        {
            current = OBJ_ForeachChild(Base, current);
            if(current == NULL)         //匹配结束
            {
                Base = NULL;
                break;
            }
            ResultName = (char*)OBJ_GetName(current);
            i = strlen(ResultName);
            if(memcmp(ResultName, path, i) == 0)
            {
                if('\0' == path[i])     //匹配结束，current是匹配项
                {
                    result = current;
                    Base = NULL;
                    *left = NULL;
                    break;
                }
                else if(('\\' == path[i]) || ('/' == path[i]))
                {
                    result = current;
                    path += i;
                    *left = path+1;
                    Base = current;     // 匹配当前节点，继续匹配子节点
                    break;
                }
                else    // 当前对象不匹配，继续遍历兄弟节点
                {
//                  break;
                }
            }
        }

    }

    OBJ_Unlock();
    return (result);
}

// ============================================================================
// 功能：基于路径创建一连串对象
// 参数：begin，开始对象节点；
//      ops，所有对象的操作函数
//      Private，所有对象的私有数据
//      path，需建立的临时路径；
// 返回：新创建的最低一级的对象；
// 备注：这里新建时，有对重名做判断；
// ============================================================================
struct Object *OBJ_BuildTempPath(struct Object *begin, fnObjOps ObjOps,
                            ptu32_t Private, char *path)
{
    char *segst, *name=NULL;
    struct Object *current, *find;
    u16 i, seglen;

    if(!path)
        return (begin);

    OBJ_Lock();

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
            OBJ_Unlock();
            return (NULL);
        }

        for(i = 0 ; i < seglen; i++)
            name[i] = segst[i];

        name[seglen] = '\0';
        find = OBJ_SearchChild(current, name);
        if(find)
        {
            current = find; // child已经存在；
        }
        else
        {
            // 这里的ops和represent是一个继承关系，即继承父节点的
            // current = OBJ_AddChild(current, current->ObjOps, current->represent, (const char*)name);
            current = OBJ_NewChild(current, ObjOps, Private, (const char*)name);
            if(!current)
            {
                printf("\r\n: dbug : object :  memory out(%s). ", __FUNCTION__);
                OBJ_Unlock();
                return (NULL);
            }
            OBJ_SetToTemp(current);
        }
    }
    free(name);
    OBJ_Unlock();
    return (current);
}

// ============================================================================
// 功能：释放对象构建的路径；
// 参数：start -- 路径开始的对象；
// 返回：释放了的对象数量；
// 备注：
// ============================================================================
s32 OBJ_ReleaseTempPath(struct Object *start)
{
    s32 dels = 0;
    struct Object *parent, *current = start;

    OBJ_Lock();

    while(1)
    {
        if((current->BitFlag.temporary == 1)
                && (current->BitFlag.inuse == 0)) // 无人使用,且是临时对象
        {
            parent = OBJ_GetParent(current);
            OBJ_Delete(current);
//          __OBJ_FreeObj(current);
            current = parent;
            dels++;
        }
        else
        {
            break;
        }
    }

    OBJ_Unlock();
    return (dels);
}

// ============================================================================
// 功能：获取对象系统的当前对象；
// 参数：
// 返回：对象系统的当前对象；
// 备注：
// ============================================================================
struct Object * OBJ_GetCurrent(void)
{
    return (s_ptCurrentObject);
}

// ============================================================================
// 功能：设置对象系统的当前对象；
// 参数：ob -- 对象；
// 返回：无；
// 备注：
// ============================================================================
void OBJ_SetCurrent(struct Object *ob)
{
    OBJ_DutyDown(s_ptCurrentObject);
    s_ptCurrentObject = ob;
    OBJ_DutyUp(ob);
}

// ============================================================================
// 功能：获取对象系统的根对象；
// 参数：无；
// 返回：对象系统的根对象；
// 备注：
// ============================================================================
struct Object *OBJ_GetRoot(void)
{
    return (s_ptRootObject);
}

// ============================================================================
// 功能：设置对象系统的根对象；
// 参数：ob -- 对象；
// 返回：无；
// 备注：
// ============================================================================
//void objsys_setroot(struct Object *ob)
//{
//    s_ptRootObject = ob;
//}

// ============================================================================
// 功能：给对象新建一个prev关系对象；
// 参数：loc -- 对象；为空，则为当前设置对象；
//      ObjOps -- 对象操作方法；（NULL，继承父方法；-1，使用缺省方法；）
//      ObjPrivate -- 对象私有数据；
//      name -- 对象名字；为空，则使用系统默认名；
// 返回：成功（对象）；失败（NULL）；
// 备注：
// ============================================================================
struct Object *OBJ_NewPrev(struct Object *loc, fnObjOps ObjOps,
                            ptu32_t represent, const char *name)
{
    struct Object *prev;
    char *cname;

    if(!loc)
        loc = s_ptCurrentObject; // 如果未指定父节点，则指向当前工作节点；

    if(name)
    {
        if(__OBJ_CheckName(name))
          return (NULL);

        if(loc->parent)
        {
            if(OBJ_SearchChild(loc->parent, name))
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

    prev = __OBJ_NewObj();
    if(!prev)
    {
        if(name)
            free(cname);
        return (NULL);
    }

    OBJ_Lock();
    prev->parent = loc->parent;
    prev->child = NULL;
    prev->ObjPrivate = represent;
    prev->BitFlag.temporary = 0;
    prev->BitFlag.inuse = 0;


    if(ObjOps)
    {
        if(-1==(s32)ObjOps)
            prev->ObjOps = (fnObjOps)__OBJ_DefaultOps;
        else
            prev->ObjOps = ObjOps;
    }
    else
    {
        if(prev->parent)
            prev->ObjOps = prev->parent->ObjOps;
        else
            prev->ObjOps = (fnObjOps)__OBJ_DefaultOps;
    }

//  prev->rights = prev->parent->rights;

    prev->name = cname;
//  prev->seton = (struct Object*)-1; // 默认对象之上不允许建立对象集合；
//  if(loc->parent)
//      prev->set = loc->parent->set;
//  else
//      prev->set = NULL;

    dListInit(&prev->handles);
    __OBJ_LIST_INS_BEFORE(loc, prev);
    OBJ_Unlock();
    return (prev);
}

// ============================================================================
// 功能：为对象新建一个next关系对象；
// 参数：loc -- 对象；为空，则为当前设置对象；
//      ObjOps -- 对象操作方法；（NULL，继承父方法；-1，使用缺省方法；）
//      ObjPrivate -- 对象私有数据；
//      name -- 对象名字。为空，则使用系统默认名；
// 返回：成功（对象）；失败（NULL）；
// 备注：
// ============================================================================
struct Object *OBJ_NewNext(struct Object *loc, fnObjOps ObjOps,
                        ptu32_t represent, const char *name)
{
    struct Object *next;
    char *cname;

    if(!loc)
        loc = s_ptCurrentObject; // 如果未指定父节点，则指向当前工作节点；

    if(name)
    {
        if(__OBJ_CheckName(name))
            return (NULL);

        if(loc->parent)
        {
            if(NULL != OBJ_SearchChild(loc->parent, name))
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

    next = __OBJ_NewObj();
    if(!next)
    {
        if(name)
            free(cname);
        return (NULL);
    }

    OBJ_Lock();

    next->parent = loc->parent;
    next->child = NULL;
    next->ObjPrivate = represent;
    next->BitFlag.temporary = 0;
    next->BitFlag.inuse = 0;
    if(ObjOps)
    {
        if(-1==(s32)ObjOps)
            next->ObjOps = (fnObjOps)__OBJ_DefaultOps;
        else
            next->ObjOps = ObjOps;
    }
    else
    {
        if(next->parent)
            next->ObjOps = next->parent->ObjOps;
        else
            next->ObjOps = (fnObjOps)__OBJ_DefaultOps;
    }

//  next->rights = next->parent->rights;

    next->name = (char *)name;
//  next->seton = (struct Object*)-1; // 默认对象之上不允许建立对象集合；
    dListInit(&next->handles);
//  if(loc->parent)
//      next->set = loc->parent->set;
//  else
//      next->set = NULL;

    __OBJ_LIST_INS_AFTER(loc, next);
    OBJ_Unlock();
    return (next);

}

// ============================================================================
// 功能：新建子对象；
// 参数：parent -- 父对象；如果未设置，默认为当前设置对象；
//      ObjOps -- 对象方法;（NULL，继承父方法；-1，使用缺省方法；）
//      ObjPrivate -- 对象私有数据；
//      name -- 对象名；不可为系统默认名；（系统默认名用于未设置对象名的逻辑）
// 返回：成功（新建子对象）；失败（NULL）；
// 备注：新建的子对象，放置在子对象链的末尾；
// ============================================================================
struct Object *OBJ_NewChild(struct Object *parent, fnObjOps ObjOps,
                            ptu32_t ObjPrivate, const char *name)
{
    struct Object *child;
    char *cname;

    if(!parent)
        parent = s_ptCurrentObject; // 如果未指定父节点，则指向当前工作节点；

    if(name)
    {
        if(__OBJ_CheckName(name))
            return (NULL);

        if(NULL != OBJ_SearchChild(parent, name))
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

    child = __OBJ_NewObj();
    if(!child)
    {
        if(name)
            free(cname);
        return (NULL);
    }

    OBJ_Lock();

    child->parent = parent;
    child->child = NULL;
    child->ObjPrivate = ObjPrivate;
    child->BitFlag.temporary = 0;
    child->BitFlag.inuse = 0;

    if(ObjOps)
    {
        if(-1==(s32)ObjOps)
            child->ObjOps = (fnObjOps)__OBJ_DefaultOps;
        else
            child->ObjOps = ObjOps;
    }
    else
    {
        child->ObjOps = parent->ObjOps;
    }

//  child->rights = parent->rights;

    child->name = cname;
//  child->seton = (struct Object*)-1; // 默认对象之上不允许建立对象集合；
//  child->set = parent->set; // 集合逻辑必须继承父；
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

    OBJ_Unlock();
    return (child);
}
// ============================================================================
// 功能：新建一个对象，其处于loc对象所处队列的首；
// 参数： ObjOps -- 对象方法；（NULL，继承父方法；-1，使用缺省方法；）
//      ObjPrivate -- 对象私有数据；
//      name -- 对象名；不可为系统默认名；（系统默认名用于未设置对象名的逻辑）
// 返回：成功（新建子对象）；失败（NULL）；
// 备注：
// ============================================================================
struct Object *OBJ_NewHead(struct Object *loc, fnObjOps ObjOps,
                           ptu32_t ObjPrivate, const char *name)
{
    struct Object *head;
    char *cname;

    if(!loc)
        loc = s_ptCurrentObject; // 如果未指定父节点，则指向当前工作节点；

    if(name)
    {
        if(__OBJ_CheckName(name))
            return (NULL);

        if(loc->parent)
        {
            if(NULL != OBJ_SearchChild(loc->parent, name))
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

    head = __OBJ_NewObj();
    if(!head)
    {
        if(name)
            free(cname);
        return (NULL);
    }

    OBJ_Lock();

    head->parent = loc->parent;
    head->child = NULL;
    head->ObjPrivate = ObjPrivate;
    head->BitFlag.temporary = 0;
    head->BitFlag.inuse = 0;
    if(ObjOps)
    {
        if(-1==(s32)ObjOps)
            head->ObjOps = (fnObjOps)__OBJ_DefaultOps;
        else
            head->ObjOps = ObjOps;
    }
    else
    {
        if(head->parent)
            head->ObjOps = head->parent->ObjOps;
        else
            head->ObjOps = (fnObjOps)__OBJ_DefaultOps;
    }

//  head->rights = head->parent->rights;

    head->name = (char *)name;
//  head->seton = (struct Object*)-1;
//  if(loc->parent)
//      head->set = loc->parent->set;
//  else
//      head->set = NULL;

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

    OBJ_Unlock();
    return (head);
}

#if 0
// ============================================================================
// 功能：移动一个对象树枝到别的节点下面成为其子树；
// 参数：pParent -- 新父节点；
//      ob，待插入的子节点，可能有下级节点；
// 返回：成功（0），失败（-1）；
// 备注：
// ============================================================================
s32 OBJ_MoveToBranch(struct Object *pParent,struct Object *ob)
{
    if((pParent==NULL)||(ob==NULL))
        return (-1);

    low_atom = Int_LowAtomStart();
    OBJ_Detach(ob);
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
s32 obj_MoveToLast(struct Object *ob)
{
    struct Object *head;

    if((!ob)||(!ob->parent))
        return (-1);

    OBJ_Lock();

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

    OBJ_Unlock();
    return (0);
}

// ============================================================================
// 功能：将对象移动到队列头；
// 参数：ob -- 对象；
// 返回：成功（0），失败（-1）；
// 备注：
// ============================================================================
s32 OBJ_MoveToHead(struct Object *ob)
{
    struct Object *head;

    if((!ob)||(!ob->parent))
        return (-1);

    OBJ_Lock();

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

    OBJ_Unlock();
    return (0);
}

// ============================================================================
// 功能：将队列child插入到对象loc下，成为其子对象；
// 参数：loc -- 被插入对象；
//      child -- 将插入的对象；
// 返回：成功（被插入的对象）；失败（NULL）；
// 备注：
// ============================================================================
struct Object *OBJ_InsertToChild(struct Object *loc, struct Object *child)
{
    if((loc==NULL)||(child==NULL))
        return (NULL);

    OBJ_Lock();

    OBJ_Detach(child); // 就对象从原对象树中分离；
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

    OBJ_Unlock();
    return (child);
}

// ============================================================================
// 功能：将对象（next）插入到对象(loc)的后（next）位置；
// 参数：loc -- 目标对象；
//      next -- 目标节点,它可能原本就在Object队列中；
// 返回：成功（0），失败（-1）；
// 备注：
// ============================================================================
s32 OBJ_InsertToNext(struct Object *loc, struct Object *next)
{

    if((loc==NULL)||(next==NULL)||(loc==next))
        return (-1);

    OBJ_Lock();

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

    OBJ_Unlock();
    return (0);
}

// ============================================================================
// 功能：将对象（prev）插入到对象(loc)的前（previous）位置；
// 参数：loc -- 被插入的对象；
//      next -- 需插入的对象；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 OBJ_InsertToPrev(struct Object *loc, struct Object *prev)
{
    if((loc==NULL)||(prev==NULL)||(loc==prev))
        return (-1);

    OBJ_Lock();

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

    OBJ_Unlock();
    return (0);
}

// ============================================================================
// 功能: 父对象的子对象的相对位置不变，将子对象链表的队列头朝前（previous）方向移动一格。
// 参数：parent -- 父对象；
// 返回：成功（0），失败（-1）；
// 备注：
// ============================================================================
s32 OBJ_ChildMoveToOrev(struct Object *parent)
{
    if(!parent)
        return (-1);

    OBJ_Lock();

    if(parent->child)
        parent->child = parent->child->prev;

    OBJ_Unlock();
    return (0);
}

// ============================================================================
// 功能: 父对象的子对象的相对位置不变，将子对象链表的队列头朝后（next）方向移动一格。
// 参数：parent -- 父对象；
// 返回：成功（0），失败（-1）；
// 备注：
// ============================================================================
s32 OBJ_ChildMoveToNext(struct Object *parent)
{
    if(!parent)
        return (-1);

    OBJ_Lock();

    if(parent->child)
        parent->child = parent->child->next;

    OBJ_Unlock();
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
struct Object *OBJ_GetTwig(struct Object *ob)
{
    struct Object *result = NULL, *current;

    if(!ob)
        return (NULL);

    OBJ_Lock();

    current = ob;
    while(current->child != NULL)
    {
        current = current->child;
    }

    if(current == ob)
        result = NULL;
    else
        result = current;

    OBJ_Unlock();
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
char *obj_getpath(struct Object *ob, char *path, u32 limit)
{
    struct Object *Current,*root;
    char *pos;
    u32 count = 0;
    if((ob == NULL)||(path == NULL) || (limit < 2))
        return (NULL);
    Current = ob;
    root = s_ptRootObject;
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
struct Object *OBJ_ForeachChild(struct Object *parent, struct Object *current)
{
    if((parent==NULL)||(current==NULL))
        return (NULL);

    OBJ_Lock();

    if((current==parent)||(current->parent!=parent))
    {
        current = parent->child; // data不是某个子对象，则从首子对象开始；
    }
    else
    {
        current = current->next;
        if(current == parent->child)
            current = NULL;
    }

    OBJ_Unlock();
    return (current);
}

// ============================================================================
// 功能：遍历对象的所有子孙对象；
// 参数：ancester -- 对象；
//      scion -- 当前遍历到的对象,
// 返回：当前搜索位置的下一个节点指针,如果已经搜索完成,则返回NULL.
// 备注：当需要对对象树中某一个树枝或者整个链表中的节点逐一进行某种操作时,可
//      反复调用本函数,第一次调用current_node = parent_node,其后current_node
//      = 上次返回值，直到返回空.
//      本函数按父、子、孙、曾孙....的顺序搜索，先搜直系，再搜旁系，确保所有子孙
//      节点都能够访问到，如果对访问顺序有特殊要求，不能使用本函数；
// ============================================================================
struct Object *OBJ_ForeachScion(struct Object *ancester, struct Object *current)
{
    struct Object *result = NULL, *current_copy;
    bool_t up = FALSE;

    if((ancester==NULL)||(current==NULL))
        return (NULL);

    OBJ_Lock();

    if((current != ancester)||ancester->child)
    {
        current_copy = current;
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

    OBJ_Unlock();
    return (result);
}

// ============================================================================
// 功能：查找兄弟（同级链表上的）对象；
// 参数：ob -- 对象;.
//      name -- 需要搜索的对象名;
// 返回：成功（名为name的对象）；失败（NULL）；
// 备注：
// ============================================================================
struct Object *OBJ_SearchSibling(struct Object *ob, const char *name)
{
    struct Object *tmp, *result = NULL;

    if((ob==NULL)||(name==NULL))
        return (NULL);

    OBJ_Lock();

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

    OBJ_Unlock();
    return (result);
}

// ============================================================================
// 功能：查找子对象；
// 参数：parent -- 对象；
//      name -- 需要搜索的对象名
// 返回：成功（名为name的对象）；失败（NULL）；
// 备注：
// ============================================================================
struct Object *OBJ_SearchChild(struct Object *parent, const char *name)
{
    struct Object *tmp, *result = NULL;

    if((name == NULL) || (parent == NULL))
        return (NULL);

    OBJ_Lock();

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

    OBJ_Unlock();
    return (result);
}

// ============================================================================
// 功能：查找对象的子孙对象
// 参数：ancester -- 对象；
//      name -- 需查找的对象名；
// 返回：成功（名为name的对象）；失败（NULL）；
// 备注：
// ============================================================================
struct Object *OBJ_SearchScion(struct Object *ancester, const char *name)
{
    struct Object *current, *tmp, *result = NULL;

    if((ancester == NULL)||(name == NULL))
        return (NULL);

    OBJ_Lock();

    current = ancester;
    while((tmp = OBJ_ForeachScion(ancester, current)) != NULL)
    {
        if(strcmp(tmp->name, name) == 0)
        {
            result = tmp;
            break;
        }
        current = tmp;
    }

    OBJ_Unlock();
    return (result);
}

// ============================================================================
// 功能：从对象（start）开始依据路径查找对象；
// 参数：start -- 路径的起始对象；
//      path -- 检索的对象路径；
// 返回：成功（路径结束的对象）；失败（NULL）；
// 备注：
// ============================================================================
struct Object *OBJ_SearchPath(struct Object *start, const char *path)
{
    char *ResultName;
    struct Object *Base, *current, *result = NULL;
    u32 i;

    if(path == NULL)
        return NULL;
    if('\0' == *path)
        return NULL;
    OBJ_Lock();

    Base = start;
    if(Base == NULL)
    {
        Base = OBJ_GetRoot();          // 绝对路径
    }
    current = Base;
    while(Base)
    {
        while(('/' == *path) || ('\\' == *path))
            path++; // 过滤多余的'/'
        if('\0' == *path)
        {
            break; // 遍历路径结束
        }

        if('.' == path[0])
        {
            if('.' == path[1])  //看是否要返回上一级目录
            {
                if(Base == OBJ_GetRoot())
                {
                    result = NULL;
                    return NULL;
                }
                Base = OBJ_GetParent(Base);        // ".."字符，表示上一级目录
                result = Base;
                current = Base;
                if(('\\' == path[2])||('/' == path[2]))
                {
                    path++;
                    continue;
                }
                else if('\0' != path[2])        //完成path路径匹配
                {
                    return current;
                }
                else                            //".."后不是合法的分隔符，非法
                {
                    return NULL;
                }
            }
            else if(('\\' == path[1])||('/' == path[1]))
            {
                path++;
                continue;
            }
            else if('\0' != path[1])        //完成path路径匹配
            {
                return result;
            }
            else                            //"."后不是合法的分隔符，非法
            {
                return NULL;
            }
        }

        while(current)
        {
            current = OBJ_ForeachChild(Base, current);
            if(current == NULL)         //匹配结束
            {
                Base = NULL;
                break;
            }
            ResultName = (char*)OBJ_GetName(current);
            i = strlen(ResultName);
            if(memcmp(ResultName, path, i) == 0)
            {
                if('\0' == path[i])     //匹配结束，current是匹配项
                {
                    result = current;
                    Base = NULL;
                    break;
                }
                else if(('\\' == path[i]) || ('/' == path[i]))
                {
                    result = current;
                    path += i;
                    Base = current;     // 匹配当前节点，继续匹配子节点
                    break;
                }
                else    // 当前对象不匹配，继续遍历兄弟节点
                {
//                  break;
                }
            }
        }
    }

    OBJ_Unlock();
    return (result);
}
#if 0
{
#if 1
    const char *path_name;
    char *ObjName;
    bool_t end = FALSE;
    u32 i;
    struct Object *current, *result = NULL;

    if((path == NULL) || (start == NULL))
        return (NULL);

    if(OBJ_Lock())
        return (NULL);


    current = start->child;
    if(current == NULL)
    {
        OBJ_Unlock();
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

    OBJ_Unlock();
    return result;
#else
    char *segment;
    struct Object *current = start, *head;
    u32 i;

    if((!current)||(!path))
        return (NULL);

    OBJ_Lock();

    while(current)
    {
__SEARCH_NEXT:
        while(('/' == *path) || ('\\' == *path))
            path++; // 过滤多余的'/'

        if('\0' == *path)
            break; // 遍历路径结束

        if(('.' == path[0]) && ('.' == path[1])
           && (('\\' == path[2]) || ('/' == path[2]) || ('\0' == path[2])))
        {
            current = OBJ_GetParent(current);
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

        segment = (char*)OBJ_GetName(head);
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
            segment = (char*)OBJ_GetName(current);
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

        current = OBJ_GetParent(current); // 当前节点与路径不匹配，退回上上以及节点
        break; // 未找到节点
    }

__SEARCH_DONE:
   OBJ_Unlock();
   return (current);
#endif
}
#endif

//-----------------------------------------------------------------------------
//功能: 设置环境变量,系统当前工作路径
//参数:
//返回: 0 -- 成功; -1 -- 对象找不到;
//备注:
//-----------------------------------------------------------------------------
s32 OBJ_SetPwd(const char *Path)
{
    DIR *dir;
    s32 res = 0;
    struct objhandle *hdl;
    struct Object *ob;

    if(!Path)
        return (-1);
    dir = opendir(Path);
    if(!dir)
        return (-1);
    hdl = (struct objhandle*)(dir->__fd); // 目录的上下文
    ob = hdl->HostObj;      // 目录的节点
    if(NULL == ob)
        res = -1;

    OBJ_SetCurrent(ob);
    closedir(dir);// 关闭目录
    return res;
}
//-----------------------------------------------------------------------------
//功能: 获取当前工作路径字符串长度(含结束符)
//参数:
//返回: 非零-- 成功; 零 -- 失败，未设置当前工作路径;
//备注:
//-----------------------------------------------------------------------------
s32 OBJ_CurWorkPathLen(void)
{
    u32 PathLen = 0;
    struct Object *ob;
    ob = s_ptCurrentObject;

    while(1)
    {
        PathLen++;
        PathLen += strlen(OBJ_GetName(ob));
        if(OBJ_GetParent(ob) == NULL)
            break;
        ob = OBJ_GetParent(ob);
    }

    return PathLen;
}

//-----------------------------------------------------------------------------
//功能: 获取当前工作路径
//参数: Buf -- 当前工作路径
//      BufSize -- 路径长度，包括字符串结束符
//返回: 0 -- 成功; -1 -- 未设置当前工作路径; -2 -- 参数错误; -3 -- 内存不足;
//      -4 -- 其他;
//备注:
//-----------------------------------------------------------------------------
s32 OBJ_CurWorkPath(char *Buf, u32 BufSize)
{
    struct Object *Obj;
    u32 ObjNameLen, Offset;
    char *PathTemp=NULL, *ObjName;
    s32 Ret = 0;

    OBJ_Lock();// 进互斥(防止操作过程当前工作路径被更改)

    Offset = OBJ_CurWorkPathLen();
    if((NULL == Buf) || (BufSize < Offset))
    {
        Ret = -2; // 参数错误
        goto FAIL;
    }

    PathTemp = (char*)malloc(Offset + 1);
    if(NULL == PathTemp)
    {
        Ret = -3; // 内存不足
        goto FAIL;
    }
    memset(PathTemp, 0, Offset + 1);
    Offset -= 1;
    PathTemp[Offset] = '\0'; // 路径的结束符
    Obj = s_ptCurrentObject; // 路径最后一个节点

    for(;;)
    {
        ObjName = (char *)OBJ_GetName(Obj);
        ObjNameLen = strlen(ObjName);
        Offset = Offset - ObjNameLen;
        memcpy((PathTemp + Offset), ObjName, ObjNameLen);
        if(Offset) // 去除根的情况
        {
            Offset--;
            PathTemp[Offset] = '/'; // 路径之间的分隔或者是根
        }

        if(0 == Offset)
        {
            if(PathTemp[Offset] == '\0')
                PathTemp[Offset] = '/';
            strcpy(Buf, PathTemp);
            break; // 结束
        }
        Obj = OBJ_GetParent(Obj);
        if(NULL == Obj)
        {
            Ret = -4;
            break;
        }
    }

FAIL:
    if(PathTemp)
        free(PathTemp);
    OBJ_Unlock(); // 出互斥
    return (Ret);
}

bool_t OBJ_Show(char *param)
{
    struct Object *Obj;
    s32 res;
    Obj = OBJ_SearchPath(OBJ_GetCurrent(), param);
    if(Obj)
    {
        res = Obj->ObjOps((void *)Obj, CN_OBJ_CMD_SHOW, 0,0,0);
        if(res != CN_OBJ_CMD_TRUE)
            printf("%s 无法显示\r\n", param);
    }
    else
        printf("%s 不存在\r\n",param);
    return true;
}
ADD_TO_ROUTINE_SHELL(catobj,OBJ_Show,"显示一个对象内容，不包括磁盘文件，COMMAND:cat + 对象路径 + enter.");

