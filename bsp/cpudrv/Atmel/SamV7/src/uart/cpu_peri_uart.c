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
// ģ������: UARTģ��ײ�Ӳ���������Ĵ�������Ĳ���
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 05/15.2014
// =============================================================================

#include "cpu_peri.h"
#include "stdio.h"
#include "stdlib.h"
#include <device/djy_uart.h>
#include "string.h"
#include "uart_dma.h"
#include "uart_dma.h"
#include "xdmad.h"
#include "xdmac.h"

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    ptu32_t ModuleInstall_UART(ptu32_t SerialNo);
//    #if CFG_UART0_ENABLE ==1
//    ModuleInstall_UART(CN_UART0);
//    #endif
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

//    #if CFG_UART4_ENABLE ==1
//    ModuleInstall_UART(CN_UART4);
//    #endif

//    #if CFG_USART0_ENABLE ==1
//    ModuleInstall_UART(CN_USART0);
//    #endif
//
//    #if CFG_USART1_ENABLE ==1
//    ModuleInstall_UART(CN_USART1);
//    #endif
//
//    #if CFG_USART2_ENABLE ==1
//    ModuleInstall_UART(CN_USART2);
//    #endif
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip uart"//CPU��uart��������
//parent:"uart device file"      //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                     //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                  //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                    //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                   //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                    //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"device file system","lock","uart device file","cpu onchip xdma","heap"//������������������������none����ʾ�������������
                                    //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                    //����������������������г����á�,���ָ�
//weakdependence:"none"             //��������������������������none����ʾ�������������
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
#define CFG_UART_SENDBUF_LEN     (256)          //"���ڷ��ͻ��λ�������С",
#define CFG_UART_RECVBUF_LEN     (256)          //"���ڽ��ջ��λ�������С",
#define CFG_UART_DMABUF_LEN      (64)           //"����DMA��������С",

//%$#@enum,true,false,
#define CFG_UART0_ENABLE         true           //"�Ƿ�����UART0",
#define CFG_UART1_ENABLE         false          //"�Ƿ�����UART1",
#define CFG_UART2_ENABLE         false          //"�Ƿ�����UART2",
#define CFG_UART3_ENABLE         false          //"�Ƿ�����UART3",
#define CFG_UART4_ENABLE         false          //"�Ƿ�����UART4",
#define CFG_USART0_ENABLE        false          //"�Ƿ�����USART0",
#define CFG_USART1_ENABLE        false          //"�Ƿ�����USART1",
#define CFG_USART2_ENABLE        false          //"�Ƿ�����USART2",
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================



// =============================================================================
//base address
#define CN_UART0_BASE   (0x400E0800)
#define CN_UART1_BASE   (0x400E0A00)
#define CN_UART2_BASE   (0x400E1A00)
#define CN_UART3_BASE   (0x400E1C00)
#define CN_UART4_BASE   (0x400E1E00)

#define CN_USART0_BASE  (0x40024000)
#define CN_USART1_BASE  (0x40028000)
#define CN_USART2_BASE  (0x4002C000)

static u32 TxByteTime;                    //��������һ���ֽ�����Ҫ��ʱ��
static tagUartReg *PutStrDirectReg;     //����printk���͵Ĵ��ڼĴ���
static tagUartReg *GetCharDirectReg;     //����ֱ�ӽ��յĴ��ڼĴ���
static u32 TxDirectPort;                  //����printk���͵Ĵ��ں�
static u32 RxDirectPort;                  //����ֱ�ӽ��յĴ��ں�
//==============================================================================
static tagUartReg volatile * const tg_UART_Reg[] = {(tagUartReg *)CN_UART0_BASE,
                                                    (tagUartReg *)CN_UART1_BASE,
                                                    (tagUartReg *)CN_UART2_BASE,
                                                    (tagUartReg *)CN_UART3_BASE,
                                                    (tagUartReg *)CN_UART4_BASE,
                                                    (tagUartReg *)CN_USART0_BASE,
                                                    (tagUartReg *)CN_USART1_BASE,
                                                    (tagUartReg *)CN_USART2_BASE};

//#define CFG_UART_SENDBUF_LEN  256
//#define CFG_UART_RECVBUF_LEN  256
//#define CFG_UART_DMABUF_LEN  64

static bool_t s_UART_DmaUsed[CN_UART_NUM]={0,0,0,0,0,0,0,0};

//����ʹ��DMAʱ��ʹ��LLI0��LLI1,����UARTû�г�ʱ�жϣ����UART ��LLI0 ���ȶ���Ϊ1;
//����UASART�г�ʱ�ж���ˣ���˽�LLI0�ĳ����ʵ�����
//������LLI�ĳ���ʱ��ʹ��32λ������16λ�洢LLI1�ĳ��ȣ���16λ�洢LLI0�ĳ��ȣ�������ʾ��
#define UART_LLI_LEN     (1 + ((CFG_UART_DMABUF_LEN-1) << 16) )
#define USART_LLI_LEN    ((CFG_UART_DMABUF_LEN - 16 ) + (16 << 16))
const u32 s_UART_DmaRcvLen[CN_UART_NUM] ={UART_LLI_LEN,UART_LLI_LEN,UART_LLI_LEN
        ,UART_LLI_LEN,UART_LLI_LEN,USART_LLI_LEN,USART_LLI_LEN,USART_LLI_LEN};

u32 s_UART_TimeOut[] = {115200,115200,115200};// USART��ʱ�ж�ʱ������

