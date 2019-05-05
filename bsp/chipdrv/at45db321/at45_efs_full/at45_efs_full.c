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
#include <efs_full.h>

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_At45InstallEfsFull(char *pBusName, const char *TargetFs, s32 bstart, s32 bend, u32 doformat);
//    ModuleInstall_At45InstallEfsFull(CFG_AT45_BUSNAME, CFG_AT45_EFSFULL_MOUNT_NAME,
//                  CFG_AT45_EFSFULL_PART_START, CFG_AT45_EFSFULL_PART_END, CFG_AT45_EFSFULL_PART_FORMAT);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"at45db321_install_efsfull"    //��norflash�ϰ�װefs�ļ�ϵͳ
//parent:"at45db321"                //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"easyfullfilesystem","lock","spibus","cpu_peri_spi"             //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef CFG_AT45_EFSFULL_MOUNT_NAME           //****�������Ƿ��Ѿ����ú�
#warning   at45db321_install_efsfull�������δ���ã�ʹ��Ĭ��ֵ
//%$#@enum,512,528,
//%$#@string,1,10,
#define CFG_AT45_BUSNAME                       "SPI4"     //"SPI��������",AT45ʹ�õ���������
#define CFG_AT45_EFSFULL_MOUNT_NAME            "efsfull"      //"�ļ�ϵͳmount������",��Ҫ���ص�efs�ļ�ϵͳmount������
//%$#@num,-1,1024,
#define CFG_AT45_EFSFULL_PART_START                  0        //������ʼ����д��ţ���Ŵ�0��ʼ����
#define CFG_AT45_EFSFULL_PART_END                   -1        //����������-1��ʾ���һ��
//%$#@enum,true,false,
#define CFG_AT45_EFSFULL_PART_FORMAT               false      //����ѡ��,�Ƿ���Ҫ��ʽ���÷�����
//%$#@select
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

extern s32 __at45_write(u32 unit, void *data, struct uopt opt);
extern s32 __at45_read(u32 unit, void *data, struct uopt opt);
extern s32 __at45_erase(u32 unit, struct uesz sz);
extern s32 __at45_req(enum ucmd cmd, ptu32_t args, ...);
extern s32 __AT45_FsInstallInit(const char *fs, s32 dwStart, s32 dwEnd,void *mediadrv);
s32 efs_at45_erase(struct __ecore *core, s64 units, s64 size);
s32 efs_at45_write(struct __ecore *core, s64 units, void *buf);
s32 efs_at45_read(struct __ecore *core, s64 units, void *buf);

struct __efs_full_media_drv EFS_FULL_AT45_DRV =
{
    .efs_erase_media = efs_at45_erase,
    .efs_read_media = efs_at45_read,
    .efs_write_media = efs_at45_write
};

// ============================================================================
// ���ܣ�����
// ������core -- �ļ�ϵͳ���ƣ�
//      units -- ��ʼunit�ţ�
//      buf -- ���ݣ�
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע����unit�ռ��С����
// ============================================================================
s32 efs_at45_read(struct __ecore *core, s64 units, void *buf)
{
    struct uopt opt = {0};

    opt.necc = 1;
    opt.main = 1;
    units = core->MStart + units;
    return (__at45_read((u32)units, (u8*)buf, opt));
}

// ============================================================================
// ���ܣ�д��
// ������core -- �ļ�ϵͳ���ƣ�
//      units -- ��ʼunit�ţ�
//      buf -- ���ݣ�
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע����unit�ռ��Сд��
// ============================================================================
s32 efs_at45_write(struct __ecore *core, s64 units, void *buf)
{
    struct uopt opt = {0};

    opt.necc = 1;
    opt.main = 1;
    units = core->MStart + units;
    return (__at45_write((u32)units, (u8*)buf, opt));

}

// ============================================================================
// ���ܣ�������
// ������core -- �ļ�ϵͳ���ƣ�
//      units -- ��ʼunit�Ų������谴�ɲ��ߴ���룻
//      size -- �������unit�������谴�ɲ��ߴ���룻
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 efs_at45_erase(struct __ecore *core, s64 units, s64 size)
{
    s32 res = -1;
    s64 erases;
    struct uesz sz;

    sz.unit = 1;
    sz.block = 0;
    erases = 1 << (core->blacksize - core->pagesize); // �ɲ����ռ������е�unit����
    if((size % erases) || (units % erases))
    {
        printf("\r\n: warn : efs    : not aligned area will be erased(%d, %d)", (u32)units, (u32)size); // ���������ݲ�û�ж��룬���ܴ�������
    }

    while(size > 0)
    {
        res = __at45_erase((u32)units, sz);
        if(-1 == res)
            return (-1);

        units += erases;
        size -= erases;
    }

    return (0);
}

// =============================================================================
// ���ܣ���at45�а�װefs_full�ļ�ϵͳ
// ������   pBusName -- AT45��Ҫ�õ�ͨ����
//      TargetFs -- Ҫ���ص��ļ�ϵͳ
//      �������� -- ��ʼ�飬��������������ʱ�������ÿ飬ֻ�����ÿ����һ�飩���Ƿ��ʽ����
// ���أ��ɹ���true����ʧ�ܣ�false����
// ��ע��
// =============================================================================
bool_t ModuleInstall_At45InstallEfsFull(char *pBusName, const char *TargetFs, s32 bstart, s32 bend, u32 doformat)
{
    struct FsCore *super;
    char *notfind;
    struct obj *targetobj;
    if(ModuleInstall_at45db321(pBusName,0) == true)
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
                    warning_printf("efsfull"," Format failure.");
                }
            }
            targetobj = obj_matchpath(TargetFs, &notfind);
            if(notfind)
            {
                error_printf("at45"," not found need to install file system.");
                return false;
            }
            super = (struct FsCore *)obj_GetPrivate(targetobj);
            if(strcmp(super->pFsType->pType, "EFSFULL") == 0)
            {
                if(__AT45_FsInstallInit(TargetFs,bstart,bend,&EFS_FULL_AT45_DRV) == 0)
                    return true;
            }
            error_printf("at45"," need to install file system not EFSFULL.");
        }
    }

    return false;
}
