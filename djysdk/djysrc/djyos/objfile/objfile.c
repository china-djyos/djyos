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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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
#include "__object.h"
#include "../include/object.h"
#include "../include/objfile.h"
#include "dbug.h"
#define CN_BASIC_OFILES               32 // �ļ�Ԥ�ֵ�����������̬��չ��

static struct MutexLCB *ofmutex; // �ļ�ϵͳ������
static struct MemCellPool ofpool; // �ļ�Ԥ�����

// �����ļ��������ָ�룬��open���ص�fd��s32��������Ҫ�ж�Ӧ��ϵ
// of �� fd ��Ȼ�ܹ�����ֱ��ת����������
// 1��ָ�볤��С��32
// 2��ָ�볤��==32�����ֽ�λ��32
#if(( CN_PTR_BITS < 32)  || ( (CN_PTR_BITS == 32) && (CN_BYTE_BITS < 32)))
// ============================================================================
// ���ܣ��ڲ��ļ�����ת�ļ����
// ������
// ���أ�
// ��ע��
// ============================================================================
s32 ofno(tagOFile *of)
{
    if(!of)
        return (-1);

    return (s32)(((u32)of) >> 1);
}

// ============================================================================
// ���ܣ��ļ����ת�ļ�����
// ������
// ���أ�
// ��ע��
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
// ������Ȼ�໥ת��������͸��Ӷ��ˡ�
// ��������object��ʹ�õ��ڴ�ռ䷶Χ��ֻҪ������2G���ڣ��Ϳ��Ի�ת�ˡ�
#error FD logic is not supported.
#endif

// ============================================================================
// ���ܣ��ļ������Ŀռ�س�ʼ��
// ������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
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
// ���ܣ��½��ļ�������
// ������
// ���أ��ɹ����ļ������ģ���ʧ�ܣ�NULL����
// ��ע��
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
// ���ܣ��ͷ��ļ�������
// ������of -- �ļ������ģ�
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
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
// ���ܣ��ļ������ĳ�ʼ��
// ������of -- �ļ������ģ�
//      obj -- ����
//      flags -- �ļ�ʹ�ñ�־��
//      context -- �ļ�ʵ�������ģ�
// ���أ�
// ��ע��TODO :INLINE
// ============================================================================
inline void of_init(tagOFile *of, struct Object *obj, u32 flags, ptu32_t context)
{
    of->obj = obj;
    of->context = context;
    of->flags = flags;
}

// ============================================================================
// ���ܣ�
// ������target -- ��װ�㣻
//      pTargetRoot -- ��װ��ĸ�·����
//      ops -- ������ʽ��
//      pPrivate -- ��װ�ڵ�ı�ʾ��
// ���أ�
// ��ע��
// ============================================================================
struct Object *__mounto(const char *target, char *pTargetRoot, tagObjOps ops, ptu32_t represent)
{
    struct Object *obj;
    char *unfinish; //

