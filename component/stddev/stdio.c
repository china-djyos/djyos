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
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
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
#include <stat.h>
#include <stdint.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <objhandle.h>
#include <object.h>
#include <device.h>
#include <systime.h>
#include <int.h>
#include "dbug.h"
#include "component_config_stdio.h"

s32 (* PutStrDirect)(const char *buf,u32 len);
char (* GetCharDirect)(void);

static struct MultiplexSetsCB *stdin_multiplexset; // in����������
static FILE __stdio_filestruct[3] = {
        {
            .fd = -1, // ��ʾδ��ʼ����
        },
        {
            .fd = -1, // ��ʾδ��ʼ����
        },
        {
            .fd = -1, // ��ʾδ��ʼ����
        },
};

//
// STDIO�ļ���Ϣ
//
static struct __stdio{
    u32 runmode; // ��ʼ��ʱ���õ�����ģʽ��
    union {
        s32 direct; // �Ǹ���ģʽ�µ��ļ�ָ��;
        s32 *follow; // ����ģʽ�µ��ļ�ָ����ʵ��ָ�����STDIN��dwDirect;
    }fd;
}__stdio_in_out_err[3];

//
// 0��1��2�����ļ��Ĳ�ѯ����
//
static struct objhandle *__stdio_lookup[3];

