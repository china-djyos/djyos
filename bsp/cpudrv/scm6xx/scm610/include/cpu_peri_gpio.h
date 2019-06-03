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
//����ģ��:IO��ʼ��
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:IO��ʼ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-03-10
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __cpu_peri_gpio_h_
#define __cpu_peri_gpio_h_

#ifdef __cplusplus
extern "C" {
#endif

enum EN_GPIO_MODE
{
    EN_INPUT_MODE = 0,   //����Ϊ����ģʽ
    EN_OUTPUT_MODE,      //����Ϊ���ģʽ
};

enum EN_GPIO_LEV
{
    EN_GPIO_LEV_L,       //�������Ϊ�͵�ƽ
    EN_GPIO_LEV_H,       //�������Ϊ�ߵ�ƽ
};

//�˿ڶ�ȡ��ƽʧ��

#define CN_RORT_READ_FAILED (-1)

enum EN_GPIO_MASK
{
    EN_GPIO_UMASK = 0,
    EN_GPIO_MASK,
};



enum EN_GPIO_INT_MODE
{
    EN_MODE_BEGIN = 0,
    EN_MODE_HIGHT_LEVEL = EN_MODE_BEGIN,  //�ߵ�ƽ����
    EN_MODE_LOW_LEVEL,                    //�͵�ƽ����
    EN_MODE_RISINGEDGE,                   //�����ش���
    EN_MODE_FALLINGEDGE,                  //�½��ش���
    EN_MODE_BOTHEDGE,                     //˫���ش���
    EN_MODE_END = EN_MODE_BOTHEDGE,
};


void GPIO_CfgPinFunc(u32 port,u32 Pinx,u32 Mode,
                        u32 OutType,u32 OutSpeed,u32 PUPD);
u32 GPIO_AFSet(u32 port,u32 pinnum,u32 af_no);
u32 GPIO_GetData(u32 port);
void GPIO_OutData(u32 port,u32 data);
void GPIO_SettoHigh(u32 port,u32 msk);
void GPIO_SettoLow(u32 port,u32 msk);
void GPIO_PowerOn(u32 port);
void GPIO_PowerOff(u32 port);

bool_t Gpio_DirConfig(const u8 Mode,const u8 PortNum);
char Gpio_ReadLevel(u8 PortNum);
bool_t Gpio_ExIntEnable(u8 PortNum);
bool_t Gpio_ExIntDisable(u8 PortNum);
bool_t Gpio_IntMask(u8 PortNum,u8 MaskCode);
bool_t Gpio_ClearIntFlag(u8 PortNum);
bool_t Gpio_IntModeConfig(u8 PortNum,u8 IntMode);

#ifdef __cplusplus
}
#endif

#endif /*__cpu_peri_gpio_h_*/
