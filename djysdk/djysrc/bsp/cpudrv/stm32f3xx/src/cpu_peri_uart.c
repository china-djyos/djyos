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
// Copyright (c) 2014 ����Ȩ�����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��cpu_peri_uart.c
// ģ������: DJYOS����ģ��ĵײ��������֣���Ҫʵ�ּĴ�������Ĳ��������жϵ�
// ģ��汾: V1.10
// ������Ա:
// ����ʱ��:
// =============================================================================
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "errno.h"
#include "systime.h"
#include "cpu_peri.h"
#include <driver/include/uart.h>
#include "int.h"
#include "djyos.h"
#include "cpu_peri_uart.h"
#include "cpu_peri_int_line.h"
//#include "cpu_peri_dma.h"
#include "stm32f303xc.h"

extern void Board_UartHalfDuplexSend(u8 SerialNo);
extern void Board_UartHalfDuplexRecv(u8 SerialNo);

extern const char *gc_pCfgStdinName;    //��׼�����豸
extern const char *gc_pCfgStdoutName;   //��׼����豸
//extern const char *gc_pCfgStderrName;   //��׼��������豸
// =============================================================================
#define CN_UART1_BASE USART1_BASE
#define CN_UART2_BASE USART2_BASE
#define CN_UART3_BASE USART3_BASE
#define CN_UART4_BASE UART4_BASE
#define CN_UART5_BASE UART5_BASE


static u32 TxByteTime;                    //��������һ���ֽ�����Ҫ��ʱ��
static tagUartReg *PutStrDirectReg;       //����printk���͵Ĵ��ڼĴ���
static tagUartReg *GetCharDirectReg;      //����ֱ�ӽ��յĴ��ڼĴ���
static u32 TxDirectPort;                  //����printk���͵Ĵ��ں�
static u32 RxDirectPort;                  //����ֱ�ӽ��յĴ��ں�
// =============================================================================
static tagUartReg volatile * const tg_UART_Reg[] = {(tagUartReg *)CN_UART1_BASE,
                                                    (tagUartReg *)CN_UART2_BASE,
                                                    (tagUartReg *)CN_UART3_BASE,
                                                    (tagUartReg *)CN_UART4_BASE,
                                                    (tagUartReg *)CN_UART5_BASE,
                                                    };
#define CN_DMA_UNUSED   0
#define CN_DMA_USED     1

// UART��Ӧ��DMA Channel�жϺ�,F3û��Stream�ж�
static u8 const UartDmaRxInt[] = {CN_INT_LINE_DMA1_Channel5,CN_INT_LINE_DMA1_Channel6,
                                  CN_INT_LINE_DMA1_Channel3,CN_INT_LINE_DMA2_Channel3,
                                  };

static u8 const UartDmaTxInt[] = {CN_INT_LINE_DMA1_Channel4,CN_INT_LINE_DMA1_Channel7,
                                  CN_INT_LINE_DMA1_Channel2,CN_INT_LINE_DMA2_Channel5,
                                 };

static DMA_Channel_TypeDef * const UartDmaRxChannel[] =
                                 {DMA1_Channel5,DMA1_Channel6,
                                 DMA1_Channel3,DMA2_Channel3,};

static DMA_Channel_TypeDef * const UartDmaTxChannel[] =
                                 {DMA1_Channel4,DMA1_Channel7,
                                  DMA1_Channel2,DMA2_Channel5,};

// DMA����ʹ�ñ�ǣ��Ƿ�ʹ��DMA���,F3 DMA û��UART5
static bool_t s_UART_DmaSending[] = {false,false,false,false};
static bool_t s_UART_DmaUsed[]    = {false,false,false,false};

#define UART1_SendBufLen            64
#define UART1_RecvBufLen            64
#define UART1_DmaBufLen             100
static u8  UART1_DmaSendBuf[UART1_DmaBufLen];
static u8  UART1_DmaRecvBuf[UART1_DmaBufLen];

#define UART2_SendBufLen            64
#define UART2_RecvBufLen            64
#define UART2_DmaBufLen             100
static u8  UART2_DmaSendBuf[UART2_DmaBufLen];
static u8  UART2_DmaRecvBuf[UART2_DmaBufLen];

#define UART3_SendBufLen            32
#define UART3_RecvBufLen            32
#define UART3_DmaBufLen             100
static u8  UART3_DmaSendBuf[UART3_DmaBufLen];
static u8  UART3_DmaRecvBuf[UART3_DmaBufLen];

#define UART4_SendBufLen            32
#define UART4_RecvBufLen            32
#define UART4_DmaBufLen             100
static u8  UART4_DmaSendBuf[UART4_DmaBufLen];
static u8  UART4_DmaRecvBuf[UART4_DmaBufLen];


static struct UartCB *pUartCB[CN_UART_NUM];

//���ڱ�ʶ�����Ƿ��ʼ����ǣ���0λ��ʾUART0����һλ��UART1....
//�������ƣ�1��ʾ��ʼ����0��ʾδ��ʼ��
static u8 sUartInited = 0;

// =============================================================================
static ptu32_t UART_ISR(ptu32_t IniLine);