FILE *stdin  = (FILE*)&__stdio_filestruct[0];
FILE *stderr = (FILE*)&__stdio_filestruct[1];
FILE *stdout = (FILE*)&__stdio_filestruct[2];

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
// ���ܣ�дSTDIO��
// ������hdl -- STDIO��������
//      data -- д���ݣ�
//      size -- д���ݴ�С��
// ���أ�ʵ��д����С��
// ��ע��
// ============================================================================
static s32 __stdio_write(struct objhandle *hdl, u8 *data, u32 size)
{
    struct __stdio *stdio;
    s32 fd;
    s32 res = 0;

    stdio = (struct __stdio*)handle_val(hdl);
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
// ���ܣ���STDIO��
// ������hdl -- STDIO��������
//      data -- �����棻
//      size -- �����ݴ�С��
// ���أ�ʵ�ʶ����С��
// ��ע��
// ============================================================================
static s32 __stdio_read(struct objhandle *hdl, u8 *buf, u32 size)
{
    struct __stdio *stdio;
    s32 fd, res = 0;

    stdio = (struct __stdio*)handle_val(hdl);
    if(stdio->runmode&CN_STDIO_STDIN_MULTI) // ֻ�б�׼�����ļ����ڶ�Դ
    {
        fd = Multiplex_Wait(stdin_multiplexset, NULL, handle_timeout(hdl)); // timeout��forever
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
// ���ܣ���STDIO��
// ������ob -- STDIO����
//      mode -- ��ģʽ��
// ���أ��ɹ���STDIO����������ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
static struct objhandle *__stdio_open(struct obj *ob, u32 mode)
{
    struct __stdio *stdio;
    struct objhandle *hdl;
    s32 fd, res;
    u32 timeout = 0;

    if(test_directory(mode))
    {
        if(!obj_isset(ob))
            return (NULL); // ֻ�����"stdio"Ŀ¼�Ĳ�������������
    }
    else
    {
        stdio = (struct __stdio*)obj_GetPrivate(ob);
        if(stdio->runmode & (CN_STDIO_STDOUT_FOLLOW | CN_STDIO_STDERR_FOLLOW))
            fd = *stdio->fd.follow;
        else
            fd = stdio->fd.direct;

        res = fcntl(fd, F_SETTIMEOUT, &timeout);
        if(res)
            return (NULL); // �޷�������������ļ���
    }

    hdl = handle_new();
    if(hdl)
    {
        handle_init(hdl, NULL, mode, 0);
        if(timeout)
            handle_settimeout(hdl, timeout);
    }

    return (hdl);
}

// ============================================================================
// ���ܣ��ر�STDIO��
// ������hdl -- STDIO��������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
static s32 __stdio_close(struct objhandle *hdl)
{
    if(hdl==__stdio_lookup[0])
    {
        __stdio_lookup[0] = NULL;
        warning_printf("stdio","\"in\"(0) is closed.");
    }
    else if(hdl==__stdio_lookup[1])
    {
        __stdio_lookup[1] = NULL;
        warning_printf("stdio","\"out\"(1) is closed.");
    }
    else if(hdl==__stdio_lookup[2])
    {
        __stdio_lookup[2] = NULL;
        warning_printf("stdio","\"err\"(2) is closed.");
    }

    return (handle_free(hdl));
}

// ============================================================================
// ���ܣ�STDIO�ض���
// ������hdl -- STDIO��������
//      fd -- �¶�����ļ���
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
static s32 __stdio_redirect(struct objhandle *hdl, s32 fd)
{
    struct __stdio *stdio;

    if(isdirectory(hdl))
        return (-1); // ֻ��STDIO��Ŀ¼�����䲻�������¶���

    stdio = (struct __stdio*)handle_val(hdl);
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
// ������hdl -- STDIO��������
//      acts -- ���ӣ�1����ɾ����0����
//      fd -- �ļ���
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 __stdio_multi(struct objhandle *hdl, u32 acts, s32 fd)
{
    struct __stdio *stdio;
    s32 res = -1;

    if(isdirectory(hdl))
        return (-1); // ֻ��STDIO��Ŀ¼�����䲻�����������ü���

    stdio = (struct __stdio*)handle_val(hdl);
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
// ������hdl -- STDIO��������
//      acts -- ���ã�1���������0����
//      flags -- TAGλ��
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 __stdio_tag(struct objhandle *hdl, u32 acts, u32 flags)
{
    s32 res = 0;

    switch (acts)
    {
        case 1:
                hdl->tag |= flags;
                break;

        case 0:
                hdl->tag &= ~flags;
                break;

        default:
                res = -1;
                break;
    }

    return (res);
}
#endif

// ============================================================================
// ���ܣ���ѯstdio״̬��
// ������ob -- stdio����
//      data -- stdio״̬��
// ���أ�
// ��ע��
// ============================================================================
static s32 __stdio_stat(struct obj *ob, struct stat *data)
{
    memset(data, 0x0, sizeof(*stat));
    if(obj_isset(ob))
        data->st_mode = S_IFDIR;
    else
        data->st_mode = S_IFREG;

    return (0);
}

// ============================================================================
// ���ܣ�STDIO timeout����
// ������hdl -- stdio��������
//      act -- 1 ����ʱ�䣻0 ��ȡʱ�䣻
//      timeout -- ʱ�䣻
// ���أ�
// ��ע��
// ============================================================================
static s32 __stdio_timeout(struct objhandle *hdl, u32 acts, u32 *timeout)
{
    struct __stdio *stdio;
    s32 fd, res = 0;

    if(isdirectory(hdl))
    {
        // ֻ��STDIO��Ŀ¼
        if(1 == acts)
            handle_settimeout(hdl, *timeout);
        else
            *timeout = handle_timeout(hdl);
    }
    else
    {
        stdio = (struct __stdio*)handle_val(hdl);
        if(stdio->runmode & (CN_STDIO_STDOUT_FOLLOW | CN_STDIO_STDERR_FOLLOW))
            fd = *stdio->fd.follow;
        else
            fd = stdio->fd.direct;

        switch (acts)
        {
            case 1:
                    res = fcntl(fd,  F_SETTIMEOUT, *timeout); // ���ñ������ļ���timeout
                    if(!res)
                        handle_settimeout(hdl, *timeout);
                    break;

            case 0:
                    res = fcntl(fd,  F_GETTIMEOUT, timeout);
                    break;

            default:
                    res = -1;
                    break;
        }
    }

    return (res);
}

// ============================================================================
// ���ܣ�stdio���󼯺ϣ��ࣩ������
// ��������enum objops˵����
// ���أ���enum objops˵����
// ��ע��
// ============================================================================
static ptu32_t __stdio_ops(enum objops ops, ptu32_t o_hdl, ptu32_t args,  ...)
{
    va_list list;

    switch(ops)
    {
        case OBJOPEN:
        {
            char *notExist;
            struct obj *ob = (struct obj*)o_hdl;
            u32 mode = (u32)args;

            va_start(list, args);
            notExist = (char*)va_arg(list, u32);
            va_end(list);

            if(notExist)
                return (-1);

            return ((ptu32_t)__stdio_open(ob, mode));
        }

        case OBJCLOSE:
        {
            struct objhandle *hdl = (struct objhandle*)o_hdl;

            return ((ptu32_t)__stdio_close(hdl));
        }

        case OBJCHILDS:
        {
            return(-1); // ��Ŀ¼�
        }

        case OBJDEL:
        {
            return (-1); // STDIO�ļ�������ɾ����
        }

        case OBJSEEK:
        {
            return (-1);
        }

        case OBJWRITE:
        {
            struct objhandle *hdl;
            u8 *buf;
            u32 len;

            hdl = (struct objhandle *)o_hdl;
            buf = (u8*)args;
            va_start(list, args);
            len = va_arg(list, u32);
            va_end(list);
            return((ptu32_t)__stdio_write(hdl, buf, len));
        }

        case OBJREAD:
        {
            struct objhandle *hdl;
            u8 *buf;
            u32 len;

            hdl = (struct objhandle *)o_hdl;
            buf = (u8*)args;
            va_start(list, args);
            len = va_arg(list, u32);
            va_end(list);
            return((ptu32_t)__stdio_read(hdl, buf, len));
        }

        case OBJSTAT:
        {
            char *path;
            struct obj *ob = (struct obj*)o_hdl;
            struct stat *info = (struct stat*)args;

            va_start(list, args);
            path = (char*)va_arg(list, u32);
            va_end(list);
            if(path&&('\0'!=*path))
                return (-1); // ���󲻴��ڣ�

            return ((ptu32_t)__stdio_stat(ob, info));
        }

        case OBJCTL:
        {
            struct objhandle *hdl = (struct objhandle*)o_hdl;
            u32 cmd = args;
            va_start(list, args);
            switch(cmd)
            {
                case F_SETTIMEOUT:
                {
                    u32 *timeout = (u32*)va_arg(list, u32);
                    return ((ptu32_t)__stdio_timeout(hdl, 1, timeout));
                }

                case F_GETTIMEOUT:
                    break;

                case F_STDIO_MULTI_ADD:
                {
                    s32 *fd = (s32*)va_arg(list, u32);
                    return ((ptu32_t)__stdio_multi(hdl, 1, *fd));
                }

                case F_STDIO_MULTI_DEL:
                {
                    s32 *fd = (s32*)va_arg(list, u32);
                    return ((ptu32_t)__stdio_multi(hdl, 0, *fd));
                }

                case F_STDIO_REDRIECT:
                {
                    s32 *fd = (s32*)va_arg(list, u32);
                    return ((ptu32_t)__stdio_redirect(hdl, *fd));
                }

                default : break;
            }
        }

        case OBJIOCTL:
        {
            break;
        }

#if 0
        case (CN_OBJ_FCNTL_START + F_OF_SETTAG):
        {
            struct objhandle *hdl = (struct objhandle*)o_hdl;
            ptu32_t tag = (ptu32_t)args;

            return ((ptu32_t)__stdio_tag(hdl, 1, tag));
        }

        case (CN_OBJ_FCNTL_START + F_OF_CLRTAG):
        {
            struct objhandle *hdl = (struct objhandle*)o_hdl;
            ptu32_t tag = (ptu32_t)args;

            return ((ptu32_t)__stdio_tag(hdl, 0, tag));
        }
#endif

#if 0
        case (CN_OBJ_FCNTL_START + F_STDIO_MULTI_ADD):
        {
            struct objhandle *hdl = (struct objhandle*)o_hdl;
            u32 add = (u32)args;

            return ((ptu32_t)__stdio_multi(hdl, 1, add));
        }

        case (CN_OBJ_FCNTL_START + F_STDIO_MULTI_DEL):
        {
            struct objhandle *hdl = (struct objhandle*)o_hdl;
            u32 del = (u32)args;

            return ((ptu32_t)__stdio_multi(hdl, 0, del));
        }

        case (CN_OBJ_FCNTL_START + F_STDIO_REDRIECT):
        {
            struct objhandle *hdl = (struct objhandle*)o_hdl;
            u32 fd = (u32)args;

            return ((ptu32_t)__stdio_redirect(hdl, fd));
        }
#endif
        default:
            break;
    }

    return (-1);
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
    struct obj *ob;
    struct stat info;
    s32 res;
    FILE *fp;
    struct objhandle *hdl = NULL;
    extern s32 __filebuf_new(FILE *stream);
    extern void __filebuf_del(FILE *stream);

    res = fstat(fd, &info);
    if(res)
        return (-1);

    fp = (FILE*)&__stdio_filestruct[type];
    fp->unget = EOF;
    if((Djy_GetRunMode() < CN_RUNMODE_MP) && (S_ISFLOW(info.st_mode)))
    {
        res = __filebuf_new(fp);
        if(res)
            goto __ERR_STDIO_SET;
    }

    fcntl(fd, F_SETTIMEOUT, CN_TIMEOUT_FOREVER); // ����������豸����Ϊforver������ʽ����
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

            ob = obj_matchpath("/stdio/in", &notfound, NULL);
            if(notfound)
            {
                goto __ERR_STDIO_SET;
            }

            hdl = handle_new();
            if(!hdl)
            {
                goto __ERR_STDIO_SET;
            }

            handle_init(hdl, ob, mode, 0);
            handle_linkobj(hdl, ob);
            __stdio_lookup[0] = hdl;
            stdin->fd = 0;
            __stdio_in_out_err[0].fd.direct = fd;
            break;
        }

        case 1 :
        {
            ob = obj_matchpath("/stdio/out", &notfound, NULL);
            if(notfound)
            {
                goto __ERR_STDIO_SET;
            }

            hdl = handle_new();
            if(!hdl)
            {
                goto __ERR_STDIO_SET;
            }

            handle_init(hdl, ob, mode, 0);
            handle_linkobj(hdl, ob);
            __stdio_lookup[1] = hdl;
            stdout->fd = 1;
            if(runmode & CN_STDIO_STDOUT_FOLLOW)
                __stdio_in_out_err[1].fd.follow = &(__stdio_in_out_err[0].fd.direct);
            else
                __stdio_in_out_err[1].fd.direct = fd;

            break;
        }

        case 2 :
        {
            ob = obj_matchpath("/stdio/err", &notfound, NULL);
            if(notfound)
                goto __ERR_STDIO_SET;

            hdl = handle_new();
            if(!hdl)
            {
                goto __ERR_STDIO_SET;
            }

            handle_init(hdl, ob, mode, 0);
            handle_linkobj(hdl, ob);
            __stdio_lookup[2] = hdl;
            stderr->fd = 2;
            if(runmode & CN_STDIO_STDERR_FOLLOW)
                __stdio_in_out_err[2].fd.follow = &(__stdio_in_out_err[0].fd.direct);
            else
                __stdio_in_out_err[2].fd.direct = fd;

            break;
        }

        default:
            break;
    }

    return (0);

__ERR_STDIO_SET:

    __filebuf_del(fp);
    free(fp);
    if(hdl)
        __stdio_close(hdl);

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
    struct obj *stdio_root;
    u8 i;

    stdio_root = obj_newchild_set(objsys_root(), "stdio", __stdio_ops, 0, O_RDWR);
    if(!stdio_root)
        return (-1);

    // �����ͳ�ʼ��in��out��err�����ļ�
    for(i = 0; i < 3; i++)
    {
        __stdio_in_out_err[i].fd.direct = -1;
        __stdio_in_out_err[i].runmode = 0;
    }

    if(runmode & CN_STDIO_STDIN_MULTI)
        __stdio_in_out_err[0].runmode |= CN_STDIO_STDIN_MULTI; // STDIN�Ƕ�·����ģʽ��

    if(runmode & (CN_STDIO_STDOUT_FOLLOW))
    {
        __stdio_in_out_err[1].runmode |= CN_STDIO_STDOUT_FOLLOW; // STDOUT����STDINָ��
        __stdio_in_out_err[1].fd.follow = &__stdio_in_out_err[0].fd.direct;
    }

    if(runmode & CN_STDIO_STDERR_FOLLOW)
    {
        __stdio_in_out_err[2].runmode |= CN_STDIO_STDERR_FOLLOW; // STDERR����STDINָ��
        __stdio_in_out_err[2].fd.follow = &__stdio_in_out_err[0].fd.direct;
    }

    if(!obj_newchild(stdio_root, NULL, O_RDWR, (ptu32_t)(&__stdio_in_out_err[0]), "in"))
        return (-1);

    if(!obj_newchild(stdio_root, NULL, O_RDWR, (ptu32_t)(&__stdio_in_out_err[1]), "out"))
        return (-1);

    if(!obj_newchild(stdio_root, NULL, O_RDWR, (ptu32_t)(&__stdio_in_out_err[2]), "err"))
        return (-1);

    return (0);
}
// ============================================================================
// ���ܣ���װSTDIOģ�飻
// ������in -- ��׼����ָ����ļ���
//      out -- ��׼���ָ����ļ���
//      err -- ��׼�������ָ����ļ���
// ���أ��ɹ���0��;ʧ�ܣ�-1����
// ��ע��STDIO������ģʽ�ǲ��ܶ�̬�ĵġ������ʼ��ʱ���õ��Ǹ���ģʽ���߶�����ģʽ����ôϵͳ
//      ���������Ժ󣬲��ܶ�̬�ı䣨�漰����ر���û�д�������
// ============================================================================
s32 ModuleInstall_STDIO(const char *in,const char *out, const char *err)
{
    char *inname, *outname, *errname;
    s32 res, inFD, fd;
    u32 mode, runmode = CFG_STDIO_RUN_MODE;

    res = __stdio_build(CFG_STDIO_RUN_MODE);
    if(res)
    {
        debug_printf("module","STDIO install failed(cannot build).");
        return (-1);
    }

    // STDIN��ʼ��
    if(runmode & (CN_STDIO_STDOUT_FOLLOW | CN_STDIO_STDERR_FOLLOW))
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
    res = __stdio_set(0, inFD, O_RDWR, runmode);
    if(res)
    {
        debug_printf("module","STDIO install failed(\"in\" cannot set).");
        goto __INSTALL_STDIO_ERR;
    }

     // STDOUT ��ʼ��
    if (runmode & CN_STDIO_STDOUT_FOLLOW)
    {
        outname = inname; // STDOUT ���� STDIN��ʹ�õ���ͬһ���ļ���
        fd = inFD;
    }
    else
    {
        fd = open(out, O_RDWR | O_APPEND);
        outname = (char*)out;
    }

    res = __stdio_set(1, fd, (O_RDWR | O_APPEND), runmode);
    if(res)
    {
        debug_printf("module","STDIO install failed(\"out\" cannot set).");
        goto __INSTALL_STDIO_ERR;
    }

    // STDERR��ʼ��
    if (runmode & CN_STDIO_STDERR_FOLLOW)
    {
        errname = inname; // STDOUT ���� STDIN��ʹ�õ���ͬһ���ļ���
        fd = inFD;
    }
    else
    {
        fd = open(err, O_RDWR | O_APPEND);
        errname = (char*)err;
    }

    res = __stdio_set(2, fd, (O_RDWR | O_APPEND), runmode);
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
// ���ܣ���ȡ��׼IO��in��out��err�����ļ���Ϣ��
// ������fd -- ��׼IO���ͣ�0�����룩��1���������2�������������
// ���أ���׼IO���ļ���Ϣ��
// ��ע����Ϊ"stdin"�ȵĽṹ�е�fd���ļ��ţ�����Ϊ�ı����0�ȣ�
//      ��ᵼ�¶�дʱ�Ҳ�����ԭʼ�������ģ��˴��Ĳ��ұ�ʾΪ�˱�����������ģ�
// ============================================================================
struct objhandle *fd2stdio(s32 fd)
{
    struct objhandle *hdl;

    switch(fd)
    {
        case 0: hdl = __stdio_lookup[0]; break;
        case 1: hdl = __stdio_lookup[1]; break;
        case 2: hdl = __stdio_lookup[2]; break;
        default:hdl = NULL; break;
    }

    return (hdl);
}

// ============================================================================
// ���ܣ���һ���ļ����뵽��׼�������������
// ������name -- �ļ�·����
// ���أ��ɹ�����(TREE)��ʧ�ܣ�FALSE����
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