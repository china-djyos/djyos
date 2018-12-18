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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合以下三条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。
// 3. 本软件作为都江堰操作系统的组成部分，未获事前取得的书面许可，不允许移植到非
//    都江堰操作系统环境下运行。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <stat.h>
#include <unistd.h>
#include "file.h"
#include "dbug.h"
#include "component_config_fs.h"


// ============================================================================
// 功能：测试是否是有效的的文件流；
// 参数：stream -- 文件流；
// 返回：是（1）；不是（0）；
// 备注：
// ============================================================================
s32 isvalid(FILE* stream)
{
    if((!stream) // 非法参数
       ||(-1==stream->fd)) // 未初始化的STDIO文件流
        return (0);

    return (1);
}

// ============================================================================
// 功能：新建文件缓冲；
// 参数：stream -- 文件流；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 __filebuf_new(FILE *stream)
{
    struct __buf *buf;

    if(stream->buf)
        return (-1);

    buf = malloc(sizeof(*buf)+CFG_CLIB_BUFFERSIZE);
    if(!buf)
    {
        return (-1);
    }

    buf->prefetched = buf->movs = buf->data = (u8*)buf + sizeof(*buf);
    buf->size = CFG_CLIB_BUFFERSIZE;
    stream->buf = buf;
    return (0);
}

// ============================================================================
// 功能：删除文件缓冲；
// 参数：stream -- 文件流；
// 返回：
// 备注：
// ============================================================================
void __filebuf_del(FILE *stream)
{
    if(stream->buf)
    {
        free(stream->buf);
        stream->buf = 0;
    }
}

// ============================================================================
// 功能：文件缓冲空闲量；
// 参数：stream -- 文件流；
// 返回：缓冲空闲量；
// 备注：
// ============================================================================
s32 __filebuf_frees(FILE *stream)
{
    return (stream->buf->size - (stream->buf->movs - stream->buf->data));
}

// ============================================================================
// 功能：文件缓冲预取量；
// 参数：stream -- 文件流；
// 返回：
// 备注：
// ============================================================================
s32 __filebuf_fetched(FILE *stream)
{
    return (stream->buf->prefetched - stream->buf->movs);
}

// ============================================================================
// 功能：获取文件流预读的量；
// 参数：stream -- 文件流；
// 返回：文件流预读的量；
// 备注：
// ============================================================================
s32 __filebuf_mark(FILE *stream)
{
    return (stream->buf->prefetched - stream->buf->data);
}

// ============================================================================
// 功能：文件流已缓存的量；
// 参数：stream -- 文件流；
// 返回：已缓存的量；
// 备注：
// ============================================================================
s32 __filebuf_used(FILE*stream)
{
    return (stream->buf->movs - stream->buf->data);
}

// ============================================================================
// 功能：测试是否存在文件缓冲；
// 参数：stream -- 文件流；
// 返回：不存在缓冲（NULL）；存在缓冲（缓冲空间）；
// 备注：
// ============================================================================
s32 __isfilebufed(FILE*stream)
{
    return ((s32)stream->buf);
}

// ============================================================================
// 功能：模式转换
// 参数：pMode -- C库标准文件操作模式；
//      pFlags -- POSIX标准文件操作标志；
//      pCFlags -- 自定义标志
// 返回：成功（0）； 失败（-1）;
// 备注：
// ============================================================================
s32 __transform(const char *pMode, s32 *pFlags, s32 *pCFlags)
{
    bool_t WhileContinue = true;

    //注：O_RDONLY、O_WRONLY、O_RDWR 须符合POSIX（2016）的最新定义
    switch (*pMode)
    {
        case 'r':
            *pFlags = O_RDONLY;
            *pCFlags |= FP_IOREAD;
            break;
        case 'w':
            *pFlags = O_WRONLY | O_CREAT | O_TRUNC;
            *pCFlags |= FP_IOWRITE;
            break;
        case 'a':
            *pFlags = O_WRONLY | O_CREAT | O_APPEND;
            *pCFlags |= FP_IOWRITE + FP_IOREAD + FP_IOAPPEND;
            break;
        default:
            return (-1);
    }

    pMode++;
    while(*pMode && WhileContinue)
    {
        switch(*pMode)
        {
            case '+':
                if (*pFlags & O_RDWR)
                    WhileContinue = false;
                else
                {
                    *pFlags |= O_RDWR;
                    *pFlags &= ~(O_RDONLY | O_WRONLY);
                    *pCFlags |= FP_IORW;
                    *pCFlags |= FP_IOWRITE + FP_IOREAD;
                }
                break;

            case 'b':
                if (*pCFlags & (FP_TEXT | FP_BINARY))
                    WhileContinue = false;
                else
                    *pCFlags |= FP_BINARY;
                break;

            case 't':
                if (*pCFlags & (FP_TEXT | FP_BINARY))
                    WhileContinue = false;
                else
                    *pCFlags |= FP_TEXT;
                break;

            default:
                WhileContinue =false;
                break;
        }
        pMode++;
    }

    return (0);
}


