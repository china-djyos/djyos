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
// ���ܣ�ͨ��yaf�ļ�ϵͳ��flash��д����
// ������yaf2dev -- yaf�������ṹ�� chunk -- flash��ҳ�ţ� data -- ��д���ݵĻ��棻 data_len -- ��д���ݳ���
//     oob -- eccУ���룻 oob_len -- eccУ���볤��
// ���أ��ɹ� -- YAFFS_OK��ʧ�� -- YAFFS_FAIL
// ��ע��intagģʽ��oob�ǲ���ͬʱ���ڵģ������ʼ��ʱʱintagģʽ��oob�ϲ���������ݣ�
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

#if 0// todo: yaffs����ʱ,����data==NULL,data_len��Ϊ��������
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
// ���ܣ�ͨ��yaf�ļ�ϵͳ��flash�������
// ������yaf2dev -- yaf�������ṹ�� chunk -- flash��ҳ�ţ� data -- �������ݵĻ��棻 data_len -- ��д���ݳ���
//     oob -- eccУ���룻 oob_len -- eccУ���볤��
// ���أ��ɹ� -- YAFFS_OK��ʧ�� -- YAFFS_FAIL
// ��ע��intagģʽ��oob�ǲ���ͬʱ���ڵģ������ʼ��ʱʱintagģʽ��oob�ϲ���������ݣ�
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

#if 0// todo: yaffs����ʱ,����data==NULL,data_len��Ϊ��������
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
                *ecc_result_out = YAFFS_ECC_RESULT_UNFIXED;/* ECCУ��ʧ��(��֤���Ǳ������Ŀ��е�ҳ) */
            else
                *ecc_result_out = YAFFS_ECC_RESULT_UNKNOWN;/* �������,�����޴����龰��ECC��һ���ձ�������ҳ�� */

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
// ���ܣ�ͨ��yaf�ļ�ϵͳ����flash��ĳ���������
// ������yaf2dev -- yaf�������ṹ�� block_no -- ��Ҫ�����Ŀ��
// ���أ��ɹ� -- YAFFS_OK��ʧ�� -- YAFFS_FAIL
// ��ע��
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
// ���ܣ����ĳ���ǻ���
// ������yaf2dev -- yaf�������ṹ�� block_no -- ��Ҫ��ǵĿ��
// ���أ��ɹ� -- YAFFS_OK��ʧ�� -- YAFFS_FAIL
// ��ע��
// ============================================================================
int yaf_at45_markbad(struct yaffs_dev *yaf2dev, int block_no)
{
    __at45_req(markbad, block_no);
    return (YAFFS_OK);
}
// ============================================================================
// ���ܣ����ĳ���Ƿ��ǻ���
// ������
// ���أ�
// ��ע��
// ============================================================================
int yaf_at45_checkbad(struct yaffs_dev *yaf2dev, int block_no)
{
    if(__at45_req(checkbad, block_no))
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
int yaf_at45_initialize(struct yaffs_dev *yaf2dev)
{
    return (YAFFS_OK);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
int yaf_at45_deinitialize(struct yaffs_dev *yaf2dev)
{
    return (YAFFS_OK);
}

