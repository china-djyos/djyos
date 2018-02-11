// =============================================================================
// Copyright (C) 2012-2020 �����߲���ϵͳ�з��Ŷ�  All Rights Reserved
// �ļ���     ��cpu_peri_iic.c
// ģ������: FRAMģ�����������������������������Ա���
// ģ��汾: V1.00
// ������Ա: hm
// ����ʱ��: 20/06.2014
// =============================================================================
#include "stdint.h"
#include "cpu_peri.h"
#include "stdio.h"
#include "endian.h"
#include "fram_fm24cl64b.h"
#include "iicbus.h"
#include "endian.h"
#include "fram_fm24cl64b.h"

#define FRAM_MAX_ADDR                 (0x2000) //�����С���ֽڵ�λ
struct MutexLCB *  ptSemID_Fram;       // IIC���������־
static struct IIC_Device *pg_FRAM_Dev=NULL;
static s32 timeout = 2000*mS;//2000*mS;//;100*mS
#define FM24CL64B_CLK_FRE             (100*1000)      //�����ٶȣ���λHz
// =============================================================================
// ���ܣ�������ָ����ַ��ʼDstlen���ȵ�����
// ������fram_no,����ţ�Ϊ0��1
//       Fram_DstAddr��Ŀ��洢��Ԥ��ȡ���ݵĵ�ַ
//       pDstBuf����Ž��յ������ݻ����ַ
//       DstLen�� Ԥ��ȡ���ݵĳ���
// ���أ�ret, ʵ�ʶ������ֽ���
// =============================================================================
u32 FRAM_Read_Data(u32 Fram_DstAddr,u8 *pDstBuf,u32 DstLen)
{
    u32 ret = 0;
    if(Fram_DstAddr + DstLen> FRAM_MAX_ADDR)
        return 0;
    ret=IIC_Read(pg_FRAM_Dev, Fram_DstAddr, pDstBuf, DstLen, timeout);
    return ret;
}
// =============================================================================
// ���ܣ�д����ָ����ַ��ʼDstlen���ȵ�����
// ������fram_no,����ţ�Ϊ0��1
//      Fram_DstAddr��Ŀ��洢��Ԥд���ݵ�ַ
//      pSrcBuf���������ݻ����ַ
//      SrcLen�� �������ݳ���
// ���أ�ret, ʵ��д����ֽ���
// =============================================================================
u32 FRAM_Write_Data(u32 Fram_DstAddr,u8 *pSrcBuf,u32 SrcLen)
{
    u32 ret = 0;
    if(Fram_DstAddr + SrcLen > FRAM_MAX_ADDR)
        return 0;
    ret=IIC_Write(pg_FRAM_Dev, Fram_DstAddr, pSrcBuf, SrcLen,0, timeout);
    return ret;
}
// =============================================================================
// ���ܣ�д�ֵ����硣������ָ����ַ˫�ֳ��ȵ�����
// ������fram_no,����ţ�Ϊ0��1
//       Fram_DstAddr��Ŀ��洢��Ԥ����ַ
// ���أ�u32data,���ض���������
// =============================================================================
u32 FRAM_Read_DWord(u32 Fram_DstAddr)
{
    u32 u32data,buf[4];
    u32  rLen = 4;
    u32 ret = 0;

    ret = FRAM_Read_Data(Fram_DstAddr,(u8 *)&buf,rLen);
    if(ret != rLen)
        return 0;
    //��С��ת��
    u32data = pick_little_32bit((u8 *)&buf,0);
    return u32data;
}
// =============================================================================
// ���ܣ�д�ֵ����磬д˫�����ݵ�ָ����ַ��FRAM
// ������fram_no,����ţ�Ϊ0��1
//       Fram_DstAddr��Ŀ��洢��Ԥд��ַ
// ���أ�ret, ���������룬�޴���ΪERR_N0_ERROR
// =============================================================================
u32 FRAM_Write_DWord(u32 Fram_DstAddr,u32 u32data)
{
    u32  rLen = 4;
    u8 buf[4];
    u32 ret = 0;

    fill_little_32bit((u8 *)&buf,0,u32data);
    ret = FRAM_Write_Data(Fram_DstAddr,(u8 *)&buf,rLen);
    return ret;
}

// =============================================================================
// ���ܣ���˫�֣�������ָ����ַ˫�ֳ��ȵ�����
// ������fram_no,����ţ�Ϊ0��1
//       Fram_DstAddr��Ŀ��洢��Ԥ����ַ
// ���أ�u16data,���ض���������
// =============================================================================
u16 FRAM_Read_Word(u32 Fram_DstAddr)
{
    u16 u16data,buf[2];
    u8  rLen = 2;
    u32 ret = 0;

    ret = FRAM_Read_Data(Fram_DstAddr,(u8 *)&buf,rLen);
    if(ret != rLen)
        return 0;
    //��С�˴洢�ж�ȡ�������ݣ�ת��Ϊ���ģʽ
    u16data = pick_little_16bit((u8 *)&buf,0);
    return u16data;
}

