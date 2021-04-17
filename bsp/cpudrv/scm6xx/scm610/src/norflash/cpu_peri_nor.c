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

//˼·:
//   ����һ��buf��ʾҳ��oob����,�����ǽ����Ƿֿ���ʾ��ԭ���Ƕ���������ʱ,ECCУ��
//   ȴ����Ҫ����ҳ��OOB���ݡ���˴�ECCУ��ʱ,ÿ�ζ����д������ҳ��OOB�����ݲ���
//

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <device.h>
#include <device/djy_flash.h>
#include <cpu_peri.h>
#include <int.h>
#include <device/unit_media.h>
#include <dbug.h>
#include <djyfs/filesystems.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�


//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//   extern s32 ModuleInstall_NorFlash(u32 doformat)
//   ModuleInstall_NorFlash(CFG_NFLASH_PART_FORMAT);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu peripheral register definition"//CPU��nor����
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"device file system","file system"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"xip_app","xip_iboot"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_PERIPHERAL_REGISTER_DEFINITION == false )
//#warning  " cpu_peripheral_register_definition  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_PERIPHERAL_REGISTER_DEFINITION    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@enum,true,false,
#define CFG_NORFLASH_PART_FORMAT     false      //�Ƿ���Ҫ������оƬ��
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure

//W25Q64������С8M-ÿҳ256Bytes ÿ����4K ÿ�� 32k����64K����

static struct umedia *emflash_um;

struct NorFlashDescr{
    u16     BytesPerPage;                // ҳ�а������ֽ���-256Bytes
    u16     BytesPerSector;              // �����е��ֽ���
    u16     PagesPerSector;              // ÿ�����е�ҳ��
//    u16     SectorsPerBlock;           // ÿ���е�������
    u16     PagesPerBlock;               //ÿ���е�ҳ��
    u32     TotalPages;                  // �ܵ�ҳ��
    u32     TotalBlocks;                  //�ܵĿ���
    u32     MappedStAddr;
} *sgpt_NorFlash;


#define CN_PAGE_SIZE   (256)             //ҳ��С
#define CN_SECTOR_SIZE (4*1024)          //������С
#define CN_BLOCK_SIZE  (64*1024)         //���С
#define CN_CHIP_SIZE   (8*1024*1024)     //оƬ��С

extern void flash_write_bytes(uint32_t addr,uint8_t *buf, uint8_t len);
extern void flash_read_bytes(uint32_t addr,uint8_t *buf,uint32_t len);
extern void flash_erase(uint8_t cmd,u32 addr);
static bool_t sflashInited = false;
static struct MutexLCB *sgpt_FlashLock;
#define NFlashLockTimeOut 0xffffffff

static const char *flashName = "norFlash";      //��flash��obj�ڵ�����

//�½ӿ�
extern struct Object *s_ptDeviceRoot;
//s32 __Flash_FsInstallInit(const char *fs, u32 bstart, u32 bcount);
static s32 __Flash_init(void);

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
// ���ܣ�Flash ��Ϣ��ʼ��
// ��������ڲ���:NONE
// ���أ�
// ��ע��
// ============================================================================
static s32 __Flash_init(void)
{
    if(!sgpt_NorFlash)
        return (-1);

    if(sgpt_NorFlash == NULL)
    {
        sgpt_FlashLock = Lock_MutexCreate("Nor Flash Lock");        //����nand�Ļ�����
    }
//    stm32f7_NAND_ControllerConfig();// оƬ�ܽŵȻ�������

    sgpt_FlashLock = Lock_MutexCreate("Nand Flash Lock");        //����nand�Ļ�����
    sgpt_NorFlash->BytesPerPage   = CN_PAGE_SIZE;
    sgpt_NorFlash->BytesPerSector = CN_SECTOR_SIZE;
    sgpt_NorFlash->PagesPerBlock  = CN_BLOCK_SIZE / CN_PAGE_SIZE;
    sgpt_NorFlash->TotalBlocks    = CN_CHIP_SIZE / CN_BLOCK_SIZE;
    sgpt_NorFlash->PagesPerSector = CN_SECTOR_SIZE /CN_PAGE_SIZE;
    sgpt_NorFlash->MappedStAddr   = 0x0;
    sgpt_NorFlash->TotalPages     = CN_CHIP_SIZE / CN_PAGE_SIZE;

    return (0);
}

//-----------------------------------------------------------------------------
//����:��ȡflash��Ϣ
//����:
//����: "0" -- �ɹ�; "-1" -- ����ʧ��; "-2" -- �ڴ治��;
//��ע:
//-----------------------------------------------------------------------------

static s32 Flash_GetDescr(struct NorFlashDescr *Descr)
{
    if(Descr == NULL)
    {
        return -1;
    }

    Descr->BytesPerPage   = sgpt_NorFlash->BytesPerPage;
    Descr->BytesPerSector = sgpt_NorFlash->BytesPerSector;
    Descr->PagesPerBlock  = sgpt_NorFlash->PagesPerBlock;
    Descr->TotalBlocks    = sgpt_NorFlash->TotalBlocks;
    Descr->TotalPages     = sgpt_NorFlash->TotalPages;
    Descr->PagesPerSector = sgpt_NorFlash->PagesPerSector;
    Descr->MappedStAddr   = sgpt_NorFlash->MappedStAddr;

    return (0);
}

