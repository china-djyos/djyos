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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
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
    .fd = -1, // 表示未初始化；
};

//
// STDIO文件信息
//
static struct __stdio_file{
    struct __portBasic basic;
    u32 runmode; // 初始化时配置的运行模式；
    union {
        s32 direct; // 非跟随模式下的文件指向;
        s32 *follow; // 跟随模式下的文件指向，其实际指向的是STDIN的dwDirect;
    }fd;
}__stdio_files[3];

//
// 0、1、2三个文件的查询表；
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
// 功能：写STDIO文件；
// 参数：of -- STDIO文件；
//      data -- 写数据；
//      size -- 写数据大小；
// 返回：实际写出大小；
// 备注：
// ============================================================================
static s32 __stdio_filewrite(tagOFile *of, u8 *data, u32 size)
{
    struct __stdio_file *stdio;
    s32 fd;
    s32 res = 0;

    stdio = dListEntry(of_basic(of), struct __stdio_file, basic);
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
// 功能：读STDIO文件；
// 参数：of -- STDIO文件；
//      data -- 读缓存；
//      size -- 读数据大小；
// 返回：实际读入大小；
// 备注：
// ============================================================================
static s32 __stdio_fileread(tagOFile *of, u8 *buf, u32 size)
{
    struct __stdio_file *stdio;
    s32 fd, res = 0;

    stdio = dListEntry(of_basic(of), struct __stdio_file, basic);
    if(stdio->runmode&CN_STDIO_STDIN_MULTI) // 只有标准输入文件存在多源
    {
        fd = Multiplex_Wait(stdin_multiplexset, NULL, of_timeout(of)); // timeout是forever
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
// 功能：打开STDIO文件；
// 参数：obj -- STDIO文件对象；
//      mode -- 打开模式；
// 返回：
// 备注：
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
            return (NULL); // 只有针对"stdio"目录的操作才是允许的
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
            return (NULL); // 无法访问所定向的文件；
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
// 功能：关闭STDIO；
// 参数：
// 返回：
// 备注：
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
// 功能：STDIO重定向；
// 参数：of -- 文件上下文；
//      fd -- 新定向的文件；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 __stdio_fileredirect(tagOFile *of, u32 fd)
{
    struct __stdio_file *stdio;

    if(isdirectory(of))
        return (-1); // 只有STDIO是目录，但其不允许重新定向；

    stdio = dListEntry(of_basic(of), struct __stdio_file, basic);
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
// 参数：of -- STDIO文件上下文；
//      acts -- 添加（1）；删除（0）；
//      fd -- 文件；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 __stdio_filemulti(tagOFile *of, u32 acts, u32 fd)
{
    struct __stdio_file *stdio;
    s32 res = -1;

    if(isdirectory(of))
        return (-1); // 只有STDIO是目录，但其不允许操作复用集；

    stdio = dListEntry(of_basic(of), struct __stdio_file, basic);

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
// 参数：of -- STDIO文件上下文；
//      acts -- 设置（1）；清除（0）；
//      flags -- TAG位；
// 返回：成功（0）；失败（-1）；
// 备注：
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
// 功能：查询stdio文件状态
// 参数：
// 返回：
// 备注：
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
// 功能：STDIO timeout操作
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 __stdio_filetimeout(tagOFile *of, u32 acts, u32 *timeout)
{
    struct __stdio_file *stdio;
    s32 fd, res = 0;

    if(isdirectory(of))
    {
        // 只有STDIO是目录
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
// 功能：
// 参数：
// 返回：
// 备注：
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
            return (-1); // STDIO文件不允许删除；
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
// 功能：初始化STDIO（in、out和err）的对应文件流
// 参数：type -- 标准IO类型，0（输入），1（输出），2（错误输出）；
//      fd -- 标准IO文件流定向到的文件；
//      mode -- 标准IO文件的模式；
//      runmode -- 标准IO文件的运行模式；
// 返回：成功（0）；失败（-1）；
// 备注：因为"STDIN"等的结构中的fd（文件号）被人为改变成了0等，
//      这会导致读写时找不到其原始的上下文；此处的查找表示为了保存这个上下文；
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

    fcntl(fd, F_OF_SETTIMEOUT, CN_TIMEOUT_FOREVER); // 将所定向的设备设置为forver（阻塞式）；
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
                if(FALSE == Multiplex_AddObject(stdin_multiplexset, fd,
                        (CN_MULTIPLEX_SENSINGBIT_READ + CN_MULTIPLEX_SENSINGBIT_OR)))
                {
                    goto __ERR_STDIO_SET;// TODO: 没有找到删除多路复用集的函数；
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
    struct Object *root;
    u8 i;

    root = __mounto("stdio", "/", (tagObjOps)__stdio_fileops, 0);
    if(!root)
        return (-1);

    // 建立和初始化in、out和err三个文件
    for(i = 0; i < 3; i++)
    {
        __stdio_files[i].fd.direct = -1;
        __stdio_files[i].runmode = 0;
        of_initbasic(&__stdio_files[i].basic, O_RDWR);
    }

    if(runmode & CN_STDIO_STDIN_MULTI)
        __stdio_files[0].runmode |= CN_STDIO_STDIN_MULTI; // STDIN是多路复用模式；

    if(runmode & (CN_STDIO_STDOUT_FOLLOW))
    {
        __stdio_files[1].runmode |= CN_STDIO_STDOUT_FOLLOW; // STDOUT跟随STDIN指向
        __stdio_files[1].fd.follow = &__stdio_files[0].fd.direct;
    }

    if(runmode & CN_STDIO_STDERR_FOLLOW)
    {
        __stdio_files[2].runmode |= CN_STDIO_STDERR_FOLLOW; // STDERR跟随STDIN指向
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
// 功能：安装STDIO模块；
// 参数：runmode -- 运行模式；
//      in -- 标准输入指向的文件；
//      out -- 标准输出指向的文件；
//      err -- 标准错误输出指向的文件；
// 返回：成功（0）;失败（-1）；
// 备注：STDIO的运行模式是不能动态改的。例如初始化时设置的是跟随模式或者多输入模式，那么系统
//      运行起来以后，不能动态改变（涉及到相关变量没有创建）；
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

    // STDIN初始化
    if(CFG_STDIO_RUN_MODE & (CN_STDIO_STDOUT_FOLLOW | CN_STDIO_STDERR_FOLLOW))
    {
        // STDOUT或STDERR只要有一个跟随STDIN，STDIN都必须以可读写的模式打开。
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

     // STDOUT 初始化
    if (CFG_STDIO_RUN_MODE & CN_STDIO_STDOUT_FOLLOW)
    {
        outname = inname; // STDOUT 跟随 STDIN，使用的是同一个文件；
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

    // STDERR初始化
    if (CFG_STDIO_RUN_MODE & CN_STDIO_STDERR_FOLLOW)
    {
        errname = inname; // STDOUT 跟随 STDIN，使用的是同一个文件；
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
// 功能：测试是否是未初始化的STDIO；
// 参数：stream -- STDIO文件流；
// 返回：是（1）；不是（0）；未初始化的STDIO（-1）；
// 备注：
// ============================================================================
s32 IsSTDIO(FILE* stream)
{
    if(!stream)
        return (0); // 非法参数

    if(-1 == stream->fd)
        return (-1); // 未初始化的STDIO文件流

    if((0 == stream->fd) || (1 == stream->fd) || (2 == stream->fd))
        return (1); // 是STDIO文件流

    return (0); // 不是STDIO文件流
}

// ============================================================================
// 功能：获取标准IO（in、out和err）的文件信息；
// 参数：type -- 标准IO类型，0（输入），1（输出），2（错误输出）；
// 返回：标准IO的文件信息；
// 备注：因为"stdin"等的结构中的fd（文件号）被人为改变成了0等，
//      这会导致读写时找不到其原始的上下文；此处的查找表示为了保存这个上下文；
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
// 功能：
// 参数：
// 返回：
// 备注：
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
