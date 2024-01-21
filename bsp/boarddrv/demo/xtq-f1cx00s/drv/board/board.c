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


#include "stdint.h"
#include "stddef.h"
#include <int.h>
#include <shell.h>
#include "cpu_peri.h"
#include "board.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"board config"//�����
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:required               //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:                   //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_BOARD_CONFIG == false )
//#warning  " board_config  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_BOARD_CONFIG    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure



void OpenBackLight()
{
}

void CloseBackLight()
{
}

void OpenScreen()
{
}

void CloseScreen()
{
}

void CloseSpeaker()
{
}
void OpenSpeaker()
{
}

//===============================================================
// TP��λ���ſ��ƣ�RST��INT����ʱ����ϣ�����GT911��IIC��ַΪ0x5d
//  0=����RESETΪ�͵�ƽ��1=�ߵ�ƽ
//==============================================================
void LcdTp_Reset_OnOff(u8 onoff)
{
  if(onoff)
  {
      GPIO_SettoHigh(GPIOA,GPIO_Pin_3 );
  }
  else
  {
      GPIO_SettoLow(GPIOA,GPIO_Pin_3);
  }

}

//===============================================================
// TP INT���ſ��ƣ�RST��INT����ʱ����ϣ�����GT911��IIC��ַΪ0x5d
//  0=���� INTΪ�͵�ƽ��1=���裨CPU��INT���ű��趨ΪOD��
//==============================================================
void LcdTp_Int_OnOff(u8 onoff)
{
  if(onoff)
  {
      GPIO_SettoHigh(GPIOA,GPIO_Pin_1 );
  }
  else
  {
      GPIO_SettoLow(GPIOA,GPIO_Pin_1);
  }

}

//===============================================================
// TP INT������Ϊ����̬
//==============================================================
void LcdTp_Int_Input(void)
{
    GPIO_Congif(GPIOA, GPIO_Pin_1, GPIO_Mode_IN,  GPIO_PuPd_NOPULL);   //int
}

u32 IIC_IoCtrlFunc(enum IIc_Io IO,u32 tag)
{
    switch(tag)
    {
        case 1 :
            switch(IO)
            {
            case scl_set_High : CT_IIC_SCL(1); break;
            case scl_set_Low  : CT_IIC_SCL(0); break;
            case scl_set_out  : CT_SCL_OUT();  break;
            case sda_set_High : CT_IIC_SDA(1); break;
            case sda_get      : return CT_READ_SDA;
            case sda_set_Low  : CT_IIC_SDA(0); break;
            case sda_set_out  : CT_SDA_OUT();  break;
            case sda_set_in   : CT_SDA_IN();   break;
            default:
                break;
            }
        break;
    }
    return 0;
}

#define SPI1_PA_EN  0
void Board_Init(void)
{
    u32 i=0;
    //uart PIN��ʼ��
    GPIO_Congif(GPIOE,GPIO_Pin_0,GPIO_Mode_101,GPIO_PuPd_NOPULL);//RX0
    GPIO_Congif(GPIOE,GPIO_Pin_1,GPIO_Mode_101,GPIO_PuPd_NOPULL);//TX0
//    GPIO_Congif(GPIOA,GPIO_Pin_2,GPIO_Mode_101,GPIO_PuPd_NOPULL);//RX1
//    GPIO_Congif(GPIOA,GPIO_Pin_3,GPIO_Mode_101,GPIO_PuPd_NOPULL);//TX1
    GPIO_Congif(GPIOE,GPIO_Pin_7,GPIO_Mode_011,GPIO_PuPd_NOPULL);//TX2
    GPIO_Congif(GPIOE,GPIO_Pin_8,GPIO_Mode_011,GPIO_PuPd_NOPULL);//RX2

    GPIO_Congif(GPIOA, GPIO_Pin_1, GPIO_Mode_OUT, GPIO_PuPd_UP);   //rst
    GPIO_SettoHigh(GPIOA,GPIO_Pin_1 );

    //iic0 PIN��ʼ��,ioģ��IIC
    GPIO_Congif(GPIOA, GPIO_Pin_0, GPIO_Mode_OUT, GPIO_PuPd_UP);   //scl
    GPIO_Congif(GPIOA, GPIO_Pin_1, GPIO_Mode_OUT,  GPIO_PuPd_NOPULL);   //int
    GPIO_Congif(GPIOA, GPIO_Pin_2, GPIO_Mode_OUT, GPIO_PuPd_UP);   //sda
    GPIO_Congif(GPIOA, GPIO_Pin_3, GPIO_Mode_OUT, GPIO_PuPd_UP);   //rst

    //������SPI PIN��ʼ��
    for(i=0;i<4;i++)    //SPI0
    {
        GPIO_Congif(GPIOC,GPIO_Pin_0+i,GPIO_Mode_010,GPIO_PuPd_NOPULL);
        GPIO_Multi_Driving(GPIOC,GPIO_Pin_0+i,3);//����������
    }
    for(i=0;i<4;i++)    //SPI1
    {
#if SPI1_PA_EN == 1      //SPI1ʹ��PA0~3
        GPIO_Congif(GPIOA,GPIO_Pin_0+i,GPIO_Mode_110,GPIO_PuPd_NOPULL);
        GPIO_Multi_Driving(GPIOA,GPIO_Pin_0+i,3);//����������
#else                       //SPI1ʹ��PE7~10
        GPIO_Congif(GPIOE,GPIO_Pin_7+i,GPIO_Mode_100,GPIO_PuPd_NOPULL);
        GPIO_Multi_Driving(GPIOE,GPIO_Pin_7+i,3);//����������
#endif
    }
}

void Init_Cpu(void);
void SoftReset(void)
{
    Init_Cpu();
}
