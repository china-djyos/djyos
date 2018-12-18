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
#define DEFAULT_LOCK_TIMEOUT            (360*1000*mS) // һ��Сʱ������δFOREVER��Ϊ�˷�ֹ�߼�������
#define BASIC_OBJECTS                   32

static inline s32 __checkname(const char *name);
static inline s32 __lock_objsys(void);
static inline void __unlock_objsys(void);
static inline s32 __init_obj_struct_pool(void);
static inline struct obj *__newobj(void);
static inline void __freeobj(struct obj *ob);

static struct MemCellPool __basic_obj_pool;
static struct obj __basic_obj[BASIC_OBJECTS];
static struct obj *__root_obj, *__current_obj; // �߼��ϣ�Ӧ����ÿһ������һ����
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
// ���ܣ�Ϊ�����ṩһ��Ĭ�ϲ�������������ֻ��Ҫ����һ�����󣬲�����Ҫ��Ϊ�ļ�����
//      ʱ���Ͳ���Ҫ�ṩ Operation ��������ʱ��ϵͳ���Զ��Ѷ����������ָ�����
// ��������û�����塣
// ���أ��̶����� OBJUNSUPPORTED
// ��ע��
// ============================================================================
static inline ptu32_t __objsys_default_ops(enum objops ops, ptu32_t p_hdl, ptu32_t args, ...)
{
    return (OBJUNSUPPORTED);
}

// ============================================================================
// ���ܣ���������ϵͳ��
// ������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
static inline s32 __lock_objsys(void)
{
    if(TRUE==Lock_MutexPend(&__objsys_mutex, DEFAULT_LOCK_TIMEOUT))
        return (0);

    return (-1);
}

// ============================================================================
// ���ܣ���������ϵͳ��
// ������
// ���أ�
// ��ע��
// ============================================================================
static inline void __unlock_objsys(void)
{
    Lock_MutexPost(&__objsys_mutex);
}

// ============================================================================
// ���ܣ���ʼ�������
// ������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��POOL�������ǿ����Զ���չ
// ============================================================================
static inline s32 __init_obj_struct_pool(void)
{
    if(Mb_CreatePool_s(&__basic_obj_pool, __basic_obj,
            BASIC_OBJECTS, sizeof(struct obj), 16, 16384, "object"))
        return (0);

    return (-1);
}

// ============================================================================
// ���ܣ������¶���
// �������ޣ�
// ���أ��ɹ����루�¶��󣩣�ʧ�ܣ�NULL����
// ��ע��
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
// ���ܣ��ͷŶ���
// ������ob -- ����
// ���أ��ޣ�
// ��ע��
// ============================================================================
static inline void __freeobj(struct obj *ob)
{
    Mb_Free(&__basic_obj_pool, ob);
    __objsys_statics.fress++;
    __objsys_statics.alives--;
}

// ============================================================================
// ���ܣ����������ĺϷ��ԣ�
// ������name -- ��������
// ���أ��Ϸ���0�����Ƿ���0����
// ��ע��
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
// ���ܣ���ȡ�����ֵ��
// ������ob -- ����
// ���أ������ֵ��
// ��ע��
// ============================================================================
inline ptu32_t obj_GetPrivate(struct obj *ob)
{
    if(ob)
        return (ob->ObjPrivate);

    return (0);
}

// ============================================================================
// ���ܣ���ȡ����ļ��ϵ㣻
// ������ob -- ����
// ���أ����ϵ㣻
// ��ע��
// ============================================================================
inline struct obj *obj_set(struct obj *ob)
{
    if(ob)
        return (ob->set);

    return (NULL);
}

// ============================================================================
// ���ܣ���ȡ��������ù�ϵ��
// ������ob -- ����
// ���أ���������ù�ϵ��
// ��ע��
// ============================================================================
inline list_t *obj_quote(struct obj *ob)
{
    if(dListIsEmpty(&ob->handles))
        return (NULL);

    return (dListGetAfter(&ob->handles));
}

