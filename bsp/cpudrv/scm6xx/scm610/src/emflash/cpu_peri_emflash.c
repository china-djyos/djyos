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
#include <device/djy_flash.h>
#include <cpu_peri.h>
#include <int.h>
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
//component name:"cpu_peri_emflash"     //Ƭ��flash
//parent:"none"                          //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                         //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                      //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                        //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                       //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                        //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"device file system","lock" //������������������������none����ʾ�������������
                                        //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                        //����������������������г�
//weakdependence:"xip app file system","xip iboot file system"  //��������������������������none����ʾ�������������
                                        //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                        //����������������������г����á�,���ָ�
//mutex:"none"                          //������Ļ����������������none����ʾ�޻����������
                                        //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_PERI_EMFLASH == false )
//#warning  " cpu_peri_emflash  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_PERI_EMFLASH    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@enum,true,false,
#define CFG_EFLASH_PART_FORMAT     false      //����ѡ��,�Ƿ���Ҫ������оƬ��
//%$#@num,0,,
#define CFG_EFLASH_PAGE_SIZE                 256             //Ƭ��flash��ҳ��С����λ�ֽڡ�
#define CFG_EFLASH_SECTOR_SIZE               512             //Ƭ��flash�������е��ֽ�����
#define CFG_EFLASH_SECTOR_PAGE_NUM           2               //Ƭ��flashÿ�����е�ҳ����
#define CFG_EFLASH_BLOCK_PAGE_NUM            16              //Ƭ��flashÿ���е�ҳ����
#define CFG_EFLASH_ALL_PAGE_SIZE             4096            //Ƭ��flash���ܵ�ҳ����
#define CFG_EFLASH_ALL_BLOCK_SIZE            256             //Ƭ��flash���ܵĿ�����
#define CFG_EFLASH_MAPPED_START_ADDR         0x00000000      //Ƭ��flash��ӳ����ʼ��ַ��
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

static const char *EmflashName = "emflash";      //��flash��obj�ڵ�����
extern struct Object *s_ptDeviceRoot;

#define CN_FLASH_SIZE    (0x100000)      //1Mbytes
#define CN_PAGE_SIZE     (256)         //set page size
#define CN_SECTOR_SIZE   (512)           //set sector size 4k
//set block size 4k
#define CN_BLOCK_SIZE    (CN_SECTOR_SIZE * 2 *4)

static struct EmbdFlashDescr{
     u16     BytesPerPage;                // ҳ�а������ֽ���-256Bytes
     u16     BytesPerSector;              // �����е��ֽ���
     u16     PagesPerSector;              // ÿ�����е�ҳ��
     u16     PagesPerBlock;               //ÿ���е�ҳ��
     u16     TotalPages;                  // �ܵ�ҳ��
     u32     TotalBlocks;                  //�ܵĿ���
     u32     MappedStAddr;
} *embeddescription;

static struct umedia *emflash_um;
extern u32 gc_ptIbootSize;
static bool_t sEmflashInited = false;
s32 __embed_FsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv);
s32 __embed_read(s64 unit, void *data, struct uopt opt);
s32 __embed_req(enum ucmd cmd, ptu32_t args, ...);
s32 __embed_write(s64 unit, void *data, struct uopt opt);
s32 __embed_erase(s64 unit, struct uesz sz);
// ============================================================================
// ���ܣ�ι��
// ������
// ���أ�
// ��ע��������Ӳ�����Ź�ģ��ʵ�֣�����max706.c
// ============================================================================
extern bool_t BrdWdt_FeedDog(void) __attribute__((weak));
bool_t BrdWdt_FeedDog(void)
{
    return (TRUE); // �պ���
}

// ============================================================================
// ���ܣ���ȡ����FLASH����Ϣ
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 EmFlash_Init(struct EmbdFlashDescr *Description)
{
    Description->BytesPerPage     = CFG_EFLASH_PAGE_SIZE;
    Description->BytesPerSector   = CFG_EFLASH_SECTOR_SIZE;
    Description->PagesPerBlock    = CFG_EFLASH_BLOCK_PAGE_NUM;
    Description->PagesPerSector   = CFG_EFLASH_SECTOR_PAGE_NUM;
    Description->TotalBlocks      = CFG_EFLASH_ALL_BLOCK_SIZE;
    Description->TotalPages       = CFG_EFLASH_ALL_PAGE_SIZE;
    Description->MappedStAddr = 0x00000000;
    return (0);
}

