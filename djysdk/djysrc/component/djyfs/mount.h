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

#ifndef __MOUNT_H__
#define __MOUNT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "vfile.h"

//
// MOUNT������
//
 #define MOUNT_TYPE_MASK                (0xF0)// �ļ�ϵͳ����
 #define MOUNT_DEV                      (0x00)// �豸�ļ�ϵͳ
 #define MOUNT_FILE                     (0x10)// ��ͨ�ļ�ϵͳ
 #define MOUNT_ON_DIR                   (0x01)// ���ص㱾���Ǹ�Ŀ¼
 #define MOUNT_ON_MOUNT                 (0x02)// ���ص㱾���Ǹ����ص�

//
// ��װ�����
//
struct VMount
{
    struct VOperations *VOps;
    struct Object *OldObj;
    u32 Property;
    void *Context;
    char *Name;// �ļ�ϵͳ����
    void *Private;// �����ļ�ϵͳ����,ָ��struct FileOperations
};


//
// �ļ�ϵͳ��װ����
//
struct MountInfo
{
    struct FileSysType *Type;
    struct VMount Mount;
    struct Object *Dev;
    void *Private;
    struct MountInfo *Next;
};

//
// �ļ�ϵͳ���ͳ���
//
struct FileSysType
{
    struct FileOperations *FileOps;
    struct VOperations *VOps;
    s32 (*Install)(struct MountInfo *Info, void *Private);
    s32 (*Uninstall)(struct MountInfo *Info);
    s32 (*Format)(void *Private);
    u32 Property;// �ļ�ϵͳ����
    struct MountInfo *MountList;
    char Name[];
};

//
// �ļ�ϵͳ��������ṹ��
//
struct FileSysTypeInfo
{
    struct FileSysTypeInfo *Next;
    struct FileSysType *Type;
};


//
// ��������
//
s32 RegisterFSType(struct FileSysType *NewType);
s32 Mount(const char *MountPath, const char *DevPath, const char *Type, void *Private);
s32 Unmount(const char *Path);
s32 UnRegisterFSType(const char *Type);
s32 SetPWD(const char *Path);
s32 CurWorkPathLen(void);
s32 CurWorkPath(char *Buf, u32 BufSize);
s32 Get(struct Object *Obj);
void Put(struct Object *Obj);

#ifdef __cplusplus
}
#endif

#endif /* __MOUNT_H__ */
