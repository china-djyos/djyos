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
// ������Ա:
// ����ʱ��:
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
#include "cpu_peri_uart.h"
#include "cpu_peri_int_line.h"
//#include "cpu_peri_dma.h"
#include "stm32f303xc.h"
#include "stdlib.h"
#include "math.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�
//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern ptu32_t ModuleInstall_UART(u32 serial_no,u32 SendBufLen, u32 RecvBufLen,u8 mode);
//
//    #if CFG_UART1_ENABLE ==1
//    ModuleInstall_UART(CN_UART1,CFG_UART1_SENDBUF_LEN,CFG_UART1_RECVBUF_LEN,CFG_UART1_MODE);
//    #endif
//
//    #if CFG_UART2_ENABLE ==1
//    ModuleInstall_UART(CN_UART2,CFG_UART2_SENDBUF_LEN,CFG_UART2_RECVBUF_LEN,CFG_UART2_MODE);
//    #endif
//
//    #if CFG_UART3_ENABLE ==1
//    ModuleInstall_UART(CN_UART3,CFG_UART3_SENDBUF_LEN,CFG_UART3_RECVBUF_LEN,CFG_UART3_MODE);
//    #endif
//
//    #if CFG_UART4_ENABLE ==1
//    ModuleInstall_UART(CN_UART4,CFG_UART4_SENDBUF_LEN,CFG_UART4_RECVBUF_LEN,CFG_UART4_MODE);
//    #endif
//
//    #if CFG_UART5_ENABLE ==1
//    ModuleInstall_UART(CN_UART5,CFG_UART5_SENDBUF_LEN,CFG_UART5_RECVBUF_LEN,CFG_UART5_MODE);
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
//dependence:"device file system","uart device file","heap","cpu onchip dma"//������������������������none����ʾ�������������
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
//%$#@target = header              //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_UART    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,16,512,
#define CFG_UART1_SENDBUF_LEN            32                 //"UART1���ͻ��λ�������С",
#define CFG_UART1_RECVBUF_LEN            32                 //"UART1���ջ��λ�������С",
#define CFG_UART1_DMABUF_LEN             32                 //"UART1 DMA��������С"

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
//%$#@enum,true,false,
#define CFG_UART1_MODE                   1                  //"UART1ģʽ",CN_UART_POLL or CN_UART_GENERAL
#define CFG_UART2_MODE                   1                  //"UART2ģʽ",CN_UART_POLL or CN_UART_GENERAL
#define CFG_UART3_MODE                   1                  //"UART3ģʽ",CN_UART_POLL or CN_UART_GENERAL
#define CFG_UART4_MODE                   1                  //"UART4ģʽ",CN_UART_POLL or CN_UART_GENERAL
#define CFG_UART5_MODE                   1                  //"UART5ģʽ",CN_UART_POLL or CN_UART_GENERAL
//%$#@enum,true,false,
#define CFG_UART1_ENABLE                 true               //"�Ƿ�����ʹ��UART1",
#define CFG_UART2_ENABLE                 false              //"�Ƿ�����ʹ��UART2",
#define CFG_UART3_ENABLE                 false              //"�Ƿ�����ʹ��UART3",
#define CFG_UART4_ENABLE                 false              //"�Ƿ�����ʹ��UART4",
#define CFG_UART5_ENABLE                 false              //"�Ƿ�����ʹ��UART5",

//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

extern void Board_UartHalfDuplexSend(u8 SerialNo);
extern void Board_UartHalfDuplexRecv(u8 SerialNo);


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

//Ĭ��DMA������Pollģʽ�������������⼸������
static u32 UART1_DmaBufLen = CFG_UART1_DMABUF_LEN;
static u32 UART2_DmaBufLen = CFG_UART2_DMABUF_LEN;
static u32 UART3_DmaBufLen = CFG_UART3_DMABUF_LEN;
static u32 UART4_DmaBufLen = CFG_UART4_DMABUF_LEN;
static u32 UART5_DmaBufLen = CFG_UART5_DMABUF_LEN;

static u8 *pUART_DmaSendBuf[CN_UART_NUM];
static u8 *pUART_DmaRecvBuf[CN_UART_NUM]={NULL,NULL,NULL,NULL};

struct DivPollCB
{
    u8 *DmaRecvBuf[5];
    u32 RcvBufLen[5]; //��������Ч���ݳ���
    u8 NowRecvbuf;    //��ǰʹ�õĻ�����
    u8 NoCopyBufNum;//δ��������������
} *pUartPollCB[CN_UART_NUM];

static u8 UART_Mode[CN_UART_NUM];
static struct UartGeneralCB *pUartCB[CN_UART_NUM];

//���ڱ�ʶ�����Ƿ��ʼ����ǣ���0λ��ʾUART0����һλ��UART1....
//�������ƣ�1��ʾ��ʼ����0��ʾδ��ʼ��
static u8 sUartInited = 0;

