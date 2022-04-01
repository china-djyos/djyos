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
// ���ܣ������Ƿ�����Ч�ĵ��ļ�����
// ������stream -- �ļ�����
// ���أ��ǣ�1�������ǣ�0����
// ��ע��
// ============================================================================
s32 File_IsValid(FILE* stream)
{
    if((!stream) // �Ƿ�����
       ||(-1==stream->fd)) // δ��ʼ����STDIO�ļ���
        return (0);

    return (1);
}

// ============================================================================
// ���ܣ��½��ļ����壻
// ������stream -- �ļ�����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
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
// ���ܣ�ɾ���ļ����壻
// ������stream -- �ļ�����
// ���أ�
// ��ע��
// ============================================================================
void __File_BufDel(FILE *stream)
{
    if((stream->buf != NULL))
    {
        free(stream->buf);//buf���Ǻ�fpһ������ģ��뵥���ͷ�
                        //fopen��buf��fp��һ������ģ���freopen���ܵ��·ֿ�����
        stream->current = stream->buf = 0;
    }
    stream->bufsize = 0;
}

// ============================================================================
// ���ܣ��ļ������������
// ������stream -- �ļ�����
// ���أ������������
// ��ע��
// ============================================================================
s32 __File_BufFrees(FILE *stream)
{
    return (stream->bufsize - (s32)(stream->current - stream->buf));
}

// ============================================================================
// ���ܣ��ļ�����Ԥȡ����
// ������stream -- �ļ�����
// ���أ�
// ��ע��
// ============================================================================
s32 __File_BufFetched(FILE *stream)
{
    return (stream->count);
}
#if 0
// ============================================================================
// ���ܣ���ȡ�ļ���Ԥ��������
// ������stream -- �ļ�����
// ���أ��ļ���Ԥ��������
// ��ע��
// ============================================================================
s32 __filebuf_mark(FILE *stream)
{
    return (stream->count);
}
#endif
// ============================================================================
// ���ܣ��ļ����ѻ��������
// ������stream -- �ļ�����
// ���أ��ѻ��������
// ��ע��
// ============================================================================
s32 __File_BufUsed(FILE*stream)
{
    return (stream->current - stream->buf);
}

// ============================================================================
// ���ܣ������Ƿ�����ļ����壻
// ������stream -- �ļ�����
// ���أ������ڻ��壨NULL�������ڻ��壨����ռ䣩��
// ��ע��
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
//// ���ܣ��ļ���������
//// ������stream -- �ļ�����
//// ���أ���
//// ��ע��
//// ============================================================================
//void __isfilebufclean(FILE*stream)
//{
//    memset(stream->buf, 0xff, stream->bufsize);
//    stream->current = stream->buf;
//    stream->count = 0;
//}

