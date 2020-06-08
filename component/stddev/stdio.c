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
//所属模块：标准IO模块
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 标准输入输出管理
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2015-09-11
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 创建文件
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
#include <djyos.h>
#include <int.h>
#include "dbug.h"
#include "component_config_stdio.h"

//以下参数在初始化module时确定，运行中不可更改
//如果定义了多stdin以及stdout跟随，则可以实现uart和Telnet均可以作为终端使用，且
//根据用户的输入命令的设备自动切换。注意，设为跟随时，stdin必须是可读写的。
#define CN_STDIO_STDOUT_FOLLOW      2       // stdout跟随stdin
#define CN_STDIO_STDERR_FOLLOW      4       // stderr跟随stdin
#define CN_STDIO_STDIN_MULTI        8       // 允许stdin多源，典型地是uart和telnet

s32 (* PutStrDirect)(const char *buf,u32 len);
char (* GetCharDirect)(void);

static struct MultiplexSetsCB *stdin_multiplexset; // in的侦听集；
static FILE __stdio_filestruct[3] = {
        {
            .fd = -1, // 表示未初始化；
        },
        {
            .fd = -1, // 表示未初始化；
        },
        {
            .fd = -1, // 表示未初始化；
        },
};

//
// STDIO文件信息
//
static struct __stdio{
    u32 runmode; // 初始化时配置的运行模式；
    union {
        s32 direct; // 非跟随模式下的文件指向;
        s32 *follow; // 跟随模式下的文件指向，其实际指向的是STDIN的dwDirect;
    }fd;
}__stdio_in_out_err[3];

//
// 0、1、2三个文件的查询表；
//
static struct objhandle *__stdio_lookup[3];

FILE *stdin  = (FILE*)&__stdio_filestruct[0];
FILE *stdout = (FILE*)&__stdio_filestruct[1];
FILE *stderr = (FILE*)&__stdio_filestruct[2];

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
s32 skip_atoi(const char **s)
{
    register s32 i=0;

    while(isdigit((s32)**s))
        i = i*10 + *((*s)++) - '0';

    return i;
}