// =============================================================================
static ptu32_t UART_ISR(ptu32_t port);
static uint32_t UART_DmaRx_ISR(ptu32_t port);
static uint32_t UART_DmaTx_ISR(ptu32_t port);
uint32_t UART_Poll_DmaTx_ISR(ptu32_t port);
uint32_t UART_Poll_DmaRx_ISR(ptu32_t port);
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
static void __UART_ComConfig(tagUartReg volatile *Reg,u32 port,struct COMParam *COM)
{
    if((COM == NULL) || (Reg == NULL))
        return;
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
            switch(COM->Parity)
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
//    Int_SettoReal(IntLine);
    Int_ClearLine(IntLine);
    Int_SetIsrPara(IntLine,SerialNo);
    Int_RestoreAsynLine(IntLine);
    Int_SetIsrPara(IntLine,SerialNo);   //todo:���ϼ������Ƿ���Ҫ
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
    tg_UART_Reg[SerialNo]->CR1 = 0;
    tg_UART_Reg[SerialNo]->CR3 |= (1<<14);
    tg_UART_Reg[SerialNo]->CR1 = 0x802d;
    tg_UART_Reg[SerialNo]->CR2 = 0x0;
    tg_UART_Reg[SerialNo]->CR3 = 0x0000;
    __UART_BaudSet(tg_UART_Reg[SerialNo],SerialNo,115200);
    DJY_DelayUs(100);//
    tg_UART_Reg[SerialNo]->ICR|=(1<<4);
    Board_UartHalfDuplexRecv(SerialNo);
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
u32 __UART_DMA_SendStart(u32 port)     //uart5û�ж�ӦDMA
{
    u32 num,addr;
    u8 *DmaSendBuf;
    u32 DmaSendBufLen;

    switch(port)
    {
        case CN_UART1:
            DmaSendBufLen = UART1_DmaBufLen; break;
        case CN_UART2:
            DmaSendBufLen = UART2_DmaBufLen; break;
        case CN_UART3:
            DmaSendBufLen = UART3_DmaBufLen; break;
        case CN_UART4:
            DmaSendBufLen = UART4_DmaBufLen; break;
        case CN_UART5:
            DmaSendBufLen = UART5_DmaBufLen; break;
         default:   break;
    }

    DmaSendBuf = pUART_DmaSendBuf[port];
//    if(true == __uart_dma_timeout(s_UART_DmaSending[port]))
    if(true ==s_UART_DmaSending[port])
        return 0;
    num = UART_PortRead(pUartCB[port],DmaSendBuf,DmaSendBufLen);
    addr = (u32)DmaSendBuf;

    if(num > 0)
    {
        DMA_Enable(UartDmaTxChannel[port],addr,num); //����DMA
        s_UART_DmaSending[port] = true;
    }
    else
    {
        tg_UART_Reg[port]->CR1 |= (1<<6);
    }
    return 0;
}

// =============================================================================
// ����: �������ڷ��ͣ���Ŀ���Ǵ����жϣ����жϷ�ʽ�������ݡ�
// ����: Reg,�������Ĵ��ڼĴ���ָ��.
// ����: ���͵ĸ���
// =============================================================================
static u32 __UART_SendStart (tagUartReg *Reg)
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
    Reg->CR1 &=~(1<<6);
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
        DmaBufLen = UART1_DmaBufLen;   break;
    case CN_UART2:
        DmaBufLen = UART2_DmaBufLen;   break;
    case CN_UART3:
        DmaBufLen = UART3_DmaBufLen;   break;
    case CN_UART4:
        DmaBufLen = UART4_DmaBufLen;   break;
    case CN_UART5:
        DmaBufLen = UART5_DmaBufLen;   break;
    default :        break;
    }

    if(pUART_DmaRecvBuf[port]==NULL)  //�����ڴ�
    {
        pUART_DmaRecvBuf[port] = (u8*)M_Malloc(DmaBufLen,0);
        if(pUART_DmaRecvBuf[port] == NULL)
            goto exit_from_Rcvbuf;
    }
    if(pUART_DmaSendBuf[port]==NULL)
    {
        pUART_DmaSendBuf[port] = (u8*)M_Malloc(DmaBufLen,0);
        if(pUART_DmaSendBuf[port] == NULL)
            goto exit_from_Sendbuf;
    }
    if(UART_Mode[port] == CN_UART_POLL)
    {
        if(pUartPollCB[port]==NULL)
        {
            pUartPollCB[port]= (struct DivPollCB *)M_Malloc(sizeof( struct DivPollCB),0);
            if(pUartPollCB[port]==NULL)
                goto exit_from_Poll;
            memset(pUartPollCB[port],0x00,sizeof(struct DivPollCB));
        }
        pUartPollCB[port]->DmaRecvBuf[0]=pUART_DmaRecvBuf[port];

        if(pUartPollCB[port]->DmaRecvBuf[1]==NULL)
        {
            pUartPollCB[port]->DmaRecvBuf[1] = (u8*)M_Malloc(DmaBufLen,0);
            if(pUartPollCB[port]->DmaRecvBuf[1] == NULL)
                goto exit_from_Rcvbuf2;
        }
        if(pUartPollCB[port]->DmaRecvBuf[2]==NULL)
        {
            pUartPollCB[port]->DmaRecvBuf[2] = (u8*)M_Malloc(DmaBufLen,0);
            if(pUartPollCB[port]->DmaRecvBuf[2] == NULL)
                goto exit_from_Rcvbuf2;
        }
        if(pUartPollCB[port]->DmaRecvBuf[3]==NULL)
        {
            pUartPollCB[port]->DmaRecvBuf[3] = (u8*)M_Malloc(DmaBufLen,0);
            if(pUartPollCB[port]->DmaRecvBuf[3] == NULL)
                goto exit_from_Rcvbuf2;
        }
        if(pUartPollCB[port]->DmaRecvBuf[4]==NULL)
        {
           pUartPollCB[port]->DmaRecvBuf[4] = (u8*)M_Malloc(DmaBufLen,0);
           if(pUartPollCB[port]->DmaRecvBuf[4] == NULL)
               goto exit_from_Rcvbuf2;
        }
        pUartPollCB[port]->NowRecvbuf = 0;
        pUartPollCB[port]->NoCopyBufNum = 0;
        free(pUART_DmaSendBuf[port]); //pollģʽ���ͻ��������ڷ���ʱ��λ��Ӧ�ò�����ݻ�����
    }

    DmaRecvBuf = pUART_DmaRecvBuf[port];
    DmaSendBuf = pUART_DmaSendBuf[port];
    __UART_RxIntDisable(CN_DMA_UNUSED,port); //�����ж�ʧ��
    __UART_TxIntDisable(CN_DMA_UNUSED,port);
    tg_UART_Reg[port]->CR1 |= (1<<4);        //ʹ��IDLE�ж�

    DMA_Config(UartDmaRxChannel[port],(u32)&(tg_UART_Reg[port]->RDR),  //DMA����
            (u32)DmaRecvBuf,DMA_DIR_P2M,DMA_DATABITS_8,DMA_DATABITS_8,DmaBufLen);

    DMA_Config(UartDmaTxChannel[port],(u32)&(tg_UART_Reg[port]->TDR),
            (u32)DmaSendBuf,DMA_DIR_M2P,DMA_DATABITS_8,DMA_DATABITS_8,DmaBufLen);

    tg_UART_Reg[port]->CR3 |= 0x00c0;  //���ô���ʹ��DMA�շ�
    s_UART_DmaUsed[port] = CN_DMA_USED;

    Int_Register(UartDmaTxInt[port]);  //ע�ᷢ��DMA�ж�
    Int_Register(UartDmaRxInt[port]);  //ע�����DMA�ж�

    Int_SetClearType(UartDmaRxInt[port],CN_INT_CLEAR_AUTO);  //�����ж�
    Int_SetClearType(UartDmaTxInt[port],CN_INT_CLEAR_AUTO);

    if(UART_Mode[port] == CN_UART_GENERAL)
    {
        Int_IsrConnect(UartDmaRxInt[port],UART_DmaRx_ISR);
        Int_IsrConnect(UartDmaTxInt[port],UART_DmaTx_ISR);
    }
    else
    {
        Int_IsrConnect(UartDmaRxInt[port],UART_Poll_DmaRx_ISR);
        Int_IsrConnect(UartDmaTxInt[port],UART_Poll_DmaTx_ISR);
    }

    Int_SettoAsynSignal(UartDmaRxInt[port]);
    Int_SettoAsynSignal(UartDmaTxInt[port]);

