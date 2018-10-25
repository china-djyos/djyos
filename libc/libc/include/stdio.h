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
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
//所属模块:内核模块
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 消息系统相关的定义
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2011-01-24
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 创建文件
//------------------------------------------------------


#ifndef _STDIO_H_
#define _STDIO_H_

#include "_ansi.h"

//#define   _FSTDIO         /* ``function stdio'' */

//#define __need_size_t
//#define __need_NULL
#include <stddef.h>

//#define __need___va_list
#include <stdarg.h>

/*
 * <sys/reent.h> defines __FILE, _fpos_t.
 * They must be defined there because struct _reent needs them (and we don't
 * want reent.h to include this file.
 */

//#include <sys/reent.h>
//#include <sys/types.h>

//
// 增加项
//
#include <fs/file.h>

typedef struct LibcFile FILE;

/**
 * Standard file descriptors
 */
extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

extern s32 (* PutStrDirect)(const char *buf,u32 len);
extern char (* GetCharDirect)(void);

bool_t AddToStdin(s32 fd, bool_t LineEdit);
bool_t DisableLineEdit(void);
bool_t EnableLineEdit(void);
s32 ModuleInstall_STDIO(const char *in,const char *out, const char *err);
bool_t IsInitedStdio(FILE* stream);


//typedef __FILE FILE;

//#ifdef __CYGWIN__
//typedef _fpos64_t fpos_t;
//#else
//typedef _fpos_t fpos_t;
//#ifdef __LARGE64_FILES
//typedef _fpos64_t fpos64_t;
//#endif
//#endif /* !__CYGWIN__ */

//#include <sys/stdio.h>

#define __SLBF  0x0001      /* line buffered */
#define __SNBF  0x0002      /* unbuffered */
#define __SRD   0x0004      /* OK to read */
#define __SWR   0x0008      /* OK to write */
    /* RD and WR are never simultaneously asserted */
#define __SRW   0x0010      /* open for reading & writing */
#define __SEOF  0x0020      /* found EOF */
#define __SERR  0x0040      /* found error */
#define __SMBF  0x0080      /* _buf is from malloc */
#define __SAPP  0x0100      /* fdopen()ed in append mode - so must  write to end */
#define __SSTR  0x0200      /* this is an sprintf/snprintf string */
#define __SOPT  0x0400      /* do fseek() optimisation */
#define __SNPT  0x0800      /* do not do fseek() optimisation */
#define __SOFF  0x1000      /* set iff _offset is in fact correct */
#define __SORD  0x2000      /* true => stream orientation (byte/wide) decided */
//#if defined(__CYGWIN__)
//#  define __SCLE  0x4000        /* convert line endings CR/LF <-> NL */
//#endif
#define __SL64  0x8000      /* is 64-bit offset large file */

/* _flags2 flags */
#define __SWID  0x2000      /* true => stream orientation wide, false => byte, only valid if __SORD in _flags is true */


// 定义缓冲文件系统操作的标志
#define FP_IOREAD           0x0001      // currently reading
#define FP_IOWRITE          0x0002      // currently writing
//#define FP_IOFLOW           0x0100      // 流动数据，例如串口，display等，不允许写缓冲
#define FP_IORW             0x0080      // opened as "r+w"
#define FP_BINARY           0x1000      // reading binary file
#define FP_TEXT             0x2000      // reading text file

#define FP_IOFBF            0x0000      // full buffered
#define FP_IOLBF            0x0040      // line buffered
#define FP_IONBF            0x0004      // not buffered

#define FP_IOMYBUF          0x0008      // stdio malloc()'d buffer
#define FP_IOEOF            0x0010      // EOF reached on read
#define FP_IOERR            0x0020      // I/O error from system
#define FP_IOSTRG           0x0040      // Strange or no file descriptor
#define FP_IOAPPEND         0x0400      // Open with append

#define EOF (-1)

#ifdef __BUFSIZ__
#define FILEBUFSIZ      __BUFSIZ__
#else
#define FILEBUFSIZ      1024
#endif

#ifdef __FILENAME_MAX__
#define FILENAME_MAX    __FILENAME_MAX__
#else
#define FILENAME_MAX    255
#endif

/*
 * The maximum size of name (including NUL) that will be put in the user
 * supplied buffer caName for tmpnam.
 * Inferred from the size of the static buffer returned by tmpnam
 * when passed a NULL argument. May actually be smaller.
 */
#ifdef __L_tmpnam__
#define L_tmpnam    __L_tmpnam__
#else
#define L_tmpnam    FILENAME_MAX
#endif

#ifndef __STRICT_ANSI__
#define P_tmpdir        "/tmp"
#endif

#ifndef SEEK_SET
#define SEEK_SET    0   /* set file offset to offset */
#endif
#ifndef SEEK_CUR
#define SEEK_CUR    1   /* set file offset to current plus offset */
#endif
#ifndef SEEK_END
#define SEEK_END    2   /* set file offset to EOF plus offset */
#endif