// =============================================================================
// ���ܣ�д�ֵ����磬д�����ݵ�ָ����ַ��FRAM
// ������fram_no,����ţ�Ϊ0��1
//       Fram_DstAddr��Ŀ��洢��Ԥд��ַ
//       u16data,д�������
// ���أ�ret, ���������룬�޴���ΪERR_N0_ERROR
// =============================================================================
u32 FRAM_Write_Word( u32 Fram_DstAddr,u16 u16data)
{
    u8  rLen = 2,buf[2];
    u32 ret = 0;

    //����˸ĳ�С�� ����С�˷�ʽ�洢��FRAM
    fill_little_16bit((u8 *)&buf,0,u16data);
    ret = FRAM_Write_Data(Fram_DstAddr,(u8 *)&buf,rLen);
    return ret;
}
// =============================================================================
// ���ܣ�������д�ֵ����磬������WORD��ȡ��������
// ��������ȡWORD��ַ
// ���أ���ȡ��WORD����
// =============================================================================
u16 FRAM_Read_Word_r(u16 wAddress )
{
    u16    wData = 0;
	if(-1 == Lock_MutexPend( ptSemID_Fram, CN_TIMEOUT_FOREVER))  // ��ȡдȨ��
	{
		printf("FRAM1_Read_Word_r ��ȡдȨ��ʧ��\n");
		return  false;
	}
	wData = FRAM_Read_Word(wAddress );   // д����
	Lock_MutexPost( ptSemID_Fram );
    return    wData;
}

// =============================================================================
// ���ܣ��źű��������ݣ������u8��ȡ��������
// ��������ȡWORD��ַ, ���滺���� ��ȡ����
// ���أ� ��ȡ���ݳ���
// =============================================================================
u32 FRAM_Read_Data_r( u16 wAddress, u8 *pbyBuf, u32 dwLen )
{
    register u32      dwStatus = 0;

	if(-1 == Lock_MutexPend( ptSemID_Fram, CN_TIMEOUT_FOREVER))   // ��ȡдȨ��
	{
		printf("FRAM1_Read_Data ��ȡдȨ��ʧ��\n");
		return  false;
	}
	dwStatus = FRAM_Read_Data(wAddress, pbyBuf, dwLen );// д����
	Lock_MutexPost( ptSemID_Fram );

    return    dwStatus;
}

// =============================================================================
// ���ܣ��źű������֣�������WORDд��������
// ��������ȡWORD��ַ, д������
// ���أ�  д�����ݳ���
// =============================================================================
s32 FRAM_Write_Word_r(u16 wAddress, u16 wData )
{
    s32    Status = 0;
	if(-1 == Lock_MutexPend( ptSemID_Fram, CN_TIMEOUT_FOREVER))       // ��ȡдȨ��
	{
		printf("FRAM1_Write_Word_r ��ȡдȨ��ʧ��\n");
		return  false;
	}
	Status = FRAM_Write_Word(wAddress, wData);   // д����
	Lock_MutexPost( ptSemID_Fram);
    return    Status;
}

// =============================================================================
// ���ܣ��źű���д���ݣ������u8д��������
// ��������ȡWORD��ַ, д������
// ���أ�  д�����ݳ���
// =============================================================================
u32 FRAM_Write_Data_r(u16 wAddress, u8 *pbyBuf, u32 dwLen)
{
    register u32      dwStatus = 0;
	if(-1 == Lock_MutexPend( ptSemID_Fram, CN_TIMEOUT_FOREVER))   // ��ȡдȨ��
	{
		printf("FRAM1_Write_Data_r ��ȡдȨ��ʧ��\n");
		return  false;
	}
	dwStatus = FRAM_Write_Data(wAddress, pbyBuf, dwLen );// д����
	Lock_MutexPost( ptSemID_Fram );

    return    dwStatus;
}

// =============================================================================
// ���ܣ���ʼ��framоƬ������IIC������ʼ������
// ����: para,����ţ�0��1
// ����: true= �ɹ���false=ʧ��
// =============================================================================
ptu32_t ModuleInit_Fram(char *BusName)
{
    static struct IIC_Device Dev;
    char *DevName;
	DevName = "FRAM_FM24CL64B";
	ptSemID_Fram = Lock_MutexCreate("Fram0 Lock");
    if(NULL != IIC_DevAdd_s(&Dev,BusName,DevName,FRAM_ADDR,1,17))
    {
    	pg_FRAM_Dev=&Dev;
    	IIC_BusCtrl(pg_FRAM_Dev,CN_IIC_SET_CLK,FM24CL64B_CLK_FRE,0);
    	IIC_BusCtrl(pg_FRAM_Dev,CN_IIC_SET_POLL,0,0);
    	return true;
    }
    else
        return true;
}




