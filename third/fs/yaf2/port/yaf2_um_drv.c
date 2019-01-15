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

#include <lock.h>
#include <stdlib.h>
#include <device/include/unit_media.h>
#include "../yaffs2-583dbd9/yaffs2-583dbd9/direct/yportenv.h"
#include "../yaffs2-583dbd9/yaffs2-583dbd9/yaffs_guts.h"

struct __yaf2drv
{
    struct umedia * media;
    u32 chunkunits;
};

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
s32 __iserased(const u8 *buf, s32 datalen, s32 taglen)
{
    extern int yaffs_check_ff(u8 *buffer, int n_bytes);

    if(datalen && (!yaffs_check_ff((u8*)buf, datalen)))
        return (0);

    if(taglen && (!yaffs_check_ff((u8*)(buf+datalen), taglen)))
        return (0);

    return (1);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static inline struct __yaf2drv *__drv(struct yaffs_dev *yaf2dev)
{
    return ((struct __yaf2drv*)yaf2dev->driver_context);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：intag模式与oob是不会同时存在的，如果初始化时时intag模式，oob上不会存在内容；
// ============================================================================
static int __writechunk(struct yaffs_dev *yaf2dev, int chunk,
                        const u8 *data, int data_len,
                        const u8 *oob, int oob_len)
{
#if 0
    //逻辑：
    //   操作上带ECC的写，每页只能写一次的。

    struct FlashChip *Chip = Dev2Chip(dev);
    struct NandDescr *NandInfo = &Chip->Descr.Nand;
    struct FlashOps *NandOps = &Chip->Ops;
    u8 *Buf = ChipBuf(dev);
    u32 Flags = 0;
    s32 Ret;

#if 0// todo: yaffs调用时,存在data==NULL,data_len不为零的情况。
    if (((!data) && (data_len > 0)) || ((!oob) && (oob_len > 0)))
        return (YAFFS_FAIL);
#endif

    Lock_MutexPend((struct MutexLCB *)(Chip->Lock), CN_TIMEOUT_FOREVER);

    if (dev->param.use_nand_ecc)
    {
        //  硬件ECC优先使用
        if(NandInfo->Controller & HW_ECC_SUPPORTED)
            Flags |= HW_ECC;
        else if(NandInfo->Controller & SW_ECC_SUPPORTED)
            Flags |= SW_ECC;
        else
            Flags |= NO_ECC;
    }
    else
        Flags |= NO_ECC;

    if (data)
        memcpy(Buf, data, data_len);

    if (oob)
    {
        Flags |= SPARE_REQ;
        memcpy((Buf + NandInfo->BytesPerPage), oob, oob_len);
    }

    Ret = NandOps->WrPage(chunk, Buf, Flags);

    Lock_MutexPost((struct MutexLCB *)(Chip->Lock));

    if (Ret < 0)
    {
        //出错。
        return (YAFFS_FAIL);
    }

    return (YAFFS_OK);
#else
    struct uopt opt = {0};
    struct __yaf2drv *drv = __drv(yaf2dev);
    s32 i, spare = 0, res = YAFFS_OK, main;

    if((drv->chunkunits>1)&&(oob))
   {
       printf("\r\n: erro : yaffs2 : bad driver logic when write");
       return (YAFFS_FAIL);
   }

#if 0// todo: yaffs调用时,存在data==NULL,data_len不为零的情况。
    if (((!data) && (data_len > 0)) || ((!oob) && (oob_len > 0)))
        return (YAFFS_FAIL);
#endif

    if(yaf2dev->param.use_nand_ecc)
    {
        opt.hecc = 1;
        opt.secc = 1;
    }

    main = 1 << drv->media->usz;
    drv->media->mreq(sparebytes, (ptu32_t)&spare);
    drv->media->mreq(lock, -1);
    for(i = 0; i < drv->chunkunits; i++)
    {
        memset(drv->media->ubuf, 0xFF, main+spare);
        if(data)
        {
            opt.main = 1;
            memcpy(drv->media->ubuf, data, data_len);
            data += main;
        }

        if(oob)
        {
            opt.spare = 1;
            memcpy(drv->media->ubuf+main, oob, oob_len);
        }

        if(drv->media->mwrite((s64)chunk, (void*)drv->media->ubuf, opt) != 0)
        {
            res = YAFFS_FAIL;
            break;
        }
    }

    drv->media->mreq(unlock, 0);
    return (res);
#endif
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：intag模式与oob是不会同时存在的，如果初始化时时intag模式，oob上不会存在内容；
// ============================================================================
static int __readchunk(struct yaffs_dev *yaf2dev, int chunk,
                   u8 *data, int data_len,
                   u8 *oob, int oob_len,
                   enum yaffs_ecc_result *ecc_result_out)
{
#if 0
    struct FlashChip *Chip = Dev2Chip(dev);
    struct NandDescr *NandInfo = &Chip->Descr.Nand;
    struct FlashOps *NandOps = &Chip->Ops;
    u8 *Buf = ChipBuf(dev);
    u32 Flags = 0;
    s32 Ret;
    *ecc_result_out = YAFFS_ECC_RESULT_NO_ERROR;

#if 0// todo: 存在data==NULL,data_len不为零的情况。
    if (((!data) && (data_len > 0)) || ((!oob) && (oob_len > 0)))
        return (YAFFS_FAIL);
#endif

    Lock_MutexPend((struct MutexLCB *)(Chip->Lock), CN_TIMEOUT_FOREVER);

    if (dev->param.use_nand_ecc)
    {
        //  硬件ECC优先使用
        if(NandInfo->Controller & HW_ECC_SUPPORTED)
            Flags |= HW_ECC;
        else if(NandInfo->Controller & SW_ECC_SUPPORTED)
            Flags |= SW_ECC;
        else
            Flags |= NO_ECC;
    }
    else
        Flags |= NO_ECC;

    if (oob)
        Flags |= SPARE_REQ;

    Ret = NandOps->RdPage(chunk, Buf, Flags);
    if (Ret < 0)
    {
        // 出错。
        if ((-3 == Ret) && (!__iserased(Buf, NandInfo->BytesPerPage, oob_len)))
            *ecc_result_out = YAFFS_ECC_RESULT_UNFIXED;/* ECC校验失败(保证不是被擦除的块中的页) */
        else
            *ecc_result_out = YAFFS_ECC_RESULT_UNKNOWN;/* 读错误等,或者无错误（情景：ECC读一个刚被擦除的页） */
    }

    if (data)
        memcpy(data, Buf, data_len);

    if (oob)
        memcpy(oob, (Buf + NandInfo->BytesPerPage), oob_len);

    Lock_MutexPost((struct MutexLCB *)(Chip->Lock));

    if(Ret < 0)
        return (YAFFS_FAIL);/* 就算发生错误，也将数据传递回去(启动时,新块的ECC读肯定是错的) */

    return (YAFFS_OK);
#else
    struct uopt opt = {0};
    struct __yaf2drv *drv = __drv(yaf2dev);
    s32 i, spare = 0, res = YAFFS_OK, main;

    if((drv->chunkunits>1)&&(oob))
    {
        printf("\r\n: erro : yaffs2 : bad driver logic when read");
        *ecc_result_out = YAFFS_ECC_RESULT_UNKNOWN;
        return (YAFFS_FAIL);
    }

#if 0// todo: yaffs调用时,存在data==NULL,data_len不为零的情况。
    if (((!data) && (data_len > 0)) || ((!oob) && (oob_len > 0)))
        return (YAFFS_FAIL);
#endif



    if(yaf2dev->param.use_nand_ecc)
    {
        opt.hecc = 1;
        opt.secc = 1;
    }

    main = 1 << drv->media->usz;
    drv->media->mreq(sparebytes, (ptu32_t)&spare);
    drv->media->mreq(lock, -1);
    opt.main = 1;
    if(oob)
        opt.spare = 1;

    for(i = 0; i < drv->chunkunits; i++)
    {
        if(drv->media->mread((s64)chunk, (void*)drv->media->ubuf, opt) != 0)
        {
            if(!__iserased(drv->media->ubuf, main, oob_len))
                *ecc_result_out = YAFFS_ECC_RESULT_UNFIXED;/* ECC校验失败(保证不是被擦除的块中的页) */
            else
                *ecc_result_out = YAFFS_ECC_RESULT_UNKNOWN;/* 读错误等,或者无错误（情景：ECC读一个刚被擦除的页） */

            printf("\r\n: erro : yaffs2 : bad when read");
            res = YAFFS_FAIL;
            break;
        }

        if(data)
        {
            memcpy(data, drv->media->ubuf, main);
            data += main;
        }

        if(oob)
            memcpy(oob, (drv->media->ubuf+main), oob_len);
    }

    drv->media->mreq(unlock, 0);
    return (res);

#endif
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static int __eraseblock(struct yaffs_dev *yaf2dev, int block_no)
{
    struct uesz sz = {0};
    struct __yaf2drv *drv = __drv(yaf2dev);

    sz.block = 1;
    if(drv->media->merase(block_no, sz) != 0)
        return (YAFFS_FAIL);
    else
        return (YAFFS_OK);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static int __markbad(struct yaffs_dev *yaf2dev, int block_no)
{
    struct __yaf2drv *drv = __drv(yaf2dev);

    drv->media->mreq(markbad, block_no);
    return (YAFFS_OK);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static int __checkbad(struct yaffs_dev *yaf2dev, int block_no)
{
    struct __yaf2drv *drv = __drv(yaf2dev);

    if(drv->media->mreq(checkbad, block_no))
        return (YAFFS_FAIL);
    else
        return (YAFFS_OK);

}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static int __initialize(struct yaffs_dev *dev)
{
    return (YAFFS_OK);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static int __deinitialize(struct yaffs_dev *dev)
{
    return (YAFFS_OK);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
int yaf2_install_drv(struct yaffs_dev *yaf2dev, struct umedia *um, u32 splices)
{
    struct yaffs_driver *drv;
    struct __yaf2drv *__drv = (struct __yaf2drv*)malloc(sizeof(*__drv));

    if (!yaf2dev || !um)
         return (YAFFS_FAIL);

    drv = &(yaf2dev->drv);
    drv->drv_write_chunk_fn = __writechunk;
    drv->drv_read_chunk_fn = __readchunk;
    drv->drv_erase_fn = __eraseblock;
    drv->drv_mark_bad_fn = __markbad;
    drv->drv_check_bad_fn = __checkbad;
    drv->drv_initialise_fn = __initialize;
    drv->drv_deinitialise_fn = __deinitialize;
    __drv->media = um;
    if(!splices)
        __drv->chunkunits = 1;
    else
        __drv->chunkunits = splices;

    yaf2dev->driver_context = (void*)__drv;
    return (YAFFS_OK);
}
