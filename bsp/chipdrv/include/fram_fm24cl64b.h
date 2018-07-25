// =============================================================================
// Copyright (C) 2012-2020 �����߲���ϵͳ�з��Ŷ�  All Rights Reserved
// �ļ���     ��cpu_peri_iic.c
// ģ������: FRAMģ�����������������������������Ա���
// ģ��汾: V1.00
// ������Ա: hm
// ����ʱ��: 20/06.2014
// =============================================================================

#ifndef FRAM_FM24CL64B_DRIVER_H_
#define FRAM_FM24CL64B_DRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

#define CN_FRAM0_NO    0
#define CN_FRAM1_NO    1

u32 FRAM_Read_Data(u32 Fram_DstAddr,u8 *pDstBuf,u32 DstLen);
u32 FRAM_Write_Data(u32 Fram_DstAddr,u8 *pSrcBuf,u32 SrcLen);
u32 FRAM_Read_DWord(u32 Fram_DstAddr);
u32 FRAM_Write_DWord(u32 Fram_DstAddr,u32 u32data);
u16 FRAM_Read_Word(u32 Fram_DstAddr);
u32 FRAM_Write_Word(u32 Fram_DstAddr,u16 u16data);

// ������IIC��������
u16   FRAM_Read_Word_r   (u16 wAddress );
s32   FRAM_Write_Word_r  (u16 wAddress, u16 wData );
u32   FRAM_Read_Data_r   (u16 wAddress, u8 *pbyBuf, u32 dwLen );
u32   FRAM_Write_Data_r  (u16 wAddress, u8 *pbyBuf, u32 dwLen );

ptu32_t ModuleInit_Fram(char *BusName);

#ifdef __cplusplus
}
#endif


#endif