//-----------------------------------------------------------------------------
//����:����� - ��������ÿ���СΪ64K
//����:���
//����: "0" -- �ɹ�;"-1" -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------

s32 Flash_BlockErase(u32 BlkNo)
{
    u16 BlockAddress;

    BlockAddress = BlkNo * 64*1024;

    if(Lock_MutexPend(sgpt_FlashLock, NFlashLockTimeOut) == false)
    {
        return (-1);
    }
    #ifndef FLASH_BLOCK_64K_ERASE
    #define FLASH_BLOCK_64K_ERASE 0xD8
    #endif
    flash_erase(FLASH_BLOCK_64K_ERASE,BlockAddress);
    Lock_MutexPost(sgpt_FlashLock);

    return (0);    //�ɹ�
}


//-----------------------------------------------------------------------------
//����:
//����: Data -- ���ݿռ䡣����ΪNULL��
//      Flags --
//����: ">0" -- ������д����ֽ���;
//      "-1" -- ��������;
//      "-2" -- дʧ��;
//��ע:
//-----------------------------------------------------------------------------
s32 Flash_PageProgram(u32 PageNo, u8 *Data, u32 Flags)
{
    u8 *Buf = (u8 *)Data;
    u32 PageAddress;

    if(Lock_MutexPend(sgpt_FlashLock, NFlashLockTimeOut) == false)
    {
        return (-2);
    }

    PageAddress = sgpt_NorFlash->MappedStAddr + PageNo*sgpt_NorFlash->BytesPerPage;
    flash_write_bytes(PageAddress,Buf,128);
    flash_write_bytes(PageAddress+128,Buf+128,128);

    Lock_MutexPost(sgpt_FlashLock);

    return (sgpt_NorFlash->BytesPerPage);

}

//-----------------------------------------------------------------------------
//����:
//����:
//
//��ע: ���ܶ�д�Ƿ���ȷ���������ݻش�
//-----------------------------------------------------------------------------

s32  Flash_PageRead(u32 PageNo, u8 *Data, u32 Flags)
{
    //�߼�:
    u8 *Buf = (u8 *)Data;
    u32 PageAddress;

    if(Lock_MutexPend(sgpt_FlashLock, NFlashLockTimeOut) == false)
    {
        return (-2);
    }

    PageAddress = sgpt_NorFlash->MappedStAddr + PageNo*sgpt_NorFlash->BytesPerPage;
    flash_read_bytes(PageAddress,Buf,sgpt_NorFlash->BytesPerPage);
    Lock_MutexPost(sgpt_FlashLock);

    return (sgpt_NorFlash->BytesPerPage);
}

u32 Flash_ReadID(void)
{
    u32 id;

    id = flash_read_jedec_id();

    return id;
}

