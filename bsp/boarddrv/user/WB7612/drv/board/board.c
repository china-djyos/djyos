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
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri_gpio.h"
#include "cpu_peri_iic.h"
#include "cpu_peri_uart.h"
#include "uartctrl.h"


//�����gpioֻ������һ���� ���õ���ͬ��GPIO��������ֲ����


//485-1   PA9(TX),PA10(RX),PA12(RTS)
static const Pin uart1_pin[] = {

        {GPIO_A,PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
        {GPIO_A,PIN9,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
        {GPIO_A,PIN12,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF7},
};

//485-2   PD5(TX),PD6(RX),PD4(RTS)
static const Pin uart2_pin[] = {

        {GPIO_D,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
        {GPIO_D,PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
        {GPIO_D,PIN4,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF7},
};

//485-3   PB10(RX),PB11(TX),PB14(RTS)
static const Pin uart3_pin[] = {

        {GPIO_B,PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
        {GPIO_B,PIN11,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
        {GPIO_B,PIN14,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF7},
};
//
static const Pin uart4_pin[] = {

        {GPIO_C,PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF5},
        {GPIO_C,PIN11,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF5},
};

//IIC-1   PB8(SCL),PB9(SDA)
static const Pin iic2_pin[] = {
        {GPIO_B,PIN8,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},
        {GPIO_B,PIN9,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},
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

     PIO_Configure(uart1_pin, PIO_LISTSIZE(uart1_pin));  //485-1
     PIO_Configure(uart2_pin, PIO_LISTSIZE(uart2_pin));  //485-2
     PIO_Configure(uart3_pin, PIO_LISTSIZE(uart3_pin));  //485-3
     PIO_Configure(uart4_pin, PIO_LISTSIZE(uart4_pin));  //debug

     PIO_Set(&uart1_pin[2]);
     PIO_Set(&uart2_pin[2]);
     PIO_Set(&uart3_pin[2]);

     IIC_Busfree(GPIO_B,PIN9,PIN8);  //iic1,����IIC_Busfree(u32 port,u32 sda_pin,u32 sck_pin)
     PIO_Configure(iic2_pin, PIO_LISTSIZE(iic2_pin));    //iic1


//=====================IIC GPIO��ʼ��===================================//
//����û��Ƭѡ��ʹ�����ŵ�IIC��������CPU��λ������IIC���Ĺ����о��п��ܷ������߱�ռ���磺qh_1��stmpe811
//     ��������������I/Oģ��ķ�ʽ���������ͷ�

//        PIO_Configure(iic1_pin, PIO_LISTSIZE(iic1_pin));   //IIC

//    PIO_Configure(iic2_pin, PIO_LISTSIZE(iic2_pin));      //IIC
//    PIO_Configure(Ft_pin, PIO_LISTSIZE(Ft_pin));      //IIC
//
////    IIC_Busfree(GPIO_H,PIN8,PIN7);
////    IIC_Busfree(GPIO_H,PIN12,PIN11);
////I/Oģ��IIC �������õ�
//    PIO_Configure(Key_pin,PIO_LISTSIZE(Key_pin));  //KEY
//    PIO_Configure(EthRmiiPins,PIO_LISTSIZE(EthRmiiPins));  //ETH RMII
//    PIO_Configure(FmcNandPins,PIO_LISTSIZE(FmcNandPins)); //nand flash
}

//bool_t Board_SpiCsCtrl(u8 SPIPort,u8 cs,u8 level)
//{
//    return true;
//}
