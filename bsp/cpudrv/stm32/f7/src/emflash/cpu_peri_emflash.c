//-----------------------------------------------------------------------------
// Copyright (c) 2018,Open source team. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ�ɶ����߲���ϵͳ��Դ�Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
#include <stm32f7xx_hal_flash.h>
#include <int.h>
#include <device/include/unit_media.h>

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//extern s32 __embed_part_init(u32 bstart, u32 bcount, u32 doformat);
//ModuleInstall_UnitMedia(__embed_part_init, CFG_PARTS, ...);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu_peri_emflash"     //Ƭ��flash��д
//parent:"iap"                          //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp���                      //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                            //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                        //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                      //��ʼ��ʱ������ѡֵ��early��medium��later��
                                        //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"iap","devfile","lock","stm32f7", //������������������������none����ʾ�������������
                                        //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                        //����������������������г�
//weakdependence:"none"                 //��������������������������none����ʾ�������������
                                        //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                        //����������������������г����á�,���ָ�
//mutex:"none"                          //������������������������none����ʾ�������������
                                        //����������������������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����

//%$#@num,0,100,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#define CFG_EMBEDED     __embed_part_init    //"������ʼ��"
//%$#@object_num,0,1,2,3,4,..,100
#define CFG_PARTS   0     //������
//%$#@object_para,
#define CFG_PART_START      //������ʼ
#define CFG_PART_SIZE       //������С
#define CFG_PART_OPTION     //����ѡ��
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure
// ============================================================================

static struct EmbdFlashDescr{
    u16     BytesPerPage;                // ҳ�а������ֽ���
    u16     PagesPerSmallSect;           // small sector��ҳ��
    u16     PagesPerLargeSect;           // large sector��ҳ��
    u16     PagesPerNormalSect;          // Normal sector��ҳ��
    u16     SmallSectorsPerPlane;        // ÿplane��small sector��
    u16     LargeSectorsPerPlane;        // ÿplane��large sector��
    u16     NormalSectorsPerPlane;       // ÿplane��normal sector��
    u16     Planes;                      // plane��
    u32     MappedStAddr;
} *embeddescription;

extern u32 gc_ptIbootSize;

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
    Description->BytesPerPage = 512;
    Description->PagesPerSmallSect = 64;
    Description->PagesPerLargeSect = 256;
    Description->PagesPerNormalSect = 512;
    Description->SmallSectorsPerPlane = 4;
    Description->LargeSectorsPerPlane = 1;
    Description->NormalSectorsPerPlane = 7; // STM32F767
    Description->Planes = 1;
    Description->MappedStAddr = 0x08000000;
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
    Description->BytesPerPage = embeddescription->BytesPerPage;
    Description->TotalPages = (embeddescription->PagesPerSmallSect *
                        embeddescription->SmallSectorsPerPlane +
                        embeddescription->PagesPerLargeSect *
                        embeddescription->LargeSectorsPerPlane +
                        embeddescription->PagesPerNormalSect *
                        embeddescription->NormalSectorsPerPlane) *
                        embeddescription->Planes;
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
static s32 Flash_SectorEarse(u32 SectorNo)
{
    u8 retry = 0; // �����п��ܻ�ʧ�ܣ�
    s32 Ret = -1;
    u32 SECTORError=0;//�������������Ϣ
    static FLASH_EraseInitTypeDef EraseInitStruct;

    do
    {
        HAL_FLASH_Unlock();
        EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
        EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
        EraseInitStruct.Sector        = SectorNo;
        EraseInitStruct.NbSectors     = 1;

        BrdWdt_FeedDog(); // ι��
        Int_CutTrunk();
        if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
             Ret=-1;
        else
            Ret=0;
        Int_ContactTrunk();
        BrdWdt_FeedDog(); // ι��
        HAL_FLASH_Lock();
        SCB_CleanInvalidateDCache();//�����������cache�������flash�������ݴ���
    }
    while(Ret&&((retry++)<1));

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
static s32 Flash_PageProgram(u32 Page, u8 *Data, u32 Flags)
{
    u32 Ret,i;
    u32 *pData = (u32*)Data;
    u32 Addr = Page * embeddescription->BytesPerPage + embeddescription->MappedStAddr;

    if(!Data)
        return (-1);

    HAL_FLASH_Unlock();

    for(i = 0; i < embeddescription->BytesPerPage;)
    {
        if(*(u32*)Addr != *pData)
        {
            BrdWdt_FeedDog(); // ι��
            Int_CutTrunk();
            Ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,Addr,*pData);
            Int_ContactTrunk();
            BrdWdt_FeedDog(); // ι��
            if(Ret != HAL_OK)
                break;
        }
        pData++;
        i += 4;
        Addr += 4;
    }

    HAL_FLASH_Lock();

    return (i);
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
    u32 PagesLeft, PagesDone;
    u32 i;
    u32 Sector;

    if((!Remains) || (!SectorNo))
        return (-1);

    Sector = 0;
    PagesDone = 0;
    PagesLeft = embeddescription->PagesPerSmallSect -
               (PageNo % embeddescription->PagesPerSmallSect);
    for(i = 1; i <= embeddescription->SmallSectorsPerPlane; i++)
    {
        if(PageNo < (PagesDone + embeddescription->PagesPerSmallSect * i))
            goto DONE;
        Sector++;
    }

    PagesDone += embeddescription->SmallSectorsPerPlane *
            embeddescription->PagesPerSmallSect;
    PagesLeft = embeddescription->PagesPerLargeSect -
                   (PageNo % embeddescription->PagesPerLargeSect);
    for(i = 1; i <= embeddescription->LargeSectorsPerPlane; i++)
    {
        if(PageNo < (PagesDone + embeddescription->PagesPerLargeSect * i))
            goto DONE;
        Sector++;
    }

    PagesDone += embeddescription->LargeSectorsPerPlane *
            embeddescription->PagesPerLargeSect;
    PagesLeft = embeddescription->PagesPerNormalSect -
                   (PageNo % embeddescription->PagesPerNormalSect);
    for(i = 1; i <= embeddescription->NormalSectorsPerPlane; i++)
    {
        if(PageNo < (PagesDone + embeddescription->PagesPerNormalSect * i))
            goto DONE;
        Sector++;
    }

    return (-1);

DONE:
    *SectorNo = Sector; // ��sector���
    *Remains = PagesLeft -1; // page�����
    return (0);
}

