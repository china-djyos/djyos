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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
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
#include <dbug.h>
#include <math.h>
#include "../include/object.h"
#include "../include/objhandle.h"

#define CN_BASIC_HANDLES               32 // ������Ԥ�ֵ�����������̬��չ��

static struct MutexLCB __handle_sys_mutex; // �ļ�ϵͳ������
static struct MemCellPool __handle_struct_pool; // �ļ�Ԥ�����

static ptu32_t __rf_operations(enum objops ops, ptu32_t oof, ptu32_t args, ...);

// ============================================================================
// ���ܣ�������ϵͳ������
// �������ޣ�
// ���أ��ޣ�
// ��ע��
// ============================================================================
static inline void __lock_handle_sys(void)
{
    Lock_MutexPend(&__handle_sys_mutex, CN_TIMEOUT_FOREVER);
}

// ============================================================================
// ���ܣ�������ϵͳ������
// �������ޣ�
// ���أ��ޣ�
// ��ע��
// ============================================================================
static inline void __unlock_handle_sys(void)
{
    Lock_MutexPost(&__handle_sys_mutex);
}

// ============================================================================
// ���ܣ��������ռ�س�ʼ��
// ������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
static s32 __handle_init_struct_pool(void)
{
    static struct objhandle handle_basic_struct_space[CN_BASIC_HANDLES];

    if(Mb_CreatePool_s(&__handle_struct_pool, handle_basic_struct_space,
                CN_BASIC_HANDLES, sizeof(struct objhandle), 16, 16384, "object file"))
        return (0);

    return (-1);
}

// ============================================================================
// ���ܣ�����ϵͳϵͳ��ʼ����
// ������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 objsys_init(void)
{
    s32 res;
    extern s32 raw_objsys_init(void);

    res = raw_objsys_init();
    if(res)
    {
        return (-1);
    }

    if(!Lock_MutexCreate_s(&__handle_sys_mutex, "ohdl sys"))
    {
        return (-1);
    }

    res = __handle_init_struct_pool();
    if(res)
    {
        return (-1);
    }

    obj_setops(objsys_root(), __rf_operations); // ȱʡ��װ���ǻ���RAM���ļ�����ϵͳ

    return (0);
}

// �����ļ��������ָ�룬��open���ص�fd��s32��������Ҫ�ж�Ӧ��ϵ
// hdl �� fd ��Ȼ�ܹ�����ֱ��ת����������
// 1��ָ�볤��С��32
// 2��ָ�볤��==32�����ֽ�λ��32
#if(( CN_PTR_BITS < 32)  || ( (CN_PTR_BITS == 32) && (CN_BYTE_BITS < 32)))
// ============================================================================
// ���ܣ�������ת�ļ���������
// ������hdl -- �ڲ������
// ���أ��ɹ����ļ�����������ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 Handle2fd(struct objhandle *hdl)
{
    if(!hdl)
        return (-1);

    return (s32)(((u32)hdl) >> 1);
}

// ============================================================================
// ���ܣ��ļ�������ת������
// ������fd -- �ļ���������
// ���أ��ɹ�������������ʧ�ܣ�NULL����
// ��ע��
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
// ������Ȼ�໥ת��������͸��Ӷ��ˡ�
// ��������object��ʹ�õ��ڴ�ռ䷶Χ��ֻҪ������2G���ڣ��Ϳ��Ի�ת�ˡ�
#error FD logic is not supported.
#endif

// ============================================================================
// ���ܣ��Ƿ�Ŀ¼��־��
// ������flags -- ʹ�ñ�־��
// ���أ�Ŀ¼��1������Ŀ¼��0����
// ��ע��INLINE
// ============================================================================
inline s32 test_directory(u32 flags)
{
    if(flags & O_DIRECTORY)
        return (1);

    return (0);
}

// ============================================================================
// ���ܣ��Ƿ��ļ���־��
// ������flags -- ʹ�ñ�־��
// ���أ��ļ���1�������ļ���0����
// ��ע��INLINE
// ============================================================================
inline s32 test_regular(u32 flags)
{
    if(flags & O_DIRECTORY)
        return (0);

    return (1);
}

// ============================================================================
// ���ܣ��Ƿ񴴽���򿪱�־��
// ������flags -- ʹ�ñ�־��
// ���أ�������򿪣�1�����Ǵ�����򿪣�0����
// ��ע��INLINE
// ============================================================================
inline s32 test_creat(u32 flags)
{
    if(flags & O_CREAT)
        return (1);

    return (0);
}

// ============================================================================
// ���ܣ��Ƿ��������־��
// ������flags -- ʹ�ñ�־��
// ���أ���������1�����ǽ�������0����
// ��ע��INLINE
// ============================================================================
inline s32 test_onlycreat(u32 flags)
{
    if((flags & (O_CREAT | O_EXCL)) == (O_CREAT | O_EXCL))
        return (1);

    return (0);
}

// ============================================================================
// ���ܣ��Ƿ�׷�ӣ�
// ������flags -- �ļ�ʹ�ñ�־��
// ���أ�׷�ӣ�1������׷�ӣ�0����
// ��ע��INLINE
// ============================================================================
inline s32 test_append(u32 flags)
{
    if(flags & O_APPEND)
        return (1);

    return (0);
}

// ============================================================================
// ���ܣ��Ƿ�ضϱ�־��
// ������flags -- �ļ�ʹ�ñ�־��
// ���أ��ضϣ�1�����ǽضϣ�0����
// ��ע��INLINE
// ============================================================================
inline s32 test_trunc(u32 flags)
{
    if(flags & (O_TRUNC))
        return (1);

    return (0);
}

// ============================================================================
// ���ܣ��Ƿ�ɶ���־��
// ������flags -- �ļ�ʹ�ñ�־��
// ���أ��ɶ���1�����ǿɶ���0����
// ��ע��INLINE
// ============================================================================
inline s32 test_readable(u32 flags)
{
    if(flags & O_RDONLY)
        return (1);

    return (0);
}

// ============================================================================
// ���ܣ��Ƿ��д��־��
// ������flags -- �ļ�ʹ�ñ�־��
// ���أ���д��1�����ǿ�д��0����
// ��ע��INLINE
// ============================================================================
inline s32 test_writeable(u32 flags)
{
    return (flags & O_WRONLY);
}

// ============================================================================
// ���ܣ��������������ļ��Ƿ���Ŀ¼��
// ������hdl -- ��������
// ���أ�Ŀ¼��1������Ŀ¼��0����
// ��ע��INLINE
// ============================================================================
inline s32 isdirectory(struct objhandle *hdl)
{
    return(test_directory(hdl->flags));
}

