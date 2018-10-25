//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
//���ߣ�djyos�Ŷ�
//�汾��V1.0.0
//�ļ�����: s3c2440 nand�������Ĵ�������
//����˵��:
//�޶���ʷ:
// 1. ����: 2011-11-29
//   ����: djyos�Ŷ�
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __CPU_PERI_NAND__
#define __CPU_PERI_NAND__

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

struct NFC_REG
{
    vu32 NFCONF;        //NAND Flash configuration
    vu32 NFCONT;        //NAND Flash control

    vu8  NFCMD;         //NAND Flash command
    vu8  rsv0[3];

    vu8  NFADDR;        //NAND Flash address
    vu8  rsv1[3];

    vu8  NFDATA;        //NAND Flash data
    vu8  rsv2[3];

    vu32 NFMECCD0;      //NAND Flash ECC for Main
    vu32 NFMECCD1;      //NAND Flash ECC for Main
    vu32 NFSECCD;       //NAND Flash ECC for Spare Area
    vu32 NFSBLK;        //NAND Flash programmable start block address
    vu32 NFEBLK;        //NAND Flash programmable end block address
    vu32 NFSTAT;        //NAND Flash operation status
    vu32 NFECCERR0;     //NAND Flash ECC Error Status for I/O [7:0]
    vu32 NFECCERR1;     //NAND Flash ECC Error Status for I/O [15:8]
    vu32 NFMECC0;       //SLC or MLC NAND Flash ECC status
    vu32 NFMECC1;       //SLC or MLC NAND Flash ECC status
    vu32 NFSECC;        //NAND Flash ECC for I/O[15:0]
    vu32 NFMLCBITPT;    //NAND Flash 4-bit ECC Error Pattern for data[7:0]
};

#define NFC_REG_BASE    ((struct NFC_REG*)0x4E000000)

/*
#define NFCONF      0x4E000000      //NAND Flash configuration
#define NFCONT      0x4E000004      //NAND Flash control
#define NFCMD       0x4E000008      //NAND Flash command
#define NFADDR      0x4E00000C      //NAND Flash address
#define NFDATA      0x4E000010      //NAND Flash data
#define NFMECCD0    0x4E000014      //NAND Flash ECC for Main
#define NFMECCD1    0x4E000018      //NAND Flash ECC for Main
#define NFSECCD     0x4E00001C      //NAND Flash ECC for Spare Area
#define NFSBLK      0x4E000020      //NAND Flash programmable start block address
#define NFEBLK      0x4E000024      //NAND Flash programmable end block address
#define NFSTAT      0x4E000028      //NAND Flash operation status
#define NFECCERR0   0x4E00002C      //NAND Flash ECC Error Status for I/O [7:0]
#define NFECCERR1   0x4E000030      //NAND Flash ECC Error Status for I/O [15:8]
#define NFMECC0     0x4E000034      //SLC or MLC NAND Flash ECC status
#define NFMECC1     0x4E000038      //SLC or MLC NAND Flash ECC status
#define NFSECC      0x4E00003C      //NAND Flash ECC for I/O[15:0]
#define NFMLCBITPT  0x4E000040      //NAND Flash 4-bit ECC Error Pattern for data[7:0]
*/
#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_NAND__