uint32_t UART_DmaRx_ISR(ptu32_t IntLine);
uint32_t UART_DmaTx_ISR(ptu32_t IntLine);

// =============================================================================
// ����: ��ֹuart�Ľ����жϡ�
// ����: flag,�Ƿ�ʹ��dma����
//       port,���ںţ�1~8
// ����: ��
// =============================================================================
void __UART_RxIntEnable(u8 flag,u8 port)
{
    if(flag == CN_DMA_USED)
        Int_RestoreAsynLine(UartDmaRxInt[port]);
    else
        tg_UART_Reg[port]->CR1 |= (1<<5);//�ж�ʹ��
}

// =============================================================================
// ����: ��ֹuart�Ľ����ж�
// ����: reg,�������ļĴ�����ָ��
//       port,���ں�
// ����: ��
// =============================================================================
void __UART_RxIntDisable(u8 flag, u8 port)
{
    if(flag == CN_DMA_USED)
        Int_SaveAsynLine(UartDmaRxInt[port]);
    else
        tg_UART_Reg[port]->CR1 &= ~(1<<5);
}

// =============================================================================
// ����: ʹ��uart�ķ����ж�
// ����: reg,�������ļĴ�����ָ��
//       port,���ں�
// ����: ��
// =============================================================================
static void __UART_TxIntEnable(u8 flag,u8 port)
{
    if(flag == CN_DMA_USED)
        Int_RestoreAsynLine(UartDmaTxInt[port]);
    else
        tg_UART_Reg[port]->CR1 |= (1<<7);
}

// =============================================================================
// ����: ��ֹUART�ķ����ж�
// ����: Reg,�������ļĴ�����ָ��
//       port,���ں�
// ����: ��
// =============================================================================
static void __UART_TxIntDisable(u8 flag, u8 port)
{
    if(flag == CN_DMA_USED)
        Int_SaveAsynLine(UartDmaTxInt[port]);
    else
        tg_UART_Reg[port]->CR1 &= ~(1<<7);//��ֹ���Ϳ��ж�
}

// =============================================================================
// ����: ��鷢�ͼĴ�����״̬��
// ������reg,�������ļĴ�����ָ��
// ����: true = �������գ�false = �ǿ�
// =============================================================================
static bool_t __UART_TxTranEmpty(tagUartReg volatile *reg)
{
    return ((reg->ISR & (1<<7)) !=0);
}

// =============================================================================
// ����: �����ռĴ�����״̬�������ݾͷ���true�����򷵻�false
// ������reg,�������ļĴ�����ָ��
// ����: true = �����ݣ�false = ������
// =============================================================================
static bool_t __UART_RxHadChar(tagUartReg volatile *reg)
{
    return ((reg->ISR & (1<<5)) !=0);
}

// =============================================================================
// ����: ���ô���baud,PCLKΪ25M��CPU��ƵΪ216M����,������ֻ�ṩ9600��19200��57600��
//       115200���ֲ���������
// ����: Reg,�������ļĴ�����ָ��
//       port,���ں�
//       baud,������
// ����: ��
// =============================================================================
static void __UART_BaudSet(tagUartReg volatile *Reg,u32 port,u32 baud)
{
    u32 mantissa,fraction;
    float temp;

    Reg->CR1=0x2c;     //8������������
    Reg->CR1=0x802c;
    Reg->CR1=0x802d;

    if(port == CN_UART1) //UART1ʱ��ԴΪPCLK2,����72M
    {
        temp = (float)2*CN_CFG_PCLK2/(16*baud);
        mantissa = temp;
        fraction = (temp - mantissa)*16;
    }
    else
    {
        temp = (float)  2*CN_CFG_PCLK1/(16*baud);  //UART2 3 4 5ʱ��ԴΪPCLK1,����36M
        mantissa = temp;
        fraction = (temp - mantissa)*16;
    }

    Reg->BRR =  ((u16)mantissa<<4)+((u16)fraction>>1);
//      Reg->BRR =  ((u16)mantissa<<4)+(u16)fraction;

    if(TxDirectPort == port)
    {
        TxByteTime = 11000000/baud;  //1���ֽڴ���ʱ�䣬��10bit��+10%����
    }
}

// =============================================================================
// ����: ���ö�ӦUART��IO�ڣ�����ʱ�Ӻ�IO����
// ����: SerialNo,���ں�
// ע :�򴮿ڸ������Ų�ȷ��������Ҫ��ʼ����Ҫ������
// ����: ��
// =============================================================================
static void __UART_GpioConfig(u8 SerialNo)
{

    switch(SerialNo)
    {
    case CN_UART1:
        RCC->APB2ENR |=(1<<14); //����ʱ��ʹ��
        break;
    case CN_UART2:
        RCC->APB1ENR |=(1<<17);
        break;
    case CN_UART3:
        RCC->APB1ENR |=(1<<18);

        break;
    case CN_UART4:
        RCC->APB1ENR |=(1<<19);
        break;
    case CN_UART5:
        RCC->APB1ENR |=(1<<20);
        break;
    default: break;

    }

}

