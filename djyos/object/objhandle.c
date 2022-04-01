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

static struct MutexLCB s_tHandleMutex; // �ļ�ϵͳ������
static struct MemCellPool s_tHandlePool; // �ļ�Ԥ�����
static struct objhandle s_tHandleInitPool[CFG_HANDLE_LIMIT];

// ============================================================================
// ���ܣ�������ϵͳ������
// �������ޣ�
// ���أ��ޣ�
// ��ע��
// ============================================================================
static inline void __Handle_LockSys(void)
{
    Lock_MutexPend(&s_tHandleMutex, CN_TIMEOUT_FOREVER);
}

// ============================================================================
// ���ܣ�������ϵͳ������
// �������ޣ�
// ���أ��ޣ�
// ��ע��
// ============================================================================
static inline void __Handle_UnlockSys(void)
{
    Lock_MutexPost(&s_tHandleMutex);
}


// ============================================================================
// ���ܣ�������ϵͳ��ʼ����
// ������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
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
s32 Handle_FlagIsDirectory(u32 flags)
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
s32 Handle_FlagIsRegular(u32 flags)
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
s32 Handle_FlagIsCreate(u32 flags)
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
s32 Handle_FlagIsOnlyCreate(u32 flags)
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
s32 Handle_FlagIsAppend(u32 flags)
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
s32 Handle_FlagIsTrunc(u32 flags)
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
s32 Handle_FlagIsReadable(u32 flags)
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
s32 Handle_FlagIsWriteable(u32 flags)
{
    return (flags & O_WRONLY);
}

// ============================================================================
// ���ܣ������豸д�����Ƿ�����ֱ���������ɴ���
// ������flags -- �ļ�ʹ�ñ�־��
// ���أ���1������ֱ���������ɴ��䣻��0��д������������Ϊ��ɣ�
// ��ע�������ж��豸�Ĳ���ģʽ
// ============================================================================
s32 Handle_FlagIsBlockComplete(u32 flags)
{
    if(flags & O_BLOCK_COMPLETE)
        return (1);

    return (0);
}

// ============================================================================
// ���ܣ��������Ƿ�׷��ģʽ��
// ������hdl -- ��������
// ���أ�׷�ӣ�1������׷�ӣ�0����
// ��ע��INLINE
// ============================================================================
s32 Handle_IsAppend(struct objhandle *hdl)
{
    return (Handle_FlagIsAppend(hdl->flags));
}

// ============================================================================
// ���ܣ��������Ƿ�ɶ�ģʽ��
// ������hdl -- ��������
// ���أ��ɶ���1�������ɶ���0����
// ��ע��INLINE
// ============================================================================
s32 Handle_IsReadable(struct objhandle *hdl)
{
    return (Handle_FlagIsReadable(hdl->flags));
}

// ============================================================================
// ���ܣ��������Ƿ��дģʽ��
// ������hdl -- ��������
// ���أ��ɶ���1�������ɶ���0����
// ��ע��INLINE
// ============================================================================
s32 Handle_IsWritable(struct objhandle *hdl)
{
    return (Handle_FlagIsWriteable(hdl->flags));
}

// ============================================================================
// ���ܣ������Ƿ���ھ����ߣ������������ط�ͬʱ��
// ������hdl -- ��������
// ���أ����ڣ�1���������ڣ�0����
// ��ע��INLINE
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
// ���ܣ��½���������
// �������ޣ�
// ���أ��ɹ�������������ʧ�ܣ�NULL����
// ��ע��INLINE
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
// ���ܣ��ͷŶ�������
// ������hdl -- ��������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��INLINE
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
// ���ܣ��������ĳ�ʼ����
// ������hdl -- ��������
//      ob -- �����Ķ���
//      flags���ļ����ʱ�־����fcntl.h�ж��壬�� O_RDONLY
//      context -- ���������ݣ�
// ���أ��ޣ�
// ��ע��
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
// ���ܣ���ȡ��������
// ������hdl -- ������
// ���أ��ɹ����ļ�������ʧ�ܣ�NULL����
// ��ע��INLINE
// ============================================================================
const char *Handle_GetName(struct objhandle *hdl)
{
    if(!hdl)
        return (NULL);

    return (OBJ_GetName(hdl->HostObj));
}

