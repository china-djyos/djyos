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
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <misc/misc.h>
#include <systime.h>
#include <dbug.h>
#include <Iboot_info.h>
#include <djyfs/filesystems.h>
#include <device/djy_flash.h> // will be obsolete
#include <device/unit_media.h>
#include <xip.h>
#include <endian.h>
#include <objhandle.h>
#include "cpu_peri.h"

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_FlashInstallXIP(const char *TargetFs,s32 bstart, s32 bend, u32 doformat);
//    ModuleInstall_FlashInstallXIP(CFG_EFLASH_XIPFSMOUNT_NAME,CFG_EFLASH_XIP_PART_START,
//                                              CFG_EFLASH_XIP_PART_END, CFG_EFLASH_XIP_PART_FORMAT);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"emflash insatall xip"//Ƭ��flash��װxip
//parent:"cpu onchip flash"//��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                         //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                      //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                        //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium                       //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                        //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"device file system","lock","cpu onchip flash"//������������������������none����ʾ�������������
                                        //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                        //����������������������г�
//weakdependence:"xip app file system","xip iboot file system"  //��������������������������none����ʾ�������������
                                        //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                        //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                        //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_EMFLASH_INSATALL_XIP == false )
//#warning  " emflash_insatall_xip  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_EMFLASH_INSATALL_XIP    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,-1,1024,
#define CFG_EFLASH_XIP_PART_START      128          //"������ʼ��ţ���"����д��ţ���Ŵ�0��ʼ���㣬CPU�ӽǣ�������*32/34
#define CFG_EFLASH_XIP_PART_END        -1         //"����������ţ�����"��-1��ʾ���һ�飬CPU�ӽǣ�������*32/34
//%$#@enum,true,false,
#define CFG_EFLASH_XIP_PART_FORMAT     false      //"����ѡ��",�Ƿ���Ҫ��ʽ���÷�����
//%$#@enum,"xip-app","xip-iboot",NULL
#define CFG_EFLASH_XIPFSMOUNT_NAME   "xip-app"    //"�ļ�ϵͳ��mountλ��"
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure
// ============================================================================
extern struct NorDescr *nordescription;
u8 is_protect = 1;   //1 -- ��д������0 -- ��д����
//extern bool_t flash_is_install(void);
extern u32 gc_ProductSn;
extern void calc_crc(u32 *buf, u32 packet_num);

//extern void djy_flash_read(uint32_t address, void *data, uint32_t size);
//extern void djy_flash_write(uint32_t address, const void *data, uint32_t size);
//extern void djy_flash_erase(uint32_t address);
//extern s32 djy_flash_req(enum ucmd cmd, ptu32_t args, ...);
extern s32 EmbFsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv);
//extern void djy_flash_read_crc(uint32_t address, void *data, uint32_t size);
extern void flash_protection_op(UINT8 mode, PROTECT_TYPE type);
s32 xip_flash_write(struct __icore *core, u8 *data, u32 bytes, u32 pos);
s32 xip_flash_read(struct __icore *core, u8 *data, u32 bytes, u32 pos);
s32 xip_flash_erase(struct __icore *core, u32 bytes, u32 pos);

