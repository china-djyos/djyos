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
// =============================================================================

// �ļ���     ��cpu_peri_uart.c
// ģ������: DJYOS����ģ��ĵײ��������֣���Ҫʵ�ּĴ�������Ĳ��������жϵ�
// ģ��汾: V1.10
// ������Ա: hm
// ����ʱ��: 14/08.2014
// =============================================================================
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "errno.h"
#include "systime.h"
#include "cpu_peri.h"
#include <device/djy_uart.h>
#include "int.h"
#include "djyos.h"
#include "cpu_peri_dma.h"
#include "stdlib.h"

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern ptu32_t ModuleInstall_UART(ptu32_t SerialNo);
//
//    #if CFG_UART1_ENABLE ==1
//    ModuleInstall_UART(CN_UART1);
//    #endif
//
//    #if CFG_UART2_ENABLE ==1
//    ModuleInstall_UART(CN_UART2);
//    #endif
//
//    #if CFG_UART3_ENABLE ==1
//    ModuleInstall_UART(CN_UART3);
//    #endif
//
//    #if CFG_UART4_ENABLE ==1
//    ModuleInstall_UART(CN_UART4);
//    #endif
//
//    #if CFG_UART5_ENABLE ==1
//    ModuleInstall_UART(CN_UART5);
//    #endif
//
//    #if CFG_UART6_ENABLE ==1
//    ModuleInstall_UART(CN_UART6);
//    #endif
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip uart"//CPU��uart��������
//parent:"uart device file"      //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"device file system","lock","uart device file","heap","cpu onchip dma"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_UART == false )
//#warning  " cpu_onchip_uart  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_UART    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,16,512,
#define CFG_UART1_SENDBUF_LEN            32                 //"UART1���ͻ��λ�������С",
#define CFG_UART1_RECVBUF_LEN            32                 //"UART1���ջ��λ�������С",
#define CFG_UART1_DMABUF_LEN             32                 //"UART1 DMA��������С",

#define CFG_UART2_SENDBUF_LEN            32                 //"UART2���ͻ��λ�������С",
#define CFG_UART2_RECVBUF_LEN            32                 //"UART2���ջ��λ�������С",
#define CFG_UART2_DMABUF_LEN             32                 //"UART2 DMA��������С",

#define CFG_UART3_SENDBUF_LEN            32                 //"UART3���ͻ��λ�������С",
#define CFG_UART3_RECVBUF_LEN            32                 //"UART3���ջ��λ�������С",
#define CFG_UART3_DMABUF_LEN             32                 //"UART3 DMA��������С",

#define CFG_UART4_SENDBUF_LEN            32                 //"UART4���ͻ��λ�������С",
#define CFG_UART4_RECVBUF_LEN            32                 //"UART4���ջ��λ�������С",
#define CFG_UART4_DMABUF_LEN             32                 //"UART4 DMA��������С",

#define CFG_UART5_SENDBUF_LEN            32                 //"UART5���ͻ��λ�������С",
#define CFG_UART5_RECVBUF_LEN            32                 //"UART5���ջ��λ�������С",
#define CFG_UART5_DMABUF_LEN             32                 //"UART5 DMA��������С",

#define CFG_UART6_SENDBUF_LEN            32                 //"UART6���ͻ��λ�������С",
#define CFG_UART6_RECVBUF_LEN            32                 //"UART6���ջ��λ�������С",
#define CFG_UART6_DMABUF_LEN             32                 //"UART6 DMA��������С",
//%$#@enum,true,false,
#define CFG_UART1_ENABLE                 true               //"�Ƿ�ʹ��UART1",
#define CFG_UART2_ENABLE                 false              //"�Ƿ�ʹ��UART2",
#define CFG_UART3_ENABLE                 false              //"�Ƿ�ʹ��UART3",
#define CFG_UART4_ENABLE                 false              //"�Ƿ�ʹ��UART4",
#define CFG_UART5_ENABLE                 false              //"�Ƿ�ʹ��UART5",
#define CFG_UART6_ENABLE                 false              //"�Ƿ�ʹ��UART6",
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

#define CN_UART1_BASE 0x40011000//0x40013800
#define CN_UART2_BASE 0x40004400
#define CN_UART3_BASE 0x40004800
#define CN_UART4_BASE 0x40004c00
#define CN_UART5_BASE 0x40005000
#define CN_UART6_BASE 0X40011400

