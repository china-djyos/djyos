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

#include "stdint.h"
#include "stddef.h"
#include "stdio.h"
#include "stdarg.h"
#include "string.h"
#include "ctype.h"
#include "object.h"
#include <device.h>
#include <fs/file.h>
#include "systime.h"
#include "ring.h"
#include "component_config_stdio.h"

#define SCANF_FROM_STRING     0   //从string中读取字符
#define SCANF_FROM_FILE       1   //从文件中输入，用于scanf（stdin是文件），或者fscanf

s32 skip_atoi(const char **s);
#define CN_SCANF_BUF_LEN        32
u32 __sc_strtoul(ptu32_t Source,char (*__GetChar)(ptu32_t Source,s32 *Offset),s32 *Offset,s32 base,char *LastCh)
{
    u32 result = 0,value;
    char ch = *LastCh;
    switch (base)
    {
        case 8:
            while ((ch <= '7') && (ch >= '0'))
            {
                result = (result<<3) + (ch-'0');
                ch = __GetChar(Source,Offset);
            }
            break;
        case 10:
            while (isdigit((s32)ch))
            {
                result = (result * 10) + (ch-'0');
                ch = __GetChar(Source,Offset);
            }
            break;
        case 16:
            while (isxdigit((s32)ch))
            {
                value = isdigit((s32)ch) ? ch-'0' : toupper((s32)ch)-'A'+10;
                result = (result<<4) + value;
                ch = __GetChar(Source,Offset);
            }
            break;
    }
    *LastCh = ch;
    return result;
}

s32 __sc_strtol(ptu32_t Source,char (*__GetChar)(ptu32_t Source,s32 *Offset),s32 *Offset,s32 base,char *LastCh)
{
    s32 result = 0,value;
    char ch = *LastCh;
    switch (base)
    {
        case 8:
            while ((ch <= '7') && (ch >= '0'))
            {
                result = (result<<3) + (ch-'0');
                ch = __GetChar(Source,Offset);
            }
            break;
        case 10:
            while (isdigit((s32)ch))
            {
                result = (result * 10) + (ch-'0');
                ch = __GetChar(Source,Offset);
            }
            break;
        case 16:
            while (isxdigit((s32)ch))
            {
                value = isdigit((s32)ch) ? ch-'0' : toupper((s32)ch)-'A'+10;
                result = (result<<4) + value;
                ch = __GetChar(Source,Offset);
            }
            break;
    }
    *LastCh = ch;
    return result;
}

u64 __sc_strtoull(ptu32_t Source,char (*__GetChar)(ptu32_t Source,s32 *Offset),s32 *Offset,s32 base,char *LastCh)
{
    u64 result = 0,value;
    char ch = *LastCh;
    switch (base)
    {
        case 8:
            while ((ch <= '7') && (ch >= '0'))
            {
                result = (result<<3) + (ch-'0');
                ch = __GetChar(Source,Offset);
            }
            break;
        case 10:
            while (isdigit((s32)ch))
            {
                result = (result * 10) + (ch-'0');
                ch = __GetChar(Source,Offset);
            }
            break;
        case 16:
            while (isxdigit((s32)ch))
            {
                value = isdigit((s32)ch) ? ch-'0' : toupper((s32)ch)-'A'+10;
                result = (result<<4) + value;
                ch = __GetChar(Source,Offset);
            }
            break;
    }
    *LastCh = ch;
    return result;
}

s64 __sc_strtoll(ptu32_t Source,char (*__GetChar)(ptu32_t Source,s32 *Offset),s32 *Offset,s32 base,char *LastCh)
{
    s64 result = 0,value;
    char ch = *LastCh;
    switch (base)
    {
        case 8:
            while ((ch <= '7') && (ch >= '0'))
            {
                result = (result<<3) + (ch-'0');
                ch = __GetChar(Source,Offset);
            }
            break;
        case 10:
            while (isdigit((s32)ch))
            {
                result = (result * 10) + (ch-'0');
                ch = __GetChar(Source,Offset);
            }
            break;
        case 16:
            while (isxdigit((s32)ch))
            {
                value = isdigit((s32)ch) ? ch-'0' : toupper((s32)ch)-'A'+10;
                result = (result<<4) + value;
                ch = __GetChar(Source,Offset);
            }
            break;
    }
    *LastCh = ch;
    return result;
}

//----从stdin读入一个字符------------------------------------------------------
//功能：从stdin读入一个字符，stdin可以是设备，也可以是文件
//参数：无
//返回：输入的字符，错误则返回EOF
//-----------------------------------------------------------------------------
s32 getchar( void )
{
    return getc(stdin);//todo:stdio.h已经宏实现
}

