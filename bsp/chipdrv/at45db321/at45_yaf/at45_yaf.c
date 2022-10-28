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
#include <device/djy_flash.h>
#include <device/unit_media.h>
#include <device.h>
#include <spibus.h>
#include <systime.h>
#include <math.h>
#include "at45db321.h"
#include <dbug.h>
#include <djyfs/filesystems.h>
#include <yaf2/yaffs2-583dbd9/yaffs2-583dbd9/yaffs_guts.h>

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_At45InstallYaf(const char *TargetFs, s32 bstart, s32 bend, u32 doformat);
//    ModuleInstall_At45InstallYaf(CFG_AT45_YAF_MOUNT_NAME,
//              CFG_AT45_YAF_PART_START, CFG_AT45_YAF_PART_END, CFG_AT45_YAF_PART_FORMAT);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"at45db321 install yaf"    //��norflash�ϰ�װyaf�ļ�ϵͳ
//parent:"nor flash at45db321"                //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"yaf2 file system","lock","spi bus","cpu onchip spi","at45db321"
                                //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_AT45DB321_INSTALL_YAF == false )
//#warning  " at45db321_install_yaf  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_AT45DB321_INSTALL_YAF    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@enum,512,528,
//%$#@string,1,10,
#define CFG_AT45_YAF_MOUNT_NAME            "yaf2"      //"�ļ�ϵͳmount������",��Ҫ���ص�yaf�ļ�ϵͳmount������
//%$#@num,-1,1024,
#define CFG_AT45_YAF_PART_START                  0        //������ʼ����д��ţ���Ŵ�0��ʼ����
#define CFG_AT45_YAF_PART_END                   -1        //����������-1��ʾ���һ��
//%$#@enum,true,false,
#define CFG_AT45_YAF_PART_FORMAT               false      //����ѡ��,�Ƿ���Ҫ��ʽ���÷�����
//%$#@select
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

extern struct NorDescr *nordescription;
extern struct umedia *at45_umedia;
extern bool_t At45_is_install(void);
extern s32 __at45_write(u32 unit, void *data, struct uopt opt);
extern s32 __at45_read(u32 unit, void *data, struct uopt opt);
extern s32 __at45_req(enum ucmd cmd, ptu32_t args, ...);
extern s32 __at45_erase(u32 unit, struct uesz sz);
extern s32 __AT45_FsInstallInit(const char *fs, s32 dwStart, s32 dwEnd,void *mediadrv);

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
// ���ܣ��жϸ������Ƿ����
// ������buf -- ��������������ݣ�datalen -- ���ݳ��ȣ�taglen -- oob����
// ���أ����� -- 1��δ���� -- 0��
// ��ע��
// ============================================================================
s32 yaf_at45_is_erased(const u8 *buf, s32 datalen, s32 taglen)
{
    s32 alllen = datalen + taglen;
    while (alllen--)
    {
        if (*buf != 0xff)
            return 0;
        buf++;
    }
    return 1;
}

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

        if(__at45_write(chunk, (void*)at45_umedia->ubuf, opt) != 0)
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
        if(__at45_read(chunk, (void*)at45_umedia->ubuf, opt) != 0)
        {
            if(!yaf_at45_is_erased(at45_umedia->ubuf, main, oob_len))
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

// =============================================================================
// ���ܣ���at45�а�װyaf�ļ�ϵͳ
// ������   pBusName -- AT45��Ҫ�õ�ͨ����
//      TargetFs -- Ҫ���ص��ļ�ϵͳ
//      �������� -- ��ʼ�飬��������������ʱ�������ÿ飬ֻ�����ÿ����һ�飩���Ƿ��ʽ����
// ���أ��ɹ���true����ʧ�ܣ�false����
// ��ע��
// =============================================================================
bool_t ModuleInstall_At45InstallYaf (const char *TargetFs, s32 bstart, s32 bend, u32 doformat)
{
    struct FsCore *super;
    char *notfind;
    struct Object *targetobj;
    if(At45_is_install() == true)
    {
        if((TargetFs != NULL) && (bstart != bend))
        {
            if(doformat)
            {
                struct uesz sz;
                sz.unit = 0;
                sz.block = 1;
                if(-1 == __at45_req(format, bstart , bend, &sz))
                {
                    warning_printf("yaf2"," Format failure.\r\n");
                }
            }
            targetobj = OBJ_MatchPath(TargetFs, &notfind);
            if(notfind)
            {
                error_printf("at45"," not found need to install file system.\r\n");
                return false;
            }
            super = (struct FsCore *)OBJ_GetPrivate(targetobj);
            if(strcmp(super->pFsType->pType, "YAF2") == 0)
            {
                if(__AT45_FsInstallInit(TargetFs,bstart,bend,&YAF_AT45_DRV) == 0)
                    return true;
            }
            error_printf("at45"," need to install file system not YAF2.\r\n");
        }
    }

    return false;
}