//    Int_SettoReal(UartDmaRxInt[port]);
//    Int_SettoReal(UartDmaTxInt[port]);

    Int_SetIsrPara(UartDmaRxInt[port],port);
    Int_SetIsrPara(UartDmaTxInt[port],port);

    Int_RestoreAsynLine(UartDmaRxInt[port]);
    Int_RestoreAsynLine(UartDmaTxInt[port]);

    Int_ClearLine(UartDmaTxInt[port]);
    Int_ClearLine(UartDmaRxInt[port]);

    DMA_Enable(UartDmaRxChannel[port],(u32)DmaRecvBuf,DmaBufLen);  //����DMA����
    return ;

    exit_from_Rcvbuf2:
        free(pUartPollCB[port]);
    exit_from_Poll:
        free(pUART_DmaSendBuf[port]);
        pUART_DmaSendBuf[port]=NULL;
    exit_from_Sendbuf:
        free(pUART_DmaRecvBuf[port]);
        pUART_DmaRecvBuf[port]=NULL;
    exit_from_Rcvbuf:
        return;
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

    free(pUART_DmaRecvBuf[port]);
    pUART_DmaRecvBuf[port]=NULL;
    free(pUART_DmaSendBuf[port]);
    pUART_DmaSendBuf[port]=NULL;

    tg_UART_Reg[port]->CR3 &= ~0x00c0;  //���ò�ʹ��dma����
    Int_RestoreAsynLine(IntLine);
    if(s_UART_DmaUsed[port] == CN_DMA_USED)
    {
        Int_SaveAsynLine(UartDmaRxInt[port]);
        Int_SaveAsynLine(UartDmaTxInt[port]);
        s_UART_DmaUsed[port] = CN_DMA_UNUSED;
    }
    __UART_RxIntEnable(CN_DMA_UNUSED,port);
    __UART_TxIntEnable(CN_DMA_UNUSED,port);
    tg_UART_Reg[port]->CR1 &= ~(1<<4); //disable idle int
}
// =============================================================================
// ����: �����豸�Ŀ��ƺ���,������Ӳ���Ĵ����������
// ����: Reg,�������Ĵ��ڼĴ���ָ��.
//       cmd,��������
//       data1,data2,������cmd����
// ����: ������.
// =============================================================================
static ptu32_t __UART_Ctrl(tagUartReg *Reg,u32 cmd, va_list *args)
{
    ptu32_t result = 0;
    u32 port;
    u32 len,len1,len2,len3;
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
            data = va_arg(*args, u32);
             __UART_BaudSet(Reg,port, data);
            break;
        }
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
            struct COMParam *com;
            com = va_arg(*args, struct COMParam *);
            __UART_ComConfig(Reg,port,com);
            break;
        }
        case CN_UART_SEND_BUF:
        {
            u32 mar;
            u16 ndtr;
            mar = va_arg(*args, u32);
            ndtr = va_arg(*args, u16);
            Board_UartHalfDuplexSend(port);
            DMA_Enable(UartDmaTxChannel[port],mar,ndtr);
            result = ndtr;
            break;
        }
        case CN_UART_RECV_BUF:
            switch (pUartPollCB[port]->NoCopyBufNum)
            {
                u8 *buf;
                s32 rcvlen;
                buf = va_arg(*args, char *);
                rcvlen = va_arg(*args, s32);
                case 1:
                    switch (pUartPollCB[port]->NowRecvbuf)
                    {
                       case 0:memcpy(buf,pUartPollCB[port]->DmaRecvBuf[4],rcvlen);
                              break;
                       case 1:memcpy(buf,pUartPollCB[port]->DmaRecvBuf[0],rcvlen);
                        break;
                       case 2:memcpy(buf,pUartPollCB[port]->DmaRecvBuf[1],rcvlen);
                              break;
                       case 3:memcpy(buf,pUartPollCB[port]->DmaRecvBuf[2],rcvlen);
                              break;
                       case 4:memcpy(buf,pUartPollCB[port]->DmaRecvBuf[3],rcvlen);
                              break;
                       default:  break;
                    }
                    break;
                case 2:
                    switch (pUartPollCB[port]->NowRecvbuf)
                        {
                       case 0: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[3]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[3],len);
                               memcpy((buf+pUartPollCB[port]->RcvBufLen[3]),pUartPollCB[port]->DmaRecvBuf[4],rcvlen-len);
                               break;
                       case 1: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[4]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[4],len);
                               memcpy((buf+pUartPollCB[port]->RcvBufLen[4]),pUartPollCB[port]->DmaRecvBuf[0],rcvlen-len);
                               break;
                       case 2: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[0]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[0],len);
                               memcpy((buf+pUartPollCB[port]->RcvBufLen[0]),pUartPollCB[port]->DmaRecvBuf[1],rcvlen-len);
                               break;
                       case 3: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[1]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[1],len);
                               memcpy((buf+pUartPollCB[port]->RcvBufLen[1]),pUartPollCB[port]->DmaRecvBuf[2],rcvlen-len);
                               break;
                       case 4: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[2]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[2],len);
                               memcpy((buf+pUartPollCB[port]->RcvBufLen[2]),pUartPollCB[port]->DmaRecvBuf[3],rcvlen-len);
                               break;
                       default:  break;
                    }
                    break;
                case 3:
                    switch (pUartPollCB[port]->NowRecvbuf)
                    {
                       case 0: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[2]);
                               len1 = MIN(rcvlen-len,pUartPollCB[port]->RcvBufLen[3]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[2],len);
                               memcpy((buf+len),pUartPollCB[port]->DmaRecvBuf[3],len1);
                               memcpy((buf+len+len1),pUartPollCB[port]->DmaRecvBuf[4],rcvlen-len-len1);
                               break;
                       case 1: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[3]);
                               len1 = MIN(rcvlen-len,pUartPollCB[port]->RcvBufLen[4]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[3],len);
                               memcpy((buf+len),pUartPollCB[port]->DmaRecvBuf[4],len1);
                               memcpy((buf+len+len1),pUartPollCB[port]->DmaRecvBuf[0],rcvlen-len-len1);
                               break;
                       case 2: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[4]);
                               len1 = MIN(rcvlen-len,pUartPollCB[port]->RcvBufLen[0]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[4],len);
                               memcpy((buf+len),pUartPollCB[port]->DmaRecvBuf[0],len1);
                               memcpy((buf+len+len1),pUartPollCB[port]->DmaRecvBuf[1],rcvlen-len-len1);
                               break;
                       case 3: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[0]);
                               len1 = MIN(rcvlen-len,pUartPollCB[port]->RcvBufLen[1]);
                            memcpy(buf,pUartPollCB[port]->DmaRecvBuf[0],len);
                               memcpy((buf+len),pUartPollCB[port]->DmaRecvBuf[1],len1);
                               memcpy((buf+len+len1),pUartPollCB[port]->DmaRecvBuf[2],rcvlen-len-len1);
                               break;
                       case 4: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[1]);
                               len1 = MIN(rcvlen-len,pUartPollCB[port]->RcvBufLen[2]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[1],len);
                               memcpy((buf+len),pUartPollCB[port]->DmaRecvBuf[2],len1);
                               memcpy((buf+len+len1),pUartPollCB[port]->DmaRecvBuf[3],rcvlen-len-len1);
                               break;
                       default:  break;
                        }
                    break;
                case 4:
                    switch (pUartPollCB[port]->NowRecvbuf)
                        {
                       case 0: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[1]);
                               len1 = MIN(rcvlen-len,pUartPollCB[port]->RcvBufLen[2]);
                               len2 = MIN(rcvlen-len-len1,pUartPollCB[port]->RcvBufLen[3]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[1],len);
                               memcpy((buf+len),pUartPollCB[port]->DmaRecvBuf[2],len1);
                               memcpy((buf+len+len1),pUartPollCB[port]->DmaRecvBuf[3],len2);
                               memcpy((buf+len+len1+len2),pUartPollCB[port]->DmaRecvBuf[4],rcvlen-len-len1-len2);
                               break;
                       case 1: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[2]);
                               len1 = MIN(rcvlen-len,pUartPollCB[port]->RcvBufLen[3]);
                               len2 = MIN(rcvlen-len-len1,pUartPollCB[port]->RcvBufLen[4]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[2],len);
                               memcpy((buf+len),pUartPollCB[port]->DmaRecvBuf[3],len1);
                               memcpy((buf+len+len1),pUartPollCB[port]->DmaRecvBuf[4],len2);
                               memcpy((buf+len+len1+len2),pUartPollCB[port]->DmaRecvBuf[0],rcvlen-len-len1-len2);
                               break;
                       case 2: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[3]);
                               len1 = MIN(rcvlen-len,pUartPollCB[port]->RcvBufLen[4]);
                               len2 = MIN(rcvlen-len-len1,pUartPollCB[port]->RcvBufLen[0]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[3],len);
                               memcpy((buf+len),pUartPollCB[port]->DmaRecvBuf[4],len1);
                               memcpy((buf+len+len1),pUartPollCB[port]->DmaRecvBuf[0],len2);
                               memcpy((buf+len+len1+len2),pUartPollCB[port]->DmaRecvBuf[1],rcvlen-len-len1-len2);
                               break;
                       case 3: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[4]);
                               len1 = MIN(rcvlen-len,pUartPollCB[port]->RcvBufLen[0]);
                               len2 = MIN(rcvlen-len-len1,pUartPollCB[port]->RcvBufLen[1]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[4],len);
                               memcpy((buf+len),pUartPollCB[port]->DmaRecvBuf[0],len1);
                               memcpy((buf+len+len1),pUartPollCB[port]->DmaRecvBuf[1],len2);
                               memcpy((buf+len+len1+len2),pUartPollCB[port]->DmaRecvBuf[2],rcvlen-len-len1-len2);
                               break;
                       case 4: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[0]);
                               len1 = MIN(rcvlen-len,pUartPollCB[port]->RcvBufLen[1]);
                               len2 = MIN(rcvlen-len-len1,pUartPollCB[port]->RcvBufLen[2]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[0],len);
                               memcpy((buf+len),pUartPollCB[port]->DmaRecvBuf[1],len1);
                               memcpy((buf+len+len1),pUartPollCB[port]->DmaRecvBuf[2],len2);
                               memcpy((buf+len+len1+len2),pUartPollCB[port]->DmaRecvBuf[3],rcvlen-len-len1-len2);
                               break;
                       default:  break;
                        }
                        break;
                case 5:
                    switch (pUartPollCB[port]->NowRecvbuf)
                    {
                       case 0: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[0]);
                               len1 = MIN(rcvlen-len,pUartPollCB[port]->RcvBufLen[1]);
                               len2 = MIN(rcvlen-len-len1,pUartPollCB[port]->RcvBufLen[2]);
                               len3 = MIN(rcvlen-len-len1-len2,pUartPollCB[port]->RcvBufLen[3]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[0],len);
                               memcpy((buf+len),pUartPollCB[port]->DmaRecvBuf[1],len1);
                               memcpy((buf+len+len1),pUartPollCB[port]->DmaRecvBuf[2],len2);
                               memcpy((buf+len+len1+len2),pUartPollCB[port]->DmaRecvBuf[3],len3);
                               memcpy((buf+len+len1+len2+len3),pUartPollCB[port]->DmaRecvBuf[4],rcvlen-len-len1-len2-len3);
                               break;
                       case 1: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[1]);
                               len1 = MIN(rcvlen-len,pUartPollCB[port]->RcvBufLen[2]);
                               len2 = MIN(rcvlen-len-len1,pUartPollCB[port]->RcvBufLen[3]);
                               len3 = MIN(rcvlen-len-len1-len2,pUartPollCB[port]->RcvBufLen[4]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[1],len);
                               memcpy((buf+len),pUartPollCB[port]->DmaRecvBuf[2],len1);
                               memcpy((buf+len+len1),pUartPollCB[port]->DmaRecvBuf[3],len2);
                               memcpy((buf+len+len1+len2),pUartPollCB[port]->DmaRecvBuf[4],len3);
                               memcpy((buf+len+len1+len2+len3),pUartPollCB[port]->DmaRecvBuf[0],rcvlen-len-len1-len2-len3);
                               break;
                       case 2: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[2]);
                               len1 = MIN(rcvlen-len,pUartPollCB[port]->RcvBufLen[3]);
                               len2 = MIN(rcvlen-len-len1,pUartPollCB[port]->RcvBufLen[4]);
                               len3 = MIN(rcvlen-len-len1-len2,pUartPollCB[port]->RcvBufLen[0]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[2],len);
                               memcpy((buf+len),pUartPollCB[port]->DmaRecvBuf[3],len1);
                               memcpy((buf+len+len1),pUartPollCB[port]->DmaRecvBuf[4],len2);
                               memcpy((buf+len+len1+len2),pUartPollCB[port]->DmaRecvBuf[0],len3);
                               memcpy((buf+len+len1+len2+len3),pUartPollCB[port]->DmaRecvBuf[1],rcvlen-len-len1-len2-len3);
                               break;
                       case 3: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[3]);
                               len1 = MIN(rcvlen-len,pUartPollCB[port]->RcvBufLen[4]);
                               len2 = MIN(rcvlen-len-len1,pUartPollCB[port]->RcvBufLen[0]);
                               len3 = MIN(rcvlen-len-len1-len2,pUartPollCB[port]->RcvBufLen[1]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[3],len);
                               memcpy((buf+len),pUartPollCB[port]->DmaRecvBuf[4],len1);
                               memcpy((buf+len+len1),pUartPollCB[port]->DmaRecvBuf[0],len2);
                               memcpy((buf+len+len1+len2),pUartPollCB[port]->DmaRecvBuf[1],len3);
                               memcpy((buf+len+len1+len2+len3),pUartPollCB[port]->DmaRecvBuf[2],rcvlen-len-len1-len2-len3);
                               break;
                       case 4: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[4]);
                               len1 = MIN(rcvlen-len,pUartPollCB[port]->RcvBufLen[0]);
                               len2 = MIN(rcvlen-len-len1,pUartPollCB[port]->RcvBufLen[1]);
                               len3 = MIN(rcvlen-len-len1-len2,pUartPollCB[port]->RcvBufLen[2]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[4],len);
                               memcpy((buf+len),pUartPollCB[port]->DmaRecvBuf[0],len1);
                               memcpy((buf+len+len1),pUartPollCB[port]->DmaRecvBuf[1],len2);
                               memcpy((buf+len+len1+len2),pUartPollCB[port]->DmaRecvBuf[2],len3);
                               memcpy((buf+len+len1+len2+len3),pUartPollCB[port]->DmaRecvBuf[3],rcvlen-len-len1-len2-len3);
                               break;
                    default:                break;
                }
                break;
                default:    break;
            }
            pUartPollCB[port]->NoCopyBufNum = 0;  //��0
