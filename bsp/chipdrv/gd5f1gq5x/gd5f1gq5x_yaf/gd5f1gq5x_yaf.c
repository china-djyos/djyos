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
#include <device/djy_flash.h>
#include <cpu_peri.h>
#include <djyos.h>
#include <math.h>
#include "stm32f7xx_hal_conf.h"
#include <dbug.h>
#include <djyfs/filesystems.h>
#include <device/unit_media.h>
#include <board.h>
#include <gd5f1gq5x.h>
#include <yaf2/yaffs2-583dbd9/yaffs2-583dbd9/yaffs_guts.h>


//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//   extern bool_t ModuleInstall_Gd5f1gInstallYaf(const char *TargetFs,s32 bstart, s32 bend, u32 doformat);
//   ModuleInstall_Gd5f1gInstallYaf(CFG_GD5F1G_YAFFSMOUNT_NAME, CFG_GD5F1G_YAF_PART_START,
//                                                  CFG_GD5F1G_YAF_PART_END, CFG_GD5F1G_YAF_PART_FORMAT);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"gd5f1g install yaf"//nand��װyaf
//parent:"gd5f1gq5x"     //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"yaf2 file system","gd5f1gq5x"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_GD5F1G_INSTALL_YAF == false )
//#warning  " gd5f1g install yaf  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_GD5F1G_INSTALL_YAF    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@string,1,10,
#define CFG_GD5F1G_YAFFSMOUNT_NAME     "yaf2" //�谲װ���ļ�ϵͳ��mount������
//%$#@num,-1,2048
#define CFG_GD5F1G_YAF_PART_START      1      //������ʼ����д��ţ���Ŵ�0��ʼ����,��0���д滵����Ϣ
#define CFG_GD5F1G_YAF_PART_END        -1     //����������-1��ʾ���һ��
//%$#@enum,true,false,
#define CFG_GD5F1G_YAF_PART_FORMAT     false      //����ѡ��,�Ƿ���Ҫ��ʽ���÷�����
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure


extern struct umedia *nand_umedia;

int Gd5f1g_YafWrite(struct yaffs_dev *yaf2dev, int chunk,
                        const u8 *data, int data_len,
                        const u8 *oob, int oob_len);
int Gd5f1g_YafRead(struct yaffs_dev *yaf2dev, int chunk,
                   u8 *data, int data_len,
                   u8 *oob, int oob_len,
                   enum yaffs_ecc_result *ecc_result_out);
int Gd5f1g_YafEraseBlock(struct yaffs_dev *yaf2dev, int block_no);
int Gd5f1g_YafMarkBad(struct yaffs_dev *yaf2dev, int block_no);
int Gd5f1g_YafCheckBad(struct yaffs_dev *yaf2dev, int block_no);
int Gd5f1g_YafInitialize(struct yaffs_dev *dev);
int Gd5f1g_YafDeinitialize(struct yaffs_dev *dev);

struct yaffs_driver YAF_NAND_DRV =
{
    .drv_write_chunk_fn = Gd5f1g_YafWrite,
    .drv_read_chunk_fn = Gd5f1g_YafRead,
    .drv_erase_fn = Gd5f1g_YafEraseBlock,
    .drv_mark_bad_fn = Gd5f1g_YafMarkBad,
    .drv_check_bad_fn = Gd5f1g_YafCheckBad,
    .drv_initialise_fn = Gd5f1g_YafInitialize,
    .drv_deinitialise_fn = Gd5f1g_YafDeinitialize
};

// ============================================================================
// ���ܣ��жϸ������Ƿ����
// ������buf -- ��������������ݣ�datalen -- ���ݳ��ȣ�taglen -- oob����
// ���أ����� -- 1��δ���� -- 0��
// ��ע��
// ============================================================================
s32 Gd5f1g_YafIsErased(const u8 *buf, s32 datalen, s32 taglen)
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
int Gd5f1g_YafWrite(struct yaffs_dev *yaf2dev, int chunk,
                        const u8 *data, int data_len,
                        const u8 *oob, int oob_len)
{
//    struct uopt opt = {0};
    u32 flags = 0;
    s32 i, spare = 0, res = YAFFS_OK, main;

    if(((s32)(yaf2dev->driver_context) > 1)&&(oob))
   {
       printf("\r\n: erro : yaffs2 : bad driver logic when write");
       return (YAFFS_FAIL);
   }

    if(yaf2dev->param.use_nand_ecc)
    {
        flags |= HW_ECC;
    }
    Gd5f1g_Ctrl(unitbytes, (ptu32_t)&main);
    Gd5f1g_Ctrl(sparebytes, (ptu32_t)&spare);
    for(i = 0; i < (s32)(yaf2dev->driver_context); i++)
    {
        memset(nand_umedia->ubuf, 0xFF, main+spare);
        if(data)
        {
            memcpy(nand_umedia->ubuf, data, data_len);
//            data += main;
        }
        else
            warning_printf("gd5f1g", "only write obb.\r\n");

        if(oob)
        {
            flags |= SPARE_REQ;
            memcpy(nand_umedia->ubuf+main, oob, oob_len);
        }

        if(Gd5f1g_WritePage((u32)chunk, nand_umedia->ubuf, flags) == 0)
        {
            res = YAFFS_FAIL;
            break;
        }
    }

    return (res);
}