extern sXdmad dmad;
static UartDma Uartd0;
static UartChannel Uart0Tx, Uart0Rx;
static UartDma Uartd1;
static UartChannel Uart1Tx, Uart1Rx;
static UartDma Uartd2;
static UartChannel Uart2Tx, Uart2Rx;
static UartDma Uartd3;
static UartChannel Uart3Tx, Uart3Rx;
static UartDma Uartd4;
static UartChannel Uart4Tx, Uart4Rx;
static UartDma Usartd0;
static UartChannel Usart0Tx, Usart0Rx;
static UartDma Usartd1;
static UartChannel Usart1Tx, Usart1Rx;
static UartDma Usartd2;
static UartChannel Usart2Tx, Usart2Rx;

ptu32_t UART_ISR(ptu32_t IntLine);

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
//----ʹ�ܽ����ж�-------------------------------------------------------------
//����: ʹ��uart�Ľ����ж�,DMA�ж�ʹ�õ���endrx
//����: reg,�������ļĴ�����ָ��
//����: ��
//-----------------------------------------------------------------------------
static void __UART_RecvIntEnable(tagUartReg volatile *reg,u8 enable)
{
    if(enable)
        reg->UART_IER = (1<<0);//rxrdy int
    else
        reg->UART_IDR = (1<<0);//rxrdy int
}

//----ʹ�ܷ����ж�-------------------------------------------------------------
//����: ʹ��uart�ķ����ж�,DMA�ж�ʹ�õ���endtx �ж�
//����: reg,�������ļĴ�����ָ��
//����: ��
//-----------------------------------------------------------------------------
static void __UART_SendIntEnable(tagUartReg volatile *reg,u8 enable)
{
    if(enable)
        reg->UART_IER = (1<<9);//tx int
    else
        reg->UART_IDR = (1<<9);//tx int
}

//----ʹ�ܴ��ڷ��͹���------------------------------------------------------------
//����: ʹ�ܴ��ڿ������ķ������ݹ���
//����: reg,�������ļĴ�����ָ��
//      enable,1��ʹ�ܣ�0��ʧ��
//����: ��
//-----------------------------------------------------------------------------
static void __UART_TxEnable(tagUartReg volatile *reg,u8 enable)
{
    if(enable)
    {
        reg->UART_CR = (1<<6);//TXEN
    }
    else
    {
        reg->UART_CR = (1<<7);
    }
}

//----ʹ�ܴ��ڽ��չ���------------------------------------------------------------
//����: ʹ�ܴ��ڿ������Ľ������ݹ���
//����: reg,�������ļĴ�����ָ��
//      enable,1��ʹ�ܣ�0��ʧ��
//����: ��
//-----------------------------------------------------------------------------
static void __UART_RxEnable(tagUartReg volatile *reg,u8 enable)
{
    if(enable)
    {
        reg->UART_CR = (1<<4);//RXEN
    }
    else
    {
        reg->UART_CR = (1<<5);
    }
}
//----��鴮�ڷ��ͼĴ����Ƿ��-------------------------------------------------
//����: ��鷢�ͼĴ�����״̬������վͷ���true����Ϊ�գ�����λ�Ĵ�����thr��Ϊ��
//�������ޡ�
//����: true = �������գ�false = �ǿ�
//-----------------------------------------------------------------------------
static bool_t __UART_TxTranEmpty(volatile tagUartReg *reg)
{
    return (0 != (reg->UART_SR & (1<<9)));//TXR EMPTY
}

// =============================================================================
// ����: �����ռĴ�����״̬�������ݾͷ���true�����򷵻�false
// ������reg,�������ļĴ�����ָ��
// ����: true = �����ݣ�false = ������
// =============================================================================
static bool_t __UART_RxHadChar(tagUartReg volatile *reg)
{
    return reg->UART_SR & (0x01<<0);//RXRDY
}

//----����baud-----------------------------------------------------------------
//����: ���ô���baud,�����������裬ʱ��ԴΪMCK����150Mδ��Ƶ�����
//����: reg,�������ļĴ�����ָ��
//����: ��
//-----------------------------------------------------------------------------
static void __UART_BaudSet(tagUartReg volatile *reg,u32 baud)
{
    if(baud != 0)
    {
        reg->UART_BRGR = ((CN_CFG_MCLK/2)/baud)/16;
        if(tg_UART_Reg[TxDirectPort] == reg)
        {
            TxByteTime = 11000000/baud;     //1���ֽڴ���ʱ�䣬��10bit��+10%����
        }
    }
}

