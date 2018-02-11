
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
// �����Ȩ�����ʹ���߷������¶������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�����������У���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�����������У��Լ�����
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
// =============================================================================
//
//
// =============================================================================

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "cpu_peri.h"
#include <at24c128b.h>
#include <at45db321.h>


#define CN_UPDATE_BUF_LEN		1024				//��������С
#define CN_UPDATE_RETRY_TIMES	4					//��AT45���ݴ����
#define CN_UPDETE_FLAG			(0xA5A5)			//������־
#define CN_FLASH_BASE_ADDR      (0x08000000)		//оƬFLASH��ʼ��ַ
#define CN_FLASH_PAGE_SIZE      (2048)				//оƬFLASHҳ��С

extern bool_t  Sh_RunAPP(char *param);

typedef struct
{
	u32 CodeSize;				//APP�����С
	u32 CodeCrc;				//APP����CRC���ۼӺ�
	u32 Res0[2];
	u32 CodeStartAddr;			//app������ʼ��ַ
	u32 CodeMaxSize;			//app������󳤶�
	u32 Res1;
	u32 UpdateFlag; 			//������־
	u32 InfoCrc;				//�洢�ṹCRC
}tagUpdateInfo;

typedef struct
{
	u32 InfoStartAddr;		//��Ϣ�洢��ַ
	u32 InfoLen;			//��Ϣ����
	u32 CodeRdStartAddr;	//Ӧ�ó���洢
	u32 CodeRdLen;			//Ӧ�ó���ĳ���
	u32 CodeRdCrc;			//�洢��Ӧ��CRC
	u32 CalCrc;				//���ؼ����CRC

	u32 CodeWrStartAddr;	//APP��Flashд����ʼ��ַ
	u32 CodeWrMaxLen;		//APP���������
	u8 Buf[CN_UPDATE_BUF_LEN];
}tagAppInfo;

static tagAppInfo gAppInfo;

extern u32 gc_ptIbootSize;				//iboot.bin��ռ��С
extern u32 gc_ptFlashRange;				//�ܹ�flash�Ĵ�С

// =============================================================================
// ���ܣ���AT45�ж�����
// ������
// ���أ������ֽ���
// =============================================================================
static u32 AppCode_Read(u32 addr,u8 *buf,u32 len)
{
	return AT45_FLASH_Read(addr,buf,len);
}

// =============================================================================
// ���ܣ�����Ҫд���������flash��Flash��Ϊ����bank��ÿ��bank���е�������page
// ������PageStart,������ʼҳ������ҳ���㣩
//      Pages��������ҳ��
// ���أ�0,��ȷ���������
// =============================================================================
static s32 AppCode_Erase(u32 StartAddr,u32 len)
{
	s32 Ret;
	u32 SECTORError=0;//�������������Ϣ
	u8 PageStart,Pages;
	u8 Page1Start,pages1;
	u8 Page2Start,pages2;
	static FLASH_EraseInitTypeDef EraseInitStruct;

	PageStart = (StartAddr - CN_FLASH_BASE_ADDR)/CN_FLASH_PAGE_SIZE;
	Pages = len / CN_FLASH_PAGE_SIZE;
	if(PageStart < gc_ptFlashRange/4096)
	{
		Page1Start = PageStart;
		if(PageStart + Pages > gc_ptFlashRange/4096)
		{
			pages1 = gc_ptFlashRange/4096 - PageStart;
			pages2 = Pages - pages1;
			Page2Start = 0;
		}
		else
		{
			pages1 = Pages;
			pages2 = 0;
		}
	}
	else
	{
		pages1 = 0;
		pages2 = Pages;
		Page2Start = 0;
	}

	HAL_FLASH_Unlock();

	if(pages1)
	{
		EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
		EraseInitStruct.Banks       = FLASH_BANK_1;
		EraseInitStruct.Page        = Page1Start;
		EraseInitStruct.NbPages     = pages1;

		Wdt_FeedDog();
		if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
			Ret=-1;
		else
			Ret=0;
	}

	if(pages2)
	{
		EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
		EraseInitStruct.Banks       = FLASH_BANK_2;
		EraseInitStruct.Page        = Page2Start;
		EraseInitStruct.NbPages     = pages2;

		Wdt_FeedDog();
		if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
			Ret=-1;
		else
			Ret=0;
	}

	HAL_FLASH_Lock();

	return Ret;
}

// =============================================================================
// ���ܣ���Code����д�뵽FLASHָ���ĵ�ַ��ֱ�ӵ����˹ٷ��⺯��(Ҫ��Addr��len��8�ֽڶ���)
// ������Addr,��ַ��buf����������len���ֽ�����
// ���أ�д����ֽ���
// =============================================================================
static u32 AppCode_Write(u32 Addr,u8 *buf,u32 len)
{
	u32 Ret,i;
	u64 *pData = (u64*)buf;

	if(!buf)
		return (-1);

	HAL_FLASH_Unlock();

	for(i = 0; i < len;)
	{
    	if(*(u64*)Addr != *pData)
    	{
    		Ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,Addr,*pData);
    		if(Ret != HAL_OK)
    			break;
    	}
		pData++;
		i += 8;
		Addr += 8;
	}

	HAL_FLASH_Lock();
	return i;
}
// =============================================================================
// ���ܣ��ۼӺͼ��㲢���ؼ���ֵ
// ������buf,len
// ���أ�������
// =============================================================================
static u16 Crc_Calculate(u8* buf,u32 len)
{
	u32 i;
	u16 temp;

	temp = 0;
	for(i = 0; i < len; i ++)
	{
		temp += buf[i];
	}
	return temp;
}