// ============================================================================
// ���ܣ�nand ����
// ������ucmd -- ���
//      ���� -- ���������
// ���أ�
// ��ע��
// ============================================================================
s32 __Flash_req(enum ucmd cmd, ptu32_t args, ...)
{
    s32 res = 0;

    switch(cmd)
    {
        case remain:                                 //���ڿ�ʣ��ҳ
        {
            va_list list;
            u32 *left;
            s64 *unit;

            left = (u32*)args;
            va_start(list, args);
            unit = (s64*)va_arg(list, u32);
            va_end(list);

            *left = sgpt_NorFlash->PagesPerBlock - (*unit + 1) % sgpt_NorFlash->PagesPerBlock;
            break;
        }
        case whichblock:                              //��һ��
        {
            va_list list;
            u32 *block;
            s64 unit;

            block = (u32*)args;
            va_start(list, args);
            unit = (s64)va_arg(list, u32);            //unit �ڼ�ҳ
            va_end(list);
            *block = unit / sgpt_NorFlash->PagesPerBlock;
            break;
        }

        case totalblocks:                             //�ܵĿ���
        {
            // args = &blocks
            *((u32*)args) =  sgpt_NorFlash->TotalBlocks;
            break;
        }

        case blockunits:                              //ÿ���е�ҳ����
        {
            // args = &units
            *((u32*)args) = sgpt_NorFlash->PagesPerBlock;
            break;
        }

        case unitbytes:                               //ÿҳ���ֽ���
        {
            // args = &bytes
            *((u32*)args) = sgpt_NorFlash->BytesPerPage;
            break;
        }

        case format:
        {
            va_list list;
            u32 start, end;
            struct uesz *sz;

            start = (u32)args;                    //��ʼ��
            va_start(list, args);
            end = va_arg(list, u32);              //������
            sz = (struct uesz*)va_arg(list, u32); //��С
            va_end(list);

            if(!sz->block)
                return (-1);

            if(-1==(s32)end)                      //��������ж�
                end = CN_CHIP_SIZE / CN_BLOCK_SIZE;// �����ĺ�

            do
            {
                if(__Flash_erase((s64)--end, *sz))
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

           *((u32*)args) = sgpt_NorFlash->MappedStAddr;
           break;
        }
        case checkbad: break;
        default: res = -1; break;
    }

    return (res);
}

// ============================================================================
// ���ܣ�nand ����
// ������unit -- ������ţ�ҳ����
//      data -- �������ݣ�
//      opt -- ���ķ�ʽ��
// ���أ��ɹ� -- ��0����ʧ�� -- ��-1��
// ��ע��
// ============================================================================
s32 __Flash_read(s64 unit, void *data, struct uopt opt)
{
    s32 res;

    res = Flash_PageRead((u32)unit, data, 0);
    if(res!=sgpt_NorFlash->BytesPerPage)
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

s32 __Flash_write(s64 unit, void *data, struct uopt opt)
{
    s32 res;

    res = Flash_PageProgram((u32)unit, data, 0);
    if(res!=sgpt_NorFlash->BytesPerPage)
      return (-1);

    return (0);
}


// ============================================================================
// ���ܣ�nand ����
// ������unit -- ��������ţ�
//      sz -- �����ĵ�λ��unit��block��
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================

s32 __Flash_erase(s64 unit, struct uesz sz)
{
    u32 block;

    if(sz.unit)
    {
        if(__Flash_req(whichblock, (ptu32_t)&block, &unit))
            return (-1);
    }
    else
        block = (u32)unit;

    return (Flash_BlockErase(block));

}


// ============================================================================
// ���ܣ���ʼ��Ƭ��flash
// ������fs -- ��Ҫ���ص��ļ�ϵͳ��MountPart -- ���ص���ý��ĵڼ���������������0��ʼ��
//       bstart -- ��ʼ�飬bend -- �����飨�������ÿ飬ֻ���ÿ����һ�飩
// ���أ�0 -- �ɹ��� -1 -- ʧ��
// ��ע��
// ============================================================================

s32 __Flash_FsInstallInit(const char *fs, u32 bstart, u32 bend, void *mediadrv)
{
    u32 units, total = 0,endblock = bend;
    char *FullPath,*notfind;
    struct Object *targetobj;
    struct FsCore *super;
    s32 res;
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
        endblock = bend = 128;// �����
    }

    do
    {
        if(__Flash_req(blockunits, (ptu32_t)&units, --endblock))
        {
            return (-1);
        }

        total += units;     //����÷���һ���ж���ҳ
    }
    while(endblock!=bstart);

    super->AreaSize = total * sgpt_NorFlash->BytesPerPage;
    endblock = 0;
    total = 0;
    while(endblock<bstart)
    {
        if(__Flash_req(blockunits, (ptu32_t)&units, endblock++))
        {
            return (-1);
        }
        total += units;
    }

    super->MediaStart = total; // ��ʼunit��

    res = strlen(flashName) + strlen(s_ptDeviceRoot->name) + 1;
    FullPath = malloc(res);
    memset(FullPath, 0, res);
    sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,flashName);   //��ȡ���豸��ȫ·��
    File_BeMedia(FullPath,fs); //�����豸�����ļ�ϵͳ
    free(FullPath);

    printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, bstart, bend);
    return (0);

}

//-----------------------------------------------------------------------------
// ���ܣ���װƬ��Flash����
// ������  doformat -- �ǲ���оƬ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
//-----------------------------------------------------------------------------
s32 ModuleInstall_NorFlash(u32 doformat)
{
    if(!sgpt_NorFlash)
    {
        sgpt_NorFlash = malloc(sizeof(*sgpt_NorFlash));
        if(!sgpt_NorFlash)
        {
            return (-1);
        }

        __Flash_init();
    }

    if(doformat)
    {
        struct uesz sz;
        sz.unit = 0;
        sz.block = 1;
        __Flash_req(format, (ptu32_t)0 , -1, &sz);           //��ʽ��ָ������
    }

    emflash_um = malloc(sizeof(struct umedia)+sgpt_NorFlash->BytesPerPage);
    if(!emflash_um)
    {
        return (-1);
    }
    emflash_um->mreq = __Flash_req;
    emflash_um->type = embed;
    emflash_um->ubuf = (u8*)emflash_um + sizeof(struct umedia);

    if(!Device_Create((const char*)flashName, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)emflash_um)))
    {
        printf("\r\n: erro : device : %s addition failed.", flashName);
        free(emflash_um);
        return (-1);
    }

    sflashInited = true;
    return 0;
}

// =============================================================================
// ���ܣ��ж�flash�Ƿ�װ
// ������  ��
// ���أ��ѳɹ���װ��true����δ�ɹ���װ��false����
// ��ע��
// =============================================================================
bool_t Norflash_is_install(void)
{
    return sflashInited;
}