//----����COM-----------------------------------------------------------------
//����: ���ô�����صĲ��������������ʡ�����λ��ֹͣλ����У��λ
//����: reg,�������ļĴ�����ָ��
//����: ��
//ע�⣺ATMEL M7��UART�����޸�����λ��ֹͣλ�������ڼĴ���������ЩλԤ�������ö�UART��Ч
//     ��ֻ��USART��Ч
//-----------------------------------------------------------------------------
static void __UART_ComConfig(tagUartReg volatile *reg,struct COMParam *Com)
{
    u32 mr;
    __UART_TxEnable(reg,0);
    __UART_RxEnable(reg,0);

    __UART_BaudSet(reg,Com->BaudRate);

    mr = (reg->UART_MR) & ( ~(7 << 9));
    //��żУ��
    switch(Com->Parity)
    {
    case CN_UART_PARITY_NONE:
        mr |=  (4 << 9);
        break;
    case CN_UART_PARITY_ODD:
        mr |= (1 << 9);
        break;
    case CN_UART_PARITY_EVEN:
        mr |= (0 << 9);
        break;
    case CN_UART_PARITY_MARK:
        mr |= (3 << 9);
        break;
    case CN_UART_PARITY_SPACE:
        mr |= (2 << 9);
        break;
    default:
        break;
    }
    reg->UART_MR = mr;

    mr = (reg->UART_MR) & ( ~(3 << 6));
    //UART�����޸�databits
    switch(Com->DataBits)
    {
    case CN_UART_DATABITS_5:
        mr |= (0 << 6);
        break;
    case CN_UART_DATABITS_6:
        mr |= (1 << 6);
        break;
    case CN_UART_DATABITS_7:
        mr |= (2 << 6);
        break;
    case CN_UART_DATABITS_8:
        mr |= (3 << 6);
        break;
    default:
        break;
    }
    reg->UART_MR = mr;


    mr = (reg->UART_MR) & ( ~(3 << 12));
    //UART�����޸�ֹͣλ
    switch(Com->StopBits)
    {
    case CN_UART_STOPBITS_1:
        mr |= (0 << 12);
        break;
    case CN_UART_STOPBITS_1_5:
        mr |= (1 << 12);
        break;
    case CN_UART_STOPBITS_2:
        mr |= (2 << 12);
        break;
    default:
        break;
    }
    reg->UART_MR = mr;

    __UART_TxEnable(reg,1);
    __UART_RxEnable(reg,1);
}

