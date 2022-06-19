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
//����ģ��: �������
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ��������ӡ��ƶ���ɾ������������ڵ�ķ���
//����˵��:
//�޶���ʷ:
//4. ����:20130220
//   ����:  ������.
//   �°汾�ţ�v1.3.0
//   �޸�˵��:
//      1.������rsc�������ܵ������������⣬bug������ë���ύ�ġ�
//3. ����:20090518
//   ����:  ������.
//   �°汾�ţ�v1.2.0
//   �޸�˵��:
//      1.�ڸ����ϣ��ֿ��������Ͷ������������Ҳ��Ӧ�ص�����
//      2.����rsc_get_root������ԭrsc_get_root����Ϊrsc_get_tree���Ҳ��ټ�ְ
//      3.������rsc_get_root��ȡ��1.1.0�汾��"�޸�3"
//      4.��ȫ��NULL�������
//2. ����:20090131
//   ����:  ������.
//   �°汾�ţ�v1.1.0
//   �޸�˵��:
//      1.���˸��ܸ��ڵ㣬�����ģ���ʼ������
//      2.���ź��������������ʰ�ȫ
//      3.�Ľ��˱���������������NULL���������������
//      4.������һЩbug
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           �������㷨˵��
// ��ͬ���ڵ�����˫��ѭ������
// ÿ���ڵ��childָ��ֱ��ָ��Ľڵ��Ϊ�ýڵ�����ڶ��еĶ���ͷ������previous�ڵ�
// �Ƕ���β��
// ���нڵ��Parentָ���ָ�򸸽ڵ�

// �ر���ʾ��������еĲ������ʰ�ȫ�ɶ������ģ�鸺�𣬶���ڵ�����Ĳ�������
//          ������ʹ���߸���
// ��������޸Ķ���ʹ��ԭ�Ӳ�����֤������걸��
// ������Ķ�������ʹ���κΰ�ȫ���ƣ��ɶ�������ģ������֤�������İ�ȫ��
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

// OBJ˫�������ʼ��
#define __OBJ_LIST_INIT(l)              (l)->next = (l)->prev = (l)

// ��˫��ѭ�������lλ�ú������ڵ�o
#define __OBJ_LIST_INS_AFTER(l, o)      \
                                        do \
                                        {  \
                                            (l)->next->prev = (o); \
                                            (o)->next = (l)->next; \
                                            (l)->next = (o); \
                                            (o)->prev = (l); \
                                        }while(0)

// ��˫��ѭ�������lλ��ǰ�����ڵ�o
#define __OBJ_LIST_INS_BEFORE(l, o)     \
                                        do \
                                        { \
                                            (l)->prev->next = (o); \
                                            (o)->prev = (l)->prev; \
                                            (l)->prev = (o); \
                                            (o)->next = (l); \
                                        }while(0)

// ��˫��ѭ���������Ƴ�, NOTE: ����δ�������¼�����Ĵ���
#define __OBJ_LIST_REMOVE(l)            \
                                        do \
                                        { \
                                            (l)->next->prev = (l)->prev; \
                                            (l)->prev->next = (l)->next; \
                                            (l)->next = (l)->prev = (l); \
                                        }while(0)

// �Ƿ������һ�������Ա��
#define __OBJ_LIST_LAST(l)              (((l)->next == (l)) ? 1 : 0)

// ѭ����ѯ�ֵ�����
#define __OBJ_LIST_FOR_EACH_BROTHERS(l, h)            \
                                        for((l) = (h)->next; (l) != (h); (l) = (l)->next)


#define CN_OBJ_NAME_LIMIT               255 // ��������󳤶ȣ�

static inline s32 __OBJ_CheckName(const char *name);
static inline struct Object *__OBJ_NewObj(void);
static inline void __OBJ_FreeObj(struct Object *ob);

static struct MemCellPool s_tObjectPool;
static struct Object s_tObjectInitPool[CFG_OBJECT_LIMIT];
static struct Object *s_ptRootObject, *s_ptCurrentObject; // �߼��ϣ�Ӧ����ÿһ������һ����
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
// ���ܣ�Ϊ�����ṩһ��Ĭ�ϲ�������������ֻ��Ҫ����һ�����󣬲�����Ҫ��Ϊ�ļ�����
//      ʱ���Ͳ���Ҫ�ṩ Operation ��������ʱ��ϵͳ���Զ��Ѷ����������ָ�����
// ��������û�����塣
// ���أ��̶����� CN_OBJ_CMD_UNSUPPORT
// ��ע��
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
//      ftp��ͨ���ܷ��ȡ����obj��״̬��������obj�Ƿ�Ҫͨ��ftp��ʾ��������������ѻ�ȡ״̬�Ĺ���ע���ˣ���Ȼ���нڵ㶼����ʾ����


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
// ���ܣ�Ĭ�ϵĴ򿪶�������������ʹ�ã������������ʱ������ops == -1����ʹ�ô˺�����
// ������ob -- ��Ŀ¼����(���ܲ�����Ҫ�򿪵��ļ�)��
//      flags -- �ļ���ǣ�
//      uncached -- δƥ��Ĳ��֣�
// ���أ��ɹ����루�¶��󣩣�ʧ�ܣ�NULL����
// ��ע��
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
    Handle_Init(hdl, ob, flags, (ptu32_t)NULL);     //��obj��hdl��������
    return (hdl);
}
#pragma GCC diagnostic pop