// =============================================================================
// ����: �Դ��ڴ���������ã����������ʡ���żУ�顢����λ��ֹͣλ
// ����:  Reg,�������ļĴ�����ָ��
//        port,���ں�
//        data,�ṹ��tagCOMParam���͵�ָ����ֵ
// ����: ��
// =============================================================================
static void __UART_ComConfig(tagUartReg volatile *Reg,u32 port,ptu32_t data)
{
    struct COMParam *COM;
    if((data == 0) || (Reg == NULL))
        return;
    COM = (struct COMParam *)data;
    __UART_BaudSet(Reg,port,COM->BaudRate);

    Reg->CR1 &= ~(1);//��ֹ����
    switch(COM->DataBits)               // data bits
    {
        case CN_UART_DATABITS_7:
            switch(COM->Parity)
            {
                case CN_UART_PARITY_NONE:
                    Reg->CR1 |= (1<<28);//����Ч�鹲7λ��ֹ��żЧ��
                    Reg->CR1 &= ~(1<<12);

                    Reg->CR1 &= ~(1<<10);
                    break;//��Ч��

                case CN_UART_PARITY_ODD:
                    Reg->CR1 &= ~(1<<28);//����Ч�鹲8λ��żЧ��
                    Reg->CR1 &= ~(1<<12);

                    Reg->CR1 |= (1<<9);
                    Reg->CR1 |= (1<<10);break;//��Ч��

                case CN_UART_PARITY_EVEN:
                    Reg->CR1 &= ~(1<<28);//����Ч�鹲8λ��żЧ��
                    Reg->CR1 &= ~(1<<12);

                    Reg->CR1 &=~(1<<9);
                    Reg->CR1 |= (1<<10);break;
                default:break;
            }
            break;

        case CN_UART_DATABITS_8:
            switch(COM->Parity)        //
            {
                case CN_UART_PARITY_NONE:
                    Reg->CR1 &= ~(1<<28);//����Ч�鹲8λ��ֹ��żЧ��
                    Reg->CR1 &= ~(1<<12);
                    Reg->CR1 &= ~(1<<10);
                    break;

                case CN_UART_PARITY_ODD:
                    Reg->CR1 &= ~(1<<28);//����Ч�鹲9λ��żЧ��
                    Reg->CR1 |= (1<<12);

                    Reg->CR1 |= (1<<9);
                    Reg->CR1 |= (1<<10);
                    break;//��Ч��
                case CN_UART_PARITY_EVEN:
                    Reg->CR1 &= ~(1<<28);//����Ч�鹲9λ��żЧ��
                    Reg->CR1 |= (1<<12);

                    Reg->CR1 &=~(1<<9);
                    Reg->CR1 |= (1<<10);break;
                default:break;
            }
        break;

        case CN_UART_DATABITS_9:
            Reg->CR1 &= ~(1<<28);
            Reg->CR1 |= (1<<12);
            switch(COM->Parity)        //
            {
                case CN_UART_PARITY_NONE:
                    Reg->CR1 &= ~(1<<28);//����Ч�鹲9λ��ֹ��żЧ��
                    Reg->CR1 |= (1<<12);
                    Reg->CR1 &= ~(1<<10);
                    break;
                case CN_UART_PARITY_ODD:break;//��֧�����������
                case CN_UART_PARITY_EVEN:break;
                default:break;
            }
            break;
        default:break;
    }

    switch(COM->StopBits)  //todu:F7�� 0.5��ֹͣλû��д����
    {
        Reg->CR2 &= ~(3<<12);
//      case CN_UART_STOPBITS_0_5;  Reg->CR2 |= (1<<12);break;
        case CN_UART_STOPBITS_1:    Reg->CR2 |= (0<<12);break;
        case CN_UART_STOPBITS_1_5:  Reg->CR2 |= (3<<12);break;
        case CN_UART_STOPBITS_2:    Reg->CR2 |= (2<<12);break;
        default:break;
    }
    Reg->CR1 |= (1);//ʹ�ܴ���
}

// =============================================================================
// ����: ʹ���ڿ�ʼ������ʱ�Ӻ�uartģ��
// ����: port,���ں�
// ����: ��
// =============================================================================
void __UART_Enable(u32 port)
{
    switch(port)
    {//ʹ�ܴ���1ʱ��
        case CN_UART1:
            RCC->APB2ENR |=(1<<14);break;
        case CN_UART2:
            RCC->APB1ENR |=(1<<17);break;
        case CN_UART3:
            RCC->APB1ENR |=(1<<18);break;
        case CN_UART4:
            RCC->APB1ENR |=(1<<19);break;
        case CN_UART5:
            RCC->APB1ENR |=(1<<20);break;
        default:
            break;
    }
    tg_UART_Reg[port]->CR1 |= (1<<13);


}

// =============================================================================
// ����: ʹ����ֹͣ����������ʱ�Ӻ�uartģ��
// ����: port,���ں�
// ����: ��
// =============================================================================
void __UART_Disable(u32 port)
{
    switch(port)
    {
        case CN_UART1:  RCC->APB2ENR &=~(1<<14);  break;
        case CN_UART2:  RCC->APB1ENR &=~(1<<17); break;
        case CN_UART3:  RCC->APB1ENR &=~(1<<18); break;
        case CN_UART4:  RCC->APB1ENR &=~(1<<19); break;
        case CN_UART5:  RCC->APB1ENR &=~(1<<20); break;
        default:break;
    }
    tg_UART_Reg[port]->CR1 &= ~(1);
}