// ============================================================================
// ���ܣ�ģʽת��
// ������pMode -- C���׼�ļ�����ģʽ��
//      pFlags -- POSIX��׼�ļ�������־��
//      pCFlags -- �Զ����־
// ���أ��ɹ���0���� ʧ�ܣ�-1��;
// ��ע��
// ============================================================================
s32 __File_Transform(const char *pMode, s32 *pFlags, s32 *pCFlags)
{
    bool_t WhileContinue = true;

    //ע��O_RDONLY��O_WRONLY��O_RDWR �����POSIX��2016�������¶���
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
// ���ܣ����ļ�����������д���ļ������豸
// ������stream -- �ļ���;
// ���أ��ɹ���0��������EOF����
// ��ע��
// ============================================================================
s32 fflush(FILE *stream)
{
    s32 res = 0, buffed;
    u8 *src, *des;
    s32 i;
    off_t offset;

    if(!stream)
        return (EOF); // C�����Ǹ������е�stream

    if(0 != stream->wrt_end)    //��������������� buffer
    {
        offset = ftell(stream);
        lseek(stream->fd, stream->FilePos - (s32)(stream->current - stream->buf) - stream->wrt_start, SEEK_SET);
        res = write(stream->fd, stream->buf + stream->wrt_start, stream->wrt_end - stream->wrt_start);
        if(res != stream->wrt_end - stream->wrt_start)
        {
            buffed = (stream->wrt_end - stream->wrt_start) - res; // δˢ��ȥ�Ĳ���
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
// ���ܣ����ļ� ��
// ������filename -- �ļ�����·��;
//      mode -- �ļ�����ģʽ;
// ���أ��ɹ����ļ�����; ʧ�ܣ�NULL��;
// ��ע��
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
    //������ģʽ�£��������ں�̬�û�̬���л���c�ⲻ��buf��Ҳ����ôӰ��Ч�ʡ�
    //��ʽ�����ļ�����д���壬������ܶ����಻��Ҫbuf��
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
// ���ܣ��ر��ļ���
// ������stream -- �ļ�����
// ���أ��ɹ���0��;ʧ�ܣ�-1����
// ��ע��
// ============================================================================
int fclose(FILE *stream)
{
    if(!stream)
        return (-1);

//    if(IsSTDIO(stream))
//        return (-1); // STDIO�ļ�������رգ�

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
// ���ܣ�������ʱ�ļ���
// ������
// ���أ��ɹ�����ʱ�ļ���;ʧ�ܣ�NULL����
// ��ע���ļ���ͨ��tmpname���������߲����ģ�
// ============================================================================
FILE *tmpfile(void)
{
    return (NULL); // ��һ���ض�Ŀ¼/temp�£�������ʱ�ļ���
}

// ============================================================================
// ���ܣ�������ʱ�ļ�����
// �������û�ָ������ʱ�ļ����ռ䣻
// ���أ������ɵ���ʱ�ļ�����
// ��ע��s�����û��ṩ�����ݿռ䣬������ʵ�ʴ�С��������L_tmpnam��
// ============================================================================
char *tmpname(char *s)
{
    s = s;
    return (NULL);
}

// ============================================================================
// ���ܣ���һ���ַ��˻ظ�����������������ָ��ӵ�ǰλ�û���һ���ַ������ô����
//      �ַ� c ���Ǹ��ַ���ֻ�ı仺�������ݣ���ʹfflush��fclose�ļ������˻ص�
//      �ַ�Ҳ����д���ļ���
// ������c -- �����ظ����������ַ���
//      stream -- Ŀ����������
// ���أ��ɹ�ִ�з��ز���c�����򷵻�EOF��
// ============================================================================
s32 ungetc(s32 c, FILE *stream)
{
    if((!stream) || (EOF == c))
        return (EOF);

    if(EOF == stream->ungetbuf)
    {
        stream->ungetbuf = c;                   //ungetc�������ǿյ�
        stream->flags &= ~FP_IOEOF;
        return ((u8)c);
    }

    return (EOF);
}

// ============================================================================
// ���ܣ����ļ���
// ������buf -- ������;
//      size -- ��Ҫ���ĵ�λ��;
//      count -- ��λ��;
//      stream -- �ļ���;
// ���أ���ȡ�ļ�¼����ȡ����������ļ�ʣ�����ݲ��㣬��ȫ����ȡ����������������
//      ��¼��ȡ���ݣ���ʱ���޷�֪��������ȡ�˶������ݡ�
// ��ע��
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
        *(u8*)buf++ = (u8)stream->ungetbuf;        //�ȶ�ȡungetc���������ַ�
        stream->ungetbuf = EOF;
        result += 1;
        read_size--;
        if(read_size == 0)
        {
            return (1/size);
        }

        i = 1;
    }

    if(!File_IsValid(stream)) // �ļ�����δ��ʼ����STDIO
    {
        if(GetCharDirect) // ������ע�᣻��TODO:����߼���Ӧ�÷�ֹ����ʵ�֣�
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
                if(PutStrDirect) // ������ע�᣻
                    PutStrDirect((const char*)&ch, 1);
#endif
            }

            return (i/size);
        }
    }

    if(__File_IsFileBufed(stream) == 0)          //��buf��ע�⣬������ stream->buf == NULL �ж����ο�freopen
    {
        res = read(stream->fd, buf, read_size);
        if(-1 != (s32)res)
            result += res;
        else
            res = 0; // ����
    }
    else
    {
        buffered = stream->count;
        if(read_size <= buffered)         //�ж�Ԥ��ȡ���������Ƿ���ڱ�����Ҫ��ȡ��������
        {
            // ��Ԥ�����ļ��㹻���ζ�
            memcpy(buf, stream->current, read_size);
            stream->count -= read_size;
            stream->current += read_size;
            result += read_size;
        }
        else
        {
            //buffer��û���㹻���ݵĴ�������
            //1����copy����nmemb���ݵ��û�buf
            //2����buf���Ƿ�����Ҫfflush���ļ��е����ݡ�
            res = fflush(stream);       //���ļ�������ȫ��д��flash��
            if(EOF==res)
            {
                debug_printf("clib","\"fread\" file failed(buffer flash).");
                return (0);
            }
            if(buffered > 0)
            {
                memcpy(buf, stream->current, buffered);   //���ļ�����������ݶ�����
                buf += buffered;
                stream->count -= buffered;
                result += buffered;
                read_size -= buffered;               //��Ҫ��������
            }
//            __isfilebufclean(stream);          //�����ļ�����

            round = read_size - read_size % stream->bufsize;      //��Ҫ��ȡ��������bufsize
            reads = 0;
            if(round != 0)
            {
                reads = read(stream->fd, buf, round);   //��ȡ������bufsize
                buf += reads;
                result += reads;
            }
            if(reads < round)       //�ѵ��ļ�β��
            {
                result += reads;
                stream->current = stream->buf;
                stream->count = 0;
            }
            else
            {
                reads = read(stream->fd, stream->buf, stream->bufsize); //��ȡ bufsize
                if(reads > (read_size - round))             //�����˳������������
                {
                    memcpy(buf, stream->buf, (read_size - round));
                    stream->count = reads - (read_size - round);
                    stream->current = stream->buf + (read_size - round);
                    result += read_size-round;
                }
                else                                        //����������������������
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
// ���ܣ�дcount����λ���ļ�����
// ������buf -- ��д��Ŀռ䣻
//      size -- ��λ���ֽ���������λ�ĳߴ磻
//      count -- ��λ������;
//      stream -- �ļ���;
// ���أ� ʵ��д��ĵ�λ������
// ��ע��
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
        stream->ungetbuf = EOF; // ������ungetc������

    if(!File_IsValid(stream)) // TODO
    {
        if(PutStrDirect) // ������ע�᣻
        {
            PutStrDirect((const char*)buf, WriteSize);
        }

        return (count);
    }
    //ע�⣬������ stream->buf == NULL �ж���û��buf���ο�freopen
    if(__File_IsFileBufed(stream) == 0)
    {
        res = write(stream->fd, buf, WriteSize);
        if(-1!=res)
            result += res;
        else
            res = 0; // ����
    }
    else
    {
        if(stream->wrt_start == -1)
            stream->wrt_start = 0;
        BufRest = __File_BufFrees(stream);
        if(WriteSize <= BufRest)
        {
            //��д�������δ���� buf��ֻ���� buffer ��������д���ļ���
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
            //��д�����ݣ����� buf �ռ䣬��Ҫд���ļ�
            memcpy(stream->current, buf, BufRest);
            //��seek��buf��wrt_start��Ӧλ��
            lseek(stream->fd, stream->FilePos - (s32)(stream->current - stream->buf) - stream->wrt_start, SEEK_SET);
            //�ѵ�ǰbuf�б��޸ĵ�����д���ļ���
            res = write(stream->fd, stream->buf + stream->wrt_start,  stream->bufsize - stream->wrt_start);
            result += res;
            WriteSize -= res;
            //����ʣ�������з��� bufsize ���֣���������
            BufRest = WriteSize % stream->bufsize;
            //��������bufsize����д���ļ���ʣ�����bufsize������copy��buf�С�
            res = write(stream->fd, buf+result, WriteSize - BufRest);
            result += res;
            if(BufRest != 0)        //������0
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
// ���ܣ����ļ�����һ���ַ���getc��fgetc�ǵȼ۵ģ���Ц��c��׼��
// ������stream�����Ŀ���ļ��������stdin����Ҫ�ж��Ƿ��豸��
// ���أ���ȡ���ַ��������򷵻�EOF
// ��ע��
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
// ���ܣ����ļ�����һ���ַ�������������ֱ���س��������ո�
// ������buf -- �����ַ����Ļ�������
//      size -- buf�ĳ��ȣ�
//      stream -- ����Դ�ļ���
// ���أ�1���ɹ����򷵻ص�һ������buf��
//      2����������������errorָʾ�������ã�����NULL��buf�����ݿ��ܱ��ı䡣
// ��ע��
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
       // ���Իس����Ի��з��������ܼ���WINDOWS��LINUX��ʽ�ļ���
       // MAC���ÿ��ǣ��� OSx��ʼ�Ѿ�����LINUX�ˡ�
       if('\r' == ch)
       {
           continue; // WINDOWS��ʽ�ı��ļ��� '\r'������
       }
       else if('\n' == ch) // �����з����������
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
// ���ܣ����һ���ַ����ļ���putc��fputc�ǵȼ۵ģ���Ц��c��׼
// ������ch -- ��������ַ�
//      stream -- ���Ŀ���ļ����豸��
// ���أ���ȷ����򷵻ر�������ַ��������򷵻�EOF
// ��ע��
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
// ���ܣ����һ���ַ����ļ���putc��fputc�ǵȼ۵ģ���Ц��c��׼
// ������ch -- ��������ַ�
//      stream -- ���Ŀ���ļ����豸��
// ���أ���ȷ����򷵻ر�������ַ��������򷵻�EOF
// ��ע��
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
// ���ܣ����һ���ַ������ļ���ע�⣬����������������Ҳ������ӻس����з�
// ������s -- ��������ַ�����
//      stream -- ���Ŀ���ļ�����
// ���أ���ȷ�������������ֽ��������󷵻�EOF��
// ��ע��
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
// ���ܣ��ƶ���дλ�ã����c��û��buf����ֱ�ӵ���ϵͳ�� lseek��
// ������stream -- �ļ���;
//      offset -- ���ƶ����ƫ��;
//      whence -- offset�����;
// ���أ� 0 �ɹ�; -1 ʧ��;
// ��ע����֧���豸�ļ�
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
                // ƫ�����ڻ����ڲ��ƶ���
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

            stream->FilePos = offset; // ���ﲻ���������棬�ȶ���дʱ������
            return (0);
        }

        default : return (EOF);
    }

    return (EOF);
}

// ============================================================================
// ���ܣ��ƶ���дλ�ã����c��û��buf����ֱ�ӵ���ϵͳ�� lseek������
// ������stream -- �ļ���;
//      offset -- ���ƶ����ƫ��;
//      whence -- offset�����;SEEK_SET,SEEK_CUR,SEEK_END
// ���أ� �ɹ�����ǰλ�ã�; ʧ�ܣ�EOF��;
// ��ע��off_t��POSIX��׼
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
// ���ܣ������ļ�������ļ���������
// ������stream -- �ļ�����
// ���أ��ɹ����ļ��ţ�������-1��
// ��ע��POSIX
// ============================================================================
int fileno(FILE *stream)
{
    if(stream)
        return stream->fd;
    else
        return -1;
}
// ============================================================================
// ���ܣ���ȡ�ļ��Ķ�дλ��;
// ������stream���ļ�ָ��
// ���أ���дλ�ã������򷵻�-1
// ��ע��
// ============================================================================
off_t ftell(FILE *stream)
{
    if(stream)
        return stream->FilePos;
    else
        return EOF;
}

// ============================================================================
// ���ܣ������Ƿ�ﵽ�ļ�β��
// ������stream -- �ļ�����
// ���أ������ļ�β��1��������������أ�0����
// ��ע��
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
// ���ܣ������ļ�����дλ��Ϊ�ļ���ͷ��
// ������stream -- �ļ�����
// ���أ���
// ��ע���ļ����������ˢ���������������
// ============================================================================
void rewind(FILE *stream)
{
    fseek(stream, 0L, SEEK_SET);
}

// ============================================================================
// ���ܣ������ļ����ƻ�λ��
// ������oldpath -- ԭ·����
//      newpath -- ��·����
// ���أ��ɹ� -- 0��ʧ�� -- -1��
// ��ע��
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
// ���ܣ����ļ�
// ������
// ���أ���NULL -- �ɹ�; NULL -- ʧ��;
// ��ע����� stream ԭ������buf�ģ����´򿪵��ļ�����Ҫbuf�����fp��buf��һ������ģ�
//      �޷������ͷ�buf���ʲ����ͷ�buf���������һ������ģ����ͷ�֮�������fp
//      ����buf������fp��Ҫ����Ҫ��������buf�����ǵ���fp��buf�ֱ������ԭ��c
//      ��׼�涨��freopen�����޸�fpָ�룬�ʲ���������malloc stream��
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
    //������ģʽ�£��������ں�̬�û�̬���л���c�ⲻ��buf��Ҳ����ôӰ��Ч�ʡ�
    //��ʽ�����ļ�����д���壬������ܶ����಻��Ҫbuf��
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

    stream->flags = cflags; // �µ�flag
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
//�ļ�ϵͳ��ص�shellָ��
//=============================================================================
//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע:
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
//����: �ļ�ϵͳ��ʽ��
//����:
//����:
//��ע: todo: �߼�������
//-----------------------------------------------------------------------------
static bool_t __File_FS_Format(char *Param)
{
    extern s32 File_Format(const char *MountPath);

    if(NULL == Param)
        return (FALSE);

    printf("�ļ�ϵͳ��ʽ�� ----");

    if(File_Format(Param))
        printf("��ʽ��ʧ��!");
    else
        printf("��ʽ���ɹ�!");

    printf("\r\n");
    return (TRUE);
}

//-----------------------------------------------------------------------------
//����: ���ĵ�ǰ����·��
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static bool_t __File_ChangeDir(char *Param)
{
    s32 Ret;
    char *Path = (char *)Param;

    if(NULL == Path)
        return (FALSE);

    while(*Path == ' ')
        Path = Path + 1; // ȥ������Ŀո��

    Ret = OBJ_SetPwd(Path);
    if(Ret)
    {
        printf("���󣺵�ǰ����·�����ʧ��");
        if(-1 == Ret)
            printf("-- Ŀ¼�����ڻ��޷�����");
        else if(-2 == Ret)
            printf("-- Ŀ¼�޷�����");
        printf("\r\n");
        return (TRUE);
    }

    return (TRUE);
}
//-----------------------------------------------------------------------------
//����: ����Ŀ¼
//����: -p -- ��ͼ����·�������в����ڵ�Ŀ¼��
//����:
//��ע:
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
        Path = (char*)Temp + 3; // ȥ��"-p"������
    else
    {
        Path = (char*)Param;
        JustLast = 1;
    }

    while(*Path == ' ')
        Path = Path + 1; // ȥ������Ŀո��,�����ļ�ϵͳ·������,�ǲ������Կո�ʼ��

    FullPath = malloc(strlen(Path)+1);
    strcpy(FullPath, Path);
    if(JustLast)
        res = mkdir(FullPath,0);
    else
    {
        // ��ȡ·����ÿ���ڵ�,����/aa/bb/cc��·�����ֱ���ȡ/aa��/aa/bb��/aa//bb/cc����ͼ����
        SlashPos = FullPath;
        do
        {
            while(*SlashPos == '/')
                SlashPos = SlashPos + 1; // ȥ������'/'

            while((*SlashPos != '/') && (*SlashPos != '\0'))
                SlashPos = SlashPos + 1; // ��ȡһ��segment

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
        printf("����ʧ�ܻ�Ŀ¼�Ѵ���\r\n");

    free(FullPath);
    return (TRUE);
}
//-----------------------------------------------------------------------------
//����: ��ʾĿ¼��
//����: -l -- ��ʾĿ¼�ļ���������Ϣ
//����:
//��ע:
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
        Flags = 1; // ��ʾ������Ϣ

    Len = OBJ_CurWorkPathLen();
    CWP = malloc(257);
    if(NULL == CWP)
    {
        printf("�ڴ治��\r\n");
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

                        printf("<�޷�����>");
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
//����:��ʾ��Ȼ�Ĺ���·��
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
bool_t PrintWorkPath(void)
{
    char WokingPath[257];
    OBJ_CurWorkPath(WokingPath, 257);
    printf("�û���ǰ����·����\"%s\"", WokingPath);
    return (TRUE);
}

//-----------------------------------------------------------------------------
//����: ɾ���ļ���Ŀ¼
//����: -r �ݹ�ɾ��(��ɾ��Ŀ¼��)
//����:
//��ע:
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
        PathParam = (char*)Temp + 2; // ȥ��"-p"������
        RemoveAll = 1;
    }

    while(*PathParam == ' ')
        PathParam = PathParam + 1; // ȥ������Ŀո��

    Path = malloc(257);
    if(NULL == Path)
    {
        printf("�ڴ治��\r\n");
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
                printf("��ʽ����\r\n");

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
                            printf("�ļ�\"%s\"�޷�ɾ��(�ļ�ʹ����)\r\n", Dirent->d_name);
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
                printf("�ļ�ɾ��ʧ��\r\n");
        }
    }
    free(Path);
    return (TRUE);
}
//-----------------------------------------------------------------------------
//����: ���ļ�
//����:
//����:
//��ע:
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
        Path = Path + 1; // ȥ������Ŀո��,�����ļ�ϵͳ·������,�ǲ������Կո�ʼ��


    FH = open(Param, O_RDONLY, 0);
    if(-1 == FH)
    {
        printf("\"cat\": �ļ��޷���");
        return (FALSE);
    }

    printf("16������ʾģʽ:\r\n");
    do{
        Ret = read(FH, Buf, 64);
        for(i = 0; i < Ret; i++)
            printf("%02x ", Buf[i]);
    }while(Ret == 64);

    close(FH);

    return (TRUE);
}

//-----------------------------------------------------------------------------
//����: �����ļ�
//����: "cp /fat/abc /yaffs/abc"
//����:
//��ע:
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
        PathTemp = PathTemp + 1; // ȥ������Ŀո��,�����ļ�ϵͳ·������,�ǲ������Կո�ʼ��

    PathDes = PathTemp; // ��ʱ����ͷλ��
    PathSrc = PathTemp;
    while(*PathTemp != ' ')
        PathTemp = PathTemp + 1; // �ҵ��ո�

    Length = PathTemp - PathSrc;
    PathSrc = malloc(Length + 1); // ��������
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
            break; // ȫ������

        if(Res != write(HandleDes, Buf, Res))
        {
            printf("\"cp\": write destination file error.\r\n");
            break;
        }

        if(Res != RW_SIZE)
            break; // ȫ������
    }

    Res = close(HandleDes);
    if(Res)
        printf("\"cp\": close destination file error.\r\n");

    Res = close(HandleSrc);
    if(Res)
        printf("\"cp\": close source file error.\r\n");

    return (TRUE);
}

