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
// �����Ȩ�����ʹ���߷������¶������������£�����ʹ����ʹ�ü���ɢ����
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�����������У���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�����������У��Լ�����
//    ������������

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
//����ģ��:
//���ߣ�������
//�汾��V1.0.0
//�ļ�����:
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����  : 2014/12/17
//   �޸���: ������
//   �°汾��:
//   �޸�˵��: �½�
//------------------------------------------------------

#ifndef __STDDEF_H__
#define __STDDEF_H__

#ifdef  __cplusplus
extern "C"{
#endif

#include "stdint.h"

typedef ptu32_t ptrdiff_t;
typedef u32 size_t;     //todo,��������������������һ��,��Ȼmemcpy�������⡣
typedef s64 off_t;
typedef s32 ssize_t;
typedef s32 mode_t;
void NULL_func(void);

#ifndef true
  #define true  1
#endif
#ifndef false
  #define false               0
#endif
#ifndef FALSE
    #define FALSE             0
#endif
#ifndef TRUE
    #define TRUE              1
#endif

typedef ufast_t         bool_t;

#ifndef __cplusplus  /* wchar_t is a builtin type for C++ */
  #if !defined(__STRICT_ANSI__)
  /* unconditional in non-strict C for consistency of debug info */
    #if defined(__WCHAR32)
      typedef u32 wchar_t; /* also in <stdlib.h> and <inttypes.h> */
      typedef u32 wint_t;  /* also in <stdlib.h> and <inttypes.h> */
    #else
      typedef u16 wchar_t; /* also in <stdlib.h> and <inttypes.h> */
      typedef u16 wint_t;  /* also in <stdlib.h> and <inttypes.h> */
    #endif
  #elif !defined(__wchar_t)
    #define __wchar_t 1
    #if defined(__WCHAR32)
      typedef u32 wchar_t; /* also in <stdlib.h> and <inttypes.h> */
      typedef u32 wint_t;  /* also in <stdlib.h> and <inttypes.h> */
    #else
      typedef u16 wchar_t; /* also in <stdlib.h> and <inttypes.h> */
      typedef u16 wint_t;  /* also in <stdlib.h> and <inttypes.h> */
    #endif
   /*
    * An integral type whose range of values can represent distinct codes for
    * all members of the largest extended character set specified among the
    * supported locales; the null character shall have the code value zero and
    * each member of the basic character set shall have a code value when used
    * as the lone character in an integer character constant.
    */
  #endif
#else
#if defined(__WCHAR32)
  typedef   u32 wint_t;
#else
  typedef   u16 wint_t;
#endif
#endif

#ifndef __cplusplus
#ifndef NULL
    #define     NULL ((void*) 0)
#endif
#else   /* C++ */
#define NULL 0
#endif

typedef struct
{
  int __count;
  union
  {
    wint_t __wch;
    unsigned char __wchb[4];
  } __value;    /* Value so far.  */
} _mbstate_t;

   /* null pointer constant. */

  /* EDG uses __INTADDR__ to avoid errors when strict */
//lst ��__INTADDR__ ��__CLIBNS, ��minGW���޷�����
//  #define offsetof(t, memb) ((__CLIBNS size_t)__INTADDR__(&(((t *)0)->memb)))
  #define offsetof(t, memb) ((size_t)(&(((t *)0)->memb)))


#ifdef  __cplusplus
}
#endif

#endif

/* end of stddef.h */
