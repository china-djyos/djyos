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
// Copyright (c) 2018 ����Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
#include <stddef.h>
#include <string.h>
#include <device.h>
#include <device/flash/flash.h>
#include <cpu_peri.h>
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
//component name:"cpu_peri_emflash"                 //Ƭ��flash��д
//parent:none                                       //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp���                                  //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                                        //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                                    //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                                   //��ʼ��ʱ������ѡֵ��early��medium��later��
                                                    //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"iap","int","devfile","heap","lock"    //������������������������none����ʾ�������������
                                                    //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                                    //����������������������г����á�,���ָ�
//weakdependence:"none"                             //��������������������������none����ʾ�������������
                                                    //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                                    //����������������������г����á�,���ָ�
//mutex:"none"                                      //������������������������none����ʾ�������������
                                                    //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef CFG_FLASH_FLAG   //****�������Ƿ��Ѿ����ú�
#warning    cpu_peri_emflash�������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,0,100,
#define CFG_FLASH_FLAG                   (2)        //"��Ǻ�"��
#define CFG_FLASH_RES                    (0)        //"����ҳ�������"
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_FLASH_CHIP_NAME              "embedded flash" //"FLASH����",
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

typedef struct __NxpIAP
{
    unsigned long cmd;           // Command
    unsigned long par[4];        // Parameters
    unsigned long stat;          // Status
    unsigned long res[2];        // Result
}tagIAP;

/* IAP Call */
typedef void (*IAP_Entry) (unsigned long *cmd, unsigned long *stat);
#define IAP_Call ((IAP_Entry) 0x1FFF1FF1)

#define CLCK 100000
static tagIAP sIAP;


extern u32 gc_ptIbootSize;
extern u32 gc_ptFlashOffset;
//-----------------------------------------------------------------------------
//����: ��ȡ����FLASH����Ϣ
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static s32 Flash_Init(struct EmbdFlashDescr *Description)
{
    sIAP.cmd = 54;
    sIAP.stat = 0;

    IAP_Call(&sIAP.cmd,&sIAP.stat);

    Description->BytesPerPage = 1024;
    Description->PagesPerSmallSect = 4;
    Description->PagesPerLargeSect = 0;
    Description->PagesPerNormalSect = 32;
    Description->SmallSectorsPerPlane = 16;
    Description->LargeSectorsPerPlane = 0;
    Description->NormalSectorsPerPlane = 14;
    Description->Planes = 1;
    Description->MappedStAddr = gc_ptFlashOffset;
    return (0);
}

