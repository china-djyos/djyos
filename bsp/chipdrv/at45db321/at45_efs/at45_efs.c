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
#include <djyfs/efs.h>

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_At45InstallEfs(const char *TargetFs, s32 bstart, s32 bend, u32 doformat);
//    ModuleInstall_At45InstallEfs(CFG_AT45_EFS_MOUNT_NAME,
//                  CFG_AT45_EFS_PART_START, CFG_AT45_EFS_PART_END, CFG_AT45_EFS_PART_FORMAT);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"at45db321 install efs"    //��norflash�ϰ�װefs�ļ�ϵͳ
//parent:"nor flash at45db321"                //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"easy file system","lock","spi bus","cpu onchip spi","at45db321"
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
#if ( CFG_MODULE_ENABLE_AT45DB321_INSTALL_EFS == false )
//#warning  " at45db321_install_efs  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_AT45DB321_INSTALL_EFS    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@enum,512,528,
//%$#@string,1,10,
#define CFG_AT45_EFS_MOUNT_NAME            "efs"      //"�ļ�ϵͳmount������",��Ҫ���ص�efs�ļ�ϵͳmount������
//%$#@num,-1,1024,
#define CFG_AT45_EFS_PART_START                  0        //������ʼ����д��ţ���Ŵ�0��ʼ����
#define CFG_AT45_EFS_PART_END                   -1        //����������-1��ʾ���һ��
//%$#@enum,true,false,
#define CFG_AT45_EFS_PART_FORMAT               false      //����ѡ��,�Ƿ���Ҫ��ʽ���÷�����
//%$#@select
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

extern bool_t At45_is_install(void);
extern s32 __at45_write(u32 unit, void *data, struct uopt opt);
extern s32 __at45_read(u32 unit, void *data, struct uopt opt);
extern s32 __at45_erase(u32 unit, struct uesz sz);
extern s32 __at45_req(enum ucmd cmd, ptu32_t args, ...);
extern s32 __AT45_FsInstallInit(const char *fs, s32 dwStart, s32 dwEnd,void *mediadrv);
u32 efs_at45_write(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize, u8 bFlags);
u32 efs_at45_read(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize, u8 bFlags);
bool_t efs_at45_erase(u32 dwBlock);
bool_t efs_at45_CheckBlockReady(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize);

extern struct NorDescr *nordescription;;

struct __efs_media_drv EFS_AT45_DRV =
{
    .efs_erase_media = efs_at45_erase,
    .efs_read_media = efs_at45_read,
    .efs_write_media = efs_at45_write,
    .efs_check_block_ready = efs_at45_CheckBlockReady
};

// ============================================================================
// ���ܣ� д����
// ������ dwBlock -- ���;
//       dwOffset -- ����ƫ��;
//       pBuf -- д����;
//       dwSize -- д���ݴ�С
//       bFlags -- �޹���(0x0); ECC���ܿ��������ݴ�СΪ����256��0x1����
// ���أ�����0�����ɹ���д�����ݴ�С����
// ��ע��
// ============================================================================
u32 efs_at45_write(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize, u8 bFlags)
{
    u32 page = (dwBlock * nordescription->SectorsPerBlk) + (dwOffset / nordescription->BytesPerPage); // ҳ��
    u32 offset = dwOffset & 0x1FF; // ҳ�ڵ�ƫ��
    s32 res , wrLen = dwSize,nowlen;
    u8 data[nordescription->BytesPerPage];
    struct uopt opt = {0};
    if(!pBuf)
        return (0);

    memset(data, 0xff, nordescription->BytesPerPage);
    if(dwSize < nordescription->BytesPerPage - offset)
        nowlen = dwSize;            //��Ҫд������û�г�����ǰҳ��������ȫд��
    else
        nowlen = nordescription->BytesPerPage - offset; //��Ҫд�����ݳ�����ǰҳ���Ȱѵ�ǰҳд��

    memcpy(data + offset, pBuf, nowlen);
    for(;;)
    {
        res = __at45_write(page, data, opt);
        if(res < 0)
            break; // ����
        wrLen -= nowlen;    //����Ҫд�����ݳ��ȼ�ȥ�ϴ�д�����ݳ���
        if(wrLen <= 0)
        {
            wrLen = 0;
            break; // д��
        }
        pBuf += nowlen; //��д���ݵĵ�ַƫ��

        memset(data, 0xff, nordescription->BytesPerPage);
        if(wrLen >= nordescription->BytesPerPage)
            nowlen = nordescription->BytesPerPage;      //��Ҫд�����ݳ�����ǰҳ���Ȱ�ҳд��
        else
            nowlen = wrLen; //��Ҫд������û�г�����ǰҳ��������ȫд��

        memcpy(data, pBuf, nowlen);
        page++;     //ҳ�ż�1
    }
    return (dwSize - wrLen);
}