static u32 TxByteTime;                    //��������һ���ֽ�����Ҫ��ʱ��
static tagUartReg *PutStrDirectReg;     //����printk���͵Ĵ��ڼĴ���
static tagUartReg *GetCharDirectReg;     //����ֱ�ӽ��յĴ��ڼĴ���
static u32 TxDirectPort;                  //����printk���͵Ĵ��ں�
static u32 RxDirectPort;                  //����ֱ�ӽ��յĴ��ں�
// =============================================================================
static tagUartReg volatile * const tg_UART_Reg[] = {(tagUartReg *)CN_UART1_BASE,
                                                    (tagUartReg *)CN_UART2_BASE,
                                                    (tagUartReg *)CN_UART3_BASE,
                                                    (tagUartReg *)CN_UART4_BASE,
                                                    (tagUartReg *)CN_UART5_BASE,
                                                    (tagUartReg *)CN_UART6_BASE};
#define CN_DMA_UNUSED   0
#define CN_DMA_USED     1

// UART��Ӧ��DMA Stream�жϺ�
static u8 const UartDmaRxInt[] = {CN_INT_LINE_DMA2_Stream2,CN_INT_LINE_DMA1_Stream5,
                            CN_INT_LINE_DMA1_Stream1,CN_INT_LINE_DMA1_Stream2,
                            CN_INT_LINE_DMA1_Stream0,CN_INT_LINE_DMA2_Stream1};
static u8 const UartDmaTxInt[] = {CN_INT_LINE_DMA2_Stream7,CN_INT_LINE_DMA1_Stream6,
                            CN_INT_LINE_DMA1_Stream3,CN_INT_LINE_DMA1_Stream4,
                            CN_INT_LINE_DMA1_Stream7,CN_INT_LINE_DMA2_Stream6};

static DMA_Stream_TypeDef * const UartDmaRxStream[] = {DMA2_Stream2,DMA1_Stream5,
                                                DMA1_Stream1,DMA1_Stream2,
                                                DMA1_Stream0,DMA2_Stream2};
static DMA_Stream_TypeDef * const UartDmaTxStream[] = {DMA2_Stream7,DMA1_Stream6,
                                                DMA1_Stream3,DMA1_Stream4,
                                                DMA1_Stream7,DMA2_Stream6};

static u8 const DMA_Tx_ch[] = {4,4,4,4,4,5};
static u8 const DMA_Rx_ch[] = {4,4,4,4,4,5};
// DMA����ʹ�ñ�ǣ��Ƿ�ʹ��DMA���
static bool_t s_UART_DmaSending[] = {false,false,false,false,false,false};
static bool_t s_UART_DmaUsed[] = {false,false,false,false,false,false};

//#define CFG_UART1_SENDBUF_LEN            32
//#define CFG_UART1_RECVBUF_LEN            32
//#define CFG_UART1_DMABUF_LEN             32
//
//#define CFG_UART2_SENDBUF_LEN            32
//#define CFG_UART2_RECVBUF_LEN            32
//#define CFG_UART2_DMABUF_LEN             32
//
//#define CFG_UART3_SENDBUF_LEN            32
//#define CFG_UART3_RECVBUF_LEN            32
//#define CFG_UART3_DMABUF_LEN             32
//
//#define CFG_UART4_SENDBUF_LEN            32
//#define CFG_UART4_RECVBUF_LEN            32
//#define CFG_UART4_DMABUF_LEN             32
//
//#define CFG_UART5_SENDBUF_LEN            32
//#define CFG_UART5_RECVBUF_LEN            32
//#define CFG_UART5_DMABUF_LEN             32
//
//#define CFG_UART6_SENDBUF_LEN            32
//#define CFG_UART6_RECVBUF_LEN            32
//#define CFG_UART6_DMABUF_LEN             32

static u8 *pUART_DmaSendBuf[CN_UART_NUM];
static u8 *pUART_DmaRecvBuf[CN_UART_NUM];

static struct UartGeneralCB *pUartCB[CN_UART_NUM];

//���ڱ�ʶ�����Ƿ��ʼ����ǣ���0λ��ʾUART0����һλ��UART1....
//�������ƣ�1��ʾ��ʼ����0��ʾδ��ʼ��
static u8 sUartInited = 0;
__attribute__((weak))  void Board_UartHalfDuplexSend(u8 SerialNo)
{
    return;
}
__attribute__((weak))  void Board_UartHalfDuplexRecv(u8 SerialNo)
{
    return ;
}
// =============================================================================
static ptu32_t UART_ISR(ptu32_t port);

static uint32_t UART_DmaRx_ISR(ptu32_t port);
static uint32_t UART_DmaTx_ISR(ptu32_t port);