//-----------------------------------------------------------------------------
//����: ��ȡ����FLASH����Ϣ
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------
//����: ������
//����: BlkNo -- ���;
//����: "0" -- �ɹ�;"-1" -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
static s32 Flash_SectorEarse(u32 SectorNo)
{
    sIAP.cmd    = 50;                             // Prepare Sector for Erase
    sIAP.par[0] = SectorNo;                         // Start Sector
    sIAP.par[1] = SectorNo;                         // End Sector
    IAP_Call (&sIAP.cmd, &sIAP.stat);             // Call IAP Command
    if (sIAP.stat)
        return (1);                               // Command Failed

    sIAP.cmd    = 52;                             // Erase Sector
    sIAP.par[0] = SectorNo;                         // Start Sector
    sIAP.par[1] = SectorNo;                         // End Sector
    sIAP.par[2] = CLCK;                           // CCLK in kHz
    IAP_Call (&sIAP.cmd, &sIAP.stat);             // Call IAP Command
    if (sIAP.stat)
        return (-1);                              // Command Failed
    return (0);
}
//-----------------------------------------------------------------------------
//����: дĳҳ
//����: PageNo -- ҳ��;
//      Data -- д����;
//      Flags -- д��������λ;
//����: ">0" -- ������д����ֽ���;
//      "-1" -- ��������;
//      "-2" -- дʧ��;
//��ע:
//-----------------------------------------------------------------------------
static s32 Flash_PageProgram(u32 Page, u8 *Data, u32 Flags)
{
    u32 Addr;
    u32 Sector;

    if(!Data)
        return (-1);

    Addr = Page * sp_tFlashDesrc->BytesPerPage + sp_tFlashDesrc->MappedStAddr;
    Sector = Addr >> 12;                          //  4kB Sector
    if (Sector >= 0x10)
    {
        Sector = 0x0E + (Sector >> 3);            // 32kB Sector
    }

    sIAP.cmd    = 50;                             // Prepare Sector for Write
    sIAP.par[0] = Sector;                         // Start Sector
    sIAP.par[1] = Sector;                         // End Sector
    IAP_Call (&sIAP.cmd, &sIAP.stat);             // Call IAP Command
    if (sIAP.stat)
        return (-2);                              // Command Failed

    Djy_EventDelay(2000);
    sIAP.cmd    = 51;                             // Copy RAM to Flash
    sIAP.par[0] = Addr;                           // Destination Flash Address
    sIAP.par[1] = (unsigned long)Data;            // Source RAM Address
    sIAP.par[2] = sp_tFlashDesrc->BytesPerPage;   // Fixed Page Size
    sIAP.par[3] = CLCK;                           // CCLK in kHz
    IAP_Call (&sIAP.cmd, &sIAP.stat);             // Call IAP Command
    if (sIAP.stat)
        return (1);                    // Command Failed


//  Djy_EventDelay(1000);

    return (sp_tFlashDesrc->BytesPerPage);                                  // Finished without Errors
}
//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static s32 Flash_PageRead(u32 Page, u8 *Data, u32 Flags)
{
    u32 Addr = Page * sp_tFlashDesrc->BytesPerPage + sp_tFlashDesrc->MappedStAddr;

    if(!Data)
        return (-1);

    memcpy(Data, (u8*)Addr, sp_tFlashDesrc->BytesPerPage);
    return (sp_tFlashDesrc->BytesPerPage);

}

//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static s32 Flash_PageToSector(u32 PageNo, u32 *Remains, u32 *SectorNo)
{
    u32 PagesLeft, PagesDone;
    u32 i;
    u32 Sector;

    if((!Remains) || (!SectorNo))
        return (-1);

    Sector = 0;
    PagesDone = 0;

    PagesLeft = sp_tFlashDesrc->PagesPerSmallSect -
               (PageNo % sp_tFlashDesrc->PagesPerSmallSect);
    for(i = 1; i <= sp_tFlashDesrc->SmallSectorsPerPlane; i++)
    {
        if(PageNo < (PagesDone + sp_tFlashDesrc->PagesPerSmallSect * i))
            goto DONE;
        Sector++;
    }

    PagesDone += sp_tFlashDesrc->SmallSectorsPerPlane *
                 sp_tFlashDesrc->PagesPerSmallSect;
//  PagesLeft = sp_tFlashDesrc->PagesPerLargeSect -
//                 (PageNo % sp_tFlashDesrc->PagesPerLargeSect);
//  for(i = 1; i <= sp_tFlashDesrc->LargeSectorsPerPlane; i++)
//  {
//      if(PageNo < (PagesDone + sp_tFlashDesrc->PagesPerLargeSect * i))
//          goto DONE;
//      Sector++;
//  }
//
//  PagesDone += sp_tFlashDesrc->LargeSectorsPerPlane *
//                   sp_tFlashDesrc->PagesPerLargeSect;
    PagesLeft = sp_tFlashDesrc->PagesPerNormalSect -
                   (PageNo % sp_tFlashDesrc->PagesPerNormalSect);
    for(i = 1; i <= sp_tFlashDesrc->NormalSectorsPerPlane; i++)
    {
        if(PageNo < (PagesDone + sp_tFlashDesrc->PagesPerNormalSect * i))
            goto DONE;
        Sector++;
    }

    return (-1);

DONE:
    *SectorNo = Sector; // sector�����
    *Remains = PagesLeft - 1; // page�����
    return (0);
}
//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
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
    if(Flash_GetDescr(sp_tFlashDesrc))
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
