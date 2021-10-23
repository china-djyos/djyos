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
#include <dbug.h>
#include <djyfs/filesystems.h>
#include <djyfs/efs.h>

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_EmbFlashInstallEfs(const char *TargetFs, s32 bstart, s32 bend, u32 doformat);
//    ModuleInstall_EmbFlashInstallEfs(CFG_EMBFLASH_EFS_MOUNT_NAME, CFG_EMBFLASH_EFS_PART_START,
//                                                        CFG_EMBFLASH_EFS_PART_END, CFG_EMBFLASH_EFS_PART_FORMAT);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"embflash install efs"    //��norflash�ϰ�װefs�ļ�ϵͳ
//parent:"cpu onchip flash"                //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"device file system","easy file system","lock","cpu onchip flash"
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
#if ( CFG_MODULE_ENABLE_EMBFLASH_INSTALL_EFS == false )
//#warning  " embflash_install_efs  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_EMBFLASH_INSTALL_EFS    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@enum,512,528,
//%$#@string,1,10,
#define CFG_EMBFLASH_EFS_MOUNT_NAME            "efs"      //"�ļ�ϵͳmount������",��Ҫ���ص�efs�ļ�ϵͳmount������
//%$#@num,-1,1024
#define CFG_EMBFLASH_EFS_PART_START                  0        //"������ʼ"����д��ţ���Ŵ�0��ʼ����
#define CFG_EMBFLASH_EFS_PART_END                   -1        //"��������"��-1�����һ��
//%$#@enum,true,false,
#define CFG_EMBFLASH_EFS_PART_FORMAT               false      //"����ѡ��",�Ƿ���Ҫ��ʽ���÷�����
//%$#@select
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

extern struct NorDescr *nordescription;
//extern bool_t flash_is_install(void);
//extern void djy_flash_read(uint32_t address, void *data, uint32_t size);
//extern void djy_flash_write(uint32_t address, const void *data, uint32_t size);
//extern void djy_flash_erase(uint32_t address);
//extern s32 djy_flash_req(enum ucmd cmd, ptu32_t args, ...);
extern s32 EmbFsInstallInit(const char *fs, s32 dwStart, s32 dwEnd,void *mediadrv);
//extern void djy_flash_read_crc(uint32_t address, void *data, uint32_t size);

extern void flash_protection_op(UINT8 mode, PROTECT_TYPE type);

u32 efs_embflash_write(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize, u8 bFlags);
u32 efs_embflash_read(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize, u8 bFlags);
bool_t efs_embflash_erase(u32 dwBlock);
bool_t efs_embflash_CheckBlockReady(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize);