// ============================================================================
// ���ܣ�
// ������ResPages������ҳ��
// ���أ�
// ��ע��
// ============================================================================
s32 ModuleInstall_EmbededFlash(const char *ChipName, u32 Flags, u16 ResPages)
{
    u32 Len;
    struct FlashChip *Chip;
    struct EmFlashDescr FlashDescr;
    struct MutexLCB *FlashLock;
    u8 *Buf;
    s32 Ret = 0;

    if (!ChipName)
        return (-1);

    if(embeddescription)
        return (-4); // �豸��ע��

    embeddescription = malloc(sizeof(*embeddescription));
    if(!embeddescription)
        return (-1);

    EmFlash_Init(embeddescription);
    Flash_GetDescr(&FlashDescr);// ��ȡFLASH��Ϣ
    if(ResPages > FlashDescr.TotalPages)
    {
        Ret = -1;
        goto FAILURE;
    }

    FlashDescr.ReservedPages += ResPages;
    Len = strlen (ChipName) + 1;
    Chip = (struct FlashChip*)malloc(sizeof(struct FlashChip) + Len);
    if (NULL == Chip)
    {
        printf("\r\n: erro : device : out of memory.\r\n");
        Ret = -2;
        goto FAILURE;
    }

    memset(Chip, 0x00, sizeof(*Chip));
    Chip->dwPageBytes             = FlashDescr.BytesPerPage;
    Chip->dwPagesReserved         = FlashDescr.ReservedPages;
    Chip->dwTotalPages            = FlashDescr.TotalPages;
    Chip->Type                    = F_ALIEN;
    Chip->Descr.Embd              = FlashDescr;
    Chip->Ops.ErsBlk              = Flash_SectorEarse;
    Chip->Ops.WrPage              = Flash_PageProgram;
    Chip->Ops.RdPage              = Flash_PageRead;
    Chip->Ops.PageToBlk              = Flash_PageToSector;
    strcpy(Chip->Name, ChipName); // �豸��
    if(Flags & FLASH_BUFFERED)
    {
        Buf = (u8*)malloc(embeddescription->BytesPerPage); // NAND�ײ㻺��
        if(!Buf)
        {
            printf("\r\n: erro : device : out of memory.");
            Ret = -2;
            goto FAILURE;
        }

        FlashLock = Lock_MutexCreate("Embedded Flash Lock");
        if(!FlashLock)
        {
            Ret = -3;
            goto FAILURE;
        }

        Chip->Buf = Buf;
        Chip->Lock =(void*)FlashLock;
    }

    dev_add(NULL, Chip->Name, NULL, NULL, NULL, NULL, NULL, (ptu32_t)Chip); // �豸����"/dev"
    if(Flags & FLASH_ERASE_ALL)
        EarseWholeChip(Chip);

FAILURE:
    if(Ret)
    {
        if(embeddescription)
            free(embeddescription);
        if(FlashLock)
            Lock_MutexDelete(FlashLock);
        if(Buf)
            free(Buf);
        if(Chip)
            free(Chip);
    }
    return (Ret);
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
            if((*unit<64)&&(*unit>=0))
                *left = 63 - *unit;
            else if((*unit<128)&&(*unit>=64))
                *left = 127 - *unit;
            else if((*unit<192)&&(*unit>=128))
                *left = 191 - *unit;
            else if((*unit<256)&&(*unit>=192))
                *left = 255 - *unit;
            else if((*unit<512)&&(*unit>=256))
                *left = 511 - *unit;
            else if((*unit<1024)&&(*unit>=512))
                *left = 1023 - *unit;
            else if((*unit<1536)&&(*unit>=1024))
                *left = 1535 - *unit;
            else if((*unit<2048)&&(*unit>=1536))
                *left = 2047 - *unit;
            else if((*unit<2560)&&(*unit>=2048))
                *left = 2559 - *unit;
            else if((*unit<3072)&&(*unit>=2560))
                *left = 3071 - *unit;
            else if((*unit<3584)&&(*unit>=3072))
                *left = 3583 - *unit;
            else if((*unit<4096)&&(*unit>=3584))
                *left = 40951 - *unit;
            else
                res = -1;

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

            if((*unit<64)&&(*unit>=0))
                *block = 0;
            else if((*unit<128)&&(*unit>=64))
                *block = 1;
            else if((*unit<192)&&(*unit>=128))
                *block = 2;
            else if((*unit<256)&&(*unit>=192))
                *block = 3;
            else if((*unit<512)&&(*unit>=256))
                *block = 4;
            else if((*unit<1024)&&(*unit>=512))
                *block = 5;
            else if((*unit<1536)&&(*unit>=1024))
                *block = 6;
            else if((*unit<2048)&&(*unit>=1536))
                *block = 7;
            else if((*unit<2560)&&(*unit>=2048))
                *block = 8;
            else if((*unit<3072)&&(*unit>=2560))
                *block = 9;
            else if((*unit<3584)&&(*unit>=3072))
                *block = 10;
            else if((*unit<4096)&&(*unit>=3584))
                *block = 11;
            else
                res = -1;

            break;
        }

        case totalblocks:
        {
            // args = &blocks
            *((u32*)args) =  12;
            break;
        }

        case blockunits:
        {
            va_list list;
            u32 *units = (u32*)args;
            u32 block;

            va_start(list, args);
            block = (u32)va_arg(list, u32);
            va_end(list);

            switch(block)
            {
                case 0:
                case 1:
                case 2:
                case 3: *units = embeddescription->PagesPerSmallSect; break;
                case 4: *units = embeddescription->PagesPerLargeSect; break;
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:*units = embeddescription->PagesPerNormalSect; break;
                default: res = -1;
            }
            break;
        }

        case unitbytes:
        {
            // args = &bytes
            *((u32*)args) = embeddescription->BytesPerPage;
            break;
        }

        case format:
        {
            va_list list;
            u32 start, end;
            struct uesz *sz;

            start = (u32)args;
            va_start(list, args);
            end = va_arg(list, u32);
            sz = (struct uesz*)va_arg(list, u32);
            va_end(list);

            if(!sz->block)
                return (-1);

            if(-1==end)
                end = 12; // �����ĺţ�
            else if(start)
                end += start;

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
static s32 __embed_read(s64 unit, void *data, struct uopt opt)
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
static s32 __embed_write(s64 unit, void *data, struct uopt opt)
{
    s32 res;

    res = Flash_PageProgram((u32)unit, data, 0);
    if(res!=embeddescription->BytesPerPage)
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

    return (Flash_SectorEarse(block));
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
s32 __embed_part_init(u32 bstart, u32 bcount, u32 doformat)
{
    struct umedia *um;
    struct uopt opt;
    char name[16], part[3];
    u32 units, total = 0;
    static u8 count;

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
        __embed_req(format, (ptu32_t)bstart , bcount, &sz);
    }

    um = malloc(sizeof(struct umedia)+embeddescription->BytesPerPage);
    if(!um)
    {
        return (-1);
    }

    opt.hecc = 1;
    opt.main = 1;
    opt.necc = 1;
    opt.secc = 0;
    opt.spare = 0;
    if(-1 == bcount)
        bcount = 12; // �����
    else
        bcount += bstart; // �������

    do
    {
        if(__embed_req(blockunits, (ptu32_t)&units, --bcount))
        {
            return (-1);
        }

        total += units;
    }
    while(bcount!=bstart);

    um->asz = total * embeddescription->BytesPerPage;
    um->esz = 0; // ��������ͬ
    um->usz = log2(embeddescription->BytesPerPage);
    um->merase = __embed_erase;
    um->mread = __embed_read;
    um->mreq = __embed_req;
    um->mwrite = __embed_write;
    um->opt = opt;
    um->type = embed;
    um->ubuf = (u8*)um + sizeof(struct umedia);
    bcount = 0;
    total = 0;
    while(bcount<bstart)
    {
        if(__embed_req(blockunits, (ptu32_t)&units, bcount++))
        {
            return (-1);
        }

        total += units;
    }

    um->ustart = total; // ��ʼunit��
    itoa(count++, part, 10);
    sprintf(name, "embed part %s", part);
    if(um_add((const char*)name, um))
    {
        printf("\r\n: erro : device : %s addition failed.", name);
        return (-1);
    }

    printf("\r\n: info : device : %s added(start:%d, blocks:%d).", name, bstart, bcount);
    return (0);

}

