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
#include <device.h>
#include <device/flash/flash.h>
#include <cpu_peri.h>
#include <stm32l4xx_hal_flash.h>
#include <int.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern s32 ModuleInstall_EmbededFlash(const char *ChipName, u32 Flags, u16 ResPages);
//    ModuleInstall_EmbededFlash(CFG_FLASH_CHIP_NAME,CFG_FLASH_FLAG,CFG_FLASH_RES);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu_peri_emflash"                     //Ƭ��flash��д
//parent:"iapfs"                                        //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp���                                     //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                                           //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                                        //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                                      //��ʼ��ʱ������ѡֵ��early��medium��later��
                                                        //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"int","devfile","heap","lock","stm32l4"    //������������������������none����ʾ�������������
                                                        //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                                        //����������������������г����á�,���ָ�
//weakdependence:"none"                                 //��������������������������none����ʾ�������������
                                                        //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                                        //����������������������г����á�,���ָ�
//mutex:"none"                                          //������������������������none����ʾ�������������
                                                        //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef CFG_FLASH_FLAG   //****�������Ƿ��Ѿ����ú�
#warning    cpu_peri_emflash�������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,0,100,
#define CFG_FLASH_FLAG                   (2)        //"��Ǻ�",2��ʾʹ�û�������1��ʾ����ȫ����
#define CFG_FLASH_RES                    (0)        //"����ҳ�������",
//%$#@enum,true,false,
//%$#@string,1,32,
#define CFG_FLASH_CHIP_NAME              "embedded flash"    //"flashоƬ����",
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
static struct EmbdFlashDescr{
    u16     BytesPerPage;                   // ҳ�а������ֽ���
    u16     PagesPerSmallSect;           // small sector��ҳ��
    u16     PagesPerLargeSect;           // large sector��ҳ��
    u16     PagesPerNormalSect;          // Normal sector��ҳ��
    u16     SmallSectorsPerPlane;        // ÿplane��small sector��
    u16     LargeSectorsPerPlane;        // ÿplane��large sector��
    u16     NormalSectorsPerPlane;       // ÿplane��normal sector��
    u16     Planes;                      // plane��
    u32     MappedStAddr;
} *s_ptEmbdFlash;

extern u32 gc_ptIbootSize;
//extern u32 gc_ptFlashOffset;
extern u32 gc_ptFlashRange;

// ============================================================================
// ����: ι��
// ����:
// ����:
// ��ע: �������ļ�max706.cʵ��
// ============================================================================
extern bool_t BrdWdt_FeedDog(void) __attribute__((weak));
bool_t BrdWdt_FeedDog(void)
{
    return (TRUE); // �պ���
}

// ============================================================================
// ����: ��ȡ����FLASH����Ϣ
// ����:
// ����:
// ��ע:
// ============================================================================
static s32 EmFlash_Init(struct EmbdFlashDescr *Description)
{
    Description->BytesPerPage = 512;
    Description->PagesPerSmallSect = 4;
    Description->PagesPerLargeSect = 4;
    Description->PagesPerNormalSect = 4;
    Description->SmallSectorsPerPlane = 0;
    Description->LargeSectorsPerPlane = 0;
    Description->NormalSectorsPerPlane = gc_ptFlashRange/2048;
    Description->Planes = 1;
    Description->MappedStAddr = 0x08000000;
    return (0);
}

// ============================================================================
// ����: ��ȡ����FLASH����Ϣ
// ����:
// ����:
// ��ע:
// ============================================================================
static s32 Flash_GetDescr(struct EmFlashDescr *Description)
{
    Description->BytesPerPage = s_ptEmbdFlash->BytesPerPage;
    Description->TotalPages = (s_ptEmbdFlash->PagesPerSmallSect *
                        s_ptEmbdFlash->SmallSectorsPerPlane +
                        s_ptEmbdFlash->PagesPerLargeSect *
                        s_ptEmbdFlash->LargeSectorsPerPlane +
                        s_ptEmbdFlash->PagesPerNormalSect *
                        s_ptEmbdFlash->NormalSectorsPerPlane) *
                       s_ptEmbdFlash->Planes;
    Description->ReservedPages = gc_ptIbootSize / s_ptEmbdFlash->BytesPerPage;
    Description->MappedStAddr = s_ptEmbdFlash->MappedStAddr;
    return (0);
}
// ============================================================================
// ����: ������
// ����: BlkNo -- ���;
// ����: "0" -- �ɹ�;"-1" -- ʧ��;
// ��ע:
// ============================================================================
static s32 Flash_SectorEarse(u32 SectorNo)
{
    s32 Ret;
    u32 SECTORError=0;//�������������Ϣ
    u8 bank,page;
    static FLASH_EraseInitTypeDef EraseInitStruct;

    if(SectorNo < gc_ptFlashRange/4096)
    {
        bank = FLASH_BANK_1;
        page = SectorNo;
    }
    else
    {
        bank = FLASH_BANK_2;
        page = SectorNo - gc_ptFlashRange/4096;
    }

    HAL_FLASH_Unlock();

    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.Banks       = bank;
    EraseInitStruct.Page        = page;
    EraseInitStruct.NbPages     = 1;

    BrdWdt_FeedDog(); // ι��
    Int_CutTrunk();
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
         Ret=-1;
    else
        Ret=0;
    Int_ContactTrunk();
    BrdWdt_FeedDog(); // ι��

    HAL_FLASH_Lock();

    return Ret;
}
// ============================================================================
// ����: дĳҳ
// ����: PageNo -- ҳ��;
//       Data -- д����;
//       Flags -- д��������λ;
// ����: ">0" -- ������д����ֽ���;
//       "-1" -- ��������;
//       "-2" -- дʧ��;
// ��ע:
// ============================================================================
static s32 Flash_PageProgram(u32 Page, u8 *Data, u32 Flags)
{
    u32 Ret,i;
    u64 *pData = (u64*)Data;

    u32 Addr = Page * s_ptEmbdFlash->BytesPerPage + s_ptEmbdFlash->MappedStAddr;

    if(!Data)
        return (-1);

    HAL_FLASH_Unlock();

    for(i = 0; i < s_ptEmbdFlash->BytesPerPage;)
    {
        if(*(u64*)Addr != *pData)
        {
            BrdWdt_FeedDog(); // ι��
            Int_CutTrunk();
            Ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,Addr,*pData);
            BrdWdt_FeedDog(); // ι��
            Int_ContactTrunk();
            if(Ret != HAL_OK)
            {
                break;
            }
        }
        pData++;
        i += 8;
        Addr += 8;
    }

    HAL_FLASH_Lock();

    return i;
}
// ============================================================================
// ����:
// ����:
// ����:
// ��ע:
// ============================================================================
static s32 Flash_PageRead(u32 Page, u8 *Data, u32 Flags)
{
    u32 Addr = Page * s_ptEmbdFlash->BytesPerPage + s_ptEmbdFlash->MappedStAddr;

    if(!Data)
        return (-1);

    memcpy(Data, (u8*)Addr, s_ptEmbdFlash->BytesPerPage);
    return (s_ptEmbdFlash->BytesPerPage);
}

