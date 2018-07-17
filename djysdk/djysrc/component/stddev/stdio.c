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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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
//����ģ�飺��׼IOģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ��׼�����������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2015-09-11
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: �����ļ�
//------------------------------------------------------
#include <stdint.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <objfile.h>
#include <object.h>
#include <device.h>
#include <systime.h>
#include <int.h>
#include "dbug.h"
#include "component_config_stdio.h"

s32 (* PutStrDirect)(const char *buf,u32 len);
char (* GetCharDirect)(void);

static struct MultiplexSetsCB *stdin_multiplexset;
static const FILE __stdio_uninit = {
    .fd = -1, // ��ʾδ��ʼ����
};

//
// STDIO�ļ���Ϣ
//
static struct __stdio_file{
    struct __portBasic basic;
    u32 runmode; // ��ʼ��ʱ���õ�����ģʽ��
    union {
        s32 direct; // �Ǹ���ģʽ�µ��ļ�ָ��;
        s32 *follow; // ����ģʽ�µ��ļ�ָ����ʵ��ָ�����STDIN��dwDirect;
    }fd;
}__stdio_files[3];

//
// 0��1��2�����ļ��Ĳ�ѯ��
//
static tagOFile *__stdio_lookup[3];

FILE *stdin  = (FILE*)&__stdio_uninit;
FILE *stderr = (FILE*)&__stdio_uninit;
FILE *stdout = (FILE*)&__stdio_uninit;

#if 0
struct __STDIO_Context{
    u32 dwMode; //
    u32 direct;
};
#endif
// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
s32 skip_atoi(const char **s)
{
    register s32 i=0;

    while(isdigit((s32)**s))
        i = i*10 + *((*s)++) - '0';

    return i;
}

// ============================================================================
// ���ܣ�дSTDIO�ļ���
// ������of -- STDIO�ļ���
//      data -- д���ݣ�
//      size -- д���ݴ�С��
// ���أ�ʵ��д����С��
// ��ע��
// ============================================================================
static s32 __stdio_filewrite(tagOFile *of, u8 *data, u32 size)
{
    struct __stdio_file *stdio;
    s32 fd;
    s32 res = 0;

    stdio = dListEntry(of_basic(of), struct __stdio_file, basic);
    if(stdio->runmode & (CN_STDIO_STDOUT_FOLLOW | CN_STDIO_STDERR_FOLLOW))
    {
        fd = *(stdio->fd.follow); // ��ǰ�ļ����ڸ���ģʽ
    }
    else
    {
        fd = stdio->fd.direct;
    }

    if(-1 != fd)
        res = write(fd, data, size);

    return (res);
}

// ============================================================================
// ���ܣ���STDIO�ļ���
// ������of -- STDIO�ļ���
//      data -- �����棻
//      size -- �����ݴ�С��
// ���أ�ʵ�ʶ����С��
// ��ע��
// ============================================================================
static s32 __stdio_fileread(tagOFile *of, u8 *buf, u32 size)
{
    struct __stdio_file *stdio;
    s32 fd, res = 0;

    stdio = dListEntry(of_basic(of), struct __stdio_file, basic);
    if(stdio->runmode&CN_STDIO_STDIN_MULTI) // ֻ�б�׼�����ļ����ڶ�Դ
    {
        fd = Multiplex_Wait(stdin_multiplexset, NULL, of_timeout(of)); // timeout��forever
        if(fd >= 0)
            stdio->fd.direct = fd;
        else
            fd = -1;
    }
    else if(stdio->runmode&(CN_STDIO_STDOUT_FOLLOW | CN_STDIO_STDERR_FOLLOW))
    {
        fd = *(stdio->fd.follow);
    }
    else
    {
        fd = stdio->fd.direct;
    }

    if(-1 != fd)
        res = read(fd, buf, size);

    return (res);
}