//----从stdin输入一个字符串----------------------------------------------------
//功能：从stdin输入一个字符串，stdin可以是设备，也可以是文件，甚至可以NULL（原始
//      硬件）。狗血的c标准，fgets函数有buf长度限定，但gets却没有，程序员应该自
//      己保证缓冲区足够长。
//参数：buf ，接收字符串的缓冲区
//返回：正确则返回buf指针，错误则返回NULL，并设置错误标志
//特注：1、gets会读出回车键，并替换成'\0'，scanf则把回车键留在输入缓冲区。
//      2、本函数对buf没有长度限制，强烈建议不要使用，用fgets替代。
//-----------------------------------------------------------------------------
char * gets(char *buf)
{
    fgets(buf,CN_LIMIT_SINT32,stdin);
    return buf;
}

//----从字符串中读取一个字符---------------------------------------------------
//功能：从字符串中读入一个字符
//参数：Source，字符串指针
//      Offset：读入的偏移量，返回累计读入的字符数
//返回：读入的字符。
//-----------------------------------------------------------------------------
s32 __GetCharFromStr(ptu32_t Source,s32 *Offset)
{
    s32 ch;
    ch = ((char*)Source)[*Offset];
    (*Offset)++;
    return ch;
}

//----从文件中读取一个字符-----------------------------------------------------
//功能：直接从原始硬件中读入一个字符
//参数：Source，文件指针
//      Offset：返回累计读入的字符数
//返回：读入的字符。
//-----------------------------------------------------------------------------
s32 __GetCharFromFile(ptu32_t Source,s32 *Offset)
{
    s32 ch;
    ch = getc((FILE*)Source);
    (*Offset)++;
    return ch;
}

