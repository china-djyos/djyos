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
//����:  ��ǰ��.
//�汾��V1.0.0
//�ļ�����: ��׼�������
//����˵��:
//�޶���ʷ:
//2. ���ڣ�2015-10-16
//   ����:  ������.
//   �°汾��: V1.1.0
//   �޸�˵��: ȫ���޶�������׼IO���ƥ��
//1. ����: 2013-11-06
//   ����:  ��ǰ��.
//   �°汾��: V1.0.0
//   �޸�˵��: �����ļ�
//------------------------------------------------------
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stddef.h>
#include <errno.h>
#include <systime.h>
#include <device.h>
#include <djyos.h>
#include <djyfs/file.h>
#include "component_config_stdio.h"

#define CN_BUF_LENGTH   0x40

#define ZEROPAD     1           /* pad with zero */
#define SIGN        2           /* unsigned/s32 */
#define PLUS        4           /* show plus */
#define SPACE       8           /* space if plus */
#define LEFT        16          /* left justified */
#define SMALL       32          /* Must be 32 == 0x20 */
#define SPECIAL     64          /* 0x */

#define PRINT_TO_STRING         0   //�����string������sprintf�ȡ�
#define PRINT_TO_DIRECT         1   //ֱ�������ԭʼӲ��������printk�ȡ�
#define PRINT_TO_FILE_OR_DEV    2   //������ļ�/�豸������stdout/stderr��file
s32 skip_atoi(const char **s);


// ============================================================================
// ���ܣ������Ƿ�����Ч�ĵ��ļ�����
// ������stream -- �ļ�����
// ���أ��ǣ�1�������ǣ�0����
// ��ע��
// ============================================================================
extern s32 File_IsValid(FILE* stream);

//----���һ���ַ���stdout-----------------------------------------------------
//���ܣ����һ���ַ���stdout��stdout�������豸��Ҳ�������ļ�
//������ch ����������ַ�
//���أ���ȷ����򷵻ر�������ַ��������򷵻�EOF
//-----------------------------------------------------------------------------
#ifndef putchar
s32 putchar (s32 ch)
{
    return putc(ch,stdout);
}
#endif

//----���һ���ַ�����stdout-----------------------------------------------------
//���ܣ����һ���ַ�����stdout��stdout�������豸��Ҳ�������ļ�
//������str ����������ַ���
//���أ���ȷ����򷵻�һ���Ǹ�ֵ�������򷵻�EOF
//-----------------------------------------------------------------------------
s32 puts (const char *str)
{
    return fputs(str,stdout);
}

u32 __div64_32(u64 *n, u32 base)
{
    u64 rem = *n;
    u64 b = base;
    u64 res, d = 1;
    u32  high = rem >> 32;

    /* Reduce the thing a bit first */
    res = 0;
    if (high >= base) {
        high /= base;
        res = (u64) high << 32;
        rem -= (u64) (high*base) << 32;
    }

    while ((u64 )b > 0 && b < rem) {
        b = b+b;
        d = d+d;
    }

    do {
        if (rem >= b) {
            rem -= b;
            res += d;
        }
        b >>= 1;
        d >>= 1;
    } while (d);

    *n = res;
    return rem;
}

/* Decimal conversion is by far the most typical, and is used
 * for /proc and /sys data. This directly impacts e.g. top performance
 * with many processes running. We optimize it for speed
 * using code from
 * http://www.cs.uiowa.edu/~jones/bcd/decimal.html
 * (with permission from the author, Douglas W. Jones). */

/* Formats correctly any integer in [0,99999].
 * Outputs from one to five digits depending on input.
 * On i386 gcc 4.1.2 -O2: ~250 bytes of code. */
