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
// ���ܣ�
// ������
// ���أ�
// ��ע��
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
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
static inline struct __yaf2drv *__drv(struct yaffs_dev *yaf2dev)
{
    return ((struct __yaf2drv*)yaf2dev->driver_context);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��intagģʽ��oob�ǲ���ͬʱ���ڵģ������ʼ��ʱʱintagģʽ��oob�ϲ���������ݣ�
// ============================================================================
static int __writechunk(struct yaffs_dev *yaf2dev, int chunk,
                        const u8 *data, int data_len,
                        const u8 *oob, int oob_len)
{
#if 0
    //�߼���
    //   �����ϴ�ECC��д��ÿҳֻ��дһ�εġ�

    struct FlashChip *Chip = Dev2Chip(dev);
    struct NandDescr *NandInfo = &Chip->Descr.Nand;
    struct FlashOps *NandOps = &Chip->Ops;
    u8 *Buf = ChipBuf(dev);
    u32 Flags = 0;
    s32 Ret;

#if 0// todo: yaffs����ʱ,����data==NULL,data_len��Ϊ��������
    if (((!data) && (data_len > 0)) || ((!oob) && (oob_len > 0)))
        return (YAFFS_FAIL);
#endif

    Lock_MutexPend((struct MutexLCB *)(Chip->Lock), CN_TIMEOUT_FOREVER);

    if (dev->param.use_nand_ecc)
    {
        //  Ӳ��ECC����ʹ��
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
        //����
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

#if 0// todo: yaffs����ʱ,����data==NULL,data_len��Ϊ��������
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
// ���ܣ�
// ������
// ���أ�
// ��ע��intagģʽ��oob�ǲ���ͬʱ���ڵģ������ʼ��ʱʱintagģʽ��oob�ϲ���������ݣ�
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

#if 0// todo: ����data==NULL,data_len��Ϊ��������
    if (((!data) && (data_len > 0)) || ((!oob) && (oob_len > 0)))
        return (YAFFS_FAIL);
#endif

    Lock_MutexPend((struct MutexLCB *)(Chip->Lock), CN_TIMEOUT_FOREVER);

    if (dev->param.use_nand_ecc)
    {
        //  Ӳ��ECC����ʹ��
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
        // ����
        if ((-3 == Ret) && (!__iserased(Buf, NandInfo->BytesPerPage, oob_len)))
            *ecc_result_out = YAFFS_ECC_RESULT_UNFIXED;/* ECCУ��ʧ��(��֤���Ǳ������Ŀ��е�ҳ) */
        else
            *ecc_result_out = YAFFS_ECC_RESULT_UNKNOWN;/* �������,�����޴����龰��ECC��һ���ձ�������ҳ�� */
    }

    if (data)
        memcpy(data, Buf, data_len);

    if (oob)
        memcpy(oob, (Buf + NandInfo->BytesPerPage), oob_len);

    Lock_MutexPost((struct MutexLCB *)(Chip->Lock));

    if(Ret < 0)
        return (YAFFS_FAIL);/* ���㷢������Ҳ�����ݴ��ݻ�ȥ(����ʱ,�¿��ECC���϶��Ǵ��) */

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

#if 0// todo: yaffs����ʱ,����data==NULL,data_len��Ϊ��������
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
                *ecc_result_out = YAFFS_ECC_RESULT_UNFIXED;/* ECCУ��ʧ��(��֤���Ǳ������Ŀ��е�ҳ) */
            else
                *ecc_result_out = YAFFS_ECC_RESULT_UNKNOWN;/* �������,�����޴����龰��ECC��һ���ձ�������ҳ�� */

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
// ���ܣ�
// ������
// ���أ�
// ��ע��
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
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
static int __markbad(struct yaffs_dev *yaf2dev, int block_no)
{
    struct __yaf2drv *drv = __drv(yaf2dev);

    drv->media->mreq(markbad, block_no);
    return (YAFFS_OK);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
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
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
static int __initialize(struct yaffs_dev *dev)
{
    return (YAFFS_OK);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
static int __deinitialize(struct yaffs_dev *dev)
{
    return (YAFFS_OK);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
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