// =============================================================================
// ����: ��ʼ��UART��Ӧ���ж��ߣ�����ʼ���ж���ں���
// ������SerialNo,���ں�
// ����: ��
// =============================================================================
static void __UART_IntInit(u32 SerialNo)
{
    u8 IntLine;
    switch (SerialNo)
    {
        case CN_UART1:
                    IntLine = CN_INT_LINE_USART1;break;
        case CN_UART2:
                    IntLine = CN_INT_LINE_USART2;break;
        case CN_UART3:
                    IntLine = CN_INT_LINE_USART3;break;
        case CN_UART4:
                    IntLine = CN_INT_LINE_UART4; break;
        case CN_UART5:
                    IntLine = CN_INT_LINE_UART5; break;
                default:
                    return;
    }
    Int_Register(IntLine);
    Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(IntLine,UART_ISR);
    Int_SettoAsynSignal(IntLine);
    Int_ClearLine(IntLine);
    Int_RestoreAsynLine(IntLine);
    Int_SetIsrPara(IntLine,CN_UART1);

}

// =============================================================================
// ����: Ӳ���������úͼĴ����ĳ�ʼ�������������ʡ�ֹͣλ��У��λ������λ��Ĭ�������:
//       ������:115200  �� ֹͣλ:1 ; У��:�� ; ����λ:8bit
// ����: SerialNo,���ںţ�0~3
// ����: ��
// =============================================================================
static void __UART_HardInit(u8 SerialNo)
{
    if(SerialNo > CN_UART_NUM)
        return;
    __UART_GpioConfig(SerialNo);  //ʱ��ʹ��
   //ϵͳ��ʼ��ʱ�Ѿ�ʹ�жϴ��ڽ�ֹ״̬�������ٽ�ֹ������жϡ�
   //��ʼ��uartӲ���������ݽṹ
    tg_UART_Reg[SerialNo]->CR1 = 0x002d;   //֮ǰΪ2d
    tg_UART_Reg[SerialNo]->CR2 = 0x0;
    tg_UART_Reg[SerialNo]->CR3 = 0x0000;
    __UART_BaudSet(tg_UART_Reg[SerialNo],SerialNo,115200);
    Djy_DelayUs(100);//
    tg_UART_Reg[SerialNo]->ICR|=(1<<4);

}

// =============================================================================
// ����: ��ѯ�ȴ����ڷ�����ϣ���ʱ����
// ����: sending��dma���ͷ�ʽ�������Ƿ��Ѿ��������
// ����: ��
// =============================================================================
bool_t __uart_dma_timeout(bool_t sending)
{
    u32 timeout = 1000;
    while((sending == true)&& (timeout > 0))//��ʱ
    {
        timeout--;
        Djy_DelayUs(1);
    }
    if(timeout == 0)
        return true;
    else
        return false;
}
// =============================================================================
// ����: ��������DMA���ͣ�ʹ��DMA�������ڷ���ǰ���ж��ϴ�DMA�Ƿ��Ѿ�������ɣ�ͨ��
//       �鿴��־UARTx_DMA_SENDING���ж�UARTx�Ƿ��Ѿ��������
// ����: PORT,���ں�.
// ����: ���͵ĸ���
// =============================================================================
u32 __UART_DMA_SendStart(u32 port)     //uart5û�ж�ӦDMA
{
    u32 num,addr;
    u8 *DmaSendBuf;
    u32 DmaSendBufLen;

    switch(port)
    {
        case CN_UART1:
            DmaSendBuf = UART1_DmaSendBuf;
            DmaSendBufLen = UART1_DmaBufLen;
            break;
        case CN_UART2:
            DmaSendBuf = UART2_DmaSendBuf;
            DmaSendBufLen = UART2_DmaBufLen;
            break;
        case CN_UART3:
            DmaSendBuf = UART3_DmaSendBuf;
            DmaSendBufLen = UART3_DmaBufLen;
            break;
        case CN_UART4:
            DmaSendBuf = UART4_DmaSendBuf;
            DmaSendBufLen = UART4_DmaBufLen;
            break;
         default:   break;
    }

    if(true == __uart_dma_timeout(s_UART_DmaSending[port]))
        return 0;
    num = UART_PortRead(pUartCB[port],DmaSendBuf,DmaSendBufLen,0);
    addr = (u32)DmaSendBuf;

    if(num > 0)
    {
        DMA_Enable(UartDmaTxChannel[port],addr,num); //����DMA
        s_UART_DmaSending[port] = true;
    }
    return 0;
}