// ============================================================================
// 功能：将文件缓冲区数据写回文件所在设备
// 参数：stream -- 文件流;
// 返回：成功（0）；出错（EOF）；
// 备注：
// ============================================================================
s32 fflush(FILE *stream)
{
    s32 res, buffed;
    u8 *src, *des;
    s32 i;
    off_t offset;

    if(!stream)
        return (EOF); // C库上是更新所有的stream

//    if(-1 == IsSTDIO(stream))
//    {
//        return (EOF);
//    }

    if(__isfilebufed(stream))
    {
        buffed = __filebuf_used(stream);
        if(buffed)
        {
            offset = -__filebuf_mark(stream);
            offset = lseek(stream->fd, offset, SEEK_CUR); // 将预读的空间置回；
            if(-1 == offset)
                return (EOF);

            res = write(stream->fd, stream->buf->data, buffed);
            if(res != buffed)
            {
                buffed = buffed - res; // 未刷下去的部分
                src = stream->buf->data + res;
                des = stream->buf->data;
                for(i = 0; i < (buffed - res); i++)
                {
                    *des = *src;
                    des++;
                    src++;
                }
                stream->buf->movs = stream->buf->data + buffed;
                stream->buf->prefetched = stream->buf->movs;
                return (EOF);
            }
            else
                stream->buf->movs=stream->buf->data;

        }
    }

    return (0);
}

// ============================================================================
// 功能：打开文件 。
// 参数：filename -- 文件所在路径;
//      mode -- 文件操作模式;
// 返回：成功（文件流）; 失败（NULL）;
// 备注：
// ============================================================================
FILE *fopen(const char *filename, const char *mode)
{
    FILE *stream;
    s32 flags=0, cflags=0;
    s32 fd, res;
    struct stat info;

    if((!filename) || (!mode) || ('\0' == *filename))
        return (NULL);

    res = __transform(mode, &flags, &cflags);
    if(res)
    {
        debug_printf("clib","\"fopen\" (%s) failed<bad \"mode\" = %s>", filename, mode);
    }

    fd = open(filename, flags, 0);
    if(fd == -1)
    {
        return (NULL);
    }

    res = fstat(fd, &info);
    if(res)
    {
        debug_printf("clib","\"fopen\" (%s) failed<cannot \"stat\" ", filename);

        res = close(fd);
        if(res)
            debug_printf("clib","and \"close\"");

        debug_printf("clib",">.");
        return (NULL);
    }

    stream = malloc(sizeof(FILE));
    if(!stream)
    {
        debug_printf("clib","\"fopen\" (%s) failed<memory out>.", filename);
        close(fd);
        return (NULL);
    }

    memset(stream, 0, sizeof(FILE));
    //单进程模式下，不存在内核态用户态间切换，c库不做buf，也不怎么影响效率。
    //流式数据文件不能写缓冲，如果不能读，亦不需要buf。
    if((Djy_GetRunMode( ) < CN_RUNMODE_MP )
            || (!(cflags & FP_IOREAD) && (S_ISFLOW(info.st_mode))))
    {
        res = __filebuf_new(stream);
        if(res)
        {
            debug_printf("clib","\"fopen\" (%s) failed<memory out", filename);
            res = close(fd);
            if(res)
                debug_printf("clib"," and \"close\"");

            debug_printf("clib",">.");
            return (NULL);
        }
    }

    stream->fd = fd;
    stream->flags = cflags;
    stream->unget = EOF;
    if(cflags & FP_IOAPPEND)
    {
        stream->pos = info.st_size;
    }

    return (stream);
}