// ============================================================================
// ���ܣ���ȡ���������
// ������ob -- ����
// ���أ��ɹ��������������ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
inline fnObjOps obj_ops(struct obj *ob)
{
    if(!ob)
        return (NULL);

    return (ob->ops);
}

// ============================================================================
// ���ܣ���ȡ��������
// ������ob -- ����
// ���أ��ɹ�������������ʧ�ܣ�NULL����
// ע�⣺
// ============================================================================
inline const char *obj_name(struct obj *ob)
{
    if(!ob)
        return (NULL);

    return ((const char*)ob->name);
}

// ============================================================================
// ����: ��ȡ����ĸ���ϵ����
// ����: ob -- ����
// ����: �ɹ������ڵ㣩��ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
inline struct obj *obj_parent(struct obj *ob)
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
inline struct obj *obj_child(struct obj *ob)
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
inline struct obj *obj_prev(struct obj *ob)
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
inline struct obj *obj_next(struct obj *ob)
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
inline struct obj *obj_head(struct obj *ob)
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
            break; // ���û�и��ڵ㣬Ҳ��Ϊ�ǵ�ͷ�ˣ�
        }

        if(level++>256) // ��ֹ���ݱ��ƻ���������
        {
            __unlock_objsys();
            return (-1);
        }
    }

    __unlock_objsys();
    return (level);
}

// ============================================================================
// ���ܣ���ȡ������ͬ�������е�λ�ã�
// ������ob -- ����
// ���أ������λ�ã�����-1����
// ��ע������ͷΪ0������ĵڶ�����ԱΪ1��......
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
// ���ܣ����ö����ֵ��
// ������ob -- ����
//      represent -- �����ֵ��
// ���أ��ޣ�
// ��ע��
// ============================================================================
inline void obj_setval(struct obj *ob, ptu32_t Private)
{
    if(ob)
        ob->ObjPrivate = Private;
}

// ============================================================================
// ���ܣ����ö��������
// ������ob -- ����
//      ops -- ���������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
inline s32 obj_setops(struct obj *ob, fnObjOps ops)
{
    if(!ob)
        return (-1);

    ob->ops = ops;
    return (0);
}

// ============================================================================
// ���ܣ����ö�������ù�ϵ��
// ������ob -- ����
//      quote -- ���ù�ϵ��
// ���أ��ޣ�
// ��ע��
// ============================================================================
inline void obj_setquote(struct obj *ob, list_t *quote)
{
    if(ob&&quote)
        dListInsertBefore(&ob->handles, quote);
}

// ============================================================================
// ���ܣ���ʼ������ϵͳ��
// ��������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
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
// ���ܣ������Ƿ���ͬ�������ͷ��Ա��
// ������ob -- ����
// ���أ��ǣ�1������0����
// ��ע��
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
// ���ܣ������Ƿ���ͬ�������β��Ա��
// ������ob -- ����
// ���أ��ǣ�1������0����
// ��ע��
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
// ���ܣ������Ƿ�ʹ���У�
// ������ob -- ����
// ���أ��ļ�ʹ���У�1�����ļ�δʹ�ã�0��
// ��ע��
// ============================================================================
inline s32 obj_isonduty(struct obj *ob)
{
    if(!ob)
        return (0);

    if((!dListIsEmpty(&ob->handles)) // δ������
       ||(ob->rights.b.muses)
       ||(ob->child))
        return (1);

    return (0);
}

// ============================================================================
// ���ܣ������Ƿ��Ƕ��󼯺ϵ㣻
// ������ob -- ����
// ���أ��ǣ�1������0����
// ��ע��
// ============================================================================
inline s32 obj_isset(struct obj *ob)
{
    if(!ob)
        return (0);

    return (ob==ob->set);
}

// ============================================================================
// ���ܣ������Ƿ����ã�
// ������ob -- ����
// ���أ��ǣ�1������0����
// ��ע��
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
// ���ܣ������Ƿ�����ʱ�����ģ�
// ������ob -- ����
// ���أ��ǣ�1������0����
// ��ע�����ڹ���·���ȣ�
// ============================================================================
inline s32 obj_istmp(struct obj *ob)
{
    if((!obj_isset(ob)) // ���Ƕ��󼯺ϵ㣻
       &&(!ob->ObjPrivate)) // ���������ݣ�
        return (1);

    return (0);
}