// =============================================================================
// ����: �������ڷ��ͣ���Ŀ���Ǵ����жϣ����жϷ�ʽ�������ݡ�
// ����: Reg,�������Ĵ��ڼĴ���ָ��.
// ����: ���͵ĸ���
// =============================================================================
static u32 __UART_SendStart (tagUartReg *Reg,u32 timeout)
{
    u8 port;

    switch((u32)Reg)
    {
    case CN_UART1_BASE:        port = CN_UART1;    break;
    case CN_UART2_BASE:        port = CN_UART2;    break;
    case CN_UART3_BASE:        port = CN_UART3;    break;
    case CN_UART4_BASE:        port = CN_UART4;    break;
    case CN_UART5_BASE:        port = CN_UART5;    break;
    default:return 0;
    }

    __UART_TxIntDisable(s_UART_DmaUsed[port],port);
    if(s_UART_DmaUsed[port])
    {
        __UART_DMA_SendStart(port);
    }
    else
    {
        //��DMA��ʽ��ֻҪʹ�ܷ��Ϳ��жϣ��򴥷��ж�
    }

    __UART_TxIntEnable(s_UART_DmaUsed[port],port);
    return 1;
}

// =============================================================================
// ����: ����uartʹ��dma�շ�������stm32�����ڵ��շ�dmaͨ�����üĴ����������³�
//       ʼ������dma�Ĵ�������Ϊ:
//       1����ʹ��dma�ڼ�����������豸ʹ��dma���Ӷ����������á�
//       2����ʹuartʹ��dma�ڼ䣬Ҳ��������ģ����ϵ����⵼�������豸ʹ��dma.
// ����: port,�˿ںţ������ںţ���1~3
// ����: ��
// =============================================================================
void __UART_SetDmaUsed(u32 port)
{
    u8 *DmaRecvBuf,*DmaSendBuf;
    switch(port)
    {
    case CN_UART1:
        DmaRecvBuf = UART1_DmaRecvBuf;
        DmaSendBuf = UART1_DmaSendBuf;
        break;
    case CN_UART2:
        DmaRecvBuf = UART2_DmaRecvBuf;
        DmaSendBuf = UART2_DmaSendBuf;
        break;
    case CN_UART3:
        DmaRecvBuf = UART3_DmaRecvBuf;
        DmaSendBuf = UART3_DmaSendBuf;
        break;
    case CN_UART4:
        DmaRecvBuf = UART4_DmaRecvBuf;
        DmaSendBuf = UART4_DmaSendBuf;
        break;
    default :        break;
    }
    __UART_RxIntDisable(CN_DMA_UNUSED,port); //�����ж�ʧ��
    __UART_TxIntDisable(CN_DMA_UNUSED,port);
    tg_UART_Reg[port]->CR1 |= (1<<4); //enable idle int ���ڿ����ж�

    DMA_Config(UartDmaRxChannel[port],(u32)&(tg_UART_Reg[port]->RDR),
            (u32)DmaRecvBuf,DMA_DIR_P2M,DMA_DATABITS_8,DMA_DATABITS_8,100);

    DMA_Config(UartDmaTxChannel[port],(u32)&(tg_UART_Reg[port]->TDR),
            (u32)DmaSendBuf,DMA_DIR_M2P,DMA_DATABITS_8,DMA_DATABITS_8,100);

    tg_UART_Reg[port]->CR3 |= 0x00c0;  //���ô���ʹ��dma�շ�
    s_UART_DmaUsed[port] = CN_DMA_USED;


    Int_Register(UartDmaTxInt[port]);//ע�ᷢ��DMA�ж�
    Int_Register(UartDmaRxInt[port]);//ע�����DMA�ж�

    Int_SetClearType(UartDmaRxInt[port],CN_INT_CLEAR_AUTO);//�����ж�
    Int_IsrConnect(UartDmaRxInt[port],UART_DmaRx_ISR);

    Int_SetClearType(UartDmaTxInt[port],CN_INT_CLEAR_AUTO);
    Int_IsrConnect(UartDmaTxInt[port],UART_DmaTx_ISR);

    Int_SettoAsynSignal(UartDmaRxInt[port]);
    Int_SettoAsynSignal(UartDmaTxInt[port]);

    Int_RestoreAsynLine(UartDmaRxInt[port]);
    Int_RestoreAsynLine(UartDmaTxInt[port]);

    Int_ClearLine(UartDmaTxInt[port]);
    Int_ClearLine(UartDmaRxInt[port]);

//    DMA_Enable(UartDmaTxChannel[port],(u32)DmaSendBuf,0);  //����dmaͨ������,�Լ�����,�տ�ʼ��Ӧ�÷���,�˴����ӻᵼ���ܶ���һ�ֽ�����
    DMA_Enable(UartDmaRxChannel[port],(u32)DmaRecvBuf,100);//����dmaͨ������
}

