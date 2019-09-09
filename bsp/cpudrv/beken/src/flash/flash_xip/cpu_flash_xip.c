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
#include <misc.h>
#include <systime.h>
#include <dbug.h>
#include <Iboot_info.h>
#include <filesystems.h>
#include <device/flash/flash.h> // will be obsolete
#include <device/include/unit_media.h>
#include <xip.h>
#include <endian.h>
#include <objhandle.h>
#include "cpu_peri.h"

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern s32 ModuleInstall_FlashInstallXIP(const char *TargetFs,s32 bstart, s32 bend, u32 doformat);
//    ModuleInstall_FlashInstallXIP(CFG_EFLASH_XIPFSMOUNT_NAME,CFG_EFLASH_XIP_PART_START,
//                                              CFG_EFLASH_XIP_PART_END, CFG_EFLASH_XIP_PART_FORMAT);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"flash insatall xip"//Ƭ��flash��װxip
//parent:"cpu onchip flash"//��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                         //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                      //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                        //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium                       //��ʼ��ʱ������ѡֵ��early��medium��later��
                                        //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"device file system","lock","cpu onchip flash"//������������������������none����ʾ�������������
                                        //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                        //����������������������г�
//weakdependence:"xip_app","xip_iboot"                 //��������������������������none����ʾ�������������
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
//%$#@num,-1,1000,
#define CFG_EFLASH_XIP_PART_START      128          //������ʼ����д��ţ���Ŵ�0��ʼ����
#define CFG_EFLASH_XIP_PART_END        -1         //����������-1��ʾ���һ��
//%$#@enum,true,false,
#define CFG_EFLASH_XIP_PART_FORMAT     false      //����ѡ��,�Ƿ���Ҫ��ʽ���÷�����
//%$#@string,1,32,
//%$#@enum,"xip-app","xip-iboot",NULL
#define CFG_EFLASH_XIPFSMOUNT_NAME   "xip-app"    //�谲װ���ļ�ϵͳ��mount�����֣�NULL��ʾ��flash�������ļ�ϵͳ
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure
// ============================================================================