static char *put_dec_trunc(char *buf, unsigned q)
{
    unsigned d3, d2, d1, d0;
    d1 = (q>>4) & 0xf;
    d2 = (q>>8) & 0xf;
    d3 = (q>>12);

    d0 = 6*(d3 + d2 + d1) + (q & 0xf);
    q = (d0 * 0xcd) >> 11;
    d0 = d0 - 10*q;
    *buf++ = d0 + '0'; /* least significant digit */
    d1 = q + 9*d3 + 5*d2 + d1;
    if (d1 != 0) {
        q = (d1 * 0xcd) >> 11;
        d1 = d1 - 10*q;
        *buf++ = d1 + '0'; /* next digit */

        d2 = q + 2*d2;
        if ((d2 != 0) || (d3 != 0)) {
            q = (d2 * 0xd) >> 7;
            d2 = d2 - 10*q;
            *buf++ = d2 + '0'; /* next digit */

            d3 = q + 4*d3;
            if (d3 != 0) {
                q = (d3 * 0xcd) >> 11;
                d3 = d3 - 10*q;
                *buf++ = d3 + '0';  /* next digit */
                if (q != 0)
                    *buf++ = q + '0'; /* most sign. digit */
            }
        }
    }
    return buf;
}
/* Same with if's removed. Always emits five digits */
static char *put_dec_full(char *buf, unsigned q)
{
    /* BTW, if q is in [0,9999], 8-bit ints will be enough, */
    /* but anyway, gcc produces better code with full-sized ints */
    unsigned d3, d2, d1, d0;
    d1 = (q>>4) & 0xf;
    d2 = (q>>8) & 0xf;
    d3 = (q>>12);

    /*
     * Possible ways to approx. divide by 10
     * gcc -O2 replaces multiply with shifts and adds
     * (x * 0xcd) >> 11: 11001101 - shorter code than * 0x67 (on i386)
     * (x * 0x67) >> 10:  1100111
     * (x * 0x34) >> 9:    110100 - same
     * (x * 0x1a) >> 8:     11010 - same
     * (x * 0x0d) >> 7:      1101 - same, shortest code (on i386)
     */

    d0 = 6*(d3 + d2 + d1) + (q & 0xf);
    q = (d0 * 0xcd) >> 11;
    d0 = d0 - 10*q;
    *buf++ = d0 + '0';
    d1 = q + 9*d3 + 5*d2 + d1;
        q = (d1 * 0xcd) >> 11;
        d1 = d1 - 10*q;
        *buf++ = d1 + '0';

        d2 = q + 2*d2;
            q = (d2 * 0xd) >> 7;
            d2 = d2 - 10*q;
            *buf++ = d2 + '0';

            d3 = q + 4*d3;
                q = (d3 * 0xcd) >> 11; /* - shorter code */
                /* q = (d3 * 0x67) >> 10; - would also work */
                d3 = d3 - 10*q;
                *buf++ = d3 + '0';
                    *buf++ = q + '0';
    return buf;
}
/* No inlining helps gcc to use registers better */
static  char *put_dec(char *buf, u64  num)
{
    while (1) {
        unsigned rem;
        if (num < 100000)
            return put_dec_trunc(buf, num);
//        rem = do_div(num, 100000);
        rem = num % 100000;
        num = num / 100000;
        buf = put_dec_full(buf, rem);
    }
}

// =============================================================================

static void forcdecpt(char *buffer)
{
  while (*buffer)
  {
    if (*buffer == '.') return;
    if (*buffer == 'e' || *buffer == 'E') break;
    buffer++;
  }
  if (*buffer)
  {
    int n = strlen(buffer);
    while (n > 0)
    {
      buffer[n + 1] = buffer[n];
      n--;
    }
    *buffer = '.';
  }
  else
  {
    *buffer++ = '.';
    *buffer = '\0';
  }
}
static void cropzeros(char *buffer)
{
  char *stop;
  while (*buffer && *buffer != '.') buffer++;
  if (*buffer++)
  {
    while (*buffer && *buffer != 'e' && *buffer != 'E') buffer++;
    stop = buffer--;
    while (*buffer == '0') buffer--;
    if (*buffer == '.') buffer--;
    while (*(++buffer) = *stop++);
  }
}
#include <stdlib.h>
char *ecvtbuf(double arg, int ndigits, int *decpt, int *sign, char *buf);
char *fcvtbuf(double arg, int ndigits, int *decpt, int *sign, char *buf);
static void cfltcvt(double value, char *buffer, char fmt, int precision)
{
  int decpt, sign, exp, pos;
  char *digits = NULL;
  char cvtbuf[80];
  int capexp = 0;
  int magnitude;
  if (fmt == 'G' || fmt == 'E')
  {
    capexp = 1;
    fmt += 'a' - 'A';
  }
  if (fmt == 'g')
  {
    digits = ecvtbuf(value, precision, &decpt, &sign, cvtbuf);
    magnitude = decpt - 1;
    if (magnitude < -4  ||  magnitude > precision - 1)
    {
      fmt = 'e';
      precision -= 1;
    }
    else
    {
      fmt = 'f';
      precision -= decpt;
    }
  }
  if (fmt == 'e')
  {
    digits = ecvtbuf(value, precision + 1, &decpt, &sign, cvtbuf);
    if (sign) *buffer++ = '-';
    *buffer++ = *digits;
    if (precision > 0) *buffer++ = '.';
    memcpy(buffer, digits + 1, precision);
    buffer += precision;
    *buffer++ = capexp ? 'E' : 'e';
    if (decpt == 0)
    {
      if (value == 0.0)
        exp = 0;
      else
        exp = -1;
    }
    else
      exp = decpt - 1;
    if (exp < 0)
    {
      *buffer++ = '-';
      exp = -exp;
    }
    else
      *buffer++ = '+';
//    buffer[2] = (exp % 10) + '0';
//    exp = exp / 10;
    buffer[1] = (exp % 10) + '0';
    exp = exp / 10;
    buffer[0] = (exp % 10) + '0';
    buffer += 2;
  }
  else if (fmt == 'f')
  {
    digits = fcvtbuf(value, precision, &decpt, &sign, cvtbuf);
    if (sign) *buffer++ = '-';
    if (*digits)
    {
      if (decpt <= 0)
      {
        *buffer++ = '0';
        *buffer++ = '.';
        for (pos = 0; pos < -decpt; pos++) *buffer++ = '0';
        while (*digits) *buffer++ = *digits++;
      }
      else
      {
        pos = 0;
        while (*digits)
        {
          if (pos++ == decpt) *buffer++ = '.';
          *buffer++ = *digits++;
        }
      }
    }
    else
    {
      *buffer++ = '0';
      if (precision > 0)
      {
        *buffer++ = '.';
        for (pos = 0; pos < precision; pos++) *buffer++ = '0';
      }
    }
  }
  *buffer = '\0';
}




