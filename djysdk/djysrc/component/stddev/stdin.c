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
#include "driver.h"
#include <djyfs/file.h>
#include "systime.h"
#include "ring.h"

#define SCANF_FROM_STRING     0   //��string�ж�ȡ�ַ�
#define SCANF_FROM_FILE       1   //���ļ������룬����scanf��stdin���ļ���������fscanf
//#define SCANF_FROM_DIRECT     2   //ֱ�Ӵ�ԭʼӲ���������ַ���
#define SCANF_FROM_DEV        3   //���豸�����룬����fscanf��fp���豸��
#define SCANF_FROM_STDIN_DEV  4   //��stdin�豸�����룬����scanf��stdin���豸��
s32 skip_atoi(const char **s);
extern struct RingBuf *g_ptStdinDevBuf;
#define CN_SCANF_BUF_LEN        32

//----�ӷ��ļ�stdin�ж�ȡһ���ַ�---------------------------------------------
//���ܣ����stdin���豸��getϵ�к�����stdin����ʱ�������ñ�
//      ������IOϵͳΪ���ļ���ʽ��stdin������һ�����λ���������������������ַ���
//      ��ֱ�Ӷ������ַ��ض��� ugetc���������ġ������stdin��ȡ���������ַ���
//      ���ظ��û�������ڻ��λ������С�
//������Source���豸ָ��
//      Offset�������ۼƶ�����ַ���
//      ch��
//���أ�������ַ���
//-----------------------------------------------------------------------------
//bool_t __ReadCharToStdinDev(char *ch)
//{
//    bool_t result = true;
//
//    if (Ring_Check(g_ptStdinDevBuf) != 0)       //��黺�������Ƿ�������
//        Ring_Read(g_ptStdinDevBuf,(u8*)ch,1);   //�����ݣ�ֱ�Ӷ�ȡ
//    else
//    {
//        if (stdin != StdNotInit)   //���stdin�Ѿ���ʼ��������ض����豸�����÷�ȷ��
//        {
//            if(Driver_ReadDevice((struct DjyDevice *)stdin,(u8*)ch,1,0,CN_TIMEOUT_FOREVER) == 1)
//            {
//                Ring_Write(g_ptStdinDevBuf,(u8*)(&ch),1);   //����ͬʱд�뻺������
//                Ring_PseudoRead(g_ptStdinDevBuf,1);           //�ƶ�һ������
//            }
//            else
//                result = false;
//        }
//   }
//    return result;
//}

//----���ļ�����һ���ַ�-------------------------------------------------------
//���ܣ����ļ�����һ���ַ���getc��fgetc�ǵȼ۵ģ���Ц��c��׼��fp�����������ļ���
//      Ҳ������stdin�������stdin��������NULL�������豸��
//      ��stdin���豸����NULL����Ҫ���ԡ�
//������fp�����Ŀ���ļ��������stdin����Ҫ�ж��Ƿ��豸��
//���أ���ȡ���ַ��������򷵻�EOF
//-----------------------------------------------------------------------------
s32 getc(FILE *fp)     //getc = fgetc
{
    char ch;
    if (fp != NULL)
    {
        //���ļ������ļ�����ungetc���ļ�ϵͳ�ṩ֧�֣�����Ҫ����
        if (IS_FILE(fp))
        {
            if(fread(&ch,1,1,fp) != 1)
            {
                ch = EOF;
            }
        }
        else if(fp == stdin)    //���ļ�������stdin��
        {
            if (fp == StdNotInit)   //stdinδ��ʼ������֧��ungetc����Ҫ����
            {
                if (GetCharDirect)
                {
                    ch = GetCharDirect( );
                    putchar(ch);
                }
                else
                {
                    ch = EOF;
                }
            }
            else                   //stdin�ѳ�ʼ����֧��ungetc����Ҫ����
                if(fread(&ch,1,1,fp) != 1)
                {
                    Ring_Write(g_ptStdinDevBuf,(u8*)(&ch),1);   //����ͬʱд�뻺������
                    Ring_PseudoRead(g_ptStdinDevBuf,1);           //�ƶ�һ������
                    putchar(ch);
                }
                else
                {
                    ch = EOF;
                }
        }
        else        //���豸���Ҳ���stdin����֧��ungetc������Ҫ����
        {
            if(fread(&ch,1,1,fp) != 1)
            {
                ch = EOF;
            }
        }
    }
    else
    {
       ch = EOF;
    }
    if (ch == EOF)
        Djy_SaveLastError(EN_FS_READFILE_EOF);
    return ch;
}