//----��ʼ��uart��Ӧ���ж�-----------------------------------------------------
//����: ��ʼ��uart��Ӧ���ж��ߣ�����ʼ���ж���ں���
//������SerialNo,���ں�
//����: ��
//-----------------------------------------------------------------------------
static void __UART_IntInit(u32 SerialNo)
{
    uint32_t int_line;
    if(SerialNo == CN_UART0)
        int_line = CN_INT_LINE_UART0;
    else if(SerialNo == CN_UART1)
        int_line = CN_INT_LINE_UART1;
    else if(SerialNo == CN_UART2)
        int_line = CN_INT_LINE_UART2;
    else if(SerialNo == CN_UART3)
        int_line = CN_INT_LINE_UART3;
    else if(SerialNo == CN_UART4)
        int_line = CN_INT_LINE_UART4;
    else if(SerialNo == CN_USART0)
        int_line = CN_INT_LINE_USART0;
    else if(SerialNo == CN_USART1)
        int_line = CN_INT_LINE_USART1;
    else
        int_line = CN_INT_LINE_USART2;

    Int_Register(int_line);
    Int_SetClearType(int_line,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(int_line,UART_ISR);
    Int_SettoAsynSignal(int_line);
    Int_ClearLine(int_line);
    Int_RestoreAsynLine(int_line);
}

//----��ʼ��uartʱ��ʹ��-----------------------------------------------------
//����: ʹ����Ӧuart�����ʱ��
//������SerialNo,���ں�
//����: ��
//-----------------------------------------------------------------------------
static void __UART_PmcInit(u32 SerialNo)
{
    if(SerialNo == CN_UART0)
        PMC_EnablePeripheral(ID_UART0);
    else if(SerialNo == CN_UART1)
        PMC_EnablePeripheral(ID_UART1);
    else if(SerialNo == CN_UART2)
        PMC_EnablePeripheral(ID_UART2);
    else if(SerialNo == CN_UART3)
        PMC_EnablePeripheral(ID_UART3);
    else if(SerialNo == CN_UART4)
        PMC_EnablePeripheral(ID_UART4);
    else if(SerialNo == CN_USART0)
        PMC_EnablePeripheral(ID_USART0);
    else if(SerialNo == CN_USART1)
        PMC_EnablePeripheral(ID_USART1);
    else
        PMC_EnablePeripheral(ID_USART2);
}

// =============================================================================
// ����: ��������DMA�����շ�����Ϊǰ���Ѿ��������Ŀ����������Ѿ����úã��˴Σ�ֻ��Ҫ����ַ
//       ����Ҫ���͵�������������һ�±��
// ������pXdmac,DMA�Ĵ���ָ��
//      iChannel,ͨ����
//      sa,da,len,Դ��ַ��Ŀ�ĵ�ַ�������ֽ���
// ����: ��
// =============================================================================
static void __UART_DmaTxConfig(Xdmac *pXdmac,u8 iChannel,u32 sa,u32 da,u32 len)
{
    XDMAC_SetSourceAddr(pXdmac, iChannel, sa);
    XDMAC_SetDestinationAddr(pXdmac, iChannel, da);
    XDMAC_SetMicroblockControl(pXdmac, iChannel, len);
}

// =============================================================================
// ���ܣ�����DMA ��LLI������ʹ��LLI0��LLI1����LIST
// ������pUartRx�����õĽ���ͨ��
//      sa������Դ��ַ������ӦUART����USART�� RHR�Ĵ���
//      da,Ŀ���ַ����DMA�İ��˵Ļ�����
//      len,32λ������ ����16λ����LLI1���ֽ�������16λ��ʾLLI0���ֽڳ���
// ���أ���
// =============================================================================
static void __UART_DmaRxLLIConfig(UartChannel *pUartRx,u32 sa,u32 da,u32 len)
{
    u8 i,LliNo= 2,xdmaCndc,xdmaIntEn;
    u16 BuffSize[2];
    u8 *pbuf;

    pbuf = (uint8_t *)da;
    BuffSize[0] = (u16)len;
    BuffSize[1] = (u16)(len >> 16);
    for (i = 0; i < LliNo; i++)
    {
         pUartRx->pLLIview[i].mbr_ubc = XDMA_UBC_NVIEW_NDV1
                                        | XDMA_UBC_NSEN_UNCHANGED
                                        | XDMA_UBC_NDEN_UPDATED
                                        | ((i== LliNo- 1)?
                                        ( (pUartRx->dmaRingBuffer)?
                                                XDMA_UBC_NDE_FETCH_EN : 0):
                                                XDMA_UBC_NDE_FETCH_EN)
                                        | BuffSize[i];
        pUartRx->pLLIview[i].mbr_sa = (uint32_t)sa;
        pUartRx->pLLIview[i].mbr_da = (uint32_t)pbuf;
        pUartRx->pLLIview[i].mbr_nda = (i == ( LliNo - 1))?
                ( (pUartRx->dmaRingBuffer)? (uint32_t)pUartRx->pLLIview : 0 ):
                (uint32_t)&pUartRx->pLLIview[ i + 1 ];

        pbuf += BuffSize[i];
    }

    __DMB();
    xdmaCndc = XDMAC_CNDC_NDVIEW_NDV1 |
               XDMAC_CNDC_NDE_DSCR_FETCH_EN |
               XDMAC_CNDC_NDSUP_SRC_PARAMS_UPDATED|
               XDMAC_CNDC_NDDUP_DST_PARAMS_UPDATED ;
    xdmaIntEn = XDMAC_CIE_BIE;//XDMAC_CIE_LIE
    XDMAC_SetDescriptorAddr(dmad.pXdmacs, pUartRx->ChNum, (u32)(pUartRx->pLLIview), 0);
    XDMAC_SetDescriptorControl(dmad.pXdmacs, pUartRx->ChNum, xdmaCndc);
    XDMAC_DisableChannelIt (dmad.pXdmacs, pUartRx->ChNum, 0xFF);
    XDMAC_EnableChannelIt (dmad.pXdmacs,pUartRx->ChNum,xdmaIntEn  );
}

//��ȡDMA��������ֵ���ü�������һ���ݼ�������
static u32 __UART_DmaGetDataLen(Xdmac *pXdmac,u8 iChannel)
{
    return XDMAC_GetMicroBlock(pXdmac,iChannel);
}

// =============================================================================
// ���ܣ�����DMA�����жϻص���������ʼ��ʱע�ᣬ��DMA�жϺ����ص�������ú���������ͣ��
//      ͨ����DMA��Ȼ��DMA�����ݶ�������д�뵽���λ�������������ж��Ƿ�������DMA
// ������channel,ͨ����
//      pArg������DMA�ṹָ��
// ���أ���
// =============================================================================
void UART_DmaRxIntIsr(uint32_t channel, UartDma* pArg)
{
    UartChannel *pUartRx;

    static struct Object *UCB;
    uint8_t *puart_dma_recv_buf;
    uint32_t num,Port;
    uint32_t len,UartDmaRcvLen;
    tagUartReg* Reg;

    for(Port = 0; Port < CN_UART_NUM; Port++)
    {
        if((u32)(pArg->pUartHw) == (u32)tg_UART_Reg[Port])
        {
            break;
        }
    }
    if(Port == CN_UART_NUM)
        return;

    if( !(sUartInited & (0x01 << Port)) )
        return ;

    pUartRx = pArg->pRxChannel;
    UCB = pUartCB[Port];
    Reg = tg_UART_Reg[Port];
    UartDmaRcvLen = s_UART_DmaRcvLen[Port];
    puart_dma_recv_buf = ALIGN_SIZE_32(pUartRx->pBuff);//UartDmaRecvBuf[Port];

    XDMAC_SoftwareFlushReq(dmad.pXdmacs,pUartRx->ChNum);
    num = __UART_DmaGetDataLen(dmad.pXdmacs,pUartRx->ChNum);

    len = CFG_UART_DMABUF_LEN - num;        //�������ǵݼ�

    UARTD_RcvPause(pArg);

    UART_PortWrite(UCB,puart_dma_recv_buf,len);

    //��������һ��DMA
//    XDMAC_SetDescriptorAddr(dmad.pXdmacs, pUartRx->ChNum, (u32)(pUartRx->pLLIview), 0);
    __UART_DmaRxLLIConfig(pUartRx,(u32)&Reg->UART_RHR,
            (u32)ALIGN_SIZE_32((pUartRx->pBuff)),UartDmaRcvLen);

    UARTD_RcvData(pArg);

    return ;
}

// =============================================================================
// ���ܣ�����DMA�����жϻص���������ʼ��ʱע�ᣬ��DMA�жϺ����ص�������ú���������ͣ��
//      ͨ����DMA��Ȼ��DMA�����ݶ�������д�뵽���λ�������������ж��Ƿ�������DMA
// ������channel,ͨ����
//      pArg������DMA�ṹָ��
// ���أ���
// =============================================================================
void UART_DmaTxIntIsr(uint32_t channel, UartDma* pArg)
{
    UartChannel *pUartTx;

    static struct Object *UCB;
    volatile tagUartReg *Reg;
    uint8_t *puart_dma_send_buf;
    uint32_t num,Port;

    for(Port = 0; Port < CN_UART_NUM; Port++)
    {
        if((u32)(pArg->pUartHw) == (u32)tg_UART_Reg[Port])
        {
            break;
        }
    }
    if(Port == CN_UART_NUM)
        return;

    if( !(sUartInited & (0x01 << Port)) )
        return ;

    pUartTx = pArg->pTxChannel;
    UCB = pUartCB[Port];
    Reg = tg_UART_Reg[Port];
    puart_dma_send_buf = ALIGN_SIZE_32(pUartTx->pBuff);//UartDmaRecvBuf[Port];

    UARTD_SendPause(pArg);

    num = UART_PortRead(UCB,puart_dma_send_buf, CFG_UART_DMABUF_LEN);
    if(num > 0)
    {
        __UART_DmaTxConfig(dmad.pXdmacs,pUartTx->ChNum,
                (u32)puart_dma_send_buf,(u32)&Reg->UART_THR,num);

        UARTD_SendData(pArg);
    }
    else
        __UART_SendIntEnable(Reg,1);
    return ;
}

// =============================================================================
// ����: ����uartΪDMA��ʽ������Ϊ��DMA��ʽʱ�����ø��Ĵ�����
// ������reg,���ڼĴ���ָ��
//       cmd,�����
//       Port,�˿ںţ������ں�
// ����: ��
// =============================================================================
static void __UART_DMA_Config(tagUartReg* Reg,u32 cmd,u32 Port)
{
    UartDma *pUartDma;
    UartChannel *pUartRx,*pUartTx;
    u32 UartId,UartDmaRcvLen;
    u8 *pUartDmaRxBuf,*pUartDmaTxBuf;

    switch(Port)
    {
    case CN_UART0: pUartDma = &Uartd0; pUartRx = &Uart0Rx; pUartTx = &Uart0Tx; UartId = ID_UART0;break;
    case CN_UART1: pUartDma = &Uartd1; pUartRx = &Uart1Rx; pUartTx = &Uart1Tx; UartId = ID_UART1;break;
    case CN_UART2: pUartDma = &Uartd2; pUartRx = &Uart2Rx; pUartTx = &Uart2Tx; UartId = ID_UART2;break;
    case CN_UART3: pUartDma = &Uartd3; pUartRx = &Uart3Rx; pUartTx = &Uart3Tx; UartId = ID_UART3;break;
    case CN_UART4: pUartDma = &Uartd4; pUartRx = &Uart4Rx; pUartTx = &Uart4Tx; UartId = ID_UART4;break;
    case CN_USART0: pUartDma = &Usartd0; pUartRx = &Usart0Rx; pUartTx = &Usart0Tx; UartId = ID_USART0;break;
    case CN_USART1: pUartDma = &Usartd1; pUartRx = &Usart1Rx; pUartTx = &Usart1Tx; UartId = ID_USART1;break;
    case CN_USART2: pUartDma = &Usartd2; pUartRx = &Usart2Rx; pUartTx = &Usart2Tx; UartId = ID_USART2;break;
    default: return;
    }

    //��������صļĴ���reset�����ܽ��պͷ���
    Reg->UART_CR = ((1<<2)|(1<<3)|(1<<5)|
                (1<<7)|(1<<8));         //���շ��Ͷ�������

    if(cmd == CN_UART_DMA_UNUSED)//��ʹ��DMA
    {
        s_UART_DmaUsed[Port] = false;

         //�رշ�dmaʱʹ�õ��жϣ���ʼdmaʹ�õ�uart�����ж�
         //�����ж�ֻ����������ʱ����
        __UART_RecvIntEnable(Reg,1);
        //�ر����߿����жϣ�ֻ��USART��Ч
        Reg->US_RTOR = 0;
        Reg->UART_IDR = (1<<8);//�ر�timeout�ж�

        if( (pUartRx->pBuff) && (pUartTx->pBuff) )
        {
            free(pUartRx->pBuff);
            free(pUartTx->pBuff);
            pUartRx->pBuff = NULL;
            pUartTx->pBuff = NULL;

            UARTD_DisableRxChannels(pUartDma, pUartRx);
            UARTD_DisableTxChannels(pUartDma, pUartTx);
        }
    }
    else    //ʹ��DMA
    {
        s_UART_DmaUsed[Port] = true;

        //�رշ�dmaʱʹ�õ��жϣ���ʼdmaʹ�õ�uart�����ж�
        __UART_RecvIntEnable(Reg, 0);
        __UART_SendIntEnable(Reg, 0);

        //����receiver time-out�жϣ�ֻ��USART�иù���
        if(Port >= CN_USART0)
        {
            Reg->US_RTOR = s_UART_TimeOut[Port-CN_USART0];// 1*115200,1S
            Reg->UART_CR = (1<<11);//ʹ��setto
            Reg->UART_IER = (1<<8);
        }

        //cache ��cache�豣֤32�ֽڶ��룬��˵ĵ�ַ��32�ֽڶ���
        pUartDmaRxBuf = (u8*)M_Malloc(CFG_UART_DMABUF_LEN + 32,0);
        if(pUartDmaRxBuf == NULL)
        {
            return ;
        }
        pUartDmaTxBuf = (u8*)M_Malloc(CFG_UART_DMABUF_LEN + 32,0);
//        pUartDmaRxBuf = (u8*)((u32)pUartDmaRxBuf - ((u32)pUartDmaRxBuf % 32));
//        pUartDmaTxBuf = pUartDmaRxBuf + CFG_UART_DMABUF_LEN;
        if(pUartDmaTxBuf == NULL)
        {
            free(pUartDmaRxBuf);
            return ;
        }

        memset(pUartTx, 0, sizeof(UartChannel));
        memset(pUartRx, 0, sizeof(UartChannel));
        pUartTx->BuffSize = CFG_UART_DMABUF_LEN;
        pUartTx->pBuff = pUartDmaTxBuf;
        pUartRx->BuffSize= CFG_UART_DMABUF_LEN/2;
        pUartRx->pBuff = pUartDmaRxBuf;
        pUartTx->sempaphore = 1;
        pUartRx->sempaphore = 1;
        pUartTx->pArgument = (void*)pUartDma;
        pUartRx->pArgument = (void*)pUartDma;

        pUartTx->callback = (UartdCallback)UART_DmaTxIntIsr;
        pUartRx->callback = (UartdCallback)UART_DmaRxIntIsr;

        //config the rx LLI
        pUartRx->dmaProgrammingMode = XDMAD_LLI;
        pUartRx->dmaRingBuffer = 1;
        pUartRx->dmaBlockSize = 2;
        UartDmaRcvLen = s_UART_DmaRcvLen[Port];

        pUartDma->pRxChannel = pUartRx;
        pUartDma->pTxChannel = pUartTx;
        pUartDma->pXdmad = &dmad;

        UARTD_Configure(pUartDma,UartId);

        UARTD_EnableRxChannels(pUartDma, pUartRx);
        UARTD_EnableTxChannels(pUartDma, pUartTx);

        UARTD_RcvPause(pUartDma);

        __UART_DmaRxLLIConfig(pUartRx,(u32)&Reg->UART_RHR,
                (u32)ALIGN_SIZE_32((pUartRx->pBuff)),UartDmaRcvLen);

        UARTD_RcvData(pUartDma);
    }

    __UART_TxEnable(Reg,1);
    __UART_RxEnable(Reg,1);
}

// =============================================================================
// ����: ����Ӳ�����������������ʡ�ֹͣλ��У��λ������λ��Ĭ�������:
//      ������:115200  �� ֹͣλ:1 ; У��:żУ�� ; ����λ:8bit
// ����: reg,�������ļĴ�����ָ��
// ����: ��
// =============================================================================
static void __UART_ResetReg(tagUartReg *reg)
{
    reg->UART_CR = (1<<2) | (1<<3) | (1<<5) | (1<<7) | (1<<8);
}
static void __UART_HardInit(u8 SerialNo)
{
    tagUartReg *reg;
    if(SerialNo > CN_UART_MAX)
        return;

    XDMAD_Initialize(&dmad,0);              //��λDMA�����֮ǰ�����ˣ����븴λ
    __UART_PmcInit(SerialNo);

    reg = (tagUartReg *)tg_UART_Reg[SerialNo];
    __UART_ResetReg(reg);
    __UART_BaudSet(reg,115200);            //����������
    reg->UART_MR = 0x000;                  //żУ�飬����ģʽ
    reg->UART_CR = ((1<<2)|(1<<3)|(1<<5)|
                    (1<<7)|(1<<8));         //���շ��Ͷ�������

    reg->UART_MR = (3<<6)|(4<<9);//����żУ�飬ʱ��ԴΪMCK,����ģʽ,ֹͣλ1��8����
    reg->UART_CR = ((1<<4)|(1<<6));// tx rx enable
    __UART_RecvIntEnable(reg,1);
}

// =============================================================================
// ����: �����豸�Ŀ��ƺ���,������Ӳ���Ĵ����������
// ����: Reg,UART�ļĴ�����ַ.
//       cmd,��������
//       data,������cmd����
// ����: ������.
// =============================================================================
ptu32_t __UART_Ctrl(tagUartReg *Reg,u32 cmd, va_list *arg0)
{
    u8 Port;
    s32 timeout = 10000;
    switch((u32)Reg)
    {
    case CN_UART0_BASE: Port = CN_UART0;break;
    case CN_UART1_BASE: Port = CN_UART1;break;
    case CN_UART2_BASE: Port = CN_UART2;break;
    case CN_UART3_BASE: Port = CN_UART3;break;
    case CN_UART4_BASE: Port = CN_UART4;break;
    case CN_USART0_BASE: Port = CN_USART0;break;
    case CN_USART1_BASE: Port = CN_USART1;break;
    case CN_USART2_BASE: Port = CN_USART2;break;
    default:return 0;
    }

    if( !(sUartInited & (0x01 << Port)) )
        return 0;

    switch(cmd)
    {
        case CN_DEV_CTRL_START:
            __UART_RxEnable(Reg,1);
            __UART_TxEnable(Reg,1);
            break;
        case CN_DEV_CTRL_STOP:
            __UART_RxEnable(Reg,0);
            __UART_TxEnable(Reg,0);
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
            __UART_BaudSet(Reg, data);
        }
            break;
        case CN_UART_COM_SET:
        {
            struct COMParam *COM;
            COM = va_arg(*arg0, void *);
            __UART_ComConfig(Reg,COM);
        }
            break;
//        case CN_UART_HALF_DUPLEX_SEND:
//            Board_UartHalfDuplexSend(Port);
//            break;
//        case CN_UART_HALF_DUPLEX_RECV:
//            while((false == __UART_TxTranEmpty(Reg))&& (timeout > 10))
//            {
//                timeout -=10;
//                DJY_DelayUs(10);
//            }
//            Board_UartHalfDuplexRecv(Port);
//            break;
        case CN_UART_DMA_USED:
        case CN_UART_DMA_UNUSED:
            __UART_DMA_Config(Reg,cmd,Port);
            break;
        default: break;
    }
    return 0;
}