// ============================================================================
// ���ܣ��������Ƿ�׷��ģʽ��
// ������hdl -- ��������
// ���أ�׷�ӣ�1������׷�ӣ�0����
// ��ע��INLINE
// ============================================================================
inline s32 isappend(struct objhandle *hdl)
{
    return (test_append(hdl->flags));
}

// ============================================================================
// ���ܣ��������Ƿ�ɶ�ģʽ��
// ������hdl -- ��������
// ���أ��ɶ���1�������ɶ���0����
// ��ע��INLINE
// ============================================================================
inline s32 isreadable(struct objhandle *hdl)
{
    return (test_readable(hdl->flags));
}

// ============================================================================
// ���ܣ��������Ƿ��дģʽ��
// ������hdl -- ��������
// ���أ��ɶ���1�������ɶ���0����
// ��ע��INLINE
// ============================================================================
inline s32 iswritable(struct objhandle *hdl)
{
    return (test_writeable(hdl->flags));
}

// ============================================================================
// ���ܣ������Ƿ���ھ����ߣ������������ط�ͬʱ��
// ������hdl -- ��������
// ���أ����ڣ�1���������ڣ�0����
// ��ע��INLINE
// ============================================================================
inline s32 iscontender(struct objhandle *hdl)
{
    if(!hdl)
        return (0);

    if(dListGetBefore(&hdl->list)==dListGetAfter(&hdl->list))
        return (0);

    return (1);
}

// ============================================================================
// ���ܣ��½���������
// �������ޣ�
// ���أ��ɹ�������������ʧ�ܣ�NULL����
// ��ע��INLINE
// ============================================================================
inline struct objhandle *handle_new(void)
{
    struct objhandle *hdl;

    hdl = (struct objhandle*)Mb_Malloc(&__handle_struct_pool, 0);
    if(!hdl)
        return (NULL);

    memset(hdl, 0x0, sizeof(*hdl));
    dListInit(&hdl->list);
    return (hdl);
}

// ============================================================================
// ���ܣ��ͷŶ�������
// ������hdl -- ��������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��INLINE
// ============================================================================
inline s32 handle_free(struct objhandle *hdl)
{
    if(hdl)
    {
        dListRemove(&hdl->list);
        Mb_Free(&__handle_struct_pool, hdl);
        return (0);
    }

    return (-1);
}

// ============================================================================
// ���ܣ��������ĳ�ʼ����
// ������hdl -- ��������
//      ob -- �����Ķ���
//      flags -- ʹ�ñ�־��
//      context -- ���������ݣ�
// ���أ��ޣ�
// ��ע��
// ============================================================================
inline void handle_init(struct objhandle *hdl, struct obj *ob, u32 flags, ptu32_t context)
{
    memset(hdl, 0, sizeof(struct objhandle));
    hdl->timeout = CN_TIMEOUT_FOREVER;
    hdl->obj = ob;
    hdl->context = context;
    hdl->flags = flags;
}

// ============================================================================
// ���ܣ���ȡ��������
// ������hdl -- ������
// ���أ��ɹ����ļ�������ʧ�ܣ�NULL����
// ��ע��INLINE
// ============================================================================
inline const char *handle_name(struct objhandle *hdl)
{
    if(!hdl)
        return (NULL);

    return (obj_name(hdl->obj));
}

// ============================================================================
// ���ܣ���ȡ���������������ݣ�
// ������hdl -- ��������
// ���أ��ɹ���˽�����ݣ���ʧ�ܣ�NULL��;
// ��ע��INLINE
// ============================================================================
inline ptu32_t handle_context(struct objhandle *hdl)
{
    if(hdl)
        return (hdl->context);

    return (0);
}

// ============================================================================
// ���ܣ����ö��������������ݣ�
// ������hdl -- ��������
//      context -- ���ݣ�
// ���أ��ޣ�
// ��ע��INLINE
// ============================================================================
inline void handle_linkcontext(struct objhandle *hdl, ptu32_t context)
{
    if(hdl)
        hdl->context = context;
}

// ============================================================================
// ���ܣ���ȡ�����������Ķ������ݣ�
// ������hdl -- ��������
// ���أ��ɹ����ļ�����ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
inline void *handle_val(struct objhandle *hdl)
{
    if(hdl&&hdl->obj)
        return ((void*)obj_GetPrivate(hdl->obj));

    return (NULL);
}

// ============================================================================
// ���ܣ���ȡ����������ϵͳ�Ĺ�����ƣ�
// ������hdl -- �ļ������ģ�
// ���أ��ɹ���ϵͳ�Ĺ�����Ϣ����ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
inline void *handle2sys(struct objhandle *hdl)
{
    extern void *corefs(struct obj *ob);

    return (corefs(hdl->obj));
}

// ============================================================================
// ���ܣ�������������������
// ������hdl -- ��������
//      ob -- ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 handle_linkobj(struct objhandle *hdl, struct obj *ob)
{
    if((!ob)||(!hdl))
        return (-1);

    hdl->obj = ob;
    obj_setquote(ob, &hdl->list);
    return (0);
}

// ============================================================================
// ���ܣ���ȡ�������Ķ�·���ÿ��ƣ�
// ������hdl -- ��������
// ���أ��ɹ�����·���ÿ��ƣ���ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
inline struct MultiplexObjectCB *handle_multiplex(struct objhandle *hdl)
{
    if(hdl)
        return (hdl->pMultiplexHead);

    return (NULL);
}

// ============================================================================
// ���ܣ����ö������Ķ�·���ÿ��ƣ�
// ������hdl -- ��������
//      cb -- ��·���ÿ��ƣ�
// ���أ��ޣ�
// ��ע��
// ============================================================================
inline void handle_setmultiplex(struct objhandle *hdl, struct MultiplexObjectCB *cb)
{
    if(hdl)
        hdl->pMultiplexHead = cb;
}

// ============================================================================
// ���ܣ���ȡ�������ĳ�ʱʱ�䣻
// ������hdl -- ��������
// ���أ���ʱʱ�䣻
// ��ע��
// ============================================================================
inline u32 handle_timeout(struct objhandle *hdl)
{
    if(hdl)
        return (hdl->timeout);

    return (0);
}

// ============================================================================
// ���ܣ����ö������ĳ�ʱʱ�䣻
// ������hdl -- ��������
//      timeout -- ��ʱʱ�䣻
// ���أ��ޣ�
// ��ע��INLINE
// ============================================================================
inline void handle_settimeout(struct objhandle *hdl, u32 timeout)
{
    if(hdl)
        hdl->timeout = timeout;
}

