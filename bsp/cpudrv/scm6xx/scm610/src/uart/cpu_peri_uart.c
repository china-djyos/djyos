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
// ������Ա: huzb
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
//#include "cpu_peri_dma.h"
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

#define CFG_UART7_SENDBUF_LEN            32                 //"UART7���ͻ��λ�������С",
#define CFG_UART7_RECVBUF_LEN            32                 //"UART7���ջ��λ�������С",
#define CFG_UART7_DMABUF_LEN             32                 //"UART7 DMA��������С",

#define CFG_UART8_SENDBUF_LEN            32                 //"UART8���ͻ��λ�������С",
#define CFG_UART8_RECVBUF_LEN            32                 //"UART8���ջ��λ�������С",
#define CFG_UART8_DMABUF_LEN             32                 //"UART8 DMA��������С",

//%$#@enum,true,false,
#define CFG_UART1_ENABLE                 false              //"�Ƿ�ʹ��UART1",
#define CFG_UART2_ENABLE                 true              //"�Ƿ�ʹ��UART2",
#define CFG_UART3_ENABLE                 false              //"�Ƿ�ʹ��UART3",
#define CFG_UART4_ENABLE                 false              //"�Ƿ�ʹ��UART4",
#define CFG_UART5_ENABLE                 false              //"�Ƿ�ʹ��UART5",
#define CFG_UART6_ENABLE                 false              //"�Ƿ�ʹ��UART6",
#define CFG_UART7_ENABLE                 false              //"�Ƿ�ʹ��UART7",
#define CFG_UART8_ENABLE                 false              //"�Ƿ�ʹ��UART8",
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================


#define CN_UART1_BASE (0x40300000)
#define CN_UART2_BASE (0x40310000)
#define CN_UART3_BASE (0x40320000)
#define CN_UART4_BASE (0x40330000)
#define CN_UART5_BASE (0x41340000)
#define CN_UART6_BASE (0x41350000)
#define CN_UART7_BASE (0x41360000)
#define CN_UART8_BASE (0x41370000)

//Uart Reg define

typedef struct
{
    volatile u32 TXDATA;
    volatile u32 RXDATA;
    volatile u32 STATUS;
    volatile u32 CTRL;
    volatile u32 INTSTA;
    volatile u32 BAUDDIV;

}tagUartReg;

static u32 TxByteTime;                    //��������һ���ֽ�����Ҫ��ʱ��
static tagUartReg *PutStrDirectReg;       //����printk���͵Ĵ��ڼĴ���
static tagUartReg *GetCharDirectReg;      //����ֱ�ӽ��յĴ��ڼĴ���
static u32 TxDirectPort;                  //����printk���͵Ĵ��ں�
static u32 RxDirectPort;                  //����ֱ�ӽ��յĴ��ں�
// =============================================================================
static tagUartReg volatile * const sgpt_UartReg[] = {(tagUartReg *)CN_UART1_BASE,
                                                    (tagUartReg *)CN_UART2_BASE,
                                                    (tagUartReg *)CN_UART3_BASE,
                                                    (tagUartReg *)CN_UART4_BASE,
                                                    (tagUartReg *)CN_UART5_BASE,
                                                    (tagUartReg *)CN_UART6_BASE,
                                                    (tagUartReg *)CN_UART7_BASE,
                                                    (tagUartReg *)CN_UART8_BASE};

#define CN_DMA_UNUSED   0
#define CN_DMA_USED     1
#define CN_UART_NUM     8

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
void __UART_RxIntEnable(u8 port)
{
    if(CN_UART1 <= port && port < CN_UART_MAX)
    {
        sgpt_UartReg[port]->CTRL |= (1<<3);
    }
}

// =============================================================================
// ����: ��ֹuart�Ľ����ж�
// ����: reg,�������ļĴ�����ָ��
//       port,���ں�
// ����: ��
// =============================================================================
void __UART_RxIntDisable(u8 port)
{
    if(CN_UART1 <= port && port < CN_UART_MAX)
    {
        sgpt_UartReg[port]->CTRL &= ~(1<<3);
    }
}

// =============================================================================
// ����: ʹ��uart�ķ����ж�
// ����: reg,�������ļĴ�����ָ��
//       port,���ں�
// ����: ��
// =============================================================================
static void __UART_TxIntEnable(u8 port)
{
    if(CN_UART1 <= port && port < CN_UART_MAX)
    {
        sgpt_UartReg[port]->CTRL |= (1<<2);
    }
}

