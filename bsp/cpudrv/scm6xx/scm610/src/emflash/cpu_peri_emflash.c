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
#include <math.h>
#include <device.h>
#include <device/flash/flash.h>
#include <cpu_peri.h>
#include <int.h>
#include <device/include/unit_media.h>
#include <dbug.h>
#include <xip.h>
#include <filesystems.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    s32 ModuleInstall_EmbededFlash(s32 bstart, s32 bend, u32 doformat);
//    ModuleInstall_EmbededFlash(CFG_EFLASH_PART_START, CFG_EFLASH_PART_END, CFG_EFLASH_PART_FORMAT);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu_peri_emflash"     //Ƭ��flash��д
//parent:"iap"                          //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                         //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                      //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                        //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                       //��ʼ��ʱ������ѡֵ��early��medium��later��
                                        //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"xip","devfile","lock" //������������������������none����ʾ�������������
                                        //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                        //����������������������г�
//weakdependence:"none"                 //��������������������������none����ʾ�������������
                                        //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                        //����������������������г����á�,���ָ�
//mutex:"none"                          //������Ļ����������������none����ʾ�޻����������
                                        //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef CFG_EFLASH_PART_START   //****�������Ƿ��Ѿ����ú�
#warning    cpu_peri_emflash �������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,-1,256,
#define CFG_EFLASH_PART_START      6          //������ʼ����д��ţ���Ŵ�0��ʼ����
#define CFG_EFLASH_PART_END        -1         //��������,-1��ʾ���һ��
//%$#@enum,true,false,
#define CFG_EFLASH_PART_FORMAT     false      //����ѡ��,�Ƿ���Ҫ��ʽ���÷�����
//%$#@string,1,32,
//%$#@enum,EN_XIP_APP_TARGET,EN_XIP_IBOOT_TARGET,NULL
#define CFG_EFLASH_FSMOUNT_NAME   EN_XIP_APP_TARGET    //�谲װ���ļ�ϵͳ��mount�����֣�NULL��ʾ��flash�������ļ�ϵͳ
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure
// ============================================================================

#define CN_SPI_FLASH_BASE (0x208000)
#define CN_ALL_CHIP_MAX   (0x100000)
#define CN_BLOCK_SIZE     (4*1024)
#define CN_SECTOR_SIZE    (512)
#define CN_PAGE_SIZE      (256)

static const char *EmflashName = "emflash";      //��flash��obj�ڵ�����
extern struct Object *s_ptDeviceRoot;
extern struct __xip_drv XIP_EMFLASH_DRV;
static struct umedia *emflash_um;
//extern struct Object *s_ptDeviceRoot;

//static volatile  tagSpiFlash* sgptSpiFlashReg = (volatile  tagSpiFlash*)CN_SPI_FLASH_BASE;

static struct EmbdFlashDescr{
    u16     BytesPerPage;                // ҳ�а������ֽ���-256Bytes
    u16     BytesPerSector;              // �����е��ֽ���
    u16     PagesPerSector;              // ÿ�����е�ҳ��
//    u16     SectorsPerBlock;           // ÿ���е�������
    u16     PagesPerBlock;               //ÿ���е�ҳ��
    u16     TotalPages;                  // �ܵ�ҳ��
    u32     TotalBlocks;                  //�ܵĿ���
    u32     MappedStAddr;
} *s_ptEmbdFlash;

//extern u32 gc_ptIbootSize;
s32 __embed_FsInstallInit(const char *fs, s32 bstart, s32 bend);
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
// ���ܣ�internal Flash Initial
// ������
// ���أ�
// ��ע��
// ============================================================================

