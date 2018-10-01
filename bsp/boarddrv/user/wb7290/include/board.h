//----------------------------------------------------
// Copyright (c) 2018,Open source team. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ�ɶ����߲���ϵͳ��Դ�Ŷ����С�����Ȩ�˱���һ��Ȩ����
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

#ifndef __BOARD__H__
#define __BOARD__H__

#include "./include/ad7616.h"
#include "./include/ads8688.h"
#include "./include/dcoutput.h"
#include "./include/kiko.h"
#include "./include/lcd-6963.h"

#ifdef __cplusplus
extern "C" {
#endif

void BackupMode_Entry(void);
u32 Lp_IRQHandler(ptu32_t para);
void BackupMode_Config(void);

void Board_UartHalfDuplexSend(u8 SerialNo);
void Board_UartHalfDuplexRecv(u8 SerialNo);
void ProgRegionGet(u32 *pStart, u32 *pEnd);
void Board_GpioInit(void);
void AT24_WPEn(u8 En);

enum EN_BlackBoxDealResult BrdExp_RecordSave(struct BlackBoxRecordPara  *recordpara);
bool_t BrdExp_RecordClean(void);
bool_t BrdExp_RecordCheckNum(u32 *recordnum);
bool_t BrdExp_RecordCheckLen(u32 assignedno, u32 *recordlen);
bool_t BrdExp_RecordGet(u32 assignedno,u32 buflen,u8 *buf,\
                            struct BlackBoxRecordPara *recordpara);
void BrdExp_RecordScan(void);
bool_t ModuleInstall_BrdExp(ptu32_t para);

bool_t BrdWdt_FeedDog(void);
u32 RttFeedDog_Isr(ptu32_t intline);
bool_t BrdBoot_FeedStart(u32 bootfeedtime);
bool_t BrdBoot_FeedEnd(void);
void ModuleInstall_BrdWdt(void);

void Lcd_Clear(u8 color);
void Lcd_ContrastAdjust(u8 Value);
void ModuleInstall_LCD(void);
void Lcd_SetBackLight(uint8_t byCtrl);
void Lcd_wrDISPram(uint32_t dwleft,uint32_t dwtop,uint32_t dwsize,uint8_t * pbysrcadd);

#ifdef __cplusplus
}
#endif
#endif






