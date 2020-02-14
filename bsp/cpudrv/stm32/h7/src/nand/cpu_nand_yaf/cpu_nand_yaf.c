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
#include <string.h>
#include <stdlib.h>
#include <device.h>
#include <device/flash/flash.h>
#include <cpu_peri.h>
#include <djyos.h>
#include <math.h>
#include "stm32h7xx_hal_conf.h"
#include <dbug.h>
#include <djyfs/filesystems.h>
#include <device/include/unit_media.h>
#include <board.h>
#include <fs/yaf2/yaffs2-583dbd9/yaffs2-583dbd9/yaffs_guts.h>

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//   extern bool_t ModuleInstall_NandInstallYaf(const char *TargetFs,s32 bstart, s32 bend, u32 doformat);
//   ModuleInstall_NandInstallYaf(CFG_NFLASH_YAFFSMOUNT_NAME, CFG_NFLASH_YAF_PART_START,
//                                                  CFG_NFLASH_YAF_PART_END, CFG_NFLASH_YAF_PART_FORMAT);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"nand_install_yaf"//nand��װyaf
//parent:"cpu onchip nand"     //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"yaf2 file system","cpu onchip nand"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_NAND_INSTALL_YAF == false )
//#warning  " nand_install_yaf  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_NAND_INSTALL_YAF    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@string,1,10,
#define CFG_NFLASH_YAFFSMOUNT_NAME     "yaf2" //�谲װ���ļ�ϵͳ��mount������
//%$#@num,-1,2048
#define CFG_NFLASH_YAF_PART_START      0      //������ʼ����д��ţ���Ŵ�0��ʼ����
#define CFG_NFLASH_YAF_PART_END        -1     //����������-1��ʾ���һ��
//%$#@enum,true,false,
#define CFG_NFLASH_YAF_PART_FORMAT     false      //����ѡ��,�Ƿ���Ҫ��ʽ���÷�����
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure

extern struct NandDescr *__nandescription; // NAND��������;
extern struct umedia *nand_umedia;
extern bool_t Nandflash_is_install(void);
extern s32 __nand_FsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv);
int yaf_nand_write(struct yaffs_dev *yaf2dev, int chunk,
                        const u8 *data, int data_len,
                        const u8 *oob, int oob_len);
int yaf_nand_read(struct yaffs_dev *yaf2dev, int chunk,
                   u8 *data, int data_len,
                   u8 *oob, int oob_len,
                   enum yaffs_ecc_result *ecc_result_out);
int yaf_nand_eraseblock(struct yaffs_dev *yaf2dev, int block_no);
int yaf_nand_markbad(struct yaffs_dev *yaf2dev, int block_no);
int yaf_nand_checkbad(struct yaffs_dev *yaf2dev, int block_no);
int yaf_nand_initialize(struct yaffs_dev *dev);
int yaf_nand_deinitialize(struct yaffs_dev *dev);

struct yaffs_driver YAF_NAND_DRV =
{
    .drv_write_chunk_fn = yaf_nand_write,
    .drv_read_chunk_fn = yaf_nand_read,
    .drv_erase_fn = yaf_nand_eraseblock,
    .drv_mark_bad_fn = yaf_nand_markbad,
    .drv_check_bad_fn = yaf_nand_checkbad,
    .drv_initialise_fn = yaf_nand_initialize,
    .drv_deinitialise_fn = yaf_nand_deinitialize
};

// ============================================================================
// ���ܣ��жϸ������Ƿ����
// ������buf -- ��������������ݣ�datalen -- ���ݳ��ȣ�taglen -- oob����
// ���أ����� -- 1��δ���� -- 0��
// ��ע��
// ============================================================================
s32 yaf_nand_is_erased(const u8 *buf, s32 datalen, s32 taglen)
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
int yaf_nand_write(struct yaffs_dev *yaf2dev, int chunk,
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

    if(yaf2dev->param.use_nand_ecc)
    {
        opt.hecc = 1;
        opt.secc = 1;
    }

    main = __nandescription->BytesPerPage;
    __nand_req(sparebytes, (ptu32_t)&spare);
    __nand_req(lock, -1);
    for(i = 0; i < (s32)(yaf2dev->driver_context); i++)
    {
        memset(nand_umedia->ubuf, 0xFF, main+spare);
        if(data)
        {
            opt.main = 1;
            memcpy(nand_umedia->ubuf, data, data_len);
            data += main;
        }

        if(oob)
        {
            opt.spare = 1;
            memcpy(nand_umedia->ubuf+main, oob, oob_len);
        }

        if(__nand_write((s64)chunk, (void*)nand_umedia->ubuf, opt) != 0)
        {
            res = YAFFS_FAIL;
            break;
        }
    }

    __nand_req(unlock, 0);
    return (res);
}

