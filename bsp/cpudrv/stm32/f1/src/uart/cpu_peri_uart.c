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
#include "stdlib.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern ptu32_t ModuleInstall_UART(u32 serial_no);
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
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip uart"//CPU��uart��������
//parent:"uart device file"      //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"uart device file","int","cpu onchip dma"//������������������������none����ʾ�������������
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
//%$#@num,32,512,
#define CFG_UART1_SENDBUF_LEN            32                 //"UART1���ͻ��λ�������С",
#define CFG_UART1_RECVBUF_LEN            32                 //"UART1���ջ��λ�������С",
#define CFG_UART1_DMABUF_LEN             32                 //"UART1 DMA��������С",

#define CFG_UART2_SENDBUF_LEN            32                 //"UART2���ͻ��λ�������С",
#define CFG_UART2_RECVBUF_LEN            32                 //"UART2���ջ��λ�������С",
#define CFG_UART2_DMABUF_LEN             32                 //"UART2 DMA��������С",

#define CFG_UART3_SENDBUF_LEN            32                 //"UART3���ͻ��λ�������С",
#define CFG_UART3_RECVBUF_LEN            32                 //"UART3���ջ��λ�������С",
#define CFG_UART3_DMABUF_LEN             32                 //"UART3 DMA��������С",

#define CFG_UART4_SENDBUF_LEN            32                  //"UART4���ͻ��λ�������С",
#define CFG_UART4_RECVBUF_LEN            32                  //"UART4���ջ��λ�������С",

#define CFG_UART5_SENDBUF_LEN            32                   //"UART5���ͻ��λ�������С",
#define CFG_UART5_RECVBUF_LEN            32                   //"UART5���ջ��λ�������С",
//%$#@enum,true,false,
#define CFG_UART1_ENABLE                 true                 //"�Ƿ�����ʹ��UART1",
#define CFG_UART2_ENABLE                 false                //"�Ƿ�����ʹ��UART2",
#define CFG_UART3_ENABLE                 false                //"�Ƿ�����ʹ��UART3",
#define CFG_UART4_ENABLE                 false                //"�Ƿ�����ʹ��UART4",
#define CFG_UART5_ENABLE                 false                //"�Ƿ�����ʹ��UART5",
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

// =============================================================================
#define CN_UART1_BASE 0x40013800
#define CN_UART2_BASE 0x40004400
#define CN_UART3_BASE 0x40004800
#define CN_UART4_BASE 0x40004c00
#define CN_UART5_BASE 0x40005000

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
                                                    (tagUartReg *)CN_UART5_BASE};
#define CN_DMA_UNUSED   0
#define CN_DMA_USED     1

//#define CFG_UART1_SENDBUF_LEN            512
//#define CFG_UART1_RECVBUF_LEN            512
//#define CFG_UART1_DMABUF_LEN             32
static u8 * UART1_DmaSendBuf;
static u8 * UART1_DmaRecvBuf;
static bool_t UART1_FIRST_DMA_SEND;
static bool_t UART1_DMA_SENDING = false;
static bool_t s_UART1_DmaUsed = false;
uint32_t UART1_DMA1CH4_ISR(ptu32_t IntLine);
uint32_t UART1_DMA1CH5_ISR(ptu32_t IntLine);

//#define CFG_UART2_SENDBUF_LEN            32
//#define CFG_UART2_RECVBUF_LEN            32
//#define CFG_UART2_DMABUF_LEN             32
static u8 * UART2_DmaSendBuf;
static u8 * UART2_DmaRecvBuf;
static bool_t UART2_FIRST_DMA_SEND;
static bool_t UART2_DMA_SENDING = false;
static bool_t s_UART2_DmaUsed = false;
uint32_t UART2_DMA1CH7_ISR(ptu32_t IntLine);
uint32_t UART2_DMA1CH6_ISR(ptu32_t IntLine);

//#define CFG_UART3_SENDBUF_LEN            32
//#define CFG_UART3_RECVBUF_LEN            32
//#define CFG_UART3_DMABUF_LEN             32
static u8 * UART3_DmaSendBuf;
static u8 * UART3_DmaRecvBuf;
static bool_t UART3_FIRST_DMA_SEND;
static bool_t UART3_DMA_SENDING = false;
static bool_t s_UART3_DmaUsed = false;

uint32_t UART3_DMA1CH2_ISR(ptu32_t IntLine);
uint32_t UART3_DMA1CH3_ISR(ptu32_t IntLine);
//#define CFG_UART4_SENDBUF_LEN            32
//#define CFG_UART4_RECVBUF_LEN            32
//
//#define CFG_UART5_SENDBUF_LEN            32
//#define CFG_UART5_RECVBUF_LEN            32

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
static ptu32_t UART_ISR(ufast_t port);