//            pUartPollCB[port]->RcvBufLen[0] = 0;
//            pUartPollCB[port]->RcvBufLen[1] = 0;
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
        DmaSendBufLen = UART1_DmaBufLen;        break;
    case CN_UART2:
        DmaSendBufLen = UART2_DmaBufLen;        break;
    case CN_UART3:
        DmaSendBufLen = UART3_DmaBufLen;        break;
    case CN_UART4:
        DmaSendBufLen = UART4_DmaBufLen;        break;
    case CN_UART5:
        DmaSendBufLen = UART5_DmaBufLen;        break;
    default:        break;
    }

    DmaSendBuf = pUART_DmaSendBuf[port];
    DMA_ClearIntFlag(UartDmaTxChannel[port]);
    num = UART_PortRead(pUartCB[port],DmaSendBuf,DmaSendBufLen);
    if(num == 0)
    {
        s_UART_DmaSending[port] = false;
        tg_UART_Reg[port]->CR1 |= (1<<6);
    }
    else
    {
        DMA_Enable(UartDmaTxChannel[port],(u32)DmaSendBuf,num);
    }
    return 0;
}
// =============================================================================
// ����: ��uart2���ó�dma��ʽ�ǣ�dma1 ch7�����ͣ�ch6�������
// ����: �жϺ���û�в���.
// ����: �жϺ���û�з���ֵ.
// =============================================================================
uint32_t UART_Poll_DmaTx_ISR(ptu32_t port)
{

    DMA_ClearIntFlag(UartDmaTxChannel[port]);
    s_UART_DmaSending[port] = false;
    UART_PollPortRead(pUartCB[port]);
    //todo:Ϊ������л����ٶ����ﲻ�ж�TC�ж����
    //��IDLE�жϵ�ʱ�������4M������һ���ֽ�
    Board_UartHalfDuplexRecv(port);
    return 0;
}
// =============================================================================
// ����: CN_UART_POLLģʽ��DMA�����жϴ�����
//      �����ж�����������������ƹ�һ����ģʽ
// ����:
// ����:
// =============================================================================
uint32_t UART_Poll_DmaRx_ISR(ptu32_t port)
{
    u32 recvs,num,DmaRecvBufLen;
    DMA_Channel_TypeDef *DMA_Channelx;
    u8 channelx;
    u32 ifcr;
    DMA_TypeDef *DMAx;
    u8 RcvBufNum;
    switch(port)
    {
    case CN_UART1:
        DmaRecvBufLen = UART1_DmaBufLen;        break;
    case CN_UART2:
        DmaRecvBufLen = UART2_DmaBufLen;        break;
    case CN_UART3:
        DmaRecvBufLen = UART3_DmaBufLen;        break;
    case CN_UART4:
        DmaRecvBufLen = UART4_DmaBufLen;        break;
    case CN_UART5:
        DmaRecvBufLen = UART5_DmaBufLen;        break;
    default:        break;
    }

    DMA_Channelx = UartDmaRxChannel[port];
    if((u32)DMA_Channelx>(u32)DMA2)   //�õ���ǰstream������DMA2����DMA1
        DMAx=DMA2;
    else
        DMAx=DMA1;
    channelx=((((u32)DMA_Channelx-(u32)DMAx)-0X08)/0X14)+1;  //�õ�ͨ����
    ifcr = 0x0000000f<<((channelx-1)*4);

    if(0 == DmaRecvBufLen - UartDmaRxChannel[port]->CNDTR)
        return 0;


    RcvBufNum = pUartPollCB[port]->NowRecvbuf;


    switch (RcvBufNum)  //�������л�
    {
       case 0:pUartPollCB[port]->NowRecvbuf = 1;
              break;
       case 1:pUartPollCB[port]->NowRecvbuf = 2;
              break;
       case 2:pUartPollCB[port]->NowRecvbuf = 3;
              break;
       case 3:pUartPollCB[port]->NowRecvbuf = 4;
              break;
       case 4:pUartPollCB[port]->NowRecvbuf = 0;
              break;
       default:  break;
    }

    atom_low_t atom_low = Int_LowAtomStart(); //����ԭ�Ӳ���״̬
    DMA_Channelx->CCR&=~(1<<0);               //�ر�DMA����
    while(DMA_Channelx->CCR&0X1);             //ȷ��DMA���Ա�����
    DMAx->IFCR|=ifcr;     //ǰ4λ��1���㣬��������
    recvs = DmaRecvBufLen - UartDmaRxChannel[port]->CNDTR;
    DMA_Channelx->CNDTR=DmaRecvBufLen;        //DMA ������
    DMA_Channelx->CMAR =(u32)pUartPollCB[port]->DmaRecvBuf[pUartPollCB[port]->NowRecvbuf]; //DMA�ڴ��ַ
    DMA_Channelx->CCR|= (1<<0);     //����DMA����
    Int_LowAtomEnd(atom_low);

    if(pUartPollCB[port]->NoCopyBufNum < 5)
    {
        pUartPollCB[port]->NoCopyBufNum++;
    }
    else
    {
        UART_ErrHandle(pUartCB[port],CN_UART_FIFO_OVER_ERR);
    }

    pUartPollCB[port]->RcvBufLen[RcvBufNum] = recvs;
    num = UART_PollPortWrite(pUartCB[port],recvs);
    if(num != recvs)
    {
        UART_ErrHandle(pUartCB[port],CN_UART_BUF_OVER_ERR);
    }
    return 0;
}
// =============================================================================
// ����: ��uart1���ó�dma��ʽ�ǣ�dma2 ch2�����ͣ�ch7�������
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
        DmaRecvBufLen = UART1_DmaBufLen;        break;
    case CN_UART2:
        DmaRecvBufLen = UART2_DmaBufLen;        break;
    case CN_UART3:
        DmaRecvBufLen = UART3_DmaBufLen;        break;
    case CN_UART4:
        DmaRecvBufLen = UART4_DmaBufLen;        break;
    case CN_UART5:
        DmaRecvBufLen = UART5_DmaBufLen;        break;
    default:        break;
    }

    DmaRecvBuf = pUART_DmaRecvBuf[port];
    DMA_Disable(UartDmaRxChannel[port]);
    DMA_ClearIntFlag(UartDmaRxChannel[port]);
    recvs = DmaRecvBufLen - UartDmaRxChannel[port]->CNDTR;
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
u32 CNT1=0,CNT2=0,CNT3=0;
u32 UART_ISR(ptu32_t port)
{
    struct UartGeneralCB *UCB;
    tagUartReg *Reg;
    u32 num;
    u8 ch;

    UCB = (struct UartGeneralCB*)pUartCB[port];
    Reg = (tagUartReg *)tg_UART_Reg[port];
    if( (Reg->ISR & (1<<5)) &&(Reg->CR1 &(1<<5))) //�����ж�
    {
        ch = (u8)Reg->RDR;

//        extern void ReceiveData(u8 data);
//        ReceiveData(ch);

        num = UART_PortWrite(UCB,&ch,1);
        if(num != 1)
        {
            UART_ErrHandle(UCB,CN_UART_BUF_OVER_ERR);
        }
    }
    if((Reg->ISR & (1<<6)) && (Reg->CR1&(1<<6)))
    {
        Reg->CR1 &=~(1<<6);//��TC�ж�
        Board_UartHalfDuplexRecv(port);
    }

    if((Reg->ISR & (1<<7)) && (Reg->CR1 & (1<<7))) //�����ж�
    {
        num = UART_PortRead(UCB,&ch,1);
        if(num != 0)
            Reg->TDR = ch;
        else
        {
            Reg->CR1 &= ~(1<<7);
            Reg->CR1 |= (1<<6);
        }
    }

    if(Reg->ISR & (1<<3) )   //ORE���ش���
    {
        CNT3++;
        ch = (u8)Reg->RDR;
        num = Reg->ISR;      //��һ��sr�Ĵ���
        UART_ErrHandle(UCB,CN_UART_FIFO_OVER_ERR);
        Reg->ICR |=(1<<3);   //����ر�־λ
    }

    if((Reg->ISR & (1<<4)) && (Reg->CR1 & (1<<4)))  //IDLE
    {

        CNT1++;
        Reg->ICR |=(1<<4);//todo
        if(UART_Mode[port] == CN_UART_POLL)
        {
            UART_Poll_DmaRx_ISR(port);
        }
        else
            UART_DmaRx_ISR(port);
    }

    if(Reg->ISR & (1<<1))//֡����ֱ�������ʱ��������
    {
        CNT2++;
        Reg->ICR |=(1<<1);
    }
    return 0;
}