extern bool_t flash_is_install(void);
extern void djy_flash_read(uint32_t address, void *data, uint32_t size);
extern void djy_flash_write(uint32_t address, const void *data, uint32_t size);
extern void djy_flash_erase(uint32_t address);
extern s32 djy_flash_req(enum ucmd cmd, ptu32_t args, ...);
extern s32 FsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv);

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
// ���ܣ�д����
// ������core -- xip�ļ�ϵͳ������Ϣ
//      data -- ���ݻ���
//      bytes -- д�ֽ��������ᳬ��һ�������С��
//      pos -- ���ݵ�ַ
// ���أ��ɹ���0����ʧ�ܣ�-1������Ҫû�п�д�ռ䣨-2����
// ��ע����д�����һ��unitʱ���᳢�Բ���
// ============================================================================
s32 xip_flash_write(struct __icore *core, u8 *data, u32 bytes, u32 pos)
{
    struct umedia *um = (struct umedia *)core->vol;
    static u8 *app_head = NULL;
    struct objhandle *hdl = (struct objhandle *)core->root->child->handles.next;
    struct __icontext *cx = (struct __icontext *)hdl->context;
    struct __ifile *file = (struct __ifile*)handle_GetHostObjectPrivate(hdl);
    u32 j, more, start, block = 0, page_size, offset = Get_AppHeadSize();
    u32 unit;

    djy_flash_req(mapaddr,(ptu32_t)&start);
    unit = start + pos + core->MStart;
    djy_flash_req(unitbytes,(ptu32_t)&page_size);
    djy_flash_req(lock, CN_TIMEOUT_FOREVER);
    if(bytes > 0)
    {
        djy_flash_read(unit, um->ubuf, page_size);

        for(j=0; j<page_size; j++)
        {
            if(um->ubuf[j]!=data[j])
            {
                if(0xFF!=um->ubuf[j])        //�жϵ�ǰҳ�Ƿ�ΪFF
                {
                    djy_flash_req(unlock, 0);
                    return (-1);
                }
            }
        }

        if(pos == offset)
        {
            offset = (offset * 34 / 32) - offset;
            app_head = malloc(offset + Get_AppHeadSize());
            if(app_head == NULL)
                return (-1);
            memcpy(app_head, cx->apphead, cx->Wappsize);
            memcpy(app_head + cx->Wappsize, data, offset);

            pos += offset;
            data += offset;
            bytes -= offset;
            unit = start + pos + core->MStart;
        }

        if(pos == 0)
        {
            offset = (offset * 34 / 32) - offset;
            if(app_head == NULL)
                return (-1);
            u8 *name = (u8 *)core->root->child->name;
            u8 flag = 1;
            u32 app_head_size = offset + Get_AppHeadSize();
            u8 *rbuf = NULL, *wbuf = NULL;
            rbuf = malloc (Get_AppHeadSize());
            wbuf = malloc (app_head_size);
            memset(rbuf, 0xff, Get_AppHeadSize());
            memset(wbuf, 0xff, app_head_size);

            file->sz += file->cxbase;
            fill_little_32bit(app_head + 4, 0, file->sz);
            app_head += 32; //32Ϊ�ļ�ͷ��Ϣ��ǰ32���ֽ�
            for(j = 0; j < 96 + 8; j++)     //���96�����ļ�ͷ���app�ļ�������Ĵ�С,��6�Ƕ������6���ֽڵ�CRC
            {
                if((j % 34) == 0)
                    j += 2;
                if(flag)
                    app_head[j] = *name;
                else
                    app_head[j] = 0xff;
                if(*name++ == 0)
                    flag=0;
            }
            app_head -= 32;
            j = 0;
            while(j < app_head_size)
            {
                j += 32;
                memset(app_head + j, 0xff, 2);
                j += 2;
            }
            calc_crc((u32 *)app_head, Get_AppHeadSize() / 32);
            djy_flash_write(unit, app_head, app_head_size);
            free(app_head);
        }
        else
        {
            djy_flash_write(unit, data, bytes);
        }
    }

    // �����ǰд��ҳ��һ�����е����һҳ����Ԥ��ɾ��������sector
    // (page+1)���ڷ�ֹ��ʽ���˲�����xip�Ŀռ�
    djy_flash_req(remain, (ptu32_t)&more, &unit);
    if(!more)
    {
        // +1�Ǳ�ʾ��ǰunit�ĺ���һ��
        if(pos + bytes >= core->ASize)
            return (-2);
        djy_flash_req(whichblock, (ptu32_t)&block, &unit);
        //block�ǵ�ǰҳ���ڵĿ�ţ�block+1��Ϊ�˲�����һ���飨block+1��Ҫ�����Ŀ飬����block+1+1��Ͳ����ˣ�
        djy_flash_req(format, block+1, block+1+1);
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
    u32 start, i;
    s32 left = bytes;
    u8 *rbuf;
    djy_flash_req(mapaddr,(ptu32_t)&start);
    unit = start + pos + core->MStart;
    djy_flash_req(lock, CN_TIMEOUT_FOREVER);
    if(left > 0)
    {
        if(pos == 0)
        {
            left = left * 34 / 32;
            rbuf = malloc (left);
            if(rbuf == NULL)
                return -1;
            memset(rbuf, 0xff, left);
            djy_flash_read(unit, rbuf, left);
            for(i = 0; i < (u32)(left / 34); i++)
            {
                memcpy(data + (i * 32), rbuf + (i * 34), 32);
            }
            free(rbuf);

        }
        else
        {
            djy_flash_read(unit, data, left);
        }
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
    u32 page_size, page_num, start, all;
    s32 left = bytes;

    djy_flash_req(mapaddr,(ptu32_t)&start);
    unit = start + pos + core->MStart;
    djy_flash_req(unitbytes,(ptu32_t)&page_size);
    djy_flash_req(totalblocks,(ptu32_t)&page_num);
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
s32 xip_fs_format(struct __icore *core)
{
    s32 left = core->ASize, start = core->MStart, all, page_size, page_num;
    djy_flash_req(unitbytes,(ptu32_t)&page_size);
    djy_flash_req(totalblocks,(ptu32_t)&page_num);
    all = page_size * page_num;
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
                if(-1 == djy_flash_req(format, bstart , bend, &sz))
                {
                    warning_printf("xip"," Format failure.\r\n");
                }
            }
            targetobj = obj_matchpath(TargetFs, &notfind);
            if(notfind)
            {
                error_printf("EmFlash"," not found need to install file system.\r\n");
                return false;
            }
            super = (struct FsCore *)obj_GetPrivate(targetobj);
            if((strcmp(super->pFsType->pType, "XIP-APP") == 0) || (strcmp(super->pFsType->pType, "XIP-IBOOT") == 0))
            {
                if(FsInstallInit(TargetFs,bstart,bend,&XIP_FLASH_DRV) == 0)
                    return true;
            }
            error_printf("EmFlash"," need to install file system not XIP.");
        }
    }
    return false;
}