//=============================================================================
// ����: ���ò�ʹ��DMA��UART��ʹ��DMA�շ���ÿ�շ�һ���ֽھͱ����ж�һ�Σ��������ڸ���
//      ����ͨ����ʹ�á�
// ����: port,�˿ںţ������ںţ�Ϊ1~3
// ����: ��
// =============================================================================
void __UART_SetDmaUnUsed(u32 port)
{
    u8 IntLine;
    if(port == CN_UART1)             IntLine = CN_INT_LINE_USART1;
    else if(port == CN_UART2)        IntLine = CN_INT_LINE_USART2;
    else if(port == CN_UART3)        IntLine = CN_INT_LINE_USART3;
    else if(port == CN_UART4)        IntLine = CN_INT_LINE_UART4;
    else if(port == CN_UART5)        IntLine = CN_INT_LINE_UART5;
    else
    return;

    tg_UART_Reg[port]->CR3 &= ~0x00c0;  //���ò�ʹ��dma����
    Int_RestoreAsynLine(IntLine);
    if(s_UART_DmaUsed[port] == CN_DMA_USED)
    {
        Int_SaveAsynLine(UartDmaRxInt[port]);
        Int_SaveAsynLine(UartDmaTxInt[port]);
        s_UART_DmaUsed[port] = CN_DMA_UNUSED;
    }
    __UART_RxIntEnable(CN_DMA_USED,port);
    __UART_TxIntEnable(CN_DMA_USED,port);
    tg_UART_Reg[port]->CR1 &= ~(1<<4); //disable idle int
}
// =============================================================================
// ����: �����豸�Ŀ��ƺ���,������Ӳ���Ĵ����������
// ����: Reg,�������Ĵ��ڼĴ���ָ��.
//       cmd,��������
//       data1,data2,������cmd����
// ����: ������.
// =============================================================================
static ptu32_t __UART_Ctrl(tagUartReg *Reg,u32 cmd, u32 data1,u32 data2)
{
    ptu32_t result = 0;
    u32 port;
    u32 timeout = 10000;
    if(Reg == NULL)
        return 0;

    switch((u32)Reg)
    {
    case CN_UART1_BASE: port = CN_UART1;break;
    case CN_UART2_BASE: port = CN_UART2;break;
    case CN_UART3_BASE: port = CN_UART3;break;
    case CN_UART4_BASE: port = CN_UART4;break;
    case CN_UART5_BASE: port = CN_UART5;break;
    default:return 0;
    }

    switch(cmd)
    {
        case CN_UART_START:
            __UART_Enable(port);
            break;
        case CN_UART_STOP:
            __UART_Disable(port);
            break;
        case CN_UART_HALF_DUPLEX_SEND: //��������
            Board_UartHalfDuplexSend(port);
            break;

        case CN_UART_HALF_DUPLEX_RECV: //��������
            while((false == __UART_TxTranEmpty(Reg))&& (timeout > 10))
            {
                timeout -=10;
                Djy_DelayUs(10);
            }
            Board_UartHalfDuplexRecv(port);
            break;

        case CN_UART_SET_BAUD:  //����Baud
             __UART_BaudSet(Reg,port, data1);
            break;
        case CN_UART_EN_RTS:
            Reg->CR3 |= 0x100;
            break;
        case CN_UART_DIS_RTS:
            Reg->CR3 &= ~0x100;
            break;
        case CN_UART_EN_CTS:
            Reg->CR3 |= 0x200;
            break;
        case CN_UART_DIS_CTS:
            Reg->CR3 &= ~0x200;
            break;
        case CN_UART_DMA_USED:
            __UART_SetDmaUsed(port);
            break;
        case CN_UART_DMA_UNUSED:
            __UART_SetDmaUnUsed(port);
            break;
        case CN_UART_COM_SET:
            __UART_ComConfig(Reg,port,data1);
            break;
        default: break;
    }
    return result;
}

// =============================================================================
// ����: ��uart2���ó�dma��ʽ�ǣ�dma1 ch7�����ͣ�ch6�������
// ����: �жϺ���û�в���.
// ����: �жϺ���û�з���ֵ.
// =============================================================================
uint32_t UART_DmaTx_ISR(ptu32_t tagIntLine)
{
    uint16_t num,port;
    u8 *DmaSendBuf;
    u32 DmaSendBufLen;

    for(port = CN_UART1; port <= CN_UART5;port++)
    {
        if(tagIntLine == UartDmaTxInt[port])
            break;
    }
    switch(port)
    {
    case CN_UART1:
        DmaSendBuf = UART1_DmaSendBuf;
        DmaSendBufLen = UART1_DmaBufLen;
        break;
    case CN_UART2:
        DmaSendBuf = UART2_DmaSendBuf;
        DmaSendBufLen = UART2_DmaBufLen;
        break;
    case CN_UART3:
        DmaSendBuf = UART3_DmaSendBuf;
        DmaSendBufLen = UART3_DmaBufLen;
        break;
    case CN_UART4:
        DmaSendBuf = UART4_DmaSendBuf;
        DmaSendBufLen = UART4_DmaBufLen;
        break;
    default:
        break;
    }

    DMA_ClearIntFlag(UartDmaTxChannel[port]);
    num = UART_PortRead(pUartCB[port],DmaSendBuf,DmaSendBufLen,0);
    if(num == 0)
    {
        s_UART_DmaSending[port] = false;
    }
    else
    {
        DMA_Enable(UartDmaTxChannel[port],(u32)DmaSendBuf,num);
    }
    return 0;
}