static u32 flt(char *TempBuf, ptu32_t Target, s32 Size, double num,
        int size, int precision, char fmt,int flags,u32 position,
        u32 (*PushChar)(char *TempBuf,ptu32_t Target, s32 Size, char ch,s32 position))
{
  char tmp[80];
  char c, sign;
  int n, i;

  memset(tmp,0x00,80);
  // Left align means no zero padding
  if (flags & LEFT) flags &= ~ZEROPAD;
  // Determine padding and sign char
  c = (flags & ZEROPAD) ? '0' : ' ';
  sign = 0;
  if (flags & SIGN)
  {
    if (num < 0.0)
    {
      sign = '-';
      num = -num;
      size--;
    }
    else if (flags & PLUS)
    {
      sign = '+';
      size--;
    }
    else if (flags & SPACE)
    {
      sign = ' ';
      size--;
    }
  }
  // Compute the precision value
  if (precision < 0)
    precision = 6; // Default precision: 6
  else if (precision == 0 && fmt == 'g')
    precision = 1; // ANSI specified
  // Convert floating point number to text
  cfltcvt(num, tmp, fmt, precision);
  // '#' and precision == 0 means force a decimal point
  if ((flags & SPECIAL) && precision == 0) forcdecpt(tmp);
  // 'g' format means crop zero unless '#' given
  if (fmt == 'g' && !(flags & SPECIAL)) cropzeros(tmp);
  n = strlen(tmp);
  // Output number with alignment and padding
  size -= n;
  if (!(flags & (ZEROPAD | LEFT)))
  {
      while(size-- > 0)
      {
          position = PushChar(TempBuf,Target, Size,' ',position);
      }
  }
  if (sign)
  {
      position = PushChar(TempBuf,Target, Size,sign,position);
  }
  if (!(flags & LEFT))
  {
      while (size-- > 0)
      {
          position = PushChar(TempBuf,Target, Size,c,position);
      }
  }
  for (i = 0; i < n; i++)
  {
      position = PushChar(TempBuf,Target, Size,tmp[i],position);
  }
  while (size-- > 0)
  {
      position = PushChar(TempBuf,Target, Size,' ',position);
  }
  return position;
}

//==============================================================================