// ============================================================================
// ���ܣ���ȡ����FLASH����Ϣ
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 Flash_GetDescr(struct EmFlashDescr *Description)
{
    Description->BytesPerPage  = embeddescription->BytesPerPage;
    Description->TotalPages    = embeddescription->TotalPages;
    //pages reserved is used for iboot program
    Description->ReservedPages = gc_ptIbootSize / embeddescription->BytesPerPage;
    Description->MappedStAddr = embeddescription->MappedStAddr;
    return (0);
}

// ============================================================================
// ���ܣ�������
// ������BlkNo -- ���;
// ���أ�"0" -- �ɹ�;"-1" -- ʧ��;
// ��ע��
// ============================================================================
s32 Flash_BlockEarse(u32 BlockNo)
{
    u8 retry = 0; // �����п��ܻ�ʧ�ܣ�
    s32 sRet = -1;
    u16 SectorNo;
    u16 tmpLoop = 0;
    u32 tmpEreaseAddBase = 0;
    u32 tmpCurAddress;
    u8  Status = 0;
    u16 tmpCnt = 0;

    //get the base of the erease address
    tmpEreaseAddBase = BlockNo * CFG_EFLASH_SECTOR_SIZE * 2 *4;

    //the numbers of sectors need to be ereased
    SectorNo = CFG_EFLASH_SECTOR_SIZE * 2 *4 / embeddescription->BytesPerSector;
//    while(tmpLoop < SectorNo)
    for(tmpLoop = 0;tmpLoop < SectorNo;tmpLoop++)
    {
        tmpCurAddress = tmpEreaseAddBase + 512*tmpLoop;
        atom_high_t atomic_low = Int_LowAtomStart();
        Status = FLASH_Sector_Erase(tmpCurAddress);
        Int_LowAtomEnd(atomic_low);
        for(tmpCnt = 0;tmpCnt < (embeddescription->BytesPerSector)/4;tmpCnt++)
        {
            u32 *tmpRead = (u32*)tmpCurAddress;
            if(tmpRead[tmpCnt] != 0xffffffff)
            {
                Status = -1;
                break;
            }

            Status = 1;

        }

        if(Status != 1)
        {
            printf("erease address %x failed\r\n",tmpEreaseAddBase);
            return sRet;
        }else{
            sRet = 0;
        }

//        BrdWdt_FeedDog();
//        tmpLoop++;
    }

    return sRet;
}

