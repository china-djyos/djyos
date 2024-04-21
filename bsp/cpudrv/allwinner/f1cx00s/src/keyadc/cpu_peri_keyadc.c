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
//����ģ��:����
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: ɨ��Ӳ��������������ֻ��ֱ�Ӹ���������ǰֵ�����������
//����˵��:
//�޶���ʷ:
//2. ����:
//   ����:
//   �°汾��:
//   �޸�˵��:
//1. ����: 2024-02-22
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "cpu_peri_keyAdc.h"
#include "cpu_peri_gpio.h"
#include "cpu_peri_io.h"
#include "int.h"


//Key_ADC�жϣ���ʱ����ɨ������ѯ���жϿ������ڻ���,�ж��Ѿ����úã�Ҫʹ��ʱ��ֻ�����
//Int_RestoreAsynLine(CN_INT_LINE_KEYADC);���ɡ�
void KeyADC_ISR(ptu32_t intline)
{
  //���ж�
  int res=0;
  res=read32(KEYADC_INTS_REG);
  write32(KEYADC_INTS_REG,res);
  if(res&ADC0_KEYDOWN_PENDING)//����
  {

  }
  if(res&ADC0_KEYUP_PENDING)//̧��
  {

  }
  if(res&ADC0_DATA_PENDING)//����
  {

  }
}
/*����ADֵ*/
int Key_ADC_Read_Value(void)
{
  return (read32(KEYADC_DATA_REG)&0x3f);
}
/*KEY ADC ��ʼ��*/
void Init_Key_ADC(void)
{
    /*�任ģʽ*/
    C_Vlue(KEYADC_CTRL_REG,12,0x3);
    S_Vlue(KEYADC_CTRL_REG,12,ADC_NormalMode);
    /*ADC HOLD DIS*/
    //       C_BIT(KEYADC_CTRL_REG,6);
    /*����UP DOWN ֵ*/
    C_Vlue(KEYADC_CTRL_REG,4,0x3);
    S_Vlue(KEYADC_CTRL_REG,4,ADC_1_6_V);
    //
    Int_Register(CN_INT_LINE_KEYADC);
    Int_SetClearType(CN_INT_LINE_KEYADC,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(CN_INT_LINE_KEYADC,KeyADC_ISR);
    Int_SettoAsynSignal(CN_INT_LINE_KEYADC);
    Int_ClearLine(CN_INT_LINE_KEYADC);
//  Int_RestoreAsynLine(CN_INT_LINE_KEYADC);
    /*ʹ���ж� UP DOWN DATA*/
    S_BIT(KEYADC_INTC_REG,0);//�����ж�
    S_BIT(KEYADC_INTC_REG,1);//down�ж�
    S_BIT(KEYADC_INTC_REG,4);//up�ж�
    /*ʹ��ADC*/
    S_BIT(KEYADC_CTRL_REG,0);

}