#define TMP_MAX     26

//#define   stdin   (_REENT->_stdin)
//#define   stdout  (_REENT->_stdout)
//#define   stderr  (_REENT->_stderr)

//#define _stdin_r(x)   ((x)->_stdin)
//#define _stdout_r(x)  ((x)->_stdout)
//#define _stderr_r(x)  ((x)->_stderr)

/*
 * Functions defined in ANSI C standard.
 */

#ifndef __VALIST
#ifdef __GNUC__
#define __VALIST __gnuc_va_list
#else
#define __VALIST char*
#endif
#endif

FILE *  _EXFUN(fopen, (const char *restrict, const char *restrict));
int   _EXFUN(fclose, (FILE *));
size_t   _EXFUN(fread, (void *restrict, size_t, size_t, FILE *restrict));
size_t   _EXFUN(fwrite, (const void *restrict, size_t, size_t, FILE *restrict));
int   _EXFUN(fseek, (FILE *, long, int));
int   _EXFUN(fseeko, (FILE *, off_t, int));
int   _EXFUN(remove, (const char *));
FILE *  _EXFUN(freopen, (const char *restrict, const char *restrict, FILE *restrict));
int   _EXFUN(vfprintf, (FILE *restrict, const char *restrict, va_list));
int   _EXFUN(fprintf, (FILE *restrict, const char *restrict, ...));
//s32 printk (const char *fmt, ...);
//void NULL_Func(void);
int   _EXFUN(fputc, (int, FILE *));
int   _EXFUN(fgetc, (FILE *));

FILE *  _EXFUN(tmpfile, (void));
char *  _EXFUN(tmpnam, (char *));
void    _EXFUN(setbuf, (FILE *__restrict, char *__restrict));
int _EXFUN(setvbuf, (FILE *__restrict, char *__restrict, int, size_t));
int _EXFUN(fscanf, (FILE *__restrict, const char *__restrict, ...)
               _ATTRIBUTE ((__format__ (__scanf__, 2, 3))));
int _EXFUN(printf, (const char *__restrict, ...)
               _ATTRIBUTE ((__format__ (__printf__, 1, 2))));
int _EXFUN(scanf, (const char *__restrict, ...)
               _ATTRIBUTE ((__format__ (__scanf__, 1, 2))));
int _EXFUN(sscanf, (const char *__restrict, const char *__restrict, ...)
               _ATTRIBUTE ((__format__ (__scanf__, 2, 3))));
int _EXFUN(vfprintf, (FILE *__restrict, const char *__restrict, __VALIST)
               _ATTRIBUTE ((__format__ (__printf__, 2, 0))));
int _EXFUN(vprintf, (const char *, __VALIST)
               _ATTRIBUTE ((__format__ (__printf__, 1, 0))));
int _EXFUN(vsprintf, (char *__restrict, const char *__restrict, __VALIST)
               _ATTRIBUTE ((__format__ (__printf__, 2, 0))));
char *  _EXFUN(fgets, (char *__restrict, int, FILE *__restrict));
int _EXFUN(fputs, (const char *__restrict, FILE *__restrict));
int _EXFUN(getc, (FILE *));
int _EXFUN(getchar, (void));
char *  _EXFUN(gets, (char *));
int _EXFUN(putc, (int, FILE *));
int _EXFUN(putchar, (int));
int _EXFUN(puts, (const char *));
int _EXFUN(ungetc, (int, FILE *));
//#ifdef _COMPILING_NEWLIB
//int   _EXFUN(fgetpos, (FILE *, _fpos_t *));
//#else
//int   _EXFUN(fgetpos, (FILE *__restrict, fpos_t *__restrict));
//#endif
//#ifdef _COMPILING_NEWLIB
//int   _EXFUN(fsetpos, (FILE *, const _fpos_t *));
//#else
//int   _EXFUN(fsetpos, (FILE *, const fpos_t *));
//#endif
long    _EXFUN(ftell, ( FILE *));
off_t    _EXFUN(ftello, ( FILE *));
void    _EXFUN(rewind, (FILE *));
void    _EXFUN(clearerr, (FILE *));
int _EXFUN(feof, (FILE *));
int _EXFUN(ferror, (FILE *));
void    _EXFUN(perror, (const char *));
//#ifndef _REENT_ONLY
int _EXFUN(sprintf, (char *__restrict, const char *__restrict, ...)
               _ATTRIBUTE ((__format__ (__printf__, 2, 3))));

