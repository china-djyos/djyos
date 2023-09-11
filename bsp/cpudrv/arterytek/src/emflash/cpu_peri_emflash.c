//-----------------------------------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

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
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

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
#include <int.h>
#include <math.h>
#include <device/unit_media.h>
#include <dbug.h>
#include <xip.h>
#include <djyfs/filesystems.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//s32 ModuleInstall_EmbededFlash(u32 doformat);
//ModuleInstall_EmbededFlash(CFG_EFLASH_PART_FORMAT);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu drive inner flash"//Ƭ��flash
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                         //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                      //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                        //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                       //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                        //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"device file system","lock"//������������������������none����ʾ�������������
                                        //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                        //����������������������г�
//weakdependence:"xip app file system","xip iboot file system"  //��������������������������none����ʾ�������������
                                        //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                        //����������������������г����á�,���ָ�
//mutex:"none"                          //������Ļ����������������none����ʾ�޻����������
                                        //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_DRIVE_INNER_FLASH == false )
//#warning  " cpu_drive_inner_flash  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_DRIVE_INNER_FLASH    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@enum,true,false,
#define CFG_EFLASH_PART_FORMAT     false      //����ѡ��,�Ƿ���Ҫ������оƬ��
//%$#@num,0,,
#define CFG_EFLASH_PAGE_SIZE                512         //"Ƭ��flash��ҳ��С��λ�ֽ�"��ʵ���Լ�����Ϊ��ͳһ��ʵ����оƬ�ֲᲢû�й涨
#define CFG_EFLASH_SECT_PAGE_NUM            4           //"Ƭ��flash���������ж���ҳ"
#define CFG_EFLASH_BLOCK_SECT_NUM           32          //"Ƭ��flash�Ŀ����ж�������"
#define CFG_EFLASH_PLANE1_SECT_NUM          256         //"Ƭ1һ���ж�������"
#define CFG_EFLASH_PLANE2_SECT_NUM          256         //"Ƭ2һ���ж�������"
#define CFG_EFLASH_PLANE1_BLOCK_NUM         8           //"Ƭ1һ���ж��ٿ�"
#define CFG_EFLASH_PLANE2_BLOCK_NUM         8           //"Ƭ2һ���ж��ٿ�"
//%$#@string,1,32,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure
// ============================================================================

static struct EmbdFlashDescr{
    u16     BytesPerPage;                   // ҳ�а������ֽ���
    u16     PagesPerSect;                   // һ�������е�ҳ��
    u16     SectsPerBolck;                  // ���е�������
    u16     SectorsPerPlane1;               // Ƭ1��������
    u16     SectorsPerPlane2;               // Ƭ2��������
    u16     BlocksPerPlane1;               // Ƭ1�Ŀ���
    u16     BlocksPerPlane2;               // Ƭ2�Ŀ���
    u32     MappedStAddr;
} *s_ptEmbdFlash;
extern u32 gc_ptIbootSize;

static const char *EmflashName = "emflash";      //��flash��obj�ڵ�����
static struct umedia *emflash_um;
extern struct Object *s_ptDeviceRoot;
static bool_t sEmflashInited = false;
s32 __embed_FsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv);
s32 __embed_read(s64 unit, void *data, struct uopt opt);
s32 __embed_req(enum ucmd cmd, ptu32_t args, ...);
s32 __embed_write(s64 unit, void *data, struct uopt opt);
s32 __embed_erase(s64 unit, struct uesz sz);
// ============================================================================
// ���ܣ� ι��
// ������
// ���أ�
// ��ע��
// ============================================================================
extern bool_t BrdWdt_FeedDog(void) __attribute__((weak));
bool_t BrdWdt_FeedDog(void)
{
    return (TRUE); // �պ���
}

// ============================================================================
// ���ܣ� ����FLASH��ʼ��
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 Flash_Init(struct EmbdFlashDescr *Description)
{
    Description->BytesPerPage = CFG_EFLASH_PAGE_SIZE;
    Description->PagesPerSect = CFG_EFLASH_SECT_PAGE_NUM;
    Description->SectsPerBolck = CFG_EFLASH_BLOCK_SECT_NUM;
    Description->SectorsPerPlane1 = CFG_EFLASH_PLANE1_SECT_NUM;
    Description->SectorsPerPlane2 = CFG_EFLASH_PLANE2_SECT_NUM;
    Description->BlocksPerPlane1 = CFG_EFLASH_PLANE1_BLOCK_NUM;
    Description->BlocksPerPlane2 = CFG_EFLASH_PLANE2_BLOCK_NUM;
    Description->MappedStAddr = FLASH_BASE;
    return (0);
}