// =============================================================================
// ����: �������ڷ��ͣ�ֱ�ӷ���FIFO��С�����ݣ����������Ϳ��жϣ����ж��н��������е�
//      ��������������ݵķ��ͣ��൱��������һ�η��͹��ܣ���ΪDMA��ʽ�ͷ�DMA��ʽ
// ����: Reg,�������Ĵ��ڼĴ�����ַ
//       timeout,��ʱʱ�䣬΢��
// ����: ���͵��ֽ���
// =============================================================================
u32 __UART_SendStart(tagUartReg *Reg)
{
    u8 Port,num;
    struct Object *UCB;
    UartDma *pUartd;
    u8 *UartDmaSendBuf;

    switch((u32)Reg)
    {
    //UART0��UART1��FIFO���Ϊ8����������Ϊ1
    case CN_UART0_BASE:UCB = pUartCB[CN_UART0];Port = CN_UART0; pUartd = &Uartd0;break;
    case CN_UART1_BASE:UCB = pUartCB[CN_UART1];Port = CN_UART1;pUartd = &Uartd1;break;
    case CN_UART2_BASE:UCB = pUartCB[CN_UART2];Port = CN_UART2;pUartd = &Uartd2;break;
    case CN_UART3_BASE:UCB = pUartCB[CN_UART3];Port = CN_UART3;pUartd = &Uartd3;break;
    case CN_UART4_BASE:UCB = pUartCB[CN_UART4];Port = CN_UART4;pUartd = &Uartd4;break;
    case CN_USART0_BASE:UCB = pUartCB[CN_USART0];Port = CN_USART0;pUartd = &Usartd0;break;
    case CN_USART1_BASE:UCB = pUartCB[CN_USART1];Port = CN_USART1;pUartd = &Usartd1;break;
    case CN_USART2_BASE:UCB = pUartCB[CN_USART2];Port = CN_USART2;pUartd = &Usartd2;break;
    default:return 0;
    }

    if( !(sUartInited & (0x01 << Port)) )
        return 0;
    Board_UartHalfDuplexSend(Port);
    if(s_UART_DmaUsed[Port])
    {
        if(pUartd->pTxChannel->sempaphore==1)
        {
            UartDmaSendBuf = ALIGN_SIZE_32(pUartd->pTxChannel->pBuff);
            UARTD_SendPause(pUartd);
            num = UART_PortRead(UCB,UartDmaSendBuf,CFG_UART_DMABUF_LEN);
            if(num > 0)
            {
                __UART_DmaTxConfig(dmad.pXdmacs,pUartd->pTxChannel->ChNum,
                        (u32)UartDmaSendBuf,(u32)&Reg->UART_THR,num);

                UARTD_SendData(pUartd);
            }
        }
    }
    else
    {
        __UART_SendIntEnable(Reg,1);
    }
    return 1;
}