// ============================================================================
// ���ܣ�ͨ��yaf�ļ�ϵͳ��flash�������
// ������yaf2dev -- yaf�������ṹ�� chunk -- flash��ҳ�ţ� data -- �������ݵĻ��棻 data_len -- ��д���ݳ���
//     oob -- eccУ���룻 oob_len -- eccУ���볤��
// ���أ��ɹ� -- YAFFS_OK��ʧ�� -- YAFFS_FAIL
// ��ע��intagģʽ��oob�ǲ���ͬʱ���ڵģ������ʼ��ʱʱintagģʽ��oob�ϲ���������ݣ�
// ============================================================================
int Gd5f1g_YafRead(struct yaffs_dev *yaf2dev, int chunk,
                   u8 *data, int data_len,
                   u8 *oob, int oob_len,
                   enum yaffs_ecc_result *ecc_result_out)
{
//    struct uopt opt = {0};
    u32 flags = 0;
    s32 i, spare = 0, res = YAFFS_OK, main;

    if(((s32)(yaf2dev->driver_context) > 1)&&(oob))
    {
        printf("\r\n: erro : yaffs2 : bad driver logic when read");
        *ecc_result_out = YAFFS_ECC_RESULT_UNKNOWN;
        return (YAFFS_FAIL);
    }

    if(yaf2dev->param.use_nand_ecc)
    {
        flags |= HW_ECC;
    }

    Gd5f1g_Ctrl(unitbytes, (ptu32_t)&main);
    Gd5f1g_Ctrl(sparebytes, (ptu32_t)&spare);
    if(oob)
        flags |= SPARE_REQ;

    for(i = 0; i < (s32)(yaf2dev->driver_context); i++)
    {
        if(Gd5f1g_ReadPage((u32)chunk, nand_umedia->ubuf, flags) == 0)
        {
            if(!Gd5f1g_YafIsErased(nand_umedia->ubuf, main, oob_len))
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
//            data += main;
        }

        if(oob)
            memcpy(oob, (nand_umedia->ubuf+main), oob_len);
    }

    return (res);
}

// ============================================================================
// ���ܣ�ͨ��yaf�ļ�ϵͳ����flash��ĳ���������
// ������yaf2dev -- yaf�������ṹ�� block_no -- ��Ҫ�����Ŀ��
// ���أ��ɹ� -- YAFFS_OK��ʧ�� -- YAFFS_FAIL
// ��ע��
// ============================================================================
int Gd5f1g_YafEraseBlock(struct yaffs_dev *yaf2dev, int block_no)
{
    if(Gd5f1g_EraseBlock((u32)block_no) != 0)
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
int Gd5f1g_YafMarkBad(struct yaffs_dev *yaf2dev, int block_no)
{
    if(Gd5f1g_Ctrl(markbad, block_no) != 0)
        return (YAFFS_FAIL);
    else
        return (YAFFS_OK);
}

// ============================================================================
// ���ܣ����ĳ���Ƿ��ǻ���
// ������
// ���أ�
// ��ע��
// ============================================================================
int Gd5f1g_YafCheckBad(struct yaffs_dev *yaf2dev, int block_no)
{
    if(Gd5f1g_Ctrl(checkbad, block_no))
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
int Gd5f1g_YafInitialize(struct yaffs_dev *yaf2dev)
{
    return (YAFFS_OK);
}


// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
int Gd5f1g_YafDeinitialize(struct yaffs_dev *yaf2dev)
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
bool_t ModuleInstall_Gd5f1gInstallYaf(const char *TargetFs,s32 bstart, s32 bend, u32 doformat)
{
    struct FsCore *super;
    char *notfind;
    struct Object *targetobj;
    if(Gd5f1g_IsInstall())
    {
        if((TargetFs != NULL) && (bstart != bend))
        {
            if(doformat)
            {
                if(-1 == Gd5f1g_Ctrl(format, bstart , bend))
                {
                    warning_printf("yaf2"," Format failure.\r\n");
                }
            }
            targetobj = OBJ_MatchPath(TargetFs, &notfind);
            if(notfind)
            {
                error_printf("nand"," not found need to install file system.\r\n");
                return false;
            }
            super = (struct FsCore *)OBJ_GetPrivate(targetobj);
            if(strcmp(super->pFsType->pType, "YAF2") == 0)
            {
                if(__Gd5f1g_FsInstallInit(TargetFs,bstart,bend,&YAF_NAND_DRV) == 0)
                    return true;
            }
            error_printf("nand"," need to install file system not YAF2.\r\n");
        }
    }
    return false;
}