// ============================================================================
// ���ܣ� ��������
// ������ SectorNo -- �������;
// ���أ� "0" -- �ɹ�;"-1" -- ʧ��;
// ��ע��
// ============================================================================
static s32 Flash_SectorEarse(u32 SectorNo)
{
    s32 Ret = -1;
    flash_status_type status = FLASH_OPERATE_DONE;
    flash_unlock();

    status = flash_operation_wait_for(ERASE_TIMEOUT);

    if(status != FLASH_OPERATE_TIMEOUT)
    {
        if((status == FLASH_PROGRAM_ERROR) || (status == FLASH_EPP_ERROR))
        {
            flash_flag_clear(FLASH_PRGMERR_FLAG | FLASH_EPPERR_FLAG);
        }
        Int_CutTrunk();
        status = flash_sector_erase(SectorNo * s_ptEmbdFlash->PagesPerSect * s_ptEmbdFlash->BytesPerPage + s_ptEmbdFlash->MappedStAddr);
        Int_ContactTrunk();
        if(status == FLASH_OPERATE_DONE)
        {
            Ret = 0;
        }
    }

    flash_lock();
    return Ret;
}

// ============================================================================
// ���ܣ� ������
// ������ BlockNo -- �������;
// ���أ� "0" -- �ɹ�;"-1" -- ʧ��;
// ��ע��
// ============================================================================
static s32 Flash_BlockEarse(u32 BlockNo)
{
    s32 Ret = -1;
    flash_status_type status = FLASH_OPERATE_DONE;
    flash_unlock();

    status = flash_operation_wait_for(ERASE_TIMEOUT);

    if(status != FLASH_OPERATE_TIMEOUT)
    {
        if((status == FLASH_PROGRAM_ERROR) || (status == FLASH_EPP_ERROR))
        {
            flash_flag_clear(FLASH_PRGMERR_FLAG | FLASH_EPPERR_FLAG);
        }
        Int_CutTrunk();
        status = flash_block_erase(BlockNo * s_ptEmbdFlash->SectsPerBolck * 
                    s_ptEmbdFlash->PagesPerSect * s_ptEmbdFlash->BytesPerPage + s_ptEmbdFlash->MappedStAddr);
        Int_ContactTrunk();
        if(status == FLASH_OPERATE_DONE)
        {
            Ret = 0;
        }
    }

    flash_lock();
    return Ret;
}