static u32 number(char *TempBuf,ptu32_t Target, s32 Size, u64  num,\
        s32 base, s32 size, s32 precision, s32 type, u32 position,\
        u32 (*PushChar)(char *TempBuf,ptu32_t Target, s32 Size, char ch,s32 position))
{
    /* we are called with base 8, 10 or 16, only, thus don't need "G..."  */
    static const char digits[ ] = "0123456789ABCDEF";

    char tmp[66];
    char sign;
    char locase;
    s32 need_pfx = ((type & SPECIAL) && base != 10);
    s32 i;

    /* locase = 0 or 0x20. ORing digits or letters with 'locase'
     * produces same digits or (maybe lowercased) letters */
    locase = (type & SMALL);
    if (type & LEFT)
        type &= ~ZEROPAD;
    sign = 0;
    if (type & SIGN) {
        if ((s64) num < 0) {
            sign = '-';
            num = -(s64) num;
            size--;
        } else if (type & PLUS) {
            sign = '+';
            size--;
        } else if (type & SPACE) {
            sign = ' ';
            size--;
        }
    }
    if (need_pfx) {
        size--;
        if (base == 16)
            size--;
    }

    /* generate full string in tmp[], in reverse order */
    i = 0;
    if (num == 0)
        tmp[i++] = '0';
    /* Generic code, for any base:
    else do {
        tmp[i++] = (digits[do_div(num,base)] | locase);
    } while (num != 0);
    */
    else if (base != 10) { /* 8 or 16 */
        s32 mask = base - 1;
        s32 shift = 3;

        if (base == 16)
            shift = 4;

        do {
            tmp[i++] = (digits[((unsigned char)num) & mask]
                    | locase);
            num >>= shift;
        } while (num);
    } else { /* base 10 */
        i = put_dec(tmp, num) - (char *)tmp;
    }

    /* printing 100 using %2d gives "100", not "00" */
    if (i > precision)
        precision = i;
    /* leading space padding */
    size -= precision;
    if (!(type & (ZEROPAD + LEFT))) {
        while (--size >= 0)
            position = PushChar(TempBuf,Target, Size,' ',position);
//          position = ADDDCH_With_push(buf, ' ', position, strputfunc);
    }
    /* sign */
    if (sign)
        position = PushChar(TempBuf,Target, Size,sign,position);
//      position = ADDDCH_With_push(buf, sign, position, strputfunc);
    /* "0x" / "0" prefix */
    if (need_pfx) {
        position = PushChar(TempBuf,Target, Size,'0',position);
//      position = ADDDCH_With_push(buf, '0', position, strputfunc);
        if (base == 16)
            position = PushChar(TempBuf,Target, Size, ' ',position);
//          position = ADDDCH_With_push(buf, 'X' | locase, position, strputfunc);
    }
    /* zero or space padding */
    if (!(type & LEFT)) {
        char c = (type & ZEROPAD) ? '0' : ' ';

        while (--size >= 0)
            position = PushChar(TempBuf,Target, Size, c,position);
//          position = ADDDCH_With_push(buf, c, position, strputfunc);
    }
    /* hmm even more zero padding? */
    while (i <= --precision)
        position = PushChar(TempBuf,Target, Size, '0',position);
//      position = ADDDCH_With_push(buf, '0', position, strputfunc);
    /* actual digits of result */
    while (--i >= 0)
        position = PushChar(TempBuf,Target, Size, tmp[i],position);
//      position = ADDDCH_With_push(buf, tmp[i], position, strputfunc);
    /* trailing space padding */
    while (--size >= 0)
        position = PushChar(TempBuf,Target, Size, ' ',position);
//      position = ADDDCH_With_push(buf, ' ', position, strputfunc);
    return position;
}

static u32 string(char *TempBuf,ptu32_t Target, s32 Size, char *s, s32 field_width,
        s32 precision, s32 flags, u32 position,
        u32 (*PushChar)(char *TempBuf,ptu32_t Target, s32 Size, char ch,s32 position))
{
    s32 len, i;

    if (s == NULL)
        s = "<NULL>";

    len = strnlen(s, precision);

    if (!(flags & LEFT))
        while (len < field_width--)
            position = PushChar(TempBuf,Target, Size, ' ',position);
//          position = ADDDCH_With_push(buf, ' ', position, strputfunc);
    for (i = 0; i < len; ++i)
        position = PushChar(TempBuf,Target, Size, *s++,position);
//      position = ADDDCH_With_push(buf, *s++, position, strputfunc);
    while (len < field_width--)
        position = PushChar(TempBuf,Target, Size, ' ',position);
//      position = ADDDCH_With_push(buf, ' ', position, strputfunc);
    return position;
}
static u32 pointer(char *TempBuf,ptu32_t Target, s32 Size, void *ptr,
        s32 field_width, s32 precision, s32 flags, u32 position,
        u32 (*PushChar)(char *TempBuf,ptu32_t Target, s32 Size, char ch,s32 position))
{
    flags |= SMALL;
    if (field_width == -1) {
        field_width = 2*sizeof(void *);
        flags |= ZEROPAD;
    }
    return number(TempBuf,Target, Size, (u32)ptr, 16, field_width,
              precision, flags,position, PushChar);
}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

