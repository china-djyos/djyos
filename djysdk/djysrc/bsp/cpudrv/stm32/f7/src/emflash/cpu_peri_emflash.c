//-----------------------------------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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
#include <stm32f7xx_hal_flash.h>
#include <int.h>


#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���

//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//  extern s32 ModuleInstall_EmbededFlash(const char *ChipName, u32 Flags, u16 ResPages);
//  ModuleInstall_EmbededFlash(cpu_peri_emflash_chipname,cpu_peri_emflash_Flags,cpu_peri_emflash_ResPages );
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu_peri_emflash"      //��д�����������
//parent:"iap"                        //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp���                      //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                        //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//grade:init                            //��ʼ��ʱ������ѡֵ��none��init��main��none��ʾ�����ʼ����
                                        //init��ʾ�ڵ���main֮ǰ��main��ʾ��main�����г�ʼ��
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
#ifndef cpu_peri_emflash_chipname   //****�������Ƿ��Ѿ����ú�
#warning    cpu_peri_emflash_chipname �������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,0,100,
#define cpu_peri_emflash_ResPages   0     //"����ҳ��",����ҳ�������û��Լ������������ļ�ϵͳ
//%$#@enum,true,false,
#define cpu_peri_emflash_Flags    0  //"FLASH��־",Ϊ1ʱ��֧�ֻ��棬Ϊ0ʱ��������������
//%$#@string,1,10,
#define CFG_MYCONFIG3   "embedded flash"     //"оƬ����",
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#endif
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
} *s_ptEmbdFlash;

extern u32 gc_ptIbootSize;

// ============================================================================
// ���ܣ�ι��
// ������
// ���أ�
// ��ע���������ļ�max706.cʵ��
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
// ���ܣ�������
// ������BlkNo -- ���;
// ���أ�"0" -- �ɹ�;"-1" -- ʧ��;
// ��ע��
// ============================================================================
static s32 Flash_SectorEarse(u32 SectorNo)
{
    s32 Ret;
    u32 SECTORError=0;//�������������Ϣ
    static FLASH_EraseInitTypeDef EraseInitStruct;

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

    u32 Addr = Page * s_ptEmbdFlash->BytesPerPage + s_ptEmbdFlash->MappedStAddr;

    if(!Data)
        return (-1);

    HAL_FLASH_Unlock();

    for(i = 0; i < s_ptEmbdFlash->BytesPerPage;)
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
    u32 Addr = Page * s_ptEmbdFlash->BytesPerPage + s_ptEmbdFlash->MappedStAddr;

    if(!Data)
        return (-1);

    memcpy(Data, (u8*)Addr, s_ptEmbdFlash->BytesPerPage);
    return (s_ptEmbdFlash->BytesPerPage);
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

    dev_add(NULL, Chip->Name, NULL, NULL, NULL, NULL, NULL, (ptu32_t)Chip); // �豸����"/dev"
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
