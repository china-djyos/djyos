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

#ifndef __UNIT_MEDIA_H__
#define __UNIT_MEDIA_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "jedec.h"
#include "nor.h"
#include "nand.h"
#include "embedded_flash.h"


enum utype{
    nand = 1,
    nor,
    usb,
    emmc,
    mmc,
    sd,
    embed,
};

enum ucmd{
    remain = 1,     // 某unit的所属block，后续的unit数量req(remain, *left, *unit)
    whichblock,     // 某unit的属于block的号, req(whichblock, *block, *unit)
    totalblocks,    // 总块数, req(totalblocks, *blocks)
    totalSectors,   // 总扇区数, req(totalblocks, *blocks)
    unitbytes,      // 某unit的Bytes, req(unitbytes, *bytes)
    sparebytes,     // 某unit的spare区域的bytes，req(sparebytes,*bytes);
    blockunits,     // 一个块中的unit数量；req(blockunits, *units, block)
    format,         // 格式化某一段区域;req(format, start, counts, *esz)
    lock,
    unlock,
    markbad,        // 标记某块是坏块；req(markbad, block)
    checkbad,       // 某块是否是坏块；req(checkbad, block)
    savebads,       // 保存坏块表；req(savebads, *table)
    getbads,        // 获取坏块表；req(getbads, **table)
    mapaddr,        // 获取存储器映射地址，req(mapaddr, *addr)
};

struct uopt{
    u32 hecc:1;     // 硬件实现ECC
    u32 secc:1;     // 软件实现ECC
    u32 necc:1;     // 无ECC
    u32 main:1;     // 0 = 不写主区
    u32 spare:1;    // OOB空间
};

struct uesz{
    u32 unit:1;     // 1 = 操作页
    u32 block:1;    // 1 = 操作块
};

struct ustatistics{     //没见哪里用到
    u32 erases;
    u32 brases;
    u32 reads;
    u32 breads;
    u32 writes;
    u32 bwrites;
};

struct umedia{
    enum utype type;
    u8 *ubuf;
    s32 (*mreq)(enum ucmd cmd, ptu32_t args, ...); // 成功（0）；失败（-1）；
};

typedef s32 (*__um_req)(enum ucmd cmd, ptu32_t args, ...);

s32 um_add(const char *name, struct umedia *media);
s32 nandscanbads(__um_req req, u32 **table);
s32 nandvalidbads(u32 *badtable);
u32 *nandbuildbads(s32 (*um_req)(enum ucmd cmd, ptu32_t args, ...));
void nandbadfreeblock(u32 *badtable, u32 *block, s32 (*um_req)(enum ucmd cmd, ptu32_t args, ...));
void nandbadfreeunit(u32 *badtable, s64 *unit, __um_req req);

//s32 ModuleInstall_UnitMedia(s32(*dev_init)(u32 bstart, u32 bcount, u32 doformat), u8 parts, ...);

#ifdef __cplusplus
}
#endif

#endif // #ifndef __UNIT_MEDIA_H__
