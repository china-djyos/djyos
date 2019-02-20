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
#ifndef __EFS_H__
#define __EFS_H__

#ifdef __cplusplus
extern "C" {
#endif
//
//
//
#include "stddef.h"

#define ESIG_LEN                20
#define ESIGN                   "easy file system v0" // 包括结束符是20个字节
#define SYS_LOOPS               6
#define SYS_TMP                 ((u64)-1)
#define BUFLEN                  (252) // 文件缓存大小,每写这么多数据会存一个4字节的ECC值
#define BUFBITS                 (8)
#define KEYLEN                  (6) // 文件key值大小；
//
// efs文件系统对flash的操作函数集
//
struct __efs_drv{
    s32 (*efs_write_media) (struct __ecore *core, s64 units, void *buf);
    s32 (*efs_read_media) (struct __ecore *core, s64 units, void *buf);
    s32 (*efs_erase_media) (struct __ecore *core, s64 units, s64 size);
};
//
// 文件系统控制结构体
//
struct __ecore{
    u32 ssz; // 系统信息大小,（包括文件系统信息、idx表和文件信息；块对齐，以unit为单位）；
    u8 serial; // 当前使用的系统信息区（包括系统和文件）;
    u8 finfo; // 文件信息所在的偏置；（以unit为单位；）
    u8 ecc[256]; // 用于ECC校验；
    u8 eccu; // 一个unit内的文件内容被ECC占用的空间，（以字节为单位）
    u8 *ebuf; // 缓冲，media的unit大小
    u16 nsz; // 文件名长度
    u32 fmax; // 可容纳文件数最大值（最大时是一个文件块存放一个文件）；
    u32 fsz; // 文件块容量;（块对齐，以unit为单位）；
    s64 MStart;             // 在媒体中的起始unit,unit为单位；
    s64 ASize;               // 所在区域的总大小；Byte为单位；
    struct umedia *media;
    struct MutexLCB *mutex;
    struct __efs_drv *drv;
};

//
// 索引
//
struct __eidx{
    u8 key[KEYLEN];
    u8 bakup[KEYLEN];
    u32 order;
    u32 ecc;
};

//
// 文件系统信息（介质中）
//
struct __ecstruct{
    char signature[ESIG_LEN];
    s64 range; // 文件系统总体容量
    u32 files; // 文件数；
    u64 age; //
    u32 ecc; // 前面的CRC校验值；
    u32 status; //
#define SYS_UPDATING        0xFFFFFFFF
#define SYS_UPDATED         0x55555555
#define SYS_NOECCSIZE       8
};

//
// 文件信息
//
struct __esize{
    s64 s;
    u32 e;
};

struct __ename{
    char *n;
    u32 *e;
};

struct __efstruct{
    struct __ename name; //
    struct __esize *size; //
};

//
// 文件内容位置
//
struct __loc{
    list_t list;
    u32 loc;
    u32 order; // 从一计
};

//
// 接入文件系统结构
//
struct __efile{
//    struct __portBasic basic; // 接入文件系统用
    u32 flags; // 文件控制标志位；
    s64 size;
    struct __loc *loc; // 文件所在位置；即idx的偏置；
};

struct __econtext{
    u8 wbuf[BUFLEN];
    u8 wpos; // wbuf使用量；
    u8 dirty; // 缓存数据是否已被改写；
    s64 pos; // 当前位置为wpos+pos;
};

//
// 扫描逻辑
//
struct __scanlogic{
    enum escan{
        nosys,
        badsys,
        brokensys,
        goodsys,
    }stat;
    u32 age;
};

enum locmove{
    position,
    direct,
};

struct __dentry{
    char *name;
    struct __dentry *nxt;
};

struct __dentrys{
    u32 items;
    u32 scans;
    struct __dentry *item;
};

#ifdef __cplusplus
}
#endif

#endif