// ============================================================================
// ���ܣ�дĳҳ
// ������PageNo -- ҳ��;
//       Data -- д����;
//       Flags -- д��������λ;
// ���أ� ">0" -- ������д����ֽ���;
//       "-1" -- ��������;
//       "-2" -- дʧ��;
// ��ע��
// ============================================================================
s32 Flash_PageProgram(u32 Page, u8 *Data, u32 Flags)
{
    u32 Ret = 0,i;
    u32 *pData = (u32*)Data;
    u32 Addr = Page * embeddescription->BytesPerPage + embeddescription->MappedStAddr;
    u16 tmpDatNum  = 0;
    bool_t bStatus = 0;

    if(!Data)
        return (-1);
    //write 4 bytes each time
    while(tmpDatNum < embeddescription->BytesPerPage)
    {
        //��ַ���
        if(Addr > 1024 * 1024)
        {
            printf("out of range of flash,please check it\r\n");
            return -1;
        }
        atom_low_t atomic_low = Int_LowAtomStart();
//        bStatus = FLASH_Program_Word_In_Page((u32*)Addr, pData, 1);
        bStatus = FLASH_Program_Single_Word((u32*)Addr, pData[0]);
        Int_LowAtomEnd(atomic_low);
        BrdWdt_FeedDog();
        if(bStatus)
        {

        }else{
            printf("program %x failed\r\n",Addr);
            return Ret;
        }
        pData++;
        tmpDatNum += 4;
        Addr += 4;
    }

    return (tmpDatNum);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 Flash_PageRead(u32 Page, u8 *Data, u32 Flags)
{
    u32 Addr = Page * embeddescription->BytesPerPage + embeddescription->MappedStAddr;

    if(!Data)
        return (-1);

    memcpy(Data, (u8*)Addr, embeddescription->BytesPerPage);
    return (embeddescription->BytesPerPage);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 Flash_PageToSector(u32 PageNo, u32 *Remains, u32 *SectorNo)
{

    return (0);
}

//-----------------------------------------------------------------------------
// ���ܣ���װƬ��Flash����
// ������doformat -- �Ƿ��ʽ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע���������֪��Ҫ��װʲô�ļ�ϵͳ�����߲���װ�ļ�ϵͳTargetFs��NULL��TargetPart��-1��
//-----------------------------------------------------------------------------
s32 ModuleInstall_EmbededFlash(u32 doformat)
{
    if(!embeddescription)
    {
        embeddescription = malloc(sizeof(*embeddescription));
        if(!embeddescription)
        {
            return (-1);
        }

        EmFlash_Init(embeddescription);
    }

    if(doformat)
    {
        struct uesz sz;
        sz.unit = 0;
        sz.block = 1;
        __embed_req(format, (ptu32_t)0 , -1, &sz);           //��ʽ��ָ������
    }

    emflash_um = malloc(sizeof(struct umedia)+embeddescription->BytesPerPage);
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

    return 0;
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
        case remain:         //��ҳ���ڵĿ��л�ʣ�����ҳ,ҳ�������ε����ӵ�
        {
            va_list list;
            u32 *left;
            s64 *unit;

            left = (u32*)args;
            va_start(list, args);
            unit = (s64*)va_arg(list, u32);
            va_end(list);

            if(*unit < embeddescription->TotalPages)
            {
                u16 CurPageInBlkNo;
                CurPageInBlkNo = (*unit) / embeddescription->PagesPerBlock;
                *left = (CurPageInBlkNo + 1)* embeddescription->PagesPerBlock - (*unit + 1);
            }
            else
                res = -1;

            break;
        }

        case whichblock:   //��ҳ���ڿ�
        {
            va_list list;
            s64 *unit;
            u32 *block;

            block = (u32*)args;
            va_start(list, args);
            unit = (s64*)va_arg(list, u32);
            va_end(list);

            if(*unit < embeddescription->TotalPages)
            {
                *block = *unit / embeddescription->PagesPerBlock;
            }
            else
                res = -1;

            break;
        }

        case totalblocks:
        {
            // args = &blocks
            *((u32*)args) = embeddescription->TotalBlocks;
            break;
        }

        case blockunits:   //ÿ���е�ҳ����
        {
           va_list list;
           u32 *units = (u32*)args;
           u32 block;

           va_start(list, args);
           block = (u32)va_arg(list, u32);
           va_end(list);

           *units = embeddescription->PagesPerBlock;
           break;
        }

        case unitbytes://ÿҳ�е��ֽ���
        {
            // args = &bytes
            *((u32*)args) = embeddescription->BytesPerPage;
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

            if(-1==(s32)end)//�����-1����ô�������������оƬ�������
                end = embeddescription->TotalBlocks;

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

            *((u32*)args) = embeddescription->MappedStAddr;
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
    if(res!=embeddescription->BytesPerPage)
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
    if(res!=embeddescription->BytesPerPage)
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

    if(mediadrv == NULL)
        return -1;
    targetobj = OBJ_MatchPath(fs, &notfind);
    if(notfind)
    {
        error_printf("embed"," not found need to install file system.");
        return -1;
    }
    super = (struct FsCore *)OBJ_GetPrivate(targetobj);
    super->MediaInfo = emflash_um;
    super->MediaDrv = mediadrv;

    if(-1 == (s32)endblock)
    {
        endblock = bend = embeddescription->TotalBlocks;// �����

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

    super->AreaSize = total * embeddescription->BytesPerPage;
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