// ============================================================================
// ����:
// ����:
// ����:
// ��ע:
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
    PagesLeft = s_ptEmbdFlash->PagesPerSmallSect -
               (PageNo % s_ptEmbdFlash->PagesPerSmallSect);
    for(i = 1; i <= s_ptEmbdFlash->SmallSectorsPerPlane; i++)
    {
        if(PageNo < (PagesDone + s_ptEmbdFlash->PagesPerSmallSect * i))
            goto DONE;
        Sector++;
    }

    PagesDone += s_ptEmbdFlash->SmallSectorsPerPlane *
            s_ptEmbdFlash->PagesPerSmallSect;
    PagesLeft = s_ptEmbdFlash->PagesPerLargeSect -
                   (PageNo % s_ptEmbdFlash->PagesPerLargeSect);
    for(i = 1; i <= s_ptEmbdFlash->LargeSectorsPerPlane; i++)
    {
        if(PageNo < (PagesDone + s_ptEmbdFlash->PagesPerLargeSect * i))
            goto DONE;
        Sector++;
    }

    PagesDone += s_ptEmbdFlash->LargeSectorsPerPlane *
            s_ptEmbdFlash->PagesPerLargeSect;
    PagesLeft = s_ptEmbdFlash->PagesPerNormalSect -
                   (PageNo % s_ptEmbdFlash->PagesPerNormalSect);
    for(i = 1; i <= s_ptEmbdFlash->NormalSectorsPerPlane; i++)
    {
        if(PageNo < (PagesDone + s_ptEmbdFlash->PagesPerNormalSect * i))
            goto DONE;
        Sector++;
    }

    return (-1);

DONE:
    *SectorNo = Sector; // sector�����
    *Remains = PagesLeft - 1; // page�����
    return (0);
}
// ============================================================================
// ����:
// ����: ResPages������ҳ��
// ����:
// ��ע:
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

    if(s_ptEmbdFlash)
        return (-4); // �豸��ע��

    s_ptEmbdFlash = malloc(sizeof(*s_ptEmbdFlash));
    if(!s_ptEmbdFlash)
        return (-1);

    EmFlash_Init(s_ptEmbdFlash);
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
        TraceDrv(FLASH_TRACE_ERROR, "out of memory!\r\n");
        Ret = -2;
        goto FAILURE;
    }

    memset(Chip, 0x00, sizeof(*Chip));
    Chip->dwPageBytes             = FlashDescr.BytesPerPage;
    Chip->dwPagesReserved         = FlashDescr.ReservedPages;
    Chip->dwTotalPages              = FlashDescr.TotalPages;
    Chip->Type                    = F_ALIEN;
    Chip->Descr.Embd              = FlashDescr;
    Chip->Ops.ErsBlk              = Flash_SectorEarse;
    Chip->Ops.WrPage              = Flash_PageProgram;
    Chip->Ops.RdPage              = Flash_PageRead;
    Chip->Ops.PageToBlk              = Flash_PageToSector;
    strcpy(Chip->Name, ChipName); // �豸��
    if(Flags & FLASH_BUFFERED)
    {
        Buf = (u8*)malloc(s_ptEmbdFlash->BytesPerPage); // NAND�ײ㻺��
        if(!Buf)
        {
            TraceDrv(FLASH_TRACE_ERROR, "out of memory!\r\n");
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

    dev_add(NULL,Chip->Name, NULL, NULL, NULL, NULL, NULL, (ptu32_t)Chip); // �豸����"/dev"
    if(Flags & FLASH_ERASE_ALL)
        EarseWholeChip(Chip);

FAILURE:
    if(Ret)
    {
        if(s_ptEmbdFlash)
            free(s_ptEmbdFlash);
        if(FlashLock)
            Lock_MutexDelete(FlashLock);
        if(Buf)
            free(Buf);
        if(Chip)
            free(Chip);
    }
    return (Ret);
}