//---------------------------------------------------------------------------
//功能: 按照格式字符串，解析输入字符串，Method参数设定输入字符串的来源
//参数: Source，这是一个多用途的变量，设定被解析的字符串的来源属性，
//          依据Method不同，含义不同。
//          Method==SCANF_FROM_STRING，Source是字符串指针
//          Method==SCANF_FROM_FILE，Source是文件指针
//      Method，SCANF_FROM_STRING 等常量之一
//      fmt，格式字符串
//      args，可变参数的参数列表
//返回: 转换结果数量
//说明：目前没有加入浮点数解析--TODO
//-----------------------------------------------------------------------------
static s32 __vsnscanf(ptu32_t Source,u32 Method, const char *fmt,va_list args)
{
    char (*__GetChar)(ptu32_t Source,s32 *Offset);
    s32 Offset = 0;
    char ch,sign;
    s32 num = 0;
    s32 qualifier;
    s32 base;
    s32 field_width = -1;
    s32 is_sign = 0;
    switch (Method)
    {
        case SCANF_FROM_STRING:
            __GetChar = __GetCharFromStr;
            break;

        case SCANF_FROM_FILE:
            __GetChar = __GetCharFromFile;
            break;
        default:
            __GetChar = __GetCharFromStr;
            break;
    }
    ch = __GetChar(Source,&Offset);
    while(*fmt && ch)
    {
        /* skip any white space in format */
        /* white space in format matchs any amount of
         * white space, including none, in the input.
         */
        if (isspace((s32)*fmt))
        {
            while (isspace((s32)*fmt))
                ++fmt;
            while (isspace((s32)ch))
                ch = __GetChar(Source,&Offset);
        }

        /* anything that is not a conversion must match exactly */
        if (*fmt != '%' && *fmt)
        {
            if (*fmt++ != ch)
                break;
            ch = __GetChar(Source,&Offset);
            continue;
        }

        if (!*fmt)
            break;
        ++fmt;

        /* skip this conversion.
         * advance both strings to next white space
         */
        if (*fmt == '*') {
            while (!isspace((s32)*fmt) && *fmt)
                fmt++;
            while (!isspace((s32)ch) && ch)
                ch = __GetChar(Source,&Offset);
            continue;
        }

        /* get field width */
        if (isdigit((s32)*fmt))
            field_width = skip_atoi(&fmt);

        /* get conversion qualifier */
        qualifier = -1;
        if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L' || *fmt == 'Z') {
            qualifier = *fmt;
            fmt++;
        }

        if (!*fmt || !ch)
            break;

        switch(*fmt++)
        {
            case 'c':
            {
                char *s = (char *) va_arg(args,char*);
                if (field_width == -1)
                    field_width = 1;
                do
                {
                    *s++ = ch;
                    ch = __GetChar(Source,&Offset);
                } while(field_width-- > 0 && ch);
                num++;
            }
            continue;
            case 's':
            {
                char *s = (char *) va_arg(args, char *);
                if(field_width == -1)
                    field_width = INT_MAX;
                /* first, skip leading white space in buffer */
                while (isspace((s32)ch))
                    ch = __GetChar(Source,&Offset);

                /* now copy until next white space */
                while (ch && !isspace((s32)ch) && field_width--)
                {
                    *s++ = ch;
                    ch = __GetChar(Source,&Offset);
                }
                *s = '\0';
                num++;
            }
            continue;
            case 'n':
                /* return number of characters read so far */
            {
                s32 *i = (s32 *)va_arg(args,s32*);
                *i = Offset;
            }
            continue;
            is_sign = 0;
            case 'o':
                base = 8;
                is_sign = 1;
                break;
            case 'x':
            case 'X':
                base = 16;
                is_sign = 1;
                break;
            case 'd':
                base = 10;
                is_sign = 1;
                break;
            case 'i':
                base = 0;
                is_sign = 1;
                break;
            case 'u':
                base = 10;
                is_sign = 0;
                break;
                break;
            case '%':
                if (ch != '%')      //查找 %
                    goto GotoErrorExit;         //非法格式，结束
                ch = __GetChar(Source,&Offset);
                continue;
            default:
                goto GotoErrorExit;         //非法格式，结束
        }

        //各种数值转换
        while (isspace((s32)ch))       //首先删除前导空格
            ch = __GetChar(Source,&Offset);

        if (!ch)                  //串结束
            break;

        sign = ch;
        if ((sign == '-') || (sign == '+'))
            ch = __GetChar(Source,&Offset);

        //base = 0表示由输入流表示数据格式（8/10/16进制）。
        if (base == 0)
        {
            base = 10;
            if (ch == '0') {
                base = 8;
                ch = __GetChar(Source,&Offset);
                if ((ch == 'x') || (ch == 'X'))
                {
                    ch = __GetChar(Source,&Offset);
                    base = 16;
                }
            }
        } else if (base == 16)
        {
            if (ch == '0')
            {
                ch = __GetChar(Source,&Offset);
                if ((ch == 'x') || (ch == 'X'))
                    ch = __GetChar(Source,&Offset);
            }
        }

        switch(qualifier)
        {
        case 'h':
            if (is_sign) {
                s16 *s = (s16 *) va_arg(args,s16 *);
                *s = (s16) __sc_strtol(Source,__GetChar,&Offset,base,&ch);
                if (sign == '-')
                    *s = -*s;
            } else {
                u16 *s = (u16 *) va_arg(args, u16 *);
                *s = (u16) __sc_strtoul(Source,__GetChar,&Offset,base,&ch);
            }
            break;
        case 'l':
            if (is_sign) {
                s32 *l = (s32 *) va_arg(args,s32 *);
                *l = __sc_strtol(Source,__GetChar,&Offset,base,&ch);
                if (sign == '-')
                    *l = -*l;
            } else {
                u32 *l = (u32 *) va_arg(args,u32*);
                *l = __sc_strtoul(Source,__GetChar,&Offset,base,&ch);
            }
            break;
        case 'L':
            if (is_sign) {
                s64 *l = (s64 *) va_arg(args,s64 *);
                *l = __sc_strtoll(Source,__GetChar,&Offset,base,&ch);
                if (sign == '-')
                    *l = -*l;
            } else {
                u64 *l = (u64 *) va_arg(args,u64*);
                *l = __sc_strtoull(Source,__GetChar,&Offset,base,&ch);
            }
            break;
        case 'Z':
        {
            u32 *s = (u32*) va_arg(args,u32*);
            *s = (u32) __sc_strtoul(Source,__GetChar,&Offset,base,&ch);
        }
        break;
        default:
            if (is_sign) {
                s32 *i = (s32 *) va_arg(args, s32*);
                *i = (s32) __sc_strtol(Source,__GetChar,&Offset,base,&ch);
                if (sign == '-')
                    *i = -*i;
            } else {
                u32 *i = (u32*) va_arg(args, u32*);
                *i = (u32) __sc_strtoul(Source,__GetChar,&Offset,base,&ch);
            }
            break;
        }
        num++;

        if (ch == '\0')
            break;
    }
    if((*fmt == '%') && (*(fmt+1) == 'n'))
    {
        s32 *i = (s32 *)va_arg(args,s32*);
        *i = Offset;
    }

GotoErrorExit:
    return num;
}

s32 fscanf(FILE *fp,const char * fmt, ...)
{
    va_list args;
    s32 i;
    if (fp == NULL)
        return 0;
    else
    {
        va_start(args,fmt);
        i = __vsnscanf((ptu32_t)fp,SCANF_FROM_FILE,fmt,args);
        va_end(args);
    }
    return i;
}

s32 sscanf(const char * buf, const char * fmt, ...)
{
    va_list args;
    s32 i;

    va_start(args,fmt);
    i = __vsnscanf((ptu32_t)buf,SCANF_FROM_STRING,fmt,args);
    va_end(args);

    return i;
}


s32 scanf(const char * fmt, ...)
{
    va_list args;
    s32 i;

    va_start(args,fmt);
    i = __vsnscanf((ptu32_t)stdin,SCANF_FROM_FILE,fmt,args);
    va_end(args);

    return i;
}