// ============================================================================
// ���ܣ���STDIO�ļ���
// ������obj -- STDIO�ļ�����
//      mode -- ��ģʽ��
// ���أ�
// ��ע��
// ============================================================================
static tagOFile *__stdio_fileopen(struct Object *obj, u32 mode)
{
    struct __stdio_file *stdio;
    tagOFile *of;
    s32 fd, res;
    u32 timeout = 0;

    if(testdirectory(mode))
    {
        if(GROUP_POINT != __OBJ_Type(obj))
            return (NULL); // ֻ�����"stdio"Ŀ¼�Ĳ������������
    }
    else
    {
        stdio = dListEntry(of_basiclinko(obj), struct __stdio_file, basic);
        if(stdio->runmode & (CN_STDIO_STDOUT_FOLLOW | CN_STDIO_STDERR_FOLLOW))
            fd = *stdio->fd.follow;
        else
            fd = stdio->fd.direct;

        res = fcntl(fd, F_OF_GETTIMEOUT, &timeout);
        if(res)
            return (NULL); // �޷�������������ļ���
    }

    of = of_new();
    if(of)
    {
        of_init(of, NULL, mode, 0);
        if(timeout)
            of_settimeout(of, timeout);
    }

    return (of);
}

// ============================================================================
// ���ܣ��ر�STDIO��
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 __stdio_fileclose(tagOFile *of)
{
    if(of==__stdio_lookup[0])
    {
        __stdio_lookup[0] = NULL;
        warning_printf("stdio","\"in\"(0) is closed.");
    }
    else if(of==__stdio_lookup[1])
    {
        __stdio_lookup[1] = NULL;
        warning_printf("stdio","\"out\"(1) is closed.");
    }
    else if(of==__stdio_lookup[2])
    {
        __stdio_lookup[2] = NULL;
        warning_printf("stdio","\"err\"(2) is closed.");
    }

    return (of_free(of));
}

// ============================================================================
// ���ܣ�STDIO�ض���
// ������of -- �ļ������ģ�
//      fd -- �¶�����ļ���
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
static s32 __stdio_fileredirect(tagOFile *of, u32 fd)
{
    struct __stdio_file *stdio;

    if(isdirectory(of))
        return (-1); // ֻ��STDIO��Ŀ¼�����䲻�������¶���

    stdio = dListEntry(of_basic(of), struct __stdio_file, basic);
    if(!(stdio->runmode & (CN_STDIO_STDOUT_FOLLOW | CN_STDIO_STDERR_FOLLOW)))
        close(stdio->fd.direct); // �Ǹ���ģʽʱ�����ԹرվɵĶ����ļ���

    stdio->fd.direct = fd;
    // ����ģʽʱ����Ϊ�ض����ˣ��޷��������棻
    if(stdio->runmode & (CN_STDIO_STDOUT_FOLLOW | CN_STDIO_STDERR_FOLLOW))
        stdio->runmode &= ~(CN_STDIO_STDERR_FOLLOW | CN_STDIO_STDOUT_FOLLOW);

    return (0);
}

// ============================================================================
// ���ܣ�STDIN���ü�������
// ������of -- STDIO�ļ������ģ�
//      acts -- ��ӣ�1����ɾ����0����
//      fd -- �ļ���
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 __stdio_filemulti(tagOFile *of, u32 acts, u32 fd)
{
    struct __stdio_file *stdio;
    s32 res = -1;

    if(isdirectory(of))
        return (-1); // ֻ��STDIO��Ŀ¼�����䲻����������ü���

    stdio = dListEntry(of_basic(of), struct __stdio_file, basic);

    if((!(stdio->runmode & CN_STDIO_STDIN_MULTI))&&(!stdin_multiplexset));
        return (-1); // ֻ������Ŵ��ڶ�·���ü���

    switch (acts)
    {
        case 1:
                if(TRUE == Multiplex_AddObject(stdin_multiplexset, fd,
                    (CN_MULTIPLEX_SENSINGBIT_READ + CN_MULTIPLEX_SENSINGBIT_OR)))
                    res = 0;

                break;

        case 0:
                if(TRUE == Multiplex_DelObject(stdin_multiplexset, fd))
                    res = 0;

                break;

        default:
                break;
    }

    return (res);
}