// ============================================================================
// ���ܣ�дĳҳ
// ������PageNo -- ҳ��;
//       Data -- д����;
//       Flags -- д��������λ;
// ���أ�">0" -- ������д����ֽ���;
//       "-1" -- ��������;
// ��ע��
// ============================================================================
static s32 Flash_PageProgram(u32 Page, u8 *Data, u32 Flags)
{
    u32 i = 0;
    flash_status_type status = FLASH_OPERATE_DONE;
    u32 *pData = (u32*)Data;

    u32 Addr = Page * s_ptEmbdFlash->BytesPerPage + s_ptEmbdFlash->MappedStAddr;

    if(!Data)
        return (-1);

    flash_unlock();

    for(i = 0; i < s_ptEmbdFlash->BytesPerPage;)
    {
        if(*(u32*)Addr != *pData)
        {
            Int_CutTrunk();
            status = flash_word_program(Addr, *pData);
            Int_ContactTrunk();
            if(status != FLASH_OPERATE_DONE)
            {
                break;
            }
        }
        pData++;
        i += 4;
        Addr += 4;
    }

    flash_lock();

    return i;
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 Flash_PageRead(u32 Page, u8 *Data, u32 Flags)
{
    u32 Addr = Page * s_ptEmbdFlash->BytesPerPage + s_ptEmbdFlash->MappedStAddr;

    if(!Data)
        return (-1);

    memcpy(Data, (u8*)Addr, s_ptEmbdFlash->BytesPerPage);
    return (s_ptEmbdFlash->BytesPerPage);

}

//-----------------------------------------------------------------------------
// ���ܣ���װƬ��Flash����
// ������doformat -- �Ƿ��ʽ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע���������֪��Ҫ��װʲô�ļ�ϵͳ�����߲���װ�ļ�ϵͳTargetFs��NULL��TargetPart��-1��
//-----------------------------------------------------------------------------
s32 ModuleInstall_EmbededFlash(u32 doformat)
{
    if(s_ptEmbdFlash)
        return (-4); // �豸��ע��

    s_ptEmbdFlash = malloc(sizeof(*s_ptEmbdFlash));
    if(!s_ptEmbdFlash)
        return (-1);

    Flash_Init(s_ptEmbdFlash);

    if(doformat)
    {
        struct uesz sz;
        sz.unit = 0;
        sz.block = 1;
        __embed_req(format, (ptu32_t)0 , -1, &sz);           //��ʽ��ָ������
    }

    emflash_um = malloc(sizeof(struct umedia)+s_ptEmbdFlash->BytesPerPage);
    if(!emflash_um)
    {
        return (-1);
    }

    emflash_um->mreq = __embed_req;
    emflash_um->type = embed;
    emflash_um->ubuf = (u8*)emflash_um + sizeof(struct umedia);

    if(!Device_Create((const char*)EmflashName, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)emflash_um)))
    {
        printf("\r\n: erro : device : %s addition failed.", EmflashName);
        free(emflash_um);
        return (-1);
    }
    sEmflashInited = true;
    return (0);
}
// =============================================================================
// ���ܣ��ж�emflash�Ƿ�װ
// ������  ��
// ���أ��ѳɹ���װ��true����δ�ɹ���װ��false����
// ��ע��
// =============================================================================
bool_t emflash_is_install(void)
{
    return sEmflashInited;
}
// ============================================================================
// ���ܣ�embeded flash ����
// ������ucmd -- ���
//      ���� -- ���������
// ���أ�
// ��ע��
// ============================================================================
s32 __embed_req(enum ucmd cmd, ptu32_t args, ...)
{
    s32 res = 0;

    switch(cmd)
    {
        case remain:
        {
            va_list list;
            u32 *left;
            s64 *unit;

            left = (u32*)args;
            va_start(list, args);
            unit = (s64*)va_arg(list, u32);
            va_end(list);

            // ��1����Ϊ�����統ǰ��һ����8ҳ��*unit��0�Ļ�������ǰҳ�ǵ�0ҳ�����ػ�ʣ7ҳ
            *left = s_ptEmbdFlash->SectsPerBolck * s_ptEmbdFlash->PagesPerSect 
                        - (*unit % s_ptEmbdFlash->SectsPerBolck * s_ptEmbdFlash->PagesPerSect) - 1;
            break;
        }

        case whichblock:
        {
            va_list list;
            s64 *unit;
            u32 *block;

            block = (u32*)args;
            va_start(list, args);
            unit = (s64*)va_arg(list, u32);
            va_end(list);

            *block = *unit / (s_ptEmbdFlash->SectsPerBolck * s_ptEmbdFlash->PagesPerSect);
            break;
        }

        case totalblocks:
        {
            *((u32*)args) = s_ptEmbdFlash->BlocksPerPlane1 + s_ptEmbdFlash->BlocksPerPlane2;
            break;
        }
        case totalSectors:
        {
            *((u32*)args) = s_ptEmbdFlash->SectorsPerPlane1 + s_ptEmbdFlash->SectorsPerPlane2;
            break;
        }

        case blockunits:
        {
            va_list list;
            u32 *units = (u32*)args;

            *units = s_ptEmbdFlash->SectsPerBolck * s_ptEmbdFlash->PagesPerSect;
            break;
        }

        case unitbytes:
        {
            // args = &bytes
            *((u32*)args) = s_ptEmbdFlash->BytesPerPage;
            break;
        }

        case format:
        {
            va_list list;
            s32 start, end;
            struct uesz *sz;

            start = (u32)args;
            va_start(list, args);
            end = va_arg(list, u32);
            sz = (struct uesz*)va_arg(list, u32);
            va_end(list);

            if(!sz->block)
                return (-1);

            if(-1==(s32)end)
                end = s_ptEmbdFlash->BlocksPerPlane1 + s_ptEmbdFlash->BlocksPerPlane2; // �����ĺţ�

            do
            {
                if(__embed_erase((s64)--end, *sz))
                {
                    res = -1;
                    break;
                }
            }
            while(end!=start);

            break;
        }

        case mapaddr:
        {

            *((u32*)args) = s_ptEmbdFlash->MappedStAddr;
            break;
        }
        case checkbad: break;
        default: res = -1; break;
    }

    return (res);
}