s32 EmFlash_Init(struct EmbdFlashDescr *Description)
{
    Description->BytesPerPage   = CN_PAGE_SIZE;
    Description->BytesPerSector = CN_SECTOR_SIZE;
    Description->PagesPerSector = Description->BytesPerSector / Description->BytesPerPage;
    Description->PagesPerBlock  = CN_BLOCK_SIZE / CN_PAGE_SIZE;
    Description->TotalPages     = CN_ALL_CHIP_MAX / Description->BytesPerPage;
    Description->TotalBlocks    = CN_ALL_CHIP_MAX / CN_BLOCK_SIZE;
    Description->MappedStAddr   = 0x00000000;
    return (0);
}

// ============================================================================
// ���ܣ� ��ȡ����FLASH����Ϣ
// ������
// ���أ�
// ��ע��
// ============================================================================

static s32 EmFlash_GetDescr(struct EmFlashDescr *Description)
{
    Description->BytesPerPage   = s_ptEmbdFlash->BytesPerPage;
    Description->ReservedPages  = 256*1024/256;
    Description->TotalPages     = s_ptEmbdFlash->TotalPages;
    Description->MappedStAddr   = s_ptEmbdFlash->MappedStAddr;

    return (0);
}

// ============================================================================
// ���ܣ���������
// ������ BlkNo -- ���;
// ���أ� "0" -- �ɹ�;"-1" -- ʧ��;
// ��ע��
// ============================================================================

s32 Flash_BlockEarse(u32 BlkNo)
{
    s32 Ret;
    u32 SECTORError=0;//�������������Ϣ
    u32 BlockAddress;
    u16 SectorsPerBlock;//ÿ���е���������
    u16 SectLoop = 0;

    SectorsPerBlock = CN_BLOCK_SIZE / CN_SECTOR_SIZE;
    BlockAddress = BlkNo *CN_BLOCK_SIZE;

    while(SectLoop < SectorsPerBlock)
    {
        BrdWdt_FeedDog(); // ι��
        Int_CutTrunk();
        FLASH_Sector_Erase((UINT32 *)BlockAddress + CN_SECTOR_SIZE*SectLoop);
        Int_ContactTrunk();
        BrdWdt_FeedDog(); // ι��
        SectLoop++;
    }


    return Ret;
}


// ============================================================================
// ���ܣ�дĳҳ
// ������PageNo -- ҳ��;
//       Data -- д����;
//       Flags -- д��������λ;
// ���أ�">0" -- ������д����ֽ���;
//       "-1" -- ��������;
//       "-2" -- дʧ��;
// ��ע��
// ============================================================================
s32 EmFlash_PageProgram(u32 Page, u8 *Data, u32 Flags)
{
    u32 Ret = 0;
    u32 *pWriteData = (u32*)Data;
    u16 dbyLoop = 0;
    u32 dwWriteBytes = 0;

    u32 Addr = Page * s_ptEmbdFlash->BytesPerPage + s_ptEmbdFlash->MappedStAddr;

    if(!Data)
        return (-1);

    while(dbyLoop < CN_PAGE_SIZE /4)
    {
        BrdWdt_FeedDog(); // ι��
        Int_CutTrunk();
        //write 4 bytes each
        Ret = FLASH_Program_Single_Word((UINT32*)(Addr + 4*dbyLoop),pWriteData[dbyLoop]);
        if(Ret != SUCCESS)
        {
            Int_ContactTrunk();
            BrdWdt_FeedDog(); // ι��
            dwWriteBytes = -1;
            break;
        }
        dbyLoop++;
        Int_ContactTrunk();
        BrdWdt_FeedDog(); // ι��
        dwWriteBytes += 4;
    }

    return dwWriteBytes;

}


// ============================================================================
// ���ܣ�Read Pages
// ������Page  - number of page
// ���أ�Page  - user buffer
// ��ע��Flags - no use Now
// ============================================================================