// ============================================================================
// ���ܣ���ȡ��������ʹ�ñ�־��
// ������hdl -- ��������
// ���أ�ʹ�ñ�־��
// ��ע��INLINE
// ============================================================================
inline u32 handle_mode(struct objhandle *hdl)
{
    if(hdl)
        return (hdl->flags);

    return (0);
}

// ============================================================================
// ���ܣ����ö�������ʹ�ñ�־��
// ������hdl -- ��������
//      mode -- ʹ�ñ�־��
// ���أ��ޣ�
// ��ע��INLINE
// ============================================================================
inline void handle_changemode(struct objhandle *hdl, u32 mode)
{
    if(hdl)
        hdl->flags = mode;
}
// ============================================================================
// ���ܣ���ȡ�������Ĺ�������
// ������hdl -- ��������
// ���أ��ɹ����������󣩣�ʧ�ܣ�NULL����
// ��ע��INLINK
// ============================================================================
inline struct obj *handle2obj(struct objhandle *hdl)
{
    if(hdl)
        return (hdl->obj);

    return (NULL);
}

#if 0
// ============================================================================
// ���ܣ������ļ����������һ�������´�����ȫ���ļ��������÷���
//      struct objhandle *Current = NULL;
//      while(1)
//      {
//          Current = OBJ_TraveFile(Current, s_ptSocketObject);
//          if(Current != NULL)
//              do something;
//          else
//              break;
//      }
// ������Current����ǰ����λ�ã���ʼΪNULL��
// ���أ���һ���ļ�ָ�룬��������򷵻�NULL
// ��ע����ǰ��SOCKETר�ã�δ��ʹ�ã�
// ============================================================================
struct objhandle *traverseall(struct objhandle *hdl, struct obj *ob)
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
        if(testcontent(reflagsode)) // Խ��content
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
// ���ܣ����ö�������ʹ�ñ�־��
// ������hdl -- ��������
//      flags -- ʹ�ñ�־��
// ���أ��ޣ�
// ��ע��
// ============================================================================
void hdl2objrflag(struct objhandle *hdl, u32 flags)
{
    if(hdl)
        hdl->flags |= flags;
}
#endif
// ============================================================================
// ���ܣ���ȡ�������������õĶ�·���ô����¼���
// ������hdl -- ��������
// ���أ������õĶ�·�����¼���
// ��ע��
// ============================================================================
inline u32 handle_multievents(struct objhandle *hdl)
{
    if(hdl)
        return (hdl->MultiplexEvents);

    return (0);
}

// ============================================================================
// ���ܣ����ö������Ķ�·���ô����¼���
// ������hdl -- ��������
//      events -- ��·���ô����¼���
// ���أ��ޣ�
// ��ע��
// ============================================================================
inline void handle_set_multievent(struct objhandle *hdl, u32 events)
{
    u32 check;
    extern bool_t __Multiplex_Set(s32 Fd, u32 dwAccess);
    Int_SaveAsynSignal();
    if(hdl)
    {
        check = hdl->MultiplexEvents;
        hdl->MultiplexEvents |= events;
        if(check!=hdl->MultiplexEvents)
            __Multiplex_Set(Handle2fd(hdl), hdl->MultiplexEvents);
    }
    Int_RestoreAsynSignal();
}

// ============================================================================
// ���ܣ�����������Ķ�·���ô����¼���
// ������hdl -- ��������
//      events -- ��·���ô����¼���
// ���أ��ޣ�
// ��ע��
// ============================================================================
inline void handle_unset_multievent(struct objhandle *hdl, u32 events)
{
    u32 check;
    extern bool_t __Multiplex_Set(s32 Fd, u32 dwAccess);
    Int_SaveAsynSignal();
    if(hdl)
    {
        check = hdl->MultiplexEvents;
        hdl->MultiplexEvents &= ~events;
        if(check != hdl->MultiplexEvents)
            __Multiplex_Set(Handle2fd(hdl), hdl->MultiplexEvents);
    }
    Int_RestoreAsynSignal();
}

// ============================================================================
// ���ܣ�����ͬ�ļ��µ����ж������Ķ�·���ô����¼���
// ������hdl -- ��������
//      events -- ��·���ô����¼���
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 handle_multievent_setall(list_t* all, u32 events)
{
    struct objhandle *hdl;
    u32 MultiplexEvents;
    list_t *head, *cur;
    extern bool_t __Multiplex_Set(s32 Fd, u32 dwAccess);

    if(!all)
        return (-1);

    Int_SaveAsynSignal();
    head = all; // head�ǲ���Ҫ���õģ�
    dListForEach(cur, head)
    {
        hdl = dListEntry(cur, struct objhandle, list);
        MultiplexEvents = hdl->MultiplexEvents;
        hdl->MultiplexEvents |= events;
        if(MultiplexEvents!=hdl->MultiplexEvents)
            __Multiplex_Set(Handle2fd(hdl), hdl->MultiplexEvents);
    }
    Int_RestoreAsynSignal();
    return (0);
}

// ============================================================================
// ���ܣ����ͬ�ļ��µ����ж������Ķ�·���ô����¼���
// ������hdl -- ��������
//      events -- ��·���ô����¼���
// ���أ��ޣ�
// ��ע��
// ============================================================================
s32 handle_multievent_unsetall(list_t *all, u32 events)
{
    struct objhandle *hdl;
    u32 MultiplexEvents;
    list_t *head, *cur;
    extern bool_t __Multiplex_Set(s32 Fd, u32 dwAccess);

    if(!all)
        return (-1);

    Int_SaveAsynSignal();
    head = all; // ͷ���ǲ���Ҫ���õģ�
    dListForEach(cur, head)
    {
        hdl = dListEntry(cur, struct objhandle, list);
        MultiplexEvents = hdl->MultiplexEvents;
        hdl->MultiplexEvents &= ~events;
        if(MultiplexEvents != hdl->MultiplexEvents)
            __Multiplex_Set(Handle2fd(hdl),hdl->MultiplexEvents);
    }

    Int_RestoreAsynSignal();
    return (0);
}