//----���ַ�ѹ���ַ�����-------------------------------------------------------
//���ܣ���һ���ַ������ַ����У����������ȣ�����ԡ�
//������TempBuf��Ϊ����ԭ��һ�¶����Ĳ�����������
//      Target,�����ַ����Ļ�������ַ��
//      Size���������ߴ磬��ĩ��'\0'
//      ch��ѹ����ַ�
//      Position��Buf��ƫ����
//���أ�Position����λ�á�
//-----------------------------------------------------------------------------
u32 __PushCharToString(char *TempBuf,ptu32_t Target,s32 Size,const char ch,s32 Position)
{
    if(Position < (Size-1))
    {
        *((char*)Target + Position) = ch;
        Position ++;
    }
    return Position;
}

//----���ֻ����ֱ�����------------------------------------------------------
//���ܣ���һ���ַ��Ȼ��嵽�ַ���TempBuf�У��������˺�ֱ�������Ӳ�������紮�ڡ�
//������TempBuf���ݴ滺������������CN_BUF_LENGTH
//      Target,�����塣
//      Size��������
//      ch��ѹ����ַ�
//      Position��Buf��ƫ����
//���أ�Position����λ�á�
//-----------------------------------------------------------------------------
u32 __PushCharDirect(char *TempBuf,ptu32_t Target,s32 Size,const char ch,s32 Position)
{
    if(Position >= CN_BUF_LENGTH)
    {
        if (PutStrDirect != NULL)
            PutStrDirect((const char *)TempBuf,CN_BUF_LENGTH);
        Position = 1;
        *TempBuf = ch;
    }
    else
    {
        *(TempBuf + Position) = ch;
        Position ++;
    }
    return Position;
}

//----���ַ�ѹ���ļ�/�豸��--------------------------------------------------
//���ܣ���һ���ַ��ļ�/�豸�����У��Ȼ��嵽�ַ���TempBuf�У��������˺����
//      ���ļ�/�豸������������printf/fprintf/vprintf/vfprintf�ȡ�
//      djyos�Ա�׼c��stdout/stdin/stderr������չ����׼c�У�����ֻ����FILE *
//      ���ͣ���djyos�У�����Ҳ������tagDevHandle����������ʹ���ļ�ϵͳ���õ�
//      �ĳ��ϣ�Ҳ��������ʹ��printf�Ⱥ�����
//������TempBuf��Ϊ����ԭ��һ�¶����Ĳ�����������
//      Target,�����ַ������ļ�/�豸ָ�롣
//      Size���������ߴ磬��ĩ��'\0'
//      ch��ѹ����ַ�
//      Position��Buf��ƫ����
//���أ�Position����λ�á�
//-----------------------------------------------------------------------------
u32 __PushCharToFileDev(char *TempBuf,ptu32_t Target,s32 Res2,const char ch,s32 Position)
{
    if(Position >= CN_BUF_LENGTH)
    {
        fwrite(TempBuf, CN_BUF_LENGTH, 1, (FILE*)Target);
        Position = 1;
        *TempBuf = ch;
    }
    else
    {
        *(TempBuf + Position) = ch;
        Position ++;
    }
    return Position;
}
#pragma GCC diagnostic pop