struct __efs_media_drv EFS_EMBFLASH_DRV =
{
    .efs_erase_media = efs_embflash_erase,
    .efs_read_media = efs_embflash_read,
    .efs_write_media = efs_embflash_write,
    .efs_check_block_ready = efs_embflash_CheckBlockReady
};
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

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
u32 efs_embflash_write(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize, u8 bFlags)
{
    u32 addr = (dwBlock * nordescription->PagesPerSector * nordescription->BytesPerPage) + dwOffset;
    bool_t crc_start;
    if(!pBuf)
        return (0);

    if(dwSize > 0)
    {
        flash_protection_op(0,FLASH_PROTECT_NONE);
        djy_flash_req(lock, CN_TIMEOUT_FOREVER);
        crc_start = GetOperFalshMode();
        SetOperFalshMode(true);
        djy_flash_write(addr, pBuf, dwSize);
        SetOperFalshMode(crc_start);
        djy_flash_req(unlock, 0);
        flash_protection_op(0,FLASH_PROTECT_ALL);
    }
    return dwSize;
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
u32 efs_embflash_read(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize, u8 bFlags)
{
    u32 addr = (dwBlock * nordescription->PagesPerSector * nordescription->BytesPerPage) + dwOffset;
    if(!pBuf)
        return (0);
    djy_flash_req(lock, CN_TIMEOUT_FOREVER);
    if(dwSize > 0)
    {
        djy_flash_read_crc(addr, pBuf, dwSize);
    }
    djy_flash_req(unlock, 0);
    return dwSize;
}
#pragma GCC diagnostic pop

// ============================================================================
// ���ܣ� ����
// ������ BlkNo -- ���;
// ���أ��ɹ���TRUE����ʧ�ܣ�FALSE����
// ��ע��
// ============================================================================
bool_t efs_embflash_erase(u32 dwBlock)
{
    u32 addr = dwBlock * nordescription->PagesPerSector * nordescription->BytesPerPage;

    flash_protection_op(0,FLASH_PROTECT_NONE);
    djy_flash_req(lock, CN_TIMEOUT_FOREVER);
    djy_flash_erase(addr);
    djy_flash_req(unlock, 0);
    flash_protection_op(0,FLASH_PROTECT_ALL);
    return (TRUE);
}
// ============================================================================
// ���ܣ� ���ָ����ַ�Ƿ�׼����
// ������dwBlock -- ��ţ�dwOffset -- ��ƫ�ƣ� pBuf -- ��Ҫ�жϵ����ݣ� dwSize -- ���ݳ���
// ���أ��ɹ���TRUE����ʧ�ܣ�FALSE����
// ��ע��
// ============================================================================
bool_t efs_embflash_CheckBlockReady(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize)
{
    u32 i, addr = (dwBlock * nordescription->PagesPerSector * nordescription->BytesPerPage) + dwOffset;
    u8 *buf = NULL;
    djy_flash_req(lock, CN_TIMEOUT_FOREVER);
    if(dwSize > 0)
    {
        buf = malloc(dwSize);
        if(buf)
        {
            djy_flash_read_crc(addr, buf, dwSize);
            for(i=0; i < dwSize; i++)
            {
                if((0xFF != buf[i]) && (buf[i] != pBuf[i]))
                {   //���ָ����ַ��������ݼȲ���ȫFF�ֲ��Ǵ��жϵ����ݣ��������û��׼����
                    free(buf);
                    djy_flash_req(unlock, 0);
                    return (FALSE);
                }
            }
            free(buf);
        }
    }
    djy_flash_req(unlock, 0);
    return (TRUE);
}
// =============================================================================
// ���ܣ���at45�а�װefs�ļ�ϵͳ
// ������
//      TargetFs -- Ҫ���ص��ļ�ϵͳ
//      �������� -- ��ʼ�飬��������������ʱ�������ÿ飬ֻ�����ÿ����һ�飩���Ƿ��ʽ����
// ���أ��ɹ���true����ʧ�ܣ�false����
// ��ע��
// =============================================================================
bool_t ModuleInstall_EmbFlashInstallEfs(const char *TargetFs, s32 bstart, s32 bend, u32 doformat)
{
    struct FsCore *super;
    char *notfind;
    struct Object *targetobj;
    if(flash_is_install() == true)
    {
        if((TargetFs != NULL) && (bstart != bend))
        {
            if(doformat)
            {
                struct uesz sz;
                sz.unit = 0;
                sz.block = 1;
                warning_printf("efs"," Format flash.\r\n");
                flash_protection_op(0,FLASH_PROTECT_NONE);
                if(-1 == djy_flash_req(format, bstart , bend, &sz))
                {
                    warning_printf("efs"," Format failure.");
                }
                flash_protection_op(0,FLASH_PROTECT_ALL);
            }
            targetobj = OBJ_MatchPath(TargetFs, &notfind);
            if(notfind)
            {
                error_printf("efs"," not found need to install file system.");
                return false;
            }
            super = (struct FsCore *)OBJ_GetPrivate(targetobj);
            if(strcmp(super->pFsType->pType, "EFS") == 0)
            {
                flash_protection_op(0,FLASH_PROTECT_NONE);
                if(EmbFsInstallInit(TargetFs,bstart,bend,&EFS_EMBFLASH_DRV) == 0)
                {
                    flash_protection_op(0,FLASH_PROTECT_ALL);
                    return true;
                }
                flash_protection_op(0,FLASH_PROTECT_ALL);
            }
            error_printf("efs"," need to install file system not EFS.");
        }
    }

    return false;
}