// ============================================================================
// ���ܣ�embed ����
// ������unit -- ������ţ�ҳ����
//      data -- �������ݣ�
//      opt -- ���ķ�ʽ��
// ��ע��
// ============================================================================
s32 __embed_read(s64 unit, void *data, struct uopt opt)
{
    s32 res;

    res = Flash_PageRead((u32)unit, data, 0);
    if(res!=s_ptEmbdFlash->BytesPerPage)
        return (-1);

    return (0);
}

// ============================================================================
// ���ܣ�embed д��
// ������unit -- д����ţ�ҳ����
//      data -- д�����ݣ�
//      opt -- д�ķ�ʽ��
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 __embed_write(s64 unit, void *data, struct uopt opt)
{
    s32 res;

    res = Flash_PageProgram((u32)unit, data, 0);
    if(res!=s_ptEmbdFlash->BytesPerPage)
        return (-1);

    return (0);
}

// ============================================================================
// ���ܣ�embed ����
// ������unit -- ��������ţ�
//      sz -- �����ĵ�λ��unit��block��
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 __embed_erase(s64 unit, struct uesz sz)
{
    u32 block;

    if(sz.unit)
    {
        if(__embed_req(whichblock, (ptu32_t)&block, &unit))
            return (-1);
    }
    else
        block = (u32)unit;

    return (Flash_BlockEarse(block));
}


// ============================================================================
// ���ܣ���ʼ��Ƭ��flash
// ������fs -- ��Ҫ���ص��ļ�ϵͳ��mediadrv -- ý��������
//       bstart -- ��ʼ�飬bend -- �����飨�������ÿ飬ֻ���ÿ����һ�飩
// ���أ�0 -- �ɹ��� -1 -- ʧ��
// ��ע��
// ============================================================================
s32 __embed_FsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv)
{
    u32 units, total = 0;
    char *FullPath,*notfind;
    struct Object *targetobj;
    struct FsCore *super;
    s32 res,endblock = bend;
    targetobj = OBJ_MatchPath(fs, &notfind);
    if(notfind)
    {
        error_printf("embed"," not found need to install file system.\r\n");
        return -1;
    }
    super = (struct FsCore *)OBJ_GetPrivate(targetobj);
    super->MediaInfo = emflash_um;
    super->MediaDrv = mediadrv;

    if(-1 == (s32)endblock)
    {
        endblock = bend = s_ptEmbdFlash->BlocksPerPlane1 + s_ptEmbdFlash->BlocksPerPlane2;
    }

    do
    {
        if(__embed_req(blockunits, (ptu32_t)&units, --endblock))
        {
            return (-1);
        }

        total += units;     //����÷���һ���ж���ҳ
    }
    while(endblock!=bstart);

    super->AreaSize = total * s_ptEmbdFlash->BytesPerPage;
    endblock = 0;
    total = 0;
    while(endblock<bstart)
    {
        if(__embed_req(blockunits, (ptu32_t)&units, endblock++))
        {
            return (-1);
        }
        total += units;
    }

    super->MediaStart = total; // ��ʼunit��

    res = strlen(EmflashName) + strlen(s_ptDeviceRoot->name) + 1;
    FullPath = malloc(res);
    memset(FullPath, 0, res);
    sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,EmflashName);   //��ȡ���豸��ȫ·��
    File_BeMedia(FullPath,fs); //�����豸�����ļ�ϵͳ
    free(FullPath);

    printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, bstart, bend);
    return (0);

}

void djy_flash_write_ori(uint32_t address, u8 *data, uint32_t size)
{
    u32 i = 0;
    flash_status_type status = FLASH_OPERATE_DONE;
    u32 *pData = (u32*)data;
    flash_unlock();

    for(i = 0; i < size;)
    {
        if(*(u32*)address == 0xffffffff)
        {
            Int_CutTrunk();
            status = flash_word_program(address, *pData);
            Int_ContactTrunk();
            if(status != FLASH_OPERATE_DONE)
            {
                break;
            }
        }
        pData++;
        i += 4;
        address += 4;
    }

    flash_lock();
}