// ============================================================================
// 功能：关闭文件；
// 参数：stream -- 文件流；
// 返回：成功（0）;失败（-1）；
// 备注：
// ============================================================================
int fclose(FILE *stream)
{
    s32 res = 0;

    if(!stream)
        return (-1);

//    if(IsSTDIO(stream))
//        return (-1); // STDIO文件不允许关闭；

    if(EOF==fflush(stream))
    {
        res = -1;
        debug_printf("clib","\"fclose\" file failed<cannot flush>.");
    }

    if(close(stream->fd))
    {
        debug_printf("clib","\"fclose\" file failed<cannot close>.");
        return (-1);
    }

    if(__isfilebufed(stream))
        __filebuf_del(stream);

    free(stream);

    return (res);
}

// ============================================================================
// 功能：创建临时文件；
// 参数：
// 返回：成功（临时文件）;失败（NULL）；
// 备注：文件名通过tmpname产生，或者不关心；
// ============================================================================
FILE *tmpfile(void)
{
    return (NULL); // 则一个特定目录/temp下，建立临时文件；
}

// ============================================================================
// 功能：生成临时文件名；
// 参数：用户指定的临时文件名空间；
// 返回：新生成的临时文件名；
// 备注：s是由用户提供的数据空间，但是其实际大小必须满足L_tmpnam；
// ============================================================================
char *tmpname(char *s)
{
    s = s;
    return (NULL);
}

// ============================================================================
// 功能：把一个字符退回给输入流，输入流的指针从当前位置回退一个字符，并用传入的
//      字符 c 覆盖该字符。只改变缓冲区数据，即使fflush和fclose文件，所退回的
//      字符也不会写入文件。
// 参数：c -- 待返回给输入流的字符；
//      stream -- 目标输入流；
// 返回：成功执行返回参数c，否则返回EOF；
// ============================================================================
s32 ungetc(s32 c, FILE *stream)
{
    if((!stream) || (EOF == c))
        return (EOF);

    if(EOF == stream->unget)
    {
        stream->unget = c;
        return (c);
    }

    return (EOF);
}

// ============================================================================
// 功能：读文件；
// 参数：buf -- 流缓冲;
//      size -- 需要读的单位数;
//      count -- 单位数;
//      stream -- 文件流;
// 返回：读取的记录数（取整），如果文件剩余数据不足，将全部读取，并不按照整数个
//      记录截取数据，此时将无法知道究竟读取了多少数据。
// 备注：
// ============================================================================
size_t fread(void *buf, size_t size, size_t count, FILE *stream)
{
    s32 res, all, prefetch, toBuf, i = 0;
    char ch;

    all = size * count;
    if((!all) || (stream->flags & FP_IOREAD))
        return (0);

    if(EOF != stream->unget)
    {
        *(u8*)buf = (u8)stream->unget;
        stream->unget = EOF;
        if(--all)
            return (1/size);

        i = 1;
    }

    if(!isvalid(stream)) // 文件流是未初始化的STDIO
    {
        if(GetCharDirect) // 函数已注册；（TODO:这个逻辑不应该防止这里实现）
        {
            for(; i < all; i++)
            {
                ch = GetCharDirect();
                if(!ch)
                {
                    ch = EOF;
                    break;
                }

                ((u8*)buf)[i] = ch;
#if 0
                if(PutStrDirect) // 函数已注册；
                    PutStrDirect((const char*)&ch, 1);
#endif
            }

            return (i/size);
        }
    }

    if(!__isfilebufed(stream))
    {
        res = read(stream->fd, buf, all);
    }
    else
    {

        if(all <= __filebuf_fetched(stream))
        {
            // 已预读的文件足够本次读
            memcpy(buf, stream->buf->movs, all);
            stream->buf->movs += all;
            res = all;
        }
        else
        {
            res = fflush(stream);
            if(EOF==res)
            {
                debug_printf("clib","\"fread\" file failed(buffer flash).");
                return (0);
            }

            // 带有预读功能的逻辑，即如果发现读的量不是缓存对齐的，将其进行缓存对齐，这部分大于实际要读的量即为预读；
            prefetch = stream->buf->size - ((stream->pos+all) % stream->buf->size);
            res = read(stream->fd, buf, all + prefetch);
            if(res) // 实际读到的数据可能小于目标
            {
                stream->pos += res;
                toBuf = stream->pos % stream->buf->size; // 将缓存不对齐的部分进行缓存
                memcpy((stream->buf->movs), (buf+res-toBuf), toBuf);
                stream->buf->movs += toBuf;
                stream->buf->prefetched += toBuf;
                if(res > all) // 回读的含有预读
                {
                    // 文件当前位置必须是指定的位置，不能含有预读部分；
                    stream->buf->movs -= (res - all);
                    stream->pos -= (res-all);
                    res = all; // 返回值；
                }
            }
        }
    }

    if(-1==res)
        res = 0; // 出错了，读到的数据为零；

    return (res/size);
}