//---------------------------------------------------------------------------
//����: ���ո�ʽ�ַ�������������ַ�����������Method����ָ���ķ�ʽ������ַ�����
//����: TempBuf���ַ���ת��ʹ�õ���ʱ��������������CN_BUF_LENGTH�����
//          Method==PRINT_TO_STRING����Ա��������ɴ���NULL
//      Target������һ������;�ı������趨�ַ������Ŀ�ĵص����ԣ�����Method
//          ��ͬ���京�岻ͬ��
//          Method==PRINT_TO_STRING��Target���ַ���ָ��
//          Method==PRINT_TO_DIRECT��Target������
//          Method==PRINT_TO_FILE_OR_DEV��Target���ļ����豸ָ��
//      Size�����Target���ַ���ָ�룬Size�޶��䳤�ȣ�-1��ʾ���޶�����
//      Method�����������PRINT_TO_STRING�ȳ���֮һ
//      fmt����ʽ�ַ���
//      args���ɱ�����Ĳ����б�
//����: ת������ַ�������
//˵����Ŀǰû�м��븡���ӡ--TODO
//-----------------------------------------------------------------------------
static s32 __vsnprintf(char *TempBuf,ptu32_t Target, s32 Size,
                       u32 Method, const char *fmt,va_list args)
{
    u32 (*PushChar)(char *TempBuf,ptu32_t Target, s32 Size, char ch,s32 position);
    u64 num;
    s32 position;
    s32 base;
    s32 flags;          //��ʾ����ʱ�ı�־

    s32 field_width;    //��ʾ����С���
    s32 precision;      //��ʾ���ȣ������ֵ����λ�����ַ�������󳤶ȣ��������ֽص���
    s32 qualifier;      //�������ֵĳ���
                        // 'h', 'l', or 'L' for integer fields
                        // 'z' support added 23/7/1999 S.H.
                        // 'z' changed to 'Z' --davidm 1/25/99
                        // 't' added for ptrdiff_t
    position = 0;
    if(DJY_IsMultiEventStarted() == false)//������Ȳ�δ��ʼ,����ֱ�ӷ��ͷ�ʽ
    {
        if (Method == PRINT_TO_FILE_OR_DEV)
            Method = PRINT_TO_DIRECT;
    }
    switch (Method)
    {
        case PRINT_TO_STRING:
            PushChar = __PushCharToString;
            break;
        case PRINT_TO_DIRECT:
            PushChar = __PushCharDirect;
            break;
        case PRINT_TO_FILE_OR_DEV:
            PushChar = __PushCharToFileDev;
            break;
        default:
            PushChar = __PushCharDirect;
            break;
    }

    for (; *fmt ; ++fmt)
    {
        if (*fmt != '%') {
            position = PushChar(TempBuf,Target, Size,*fmt,position);
//            position = ADDDCH_With_push(Target, *fmt, position,Method);
            continue;
        }

        /* process flags */
        flags = 0;
repeat:
            ++fmt;        /* this also skips first '%' */
            switch (*fmt)
            {
            case '-':
                flags |= LEFT;
                goto repeat;
            case '+':
                flags |= PLUS;
                goto repeat;
            case ' ':
                flags |= SPACE;
                goto repeat;
            case '#':
                flags |= SPECIAL;
                goto repeat;
            case '0':
                flags |= ZEROPAD;
                goto repeat;
            }

        /* get field width */
        field_width = -1;
        if (isdigit((s32)*fmt))
            field_width = skip_atoi(&fmt);
        else if (*fmt == '*')
        {
            ++fmt;
            /* it's the next argument */
            field_width = va_arg(args, s32);
            if (field_width < 0) {
                field_width = -field_width;
                flags |= LEFT;
            }
        }

        /* get the precision */
        precision = -1;
        if (*fmt == '.')
        {
            ++fmt;
            if (isdigit((s32)*fmt))
                precision = skip_atoi(&fmt);
            else if (*fmt == '*') {
                ++fmt;
                /* it's the next argument */
                precision = va_arg(args, s32);
            }
            if (precision < 0)
                precision = 0;
        }

        /* get the conversion qualifier */
        qualifier = -1;
        if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L' ||
            *fmt == 'Z' || *fmt == 'z' || *fmt == 't')
        {
            qualifier = *fmt;
            ++fmt;
            if (qualifier == 'l' && *fmt == 'l') {
                qualifier = 'L';
                ++fmt;
            }
        }

        /* default base */
        base = 10;

        switch (*fmt)
        {
        case 'c':
            if (!(flags & LEFT))
            {
                while (--field_width > 0)
                    position = PushChar(TempBuf,Target, Size, ' ',position);
//                  position = ADDDCH_With_push(Target, ' ', position, Method);
            }
            position = PushChar(TempBuf,Target, Size, (unsigned char) va_arg(args, s32),position);
//          position = ADDDCH_With_push(Target, (unsigned char) va_arg(args, s32),position, Method);
            while (--field_width > 0)
                position = PushChar(TempBuf,Target, Size, ' ',position);
//              position = ADDDCH_With_push(Target, ' ', position, Method);
            continue;

        case 's':
            position = string(TempBuf,Target, Size, va_arg(args, char *),
                     field_width, precision, flags, position, PushChar);
            continue;

        case 'p':
            position = pointer(TempBuf,Target, Size,va_arg(args, void *),
                    field_width, precision, flags, position, PushChar);
            /* Skip all alphanumeric pointer suffixes */
            while (isalnum((s32)fmt[1]))
                fmt++;
            continue;

        case 'n':
            if (qualifier == 'l')
            {
                s32 *ip = va_arg(args, s32 *);
                *ip = position;
            } else {
                s32 *ip = va_arg(args, s32 *);
                *ip = position;
            }
            continue;

        case '%':
            position = PushChar(TempBuf,Target, Size,'%',position);
//          position = ADDDCH_With_push(Target, '%', position, Method);
            continue;

        /* integer number formats - set up the flags and "break" */
        case 'o':
            base = 8;
            break;

        case 'x':
            flags |= SMALL;
            __attribute__((fallthrough));
        case 'X':
            base = 16;
            break;

        case 'd':
        case 'i':
            flags |= SIGN;
        case 'u':
            break;

        case 'E':
        case 'G':
        case 'e':
        case 'f':
        case 'g':
#if(CFG_STDIO_FLOAT_PRINT == true)
            position = flt(TempBuf,Target, Size, va_arg(args, double), field_width, precision,
                             *fmt, flags | SIGN, position, PushChar);
#else
            va_arg(args, double);   //������һ������
            //��֧�ָ����ӡʱ����Ƿ���������־"NaN"
            position = PushChar(TempBuf,Target, Size,'N',position);
            position = PushChar(TempBuf,Target, Size,'a',position);
            position = PushChar(TempBuf,Target, Size,'N',position);
#endif
          continue;

        default:
            position = PushChar(TempBuf,Target, Size,'%',position);
//          position = ADDDCH_With_push(Target, '%', position, Method);
            if (*fmt)
                position = PushChar(TempBuf,Target, Size,*fmt,position);
//              position = ADDDCH_With_push(Target, *fmt, position, Method);
            else
                --fmt;
            continue;
        }
        if (qualifier == 'L')  /* "quad" for 64 bit variables */
            num = va_arg(args, u64);
        else if (qualifier == 'l')
        {
            num = va_arg(args, u32);
            if (flags & SIGN)
                num = (s32) num;
        } else if (qualifier == 'Z' || qualifier == 'z')
        {
            num = va_arg(args, s32);
        } else if (qualifier == 't')
        {
            num = va_arg(args, s32);
        } else if (qualifier == 'h')
        {
            num = (u16) va_arg(args, s32);
            if (flags & SIGN)
                num = (s16) num;
        } else
        {
            num = va_arg(args, u32);
            if (flags & SIGN)
                num = (s32) num;
        }
        position = number(TempBuf,Target, Size, num, base, field_width, precision,
                 flags, position, PushChar);
    }

    if (position != 0)
    {
        switch (Method)
        {
            case PRINT_TO_STRING:
                *(char*)(Target + position) = '\0';
                break;
            case PRINT_TO_DIRECT:
                if (PutStrDirect != NULL)
                    PutStrDirect((const char *)TempBuf,position);
                break;
            case PRINT_TO_FILE_OR_DEV:
                fwrite((const void *)TempBuf,position,1,(FILE*)Target);
                break;
            default:
                if (PutStrDirect != NULL)
                    PutStrDirect((const char *)TempBuf,position);
                break;
        }
    }
    return  position;
}
s32 vsprintf(char *buf, const char *fmt, va_list args)
{
    return __vsnprintf(NULL,(ptu32_t)buf, CN_LIMIT_SINT32,PRINT_TO_STRING,fmt, args);
}
s32 vsnprintf(char *buf, size_t n, const char *fmt, va_list args)
{

    return __vsnprintf(NULL,(ptu32_t)buf, n,PRINT_TO_STRING,fmt, args);
}