// =============================================================================
// ����: ��ֹuart�ķ����жϡ�
// ����: flag,�Ƿ�ʹ��dma����
//       port,���ںţ�1~5
// ����: ��
// =============================================================================
void __UART_RxIntEnable(u8 flag,u8 port)
{
    switch(port)
    {
    case CN_UART1:
        if(flag == CN_DMA_USED)
            Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch5);
        else
            bb_uart1_cr1_rxneie = 1;
        break;
    case CN_UART2:
        if(flag == CN_DMA_USED)
            Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch6);
        else
            bb_uart2_cr1_rxneie = 1;//ʹ�ܽ����ж�
        break;
    case CN_UART3:
        if(flag == CN_DMA_USED)
            Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch3);
        else
            bb_uart3_cr1_rxneie = 1;//ʹ�ܽ����ж�
        break;
    case CN_UART4:
        if(flag == CN_DMA_USED)
            Int_RestoreAsynLine(CN_INT_LINE_DMA2_Ch3);
        else
            bb_uart4_cr1_rxneie = 1;//ʹ�ܽ����ж�
        break;
    case CN_UART5:
        bb_uart5_cr1_rxneie = 1;//ʹ�ܽ����ж�
        break;
    default:
        break;
    }
}

// =============================================================================
// ����: ��ֹuart�Ľ����ж�
// ����: reg,�������ļĴ�����ָ��
//       port,���ں�
// ����: ��
// =============================================================================
void __UART_RxIntDisable(u8 flag, u8 port)
{
    switch(port)
    {
    case CN_UART1:
        if(flag == CN_DMA_USED)
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch5);
        else
            bb_uart1_cr1_rxneie = 0;
        break;
    case CN_UART2:
        if(flag == CN_DMA_USED)
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch6);
        else
            bb_uart2_cr1_rxneie = 0;//��ֹ�����ж�
        break;
    case CN_UART3:
        if(flag == CN_DMA_USED)
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch3);
        else
            bb_uart3_cr1_rxneie = 0;//��ֹ�����ж�
        break;
    case CN_UART4:
        if(flag == CN_DMA_USED)
            Int_SaveAsynLine(CN_INT_LINE_DMA2_Ch3);
        else
            bb_uart4_cr1_rxneie = 0;//��ֹ�����ж�
        break;
    case CN_UART5:
        bb_uart5_cr1_rxneie = 0;//��ֹ�����ж�
        break;
    default:
        break;
    }
}

// =============================================================================
// ����: ʹ��uart�ķ����ж�
// ����: reg,�������ļĴ�����ָ��
//       port,���ں�
// ����: ��
// =============================================================================
static void __UART_TxIntEnable(u8 flag,u8 port)
{
    switch(port)
    {
    case CN_UART1:
        if(flag == CN_DMA_USED)
            Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch4);
        else
            bb_uart1_cr1_txeie = 1;//ʹ�ܷ��Ϳ��ж�
        break;
    case CN_UART2:
        if(flag == CN_DMA_USED)
            Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch7);
        else
            bb_uart2_cr1_txeie = 1;//ʹ�ܷ��Ϳ��ж�
        break;
    case CN_UART3:
        if(flag == CN_DMA_USED)
            Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch2);
        else
            bb_uart3_cr1_txeie = 1;//ʹ�ܷ��Ϳ��ж�
        break;
    case CN_UART4:
        if(flag == CN_DMA_USED)
            Int_RestoreAsynLine(CN_INT_LINE_DMA2_Ch4_5);
        else
            bb_uart4_cr1_txeie = 1;//ʹ�ܷ��Ϳ��ж�
        break;
    case CN_UART5:
        bb_uart5_cr1_txeie = 1;//ʹ�ܷ��Ϳ��ж�
        break;
    default:
        break;
    }
}