// ============================================================================
// 功能：写count个单位入文件流；
// 参数：buf -- 需写入的空间；
//      size -- 单位的字节数，即单位的尺寸；
//      count -- 单位的总数;
//      stream -- 文件流;
// 返回： 实际写入的单位总数；
// 备注：
// ============================================================================
size_t fwrite(const void *buf, size_t size, size_t count, FILE *stream)
{
    u32 all, frees, rest;
    s32 res;
    off_t tmp;

    rest = all = size * count;
    if((!all) || (stream->flags & FP_IOREAD))
        return (0);

    if(EOF != stream->unget)
        stream->unget = EOF; // 抛弃掉ungetc的内容

    if(!isvalid(stream)) // TODO
    {
        if(PutStrDirect) // 函数已注册；
        {
            PutStrDirect((const char*)buf, all);
        }

        return (count);
    }

    if(!__isfilebufed(stream))
    {
        res = write(stream->fd, buf, all);
        if(-1!=res)
            stream->pos += res;
        else
            res = 0; // 出错；
    }
    else
    {
        tmp = stream->pos;
        frees = __filebuf_frees(stream);
        while(1)
        {
            if(frees > rest)
                frees = rest;
            memcpy(stream->buf->movs, buf, frees);
            stream->buf->movs += frees;
#if 0
            if(stream->buf->prefetched < stream->buf->pMov)
                stream->buf->prefetched = stream->buf->pMov;
#endif

            if(__filebuf_frees(stream))
            {
                res = all;
                break; // 全部正常写入
            }

            res = fflush(stream); // 缓存已满，将数据输入介质
            if(EOF==res)
            {
                res = stream->pos - tmp; // 实际写入的数据数
                break;
            }
            memset(stream->buf->data, 0xff, stream->buf->size);
//            stream->buf->movs -= stream->buf->size;
            stream->pos += stream->buf->size;
            rest -= frees;
            buf += frees;
            frees = stream->buf->size;
        }
    }

    return (res/size);
}


// ============================================================================
// 功能：从文件输入一个字符，getc和fgetc是等价的，搞笑的c标准。
// 参数：fp，输出目标文件，如果是stdin，则要判断是否设备。
// 返回：读取的字符，错误则返回EOF
// 备注：
// ============================================================================
s32 getc(FILE *stream)     //getc = fgetc
{
    s32 ch;

    if(!stream)
        return (EOF);

    if(1 != fread(&ch, 1, 1, stream))
        ch = EOF;

    return (ch);
}

// ============================================================================
// 功能：从文件输入一行字符串，持续输入直到回车，包含空格。
// 参数：buf -- 输入字符串的缓冲区；
//      size -- buf的长度；
//      stream -- 输入源文件；
// 返回：1、成功，则返回第一个参数buf；
//      2、如果发生读入错误，error指示器被设置，返回NULL，buf的内容可能被改变。
// 备注：
// ============================================================================
char *fgets(char *buf, s32 n, FILE *stream)
{
    s32 i = 0;
    char ch;

    if(!stream || !n || !buf)
        return (NULL);

    if(1 == n)
    {
        *buf = '\0';
        return (buf);
    }

    for (; i < n; i++)
    {
       if(1 != fread(&ch, 1, 1, stream))
       {
           buf[i] = '\0';
           break;
       }

       // TODO:
       // 忽略回车，以换行符结束，能兼容WINDOWS和LINUX格式文件。
       // MAC不用考虑，从 OSx开始已经兼容LINUX了。
       if('\r' == ch)
       {
           continue; // WINDOWS格式文本文件有 '\r'，跳过
       }
       else if('\n' == ch) // 遇换行符，输入结束
       {
           buf[i] = '\0';
           break;
       }

       buf[i] = ch;
   }

    return (buf);
}

