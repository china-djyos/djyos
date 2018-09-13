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
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri_gpio.h"
#include "cpu_peri_iic.h"
#include "cpu_peri_uart.h"
#include "uartctrl.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern void Board_GpioInit(void);
//    Board_GpioInit();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"stm32f3_demo_board"                           //�����������
//parent:"none"                                                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp���                                             //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                                                   //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//grade:init                                                    //��ʼ��ʱ������ѡֵ��none��init��main��none��ʾ�����ʼ����
                                                                //init��ʾ�ڵ���main֮ǰ��main��ʾ��main�����г�ʼ��
//dependence:"cpu_peri_gpio","cpu_peri_iic","cpu_peri_uart"     //������������������������none����ʾ�������������
                                                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                                                //����������������������г����á�,���ָ�
//weakdependence:"none"                                         //��������������������������none����ʾ�������������
                                                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                                                //����������������������г����á�,���ָ�
//mutex:"none"                                                  //������������������������none����ʾ�������������
                                                                //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
//%$#@end configue  ****�������ý���
//@#$%component end configure


//�����gpioֻ������һ���� ���õ���ͬ��GPIO��������ֲ����

//485-1   PA9(TX),PA10(RX),PA12(RTS)
static const Pin uart1_pin[] = {


};

//485-2   PD5(TX),PD6(RX),PD4(RTS)
static const Pin uart2_pin[] = {


};

//485-3   PB10(RX),PB11(TX),PB14(RTS)
static const Pin uart3_pin[] = {


};
//DBG  UART5
static const Pin uart5_pin[] = {


};

//IIC-1   PB8(SCL),PB9(SDA)
static const Pin iic1_pin[] = {

 };

//SPI-1
static const Pin spi1_pin[]={

};

//SPI-2
static const Pin spi2_pin[]={

};


//SPI-3
static const Pin spi3_pin[]={

};


//MAX14001-COUT�ܽŲ���
static const Pin MAX14001_COUT[] =
{

};

// ============================================================================
// ���ܣ����ô��ڱ��ΪSerialNo�Ĵ���Ϊ��˫�����͹���
// ������SerialNo,���ں�
// ���أ���
// ============================================================================
void Board_UartHalfDuplexSend(u8 SerialNo)
{
    switch(SerialNo)
    {
    case CN_UART1:
//        GPIO_SettoHigh(GPIO_A,PIN12);
        break;
    case CN_UART2:
//        GPIO_SettoHigh(GPIO_D,PIN4);
        break;
    case CN_UART3:
//        GPIO_SettoHigh(GPIO_B,PIN14);
        break;
    default:        break;
    }
}

// ============================================================================
// ���ܣ����ô��ڱ��ΪSerialNo�Ĵ���Ϊ��˫�����չ���
// ������SerialNo,���ں�
// ���أ���
// ============================================================================
void Board_UartHalfDuplexRecv(u8 SerialNo)
{
//    ����Ϊ485����I
    switch(SerialNo)
    {
    case CN_UART1:
//        GPIO_SettoLow(GPIO_A,PIN12);
        break;
    case CN_UART2:
//        GPIO_SettoLow(GPIO_D,PIN4);
        break;
    case CN_UART3:
//        GPIO_SettoLow(GPIO_B,PIN14);
        break;
    default:        break;
    }
}

// =============================================================================
// ���ܣ�Board_GpioInit���������õ���GPIO��ʼ��
//    �����ｫ���п����õ��ĸ���ģʽ��gpio������������Ҫ��ȡ��ע�ͼ���
//    ���鲻Ҫɾ��
// ��������
// ���أ���
// =============================================================================
void Board_GpioInit(void)
{

}

//level = 0 ʹ��
#include "cpu_peri_spi.h"
bool_t Board_SpiCsCtrl(u8 SPIPort,u8 cs,u8 level)
{

    return true;
}