//int   _EXFUN(rename, (const char *, const char *));
//#ifdef _COMPILING_NEWLIB
//int   _EXFUN(_rename, (const char *, const char *));
//#endif
//#endif
//#if !defined(__STRICT_ANSI__) || defined(__USE_XOPEN2K)
//#ifdef _COMPILING_NEWLIB
//int   _EXFUN(fseeko, (FILE *, _off_t, int));
//_off_t    _EXFUN(ftello, ( FILE *));
//#else
//int   _EXFUN(fseeko, (FILE *, off_t, int));
//off_t _EXFUN(ftello, ( FILE *));
//#endif
//#endif
#if !defined(__STRICT_ANSI__) || (__STDC_VERSION__ >= 199901L)
//#ifndef _REENT_ONLY
//int   _EXFUN(asiprintf, (char **, const char *, ...)
//               _ATTRIBUTE ((__format__ (__printf__, 2, 3))));
char *  _EXFUN(asniprintf, (char *, size_t *, const char *, ...)
               _ATTRIBUTE ((__format__ (__printf__, 3, 4))));
char *  _EXFUN(asnprintf, (char *__restrict, size_t *__restrict, const char *__restrict, ...)
               _ATTRIBUTE ((__format__ (__printf__, 3, 4))));
int _EXFUN(asprintf, (char **__restrict, const char *__restrict, ...)
               _ATTRIBUTE ((__format__ (__printf__, 2, 3))));
//#ifndef diprintf
//int   _EXFUN(diprintf, (int, const char *, ...)
//               _ATTRIBUTE ((__format__ (__printf__, 2, 3))));
//#endif
int _EXFUN(fcloseall, (_VOID));
int _EXFUN(fiprintf, (FILE *, const char *, ...)
               _ATTRIBUTE ((__format__ (__printf__, 2, 3))));
int _EXFUN(fiscanf, (FILE *, const char *, ...)
               _ATTRIBUTE ((__format__ (__scanf__, 2, 3))));
int _EXFUN(iprintf, (const char *, ...)
               _ATTRIBUTE ((__format__ (__printf__, 1, 2))));
int _EXFUN(iscanf, (const char *, ...)
               _ATTRIBUTE ((__format__ (__scanf__, 1, 2))));
int _EXFUN(siprintf, (char *, const char *, ...)
               _ATTRIBUTE ((__format__ (__printf__, 2, 3))));
int _EXFUN(siscanf, (const char *, const char *, ...)
               _ATTRIBUTE ((__format__ (__scanf__, 2, 3))));
int _EXFUN(snprintf, (char *__restrict, size_t, const char *__restrict, ...)
               _ATTRIBUTE ((__format__ (__printf__, 3, 4))));
//int   _EXFUN(sniprintf, (char *, size_t, const char *, ...)
//               _ATTRIBUTE ((__format__ (__printf__, 3, 4))));
char *  _EXFUN(tempnam, (const char *, const char *));
int _EXFUN(vasiprintf, (char **, const char *, __VALIST)
               _ATTRIBUTE ((__format__ (__printf__, 2, 0))));
char *  _EXFUN(vasniprintf, (char *, size_t *, const char *, __VALIST)
               _ATTRIBUTE ((__format__ (__printf__, 3, 0))));
char *  _EXFUN(vasnprintf, (char *, size_t *, const char *, __VALIST)
               _ATTRIBUTE ((__format__ (__printf__, 3, 0))));
int _EXFUN(vasprintf, (char **, const char *, __VALIST)
               _ATTRIBUTE ((__format__ (__printf__, 2, 0))));
int _EXFUN(vdiprintf, (int, const char *, __VALIST)
               _ATTRIBUTE ((__format__ (__printf__, 2, 0))));
int _EXFUN(vfiprintf, (FILE *, const char *, __VALIST)
               _ATTRIBUTE ((__format__ (__printf__, 2, 0))));
int _EXFUN(vfiscanf, (FILE *, const char *, __VALIST)
               _ATTRIBUTE ((__format__ (__scanf__, 2, 0))));
int _EXFUN(vfscanf, (FILE *__restrict, const char *__restrict, __VALIST)
               _ATTRIBUTE ((__format__ (__scanf__, 2, 0))));
int _EXFUN(viprintf, (const char *, __VALIST)
               _ATTRIBUTE ((__format__ (__printf__, 1, 0))));
int _EXFUN(viscanf, (const char *, __VALIST)
               _ATTRIBUTE ((__format__ (__scanf__, 1, 0))));
int _EXFUN(vscanf, (const char *, __VALIST)
               _ATTRIBUTE ((__format__ (__scanf__, 1, 0))));
int _EXFUN(vsiprintf, (char *, const char *, __VALIST)
               _ATTRIBUTE ((__format__ (__printf__, 2, 0))));
int _EXFUN(vsiscanf, (const char *, const char *, __VALIST)
               _ATTRIBUTE ((__format__ (__scanf__, 2, 0))));
int _EXFUN(vsniprintf, (char *, size_t, const char *, __VALIST)
               _ATTRIBUTE ((__format__ (__printf__, 3, 0))));