// ============================================================================
// 功能：输出一个字符到文件，putc和fputc是等价的，搞笑的c标准
// 参数：ch -- 待输出的字符
//      stream -- 输出目标文件或设备。
// 返回：正确输出则返回被输出的字符，错误则返回EOF
// 备注：
// ============================================================================
s32 putc(s32 ch, FILE *stream)     //putc = fputc
{
    if(!stream)
        return (EOF);

    if(1 !=  fwrite(&ch, 1, 1, stream))
        return (EOF);

    return (ch);
}

// ============================================================================
// 功能：输出一个字符串到文件，注意，不包含串结束符，也不会添加回车换行符
// 参数：s -- 待输出的字符串；
//      stream -- 输出目标文件流；
// 返回：正确输出返回输出的字节数；错误返回EOF；
// 备注：
// ============================================================================
s32 fputs(const char *s, FILE *stream)
{
    s32 len;

    if(!stream)
        return (EOF);

    len = strlen(s);
    if(!len)
        return (0);

    len = fwrite(s, 1, len, stream);
    return (len);
}

// ============================================================================
// 功能：移动读写位置，如果c库没有buf，将直接调用系统的 lseek。
// 参数：stream -- 文件流;
//      offset -- 需移动相对偏置;
//      whence -- offset的起点;
// 返回： 0 成功; -1 失败;
// 备注：不支持设备文件
// ============================================================================
int fseeko(FILE *stream, off_t offset, int whence)
{

    if(!stream)
        return (EOF);

    if(EOF != stream->unget)
        stream->unget = EOF;

//    if(-1 == IsSTDIO(stream))
//    {
//        return (EOF);
//    }

    if(!__isfilebufed(stream))
    {
        offset = lseek(stream->fd, offset, whence);
        if(-1 == offset)
            return (EOF);

        stream->pos = offset;
        return (0);
    }

    switch(whence)
    {
        case SEEK_END:
        {
            struct stat info;

            if(-1 == fstat(stream->fd, &info))
            {
                debug_printf("clib","\"fseek\" file failed<cannot stat>.");
                return (EOF);
            }

            offset = info.st_size + offset; // 转为SEEK_SET；
        }

        case SEEK_SET:
        {
            if(offset < 0)
            {
                debug_printf("clib","\"fseek\" file failed<out of range>.");
                return (EOF);
            }

            offset = offset - stream->pos; // 转化为SEEK_CUR；
        }

        case SEEK_CUR:
        {
            if(((offset < 0) && ((__filebuf_used(stream) + offset) >= 0)) ||
               ((offset > 0) && (__filebuf_fetched(stream) - offset >= 0)))
            {
                // 偏置量在缓存内部移动；
                stream->buf->movs += offset;
#if 0
                if(stream->buf->movs > stream->buf->prefetched)
                    stream->buf->prefetched = stream->buf->movs;
#endif
                stream->pos += offset;
                return (0);
            }

            if(EOF == fflush(stream))
                return (EOF);

            offset = lseek(stream->fd, offset, whence);
            if(-1 == offset)
                return (EOF);

            stream->pos = offset; // 这里不事先做缓存，等读或写时再做。
            return (0);
        }

        default : return (EOF);
    }

    return (EOF);
}

// ============================================================================
// 功能：移动读写位置，如果c库没有buf，将直接调用系统的 lseek。否则，
// 参数：stream -- 文件流;
//      offset -- 需移动相对偏置;
//      whence -- offset的起点;
// 返回： 成功（当前位置）; 失败（EOF）;
// 备注：off_t是POSIX标准
// ============================================================================
int fseek(FILE *stream, long offset, int whence)
{
    if(!stream)
        return (EOF);

    if(sizeof(off_t) != sizeof(long))
    {
        return ((int)fseeko(stream, (off_t)(offset&(0x00000000FFFFFFFF)), whence));
    }

    return (fseeko(stream, (off_t)offset, whence));
}

// ============================================================================
// 功能：根据文件流获得文件描述符；
// 参数：stream -- 文件流；
// 返回：成功（文件号）；出错（-1）
// 备注：POSIX
// ============================================================================
int fileno(FILE *stream)
{
    if(!stream)
        return (-1);

//    if(-1 != IsSTDIO(stream))
//        return (-1);

    return (stream->fd);
}