// =============================================================================
// ����: ��ֹuart�ķ����жϡ�
// ����: flag,�Ƿ�ʹ��dma����
//       port,���ںţ�1~5
// ����: ��
// =============================================================================
void __UART_RxIntEnable(u8 flag,u8 port)
{
    if(flag == CN_DMA_USED)
        Int_RestoreAsynLine(UartDmaRxInt[port]);
    else
        tg_UART_Reg[port]->CR1 |= (1<<5);
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
// ����: ��鷢�ͼĴ�����״̬������վͷ���true����Ϊ�գ����򷵻�false
// ������reg,�������ļĴ�����ָ��
// ����: true = �������գ�false = �ǿ�
// =============================================================================
static bool_t __UART_TxTranEmpty(tagUartReg volatile *reg)
{
    return (0 != (reg->SR & (1<<7)));
}

// =============================================================================
// ����: �����ռĴ�����״̬�������ݾͷ���true�����򷵻�false
// ������reg,�������ļĴ�����ָ��
// ����: true = �����ݣ�false = ������
// =============================================================================
static bool_t __UART_RxHadChar(tagUartReg volatile *reg)
{
    return ((reg->SR & (1<<5)) !=0);
}

// =============================================================================
// ����: ���ô���baud,PCLKΪ25M��CPU��ƵΪ100M����,������ֻ�ṩ9600��19200��57600��
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
    if((port == CN_UART1) || (port == CN_UART6))
    {
        temp = (float)CN_CFG_PCLK2/(16*baud);
        mantissa = temp;
        fraction = (temp - mantissa)*16;
    }
    else
    {
        temp = (float)CN_CFG_PCLK1/(16*baud);
        mantissa = temp;
        fraction = (temp - mantissa)*16;
    }

    Reg->BRR =  ((u16)mantissa<<4)+(u16)fraction;

    if(TxDirectPort == port)
    {
        TxByteTime = 11000000/baud;     //1���ֽڴ���ʱ�䣬��10bit��+10%����
    }
}


// =============================================================================
// ����: �Դ��ڴ���������ã����������ʡ���żУ�顢����λ��ֹͣλ
// ����:  Reg,�������ļĴ�����ָ��
//        port,���ں�
//        data,�ṹ��tagCOMParam���͵�ָ����ֵ
// ����: ��
// =============================================================================
static void __UART_ComConfig(tagUartReg volatile *Reg,u32 port,struct COMParam *COM)
{
    if((COM == NULL) || (Reg == NULL))
        return;
    __UART_BaudSet(Reg,port,COM->BaudRate);

    switch(COM->DataBits)               // data bits
    {
    case CN_UART_DATABITS_8:    Reg->CR1 &= ~(1<<12);break;
    case CN_UART_DATABITS_9:    Reg->CR1 |= (1<<12);;break;
    default:break;
    }

    switch(COM->Parity)                 // parity
    {
    Reg->CR1 |= (1<<10);                // enable parity
    case CN_UART_PARITY_ODD:    Reg->CR1 |= (1<<9);break;
    case CN_UART_PARITY_EVEN:   Reg->CR1 &=~(1<<9);break;
    default:break;
    }

    switch(COM->StopBits)               // stop bits
    {
    Reg->CR2 &= ~(3<<12);
    case CN_UART_STOPBITS_1:    Reg->CR2 |= (0<<12);break;
    case CN_UART_STOPBITS_1_5:  Reg->CR2 |= (1<<12);break;
    case CN_UART_STOPBITS_2:    Reg->CR2 |= (2<<12);break;
    default:break;
    }
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
    case CN_UART1:      RCC->APB2ENR |= (1<<4);         break;
    case CN_UART2:      RCC->APB1ENR |= (1<<17);        break;
    case CN_UART3:      RCC->APB1ENR |= (1<<18);        break;
    case CN_UART4:      RCC->APB1ENR |= (1<<19);        break;
    case CN_UART5:      RCC->APB1ENR |= (1<<20);        break;
    case CN_UART6:      RCC->APB2ENR |= (1<<5);         break;
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
    case CN_UART1:      RCC->APB2ENR &= ~(1<<4);         break; //ʹ�ܴ���1ʱ��
    case CN_UART2:      RCC->APB1ENR &= ~(1<<17);        break;
    case CN_UART3:      RCC->APB1ENR &= ~(1<<18);        break;
    case CN_UART4:      RCC->APB1ENR &= ~(1<<19);        break;
    case CN_UART5:      RCC->APB1ENR &= ~(1<<20);        break;
    case CN_UART6:      RCC->APB2ENR &= ~(1<<5);         break;
        break;
    default:
        break;
    }
    tg_UART_Reg[port]->CR1 &= ~(1<<13);
}