s32 EmFlash_PageRead(u32 Page, u8 *Data, u32 Flags)
{
    u8 *pbyUsrBuf = NULL;
    u8 *pbyBspBuf = NULL;
    u32 dbBspBuf[CN_PAGE_SIZE/4];
    u16 dbyLoop = 0;

    if(Data != NULL)
    {
        pbyUsrBuf = (u8 *)Data;
        pbyBspBuf = (u8 *)dbBspBuf;
        memset(pbyBspBuf,0,CN_PAGE_SIZE);
    }else
    {
        return -1;
    }

    u32 Addr = Page * s_ptEmbdFlash->BytesPerPage + s_ptEmbdFlash->MappedStAddr;

    FLASH_Read((UINT32*)Addr,dbBspBuf,CN_PAGE_SIZE/4);

    //copy to usr buffer

    while(dbyLoop < CN_PAGE_SIZE)
    {
        pbyUsrBuf[dbyLoop] = pbyBspBuf[dbyLoop];
        dbyLoop++;
    }


    return (s_ptEmbdFlash->BytesPerPage);

}


// ============================================================================
// ���ܣ�����ҳ���ڵ�����
// ������PageNo  - page number
// ������Remains -  ʣ��ҳ��
// ����:SectorNo - ҳ���ڵ�����
// ��ע��
// ============================================================================

static s32 EmFlash_PageToSector(u32 PageNo, u32 *Remains, u32 *SectorNo)
{

    u32 PagesLeft, PagesDone;
    u32 RemainPages;
    u32 i;
    u32 Sector;
    u16 PagesPerSect;
    u16 PageAtSect;

    if((!Remains) || (!SectorNo))
        return (-1);

    //�����е�ҳ��
    PagesPerSect = CN_SECTOR_SIZE / CN_PAGE_SIZE;

    PagesLeft = PagesPerSect - PageNo % s_ptEmbdFlash->PagesPerSector;
    *Remains  = PagesLeft;

    //��������
    PageAtSect = PageNo / PagesPerSect;
    *SectorNo  = PageAtSect;
    return (0);
}

// ============================================================================
// ���ܣ���ʼ��Ƭ��flash
// ������fs -- ��Ҫ���ص��ļ�ϵͳ��MountPart -- ���ص���ý��ĵڼ���������������0��ʼ��
//       bstart -- ��ʼ�飬bend -- �����飨�������ÿ飬ֻ���ÿ����һ�飩
// ���أ�0 -- �ɹ��� -1 -- ʧ��
// ��ע��
// ============================================================================
s32 __embed_FsInstallInit(const char *fs, s32 bstart, s32 bend)
{
    char *FullPath,*notfind;
    struct Object *targetobj;
    struct FsCore *super;
    s32 res,BlockNum;
    targetobj = obj_matchpath(fs, &notfind);
    if(notfind)
    {
        error_printf("embed"," not found need to install file system.");
        return -1;
    }
    super = (struct FsCore *)obj_GetPrivate(targetobj);
    super->MediaInfo = emflash_um;
    //�����"XIP-APP"��"XIP-IBOOT"Ϊ�ļ�ϵͳ��������
    if((strcmp(super->pFsType->pType, "XIP-APP") == 0) || (strcmp(super->pFsType->pType, "XIP-IBOOT") == 0))
    {
        super->MediaDrv = &XIP_EMFLASH_DRV;
    }
    else
    {
        super->MediaDrv = 0;
        error_printf("embed"," \"%s\" file system type nonsupport", super->pFsType->pType);
        return -1;
    }

    if((s32)bend == -1)
    {
        bend = s_ptEmbdFlash->TotalBlocks;
        BlockNum = bend - bstart;
    }
    else
    {
        BlockNum = bend - bstart;
    }
    super->AreaSize = BlockNum * s_ptEmbdFlash->BytesPerSector;
    super->MediaStart = bstart * s_ptEmbdFlash->PagesPerBlock; // ��ʼunit��


    res = strlen(EmflashName) + strlen(s_ptDeviceRoot->name) + 1;
    FullPath = malloc(res);
    memset(FullPath, 0, res);
    sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,EmflashName);   //��ȡ���豸��ȫ·��
    FsBeMedia(FullPath,fs); //�����豸�����ļ�ϵͳ
    free(FullPath);

    printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, bstart, bend);
    return (0);

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
        case remain:   //��ҳ���ڵĿ��л�ʣ�����ҳ,ҳ�������ε����ӵ�
        {
            va_list list;
            u32 *left;
            s64 *unit;

            left = (u32*)args;
            va_start(list, args);
            unit = (s64*)va_arg(list, u32);
            va_end(list);

            if(*unit < s_ptEmbdFlash->TotalPages)
            {
                *left = s_ptEmbdFlash->PagesPerBlock - (*unit + 1) % s_ptEmbdFlash->PagesPerBlock;
            }
            else
                res = -1;

            break;
        }

        case whichblock:                   //��ҳ���ڿ�
        {
            va_list list;
            s64 *unit;
            u32 *block;

            block = (u32*)args;
            va_start(list, args);
            unit = (s64*)va_arg(list, u32);
            va_end(list);

            if(*unit < s_ptEmbdFlash->TotalPages)
            {
                *block = *unit / s_ptEmbdFlash->PagesPerBlock;
            }
            else
                res = -1;

            break;
        }

        case totalblocks:
        {
            // args = &blocks
            *((u32*)args) =  s_ptEmbdFlash->TotalBlocks;
            break;
        }

        case blockunits:               //ÿ���е�ҳ����
        {
            va_list list;
            u32 *units = (u32*)args;
            u32 block;

            va_start(list, args);
            block = (u32)va_arg(list, u32);
            va_end(list);

            *units = s_ptEmbdFlash->PagesPerBlock;
            break;
        }

        case unitbytes:                //ÿҳ�е��ֽ���
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
                end = s_ptEmbdFlash->TotalBlocks; // �����ĺţ�

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