// ============================================================================
// ���ܣ�Ĭ�ϵĶ���������������ʹ�ã������������ʱ������ops == -1����ʹ�ô˺�����
// ������directory -- ��Ŀ¼�Ķ���������
//      dentry -- Ŀ¼�
// ���أ�ȫ�����꣨1��������һ�0����
// ��ע��
// ============================================================================
static s32 __OBJ_ReadDentry(struct objhandle *directory, struct dirent *dentry)
{
    struct Object *ob = (struct Object *)dentry->d_ino;
    if(!ob) // ��һ�ζ���
    {
        ob = OBJ_GetChild(Handle_GetHostObj(directory));
        if(!ob)
            return (1); // û������Ŀ��
    }
    else // ��������
    {
        ob = OBJ_GetNext(ob);
        if(ob==OBJ_GetChild(Handle_GetHostObj(directory)))
            return (1); // ȫ�����ꣻ
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
// ���ܣ�Ĭ�ϵĹرն�������������ʹ�ã������������ʱ������ops == -1����ʹ�ô˺�����
// ������hdl -- ��Ŀ¼�Ķ�������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
static s32 __OBJ_Close(struct objhandle *hdl)
{
    return Handle_Delete(hdl);
}

// ============================================================================
// ���ܣ���������ϵͳ��
// ������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
bool_t OBJ_Lock(void)
{
    return Lock_MutexPend(&s_tObjectMutex, CN_TIMEOUT_FOREVER);

}

// ============================================================================
// ���ܣ���������ϵͳ��
// ������
// ���أ�
// ��ע��
// ============================================================================
void OBJ_Unlock(void)
{
    Lock_MutexPost(&s_tObjectMutex);
}

// ============================================================================
// ���ܣ������¶���
// �������ޣ�
// ���أ��ɹ����루�¶��󣩣�ʧ�ܣ�NULL����
// ��ע��
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
// ���ܣ��ͷŶ���
// ������ob -- ����
// ���أ��ޣ�
// ��ע��
// ============================================================================
static inline void __OBJ_FreeObj(struct Object *ob)
{
    Mb_Free(&s_tObjectPool, ob);
    __objsys_statics.fress++;
    __objsys_statics.alives--;
}

// ============================================================================
// ���ܣ����������ĺϷ��ԣ�
// ������name -- ��������
// ���أ��Ϸ���0�����Ƿ���-1����
// ��ע��
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
// ���ܣ���ȡ�����˽�г�Աֵ��
// ������ob -- ����
// ���أ������ֵ��
// ��ע��
// ============================================================================
ptu32_t OBJ_GetPrivate(struct Object *ob)
{
    if(ob)
        return (ob->ObjPrivate);

    return (0);
}

// ============================================================================
// ���ܣ����ö����˽�г�Աֵ��
// ������ob -- ����
//      represent -- �����ֵ��
// ���أ��ޣ�
// ��ע��
// ============================================================================
void OBJ_SetPrivate(struct Object *ob, ptu32_t Private)
{
    if(ob)
        ob->ObjPrivate = Private;
}

// ============================================================================
// ���ܣ���ȡ���������
// ������ob -- ����
// ���أ��ɹ��������������ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
fnObjOps OBJ_GetOps(struct Object *ob)
{
    if(!ob)
        return (NULL);

    return (ob->ObjOps);
}

// ============================================================================
// ���ܣ����ö��������
// ������ob -- ����
//      ObjOps -- ���������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 OBJ_SetOps(struct Object *ob, fnObjOps ObjOps)
{
    if(!ob)
        return (-1);

    ob->ObjOps = ObjOps;
    return (0);
}

// ============================================================================
// ���ܣ����ö���Ϊ��ʱ������ʱ������closeʱ��ֻҪ��������0������ɾ����ע�⣬�������
//      �����д��ڷ���ʱ������������Ϊ��ʱ����
// ������ob -- ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע���κ���û�����������ʱ�������Բ������
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
// ���ܣ���ȡ��������
// ������ob -- ����
// ���أ��ɹ�������������ʧ�ܣ�NULL����
// ע�⣺
// ============================================================================
char *OBJ_GetName(struct Object *ob)
{
    if(!ob)
        return (NULL);

    return (ob->name);
}

// ============================================================================
// ����: ��ȡ����ĸ���ϵ����
// ����: ob -- ����
// ����: �ɹ������ڵ㣩��ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
struct Object *OBJ_GetParent(struct Object *ob)
{
    if(!ob)
        return (NULL);

    return (ob->parent);
}

// ============================================================================
// ���ܣ���ȡ������ӹ�ϵ����ͬ���Ӷ������������ͷ��
// ������ob -- ����
// ���أ��ɹ����Ӷ��󣩣�ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
struct Object *OBJ_GetChild(struct Object *ob)
{
    if(!ob)
        return (NULL);

    return (ob->child);
}

// ============================================================================
// ���ܣ���ȡ�����ǰһ������ͬ�����������֮�ϳ�Ա��
// ������ob -- ����
// ���أ��ɹ���ǰһ�����󣩣�ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
struct Object *OBJ_GetPrev(struct Object *ob)
{
    if(!ob)
        return (NULL);

    return (ob->prev);
}

// ============================================================================
// ���ܣ���ȡ����ĺ�һ�����󣨶�������֮�ϳ�Ա��
// ������ob -- ����
// ���أ��ɹ�����һ�����󣩣�ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
struct Object *OBJ_GetNext(struct Object *ob)
{
    if(!ob)
        return (NULL);

    return (ob->next);
}

// ============================================================================
// ���ܣ���ȡ����������׳�Ա
// ������ob -- ����
// ���أ��ɹ�����һ�����󣩣�ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
struct Object *OBJ_GetHead(struct Object *ob)
{
    if(!ob)
        return (NULL);

    if(!ob->parent)
        return (ob); // û�и������䱾����ǵ����ģ�����Ҳ�����׳�Ա�ˣ�

    return (ob->parent->child);
}

// ============================================================================
// ���ܣ���ȡ�����ڶ�����ϵ�еĵȼ�����ȣ���
// ������ob -- ����
// ���أ��ɹ�����ȣ���ʧ�ܣ�-1����
// ��ע������ʾ0���������ӹ�ϵ�����ʾ1���������ӹ�ϵ������ӹ�ϵ�����ʾ2����......
//      Ŀǰ�޶���Ȳ�����256�����ǵ�ԭ���ǣ�����256ʱ��ʵ����·��Ҳ�ǿ��ܱ�ʾ�������ģ�
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
            break; // ���û�и��ڵ㣬Ҳ��Ϊ�ǵ�ͷ�ˣ�
        }

        if(level++>256) // ��ֹ���ݱ��ƻ���������
        {
            OBJ_Unlock();
            return (-1);
        }
    }

    OBJ_Unlock();
    return (level);
}

