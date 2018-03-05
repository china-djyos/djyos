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

#include <stdlib.h>
#include <driver.h>
#include <driver/flash/flash.h>
#include <cpu_peri.h>
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
} *sp_tFlashDesrc;

void Cache_Invalidate(void)
{

//  if (cache == CODE_CACHE)
//  {
      /* Cache Set Command: set command bits in CCR */
      /* set invalidate way 1 and invalidate way 0 bits */
      LMEM->PCCCR = LMEM_PCCCR_INVW0_MASK | LMEM_PCCCR_INVW1_MASK;

      /* set ccr[go] bit to initiate command to invalidate cache */
      LMEM->PCCCR |= LMEM_PCCCR_GO_MASK;

      /* wait until the ccr[go] bit clears to indicate command complete */
      while(((LMEM->PCCCR & LMEM_PCCCR_GO_MASK)>>LMEM_PCCCR_GO_SHIFT)== 0x1)
      {};

//  }else if (cache == SYS_CACHE)
//  {
      /* Cache Set Command: set command bits in CCR */
      /* set invalidate way 1 and invalidate way 0 bits */
      LMEM->PSCCR = LMEM_PSCCR_INVW0_MASK | LMEM_PSCCR_INVW1_MASK;

      /* set ccr[go] bit to initiate command to invalidate cache */
      LMEM->PSCCR |= LMEM_PSCCR_GO_MASK;

      /* wait until the ccr[go] bit clears to indicate command complete */
      while(((LMEM->PSCCR & LMEM_PSCCR_GO_MASK)>>LMEM_PSCCR_GO_SHIFT)== 0x1)
      {};

//  } else
//  {
//     return CACHE_INVALID_PARAM;
//  }

//  return CACHE_OK;

}
static void Kinetis_FlashInit(void)
{
//	Cache_Disable();
	//���Flash���ʴ���
	if (FTFE->FSTAT & FTFE_FSTAT_ACCERR_MASK)
	{
	FTFE->FSTAT |= FTFE_FSTAT_ACCERR_MASK;//��������־
	}
	//��鱣������
	else if (FTFE->FSTAT & FTFE_FSTAT_FPVIOL_MASK)
	{
	FTFE->FSTAT |= FTFE_FSTAT_FPVIOL_MASK;
	}
	else if (FTFE->FSTAT & FTFE_FSTAT_RDCOLERR_MASK)
	{
	//������ͻ����
	FTFE->FSTAT |= FTFE_FSTAT_RDCOLERR_MASK;
	} /* EndIf */

	//����Flashģ������ݻ���
	FMC->PFB01CR &= ~FMC_PFB01CR_B01DCE_MASK;
	FMC->PFB23CR &= ~FMC_PFB23CR_B23DCE_MASK;

	while(!(FTFE->FSTAT & FTFE_FSTAT_CCIF_MASK));    // �ȴ��������
}
static s32 Kinetis_CmdLaunch(void)
{
    // ������ʴ����־λ�ͷǷ����ʱ�־λ
	FTFE->FSTAT = (1<<5) | (1<<4);

    // ��������
    FTFE->FSTAT = FTFE_FSTAT_CCIF_MASK;

    // �ȴ��������
    while(!(FTFE->FSTAT & FTFE_FSTAT_CCIF_MASK));

    u32 temp;
    temp = FTFE->FSTAT;
    // �������־
    if(temp & ((1<<5) | (1<<4) | 1))
        return 1 ; //ִ���������

    return 0; //ִ������ɹ�
}
static s32 Kinetis_EraseSector(u16 sectorNo)
{
    union
    {
        u32  word;
        u8   byte[4];
    } dest;

    dest.word    = (u32)(sectorNo*(1<<11));

    FTFE->FCCOB0 = 0x09; //��������

    // ����Ŀ���ַ
    FTFE->FCCOB1 = dest.byte[2];
    FTFE->FCCOB2 = dest.byte[1];
    FTFE->FCCOB3 = dest.byte[0];

    // ִ����������
    if(1 == Kinetis_CmdLaunch())    //��ִ��������ִ���
        return 1;     //�����������

    // ������sector0ʱ��������豸
    if(dest.word <= 0x800)
    {

        FTFE->FCCOB0 = 0x06; // д��4�ֽ�
        // ����Ŀ���ַ
        FTFE->FCCOB1 = 0x00;
        FTFE->FCCOB2 = 0x04;
        FTFE->FCCOB3 = 0x0C;
        // ����
        FTFE->FCCOB4 = 0xFF;
        FTFE->FCCOB5 = 0xFF;
        FTFE->FCCOB6 = 0xFF;
        FTFE->FCCOB7 = 0xFE;
        // ִ����������
        if(1 == Kinetis_CmdLaunch())  //��ִ��������ִ���
            return 2;   //�����������
    }

    return 0;  //�ɹ�����
}

