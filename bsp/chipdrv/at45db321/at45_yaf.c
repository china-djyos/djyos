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
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <cpu_peri.h>
#include <device/flash/flash.h>
#include <device/include/unit_media.h>
#include <device.h>
#include <spibus.h>
#include <systime.h>
#include <math.h>
#include "at45db321.h"
#include <dbug.h>
#include <filesystems.h>
#include <fs/yaf2/yaffs2-583dbd9/yaffs2-583dbd9/yaffs_guts.h>

extern struct NorDescr *nordescription;
extern struct umedia *at45_umedia;
extern s32 __at45_write(s64 unit, void *data, struct uopt opt);
extern s32 __at45_read(s64 unit, void *data, struct uopt opt);
extern s32 __at45_req(enum ucmd cmd, ptu32_t args, ...);
extern s32 __at45_erase(s64 unit, struct uesz sz);
extern s32 __iserased(const u8 *buf, s32 datalen, s32 taglen);

int yaf_at45_write(struct yaffs_dev *yaf2dev, int chunk,
                        const u8 *data, int data_len,
                        const u8 *oob, int oob_len);
int yaf_at45_read(struct yaffs_dev *yaf2dev, int chunk,
                   u8 *data, int data_len,
                   u8 *oob, int oob_len,
                   enum yaffs_ecc_result *ecc_result_out);
int yaf_at45_eraseblock(struct yaffs_dev *yaf2dev, int block_no);
int yaf_at45_markbad(struct yaffs_dev *yaf2dev, int block_no);
int yaf_at45_checkbad(struct yaffs_dev *yaf2dev, int block_no);
int yaf_at45_initialize(struct yaffs_dev *dev);
int yaf_at45_deinitialize(struct yaffs_dev *dev);

struct yaffs_driver YAF_AT45_DRV =
{
    .drv_write_chunk_fn = yaf_at45_write,
    .drv_read_chunk_fn = yaf_at45_read,
    .drv_erase_fn = yaf_at45_eraseblock,
    .drv_mark_bad_fn = yaf_at45_markbad,
    .drv_check_bad_fn = yaf_at45_checkbad,
    .drv_initialise_fn = yaf_at45_initialize,
    .drv_deinitialise_fn = yaf_at45_deinitialize
};

// ============================================================================
// 功能：通过yaf文件系统往flash里写数据
// 参数：yaf2dev -- yaf的驱动结构； chunk -- flash的页号； data -- 待写数据的缓存； data_len -- 待写数据长度
//     oob -- ecc校验码； oob_len -- ecc校验码长度
// 返回：成功 -- YAFFS_OK；失败 -- YAFFS_FAIL
// 备注：intag模式与oob是不会同时存在的，如果初始化时时intag模式，oob上不会存在内容；
// ============================================================================
int yaf_at45_write(struct yaffs_dev *yaf2dev, int chunk,
                        const u8 *data, int data_len,
                        const u8 *oob, int oob_len)
{
    struct uopt opt = {0};
    s32 i, spare = 0, res = YAFFS_OK, main;

    if(((s32)(yaf2dev->driver_context) > 1)&&(oob))
   {
       printf("\r\n: erro : yaffs2 : bad driver logic when write");
       return (YAFFS_FAIL);
   }

#if 0// todo: yaffs调用时,存在data==NULL,data_len不为零的情况。
    if (((!data) && (data_len > 0)) || ((!oob) && (oob_len > 0)))
        return (YAFFS_FAIL);
#endif


    main = nordescription->BytesPerPage;
    __at45_req(sparebytes, (ptu32_t)&spare);
    __at45_req(lock, -1);
    for(i = 0; i < (s32)(yaf2dev->driver_context); i++)
    {
        memset(at45_umedia->ubuf, 0xFF, main+spare);
        if(data)
        {
            memcpy(at45_umedia->ubuf, data, data_len);
            data += main;
        }

        if(oob)
        {
            memcpy(at45_umedia->ubuf+main, oob, oob_len);
        }

        if(__at45_write((s64)chunk, (void*)at45_umedia->ubuf, opt) != 0)
        {
            res = YAFFS_FAIL;
            break;
        }
    }

    __at45_req(unlock, 0);
    return (res);
}