// =============================================================================
// ���ܣ���AT24�ж�APP��Ϣ��������Ϣ�����ۼӺ�У��
// ������Infoָ��
// ���أ�true,��ȷ���������
// =============================================================================
static bool_t AppInfo_Read(tagAppInfo *Info)
{
	bool_t Ret = false;
	tagUpdateInfo *CodeInfo;
	u32 CalInfoCrc;

	if ( true == AT24_ReadBytes(Info->InfoStartAddr,\
			Info->Buf,Info->InfoLen))
	{
		CodeInfo = (tagUpdateInfo *)Info->Buf;

		CalInfoCrc = CodeInfo->CodeCrc + CodeInfo->CodeMaxSize + \
				CodeInfo->CodeSize + CodeInfo->CodeStartAddr + \
				CodeInfo->Res0[0] + CodeInfo->Res0[1] + \
				CodeInfo->Res1 + CodeInfo->UpdateFlag;

		printf("APP Update Info: Info.CodeSize = %d \r\n",CodeInfo->CodeSize);
		printf("APP Update Info: Info.CodeCrc = %d \r\n",CodeInfo->CodeCrc);
		printf("APP Update Info: Info.CodeStartAddr = %d \r\n",CodeInfo->CodeStartAddr);
		printf("APP Update Info: Info.CodeMaxSize = %d \r\n",CodeInfo->CodeMaxSize);
		printf("APP Update Info: Info.UpdateFlag = %d \r\n",CodeInfo->UpdateFlag);
		printf("APP Update Info: Info.InfoCrc = %d \r\n",CodeInfo->InfoCrc);

		printf("APP Update Info: CalInfoCrc = %d\r\n",CalInfoCrc);
		if(CalInfoCrc == CodeInfo->InfoCrc)
		{
			if( (CodeInfo->CodeSize <= CodeInfo->CodeMaxSize) && \
					((CodeInfo->UpdateFlag & 0xFFFF) == CN_UPDETE_FLAG) )
			{
				Info->CodeRdStartAddr 	= CodeInfo->CodeStartAddr;
				Info->CodeRdLen 		= CodeInfo->CodeSize;
				Info->CodeRdCrc      	= CodeInfo->CodeCrc & 0x0FFFF;

				Info->CalCrc      		= 0xFFFF;
				Ret = true;
			}
			else
			{
				printf("APP Update Info:CodeSize or Flag Error!\r\n");
			}
		}
		else
		{
			printf("APP Update Info:InfoCrc != CalInfoCrc!\r\n");
		}
	}
	return Ret;
}

// =============================================================================
// ���ܣ�APP�����¼��̣߳���AT45�ж�ȡ���ݣ�����ÿ���ֽ��ۼӣ�����������д�뵽Ƭ��FLASH��
//      Ӧ�ó���洢���������ۼӺ͵ĵ�16λУ�顣
// ��������
// ���أ�0��˵������ʧ�ܣ�����ֱ������APP
// =============================================================================
static ptu32_t Event_AppUpdate(void)
{
	u32 cnt = 0;
	tagAppInfo *Info;
	tagUpdateInfo *CodeInfo;
	u32 offset,ReadLen,RetryCnt;

	Info = &gAppInfo;
	RetryCnt = 0;
	offset = 0;
	AppCode_Erase(Info->CodeWrStartAddr,Info->CodeWrMaxLen);

	printf("APP Start Update...");
	while(1)
	{
		ReadLen = (Info->CodeRdLen > CN_UPDATE_BUF_LEN ) \
				? CN_UPDATE_BUF_LEN :Info->CodeRdLen;
		if(ReadLen != AppCode_Read(Info->CodeRdStartAddr + offset,Info->Buf,ReadLen))
		{
			RetryCnt++;
		}
		else
		{
			RetryCnt = 0;
			AppCode_Write(Info->CodeWrStartAddr+offset,Info->Buf,ReadLen);
			Info->CalCrc += Crc_Calculate(Info->Buf,ReadLen);
			Info->CodeRdLen -= ReadLen;
			offset       += ReadLen;
		}

		if(Info->CodeRdLen == 0)
		{
			Info->CalCrc = Info->CalCrc & 0x0FFFF;
			if(Info->CalCrc == Info->CodeRdCrc)
			{
				printf("APP Update Success!\r\n");
				CodeInfo = (tagUpdateInfo *)Info->Buf;
				offset = (u32)(&(CodeInfo->UpdateFlag)) - (u32)(CodeInfo);
				AT24_WriteBytes(Info->InfoStartAddr+offset,0x00,sizeof(u32));
				Sh_RunAPP(NULL);
				break;
			}
			else
			{
				printf("App Update Info:CalCrc != CodeRdCrc!\r\n");
				printf("CalCrc = %d, CodeRdCrc = %d \r\n",Info->CalCrc,Info->CodeRdCrc);
				break;
			}
		}

		if(cnt >= CN_UPDATE_RETRY_TIMES)
		{
			printf("App Update Info: Failed!\r\n");
			break;
		}

		Djy_EventDelay(10*mS);
	}

	return 0;
}