// =============================================================================
// ����: ��uart1���ó�dma��ʽ�ǣ�dma2 ch2�����ͣ�ch7�������
// ����: �жϺ���û�в���.
// ����: �жϺ���û�з���ֵ.
// =============================================================================
uint32_t UART_DmaRx_ISR(ptu32_t tagIntLine)
{
    u32 recvs,num,DmaRecvBufLen;
    u8 *DmaRecvBuf;
    u8 port;
    for(port = CN_UART1; port <= CN_UART5;port++)
    {
        if(tagIntLine == UartDmaRxInt[port])
            break;
    }
    switch(port)
    {
    case CN_UART1:
        DmaRecvBuf = UART1_DmaRecvBuf;
        DmaRecvBufLen = UART1_DmaBufLen;
        break;
    case CN_UART2:
        DmaRecvBuf = UART2_DmaRecvBuf;
        DmaRecvBufLen = UART2_DmaBufLen;
        break;
    case CN_UART3:
        DmaRecvBuf = UART3_DmaRecvBuf;
        DmaRecvBufLen = UART3_DmaBufLen;
        break;
    case CN_UART4:
        DmaRecvBuf = UART4_DmaRecvBuf;
        DmaRecvBufLen = UART4_DmaBufLen;
        break;
    default:
        break;
    }

    DMA_Disable(UartDmaRxChannel[port]);
    DMA_ClearIntFlag(UartDmaRxChannel[port]);
    recvs = DmaRecvBufLen - UartDmaRxChannel[port]->CNDTR;
    if(recvs == DmaRecvBufLen)     //dma�������������������ݶ�ʧ
    {
        UART_ErrHandle(pUartCB[port],CN_UART_FIFO_OVER_ERR);
    }else
    {
        num = UART_PortWrite(pUartCB[port],DmaRecvBuf,recvs,0);
        if(num != recvs)
        {
            UART_ErrHandle(pUartCB[port],CN_UART_BUF_OVER_ERR);
        }
    }

    DMA_Enable(UartDmaRxChannel[port],(u32)DmaRecvBuf,DmaRecvBufLen);
    return 0;
}

// =============================================================================
// ����: UART���жϷ��������������Ϳա����ա����������жϡ�
//      1.�����жϣ��ӽ���FIFO�ж����ݣ�������UART_PortWriteд����ջ������У�
//      2.�����жϣ�����UART_PortRead�����ͻ����������ݣ���д�뷢��FIFO��
//      3.�����жϣ����������¼�
// ����: �жϺ�.
// ����: 0.
// =============================================================================
u32 UART_ISR(ptu32_t IntLine)
{
    struct UartCB *UCB;
    tagUartReg *Reg;
    u32 num,port;
    u8 ch;

    switch(IntLine)
    {
        case CN_INT_LINE_USART1:   port = CN_UART1;   break;
        case CN_INT_LINE_USART2:   port = CN_UART2;   break;
        case CN_INT_LINE_USART3:   port = CN_UART3;   break;
        case CN_INT_LINE_UART4:    port = CN_UART4;   break;
        case CN_INT_LINE_UART5:    port = CN_UART5;   break;
        default:return 0;
    }

    UCB = pUartCB[port];
    Reg = (tagUartReg *)tg_UART_Reg[port];
    if( (Reg->ISR & (1<<5)) &&(Reg->CR1 &(1<<5)))   //�����ж�
    {
        ch = (u8)Reg->RDR;
        num = UART_PortWrite(UCB,&ch,1,0);
        if(num != 1)
        {
            UART_ErrHandle(UCB,CN_UART_BUF_OVER_ERR);
        }
    }
    if((Reg->ISR & (1<<6)) &&(Reg->CR1 & (1<<7)))    //�����ж�
    {
        num = UART_PortRead(UCB,&ch,1,0);
        if(num != 0)
            Reg->TDR = ch;
        else
        {
            Reg->CR1 &= ~(1<<7); //txeie
        }
    }
    if(Reg->ISR & (1<<3))   //ORE���ش���
    {
        ch = (u8)Reg->RDR;
        num = Reg->ISR;     //��һ��sr�Ĵ���
        UART_ErrHandle(UCB,CN_UART_FIFO_OVER_ERR);
        Reg->ICR |=(1<<3);//����ر�־λ
    }
    if(Reg->ISR & (1<<4))//IDLE
    {
        Reg->ICR |=(1<<4);
        Int_TapLine(UartDmaRxInt[port]);
    }
    return 0;
}

// =============================================================================
// ���ܣ���ʼ��uart1ģ��,��װ�ش����豸�������uart.c��API�������ú�����Ҫ��ɵĹ���
//       ���£�
//       1.��ʼ�����ڵ�Ӳ�����֣���GPIO���жϡ��������õ�COM����������baud = 115200
//         stop bits = 1,����żУ��, data bits = 8
//       2.����uart.c��API����UART_InstallPort��װ�봮���豸
// ���������ںţ�0~3
// ���أ�1=�ɹ���0=ʧ��
// =============================================================================
ptu32_t ModuleInstall_UART(u32 serial_no)
{
    struct UartParam UART_Param;


    switch(serial_no)
    {
    case CN_UART1://����1
        UART_Param.Name         = "UART1";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART1_BASE;
        UART_Param.TxRingBufLen = UART1_SendBufLen;
        UART_Param.RxRingBufLen = UART1_RecvBufLen;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART2://����2
        UART_Param.Name         = "UART2";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART2_BASE;
        UART_Param.TxRingBufLen = UART2_SendBufLen;
        UART_Param.RxRingBufLen = UART2_RecvBufLen;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
        break;
    case CN_UART3://����3
        UART_Param.Name         = "UART3";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART3_BASE;
        UART_Param.TxRingBufLen = UART3_SendBufLen;
        UART_Param.RxRingBufLen = UART3_RecvBufLen;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
        break;
    case CN_UART4://����4
        UART_Param.Name         = "UART4";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART4_BASE;
        UART_Param.TxRingBufLen = UART4_SendBufLen;
        UART_Param.RxRingBufLen = UART4_RecvBufLen;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    default:
        return 0;
    }

    //Ӳ����ʼ��
    __UART_HardInit(serial_no);
    __UART_IntInit(serial_no);



    sUartInited |= (0x01 << serial_no);
    pUartCB[serial_no] = UART_InstallPort(&UART_Param);
    if( pUartCB[serial_no] == NULL)
        return 0;
    else
        return 1;
}