// =============================================================================
// ����: ��ֹUART�ķ����ж�
// ����: Reg,�������ļĴ�����ָ��
//       port,���ں�
// ����: ��
// =============================================================================
static void __UART_TxIntDisable(u8 flag, u8 port)
{
    switch(port)
    {
    case CN_UART1:
        if(flag == CN_DMA_USED)
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch4);
        else
            bb_uart1_cr1_txeie = 0;//��ֹ���Ϳ��ж�
        break;
    case CN_UART2:
        if(flag == CN_DMA_USED)
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch7);
        else
            bb_uart2_cr1_txeie = 0;//��ֹ���Ϳ��ж�
        break;
    case CN_UART3:
        if(flag == CN_DMA_USED)
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch2);
        else
            bb_uart3_cr1_txeie = 0;//��ֹ���Ϳ��ж�
        break;
    case CN_UART4:
        if(flag == CN_DMA_USED)
            Int_SaveAsynLine(CN_INT_LINE_DMA2_Ch4_5);
        else
            bb_uart4_cr1_txeie = 0;//��ֹ���Ϳ��ж�
        break;
    case CN_UART5:
        bb_uart5_cr1_txeie = 0;//��ֹ���Ϳ��ж�
        break;
    default:
        break;
    }
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
    return (1 == (reg->SR & (1<<5)));
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
    u32 temp1,temp2;
    if((port == CN_UART1) || (port == CN_UART5))
    {
        temp1 = CN_CFG_PCLK2/(16*baud);
        temp2 = CN_CFG_PCLK2%(16*baud);
        temp2 = temp2/baud;
    }
    else
    {
        temp1 = CN_CFG_PCLK1/(16*baud);
        temp2 = CN_CFG_PCLK1%(16*baud);
        temp2 = temp2/baud;
    }
    Reg->BRR =  ((u16)temp1<<4)+(u16)temp2;
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
    {
    case CN_UART1:
        bb_rcc_apb2enr_uart1en = 1;    //uart1ʱ��ʹ��
        bb_uart1_cr1_ue = 1;
        break;
    case CN_UART2:
        bb_rcc_apb1enr_uart2en = 1;    //uart2ʱ��ʹ��
        bb_uart2_cr1_ue = 1;
        break;
    case CN_UART3:
        bb_rcc_apb1enr_uart3en = 1;    //uart3ʱ��ʹ��
        bb_uart3_cr1_ue = 1;
        break;
    case CN_UART4:
        bb_rcc_apb1enr_uart4en = 1;    //uart4ʱ��ʹ��
        bb_uart4_cr1_ue = 1;
        break;
    case CN_UART5:
        bb_rcc_apb1enr_uart5en = 1;    //uart5ʱ��ʹ��
        bb_uart5_cr1_ue = 1;
        break;
    default:
        break;
    }
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
    case CN_UART1:
        bb_rcc_apb2enr_uart1en = 0;    //uart1ʱ��ʹ��
        bb_uart1_cr1_ue = 0;
        break;
    case CN_UART2:
        bb_rcc_apb1enr_uart2en = 0;    //uart2ʱ��ʹ��
        bb_uart2_cr1_ue = 0;
        break;
    case CN_UART3:
        bb_rcc_apb1enr_uart3en = 0;    //uart3ʱ��ʹ��
        bb_uart3_cr1_ue = 0;
        break;
    case CN_UART4:
        bb_rcc_apb1enr_uart4en = 0;    //uart4ʱ��ʹ��
        bb_uart4_cr1_ue = 0;
        break;
    case CN_UART5:
        bb_rcc_apb1enr_uart5en = 0;    //uart5ʱ��ʹ��
        bb_uart5_cr1_ue = 0;
        break;
    default:
        break;
    }
}

// =============================================================================
// ����: ��ʼ��UART��Ӧ���ж��ߣ�����ʼ���ж���ں���
// ������SerialNo,���ں�
// ����: ��
// =============================================================================
static void __UART_IntInit(ptu32_t SerialNo)
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
    else if(SerialNo == CN_UART4)
        IntLine = CN_INT_LINE_UART5;
    else
        return;

    Int_Register(IntLine);
    Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(IntLine,UART_ISR);
    Int_SetIsrPara(IntLine,SerialNo);
    Int_SettoAsynSignal(IntLine);
    Int_ClearLine(IntLine);
    Int_RestoreAsynLine(IntLine);
}