struct __xip_drv XIP_FLASH_DRV =
{
    .xip_erase_media = xip_flash_erase,
    .xip_read_media = xip_flash_read,
    .xip_write_media = xip_flash_write
};
// ============================================================================
// ���ܣ�д���ݣ����������Ƿ��Ѿ����crcУ���롣
// ������core -- xip�ļ�ϵͳ������Ϣ
//      data -- ���ݻ���
//      bytes -- д�ֽ��������ᳬ��һ�������С��
//      pos -- ���ݵ�ַ������data�Ƿ����crc�������߼���ַ��
// ���أ��ɹ���0����ʧ�ܣ�-1������Ҫû�п�д�ռ䣨-2����
// ��ע����д�����һ��unitʱ���᳢�Բ���
// ============================================================================
s32 xip_flash_write(struct __icore *core, u8 *data, u32 bytes, u32 pos)
{
    struct umedia *um = (struct umedia *)core->vol;
    static u8 *app_head = NULL;
    static ptu32_t iboot_sn_addr = NULL;
    struct objhandle *hdl = (struct objhandle *)core->root->child->handles.next;
    struct __icontext *cx = (struct __icontext *)hdl->context;
    struct __ifile *file = (struct __ifile*)handle_GetHostObjectPrivate(hdl);
    u32 j, page_size, offset = Iboot_GetAppHeadSize();
    u32 unit;
    u32 check_len = bytes;
    struct ProductInfo *p_productinfo;
    u8 iboot_sn_buf[32];

    if(GetOperFalshMode() == true)
    {
        //data������crc��ֻ��Ҫ����ʼ��ַ * 34 / 32�����������ֱ��д�����ˣ���Ϊ�Ѿ���crc��
        unit = pos + (core->MStart * nordescription->BytesPerPage) * 34 / 32;
    }
    else
    {
        //û�����crc����������Ҫ��������ݵ�ַ�������� * 34 / 32����Ϊ������û��crc����Ҫ�����������
        unit = (pos + (core->MStart * nordescription->BytesPerPage)) * 34 / 32;
    }
    djy_flash_req(lock, CN_TIMEOUT_FOREVER);
    if(strstr(CN_XIP_APP_PATH,core->root->name))
    {
        if(bytes > 0)
        {
            while(check_len > 0)    //��ѭ�����flash�Ƿ��Ѿ�����
            {
                if(check_len > nordescription->BytesPerPage)
                    page_size = nordescription->BytesPerPage;
                else
                    page_size = check_len;
                if(GetOperFalshMode() == true)
                    djy_flash_read_crc(unit, um->ubuf, page_size);
                else
                    djy_flash_read(unit, um->ubuf, page_size);

                for(j=0; j<page_size; j++)
                {
                    if(0xFF!=um->ubuf[j])        //�жϵ�ǰҳ�Ƿ�ΪFF
                    {
                        if(um->ubuf[j]!=data[j])
                        {
                            djy_flash_req(unlock, 0);
                            error_printf("EmFlash"," Data already exists in flash.\r\n");
                            return (-1);
                        }
                    }
                }
                check_len -= page_size;
            }

            if(pos == offset)
            {
                if(GetOperFalshMode() == true)      //���뿼��crc�����ļ��Ѿ����crc
                {
                    offset = (offset * 34 / 32) - offset;   //����crc��bin�ļ���app���ļ�ͷҪ�������ȱ�������
                    app_head = malloc(offset + Iboot_GetAppHeadSize());
                    if(app_head == NULL)
                    {
                        djy_flash_req(unlock, 0);
                        return (-1);
                    }
                    memcpy(app_head, cx->apphead, cx->Wappsize);
                    memcpy(app_head + cx->Wappsize, data, offset);
                    //�ж�App���Ƿ����ṩSN�ţ�����ʱ������������Ŷ������0x2a(*��)
                    if(*(app_head + ((sizeof(struct AppHead) + (u32)offsetof(struct ProductInfo, ProductionTime)) / 32 * 34)) == '*')
                    {
                        iboot_sn_addr = (u32)(&gc_ProductSn) / 32 * 34;
                        if(iboot_sn_addr)
                        {
                            memset(iboot_sn_buf, 0xff, sizeof(iboot_sn_buf));
                            djy_flash_read_crc(iboot_sn_addr, iboot_sn_buf, sizeof(iboot_sn_buf));  //appδ�ṩSN�ţ���iboot�л�ȡ��
                            if(iboot_sn_buf[0] != 0xff)
                            {   //��iboot��SN���Ƶ�app����
                                memcpy(app_head + ((sizeof(struct AppHead) + (u32)offsetof(struct ProductInfo, ProductionTime)) / 32 * 34),
                                                    iboot_sn_buf + sizeof(p_productinfo->TypeCode), sizeof(p_productinfo->ProductionTime) + sizeof(p_productinfo->ProductionNumber));
                            }
                        }
                    }
                    pos += offset;
                    data += offset;
                    bytes -= offset;
                    if(GetOperFalshMode() == true)
                    {
                        unit = pos + (core->MStart * nordescription->BytesPerPage) * 34 / 32;
                    }
                    else
                    {
                        unit = pos + (core->MStart * nordescription->BytesPerPage);
                        unit = unit * 34 / 32;
                    }
                }
                else
                {
                    if(*(cx->apphead + (sizeof(struct AppHead) + (u32)offsetof(struct ProductInfo, ProductionTime))) == 0xff)
                    {
                        iboot_sn_addr = (u32)(&gc_ProductSn) / 32 * 34;
                        if(iboot_sn_addr)
                        {
                            memset(iboot_sn_buf, 0xff, sizeof(iboot_sn_buf));
                            djy_flash_read_crc(iboot_sn_addr, iboot_sn_buf, sizeof(iboot_sn_buf));
                            if(iboot_sn_buf[0] != 0xff)
                            {   //��iboot��SN���Ƶ�app����
                                memcpy(cx->apphead + (sizeof(struct AppHead) + (u32)offsetof(struct ProductInfo, ProductionTime)),
                                            iboot_sn_buf + sizeof(p_productinfo->TypeCode),
                                    sizeof(p_productinfo->ProductionTime) + sizeof(p_productinfo->ProductionNumber));
                            }
                        }
                    }
                }
                if(is_protect == 1)
                {
                    is_protect = 0;
                    flash_protection_op(0,FLASH_PROTECT_NONE);
                }
            }
            if(GetOperFalshMode() == true)
            {
                if(pos == 0)
                {
                    offset = (offset * 34 / 32) - offset;       //�����������ļ�ͷ������������������ݣ�������crc
                    if(app_head == NULL)
                    {
                        djy_flash_req(unlock, 0);
                        return (-1);
                    }
                    u8 *name = (u8 *)core->root->child->name;
                    u8 flag = 1;
                    u32 app_head_size = offset + Iboot_GetAppHeadSize();

    //                    file->sz += file->cxbase;
                    fill_little_32bit(app_head + 4, 0, file->sz);
                    app_head += 32; //32Ϊ�ļ�ͷ��Ϣ��ǰ32���ֽ�
                    for(j = 0; j < 96 + 8; j++)     //���96�����ļ�ͷ���app�ļ�������Ĵ�С,��8�Ƕ������8���ֽڵ�CRC
                    {
                        if((j % 34) == 0)
                            j += 2;
                        if(j < 96 + 8)
                        {
                            if(flag)
                                app_head[j] = *name;
                            else
                                app_head[j] = 0xff;
                            if(*name++ == 0)
                                flag=0;
                        }
                    }
                    app_head -= 32;
                    j = 0;
                    while(j < app_head_size)
                    {
                        j += 32;
                        memset(app_head + j, 0xff, 2);
                        j += 2;
                    }
                    calc_crc((u32 *)app_head, Iboot_GetAppHeadSize() / 32);
                    djy_flash_write(unit, app_head, app_head_size);
                    free(app_head);
                }
                else
                {
                    djy_flash_write(unit, data, bytes);
                }
            }
            else
                djy_flash_write(unit, data, bytes);

            if(pos == 0)
            {
                if(is_protect == 0)
                {
                    is_protect = 1;
                    flash_protection_op(0,FLASH_PROTECT_ALL);
                }
            }
        }

//        // �����ǰд��ҳ��һ�����е����һҳ����Ԥ��ɾ��������sector
//        // (page+1)���ڷ�ֹ��ʽ���˲�����xip�Ŀռ�
//        if(GetOperFalshMode() == true)
//        {
//            djy_flash_req(whichblock, (ptu32_t)&block, &unit);
//            djy_flash_req(remain, (ptu32_t)&more, &unit);
//            if(!more)
//            {
//                // +1�Ǳ�ʾ��ǰunit�ĺ���һ��
//                if(pos + bytes >= core->ASize)
//                    return (-2);
//                djy_flash_req(whichblock, (ptu32_t)&block, &unit);
//                //block�ǵ�ǰҳ���ڵĿ�ţ�block+1��Ϊ�˲�����һ���飨block+1��Ҫ�����Ŀ飬����block+1+1��Ͳ����ˣ�
//                djy_flash_req(format, block+1, block+1+1);
//            }
//        }
    }
    else
    {
        if(strstr(CN_XIP_IBOOT_PATH,core->root->name))
        {
            djy_flash_write(unit, data, bytes);
        }
    }
    djy_flash_req(unlock, 0); //
    return (0);
}