// =============================================================================
// ����: ��ֹUART�ķ����ж�
// ����: Reg,�������ļĴ�����ָ��
//       port,���ں�
// ����: ��
// =============================================================================
static void __UART_TxIntDisable(u8 port)
{
    if(CN_UART1 <= port && port < CN_UART_MAX)
    {
        sgpt_UartReg[port]->CTRL &= ~(1<<2);
    }
}

// =============================================================================
// ����: ��鷢�ͼĴ�����״̬������վͷ���true����Ϊ�գ����򷵻�false
// ������reg,�������ļĴ�����ָ��
// ����: true = �������գ�false = �ǿ�
// =============================================================================
static bool_t __UART_TxTranEmpty(tagUartReg volatile *reg)
{
    bool_t bRet;

    if(reg->STATUS & (1<<5))
    {
        bRet = true;
    }else{
        bRet = false;
    }
    return bRet;
}

// =============================================================================
// ����: �����ռĴ�����״̬�������ݾͷ���true�����򷵻�false
// ������reg,�������ļĴ�����ָ��
// ����: true = �����ݣ�false = ������
// =============================================================================

static bool_t __UART_RxTranNotEmpty(tagUartReg volatile *reg)
{
    bool_t bRet;

    if(reg->STATUS & (1<<1))
    {
        bRet = false;
    }else{
        bRet =  true;
    }

    return bRet;
}

// =============================================================================
// ����: ���ô���baud,PCLKΪ25M��CPU��ƵΪ100M����,������ֻ�ṩ9600��19200��57600��
//       115200���ֲ���������
// ����: Reg,�������ļĴ�����ָ��
//       port,���ں�
//       baud,������
// ����: ��
// =============================================================================

#define CN_UART_CLK (37500000UL)

