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

/*#include "norflash.h"*/
#include "silan_m0_cache.h"
#include "silan_irq.h"
#include "int.h"
#include "string.h"
#include <stdlib.h>
#include "flash.h"

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


extern int32_t EraseSomeBlocks(uint32_t addr, uint32_t size);
extern int32_t ProgramOnePackage(char *data, uint32_t addr, uint32_t size);

#define PAGE_SIZE          (0x100)      /* 256 byte */
#define SECTOR_SIZE        (0x1000)      /* 4 Kbyte */
#define BLOCK_SIZE         (0x10000)     /* 8 Bbyte*/
#define FLASH_SIZE         (0x200000)    /* 2 MBytes */

extern u32 Lock_Cache_Add_Start;
extern u32 Lock_Cache_Add_End;

static struct EmbdFlashDescr *sp_tFlashDesrc;


#define MPU_APP_FLASH_REGION     (12)
#define MPU_APP_START_ADDRESS    (0x00400000)            // ����FLASH��ʼ��ַ
#define MPU_APP_END_ADDRESS      (0x00400000 + 0x200000) // 2MB

extern u32 gc_ptIbootSize;
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
    Description->BytesPerPage = PAGE_SIZE;
    Description->PagesPerSmallSect = 0;
    Description->PagesPerLargeSect = 0;
    Description->PagesPerNormalSect = 1;
    Description->SmallSectorsPerPlane = 0;
    Description->LargeSectorsPerPlane = 0;
    Description->NormalSectorsPerPlane = gc_ptFlashRange/PAGE_SIZE;
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
    Description->TotalPages = ( sp_tFlashDesrc->PagesPerSmallSect *
                                sp_tFlashDesrc->SmallSectorsPerPlane +
                                sp_tFlashDesrc->PagesPerLargeSect *
                                sp_tFlashDesrc->LargeSectorsPerPlane +
                                sp_tFlashDesrc->PagesPerNormalSect *
                                sp_tFlashDesrc->NormalSectorsPerPlane) *
                                sp_tFlashDesrc->Planes;
    Description->ReservedPages = gc_ptIbootSize / sp_tFlashDesrc->BytesPerPage;
    Description->MappedStAddr  = sp_tFlashDesrc->MappedStAddr;
    return (0);
}


// ============================================================================
// ���ܣ�����flash ����ģʽ(����ģʽ����д������)
// ������
// ���أ�
// ��ע��
// ============================================================================

static u32 restored;

static void enterFlashOperaterMode(void)
{
    u32 optStartAddr, Remain, optAddrLen;

    restored = pic_get_enable();
    pic_clr_enable(restored);

    silan_m0_cache_disable();
    silan_m0_cache_clear();

    optStartAddr =    ((u32) &Lock_Cache_Add_Start) & 0xFFFFFFF0;
    Remain       =    ((u32) &Lock_Cache_Add_Start) & 0x0000000F;
    optAddrLen   =    ((((u32) &Lock_Cache_Add_End) - ((u32) &Lock_Cache_Add_Start)) + Remain + M0_CACHE_LINE_SIZE - 1) / M0_CACHE_LINE_SIZE;

    silan_m0_cache_lock(optStartAddr, optAddrLen);

    silan_m0_cache_enable();

    iap_norflash_init();
}


// ============================================================================
// ���ܣ��˳� flash ����ģʽ
// ������
// ���أ�
// ��ע��
// ============================================================================

static void exitFlashOperaterMode(void)
{

    u32 optStartAddr, Remain, optAddrLen;

    optStartAddr =    ((u32) &Lock_Cache_Add_Start) & 0xFFFFFFF0;
    Remain       =    ((u32) &Lock_Cache_Add_Start) & 0x0000000F;
    optAddrLen   =    ((((u32) &Lock_Cache_Add_End) - ((u32) &Lock_Cache_Add_Start)) + Remain + M0_CACHE_LINE_SIZE - 1) / M0_CACHE_LINE_SIZE;

    silan_m0_cache_unlock(optStartAddr, optAddrLen, 0);

    pic_set_enable(restored);

}


