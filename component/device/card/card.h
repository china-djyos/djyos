//-----------------------------------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
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

#ifndef __CARD_H__
#define __CARD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stddef.h"

//
// SD��SDIO��MMC�ȳ���ṹ��
//

// CardDescription.Typeֵ
#define MMC         ((u8)0)
#define SDIO        ((u8)1)
#define SDSC        ((u8)2)
#define SDHC        ((u8)3)
#define SDXC        ((u8)4)

// CardDescription.SpecVerֵ
#define V1          ((u8)0)     // Version 1.0 and 1.01
#define V1_1        ((u8)1)     // Version 1.10
#define V2          ((u8)2)     // Version 2.00
#define V3          ((u8)3)     // Version 3.0X
#define V4          ((u8)4)     // Version 4.XX
#define UNKNOWN     ((u8)255)   // δ֪,������

// CardDescription.Stateֵ
#define IDLE        ((u8)0)
#define IDENT       ((u8)1)
#define STANDBY     ((u8)2)
#define TRANSFER    ((u8)3)
#define PROGRAM     ((u8)4)

struct CardDescription{
    u8 Type;                    // ��������
    u8 SpecVer;                 // Physical Layer Specification Version
    u8 State;                   // ���ĵ�ǰ״̬
    u8 TransSpeed;              // ������ٶ�(0x32 = 25MBit/s)
    u32 CSize;                  // ����(byteΪ��λ)
    u8 ReadBlLen;               // ���С,2�η�
    u8 ReadBlPartial;           // ��С��д��(0:��Ϊ��λ,1:ByteΪ��λ)
    u8 CSizeMult;               // todo: �������������̫��ʵ����;
    u8 BusWidth;                //
    u8 DataStatusAfterErases;   // �鱻�������״̬��Ϊ1��0
    u16 RCA;                    // ��������Ե�ַ
    u16 BlkLenSettings;         // ʵ�ʶ�д���С,�������Ϳ����������,2�η�
    u64 CapacityInBytes;        // ������, �ֽ�Ϊ��λ.
};

#ifdef __cplusplus
}
#endif

#endif //__CARD_H__