//int   _EXFUN(vsnprintf, (char *__restrict, size_t, const char *__restrict, __VALIST)
//               _ATTRIBUTE ((__format__ (__printf__, 3, 0))));
int _EXFUN(vsscanf, (const char *__restrict, const char *__restrict, __VALIST)
               _ATTRIBUTE ((__format__ (__scanf__, 2, 0))));
//#endif /* !_REENT_ONLY */
#endif /* !__STRICT_ANSI__ */

/*
 * Routines in POSIX 1003.1:2001.
 */

#ifndef __STRICT_ANSI__
//#ifndef _REENT_ONLY
FILE *  _EXFUN(fdopen, (int, const char *));
//#endif
int _EXFUN(fileno, (FILE *));
int _EXFUN(getw, (FILE *));
int _EXFUN(pclose, (FILE *));
FILE *  _EXFUN(popen, (const char *, const char *));
int _EXFUN(putw, (int, FILE *));
void    _EXFUN(setbuffer, (FILE *, char *, int));
int _EXFUN(setlinebuf, (FILE *));
int _EXFUN(getc_unlocked, (FILE *));
int _EXFUN(getchar_unlocked, (void));
void    _EXFUN(flockfile, (FILE *));
int _EXFUN(ftrylockfile, (FILE *));
void    _EXFUN(funlockfile, (FILE *));
int _EXFUN(putc_unlocked, (int, FILE *));
int _EXFUN(putchar_unlocked, (int));
#endif /* ! __STRICT_ANSI__ */

/*
 * Routines in POSIX 1003.1:200x.
 */

#ifndef __STRICT_ANSI__
//# ifndef _REENT_ONLY
#  ifndef dprintf
int _EXFUN(dprintf, (int, const char *__restrict, ...)
               _ATTRIBUTE ((__format__ (__printf__, 2, 3))));
#  endif
FILE *  _EXFUN(fmemopen, (void *__restrict, size_t, const char *__restrict));
/* getdelim - see __getdelim for now */
/* getline - see __getline for now */
FILE *  _EXFUN(open_memstream, (char **, size_t *));
//#if defined (__CYGWIN__)
//int   _EXFUN(renameat, (int, const char *, int, const char *));
//#endif
int _EXFUN(vdprintf, (int, const char *__restrict, __VALIST)
               _ATTRIBUTE ((__format__ (__printf__, 2, 0))));
//# endif
#endif

/*
 * Recursive versions of the above.
 */