#if 0
// ============================================================================
// ���ܣ�STDIN��TAG������
// ������of -- STDIO�ļ������ģ�
//      acts -- ���ã�1���������0����
//      flags -- TAGλ��
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 __stdio_tag(tagOFile *of, u32 acts, u32 flags)
{
    s32 res = 0;

    switch (acts)
    {
        case 1:
                of->tag |= flags;
                break;

        case 0:
                of->tag &= ~flags;
                break;

        default:
                res = -1;
                break;
    }

    return (res);
}
#endif

// ============================================================================
// ���ܣ���ѯstdio�ļ�״̬
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 __stdio_filestat(struct Object *obj, struct stat *stat)
{
    memset(stat, 0x0, sizeof(*stat));
    if(GROUP_POINT == __OBJ_Type(obj))
        stat->st_mode = S_IFDIR;
    else
        stat->st_mode = S_IFREG;

    return (0);
}

// ============================================================================
// ���ܣ�STDIO timeout����
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 __stdio_filetimeout(tagOFile *of, u32 acts, u32 *timeout)
{
    struct __stdio_file *stdio;
    s32 fd, res = 0;

    if(isdirectory(of))
    {
        // ֻ��STDIO��Ŀ¼
        if(1 == acts)
            of_settimeout(of, *timeout);
        else
            *timeout = of_timeout(of);
    }
    else
    {
        stdio = dListEntry(of_basic(of), struct __stdio_file, basic);
        if(stdio->runmode & (CN_STDIO_STDOUT_FOLLOW | CN_STDIO_STDERR_FOLLOW))
            fd = *stdio->fd.follow;
        else
            fd = stdio->fd.direct;

        switch (acts)
        {
            case 1:
                    res = fcntl(fd,  F_OF_SETTIMEOUT, *timeout);
                    if(!res)
                        of_settimeout(of, *timeout);
                    break;

            case 0:
                    res = fcntl(fd,  F_OF_GETTIMEOUT, timeout);
                    break;

            default:
                    res = -1;
                    break;
        }
    }

    return (res);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 __stdio_fileops(u32 dwCMD, ptu32_t context, ptu32_t args,  ...)
{
    va_list list;

    switch(dwCMD)
    {
        case CN_OBJ_CMD_OPEN:
        {
            char *notExist;
            struct Object *obj = (struct Object*)context;
            u32 mode = (u32)args;

            va_start(list, args);
            notExist = (char*)va_arg(list, u32);
            va_end(list);

            if(notExist)
                return (-1);

            return ((ptu32_t)__stdio_fileopen(obj, mode));
        }

        case CN_OBJ_CMD_CLOSE:
        {
            tagOFile *of = (tagOFile*)context;

            return ((ptu32_t)__stdio_fileclose(of));
        }

        case CN_OBJ_CMD_READDIR:
        {
            return(-1); //
        }

        case CN_OBJ_CMD_DELETE:
        {
            return (-1); // STDIO�ļ�������ɾ����
        }

        case CN_OBJ_CMD_SEEK:
        {
            return (-1);
        }

        case CN_OBJ_CMD_WRITE:
        {
            tagOFile *of;
            u8 *buf;
            u32 len;

            of = (tagOFile *)context;
            buf = (u8*)args;
            va_start(list, args);
            len = va_arg(list, u32);
            va_end(list);

            return((ptu32_t)__stdio_filewrite(of, buf, len));
        }

        case CN_OBJ_CMD_READ:
        {
            tagOFile *of;
            u8 *buf;
            u32 len;

            of = (tagOFile *)context;
            buf = (u8*)args;
            va_start(list, args);
            len = va_arg(list, u32);
            va_end(list);

            return((ptu32_t)__stdio_fileread(of, buf, len));
        }

        case CN_OBJ_CMD_STAT:
        {
            char *notExist;
            struct Object *obj = (struct Object*)context;
            struct stat *info = (struct stat*)args;

            va_start(list, args);
            notExist = (char*)va_arg(list, u32);
            va_end(list);

            if(notExist)
                return (-1);

            return ((ptu32_t)__stdio_filestat(obj, info));
        }

        case CN_OBJ_FCNTL_START + F_OF_SETTIMEOUT:
        {
            tagOFile *of = (tagOFile*)context;
            u32 timeout = (u32)args;

            return ((ptu32_t)__stdio_filetimeout(of, 1, &timeout));
        }

        case CN_OBJ_FCNTL_START + F_OF_GETTIMEOUT:
        {
            tagOFile *of = (tagOFile*)context;
            u32 *timeout = (u32*)args;

            return ((ptu32_t)__stdio_filetimeout(of, 1, timeout));
        }

#if 0
        case (CN_OBJ_FCNTL_START + F_OF_SETTAG):
        {
            tagOFile *of = (tagOFile*)context;
            ptu32_t tag = (ptu32_t)args;

            return ((ptu32_t)__stdio_tag(of, 1, tag));
        }

        case (CN_OBJ_FCNTL_START + F_OF_CLRTAG):
        {
            tagOFile *of = (tagOFile*)context;
            ptu32_t tag = (ptu32_t)args;

            return ((ptu32_t)__stdio_tag(of, 0, tag));
        }
#endif

        case (CN_OBJ_FCNTL_START + F_STDIO_MULTI_ADD):
        {
            tagOFile *of = (tagOFile*)context;
            u32 add = (u32)args;

            return ((ptu32_t)__stdio_filemulti(of, 1, add));
        }

        case (CN_OBJ_FCNTL_START + F_STDIO_MULTI_DEL):
        {
            tagOFile *of = (tagOFile*)context;
            u32 del = (u32)args;

            return ((ptu32_t)__stdio_filemulti(of, 0, del));
        }

        case (CN_OBJ_FCNTL_START + F_STDIO_REDRIECT):
        {
            tagOFile *of = (tagOFile*)context;
            u32 fd = (u32)args;

            return ((ptu32_t)__stdio_fileredirect(of, fd));
        }

        default:
            break;
    }

    return (CN_OBJ_CMD_UNSUPPORT);
}

// ============================================================================
// ���ܣ���ʼ��STDIO��in��out��err���Ķ�Ӧ�ļ���
// ������type -- ��׼IO���ͣ�0�����룩��1���������2�������������
//      fd -- ��׼IO�ļ������򵽵��ļ���
//      mode -- ��׼IO�ļ���ģʽ��
//      runmode -- ��׼IO�ļ�������ģʽ��
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע����Ϊ"STDIN"�ȵĽṹ�е�fd���ļ��ţ�����Ϊ�ı����0�ȣ�
//      ��ᵼ�¶�дʱ�Ҳ�����ԭʼ�������ģ��˴��Ĳ��ұ�ʾΪ�˱�����������ģ�
// ============================================================================
static s32 __stdio_set(u32 type, s32 fd, u32 mode, u32 runmode)
{
    char *notfound;
    struct Object *obj;
    struct stat info;
    s32 res;
    FILE *fp;
    tagOFile *of = NULL;
    extern s32 __FBNew(FILE *stream);
    extern void __FBDel(FILE *stream);

    res = fstat(fd, &info);
    if(res)
        return (-1);

    fp = malloc(sizeof(*fp));
    if(!fp)
        return (-1);

    memset(fp, 0x0, sizeof(*fp));
    fp->unget = EOF;
    if((Djy_GetRunMode() < CN_RUNMODE_MP) && (S_ISBF(info.st_mode)))
    {
        res = __FBNew(fp);
        if(res)
            goto __ERR_STDIO_SET;
    }

    fcntl(fd, F_OF_SETTIMEOUT, CN_TIMEOUT_FOREVER); // ����������豸����Ϊforver������ʽ����
    switch(type)
    {
        case 0 :
        {
            // ���֧�ֶ�STDIN����Դ���򴴽���·���ü������ѳ�ʼ��ʱָ����IN�ļ�����
            // ����·���ü��У�TODO�������򣬰� StdinEdit�������� stdinname �ļ����ԣ���
            if(runmode&CN_STDIO_STDIN_MULTI)
            {
                stdin_multiplexset = Multiplex_Create(1);
                if(!stdin_multiplexset)
                {
                    goto __ERR_STDIO_SET;
                }

                // ����ǰ�򿪵��ļ������������
                if(FALSE == Multiplex_AddObject(stdin_multiplexset, fd,
                        (CN_MULTIPLEX_SENSINGBIT_READ + CN_MULTIPLEX_SENSINGBIT_OR)))
                {
                    goto __ERR_STDIO_SET;// TODO: û���ҵ�ɾ����·���ü��ĺ�����
                }
            }

            obj = OBJ_MatchPath("/stdio/in", &notfound, NULL);
            if(notfound)
            {
                goto __ERR_STDIO_SET;
            }

            of = __stdio_fileopen(obj, mode);
            if(!of)
            {
                goto __ERR_STDIO_SET;
            }

            of_linko(of, obj);
            __stdio_lookup[0] = of;
            fp->fd = 0;
            stdin = fp;
            __stdio_files[0].fd.direct = fd;

            break;
        }

        case 1 :
        {
            obj = OBJ_MatchPath("/stdio/out", &notfound, NULL);
            if(notfound)
            {
                goto __ERR_STDIO_SET;
            }
            of = __stdio_fileopen(obj, mode);
            if(!of)
            {
                goto __ERR_STDIO_SET;
            }

            of_linko(of, obj);
            __stdio_lookup[1] = of;
            fp->fd = 1;
            stdout = fp;
            if(runmode & CN_STDIO_STDOUT_FOLLOW)
                __stdio_files[1].fd.follow = &(__stdio_files[0].fd.direct);
            else
                __stdio_files[1].fd.direct = fd;

            break;
        }

        case 2 :
        {
            obj = OBJ_MatchPath("/stdio/err", &notfound, NULL);
            if(notfound)
                goto __ERR_STDIO_SET;

            of = __stdio_fileopen(obj, mode);
            if(!of)
                goto __ERR_STDIO_SET;

            of_linko(of, obj);
            __stdio_lookup[2] = of;
            fp->fd = 2;
            stderr = fp;
            if(runmode & CN_STDIO_STDERR_FOLLOW)
                __stdio_files[2].fd.follow = &(__stdio_files[0].fd.direct);
            else
                __stdio_files[2].fd.direct = fd;

            break;
        }

        default:
            break;
    }

    return (0);

__ERR_STDIO_SET:

    __FBDel(fp);
    free(fp);
    if(of)
        __stdio_fileclose(of);

    return (-1);
}

// ============================================================================
// ���ܣ�ע��STDIOϵͳ
// ������
// ���أ�
// ��ע��
// ============================================================================
static void __stdio_destory(void)
{
    ;
}
// ============================================================================
// ���ܣ�����STDIOϵͳ
// ������runmode -- STDIOϵͳ����ģʽ��
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
static s32 __stdio_build(u32 runmode)
{
    struct Object *root;
    u8 i;

    root = __mounto("stdio", "/", (tagObjOps)__stdio_fileops, 0);
    if(!root)
        return (-1);

    // �����ͳ�ʼ��in��out��err�����ļ�
    for(i = 0; i < 3; i++)
    {
        __stdio_files[i].fd.direct = -1;
        __stdio_files[i].runmode = 0;
        of_initbasic(&__stdio_files[i].basic, O_RDWR);
    }

    if(runmode & CN_STDIO_STDIN_MULTI)
        __stdio_files[0].runmode |= CN_STDIO_STDIN_MULTI; // STDIN�Ƕ�·����ģʽ��

    if(runmode & (CN_STDIO_STDOUT_FOLLOW))
    {
        __stdio_files[1].runmode |= CN_STDIO_STDOUT_FOLLOW; // STDOUT����STDINָ��
        __stdio_files[1].fd.follow = &__stdio_files[0].fd.direct;
    }

    if(runmode & CN_STDIO_STDERR_FOLLOW)
    {
        __stdio_files[2].runmode |= CN_STDIO_STDERR_FOLLOW; // STDERR����STDINָ��
        __stdio_files[2].fd.follow = &__stdio_files[0].fd.direct;
    }

    if(-1 == of_virtualize(root, &__stdio_files[0].basic, "in"))
        return (-1);

    if(-1 == of_virtualize(root, &__stdio_files[1].basic, "out"))
        return (-1);

    if(-1 == of_virtualize(root, &__stdio_files[2].basic, "err"))
        return (-1);

    return (0);
}
// ============================================================================
// ���ܣ���װSTDIOģ�飻
// ������runmode -- ����ģʽ��
//      in -- ��׼����ָ����ļ���
//      out -- ��׼���ָ����ļ���
//      err -- ��׼�������ָ����ļ���
// ���أ��ɹ���0��;ʧ�ܣ�-1����
// ��ע��STDIO������ģʽ�ǲ��ܶ�̬�ĵġ������ʼ��ʱ���õ��Ǹ���ģʽ���߶�����ģʽ����ôϵͳ
//      ���������Ժ󣬲��ܶ�̬�ı䣨�漰����ر���û�д�������
// ============================================================================
s32 ModuleInstall_STDIO(u32 runmode, const char *in, const char *out, const char *err)
{
    char *inname, *outname, *errname;
    s32 res, inFD, fd;
    u32 mode;

    res = __stdio_build(CFG_STDIO_RUN_MODE);
    if(res)
    {
        debug_printf("module","STDIO install failed(cannot build).");
        return (-1);
    }

    // STDIN��ʼ��
    if(CFG_STDIO_RUN_MODE & (CN_STDIO_STDOUT_FOLLOW | CN_STDIO_STDERR_FOLLOW))
    {
        // STDOUT��STDERRֻҪ��һ������STDIN��STDIN�������Կɶ�д��ģʽ�򿪡�
        mode = O_RDWR | O_APPEND;
    }
    else
    {
        mode =  O_RDONLY;
    }

    inFD = open(in, mode);
    inname = (char*)in;
    res = __stdio_set(0, inFD, O_RDWR, CFG_STDIO_RUN_MODE);
    if(res)
    {
        debug_printf("module","STDIO install failed(\"in\" cannot set).");
        goto __INSTALL_STDIO_ERR;
    }

     // STDOUT ��ʼ��
    if (CFG_STDIO_RUN_MODE & CN_STDIO_STDOUT_FOLLOW)
    {
        outname = inname; // STDOUT ���� STDIN��ʹ�õ���ͬһ���ļ���
        fd = inFD;
    }
    else
    {
        fd = open(out, O_RDWR | O_APPEND);
        outname = (char*)out;
    }

    res = __stdio_set(1, fd, (O_RDWR | O_APPEND), CFG_STDIO_RUN_MODE);
    if(res)
    {
        debug_printf("module","STDIO install failed(\"out\" cannot set).");
        goto __INSTALL_STDIO_ERR;
    }

    // STDERR��ʼ��
    if (CFG_STDIO_RUN_MODE & CN_STDIO_STDERR_FOLLOW)
    {
        errname = inname; // STDOUT ���� STDIN��ʹ�õ���ͬһ���ļ���
        fd = inFD;
    }
    else
    {
        fd = open(err, O_RDWR | O_APPEND);
        errname = (char*)err;
    }

    res = __stdio_set(2, fd, (O_RDWR | O_APPEND), CFG_STDIO_RUN_MODE);
    if(res)
    {
        debug_printf("module","STDIO install failed(\"err\" cannot set).");
        goto __INSTALL_STDIO_ERR;
    }

    info_printf("module","STDIO installed, IN : \"%s\", OUT : \"%s\", ERR : \"%s\".", inname, outname, errname);
    return (0);

__INSTALL_STDIO_ERR:
    __stdio_destory();
    return (-1);
}

// ============================================================================
// ���ܣ������Ƿ���δ��ʼ����STDIO��
// ������stream -- STDIO�ļ�����
// ���أ��ǣ�1�������ǣ�0����δ��ʼ����STDIO��-1����
// ��ע��
// ============================================================================
s32 IsSTDIO(FILE* stream)
{
    if(!stream)
        return (0); // �Ƿ�����

    if(-1 == stream->fd)
        return (-1); // δ��ʼ����STDIO�ļ���

    if((0 == stream->fd) || (1 == stream->fd) || (2 == stream->fd))
        return (1); // ��STDIO�ļ���

    return (0); // ����STDIO�ļ���
}

// ============================================================================
// ���ܣ���ȡ��׼IO��in��out��err�����ļ���Ϣ��
// ������type -- ��׼IO���ͣ�0�����룩��1���������2�������������
// ���أ���׼IO���ļ���Ϣ��
// ��ע����Ϊ"stdin"�ȵĽṹ�е�fd���ļ��ţ�����Ϊ�ı����0�ȣ�
//      ��ᵼ�¶�дʱ�Ҳ�����ԭʼ�������ģ��˴��Ĳ��ұ�ʾΪ�˱�����������ģ�
// ============================================================================
tagOFile *STDIO_Get(u32 type)
{
    tagOFile *of;

    switch(type)
    {
        case 0: of = __stdio_lookup[0]; break;
        case 1: of = __stdio_lookup[1]; break;
        case 2: of = __stdio_lookup[2]; break;
        default:of = NULL; break;
    }

    return (of);
}

#if 0
// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
s32 STDIO_Redirect(FILE *stream, u32 fd)
{
    s32 res = 0;

    switch(stream->fd)
    {
        case 0 :
                __stdio_files[0].fd.direct = fd;
                break;
        case 1 :
                __stdio_files[1].fd.direct = fd;
                if(__stdio_files[1].runmode & CN_STDIO_STDOUT_FOLLOW)
                    __stdio_files[1].runmode &= (~CN_STDIO_STDOUT_FOLLOW);
                break;
        case 2 :
                __stdio_files[2].fd.direct = fd;
                if(__stdio_files[1].runmode & CN_STDIO_STDERR_FOLLOW)
                    __stdio_files[1].runmode &= (~CN_STDIO_STDERR_FOLLOW);
                break;
        default:
                res = -1;
                break;
    }

    return (res);
}
#endif

// ============================================================================
// ���ܣ���һ���ļ����뵽��׼�������������
// ������name -- �ļ�·����
// ���أ��ɹ����(TREE)��ʧ�ܣ�FALSE����
// ��ע��
// ============================================================================
typedef struct __StdinDev
{
    const char *name;
    int fd;
    struct __StdinDev *nxt;
}tagStdinDev;
static tagStdinDev  *pStdinDevLst = NULL;

bool_t add2listenset(const char *name)
{
    tagStdinDev *dev;
    atom_low_t  atom;

    if(!stdin_multiplexset)
        return (FALSE);

    dev = malloc(sizeof(tagStdinDev));
    if(NULL == dev)
    {
        return (FALSE);
    }

    memset(dev,0,sizeof(tagStdinDev));
    dev->name = name; // TODO����������
    dev->fd = open(name, O_RDWR);
    if(dev->fd < 0)
    {
        free(dev);
        return (FALSE);
    }

    //JOIN THE STDIN SETS
    Multiplex_AddObject(stdin_multiplexset, dev->fd,
            (CN_MULTIPLEX_SENSINGBIT_READ + CN_MULTIPLEX_SENSINGBIT_OR));

    //ADD IT TO THE STDINDEV LIST

    atom = Int_LowAtomStart();
    dev->nxt = pStdinDevLst;
    pStdinDevLst = dev;
    Int_LowAtomEnd(atom);

    return (TRUE);
}