// ============================================================================
// ���ܣ���ȡ���������������ݣ�
// ������hdl -- ��������
// ���أ��ɹ���˽�����ݣ���ʧ�ܣ�NULL��;
// ��ע��INLINE
// ============================================================================
ptu32_t Handle_GetContext(struct objhandle *hdl)
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
void Handle_SetContext(struct objhandle *hdl, ptu32_t context)
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
ptu32_t handle_GetHostObjectPrivate(struct objhandle *hdl)
{
    if(hdl&&hdl->HostObj)
        return (OBJ_GetPrivate(hdl->HostObj));

    return (0);
}

// ============================================================================
// ���ܣ���ȡ�������ĳ�ʱʱ�䣻
// ������hdl -- ��������
// ���أ���ʱʱ�䣻
// ��ע��
// ============================================================================
u32 handle_gettimeout(struct objhandle *hdl)
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
void Handle_SetTimeOut(struct objhandle *hdl, u32 timeout)
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
u32 Handle_GetMode(struct objhandle *hdl)
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
void Handle_SetMode(struct objhandle *hdl, u32 mode)
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
struct Object *Handle_GetHostObj(struct objhandle *hdl)
{
    if(hdl)
        return (hdl->HostObj);

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
inline u32 Handle_MultiEvents(struct objhandle *hdl)
{
    if(hdl)
        return (hdl->MultiplexEvents);

    return (0);
}

// ============================================================================
// ���ܣ���ȡ�������Ķ�·���ÿ��ƣ�
// ������hdl -- ��������
// ���أ��ɹ�����·���ÿ��ƣ���ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
struct MultiplexObjectCB *__Handle_GetMultiplexHead(struct objhandle *hdl)
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
void __Handle_SetMultiplexHead(struct objhandle *hdl, struct MultiplexObjectCB *cb)
{
    if(hdl)
        hdl->pMultiplexHead = cb;
}

// ============================================================================
// ���ܣ����ö������Ķ�·���ô����¼���
// ������hdl -- ��������
//      events -- ��·���ô����¼���
// ���أ��ޣ�
// ��ע��
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
// ���ܣ�����������Ķ�·���ô����¼���
// ������hdl -- ��������
//      events -- ��·���ô����¼���
// ���أ��ޣ�
// ��ע��
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
// ���ܣ����ö�������о���Ķ�·���ô����¼���
// ������ob -- ���������ļ�����
//      events -- ��·���ô����¼���
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 OBJ_SetMultiplexEvent(struct Object *ob, u32 events)
{
    struct objhandle *hdl;
//    u32 MultiplexEvents;
    list_t *head, *cur;
    extern bool_t __Multiplex_Set(s32 Fd, u32 dwAccess);

    if(!ob)
        return (-1);

    head = &ob->handles; // head�ǲ���Ҫ���õģ�
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
// ���ܣ������������о���Ķ�·���ô����¼���
// ������ob -- ���������ļ�����
//      events -- ��·���ô����¼���
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 OBJ_ClrMultiplexEvent(struct Object *ob, u32 events)
{
    struct objhandle *hdl;
//    u32 MultiplexEvents;
    list_t *head, *cur;
    extern bool_t __Multiplex_Set(s32 Fd, u32 dwAccess);

    if(!ob)
        return (-1);

    head = &ob->handles;             // ͷ���ǲ���Ҫ���õģ�
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

//----IO״̬--------------------------------------------------------------------
//���ܣ�����һ��Ϊ select ����׼���Ļ���������Ϊ�˼���һЩ��Դ������裬���ʱ������ʹ��
//      select������Ӧ��ʹ�ø���Ч����ݵ� Multiplex_Wait ϵ�к���
//������Fd�����жϵ��ļ�
//     mode������ע��״̬���ɶ�����д����������
//���أ�1=actived��0=unactive
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
// ���ܣ����ļ�
// ������pathname -- �ļ�·��;
//      flags -- ������־;fcntl.h�� O_RDONLY ��
//      mode -- ����ģʽ;flags������create��־�������壬stat.h�� S_IRUSR ��
// ���أ��ļ���������ʧ��==NULL
// ��ע��O_RDONLY��O_WRONLY��O_RDWR �����POSIX��2016�������¶���
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
    OBJ_DutyUp(ob);        // ��ֹ�ļ����������У���ɾ���ˣ�

//  __Handle_UnlockSys();
        //todo:Ȩ�޹�����δʵ�֡���ܣ�����stat�����жϵ�ǰst_mode�Ƿ�����flagsȨ��
//      StatResult = ob->ObjOps(CN_OBJ_CMD_STAT, (ptu32_t)ob, &statbuf, uncached, full);
//  if(Ȩ������Ҫ��)
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
// ���ܣ��ر��ļ�
// ������hdl -- ������;
// ���أ��ɹ���0��;ʧ�ܣ�-1����
// ��ע��
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
        OBJ_ReleaseTempPath(ob); // �ͷŶ�����ʱ·��
        return (0);
    }
    else
        return (-1);
}

// ============================================================================
// ���ܣ����ļ�
// ������pathname -- �ļ�·��;
//      flags -- ������־;fcntl.h�� O_RDONLY ��
//      mode -- ����ģʽ;flags������create��־�������壬stat.h�� S_IRUSR ��
// ���أ�ʧ�ܣ�-1��; �ɹ����ļ�����������-1����;
// ��ע��O_RDONLY��O_WRONLY��O_RDWR �����POSIX��2016�������¶���
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
// ���ܣ��ر��ļ�
// ������fd -- �ļ�������;
// ���أ��ɹ���0��;ʧ�ܣ�-1����
// ��ע��
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
// ���ܣ�ɾ���ļ���
// ������path -- �ļ�·����
// ���أ��ɹ���0���� ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 remove(const char *path)
{
    struct Object *ob;
    char *uncached;
    s32 res;

    __Handle_LockSys();
    ob = OBJ_MatchPath(path, &uncached);
    if(!uncached) // �ļ��Ѿ���ϵͳ��
    {
        if(OBJ_IsOnDuty(ob))  // �ļ����ڱ�ʹ����
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
        OBJ_ReleaseTempPath(ob); // �ͷŶ�����ʱ·��
        return (0);
    }
    else
        return (-1);

}

// ============================================================================
// ���ܣ����ļ���
// ������fd -- �ļ���������
//      buf -- �����壻
//      count -- ��Ҫ�������ݴ�С��Byte�ƣ���
// ���أ��ɹ���ʵ�ʶ�ȡ���ĳ��ȣ�������-1����
// ��ע��
// ============================================================================
ssize_t read(s32 fd, void *buf, size_t size)
{
    size_t result;
    struct objhandle *hdl;
    s32 res = -1;

    if(NULL == buf)
        return (-1); // ��������

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
// ���ܣ�д�ļ���
// ������fd -- �ļ���������
//      buf -- д���壻
//      count -- ��Ҫд�����ݴ�С��Byte�ƣ���
// ���أ� �ɹ���ʵ��д��ĳ��ȣ�������-1����
// ��ע��
// ============================================================================
ssize_t write(s32 fd, const void *buf, size_t count)
{
    size_t result;
    struct objhandle *hdl;
    s32 res = -1;

    if(!buf)
        return (-1); // ��������

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
// ���ܣ��ļ��ض�λ
// ������fd -- �ļ���������
//      offset -- �ļ�ƫ�ã�
//      whence -- �ļ�ƫ�õ���㣻
// ���أ��ɹ����ض�λ���λ�ã���ʧ�ܣ�-1����
// ��ע��NOTE: lseek��֧���ļ���С��չ
// ============================================================================
off_t lseek(s32 fd, off_t offset, s32 whence)
{
    struct objhandle *hdl;
    s32 res;
    off_t result = -1;

    if((SEEK_SET != whence) && (SEEK_CUR != whence) && (SEEK_END != whence))
        return (-1); // ����Ĳ���

    if((SEEK_SET == whence) && (offset < 0))
        return (-1); // ������߼�

    hdl = fd2Handle(fd);
    if(!hdl)
        return (-1);

    if(Handle_FlagIsDirectory(hdl->flags))
        return (-1); // Ŀ¼������seek

    res = hdl->HostObj->ObjOps((void *)hdl, CN_OBJ_CMD_SEEK, (ptu32_t)&result,
                             (ptu32_t)&offset, whence);
    if(res == CN_OBJ_CMD_EXECUTED)
        return (result);
    else
        return (-1);
}

// ============================================================================
// ���ܣ��ļ�ͬ����
// ������fd -- �ļ���������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 fsync(s32 fd)
{
    struct objhandle *hdl;
    s32 res;

    hdl = fd2Handle(fd);
    if(!hdl)
    {
        errno = EBADF; // ��������
        return (-1);
    }

    res = hdl->HostObj->ObjOps((void *)hdl, CN_OBJ_CMD_SYNC, 0, 0, 0);
    if(res == CN_OBJ_CMD_TRUE)
        return (0);
    else
        return (-1);
}

// ============================================================================
// ���ܣ���ȡ�ļ��ĵ�ǰλ�ã�
// ������fd -- �ļ���������
// ���أ��ɹ����ļ���ǰλ�ã���ʧ�ܣ�-1����
// ��ע��
// ============================================================================
off_t tell(s32 fd)
{
    struct objhandle *hdl;
    off_t offset;
    s32 res;

    hdl = fd2Handle(fd);
    if(!hdl)
    {
        errno = EBADF; // ��������
        return (-1);
    }

    res = hdl->HostObj->ObjOps((void *)hdl, CN_OBJ_CMD_TELL, (ptu32_t)&offset,0,0);
    if(res == CN_OBJ_CMD_EXECUTED)
        return (offset);
    else
        return -1;
}

// ============================================================================
//���ܣ���ѯ�ļ�״̬��
//������fd -- �ļ���������
//     buf -- �ļ�״̬;
//���أ��ɹ���0��; ʧ�ܣ�-1��;
//��ע��
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
// ���ܣ���ѯ�ļ�״̬��
// ������path -- �ļ�·����
//      buf -- �ļ�״̬;
// ���أ��ɹ���0��; ʧ�ܣ�-1��;
// ��ע��
// ============================================================================
s32 stat(const char *path, struct stat *buf)
{
    struct Object *ob;
    char *uncache;
    s32 res;

    if((!buf) || (!path))
        return(-1);

    __Handle_LockSys();// ��ֹ���������ļ���ɾ����
    ob = OBJ_MatchPath((char*)path, &uncache);
    OBJ_DutyUp(ob);
    __Handle_UnlockSys();

    if(!uncache)
        uncache = ""; // ȫ��·�����Ѿ�����ʱ������Ϊ���ַ�������'\0'����������fstat�߼����֣�

    res = (s32)ob->ObjOps((void *)ob, CN_OBJ_CMD_STAT, (ptu32_t)buf, 0,
                                    (ptu32_t)uncache);
    OBJ_DutyDown(ob);
    if(res == CN_OBJ_CMD_TRUE)
        return 0;
    else
        return (-1);
}

// ============================================================================
// ���ܣ��趨�ļ����ȣ�
// ������fd -- �ļ���������
//      length -- �ļ����ȣ�
// ���أ��ɹ���0��; ʧ�ܣ�-1��;
// ��ע��the file offset is not changed.
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
//���ܣ�ɾ���ļ���Ŀ¼
//������pathname -- �ļ�·����
//���أ�0 -- �ɹ�; -1 -- ʧ��;
//��ע:
// ============================================================================
s32 unlink(const char *pathname)
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
// ���ܣ���Ŀ¼�
// ������dir -- �Ѵ򿪵�Ŀ¼��
// ���أ�Ŀ¼�ȫ���Ѷ�����NULL��
// ��ע��ÿ�εķ���ֵ������ͬһ���ռ䣬����������Ϊ��ʡ�ռ䣻
// ============================================================================
struct dirent *readdir(DIR *dir)
{
    struct objhandle *hdl;
    struct Object *ob;
    s32 res;

    if(!dir)
        return (NULL);

    hdl = (struct objhandle*)(dir->__fd); // Ŀ¼��������

    ob = hdl->HostObj;      // Ŀ¼�Ľڵ�
    res = ob->ObjOps((void *)ob, CN_OBJ_CMD_READDIR, (ptu32_t)&dir->__ptr, 0, (ptu32_t)hdl);
    if(res == CN_OBJ_CMD_TRUE)
        return (struct dirent*)&(dir->__ptr);
    else
        return (NULL); // ��ȫ��������
}

// ============================================================================
// ���ܣ��ر�Ŀ¼
// ������dir -- Ŀ¼��
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
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
// ���ܣ�����Ŀ¼
// ������filename -- Ŀ¼����·������
//      mode -- ģʽ��
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע�����Ŀ¼�Ѵ��ڣ�����ʧ�ܣ�
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
// ���ܣ�������������ļ����ƣ�
// ������hdl����������
//      cmd����������;
//      argspace��������������ռ䣻
// ���أ�δ֧�֣�-1�����ɹ���0��
// ��ע��
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
// ���ܣ��ļ����ƣ�
// ������fd���ļ���������
//      cmd����������� fcntl.h �е� F_DUPFD �ȳ������塣
//      ...���������������
// ���أ�cmd = F_DUPFD�������µ��ļ���������
//      cmd = F_GETFD��F_GETFL��������Ӧ��־
//      cmd = F_GETOWN������һ�����Ľ���ID�򸺵Ľ�����ID��
//      cmd = ������-1 = ��������ֵ = �ɹ���
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
// ���ܣ�IO���ƣ�
// ������fd���ļ���������
//      request�����������룬POSIXҲû�й涨��Linux��ʽΪ������(2bit)������������
//      �����У����ݳߴ�(14bit)���豸���� (8it) ���������(8bit)������������ stropts.h
//      �е� I_ATMARK �ȳ�������
//      ...,�������������
// ���أ�0 = �ɹ���-1 = ����
// ��ע��
//      ���µ�POSIX�Ѿ�������ioctl��������Ȼ���ѣ�������Դ��Ŀ��Ȼ���á�
//      ע����ֵ�POSIX��ioctl��صĳ�����ͷ�ļ����� stropts.h ������ ioctl.h
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
//      C����Щ��Դ��Ŀ�������ǵ�iotjs���Զ�����һЩioctl�������A��B�������С�
//      djyos�������ʵ�ַ�����
//      1��ʹ�õ�һ����������
//      2��ϵͳ�Դ���ģ�飬��ʹ�õ��ĸ��������������������õģ��ýṹָ��
//      3��Ŀǰ��δ����ʹ�õ��ĸ������Ŀ�Դ���룬djyosʵ���ϲ�����֧�֡�
//      4��djyos��IOϵͳֻ�ṩ�ӿڣ�ʵ���Ƿ�֧��ioctl�Ĺ��ܣ������ʵ�������о�����
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
// ���ܣ�����ǰ����Ŀ¼�ľ���·�����Ƶ�����buf��
// ������buf -- ���·���Ŀռ䣻
//      size -- buf�Ĵ�С��
// ���أ��ɹ�����ǰ����Ŀ¼�ľ���·������ʧ�ܣ�NULL��
// ��ע��bufΪ NULL��ʹ sizeΪ�㣬�򱾺����ᶯ̬����һ���ռ������ظ��û��������û���Ҫע�⽫�ռ��ͷţ�
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
// ���ܣ��ı䵱ǰ����Ŀ¼��
// ������path -- �µĵ�ǰ����Ŀ¼
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 chdir(const char *path)
{
    path = path;
    return (-1); // TODO; NOTE:���õ�·����һ��Ŀ¼
}

#if 0
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
static struct objhandle *__rf_open(struct Object *ob, u32 flags, char *name)
{
    struct objhandle *hdl;
    struct __ramfile *file = NULL;
    struct __ramreg *reg = NULL;
    struct __context *cx = NULL;

    if(name) // ��Ҫ�½��ļ���Ŀ¼��
    {
        if(!Handle_FlagIsCreate(flags))
            return (NULL); // �߼��ϲ�Ҫ�󴴽�����ʵ���ļ�ȴ�����ڣ�

        if(Handle_FlagIsRegular(flags)) // �����ļ���
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
//          if(obj_allow2set(ob)) // ����Ŀ¼����֮�Ͻ������󼯺ϣ�������Ŀ¼֮�Ϲ����ļ�ϵͳ��
            {
                free(reg);
                OBJ_Delete(ob);
                return (NULL);
            }
        }
    }
    else // �ļ���Ŀ¼�Ѵ��ڣ�
    {
        if(Handle_FlagIsOnlyCreate(flags))
            return (NULL); // �߼��ϲ���Ҫ�󣻱��봴�������Ѵ��ڣ�

        file = (struct __ramfile*)OBJ_GetPrivate(ob);
        if(Handle_FlagIsRegular(flags)) // ���ļ���
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
        else // �򿪷��ļ���
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
// ���ܣ��ر��ļ���
// ������hdl -- �ļ���������
// ���أ�
// ��ע��
// ============================================================================
static s32 __rf_close(struct objhandle *hdl)
{
    struct __context *cx = (struct __context*)Handle_GetContext(hdl);

    if(cx) // Ŀ¼û��__context;
    {
        __rf_down_content(cx->content);
        free(cx);
    }

    Handle_Delete(hdl);
    return (0);
}

// ============================================================================
// ���ܣ�ɾ���ļ���
// ������
// ���أ�
// ��ע��ɾ����ʱ�򣬶����Ѿ�����ס���������µķ��ʣ�
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
// ���ܣ�д�ļ���
// ������hdl -- �ļ���������
//      data -- д���ݣ�
//      size  -- д�ߴ磻
// ���أ��ɹ�д�����ݵĳߴ磻
// ��ע��
// ============================================================================
static s32 __rf_write(struct objhandle *hdl, u8 *data, u32 size)
{
    struct __ramfile *file = (struct __ramfile*)handle_GetHostObjectPrivate(hdl);
    struct __context *cx = (struct __context*)Handle_GetContext(hdl);
    struct __ramreg *reg;
    s32 once, extend, left = size;
    struct __content *tmp;

    if(RAM_REG!=file->type)
        return (0); // Ŀ¼����д��

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
    struct __ramfile *file = (struct __ramfile*)handle_GetHostObjectPrivate(hdl);
    struct __context *cx = (struct __context*)Handle_GetContext(hdl);
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
    struct Object *ob = (struct Object *)dentry->d_ino;

    if(!ob) // ��һ�ζ���
    {
        ob = OBJ_GetChild(Handle_GetHostObj(hdl));
        if(!ob)
            return (1); // û������Ŀ��
    }
    else // ��������
    {
        ob = OBJ_GetNext(ob);
        if(ob==OBJ_GetChild(Handle_GetHostObj(hdl)))
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
// ���ܣ����ļ�״̬��
// ������ob -- �ļ�����
//      data -- �ļ�����״̬��
// ���أ��ɹ���0����
// ��ע��
// ============================================================================
static s32 __rf_stat(struct Object *ob, struct stat *data)
{
    struct __ramfile *file = (struct __ramfile*)OBJ_GetPrivate(ob);
    struct __ramreg *reg;

    if(RAM_REG==file->type) // �ļ�
    {
        reg = (struct __ramreg*)file->u;
        data->st_size = reg->sz; //
        data->st_mode = S_IFREG|S_IRWXUGO; //
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
    struct __ramfile *file = (struct __ramfile*)handle_GetHostObjectPrivate(hdl);
    struct __ramreg *reg;
    struct __context *cx;
    s32 npos, movs, pos, bufed;
    s16 nindex;
    u8 *tmp;

    if(RAM_REG!=file->type)
        return (-1); // ���ļ��޷�seek��

    reg = (struct __ramreg*)file->u;
    cx = (struct __context*)Handle_GetContext(hdl);
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
                return (-1);    // ��ѯ���ļ������ڣ�ramfs���ļ����ǻ����˵ģ�
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
