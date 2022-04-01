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

#ifndef __FILE_SYSTEMS_H__
#define __FILE_SYSTEMS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <object.h>
#include <objhandle.h>

#define DJYFS_PATH_BUFFER_SIZE          266

/*
 * These are the fs-independent mount-flags: up to 32 flags are supported
 */
//��������Linux����û��ȫʵ��
#define MS_RDONLY       1  /* Mount read-only */
#define MS_NOSUID       2  /* Ignore suid and sgid bits */
#define MS_NODEV        4  /* Disallow access to device special files */
#define MS_NOEXEC       8  /* Disallow program execution */
#define MS_SYNCHRONOUS  16  /* Writes are synced at once */
#define MS_REMOUNT      32  /* Alter flags of a mounted FS */
#define MS_MANDLOCK     64  /* Allow mandatory locks on an FS */
#define MS_DIRSYNC      128 /* Directory modifications are synchronous */
#define MS_NOATIME      1024    /* Do not update access times. */
#define MS_NODIRATIME   2048    /* Do not update directory access times */
#define MS_BIND         4096
#define MS_MOVE         8192
#define MS_REC          16384
#define MS_VERBOSE      32768   /* War is peace. Verbosity is silence.
                                MS_VERBOSE is deprecated. */
#define MS_SILENT       32768
#define MS_POSIXACL     (1<<16) /* VFS does not apply the umask */
#define MS_UNBINDABLE   (1<<17) /* change to unbindable */
#define MS_PRIVATE      (1<<18) /* change to private */
#define MS_SLAVE        (1<<19) /* change to slave */
#define MS_SHARED       (1<<20) /* change to shared */
#define MS_RELATIME     (1<<21) /* Update atime relative to mtime/ctime. */
#define MS_KERNMOUNT    (1<<22) /* this is a kern_mount call */
#define MS_I_VERSION    (1<<23) /* Update inode I_version field */
#define MS_ACTIVE       (1<<30)
#define MS_NOUSER       (1<<31)
#define MS_DIRECTMOUNT  (1<<29) //DJYOS���ӣ�ֱ��mount�����ص㣬������
#define MS_INSTALLFORMAT  256   //DJYOS���ӣ� ��װʱ���ʽ�������ļ�ϵͳ
#define MS_INSTALLUSE     512   //DJYOS���ӣ�ʹ��ʱ�ŷ�����װ����
#define MS_INSTALLCREAT (1<<24) //DJYOS���ӣ��ļ�ϵͳ������ʱ�����½�

struct FsCore;


struct filesystem{
    struct dListNode list;
    s32 (*fileOps)(void *opsTarget, u32 cmd, ptu32_t OpsArgs1,
                        ptu32_t OpsArgs2, ptu32_t OpsArgs3);
    s32 (*install)(struct FsCore *pFsCore, u32 dwOpt, void *data);
    s32 (*uninstall)();
    s32 (*format)(void *core);
    char *pType;
};

struct FsCore
{
    struct Object *pTarget;     // ���ص�
    void *MediaInfo;            // �ļ�ϵͳ�豸��Ϣ
    void *MediaDrv;             // �ļ�ϵͳ�豸����
    void *pCore;                // ������ļ�ϵͳ������Ϣ
    struct Object *MountBak;    // ��������ԭĿ¼���ݣ���� flags �� MS_DIRECTMOUNT
                                // ���������ÿ�
    struct filesystem *pFsType; // �����ļ�ϵͳ����
    u32 InstallWay;             // �ļ�ϵͳ�İ�װ��ʽ����MS_INSTALLCREAT��
    void *Config;               // �ļ�ϵͳ��˽������
    s64 MediaStart;             // ��ý���е���ʼunit,unitΪ��λ��
    s64 AreaSize;               // ����������ܴ�С��ByteΪ��λ��
};


s32 File_RegisterFs(struct filesystem *type);
bool_t File_GetEntirePath(struct Object *BaseObject, char * PathTail, char * EntirePath,
                     u32 BufSize);
bool_t File_IsDirectory(struct objhandle *hdl);
bool_t File_ObjIsMount(struct Object *obj);
s32 File_Mount(const char *source, const char *target, const char *type, u32 flags, void *data);
s32 File_UnMount(const char *target, const char *type);
void *File_Core(struct Object *ob);
s32 File_Format(const char *MountPath);
s32 __File_MountOps(void *opsTarget, u32 objcmd, ptu32_t OpsArgs1,
                       ptu32_t OpsArgs2, ptu32_t OpsArgs3);
void File_BeMedia(const char *source, const char *target);
#ifdef __cplusplus
}
#endif

#endif // __FILE_SYSTEMS_H__
