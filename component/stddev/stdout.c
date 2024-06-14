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
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
//所属模块：标准IO模块
//作者:  张前福.
//版本：V1.0.0
//文件描述: 标准输出函数
//其他说明:
//修订历史:
//2. 日期：2015-10-16
//   作者:  罗侍田.
//   新版本号: V1.1.0
//   修改说明: 全面修订，跟标准IO设计匹配
//1. 日期: 2013-11-06
//   作者:  张前福.
//   新版本号: V1.0.0
//   修改说明: 创建文件
//------------------------------------------------------
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <errno.h>
#include <systime.h>
#include <device.h>
#include <djyos.h>
#include <djyfs/file.h>
#include "component_config_stdio.h"

typedef unsigned long int u_quad_t;
typedef long int quad_t;
#define UQUAD_MAX   ((u_quad_t)0-1) /* max value for a uquad_t */
                    /* max value for a quad_t */
#define QUAD_MAX    ((quad_t)(UQUAD_MAX >> 1))
#define QUAD_MIN    (-QUAD_MAX-1)   /* min value for a quad_t */
#define BUF     32  /* Maximum length of numeric string. */

/*
 * Flags used during conversion.
 */
#define LONG        0x01    /* l: long or double */
#define SHORT       0x04    /* h: short */
#define SUPPRESS    0x08    /* *: suppress assignment */
#define POINTER     0x10    /* p: void * (as hex) */
#define NOSKIP      0x20    /* [ or c: do not skip blanks */
#define LONGLONG    0x400   /* ll: long long (+ deprecated q: quad) */
#define SHORTSHORT  0x4000  /* hh: char */
#define UNSIGNED    0x8000  /* %[oupxX] conversions */

/*
 * Conversion types.
 */
#define CT_CHAR     0   /* %c conversion */
#define CT_CCL      1   /* %[...] conversion */
#define CT_STRING   2   /* %s conversion */
#define CT_INT      3   /* %[dioupxX] conversion */

/*
 * The following are used in numeric conversions only:
 * SIGNOK, NDIGITS, DPTOK, and EXPOK are for floating point;
 * SIGNOK, NDIGITS, PFXOK, and NZDIGITS are for integral.
 */
#define SIGNOK      0x40    /* +/- is (still) legal */
#define NDIGITS     0x80    /* no digits detected */

#define DPTOK       0x100   /* (float) decimal point is still legal */
#define EXPOK       0x200   /* (float) exponent (e+3, etc) still legal */

#define PFXOK       0x100   /* 0x prefix is (still) legal */
#define NZDIGITS    0x200   /* no zero digits detected */

#define C_SNPRINTF_FLAG_ZERO 1

#define C_SNPRINTF_APPEND_CHAR(ch)       \
  do {                                   \
    if (i < (int) buf_size) buf[i] = ch; \
    i++;                                 \
  } while (0)

#define CN_BUF_LENGTH   0x40

#define ZEROPAD     1           /* pad with zero */
#define SIGN        2           /* unsigned/s32 */
#define PLUS        4           /* show plus */
#define SPACE       8           /* space if plus */
#define LEFT        16          /* left justified */
#define SMALL       32          /* Must be 32 == 0x20 */
#define SPECIAL     64          /* 0x */

#define PRINT_TO_STRING         0   //输出到string，用于sprintf等。
#define PRINT_TO_DIRECT         1   //直接输出到原始硬件，用于printk等。
#define PRINT_TO_FILE           2   //输出到文件，例如stdout/stderr，file
s32 skip_atoi(const char **s);


// ============================================================================
// 功能：测试是否是有效的的文件流；
// 参数：stream -- 文件流；
// 返回：是（1）；不是（0）；
// 备注：
// ============================================================================
extern s32 File_IsValid(FILE* stream);

//----输出一个字符到stdout-----------------------------------------------------
//功能：输出一个字符到stdout，stdout可以是设备，也可以是文件
//参数：ch ，待输出的字符
//返回：正确输出则返回被输出的字符，错误则返回EOF
//-----------------------------------------------------------------------------
s32 putchar (s32 ch)
{
#if(CFG_STDIO_STDIOFILE == true)
    if(File_IsValid(stdout))
    {
        return putc(ch,stdout);
    }
    else
    {
        return PutStrDirect(&ch,1);
    }
#else       // for #if(CFG_STDIO_STDIOFILE == true)
    return PutStrDirect(&ch,1);
#endif      // for #if(CFG_STDIO_STDIOFILE == true)
}