s32 vprintf (const char *fmt, va_list args)
{
    s32 i;
    char TempBuf[CN_BUF_LENGTH];

    // if (stdout == StdNotInit)
    if(!File_IsValid(stdout))
        i = __vsnprintf (TempBuf, (ptu32_t)NULL, 0, PRINT_TO_DIRECT, fmt, args);
    else
        i =  __vsnprintf (TempBuf, (ptu32_t)stdout, 0, PRINT_TO_FILE_OR_DEV, fmt, args);
    return i;
}

//----��ʽ��ӡ-----------------------------------------------------------------
//����: ����ʽ��ӡ�ַ�������C�⺯�����ƣ���ʹ�ö�̬���䣬һ�δ�ӡ�ĳ�������Ϊ255
//      �����ֽ��ַ������Ч���ֽ��ַ���
//����: const char *fmt, ...
//����: ת�����ַ�������
//˵�����ô�ӡ���������ն��豸��÷��񣬽�Ϊ���ӣ���ӡ�й������롣��˴������
//      �׶���ʹ�øô�ӡ������лл����������������£�ϣ��ֱ���������ʹ��
//      djy_printk������
//-----------------------------------------------------------------------------
s32 printf(const char *fmt, ...)
{
    va_list args;
    u32  i;
    char TempBuf[CN_BUF_LENGTH];

    va_start (args, fmt);

    // if (stdout == StdNotInit)
    if(!File_IsValid(stdout))
        i = __vsnprintf (TempBuf,(ptu32_t)NULL,0,PRINT_TO_DIRECT, fmt, args);
    else
        i =  __vsnprintf (TempBuf,(ptu32_t)stdout, 0,PRINT_TO_FILE_OR_DEV, fmt, args);
    va_end (args);

    return i;
}