static void __UART_BaudSet(tagUartReg volatile *Reg,u32 port,u32 baud)
{
    if(CN_UART1 <= port && port < CN_UART_MAX)
    {
        sgpt_UartReg[port]->BAUDDIV = (u32)(CN_UART_CLK / baud);
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
    struct COMParam *COM;
    if((COM == NULL) || (Reg == NULL))
        return;
    __UART_BaudSet(Reg,port,COM->BaudRate);

    //����λ������-�̶�Ϊ8BIT

    switch(COM->Parity)                 // parity
    {

        case CN_UART_PARITY_NONE:       //ONOE PARITY
             Reg->CTRL &= ~(1<<4);
             break;
        case CN_UART_PARITY_ODD:
             Reg->CTRL |= (1<<4);
             Reg->CTRL &= ~(1<<7);
             break;
        case CN_UART_PARITY_EVEN:
             Reg->CTRL |= (1<<4);
             Reg->CTRL |= (1<<7);
             break;
        default:Reg->CTRL &= ~(1<<4);break;
    }

    //Stop Bit Config
    Reg->CTRL |= (1<<6);
    switch(COM->StopBits)               // stop bits
    {
        case CN_UART_STOPBITS_1:
             Reg->CTRL &= ~(1<<5);
             break;
        case CN_UART_STOPBITS_1_5:     //��֧��1.5��ֹͣλ
             ;break;
        case CN_UART_STOPBITS_2:
             Reg->CTRL |= (1<<5);
             break;
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
    if(CN_UART1 <= port && port < CN_UART_MAX)
    {
        //�����շ�ʹ��
        sgpt_UartReg[port]->CTRL      |= (1<<0) ;
        sgpt_UartReg[port]->CTRL      |= (1<<1) ;
    }
}

// =============================================================================
// ����: ʹ����ֹͣ����������ʱ�Ӻ�uartģ��
// ����: port,���ں�
// ����: ��
// =============================================================================

void __UART_Disable(u32 port)
{
    if(CN_UART1 <= port && port < CN_UART_MAX)
    {
        sgpt_UartReg[port]->CTRL      &= ~(1<<0) ;
        sgpt_UartReg[port]->CTRL      &= ~(1<<1) ;
    }
}


// =============================================================================
// ����: ��ʼ��UART��Ӧ���ж��ߣ�����ʼ���ж���ں���
// ������SerialNo,���ں�
// ����: ��
// =============================================================================
static void __UART_IntInit(u32 SerialNo)
{
    u8 IntLine;

    switch(SerialNo)
    {
        case CN_UART1:
             IntLine = CN_INT_LINE_UART0;
              break;
        case CN_UART2:
             IntLine = CN_INT_LINE_UART1;
              break;
        case CN_UART3:
             IntLine = CN_INT_LINE_UART2;
              break;
        case CN_UART4:
             IntLine = CN_INT_LINE_UART3;
              break;
        case CN_UART5:
             IntLine = CN_INT_LINE_UART4;
              break;
        case CN_UART6:
             IntLine = CN_INT_LINE_UART5;
              break;
        case CN_UART7:
             IntLine = CN_INT_LINE_UART6;
              break;
        case CN_UART8:
             IntLine = CN_INT_LINE_UART7;
              break;
    }

    u32 UART_ISR(ptu32_t port);
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
    if(SerialNo > CN_UART8)
        return;
    //ϵͳ��ʼ��ʱ�Ѿ�ʹ�жϴ��ڽ�ֹ״̬�������ٽ�ֹ������жϡ�
   //��ʼ��uartӲ���������ݽṹ
   //��λ��ʼ��
    sgpt_UartReg[SerialNo]->CTRL     = 0;
    sgpt_UartReg[SerialNo]->BAUDDIV  = 0;

    //��������żУ��λ
    sgpt_UartReg[SerialNo]->CTRL &= ~(1<<4);
    //����ֹͣλ1 bit
    sgpt_UartReg[SerialNo]->CTRL &= ~(1<<5);
    //����ֹͣλ���
    sgpt_UartReg[SerialNo]->CTRL |= (1<<6);

    //��Ϊ��֧�ַ��Ϳ��жϣ����﷢�Ͳ�ͨ���жϷ���,ֱ�Ӱѷ����жϽ�ֹ
    sgpt_UartReg[SerialNo]->CTRL &= ~(1<<2);

    //����ֻ�������ж�
    sgpt_UartReg[SerialNo]->CTRL |=  (1<<3);

    //ʹ���շ�
    sgpt_UartReg[SerialNo]->CTRL      |= (1<<0) ;
    sgpt_UartReg[SerialNo]->CTRL      |= (1<<1) ;

    //Ĭ�ϲ���������115200
    //sgpt_UartReg[SerialNo]->BAUDDIV = (uint32_t)(UART_CLK/115200);
    __UART_BaudSet(sgpt_UartReg[SerialNo],SerialNo,115200);
}

// =============================================================================
// ����: �������ڷ��ͣ���Ŀ���Ǵ����жϣ����жϷ�ʽ�������ݡ�
// ����: Reg,�������Ĵ��ڼĴ���ָ��.
// ����: ���͵ĸ���
// =============================================================================

static u32 __UART_SendStart (tagUartReg *Reg)
{
    u8 port;
    u32 delay=0,len;
    struct UartGeneralCB *UCB;
    u8 fifo[32];
    atom_low_t atom_low;

    switch((u32)Reg)
    {
        case CN_UART1_BASE:        port = CN_UART1;    break;
        case CN_UART2_BASE:        port = CN_UART2;    break;
        case CN_UART3_BASE:        port = CN_UART3;    break;
        case CN_UART4_BASE:        port = CN_UART4;    break;
        case CN_UART5_BASE:        port = CN_UART5;    break;
        case CN_UART6_BASE:        port = CN_UART6;    break;
        case CN_UART7_BASE:        port = CN_UART7;    break;
        case CN_UART8_BASE:        port = CN_UART8;    break;
        default:return 0;
    }

    UCB = pUartCB[port];

    Board_UartHalfDuplexSend(port);//�л�������
    __UART_TxIntDisable(port);

    //û�з��Ϳ��жϣ����﷢�Ͳ�ʹ���жϷ���

    while((len = UART_PortRead(UCB,&fifo[0],1)) != 0)
    {
         //Ϊ��֤���͹��̲����жϴ�ϣ�����ͼ�ԭ�Ӳ���
         atom_low = Int_LowAtomStart();
         //�ȴ�����fifo ��
         while(sgpt_UartReg[port]->STATUS & (1<<0));
         sgpt_UartReg[port]->TXDATA = fifo[0];
         while(!(sgpt_UartReg[port]->STATUS & (1<<5)));
         Int_LowAtomEnd(atom_low);
    }

//    __UART_TxIntEnable(port);

    return 1;
}

// =============================================================================
// ����: ��������DMA���ͣ�ʹ��DMA�������ڷ���ǰ���ж��ϴ�DMA�Ƿ��Ѿ�������ɣ�ͨ��
//       �鿴��־UARTx_DMA_SENDING���ж�UARTx�Ƿ��Ѿ��������
// ����: PORT,���ں�.
// ����: ���͵ĸ���
// =============================================================================

u32 __UART_DMA_SendStart(u32 port)
{

   return 0;
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
            case CN_UART7_BASE: port = CN_UART7;break;
            case CN_UART8_BASE: port = CN_UART8;break;
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
        case CN_UART_SET_BAUD:  //����Baud
        {
            u32 data;
            data = va_arg(*arg0, u32);
            __UART_BaudSet(Reg,port, data);
        }
            break;
        case CN_UART_EN_RTS:

            break;
        case CN_UART_DIS_RTS:

            break;
        case CN_UART_EN_CTS:

            break;
        case CN_UART_DIS_CTS:

            break;
        case CN_UART_DMA_USED:

            break;
        case CN_UART_DMA_UNUSED:

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
// ����: ����uartʹ��dma�շ�������stm32�����ڵ��շ�dmaͨ�����üĴ����������³�
//       ʼ������dma�Ĵ�������Ϊ:
//       1����ʹ��dma�ڼ�����������豸ʹ��dma���Ӷ����������á�
//       2����ʹuartʹ��dma�ڼ䣬Ҳ�������ģ����ϵ����⵼�������豸ʹ��dma.
// ����: port,�˿ںţ������ںţ���1~3
// ����: ��
// =============================================================================
void __UART_SetDmaUsed(u32 port)
{

}

//=============================================================================
// ����: ���ò�ʹ��DMA��UART��ʹ��DMA�շ���ÿ�շ�һ���ֽھͱ����ж�һ�Σ��������ڸ���
//      ����ͨ����ʹ�á�
// ����: port,�˿ںţ������ںţ�Ϊ1~3
// ����: ��
// =============================================================================

void __UART_SetDmaUnUsed(u32 port)
{

}



// =============================================================================
// ����: ��uart2���ó�dma��ʽ�ǣ�dma1 ch7�����ͣ�ch6�������
// ����: �жϺ���û�в���.
// ����: �жϺ���û�з���ֵ.
// =============================================================================
uint32_t UART_DmaTx_ISR(ptu32_t port)
{

    return 0;
}

// =============================================================================
// ����: ��uart2���ó�dma��ʽ�ǣ�dma1 ch7�����ͣ�ch6�������
// ����: �жϺ���û�в���.
// ����: �жϺ���û�з���ֵ.
// =============================================================================

uint32_t UART_DmaRx_ISR(ptu32_t port)
{
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
    Reg = (tagUartReg *)sgpt_UartReg[port];

    //�����ж�

    if(!(Reg->STATUS & (1<<1)))//�н���FiFo
    {
        //�����ջ���ǿ�
        while(!(Reg->STATUS & (1<<1)))
        {
            ch = (u8)Reg->RXDATA;
//            Reg->TXDATA = ch;
            num = UART_PortWrite(UCB,&ch,1);
            if(num != 1)
            {
                UART_ErrHandle(UCB,CN_UART_BUF_OVER_ERR);
            }
        }

        //������ж�
        Reg->INTSTA |= (1<<1);
    }

     //��Ϊ����û�з��Ϳ��жϣ��̷��Ͳ����ж�

    if(Reg->INTSTA & (1<<2))    //���մ���
    {
        printk("Rev Overrun\r\n");
        //����մ���
        Reg->INTSTA |= (1<<2);
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
     case CN_UART7://����6
        UART_Param.Name         = "UART7";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART7_BASE;
        UART_Param.TxRingBufLen = CFG_UART7_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART7_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
     case CN_UART8://����6
        UART_Param.Name         = "UART8";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART8_BASE;
        UART_Param.TxRingBufLen = CFG_UART8_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART8_RECVBUF_LEN;
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
    for(port=0;port<CN_UART_MAX;port++)
    {
        if(PutStrDirectReg ==sgpt_UartReg[port])
            break;
    }
    Board_UartHalfDuplexSend(port);
//    CR_Bak = PutStrDirectReg->CTRL;                          //Save INT
//    PutStrDirectReg->CTRL &= ~(1<<2);                        //disable send INT
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
        PutStrDirectReg->TXDATA = str[result];

        //�ȴ��������
        while(!(PutStrDirectReg->STATUS & (1<<5)));

    }

//    PutStrDirectReg->CTRL = CR_Bak;  //�ָ��ж�
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

    CR_Bak = GetCharDirectReg->CTRL;                          //Save INT
    PutStrDirectReg->CTRL &= ~(1<<2);                         //disable send INT
    while(__UART_RxTranNotEmpty(GetCharDirectReg) == false) ;
    result = GetCharDirectReg->RXDATA;
    PutStrDirectReg->CTRL = CR_Bak;                            //restore send INT
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