// ============================================================================
// ���ܣ���ȡ������ͬ�������е�λ�ã�
// ������ob -- ����
// ���أ������λ�ã�����-1����
// ��ע������ͷΪ0������ĵڶ�����ԱΪ1��......
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

//----�����ļ����-------------------------------------------------------------
//���ܣ�����һ�������´�����ȫ���ļ��������÷���
//      struct objhandle *Current = NULL;
//      while(1)
//      {
//          Current = OBJ_TraveFile(Current, s_ptSocketObject);
//          if(Current != NULL)
//              do something;
//          else
//              break;
//      }
//������Current����ǰ����λ�ã���ʼΪNULL��
//���أ���һ���ļ�ָ�룬��������򷵻�NULL
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
// ���ܣ�������������������
// ������hdl -- ��������
//      ob -- ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
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
// ���ܣ���ʼ������ϵͳ��
// ��������
// ���أ��ɹ���0
// ��ע��
// ============================================================================
s32 OBJ_ModuleInit(void)
{
    Lock_MutexCreate_s(&s_tObjectMutex, "obj sys");

    Mb_CreatePool_s(&s_tObjectPool, s_tObjectInitPool,
            CFG_OBJECT_LIMIT, sizeof(struct Object), 16, 16384, "object");
    s_ptRootObject = __OBJ_NewObj();

    __OBJ_LIST_INIT(s_ptRootObject);
    s_ptRootObject->name = "";
//  s_ptRootObject->rights = S_IRWXUGO;       //����Ĭ��Ȩ����ӵ������Ȩ��
    s_ptRootObject->parent = NULL;
    s_ptRootObject->ObjOps = (fnObjOps)__OBJ_DefaultOps;
    s_ptRootObject->BitFlag.temporary = 0;
    s_ptRootObject->BitFlag.inuse = 1;  //����Ϊ��ǰĿ¼���ʳ�ʼ��Ϊ1
    s_ptCurrentObject = s_ptRootObject;
    return (0);
}

// ============================================================================
// ���ܣ������Ƿ���ͬ�������ͷ��Ա��
// ������ob -- ����
// ���أ��ǣ�1������0����
// ��ע��
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
// ���ܣ������Ƿ���ͬ�������β��Ա��
// ������ob -- ����
// ���أ��ǣ�1������0����
// ��ע��
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
// ���ܣ������Ƿ�ʹ���У�
// ������ob -- ����
// ���أ��ļ�ʹ���У�1�����ļ�δʹ�ã�0��
// ��ע��
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
//// ���ܣ�������������ֹ�·��ʣ�
//// ������ob -- ����
//// ���أ��ɹ���0����ʧ�ܣ�-1����
//// ��ע��
//// ============================================================================
//s32 OBJ_Lock(struct Object *ob)
//{
//    if(OBJ_Lock())
//        return (-1);
//
//    if(ob->rights.lock) // �Ѿ�����
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
//// ���ܣ����������
//// ������ob -- ����
//// ���أ��ɹ���0����ʧ�ܣ�-1����
//// ��ע��INLINE
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
//// ���ܣ����������ü�����
//// ������ob -- ����
//// ���أ��ɹ���0����ʧ�ܣ�-1����
//// ��ע�������ú��򲻿�ɾ��
//// ============================================================================
//s32 obj_up(struct Object *ob)
//{
//    if(!ob)
//        return (-1);
//
//    if(__lock_objsys())
//        return (-1);
//
//    if((ob->rights.b.lock)||(-1==ob->rights.b.muses)) // ���󱻽�ֹ���ʣ�����ɾ����������ͬ��ʹ���ߴﵽ���ޣ�
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
//// ���ܣ���������ü�����
//// ������ob -- ����
//// ���أ��ɹ���0����ʧ�ܣ�-1����
//// ��ע�������ú��򲻿�ɾ��
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
// ���ܣ����������ü�����
// ������ob -- ����
// ���أ����Ӻ�Ķ������ô�����
// ��ע�������ú��򲻿�ɾ��
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
// ���ܣ���·����Χ�����������ü��������磬��·��"/a/b/c/d/e/f/g"��start��Ӧ"c",end
//      ��Ӧ"f"����"d","e","f"������������ô���������
// ������start��·����ʼ�㣬���������ڡ�
//      end��·���յ㣬��������
// ���أ���
// ��ע�������ú��򲻿�ɾ��
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
// ���ܣ���·�����������ü�������һ���ļ�����Ӹ���ʼ�ص����򿪵��ļ�����;���ж���
//      �����ô������� 1
// ������ob -- ����
// ���أ���
// ��ע�������ú��򲻿�ɾ��
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
// ���ܣ���·����Χ�ڼ��������ü��������磬��·��"/a/b/c/d/e/f/g"��start��Ӧ"c",end
//      ��Ӧ"f"����"d","e","f"������������ô���������
// ������start��·����ʼ�㣬���������ڡ�
//      end��·���յ㣬��������
// ���أ���
// ��ע�������ú��򲻿�ɾ��
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
// ���ܣ����ٶ������ü������ر��ļ�ʱ�����������ô�����
// ������ob -- ����
// ���أ����ٺ�Ķ������ô�����
// ��ע�������ú��򲻿�ɾ��
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
// ���ܣ���·�����������ü������ر�һ���ļ�����Ӹ���ʼ�ص����򿪵��ļ�����;���ж���
//      �����ô������� 1
// ������ob -- ����
// ���أ���
// ��ע�������ú��򲻿�ɾ��
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
// ���ܣ�ɾ������
// ������ob -- ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 OBJ_Delete(struct Object *Obj)
{
    if(!Obj)
        return (-1);

    OBJ_Lock();

    if(OBJ_IsOnDuty(Obj))
    {
        OBJ_Unlock();
        return (-1); // ����ʹ��
    }

    if(Obj->next==Obj) // ˵���ýڵ�û���ֵܽڵ�.
    {
        Obj->parent->child = NULL;
    }
    else
    {
        if(Obj->parent->child==Obj)
            Obj->parent->child = Obj->next; // ˵���ýڵ��Ƕ���ͷ�ڵ�,��Ҫ�ı����ͷ�ڵ�

        __OBJ_LIST_REMOVE(Obj);
    }
    if(Obj->name != __uname_obj)
       free(Obj->name);
    __OBJ_FreeObj(Obj);
    OBJ_Unlock();
    return (0);
}

