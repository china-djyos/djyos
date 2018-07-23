//----------------------------------------------------
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
// �ļ���     ��
// ģ������:	Ƭ��flash����
// ģ��汾:
// ������Ա: HM
// ����ʱ��:
// =============================================================================

#include "stdint.h"
#include "stddef.h"
#include "os.h"
#include "cpu_peri.h"
#include "flashd.h"
#include "flashdrv.h"
#include "mpu.h"
#include "int.h"

#define CN_FLASH_START_ADDRESS   (0x00400000)
#define CN_FLASH_SIZE			 (0x200000)		//FLASH��СΪ2Mbytes
#define CN_APP_START_ADDRESS	 (0x00500000)
#define CN_APP_END_ADDRESS		 (CN_APP_START_ADDRESS + CN_FLASH_SIZE/2)

#define MPU_APP_FLASH_REGION	 (12)
#define MPU_APP_START_ADDRESS    CN_APP_START_ADDRESS
#define MPU_APP_END_ADDRESS      CN_APP_END_ADDRESS

// ============================================================================
// ���ܣ�ʹ�ܺͽ���APP�ε�cache���ܣ�����APPʱ���öε�ַ���ܿ�cache
// ������
// ���أ�
// ============================================================================
void Flash_CacheMpuEnable(u8 IsEnable)
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
// =============================================================================
// ����: д���ݣ�ֻ����дƬ�ں���512k�ֽ�
// �����������ֱ��ǵ�ַ��������ָ�룬�ֽ���
// ���أ�0��ʾ�޴��󣬷������
// =============================================================================
u32 Flash_Write(u32 addr, u8 *buf, u32 nbytes)
{
	u32 err;
	if( (addr < CN_APP_START_ADDRESS) ||
			(addr + nbytes > CN_APP_END_ADDRESS)
			|| (buf == NULL) )
	{
		return 0;
	}

	//дflash����Ϊ�豸���õ�ع��磬�����õ�ѹҪ����͵��ֽڱ��
	err = FLASHD_Write(addr,buf,nbytes);
	if(err == 0)
		return nbytes;
	return err;
}


// =============================================================================
// ����: д֮ǰ��Ҫ����оƬ
// ��������
// ���أ���
// =============================================================================
void Flash_Unlock(void)
{
	u32 start_addr = CN_APP_START_ADDRESS;
	u32 end_addr = CN_APP_START_ADDRESS + CN_FLASH_SIZE/2 - 1;

	if(FLASHD_IsLocked(start_addr,end_addr) != 0)
	{
		FLASHD_Unlock(start_addr,end_addr,NULL,NULL);
	}
}
// =============================================================================
// ����: д��֮����Ҫ��סоƬ
// ��������
// ���أ���
// =============================================================================
void Flash_Lock(void)
{
	u32 start_addr = CN_APP_START_ADDRESS;
	u32 end_addr = CN_APP_START_ADDRESS + CN_FLASH_SIZE/2 - 1;

	FLASHD_Lock(start_addr,end_addr,NULL,NULL);
}

// =============================================================================
// ����: �����ݣ�
// �����������ֱ��ǵ�ַ��������ָ�룬�ֽ���
// ���أ�ʵ�ʶ������ֽ���
// =============================================================================
u32 Flash_Read(u32 addr, u8 *buf, u32 nbytes)
{
	u32 loop;

	if( (addr < CN_APP_START_ADDRESS) ||
			(addr + nbytes > CN_APP_END_ADDRESS)
			|| (buf == NULL) )
	{
		return 0;
	}

	for(loop = 0; loop < nbytes; loop++)
	{
		buf[loop] = *((u8 *)(addr + loop));
	}

	return loop;
}

// =============================================================================
// ����:����Ƭ��flash��ǰ1024M�ֽ�
// ��������
// ���أ���
// =============================================================================
void Flash_Erase(void)
{
	u32 sector_addr;
	u32 start_addr,end_addr;

	u32 sector_length = 128*1024;		//ÿ��sector��128k
	start_addr = CN_APP_START_ADDRESS;
	end_addr = CN_APP_START_ADDRESS + CN_FLASH_SIZE/2;

	for(sector_addr = start_addr; sector_addr < end_addr; )
	{
		FLASHD_EraseSector(sector_addr);
		sector_addr += sector_length;
	}
}