// =============================================================================
// ����: Ӳ���������úͼĴ����ĳ�ʼ�������������ʡ�ֹͣλ��У��λ������λ��Ĭ�������:
//       ������:115200  �� ֹͣλ:1 ; У��:żУ�� ; ����λ:8bit
// ����: SerialNo,���ںţ�0~3
// ����: ��
// =============================================================================
static void __UART_HardInit(u8 SerialNo)
{
    if(SerialNo > CN_UART5)
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
    u32 timeout = 1000;
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

    switch(port)
    {
    case CN_UART1:
//        if(true == __uart_dma_timeout(UART1_DMA_SENDING))
        if(true == UART1_DMA_SENDING)
            break;
        num = UART_PortRead(pUartCB[CN_UART1],UART1_DmaSendBuf,CFG_UART1_DMABUF_LEN);
        if(UART1_FIRST_DMA_SEND)
        {
            tg_UART_Reg[CN_UART1]->DR = UART1_DmaSendBuf[0];
            UART1_FIRST_DMA_SEND = false;
            addr = (u32)UART1_DmaSendBuf+1;
            num--;
        }else
            addr = (u32)UART1_DmaSendBuf;

        if(num > 0)
        {
            bb_dma1_ch4_ccr_en = 0;    //��ͣdma����
            pg_dma1_channel4_reg->CMAR = addr;
            pg_dma1_channel4_reg->CNDTR = num;
            bb_dma1_ch4_ccr_en = 1;    //����dma����
            UART1_DMA_SENDING = true;
        }
        else
            tg_UART_Reg[CN_UART1]->CR1 |=(1<<6);
        break;
    case 2:
//        if(true == __uart_dma_timeout(UART2_DMA_SENDING))
        if(true == UART2_DMA_SENDING)
            break;
        num = UART_PortRead(pUartCB[CN_UART2],UART2_DmaSendBuf,CFG_UART2_DMABUF_LEN);
        if(UART2_FIRST_DMA_SEND)
        {
            tg_UART_Reg[CN_UART1]->DR = UART2_DmaSendBuf[0];
            UART2_FIRST_DMA_SEND = false;
            addr = (u32)UART2_DmaSendBuf+1;
            num--;
        }else
            addr = (u32)UART2_DmaSendBuf;

        if(num > 0)
        {
            bb_dma1_ch7_ccr_en = 0;    //��ͣdma����
            pg_dma1_channel7_reg->CMAR = addr;
            pg_dma1_channel7_reg->CNDTR = num;
            bb_dma1_ch7_ccr_en = 1;    //����dma����
            UART2_DMA_SENDING = true;
        }
        else
            tg_UART_Reg[CN_UART1]->CR1 |=(1<<6);
        break;
    case 3:
//        if(true == __uart_dma_timeout(UART3_DMA_SENDING))
        if(true == UART3_DMA_SENDING)
            break;
        num = UART_PortRead(pUartCB[CN_UART3],(u8*)UART3_DmaSendBuf,CFG_UART3_DMABUF_LEN);
        if(UART3_FIRST_DMA_SEND)
        {
            tg_UART_Reg[CN_UART1]->DR = UART3_DmaSendBuf[0];
            UART3_FIRST_DMA_SEND = false;
            addr = (u32)UART3_DmaSendBuf+1;
            num--;
        }else
            addr = (u32)UART3_DmaSendBuf;

        if(num > 0)
        {
            bb_dma1_ch2_ccr_en = 0;    //��ͣdma����
            pg_dma1_channel2_reg->CMAR = addr;
            pg_dma1_channel2_reg->CNDTR = num;
            bb_dma1_ch2_ccr_en = 1;    //����dma����
            UART3_DMA_SENDING = true;
        }
        else
            tg_UART_Reg[CN_UART1]->CR1 |=(1<<6);
        break;
    default:
            tg_UART_Reg[CN_UART1]->CR1 |=(1<<6);
        break;
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
//        while((false == __UART_TxTranEmpty(Reg))&& (timeout > 10))
//        {
//            timeout -=10;
//            DJY_DelayUs(10);
//        }
//        if(timeout <= 10)
//            break;
//        Reg->DR = send_buf[result];
//    }
//    Reg->CR1 |= (1<<7);                             //enable send INT
//    return result;
//}
//
// =============================================================================
// ����: �������ڷ��ͣ���Ŀ���Ǵ����жϣ����жϷ�ʽ�������ݡ�
// ����: Reg,�������Ĵ��ڼĴ���ָ��.
// ����: ���͵ĸ���
// =============================================================================
static u32 __UART_SendStart (tagUartReg *Reg)
{
    u8 port,dmaused;


    switch((u32)Reg)
    {
    case CN_UART1_BASE:
        dmaused = s_UART1_DmaUsed;
        port = CN_UART1;    break;
    case CN_UART2_BASE:
        dmaused = s_UART2_DmaUsed;
        port = CN_UART2;    break;
    case CN_UART3_BASE:
        dmaused = s_UART3_DmaUsed;
        port = CN_UART3;    break;
    case CN_UART4_BASE:
        dmaused = CN_DMA_UNUSED;
        port = CN_UART4;    break;
    case CN_UART5_BASE:
        dmaused = CN_DMA_UNUSED;
        port = CN_UART5;    break;
    default:return 0;
    }
    Reg->CR1 &=~(1<<6);
    Board_UartHalfDuplexSend(port);//�л�������
    __UART_TxIntDisable(dmaused,port);
    if(dmaused)
    {
        __UART_DMA_SendStart(port);
    }
    else
    {
        //��DMA��ʽ��ֻҪʹ�ܷ��Ϳ��жϣ��򴥷��ж�
    }

    __UART_TxIntEnable(dmaused,port);
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


    switch(port)
    {
    case CN_UART1:

       if(UART1_DmaSendBuf==NULL)
           UART1_DmaSendBuf = (u8*)M_Malloc(CFG_UART1_DMABUF_LEN,0);
           if(UART1_DmaSendBuf == NULL)
               return;

       if(UART1_DmaRecvBuf==NULL)
           UART1_DmaRecvBuf = (u8*)M_Malloc(CFG_UART1_DMABUF_LEN,0);
           if(UART1_DmaRecvBuf == NULL)
           {
               free(UART1_DmaRecvBuf);
               UART1_DmaRecvBuf=NULL;
               return;
           }

        bb_rcc_ahbenr_dma1en = 1;
        bb_dma1_ch4_ccr_en = 0;    //ֹͣdmaͨ��
        bb_dma1_ch5_ccr_en = 0;    //ֹͣdmaͨ��
        bb_uart1_cr1_rxneie = 0;
        bb_uart1_cr1_txeie = 0;
        bb_uart1_cr1_idleie = 1;
        pg_dma1_reg->IFCR |= 0x000ff000;        //�������ж�״̬
        pg_dma1_channel4_reg->CCR = 0x00000092; //����dma
        pg_dma1_channel4_reg->CPAR = (u32)&(tg_UART_Reg[CN_UART1]->DR);

        pg_dma1_channel5_reg->CCR = 0x00000086; //����dma
        pg_dma1_channel5_reg->CPAR = (u32)&(tg_UART_Reg[CN_UART1]->DR);
        pg_dma1_channel5_reg->CNDTR = 32;
        pg_dma1_channel5_reg->CMAR = (u32)UART1_DmaRecvBuf;

        Int_Register(CN_INT_LINE_DMA1_Ch4);
        Int_Register(CN_INT_LINE_DMA1_Ch5);

        Int_SetClearType(CN_INT_LINE_DMA1_Ch4,CN_INT_CLEAR_AUTO);
        Int_IsrConnect(CN_INT_LINE_DMA1_Ch4,UART1_DMA1CH4_ISR);
        Int_SetClearType(CN_INT_LINE_DMA1_Ch5,CN_INT_CLEAR_AUTO);
        Int_IsrConnect(CN_INT_LINE_DMA1_Ch5,UART1_DMA1CH5_ISR);

        Int_SettoAsynSignal(CN_INT_LINE_DMA1_Ch4);
        Int_SettoAsynSignal(CN_INT_LINE_DMA1_Ch5);
        Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch4);
        Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch5);

        tg_UART_Reg[CN_UART1]->CR3 |= 0x00c0;  //���ô���ʹ��dma�շ�
        s_UART1_DmaUsed = CN_DMA_USED;
        bb_dma1_ch5_ccr_en = 1;    //����dmaͨ��
        UART1_FIRST_DMA_SEND = true;
        break;
    case CN_UART2:
        if(UART2_DmaSendBuf==NULL)
            UART2_DmaSendBuf = (u8*)M_Malloc(CFG_UART2_DMABUF_LEN,0);
            if(UART2_DmaSendBuf == NULL)
                return;

        if(UART2_DmaRecvBuf==NULL)
            UART2_DmaRecvBuf = (u8*)M_Malloc(CFG_UART2_DMABUF_LEN,0);
            if(UART2_DmaRecvBuf == NULL)
            {
                free(UART2_DmaRecvBuf);
                UART2_DmaRecvBuf=NULL;
                return;
            }
        bb_rcc_ahbenr_dma1en = 1;
        bb_dma1_ch7_ccr_en = 0;    //ֹͣdmaͨ��
        bb_dma1_ch6_ccr_en = 0;    //ֹͣdmaͨ��
        bb_uart2_cr1_rxneie = 0;
        bb_uart2_cr1_txeie = 0;
        bb_uart2_cr1_idleie = 1;
        pg_dma1_reg->IFCR |= 0x0ff00000;        //�������ж�״̬
        pg_dma1_channel7_reg->CCR = 0x00000092; //����dma
        pg_dma1_channel7_reg->CPAR = (u32)&(tg_UART_Reg[CN_UART2]->DR);

        pg_dma1_channel6_reg->CCR = 0x00000086; //����dma
        pg_dma1_channel6_reg->CPAR = (u32)&(tg_UART_Reg[CN_UART2]->DR);
        pg_dma1_channel6_reg->CNDTR = 32;
        pg_dma1_channel6_reg->CMAR = (u32)UART2_DmaRecvBuf;

        Int_Register(CN_INT_LINE_DMA1_Ch6);
        Int_Register(CN_INT_LINE_DMA1_Ch7);

        Int_SetClearType(CN_INT_LINE_DMA1_Ch6,CN_INT_CLEAR_AUTO);
        Int_IsrConnect(CN_INT_LINE_DMA1_Ch6,UART2_DMA1CH6_ISR);
        Int_SetClearType(CN_INT_LINE_DMA1_Ch7,CN_INT_CLEAR_AUTO);
        Int_IsrConnect(CN_INT_LINE_DMA1_Ch7,UART2_DMA1CH7_ISR);

        Int_SettoAsynSignal(CN_INT_LINE_DMA1_Ch6);
        Int_SettoAsynSignal(CN_INT_LINE_DMA1_Ch7);
        Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch6);
        Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch7);

        tg_UART_Reg[CN_UART2]->CR3 |= 0x00c0;  //���ô���ʹ��dma�շ�
        s_UART2_DmaUsed = CN_DMA_USED;
        bb_dma1_ch6_ccr_en = 1;    //����dmaͨ��
        UART2_FIRST_DMA_SEND = true;
        break;
    case CN_UART3:
        if(UART3_DmaSendBuf==NULL)
            UART3_DmaSendBuf = (u8*)M_Malloc(CFG_UART3_DMABUF_LEN,0);
            if(UART3_DmaSendBuf == NULL)
                return;

        if(UART3_DmaRecvBuf==NULL)
            UART3_DmaRecvBuf = (u8*)M_Malloc(CFG_UART3_DMABUF_LEN,0);
            if(UART3_DmaRecvBuf == NULL)
            {
                free(UART3_DmaSendBuf);
                UART3_DmaRecvBuf=NULL;
                return;
            }
        bb_rcc_ahbenr_dma1en = 1;
        bb_dma1_ch2_ccr_en = 0;    //ֹͣdmaͨ��
        bb_dma1_ch3_ccr_en = 0;    //ֹͣdmaͨ��
        bb_uart3_cr1_rxneie = 0;
        bb_uart3_cr1_txeie = 0;
        bb_uart3_cr1_idleie = 1;
        pg_dma1_reg->IFCR |= 0x000ff000;        //�������ж�״̬
        pg_dma1_channel2_reg->CCR = 0x00000092; //����dma
        pg_dma1_channel2_reg->CPAR = (u32)&(tg_UART_Reg[CN_UART3]->DR);

        pg_dma1_channel3_reg->CCR = 0x00000086; //����dma
        pg_dma1_channel3_reg->CPAR = (u32)&(tg_UART_Reg[CN_UART3]->DR);
        pg_dma1_channel3_reg->CNDTR = 32;
        pg_dma1_channel3_reg->CMAR = (u32)UART3_DmaRecvBuf;

        Int_Register(CN_INT_LINE_DMA1_Ch2);
        Int_Register(CN_INT_LINE_DMA1_Ch3);

        Int_SetClearType(CN_INT_LINE_DMA1_Ch2,CN_INT_CLEAR_AUTO);
        Int_IsrConnect(CN_INT_LINE_DMA1_Ch2,UART3_DMA1CH2_ISR);
        Int_SetClearType(CN_INT_LINE_DMA1_Ch3,CN_INT_CLEAR_AUTO);
        Int_IsrConnect(CN_INT_LINE_DMA1_Ch3,UART3_DMA1CH3_ISR);

        Int_SettoAsynSignal(CN_INT_LINE_DMA1_Ch2);
        Int_SettoAsynSignal(CN_INT_LINE_DMA1_Ch3);
        Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch2);
        Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch3);

        tg_UART_Reg[CN_UART3]->CR3 |= 0x00c0;  //���ô���ʹ��dma�շ�
        s_UART3_DmaUsed = CN_DMA_USED;
        bb_dma1_ch3_ccr_en = 1;    //����dmaͨ��
        UART3_FIRST_DMA_SEND = true;
        break;
    default:
        break;
    }

}