// ============================================================================
// 功能：通过yaf文件系统读flash里的数据
// 参数：yaf2dev -- yaf的驱动结构； chunk -- flash的页号； data -- 待读数据的缓存； data_len -- 待写数据长度
//     oob -- ecc校验码； oob_len -- ecc校验码长度
// 返回：成功 -- YAFFS_OK；失败 -- YAFFS_FAIL
// 备注：intag模式与oob是不会同时存在的，如果初始化时时intag模式，oob上不会存在内容；
// ============================================================================
int yaf_at45_read(struct yaffs_dev *yaf2dev, int chunk,
                   u8 *data, int data_len,
                   u8 *oob, int oob_len,
                   enum yaffs_ecc_result *ecc_result_out)
{
    struct uopt opt = {0};
    s32 i, spare = 0, res = YAFFS_OK, main;

    if(((s32)(yaf2dev->driver_context) > 1)&&(oob))
    {
        printf("\r\n: erro : yaffs2 : bad driver logic when read");
        *ecc_result_out = YAFFS_ECC_RESULT_UNKNOWN;
        return (YAFFS_FAIL);
    }

#if 0// todo: yaffs调用时,存在data==NULL,data_len不为零的情况。
    if (((!data) && (data_len > 0)) || ((!oob) && (oob_len > 0)))
        return (YAFFS_FAIL);
#endif

    main = nordescription->BytesPerPage;;
    __at45_req(sparebytes, (ptu32_t)&spare);
    __at45_req(lock, -1);

    for(i = 0; i < (s32)(yaf2dev->driver_context); i++)
    {
        if(__at45_read((s64)chunk, (void*)at45_umedia->ubuf, opt) != 0)
        {
            if(!__iserased(at45_umedia->ubuf, main, oob_len))
                *ecc_result_out = YAFFS_ECC_RESULT_UNFIXED;/* ECC校验失败(保证不是被擦除的块中的页) */
            else
                *ecc_result_out = YAFFS_ECC_RESULT_UNKNOWN;/* 读错误等,或者无错误（情景：ECC读一个刚被擦除的页） */

            printf("\r\n: erro : yaffs2 : bad when read");
            res = YAFFS_FAIL;
            break;
        }

        if(data)
        {
            memcpy(data, at45_umedia->ubuf, main);
            data += main;
        }

        if(oob)
            memcpy(oob, (at45_umedia->ubuf+main), oob_len);
    }

    __at45_req(unlock, 0);
    return (res);
}

// ============================================================================
// 功能：通过yaf文件系统擦除flash里某个块的数据
// 参数：yaf2dev -- yaf的驱动结构； block_no -- 需要擦除的块号
// 返回：成功 -- YAFFS_OK；失败 -- YAFFS_FAIL
// 备注：
// ============================================================================
int yaf_at45_eraseblock(struct yaffs_dev *yaf2dev, int block_no)
{
    struct uesz sz = {0};

    sz.block = 1;
    if(__at45_erase(block_no, sz) != 0)
        return (YAFFS_FAIL);
    else
        return (YAFFS_OK);
}

// ============================================================================
// 功能：标记某块是坏块
// 参数：yaf2dev -- yaf的驱动结构； block_no -- 需要标记的块号
// 返回：成功 -- YAFFS_OK；失败 -- YAFFS_FAIL
// 备注：
// ============================================================================
int yaf_at45_markbad(struct yaffs_dev *yaf2dev, int block_no)
{
    __at45_req(markbad, block_no);
    return (YAFFS_OK);
}
// ============================================================================
// 功能：检查某块是否是坏块
// 参数：
// 返回：
// 备注：
// ============================================================================
int yaf_at45_checkbad(struct yaffs_dev *yaf2dev, int block_no)
{
    if(__at45_req(checkbad, block_no))
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
int yaf_at45_initialize(struct yaffs_dev *yaf2dev)
{
    return (YAFFS_OK);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
int yaf_at45_deinitialize(struct yaffs_dev *yaf2dev)
{
    return (YAFFS_OK);
}

