
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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合以下二条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、此三条件表列，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、此三条件表列，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
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


#define CN_UPDATE_BUF_LEN		1024				//缓冲区大小
#define CN_UPDATE_RETRY_TIMES	4					//读AT45的容错次数
#define CN_UPDETE_FLAG			(0xA5A5)			//升级标志
#define CN_FLASH_BASE_ADDR      (0x08000000)		//芯片FLASH起始地址
#define CN_FLASH_PAGE_SIZE      (2048)				//芯片FLASH页大小

extern bool_t  Sh_RunAPP(char *param);

typedef struct
{
	u32 CodeSize;				//APP代码大小
	u32 CodeCrc;				//APP代码CRC，累加和
	u32 Res0[2];
	u32 CodeStartAddr;			//app代码起始地址
	u32 CodeMaxSize;			//app代码最大长度
	u32 Res1;
	u32 UpdateFlag; 			//升级标志
	u32 InfoCrc;				//存储结构CRC
}tagUpdateInfo;

typedef struct
{
	u32 InfoStartAddr;		//信息存储地址
	u32 InfoLen;			//信息长度
	u32 CodeRdStartAddr;	//应用程序存储
	u32 CodeRdLen;			//应用程序的长度
	u32 CodeRdCrc;			//存储的应用CRC
	u32 CalCrc;				//本地计算的CRC

	u32 CodeWrStartAddr;	//APP在Flash写入起始地址
	u32 CodeWrMaxLen;		//APP最大允许长度
	u8 Buf[CN_UPDATE_BUF_LEN];
}tagAppInfo;

static tagAppInfo gAppInfo;

extern u32 gc_ptIbootSize;				//iboot.bin所占大小
extern u32 gc_ptFlashRange;				//总共flash的大小

// =============================================================================
// 功能：从AT45中读数据
// 参数：
// 返回：所读字节数
// =============================================================================
static u32 AppCode_Read(u32 addr,u8 *buf,u32 len)
{
	return AT45_FLASH_Read(addr,buf,len);
}

// =============================================================================
// 功能：擦除要写入代码区的flash，Flash分为两个bank，每个bank都有等数量的page
// 参数：PageStart,擦除起始页（按总页数算）
//      Pages，擦除的页数
// 返回：0,正确，否则错误
// =============================================================================
static s32 AppCode_Erase(u32 StartAddr,u32 len)
{
	s32 Ret;
	u32 SECTORError=0;//保存出错类型信息
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
// 功能：将Code代码写入到FLASH指定的地址，直接调用了官方库函数(要求Addr和len按8字节对齐)
// 参数：Addr,地址；buf，缓冲区；len，字节数量
// 返回：写入的字节数
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
// 功能：累加和计算并返回计算值
// 参数：buf,len
// 返回：计算结果
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
// 功能：从AT24中读APP信息，并对信息进行累加和校验
// 参数：Info指针
// 返回：true,正确，否则错误
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
// 功能：APP升级事件线程，从AT45中读取数据，并对每个字节累加，读到的数据写入到片内FLASH的
//      应用程序存储区，最后对累加和的低16位校验。
// 参数：无
// 返回：0，说明升级失败，否则直接运行APP
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
// 功能：检测存储在AT24里面的APP升级信息，并校验，校验通过则创建升级事件任务，进行升级
// 参数：APP升级信息存储地址，即AT24中的地址
// 返回：true,正确，否则错误
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