// ============================================================================
// 功能：写STDIO；
// 参数：hdl -- STDIO对象句柄；
//      data -- 写数据；
//      size -- 写数据大小；
// 返回：实际写出大小；
// 备注：
// ============================================================================
static s32 __stdio_write(struct objhandle *hdl, u8 *data, u32 size)
{
    struct __stdio *stdio;
    s32 fd;
    s32 res = 0;

    stdio = (struct __stdio*)handle_GetHostObjectPrivate(hdl);
    if(stdio->runmode & (CN_STDIO_STDOUT_FOLLOW | CN_STDIO_STDERR_FOLLOW))
    {
        fd = *(stdio->fd.follow); // 当前文件处于跟随模式
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
// 功能：读STDIO；
// 参数：hdl -- STDIO对象句柄；
//      data -- 读缓存；
//      size -- 读数据大小；
// 返回：实际读入大小；
// 备注：
// ============================================================================
static s32 __stdio_read(struct objhandle *hdl, u8 *buf, u32 size)
{
    struct __stdio *stdio;
    s32 fd, res = 0;

    stdio = (struct __stdio*)handle_GetHostObjectPrivate(hdl);
    if(stdio->runmode&CN_STDIO_STDIN_MULTI) // 只有标准输入文件存在多源
    {
        fd = Multiplex_Wait(stdin_multiplexset, NULL, handle_gettimeout(hdl)); // timeout是forever
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
// 功能：打开STDIO；
// 参数：ob -- STDIO对象；
//      mode -- 打开模式；
// 返回：成功（STDIO对象句柄）；失败（NULL）；
// 备注：
// ============================================================================
static struct objhandle *__stdio_open(struct Object *ob, u32 mode)
{
    struct __stdio *stdio;
    struct objhandle *hdl;
    s32 fd, res;
    u32 timeout = 0;

    if(Handle_FlagIsDirectory(mode))
    {
        if(!File_ObjIsMount(ob))
            return (NULL); // 只有针对"stdio"目录的操作才是允许的
    }
    else
    {
        stdio = (struct __stdio*)OBJ_GetPrivate(ob);
        if(stdio->runmode & (CN_STDIO_STDOUT_FOLLOW | CN_STDIO_STDERR_FOLLOW))
            fd = *stdio->fd.follow;
        else
            fd = stdio->fd.direct;

        res = fcntl(fd, F_SETTIMEOUT, &timeout);
        if(res)
            return (NULL); // 无法访问所定向的文件；
    }

    hdl = Handle_New();
    if(hdl)
    {
        Handle_Init(hdl, NULL, mode, 0);
        if(timeout)
            Handle_SetTimeOut(hdl, timeout);
    }

    return (hdl);
}

// ============================================================================
// 功能：关闭STDIO；
// 参数：hdl -- STDIO对象句柄；
// 返回：成功（0）；失败（-1）；
// 备注：
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

//  return (Handle_Delete(hdl));
    return;
}

// ============================================================================
// 功能：STDIO重定向；
// 参数：hdl -- STDIO对象句柄；
//      fd -- 新定向的文件；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 __stdio_redirect(struct objhandle *hdl, s32 fd)
{
    struct __stdio *stdio;

    if(File_IsDirectory(hdl))
        return (-1); // 只有STDIO是目录，但其不允许重新定向；

    stdio = (struct __stdio*)handle_GetHostObjectPrivate(hdl);
    if(!(stdio->runmode & (CN_STDIO_STDOUT_FOLLOW | CN_STDIO_STDERR_FOLLOW)))
        close(stdio->fd.direct); // 非跟随模式时，尝试关闭旧的定向文件；

    stdio->fd.direct = fd;
    // 跟随模式时，因为重定向了，无法继续跟随；
    if(stdio->runmode & (CN_STDIO_STDOUT_FOLLOW | CN_STDIO_STDERR_FOLLOW))
        stdio->runmode &= ~(CN_STDIO_STDERR_FOLLOW | CN_STDIO_STDOUT_FOLLOW);

    return (0);
}

// ============================================================================
// 功能：STDIN复用集操作；
// 参数：hdl -- STDIO对象句柄；
//      acts -- 添加（1）；删除（0）；
//      fd -- 文件；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 __stdio_multi(struct objhandle *hdl, u32 acts, s32 fd)
{
    struct __stdio *stdio;
    s32 res = -1;

    stdio = (struct __stdio*)handle_GetHostObjectPrivate(hdl);
    if((!(stdio->runmode & CN_STDIO_STDIN_MULTI))&&(!stdin_multiplexset));
        return (-1); // 只有输入才存在多路复用集；

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
// 功能：STDIN的TAG操作；
// 参数：hdl -- STDIO对象句柄；
//      acts -- 设置（1）；清除（0）；
//      flags -- TAG位；
// 返回：成功（0）；失败（-1）；
// 备注：
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
// 功能：查询stdio状态；
// 参数：ob -- stdio对象；
//      data -- stdio状态；
// 返回：
// 备注：
// ============================================================================
static s32 __stdio_stat(struct Object *ob, struct stat *data)
{
    memset(data, 0x0, sizeof(struct stat));

    data->st_mode = S_IFREG|S_IRUGO|S_IWUGO;

    return (0);
}
//
//// ============================================================================
//// 功能：STDIO timeout操作
//// 参数：hdl -- stdio对象句柄；
////      act -- 1 设置时间；0 获取时间；
////      timeout -- 时间；
//// 返回：
//// 备注：
//// ============================================================================
//static s32 __stdio_timeout(struct objhandle *hdl, u32 acts, u32 *timeout)
//{
//    struct __stdio *stdio;
//    s32 fd, res = 0;
//
//    if(File_IsDirectory(hdl))
//    {
//        // 只有STDIO是目录
//        if(1 == acts)
//            Handle_SetTimeOut(hdl, *timeout);
//        else
//            *timeout = handle_gettimeout(hdl);
//    }
//    else
//    {
//        stdio = (struct __stdio*)handle_GetHostObjectPrivate(hdl);
//        if(stdio->runmode & (CN_STDIO_STDOUT_FOLLOW | CN_STDIO_STDERR_FOLLOW))
//            fd = *stdio->fd.follow;
//        else
//            fd = stdio->fd.direct;
//
//        switch (acts)
//        {
//            case 1:
//                    res = fcntl(fd,  F_SETTIMEOUT, *timeout); // 设置被定向文件的timeout
//                    if(!res)
//                        Handle_SetTimeOut(hdl, *timeout);
//                    break;
//
//            case 0:
//                    res = fcntl(fd,  F_GETTIMEOUT, timeout);
//                    break;
//
//            default:
//                    res = -1;
//                    break;
//        }
//    }
//
//    return (res);
//}

// ============================================================================
// 功能：stdio对象集合（类）操作集
// 参数：见enum objops说明；
// 返回：见enum objops说明；
// 备注：
// ============================================================================
s32 __stdio_ops(void *opsTarget, u32 objcmd, ptu32_t OpsArgs1,
                        ptu32_t OpsArgs2, ptu32_t OpsArgs3)
{
    s32 result = CN_OBJ_CMD_EXECUTED;
    switch(objcmd)
    {
        case CN_OBJ_CMD_OPEN:
        {
            struct objhandle *hdl;
            hdl = __stdio_open((struct Object *)opsTarget, (u32)(*(u64*)OpsArgs2));
            *(struct objhandle **)OpsArgs1 = hdl;
            break;
        }

//      case OBJ_TRAVERSE:
//      {
//          return(-1); // 读目录项；
//      }

        case CN_OBJ_CMD_READ:
        {
            ssize_t len;
            struct objhandle *devfile = (struct objhandle*)opsTarget;

            len = __stdio_read(devfile, (u8*)OpsArgs2, (ssize_t)OpsArgs3);
            *(ssize_t *)OpsArgs1 = len;
            break;
        }

        case CN_OBJ_CMD_WRITE:
        {
            ssize_t len;
            struct objhandle *devfile = (struct objhandle*)opsTarget;

            len = __stdio_write(devfile, (u8*)OpsArgs2, (ssize_t)OpsArgs3);
            *(ssize_t *)OpsArgs1 = len;
            break;
        }

        case CN_OBJ_CMD_CLOSE:
        {
            if(__stdio_close((struct objhandle*)opsTarget) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }

        case CN_OBJ_CMD_SEEK:
        {
            result = (CN_OBJ_CMD_UNSUPPORT);
            break;
        }

        case CN_OBJ_CMD_STAT:
        {
            char * path = (char*)OpsArgs2;
            if(path&&('\0'!=*path))
                return (-1); // 查询的文件不存在；
            if(__stdio_stat((struct Object*)opsTarget, (struct stat *)OpsArgs1) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }

        case CN_OBJ_FCNTL:
        {
            struct objhandle *hdl = (struct objhandle*)opsTarget;
            u32 cmd = OpsArgs2;
//          list = (va_list)va_arg(list, va_list);
            switch(cmd)
            {
                case F_STDIO_MULTI_ADD:
                {
                    s32 fd = va_arg(*(va_list*)OpsArgs3, s32);
                    __stdio_multi(hdl, 1, fd);
                    break;
                }

                case F_STDIO_MULTI_DEL:
                {
                    s32 fd = va_arg(*(va_list*)OpsArgs3, s32);
                    __stdio_multi(hdl, 0, fd);
                    break;
                }

                case F_STDIO_REDRIECT:
                {
                    s32 fd = va_arg(*(va_list*)OpsArgs3, s32);
                    __stdio_redirect(hdl, fd);
                    break;
                }

                default : break;
            }
            result = CN_OBJ_CMD_TRUE;
            break;
        }

        case CN_OBJ_IOCTL:
        {
            result = CN_OBJ_CMD_UNSUPPORT;
            break;
        }

#if 0
        case (CN_OBJ_FCNTL_START + F_OF_SETTAG):
        {
            struct objhandle *hdl = (struct objhandle*)opsTarget;
            ptu32_t tag = (ptu32_t)OpsArgs1;

            return ((ptu32_t)__stdio_tag(hdl, 1, tag));
        }

        case (CN_OBJ_FCNTL_START + F_OF_CLRTAG):
        {
            struct objhandle *hdl = (struct objhandle*)opsTarget;
            ptu32_t tag = (ptu32_t)OpsArgs1;

            return ((ptu32_t)__stdio_tag(hdl, 0, tag));
        }
#endif

#if 0
        case (CN_OBJ_FCNTL_START + F_STDIO_MULTI_ADD):
        {
            struct objhandle *hdl = (struct objhandle*)opsTarget;
            u32 add = (u32)OpsArgs1;

            return ((ptu32_t)__stdio_multi(hdl, 1, add));
        }

        case (CN_OBJ_FCNTL_START + F_STDIO_MULTI_DEL):
        {
            struct objhandle *hdl = (struct objhandle*)opsTarget;
            u32 del = (u32)OpsArgs1;

            return ((ptu32_t)__stdio_multi(hdl, 0, del));
        }

        case (CN_OBJ_FCNTL_START + F_STDIO_REDRIECT):
        {
            struct objhandle *hdl = (struct objhandle*)opsTarget;
            u32 fd = (u32)OpsArgs1;

            return ((ptu32_t)__stdio_redirect(hdl, fd));
        }
#endif
        default:
        {
            result = CN_OBJ_CMD_UNSUPPORT;
            break;
        }
    }

    return result;
}

// ============================================================================
// 功能：初始化STDIO（in、out和err）的对应文件流
// 参数：type -- 标准IO类型，0（输入），1（输出），2（错误输出）；
//      fd -- 标准IO文件流定向到的文件；
//      mode -- 标准IO文件的模式；
//      runmode -- 标准IO文件的运行模式；
// 返回：成功（0）；失败（-1）；
// 备注：因为"STDIN"等的结构中的fd（文件号）被人为改变成了0等，
//      这会导致读写时找不到其原始的上下文；此处的查找表示为了保存这个上下文；
// ============================================================================
static s32 __stdio_set(u32 type, FILE *fp, u32 mode, u32 runmode)
{
    char *notfound;
    struct Object *ob;
    struct stat info;
    s32 res;
    FILE *new_fp;
    struct objhandle *hdl = NULL;
    extern s32 __File_BufNew(FILE *stream);
    extern void __File_BufDel(FILE *stream);

    res = fstat(fp->fd, &info);
    if(res)
        return (-1);

    new_fp = (FILE*)&__stdio_filestruct[type];
    new_fp->ungetbuf = EOF;
    new_fp->flags = fp->flags;
    if((DJY_GetRunMode() < CN_RUNMODE_MP) && (S_ISFLOW(info.st_mode)))
    {
        res = __File_BufNew(new_fp);
        if(res)
            goto __ERR_STDIO_SET;
    }

    fcntl(fp->fd, F_SETTIMEOUT, CN_TIMEOUT_FOREVER); // 将所定向的设备设置为forver（阻塞式）；
    switch(type)
    {
        case 0 :
        {
            // 如果支持多STDIN输入源，则创建多路复用集，并把初始化时指定的IN文件加入
            // 到多路复用集中（TODO：，否则，按 StdinEdit参数设置 stdinname 文件属性）。
            if(runmode&CN_STDIO_STDIN_MULTI)
            {
                stdin_multiplexset = Multiplex_Create(1);
                if(!stdin_multiplexset)
                {
                    goto __ERR_STDIO_SET;
                }

                // 将当前打开的文件加入监听集合
                if(FALSE == Multiplex_AddObject(stdin_multiplexset, fp->fd,
                        (CN_MULTIPLEX_SENSINGBIT_READ + CN_MULTIPLEX_SENSINGBIT_OR)))
                {
                    goto __ERR_STDIO_SET;// TODO: 没有找到删除多路复用集的函数；
                }
            }

            ob = OBJ_MatchPath("/stdio/in", &notfound);
            if(notfound)
            {
                goto __ERR_STDIO_SET;
            }

            hdl = Handle_New();
            if(!hdl)
            {
                goto __ERR_STDIO_SET;
            }

            Handle_Init(hdl, ob, mode, 0);
            OBJ_LinkHandle(hdl, ob);
            __stdio_lookup[0] = hdl;
            stdin->fd = 0;
            __stdio_in_out_err[0].fd.direct = fp->fd;
            break;
        }

        case 1 :
        {
            ob = OBJ_MatchPath("/stdio/out", &notfound);
            if(notfound)
            {
                goto __ERR_STDIO_SET;
            }

            hdl = Handle_New();
            if(!hdl)
            {
                goto __ERR_STDIO_SET;
            }

            Handle_Init(hdl, ob, mode, 0);
            OBJ_LinkHandle(hdl, ob);
            __stdio_lookup[1] = hdl;
            stdout->fd = 1;
            if(runmode & CN_STDIO_STDOUT_FOLLOW)
                __stdio_in_out_err[1].fd.follow = &(__stdio_in_out_err[0].fd.direct);
            else
                __stdio_in_out_err[1].fd.direct = fp->fd;

            break;
        }

        case 2 :
        {
            ob = OBJ_MatchPath("/stdio/err", &notfound);
            if(notfound)
                goto __ERR_STDIO_SET;

            hdl = Handle_New();
            if(!hdl)
            {
                goto __ERR_STDIO_SET;
            }

            Handle_Init(hdl, ob, mode, 0);
            OBJ_LinkHandle(hdl, ob);
            __stdio_lookup[2] = hdl;
            stderr->fd = 2;
            if(runmode & CN_STDIO_STDERR_FOLLOW)
                __stdio_in_out_err[2].fd.follow = &(__stdio_in_out_err[0].fd.direct);
            else
                __stdio_in_out_err[2].fd.direct = fp->fd;

            break;
        }

        default:
            break;
    }

    return (0);

__ERR_STDIO_SET:

    __File_BufDel(new_fp);
    free(new_fp);
    if(hdl)
        __stdio_close(hdl);

    return (-1);
}

// ============================================================================
// 功能：注销STDIO系统
// 参数：
// 返回：
// 备注：
// ============================================================================
static void __stdio_destory(void)
{
    ;
}
// ============================================================================
// 功能：建立STDIO系统
// 参数：runmode -- STDIO系统运行模式；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 __stdio_build(u32 runmode)
{
    struct Object *stdio_root;
    u8 i;

    stdio_root = OBJ_NewChild(OBJ_GetRoot(), __stdio_ops, 0, "stdio");
    if(!stdio_root)
        return (-1);

    // 建立和初始化in、out和err三个文件
    for(i = 0; i < 3; i++)
    {
        __stdio_in_out_err[i].fd.direct = -1;
        __stdio_in_out_err[i].runmode = 0;
    }

    if(runmode & CN_STDIO_STDIN_MULTI)
        __stdio_in_out_err[0].runmode |= CN_STDIO_STDIN_MULTI; // STDIN是多路复用模式；

    if(runmode & (CN_STDIO_STDOUT_FOLLOW))
    {
        __stdio_in_out_err[1].runmode |= CN_STDIO_STDOUT_FOLLOW; // STDOUT跟随STDIN指向
        __stdio_in_out_err[1].fd.follow = &__stdio_in_out_err[0].fd.direct;
    }

    if(runmode & CN_STDIO_STDERR_FOLLOW)
    {
        __stdio_in_out_err[2].runmode |= CN_STDIO_STDERR_FOLLOW; // STDERR跟随STDIN指向
        __stdio_in_out_err[2].fd.follow = &__stdio_in_out_err[0].fd.direct;
    }

    if(!OBJ_NewChild(stdio_root, __stdio_ops, (ptu32_t)(&__stdio_in_out_err[0]), "in"))
        return (-1);

    if(!OBJ_NewChild(stdio_root, __stdio_ops, (ptu32_t)(&__stdio_in_out_err[1]), "out"))
        return (-1);

    if(!OBJ_NewChild(stdio_root, __stdio_ops, (ptu32_t)(&__stdio_in_out_err[2]), "err"))
        return (-1);

    return (0);
}
// ============================================================================
// 功能：安装STDIO模块；
// 参数：in -- 标准输入指向的文件；
//      out -- 标准输出指向的文件；
//      err -- 标准错误输出指向的文件；
// 返回：成功（0）;失败（-1）；
// 备注：STDIO的运行模式是不能动态改的。例如初始化时设置的是跟随模式或者多输入模式，那么系统
//      运行起来以后，不能动态改变（涉及到相关变量没有创建）；
// ============================================================================
s32 ModuleInstall_STDIO(const char *in,const char *out, const char *err)
{
    char *inname, *outname, *errname;
    s32 res = -1;
    FILE *inFD, *fd;
    u32 runmode = 0;
    char *mode;

    if(CFG_STDIO_STDIN_MULTI == true)
        runmode |= CN_STDIO_STDIN_MULTI;
    if(CFG_STDIO_STDOUT_FOLLOW == true)
        runmode |= CN_STDIO_STDOUT_FOLLOW;
    if(CFG_STDIO_STDERR_FOLLOW == true)
        runmode |= CN_STDIO_STDERR_FOLLOW;

    res = __stdio_build(runmode);
    if(res)
    {
        debug_printf("module","STDIO install failed(cannot build).");
        return (-1);
    }

    // STDIN初始化
    if(runmode & (CN_STDIO_STDOUT_FOLLOW | CN_STDIO_STDERR_FOLLOW))
    {
        // STDOUT或STDERR只要有一个跟随STDIN，STDIN都必须以可读写的模式打开。
        mode = "a+";
    }
    else
    {
        mode =  "r";
    }

    inFD = fopen(in,mode);
    inname = (char*)in;
    if(inFD != NULL)
    {
        res = __stdio_set(0, inFD, O_RDWR, runmode);
    }
    else
    {
        res = -1;
    }
    if(res == -1)
    {
        debug_printf("module","STDIO install failed(\"in\" cannot set).");
        goto __INSTALL_STDIO_ERR;
    }


     // STDOUT 初始化
    if (runmode & CN_STDIO_STDOUT_FOLLOW)
    {
        outname = inname; // STDOUT 跟随 STDIN，使用的是同一个文件；
        fd = inFD;
    }
    else
    {
        fd = fopen(out, "a+");
        outname = (char*)out;
    }

    if(fd != NULL)
    {
        res = __stdio_set(1, fd, (O_RDWR | O_APPEND), runmode);
    }
    else
    {
        res = -1;
    }
    if(res == -1)
    {
        debug_printf("module","STDIO install failed(\"out\" cannot set).");
        goto __INSTALL_STDIO_ERR;
    }

    // STDERR初始化
    if (runmode & CN_STDIO_STDERR_FOLLOW)
    {
        errname = inname; // STDOUT 跟随 STDIN，使用的是同一个文件；
        fd = inFD;
    }
    else
    {
        fd = fopen(err, "a+");
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
// 功能：获取标准IO（in、out和err）的文件信息；
// 参数：fd -- 标准IO类型，0（输入），1（输出），2（错误输出）；
// 返回：标准IO的文件信息；
// 备注：因为"stdin"等的结构中的fd（文件号）被人为改变成了0等，
//      这会导致读写时找不到其原始的上下文；此处的查找表示为了保存这个上下文；
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
// 功能：将一个文件加入到标准输入的侦听集；
// 参数：name -- 文件路径；
// 返回：成功添加(TREE)；失败（FALSE）；
// 备注：
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
    dev->name = name; // TODO：有隐患；
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