// ============================================================================
// ���ܣ��Ƿ��������µĶ��󼯺ϣ�
// ������ob -- ����
// ���أ�����1����������0����
// ��ע��
// ============================================================================
inline s32 obj_testset(struct obj *ob)
{
    if(ob&&(-1==(u32)(ob->seton)))
        return (0);

    return (1);
}

// ============================================================================
// ���ܣ������ڶ����Ͻ����µĶ��󼯺ϣ�
// ������ob -- ����
// ���أ��ɹ�����0������������-1����
// ��ע��
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
// ���ܣ�������ı������Ͻ����µĶ��󼯺ϣ�
// ������ob -- ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
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
// ���ܣ�������תΪ���󼯺�;
// ������ob -- ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
inline s32 obj_change2set(struct obj *ob)
{
    if((!ob)
       ||(!obj_testset(ob)) // ���������½�����
       ||(obj_isset(ob))) // �����Ѿ��Ƕ��󼯺�
        return (-1);

    ob->set = ob;
    ob->seton = ob;
    return (0);
}


// ============================================================================
// ���ܣ�������������ֹ�·��ʣ�
// ������ob -- ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 obj_lock(struct obj *ob)
{
    if(__lock_objsys())
        return (-1);

    if(ob->rights.b.lock) // �Ѿ�����
    {
        __unlock_objsys();
        return (-1);
    }

    ob->rights.b.lock = 1;
    __unlock_objsys();
    return (0);
}

// ============================================================================
// ���ܣ����������
// ������ob -- ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��INLINE
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
// ���ܣ����������ü�����
// ������ob -- ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע�������ú��򲻿�ɾ��
// ============================================================================
s32 obj_up(struct obj *ob)
{
    if(!ob)
        return (-1);

    if(__lock_objsys())
        return (-1);

    if((ob->rights.b.lock)||(-1==ob->rights.b.muses)) // ���󱻽�ֹ���ʣ�����ɾ����������ͬ��ʹ���ߴﵽ���ޣ�
    {
        __unlock_objsys();
        return (-1);
    }

    ob->rights.b.muses++;
    __unlock_objsys();
    return (0);
}

// ============================================================================
// ���ܣ���������ü�����
// ������ob -- ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע�������ú��򲻿�ɾ��
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
// ���ܣ�ɾ������
// ������ob -- ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
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
        return (-1); // ����ʹ��
    }

    if(ob->next==ob) // ˵���ýڵ�û���ֵܽڵ�.
    {
        ob->parent->child = NULL;
    }
    else
    {
        if(ob->parent->child==ob)
            ob->parent->child = ob->next; // ˵���ýڵ��Ƕ���ͷ�ڵ�,��Ҫ�ı����ͷ�ڵ�

        __OBJ_LIST_REMOVE(ob);
    }

    __freeobj(ob);
    __unlock_objsys();
    return (0);
}

// ============================================================================
// ���ܣ�����������չ���Ӷ���ȣ��Ӷ�������ϵ�Ϸ��룻
// ������branch -- ��ɾ���ķ�֧;
// ���أ��ɹ���������Ķ����֧��;ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
struct obj *obj_detach(struct obj *branch)
{
    struct obj *ob;

    if(!branch)
        return (NULL);

    if(__lock_objsys())
        return (NULL);

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
    __unlock_objsys();
    return (ob);
}

// ============================================================================
// ���ܣ����������ĺϷ��ԣ�
// ������name -- ��������
// ���أ��Ϸ���0�����Ƿ���-1����
// ��ע�������򣺲��������'/'��'\'�ַ������Ȳ��������涨��
// ============================================================================
inline s32 obj_checkname(const char *name)
{
    if(name)
        return (-1);

    return (__checkname(name));
}