//int   _EXFUN(_asiprintf_r, (struct _reent *, char **, const char *, ...)
//               _ATTRIBUTE ((__format__ (__printf__, 3, 4))));
//char *    _EXFUN(_asniprintf_r, (struct _reent *, char *, size_t *, const char *, ...)
//               _ATTRIBUTE ((__format__ (__printf__, 4, 5))));
//char *    _EXFUN(_asnprintf_r, (struct _reent *, char *__restrict, size_t *__restrict, const char *__restrict, ...)
//               _ATTRIBUTE ((__format__ (__printf__, 4, 5))));
//int   _EXFUN(_asprintf_r, (struct _reent *, char **__restrict, const char *__restrict, ...)
//               _ATTRIBUTE ((__format__ (__printf__, 3, 4))));
//int   _EXFUN(_diprintf_r, (struct _reent *, int, const char *, ...)
//               _ATTRIBUTE ((__format__ (__printf__, 3, 4))));
//int   _EXFUN(_dprintf_r, (struct _reent *, int, const char *__restrict, ...)
//               _ATTRIBUTE ((__format__ (__printf__, 3, 4))));
//int   _EXFUN(_fclose_r, (struct _reent *, FILE *));
//int   _EXFUN(_fcloseall_r, (struct _reent *));
//FILE *    _EXFUN(_fdopen_r, (struct _reent *, int, const char *));
//int   _EXFUN(_fflush_r, (struct _reent *, FILE *));
//int   _EXFUN(_fgetc_r, (struct _reent *, FILE *));
//char *  _EXFUN(_fgets_r, (struct _reent *, char *__restrict, int, FILE *__restrict));
//#ifdef _COMPILING_NEWLIB
//int   _EXFUN(_fgetpos_r, (struct _reent *, FILE *__restrict, _fpos_t *__restrict));
//int   _EXFUN(_fsetpos_r, (struct _reent *, FILE *, const _fpos_t *));
//#else
//int   _EXFUN(_fgetpos_r, (struct _reent *, FILE *, fpos_t *));
//int   _EXFUN(_fsetpos_r, (struct _reent *, FILE *, const fpos_t *));
//#endif
//int   _EXFUN(_fiprintf_r, (struct _reent *, FILE *, const char *, ...)
//               _ATTRIBUTE ((__format__ (__printf__, 3, 4))));
//int   _EXFUN(_fiscanf_r, (struct _reent *, FILE *, const char *, ...)
//               _ATTRIBUTE ((__format__ (__scanf__, 3, 4))));
//FILE *    _EXFUN(_fmemopen_r, (struct _reent *, void *__restrict, size_t, const char *__restrict));
//FILE *    _EXFUN(_fopen_r, (struct _reent *, const char *__restrict, const char *__restrict));
//FILE *    _EXFUN(_freopen_r, (struct _reent *, const char *__restrict, const char *__restrict, FILE *__restrict));
//int   _EXFUN(_fprintf_r, (struct _reent *, FILE *__restrict, const char *__restrict, ...)
//               _ATTRIBUTE ((__format__ (__printf__, 3, 4))));
//int   _EXFUN(_fpurge_r, (struct _reent *, FILE *));
//int   _EXFUN(_fputc_r, (struct _reent *, int, FILE *));
//int   _EXFUN(_fputs_r, (struct _reent *, const char *__restrict, FILE *__restrict));
//size_t    _EXFUN(_fread_r, (struct _reent *, _PTR __restrict, size_t _size, size_t _n, FILE *__restrict));
//int   _EXFUN(_fscanf_r, (struct _reent *, FILE *__restrict, const char *__restrict, ...)
//               _ATTRIBUTE ((__format__ (__scanf__, 3, 4))));
//int   _EXFUN(_fseek_r, (struct _reent *, FILE *, long, int));
//int   _EXFUN(_fseeko_r,(struct _reent *, FILE *, _off_t, int));
//long  _EXFUN(_ftell_r, (struct _reent *, FILE *));
//_off_t    _EXFUN(_ftello_r,(struct _reent *, FILE *));
//void  _EXFUN(_rewind_r, (struct _reent *, FILE *));
//size_t    _EXFUN(_fwrite_r, (struct _reent *, const _PTR __restrict, size_t _size, size_t _n, FILE *__restrict));
//int   _EXFUN(_getc_r, (struct _reent *, FILE *));
//int   _EXFUN(_getc_unlocked_r, (struct _reent *, FILE *));
//int   _EXFUN(_getchar_r, (struct _reent *));
//int   _EXFUN(_getchar_unlocked_r, (struct _reent *));
//char *    _EXFUN(_gets_r, (struct _reent *, char *));
//int   _EXFUN(_iprintf_r, (struct _reent *, const char *, ...)
//               _ATTRIBUTE ((__format__ (__printf__, 2, 3))));
//int   _EXFUN(_iscanf_r, (struct _reent *, const char *, ...)
//               _ATTRIBUTE ((__format__ (__scanf__, 2, 3))));
//FILE *    _EXFUN(_open_memstream_r, (struct _reent *, char **, size_t *));
//void  _EXFUN(_perror_r, (struct _reent *, const char *));
//int   _EXFUN(_printf_r, (struct _reent *, const char *__restrict, ...)
//               _ATTRIBUTE ((__format__ (__printf__, 2, 3))));
//int   _EXFUN(_putc_r, (struct _reent *, int, FILE *));
//int   _EXFUN(_putc_unlocked_r, (struct _reent *, int, FILE *));
//int   _EXFUN(_putchar_unlocked_r, (struct _reent *, int));
//int   _EXFUN(_putchar_r, (struct _reent *, int));
//int   _EXFUN(_puts_r, (struct _reent *, const char *));
//int   _EXFUN(_remove_r, (struct _reent *, const char *));
//int   _EXFUN(_rename_r, (struct _reent *,
//             const char *_old, const char *_new));
//int   _EXFUN(_scanf_r, (struct _reent *, const char *__restrict, ...)
//               _ATTRIBUTE ((__format__ (__scanf__, 2, 3))));
//int   _EXFUN(_siprintf_r, (struct _reent *, char *, const char *, ...)
//               _ATTRIBUTE ((__format__ (__printf__, 3, 4))));
//int   _EXFUN(_siscanf_r, (struct _reent *, const char *, const char *, ...)
//               _ATTRIBUTE ((__format__ (__scanf__, 3, 4))));
//int   _EXFUN(_sniprintf_r, (struct _reent *, char *, size_t, const char *, ...)
//               _ATTRIBUTE ((__format__ (__printf__, 4, 5))));
//int   _EXFUN(_snprintf_r, (struct _reent *, char *__restrict, size_t, const char *__restrict, ...)
//               _ATTRIBUTE ((__format__ (__printf__, 4, 5))));
//int   _EXFUN(_sprintf_r, (struct _reent *, char *__restrict, const char *__restrict, ...)
//               _ATTRIBUTE ((__format__ (__printf__, 3, 4))));
//int   _EXFUN(_sscanf_r, (struct _reent *, const char *__restrict, const char *__restrict, ...)
//               _ATTRIBUTE ((__format__ (__scanf__, 3, 4))));
//char *    _EXFUN(_tempnam_r, (struct _reent *, const char *, const char *));
//FILE *    _EXFUN(_tmpfile_r, (struct _reent *));
//char *    _EXFUN(_tmpnam_r, (struct _reent *, char *));
//int   _EXFUN(_ungetc_r, (struct _reent *, int, FILE *));
//int   _EXFUN(_vasiprintf_r, (struct _reent *, char **, const char *, __VALIST)
//               _ATTRIBUTE ((__format__ (__printf__, 3, 0))));
//char *    _EXFUN(_vasniprintf_r, (struct _reent*, char *, size_t *, const char *, __VALIST)
//               _ATTRIBUTE ((__format__ (__printf__, 4, 0))));
//char *    _EXFUN(_vasnprintf_r, (struct _reent*, char *, size_t *, const char *, __VALIST)
//               _ATTRIBUTE ((__format__ (__printf__, 4, 0))));
//int   _EXFUN(_vasprintf_r, (struct _reent *, char **, const char *, __VALIST)
//               _ATTRIBUTE ((__format__ (__printf__, 3, 0))));
//int   _EXFUN(_vdiprintf_r, (struct _reent *, int, const char *, __VALIST)
//               _ATTRIBUTE ((__format__ (__printf__, 3, 0))));
//int   _EXFUN(_vdprintf_r, (struct _reent *, int, const char *__restrict, __VALIST)
//               _ATTRIBUTE ((__format__ (__printf__, 3, 0))));
//int   _EXFUN(_vfiprintf_r, (struct _reent *, FILE *, const char *, __VALIST)
//               _ATTRIBUTE ((__format__ (__printf__, 3, 0))));
//int   _EXFUN(_vfiscanf_r, (struct _reent *, FILE *, const char *, __VALIST)
//               _ATTRIBUTE ((__format__ (__scanf__, 3, 0))));
//int   _EXFUN(_vfprintf_r, (struct _reent *, FILE *__restrict, const char *__restrict, __VALIST)
//               _ATTRIBUTE ((__format__ (__printf__, 3, 0))));
//int   _EXFUN(_vfscanf_r, (struct _reent *, FILE *__restrict, const char *__restrict, __VALIST)
//               _ATTRIBUTE ((__format__ (__scanf__, 3, 0))));
//int   _EXFUN(_viprintf_r, (struct _reent *, const char *, __VALIST)
//               _ATTRIBUTE ((__format__ (__printf__, 2, 0))));
//int   _EXFUN(_viscanf_r, (struct _reent *, const char *, __VALIST)
//               _ATTRIBUTE ((__format__ (__scanf__, 2, 0))));
//int   _EXFUN(_vprintf_r, (struct _reent *, const char *__restrict, __VALIST)
//               _ATTRIBUTE ((__format__ (__printf__, 2, 0))));
//int   _EXFUN(_vscanf_r, (struct _reent *, const char *__restrict, __VALIST)
//               _ATTRIBUTE ((__format__ (__scanf__, 2, 0))));
//int   _EXFUN(_vsiprintf_r, (struct _reent *, char *, const char *, __VALIST)
//               _ATTRIBUTE ((__format__ (__printf__, 3, 0))));
//int   _EXFUN(_vsiscanf_r, (struct _reent *, const char *, const char *, __VALIST)
//               _ATTRIBUTE ((__format__ (__scanf__, 3, 0))));
//int   _EXFUN(_vsniprintf_r, (struct _reent *, char *, size_t, const char *, __VALIST)
//               _ATTRIBUTE ((__format__ (__printf__, 4, 0))));
//int   _EXFUN(_vsnprintf_r, (struct _reent *, char *__restrict, size_t, const char *__restrict, __VALIST)
//               _ATTRIBUTE ((__format__ (__printf__, 4, 0))));
//int   _EXFUN(_vsprintf_r, (struct _reent *, char *__restrict, const char *__restrict, __VALIST)
//               _ATTRIBUTE ((__format__ (__printf__, 3, 0))));
//int   _EXFUN(_vsscanf_r, (struct _reent *, const char *__restrict, const char *__restrict, __VALIST)
//               _ATTRIBUTE ((__format__ (__scanf__, 3, 0))));

