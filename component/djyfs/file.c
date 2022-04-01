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
#include <djyfs/file.h>
#include "dbug.h"
#include "component_config_fs.h"
#include <shell.h>

// ============================================================================
// 功能：测试是否是有效的的文件流；
// 参数：stream -- 文件流；
// 返回：是（1）；不是（0）；
// 备注：
// ============================================================================
s32 File_IsValid(FILE* stream)
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
s32 __File_BufNew(FILE *stream)
{
    u8 *buf;
    if(stream->buf)
    {
        return -1;
    }
    buf = malloc(CFG_CLIB_BUFFERSIZE);
    if(!buf)
    {
        return -1;
    }
    memset(buf, 0xff, CFG_CLIB_BUFFERSIZE);
    stream->current = stream->buf = buf;
    stream->bufsize = CFG_CLIB_BUFFERSIZE;
    return (0);
}

// ============================================================================
// 功能：删除文件缓冲；
// 参数：stream -- 文件流；
// 返回：
// 备注：
// ============================================================================
void __File_BufDel(FILE *stream)
{
    if((stream->buf != NULL))
    {
        free(stream->buf);//buf不是和fp一起申请的，须单独释放
                        //fopen中buf和fp是一起申请的，但freopen可能导致分开申请
        stream->current = stream->buf = 0;
    }
    stream->bufsize = 0;
}

// ============================================================================
// 功能：文件缓冲空闲量；
// 参数：stream -- 文件流；
// 返回：缓冲空闲量；
// 备注：
// ============================================================================
s32 __File_BufFrees(FILE *stream)
{
    return (stream->bufsize - (s32)(stream->current - stream->buf));
}

// ============================================================================
// 功能：文件缓冲预取量；
// 参数：stream -- 文件流；
// 返回：
// 备注：
// ============================================================================
s32 __File_BufFetched(FILE *stream)
{
    return (stream->count);
}
#if 0
// ============================================================================
// 功能：获取文件流预读的量；
// 参数：stream -- 文件流；
// 返回：文件流预读的量；
// 备注：
// ============================================================================
s32 __filebuf_mark(FILE *stream)
{
    return (stream->count);
}
#endif
// ============================================================================
// 功能：文件流已缓存的量；
// 参数：stream -- 文件流；
// 返回：已缓存的量；
// 备注：
// ============================================================================
s32 __File_BufUsed(FILE*stream)
{
    return (stream->current - stream->buf);
}

// ============================================================================
// 功能：测试是否存在文件缓冲；
// 参数：stream -- 文件流；
// 返回：不存在缓冲（NULL）；存在缓冲（缓冲空间）；
// 备注：
// ============================================================================
s32 __File_IsFileBufed(FILE*stream)
{
    if((s32)stream->bufsize != CFG_CLIB_BUFFERSIZE)
    {
        return 0;
    }
    return -1;
}

//// ============================================================================
//// 功能：文件缓存清理；
//// 参数：stream -- 文件流；
//// 返回：无
//// 备注：
//// ============================================================================
//void __isfilebufclean(FILE*stream)
//{
//    memset(stream->buf, 0xff, stream->bufsize);
//    stream->current = stream->buf;
//    stream->count = 0;
//}

// ============================================================================
// 功能：模式转换
// 参数：pMode -- C库标准文件操作模式；
//      pFlags -- POSIX标准文件操作标志；
//      pCFlags -- 自定义标志
// 返回：成功（0）； 失败（-1）;
// 备注：
// ============================================================================
s32 __File_Transform(const char *pMode, s32 *pFlags, s32 *pCFlags)
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
                if ((*pFlags & O_RDWR) == O_RDWR)