// =============================================================================
// ����: ��ʼ��UART��Ӧ���ж��ߣ�����ʼ���ж���ں���
// ������SerialNo,���ں�
// ����: ��
// =============================================================================
static void __UART_IntInit(u32 SerialNo)
{
    u8 IntLine;
    if(SerialNo == CN_UART1)
        IntLine = CN_INT_LINE_USART1;
    else if(SerialNo == CN_UART2)
        IntLine = CN_INT_LINE_USART2;
    else if(SerialNo == CN_UART3)
        IntLine = CN_INT_LINE_USART3;
    else if(SerialNo == CN_UART4)
        IntLine = CN_INT_LINE_UART4;
    else if(SerialNo == CN_UART5)
        IntLine = CN_INT_LINE_UART5;
    else if(SerialNo == CN_UART6)
        IntLine = CN_INT_LINE_USART6;
    else
        return;

    Int_Register(IntLine);
    Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(IntLine,UART_ISR);
    Int_SettoAsynSignal(IntLine);
    Int_ClearLine(IntLine);
    Int_RestoreAsynLine(IntLine);
    Int_SetIsrPara(IntLine,SerialNo);
}

// =============================================================================
// ����: Ӳ���������úͼĴ����ĳ�ʼ�������������ʡ�ֹͣλ��У��λ������λ��Ĭ�������:
//       ������:115200  �� ֹͣλ:1 ; У��:żУ�� ; ����λ:8bit
// ����: SerialNo,���ںţ�0~3
// ����: ��
// =============================================================================
static void __UART_HardInit(u8 SerialNo)
{
    if(SerialNo > CN_UART6)
        return;
    //ϵͳ��ʼ��ʱ�Ѿ�ʹ�жϴ��ڽ�ֹ״̬�������ٽ�ֹ������жϡ�
   //��ʼ��uartӲ���������ݽṹ
    tg_UART_Reg[SerialNo]->CR1 = 0x20ac;
    tg_UART_Reg[SerialNo]->CR2 = 0x0;
    tg_UART_Reg[SerialNo]->CR3 = 0x0000;
    __UART_BaudSet(tg_UART_Reg[SerialNo],SerialNo,115200);
}