// =============================================================================
// ����: ʹ���жϷ�ʽЧ���ܹ�����������ִ��Ч�ʣ��������ա����ͺ��쳣�ȣ����յ�����
//       �󣬵���ͨ�ýӿ�ģ�飬д���������������������ʱ������ͨ�ýӿ�ģ�飬�������
//       �����������ݣ�д��Ӳ�����ͼĴ��������⣬����оƬ����ͨ��DMA��ʽ����
// ����: �жϺ�.
// ����: 0.
// =============================================================================
uint32_t UART_ISR(ptu32_t IntLine)
{
    static struct UartGeneralCB *UCB;
    volatile tagUartReg *Reg;
    uint8_t ch,Port;
    uint32_t IIR=0,num;
    s32 timeout = 1000;
    UartDma *pUartDma = NULL;

    switch(IntLine)
    {
        case CN_INT_LINE_UART0:         Port = CN_UART0;            break;
        case CN_INT_LINE_UART1:         Port = CN_UART1;            break;
        case CN_INT_LINE_UART2:         Port = CN_UART2;            break;
        case CN_INT_LINE_UART3:         Port = CN_UART3;            break;
        case CN_INT_LINE_UART4:         Port = CN_UART4;            break;
        case CN_INT_LINE_USART0:        Port = CN_USART0;
                                        pUartDma = &Usartd0;        break;
        case CN_INT_LINE_USART1:        Port = CN_USART1;
                                        pUartDma = &Usartd1;        break;
        case CN_INT_LINE_USART2:        Port = CN_USART2;
                                        pUartDma = &Usartd2;        break;
        default:    return 0;
    }

    if( !(sUartInited & (0x01 << Port)) )
        return 0;

    Reg = tg_UART_Reg[Port];
    UCB = pUartCB[Port];
    IIR = Reg->UART_SR;

    if((IIR & (1<<0)) && (Reg->UART_IMR &(1<<0)))//rxrdy int
    {
        ch = Reg->UART_RHR;
        num = UART_PortWrite(UCB,&ch,1);
        if(num != 1)
        {
            UART_ErrHandle(UCB,CN_UART_BUF_OVER_ERR);
        }
    }
    //tx empty int
    if((IIR &(1<<9)) && (Reg->UART_IMR &(1<<9)))
    {
        num = UART_PortRead(UCB,&ch,1);
        while((!__UART_TxTranEmpty(Reg)) && (timeout-- > 0));
        if(num != 0)
            Reg->UART_THR = ch;
        else
        {
            __UART_SendIntEnable(Reg,0);
            Board_UartHalfDuplexRecv(Port);
        }
    }


    if( (IIR & (1<<8) ) && (Reg->UART_IMR &(1<<8)) )//timeout int
    {
        if( (Port >= CN_USART0) && (s_UART_DmaUsed[Port] == true) )
        {
            Reg->UART_CR = (1<<11);//STTTO,����USART�ĳ�ʱ����
            UART_DmaRxIntIsr(0,pUartDma);
        }
    }

    if(IIR &(0xE0)) //�����ж�
    {
        UART_ErrHandle(UCB,CN_UART_HARD_COMM_ERR);
    }

    return 0;
}

