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

#include <stdlib.h>
#include <string.h>
#include <device.h>
#include <device/flash/flash.h>
#include  "cpu_peri.h"
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
//component name:"cpu_peri_emflash" //Ƭ��flash��д
//parent:"iap"                      //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                     //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                  //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                    //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                   //��ʼ��ʱ������ѡֵ��early��medium��later��
                                    //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"int","devfile","heap","lock"      //������������������������none����ʾ�������������
                                    //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                    //����������������������г����á�,���ָ�
//weakdependence:"none"             //��������������������������none����ʾ�������������
                                    //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                    //����������������������г����á�,���ָ�
//mutex:"none"                      //������������������������none����ʾ�������������
                                    //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef CFG_FLASH_FLAG  //****�������Ƿ��Ѿ����ú�
#warning    cpu_peri_emflash�������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,0,100,
#define CFG_FLASH_FLAG                   (2)        //"��Ǻ�",2��ʾʹ�û�������1��ʾ����ȫ����
#define CFG_FLASH_RES                    (0)        //"����ҳ�������",
//%$#@enum,true,false,
//%$#@string,1,30,
#define CFG_FLASH_CHIP_NAME              "embedded flash"     //"оƬ����",
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
//-----------------------------------------------------------------------------

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
} *sp_tFlashDesrc;
extern u32 gc_ptIbootSize;
extern u32 gc_ptFlashOffset;
extern u32 gc_ptFlashRange;

// ============================================================================
// ���ܣ�ι��
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
// ���ܣ�����FLASH�ĳ�ʼ��
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 Flash_Init(struct EmbdFlashDescr *Description)
{
    Description->BytesPerPage = 2048;
    Description->PagesPerSmallSect = 0;
    Description->PagesPerLargeSect = 0;
    Description->PagesPerNormalSect = 1;
    Description->SmallSectorsPerPlane = 0;
    Description->LargeSectorsPerPlane = 0;
    Description->NormalSectorsPerPlane = gc_ptFlashRange/2048;
    Description->Planes = 1;
    Description->MappedStAddr = gc_ptFlashOffset;
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
    Description->BytesPerPage = sp_tFlashDesrc->BytesPerPage;
    Description->TotalPages = (sp_tFlashDesrc->PagesPerSmallSect *
                               sp_tFlashDesrc->SmallSectorsPerPlane +
                               sp_tFlashDesrc->PagesPerLargeSect *
                               sp_tFlashDesrc->LargeSectorsPerPlane +
                               sp_tFlashDesrc->PagesPerNormalSect *
                               sp_tFlashDesrc->NormalSectorsPerPlane) *
                              sp_tFlashDesrc->Planes;
    Description->ReservedPages = gc_ptIbootSize / sp_tFlashDesrc->BytesPerPage;
    Description->MappedStAddr = sp_tFlashDesrc->MappedStAddr;
    return (0);
}

// ============================================================================
// ���ܣ�����ҳ
// ������pageNo -- ���;
// ���أ�"0" -- �ɹ�;"-1" -- ʧ��;
// ��ע��
// ============================================================================
static s32 Flash_SectorEarse(u32 Page)
{
    u32 Addr;
    u32 PAGEError = 0;//�洢��������
    s32 flag = 0;
    static FLASH_EraseInitTypeDef EraseInitStruct;

    HAL_FLASH_Unlock();
    Addr = Page * sp_tFlashDesrc->BytesPerPage + sp_tFlashDesrc->MappedStAddr;

    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = Addr;
    EraseInitStruct.NbPages     = 1;

    BrdWdt_FeedDog(); // ι��
    Int_CutTrunk();
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
        flag = -1;
    Int_ContactTrunk();
    BrdWdt_FeedDog(); // ι��
    HAL_FLASH_Lock();
    return flag;
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
    u32 i;
    u32 *pData = (u32*)Data;
    u32 Addr = Page * sp_tFlashDesrc->BytesPerPage + sp_tFlashDesrc->MappedStAddr;

    if(!Data)
        return (-1);

    HAL_FLASH_Unlock();
    for(i = 0; i < sp_tFlashDesrc->BytesPerPage; )
    {
        if(*(u32*)Addr != *pData)
        {
            BrdWdt_FeedDog(); // ι��
            Int_CutTrunk();
            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Addr, *pData) != HAL_OK)
            {
                Int_ContactTrunk();
                BrdWdt_FeedDog(); // ι��
                break;
            }
            Int_ContactTrunk();
            BrdWdt_FeedDog(); // ι��
        }
        pData++;
        i += 4;
        Addr += 4;;
    }
    HAL_FLASH_Unlock();
    return (i);
}

// ============================================================================
// ���ܣ���ĳҵ
// ������PageNo--ҳ��;
//       Data  --������;
//       Flags --����������λ;
// ���أ�
// ��ע��
// ============================================================================
static s32 Flash_PageRead(u32 Page, u8 *Data, u32 Flags)
{
    u32 Addr = Page * sp_tFlashDesrc->BytesPerPage + sp_tFlashDesrc->MappedStAddr;

    if(!Data)
        return (-1);

    memcpy(Data, (u8*)Addr, sp_tFlashDesrc->BytesPerPage);
    return (sp_tFlashDesrc->BytesPerPage);

}

// ============================================================================
// ���ܣ�����page����sector
// ������PageNo -- ҳ��
//       Remains -- ʣ��ҳ��
//       SectorNo -- ҳ����sector
// ���أ�
// ��ע�� sector0��normal sector��Сʱһ���ģ��������ֿ�����ֻ��Ϊ�˱����Ķ����
// ============================================================================
s32 Flash_PageToSector(u32 PageNo, u32 *Remains, u32 *SectorNo)
{
    if((!Remains) || (!SectorNo))
        return (-1);

    *Remains  = 1;
    *SectorNo = PageNo;

    return (0);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
s32 ModuleInstall_EmbededFlash(const char *ChipName, u32 Flags, u16 ResPages)
{

    u32 Len;
    struct FlashChip *Chip;
    struct MutexLCB *FlashLock;
    struct EmFlashDescr FlashDescr;
    u8 *Buf;
    s32 Ret = 0;

    if (!ChipName)
        return (-1);

    if(sp_tFlashDesrc)
        return (-4); // �豸��ע��

    sp_tFlashDesrc = malloc(sizeof(*sp_tFlashDesrc));
    if(!sp_tFlashDesrc)
        return (-1);

    // ��ȡFLASH��Ϣ
    if(Flash_Init(sp_tFlashDesrc))
    {
        TraceDrv(FLASH_TRACE_ERROR, "��������FLASH��Ϣʧ��\r\n");
        Ret = -3;
        goto FAILURE;
    }

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
    Chip->dwTotalPages            = FlashDescr.TotalPages;
    Chip->Type                    = F_ALIEN;
    Chip->Descr.Embd              = FlashDescr;
    Chip->Ops.ErsBlk              = Flash_SectorEarse;
    Chip->Ops.WrPage              = Flash_PageProgram;
    Chip->Ops.RdPage              = Flash_PageRead;
    Chip->Ops.PageToBlk           = Flash_PageToSector;
    strcpy(Chip->Name, ChipName); // �豸��
    if(Flags & FLASH_BUFFERED)
    {
        Buf = (u8*)malloc(sp_tFlashDesrc->BytesPerPage); // NAND�ײ㻺��
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
        if(sp_tFlashDesrc)
            free(sp_tFlashDesrc);
        if(FlashLock)
            Lock_MutexDelete(FlashLock);
        if(Buf)
            free(Buf);
        if(Chip)
            free(Chip);
    }
    return (Ret);
}
