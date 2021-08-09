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

#ifndef __OBJFILE_H__
#define __OBJFILE_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>
#include <include/list.h>
#include <fcntl.h>
#include <unistd.h>
#include <stat.h>
#include "object.h"

// ����Ϊ��������
// ����ϵͳ�У������read/write����Ľӿڣ�һ��object�������û�ʹ�ö�����
// read/write��һ���ļ���ϵͳ�У�ֻ��ռ��һ��object�ṹ��object����ֱ�ӱ����ʡ�
// �����ļ�����ʹ��struct objhandle���ļ����Ա���δ򿪣������ļ�ϵͳ�ľ���ʵ
// �֣�����ÿ�δ򿪶�����һ��struct objhandle�ṹ��Ҳ���ܹ���һ�� struct objhandle�ṹ��
// fd��struct objhandleһһ��Ӧ��
// ע�⣬һ���򿪵��ļ�����·���ϵ�Ŀ¼����ʹû�д򿪣�Ҳ�����һ��object��������
// ����struct objhandle�ṹ��

struct objhandle{
    list_t list;            // ����������Ķ���ľ������
    u32 flags;              // ��ǰ�ļ�������־���� O_RDONLY�ȣ���fcntl.h�ж��塣
    struct Object *HostObj;    // �����Ķ���
    u32 timeout;            // ͬ�����ʵ�timeout����λus��������1.19Сʱ��
                            //ע�⣬socket�����ã�posix������涨��
    struct MultiplexObjectCB * pMultiplexHead; // ��·����Ŀ�����ͷָ�룻
    u32 MultiplexEvents;    // ����ĵ�ǰ����״̬����ɶ�����д�ȡ�24bit����8λ��Ч��
                            // �����ڶ�·���ã�
    ptu32_t UserTag;        // �û���ǩ�����û��趨�ñ�ǩ��;��
    ptu32_t context;        // handle����������
};

#define CN_INVALID_FD  -1

s32 Handle2fd(struct objhandle *hdl);
struct objhandle *fd2Handle(s32 fd);
s32 Handle_FlagIsDirectory(u32 flags);
s32 Handle_FlagIsRegular(u32 flags);
s32 Handle_FlagIsCreate(u32 flags);
s32 Handle_FlagIsOnlyCreate(u32 flags);
s32 Handle_FlagIsAppend(u32 flags);
s32 Handle_FlagIsTrunc(u32 flags);
s32 Handle_FlagIsReadable(u32 flags);
s32 Handle_FlagIsWriteable(u32 flags);
s32 Handle_FlagIsBlockComplete(u32 flags);
s32 Handle_IsAppend(struct objhandle *hdl);
s32 Handle_IsReadable(struct objhandle *hdl);
s32 Handle_IsWritable(struct objhandle *hdl);
s32 iscontender(struct objhandle *hdl);
struct objhandle *Handle_New(void);
s32 Handle_Delete(struct objhandle *hdl);
void Handle_Init(struct objhandle *hdl, struct Object *ob, u32 flags, ptu32_t context);
const char *Handle_GetName(struct objhandle *hdl);
ptu32_t Handle_GetContext(struct objhandle *hdl);
void Handle_SetContext(struct objhandle *hdl, ptu32_t context);
ptu32_t handle_GetHostObjectPrivate(struct objhandle *hdl);
struct MultiplexObjectCB *__Handle_GetMultiplexHead(struct objhandle *hdl);
void __Handle_SetMultiplexHead(struct objhandle *hdl, struct MultiplexObjectCB *cb);
u32 handle_gettimeout(struct objhandle *hdl);
void Handle_SetTimeOut(struct objhandle *hdl, u32 timeout);
u32 Handle_GetMode(struct objhandle *hdl);
void Handle_SetMode(struct objhandle *hdl, u32 mode);
struct Object *Handle_GetHostObj(struct objhandle *hdl);
u32 Handle_MultiEvents(struct objhandle *hdl);
void Handle_SetMultiplexEvent(struct objhandle *hdl, u32 events);
void Handle_ClrMultiplexEvent(struct objhandle *hdl, u32 events);
#ifdef __cplusplus
}
#endif

#endif/* __OBJFILE_H__ */
