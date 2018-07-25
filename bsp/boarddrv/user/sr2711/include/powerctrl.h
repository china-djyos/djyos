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
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��powerctrl.h
// ģ������: SR2711�����Դ�������
// ģ��汾: V1.00
// ������Ա: zhb
// ����ʱ��: 8/14.2017
// =============================================================================
#ifndef __POWER_CTRL_H__
#define __POWER_CTRL_H__

#include "stdint.h"


#ifdef __cplusplus
extern "C" {
#endif

//#define In_LowerPower       1
//#define Out_LowerPower      0

enum BoardPeriCode
{
	EN_PERI_SDRAM=0,             //SDRAM
	EN_PERI_EMMC,              //EMMC
	EN_PERI_EMAC,              //EMAC
	EN_PERI_IMAGE_PROCESS,     //ģ��ͼ����оƬ
	EN_PERI_CAMERA_HEADER,     //����ͷ���
	EN_PERI_30W_POWER,         //30W��Դ
	EN_PERI_10W_POWER,         //10W��Դ
	EN_PERI_SENSOR1,           //��������1
	EN_PERI_SENSOR2,           //��������2
	EN_PERI_SENSOR3,           //��������3
	EN_PERI_SENSOR4,           //��������4
	EN_PERI_SENSOR5,           //��������5
	EN_PERI_SENSOR6,           //��������6
	EN_PERI_SENSOR7,           //��������7
	EN_PERI_SENSOR8,           //��������8
	EN_PERI_SENSOR9,           //��������9
	EN_PERI_SENSOR10,          //��������10
	EN_PERI_SENSOR11,          //��������11
	EN_PERI_SENSOR12,          //��������12
	EN_PERI_SENSOR13,          //��������13
	EN_PERI_RF1,               //RFģ��
	EN_PERI_RTC,               //RTC
	EN_PERI_ENCRYPTION,        //��������оƬ
	EN_PERI_RF2,               //������Ƶģ��
	EN_PERI_BATTERY_SAMPLE,    //��ز���
	EN_PERI_5V_SAMPLE,         //5V����
	EN_PERI_SAMPLE_RESERVED,   //Ԥ������
	EN_DATA_EXPORT_USB,        //USB���ݵ���
	EN_PERI_4G_USB,            //4Gȫ��ͨUSB
	EN_PERI_4G_UART,           //4Gȫ��ͨ����
	EN_PERI_DEBUG_UART,        //
	EN_PERI_END,
};
// =============================================================================
// ���ܣ����ģ���������Դ���ƺ���
// ������PeriIdx,������;
//     Flag,ʹ�ܱ�ʶ��,true������͹���״̬;false,�˳��͹���״̬.
// ���أ���
// =============================================================================
void Peri_LowPowerCfg(enum BoardPeriCode PeriIdx,bool_t Flag);
void Board_PowerCtrlInit(void);
void Peri_NameInit(void);
ptu32_t Power_Ctrl_Shell_Module_Install(void);
void Module_PeriCtrlInit(void);


#ifdef __cplusplus
}
#endif

#endif