//=============================================================================
// ����: ���ò�ʹ��DMA��UART��ʹ��DMA�շ���ÿ�շ�һ���ֽھͱ����ж�һ�Σ��������ڸ���
//      ����ͨ����ʹ�á�
// ����: port,�˿ںţ������ںţ�Ϊ1~3
// ����: ��
// =============================================================================
void __UART_SetDmaUnUsed(u32 port)
{
    switch(port)
    {
    case CN_UART1:
        free(UART1_DmaSendBuf);
        free(UART1_DmaRecvBuf);
        UART1_DmaRecvBuf=NULL;
        UART1_DmaRecvBuf=NULL;
        tg_UART_Reg[CN_UART1]->CR3 &= ~0x00c0;     //���ò�ʹ��dma����
        Int_RestoreAsynLine(CN_INT_LINE_USART1);
        if(s_UART1_DmaUsed == CN_DMA_USED)
        {
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch4);
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch5);
            s_UART1_DmaUsed = CN_DMA_UNUSED;
        }
        bb_uart1_cr1_rxneie = 1;
        bb_uart1_cr1_txeie  = 1;
        bb_uart1_cr1_idleie = 0;
        break;
    case CN_UART2:
        free(UART2_DmaSendBuf);
        free(UART2_DmaRecvBuf);
        UART2_DmaRecvBuf=NULL;
        UART2_DmaRecvBuf=NULL;
        tg_UART_Reg[CN_UART2]->CR3 &= ~0x00c0;     //���ò�ʹ��dma����
        Int_RestoreAsynLine(CN_INT_LINE_USART2);
        if(s_UART2_DmaUsed == CN_DMA_USED)
        {
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch6);
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch7);
            s_UART2_DmaUsed = CN_DMA_UNUSED;
        }
        bb_uart2_cr1_rxneie = 1;
        bb_uart2_cr1_txeie = 1;
        bb_uart2_cr1_idleie = 0;
        break;
    case CN_UART3:
        free(UART3_DmaSendBuf);
        free(UART3_DmaRecvBuf);
        UART3_DmaRecvBuf=NULL;
        UART3_DmaRecvBuf=NULL;
        tg_UART_Reg[CN_UART3]->CR3 &= ~0x00c0;     //���ò�ʹ��dma����
        Int_RestoreAsynLine(CN_INT_LINE_USART3);
        if(s_UART3_DmaUsed == CN_DMA_USED)
        {
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch2);
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch3);
            s_UART3_DmaUsed = CN_DMA_UNUSED;
        }
        bb_uart3_cr1_rxneie = 1;
        bb_uart3_cr1_txeie = 1;
        bb_uart3_cr1_idleie = 0;
        break;
    default:
        break;
    }
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
// ����: UART1��DMA�жϣ���uart1���ó�dma��ʽʱ��dma1 ch4�����ͣ�ch5�������
// ����: �жϺ���û�в���.
// ����: �жϺ���û�з���ֵ.
// =============================================================================
uint32_t UART1_DMA1CH4_ISR(ptu32_t tagIntLine)
{
    uint16_t num;
    pg_dma1_reg->IFCR |= 0x0000f000;
    num = UART_PortRead(pUartCB[CN_UART1],UART1_DmaSendBuf,CFG_UART1_DMABUF_LEN);
    if(num == 0)
        {
            UART1_DMA_SENDING = false;
            tg_UART_Reg[CN_UART1]->CR1 |= (1<<6);
        }
    else
    {
        bb_dma1_ch4_ccr_en = 0;    //��ͣdma����
        pg_dma1_channel4_reg->CMAR = (u32)UART1_DmaSendBuf;
        pg_dma1_channel4_reg->CNDTR = num;
        bb_dma1_ch4_ccr_en = 1;    //����dma����
    }
    return 0;
}