//                    if ((*pFlags & O_RDWR))
                    WhileContinue = false;
                else
                {
                    *pFlags |= O_RDWR;
//                    *pFlags &= ~(O_RDONLY | O_WRONLY);
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
    s32 res = 0, buffed;
    u8 *src, *des;
    s32 i;
    off_t offset;

    if(!stream)
        return (EOF); // C库上是更新所有的stream

    if(0 != stream->wrt_end)    //条件成立，则必有 buffer
    {
        offset = ftell(stream);
        lseek(stream->fd, stream->FilePos - (s32)(stream->current - stream->buf) - stream->wrt_start, SEEK_SET);
        res = write(stream->fd, stream->buf + stream->wrt_start, stream->wrt_end - stream->wrt_start);
        if(res != stream->wrt_end - stream->wrt_start)
        {
            buffed = (stream->wrt_end - stream->wrt_start) - res; // 未刷下去的部分
            src = stream->buf + res;
            des = stream->buf;
            for(i = 0; i < buffed; i++)
            {
                *des = *src;
                des++;
                src++;
            }
            stream->current = stream->buf + buffed;
            stream->count = buffed;
            return (EOF);
        }
        res = fsync(stream->fd);
        lseek(stream->fd, offset, SEEK_SET);
    }
    stream->current = stream->buf;
    stream->count = 0;
    stream->ungetbuf = EOF;
    stream->wrt_start = -1;
    stream->wrt_end = 0;
    return res;
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
    s32 flags = 0, cflags = 0;
    s32 fd, res;
    struct stat info;

    if((!filename) || (!mode) || ('\0' == *filename))
        return (NULL);

    res = __File_Transform(mode, &flags, &cflags);
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
    memset(stream, 0, sizeof(FILE) );
    //单进程模式下，不存在内核态用户态间切换，c库不做buf，也不怎么影响效率。
    //流式数据文件不能写缓冲，如果不能读，亦不需要buf。
    if((DJY_GetRunMode( ) < CN_RUNMODE_MP ) || (!(cflags & FP_IOREAD) && (S_ISFLOW(info.st_mode))))
    {

    }
    else
    {
        res = __File_BufNew(stream);
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
    stream->ungetbuf = EOF;
    stream->wrt_start = -1;
    if(cflags & FP_IOAPPEND)
    {
        stream->FilePos = info.st_size;
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
    if(!stream)
        return (-1);

//    if(IsSTDIO(stream))
//        return (-1); // STDIO文件不允许关闭；

    if(EOF==fflush(stream))
    {
        debug_printf("clib","\"fclose\" file failed<cannot flush>.");
        return (-1);
    }

    if(close(stream->fd))
    {
        debug_printf("clib","\"fclose\" file failed<cannot close>.");
        return (-1);
    }

    if(stream->tmpfname)
    {
        remove(stream->tmpfname);
    }
    __File_BufDel(stream);

    free(stream);

    return 0;
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

    if(EOF == stream->ungetbuf)
    {
        stream->ungetbuf = c;                   //ungetc缓冲区是空的
        stream->flags &= ~FP_IOEOF;
        return ((u8)c);
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
    s32 buffered = 0, i = 0, result = 0;
    s32 res, read_size, reads, round;
    char ch;

    read_size = size * count;
    if((NULL == buf) || (NULL == stream) || (0 == read_size))
        return (0);

    if((!read_size) || ((stream->flags & FP_IOREAD) != FP_IOREAD))
//    if((!read_size) || (stream->flags & FP_IOREAD))
        return (0);

    if(EOF != stream->ungetbuf)
    {
        *(u8*)buf++ = (u8)stream->ungetbuf;        //先读取ungetc缓冲区中字符
        stream->ungetbuf = EOF;
        result += 1;
        read_size--;
        if(read_size == 0)
        {
            return (1/size);
        }

        i = 1;
    }

    if(!File_IsValid(stream)) // 文件流是未初始化的STDIO
    {
        if(GetCharDirect) // 函数已注册；（TODO:这个逻辑不应该防止这里实现）
        {
            for(; i < (s32)(size * count); i++)
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

    if(__File_IsFileBufed(stream) == 0)          //无buf，注意，不能用 stream->buf == NULL 判定，参考freopen
    {
        res = read(stream->fd, buf, read_size);
        if(-1 != (s32)res)
            result += res;
        else
            res = 0; // 出错；
    }
    else
    {
        buffered = stream->count;
        if(read_size <= buffered)         //判断预读取的数据量是否大于本次需要读取的数据量
        {
            // 已预读的文件足够本次读
            memcpy(buf, stream->current, read_size);
            stream->count -= read_size;
            stream->current += read_size;
            result += read_size;
        }
        else
        {
            //buffer中没有足够数据的处理方案：
            //1、先copy整数nmemb数据到用户buf
            //2、看buf中是否有需要fflush到文件中的数据。
            res = fflush(stream);       //把文件缓存先全都写到flash中
            if(EOF==res)
            {
                debug_printf("clib","\"fread\" file failed(buffer flash).");
                return (0);
            }
            if(buffered > 0)
            {
                memcpy(buf, stream->current, buffered);   //把文件缓存里的数据读出来
                buf += buffered;
                stream->count -= buffered;
                result += buffered;
                read_size -= buffered;               //还要读数据量
            }
//            __isfilebufclean(stream);          //清理文件缓存

            round = read_size - read_size % stream->bufsize;      //需要读取的整数个bufsize
            reads = 0;
            if(round != 0)
            {
                reads = read(stream->fd, buf, round);   //读取整数个bufsize
                buf += reads;
                result += reads;
            }
            if(reads < round)       //已到文件尾部
            {
                result += reads;
                stream->current = stream->buf;
                stream->count = 0;
            }
            else
            {
                reads = read(stream->fd, stream->buf, stream->bufsize); //读取 bufsize
                if(reads > (read_size - round))             //读出了超过需求的数据
                {
                    memcpy(buf, stream->buf, (read_size - round));
                    stream->count = reads - (read_size - round);
                    stream->current = stream->buf + (read_size - round);
                    result += read_size-round;
                }
                else                                        //读出的数据少于请求数据
                {
                    memcpy(buf, stream->buf, reads);
                    stream->current = stream->buf;
                    stream->count = 0;
                    result += reads;
                }
            }
        }
    }
    stream->FilePos += result;
    stream->ungetbuf = EOF;
    if(result < (s32)(size * count))
    {
        stream->flags |= FP_IOEOF;
    }
    return (result/size);
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
//    u32 all, frees, rest;
    s32 res;
//    off_t tmp;
    s32 WriteSize,BufRest;
    s32 result = 0;

    WriteSize = size * count;
    if((NULL == buf) || (NULL == stream) || (0 == WriteSize))
        return 0;
    if((!WriteSize) || ((stream->flags & FP_IOWRITE) != FP_IOWRITE))
//    if((!WriteSize) || (stream->flags & FP_IOREAD))
        return (0);

    if(EOF != stream->ungetbuf)
        stream->ungetbuf = EOF; // 抛弃掉ungetc的内容

    if(!File_IsValid(stream)) // TODO
    {
        if(PutStrDirect) // 函数已注册；
        {
            PutStrDirect((const char*)buf, WriteSize);
        }

        return (count);
    }
    //注意，不能用 stream->buf == NULL 判定有没有buf，参考freopen
    if(__File_IsFileBufed(stream) == 0)
    {
        res = write(stream->fd, buf, WriteSize);
        if(-1!=res)
            result += res;
        else
            res = 0; // 出错；
    }
    else
    {
        if(stream->wrt_start == -1)
            stream->wrt_start = 0;
        BufRest = __File_BufFrees(stream);
        if(WriteSize <= BufRest)
        {
            //欲写入的数据未超出 buf，只更新 buffer 参数，不写入文件。
            memcpy(stream->current, buf, WriteSize);
            stream->count -= WriteSize;
            if(stream->count < 0)
                stream->count = 0;
            if(stream->wrt_start > (s32)(stream->current - stream->buf))
                stream->wrt_start = (s32)(stream->current - stream->buf);

            stream->current += WriteSize;
            if(stream->wrt_end < (s32)(stream->current - stream->buf))
                stream->wrt_end = (s32)(stream->current - stream->buf);
            result = WriteSize;
        }
        else
        {
            //待写入数据，超出 buf 空间，需要写入文件
            memcpy(stream->current, buf, BufRest);
            //先seek到buf中wrt_start对应位置
            lseek(stream->fd, stream->FilePos - (s32)(stream->current - stream->buf) - stream->wrt_start, SEEK_SET);
            //把当前buf中被修改的数据写入文件。
            res = write(stream->fd, stream->buf + stream->wrt_start,  stream->bufsize - stream->wrt_start);
            result += res;
            WriteSize -= res;
            //计算剩余数据中非整 bufsize 部分（即余数）
            BufRest = WriteSize % stream->bufsize;
            //把整数个bufsize数据写入文件，剩余非整bufsize部分则copy到buf中。
            res = write(stream->fd, buf+result, WriteSize - BufRest);
            result += res;
            if(BufRest != 0)        //余数非0
            {
                memcpy(stream->buf, buf+result, BufRest);
                result += BufRest;
                stream->wrt_start = 0;
                stream-> wrt_end = BufRest;
                stream->current = stream->buf + BufRest;
            }
            else
            {
                stream->wrt_start = -1;
                stream->wrt_end = 0;
                stream->current = stream->buf;
            }
        }
    }
    if(result < 0)
        return (0);
    stream->FilePos +=result;
    return (result/size);
}

// ============================================================================
// 功能：从文件输入一个字符，getc和fgetc是等价的，搞笑的c标准。
// 参数：stream，输出目标文件，如果是stdin，则要判断是否设备。
// 返回：读取的字符，错误则返回EOF
// 备注：
// ============================================================================
s32 getc(FILE *stream)     //getc = fgetc
{
    s32 ch;

    if(!stream)
        return (EOF);

    if(stream->ungetbuf != EOF)
    {
        ch = stream->ungetbuf;
    }
    else
    {
        if(1 != fread(&ch, 1, 1, stream))
            ch = EOF;
    }
    stream->ungetbuf = EOF;
    return ((s8)ch);
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
char *fgets(char *buf, s32 size, FILE *stream)
{
    s32 i = 0;
    char ch;

    if(!stream || !size || !buf)
        return (NULL);

    if(1 == size)
    {
        buf[0] = '\0';
        return (buf);
    }

    for(i = 0; i < size; i++)
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
       else
       {
           buf[i] = ch;
       }
   }
    buf[size-1] = '\0';
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

    return ((u8)ch);
}
// ============================================================================
// 功能：输出一个字符到文件，putc和fputc是等价的，搞笑的c标准
// 参数：ch -- 待输出的字符
//      stream -- 输出目标文件或设备。
// 返回：正确输出则返回被输出的字符，错误则返回EOF
// 备注：
// ============================================================================
s32 fputc(s32 ch, FILE *stream)     //putc = fputc
{
    if(!stream)
        return (EOF);

    if(1 !=  fwrite(&ch, 1, 1, stream))
        return (EOF);

    return ((u8)ch);
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

    if(EOF != stream->ungetbuf)
        stream->ungetbuf = EOF;

//    if(-1 == IsSTDIO(stream))
//    {
//        return (EOF);
//    }

    if(!__File_IsFileBufed(stream))
    {
        offset = lseek(stream->fd, offset, whence);
        if(-1 == offset)
            return (EOF);

        stream->FilePos = offset;
        return (0);
    }

    switch(whence)
    {
        case SEEK_SET:
        case SEEK_END:
        {
            if(fflush(stream) == EOF)
            {
                return (EOF);
            }
        }
        __attribute__((fallthrough));
        case SEEK_CUR:
        {
            if(((offset < 0) && ((__File_BufUsed(stream) + offset) >= 0)) ||
               ((offset > 0) && (__File_BufFetched(stream) - offset >= 0)))
            {
                // 偏置量在缓存内部移动；
                stream->current += offset;
#if 0
                if(stream->buf->movs > stream->buf->prefetched)
                    stream->buf->prefetched = stream->buf->movs;
#endif
                stream->FilePos += offset;
                stream->count -= offset;

                return (0);
            }

            if(EOF == fflush(stream))
                return (EOF);

            offset = lseek(stream->fd, offset, whence);
            if(-1 == offset)
                return (EOF);

            stream->FilePos = offset; // 这里不事先做缓存，等读或写时再做。
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
//      whence -- offset的起点;SEEK_SET,SEEK_CUR,SEEK_END
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
    if(stream)
        return stream->fd;
    else
        return -1;
}
// ============================================================================
// 功能：获取文件的读写位置;
// 参数：stream，文件指针
// 返回：读写位置，出错则返回-1
// 备注：
// ============================================================================
off_t ftell(FILE *stream)
{
    if(stream)
        return stream->FilePos;
    else
        return EOF;
}

// ============================================================================
// 功能：测试是否达到文件尾；
// 参数：stream -- 文件流；
// 返回：到达文件尾（1）；其它情况返回（0）；
// 备注：
// ============================================================================
s32 feof(FILE *stream)
{
    struct stat info;
    if(!stream)
        return (0);

    if(fstat(stream->fd, &info))
        return (0);

    if(stream->FilePos >= info.st_size)
        return (1);

    return (0);
}

// ============================================================================
// 功能：重设文件流读写位置为文件开头；
// 参数：stream -- 文件流；
// 返回：无
// 备注：文件错误清楚；刷新所有输出阻塞；
// ============================================================================
void rewind(FILE *stream)
{
    fseek(stream, 0L, SEEK_SET);
}

// ============================================================================
// 功能：更改文件名称或位置
// 参数：oldpath -- 原路径；
//      newpath -- 新路径；
// 返回：成功 -- 0，失败 -- -1。
// 备注：
// ============================================================================
int rename(const char *oldpath, const char *newpath)
{
    struct Object *ob;
    s32 result;
    char *uncached;
    ob = OBJ_MatchPath(oldpath, &uncached);
    ob->ObjOps((void *)ob, CN_OBJ_RENAME,
                                (ptu32_t)&result,(ptu32_t)oldpath,(ptu32_t)newpath);

    return result;
}

// ============================================================================
// 功能：打开文件
// 参数：
// 返回：非NULL -- 成功; NULL -- 失败;
// 备注：如果 stream 原来是有buf的，但新打开的文件不需要buf，如果fp和buf是一起申请的，
//      无法单独释放buf，故不会释放buf；如果不是一起申请的，则释放之。如果老fp
//      不带buf，而新fp需要，则要单独申请buf，这是导致fp和buf分别申请的原因。c
//      标准规定，freopen不能修改fp指针，故不允许重新malloc stream。
// ============================================================================
FILE *freopen(const char *filename, const char *mode, FILE *stream)
{
    s32 fd, flags, cflags, res;
    u8 *buf;
    struct stat info;

    if((!filename) || (!mode) || ('\0' == *filename))
        return (NULL);

    res = __File_Transform(mode, &flags, &cflags);
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
    close(stream->fd);
    //单进程模式下，不存在内核态用户态间切换，c库不做buf，也不怎么影响效率。
    //流式数据文件不能写缓冲，如果不能读，亦不需要buf。
    if((DJY_GetRunMode( ) < CN_RUNMODE_MP ) || (!(cflags & FP_IOREAD) && (S_ISFLOW(info.st_mode))))
    {

        __File_BufDel(stream);
        memset(stream, 0, sizeof(FILE));
    }
    else
    {
        buf = stream->buf;
        if(stream->buf == NULL)
            buf = malloc(CFG_CLIB_BUFFERSIZE);
        if(stream->buf == NULL)
        {
            memset(stream, 0, sizeof(FILE));
        }
        else
        {
            memset(stream, 0, sizeof(FILE));
            stream->current = stream->buf = buf;
            stream->bufsize = CFG_CLIB_BUFFERSIZE;
        }
    }

    stream->flags = cflags; // 新的flag
    stream->ungetbuf = EOF;
    stream->wrt_start = -1;
    if(info.st_mode & FP_IOAPPEND)
    {
        stream->FilePos = info.st_size;
    }
    stream->fd = fd;

    return (stream);

}



//=============================================================================
//文件系统相关的shell指令
//=============================================================================
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
const char *s_cREG = "file";
const char *s_cLINK = "link";
const char *s_cDIR = "directory";
const char *s_cUNKNOW = "unknown";
static char *__File_PropertyToStr(u32 Mode)
{
    char *res;

    if(((S_ISREG(Mode)) && (S_ISLNK(Mode))) ||
        ((S_ISREG(Mode)) && (S_ISDIR(Mode))) ||
        ((S_ISDIR(Mode)) && (S_ISLNK(Mode))))
        res = (char*)s_cUNKNOW;

    if(S_ISREG(Mode))
        res = (char*)s_cREG;

    if(S_ISLNK(Mode))
        res = (char*)s_cLINK;

    if(S_ISDIR(Mode))
        res = (char*)s_cDIR;

    return (res);
}

//-----------------------------------------------------------------------------
//功能: 文件系统格式化
//参数:
//返回:
//备注: todo: 逻辑待完善
//-----------------------------------------------------------------------------
static bool_t __File_FS_Format(char *Param)
{
    extern s32 File_Format(const char *MountPath);

    if(NULL == Param)
        return (FALSE);

    printf("文件系统格式化 ----");

    if(File_Format(Param))
        printf("格式化失败!");
    else
        printf("格式化成功!");

    printf("\r\n");
    return (TRUE);
}

//-----------------------------------------------------------------------------
//功能: 更改当前工作路径
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
static bool_t __File_ChangeDir(char *Param)
{
    s32 Ret;
    char *Path = (char *)Param;

    if(NULL == Path)
        return (FALSE);

    while(*Path == ' ')
        Path = Path + 1; // 去除多余的空格符

    Ret = OBJ_SetPwd(Path);
    if(Ret)
    {
        printf("错误：当前工作路径变更失败");
        if(-1 == Ret)
            printf("-- 目录不存在或无法进入");
        else if(-2 == Ret)
            printf("-- 目录无法进入");
        printf("\r\n");
        return (TRUE);
    }

    return (TRUE);
}
//-----------------------------------------------------------------------------
//功能: 创建目录
//参数: -p -- 试图创建路径中所有不存在的目录项
//返回:
//备注:
//-----------------------------------------------------------------------------
static bool_t __File_Mk_Dir(char *Param)
{
    char *Temp, *SlashPos, *Path, *FullPath;
    int res;
    u32 PathLen;
    u8 Changed;
    u8 JustLast = 0;

    if(NULL == Param)
        return (FALSE);

    Temp = strstr(Param, "-p ");
    if(Temp)
        Path = (char*)Temp + 3; // 去除"-p"参数项
    else
    {
        Path = (char*)Param;
        JustLast = 1;
    }

    while(*Path == ' ')
        Path = Path + 1; // 去除多余的空格符,对于文件系统路径而言,是不可能以空格开始的

    FullPath = malloc(strlen(Path)+1);
    strcpy(FullPath, Path);
    if(JustLast)
        res = mkdir(FullPath,0);
    else
    {
        // 获取路径的每个节点,例如/aa/bb/cc的路径，分别会获取/aa、/aa/bb、/aa//bb/cc并试图创建
        SlashPos = FullPath;
        do
        {
            while(*SlashPos == '/')
                SlashPos = SlashPos + 1; // 去除多余'/'

            while((*SlashPos != '/') && (*SlashPos != '\0'))
                SlashPos = SlashPos + 1; // 获取一个segment

            PathLen = SlashPos - FullPath;
            if('/' == FullPath[PathLen])
            {
                FullPath[PathLen] = '\0';
                Changed = 1;
            }

            res = mkdir(FullPath,0);

            if(Changed)
                FullPath[PathLen] = '/';
        }while(*SlashPos != '\0');
    }

    if(-1 == res)
        printf("创建失败或目录已存在\r\n");

    free(FullPath);
    return (TRUE);
}
//-----------------------------------------------------------------------------
//功能: 显示目录项
//参数: -l -- 显示目录文件的所有信息
//返回:
//备注:
//-----------------------------------------------------------------------------
static bool_t __File_List(char *Param)
{
    DIR *Dir;
    s32 Len;
    char *CWP;
    struct dirent *DirentInfo;
    struct stat DirentStat;
    u8 Flags = 0;
    char size[11], *temp;
    u8 len, i;

    if(Param && strstr(Param, "-l"))
        Flags = 1; // 显示完整信息

    Len = OBJ_CurWorkPathLen();
    CWP = malloc(257);
    if(NULL == CWP)
    {
        printf("内存不足\r\n");
        return (TRUE);
    }
    memset(CWP, 0, 257);
    if(0 == OBJ_CurWorkPath(CWP, Len))
    {
        Dir = opendir(CWP);
        if(Dir)
        {
            while((DirentInfo = readdir(Dir)) != NULL)
            {
                if(1 == Flags)
                {
                    CWP[Len - 1] = '/';
                    strcpy(CWP+Len, DirentInfo->d_name);
                    if(stat(CWP, &DirentStat))
                    {
                        for(i = 0; i < 24; i++)
                            printf(" ");

                        printf("<无法解析>");
                        for(i = 0; i < 2; i++)
                            printf(" ");

                        printf("%s \r\n", DirentInfo->d_name);
                    }
                    else
                    {
                        temp = __File_PropertyToStr(DirentStat.st_mode);
                        printf("%s", temp);
                        len = strlen(temp);
                        for(i = len; i < 10; i++)
                            printf(" ");
                        len = 0;
                        if(temp != s_cDIR)
                        {
                            itoa(DirentStat.st_size, size, 10);
                            len = strlen(size);
                            printf("%s", size);
                        }
                        for(i = len; i < 16; i++)
                            printf(" ");

                        for(i = 0; i < 10; i++)
                            printf(" ");

                        printf("%s", DirentInfo->d_name);
                        printf("\r\n");
                    }
                }
                else
                {
                    printf("%s", DirentInfo->d_name);
                    printf("    ");
                }
            }

            if(0 == Flags)
                printf("\r\n");

            closedir(Dir);
        }
    }

    free(CWP);
    return (TRUE);
}
//-----------------------------------------------------------------------------
//功能:显示当然的工作路径
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
bool_t PrintWorkPath(void)
{
    char WokingPath[257];
    OBJ_CurWorkPath(WokingPath, 257);
    printf("用户当前工作路径：\"%s\"", WokingPath);
    return (TRUE);
}

//-----------------------------------------------------------------------------
//功能: 删除文件或目录
//参数: -r 递归删除(可删除目录项)
//返回:
//备注:
//-----------------------------------------------------------------------------
static bool_t File_Remove(char *Param)
{

    u16 Offset;
    char *PathParam, *Temp, *Path;
    s32 Ret = 0;
    u8 RemoveAll = 0;

    PathParam = (char*)Param;
    if(NULL == PathParam)
        return (FALSE);

    Temp = strstr(PathParam, "-r");
    if(Temp)
    {
        PathParam = (char*)Temp + 2; // 去除"-p"参数项
        RemoveAll = 1;
    }

    while(*PathParam == ' ')
        PathParam = PathParam + 1; // 去除多余的空格符

    Path = malloc(257);
    if(NULL == Path)
    {
        printf("内存不足\r\n");
        return (TRUE);
    }

    memset(Path, 0, 257);
    Offset = strlen(PathParam);
    if(Offset < 257)
    {
        if(RemoveAll)
        {
            struct stat Info;
            DIR *Dir;
            struct dirent *Dirent;

            Ret = stat(PathParam, &Info);
            if(Ret)
                printf("格式错误\r\n");

            if((!Ret) && S_ISDIR(Info.st_mode))
            {
                Dir = opendir(PathParam);
                if(Dir)
                {
                    memcpy(Path, PathParam, Offset);
                    if((RemoveAll) && ('/' !=Path[Offset-1]))
                        Path[Offset++] = '/';
                    Path[Offset] = '\0';

                    while((Dirent = readdir(Dir)) != NULL)
                    {
                        strcpy(Path+Offset, Dirent->d_name);
                        Ret = remove(Path);
                        if(Ret)
                        {
                            printf("文件\"%s\"无法删除(文件使用中)\r\n", Dirent->d_name);
                            break;
                        }
                    }
                    closedir(Dir);
                }
            }
        }

        if(!Ret)
        {
            Ret = remove(PathParam);
            if(Ret)
                printf("文件删除失败\r\n");
        }
    }
    free(Path);
    return (TRUE);
}
//-----------------------------------------------------------------------------
//功能: 读文件
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
static bool_t File_ReadFile(char *Param)
{
    char *Path = (char *)Param;
    int FH;
    u8 Buf[64];
    s32 Ret;
    u8 i;

    if(NULL == Path)
        return (FALSE);

    while(*Path == ' ')
        Path = Path + 1; // 去除多余的空格符,对于文件系统路径而言,是不可能以空格开始的


    FH = open(Param, O_RDONLY, 0);
    if(-1 == FH)
    {
        printf("\"cat\": 文件无法打开");
        return (FALSE);
    }

    printf("16进制显示模式:\r\n");
    do{
        Ret = read(FH, Buf, 64);
        for(i = 0; i < Ret; i++)
            printf("%02x ", Buf[i]);
    }while(Ret == 64);

    close(FH);

    return (TRUE);
}

//-----------------------------------------------------------------------------
//功能: 拷贝文件
//参数: "cp /fat/abc /yaffs/abc"
//返回:
//备注:
//-----------------------------------------------------------------------------
#define RW_SIZE  256
static bool_t File_CopyFile(char *Param)
{

    char *PathTemp;
    char *PathSrc;
    char *PathDes;
    u16 Length;
    s32 HandleSrc;
    s32 HandleDes;
    u8 Buf[RW_SIZE];
    s32 Res;

    PathTemp = (char*)Param;
    if(NULL == PathTemp)
    {
        printf("\"cp\": no parameters!\r\n");
        return (FALSE);
    }

    while(*PathTemp == ' ')
        PathTemp = PathTemp + 1; // 去除多余的空格符,对于文件系统路径而言,是不可能以空格开始的

    PathDes = PathTemp; // 临时保存头位置
    PathSrc = PathTemp;
    while(*PathTemp != ' ')
        PathTemp = PathTemp + 1; // 找到空格

    Length = PathTemp - PathSrc;
    PathSrc = malloc(Length + 1); // 含结束符
    if(!PathSrc)
    {
        printf("\"cp\": memory out!\r\n");
        return (FALSE);
    }

    memcpy(PathSrc, (PathTemp - Length), Length);
    PathSrc[Length] = '\0';
    PathDes = PathTemp + 1;
    HandleSrc = open(PathSrc, O_RDONLY);
    if(-1 == HandleSrc)
    {
        printf("\"cp\": cannot open source file!\r\n");
        return (FALSE);
    }

    HandleDes = open(PathDes, O_RDWR | O_CREAT);
    if(-1 == HandleDes)
    {
        printf("\"cp\": cannot open destination file!\r\n");
        return (FALSE);
    }

    while(1)
    {
        Res = read(HandleSrc, Buf, RW_SIZE);
        if(!Res)
            break; // 全部读完

        if(Res != write(HandleDes, Buf, Res))
        {
            printf("\"cp\": write destination file error.\r\n");
            break;
        }

        if(Res != RW_SIZE)
            break; // 全部读完
    }

    Res = close(HandleDes);
    if(Res)
        printf("\"cp\": close destination file error.\r\n");

    Res = close(HandleSrc);
    if(Res)
        printf("\"cp\": close source file error.\r\n");

    return (TRUE);
}

ADD_TO_ROUTINE_SHELL(format,__File_FS_Format,"格式化文件系统，COMMAND:format+文件系统mount点名+enter.");
ADD_TO_ROUTINE_SHELL(cd,__File_ChangeDir,"更改当前工作路径，COMMAND:cd+新工作路径+enter.");
ADD_TO_ROUTINE_SHELL(pwd,PrintWorkPath,"显示用户当前工作路径，COMMAND:pwd+enter.");
ADD_TO_ROUTINE_SHELL(mkdir,__File_Mk_Dir,"创建目录，COMMAND:mkdir+enter.");
ADD_TO_ROUTINE_SHELL(ls,__File_List,"显示目录项，COMMAND:format+(-l 显示文件所有信息)+enter.");
ADD_TO_ROUTINE_SHELL(rm,File_Remove,"删除文件或者目录，COMMAND:rm + (-r 递归删除)路径 + enter.");
ADD_TO_ROUTINE_SHELL(cat,File_ReadFile,"读文件并打印，COMMAND:cat + 文件路径 + enter.");
ADD_TO_ROUTINE_SHELL(cp,File_CopyFile,"拷贝文件，COMMAND:cp + /fat/abc + /yaffs/abc + enter.");
