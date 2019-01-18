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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
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
//定义来自Linux，并没有全实现
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
#define MS_DIRECTMOUNT  (1<<29) //DJYOS增加：直接mount到挂载点，不备份
#define MS_INSTALLFORMAT  256   //DJYOS增加： 安装时会格式化整个文件系统
#define MS_INSTALLUSE     512   //DJYOS增加：使用时才发生安装动作
#define MS_INSTALLCREAT (1<<24) //DJYOS增加：文件系统不存在时，则新建

struct FsCore;


struct filesystem{
    struct dListNode list;
    s32 (*fileOps)(void *opsTarget, u32 cmd, ptu32_t OpsArgs1,
                        ptu32_t OpsArgs2, ptu32_t OpsArgs3);
    s32 (*install)(struct FsCore *pFsCore, u32 dwOpt, void *data);
    s32 (*uninstall)();
    s32 (*format)();
    char *pType;
} *pFileSystemTypes;

struct FsCore
{
    struct obj *pTarget;        // 挂载点
    ptu32_t Media;              // 文件系统设备
    void *pCore;                // 具体的文件系统控制信息
    struct obj *MountBak;       // 用于隐藏原目录内容，如果 flags 带 MS_DIRECTMOUNT
                                // 参数，则置空
    struct filesystem *pFsType; // 所属文件系统类型
    u32 InstallWay;             // 文件系统的安装方式，如MS_INSTALLCREAT；
    void *Config;               // 文件系统的私有配置
    s64 MediaStart;             // 在媒体中的起始unit,unit为单位；
    s64 AreaSize;               // 所在区域的总大小；Byte为单位；
};


s32 regfs(struct filesystem *type);
bool_t GetEntirePath(struct obj *BaseObject, char * PathTail, char * EntirePath,
                     u32 BufSize);
bool_t obj_isMount(struct obj *obj);
s32 mountfs(const char *source, const char *target, const char *type, u32 flags, void *data);
void *corefs(struct obj *ob);
s32 __mount_ops(void *opsTarget, u32 objcmd, ptu32_t OpsArgs1,
                       ptu32_t OpsArgs2, ptu32_t OpsArgs3);
void FsBeMedia(const char *source, const char *target);
#ifdef __cplusplus
}
#endif

#endif // __FILE_SYSTEMS_H__