// ============================================================================
// ���ܣ����ļ�
// ������pathname -- �ļ�·��;
//      flags -- ������־;
//      mode -- ����ģʽ;
// ���أ�ʧ�ܣ�-1��; �ļ�����������-1��;
// ��ע��O_RDONLY��O_WRONLY��O_RDWR �����POSIX��2016�������¶���
// ============================================================================
static inline struct objhandle *__open(char *path, s32 flags, u32 mode)
{
    struct objhandle *hdl;
    struct obj *ob;
    char *full, *uncached;

    __lock_handle_sys();
    ob = obj_matchpath(path, &uncached, &full);
    if(obj_up(ob)) // ��ֹ�ļ����������У���ɾ���ˣ�
    {
        __unlock_handle_sys();
        return (NULL);
    }

    __unlock_handle_sys();
    hdl = (struct objhandle*)ob->ops(OBJOPEN, (ptu32_t)ob, (ptu32_t)flags, uncached, mode, full);
    __lock_handle_sys();
    obj_down(ob);
    if(hdl)
    {
        ob = obj_buildpath(ob, uncached); // �����ļ�ϵͳ���ԣ�������Ҫ�����Ŀ¼·��,�̳в���������.
        handle_linkobj(hdl, ob);
    }

    __unlock_handle_sys();
    return (hdl);
}

// ============================================================================
// ���ܣ��ر��ļ�
// ������hdl -- ������;
// ���أ��ɹ���0��;ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 __close(struct objhandle *hdl)
{
    s32 res;
    struct obj *ob = hdl->obj;

    res = (s32)ob->ops(OBJCLOSE, (ptu32_t)hdl, 0);
    if(!res)
    {
        obj_releasepath(ob); // �ͷŶ�����ʱ·��
    }

    return (res);
}

// ============================================================================
// ���ܣ����ļ�
// ������pathname -- �ļ�·��;
//      flags -- ������־;
//      mode -- ����ģʽ;
// ���أ�ʧ�ܣ�-1��; �ɹ����ļ�����������-1����;
// ��ע��O_RDONLY��O_WRONLY��O_RDWR �����POSIX��2016�������¶���
// ============================================================================
int open(const char *pathname, int flags, ...)
{
    struct objhandle *hdl;

    if((!pathname) || ('\0' == *pathname))
        return (-1);

#if 0 // TODO
    va_start(args, flags);
    mode = va_arg(args, s32);
    va_end(args);
#endif

    hdl = __open((char*)pathname, flags, 0);
    return (Handle2fd(hdl));
}

// ============================================================================
// ���ܣ��ر��ļ�
// ������fd -- �ļ�������;
// ���أ��ɹ���0��;ʧ�ܣ�-1����
// ��ע��
// ============================================================================
int close(int fd)
{
    struct objhandle *hdl;

    hdl = fd2Handle(fd);
    if(!hdl)
        return (-1);

    return (__close(hdl));
}

// ============================================================================
// ���ܣ�ɾ���ļ���
// ������path -- �ļ�·����
// ���أ��ɹ���0���� ʧ�ܣ�-1����
// ��ע��
// ============================================================================
inline int remove(const char *path)
{
    struct obj *ob;
    char *uncached, *full;
    s32 res;

    __lock_handle_sys();
    ob = obj_matchpath(path, &uncached, &full);
    if(!uncached) // �ļ��Ѿ���ϵͳ��
    {
        if(obj_isset(ob) // ��װ�㣨���󼯺ϵ㣩����ɾ��
           ||(obj_isonduty(ob)))  // �ļ����ڱ�ʹ����
        {
            __unlock_handle_sys();
            return (-1);
        }
    }

    obj_lock(ob);
    __unlock_handle_sys();
    res = (s32)ob->ops(OBJDEL, (ptu32_t)ob, (ptu32_t)uncached, full);
    obj_unlock(ob);
    if(!res)
    {
        obj_releasepath(ob); // �ͷ���·���е���ʱ����
    }

    return (res);
}

// ============================================================================
// ���ܣ����ļ���
// ������fd -- �ļ���������
//      buf -- �����壻
//      count -- ��Ҫ�������ݴ�С��Byte�ƣ���
// ���أ��ɹ���ʵ�ʶ�ȡ���ĳ��ȣ�������-1����
// ��ע��
// ============================================================================
ssize_t read(int fd, void *buf, size_t size)
{
    struct objhandle *hdl;
    s32 res = -1;

    if(NULL == buf)
        return (-1); // ��������

    if(0 == size)
        return (0);

    hdl = fd2Handle(fd);
    if(!hdl)
        return (-1);

    if(isreadable(hdl))
    {
        res = (s32)hdl->obj->ops(OBJREAD, (ptu32_t)hdl, (ptu32_t)buf, size);
    }

    return (res);
}

// ============================================================================
// ���ܣ�д�ļ���
// ������fd -- �ļ���������
//      buf -- д���壻
//      count -- ��Ҫд�����ݴ�С��Byte�ƣ���
// ���أ� �ɹ���ʵ��д��ĳ��ȣ�������-1����
// ��ע��
// ============================================================================
ssize_t write(int fd, const void *buf, size_t count)
{
    struct objhandle *hdl;
    s32 res = -1;

    if(!buf)
        return (-1); // ��������

    if(0 == count)
        return (0);

    hdl = fd2Handle(fd);
    if(!hdl)
        return (-1);

    if(iswritable(hdl))
    {
        res = (s32)hdl->obj->ops(OBJWRITE, (ptu32_t)hdl, (ptu32_t)buf, count);
    }

    return (res);
}

// ============================================================================
// ���ܣ��ļ��ض�λ
// ������fd -- �ļ���������
//      offset -- �ļ�ƫ�ã�
//      whence -- �ļ�ƫ�õ���㣻
// ���أ��ɹ����ض�λ���λ�ã���ʧ�ܣ�-1����
// ��ע��NOTE: lseek��֧���ļ���С��չ
// ============================================================================
off_t lseek(int fd, off_t offset, int whence)
{
    struct objhandle *hdl;
    s32 res;

    if((SEEK_SET != whence) && (SEEK_CUR != whence) && (SEEK_END != whence))
        return (-1); // ����Ĳ���

    if((SEEK_SET == whence) && (offset < 0))
        return (-1); // ������߼�

    hdl = fd2Handle(fd);
    if(!hdl)
        return (-1);

    if(test_directory(hdl->flags))
        return (-1); // Ŀ¼������seek

    res = (s32)hdl->obj->ops(OBJSEEK, (ptu32_t)hdl, (ptu32_t)&offset, whence);
    if(res)
        return (-1);

    return (offset);
}

// ============================================================================
// ���ܣ��ļ�ͬ����
// ������fd -- �ļ���������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
int fsync(int fd)
{
    struct objhandle *hdl;
    s32 res;

    hdl = fd2Handle(fd);
    if(!hdl)
    {
        errno = EBADF; // ��������
        return (-1);
    }

    res = (s32)hdl->obj->ops(OBJSYNC, (ptu32_t)hdl, 0);
    return (res);
}

