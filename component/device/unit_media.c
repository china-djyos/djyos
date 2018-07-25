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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
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
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <objfile.h>
#include <systime.h>
#include <device.h>
#include <string.h>
#include "unit_media.h"
#include "dbug.h"
struct umcontext{
    struct uopt opt;
    s64 offset;
};

#if 0
enum ustatstype{
    wr = 1,
    wrecc,
    rd,
    rdecc,
    erase,
};

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static void __um_stats(enum ustatstype type)
{
    ;
}
#endif

// ============================================================================
// 功能：打开unit media设备文件;
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 __um_open(tagOFile *of, u32 mode, u32 timeout)
{
    struct umcontext *mc;
    struct umedia *media;

    timeout = timeout;
    media = (struct umedia*)devfiledtag(of);
    mc = (struct umcontext*)malloc(sizeof(struct umcontext));
    if(!mc)
        return (-1);

    memset(mc, 0x0, sizeof(struct umcontext));
    mc->opt= media->opt;

    of_setcontext(of, (ptu32_t)mc);
    return (0);
}


// ============================================================================
// 功能：关闭unit media设备文件;
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 __um_close(tagOFile *of)
{
    struct umcontext *mc = (struct umcontext*)of_context(of);

    free(mc);
    return (0);
}

// ============================================================================
// 功能：写关闭unit media设备文件;
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 __um_write(tagOFile *of, u8 *data, u32 len, u32 offset, u32 timeout)
{
    s32 res, ret, page;
    s64 pos;
    s32 unaligns, unaligno;;
    u8 *dbuf, *buf;
    struct umedia *media;
    struct umcontext *mc;

    media = (struct umedia*)devfiledtag(of);
    mc = (struct umcontext*)(of);
    dbuf = media->ubuf;
    buf = (u8*)data;

    if(israw(of))
        pos = offset & (0xFFFFFFFF);
    else
        pos = mc->offset + offset;

//    if((pos > media->ustart + media->size) || ((pos+len) > media->start + media->size))
//    {
//        error_printf("device","write out of range<%s>.", of_name(of));
//        debug_printf("device","range from <%l> to <%l>.", media->start, (media->start+media->size));
//        return (0);
//    }

    res = len;
    media->mreq(lock, CN_TIMEOUT_FOREVER, 0);

    // 开始不对齐
    unaligno = pos & ((1 << media->asz) - 1); // unaligned offset
    unaligns = (1 << media->asz) - offset; // unaligned size
    page = pos >> media->asz;

    if(unaligno)
    {
        ret =  media->mread(page, dbuf, mc->opt);
        if(ret != media->asz)
        {
           goto __ERR_W;
        }

        memcpy(dbuf+unaligns, buf, unaligns);

        ret =  media->mwrite(page, dbuf, mc->opt);
        if(ret != media->asz)
        {
            goto __ERR_W;
        }
    }

    len -= unaligns;
    buf += unaligns;
    page ++;

    while(len >= media->asz)
    {
        ret =  media->mwrite(page, buf, mc->opt);
        if(ret != media->asz)
        {
            break;
        }

        len -= media->asz;
        buf += media->asz;
        page ++;
    }

    // 结束不对齐
    if(len)
    {
        ret =  media->mread(page, dbuf, mc->opt);
        if(ret != media->asz)
        {
           goto __ERR_W;
        }

        memcpy(dbuf, buf, len);
        ret =  media->mwrite(page, dbuf, mc->opt);
        if(ret != media->asz)
        {
            goto __ERR_W;
        }

        len -= len;
    }

__ERR_W:
    media->mreq(unlock, 0, 0);
    return (res-len);
}

// ============================================================================
// 功能：读unit media设备文件;
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 __um_read(tagOFile *of, u8 *data, u32 len, u32 offset, u32 timeout)
{
    s32 res, ret, page;
    s64 pos;
    s32 unaligns, unaligno;;
    u8 *dbuf, *buf;
    struct umedia *media;
    struct umcontext *mc;

    media = (struct umedia*)devfiledtag(of);
    mc = (struct umcontext*)(of);
    dbuf = media->ubuf;
    buf = (u8*)data;

    if(israw(of))
        pos = offset & (0xFFFFFFFF);
    else
        pos = mc->offset + offset;

//    if((pos > media->ustart + media->size) || ((pos+len) > media->ustart + media->size))
//    {
//        error_printf("device","read out of range<%s>.", of_name(of));
//        debug_printf("device","range from <%l> to <%l>.", media->start, (media->start+media->size));
//        return (0);
//    }

    res = len;
    media->mreq(lock, CN_TIMEOUT_FOREVER, 0);

    // 开始不对齐
    unaligno = pos & ((1 << media->asz) - 1); // unaligned offset
    unaligns = (1 << media->asz) - offset; // unaligned size
    page = pos >> media->asz;

    if(unaligno)
    {
        ret =  media->mread(page, dbuf, mc->opt);
        if(ret != media->asz)
        {
           goto __ERR_R;
        }

        memcpy(buf, dbuf+unaligns, unaligns);
    }

    len -= unaligns;
    buf += unaligns;
    page ++;

    while(len >= media->asz)
    {
        ret =  media->mread(page, buf, mc->opt);
        if(ret != media->asz)
        {
           goto __ERR_R;
        }

        len -= media->asz;
        buf += media->asz;
        page ++;
    }

    // 结束不对齐
    if(len)
    {
        ret =  media->mread(page, dbuf, mc->opt);
        if(ret != media->asz)
        {
           goto __ERR_R;
        }

        memcpy(buf, dbuf, len);
        len -= len;
    }

__ERR_R:
    media->mreq(unlock, 0, 0);
    return (res-len);
}

// ============================================================================
// 功能：控制unit media设备文件;
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 __um_cntl(tagOFile *of, u32 cmd, ptu32_t arg1, ptu32_t arg2)
{
    return (0);
}

// ============================================================================
// 功能：建立unit media设备类；
// 参数：
// 返回：
// 备注：
// ============================================================================
static inline struct Object *__isbuild(void)
{
    static u8 inited = 0;
    static struct Object *mm;

    if(inited)
        return (mm);

    mm = dev_grpaddo("unit media");
    if(!mm)
    {
        error_printf("device","cannot build \"memory media\".");
        return (NULL);
    }

    inited = 1;
    return (mm);
}

// ============================================================================
// 功能：添加unit media设备;
// 参数：
// 返回：
// 备注：
// ============================================================================
s32 um_add(const char *name, struct umedia *media)
{
    struct Object *mmo;

    mmo = __isbuild();
    if(!mmo)
    {
        error_printf("device","cannot add  \"%s\"<build>.", name);
        return (-1);
    }

    if(dev_addo(mmo, name, __um_open, __um_close, __um_write,
                __um_read, __um_cntl, (ptu32_t)media))
        return (0);

    error_printf("device","cannot add unit-media \"%s\".", name);
    return (-1);
}