// ============================================================================
// ���ܣ�������
// ������core -- xip�ļ�ϵͳ������Ϣ
//      data -- ���ݻ���
//      bytes -- ���ֽ���
//      pos -- ���ݵ�ַ
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע������Ҳ��ֻ���һ��unit��Ŀǰ
// ============================================================================
s32 xip_flash_read(struct __icore *core, u8 *data, u32 bytes, u32 pos)
{
    s64 unit;
    s32 left = bytes;
    unit = pos + (core->MStart * nordescription->BytesPerPage) * 34 / 32;
    djy_flash_req(lock, CN_TIMEOUT_FOREVER);
    if(left > 0)
    {

            djy_flash_read(unit, data, left);
    }

    djy_flash_req(unlock, 0); //
    return (0);
}

// ============================================================================
// ���ܣ���������
// ������core -- xip�ļ�ϵͳ������Ϣ
//       bytes -- �ֽ���
//       pos -- ���ݵ�ַ
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 xip_flash_erase(struct __icore *core, u32 bytes, u32 pos)
{
    s64 unit;
    u32 page_size, page_num, all;
    s32 left = bytes;

    if(is_protect == 1)
    {
        is_protect = 0;
        flash_protection_op(0,FLASH_PROTECT_NONE);
    }

    unit = pos + ((core->MStart * nordescription->BytesPerPage) * 34 / 32);

    page_size = nordescription->BytesPerPage;
    page_num = nordescription->PagesPerSector;
    all = page_size * page_num;
    djy_flash_req(lock, CN_TIMEOUT_FOREVER);
    while(left > 0)
    {
        djy_flash_erase(unit);
        unit += all;
        left -= all;
    }
    djy_flash_req(unlock, 0);

    return (0);
}