// ============================================================================
// ���ܣ���������
// ������SectorNo -- ������;
// ���أ�"0" -- �ɹ�;"-1" -- ʧ��;
// ��ע��
// ============================================================================
static s32 Flash_SectorEarse(u32 SectorNo)
{
    u32 Addr;
    u8 retry = 0; // �����п��ܻ�ʧ�ܣ�
    s32 Ret = 0;
    u32 SECTORError=0;//�������������Ϣ

    atom_high_t high_atom;                      //ԭ�Ӳ���
    high_atom =Int_HighAtomStart();

    enterFlashOperaterMode();
    Addr = SectorNo * sp_tFlashDesrc->BytesPerPage + sp_tFlashDesrc->MappedStAddr;
    EraseSomeSectors(Addr,SECTOR_SIZE);
    exitFlashOperaterMode();

    Int_HighAtomEnd(high_atom);

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
    u32 datLen;
    u32 DatAddr = Page * sp_tFlashDesrc->BytesPerPage + sp_tFlashDesrc->MappedStAddr;
    u32 DatToWrite = 0;

    atom_high_t high_atom;                      //ԭ�Ӳ���
    high_atom =Int_HighAtomStart();

    enterFlashOperaterMode();
    datLen       = sp_tFlashDesrc->BytesPerPage;
    DatToWrite = ProgramOnePackage((u8*)Data, DatAddr, datLen);

    exitFlashOperaterMode();

    Int_HighAtomEnd(high_atom);
    return DatToWrite;
}


// ============================================================================
// ���ܣ���ĳҳ
// ������
// ���أ�
// ��ע��
// ============================================================================
s32 Flash_PageRead(u32 PageNo, u8 *Data, u32 Flags)
{
    u8  *ptReadAddr;
    u32 readDatNo;
    u32 loop;

    ptReadAddr   = (u8*)(PageNo * sp_tFlashDesrc->BytesPerPage + sp_tFlashDesrc->MappedStAddr);
    readDatNo    = sp_tFlashDesrc->BytesPerPage;

    if(Data)
    {
        for(loop = 0;loop < readDatNo;loop++)
        {
            Data[loop] = ptReadAddr[loop];
        }

    }

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
    s32 ret;
    u16 secNum;
    u16 remainPage;

    //��ҳ���ڵڼ�����
    secNum      = PageNo * sp_tFlashDesrc->BytesPerPage / SECTOR_SIZE;
    remainPage  = PageNo * sp_tFlashDesrc->BytesPerPage % SECTOR_SIZE /PAGE_SIZE;
    SectorNo[0] = secNum;
    Remains [0] = remainPage;

    return secNum;
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

    //��ʼ��FLASH��Ϣ
    if(Flash_Init(sp_tFlashDesrc))
    {
        error_printf("null","��������FLASH��Ϣʧ��\r\n");
        Ret = -3;
        goto __FAILURE;
    }

    Flash_GetDescr(&FlashDescr);// ��ȡFLASH��Ϣ
    if(ResPages > FlashDescr.TotalPages)
    {
        Ret = -1;
        goto __FAILURE;
    }

    FlashDescr.ReservedPages += ResPages;
    Len = strlen (ChipName) + 1;
    Chip = (struct FlashChip*) malloc(sizeof(struct FlashChip) + Len);
    if (NULL == Chip)
    {
//        TraceDrv(FLASH_TRACE_ERROR, "out of memory!\r\n");
        error_printf("null","out of memory!\r\n");
        Ret = -2;
        goto __FAILURE;
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

    if(Flags & FLASH_BUFFERED)
    {
        Buf = (u8*)malloc(sp_tFlashDesrc->BytesPerPage);
        if(!Buf)
        {
//            TraceDrv(FLASH_TRACE_ERROR, "out of memory!\r\n");
            error_printf("null","out of memory!\r\n");
            Ret = -2;
            goto __FAILURE;
        }

        FlashLock = Lock_MutexCreate("Embedded Flash Lock");
        if(!FlashLock)
        {
            Ret = -3;
            goto __FAILURE;
        }

        Chip->Buf = Buf;
        Chip->Lock =(void*)FlashLock;
    }

    if(-1 == dev_add(NULL, Chip->Name, NULL, NULL, NULL, NULL, NULL, (ptu32_t)Chip)) // �豸����"/dev"
    {
        info_printf("null","device","add embedded flash falied.");
        Ret = -3;
        goto __FAILURE;
    }

    if(Flags & FLASH_ERASE_ALL)
        EarseWholeChip(Chip);

    __FAILURE:
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






























































































































































































































































































































































