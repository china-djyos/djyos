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
// Copyright (c) 2014 ����Ȩ�����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------

#include <stdlib.h>
#include <string.h>
#include <driver.h>
#include <driver/flash/flash.h>
#include <flashd.h>
#include <cpu_peri.h>
#include <mpu.h>
#include <int.h>

static struct EmbdFlashDescr{
	u16     BytesPerPage;      		     // ҳ�а������ֽ���
	u16     PagesPerSmallSect;      	 // small sector��ҳ��
	u16 	PagesPerLargeSect;           // large sector��ҳ��
	u16     PagesPerNormalSect;          // Normal sector��ҳ��
	u16     SmallSectorsPerPlane;        // ÿplane��small sector��
	u16     LargeSectorsPerPlane;        // ÿplane��large sector��
	u16     NormalSectorsPerPlane;       // ÿplane��normal sector��
	u16 	Planes;                      // plane��
	u32     MappedStAddr;
}*sp_tEmFlashDesrc;

#define MPU_APP_FLASH_REGION	 (12)
#define MPU_APP_START_ADDRESS    (0x00400000) // ����FLASH��ʼ��ַ
#define MPU_APP_END_ADDRESS      (0x00400000 + 0x200000) // 2MB


extern u32 gc_ptIbootSize;
//-----------------------------------------------------------------------------
//����: ι��
//����:
//����:
//��ע: �������ļ�max706.cʵ��
//-----------------------------------------------------------------------------
extern bool_t BrdWdt_FeedDog(void) __attribute__((weak));
bool_t BrdWdt_FeedDog(void)
{
	return (TRUE); // �պ���
}
//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע: todo ������Щ����
//-----------------------------------------------------------------------------
void __CacheMpuEnable(u8 IsEnable)
{
	uint32_t dwRegionBaseAddr;
	uint32_t dwRegionAttr;

	Int_CutTrunk();

	/* Clean up data and instruction buffer */
	__DSB();
	__ISB();

	if(!IsEnable)
	{
		dwRegionBaseAddr =
			MPU_APP_START_ADDRESS |
			MPU_REGION_VALID |
			MPU_APP_FLASH_REGION;      //12

		//����������ʱ���ɶ�flash 1M��λ�ö�д
		dwRegionAttr =
			MPU_AP_FULL_ACCESS |
			INNER_OUTER_NORMAL_NOCACHE_TYPE( NON_SHAREABLE ) |
			MPU_CalMPURegionSize(IFLASH_END_ADDRESS - IFLASH_START_ADDRESS) |
			MPU_REGION_ENABLE;

		MPU_SetRegion(dwRegionBaseAddr,dwRegionAttr);
	}
	else
	{
		dwRegionBaseAddr =
				MPU_APP_START_ADDRESS |
				MPU_REGION_VALID |
				MPU_APP_FLASH_REGION;      //12

		dwRegionAttr =
			MPU_AP_READONLY |
			INNER_NORMAL_WB_NWA_TYPE( NON_SHAREABLE ) |
			MPU_CalMPURegionSize(IFLASH_END_ADDRESS - IFLASH_START_ADDRESS) |
			MPU_REGION_ENABLE;

		MPU_UpdateRegions(MPU_APP_FLASH_REGION, dwRegionBaseAddr, dwRegionAttr);
	}

	/* Clean up data and instruction buffer to make the new region taking
	   effect at once */
	__DSB();
	__ISB();

	Int_ContactTrunk();
}

//-----------------------------------------------------------------------------
//����: ��ȡ����FLASH����Ϣ
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static s32 EEFC_Init(struct EmbdFlashDescr *Description)
{
	Description->BytesPerPage = 512;
	Description->PagesPerSmallSect = 16;
	Description->PagesPerLargeSect = 224;
	Description->PagesPerNormalSect = 256;
	Description->SmallSectorsPerPlane = 2;
	Description->LargeSectorsPerPlane = 1;
	Description->NormalSectorsPerPlane = 15;
	Description->Planes = 1;
	Description->MappedStAddr = 0x400000;
	return (0);
}