// =============================================================================
// ���ܣ���ʼ��uart1ģ��,��װ�ش����豸�������uart.c��API�������ú�����Ҫ��ɵĹ���
//       ���£�
//       1.��ʼ�����ڵ�Ӳ�����֣���GPIO���жϡ��������õ�COM����������baud = 115200
//         stop bits = 1,����żУ��, data bits = 8
//       2.����uart.c��API����UART_InstallPort��װ�봮���豸
// ������serial_no�����ںţ�1~4
//       SendBufLen����mode==CN_UART_GENERAL�����Ƿ��ͻ��λ���������
//                   ��mode==CN_UART_POLL�����Ƿ������ݰ�����󳤶�
//       RecvBufLen����mode==CN_UART_GENERAL�����ǽ��ջ��λ���������
//                   ��mode==CN_UART_POLL�����ǽ������ݰ�����󳤶�
//       mode :���ڹ���ģʽ���ο�CN_UART_GENERAL��CN_UART_POLL��˵��
// ���أ�1=�ɹ���0=ʧ��
// =============================================================================
ptu32_t ModuleInstall_UART(u32 serial_no,u32 SendBufLen, u32 RecvBufLen,u8 mode)
{
    struct UartParam UART_Param;
    u32 Maxbuflen = 2*MAX(SendBufLen,RecvBufLen);

    switch(serial_no)
    {
        case CN_UART1://����1
            UART_Param.Name         = "UART1";
            if(mode == CN_UART_POLL)
                UART1_DmaBufLen     = Maxbuflen;
            break;
        case CN_UART2://����2
            UART_Param.Name         = "UART2";
            if(mode == CN_UART_POLL)
                UART2_DmaBufLen     = Maxbuflen;
            break;
        case CN_UART3://����3
            UART_Param.Name         = "UART3";
            if(mode == CN_UART_POLL)
                UART3_DmaBufLen     = Maxbuflen;
            break;
        case CN_UART4://����4
            UART_Param.Name         = "UART4";
            if(mode == CN_UART_POLL)
                UART4_DmaBufLen     = Maxbuflen;
            break;
        case CN_UART5://����5
            UART_Param.Name         = "UART5";
            if(mode == CN_UART_POLL)
                UART5_DmaBufLen     = Maxbuflen;
            break;
        default:
            return 0;
    }
    UART_Param.TxRingBufLen = SendBufLen;
    UART_Param.RxRingBufLen = RecvBufLen;
    UART_Param.mode         = mode;
    UART_Param.Baud         = 115200;
    UART_Param.UartPortTag  = (ptu32_t)tg_UART_Reg[serial_no];
    UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
    UART_Param.StartSend    = (UartStartSend)__UART_SendStart;

    UART_Mode[serial_no]       = mode;
    pUART_DmaSendBuf[serial_no]= NULL;
    pUART_DmaRecvBuf[serial_no]= NULL;
    //Ӳ����ʼ��
    __UART_HardInit(serial_no);
    if(UART_Mode[serial_no] == CN_UART_POLL)
    {
        __UART_SetDmaUsed(serial_no);
    }

    __UART_IntInit(serial_no);  //�жϳ�ʼ������UART_ISR���ó�ʵʱ�жϣ�UART_ISR���з��͡����ա�IDLE��֡����

    sUartInited |= (0x01 << serial_no);
    if(CN_UART_POLL == mode)
        pUartCB[serial_no] = UART_InstallPoll(&UART_Param);
    else if(CN_UART_GENERAL == mode)
        pUartCB[serial_no] = UART_InstallGeneral(&UART_Param);

    if( pUartCB[serial_no] == NULL)
        return 0;

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
    u8 port;
    for(port=0;port<CN_UART_NUM;port++)
    {
        if(PutStrDirectReg ==tg_UART_Reg[port])
            break;
    }
    Board_UartHalfDuplexSend(port);
    CR_Bak = PutStrDirectReg->CR1;  //Save INT
    PutStrDirectReg->CR1 &= ~(1<<7);   //disable send INT
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
        PutStrDirectReg->TDR = str[result];
    }
    PutStrDirectReg->CR1 = CR_Bak;  //restore send INT
    while((PutStrDirectReg->ISR &(1<<6))!=(1<<6))
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