    obj = OBJ_MatchPath(pTargetRoot, &unfinish, 0);
    if(unfinish)
    {
        return (NULL); // ��װ��ĸ�·���������
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
// ���ܣ���ȡ�ļ���
// ������of -- �ļ����ģ�
// ���أ������ļ���1�����������ļ���0����
// ��ע��INLINE
// ============================================================================
inline const char *of_name(tagOFile *of)
{
    if(!of)
        return (NULL);

    return (__OBJ_Name(of->obj));
}

// ============================================================================
// ���ܣ��Ƿ������ļ�
// ������flags -- �ļ�ʹ�ñ�־��
// ���أ������ļ���1�����������ļ���0����
// ��ע��INLINE
// ============================================================================
inline s32 testcontent(u32 flags)
{
    if(flags & O_FILE_BASIC)
        return (1);

    return (0);
}

// ============================================================================
// ���ܣ��Ƿ�Ŀ¼
// ������flags -- �ļ�ʹ�ñ�־��
// ���أ�Ŀ¼��1������Ŀ¼��0����
// ��ע��INLINE
// ============================================================================
inline s32 testdirectory(u32 flags)
{
    if(flags & O_DIRECTORY)
        return (1);

    return (0);
}

// ============================================================================
// ���ܣ��Ƿ��ļ�
// ������flags -- �ļ�ʹ�ñ�־��
// ���أ��ļ���1�������ļ���0����
// ��ע��INLINE
// ============================================================================
inline s32 testregular(u32 flags)
{
    if(flags & O_DIRECTORY)
        return (0);

    return (1);
}

// ============================================================================
// ���ܣ��Ƿ񴴽���򿪣�
// ������flags -- �ļ�ʹ�ñ�־��
// ���أ�������򿪣�1�����Ǵ�����򿪣�0����
// ��ע��INLINE
// ============================================================================
inline s32 testcreat(u32 flags)
{
    if(flags & O_CREAT)
        return (1);

    return (0);
}

// ============================================================================
// ���ܣ��Ƿ��������
// ������flags -- �ļ�ʹ�ñ�־��
// ���أ���������1�����ǽ�������0����
// ��ע��INLINE
// ============================================================================
inline s32 testonlycreat(u32 flags)
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
inline s32 testappend(u32 flags)
{
    if(flags & O_APPEND)
        return (1);

    return (0);
}

// ============================================================================
// ���ܣ��Ƿ�ض�
// ������flags -- �ļ�ʹ�ñ�־��
// ���أ��ضϣ�1�����ǽضϣ�0����
// ��ע��INLINE
// ============================================================================
inline s32 testtrunc(u32 flags)
{
    if(flags & (O_TRUNC))
        return (1);

    return (0);
}

// ============================================================================
// ���ܣ��Ƿ�ɶ�
// ������flags -- �ļ�ʹ�ñ�־��
// ���أ��ɶ���1�����ǿɶ���0����
// ��ע��INLINE
// ============================================================================
inline s32 testreadable(u32 flags)
{
    if(flags & O_RDONLY)
        return (1);

    return (0);
}

// ============================================================================
// ���ܣ��Ƿ��д
// ������flags -- �ļ�ʹ�ñ�־��
// ���أ���д��1�����ǿ�д��0����
// ��ע��INLINE
// ============================================================================
inline s32 testwriteable(u32 flags)
{
    return (flags & O_WRONLY);
}

// ============================================================================
// ���ܣ��ļ��Ƿ���Ŀ¼
// ������
// ���أ�Ŀ¼��1������Ŀ¼��0����
// ��ע��
// ============================================================================
inline s32 isdirectory(tagOFile *of)
{
    return(testdirectory(of->flags));
}

// ============================================================================
// ���ܣ��ļ��ڵ��Ƿ���Ŀ¼
// ������
// ���أ�Ŀ¼��1������Ŀ¼��0����
// ��ע��
// ============================================================================
s32 isdirectoryo(struct Object *obj)
{
    struct __portBasic *basic;

    basic = dListEntry(obj->port.next, struct __portBasic, list);
    return (testdirectory(basic->flags));
}

// ============================================================================
// ���ܣ��ļ��Ƿ�׷��ģʽ��
// ������flags -- �ļ�ʹ�ñ�־��
// ���أ�׷�ӣ�1������׷�ӣ�0����
// ��ע��INLINE
// ============================================================================
inline s32 isappend(tagOFile *of)
{
    return (testappend(of->flags));
}

// ============================================================================
// ���ܣ��ļ��Ƿ�ɶ�
// ������
// ���أ�
// ��ע��
// ============================================================================
inline s32 isreadable(tagOFile *of)
{
    return (testreadable(of->flags));
}

// ============================================================================
// ���ܣ��ļ��Ƿ��д
// ������
// ���أ�
// ��ע��
// ============================================================================
inline s32 iswritable(tagOFile *of)
{
    return (testwriteable(of->flags));
}

// ============================================================================
// ���ܣ������ļ��Ƿ����ڱ�ʹ��
// ������
// ���أ��ļ�ʹ���У�1�����ļ�δʹ�ã�0��
// ��ע��
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
// ���ܣ��ļ��Ƿ���ھ����ߣ����ļ��������ط�ͬʱ��
// ������of -- �ļ���
// ���أ����ڣ�1���������ڣ�0����
// ��ע��INLINE
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
// ���ܣ���ȡ�ļ�˽�������ģ�
// ������of -- �ļ���
// ���أ��ļ�˽�������ģ�
// ��ע���ڲ����ã����жϲ�����
// ============================================================================
inline ptu32_t of_context(tagOFile *of)
{
    if(of)
        return (of->context);

    return (0);
}

// ============================================================================
// ���ܣ������ļ�˽�������ģ�
// ������of -- �ļ���
//      context -- ˽�������ģ�
// ���أ�ʧ�ܣ�-1�����ɹ���0��
// ��ע��
// ============================================================================
inline void of_setcontext(tagOFile *of, ptu32_t context)
{
    if(of)
        of->context = context;
}

// ============================================================================
// ���ܣ���ȡ�ļ���ʾ��
// ������of -- �ļ���
// ���أ��ļ���ʾ
// ��ע���ڲ����ã����жϲ�����
// ============================================================================
inline ptu32_t of_represent(tagOFile *of)
{
    if(of&&of->obj)
        return (of->obj->represent);

    return (0);
}

// ============================================================================
// ���ܣ������ļ���ʾ��
// ������of -- �ļ���
// ���أ��ļ���ʾ
// ��ע���ڲ����ã����жϲ�����
// ============================================================================
inline void of_setrepresent(tagOFile *of, ptu32_t represent)
{
    if(of&&of->obj)
        of->obj->represent = represent;
}

// ============================================================================
// ���ܣ����ļ����ļ��ڵ����
// ������
// ���أ�
// ��ע�� TODO:����
// ============================================================================
s32 of_linko(tagOFile *of, struct Object *obj)
{
    if(dListIsEmpty(&of->list))
    {
        // obj����content����Ա���������ļ�ϵͳû�У���obj������������β��
        dListInsertBefore(&obj->port, &of->list);
    }
    else if(dListIsEmpty(&obj->port))
    {
        // pOF����content����Ա��obj���½��ģ���list�ĵ�һ����Ա������ocntent
        dListInsertAfter(&of->list, &obj->port);
    }
    else
    {
        return (-1); // ����������߼�
    }

    of->obj = obj;
    return (0);
}

// ============================================================================
// ���ܣ���ȡ�ļ��Ķ�·���ã�
// ������
// ���أ�
// ��ע��
// ============================================================================
inline struct MultiplexObjectCB *of_mulitplex(tagOFile *of)
{
    if(of)
        return (of->mphead);

    return (NULL);
}

// ============================================================================
// ���ܣ���ȡ�ļ��Ķ�·���ã�
// ������
// ���أ�
// ��ע��
// ============================================================================
inline void of_setmulitplex(tagOFile *of, struct MultiplexObjectCB *cb)
{
    if(of)
        of->mphead = cb;
}

// ============================================================================
// ���ܣ���ȡ�ļ��ĳ�ʱ���ã�
// ������
// ���أ�
// ��ע��
// ============================================================================
inline u32 of_timeout(tagOFile *of)
{
    if(of)
        return (of->timeout);

    return (0);
}

// ============================================================================
// ���ܣ������ļ��ĳ�ʱ��
// ������
// ���أ�
// ��ע��
// ============================================================================
inline void of_settimeout(tagOFile *of, u32 timeout)
{
    of->timeout = timeout;
}


// ============================================================================
// ���ܣ���ȡ�ļ���ʹ�ñ�־��
// ������
// ���أ�
// ��ע��
// ============================================================================
inline u32 of_mode(tagOFile *of)
{
    if(of)
        return (of->flags);

    return (0);
}

// ============================================================================
// ���ܣ��ļ���Ӧ���ļ��ڵ㣻
// ������
// ���أ�
// ��ע��INLINK
// ============================================================================
inline struct Object *of_o(tagOFile *of)
{
    if(of)
        return (of->obj);

    return (NULL);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
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
// ���ܣ�
// ������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
inline void *of_core(tagOFile *of)
{
    extern void *FS_Core(struct Object *obj);

    return (FS_Core(of->obj));
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��INLINK
// ============================================================================
inline struct __portBasic *of_basic(tagOFile *of)
{
    if(of)
        return (of_basiclinko(of->obj));

    return (NULL);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��INLINK
// ============================================================================
inline struct Object *basic2obj(struct __portBasic *basic)
{
    struct Object *obj;

    obj = dListEntry(dListGetBefore(&basic->list), struct Object, port);
    return (obj);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע���������е��ļ�ϵͳ���ļ������ݱ�����PortHead�ĵ�һ�����������ڴ��������
// ============================================================================
static inline s32 __basiclinko(struct __portBasic *basic, struct Object *obj)
{
    if(-1 == __OBJ_LinksCheck(obj, 0))
        return (-1); // �������߼�

    __OBJ_Link(obj, &basic->list);
    return (0);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע���������е��ļ�ϵͳ���ļ������ݱ�����PortHead�ĵ�һ�����������ڴ��������
// ============================================================================
inline void of_initbasic(struct __portBasic *basic, u32 flags)
{
    dListInit(&basic->list);
    basic->flags = flags | O_FILE_BASIC;
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
inline void of_unlinkbasic(struct __portBasic *basic)
{
    dListRemove(&basic->list);
}

// ============================================================================
// ���ܣ����ļ�ϵͳ��parent�ڵ�������һ���ļ�
// ������
// ���أ�
// ��ע��INLINK
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
// ���ܣ����ļ�ϵͳ��parent�ڵ�������һ���ļ�
// ������
// ���أ�
// ��ע��INLINK
// ============================================================================
s32 of_virtualize(struct Object *parent, struct __portBasic *basic, char *name)
{
    if(of_virtualizeo(parent, basic, name))
        return (0);

    return (-1);
}


// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��TODO:��ʱ�ļ�ʱ����contextָ���ļ��ṹ����Ŀ¼ʱ����Ϊ�գ�
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
        return (NULL); // �½��ļ��߼�У��

    if(testregular(flags)) // Ŀǰ��֧�ִ����ļ���
    {
        debug_printf("objfile","ramfs open failed<do not support file related operation>.");
        return (NULL);
    }

    if(new) // ��Ҫ�½��ļ���Ŀ¼
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
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 __rf_close(tagOFile *of)
{
    of_free(of);
    return (0);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 __rf_remove(struct Object *obj)
{
    debug_printf("objfile","ramfs do not support file now.");
    return (0);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 __rf_write(tagOFile *of, u8 *pBuf, u32 dwSize)
{
    debug_printf("objfile","ramfs do not support file now.");
    return (0);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 __rf_read(tagOFile *of, u8 *pBuf, u32 dwSize)
{
    debug_printf("objfile","ramfs do not support file now.");
    return (0);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 __rf_readdentry(tagOFile *pDirectory, struct dirent *pDirent)
{
    debug_printf("objfile","ramfs do not support file now.");
    return (0);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 __rf_stat(struct Object *obj, struct stat *data)
{
#if 0
    struct __ramfile *file;
    file = dListEntry(of_basiclinko(obj), struct __ramfile, basic);
#endif
    memset(data, 0x0, sizeof(struct stat));
    data->st_size = 0 ; // ��ǰ��δʵ���ļ���
    data->st_mode = S_IFDIR; // ��ǰ��δʵ���ļ���
    return (0);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
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
                return (-1); // ��ѯ���ļ������ڣ�

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
// ���ܣ�
// ������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
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

    __OBJ_SetOps(OBJ_Root(), __rf_operations); // ȱʡ��װ���ǻ���RAM���ļ�ϵͳ

    return (0);
}

#if 0
// ============================================================================
// ���ܣ������ļ����������һ�������´�����ȫ���ļ��������÷���
//      tagOFile *Current = NULL;
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
        if(testcontent(reflagsode)) // Խ��content
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
// ���ܣ������ļ���ǰ��־��Ա����
// ������
// ���أ�
// ��ע��
// ============================================================================
void of_setmode(tagOFile *of, u32 flags)
{
    if(of)
        of->flags |= flags;
}

// ============================================================================
// ���ܣ���ȡ�ļ�access��Ա��
// ������
// ���أ�
// ��ע��
// ============================================================================
inline u32 of_access(tagOFile *of)
{
    if(of)
        return (of->access);

    return (0);
}

// ============================================================================
// ���ܣ������ļ���·���ô����¼�
// ������
// ���أ�
// ��ע��TODO:����
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
// ���ܣ����ļ��Ķ�·���ô����¼���
// ������
// ���أ�
// ��ע��TODO:����
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
// ���ܣ��������ѱ����ļ�������һ���ļ�������û��򿪣��Ķ�·���õĴ���������
// ������obj -- ���������ļ�����
//      events -- �ļ���ǰ״̬��ֻ�е�24bit��Ч��bitmask����1��λ��AccessStatus����Ӧ
//          ��bit����1���μ� CN_FDFLAG_IN �ȶ���
// ���أ���
// ��ע��
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
            continue; // Խ��content

        access = of->access;
        of->access |= events;
        if(access != of->access)
            __Multiplex_Set(ofno(of), of->access);
    }
}

// ============================================================================
// ���ܣ��������ѱ����ļ�������һ���ļ�������û��򿪣��Ķ�·���õĴ���������
// ������Object����SetObjectAccessStatus����
//      events����SetObjectAccessStatus����
// ���أ���
// ��ע��
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
// ���ܣ����ļ�
// ������pathname -- �ļ�·��;
//      flags -- ������־;
//      mode -- ����ģʽ;
// ���أ�ʧ�ܣ�-1��; �ļ�����ţ���-1��;
// ��ע��O_RDONLY��O_WRONLY��O_RDWR �����POSIX��2016�������¶���
// ============================================================================
tagOFile *__open(char *pPath, s32 flags, u32 mode)
{
    tagOFile *of;
    struct Object *obj;
    char *full, *uncached;

    Lock_MutexPend(ofmutex, CN_TIMEOUT_FOREVER);
    obj = OBJ_MatchPath(pPath, &uncached, &full);
    __OBJ_Protect(obj); // ��ֹ�ļ����������У���ɾ����
    Lock_MutexPost(ofmutex); // ������

    of = (tagOFile*)obj->ops(CN_OBJ_CMD_OPEN, (ptu32_t)obj, (ptu32_t)flags, uncached, mode, full);

    Lock_MutexPend(ofmutex, CN_TIMEOUT_FOREVER);
    __OBJ_Release(obj);
    if(of)
    {
        obj = OBJ_BufferPath(obj, uncached); // �����ļ�ϵͳ���ԣ�������Ҫ�����Ŀ¼·��,�̳в���������.
        of_linko(of, obj);
    }

    Lock_MutexPost(ofmutex); // ������

    return (of);
}


// ============================================================================
// ���ܣ�ֱ�ӽ�������Ϊһ���ļ��򿪣�
// ������obj -- ����
// ���أ��ɹ����ļ��ţ���ʧ�ܣ�-1����
// ��ע��
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
// ���ܣ��ر��ļ�
// ������fd -- �ļ������;
// ���أ��ɹ���0��;ʧ�ܣ�-1����
// ��ע��
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
// ���ܣ����ļ�
// ������pathname -- �ļ�·��;
//      flags -- ������־;
//      mode -- ����ģʽ;
// ���أ�ʧ�ܣ�-1��; �ļ�����ţ���-1��;
// ��ע��O_RDONLY��O_WRONLY��O_RDWR �����POSIX��2016�������¶���
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
// ���ܣ��ر��ļ�
// ������fd -- �ļ������;
// ���أ��ɹ���0��;ʧ�ܣ�-1����
// ��ע��
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
// ���ܣ�ɾ���ļ���
// ������path -- �ļ�·����
// ���أ��ɹ���0���� ʧ�ܣ�-1����
// ��ע��
// ============================================================================
int remove(const char *path)
{
    struct Object *obj;
    char *uncached, *full;
    s32 res;

    Lock_MutexPend(ofmutex, CN_TIMEOUT_FOREVER);
    obj = OBJ_MatchPath(path, &uncached, &full);

    if(!uncached) // �ļ��Ѿ���ϵͳ��
    {
        if((GROUP_POINT == __OBJ_Type(obj)) || // ��װ�㲻��ɾ��
           (isondutyo(obj)))  // �ļ����ڱ�ʹ����
        {
            Lock_MutexPost(ofmutex); // ������
            return (-1);
        }
    }

    __OBJ_Protect(obj);
    Lock_MutexPost(ofmutex); // ������

    res = (s32)obj->ops(CN_OBJ_CMD_DELETE, (ptu32_t)obj, (ptu32_t)uncached, full);

    Lock_MutexPend(ofmutex, CN_TIMEOUT_FOREVER);
    __OBJ_Release(obj);
    if(!res)
    {
        OBJ_UnBufferPath(obj);
    }

    Lock_MutexPost(ofmutex); // ������
    return (res);
}

// ============================================================================
// ���ܣ����ļ���
// ������fd -- �ļ�����ţ�
//      buf -- �����壻
//      count -- ��Ҫ�������ݴ�С��Byte�ƣ���
// ���أ��ɹ���ʵ�ʶ�ȡ���ĳ��ȣ�������-1����
// ��ע��
// ============================================================================
ssize_t read(int fd, void *buf, size_t size)
{
    tagOFile *ofile;
    s32 res = -1;

    if(NULL == buf)
        return (-1); // ��������

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
// ���ܣ�д�ļ���
// ������fd -- �ļ�����ţ�
//      buf -- д���壻
//      count -- ��Ҫд�����ݴ�С��Byte�ƣ���
// ���أ� �ɹ���ʵ��д��ĳ��ȣ�������-1����
// ��ע��
// ============================================================================
ssize_t write(int fd, const void *buf, size_t count)
{
    tagOFile *ofile;
    s32 res = -1;

    if(!buf)
        return (-1); // ��������

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
// ���ܣ�
// ������
// ���أ�
// ��ע��NOTE: lseek��֧���ļ���С��չ
// ============================================================================
off_t lseek(int fd, off_t offset, int whence)
{
    tagOFile *ofile;
    s32 res;

    if((SEEK_SET != whence) && (SEEK_CUR != whence) && (SEEK_END != whence))
        return (-1); // ����Ĳ���

    if((SEEK_SET == whence) && (offset < 0))
        return (-1); // ������߼�

    ofile = of(fd);
    if(!ofile)
        return (-1);

    if(testdirectory(ofile->flags))
        return (-1); // Ŀ¼������seek

    res = (s32)ofile->obj->ops(CN_OBJ_CMD_SEEK, (ptu32_t)ofile, (ptu32_t)&offset, whence);
    if(res)
        return (-1);

    return (offset);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
int fsync(int fd)
{
    tagOFile *ofile;
    s32 res;

    ofile = of(fd);
    if(!ofile)
    {
        errno = EBADF; // ��������
        return (-1);
    }

    res = (s32)ofile->obj->ops(CN_OBJ_CMD_SYNC, (ptu32_t)ofile, 0);

    return (res);

}

// ============================================================================
// ���ܣ���ȡ�ļ�ָ��ĵ�ǰλ��
// ������
// ���أ�
// ��ע��
// ============================================================================
off_t tell(int fd)
{
    tagOFile *ofile;
    s64 offset;
    s32 res;

    ofile = of(fd);
    if(!ofile)
    {
        errno = EBADF; // ��������
        return (-1);
    }

    res = (s32)ofile->obj->ops(CN_OBJ_CMD_TELL, (ptu32_t)ofile, (ptu32_t)&offset);
    if(!res)
        return (offset);

    return (-1);
}

//-----------------------------------------------------------------------------
//���ܣ���ѯ�ļ�״̬
//������file_name -- �ļ�·����
//     buf -- �ļ�״̬;
//���أ�0 -- �ɹ�; -1 -- ʧ��;
//��ע��
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
// ���ܣ���ѯ�ļ�״̬
// ������path -- �ļ�·����buf -- �ļ�״̬;
// ���أ�0 -- �ɹ�; -1 -- ʧ��;
// ��ע��
// ============================================================================
int stat(const char *path, struct stat *buf)
{
    struct Object *obj;
    char *uncached, *full;
    s32 res;

    if((!buf) || (!path))
        return(-1);

    Lock_MutexPend(ofmutex, CN_TIMEOUT_FOREVER); // ��ֹ���������ļ���ɾ����
    obj = OBJ_MatchPath((char*)path, &uncached, &full);
    __OBJ_Protect(obj);
    Lock_MutexPost(ofmutex);

    res = (s32)obj->ops(CN_OBJ_CMD_STAT, (ptu32_t)obj, (ptu32_t)uncached, buf, full);
    __OBJ_Release(obj);
    return (res);
}

// ============================================================================
// ���ܣ��趨�ļ�����
// ������fd -- �ļ������length -- �ļ����ȣ�
// ���أ��ɹ���0��; ʧ�ܣ�-1��;
// ��ע��the file offset is not changed.
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
//����: ɾ���ļ���Ŀ¼
//����:
//����: 0 -- �ɹ�; -1 -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
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
// ���ܣ���Ŀ¼�
// ������dir -- �Ѵ򿪵�Ŀ¼��
// ���أ�Ŀ¼�ȫ���Ѷ�����NULL��
// ��ע��ÿ�εķ���ֵ������ͬһ���ռ䣬����������Ϊ��ʡ�ռ䣻
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

    of = (tagOFile*)(dir->__fd); // Ŀ¼��������
    obj = of->obj; // Ŀ¼�Ľڵ�

    res = (s32)obj->ops(CN_OBJ_CMD_READDIR, (ptu32_t)of, (ptu32_t)dir->__ptr);
    if(!res)
        return (struct dirent*)(dir->__ptr);
    else
        return (NULL); // ��ȫ��������
}

// ============================================================================
// ���ܣ��ر�Ŀ¼
// ������dir -- �Ѵ򿪵�Ŀ¼��
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
// ������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע�����Ŀ¼�Ѵ��ڣ�����ʧ�ܣ�
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
// ���ܣ�
// ������
// ���أ�
// ��ע��cmd ����ƫ�� CN_OBJ_FCNTL_START ����ǵ��� ObjOps ��cmd ��
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
// ���ܣ��ļ�����
// ������
// ���أ�
// ��ע��cmd ����ƫ�� CN_OBJ_FCNTL_START ����ǵ��� ObjOps ��cmd ��
// ============================================================================
int fcntl(int fd, int cmd, ...)
{
    tagOFile *ofile;
    va_list args;
    s32 res;

    if((cmd < CN_OBJ_IOCTL_START) &&
       ((cmd + CN_OBJ_FCNTL_START) >= CN_OBJ_IOCTL_START))
        return (0); // ����Խ��

    ofile = of(fd);
    if(!ofile)
        return (0);

    va_start (args, cmd);
    res = __of_cntl(ofile, cmd, &args);
    if(!res)
        return (0); // �����ļ��Ĳ���

    if(cmd < CN_OBJ_IOCTL_START)
       cmd += CN_OBJ_FCNTL_START;

    res = (s32)ofile->obj->ops(cmd, (ptu32_t)ofile, (ptu32_t)&args);
    va_end (args);

    return (res);
}

// ============================================================================
// ���ܣ�io����
// ������
// ���أ�
// ��ע��
//      ���µ�POSIX�Ѿ�������ioctl��������Ȼ���ѣ�������Դ��Ŀ��Ȼ���á�
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
// ============================================================================
int ioctl(int fd, int cmd, ...)
{
    tagOFile *ofile;
    va_list args;
    u32 opcode;
    s32 res;

    if((cmd < CN_OBJ_CMD_USER) &&
       ((cmd + CN_OBJ_IOCTL_START) >= CN_OBJ_CMD_USER))
        return (0); // ����Խ��

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
// ���ܣ�����ǰ����Ŀ¼�ľ���·�����Ƶ�����buf
// ������buf -- ���·���Ŀռ䣻
//      size -- buf�Ĵ�С��
// ���أ��ɹ�����ǰ����Ŀ¼�ľ���·������ʧ�ܣ�NULL��
// ��ע��bufΪ NULL��ʹ sizeΪ�㣬�򱾺����ᶯ̬����һ���ռ������ظ��û��������û���Ҫע�⽫�ռ��ͷţ�
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
// ���ܣ��ı䵱ǰ����Ŀ¼
// ������path -- �µĵ�ǰ����Ŀ¼
// ���أ�
// ��ע��
// ============================================================================
int chdir(const char *path)
{
    return (-1);// TODO; NOTE:���õ�·����һ��Ŀ¼
}