/* Other extensions.  */

int _EXFUN(fpurge, (FILE *));
//ssize_t _EXFUN(__getdelim, (char **, size_t *, int, FILE *));
//ssize_t _EXFUN(__getline, (char **, size_t *, FILE *));

//#ifdef __LARGE64_FILES
//#if !defined(__CYGWIN__) || defined(_COMPILING_NEWLIB)
//FILE *    _EXFUN(fdopen64, (int, const char *));
//FILE *  _EXFUN(fopen64, (const char *, const char *));
//FILE *  _EXFUN(freopen64, (_CONST char *, _CONST char *, FILE *));
//_off64_t _EXFUN(ftello64, (FILE *));
//_off64_t _EXFUN(fseeko64, (FILE *, _off64_t, int));
//int     _EXFUN(fgetpos64, (FILE *, _fpos64_t *));
//int     _EXFUN(fsetpos64, (FILE *, const _fpos64_t *));
//FILE *  _EXFUN(tmpfile64, (void));

//FILE *    _EXFUN(_fdopen64_r, (struct _reent *, int, const char *));
//FILE *  _EXFUN(_fopen64_r, (struct _reent *,const char *, const char *));
//FILE *  _EXFUN(_freopen64_r, (struct _reent *, _CONST char *, _CONST char *, FILE *));
//_off64_t _EXFUN(_ftello64_r, (struct _reent *, FILE *));
//_off64_t _EXFUN(_fseeko64_r, (struct _reent *, FILE *, _off64_t, int));
//int     _EXFUN(_fgetpos64_r, (struct _reent *, FILE *, _fpos64_t *));
//int     _EXFUN(_fsetpos64_r, (struct _reent *, FILE *, const _fpos64_t *));
//FILE *  _EXFUN(_tmpfile64_r, (struct _reent *));
//#endif /* !__CYGWIN__ */
//#endif /* __LARGE64_FILES */

