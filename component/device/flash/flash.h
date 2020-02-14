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

#ifndef __FLASH_H__
#define __FLASH_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "jedec.h"
#include "nor.h"
#include "nand.h"
#include "embedded_flash.h"
#include "misc.h"

//
// Flags
//
#define FLASH_ERASE_ALL                     ((u32)0x1)
#define FLASH_BUFFERED                      ((u32)0x2)

//
// ��дFLAG��־
//
#define SPARE_REQ                           ((u32)0x00000001)
#define HW_ECC                              ((u32)0x00010000)
#define SW_ECC                              ((u32)0x00020000)
#define NO_ECC                              ((u32)0x00040000)
#define MASK_ECC                            ((u32)0x00070000)

//
// ������ECC��SPAREͬʱ����,��ΪECC�Զ�д��SPARE����
//
#define ASSERT_FLAGS(x)                     do{\
                                            if(((x & HW_ECC) || (x & SW_ECC)) && (x & SPARE_REQ))\
                                            return (-1);\
                                            }while(0)

//
// FLASH������ʽ
//
struct FlashOps{
    s32 (*RdPage)(u32 PageNo, u8 *Data, u32 Flags); // ��ҳ
    s32 (*WrPage)(u32 PageNo, u8 *Data, u32 Flags); // дҳ,ȥ��const ��Ϊ����ECC�������
    s32 (*ErsBlk)(u32 BlkNo); // ������
    s32 (*PageToBlk)(u32 PageNo, u32 *Remains, u32 *BlkNo); // ͨ��ҳ
    union{
        struct NandSpecialOps Nand; // nand�����ر����
        struct NorSpecialOps Nor; // nor�����ر����
    }Special;

};

//
// FLASH�豸�ײ������Ϣ
// TODO: ��ȡ��һЩ������Ϣ
struct FlashChip{
    union {
        struct NandDescr Nand; // ����NAND��Ϣ
        struct NorDescr Nor; // ����NOR��Ϣ
        struct EmFlashDescr Embd; // ���������FLASH
    }Descr;
    u32 dwTotalPages;
    u32 dwPageBytes;
    u32 dwPagesReserved;
    struct FlashOps Ops; // ����������,����ʵ���ҵ��ڵ㼴������Ĺ���
    void *Lock; // todo: ���ڴ��ڻ���,��Ҫ�ײ㻥������
    u8 *Buf; // �ײ㻺��
    u8 Type; // Flash������
#define F_NAND         (0x1)
#define F_NOR          (0x2)
#define F_ALIEN        (0x3)
    char Name[]; // �����ı���
};

//
//
//

#ifdef __cplusplus
}
#endif

#endif // ifndef __FLASH_H__