// =============================================================================
// ����: UART1��DMA�жϣ���uart1���ó�dma��ʽʱ��dma1 ch4�����ͣ�ch5�������
// ����: �жϺ���û�в���.
// ����: �жϺ���û�з���ֵ.
// =============================================================================
uint32_t UART1_DMA1CH5_ISR(ptu32_t tagIntLine)
{
    u32 recvs,num;
    pg_dma1_reg->IFCR |= 0x000f0000;
    bb_dma1_ch5_ccr_en = 0;    //��ͣdma����
    recvs = CFG_UART1_DMABUF_LEN - pg_dma1_channel5_reg->CNDTR;
    if(recvs == CFG_UART1_DMABUF_LEN)     //dma�������������������ݶ�ʧ
    {
        UART_ErrHandle(pUartCB[CN_UART1],CN_UART_FIFO_OVER_ERR);
    }else
    {
        num = UART_PortWrite(pUartCB[CN_UART1],UART1_DmaRecvBuf,recvs);
        if(num != recvs)
        {
            UART_ErrHandle(pUartCB[CN_UART1],CN_UART_BUF_OVER_ERR);
        }
    }
    pg_dma1_channel5_reg->CNDTR = CFG_UART1_DMABUF_LEN;
    pg_dma1_channel5_reg->CMAR = (u32)UART1_DmaRecvBuf;
    bb_dma1_ch5_ccr_en = 1;    //����dma����
    return 0;
}