/*
 * Routines internal to the implementation.
 */

//int   _EXFUN(__srget_r, (struct _reent *, FILE *));
//int   _EXFUN(__swbuf_r, (struct _reent *, int, FILE *));

/*
 * Stdio function-access interface.
 */

//#ifndef __STRICT_ANSI__
//# ifdef __LARGE64_FILES
//FILE  *_EXFUN(funopen,(const _PTR __cookie,
//      int (*__readfn)(_PTR __c, char *__buf,
//              _READ_WRITE_BUFSIZE_TYPE __n),
//      int (*__writefn)(_PTR __c, const char *__buf,
//               _READ_WRITE_BUFSIZE_TYPE __n),
//      _fpos64_t (*__seekfn)(_PTR __c, _fpos64_t __off, int __whence),
//      int (*__closefn)(_PTR __c)));
//FILE  *_EXFUN(_funopen_r,(struct _reent *, const _PTR __cookie,
//      int (*__readfn)(_PTR __c, char *__buf,
//              _READ_WRITE_BUFSIZE_TYPE __n),
//      int (*__writefn)(_PTR __c, const char *__buf,
//               _READ_WRITE_BUFSIZE_TYPE __n),
//      _fpos64_t (*__seekfn)(_PTR __c, _fpos64_t __off, int __whence),
//      int (*__closefn)(_PTR __c)));
//# else
//FILE  *_EXFUN(funopen,(const _PTR __cookie,
//      int (*__readfn)(_PTR __cookie, char *__buf,
//              _READ_WRITE_BUFSIZE_TYPE __n),
//      int (*__writefn)(_PTR __cookie, const char *__buf,
//               _READ_WRITE_BUFSIZE_TYPE __n),
//      fpos_t (*__seekfn)(_PTR __cookie, fpos_t __off, int __whence),
//      int (*__closefn)(_PTR __cookie)));
//FILE  *_EXFUN(_funopen_r,(struct _reent *, const _PTR __cookie,
//      int (*__readfn)(_PTR __cookie, char *__buf,
//              _READ_WRITE_BUFSIZE_TYPE __n),
//      int (*__writefn)(_PTR __cookie, const char *__buf,
//               _READ_WRITE_BUFSIZE_TYPE __n),
//      fpos_t (*__seekfn)(_PTR __cookie, fpos_t __off, int __whence),
//      int (*__closefn)(_PTR __cookie)));
//# endif /* !__LARGE64_FILES */

/*# define  fropen(__cookie, __fn) funopen(__cookie, __fn, (int (*)())0, \
                           (fpos_t (*)())0, (int (*)())0)*/
/*# define  fwopen(__cookie, __fn) funopen(__cookie, (int (*)())0, __fn, \
                           (fpos_t (*)())0, (int (*)())0)*/

//typedef ssize_t cookie_read_function_t(void *__cookie, char *__buf, size_t __n);
//typedef ssize_t cookie_write_function_t(void *__cookie, const char *__buf,
//                  size_t __n);
//# ifdef __LARGE64_FILES
//typedef int cookie_seek_function_t(void *__cookie, _off64_t *__off,
//                 int __whence);
//# else
//typedef int cookie_seek_function_t(void *__cookie, off_t *__off, int __whence);
//# endif /* !__LARGE64_FILES */
//typedef int cookie_close_function_t(void *__cookie);
//typedef struct
//{
  /* These four struct member names are dictated by Linux; hopefully,
     they don't conflict with any macros.  */
//  cookie_read_function_t  *read;
//  cookie_write_function_t *write;
//  cookie_seek_function_t  *seek;
//  cookie_close_function_t *close;
//} cookie_io_functions_t;
//FILE *_EXFUN(fopencookie,(void *__cookie,
//      const char *__mode, cookie_io_functions_t __functions));
//FILE *_EXFUN(_fopencookie_r,(struct _reent *, void *__cookie,
//      const char *__mode, cookie_io_functions_t __functions));
//#endif /* ! __STRICT_ANSI__ */