// =============================================================================
// ���ܣ���ʼ������ģ�飬ʹ��DJYOS�Ĵ��ڱ�׼ģ�ͣ�����Ӳ�����жϺʹ����豸����
// ���������ڳ�ʼ��ģ��Ĳ���Ϊ���ںţ���UART0
// ���أ�1=�ɹ���0=ʧ��
// =============================================================================
ptu32_t ModuleInstall_UART(ptu32_t SerialNo)
{
    struct UartParam UART_Param;

    switch(SerialNo)
    {
    case CN_UART0:        UART_Param.Name = "UART0";        break;
    case CN_UART1:        UART_Param.Name = "UART1";        break;
    case CN_UART2:        UART_Param.Name = "UART2";        break;
    case CN_UART3:        UART_Param.Name = "UART3";        break;
    case CN_UART4:        UART_Param.Name = "UART4";        break;
    case CN_USART0:       UART_Param.Name = "USART0";        break;
    case CN_USART1:       UART_Param.Name = "USART1";        break;
    case CN_USART2:       UART_Param.Name = "USART2";        break;
    default:        return 0;
    }
    UART_Param.TxRingBufLen = CFG_UART_SENDBUF_LEN;
    UART_Param.RxRingBufLen = CFG_UART_RECVBUF_LEN;
    UART_Param.UartPortTag  = (ptu32_t)tg_UART_Reg[SerialNo];
    UART_Param.Baud         = 115200;
    UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
    UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
    UART_Param.mode         = CN_UART_GENERAL;
    //Ӳ����ʼ��
    __UART_HardInit(SerialNo);
    __UART_IntInit(SerialNo);

    sUartInited |= (0x01 << SerialNo);
    pUartCB[SerialNo] = UART_InstallGeneral(&UART_Param);
    if( pUartCB[SerialNo] == NULL)
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

    CR_Bak = PutStrDirectReg->UART_IER;                          //Save INT
    __UART_SendIntEnable(PutStrDirectReg,0);                    //disable send INT
    for(result=0; result < len+1; result ++)
    {
        // ��ʱ���߷��ͻ���Ϊ��ʱ�˳�
        while((false == __UART_TxTranEmpty(PutStrDirectReg))&& (timeout > 10))
        {
            timeout -=10;
            DJY_DelayUs(10);
        }
        if( (timeout <= 10) || (result == len) )
            break;
        PutStrDirectReg->UART_THR = str[result];
    }

    PutStrDirectReg->UART_IER = CR_Bak;                         //restore send INT
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

    CR_Bak = GetCharDirectReg->UART_IER;                          //Save INT
    __UART_SendIntEnable(PutStrDirectReg,0);                      //disable send INT
    while(__UART_RxHadChar(GetCharDirectReg) == false) ;
    result = GetCharDirectReg->UART_RHR;
    PutStrDirectReg->UART_IER = CR_Bak;                         //restore send INT
    return result;
}