ADD_TO_ROUTINE_SHELL(format,__File_FS_Format,"��ʽ���ļ�ϵͳ��COMMAND:format+�ļ�ϵͳmount����+enter.");
ADD_TO_ROUTINE_SHELL(cd,__File_ChangeDir,"���ĵ�ǰ����·����COMMAND:cd+�¹���·��+enter.");
ADD_TO_ROUTINE_SHELL(pwd,PrintWorkPath,"��ʾ�û���ǰ����·����COMMAND:pwd+enter.");
ADD_TO_ROUTINE_SHELL(mkdir,__File_Mk_Dir,"����Ŀ¼��COMMAND:mkdir+enter.");
ADD_TO_ROUTINE_SHELL(ls,__File_List,"��ʾĿ¼�COMMAND:format+(-l ��ʾ�ļ�������Ϣ)+enter.");
ADD_TO_ROUTINE_SHELL(rm,File_Remove,"ɾ���ļ�����Ŀ¼��COMMAND:rm + (-r �ݹ�ɾ��)·�� + enter.");
ADD_TO_ROUTINE_SHELL(cat,File_ReadFile,"���ļ�����ӡ��COMMAND:cat + �ļ�·�� + enter.");
ADD_TO_ROUTINE_SHELL(cp,File_CopyFile,"�����ļ���COMMAND:cp + /fat/abc + /yaffs/abc + enter.");