//----���ļ�����һ��-----------------------------------------------------------
//���ܣ����ļ�����һ���ַ����������stdin���򻹿������豸����������ֱ���س���
//      �����ո�
//����buf�������ַ����Ļ�����
//      size��buf�ĳ���
//      fp�����Դ�ļ��������stdin����߱��򵥵ı༭����,�������豸����NULL��
//���أ�1���ɹ����򷵻ص�һ������buf��
//      2���ڶ��ַ�ʱ����end-of-file����eofָʾ�������ã������û�����κ��ַ���
//         ���������������buf����ԭ�������ݣ�����NULL��
//      3����������������errorָʾ�������ã�����NULL��buf��ֵ���ܱ��ı䡣
//-----------------------------------------------------------------------------
#define EditReview          0   //֧���б༭�ͻ���
#define EditNoReview        1   //֧���б༭����֧�ֻ��ԡ�
#define NoEditreview        2   //��֧���б༭��֧�ֻ���
#define NoEditNoReview      3   //��֧���б༭����֧�ֻ���
#define InputError          4
char * fgets(char *buf, s32 size,FILE *fp)
{
    s32 result;
    s32 position = 0;
    char ch = 0;
    if (size == 0)
        return NULL;
    if (size == 1)
    {
        buf[0] = '\0';
        return buf;
    }
    if (fp == NULL)
    {
        Djy_SaveLastError(EN_FS_READFILE_EOF);
        return NULL;
    }
    //���ļ������ļ�����ungetc���ļ�ϵͳ�ṩ֧�֣���֧���б༭������Ҫ����
    //���豸�ҷ�stdin�����豸������֧��ungetc����֧���б༭������Ҫ����
    if (IS_FILE(fp) || (fp != stdin))
    {
        for (position = 0; position < size; position++)
        {
            if(fread(&ch,1,1,fp) != 1)
            {
                buf[position] = '\0';
                Djy_SaveLastError(EN_FS_READFILE_EOF);
                return NULL;
            }
            //���Իس����Ի��з��������ܼ���Windows��Linux��ʽ�ļ���
            if(ch == '\r')
            {
                if (((struct Object*)fp)->Type == RSC_FILE)
                    continue;      //Windows��ʽ�ı��ļ��� '\r'������
                else
                {
                    buf[position] = '\0';   //���ļ���һ���Իس�����һ�С�
                    break;
                }
            }
            else if(ch == '\n') //�����з����������
            {
                buf[position] = '\0';
                puts("\n\r");
                break;
            }
            else
            {
                buf[position] = ch;
                continue;       //ֱ����һѭ�����ƹ�������ַ��༭����
            }
        }
        buf[size-1] = '\0';
        return buf;
    }
    if (fp == StdNotInit)   //ԭʼӲ�����룬��֧��ungetc����֧���б༭����Ҫ����
    {
        for (position = 0; position < size; position++)
        {
            if (GetCharDirect)
            {
                ch = GetCharDirect( );
            }
            else
                return NULL;
            if((ch == '\n') || (ch == '\r')) //�����з����������
            {
                buf[position] = '\0';
                break;
            }
            else
            {
                buf[position] = ch;
                putchar(ch);
                continue;       //ֱ����һѭ�����ƹ�������ַ��༭����
            }
        }
        buf[size-1] = '\0';
        return buf;
    }
    else            //stdin�豸���룬֧��ungetc��֧���б༭����Ҫ����
    {
        if ( ! Ring_IsEmpty(g_ptStdinDevBuf))
        {
            position = Ring_Read(g_ptStdinDevBuf,(u8*)buf,size-1);
            buf[position] = '\0';
        }
        else
        {
            while ( 1 )
            {
                if(fread(&ch,1,1,fp) != 1)
                {
                    position = Ring_Read(g_ptStdinDevBuf,(u8*)buf,size-1);
                    buf[position] = '\0';
                    Djy_SaveLastError(EN_FS_READFILE_EOF);
                    return NULL;
                }
                switch (ch)
                {
                    case 37:        //left
                    case 38:        //up
                    case 39:        //right
                    case 40:        //down
                    case '\t':      //tab
                        break;
                    case '\r':      //�س���
                    case '\n':      //���м�
                    {
                        position = Ring_Read(g_ptStdinDevBuf,(u8*)buf,size-1);
                        buf[position] = '\0';
                        puts("\n\r");
                        return buf;
                    }break;
                    case '\b':      //�˸��
                    {
                        if ( ! Ring_IsEmpty(g_ptStdinDevBuf))
                        {
                            Ring_SkipTail(g_ptStdinDevBuf,1);
                            puts("\b \b");
                        }
                    }break;
                    case 0x1B:      //esc�����Ѿ�������ַ�ȫ��ȡ��
                    {
                        position = Ring_Check(g_ptStdinDevBuf);
                        Ring_Clean(g_ptStdinDevBuf);
                        for (result = 0;result < position;result++)
                            puts("\b \b");
                    }break;
                    default:
                    {
                        //����л������������Ͳ��ٽ������롣
                        if( ! Ring_IsFull(g_ptStdinDevBuf))
                        {
                            Ring_Write(g_ptStdinDevBuf,(u8*)&ch,1);
                            putchar(ch);
                        }
                    }break;
                }
            }
        }
    }
    return buf;
}