//----��ʼ���ں˼�IO-----------------------------------------------------------
//���ܣ���ʼ���ں˼��������������runtime����ָ�롣
//��������
//���أ���
//-----------------------------------------------------------------------------
void Stdio_KnlInOutInit(char * StdioIn, char *StdioOut)
{
    if(!strcmp(StdioOut,"/dev/UART0"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART0_BASE;
        TxDirectPort = CN_UART0;
    }
    else if(!strcmp(StdioOut,"/dev/UART1"))
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
    else if(!strcmp(StdioOut,"/dev/USART0"))
    {
        PutStrDirectReg = (tagUartReg*)CN_USART0_BASE;
        TxDirectPort = CN_USART0;
    }
    else if(!strcmp(StdioOut,"/dev/USART1"))
    {
        PutStrDirectReg = (tagUartReg*)CN_USART1_BASE;
        TxDirectPort = CN_USART1;
    }
    else if(!strcmp(StdioOut,"/dev/USART2"))
    {
        PutStrDirectReg = (tagUartReg*)CN_USART2_BASE;
        TxDirectPort = CN_USART2;
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

    if(!strcmp(StdioIn,"/dev/UART0"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART0_BASE;
        RxDirectPort = CN_UART0;
    }
    else if(!strcmp(StdioIn,"/dev/UART1"))
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
    else if(!strcmp(StdioIn,"/dev/USART0"))
    {
        GetCharDirectReg = (tagUartReg*)CN_USART0_BASE;
        RxDirectPort = CN_USART0;
    }
    else if(!strcmp(StdioIn,"/dev/USART1"))
    {
        GetCharDirectReg = (tagUartReg*)CN_USART1_BASE;
        RxDirectPort = CN_USART1;
    }
    else if(!strcmp(StdioIn,"/dev/USART2"))
    {
        GetCharDirectReg = (tagUartReg*)CN_USART2_BASE;
        RxDirectPort = CN_USART2;
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

