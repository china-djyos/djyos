//----------------------------------------------------
// Copyright (c) 2018,Open source team. All rights reserved.

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
// Copyright (c) 2014 著作权由都江堰操作系统开源团队所有。著作权人保留一切权利。
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

//所属模块: c库
//作者:  lst.
//版本：V1.0.0
//文件描述: 公共代码集
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2017-03-03
//   作者:  lst.
//   新版本号：V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------

#include <stdint.h>
#include <stddef.h>
#include <_ansi.h>
#include <sys/types.h>
#include <stdio.h>
#include <time.h>


#if defined(CONFIG_LIBC_LOCALTIME) || defined(CONFIG_TIME_EXTENDED)
static const char * const g_abbrev_wdayname[7] =
{
  "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

static const char * const g_wdayname[7] =
{
  "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};
#endif

static const char * const g_abbrev_monthname[12] =
{
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

static const char * const g_monthname[12] =
{
  "January", "February", "March",     "April",   "May",      "June",
  "July",    "August",   "September", "October", "November", "December"
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Function:  strftime
 *
 * Description:
 *   The  strftime()  function  formats the broken-down time tm according to
 *   the format specification format and places the result in the  character
 *   array s of size max.
 *
 *   Ordinary characters placed in the format string are copied to s without
 *   conversion.  Conversion specifications are introduced by a '%'  charac-
 *   ter,  and  terminated  by  a  conversion  specifier  character, and are
 *   replaced in s as follows:
 *
 *   %b     The abbreviated month name according to the current locale.
 *   %B     The full month name according to the current locale.
 *   %C     The century number (year/100) as a 2-digit integer. (SU)
 *   %d     The day of the month as a decimal number (range 01 to 31).
 *   %e     Like %d, the day of the month as a decimal number, but a leading
 *          zero is replaced by a space.
 *   %h     Equivalent to %b.  (SU)
 *   %H     The hour as a decimal number using a 24-hour clock (range 00 to 23).
 *   %I     The  hour as a decimal number using a 12-hour clock (range 01 to 12).
 *   %j     The day of the year as a decimal number (range 001 to 366).
 *   %k     The hour (24-hour clock) as a decimal number (range  0  to  23);
 *          single digits are preceded by a blank.  (See also %H.)  (TZ)
 *   %l     The  hour  (12-hour  clock) as a decimal number (range 1 to 12);
 *          single digits are preceded by a blank.  (See also %I.)  (TZ)
 *   %m     The month as a decimal number (range 01 to 12).
 *   %M     The minute as a decimal number (range 00 to 59).
 *   %n     A newline character. (SU)
 *   %p     Either "AM" or "PM" according to the given time  value, or the
 *          corresponding  strings  for the current locale.  Noon is treated
 *          as "PM" and midnight as "AM".
 *   %P     Like %p but in lowercase: "am" or "pm" or a corresponding string
 *          for the current locale. (GNU)
 *   %s     The number of seconds since the Epoch, that is, since 1970-01-01
 *          00:00:00 UTC. (TZ)
 *   %S     The second as a decimal number (range 00 to 60).  (The range is
 *          up to 60 to allow for occasional leap seconds.)
 *   %t     A tab character. (SU)
 *   %y     The year as a decimal number without a century (range 00 to 99).
 *   %Y     The year as a decimal number including the century.
 *   %%     A literal '%' character.
 *
 * Returned Value:
 *   The strftime() function returns the number of characters placed in  the
 *   array s, not including the terminating null byte, provided the string,
 *   including the terminating null byte, fits.  Otherwise,  it returns 0,
 *   and the contents of the array is undefined.
 *
 ****************************************************************************/

size_t strftime(  char *s, size_t max,   const char *format,
                  const struct tm *tm)
{
    const char *str;
    char       *dest   = s;
  int             chleft = max;
  int             value;
  int             len;

  while (*format && chleft > 0)
    {
      /* Just copy regular characters */

      if (*format != '%')
        {
           *dest++ = *format++;
           chleft--;
           continue;
        }

      /* Handle the format character */

       format++;
       len   = 0;

       switch (*format++)
         {
#if defined(CONFIG_LIBC_LOCALTIME) || defined(CONFIG_TIME_EXTENDED)
           /* %a: A three-letter abbreviation for the day of the week. */

           case 'a':
             {
               if (tm->tm_wday < 7)
                 {
                   str = g_abbrev_wdayname[tm->tm_wday];
                   len = snprintf(dest, chleft, "%s", str);
                 }
             }
             break;

           /* %A: The full name for the day of the week. */

           case 'A':
             {
               if (tm->tm_wday < 7)
                 {
                   str = g_wdayname[tm->tm_wday];
                   len = snprintf(dest, chleft, "%s", str);
                 }
             }
             break;
#else
           /* %a: A three-letter abbreviation for the day of the week. */
           /* %A: The full name for the day of the week. */

           case 'a':
           case 'A':
             {
               len = snprintf(dest, chleft, "Day"); /* Not supported */
             }
             break;
#endif

           /* %h: Equivalent to %b */

           case 'h':

           /* %b: The abbreviated month name according to the current locale. */

           case 'b':
             {
               if (tm->tm_mon < 12)
                 {
                   str = g_abbrev_monthname[tm->tm_mon];
                   len = snprintf(dest, chleft, "%s", str);
                 }
             }
             break;

           /* %B: The full month name according to the current locale. */

           case 'B':
             {
               if (tm->tm_mon < 12)
                 {
                   str = g_monthname[tm->tm_mon];
                   len = snprintf(dest, chleft, "%s", str);
                 }
             }
             break;

           /* %y: The year as a decimal number without a century (range 00 to 99). */

           case 'y':
             {
               len = snprintf(dest, chleft, "%02d", tm->tm_year % 100);
             }
             break;

           /* %C: The century number (year/100) as a 2-digit integer. */

           case 'C':
             {
               len = snprintf(dest, chleft, "%02d", tm->tm_year / 100);
             }
             break;

           /* %d: The day of the month as a decimal number (range 01 to 31). */

           case 'd':
             {
               len = snprintf(dest, chleft, "%02d", tm->tm_mday);
             }
             break;

           /* %e: Like %d, the day of the month as a decimal number, but a leading
            * zero is replaced by a space.
            */

           case 'e':
             {
               len = snprintf(dest, chleft, "%2d", tm->tm_mday);
             }
             break;

           /* %H: The hour as a decimal number using a 24-hour clock (range 00  to 23). */

           case 'H':
             {
               len = snprintf(dest, chleft, "%02d", tm->tm_hour);
             }
             break;

           /* %I: The  hour as a decimal number using a 12-hour clock (range 01 to 12). */

           case 'I':
             {
               len = snprintf(dest, chleft, "%02d", tm->tm_hour % 12);
             }
             break;

           /* %j: The day of the year as a decimal number (range 001 to 366). */

           case 'j':
             {
               if (tm->tm_mon < 12)
                 {
//                   value = clock_daysbeforemonth(tm->tm_mon, is_leap_year(tm->tm_year)) + tm->tm_mday;
                   value = tm->tm_yday+1;
                   len   = snprintf(dest, chleft, "%03d",value );
                 }
             }
             break;

           /* %k: The hour (24-hour clock) as a decimal number (range  0  to  23);
            * single digits are preceded by a blank.
            */

           case 'k':
             {
               len = snprintf(dest, chleft, "%2d", tm->tm_hour);
             }
             break;

           /* %l: The  hour  (12-hour  clock) as a decimal number (range 1 to 12);
            * single digits are preceded by a blank.
            */

           case 'l':
             {
               len = snprintf(dest, chleft, "%2d", tm->tm_hour % 12);
             }
             break;

           /* %m: The month as a decimal number (range 01 to 12). */

           case 'm':
             {
               len = snprintf(dest, chleft, "%02d", tm->tm_mon + 1);
             }
             break;

           /* %M: The minute as a decimal number (range 00 to 59). */

           case 'M':
             {
               len = snprintf(dest, chleft, "%02d", tm->tm_min);
             }
             break;

           /* %n: A newline character. */

           case 'n':
             {
               *dest = '\n';
               len   = 1;
             }
             break;

           /* %p: Either "AM" or "PM" according to the given time  value. */

           case 'p':
             {
               if (tm->tm_hour >= 12)
                 {
                   str = "PM";
                 }
               else
                 {
                   str = "AM";
                 }
               len = snprintf(dest, chleft, "%s", str);
             }
             break;

           /* %P: Like %p but in lowercase: "am" or "pm" */

           case 'P':
             {
               if (tm->tm_hour >= 12)
                 {
                   str = "pm";
                 }
               else
                 {
                   str = "am";
                 }
               len = snprintf(dest, chleft, "%s", str);
             }
             break;

           /* %s: The number of seconds since the Epoch, that is, since 1970-01-01
            * 00:00:00 UTC.  Hmmm... mktime argume is not 'const'.
            */

           case 's':
             {
               len = snprintf(dest, chleft, "%ld", mktime((  struct tm *)tm));
             }
             break;

           /* %S: The second as a decimal number (range 00 to 60).  (The range  is
            * up to 60 to allow for occasional leap seconds.)
            */

           case 'S':
             {
               len = snprintf(dest, chleft, "%02d", tm->tm_sec);
             }
             break;

           /* %t: A tab character. */

           case 't':
             {
               *dest = '\t';
               len   = 1;
             }
             break;

           /* %Y: The year as a decimal number including the century. */

           case 'Y':
             {
               len = snprintf(dest, chleft, "%04d", tm->tm_year + 1900);
             }
             break;

           /* %%:  A literal '%' character. */

           case '%':
             {
               *dest = '%';
               len   = 1;
             }
             break;
        }

      /* Update counts and pointers */

      dest   += len;
      chleft -= len;
    }

  /* We get here because either we have reached the end of the format string
   * or because there is no more space in the user-provided buffer and the
   * resulting string has been truncated.
   *
   * Is there space remaining in the user-provided buffer for the NUL
   * terminator?
   */

  if (chleft > 0)
    {
      /* Yes, append terminating NUL byte */

      *dest = '\0';

      /* And return the number of bytes in the resulting string (excluding
       * the NUL terminator).
       */

      return max - chleft;
    }

  /* The string was truncated and/or not properly terminated.  Return
   * zero.
   */

  return 0;
}

