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
#include "stm32f4xx_flash.h"
#include "cpuflash.h"


// =============================================================================
// ����: д���ݣ�ֻ����дƬ�ں���512k�ֽ�
// �����������ֱ��ǵ�ַ��������ָ�룬�ֽ���
// ���أ�ʵ��д���ֽ���
// =============================================================================
u32 Flash_Write(u32 addr, u8 *buf, u32 nbytes)
{
	u32 loop,address;
	if( (addr < CN_APP_START_ADDRESS) ||
			(addr + nbytes > CN_APP_END_ADDRESS)
			|| (buf == NULL) )
	{
		return 0;
	}

	FLASH_Unlock();
	//дflash����Ϊ�豸���õ�ع��磬�����õ�ѹҪ����͵��ֽڱ��
	for(loop = 0; loop < nbytes; loop++)
	{
		address = addr + loop;
		FLASH_ProgramByte(address,buf[loop]);
	}
	FLASH_Lock();
	return loop;
}

// =============================================================================
// ����: �����ݣ�ֻ�����Ƭ�ں���512k�ֽ�
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
// ����:����Ƭ��flash�ĺ���512k�ֽڵ�����
// ��������
// ���أ���
// =============================================================================
void Flash_Erase(void)
{
	FLASH_Unlock();
	//дflash����Ϊ�豸���õ�ع��磬�����õ�ѹҪ����͵��ֽڱ��
	FLASH_EraseSector(FLASH_Sector_8,VoltageRange_1);
	FLASH_EraseSector(FLASH_Sector_9,VoltageRange_1);
	FLASH_EraseSector(FLASH_Sector_10,VoltageRange_1);
	FLASH_EraseSector(FLASH_Sector_11,VoltageRange_1);
	FLASH_Lock();
}


void Flash_Flush(void)
{
	FLASH_InstructionCacheCmd(DISABLE);
	FLASH_DataCacheCmd(DISABLE);

	FLASH_InstructionCacheReset();
	FLASH_DataCacheReset();

	FLASH_InstructionCacheCmd(ENABLE);
	FLASH_DataCacheCmd(ENABLE);
}

bool_t CorrectFlashWrite(u32 addr,u8 *wbuf,u8 len)
{
   bool_t result=true;
   u8 rbuf[180];
   memset(rbuf,0x00,180);
   Flash_Read(addr,rbuf,len);
   if(0!= memcmp(wbuf,rbuf,len))
   {
	 // printf("Flash write error.\r\n");
	  result=false;
	  return result;
   }
   return result;

}