// =============================================================================
// ����: ��uart2���ó�dma��ʽ�ǣ�dma1 ch7�����ͣ�ch6�������
// ����: �жϺ���û�в���.
// ����: �жϺ���û�з���ֵ.
// =============================================================================
uint32_t UART2_DMA1CH7_ISR(ptu32_t tagIntLine)
{
    uint16_t num;

    pg_dma1_reg->IFCR |= 0x0f000000;
    num = UART_PortRead(pUartCB[CN_UART2],UART2_DmaSendBuf,CFG_UART2_DMABUF_LEN);
    if(num == 0)
        UART2_DMA_SENDING = false;
    else
    {
        bb_dma1_ch7_ccr_en = 0;    //��ͣdma����
        pg_dma1_channel7_reg->CMAR = (u32)UART2_DmaSendBuf;
        pg_dma1_channel7_reg->CNDTR = num;
        bb_dma1_ch7_ccr_en = 1;    //����dma����
    }
    return 0;
}

// =============================================================================
// ����: ��uart2���ó�dma��ʽ�ǣ�dma1 ch7�����ͣ�ch6�������
// ����: �жϺ���û�в���.
// ����: �жϺ���û�з���ֵ.
// =============================================================================
uint32_t UART2_DMA1CH6_ISR(ptu32_t tagIntLine)
{
    u32 recvs,num;
    pg_dma1_reg->IFCR |= 0x00f00000;
    bb_dma1_ch6_ccr_en = 0;    //��ͣdma����
    recvs = CFG_UART2_DMABUF_LEN - pg_dma1_channel6_reg->CNDTR;
    if(recvs == CFG_UART2_DMABUF_LEN)     //dma�������������������ݶ�ʧ
    {
        UART_ErrHandle(pUartCB[CN_UART2],CN_UART_FIFO_OVER_ERR);
    }else
    {
        num = UART_PortWrite(pUartCB[CN_UART2],UART2_DmaRecvBuf,recvs);
        if(num != recvs)
        {
            UART_ErrHandle(pUartCB[CN_UART2],CN_UART_BUF_OVER_ERR);
        }
    }
    pg_dma1_channel6_reg->CNDTR = CFG_UART2_DMABUF_LEN;
    pg_dma1_channel6_reg->CMAR = (u32)UART2_DmaRecvBuf;
    bb_dma1_ch6_ccr_en = 1;    //����dma����
    return 0;
}