// ============================================================================
// ���ܣ�ͨ��yaf�ļ�ϵͳ��flash�������
// ������yaf2dev -- yaf�������ṹ�� chunk -- flash��ҳ�ţ� data -- �������ݵĻ��棻 data_len -- ��д���ݳ���
//     oob -- eccУ���룻 oob_len -- eccУ���볤��
// ���أ��ɹ� -- YAFFS_OK��ʧ�� -- YAFFS_FAIL
// ��ע��intagģʽ��oob�ǲ���ͬʱ���ڵģ������ʼ��ʱʱintagģʽ��oob�ϲ���������ݣ�
// ============================================================================
int yaf_nand_read(struct yaffs_dev *yaf2dev, int chunk,
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



    if(yaf2dev->param.use_nand_ecc)
    {
        opt.hecc = 1;
        opt.secc = 1;
    }

    main = __nandescription->BytesPerPage;;
    __nand_req(sparebytes, (ptu32_t)&spare);
    __nand_req(lock, -1);
    opt.main = 1;
    if(oob)
        opt.spare = 1;

    for(i = 0; i < (s32)(yaf2dev->driver_context); i++)
    {
        if(__nand_read((s64)chunk, (void*)nand_umedia->ubuf, opt) != 0)
        {
            if(!yaf_nand_is_erased(nand_umedia->ubuf, main, oob_len))
                *ecc_result_out = YAFFS_ECC_RESULT_UNFIXED;/* ECCУ��ʧ��(��֤���Ǳ������Ŀ��е�ҳ) */
            else
                *ecc_result_out = YAFFS_ECC_RESULT_UNKNOWN;/* �������,�����޴����龰��ECC��һ���ձ�������ҳ�� */

            printf("\r\n: erro : yaffs2 : bad when read");
            res = YAFFS_FAIL;
            break;
        }

        if(data)
        {
            memcpy(data, nand_umedia->ubuf, main);
            data += main;
        }

        if(oob)
            memcpy(oob, (nand_umedia->ubuf+main), oob_len);
    }

    __nand_req(unlock, 0);
    return (res);
}

// ============================================================================
// ���ܣ�ͨ��yaf�ļ�ϵͳ����flash��ĳ���������
// ������yaf2dev -- yaf�������ṹ�� block_no -- ��Ҫ�����Ŀ��
// ���أ��ɹ� -- YAFFS_OK��ʧ�� -- YAFFS_FAIL
// ��ע��
// ============================================================================
int yaf_nand_eraseblock(struct yaffs_dev *yaf2dev, int block_no)
{
    struct uesz sz = {0};

    sz.block = 1;
    if(__nand_erase(block_no, sz) != 0)
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
int yaf_nand_markbad(struct yaffs_dev *yaf2dev, int block_no)
{
    __nand_req(markbad, block_no);
    return (YAFFS_OK);
}
// ============================================================================
// ���ܣ����ĳ���Ƿ��ǻ���
// ������
// ���أ�
// ��ע��
// ============================================================================
int yaf_nand_checkbad(struct yaffs_dev *yaf2dev, int block_no)
{
    if(__nand_req(checkbad, block_no))
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
int yaf_nand_initialize(struct yaffs_dev *yaf2dev)
{
    return (YAFFS_OK);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
int yaf_nand_deinitialize(struct yaffs_dev *yaf2dev)
{
    return (YAFFS_OK);
}

//-----------------------------------------------------------------------------
// ���ܣ���nand��װyaf
// ������  TargetFs -- Ҫ���ص��ļ�ϵͳ
//      parts -- ��������
//      TargetPart -- ָ��Ҫ�ҵ��ĸ������£�������0��ʼ
//      �������� -- ��ʼ�飬���������������������6����ʼ����0����÷���ʹ�õĿ�Ϊ0��1��2��3��4��5�飬�����������飩���Ƿ��ʽ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע���������֪��Ҫ��װʲô�ļ�ϵͳ�����߲���װ�ļ�ϵͳTargetFs��NULL��TargetPart��-1��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_NandInstallYaf(const char *TargetFs,s32 bstart, s32 bend, u32 doformat)
{
    struct FsCore *super;
    char *notfind;
    struct Object *targetobj;
    if(Nandflash_is_install() == true)
    {
        if((TargetFs != NULL) && (bstart != bend))
        {
            if(doformat)
            {
                struct uesz sz;
                sz.unit = 0;
                sz.block = 1;
                if(-1 == __nand_req(format, bstart , bend, &sz))
                {
                    warning_printf("yaf2"," Format failure.");
                }
            }
            targetobj = obj_matchpath(TargetFs, &notfind);
            if(notfind)
            {
                error_printf("nand"," not found need to install file system.");
                return false;
            }
            super = (struct FsCore *)obj_GetPrivate(targetobj);
            if(strcmp(super->pFsType->pType, "YAF2") == 0)
            {
                if(__nand_FsInstallInit(TargetFs,bstart,bend,&YAF_NAND_DRV) == 0)
                    return true;
            }
            error_printf("nand"," need to install file system not YAF2.");
        }
    }
    return false;
}
