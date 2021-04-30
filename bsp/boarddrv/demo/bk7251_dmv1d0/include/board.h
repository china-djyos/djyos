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
#include <gpio_pub.h>
#ifdef __cplusplus
extern "C" {
#endif

/**********************************ft5x26********************************************/
//IO��������
//#define CT_SCL_OUT() djy_gpio_mode(GPIO4,PIN_MODE_OUTPUT)  //���ģʽ
#define CT_SCL_OUT() gpio_config(GPIO4, GMODE_OUTPUT);
//#define CT_SDA_IN()  djy_gpio_mode(GPIO2,PIN_MODE_INPUT_PULLUP)  //����ģʽ
#define CT_SDA_IN()  gpio_config(GPIO2, GMODE_INPUT_PULLUP)  //����ģʽ
//#define CT_SDA_OUT() djy_gpio_mode(GPIO2,PIN_MODE_OUTPUT)  //���ģʽ
#define CT_SDA_OUT() gpio_config(GPIO2, GMODE_OUTPUT)  //���ģʽ

//IO��������
//#define CT_IIC_SCL(n) djy_gpio_write(GPIO4,n)//SCL
#define CT_IIC_SCL(n) gpio_output(GPIO4,n)//SCL
//#define CT_IIC_SDA(n) djy_gpio_write(GPIO2,n)//SDA
#define CT_IIC_SDA(n) gpio_output(GPIO2,n)//SDA
//#define CT_READ_SDA   djy_gpio_read(GPIO2)//����SDA
#define CT_READ_SDA   gpio_input(GPIO2)//����SDA

/*������оƬ�ӿ�IO*/
//#define FT_RST(n)  djy_gpio_write(GPIO7,n)
#define FT_RST(n)  gpio_output(GPIO7,n)
//#define FT_INT     djy_gpio_read(GPIO6)
#define FT_INT     gpio_input(GPIO6)
/*************************************************************************************/
enum EasyKeyValue
{
    NO_KEY = 0,
    PAUSE_PLAY_KEY,
    VOL_UP_KEY,
    VOL_DOWN_KEY,
    COMEBACK_KEY,
    POWER_KEY,
    HEADSET_STATE,
} ;

enum SpeakerState
{
    Speaker_off,
    Speaker_on,
};

void Board_Init(void);
void FT6236_Pin_Init(void);
u32 IIC_IoCtrlFunc(enum IIc_Io IO,u32 tag);
void OpenScreen();
void CloseScreen();
void ClosePower();
void CloseSpeaker();
void OpenSpeaker();
enum SpeakerState GetSpeakerState();
#ifdef __cplusplus
}
#endif
#endif






