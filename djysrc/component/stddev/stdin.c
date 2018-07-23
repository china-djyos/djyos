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

#define SCANF_FROM_STRING     0   //��string�ж�ȡ�ַ�
#define SCANF_FROM_FILE       1   //���ļ������룬����scanf��stdin���ļ���������fscanf

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

//----��stdin����һ���ַ�------------------------------------------------------
//���ܣ���stdin����һ���ַ���stdin�������豸��Ҳ�������ļ�
//��������
//���أ�������ַ��������򷵻�EOF
//-----------------------------------------------------------------------------
s32 getchar( void )
{
    return getc(stdin);//todo:stdio.h�Ѿ���ʵ��
}

//----��stdin����һ���ַ���----------------------------------------------------
//���ܣ���stdin����һ���ַ�����stdin�������豸��Ҳ�������ļ�����������NULL��ԭʼ
//      Ӳ��������Ѫ��c��׼��fgets������buf�����޶�����getsȴû�У�����ԱӦ����
//      ����֤�������㹻����
//������buf �������ַ����Ļ�����
//���أ���ȷ�򷵻�bufָ�룬�����򷵻�NULL�������ô����־
//��ע��1��gets������س��������滻��'\0'��scanf��ѻس����������뻺������
//      2����������bufû�г������ƣ�ǿ�ҽ��鲻Ҫʹ�ã���fgets�����
//-----------------------------------------------------------------------------
char * gets(char *buf)
{
    fgets(buf,CN_LIMIT_SINT32,stdin);
    return buf;
}

//----���ַ����ж�ȡһ���ַ�---------------------------------------------------
//���ܣ����ַ����ж���һ���ַ�
//������Source���ַ���ָ��
//      Offset�������ƫ�����������ۼƶ�����ַ���
//���أ�������ַ���
//-----------------------------------------------------------------------------
s32 __GetCharFromStr(ptu32_t Source,s32 *Offset)
{
    s32 ch;
    ch = ((char*)Source)[*Offset];
    (*Offset)++;
    return ch;
}

//----���ļ��ж�ȡһ���ַ�-----------------------------------------------------
//���ܣ�ֱ�Ӵ�ԭʼӲ���ж���һ���ַ�
//������Source���ļ�ָ��
//      Offset�������ۼƶ�����ַ���
//���أ�������ַ���
//-----------------------------------------------------------------------------
s32 __GetCharFromFile(ptu32_t Source,s32 *Offset)
{
    s32 ch;
    ch = getc((FILE*)Source);
    (*Offset)++;
    return ch;
}

//---------------------------------------------------------------------------
//����: ���ո�ʽ�ַ��������������ַ�����Method�����趨�����ַ�������Դ
//����: Source������һ������;�ı������趨���������ַ�������Դ���ԣ�
//          ����Method��ͬ�����岻ͬ��
//          Method==SCANF_FROM_STRING��Source���ַ���ָ��
//          Method==SCANF_FROM_FILE��Source���ļ�ָ��
//      Method��SCANF_FROM_STRING �ȳ���֮һ
//      fmt����ʽ�ַ���
//      args���ɱ�����Ĳ����б�
//����: ת���������
//˵����Ŀǰû�м��븡��������--TODO
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
                if (ch != '%')      //���� %
                    goto GotoErrorExit;         //�Ƿ���ʽ������
                ch = __GetChar(Source,&Offset);
                continue;
            default:
                goto GotoErrorExit;         //�Ƿ���ʽ������
        }

        //������ֵת��
        while (isspace((s32)ch))       //����ɾ��ǰ���ո�
            ch = __GetChar(Source,&Offset);

        if (!ch)                  //������
            break;

        sign = ch;
        if ((sign == '-') || (sign == '+'))
            ch = __GetChar(Source,&Offset);

        //base = 0��ʾ����������ʾ���ݸ�ʽ��8/10/16���ƣ���
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