// =============================================================================
// ���ܣ����洢��AT24�����APP������Ϣ����У�飬У��ͨ���򴴽������¼����񣬽�������
// ������APP������Ϣ�洢��ַ����AT24�еĵ�ַ
// ���أ�true,��ȷ���������
// =============================================================================
bool_t ModuleInstall_AppUpdate(u32 InfoAddr)
{
	u16 evtt;
	bool_t Ret = false;
	tagAppInfo *Info;

	Info = &gAppInfo;

	Info->InfoStartAddr = InfoAddr;
	Info->InfoLen  = sizeof(tagUpdateInfo);
	Info->CodeWrStartAddr = gc_ptIbootSize + CN_FLASH_BASE_ADDR;
	Info->CodeWrMaxLen    = gc_ptFlashRange - gc_ptIbootSize;

	printf("App Update Info:Info Store Addr = %d\r\n",InfoAddr);
	if(AppInfo_Read(Info))
	{
		evtt = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
								Event_AppUpdate,NULL,2048,
									"SR1263_AppUpdate");
		if(evtt != CN_EVTT_ID_INVALID)
		{
			Djy_EventPop(evtt,NULL,0,0,0,0);
			Ret = true;
		}
		else
		{
			printf("App Update Info: Create Event Failed!\r\n");
		}
	}
	else
	{
		printf("App Update Info:Read Info Err!\r\n");
	}
    return Ret;
}



void Update_Test(u32 At45Addr)
{
	tagAppInfo *Info;
	u32 offset,len;
	u32 FlashAddr;
	tagUpdateInfo *CodeInfo;

	Info = &gAppInfo;

	Info->CodeWrStartAddr = gc_ptIbootSize + CN_FLASH_BASE_ADDR;
	Info->CodeWrMaxLen    = gc_ptFlashRange - gc_ptIbootSize;
	Info->CalCrc = 0xFFFF;
	printf("APP Update Test: Start...\r\n");

	len = Info->CodeWrMaxLen;
	FlashAddr = Info->CodeWrStartAddr;
	offset = 0;
	while(len)
	{
		memcpy(Info->Buf, (u8*)FlashAddr, CN_UPDATE_BUF_LEN);
		if(CN_UPDATE_BUF_LEN != AT45_FLASH_Write(At45Addr+offset,\
				Info->Buf,CN_UPDATE_BUF_LEN))
		{
			printf("APP Update Test: Write AT45 ERR!\r\n");
		}
		Info->CalCrc += Crc_Calculate(Info->Buf,CN_UPDATE_BUF_LEN);

		offset += CN_UPDATE_BUF_LEN;
		FlashAddr += CN_UPDATE_BUF_LEN;
		len -= CN_UPDATE_BUF_LEN;
	}

	CodeInfo = (tagUpdateInfo *)Info->Buf;
	CodeInfo->CodeSize = Info->CodeWrMaxLen;
	CodeInfo->CodeStartAddr = At45Addr;

	CodeInfo->CodeMaxSize = Info->CodeWrMaxLen;
	CodeInfo->UpdateFlag = CN_UPDETE_FLAG;
	CodeInfo->CodeCrc    = Info->CalCrc & 0x0FFFF;

	CodeInfo->InfoCrc = CodeInfo->CodeCrc + CodeInfo->CodeMaxSize + \
					CodeInfo->CodeSize + CodeInfo->CodeStartAddr + \
					CodeInfo->Res0[0] + CodeInfo->Res0[1] + \
					CodeInfo->Res1 + CodeInfo->UpdateFlag;

	if ( true == AT24_WriteBytes(0,Info->Buf,sizeof(tagUpdateInfo)))
		printf("APP Update Test: End!\r\n");
	else
		printf("APP Update Test: Failed!\r\n");
}


// =============================================================================
ptu32_t Wdt_Service(void)
{
	while(1)
	{
		Wdt_FeedDog();
		Djy_EventDelay(1*1000*1000);
	}
	return 1;
}
bool_t ModuleInstall_Wdt(void)
{
	u16 evtt;
	bool_t Ret = false;

	Wdt_Init();

	evtt = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_WDT,0,0,
                                Wdt_Service,NULL,0x800,
                                "Wdt Service");
	if(evtt != CN_EVTT_ID_INVALID)
	{
		Djy_EventPop(evtt,NULL,0,0,0,0);
		Ret = true;
	}
	else
	{
		printf("Wdt Service Evtt Create Failed!\r\n");
		Ret = false;
	}

	return Ret;
}