// ============================================================================
// ���ܣ���ʽ��xip�ļ�ϵͳ
// ������core -- xip�ļ�ϵͳ������Ϣ
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 xip_fs_format(void *core)
{
    struct __icore *xip_core = core;
    s32 left = xip_core->ASize, start, all, page_size, page_num;
    start = (xip_core->MStart * nordescription->BytesPerPage) * 34 / 32;
    page_size = nordescription->BytesPerPage;
    page_num = nordescription->PagesPerSector;
    all = page_size * page_num;
    if(is_protect == 1)
    {
        is_protect = 0;
        flash_protection_op(0,FLASH_PROTECT_NONE);
    }
    while(left > 0)
    {
        djy_flash_erase(start);
        start += all;
        left -= all;
    }

    return (0);
}

//-----------------------------------------------------------------------------
// ���ܣ���װƬ��Flash����
// ������TargetFs -- Ҫ���ص��ļ�ϵͳ
//      �������� -- ��ʼ�飬���������������������6����ʼ����0����÷���ʹ�õĿ�Ϊ0��1��2��3��4��5�飬�����������飩���Ƿ��ʽ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע���������֪��Ҫ��װʲô�ļ�ϵͳ�����߲���װ�ļ�ϵͳTargetFs��NULL��TargetPart��-1��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FlashInstallXIP(const char *TargetFs,s32 bstart, s32 bend, u32 doformat)
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
                warning_printf("xip"," Format flash.\r\n");
                flash_protection_op(0,FLASH_PROTECT_NONE);
                if(-1 == djy_flash_req(format, bstart , bend, &sz))
                {
                    warning_printf("xip"," Format failure.\r\n");
                }
                flash_protection_op(0,FLASH_PROTECT_ALL);
            }
            targetobj = OBJ_MatchPath(TargetFs, &notfind);
            if(notfind)
            {
                error_printf("EmFlash"," not found need to install file system.\r\n");
                return false;
            }
            super = (struct FsCore *)OBJ_GetPrivate(targetobj);
            if((strcmp(super->pFsType->pType, "XIP-APP") == 0) || (strcmp(super->pFsType->pType, "XIP-IBOOT") == 0))
            {
                if(EmbFsInstallInit(TargetFs,bstart,bend,&XIP_FLASH_DRV) == 0)
                    return true;
            }
            error_printf("EmFlash"," need to install file system not XIP.");
        }
    }
    return false;
}
