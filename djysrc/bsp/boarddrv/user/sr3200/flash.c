// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���    ��flash.c
// ģ������: Ƭ��flash����������д��
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 01/06.2015
// =============================================================================

#include "stdint.h"
#include "flash.h"

#define IAP_CLK 80000000
#define IAP_CLK_KHZ     (IAP_CLK / 1000)

#define IAP_LOCATION 0x1FFF1FF1
#define iap_entry(a, b)     ((void (*)())(IAP_LOCATION))(a, b)

// =============================================================================
// ���ܣ���ȡ����ID����ȡ��ID�洢������ָ���ַλ��
// ������PartID,����ID�洢��ַ
// ���أ� IAP_STAT_CMD_SUCCESS
//		 IAP_STAT_INVALID_SECTOR
//		 IAP_STAT_BUSY
// =============================================================================
u32 IAP_ReadParID (u32 * PartID)
{
	u32 command[5]={0,0,0,0,0};
	u32 result[5]={0,0,0,0,0};

	command[0] = IAP_CMD_ReadPartID;
	iap_entry(command, result);
	*PartID = result[1];

	return result[0];
}

// =============================================================================
// ���ܣ��ж�ָ������ʼsector�Ƿ��Ѿ�׼���ÿɹ�������д����
// ������Start��End����ʼ���
// ���أ� IAP_STAT_CMD_SUCCESS
//		 IAP_STAT_INVALID_SECTOR
//		 IAP_STAT_BUSY
// =============================================================================
u32 IAP_PrepareSector (u32 Start,  u32 End)
{
	u32 command[5]={0,0,0,0,0};
	u32 result[5]={0,0,0,0,0};

	command[0] = IAP_CMD_PrepareSecor;
	command[1] = Start;
	command[2] = End;
	iap_entry(command, result);

	return result[0];
}

// =============================================================================
// ���ܣ�����ָ����ʼ���
// ������Start��End����ʼ���
// ���أ� IAP_STAT_CMD_SUCCESS
//		 IAP_STAT_INVALID_SECTOR
//		 IAP_STAT_BUSY
// =============================================================================
u32 IAP_EraseSector (u32 Start,u32 End)
{
	u32 command[5]={0,0,0,0,0};
	u32 result[5]={0,0,0,0,0};

	command[0] = IAP_CMD_EraseSecor;
	command[1] = Start;
	command[2] = End;
	command[3] = IAP_CLK_KHZ;   // IAP_CLK / 1000;
	iap_entry(command, result);

	return result[0];
}

// =============================================================================
// ���ܣ����ָ����ʼsector�Ƿ�Ϊ�գ�һ����ڲ�����ʱ����һ�μ��
// ������Start��End����ʼ����
//       NonBlank����һ���ǿ�ֵ��λ��
// ���أ�IAP_STAT_SECTOR_NOT_BLANK
//      IAP_STAT_CMD_SUCCESS
//		IAP_STAT_INVALID_SECTOR
//		IAP_STAT_BUSY
// =============================================================================
u32 IAP_BlankCheckSecor (u32 Start,  u32 End, u32 * NonBlank)
{
	u32 command[5]={0,0,0,0,0};
	u32 result[5]={0,0,0,0,0};

	command[0] = IAP_CMD_BlankCheckSecor;
	command[1] = Start;
	command[2] = End;

	iap_entry(command, result);

	if (result[0] == IAP_STAT_SECTOR_NOT_BLANK)
	{
		*NonBlank = result[1];
		*(NonBlank+1) = result[2];
	}

	return result[0];
}

// =============================================================================
// ���ܣ���RAM�е�����д�뵽FLASH
// ������Dst����д�������ֽڵ�Ŀ�� Flash ��ַ���õ�ַ�ı߽�ӦΪ 256 �ֽڡ�
//      Src�������������ֽڵ�Դ RAM ��ַ���õ�ַӦ����Ϊ�߽硣
//      Number����д����ֽ�����Ӧ��Ϊ 256 | 512 | 1024 | 4096
// ���أ� IAP_STAT_CMD_SUCCESS
//		IAP_STAT_SRC_ADDR_ERROR
//		STAT_DST_ADDR_ERROR
//		IAP_STAT_SRC_ADDR_NOT_MAPPED
//		IAP_STAT_DST_ADDR_NOT_MAPPED
//		IAP_STAT_COUNT_ERROR
//		IAP_STAT_SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION
//		IAP_STAT_BUSY
// =============================================================================
u32 IAP_CopyRAMToFlash (u32 Dst,  u32 Src, u32 Number)
{
	u32 command[5]={0,0,0,0,0};
	u32 result[5]={0,0,0,0,0};

	command[0] = IAP_CMD_CopyRAMToFlash;
	command[1] = Dst;
	command[2] = Src;
	command[3] = Number;
	command[4] = IAP_CLK_KHZ;	// Fcclk in KHz
	iap_entry(command, result);

	return result[0];
}

// =============================================================================
// ���ܣ���flash�ж�����
// ������Addr,��ַ
//      Buf,�洢λ��
//      Len,���ȣ����ֽ�Ϊ��λ
// ���أ��޷���
// =============================================================================
void Flash_ReadData(u32 Addr, u32 *Buf, u32 Len)
{
	u32 i;

	for(i = 0; i < Len; i++)
	{
		Buf[i] = *(u32*)(Addr + i*4);
	}
}