// ============================================================================
// 功能：获取文件的读写位置;
// 参数：stream，文件指针
// 返回：读写位置，出错则返回-1
// 备注：
// ============================================================================
off_t ftello(FILE *stream)
{
    if(!stream)
        return (-1);

//    if(-1 != IsSTDIO(stream))
//        return (-1);

    return (stream->pos);
}

// ============================================================================
// 功能：获取文件的读写位置;
// 参数：stream，文件指针
// 返回：读写位置，出错则返回-1
// 备注：
// ============================================================================
long ftell(FILE *stream)
{
    if(!stream)
        return (EOF);

    if(sizeof(off_t) != sizeof(long))
    {
        debug_printf("clib","the size of \"off_t\" is not equal to the size of \"long\".");
        return (EOF); // TODO
    }

    return ((long)ftello(stream));
}

// ============================================================================
// 功能：测试是否达到文件尾；
// 参数：stream -- 文件流；
// 返回：达到文件尾（0）；未到达文件尾部（EOF）；
// 备注：
// ============================================================================
s32 feof(FILE *stream)
{
    struct stat info;
    if(!stream)
        return (EOF);

    if(fstat(stream->fd, &info))
        return (EOF);

    if(stream->pos >= info.st_size)
        return (0);

    return (EOF);
}

// ============================================================================
// 功能：文件重置；
// 参数：stream -- 文件流；
// 返回：达到文件尾（0）；未到达文件尾部（EOF）；
// 备注：文件错误清楚；刷新所有输出阻塞；
// ============================================================================
void rewind(FILE *stream)
{
    fseek(stream, 0L, SEEK_SET);
}

// ============================================================================
// 功能：打开文件
// 参数：
// 返回：非NULL -- 成功; NULL -- 失败;
// 备注：如果 fp 原来是有buf的，但新打开的文件不需要buf，如果fp和buf是一起申请的，
//      无法单独释放buf，故不会释放buf；如果不是一起申请的，则释放之。如果老fp
//      不带buf，而新fp需要，则要单独申请buf，这是导致fp和buf分别申请的原因。c
//      标准规定，freopen不能修改fp指针，故不允许重新malloc fp。
// ============================================================================
FILE *freopen(const char *filename, const char *mode, FILE *stream)
{
    s32 fd, tmp, flags, cflags, res;
    struct stat info;

    if((!filename) || (!mode) || ('\0' == *filename))
        return (NULL);

    res = __transform(mode, &flags, &cflags);
    if(res)
    {
        debug_printf("clib","\"freopen\" (%s) failed<bad \"mode\" = %s>", filename, mode);
        return (NULL);
    }

    fd = open(filename, flags, 0);
    if(fd == -1)
    {
        debug_printf("clib","\"freopen\" (%s) failed<cannot \"open\">", filename);
        return (NULL);
    }

    res = fstat(fd, &info);
    if(res)
    {
        res = close(fd);
        if(res)
        {
            debug_printf("clib","\"freopen\" (%s) failed<cannot \"stat\" and \"close\">.", filename);
        }

        debug_printf("clib"," : g : \"freopen\" (%s) failed<cannot \"stat\">.", filename);
        return (NULL);
    }

    fflush(stream);

    //单进程模式下，不存在内核态用户态间切换，c库不做buf，也不怎么影响效率。
    //流式数据文件不能写缓冲，如果不能读，亦不需要buf。
    if((Djy_GetRunMode( ) < CN_RUNMODE_MP )
            || (!(cflags & FP_IOREAD) && (S_ISFLOW(info.st_mode))))
    {
        if(!stream->buf)
            __filebuf_new(stream);
    }
    else
    {
        if(stream->buf)
            __filebuf_del(stream);
    }

    stream->flags = info.st_mode; // 新的flag

    if(1 == IsSTDIO(stream))
    {
        res = fcntl(stream->fd, F_STDIO_REDRIECT, fd); // 对STDIO进行的重定向（采用fcntl系统调用方式）
        if(res)
            return (NULL);
    }
    else
    {
        tmp = stream->fd;
        stream->fd = -1; // 防止，此时被其他线程使用；甚至是释放；应该是加个锁最好；
        close(tmp);
        stream->fd = fd;
    }

    return (stream);

}