// ============================================================================
// ���ܣ���ȡ�ļ��ĵ�ǰλ�ã�
// ������fd -- �ļ���������
// ���أ��ɹ����ļ���ǰλ�ã���ʧ�ܣ�-1����
// ��ע��
// ============================================================================
off_t tell(int fd)
{
    struct objhandle *hdl;
    s64 offset;
    s32 res;

    hdl = fd2Handle(fd);
    if(!hdl)
    {
        errno = EBADF; // ��������
        return (-1);
    }

    res = (s32)hdl->obj->ops(OBJTELL, (ptu32_t)hdl, (ptu32_t)&offset);
    if(!res)
        return (offset);

    return (-1);
}

// ============================================================================
//���ܣ���ѯ�ļ�״̬��
//������fd -- �ļ���������
//     buf -- �ļ�״̬;
//���أ��ɹ���0��; ʧ�ܣ�-1��;
//��ע��
// ============================================================================
int fstat(int fd, struct stat *buf)
{
    s32 res;
    struct objhandle *hdl;

    if(NULL == buf)
        return(-1);

    hdl = fd2Handle(fd);
    if(!hdl)
        return (-1);

    res = (s32)hdl->obj->ops(OBJSTAT, (ptu32_t)(hdl->obj), (ptu32_t)buf, 0, hdl);
    return (res);
}

// ============================================================================
// ���ܣ���ѯ�ļ�״̬��
// ������path -- �ļ�·����
//      buf -- �ļ�״̬;
// ���أ��ɹ���0��; ʧ�ܣ�-1��;
// ��ע��
// ============================================================================
int stat(const char *path, struct stat *buf)
{
    struct obj *ob;
    char *uncache, *full;
    s32 res;

    if((!buf) || (!path))
        return(-1);

    __lock_handle_sys();// ��ֹ���������ļ���ɾ����
    ob = obj_matchpath((char*)path, &uncache, &full);
    res = obj_up(ob);
    __unlock_handle_sys();
    if(res)
        return (-1);

    if(!uncache)
        uncache = ""; // ȫ��·�����Ѿ�����ʱ������Ϊ���ַ�������'\0'����������fstat�߼����֣�

    res = (s32)ob->ops(OBJSTAT, (ptu32_t)ob, (ptu32_t)buf, uncache, full);
    obj_down(ob);
    return (res);
}

// ============================================================================
// ���ܣ��趨�ļ����ȣ�
// ������fd -- �ļ���������
//      length -- �ļ����ȣ�
// ���أ��ɹ���0��; ʧ�ܣ�-1��;
// ��ע��the file offset is not changed.
// ============================================================================
int ftruncate(int fd, off_t length)
{
#if 0
    struct objhandle *hdl;
    s32 res;

    if(0 == length)
        return (0);

    hdl = fd2Handle(fd);
    if(!hdl)
        return (-1);

    res = (s32)hdl->ob->ops(CN_OBJ_CMD_TRUNCATE, (ptu32_t)hdl, (ptu32_t)&length);
    return (res);
#else
    fd = fd;
    length = length;
#endif
    return (-1);
}

// ============================================================================
//���ܣ�ɾ���ļ���Ŀ¼
//������pathname -- �ļ�·����
//���أ�0 -- �ɹ�; -1 -- ʧ��;
//��ע:
// ============================================================================
int unlink(const char *pathname)
{
    if(remove(pathname))
        return (-1);

    return (0);
}

// ============================================================================
// ���ܣ���Ŀ¼��
// ������name -- Ŀ¼·����
// ���أ��ɹ�����NULL����ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
DIR *opendir(const char *name)
{
    tagDIR *directory; // ��DIR�ṹ��
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
    }

    memset(directory, 0, sizeof(*directory));
    directory->__fd = (void*)hdl;
    return (directory);
}

// ============================================================================
// ���ܣ���Ŀ¼�
// ������dir -- �Ѵ򿪵�Ŀ¼��
// ���أ�Ŀ¼�ȫ���Ѷ�����NULL��
// ��ע��ÿ�εķ���ֵ������ͬһ���ռ䣬����������Ϊ��ʡ�ռ䣻
// ============================================================================
struct dirent *readdir(DIR *dir)
{
    struct objhandle *hdl;
    struct obj *ob;
    s32 res;

    if(!dir)
        return (NULL);

    if(!dir->__ptr)
    {
        dir->__ptr = malloc(sizeof(struct dirent));
        if(!dir->__ptr)
            return (NULL);

        memset(dir->__ptr, 0x0, (sizeof(struct dirent)));
    }

    hdl = (struct objhandle*)(dir->__fd); // Ŀ¼��������
    ob = hdl->obj; // Ŀ¼�Ľڵ�
    res = (s32)ob->ops(OBJCHILDS, (ptu32_t)hdl, (ptu32_t)dir->__ptr);
    if(!res)
        return (struct dirent*)(dir->__ptr);
    else
        return (NULL); // ��ȫ��������
}

// ============================================================================
// ���ܣ��ر�Ŀ¼
// ������dir -- Ŀ¼��
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
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
// ���ܣ�����Ŀ¼
// ������filename -- Ŀ¼����·������
//      mode -- ģʽ��
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע�����Ŀ¼�Ѵ��ڣ�����ʧ�ܣ�
// ============================================================================
int mkdir(const char *filename, mode_t mode)
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
// ���ܣ�������������ļ����ƣ�
// ������hdl -- ��������
//      cmd -- ��������;
//      argspace -- ������������ռ䣻
// ���أ�ʧ�ܣ�-1�����ɹ���0����δ֧�֣�1����
// ��ע��
// ============================================================================
static s32 __of_cntl(struct objhandle *hdl, s32 cmd, va_list *argspace)
{
    va_list args = *argspace;

    switch(cmd)
    {
        case F_GETPORT:
        {
            struct objhandle **ret;
            ret = (struct objhandle **)va_arg(args, u32);

            if(!*ret)
                return (-1);

            *ret = hdl;
            return (0);
        }

        case F_SETCONTEXT:
        {
            ptu32_t context;

            context = (ptu32_t)va_arg(args, u32);
            handle_linkcontext(hdl, context);
            return (0);
        }

        case F_GETCONTEXT:
        {
            ptu32_t *ret;

            ret = (ptu32_t*)va_arg(args, u32);
            *ret =  handle_context(hdl);
            return (0);
        }

        case F_GETFILE:
        {
            ptu32_t *ret;

            ret = (ptu32_t*)va_arg(args, u32);
            *ret = (ptu32_t)handle_val(hdl);
            return (0);
        }

        case F_SETEVENT:
        {
            u32 events;

            events = va_arg(args, u32);
            handle_set_multievent(hdl, events);
            return (0);
        }

        case F_CLREVENT:
        {
            u32 events;

            events = va_arg(args, u32);
            handle_unset_multievent(hdl, events);
            return (0);
        }

        default: break;
    }

    return (1);
}