//#ifndef __CUSTOM_FILE_IO__
/*
 * The __sfoo macros are here so that we can
 * define function versions in the C library.
 */
//#define       __sgetc_raw_r(__ptr, __f) (--(__f)->_r < 0 ? __srget_r(__ptr, __f) : (int)(*(__f)->_p++))

//#ifdef __SCLE
/*  For a platform with CR/LF, additional logic is required by
  __sgetc_r which would otherwise simply be a macro; therefore we
  use an inlined function.  The function is only meant to be inlined
  in place as used and the function body should never be emitted.

  There are two possible means to this end when compiling with GCC,
  one when compiling with a standard C99 compiler, and for other
  compilers we're just stuck.  At the moment, this issue only
  affects the Cygwin target, so we'll most likely be using GCC. */

//_ELIDABLE_INLINE int __sgetc_r(struct _reent *__ptr, FILE *__p);
//
//_ELIDABLE_INLINE int __sgetc_r(struct _reent *__ptr, FILE *__p)
//  {
//    int __c = __sgetc_raw_r(__ptr, __p);
//    if ((__p->_flags & __SCLE) && (__c == '\r'))
//      {
//      int __c2 = __sgetc_raw_r(__ptr, __p);
//      if (__c2 == '\n')
//        __c = __c2;
//      else
//        ungetc(__c2, __p);
//      }
//    return __c;
//  }
//#else
//#define __sgetc_r(__ptr, __p) __sgetc_raw_r(__ptr, __p)
//#endif

//#ifdef _never /* __GNUC__ */
/* If this inline is actually used, then systems using coff debugging
   info get hopelessly confused.  21sept93 rich@cygnus.com.  */
//_ELIDABLE_INLINE int __sputc_r(struct _reent *_ptr, int _c, FILE *_p) {
//  if (--_p->_w >= 0 || (_p->_w >= _p->_lbfsize && (char)_c != '\n'))
//      return (*_p->_p++ = _c);
//  else
//      return (__swbuf_r(_ptr, _c, _p));
//}
//#else
/*
 * This has been tuned to generate reasonable code on the vax using pcc
 */
/*#define       __sputc_raw_r(__ptr, __c, __p) \
    (--(__p)->_w < 0 ? \
        (__p)->_w >= (__p)->_lbfsize ? \
            (*(__p)->_p = (__c)), *(__p)->_p != '\n' ? \
                (int)*(__p)->_p++ : \
                __swbuf_r(__ptr, '\n', __p) : \
            __swbuf_r(__ptr, (int)(__c), __p) : \
        (*(__p)->_p = (__c), (int)*(__p)->_p++))*/
//#ifdef __SCLE
/*#define __sputc_r(__ptr, __c, __p) \
        ((((__p)->_flags & __SCLE) && ((__c) == '\n')) \
          ? __sputc_raw_r(__ptr, '\r', (__p)) : 0 , \
        __sputc_raw_r((__ptr), (__c), (__p)))*/
//#else
//#define __sputc_r(__ptr, __c, __p) __sputc_raw_r(__ptr, __c, __p)
//#endif
//#endif

//#define   __sfeof(p)  (((p)->_flags & __SEOF) != 0)
//#define   __sferror(p)    (((p)->_flags & __SERR) != 0)
//#define   __sclearerr(p)  ((void)((p)->_flags &= ~(__SERR|__SEOF)))
//#define   __sfileno(p)    ((p)->_file)

//#ifndef _REENT_SMALL
//#define   feof(p)     __sfeof(p)
//#define   ferror(p)   __sferror(p)
//#define   clearerr(p) __sclearerr(p)
//#endif

//#if 0 /*ndef __STRICT_ANSI__ - FIXME: must initialize stdio first, use fn */
//#define   fileno(p)   __sfileno(p)
//#endif

//#ifndef __CYGWIN__
//#ifndef lint
//#define   getc(fp)    __sgetc_r(_REENT, fp)
//#define putc(x, fp)   __sputc_r(_REENT, x, fp)
//#endif /* lint */
//#endif /* __CYGWIN__ */

//#ifndef __STRICT_ANSI__
/* fast always-buffered version, true iff error */
/*#define   fast_putc(x,p) (--(p)->_w < 0 ? \
    __swbuf_r(_REENT, (int)(x), p) == EOF : (*(p)->_p = (x), (p)->_p++, 0))*/

//#define   L_cuserid   9       /* posix says it goes in stdio.h :( */
//#ifdef __CYGWIN__
//#define L_ctermid       16
//#endif
//#endif

//#endif /* !__CUSTOM_FILE_IO__ */

//#define   getchar()   getc(stdin)
//#define   putchar(x)  putc(x, stdout)


#endif /* _STDIO_H_ */

