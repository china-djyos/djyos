//-----------------------------------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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

#ifndef __OBJFILE_H__
#define __OBJFILE_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "object.h"
#include <list.h>
#include <fcntl.h>
#include <unistd.h>
#include <stat.h>

// ����Ϊ�˿ڣ�������������磬�����У�ͬһ���������ò�ͬ�˿�Ϊ��ͬ�û��ṩ����
// ����ϵͳ�У��˿���read/write����Ľӿڣ�һ��Object�������û�ʹ�ö���˿�
// read/write��һ���ļ���ϵͳ�У�ֻ��ռ��һ��object�ṹ��Object����ֱ�ӱ����ʡ�
// �����ļ�����ʹ��struct ObjectPort ���ļ����Ա���δ򿪣������ļ�ϵͳ�ľ���ʵ
// �֣�����ÿ�δ򿪶�����һ��ObjectPort�ṹ��Ҳ���ܹ���һ�� ObjectPort �ṹ��
// fd��ObjectHandleһһ��Ӧ��object��VFileHead��Աָ����ļ���ObjectPort��
// ע�⣬һ���򿪵��ļ�����·���ϵ�Ŀ¼����ʹû�д򿪣�Ҳ�����һ��object��������
// ����struct ObjectPort �ṹ��
struct __portBasic{
    list_t list; // ͬһ�ں˶���������ļ����ƿ��б�
    u32 flags; // ����ģʽ����fcntl.h�ж��壻
#define O_FILE_BASIC           0x80000000
};

struct __port{
    struct dListNode list; // ��__portBasic����
    u32 flags; // ��__portBasic����
    struct Object *obj; // ����
    u32 timeout; // ͬ�����ʵ�timeout����λus��������1.19Сʱ��
    struct MultiplexObjectCB * mphead; // ��·����Ŀ�����ͷָ�룻
    u32 access; // ����ĵ�ǰ����״̬����ɶ�����д�ȡ�24bit����8λ��Ч�������ڶ�·���ã�
    ptu32_t tag; // �û���ǩ�����û��趨�ñ�ǩ��;��
    ptu32_t context; // ˽�������ģ�
};

typedef struct __port tagOFile;

s32                       testcontent(u32);
s32                       testdirectory(u32);
s32                       testregular(u32);
s32                       testcreat(u32);
s32                       testonlycreat(u32);
s32                       testappend(u32 );
s32                       testtrunc(u32);
s32                       testreadable(u32);
s32                       testwriteable(u32);
s32                       isdirectory(tagOFile*);
s32                       isreadable(tagOFile*);
s32                       isappend(tagOFile *);
s32                       iswritable(tagOFile*);
s32                       isondutyo(struct Object*);
void                      setmevento(struct Object*, u32);
void                      clrmevento(struct Object*, u32);
void                      of_setmevent(tagOFile*, u32);
void                      of_clrmevent(tagOFile*, u32);
u32                       of_access(tagOFile*);
s32                       of_setaccess(tagOFile*, u32);
struct MultiplexObjectCB *of_mulitplex(tagOFile*);
void                      of_setmulitplex(tagOFile*, struct MultiplexObjectCB*);
ptu32_t                   of_context(tagOFile*);
void                      of_setcontext(tagOFile*, ptu32_t);
ptu32_t                   of_represent(tagOFile*);
void                      of_setrepresent(tagOFile*, ptu32_t);
u32                       of_timeout(tagOFile*);
void                      of_settimeout(tagOFile*, u32);
u32                       of_mode(tagOFile*);
void                      of_setmode(tagOFile*, u32);
tagOFile                 *of_new(void);
s32                       of_free(tagOFile*);
struct Object            *of_o(tagOFile*);
s32                       ofno(tagOFile*);
tagOFile                 *of(s32 dwFD);
s32                       of_linko(tagOFile*, struct Object*);
void                      of_init(tagOFile*, struct Object*, u32, ptu32_t);
void                     *of_core(tagOFile*);
struct Object            *of_o(tagOFile*);
const char               *of_name(tagOFile*);
s32                       openo(struct Object*);
struct Object            *basic2obj(struct __portBasic*);
struct __portBasic       *of_basic(tagOFile*);
void                      of_initbasic(struct __portBasic*, u32);
void                      of_unlinkbasic(struct __portBasic*);
struct __portBasic       *of_basiclinko(struct Object*);
s32                       of_virtualize(struct Object*, struct __portBasic*, char*);
struct Object            *of_virtualizeo(struct Object*, struct __portBasic*, char*);

#ifdef __cplusplus
}
#endif

#endif/* __OBJFILE_H__ */