//----输出一个字符串到stdout-----------------------------------------------------
//功能：输出一个字符串到stdout，stdout可以是设备，也可以是文件
//参数：str ，待输出的字符串
//返回：正确输出则返回一个非负值，错误则返回EOF
//-----------------------------------------------------------------------------
s32 puts (const char *str)
{
#if(CFG_STDIO_STDIOFILE == true)
    if(File_IsValid(stdout))
    {
        return fputs(str,stdout);
    }
    else
    {
        return PutStrDirect(str,strlen(str));
    }
#else       // for #if(CFG_STDIO_STDIOFILE == true)
    return PutStrDirect(str,strlen(str));
#endif      // for #if(CFG_STDIO_STDIOFILE == true)
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

//----把字符压入字符串中-------------------------------------------------------
//功能：把一个字符推入字符串中，若超出长度，则忽略。
//参数：TempBuf，为函数原型一致而填充的参数，无意义
//      Target,接收字符串的缓冲区地址。
//      Size，缓冲区尺寸，含末端'\0'
//      ch，压入的字符
//      Position，Buf的偏移量
//返回：Position的新位置。
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

//----把字缓冲后直接输出------------------------------------------------------
//功能：把一个字符先缓冲到字符串TempBuf中，缓冲满了后，直接输出到硬件，例如串口。
//      为 printk 函数准备的。
//参数：TempBuf，由调用者提供的缓冲区，长度是CN_BUF_LENGTH，目的是减少写串口的次数，
//          提高执行效率。
//      Target,无意义。
//      Size，无意义
//      ch，压入的字符
//      Position，Buf的偏移量
//返回：Position的新位置。
//-----------------------------------------------------------------------------
u32 __PushCharDirect(char *TempBuf,ptu32_t Target,s32 Size,const char ch,s32 Position)
{
    if(Position < CN_BUF_LENGTH-1)
    {
        *(TempBuf + Position) = ch;
        Position ++;
    }
    if(Position >= CN_BUF_LENGTH-1)
    {
//      TempBuf[CN_BUF_LENGTH -1] = '\0';
        PutStrDirect(TempBuf,CN_BUF_LENGTH-1);
        Position = 0;
    }
    return Position;
}

//----把字符推送到文件中--------------------------------------------------
//功能：先缓冲到字符串TempBuf中，缓冲满了后，输出到文件，以减少IO次数，本函数用于
//      printf/fprintf/vprintf/vfprintf等。
//参数：TempBuf，由调用者提供的缓冲区，长度 CN_BUF_LENGTH，目的是减少写文件的次数，
//          提高执行效率。
//      Target,接收字符串的文件指针。
//      Res，无意义
//      ch，压入的字符
//      Position，TempBuf的当前存储位置
//返回：Position的新位置。
//-----------------------------------------------------------------------------
u32 __PushCharToFile(char *TempBuf,ptu32_t Target,s32 Res,const char ch,s32 Position)
{
    if(Position < CN_BUF_LENGTH-1)
    {
        *(TempBuf + Position) = ch;
        Position ++;
    }

    if(Position >= CN_BUF_LENGTH-1)
    {
        TempBuf[CN_BUF_LENGTH -1] = '\0';
#if(CFG_MODULE_ENABLE_FILE_SYSTEM == true)

#if(CFG_STDIO_STDIOFILE == true)
        puts(TempBuf);
#else       // for #if(CFG_STDIO_STDIOFILE == true)
        fwrite(TempBuf, CN_BUF_LENGTH, 1, (FILE*)Target);
#endif      // for #if(CFG_STDIO_STDIOFILE == true)

#endif      // for #if(CFG_MODULE_ENABLE_FILE_SYSTEM == true)
        Position = 0;
    }
    return Position;
}

#pragma GCC diagnostic pop

//---------------------------------------------------------------------------
//功能: 按照格式字符串，生成输出字符串，并按照Method参数指定的方式，输出字符串。
//参数: TempBuf，由调用者提供的字符串转换使用的临时缓冲区，长度是CN_BUF_LENGTH，如果
//          Method==PRINT_TO_STRING则忽略本参数，可传入NULL
//      Target，这是一个多用途的变量，设定字符串输出目的地的属性，依据Method
//          不同，其含义不同。
//          Method==PRINT_TO_STRING，Target是字符串指针
//          Method==PRINT_TO_DIRECT，Target无意义
//          Method==PRINT_TO_FILE_OR_DEV，Target是文件或设备指针
//      Size，如果Target是字符串指针，Size限定其长度，-1表示不限定长度
//      Method，输出方法，PRINT_TO_STRING等常量之一
//      fmt，格式字符串
//      args，可变参数的参数列表
//返回: 转换结果字符串长度
//-----------------------------------------------------------------------------
static s32 __vsnprintf(char *TempBuf,ptu32_t Target, s32 Size,
                       u32 Method, const char *fmt,va_list args)
{
    u32 (*PushChar)(char *TempBuf,ptu32_t Target, s32 Size, char ch,s32 position);
    u64 num;
    s32 position;
    s32 base;
    s32 flags;          //显示数字时的标志

    s32 field_width;    //显示的最小宽度
    s32 precision;      //显示精度，即数字的最大位数和字符串的最大长度，超出部分截掉。
    s32 qualifier;      //控制数字的长度
                        // 'h', 'l', or 'L' for integer fields
                        // 'z' support added 23/7/1999 S.H.
                        // 'z' changed to 'Z' --davidm 1/25/99
                        // 't' added for ptrdiff_t
    position = 0;
//  if(DJY_IsMultiEventStarted() == false)//如果调度并未开始,采用直接发送方式
//  {
//      if (Method == PRINT_TO_FILE)
//          Method = PRINT_TO_DIRECT;
//  }
    switch (Method)
    {
        case PRINT_TO_STRING:
            PushChar = __PushCharToString;
            break;
        case PRINT_TO_DIRECT:
            PushChar = __PushCharDirect;
            break;
        case PRINT_TO_FILE:
            PushChar = __PushCharToFile;
            break;
        default:
            PushChar = __PushCharToFile;
//          PushChar = __PushCharDirect;
            break;
    }

    for (; *fmt ; ++fmt)
    {
        if (*fmt != '%') {
            position = PushChar(TempBuf,Target, Size,*fmt,position);
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
            }
            position = PushChar(TempBuf,Target, Size, (unsigned char) va_arg(args, s32),position);
            while (--field_width > 0)
                position = PushChar(TempBuf,Target, Size, ' ',position);
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
            va_arg(args, double);   //跳至下一个参数
            //不支持浮点打印时输出非法浮点数标志"NaN"
            position = PushChar(TempBuf,Target, Size,'N',position);
            position = PushChar(TempBuf,Target, Size,'a',position);
            position = PushChar(TempBuf,Target, Size,'N',position);
#endif
          continue;

        default:
            position = PushChar(TempBuf,Target, Size,'%',position);
            if (*fmt)
                position = PushChar(TempBuf,Target, Size,*fmt,position);
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

    if (position != 0)      //说明 tempbuf 中有数据未输出
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
            case PRINT_TO_FILE:
                TempBuf[position] = '\0';
                puts(TempBuf);
//              fwrite((const void *)TempBuf,position,1,(FILE*)Target);
                break;
            default:
//              if (PutStrDirect != NULL)
//                  PutStrDirect((const char *)TempBuf,position);
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

//  if(!File_IsValid(stdout))
//      i = __vsnprintf (TempBuf, (ptu32_t)NULL, 0, PRINT_TO_DIRECT, fmt, args);
//  else
    i =  __vsnprintf (TempBuf, (ptu32_t)stdout, 0, PRINT_TO_FILE, fmt, args);
    return i;
}

//----格式打印-----------------------------------------------------------------
//功能: 按格式打印字符串，与C库函数类似，不使用动态分配，一次打印的长度限制为255
//      个单字节字符，或等效多字节字符。
//参数: const char *fmt, ...
//返回: 转换的字符串长度
//说明：该打印函数经过终端设备获得服务，较为复杂，打印有规律整齐。因此代码成熟
//      阶段请使用该打印函数，谢谢合作，在特殊情况下，希望直接输出，可使用
//      djy_printk函数。
//-----------------------------------------------------------------------------
s32 printf(const char *fmt, ...)
{
    va_list args;
    u32  i;
    char TempBuf[CN_BUF_LENGTH];

    va_start (args, fmt);

//  if(!File_IsValid(stdout))
//      i = __vsnprintf (TempBuf,(ptu32_t)NULL,0,PRINT_TO_DIRECT, fmt, args);
//  else
    i =  __vsnprintf (TempBuf,(ptu32_t)stdout, 0,PRINT_TO_FILE, fmt, args);
    va_end (args);

    return i;
}

//----格式化字符串--------------------------------------------------------------
//功能: 格式化字符串，和C库一样，按照一定的格式来格式化字符串
//参数: const char *fmt, ...
//返回: 转换的字符串长度
//说明：
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

//----格式化字符串--------------------------------------------------------------
//功能: 格式化字符串，和C库一样，按照一定的格式来格式化字符串
//参数: const char *fmt, ...
//返回: 写入的字符串长度
//说明：
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
//  if(!File_IsValid(fp))
//      i = __vsnprintf (TempBuf,(ptu32_t)NULL, 0, PRINT_TO_DIRECT, fmt, args);
//  else
    i = __vsnprintf (TempBuf,(ptu32_t)fp, 0, PRINT_TO_FILE, fmt, args);
    va_end (args);

    return i;
}
//----格式打印-----------------------------------------------------------------
//功能: 按格式打印字符串，与C库函数类似，不使用动态分配，一次打印的长度限制为
//      cn_buf_length个单字节字符，或等效多字节字符。
//参数: const char *fmt, ...
//返回: 转换的字符串长度
//说明：和printf不一样，它直接输出到终端设备上。
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

static int c_itoa(char *buf, size_t buf_size, int64_t num, int base, int flags,
                  int field_width) {
  char tmp[40];
  int i = 0, k = 0, neg = 0;

  if (num < 0) {
    neg++;
    num = -num;
  }

  /* Print into temporary buffer - in reverse order */
  do {
    int rem = num % base;
    if (rem < 10) {
      tmp[k++] = '0' + rem;
    } else {
      tmp[k++] = 'a' + (rem - 10);
    }
    num /= base;
  } while (num > 0);

  /* Zero padding */
  if (flags && C_SNPRINTF_FLAG_ZERO) {
    while (k < field_width && k < (int) sizeof(tmp) - 1) {
      tmp[k++] = '0';
    }
  }

  /* And sign */
  if (neg) {
    tmp[k++] = '-';
  }

  /* Now output */
  while (--k >= 0) {
    C_SNPRINTF_APPEND_CHAR(tmp[k]);
  }

  return i;
}

int c_vsnprintf(char *buf, size_t buf_size, const char *fmt, va_list ap)
{
  int ch, i = 0, len_mod, flags, precision, field_width;

  while ((ch = *fmt++) != '\0') {
    if (ch != '%') {
      C_SNPRINTF_APPEND_CHAR(ch);
    } else {
      /*
       * Conversion specification:
       *   zero or more flags (one of: # 0 - <space> + ')
       *   an optional minimum  field  width (digits)
       *   an  optional precision (. followed by digits, or *)
       *   an optional length modifier (one of: hh h l ll L q j z t)
       *   conversion specifier (one of: d i o u x X e E f F g G a A c s p n)
       */
      flags = field_width = precision = len_mod = 0;

      /* Flags. only zero-pad flag is supported. */
      if (*fmt == '0') {
        flags |= C_SNPRINTF_FLAG_ZERO;
      }

      /* Field width */
      while (*fmt >= '0' && *fmt <= '9') {
        field_width *= 10;
        field_width += *fmt++ - '0';
      }
      /* Dynamic field width */
      if (*fmt == '*') {
        field_width = va_arg(ap, int);
        fmt++;
      }

      /* Precision */
      if (*fmt == '.') {
        fmt++;
        if (*fmt == '*') {
          precision = va_arg(ap, int);
          fmt++;
        } else {
          while (*fmt >= '0' && *fmt <= '9') {
            precision *= 10;
            precision += *fmt++ - '0';
          }
        }
      }

      /* Length modifier */
      switch (*fmt) {
        case 'h':
        case 'l':
        case 'L':
        case 'I':
        case 'q':
        case 'j':
        case 'z':
        case 't':
          len_mod = *fmt++;
          if (*fmt == 'h') {
            len_mod = 'H';
            fmt++;
          }
          if (*fmt == 'l') {
            len_mod = 'q';
            fmt++;
          }
          break;
      }

      ch = *fmt++;
      if (ch == 's') {
        const char *s = va_arg(ap, const char *); /* Always fetch parameter */
        int j;
        int pad = field_width - (precision >= 0 ? strnlen(s, precision) : 0);
        for (j = 0; j < pad; j++) {
          C_SNPRINTF_APPEND_CHAR(' ');
        }

        /* `s` may be NULL in case of %.*s */
        if (s != NULL) {
          /* Ignore negative and 0 precisions */
          for (j = 0; (precision <= 0 || j < precision) && s[j] != '\0'; j++) {
            C_SNPRINTF_APPEND_CHAR(s[j]);
          }
        }
      } else if (ch == 'c') {
        ch = va_arg(ap, int); /* Always fetch parameter */
        C_SNPRINTF_APPEND_CHAR(ch);
      } else if (ch == 'd' && len_mod == 0) {
        i += c_itoa(buf + i, buf_size - i, va_arg(ap, int), 10, flags,
                    field_width);
      } else if (ch == 'd' && len_mod == 'l') {
        i += c_itoa(buf + i, buf_size - i, va_arg(ap, long), 10, flags,
                    field_width);
#ifdef SSIZE_MAX
      } else if (ch == 'd' && len_mod == 'z') {
        i += c_itoa(buf + i, buf_size - i, va_arg(ap, ssize_t), 10, flags,
                    field_width);
#endif
      } else if (ch == 'd' && len_mod == 'q') {
        i += c_itoa(buf + i, buf_size - i, va_arg(ap, int64_t), 10, flags,
                    field_width);
      } else if ((ch == 'x' || ch == 'u') && len_mod == 0) {
        i += c_itoa(buf + i, buf_size - i, va_arg(ap, unsigned),
                    ch == 'x' ? 16 : 10, flags, field_width);
      } else if ((ch == 'x' || ch == 'u') && len_mod == 'l') {
        i += c_itoa(buf + i, buf_size - i, va_arg(ap, unsigned long),
                    ch == 'x' ? 16 : 10, flags, field_width);
      } else if ((ch == 'x' || ch == 'u') && len_mod == 'z') {
        i += c_itoa(buf + i, buf_size - i, va_arg(ap, size_t),
                    ch == 'x' ? 16 : 10, flags, field_width);
      } else if (ch == 'p') {
        unsigned long num = (unsigned long) (uintptr_t) va_arg(ap, void *);
        C_SNPRINTF_APPEND_CHAR('0');
        C_SNPRINTF_APPEND_CHAR('x');
        i += c_itoa(buf + i, buf_size - i, num, 16, flags, 0);
      } else {
#ifndef NO_LIBC
        /*
         * TODO(lsm): abort is not nice in a library, remove it
         * Also, ESP8266 SDK doesn't have it
         */
        // abort();
#endif
      }
    }
  }

  /* Zero-terminate the result */
  if (buf_size > 0) {
    buf[i < (int) buf_size ? i : (int) buf_size - 1] = '\0';
  }

  return i;
}

//下面两个函数不添加的话工程有可能会编译不过，目前不知道具体用法，合并到主分支时加上注释
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

static const u_char * __sccl(char *tab, const u_char *fmt)
{
    int c, n, v;

    /* first `clear' the whole table */
    c = *fmt++;     /* first char hat => negated scanset */
    if (c == '^') {
        v = 1;      /* default => accept */
        c = *fmt++; /* get new first char */
    } else
        v = 0;      /* default => reject */

    /* XXX: Will not work if sizeof(tab*) > sizeof(char) */
    (void) memset(tab, v, 256);

    if (c == 0)
        return (fmt - 1);/* format ended before closing ] */

    /*
     * Now set the entries corresponding to the actual scanset
     * to the opposite of the above.
     *
     * The first character may be ']' (or '-') without being special;
     * the last character may be '-'.
     */
    v = 1 - v;
    for (;;) {
        tab[c] = v;     /* take character c */
doswitch:
        n = *fmt++;     /* and examine the next */
        switch (n) {

        case 0:         /* format ended too soon */
            return (fmt - 1);

        case '-':
            /*
             * A scanset of the form
             *  [01+-]
             * is defined as `the digit 0, the digit 1,
             * the character +, the character -', but
             * the effect of a scanset such as
             *  [a-zA-Z0-9]
             * is implementation defined.  The V7 Unix
             * scanf treats `a-z' as `the letters a through
             * z', but treats `a-a' as `the letter a, the
             * character -, and the letter a'.
             *
             * For compatibility, the `-' is not considerd
             * to define a range if the character following
             * it is either a close bracket (required by ANSI)
             * or is not numerically greater than the character
             * we just stored in the table (c).
             */
            n = *fmt;
            if (n == ']' || n < c) {
                c = '-';
                break;  /* resume the for(;;) */
            }
            fmt++;
            /* fill in the range */
            do {
                tab[++c] = v;
            } while (c < n);
            c = n;
            /*
             * Alas, the V7 Unix scanf also treats formats
             * such as [a-c-e] as `the letters a through e'.
             * This too is permitted by the standard....
             */
            goto doswitch;
            break;

        case ']':       /* end of scanset */
            return (fmt);

        default:        /* just another character */
            c = n;
            break;
        }
    }
    /* NOTREACHED */
}

quad_t strtoq(const char *nptr, char **endptr, int base)
{
    register const char *s;
    register u_quad_t acc;
    register int c;
    register u_quad_t qbase, cutoff;
    register int neg, any, cutlim;

    /*
     * Skip white space and pick up leading +/- sign if any.
     * If base is 0, allow 0x for hex and 0 for octal, else
     * assume decimal; if base is already 16, allow 0x.
     */
    s = nptr;
    do {
        c = *s++;
    } while (isspace(c));
    if (c == '-') {
        neg = 1;
        c = *s++;
    } else {
        neg = 0;
        if (c == '+')
            c = *s++;
    }
    if ((base == 0 || base == 16) &&
        c == '0' && (*s == 'x' || *s == 'X')) {
        c = s[1];
        s += 2;
        base = 16;
    }
    if (base == 0)
        base = c == '0' ? 8 : 10;

    /*
     * Compute the cutoff value between legal numbers and illegal
     * numbers.  That is the largest legal value, divided by the
     * base.  An input number that is greater than this value, if
     * followed by a legal input character, is too big.  One that
     * is equal to this value may be valid or not; the limit
     * between valid and invalid numbers is then based on the last
     * digit.  For instance, if the range for quads is
     * [-9223372036854775808..9223372036854775807] and the input base
     * is 10, cutoff will be set to 922337203685477580 and cutlim to
     * either 7 (neg==0) or 8 (neg==1), meaning that if we have
     * accumulated a value > 922337203685477580, or equal but the
     * next digit is > 7 (or 8), the number is too big, and we will
     * return a range error.
     *
     * Set any if any `digits' consumed; make it negative to indicate
     * overflow.
     */
    qbase = (unsigned)base;
    cutoff = neg ? -(u_quad_t)QUAD_MIN : QUAD_MAX;
    cutlim = cutoff % qbase;
    cutoff /= qbase;
    for (acc = 0, any = 0;; c = *s++) {
        if (isdigit(c))
            c -= '0';
        else if (isalpha(c))
            c -= isupper(c) ? 'A' - 10 : 'a' - 10;
        else
            break;
        if (c >= base)
            break;
        if (any < 0 || acc > cutoff || acc == cutoff && c > cutlim)
            any = -1;
        else {
            any = 1;
            acc *= qbase;
            acc += c;
        }
    }
    if (any < 0) {
        acc = neg ? QUAD_MIN : QUAD_MAX;
        errno = ERANGE;
    } else if (neg)
        acc = -acc;
    if (endptr != 0)
        *endptr = (char *)(any ? s - 1 : nptr);
    return (acc);
}

unsigned long long strtoull(const char *str, char **endptr, int base)
{
    const char *s;
    unsigned long long acc, cutoff;
    int c;
    int neg, any, cutlim;

    /*
     * See strtoll for comments as to the logic used.
     */
    if (base < 0 || base == 1 || base > 36) {
        if (endptr != 0)
            *endptr = (char *)str;
        errno = EINVAL;
        return 0;
    }

    s = str;
    do {
        c = (unsigned char) *s++;
    } while (isspace(c));
    if (c == '-') {
        neg = 1;
        c = *s++;
    } else {
        neg = 0;
        if (c == '+')
            c = *s++;
    }
    if ((base == 0 || base == 16) &&
        c == '0' && (*s == 'x' || *s == 'X')) {
        c = s[1];
        s += 2;
        base = 16;
    }
    if (base == 0)
        base = c == '0' ? 8 : 10;

    cutoff = ULLONG_MAX / (unsigned long long)base;
    cutlim = ULLONG_MAX % (unsigned long long)base;
    for (acc = 0, any = 0;; c = (unsigned char) *s++) {
        if (isdigit(c))
            c -= '0';
        else if (isalpha(c))
            c -= isupper(c) ? 'A' - 10 : 'a' - 10;
        else
            break;
        if (c >= base)
            break;
        if (any < 0)
            continue;
        if (acc > cutoff || (acc == cutoff && c > cutlim)) {
            any = -1;
            acc = ULLONG_MAX;
            errno = ERANGE;
        } else {
            any = 1;
            acc *= (unsigned long long)base;
            acc += c;
        }
    }
    if (neg && any > 0)
        acc = -acc;
    if (endptr != 0)
        *endptr = (char *) (any ? s - 1 : str);
    return (acc);
}

unsigned long long strtouq(const char *nptr, char **endptr, int base)
{
    return strtoull(nptr, endptr, base);
}


int vsscanf(const char *inp, char const *fmt0, va_list ap)
{
    int inr;
    const u_char *fmt = (const u_char *)fmt0;
    int c;          /* character from format, or conversion */
    size_t width;       /* field width, or 0 */
    char *p;        /* points into all kinds of strings */
    int n;          /* handy integer */
    int flags;      /* flags as defined above */
    char *p0;       /* saves original value of p when necessary */
    int nassigned;      /* number of fields assigned */
    int nconversions;   /* number of conversions */
    int nread;      /* number of characters consumed from fp */
    int base;       /* base argument to conversion function */
    char ccltab[256];   /* character class table for %[...] */
    char buf[BUF];      /* buffer for numeric conversions */

    /* `basefix' is used to avoid `if' tests in the integer scanner */
    static short basefix[17] =
        { 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

    inr = strlen(inp);

    nassigned = 0;
    nconversions = 0;
    nread = 0;
    base = 0;       /* XXX just to keep gcc happy */
    for (;;) {
        c = *fmt++;
        if (c == 0)
            return (nassigned);
        if (isspace(c)) {
            while (inr > 0 && isspace(*inp))
                nread++, inr--, inp++;
            continue;
        }
        if (c != '%')
            goto literal;
        width = 0;
        flags = 0;
        /*
         * switch on the format.  continue if done;
         * break once format type is derived.
         */
again:      c = *fmt++;
        switch (c) {
        case '%':
literal:
            if (inr <= 0)
                goto input_failure;
            if (*inp != c)
                goto match_failure;
            inr--, inp++;
            nread++;
            continue;

        case '*':
            flags |= SUPPRESS;
            goto again;
        case 'l':
            if (flags & LONG) {
                flags &= ~LONG;
                flags |= LONGLONG;
            } else
                flags |= LONG;
            goto again;
        case 'q':
            flags |= LONGLONG;  /* not quite */
            goto again;
        case 'h':
            if (flags & SHORT) {
                flags &= ~SHORT;
                flags |= SHORTSHORT;
            } else
                flags |= SHORT;
            goto again;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            width = width * 10 + c - '0';
            goto again;

        /*
         * Conversions.
         */
        case 'd':
            c = CT_INT;
            base = 10;
            break;

        case 'i':
            c = CT_INT;
            base = 0;
            break;

        case 'o':
            c = CT_INT;
            flags |= UNSIGNED;
            base = 8;
            break;

        case 'u':
            c = CT_INT;
            flags |= UNSIGNED;
            base = 10;
            break;

        case 'X':
        case 'x':
            flags |= PFXOK; /* enable 0x prefixing */
            c = CT_INT;
            flags |= UNSIGNED;
            base = 16;
            break;

        case 's':
            c = CT_STRING;
            break;

        case '[':
            fmt = __sccl(ccltab, fmt);
            flags |= NOSKIP;
            c = CT_CCL;
            break;

        case 'c':
            flags |= NOSKIP;
            c = CT_CHAR;
            break;

        case 'p':   /* pointer format is like hex */
            flags |= POINTER | PFXOK;
            c = CT_INT;
            flags |= UNSIGNED;
            base = 16;
            break;

        case 'n':
            nconversions++;
            if (flags & SUPPRESS)   /* ??? */
                continue;
            if (flags & SHORTSHORT)
                *va_arg(ap, char *) = nread;
            else if (flags & SHORT)
                *va_arg(ap, short *) = nread;
            else if (flags & LONG)
                *va_arg(ap, long *) = nread;
            else if (flags & LONGLONG)
                *va_arg(ap, long long *) = nread;
            else
                *va_arg(ap, int *) = nread;
            continue;
        }

        /*
         * We have a conversion that requires input.
         */
        if (inr <= 0)
            goto input_failure;

        /*
         * Consume leading white space, except for formats
         * that suppress this.
         */
        if ((flags & NOSKIP) == 0) {
            while (isspace(*inp)) {
                nread++;
                if (--inr > 0)
                    inp++;
                else
                    goto input_failure;
            }
            /*
             * Note that there is at least one character in
             * the buffer, so conversions that do not set NOSKIP
             * can no longer result in an input failure.
             */
        }

        /*
         * Do the conversion.
         */
        switch (c) {

        case CT_CHAR:
            /* scan arbitrary characters (sets NOSKIP) */
            if (width == 0)
                width = 1;
            if (flags & SUPPRESS) {
                size_t sum = 0;
                for (;;) {
                    if ((n = inr) < (int)width) {
                        sum += n;
                        width -= n;
                        inp += n;
                        if (sum == 0)
                            goto input_failure;
                        break;
                    } else {
                        sum += width;
                        inr -= width;
                        inp += width;
                        break;
                    }
                }
                nread += sum;
            } else {
                bcopy(inp, va_arg(ap, char *), width);
                inr -= width;
                inp += width;
                nread += width;
                nassigned++;
            }
            nconversions++;
            break;

        case CT_CCL:
            /* scan a (nonempty) character class (sets NOSKIP) */
            if (width == 0)
                width = (size_t)~0; /* `infinity' */
            /* take only those things in the class */
            if (flags & SUPPRESS) {
                n = 0;
                while (ccltab[(unsigned char)*inp]) {
                    n++, inr--, inp++;
                    if (--width == 0)
                        break;
                    if (inr <= 0) {
                        if (n == 0)
                            goto input_failure;
                        break;
                    }
                }
                if (n == 0)
                    goto match_failure;
            } else {
                p0 = p = va_arg(ap, char *);
                while (ccltab[(unsigned char)*inp]) {
                    inr--;
                    *p++ = *inp++;
                    if (--width == 0)
                        break;
                    if (inr <= 0) {
                        if (p == p0)
                            goto input_failure;
                        break;
                    }
                }
                n = p - p0;
                if (n == 0)
                    goto match_failure;
                *p = 0;
                nassigned++;
            }
            nread += n;
            nconversions++;
            break;

        case CT_STRING:
            /* like CCL, but zero-length string OK, & no NOSKIP */
            if (width == 0)
                width = (size_t)~0;
            if (flags & SUPPRESS) {
                n = 0;
                while (!isspace(*inp)) {
                    n++, inr--, inp++;
                    if (--width == 0)
                        break;
                    if (inr <= 0)
                        break;
                }
                nread += n;
            } else {
                p0 = p = va_arg(ap, char *);
                while (!isspace(*inp)) {
                    inr--;
                    *p++ = *inp++;
                    if (--width == 0)
                        break;
                    if (inr <= 0)
                        break;
                }
                *p = 0;
                nread += p - p0;
                nassigned++;
            }
            nconversions++;
            continue;

        case CT_INT:
            /* scan an integer as if by the conversion function */
#ifdef hardway
            if (width == 0 || width > sizeof(buf) - 1)
                width = sizeof(buf) - 1;
#else
            /* size_t is unsigned, hence this optimisation */
            if (--width > sizeof(buf) - 2)
                width = sizeof(buf) - 2;
            width++;
#endif
            flags |= SIGNOK | NDIGITS | NZDIGITS;
            for (p = buf; width; width--) {
                c = *inp;
                /*
                 * Switch on the character; `goto ok'
                 * if we accept it as a part of number.
                 */
                switch (c) {

                /*
                 * The digit 0 is always legal, but is
                 * special.  For %i conversions, if no
                 * digits (zero or nonzero) have been
                 * scanned (only signs), we will have
                 * base==0.  In that case, we should set
                 * it to 8 and enable 0x prefixing.
                 * Also, if we have not scanned zero digits
                 * before this, do not turn off prefixing
                 * (someone else will turn it off if we
                 * have scanned any nonzero digits).
                 */
                case '0':
                    if (base == 0) {
                        base = 8;
                        flags |= PFXOK;
                    }
                    if (flags & NZDIGITS)
                        flags &= ~(SIGNOK|NZDIGITS|NDIGITS);
                    else
                        flags &= ~(SIGNOK|PFXOK|NDIGITS);
                    goto ok;

                /* 1 through 7 always legal */
                case '1': case '2': case '3':
                case '4': case '5': case '6': case '7':
                    base = basefix[base];
                    flags &= ~(SIGNOK | PFXOK | NDIGITS);
                    goto ok;

                /* digits 8 and 9 ok iff decimal or hex */
                case '8': case '9':
                    base = basefix[base];
                    if (base <= 8)
                        break;  /* not legal here */
                    flags &= ~(SIGNOK | PFXOK | NDIGITS);
                    goto ok;

                /* letters ok iff hex */
                case 'A': case 'B': case 'C':
                case 'D': case 'E': case 'F':
                case 'a': case 'b': case 'c':
                case 'd': case 'e': case 'f':
                    /* no need to fix base here */
                    if (base <= 10)
                        break;  /* not legal here */
                    flags &= ~(SIGNOK | PFXOK | NDIGITS);
                    goto ok;

                /* sign ok only as first character */
                case '+': case '-':
                    if (flags & SIGNOK) {
                        flags &= ~SIGNOK;
                        goto ok;
                    }
                    break;

                /* x ok iff flag still set & 2nd char */
                case 'x': case 'X':
                    if (flags & PFXOK && p == buf + 1) {
                        base = 16;  /* if %i */
                        flags &= ~PFXOK;
                        goto ok;
                    }
                    break;
                }

                /*
                 * If we got here, c is not a legal character
                 * for a number.  Stop accumulating digits.
                 */
                break;
        ok:
                /*
                 * c is legal: store it and look at the next.
                 */
                *p++ = c;
                if (--inr > 0)
                    inp++;
                else
                    break;      /* end of input */
            }
            /*
             * If we had only a sign, it is no good; push
             * back the sign.  If the number ends in `x',
             * it was [sign] '0' 'x', so push back the x
             * and treat it as [sign] '0'.
             */
            if (flags & NDIGITS) {
                if (p > buf) {
                    inp--;
                    inr++;
                }
                goto match_failure;
            }
            c = ((u_char *)p)[-1];
            if (c == 'x' || c == 'X') {
                --p;
                inp--;
                inr++;
            }
            if ((flags & SUPPRESS) == 0) {
                u_quad_t res;

                *p = 0;
                if ((flags & UNSIGNED) == 0)
                    res = (u_quad_t)strtoq(buf, (char **)NULL, base);
                else
                    res = strtouq(buf, (char **)NULL, base);
                if (flags & POINTER)
                    *va_arg(ap, void **) =
                        (void *)(uintptr_t)res;
                else if (flags & SHORTSHORT)
                    *va_arg(ap, char *) = res;
                else if (flags & SHORT)
                    *va_arg(ap, short *) = res;
                else if (flags & LONG)
                    *va_arg(ap, long *) = res;
                else if (flags & LONGLONG)
                    *va_arg(ap, long long *) = res;
                else
                    *va_arg(ap, int *) = res;
                nassigned++;
            }
            nread += p - buf;
            nconversions++;
            break;

        }
    }
input_failure:
    return (nconversions != 0 ? nassigned : -1);
match_failure:
    return (nassigned);
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