// ============================================================================
// ���ܣ�ƥ��·��
// ������pPath -- ��ƥ���·����
//      left -- ��ȫƥ�䣬ΪNULL������ȫƥ�䣬�򷵻ز�ƥ�䲿�֣���֤����'/'��ͷ����
//      full -- ������İ�װ�㿪ʼ��ƥ��·����ͬһ���ļ�ϵͳ�µ�·������
// ���أ�ƥ��·�����ܼ������Ľ�������
// ��ע��
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
        current = objsys_root(); // ����·��
    }
    else
    {
        current = objsys_current();// �����������·��
    }

    //if(GROUP_POINT == __OBJ_Type(current)) // ��װ��
    if(obj_isset(current))
        sfull = path;

    while(current)
    {
        while('/' == *path)
            path++; // ���˶����'/'

        if('\0' == *path)
            break; // ����·������

        if(('.' == path[0]) && ('.' == path[1]) &&
            (('\\' == path[3]) || ('/' == path[3]) || ('\0' == path[3])))
        {
            //if(GROUP_POINT == __OBJ_Type(current))
            if(obj_isset(current))
                sfull = NULL; // �Ӱ�װ�����ϼ�������ȫ·���߼���ʧЧ��

            current = obj_parent(current);
            path += 2; // ".."�ַ�����ʾ��һ��
            continue;
        }
        else if(('.' == path[0]) &&
            (('\\' == path[1]) || ('/' == path[1]) || ('\0' == path[1])))
        {
            path += 1; // "."�ַ�����ʾ��ǰ��
            continue;
        }

        head = current->child; // �����ӽڵ�
        if(!head)
            break;

        segment = (char*)obj_name(head);
        for(i=0; i<CN_OBJ_NAME_LIMIT; i++) // ��������������
        {
            if('\0' == segment[i])
            {
                if('\0' == path[i])
                {
                    path += i; // pathΪ'\0'
                    current = head;
                    goto __MATCH_DONE; // ƥ�������������������
                }
                else if(('\\' == path[i]) || ('/' == path[i]))
                {
                    path += i;
                    current = head;
                    goto __MATCH_NEXT; // ƥ�䵱ǰ�ڵ㣬����ƥ��һ��һ���ڵ�
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
            segment = (char*)obj_name(current);
            for(i=0; i<CN_OBJ_NAME_LIMIT; i++) // ��������������
            {
                if('\0' == segment[i])
                {
                    if('\0' == path[i])
                    {
                        path += i; // pathΪ'\0'
                        goto __MATCH_DONE; // ƥ�������������������
                    }
                    else if(('\\' == path[i]) || ('/' == path[i]))
                    {
                        path += i;
                        goto __MATCH_NEXT; // ƥ�䵱ǰ�ڵ㣬����ƥ��һ��һ���ڵ�
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

        current = obj_parent(current); // ��ǰ�ڵ���·����ƥ�䣬�˻������Լ��ڵ�
        break; // δ�ҵ��ڵ�

__MATCH_NEXT:
        if(obj_isset(current))
            sfull = path;
    }

__MATCH_DONE:
    if(full)
    {
        if(obj_isset(current))
            *full = path; // // ƥ�����ĩ�ڵ��ǰ�װ��
        else
            *full = sfull;
    }

    if(left)
    {
        if('\0' == *path)
            *left = NULL;
        else
            *left = path; // �߼����豣֤������'/'��ͷ
    }

    __unlock_objsys();
    return (current);
}

// ============================================================================
// ���ܣ��������ڶ����·��
// ������begin -- ��ʼ����ڵ㣻
//      path -- �轨������ʱ·����
// ���أ�Ŀ�����
// ��ע�������½�ʱ���ж��������жϣ�
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
            path++; // ���˶����'/'

        segst = path;
        while(('/' != *path) && ('\\' != *path) && ('\0' != *path))
            path++; // ����ֱ��'/'��'\0'

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
            current = find; // child�Ѿ����ڣ�
            free(name);
        }
        else
        {
            // �����ops��represent��һ���̳й�ϵ�����̳и��ڵ��
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
// ���ܣ��ͷŶ��󹹽���·����
// ������start -- ·����ʼ�Ķ���
// ���أ��ͷ��˵Ķ���������
// ��ע��
// ============================================================================
s32 obj_releasepath(struct obj *start)
{
    s32 dels = 0;
    struct obj *parent, *current = start;

    if(__lock_objsys())
        return (0);

    while(1)
    {
        if((obj_istmp(current)) // ��ʱ����
           &&(!obj_isquoted(current)) // δ������
           &&(!current->rights.b.muses)) // ����ʹ��
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
// ���ܣ���һ�����󼯺��������
// ������ob -- ���󣨱��滻����
//      ops -- ���󼯺ϵĲ�����
//      val -- ���󼯺ϵ�ֵ��
// ���أ��ɹ������󼯺ϣ���ʧ�ܣ�NULL����
// ��ע�������¶��󣨼��ϵ㣩����󣬾ͻ�Ӷ���������ʧ�������ĽӿھͲ�����ҵ�����
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

    if(obj_testset(ob)) // �Ƿ�������set
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
                if(++i==0) // ��ֹ���ݱ��ƻ��������ѭ������ԭ��Ŀǰ���߼�������ͬһ���ڵ����½���ô��ζ��󼯺ϣ���
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
// ���ܣ��ͷŶ��󼯺ϣ�
// ������setob -- ���󼯺ϣ�
// ���أ����ع�����������Ķ���
// ��ע��
// ============================================================================
struct obj *obj_destoryset(struct obj *setob)
{
    struct obj *ob = NULL;

    if(!obj_isset(setob))
        return (NULL); // ����Ĳ������Ƕ��󼯺ϵ㣻

    if(__lock_objsys())
        return (NULL);

    if(setob->seton==setob) // ���󼯺Ͼ��Ƕ�����
    {
        if(dListIsEmpty(&setob->handles)&&(!setob->child))
            __freeobj(setob);
    }
    else // ���󼯺Ϲ�������������֮��
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
            __freeobj(setob); // ԭ�ڵ�û�б����õ������ͷ�
    }

    __unlock_objsys();
    return (ob);
}

// ============================================================================
// ���ܣ���ȡ����ϵͳ�ĵ�ǰ����
// ������
// ���أ�����ϵͳ�ĵ�ǰ����
// ��ע��
// ============================================================================
struct obj * objsys_current(void)
{
    return (__current_obj);
}

// ============================================================================
// ���ܣ����ö���ϵͳ�ĵ�ǰ����
// ������ob -- ����
// ���أ��ޣ�
// ��ע��
// ============================================================================
void objsys_setcurrent(struct obj *ob)
{
    __current_obj = ob;
}

// ============================================================================
// ���ܣ���ȡ����ϵͳ�ĸ�����
// �������ޣ�
// ���أ�����ϵͳ�ĸ�����
// ��ע��
// ============================================================================
struct obj *objsys_root(void)
{
    return (__root_obj);
}

// ============================================================================
// ���ܣ����ö���ϵͳ�ĸ�����
// ������ob -- ����
// ���أ��ޣ�
// ��ע��
// ============================================================================
void objsys_setroot(struct obj *ob)
{
    __root_obj = ob;
}

// ============================================================================
// ���ܣ��������½�һ��prev��ϵ����
// ������loc -- ����Ϊ�գ���Ϊ��ǰ���ö���
//      ops -- ���������������NULL���̳и�������-1��ʹ��ȱʡ��������
//      rights -- ����Ȩ�ޣ�Ϊ�㣬��̳У�
//      represent -- �����ʾ��
//      name -- �������֣�Ϊ�գ���ʹ��ϵͳĬ������
// ���أ��ɹ������󣩣�ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
struct obj *obj_newprev(struct obj *loc, fnObjOps ops, u32 rights,
                            ptu32_t represent, const char *name)
{
    struct obj *prev;
    char *cname;

    if(!loc)
        loc = __current_obj; // ���δָ�����ڵ㣬��ָ��ǰ�����ڵ㣻

    if(name)
    {
        if(__checkname(name))
          return (NULL);

        if(loc->parent)
        {
            if(obj_search_child(loc->parent, name))
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
    prev->seton = (struct obj*)-1; // Ĭ�϶���֮�ϲ����������󼯺ϣ�
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
// ���ܣ�Ϊ�����½�һ��next��ϵ����
// ������loc -- ����Ϊ�գ���Ϊ��ǰ���ö���
//      ops -- ���������������NULL���̳и�������-1��ʹ��ȱʡ��������
//      rights -- ����Ȩ�ޣ�Ϊ�㣬��̳У�
//      represent -- �����ʾ��
//      name -- �������֡�Ϊ�գ���ʹ��ϵͳĬ������
// ���أ��ɹ������󣩣�ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
struct obj *obj_newnext(struct obj *loc, fnObjOps ops, u32 rights,
                        ptu32_t represent, const char *name)
{
    struct obj *next;
    char *cname;

    if(!loc)
        loc = __current_obj; // ���δָ�����ڵ㣬��ָ��ǰ�����ڵ㣻

    if(name)
    {
        if(__checkname(name))
            return (NULL);

        if(loc->parent)
        {
            if(NULL != obj_search_child(loc->parent, name))
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
    next->seton = (struct obj*)-1; // Ĭ�϶���֮�ϲ����������󼯺ϣ�
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
// ���ܣ��½��Ӷ���
// ������parent -- ���������δ���ã�Ĭ��Ϊ��ǰ���ö���
//      ops -- ���󷽷�;��NULL���̳и�������-1��ʹ��ȱʡ��������
//      rights -- ����Ȩ��
//      represent -- �������ݣ���ʾ����
//      name -- ������������ΪϵͳĬ��������ϵͳĬ��������δ���ö��������߼���
// ���أ��ɹ����½��Ӷ��󣩣�ʧ�ܣ�NULL����
// ��ע���½����Ӷ��󣬷������Ӷ�������ĩβ��
// ============================================================================
struct obj *obj_newchild(struct obj *parent, fnObjOps ops, u32 rights,
                            ptu32_t ObjPrivate, const char *name)
{
    struct obj *child;
    char *cname;

    if(!parent)
        parent = __current_obj; // ���δָ�����ڵ㣬��ָ��ǰ�����ڵ㣻

    if(name)
    {
        if(__checkname(name))
            return (NULL);

        if(NULL != obj_search_child(parent, name))
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
    child->seton = (struct obj*)-1; // Ĭ�϶���֮�ϲ����������󼯺ϣ�
    child->set = parent->set; // �����߼�����̳и���
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
// ���ܣ��½�һ�������䴦��loc�����������е��ף�
// ������ ops -- ���󷽷�����NULL���̳и�������-1��ʹ��ȱʡ��������
//      rights -- ����Ȩ��
//      represent -- �������ݣ���ʾ����
//      name -- ������������ΪϵͳĬ��������ϵͳĬ��������δ���ö��������߼���
// ���أ��ɹ����½��Ӷ��󣩣�ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
struct obj *obj_newhead(struct obj *loc, fnObjOps ops, u32 rights,
                           ptu32_t ObjPrivate, const char *name)
{
    struct obj *head;
    char *cname;

    if(!loc)
        loc = __current_obj; // ���δָ�����ڵ㣬��ָ��ǰ�����ڵ㣻

    if(name)
    {
        if(__checkname(name))
            return (NULL);

        if(loc->parent)
        {
            if(NULL != obj_search_child(loc->parent, name))
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
        __OBJ_LIST_INS_BEFORE(loc, head); // �����ڸ�������ͽ��¶���ŵ���ǰ����ǰ
    }

    __unlock_objsys();
    return (head);
}

// ============================================================================
// ���ܣ�������child���뵽����loc��ϵ����Ϊ���ӹ�ϵ��
// ������loc -- ���������
//      child -- ������Ķ���
// ���أ��ɹ���������Ķ��󣩣�ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
struct obj *obj_insert2child(struct obj *loc, struct obj *child)
{
    if((loc==NULL)||(child==NULL))
        return (NULL);

    if(__lock_objsys())
        return (NULL);

    obj_detach(child); // �Ͷ����ԭ��ϵ���з��룻
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
// ���ܣ��ƶ�һ��������֦����Ľڵ������Ϊ��������
// ������pParent -- �¸��ڵ㣻
//      ob����������ӽڵ㣬�������¼��ڵ㣻
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
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
// ���ܣ��������ƶ�������β��
// ������ob -- ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 obj_move2last(struct obj *ob)
{
    struct obj *head;

    if((!ob)||(!ob->parent))
        return (-1);

    if(__lock_objsys())
        return (-1);

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

    __unlock_objsys();
    return (0);
}

// ============================================================================
// ���ܣ��������ƶ�������ͷ��
// ������ob -- ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
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
    else if(head!=ob)    //����ͷ�ڵ�
    {
        __OBJ_LIST_REMOVE(ob);
        __OBJ_LIST_INS_BEFORE(head, ob);
        ob->parent->child = ob;
    }

    __unlock_objsys();
    return (0);
}

// ============================================================================
// ���ܣ�������next�����뵽����(loc)�ĺ�next��λ�ã�
// ������loc -- Ŀ�����
//      next -- Ŀ��ڵ�,������ԭ������Object�����У�
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 obj_insert2next(struct obj *loc, struct obj *next)
{

    if((loc==NULL)||(next==NULL)||(loc==next))
        return (-1);

    if(__lock_objsys())
        return (-1);

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

    __unlock_objsys();
    return (0);
}

// ============================================================================
// ���ܣ�������prev�����뵽����(loc)��ǰ��previous��λ�ã�
// ������loc -- ������Ķ���
//      next -- �����Ķ���
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 obj_insert2prev(struct obj *loc, struct obj *prev)
{
    if((loc==NULL)||(prev==NULL)||(loc==prev))
        return (-1);

    if(__lock_objsys())
        return (-1);

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

    __unlock_objsys();
    return (0);
}

// ============================================================================
// ����: ��������Ӷ�������λ�ò��䣬���Ӷ�������Ķ���ͷ��ǰ��previous�������ƶ�һ��
// ������parent -- ������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
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
// ����: ��������Ӷ�������λ�ò��䣬���Ӷ�������Ķ���ͷ����next�������ƶ�һ��
// ������parent -- ������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
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
//���ܣ�����ĳ��֦��һ��ĩ�ҽڵ�,ĩ�ҽڵ���ָû���ӽڵ�Ľڵ�.
//������ob,��Ҫ��������֦�ĸ��ڵ�
//���أ���֦��һ��ĩ�ҽڵ�.��û���ӽڵ�ʱ������NULL
//��ע������Ҫɾ��������֦ʱ,������������,���rsc_del_node����,�������ñ�����,
//     ���ѷ��صĽڵ�ɾ��,ֱ������������NULL.��Ҫɾ��һ���ļ��л���ɾ��һ����
//     ���Ӵ��ڵ�gui����ʱ,����Ҫ�õ�ɾ��������֦�Ĳ���.
//     TODO: �ɵ��߼����ڸ�����߼��϶���������������ʹ�ã�
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
//----ȡ�ڵ��·��-------------------------------------------------------------
//����: �鿴ĳ�ڵ��Ǹ��ڵ�ĵڼ����ӽڵ㣬
//����: ob,����ȡ·���Ľڵ�
//      path������·���Ļ�����
//      limit������������
//����: path��ַ�������򷵻�NULL
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
struct obj *obj_foreach_child(struct obj *parent, struct obj *child)
{
    if((parent==NULL)||(child==NULL))
        return (NULL);

    if(__lock_objsys())
       return (NULL);

    if((child==parent)||(child->parent!=parent))
    {
        child = parent->child; // data����ĳ���Ӷ���������Ӷ���ʼ��
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
// ���ܣ���������������Ӷ�������չ���󣨶�����Ӷ����ϵ����չ�����ж��󣩣�
// ������ancester -- ����
//      scion -- ��ǰ�������Ķ���,
// ���أ���ǰ����λ�õ���һ���ڵ�ָ��,����Ѿ��������,�򷵻�NULL.
// ��ע������Ҫ�Զ�������ĳһ����֦�������������еĽڵ���һ����ĳ�ֲ���ʱ,��
//      �������ñ�����,��һ�ε���current_node = parent_node,���current_node
//      = �ϴη���ֵ��ֱ�����ؿ�.
//      �������������ӡ������....��˳������������ֱϵ��������ϵ��ȷ����������
//      �ڵ㶼�ܹ����ʵ�������Է���˳��������Ҫ�󣬲���ʹ�ñ�������
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

    __unlock_objsys();
    return (result);
}

// ============================================================================
// ���ܣ������ֵܣ�ͬ�������ϵģ�����
// ������ob -- ����;.
//      name -- ��Ҫ�����Ķ�����;
// ���أ��ɹ�����Ϊname�Ķ��󣩣�ʧ�ܣ�NULL����
// ��ע��
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
// ���ܣ������Ӷ���
// ������parent -- ����
//      name -- ��Ҫ�����Ķ�����
// ���أ��ɹ�����Ϊname�Ķ��󣩣�ʧ�ܣ�NULL����
// ��ע��
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
// ���ܣ����Ҷ�����ӹ�ϵ������չ��ϵ�µĶ���
// ������ancester -- ����
//      name -- ����ҵĶ�������
// ���أ��ɹ�����Ϊname�Ķ��󣩣�ʧ�ܣ�NULL����
// ��ע��
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
// ���ܣ��Ӷ���start����ʼ����·�����Ҷ���
// ������start -- ·������ʼ����
//      path -- �����Ķ���·����
// ���أ��ɹ���·�������Ķ��󣩣�ʧ�ܣ�NULL����
// ��ע��
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
            path++; // ���˶����'/'

        if('\0' == *path)
            break; // ����·������

        if(('.' == path[0]) && ('.' == path[1])
           && (('\\' == path[3]) || ('/' == path[3]) || ('\0' == path[3])))
        {
            current = obj_parent(current);
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

        segment = (char*)obj_name(head);
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
            segment = (char*)obj_name(current);
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

        current = obj_parent(current); // ��ǰ�ڵ���·����ƥ�䣬�˻������Լ��ڵ�
        break; // δ�ҵ��ڵ�
    }

__SEARCH_DONE:
   __unlock_objsys();
   return (current);
#endif
}

// ============================================================================
// ���ܣ��ڶ���parent֮���½��Ӷ��󼯺�
// ������parent -- ����
//      name -- ���󼯺ϵ��� ��
//      ops -- ���󼯺ϵ�Ĳ�����ʽ��NULL���̳и�������-1��ʹ��ȱʡ����������
//      represent -- ���󼯺ϵ��ֵ��
//      rights -- ���󼯺ϵ��Ȩ�ޣ�
// ���أ��ɹ����½����Ӷ��󼯺ϣ���ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
struct obj *obj_newchild_set(struct obj *parent, const char *name, fnObjOps ops,
                             ptu32_t ObjPrivate, u32 rights)
{
    struct obj *child_set;
    char *cname;

    if(!parent)
        parent = __current_obj; // ���δָ�����ڵ㣬��ָ��ǰ�����ڵ㣻

    if(name)
    {
        if(__checkname(name))
            return (NULL);

        if(NULL != obj_search_child(parent, name))
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
    child_set->seton = child_set; // Ĭ�϶���֮�ϲ����������󼯺ϣ�
    child_set->set = child_set; // �����߼�����̳и���
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
// ���ܣ����ö�������о���Ķ�·���ô����¼���
// ������ob -- ���������ļ�����
//      events -- ��·���ô����¼���
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 obj_set_handles_multievent(struct obj *ob, u32 events)
{
    extern s32 handle_multievent_setall(list_t *all, u32 events);

    if(!ob)
        return (-1);

    return (handle_multievent_setall(&ob->handles, events));
}

// ============================================================================
// ���ܣ������������о���Ķ�·���ô����¼���
// ������ob -- ���������ļ�����
//      events -- ��·���ô����¼���
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 obj_unset_handles_multievent(struct obj *ob, u32 events)
{
    extern s32 handle_multievent_unsetall(list_t *all, u32 events);

    if(!ob) // ����δ�����ã�
        return (-1);

    return (handle_multievent_unsetall(&ob->handles, events));
}
