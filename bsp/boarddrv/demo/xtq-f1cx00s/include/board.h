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

#ifndef __BOARD__H__
#define __BOARD__H__

//#include "./include/ctiic.h"
#include "cpu_peri.h"
#include "IoIicBus.h"

#ifdef __cplusplus
extern "C" {
#endif

#if CFG_ENABLE_OV2640 == true       //����ͷ����ռ����TP��PIN
/**********************************ov2640 SCCB*********************************/
//IO��������
#define SCCB_SCL_OUT() GPIO_Congif(GPIOE, GPIO_Pin_11, GPIO_Mode_OUT, GPIO_PuPd_UP)  //PA0���ģʽ
#define SCCB_SDA_IN()  GPIO_Congif(GPIOE, GPIO_Pin_12, GPIO_Mode_IN,  GPIO_PuPd_UP)  //PA1����ģʽ
#define SCCB_SDA_OUT() GPIO_Congif(GPIOE, GPIO_Pin_12, GPIO_Mode_OUT, GPIO_PuPd_UP)  //PA1���ģʽ
//IO��������
#define SCCB_IIC_SCL(n) (n?GPIO_SettoHigh(GPIOE,GPIO_Pin_11):GPIO_SettoLow(GPIOE,GPIO_Pin_11))//SCL
#define SCCB_IIC_SDA(n) (n?GPIO_SettoHigh(GPIOE,GPIO_Pin_12):GPIO_SettoLow(GPIOE,GPIO_Pin_12))//SDA
#define SCCB_READ_SDA   (GPIO_READ(GPIOE,GPIO_Pin_12))//����SDA
/******************************************************************************/
#else
/**********************************gt911**************************************/
//IO��������
#define CT_SCL_OUT() GPIO_Congif(GPIOA, GPIO_Pin_0, GPIO_Mode_OUT, GPIO_PuPd_UP)  //PA0���ģʽ
#define CT_SDA_IN()  GPIO_Congif(GPIOA, GPIO_Pin_2, GPIO_Mode_IN,  GPIO_PuPd_UP)  //PA1����ģʽ
#define CT_SDA_OUT() GPIO_Congif(GPIOA, GPIO_Pin_2, GPIO_Mode_OUT, GPIO_PuPd_UP)  //PA1���ģʽ
//IO��������
#define CT_IIC_SCL(n) (n?GPIO_SettoHigh(GPIOA,GPIO_Pin_0):GPIO_SettoLow(GPIOA,GPIO_Pin_0))//SCL
#define CT_IIC_SDA(n) (n?GPIO_SettoHigh(GPIOA,GPIO_Pin_2):GPIO_SettoLow(GPIOA,GPIO_Pin_2))//SDA
#define CT_READ_SDA   (GPIO_READ(GPIOA,GPIO_Pin_2))//����SDA

/*������оƬ�ӿ�IO*/
#define FT_RST(n)  (n?GPIO_SettoHigh(GPIOA,GPIO_Pin_3):GPIO_SettoLow(GPIOA,GPIO_Pin_3))
#define FT_INT      (GPIO_READ(GPIOA,GPIO_Pin_1))
/******************************************************************************/
#endif

void Board_Init(void);
void FT6236_Pin_Init(void);
u32 IIC_IoCtrlFunc(enum IIc_Io IO,u32 tag);
void OpenScreen();
void CloseScreen();
void ClosePower();
void CloseSpeaker();
void OpenSpeaker();
enum SpeakerState GetSpeakerState();
ptu32_t Iboot_GetResetAddr(void);

#ifdef __cplusplus
}
#endif
#endif






