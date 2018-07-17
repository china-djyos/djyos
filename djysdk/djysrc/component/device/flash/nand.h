//-----------------------------------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

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
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------

#ifndef __NAND_H__
#define __NAND_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

//
//JEDEC��׼����
//
#define PAGE_READ_CMD_BYTE1                 (0x00)
#define PAGE_READ_CMD_BYTE2                 (0x30)
#define PAGE_PROGRAM_CMD_BYTE1              (0x80)
#define PAGE_PROGRAM_CMD_BYTE2              (0x10)
#define BLOCK_ERASE_CMD_BYTE1               (0x60)
#define BLOCK_ERASE_CMD_BYTE2               (0xD0)
#define ID_READ_CMD_BYTE                    (0x90)
#define STATUS_READ_CMD_BYTE                (0x70)
#define PARAMETER_PAGE_READ_CMD_BYTE		(0xEC)
#define RESET_CMD_BYTE                      (0xFF)

// 
// @brief  NAND�豸��Ϣ
// 
struct NandDescr{
    u16      Type;              		// �Ƿ�֧��ONFI
#define ONFI                    		((u16)0x0001)
#define RAW                     		((u16)0x0000)
    u16      Revision;          		// ONFI�汾��
    char     Manufacturer[13];  		// оƬ������
    char     DeviceModel[21];   		// оƬ�ͺ�
    u16      OOB_Size;          		// Out Of Band�ռ�ߴ�,�ֽ�Ϊ��λ
    u32      BytesPerPage;      		// ҳ�а������ֽ���
    u32      PagesPerBlk;       		// ���а�����ҳ��
    u32      BlksPerLUN;        		// LUN�а����Ŀ���
    u8       LUNs;              		// LUN����
#if 0
    u8       SafeBlkNo;         		// ����ȷ�����õĿ��
#endif
    u8       BadMarkOffset;     		// ��������spare�е�ƫ��,���鷽ʽ��λ
    u16		 ReservedBlks;				// �����Ŀ������ӵ�0�鿪ʼ����
    u8       Controller;        		// NAND���������Լ�����
#define NO_ECC_SUPPORTED        		((u8)0x0)
#define HW_ECC_SUPPORTED        		((u8)0x1)
#define SW_ECC_SUPPORTED        		((u8)0x2)
};
//
//
//
struct NandSpecialOps{
	s32 (*ChkBlk)(u32 BlkNo); 			// �����Ƿ��ѻ�
	s32 (*MrkBad)(u32 BlkNo); 			// ��ǻ���
};
#ifdef __cplusplus
}
#endif

#endif //ifndef __NAND_H__