// =============================================================================
// ����: ��ѯ�ȴ����ڷ�����ϣ���ʱ����
// ����: sending��dma���ͷ�ʽ�������Ƿ��Ѿ��������
// ����: ��
// =============================================================================
bool_t __uart_dma_timeout(bool_t sending)
{
    s32 timeout = 1000;
    while((sending == true)&& (timeout > 0))//��ʱ
    {
        timeout--;
        DJY_DelayUs(1);
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
u32 __UART_DMA_SendStart(u32 port)
{
    u32 num,addr;
    u8 *DmaSendBuf;
    u32 DmaSendBufLen;

    switch(port)
    {
    case CN_UART1:
        DmaSendBufLen = CFG_UART1_DMABUF_LEN;        break;
    case CN_UART2:
        DmaSendBufLen = CFG_UART2_DMABUF_LEN;        break;
    case CN_UART3:
        DmaSendBufLen = CFG_UART3_DMABUF_LEN;        break;
    case CN_UART4:
        DmaSendBufLen = CFG_UART4_DMABUF_LEN;        break;
    case CN_UART5:
        DmaSendBufLen = CFG_UART5_DMABUF_LEN;        break;
    case CN_UART6:
        DmaSendBufLen = CFG_UART6_DMABUF_LEN;        break;
    default:
        break;
    }
    DmaSendBuf = pUART_DmaSendBuf[port];
//    if(true == __uart_dma_timeout(s_UART_DmaSending[port]))
    if(true == s_UART_DmaSending[port])
        return 0;
    num = UART_PortRead(pUartCB[port],DmaSendBuf,DmaSendBufLen);
    addr = (u32)DmaSendBuf;

    if(num > 0)
    {
        DMA_Enable(UartDmaTxStream[port],addr,num); //����DMA
        s_UART_DmaSending[port] = true;
    }
    else
    {
        tg_UART_Reg[port]->CR1 |= (1<<6);
    }
    return 0;
}
// =============================================================================
// ����: ֱ�ӷ��ͺ�����ʹ����ѯ��ʽ��
// ����: Reg,�Ĵ�����ַָ��.
//       send_buf,�������ݻ�����
//       len,���ݳ��ȣ��ֽ�
//       timeout,��ʱʱ�䣬us
// ����: �����ֽ���
// =============================================================================
//static u32 __UART_SendDirectly(tagUartReg *Reg,u8 *send_buf,u32 len,u32 timeout)
//{
//    u32  result = 0;
//
//    if(Reg == NULL)
//        return result;
//
//    Reg->CR1 &= ~(1<<7);                            //disable send INT
//    for(result=0; result < len; result ++)
//    {
//        // ��ʱ���߷��ͻ���Ϊ��ʱ�˳�
//        while((false == __UART_TxTranEmpty(Reg))&& (timeout > 0))
//        {
//            timeout--;
//            DJY_DelayUs(1);
//        }
//        if(timeout == 0)
//            break;
//        Reg->DR = send_buf[result];
//    }
//    Reg->CR1 |= (1<<7);                             //enable send INT
//    return result;
//}

// =============================================================================
// ����: �������ڷ��ͣ���Ŀ���Ǵ����жϣ����жϷ�ʽ�������ݡ�
// ����: Reg,�������Ĵ��ڼĴ���ָ��.
// ����: ���͵ĸ���
// =============================================================================
static u32 __UART_SendStart (tagUartReg *Reg)
{
    u8 port;
    u32 delay=0;
    switch((u32)Reg)
    {
    case CN_UART1_BASE:        port = CN_UART1;    break;
    case CN_UART2_BASE:        port = CN_UART2;    break;
    case CN_UART3_BASE:        port = CN_UART3;    break;
    case CN_UART4_BASE:        port = CN_UART4;    break;
    case CN_UART5_BASE:        port = CN_UART5;    break;
    case CN_UART6_BASE:        port = CN_UART6;    break;
    default:return 0;
    }

    Reg->CR1 &= ~(1<<6);;
    Board_UartHalfDuplexSend(port);//�л�������
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
//       2����ʹuartʹ��dma�ڼ䣬Ҳ�������ģ����ϵ����⵼�������豸ʹ��dma.
// ����: port,�˿ںţ������ںţ���1~3
// ����: ��
// =============================================================================
void __UART_SetDmaUsed(u32 port)
{
    u8 *DmaRecvBuf,*DmaSendBuf;
    u32 DmaBufLen;
    switch(port)
    {
    case CN_UART1:
        DmaBufLen = CFG_UART1_DMABUF_LEN;        break;
    case CN_UART2:
        DmaBufLen = CFG_UART2_DMABUF_LEN;        break;
    case CN_UART3:
        DmaBufLen = CFG_UART3_DMABUF_LEN;        break;
    case CN_UART4:
        DmaBufLen = CFG_UART4_DMABUF_LEN;        break;
    case CN_UART5:
        DmaBufLen = CFG_UART5_DMABUF_LEN;        break;
    case CN_UART6:
        DmaBufLen = CFG_UART6_DMABUF_LEN;        break;
    default :        break;
    }

    if(pUART_DmaRecvBuf[port]==NULL)
        pUART_DmaRecvBuf[port] = (u8*)M_Malloc(DmaBufLen,0);
        if(pUART_DmaRecvBuf[port] == NULL)
            return;

    if(pUART_DmaSendBuf[port]==NULL)
        pUART_DmaSendBuf[port] = (u8*)M_Malloc(DmaBufLen,0);
        if(pUART_DmaSendBuf[port] == NULL)
        {
            free(pUART_DmaRecvBuf[port]);
            pUART_DmaRecvBuf[port]=NULL;
            return;
        }
    DmaRecvBuf = pUART_DmaRecvBuf[port];
    DmaSendBuf = pUART_DmaSendBuf[port];
    __UART_RxIntDisable(CN_DMA_UNUSED,port);
    __UART_TxIntDisable(CN_DMA_UNUSED,port);
    tg_UART_Reg[port]->CR1 |= (1<<4);//enable idle int

    DMA_Config(UartDmaRxStream[port],DMA_Rx_ch[port],(u32)&(tg_UART_Reg[port]->DR),
            (u32)DmaRecvBuf,DMA_DIR_P2M,DMA_DATABITS_8,DMA_DATABITS_8,32);

    DMA_Config(UartDmaTxStream[port],DMA_Tx_ch[port],(u32)&(tg_UART_Reg[port]->DR),
            (u32)DmaSendBuf,DMA_DIR_M2P,DMA_DATABITS_8,DMA_DATABITS_8,32);

    DMA_IntEnable(UartDmaTxStream[port],DMA_INT_TCIE);  //ʹ�ܷ�������ж�
    DMA_IntEnable(UartDmaRxStream[port],DMA_INT_HTIE);  //ʹ�ܽ��հ�����ж�

    tg_UART_Reg[port]->CR3 |= 0x00c0;  //���ô���ʹ��dma�շ�
    s_UART_DmaUsed[port] = CN_DMA_USED;
    DMA_Enable(UartDmaRxStream[port],(u32)DmaRecvBuf,32);//����dmaͨ��

    Int_Register(UartDmaTxInt[port]);
    Int_Register(UartDmaRxInt[port]);//ע�����DMA�ж�
    Int_SetClearType(UartDmaRxInt[port],CN_INT_CLEAR_AUTO);
    Int_IsrConnect(UartDmaRxInt[port],UART_DmaRx_ISR);
    Int_SetClearType(UartDmaTxInt[port],CN_INT_CLEAR_AUTO);
    Int_IsrConnect(UartDmaTxInt[port],UART_DmaTx_ISR);

    Int_SetIsrPara(UartDmaTxInt[port],port);
    Int_SetIsrPara(UartDmaRxInt[port],port);

    Int_SettoAsynSignal(UartDmaRxInt[port]);
    Int_SettoAsynSignal(UartDmaTxInt[port]);
    Int_RestoreAsynLine(UartDmaRxInt[port]);
    Int_RestoreAsynLine(UartDmaTxInt[port]);
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
    else if(port == CN_UART6)        IntLine = CN_INT_LINE_USART6;
    else
        return;

    free(pUART_DmaRecvBuf[port]);
    pUART_DmaRecvBuf[port]=NULL;
    free(pUART_DmaSendBuf[port]);
    pUART_DmaSendBuf[port]=NULL;

    tg_UART_Reg[port]->CR3 &= ~0x00c0;     //���ò�ʹ��dma����
    Int_RestoreAsynLine(IntLine);
    if(s_UART_DmaUsed[port] == CN_DMA_USED)
    {
        Int_SaveAsynLine(UartDmaRxInt[port]);
        Int_SaveAsynLine(UartDmaTxInt[port]);
        s_UART_DmaUsed[port] = CN_DMA_UNUSED;
    }
    __UART_RxIntEnable(CN_DMA_UNUSED,port);
    __UART_TxIntEnable(CN_DMA_UNUSED,port);
    tg_UART_Reg[port]->CR1 &= ~(1<<4);//disable idle int
}
// =============================================================================
// ����: �����豸�Ŀ��ƺ���,������Ӳ���Ĵ����������
// ����: Reg,�������Ĵ��ڼĴ���ָ��.
//       cmd,��������
//       data1,data2,������cmd����
// ����: ������.
// =============================================================================
static ptu32_t __UART_Ctrl(tagUartReg *Reg,u32 cmd, va_list *arg0)
{
    ptu32_t result = 0;
    u32 port;

    if(Reg == NULL)
        return 0;

    switch((u32)Reg)
    {
    case CN_UART1_BASE: port = CN_UART1;break;
    case CN_UART2_BASE: port = CN_UART2;break;
    case CN_UART3_BASE: port = CN_UART3;break;
    case CN_UART4_BASE: port = CN_UART4;break;
    case CN_UART5_BASE: port = CN_UART5;break;
    case CN_UART6_BASE: port = CN_UART6;break;
    default:return 0;
    }

    switch(cmd)
    {
        case CN_DEV_CTRL_START:
            __UART_Enable(port);
            break;
        case CN_DEV_CTRL_STOP:
            __UART_Disable(port);
            break;
        case CN_DEV_CTRL_SHUTDOWN:
            break;
        case CN_DEV_CTRL_SLEEP:
            break;
        case CN_DEV_CTRL_RESUME:
            break;
        case CN_UART_SET_BAUD:  //����Baud
        {
            u32 data;
            data = va_arg(*arg0, u32);
            __UART_BaudSet(Reg,port, data);
        }
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
        {
            struct COMParam *COM;
            COM = va_arg(*arg0, void *);
            __UART_ComConfig(Reg,port,COM);
        }
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
uint32_t UART_DmaTx_ISR(ptu32_t port)
{
    uint16_t num;
    u8 *DmaSendBuf;
    u32 DmaSendBufLen;

    switch(port)
    {
    case CN_UART1:
        DmaSendBufLen = CFG_UART1_DMABUF_LEN;        break;
    case CN_UART2:
        DmaSendBufLen = CFG_UART2_DMABUF_LEN;        break;
    case CN_UART3:
        DmaSendBufLen = CFG_UART3_DMABUF_LEN;        break;
    case CN_UART4:
        DmaSendBufLen = CFG_UART4_DMABUF_LEN;        break;
    case CN_UART5:
        DmaSendBufLen = CFG_UART5_DMABUF_LEN;        break;
    case CN_UART6:
        DmaSendBufLen = CFG_UART6_DMABUF_LEN;        break;
    default:        break;
    }

    DmaSendBuf = pUART_DmaSendBuf[port];
    DMA_ClearIntFlag(UartDmaTxStream[port]);
    num = UART_PortRead(pUartCB[port],DmaSendBuf,DmaSendBufLen);
    if(num == 0)
    {
        s_UART_DmaSending[port] = false;
        tg_UART_Reg[port]->CR1 |= (1<<6);
    }
    else
    {
        DMA_Enable(UartDmaTxStream[port],(u32)DmaSendBuf,num);
    }
    return 0;
}

// =============================================================================
// ����: ��uart2���ó�dma��ʽ�ǣ�dma1 ch7�����ͣ�ch6�������
// ����: �жϺ���û�в���.
// ����: �жϺ���û�з���ֵ.
// =============================================================================
uint32_t UART_DmaRx_ISR(ptu32_t port)
{
    u32 recvs,num,DmaRecvBufLen;
    u8 *DmaRecvBuf;

    switch(port)
    {
    case CN_UART1:
        DmaRecvBufLen = CFG_UART1_DMABUF_LEN;        break;
    case CN_UART2:
        DmaRecvBufLen = CFG_UART2_DMABUF_LEN;        break;
    case CN_UART3:
        DmaRecvBufLen = CFG_UART3_DMABUF_LEN;        break;
    case CN_UART4:
        DmaRecvBufLen = CFG_UART4_DMABUF_LEN;        break;
    case CN_UART5:
        DmaRecvBufLen = CFG_UART5_DMABUF_LEN;        break;
    case CN_UART6:
        DmaRecvBufLen = CFG_UART6_DMABUF_LEN;        break;
    default:
        break;
    }

    DmaRecvBuf = pUART_DmaRecvBuf[port];
    DMA_Disable(UartDmaRxStream[port]);
    DMA_ClearIntFlag(UartDmaRxStream[port]);
    recvs = DmaRecvBufLen - UartDmaRxStream[port]->NDTR;
    if(recvs == DmaRecvBufLen)     //dma�������������������ݶ�ʧ
    {
        UART_ErrHandle(pUartCB[port],CN_UART_FIFO_OVER_ERR);
    }else
    {
        num = UART_PortWrite(pUartCB[port],DmaRecvBuf,recvs);
        if(num != recvs)
        {
            UART_ErrHandle(pUartCB[port],CN_UART_BUF_OVER_ERR);
        }
    }

    DMA_Enable(UartDmaRxStream[port],(u32)DmaRecvBuf,DmaRecvBufLen);
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
u32 UART_ISR(ptu32_t port)
{
    struct UartGeneralCB *UCB;
    tagUartReg *Reg;
    u32 num;
    u8 ch;

    UCB = pUartCB[port];
    Reg = (tagUartReg *)tg_UART_Reg[port];
    if( (Reg->SR & (1<<5)) &&(Reg->CR1 &(1<<5)))           //�����ж�
    {
        ch = (u8)Reg->DR;
        num = UART_PortWrite(UCB,&ch,1);
        if(num != 1)
        {
            UART_ErrHandle(UCB,CN_UART_BUF_OVER_ERR);
        }
    }
    if((Reg->SR & (1<<6))&&(Reg->CR1 & (1<<6)) )
    {
        Reg->CR1 &= ~(1<<6);
        Board_UartHalfDuplexRecv(port);
    }
    if((Reg->SR & (1<<7))&&(Reg->CR1 & (1<<7)))    //�����ж�
    {
        num = UART_PortRead(UCB,&ch,1);
        if(num != 0)
            Reg->DR = ch;
        else
        {
            Reg->CR1 &= ~(1<<7);        //txeie
            Reg->CR1 |= (1<<6);
        }
    }
    if(Reg->SR & (1<<3))        //ORE���ش���
    {
        ch = (u8)Reg->DR;
        num = Reg->SR;     //��һ��sr�Ĵ���
        UART_ErrHandle(UCB,CN_UART_FIFO_OVER_ERR);
    }
    if((Reg->SR & (1<<4)) && (Reg->CR1 & (1<<4)))
    {
        Reg->DR;
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
        UART_Param.TxRingBufLen = CFG_UART1_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART1_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART2://����2
        UART_Param.Name         = "UART2";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART2_BASE;
        UART_Param.TxRingBufLen = CFG_UART2_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART2_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;

    case CN_UART3://����3
        UART_Param.Name         = "UART3";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART3_BASE;
        UART_Param.TxRingBufLen = CFG_UART3_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART3_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;

    case CN_UART4://����4
        UART_Param.Name         = "UART4";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART4_BASE;
        UART_Param.TxRingBufLen = CFG_UART4_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART4_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART5://����5
        UART_Param.Name         = "UART5";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART5_BASE;
        UART_Param.TxRingBufLen = CFG_UART5_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART5_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART6://����6
        UART_Param.Name         = "UART6";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART6_BASE;
        UART_Param.TxRingBufLen = CFG_UART6_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART6_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    default:
        return 0;
    }
    UART_Param.mode = CN_UART_GENERAL;
    pUART_DmaSendBuf[serial_no]=NULL;
    pUART_DmaRecvBuf[serial_no]=NULL;
    //Ӳ����ʼ��
    __UART_HardInit(serial_no);
    __UART_IntInit(serial_no);

    sUartInited |= (0x01 << serial_no);
    pUartCB[serial_no] = UART_InstallGeneral(&UART_Param);
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
    u32 result = 0;
    s32 timeout = TxByteTime * len;
    u16 CR_Bak;
    u8 port;
    for(port=0;port<CN_UART_NUM;port++)
    {
        if(PutStrDirectReg ==tg_UART_Reg[port])
            break;
    }
    Board_UartHalfDuplexSend(port);
    CR_Bak = PutStrDirectReg->CR1;                          //Save INT
    PutStrDirectReg->CR1 &= ~(1<<7);                        //disable send INT
    for(result=0; result < len+1; result ++)
    {
        // ��ʱ���߷��ͻ���Ϊ��ʱ�˳�
        while((false == __UART_TxTranEmpty(PutStrDirectReg))&& (timeout > 10))
        {
            timeout -=10;
            DJY_DelayUs(10);
        }
        if( (timeout <= 10) || (result == len))
            break;
        PutStrDirectReg->DR = str[result];
    }
    PutStrDirectReg->CR1 = CR_Bak;                         //restore send INT
    while((PutStrDirectReg->SR &(1<<6))!=(1<<6))
    {
        timeout -=10;
        DJY_DelayUs(10);
        if(timeout < 10)
           break;
    }
    Board_UartHalfDuplexRecv(port);
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

    CR_Bak = GetCharDirectReg->CR1;                          //Save INT
    GetCharDirectReg->CR1 &= ~(1<<7);                        //disable send INT
    while(__UART_RxHadChar(GetCharDirectReg) == false) ;
    result = GetCharDirectReg->DR;
    PutStrDirectReg->CR1 = CR_Bak;                         //restore send INT
    return result;
}

//----��ʼ���ں˼�IO-----------------------------------------------------------
//���ܣ���ʼ���ں˼��������������runtime����ָ�롣
//��������
//���أ���
//-----------------------------------------------------------------------------
void Stdio_KnlInOutInit(char * StdioIn, char *StdioOut)
{
    if(!strcmp(StdioOut,"/dev/UART1"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART1_BASE;
        TxDirectPort = CN_UART1;
    }
    else if(!strcmp(StdioOut,"/dev/UART2"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART2_BASE;
        TxDirectPort = CN_UART2;
    }
    else if(!strcmp(StdioOut,"/dev/UART3"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART3_BASE;
        TxDirectPort = CN_UART3;
    }
    else if(!strcmp(StdioOut,"/dev/UART4"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART4_BASE;
        TxDirectPort = CN_UART4;
    }
    else if(!strcmp(StdioOut,"/dev/UART5"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART5_BASE;
        TxDirectPort = CN_UART5;
    }
    else if(!strcmp(StdioOut,"/dev/UART6"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART6_BASE;
        TxDirectPort = CN_UART6;
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

    if(!strcmp(StdioIn,"/dev/UART1"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART1_BASE;
        RxDirectPort = CN_UART1;
    }
    else if(!strcmp(StdioIn,"/dev/UART2"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART2_BASE;
        RxDirectPort = CN_UART2;
    }
    else if(!strcmp(StdioIn,"/dev/UART3"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART3_BASE;
        RxDirectPort = CN_UART3;
    }
    else if(!strcmp(StdioIn,"/dev/UART4"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART4_BASE;
        RxDirectPort = CN_UART4;
    }
    else if(!strcmp(StdioIn,"/dev/UART5"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART5_BASE;
        RxDirectPort = CN_UART5;
    }
    else if(!strcmp(StdioIn,"/dev/UART6"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART6_BASE;
        RxDirectPort = CN_UART6;
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