//-----------------------------------------------------------------------------
//����: ��ȡ����FLASH����Ϣ
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static s32 EEFC_GetDescr(struct EmFlashDescr *Description)
{
	Description->BytesPerPage = sp_tEmFlashDesrc->BytesPerPage;
	Description->TotalPages = (sp_tEmFlashDesrc->PagesPerSmallSect *
							   sp_tEmFlashDesrc->SmallSectorsPerPlane +
							   sp_tEmFlashDesrc->PagesPerLargeSect *
							   sp_tEmFlashDesrc->LargeSectorsPerPlane +
							   sp_tEmFlashDesrc->PagesPerNormalSect *
							   sp_tEmFlashDesrc->NormalSectorsPerPlane) *
							  sp_tEmFlashDesrc->Planes;
	Description->ReservedPages = gc_ptIbootSize / sp_tEmFlashDesrc->BytesPerPage;
	Description->MappedStAddr = sp_tEmFlashDesrc->MappedStAddr;
	return (0);
}
//-----------------------------------------------------------------------------
//����: ������
//����: BlkNo -- ���;
//����: "0" -- �ɹ�;"-1" -- ʧ��;
//��ע: sector0��normal sector��Сʱһ���ģ��������ֿ�����ֻ��Ϊ�˱����Ķ�����
//-----------------------------------------------------------------------------
 s32 EEFC_SectorEarse(u32 SectorNo)
{
	s32 Res;
	u32 Addr;
	u32 End;
	u32 SizeofSect0 = sp_tEmFlashDesrc->BytesPerPage *
					  (sp_tEmFlashDesrc->PagesPerSmallSect *
					   sp_tEmFlashDesrc->SmallSectorsPerPlane +
					   sp_tEmFlashDesrc->PagesPerLargeSect *
					   sp_tEmFlashDesrc->LargeSectorsPerPlane); // ��0��ĵ�ַ�ռ�
	u32 SizeofNormalSect = sp_tEmFlashDesrc->BytesPerPage *
			               sp_tEmFlashDesrc->PagesPerNormalSect;

	if(!SectorNo) // sector 0
	{
		Addr = 0;
		End = Addr + SizeofSect0;
	}
	else
	{
		Addr =  SizeofSect0 + SizeofNormalSect * (SectorNo - 1);
		End = Addr + SizeofNormalSect;
	}

	Addr += sp_tEmFlashDesrc->MappedStAddr;
	End += sp_tEmFlashDesrc->MappedStAddr;

	__CacheMpuEnable(0);
	if(FLASHD_IsLocked(Addr, End))
		FLASHD_Unlock(Addr, End, NULL, NULL);
	BrdWdt_FeedDog(); // ι��
	Res = FLASHD_EraseSector(Addr);
	BrdWdt_FeedDog(); // ι��
	if(Res)
		Res = -1;

	__CacheMpuEnable(1);

	return (Res);
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
s32 EEFC_PageProgram(u32 Page, u8 *Data, u32 Flags)
{
	u32 Ret;

	u32 Addr = Page * sp_tEmFlashDesrc->BytesPerPage + sp_tEmFlashDesrc->MappedStAddr;

	if(!Data)
		return (-1);

	__CacheMpuEnable(0);
	if(FLASHD_IsLocked(Addr, Addr+sp_tEmFlashDesrc->BytesPerPage))
		FLASHD_Unlock(Addr, Addr+sp_tEmFlashDesrc->BytesPerPage, NULL, NULL);

	BrdWdt_FeedDog(); // ι��
	Ret = FLASHD_Write(Addr, Data, sp_tEmFlashDesrc->BytesPerPage);
	BrdWdt_FeedDog(); // ι��
	if(Ret)
		return (-2);

	__CacheMpuEnable(1);
	return (sp_tEmFlashDesrc->BytesPerPage);
}
//-----------------------------------------------------------------------------
//����: ��ĳҳ
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
s32 EEFC_PageRead(u32 Page, u8 *Data, u32 Flags)
{
	u32 Addr = Page * sp_tEmFlashDesrc->BytesPerPage + sp_tEmFlashDesrc->MappedStAddr;

	if(!Data)
		return (-1);

	memcpy(Data, (u8*)Addr, sp_tEmFlashDesrc->BytesPerPage);
	return (sp_tEmFlashDesrc->BytesPerPage);

}

//-----------------------------------------------------------------------------
//����: ����page����sector
//����: PageNo -- ҳ��
//     Remains -- ʣ��ҳ��
//     SectorNo -- ҳ����sector
//����:
//��ע: sector0��normal sector��Сʱһ���ģ��������ֿ�����ֻ��Ϊ�˱����Ķ�����
//-----------------------------------------------------------------------------
s32 EEFC_PageToSector(u32 PageNo, u32 *Remains, u32 *SectorNo)
{
	u32 PagesLeft, PagesDone;
	u32 i;
	u32 Sector;
	u32 PagesSector0;

	if((!Remains) || (!SectorNo))
		return (-1);

	Sector = 0;
	PagesDone = 0;
	PagesSector0 = ((sp_tEmFlashDesrc->PagesPerSmallSect *
				     sp_tEmFlashDesrc->SmallSectorsPerPlane) +
			        (sp_tEmFlashDesrc->PagesPerLargeSect *
			         sp_tEmFlashDesrc->LargeSectorsPerPlane));
	PagesLeft = PagesSector0 - (PageNo % PagesSector0);
	if(PageNo < (PagesDone + PagesSector0))
		goto DONE;
	Sector++;

	PagesDone += PagesSector0;
	PagesLeft = sp_tEmFlashDesrc->PagesPerNormalSect -
				((PageNo - PagesDone) % sp_tEmFlashDesrc->PagesPerNormalSect);
	for(i = 1; i <= sp_tEmFlashDesrc->NormalSectorsPerPlane; i++)
	{
		if(PageNo < (PagesDone + sp_tEmFlashDesrc->PagesPerNormalSect * i))
			goto DONE;
		Sector++;
	}

	return (-1);

DONE:
	*SectorNo = Sector; // ��sector���
	*Remains = PagesLeft; //
	return (0);
}
//-----------------------------------------------------------------------------
//����:
//����: ResPages������ҳ��
//����:
//��ע:
//-----------------------------------------------------------------------------
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

	if(sp_tEmFlashDesrc)
		return (-4); // �豸��ע��

	sp_tEmFlashDesrc = malloc(sizeof(*sp_tEmFlashDesrc));
	if(!sp_tEmFlashDesrc)
		return (-1);

    // ��ȡFLASH��Ϣ
    if(EEFC_Init(sp_tEmFlashDesrc))
    {
        TraceDrv(FLASH_TRACE_ERROR, "��������FLASH��Ϣʧ��\r\n");
		Ret = -3;
		goto FAILURE;
    }

    EEFC_GetDescr(&FlashDescr);
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
	Chip->Type                    = F_ALIEN;
	Chip->Descr.Embd              = FlashDescr;
	Chip->Ops.ErsBlk              = EEFC_SectorEarse;
	Chip->Ops.WrPage              = EEFC_PageProgram;
	Chip->Ops.RdPage              = EEFC_PageRead;
	Chip->Ops.PageToBlk			  = EEFC_PageToSector;
	strcpy(Chip->Name, ChipName); // �豸��
	if(Flags & FLASH_BUFFERED)
	{
		Buf = (u8*)malloc(sp_tEmFlashDesrc->BytesPerPage);
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

    Driver_DeviceCreate(NULL, Chip->Name, NULL, NULL, NULL, NULL, NULL, NULL, (ptu32_t)Chip); // �豸����"/dev"
    if(Flags & FLASH_ERASE_ALL)
		EarseWholeChip(Chip);

FAILURE:
	if(Ret)
	{
		if(sp_tEmFlashDesrc)
			free(sp_tEmFlashDesrc);
		if(FlashLock)
			Lock_MutexDelete(FlashLock);
		if(Buf)
			free(Buf);
		if(Chip)
			free(Chip);
	}
	return (Ret);
}