static s32 Kinetis_PageWrite(u16 sectNo,u8 *buf)
{
    u32 size;
    u32 offset;
    u8 *Data;

    union
    {
        u32   word;
        u8  byte[4];
    } dest;

    // ����д������
    FTFE->FCCOB0 = 0x07;
    size = sp_tFlashDesrc->BytesPerPage;
    offset = 0;
    Data = buf;
    while(size)
    {
    	dest.word = (u32)(sectNo*(1<<11) + offset);//�����ַ
        // ����Ŀ���ַ
        FTFE->FCCOB1 = dest.byte[2];
        FTFE->FCCOB2 = dest.byte[1];
        FTFE->FCCOB3 = dest.byte[0];

        // ��������
        FTFE->FCCOB4 = Data[3];
        FTFE->FCCOB5 = Data[2];
        FTFE->FCCOB6 = Data[1];
        FTFE->FCCOB7 = Data[0];
        FTFE->FCCOB8 = Data[7];
        FTFE->FCCOB9 = Data[6];
        FTFE->FCCOBA = Data[5];
        FTFE->FCCOBB = Data[4];

        if(1 == Kinetis_CmdLaunch())
            break;  //д���������
        Cache_Invalidate();
        offset += 8;
        size   -= 8;
        Data   += 8;
    }

    return offset;  //�ɹ�ִ��
}

extern u32 gc_ptIbootSize;
extern u32 gc_ptFlashOffset;
extern u32 gc_ptFlashRange;
//-----------------------------------------------------------------------------
//����: ����FLASH��ʼ��
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static s32 Flash_Init(struct EmbdFlashDescr *Description)
{
	Kinetis_FlashInit();

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
	s32 Ret;

	Ret = Kinetis_EraseSector(SectorNo);

	if(Ret != 0)
		Ret = -1;
	return (Ret);
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
	u32 Ret;

	if(!Data)
		return (-1);

	Ret = Kinetis_PageWrite(Page,Data);
	return Ret;
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
//����: ����page����sector
//����: PageNo -- ҳ��
//     Remains -- ʣ��ҳ��
//     SectorNo -- ҳ����sector
//����:
//��ע: sector0��normal sector��Сʱһ���ģ��������ֿ�����ֻ��Ϊ�˱����Ķ����
//-----------------------------------------------------------------------------
s32 Flash_PageToSector(u32 PageNo, u32 *Remains, u32 *SectorNo)
{
	if((!Remains) || (!SectorNo))
		return (-1);

	*Remains  = 1;
	*SectorNo = PageNo;

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
	Chip->dwTotalPages		      = FlashDescr.TotalPages;
	Chip->Type                    = F_ALIEN;
	Chip->Descr.Embd              = FlashDescr;
	Chip->Ops.ErsBlk              = Flash_SectorEarse;
	Chip->Ops.WrPage              = Flash_PageProgram;
	Chip->Ops.RdPage              = Flash_PageRead;
	Chip->Ops.PageToBlk			  = Flash_PageToSector;
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

    Driver_DeviceCreate(NULL, Chip->Name, NULL, NULL, NULL, NULL, NULL, NULL, (ptu32_t)Chip); // �豸����"/dev"
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