//-----------------------------------------------------------------------------
// ���ܣ���װƬ��Flash����
// ������TargetFs -- Ҫ���ص��ļ�ϵͳ
//      �������� -- ��ʼ�飬�����飨�����������6����ʼ����0����÷���ʹ�õĿ�Ϊ0��1��2��3��4��5�飬�����������飩���Ƿ��ʽ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע���������֪��Ҫ��װʲô�ļ�ϵͳ�����߲���װ�ļ�ϵͳTargetFs��NULL��TargetPart��-1��
//-----------------------------------------------------------------------------
s32 ModuleInstall_EmbededFlash(s32 bstart, s32 bend, u32 doformat)
{
    static u8 emflashinit = 0;
    u32 units;
    if(!s_ptEmbdFlash)
    {
        s_ptEmbdFlash = malloc(sizeof(*s_ptEmbdFlash));
        if(!s_ptEmbdFlash)
        {
            return (-1);
        }

        EmFlash_Init(s_ptEmbdFlash);
    }

    if(doformat)
    {
        struct uesz sz;
        sz.unit = 0;
        sz.block = 1;
        __embed_req(format, (ptu32_t)bstart , bend, &sz);           //��ʽ��ָ������
    }

    if(emflashinit == 0)
    {
        emflash_um = malloc(sizeof(struct umedia)+s_ptEmbdFlash->BytesPerPage);
        if(!emflash_um)
        {
            return (-1);
        }

        emflash_um->mreq = __embed_req;
        emflash_um->type = embed;
        emflash_um->ubuf = (u8*)emflash_um + sizeof(struct umedia);

        if(!dev_Create((const char*)EmflashName, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)emflash_um)))
        {
            printf("\r\n: erro : device : %s addition failed.", EmflashName);
            free(emflash_um);
            return (-1);
        }
        emflashinit = 1;
    }
    if(CFG_EFLASH_FSMOUNT_NAME != NULL)
    {
        if(__embed_FsInstallInit(CFG_EFLASH_FSMOUNT_NAME, bstart, bend))
        {
            return -1;
        }
    }

    return 0;
}