//----�������л���һ���ַ�-----------------------------------------------------
//���ܣ���һ���ַ��˻ظ�����������������ָ��ӵ�ǰλ�û���һ���ַ������ô����
//      �ַ� c ���Ǹ��ַ������stream��stdin�����豸������stdioģ���ڽ��Ļ���
//      ���������ˡ�
//������c�������ظ����������ַ�
//      stream��Ŀ��������
//���أ��ɹ�ִ�з��ز���c�����򷵻�EOF
//-----------------------------------------------------------------------------
s32 ungetc(s32 c, FILE *stream)
{
    s32 result = c;
    if(stream == stdin)
    {
        //������stdin==StdNotInit������ִ��ungetc
        if (stdin != StdNotInit)
        {
            if(((struct Object*)stdin)->Type == RSC_DEVICE)
            {
                if ( ! Ring_IsFull(g_ptStdinDevBuf))
                    Ring_RecedeRead(g_ptStdinDevBuf,1,(u8*)(&c));
                else
                    result = EOF;       //������û�пռ���Ի���
            }
        }
        else
            return EOF;
    }
    else
    {
        if( ! fseek(stream,-1,SEEK_CUR))
            result = EOF;
    }
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

//----ֱ�Ӷ�ȡһ���ַ�---------------------------------------------------------
//���ܣ�ֱ�Ӵ�ԭʼӲ���ж���һ���ַ�
//������Source��������
//      Offset�������ۼƶ�����ַ���
//���أ�������ַ���
//-----------------------------------------------------------------------------
//char __GetCharDirect(ptu32_t Source, s32 *Offset)
//{
//    char ch;
//    ch = GetCharDirect( );
//    (*Offset)++;
//    PutStrDirect(&ch,1);
//    return ch;
//}

//----���ַ����ж�ȡһ���ַ�---------------------------------------------------
//���ܣ����ַ����ж���һ���ַ�
//������Source���ַ���ָ��
//      Offset�������ƫ�����������ۼƶ�����ַ���
//���أ�������ַ���
//-----------------------------------------------------------------------------
char __GetCharFromStr(ptu32_t Source,s32 *Offset)
{
    char ch;
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
char __GetCharFromFile(ptu32_t Source,s32 *Offset)
{
    char ch;
    fread(&ch,1,1,(FILE*)Source);
    (*Offset)++;
    return ch;
}

//----���豸�ж�ȡһ���ַ�-----------------------------------------------------
//���ܣ�ֱ�Ӵ�ԭʼӲ���ж���һ���ַ�����������������ַ�����ֱ�Ӷ������ַ��ض�
//      �� ugetc���������ġ�������豸��ȡ��
//������Source���豸ָ��
//      Offset�������ۼƶ�����ַ���
//���أ�������ַ���
//-----------------------------------------------------------------------------
char __GetCharFromDev(ptu32_t Source,s32 *Offset)
{
    char ch;
    fread(&ch,1,1,(FILE*)Source);
    (*Offset)++;
    return ch;
}

//----��Stdin�豸�ж�ȡһ���ַ�------------------------------------------------
//���ܣ�ֱ�Ӵ�ԭʼӲ���ж���һ���ַ�����������������ַ�����ֱ�Ӷ������ַ��ض�
//      �� ugetc���������ġ�������豸��ȡ��
//������Source���豸ָ��
//      Offset�������ۼƶ�����ַ���
//���أ�������ַ���
//-----------------------------------------------------------------------------
static s32 s_StdinDevBufBytes = 0;
static s32 s_StdinDevposition = 0;
static char s_StdinDevBuf[CN_SCANF_BUF_LEN];
char __GetCharFromStdinDev(ptu32_t Source,s32 *Offset)
{
    char ch;
    if (s_StdinDevposition == s_StdinDevBufBytes)        //buf��û������
    {
        fgets(s_StdinDevBuf,CN_SCANF_BUF_LEN,stdin);
        s_StdinDevBufBytes = strlen(s_StdinDevBuf);
        s_StdinDevposition = 0;
        if(Ring_IsEmpty(g_ptStdinDevBuf))
            s_StdinDevBuf[s_StdinDevBufBytes++] = '\r';
    }
    ch = s_StdinDevBuf[s_StdinDevposition++];
    (*Offset)++;
    return ch;
}

//---------------------------------------------------------------------------
//����: ���ո�ʽ�ַ��������������ַ�����Method�����趨�����ַ�������Դ
//����: TempBuf���ַ���ת��ʹ�õ���ʱ��������������CN_BUF_LENGTH�����
//          Method==SCANF_FROM_STRING����Ա��������ɴ���NULL
//      Source������һ������;�ı������趨���������ַ�������Դ���ԣ�
//          ����Method��ͬ�����岻ͬ��
//          Method==SCANF_FROM_STRING��Source���ַ���ָ��
//          Method==SCANF_FROM_DIRECT��Source������
//          Method==SCANF_FROM_FILE_OR_DEV��Source���ļ����豸ָ��
//      Size�����Source���ַ���ָ�룬Size��ʾ���ݳߴ磬����û������
//      Method�����������SCANF_FROM_STRING�ȳ���֮һ
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

//        case SCANF_FROM_DIRECT:
//            __GetChar = __GetCharDirect;
//            break;
        case SCANF_FROM_FILE:
            __GetChar = __GetCharFromFile;
            break;
        case SCANF_FROM_DEV:
            __GetChar = __GetCharFromDev;
            break;
        case SCANF_FROM_STDIN_DEV:
            __GetChar = __GetCharFromStdinDev;
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
    if (SCANF_FROM_STDIN_DEV == Method) //��û����������ݻ���������
    {
        if(s_StdinDevBufBytes != s_StdinDevposition)
        {
            if(s_StdinDevBuf[s_StdinDevBufBytes] == '\r')
                //���һ��'\r'����Ϊ��ӽ�ȥ�ģ�����Ҫ�����豸��������
                Ring_RecedeRead(g_ptStdinDevBuf,s_StdinDevBufBytes-s_StdinDevposition-1,NULL);
            else
                Ring_RecedeRead(g_ptStdinDevBuf,s_StdinDevBufBytes-s_StdinDevposition,NULL);
            s_StdinDevBufBytes = 0;
            s_StdinDevposition = 0;
        }
    }

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
        if (((struct Object*)fp)->Type == RSC_FILE)
        {
            i = __vsnscanf((ptu32_t)fp,SCANF_FROM_FILE,fmt,args);
        }
        else if (stdin == fp)   //fgets�������ṩ���Ժ��б༭����
        {
            i = __vsnscanf(0,SCANF_FROM_STDIN_DEV,fmt,args);
        }
        else
        {
            i = __vsnscanf((ptu32_t)fp,SCANF_FROM_DEV,fmt,args);
        }
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
    if (((struct Object*)stdin)->Type == RSC_FILE)
    {
        i = __vsnscanf((ptu32_t)stdin,SCANF_FROM_FILE,fmt,args);
    }
    else        //fgets�������ṩ���Ժ��б༭���ܣ����ﲻ�ܡ�
    {
        i = __vsnscanf(0,SCANF_FROM_STDIN_DEV,fmt,args);
    }
    va_end(args);

    return i;
}