// ============================================================================
// ���ܣ��ļ����ƣ�
// ������fd -- �ļ���������
//      cmd -- �������
//      �ɱ�� -- �������������
// ���أ�
// ��ע��cmd ����ƫ�� CN_OBJ_FCNTL_START ����ǵ��� ObjOps ��cmd ��
// ============================================================================
int fcntl(int fd, int cmd, ...)
{
    struct objhandle *hdl;
    va_list args;
    s32 res = -1;

    hdl = fd2Handle(fd);
    if(!hdl)
        return (-1);

    va_start (args, cmd);
#if 0
    res = __of_cntl(hdl, cmd, &args);
    if(!res)
        return (0); // �����ļ��Ĳ���

    if(cmd < CN_OBJ_IOCTL_START)
       cmd += CN_OBJ_FCNTL_START;

    res = (s32)hdl->obj->ops(cmd, (ptu32_t)hdl, (ptu32_t)&args);
    va_end (args);

    return (res);
#else
    res = __of_cntl(hdl, cmd, &args);
    if(1==res)
    {
        res = (s32)hdl->obj->ops(OBJCTL, (ptu32_t)hdl, (ptu32_t)cmd, &args);
    }

    va_end (args);
    return (res);
#endif
}

// ============================================================================
// ���ܣ�IO���ƣ�
// ������fd -- �ļ���������
//      cmd -- ���������룻
//      �ɱ���� -- �������������
// ���أ�
// ��ע��
//      ���µ�POSIX�Ѿ�������ioctl��������ʲôȡ��������ܣ�������Ȼ���ѣ�������Դ��Ŀ��Ȼ���á�
//      ע����ֵ�POSIX��ioctl��صĳ�����ͷ�ļ����� stropts.h ������ ioctl.h
//      request ����ƫ�� CN_OBJ_IOCTL_START ����ǵ��� ObjOps ��cmd ��
//      �ɱ����д������ȫ��Ϊ�˼���POSIX����ʵ�ϴ���û�����ù����ĸ�����������������
//      ���õģ����ýṹָ�롣
//      POSIX��û�й涨���� request ���ʵ�֣�ֻ������һЩ����ķ�������
//      ʵ��ʵ���У���3�ࡣ
//      A��Linux��newlib��ʵ�֣�request�����Ǹ���ϣ��䷽�����£�
//      1����������������Ǽ����ͣ�ָ����⣩������request�������˸����͵ĳߴ硣
//      2����������������ǽṹָ�룬����request�������˸ýṹ�ĳߴ硣
//      3������е��ĸ��������Բ����±ơ�
//      �ʣ�Linux��newlibʵ�����ǲ�֧�ֵ��ĸ������ġ�
//      B��VxWorks�������˵�һ����������ϵķ�ʽ��
//      C����Щ��Դ��Ŀ�������ǵ�iotjs���Զ�����һЩioctl���A��B�������С�
//      djyosʵ�ַ�����
//      1��ʹ�õ�һ����������
//      2��ϵͳ�Դ���ģ�飬��ʹ�õ��ĸ��������������������õģ��ýṹָ��
//      3��Ŀǰ��δ����ʹ�õ��ĸ������Ŀ�Դ���룬djyosʵ���ϲ�����֧�֡�
//
//      linux��cmd��������ṹ��������(2bit)���ݳߴ�(14bit)�豸���� (8it) �������(8bit)��
// ============================================================================
int ioctl(int fd, int cmd, ...)
{
    struct objhandle *hdl;
    va_list args;
    u32 arg_linux;
    s32 res = -1;

    hdl = fd2Handle(fd);
    if(!hdl)
        return (0);

    va_start (args, cmd);
    arg_linux = va_arg(args, u32);
    arg_linux = (arg_linux & 0xFF); // ��ȡ�������룻
    res = (s32)hdl->obj->ops(OBJIOCTL, (ptu32_t)hdl, (ptu32_t)cmd, arg_linux, &args);
    va_end (args);
    return (res);
}

// ============================================================================
// ���ܣ�����ǰ����Ŀ¼�ľ���·�����Ƶ�����buf��
// ������buf -- ���·���Ŀռ䣻
//      size -- buf�Ĵ�С��
// ���أ��ɹ�����ǰ����Ŀ¼�ľ���·������ʧ�ܣ�NULL��
// ��ע��bufΪ NULL��ʹ sizeΪ�㣬�򱾺����ᶯ̬����һ���ռ������ظ��û��������û���Ҫע�⽫�ռ��ͷţ�
// ============================================================================
char *getcwd(char *buf, size_t size)
{
    u32 len = 0, offset;
    struct obj *ob = objsys_current();

    if(((buf) && (!size)) || ((!buf) && (size)))
        return (NULL);

    while(ob)
    {
        len += strlen((char*)obj_name(ob)) + 1;
        if(ob==objsys_root())
            break;

        ob = obj_parent(ob);
    }

    len += 1; // ����·���ĳ���
    if((size) && (size < len))
        return (NULL);

    if(!buf)
    {
        buf = malloc(len);
        if(!buf)
            return (NULL);
    }

    offset = len -1;
    ob = objsys_current();
    buf[offset] = '\0';
    while(ob)
    {
        len = strlen((char*)obj_name(ob));
        offset -= len;
        memcpy(buf+offset, (char*)obj_name(ob), len);
        buf[--offset] = '/';
        if(ob==objsys_root())
            break;

        ob = obj_parent(ob);
    }

    return (buf);
}