// ============================================================================
// ���ܣ� ������
// ������ dwBlock -- ���;
//       Offset -- ����ƫ��;
//       pBuf -- ������;
//       dwSize -- �����ݴ�С
//       bFlags -- 0x0 -- �޹���; 0x1 - ECC���ܿ���(���ݴ�СΪ����256)
// ���أ�ʧ�ܣ�0�����ɹ���������ֽ�������
// ��ע��
// ============================================================================
u32 efs_at45_read(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize, u8 bFlags)
{
    s32 res , rdLen = dwSize;
    u32 page = (dwBlock * nordescription->SectorsPerBlk) + (dwOffset / nordescription->BytesPerPage); // ҳ��
    u32 offset = dwOffset & 0x1FF; // ҳ�ڵ�ƫ��
    u8 data[nordescription->BytesPerPage];
    struct uopt opt = {0};
    if(!pBuf)
        return (0);

    if(__at45_read(page, data, opt) != 0)       //������ǰҳ����������
        return (0);     //��ʧ��

    if(rdLen < (s32)(nordescription->BytesPerPage - offset))
        memcpy(pBuf, data + offset, rdLen);        //��Ҫ��������ȫ�ڵ�ǰҳ��
    else
        memcpy(pBuf, data + offset, nordescription->BytesPerPage - offset);  //��Ҫ��������ֻ��һ�����ڵ�ǰҳ�У��ȶ���һ����

    rdLen -= nordescription->BytesPerPage - offset;
    if(rdLen <= 0)
        return (dwSize);  //����ȫ����

    pBuf += nordescription->BytesPerPage - offset;      //�������ַƫ��
    if(offset)
        offset = 0;

    page++;
    for(;;)
    {
        res = __at45_read(page, data, opt);
        if(res < 0)
            break; // ����

        if(rdLen >= nordescription->BytesPerPage)
            memcpy(pBuf, data, nordescription->BytesPerPage);       //��Ҫ�������ݴ���1ҳ��С�����ȶ�ȡ1ҳ��������
        else
        {
            memcpy(pBuf, data, rdLen);      //��Ҫ��������ȫ�ڵ�ǰҳ��
            rdLen = 0;
            break;
        }
        rdLen -= nordescription->BytesPerPage;
        pBuf += nordescription->BytesPerPage;
        page++;
    }

    return (dwSize - (u32)rdLen);
}

// ============================================================================
// ���ܣ� ����
// ������ BlkNo -- ���;
// ���أ��ɹ���TRUE����ʧ�ܣ�FALSE����
// ��ע��
// ============================================================================
bool_t efs_at45_erase(u32 dwBlock)
{
    struct uesz sz;

    sz.unit = 0;
    sz.block = 1;
    if(!__at45_erase(dwBlock,sz))
        return (TRUE);

    return (FALSE);
}
// ============================================================================
// ���ܣ� ���ָ����ַ�Ƿ�׼����
// ������dwBlock -- ��ţ�dwOffset -- ��ƫ�ƣ� pBuf -- ��Ҫ�жϵ����ݣ� dwSize -- ���ݳ���
// ���أ��ɹ���TRUE����ʧ�ܣ�FALSE����
// ��ע��
// ============================================================================
bool_t efs_at45_CheckBlockReady(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize)
{
    static u8 data[259];
    u8 *buf = pBuf;
    u32 block = dwBlock,left = dwSize,i, rLen,offset = dwOffset,blockBytes = nordescription->SectorsPerBlk * nordescription->BytesPerPage;

    block += offset / blockBytes;
    offset = offset % blockBytes;

    do{
        rLen = left;
        if(rLen > 256)
            rLen = 256;

        if(0 == efs_at45_read(block, offset, data, rLen,0))
            return (FALSE);

        for(i=0; i < rLen; i++)
        {
            if((0xFF != data[i]) && (data[i] != buf[i]))        //���ָ����ַ��������ݼȲ���ȫFF�ֲ��Ǵ��жϵ����ݣ��������û��׼����
                return (FALSE);
        }

        left -= rLen;
        offset += rLen;
    }while(left);

    return (TRUE);
}
// =============================================================================
// ���ܣ���at45�а�װefs�ļ�ϵͳ
// ������   pBusName -- AT45��Ҫ�õ�ͨ����
//      TargetFs -- Ҫ���ص��ļ�ϵͳ
//      �������� -- ��ʼ�飬��������������ʱ�������ÿ飬ֻ�����ÿ����һ�飩���Ƿ��ʽ����
// ���أ��ɹ���true����ʧ�ܣ�false����
// ��ע��
// =============================================================================
bool_t ModuleInstall_At45InstallEfs(const char *TargetFs, s32 bstart, s32 bend, u32 doformat)
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
                    warning_printf("efs"," Format failure.");
                }
            }
            targetobj = OBJ_MatchPath(TargetFs, &notfind);
            if(notfind)
            {
                error_printf("at45"," not found need to install file system.");
                return false;
            }
            super = (struct FsCore *)OBJ_GetPrivate(targetobj);
            if(strcmp(super->pFsType->pType, "EFS") == 0)
            {
                if(__AT45_FsInstallInit(TargetFs,bstart,bend,&EFS_AT45_DRV) == 0)
                    return true;
            }
            error_printf("at45"," need to install file system not EFS.");
        }
    }

    return false;
}
