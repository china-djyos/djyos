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
// 这份授权条款，在使用者符合以下二条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、此三条件表列，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、此三条件表列，以及下述
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
//所属模块: C库
//作者：罗侍田
//版本：V1.0.0
//文件描述: stat相关函数声明。
//其他说明:
//修订历史:
//2. ...
//1. 日期  : 2014/12/31
//   修改人: 罗侍田
//   新版本号:
//   修改说明: 新建
//------------------------------------------------------
#ifndef __STAT_H__
#define __STAT_H__

#include <stdint.h>
#include <types.h>
#ifdef __cplusplus
extern "C"{
#endif

//方便第三方移植，内部逻辑未实现，对应struct stat的st_mode成员的0-11bit
//open 调用的第三个参数
#define   S_ISUID    0004000   //set-user-ID bit
#define   S_ISGID    0002000   //set-group-ID bit (see below)
#define   S_ISVTX    0001000   //sticky bit (see below)
#define   S_IRWXU    00700     //mask for file owner permissions
#define   S_IRUSR    00400     //owner has read permission
#define   S_IWUSR    00200     //owner has write permission
#define   S_IXUSR    00100     //owner has execute permission
#define   S_IRWXG    00070     //mask for group permissions
#define   S_IRGRP    00040     //group has read permission

#define   S_IWGRP    00020     //group has write permission
#define   S_IXGRP    00010     //group has execute permission
#define   S_IRWXO    00007     //mask for permissions for others (not in group)
#define   S_IROTH    00004     //others have read permission
#define   S_IWOTH    00002     //others have write permission
#define   S_IXOTH    00001     //others have execute permission

// 按LINUX逻辑
struct stat
{
    mode_t st_mode; // 文件对应的模式，文件，目录等(目前只用了低16位，15-12为文件类型，如S_IFSOCK，11-0为权限)
    ino_t st_ino; // inode节点号
    dev_t st_dev; // 设备号
    dev_t st_rdev; // 特殊设备号
    nlink_t st_nlink; // 文件的连接数
    uid_t st_uid; // 文件所有者
    gid_t st_gid; // 文件所有者对应的组
    off_t st_size; // 普通文件，对应的文件字节数
    blksize_t st_blksize; // 文件内容对应的块大小
    blkcnt_t st_blocks; // 伟建内容对应的块数量
    time_t st_atime; // 文件最后被访问的时间
    time_t st_mtime; // 文件内容最后被修改的时间
    time_t st_ctime; // 文件状态改变时间
};

#define S_ISREG(x)      ((x&S_IFMT)==S_IFREG?true:false) // is it a regular file
#define S_ISDIR(x)      ((x&S_IFMT)==S_IFDIR?true:false) // directory?
#define S_ISCHR(x)      ((x&S_IFMT)==S_IFCHR?true:false) // character device?
#define S_ISBLK(x)      ((x&S_IFMT)==S_IFBLK?true:false) // block device?
#define S_ISFIFO(x)     ((x&S_IFMT)==S_IFIFO?true:false) // FIFO (named pipe)?
#define S_ISLNK(x)      ((x&S_IFMT)==S_IFLNK?true:false) // symbolic link?  (Not in POSIX.1-1996.)
#define S_ISSOCK(x)     ((x&S_IFMT)==S_IFSOCK?true:false) // socket?  (Not in POSIX.1-1996.)
// 私有定义
#define S_ISFLOW(x)       ((x&S_IFMT)==S_IFFLOW?true:false) // 表示可以作为C文件可以作缓冲；

int   _EXFUN(stat, (const char *restrict, struct stat *restrict));
int   _EXFUN(fstat, (int, struct stat *restrict));
int   _EXFUN(lstat, (const char *restrict, struct stat *restrict));

#ifdef __cplusplus
}
#endif

#endif      //for __STAT_H__