// ============================================================================
// ���ܣ��ı䵱ǰ����Ŀ¼��
// ������path -- �µĵ�ǰ����Ŀ¼
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
int chdir(const char *path)
{
    path = path;
    return (-1); // TODO; NOTE:���õ�·����һ��Ŀ¼
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
#define __CONTENT_SIZE          512
struct __context
{
    u16 cpos; // �������__CONTENT_SIZE����ʾ��SEEEK����ĳߴ�����
    u16 cindex; // ������㣻
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
    void *u; // ָ������ļ����ͣ�ֻ���ļ��������ݣ�����ΪNULL
};

struct __ramreg
{
    u32 sz;
    struct __content *content;
};


// ============================================================================
// ���ܣ����������ļ����ݣ�
// ������content -- �ļ���
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 __rf_up_content(struct __content *content)
{
    if(-1==content->owners)
        return (-1);

    content->owners ++;
    return (0);
}


// ============================================================================
// ���ܣ����������ļ����ݣ�
// ������content -- �ļ���
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 __rf_down_content(struct __content *content)
{
    if(!content->owners)
        return (-1);

    content->owners --;
    return (0);
}

// ============================================================================
// ���ܣ����������ļ����ݣ�
// ������content -- �ļ���
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 __rf_del_content(struct __content *content)
{
    if(content->owners)
        return (-1);

    free(content);
    return (0);
}

// ============================================================================
// ���ܣ���չ�ļ����ݿռ䣻
// ������reg -- �ļ���
//      num -- �ļ����ݿռ�������
// ���أ��ɹ������һ�����ݿռ䣩��ʧ�ܣ�NULL����
// ��ע��
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
// ���ܣ������ļ����ݿռ䣻
// ������file -- �ļ���
//      size -- �������ĳߴ磻
// ���أ��ɹ������һ�����ݿռ�,����Ϊ�㣬��ΪNULL����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
struct __content *__rf_reduce(struct __ramreg *reg, u32 size)
{
    struct __content *tmp;
    list_t *last;
    u32 delsz = reg->sz - size;; // ��Ҫ�����ĳߴ磻

    if(delsz<=0)
    {
        return (NULL); // �߼����ԣ�
    }

    last = dListGetBefore(&(reg->content->list)); // ��ȡ���һ��content
    tmp = dListEntry(last, struct __content, list);
    if(delsz<(reg->sz%__CONTENT_SIZE))
    {
        // �����ĳߴ�С�����һ��content�Ŀռ䣻
        reg->sz -= delsz;
        memset(tmp->data+(reg->sz%__CONTENT_SIZE), 0x0, delsz);
        return (tmp);
    }
    else
    {
        // �����ĳߴ�������һ��content�Ŀռ䣻
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
// ���ܣ���ȡ�ļ����ݣ�
// ������file -- �ļ���
//      index -- �ļ��������У�����ƣ�
// ���أ��ɹ����ļ����ݣ���ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
struct __content *__rf_content(struct __ramreg *reg, s16 index)
{
    struct __content *content;
    list_t *nxt, *fst;

    if(!index)
        return (reg->content); // ���ҵ��ǵ�0��content

    fst = &(reg->content->list);
    nxt = dListGetAfter(fst);
    while(1)
    {

        if(nxt==fst)
            return (NULL); // δ�ҵ���Ҫ��index

        index --;
        if(!index)
        {
            content = dListEntry(nxt, struct __content, list);
            if(__rf_up_content(content))
                return (content); // �ҵ���Ҫ��content
            else
                return (NULL);
        }

        nxt = dListGetAfter(nxt);
    }

    // ���Ϸ����߼������ҵ�content������
    return (NULL);
}


// ============================================================================
// ���ܣ����ļ���
// ������ob -- �ļ�����
//      flags -- �򿪱�־��
//      name -- �ļ�����
// ���أ��ɹ����ļ��������ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
static struct objhandle *__rf_open(struct obj *ob, u32 flags, char *name)
{
    struct objhandle *hdl;
    struct __ramfile *file = NULL;
    struct __ramreg *reg = NULL;
    struct __context *cx = NULL;

    if(name) // ��Ҫ�½��ļ���Ŀ¼��
    {
        if(!test_creat(flags))
            return (NULL); // �߼��ϲ�Ҫ�󴴽�����ʵ���ļ�ȴ�����ڣ�

        if(test_regular(flags)) // �����ļ���
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
        else // �������ļ������ļ�û���ṩ�����ģ�
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

        ob = obj_newchild(ob, NULL, O_RDWR, (ptu32_t)file, name);
        if(!ob)
        {
            debug_printf("ramfs", "open failed(object create).");
            free(file);
            free(cx);
            return (NULL);
        }

        if(!test_regular(flags))
        {
            if(obj_allow2set(ob)) // ����Ŀ¼����֮�Ͻ������󼯺ϣ�������Ŀ¼֮�Ϲ����ļ�ϵͳ��
            {
                free(reg);
                obj_del(ob);
                return (NULL);
            }
        }
    }
    else // �ļ���Ŀ¼�Ѵ��ڣ�
    {
        if(!test_onlycreat(flags))
            return (NULL); // �߼��ϲ���Ҫ�󣻱��봴�������Ѵ��ڣ�

        file = (struct __ramfile*)obj_GetPrivate(ob);
        if(test_regular(flags)) // ���ļ���
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
            if(test_append(flags))
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
            else if(test_trunc(flags))
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
        else // �򿪷��ļ���
        {
            if((test_directory(flags))&&(file->type!=RAM_DIR))
                return (NULL);
        }
    }

    hdl = handle_new();
    if(!hdl)
    {
        printf("\r\n: dbug : ramfs  : open failed(context).");
        if(reg)
           free(reg);
    }

    handle_init(hdl, ob, flags, (ptu32_t)cx);
    return (hdl);
}

// ============================================================================
// ���ܣ��ر��ļ���
// ������hdl -- �ļ���������
// ���أ�
// ��ע��
// ============================================================================
static s32 __rf_close(struct objhandle *hdl)
{
    struct __context *cx = (struct __context*)handle_context(hdl);

    if(cx) // Ŀ¼û��__context;
    {
        __rf_down_content(cx->content);
        free(cx);
    }

    handle_free(hdl);
    return (0);
}

// ============================================================================
// ���ܣ�ɾ���ļ���
// ������
// ���أ�
// ��ע��ɾ����ʱ�򣬶����Ѿ�����ס���������µķ��ʣ�
// ============================================================================
static s32 __rf_remove(struct obj *ob)
{
    struct __ramfile *file = (struct __ramfile*)obj_GetPrivate(ob);

    if(RAM_REG==file->type)
    {
        if(__rf_reduce((struct __ramreg*)(file->u), 0))
        {
            return (-1);
        }

        free(file);
    }

    obj_del(ob);
    return (0);
}

// ============================================================================
// ���ܣ�д�ļ���
// ������hdl -- �ļ���������
//      data -- д���ݣ�
//      size  -- д�ߴ磻
// ���أ��ɹ�д�����ݵĳߴ磻
// ��ע��
// ============================================================================
static s32 __rf_write(struct objhandle *hdl, u8 *data, u32 size)
{
    struct __ramfile *file = (struct __ramfile*)handle_val(hdl);
    struct __context *cx = (struct __context*)handle_context(hdl);
    struct __ramreg *reg;
    s32 once, extend, left = size;
    struct __content *tmp;

    if(RAM_REG!=file->type)
        return (0); // Ŀ¼����д��

    reg = (struct __ramreg*)file->u;
    if(isappend(hdl))
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

    if(cx->cpos>__CONTENT_SIZE) // ��seek�������չ��
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
        once = __CONTENT_SIZE - cx->cpos; // ʣ���д�ռ䣻
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
// ���ܣ����ļ���
// ������hdl -- �ļ���������
//      data -- �����ݣ�
//      size  -- ���ߴ磻
// ���أ��ɹ��������ݵĳߴ磻
// ��ע��
// ============================================================================
static s32 __rf_read(struct objhandle *hdl, u8 *data, u32 size)
{
    struct __ramfile *file = (struct __ramfile*)handle_val(hdl);
    struct __context *cx = (struct __context*)handle_context(hdl);
    struct __ramreg *reg;
    s32 left, once, cfree;
    u8 *tmp;

    if(RAM_REG!=file->type)
        return (0); // Ŀ¼����д��

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
            tmp = __rf_content(reg, (cx->cindex+1)); // ��ȡ��һ�������ݣ�
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
// ���ܣ���Ŀ¼�
// ������
// ���أ�ȫ�����꣨1����ʧ�ܣ�-1��������һ�0����
// ��ע��
// ============================================================================
static s32 __rf_readdentry(struct objhandle *hdl, struct dirent *dentry)
{
    struct obj *ob = (struct obj *)dentry->d_ino;

    if(!ob) // ��һ�ζ���
    {
        ob = obj_child(handle2obj(hdl));
        if(!ob)
            return (1); // û������Ŀ��
    }
    else // ��������
    {
        ob = obj_next(ob);
        if(ob==obj_child(handle2obj(hdl)))
            return (1); // ȫ�����ꣻ
    }

    if(!obj_GetPrivate(ob))
        dentry->d_type = DIRENT_IS_DIR;
    else
        dentry->d_type = DIRENT_IS_REG;

    strcpy(dentry->d_name, obj_name(ob));
    dentry->d_ino = (long)ob;
    return (0);
}

// ============================================================================
// ���ܣ����ļ�״̬��
// ������ob -- �ļ�����
//      data -- �ļ�����״̬��
// ���أ��ɹ���0����
// ��ע��
// ============================================================================
static s32 __rf_stat(struct obj *ob, struct stat *data)
{
    struct __ramfile *file = (struct __ramfile*)obj_GetPrivate(ob);
    struct __ramreg *reg;

    if(RAM_REG==file->type) // �ļ�
    {
        reg = (struct __ramreg*)file->u;
        data->st_size = reg->sz; //
        data->st_mode = S_IFREG; //
    }
    else // Ŀ¼
    {
        data->st_size = 0 ; //
        data->st_mode = S_IFDIR; //
    }

    return (0);
}

// ============================================================================
// ���ܣ������ļ���ǰλ�ã�
// ������hdl -- IAP�ļ���������
//      offset -- �ƶ�λ�õ�����
//      whence -- �ƶ�λ�õ���㣻
// ���أ��ɹ���0��*pOffsetָ���µĵ�ǰλ�ã���ʧ�ܣ�-1����
// ��ע��
// ============================================================================
static s32 __rf_seek(struct objhandle *hdl, s64 *offset, s32 whence)
{
    struct __ramfile *file = (struct __ramfile*)handle_val(hdl);
    struct __ramreg *reg;
    struct __context *cx;
    s32 npos, movs, pos, bufed;
    s16 nindex;
    u8 *tmp;

    if(RAM_REG!=file->type)
        return (-1); // ���ļ��޷�seek��

    reg = (struct __ramreg*)file->u;
    cx = (struct __context*)handle_context(hdl);
    pos = cx->cindex * __CONTENT_SIZE + cx->cpos;
    bufed = reg->sz - pos;
    switch(whence)
    {
        case SEEK_END: // תΪ��ͷSEEK�߼�
        {
            *offset = reg->sz + *offset;
            if(*offset<0)
                *offset = 0; // ��λ��Խ����
        }

        case SEEK_SET: // תΪ��ǰλ�õ�SEEK���߼�
        {
            *offset = *offset - pos;
            if((*offset+pos)<0)
                *offset = pos; // ��λ��Խ����
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
                else // ������ǰ�ļ��ߴ�
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
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
static ptu32_t __rf_operations(enum objops ops, ptu32_t oof, ptu32_t args, ...)
{
    va_list list;

    switch(ops)
    {
        case OBJOPEN:
        {
            char *nfile;
            struct obj *ob = (struct obj*)oof;
            u32 flags = (u32)args;

            va_start(list, args);
            nfile = (char*)va_arg(list, u32);
            va_end(list);

            if(strstr(nfile, "/"))
                return (NULL); // ���ļ������ֲ��Ϸ���ֻ������һ����

            return ((ptu32_t)__rf_open(ob, flags, nfile));
        }

        case OBJCLOSE:
        {
            struct objhandle *hdl = (struct objhandle*)oof;
            return ((ptu32_t)__rf_close(hdl));
        }

        case OBJCHILDS:
        {
            struct objhandle *hdl = (struct objhandle*)oof;
            struct dirent *ret = (struct dirent *)args;

            return(__rf_readdentry(hdl, ret));
        }

        case OBJREAD:
        {
            u32 len;
            struct objhandle *hdl = (struct objhandle*)oof;
            u8 *buf = (u8*)args;

            va_start(list, args);
            len = va_arg(list, u32);
            va_end(list);

            return((ptu32_t)__rf_read(hdl, buf, len));
        }

        case OBJWRITE:
        {
            u32 len;
            struct objhandle *hdl = (struct objhandle*)oof;
            u8 *buf = (u8*)args;

            va_start(list, args);
            len = va_arg(list, u32);
            va_end(list);
            return((ptu32_t)__rf_write(hdl, buf, len));
        }

        case OBJSTAT:
        {
            struct stat *data = (struct stat*)args;
            struct obj *ob = (struct obj*)oof;
            char *path;

            va_start(list, args);
            path = (char*)va_arg(list, u32);
            va_end(list);

            if(path&&('\0'!=*path))
                return (-1); // ��ѯ���ļ������ڣ�ramfs���ļ����ǻ����˵ģ�

            return ((ptu32_t)__rf_stat(ob, data));
        }

        case OBJDEL:
        {
            struct obj *ob = (struct obj*)oof;
            char *nexist = (char*)args;

            if(nexist)
                return (-1);

            return((ptu32_t)__rf_remove(ob));
        }

        default:
        {
            printf("\r\n: dbug : ramfs  : do not support this operation now.");
            break;
        }
    }

    return (-1);
}