// =============================================================================
// ���ܣ��ַ��ն�ֱ�ӷ��ͺ�����������ѯ��ʽ��ֱ��д�Ĵ���������printk������stdout
//       û�г�ʼ��
// ������str�������ַ���ָ��
//      len�����͵��ֽ���
// ���أ�0����������result,�������ݳ��ȣ��ֽڵ�λ
// =============================================================================
s32 Uart_PutStrDirect(const char *str,u32 len)
{
    u32 result = 0,timeout = TxByteTime * len;
    u16 CR_Bak;

    CR_Bak = PutStrDirectReg->CR1;  //Save INT
    PutStrDirectReg->CR1 &= ~(1<<7);   //disable send INT
    for(result=0; result < len+1; result ++)
    {
        // ��ʱ���߷��ͻ���Ϊ��ʱ�˳�
        while((false == __UART_TxTranEmpty(PutStrDirectReg))&& (timeout > 10))
        {
            timeout -=10;
            Djy_DelayUs(10);
        }
        if( (timeout <= 10) || (result == len))
            break;
        PutStrDirectReg->TDR = str[result];
    }
    PutStrDirectReg->CR1 = CR_Bak;  //restore send INT
    return result;
}

// =============================================================================
// ���ܣ��ַ��ն�ֱ�ӽ��պ�����������ѯ��ʽ��ֱ�Ӷ��Ĵ���������stdin��ʼ��ǰ
// ������str�������ַ���ָ��
//      len�����͵��ֽ���
// ���أ�0����������result,�������ݳ��ȣ��ֽڵ�λ
// =============================================================================
char Uart_GetCharDirect(void)
{
    u16 CR_Bak;
    char result;

    CR_Bak = GetCharDirectReg->CR1;  //Save INT
    GetCharDirectReg->CR1 &= ~(1<<7);  //disable send INT
    while(__UART_RxHadChar(GetCharDirectReg) == false) ;
    result = GetCharDirectReg->RDR;
    PutStrDirectReg->CR1 = CR_Bak;   //restore send INT
    return result;
}

//----��ʼ���ں˼�IO-----------------------------------------------------------
//���ܣ���ʼ���ں˼��������������runtime����ָ�롣
//��������
//���أ���
//-----------------------------------------------------------------------------
void Stdio_KnlInOutInit(u32 para)
{
    if(!strcmp(gc_pCfgStdoutName,"/dev/UART1"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART1_BASE;
        TxDirectPort = CN_UART1;
    }
    else if(!strcmp(gc_pCfgStdoutName,"/dev/UART2"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART2_BASE;
        TxDirectPort = CN_UART2;
    }
    else if(!strcmp(gc_pCfgStdoutName,"/dev/UART3"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART3_BASE;
        TxDirectPort = CN_UART3;
    }
    else if(!strcmp(gc_pCfgStdoutName,"/dev/UART4"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART4_BASE;
        TxDirectPort = CN_UART4;
    }
    else if(!strcmp(gc_pCfgStdoutName,"/dev/UART5"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART5_BASE;
        TxDirectPort = CN_UART5;
    }
    else
    {
        PutStrDirectReg = NULL ;
    }

    if(PutStrDirectReg != NULL)
    {
        __UART_HardInit(TxDirectPort);
        TxByteTime = 95;      //��ʼ��Ĭ��115200������һ��byte��87uS,+10%����
        PutStrDirect = Uart_PutStrDirect;
    }

    if(!strcmp(gc_pCfgStdinName,"/dev/UART1"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART1_BASE;
        RxDirectPort = CN_UART1;
    }
    else if(!strcmp(gc_pCfgStdinName,"/dev/UART2"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART2_BASE;
        RxDirectPort = CN_UART2;
    }
    else if(!strcmp(gc_pCfgStdinName,"/dev/UART3"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART3_BASE;
        RxDirectPort = CN_UART3;
    }
    else if(!strcmp(gc_pCfgStdinName,"/dev/UART4"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART4_BASE;
        RxDirectPort = CN_UART4;
    }
    else if(!strcmp(gc_pCfgStdinName,"/dev/UART5"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART5_BASE;
        RxDirectPort = CN_UART5;
    }
    else
    {
        GetCharDirectReg = NULL ;
    }
    if(GetCharDirectReg != NULL)
    {
        if(TxDirectPort != RxDirectPort)
            __UART_HardInit(RxDirectPort);
        GetCharDirect = Uart_GetCharDirect;
    }
    return;
}