// ============================================================================
// ���ܣ������������Ӷ�������ϵ�Ϸ��룻
// ������branch -- ��ɾ���ķ�֧;
// ���أ��ɹ���������Ķ����֧��;ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
struct Object *OBJ_Detach(struct Object *branch)
{
    struct Object *ob;

    if(!branch)
        return (NULL);

    OBJ_Lock();

    if(branch->next==branch)   //˵���ýڵ�û���ֵܽڵ�.
    {
        branch->parent->child = NULL;
    }
    else
    {
        if(branch->parent->child == branch)
        {   //˵���ýڵ��Ƕ���ͷ�ڵ�,��Ҫ�ı����ͷ�ڵ�
            branch->parent->child = branch->next;
        }

        __OBJ_LIST_REMOVE(branch);
    }

    ob = branch;
    OBJ_Unlock();
    return (ob);
}

// ============================================================================
// ���ܣ����������ĺϷ��ԣ�
// ������name -- ��������
// ���أ��Ϸ���0�����Ƿ���-1����
// ��ע�������򣺲��������'/'��'\'�ַ������Ȳ��������涨��
// ============================================================================
inline s32 OBJ_CheckName(const char *name)
{
    if(name)
        return (-1);

    return (__OBJ_CheckName(name));
}

//----��·��ƥ�������---------------------------------------------------------
//����: ��OBJ_Search����,��ͬ���ǣ��ҵ���һ��ƥ�䲻�ϵľͷ��ء����磬����������
//      "obj1\obj2\"��path="obj1\obj2\obj3\obj4"��������obj2��ָ�롣
// ������match -- ��ƥ���·����
//      left -- ��ȫƥ�䣬ΪNULL������ȫƥ�䣬�򷵻ز�ƥ�䲿�֣���֤����'/'��ͷ����
// ���أ�ƥ��·�����ܼ����������ն���
// ��ע�������Ӧ·���Ǵ����ļ�·������ֻƥ��򿪵��ļ���·����
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
        Base = OBJ_GetRoot();          // ����·��
        result = Base;              //��Ŀ¼�ǵ�һ��ƥ����
    }
    else
    {
        Base = OBJ_GetCurrent();   // �����������·��
        result = Base;              //��Ŀ¼�ǵ�һ��ƥ����
    }
    current = Base;
    *left = (char*)match;
    while(Base)
    {
        while(('/' == *path) || ('\\' == *path))
            path++; // ���˶����'/'
        if('\0' == *path)
        {
            break; // ����·������
        }

        if('.' == path[0])
        {
            if('.' == path[1])  //���Ƿ�Ҫ������һ��Ŀ¼
            {
                if(Base == OBJ_GetRoot())
                {
                    result = NULL;
                    *left = path;
                    return NULL;
                }
                Base = OBJ_GetParent(Base);        // ".."�ַ�����ʾ��һ��Ŀ¼
                result = Base;
                current = Base;
                if(('\\' == path[2])||('/' == path[2]))
                {
                    path++;
                    continue;
                }
                else if('\0' != path[2])        //���path·��ƥ��
                {
                    *left = NULL;
                    return current;
                }
                else                            //".."���ǺϷ��ķָ������Ƿ�
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
            else if('\0' != path[1])        //���path·��ƥ��
            {
                *left = NULL;
                return result;
            }
            else                            //"."���ǺϷ��ķָ������Ƿ�
            {
                *left = NULL;
                return NULL;
            }
        }

        while(current)
        {
            current = OBJ_ForeachChild(Base, current);
            if(current == NULL)         //ƥ�����
            {
                Base = NULL;
                break;
            }
            ResultName = (char*)OBJ_GetName(current);
            i = strlen(ResultName);
            if(memcmp(ResultName, path, i) == 0)
            {
                if('\0' == path[i])     //ƥ�������current��ƥ����
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
                    Base = current;     // ƥ�䵱ǰ�ڵ㣬����ƥ���ӽڵ�
                    break;
                }
                else    // ��ǰ����ƥ�䣬���������ֵܽڵ�
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
// ���ܣ�����·������һ��������
// ������begin����ʼ����ڵ㣻
//      ops�����ж���Ĳ�������
//      Private�����ж����˽������
//      path���轨������ʱ·����
// ���أ��´��������һ���Ķ���
// ��ע�������½�ʱ���ж��������жϣ�
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
            path++; // ���˶����'/'

        segst = path;
        while(('/' != *path) && ('\\' != *path) && ('\0' != *path))
            path++; // ����ֱ��'/'��'\0'

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
            current = find; // child�Ѿ����ڣ�
        }
        else
        {
            // �����ops��represent��һ���̳й�ϵ�����̳и��ڵ��
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
// ���ܣ��ͷŶ��󹹽���·����
// ������start -- ·����ʼ�Ķ���
// ���أ��ͷ��˵Ķ���������
// ��ע��
// ============================================================================
s32 OBJ_ReleaseTempPath(struct Object *start)
{
    s32 dels = 0;
    struct Object *parent, *current = start;

    OBJ_Lock();

    while(1)
    {
        if((current->BitFlag.temporary == 1)
                && (current->BitFlag.inuse == 0)) // ����ʹ��,������ʱ����
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
// ���ܣ���ȡ����ϵͳ�ĵ�ǰ����
// ������
// ���أ�����ϵͳ�ĵ�ǰ����
// ��ע��
// ============================================================================
struct Object * OBJ_GetCurrent(void)
{
    return (s_ptCurrentObject);
}

// ============================================================================
// ���ܣ����ö���ϵͳ�ĵ�ǰ����
// ������ob -- ����
// ���أ��ޣ�
// ��ע��
// ============================================================================
void OBJ_SetCurrent(struct Object *ob)
{
    OBJ_DutyDown(s_ptCurrentObject);
    s_ptCurrentObject = ob;
    OBJ_DutyUp(ob);
}

// ============================================================================
// ���ܣ���ȡ����ϵͳ�ĸ�����
// �������ޣ�
// ���أ�����ϵͳ�ĸ�����
// ��ע��
// ============================================================================
struct Object *OBJ_GetRoot(void)
{
    return (s_ptRootObject);
}

// ============================================================================
// ���ܣ����ö���ϵͳ�ĸ�����
// ������ob -- ����
// ���أ��ޣ�
// ��ע��
// ============================================================================
//void objsys_setroot(struct Object *ob)
//{
//    s_ptRootObject = ob;
//}

// ============================================================================
// ���ܣ��������½�һ��prev��ϵ����
// ������loc -- ����Ϊ�գ���Ϊ��ǰ���ö���
//      ObjOps -- ���������������NULL���̳и�������-1��ʹ��ȱʡ��������
//      ObjPrivate -- ����˽�����ݣ�
//      name -- �������֣�Ϊ�գ���ʹ��ϵͳĬ������
// ���أ��ɹ������󣩣�ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
struct Object *OBJ_NewPrev(struct Object *loc, fnObjOps ObjOps,
                            ptu32_t represent, const char *name)
{
    struct Object *prev;
    char *cname;

    if(!loc)
        loc = s_ptCurrentObject; // ���δָ�����ڵ㣬��ָ��ǰ�����ڵ㣻

    if(name)
    {
        if(__OBJ_CheckName(name))
          return (NULL);

        if(loc->parent)
        {
            if(OBJ_SearchChild(loc->parent, name))
              return (NULL); // child�Ѿ����ڣ�
        }

        cname = malloc(strlen(name)+1);
        if(!cname)
          return (NULL);

        strcpy(cname, name);
    }
    else
    {
        cname = (char*)__uname_obj; // ϵͳĬ������
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
//  prev->seton = (struct Object*)-1; // Ĭ�϶���֮�ϲ����������󼯺ϣ�
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
// ���ܣ�Ϊ�����½�һ��next��ϵ����
// ������loc -- ����Ϊ�գ���Ϊ��ǰ���ö���
//      ObjOps -- ���������������NULL���̳и�������-1��ʹ��ȱʡ��������
//      ObjPrivate -- ����˽�����ݣ�
//      name -- �������֡�Ϊ�գ���ʹ��ϵͳĬ������
// ���أ��ɹ������󣩣�ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
struct Object *OBJ_NewNext(struct Object *loc, fnObjOps ObjOps,
                        ptu32_t represent, const char *name)
{
    struct Object *next;
    char *cname;

    if(!loc)
        loc = s_ptCurrentObject; // ���δָ�����ڵ㣬��ָ��ǰ�����ڵ㣻

    if(name)
    {
        if(__OBJ_CheckName(name))
            return (NULL);

        if(loc->parent)
        {
            if(NULL != OBJ_SearchChild(loc->parent, name))
                return (NULL); // child�Ѿ����ڣ�
        }

        cname = malloc(strlen(name)+1);
        if(!cname)
            return (NULL);

        strcpy(cname, name);
    }
    else
    {
        cname = (char*)__uname_obj; // ϵͳĬ������
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
//  next->seton = (struct Object*)-1; // Ĭ�϶���֮�ϲ����������󼯺ϣ�
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
// ���ܣ��½��Ӷ���
// ������parent -- ���������δ���ã�Ĭ��Ϊ��ǰ���ö���
//      ObjOps -- ���󷽷�;��NULL���̳и�������-1��ʹ��ȱʡ��������
//      ObjPrivate -- ����˽�����ݣ�
//      name -- ������������ΪϵͳĬ��������ϵͳĬ��������δ���ö��������߼���
// ���أ��ɹ����½��Ӷ��󣩣�ʧ�ܣ�NULL����
// ��ע���½����Ӷ��󣬷������Ӷ�������ĩβ��
// ============================================================================
struct Object *OBJ_NewChild(struct Object *parent, fnObjOps ObjOps,
                            ptu32_t ObjPrivate, const char *name)
{
    struct Object *child;
    char *cname;

    if(!parent)
        parent = s_ptCurrentObject; // ���δָ�����ڵ㣬��ָ��ǰ�����ڵ㣻

    if(name)
    {
        if(__OBJ_CheckName(name))
            return (NULL);

        if(NULL != OBJ_SearchChild(parent, name))
            return (NULL); // child�Ѿ����ڣ�

        cname = malloc(strlen(name)+1);
        if(!cname)
            return (NULL);

        strcpy(cname, name);
    }
    else
    {
        cname = (char*)__uname_obj; // ϵͳĬ������
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
//  child->seton = (struct Object*)-1; // Ĭ�϶���֮�ϲ����������󼯺ϣ�
//  child->set = parent->set; // �����߼�����̳и���
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
// ���ܣ��½�һ�������䴦��loc�����������е��ף�
// ������ ObjOps -- ���󷽷�����NULL���̳и�������-1��ʹ��ȱʡ��������
//      ObjPrivate -- ����˽�����ݣ�
//      name -- ������������ΪϵͳĬ��������ϵͳĬ��������δ���ö��������߼���
// ���أ��ɹ����½��Ӷ��󣩣�ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
struct Object *OBJ_NewHead(struct Object *loc, fnObjOps ObjOps,
                           ptu32_t ObjPrivate, const char *name)
{
    struct Object *head;
    char *cname;

    if(!loc)
        loc = s_ptCurrentObject; // ���δָ�����ڵ㣬��ָ��ǰ�����ڵ㣻

    if(name)
    {
        if(__OBJ_CheckName(name))
            return (NULL);

        if(loc->parent)
        {
            if(NULL != OBJ_SearchChild(loc->parent, name))
                return (NULL); // child�Ѿ����ڣ�
        }

        cname = malloc(strlen(name)+1);
        if(!cname)
            return (NULL);

        strcpy(cname, name);
    }
    else
    {
        cname = (char*)__uname_obj; // ϵͳĬ������
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
        __OBJ_LIST_INS_BEFORE(loc, head); // �����ڸ�������ͽ��¶���ŵ���ǰ����ǰ
    }

    OBJ_Unlock();
    return (head);
}

#if 0
// ============================================================================
// ���ܣ��ƶ�һ��������֦����Ľڵ������Ϊ��������
// ������pParent -- �¸��ڵ㣻
//      ob����������ӽڵ㣬�������¼��ڵ㣻
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
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
// ���ܣ��������ƶ�������β��
// ������ob -- ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 obj_MoveToLast(struct Object *ob)
{
    struct Object *head;

    if((!ob)||(!ob->parent))
        return (-1);

    OBJ_Lock();

    head = ob->parent->child;
    if(head==ob) // ������ʱ����ͷ
    {
        ob->parent->child = ob->next;
    }
    else if(head->prev!=ob) // ������Ҳ���Ƕ���β
    {
        __OBJ_LIST_REMOVE(ob);
        __OBJ_LIST_INS_BEFORE(head, ob);
    }

    OBJ_Unlock();
    return (0);
}

// ============================================================================
// ���ܣ��������ƶ�������ͷ��
// ������ob -- ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
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
    else if(head!=ob)    //����ͷ�ڵ�
    {
        __OBJ_LIST_REMOVE(ob);
        __OBJ_LIST_INS_BEFORE(head, ob);
        ob->parent->child = ob;
    }

    OBJ_Unlock();
    return (0);
}

// ============================================================================
// ���ܣ�������child���뵽����loc�£���Ϊ���Ӷ���
// ������loc -- ���������
//      child -- ������Ķ���
// ���أ��ɹ���������Ķ��󣩣�ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
struct Object *OBJ_InsertToChild(struct Object *loc, struct Object *child)
{
    if((loc==NULL)||(child==NULL))
        return (NULL);

    OBJ_Lock();

    OBJ_Detach(child); // �Ͷ����ԭ�������з��룻
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
// ���ܣ�������next�����뵽����(loc)�ĺ�next��λ�ã�
// ������loc -- Ŀ�����
//      next -- Ŀ��ڵ�,������ԭ������Object�����У�
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 OBJ_InsertToNext(struct Object *loc, struct Object *next)
{

    if((loc==NULL)||(next==NULL)||(loc==next))
        return (-1);

    OBJ_Lock();

    if(loc->next!=next) // ���Ƿ��Ѿ�����Ҫ��
    {
        // �Ȱ�Ŀ��ڵ�Ӷ�����ȡ��
        if(next->next == next) // ˵���ýڵ�û���ֵܽڵ�.
        {
            next->parent->child = NULL;
        }
        else
        {
            if(next->parent->child == next)
            {   //˵���ýڵ��Ƕ���ͷ�ڵ�,��Ҫ�ı����ͷ�ڵ�
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
// ���ܣ�������prev�����뵽����(loc)��ǰ��previous��λ�ã�
// ������loc -- ������Ķ���
//      next -- �����Ķ���
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 OBJ_InsertToPrev(struct Object *loc, struct Object *prev)
{
    if((loc==NULL)||(prev==NULL)||(loc==prev))
        return (-1);

    OBJ_Lock();

    if(loc->prev!=prev) // ���Ƿ��Ѿ�����Ҫ��
    {
        //�Ȱ�Ŀ��ڵ�Ӷ�����ȡ��
        if(prev->next == prev)   //˵���ýڵ�û���ֵܽڵ�.
        {
            prev->parent->child = NULL;
        }
        else
        {
            if(prev->parent->child == prev)
            {   //˵���ýڵ��Ƕ���ͷ�ڵ�,��Ҫ�ı����ͷ�ڵ�
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
// ����: ��������Ӷ�������λ�ò��䣬���Ӷ�������Ķ���ͷ��ǰ��previous�������ƶ�һ��
// ������parent -- ������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
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
// ����: ��������Ӷ�������λ�ò��䣬���Ӷ�������Ķ���ͷ����next�������ƶ�һ��
// ������parent -- ������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
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
//���ܣ�����ĳ��֦��һ��ĩ�ҽڵ�,ĩ�ҽڵ���ָû���ӽڵ�Ľڵ�.
//������ob,��Ҫ��������֦�ĸ��ڵ�
//���أ���֦��һ��ĩ�ҽڵ�.��û���ӽڵ�ʱ������NULL
//��ע������Ҫɾ��������֦ʱ,������������,���rsc_del_node����,�������ñ�����,
//     ���ѷ��صĽڵ�ɾ��,ֱ������������NULL.��Ҫɾ��һ���ļ��л���ɾ��һ����
//     ���Ӵ��ڵ�gui����ʱ,����Ҫ�õ�ɾ��������֦�Ĳ���.
//     TODO: �ɵ��߼����ڸ�����߼��϶���������������ʹ�ã�
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
//----ȡ�ڵ��·��-------------------------------------------------------------
//����: �鿴ĳ�ڵ��Ǹ��ڵ�ĵڼ����ӽڵ㣬
//����: ob,����ȡ·���Ľڵ�
//      path������·���Ļ�����
//      limit������������
//����: path��ַ�������򷵻�NULL
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
    pos = path + limit - 2;     //�������һ�� '\0'�ַ�λ��
    while(1)
    {
        if(Current != root )
        {
            count = strlen(Current->name);
            pos -= count;
            if(pos <= path)     //path[0]ҲҪ������
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
// ���ܣ���������parent���������Ӷ�������ÿ����һ�Σ�����һ���Ӷ���(current)��ֱ����ɣ�
// ������parent -- ����
//      child -- ��ǰ�Ӷ���
// ���أ��ɹ����Ӷ��󣩣�ʧ�ܣ�NULL��
// ��ע��data�����ȳ�ʼ����
//      �����ʼ��Ϊ�����������ȫ�����г�Ա��
//      ���data��ʼ��Ϊ��ĳ���Ӷ��������Ա����ֻ�����䵽���н�����������ȫ�����г�Ա��
// ============================================================================
struct Object *OBJ_ForeachChild(struct Object *parent, struct Object *current)
{
    if((parent==NULL)||(current==NULL))
        return (NULL);

    OBJ_Lock();

    if((current==parent)||(current->parent!=parent))
    {
        current = parent->child; // data����ĳ���Ӷ���������Ӷ���ʼ��
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
// ���ܣ���������������������
// ������ancester -- ����
//      scion -- ��ǰ�������Ķ���,
// ���أ���ǰ����λ�õ���һ���ڵ�ָ��,����Ѿ��������,�򷵻�NULL.
// ��ע������Ҫ�Զ�������ĳһ����֦�������������еĽڵ���һ����ĳ�ֲ���ʱ,��
//      �������ñ�����,��һ�ε���current_node = parent_node,���current_node
//      = �ϴη���ֵ��ֱ�����ؿ�.
//      �������������ӡ������....��˳������������ֱϵ��������ϵ��ȷ����������
//      �ڵ㶼�ܹ����ʵ�������Է���˳��������Ҫ�󣬲���ʹ�ñ�������
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
            {   //�ӽڵ�ǿ�,�����ӽڵ�
                result = current_copy->child;
                break;
            }
            else if(current_copy->next != current_copy->parent->child)
            {   //�ӽڵ��,�����ڵ㲻�Ƕ���β�ڵ�,����next�ڵ�
                result = current_copy->next;
                break;
            }
            else
            {   //���ӽڵ�,�ұ��ڵ��Ѿ��Ƕ���β�ڵ�,��Ҫ�жϵ�ǰ���ڵ��next�ڵ�
                current_copy = current_copy->parent;
                up = true;
            }
        }while(current_copy != ancester);
    }

    OBJ_Unlock();
    return (result);
}

// ============================================================================
// ���ܣ������ֵܣ�ͬ�������ϵģ�����
// ������ob -- ����;.
//      name -- ��Ҫ�����Ķ�����;
// ���أ��ɹ�����Ϊname�Ķ��󣩣�ʧ�ܣ�NULL����
// ��ע��
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
// ���ܣ������Ӷ���
// ������parent -- ����
//      name -- ��Ҫ�����Ķ�����
// ���أ��ɹ�����Ϊname�Ķ��󣩣�ʧ�ܣ�NULL����
// ��ע��
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
// ���ܣ����Ҷ�����������
// ������ancester -- ����
//      name -- ����ҵĶ�������
// ���أ��ɹ�����Ϊname�Ķ��󣩣�ʧ�ܣ�NULL����
// ��ע��
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
// ���ܣ��Ӷ���start����ʼ����·�����Ҷ���
// ������start -- ·������ʼ����
//      path -- �����Ķ���·����
// ���أ��ɹ���·�������Ķ��󣩣�ʧ�ܣ�NULL����
// ��ע��
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
        Base = OBJ_GetRoot();          // ����·��
    }
    current = Base;
    while(Base)
    {
        while(('/' == *path) || ('\\' == *path))
            path++; // ���˶����'/'
        if('\0' == *path)
        {
            break; // ����·������
        }

        if('.' == path[0])
        {
            if('.' == path[1])  //���Ƿ�Ҫ������һ��Ŀ¼
            {
                if(Base == OBJ_GetRoot())
                {
                    result = NULL;
                    return NULL;
                }
                Base = OBJ_GetParent(Base);        // ".."�ַ�����ʾ��һ��Ŀ¼
                result = Base;
                current = Base;
                if(('\\' == path[2])||('/' == path[2]))
                {
                    path++;
                    continue;
                }
                else if('\0' != path[2])        //���path·��ƥ��
                {
                    return current;
                }
                else                            //".."���ǺϷ��ķָ������Ƿ�
                {
                    return NULL;
                }
            }
            else if(('\\' == path[1])||('/' == path[1]))
            {
                path++;
                continue;
            }
            else if('\0' != path[1])        //���path·��ƥ��
            {
                return result;
            }
            else                            //"."���ǺϷ��ķָ������Ƿ�
            {
                return NULL;
            }
        }

        while(current)
        {
            current = OBJ_ForeachChild(Base, current);
            if(current == NULL)         //ƥ�����
            {
                Base = NULL;
                break;
            }
            ResultName = (char*)OBJ_GetName(current);
            i = strlen(ResultName);
            if(memcmp(ResultName, path, i) == 0)
            {
                if('\0' == path[i])     //ƥ�������current��ƥ����
                {
                    result = current;
                    Base = NULL;
                    break;
                }
                else if(('\\' == path[i]) || ('/' == path[i]))
                {
                    result = current;
                    path += i;
                    Base = current;     // ƥ�䵱ǰ�ڵ㣬����ƥ���ӽڵ�
                    break;
                }
                else    // ��ǰ����ƥ�䣬���������ֵܽڵ�
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
        for(i=0;i<CN_OBJ_NAME_LIMIT;i++) // ��������������
        {
            if(ObjName[i] == path_name[i])
            {
                if(path_name[i] == '\0') // ƥ�䣬��path����
                {
                    result = current;
                    end = true;
                }
            }
            else if( (ObjName[i] == '\0')
                    && ((path_name[i] == '\\') || (path_name[i] == '/')))
            {   //�������ֽ�����path�������ַָ���
                if(path_name[i+1] == '\0')      //ƥ�䣬��path����
                {
                    result = current;
                    end = true;
                }
                else // ƥ�䣬��pathδ����,���������ӽڵ�
                {
                    path_name += i+1;
                    current = current->child;
                    if(current == NULL) // û���ӽڵ��ˣ���pathδ����
                    {
                        result = NULL;
                        end = true;
                        break;
                    }
                }
            }
            else // �ַ��������
            {
                current = current->next;
                if(current == current->parent->child)
                {
                    //���������нڵ��Ѿ�������,û���ҵ�ƥ���.
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
            path++; // ���˶����'/'

        if('\0' == *path)
            break; // ����·������

        if(('.' == path[0]) && ('.' == path[1])
           && (('\\' == path[2]) || ('/' == path[2]) || ('\0' == path[2])))
        {
            current = OBJ_GetParent(current);
            path += 2; // ".."�ַ�����ʾ��һ��
            continue;
        }
        else if(('.' == path[0])
                && (('\\' == path[1]) || ('/' == path[1]) || ('\0' == path[1])))
        {
            path += 1; // "."�ַ�����ʾ��ǰ��
            continue;
        }

        head = current->child; // �����ӽڵ�
        if(!head)
            break;

        segment = (char*)OBJ_GetName(head);
        for(i=0; i<CN_OBJ_NAME_LIMIT; i++) // ��������������
        {
            if('\0' == segment[i])
            {
                if('\0' == path[i])
                {
                    path += i; // pathΪ'\0'
                    current = head;
                    goto __SEARCH_DONE; // ƥ�������������������
                }
                else if(('\\' == path[i]) || ('/' == path[i]))
                {
                    path += i;
                    current = head;
                    goto __SEARCH_NEXT; // ƥ�䵱ǰ�ڵ㣬����ƥ��һ��һ���ڵ�
                }
                else
                {
                    break; // ���뵱ǰ�ڵ���ڲ�ƥ����ַ�
                }
            }
            else if(segment[i] == path[i])
            {
                continue;
            }
            else
            {
                break; // ���뵱ǰ�ڵ���ڲ�ƥ����ַ�
            }
        }

        __OBJ_LIST_FOR_EACH_BROTHERS(current, head)
        {
            segment = (char*)OBJ_GetName(current);
            for(i=0; i<CN_OBJ_NAME_LIMIT; i++) // ��������������
            {
                if('\0' == segment[i])
                {
                    if('\0' == path[i])
                    {
                        path += i; // pathΪ'\0'
                        goto __SEARCH_DONE; // ƥ�������������������
                    }
                    else if(('\\' == path[i]) || ('/' == path[i]))
                    {
                        path += i;
                        goto __SEARCH_NEXT; // ƥ�䵱ǰ�ڵ㣬����ƥ��һ��һ���ڵ�
                    }
                    else
                    {
                        break; // ���뵱ǰ�ڵ���ڲ�ƥ����ַ�
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

        current = OBJ_GetParent(current); // ��ǰ�ڵ���·����ƥ�䣬�˻������Լ��ڵ�
        break; // δ�ҵ��ڵ�
    }

__SEARCH_DONE:
   OBJ_Unlock();
   return (current);
#endif
}
#endif

//-----------------------------------------------------------------------------
//����: ���û�������,ϵͳ��ǰ����·��
//����:
//����: 0 -- �ɹ�; -1 -- �����Ҳ���;
//��ע:
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
    hdl = (struct objhandle*)(dir->__fd); // Ŀ¼��������
    ob = hdl->HostObj;      // Ŀ¼�Ľڵ�
    if(NULL == ob)
        res = -1;

    OBJ_SetCurrent(ob);
    closedir(dir);// �ر�Ŀ¼
    return res;
}
//-----------------------------------------------------------------------------
//����: ��ȡ��ǰ����·���ַ�������(��������)
//����:
//����: ����-- �ɹ�; �� -- ʧ�ܣ�δ���õ�ǰ����·��;
//��ע:
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
//����: ��ȡ��ǰ����·��
//����: Buf -- ��ǰ����·��
//      BufSize -- ·�����ȣ������ַ���������
//����: 0 -- �ɹ�; -1 -- δ���õ�ǰ����·��; -2 -- ��������; -3 -- �ڴ治��;
//      -4 -- ����;
//��ע:
//-----------------------------------------------------------------------------
s32 OBJ_CurWorkPath(char *Buf, u32 BufSize)
{
    struct Object *Obj;
    u32 ObjNameLen, Offset;
    char *PathTemp=NULL, *ObjName;
    s32 Ret = 0;

    OBJ_Lock();// ������(��ֹ�������̵�ǰ����·��������)

    Offset = OBJ_CurWorkPathLen();
    if((NULL == Buf) || (BufSize < Offset))
    {
        Ret = -2; // ��������
        goto FAIL;
    }

    PathTemp = (char*)malloc(Offset + 1);
    if(NULL == PathTemp)
    {
        Ret = -3; // �ڴ治��
        goto FAIL;
    }
    memset(PathTemp, 0, Offset + 1);
    Offset -= 1;
    PathTemp[Offset] = '\0'; // ·���Ľ�����
    Obj = s_ptCurrentObject; // ·�����һ���ڵ�

    for(;;)
    {
        ObjName = (char *)OBJ_GetName(Obj);
        ObjNameLen = strlen(ObjName);
        Offset = Offset - ObjNameLen;
        memcpy((PathTemp + Offset), ObjName, ObjNameLen);
        if(Offset) // ȥ���������
        {
            Offset--;
            PathTemp[Offset] = '/'; // ·��֮��ķָ������Ǹ�
        }

        if(0 == Offset)
        {
            if(PathTemp[Offset] == '\0')
                PathTemp[Offset] = '/';
            strcpy(Buf, PathTemp);
            break; // ����
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
    OBJ_Unlock(); // ������
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
            printf("%s �޷���ʾ\r\n", param);
    }
    else
        printf("%s ������\r\n",param);
    return true;
}
ADD_TO_ROUTINE_SHELL(catobj,OBJ_Show,"��ʾһ���������ݣ������������ļ���COMMAND:cat + ����·�� + enter.");