// =============================================================================
// ����: ��uart3���ó�dma��ʽ�ǣ�dma1 ch2�����ͣ�ch3�������
// ����: �жϺ���û�в���.
// ����: �жϺ���û�з���ֵ.
// =============================================================================
uint32_t UART3_DMA1CH2_ISR(ptu32_t tagIntLine)
{
    uint16_t num;
    pg_dma1_reg->IFCR |= 0x000000f0;
    num = UART_PortRead(pUartCB[CN_UART3],UART3_DmaSendBuf,CFG_UART3_DMABUF_LEN);
    if(num == 0)
        UART3_DMA_SENDING = false;
    else
    {
        bb_dma1_ch2_ccr_en = 0;    //��ͣdma����
        pg_dma1_channel2_reg->CMAR = (u32)UART3_DmaSendBuf;
        pg_dma1_channel2_reg->CNDTR = num;
        bb_dma1_ch2_ccr_en = 1;    //����dma����
    }
    return 0;
}

// =============================================================================
//����: ��uart3���ó�dma��ʽ�ǣ�dma1 ch2�����ͣ�ch3�������
//����: �жϺ���û�в���.
//����: �жϺ���û�з���ֵ.
// =============================================================================
uint32_t UART3_DMA1CH3_ISR(ptu32_t tagIntLine)
{
    u32 recvs,num;
    pg_dma1_reg->IFCR |= 0x00000f00;
    bb_dma1_ch3_ccr_en = 0;    //��ͣdma����
    recvs = CFG_UART3_DMABUF_LEN - pg_dma1_channel3_reg->CNDTR;
    if(recvs == CFG_UART3_DMABUF_LEN)     //dma�������������������ݶ�ʧ
    {
        UART_ErrHandle(pUartCB[CN_UART3],CN_UART_FIFO_OVER_ERR);
    }else
    {
        num = UART_PortWrite(pUartCB[CN_UART3],UART3_DmaRecvBuf,recvs);
        if(num != recvs)
        {
            UART_ErrHandle(pUartCB[CN_UART3],CN_UART_BUF_OVER_ERR);
        }
    }
    pg_dma1_channel3_reg->CNDTR = CFG_UART3_DMABUF_LEN;
    pg_dma1_channel3_reg->CMAR = (u32)UART3_DmaRecvBuf;
    bb_dma1_ch3_ccr_en = 1;    //����dma����
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


    if((Reg->SR & (1<<5)) &&(Reg->CR1 & (1<<5))  )          //�����ж�
    {
        ch = (u8)Reg->DR;
        num = UART_PortWrite(UCB,&ch,1);
        if(num != 1)
        {
            UART_ErrHandle(UCB,CN_UART_BUF_OVER_ERR);
        }
    }
    if((Reg->SR & (1<<6)) && (Reg->CR1&(1<<6)))
    {
        Reg->CR1 &=~(1<<6);//��TC�ж�
        Board_UartHalfDuplexRecv(port);
    }
    if((Reg->SR & (1<<7)) &&(Reg->CR1 & (1<<7)))    //�����ж�
    {
        num = UART_PortRead(UCB,&ch,1);
        if(num != 0)
            Reg->DR = ch;
        else
        {
            Reg->CR1 &= ~(1<<7);        //txeie
            Reg->CR1 |=(1<<6);
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
        Int_TapLine(CN_INT_LINE_DMA1_Ch5);
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
    case CN_UART1://����0
        UART1_DmaSendBuf=NULL ;
        UART1_DmaRecvBuf=NULL ;
        UART_Param.Name         = "UART1";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART1_BASE;
        UART_Param.TxRingBufLen = CFG_UART1_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART1_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART2://����2
        UART2_DmaSendBuf=NULL ;
        UART2_DmaRecvBuf=NULL ;
        UART_Param.Name         = "UART2";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART2_BASE;
        UART_Param.TxRingBufLen = CFG_UART2_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART2_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART3://����3
        UART3_DmaSendBuf=NULL ;
        UART3_DmaRecvBuf=NULL ;
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
    default:
        return 0;
    }
    UART_Param.mode = CN_UART_GENERAL;
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