//----��ʽ���ַ���--------------------------------------------------------------
//����: ��ʽ���ַ�������C��һ��������һ���ĸ�ʽ����ʽ���ַ���
//����: const char *fmt, ...
//����: ת�����ַ�������
//˵����
//-----------------------------------------------------------------------------
s32 sprintf(char *buf, const char *fmt, ...)
{
    va_list args;
    u32  i;

    va_start (args, fmt);

    i = __vsnprintf (NULL,(ptu32_t)buf, CN_LIMIT_SINT32, PRINT_TO_STRING, fmt, args);
    va_end (args);
    return i;
}
s32 snprintf(char *buf,size_t n, const char *fmt, ...)
{
    va_list args;
    u32  i;

    va_start (args, fmt);
    i = __vsnprintf (NULL,(ptu32_t)buf, n, PRINT_TO_STRING, fmt, args);
    va_end (args);
    return i;
}

//----��ʽ���ַ���--------------------------------------------------------------
//����: ��ʽ���ַ�������C��һ��������һ���ĸ�ʽ����ʽ���ַ���
//����: const char *fmt, ...
//����: д����ַ�������
//˵����
//-----------------------------------------------------------------------------
s32 fprintf(FILE *fp, const char *fmt, ...)
{
    va_list args;
    u32  i;
    char TempBuf[CN_BUF_LENGTH];
    if (fp == NULL)
    {
        DJY_SaveLastError(EN_FS_READFILE_EOF);
        return -1;
    }
    va_start (args, fmt);
    // if (fp == StdNotInit)
    if(!File_IsValid(fp))
        i = __vsnprintf (TempBuf,(ptu32_t)NULL, 0, PRINT_TO_DIRECT, fmt, args);
    else
        i = __vsnprintf (TempBuf,(ptu32_t)fp, 0, PRINT_TO_FILE_OR_DEV, fmt, args);
    va_end (args);

    return i;
}
//----��ʽ��ӡ-----------------------------------------------------------------
//����: ����ʽ��ӡ�ַ�������C�⺯�����ƣ���ʹ�ö�̬���䣬һ�δ�ӡ�ĳ�������Ϊ
//      cn_buf_length�����ֽ��ַ������Ч���ֽ��ַ���
//����: const char *fmt, ...
//����: ת�����ַ�������
//˵������printf��һ������ֱ��������ն��豸�ϡ�
//-----------------------------------------------------------------------------
s32 printk (const char *fmt, ...)
{
    va_list args;
    u32  i;
    char TempBuf[CN_BUF_LENGTH];
    va_start (args, fmt);

    i = __vsnprintf (TempBuf, (ptu32_t)NULL, 0, PRINT_TO_DIRECT, fmt, args);
    va_end (args);

    return i;
}


//����������������ӵĻ������п��ܻ���벻����Ŀǰ��֪�������÷����ϲ�������֧ʱ����ע��
int sniprintf(char *buf, size_t buf_size, const char *fmt, ...)
{
    int result;
    va_list ap;
    va_start(ap, fmt);
    char* pnew = (char*)malloc(strlen(fmt) + 1);
    if (pnew == 0) return 0;
    strcpy(pnew, fmt);
    char *p = pnew;
    while (*p) {
        if (*p == '%' && *p != 0 && *(p + 1) == '.') {
            p++;
            *p = '0';
        }
        p++;
    }
    result = c_vsnprintf(buf, buf_size, pnew, ap);
    va_end(ap);
    if (pnew) free(pnew);
    return result;
}


int siscanf(const char *ibuf, const char *fmt, ...)
{
    va_list ap;
    int ret;
    va_start(ap, fmt);
    ret = vsscanf(ibuf, fmt, ap);
    va_end(ap);
    return(ret);
}

