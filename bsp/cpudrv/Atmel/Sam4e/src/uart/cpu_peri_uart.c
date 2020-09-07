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
#include <device/djy_uart.h>
#include "string.h"
#include "project_config.h"    //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    ptu32_t ModuleInstall_UART(ptu32_t SerialNo);
//    #if CFG_UART0_ENABLE ==1
//    ModuleInstall_UART(CN_UART0);
//    #endif
//    #if CFG_UART1_ENABLE ==1
//    ModuleInstall_UART(CN_UART1);
//    #endif
//
//    #if CFG_USART0_ENABLE ==1
//    ModuleInstall_UART(CN_USART0);
//    #endif
//
//    #if CFG_USART1_ENABLE ==1
//    ModuleInstall_UART(CN_USART1);
      //#endif
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip uart"//CPU��uart��������
//parent:"uart device file"      //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                     //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                  //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                    //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                   //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                     //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"device file system","lock","uart device file","heap","cpu drver Power management"//������������������������none����ʾ�������������
                                    //����������������������г�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                    //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_UART == false )
//#warning  " cpu_onchip_uart  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header              //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_UART    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,32,512,
#define CFG_UART0_SENDBUF_LEN   256            //"UART0���ͻ�������С",
#define CFG_UART0_RECVBUF_LEN   256            //"UART0���ջ�������С",
#define CFG_UART0_DMABUF_LEN    64             //"UART0DMA��������С",

#define CFG_UART1_SENDBUF_LEN   256            //"UART1���ͻ�������С",
#define CFG_UART1_RECVBUF_LEN   256            //"UART1���ջ�������С",
#define CFG_UART1_DMABUF_LEN    64             //"UART1DMA��������С",

#define CFG_USART0_SENDBUF_LEN  256           //"USART0���ͻ�������С",
#define CFG_USART0_RECVBUF_LEN  256           //"USART0���ջ�������С",
#define CFG_USART0_DMABUF_LEN   64            //"USART0DMA��������С",

#define CFG_USART1_SENDBUF_LEN  256           //"USART1���ͻ�������С",
#define CFG_USART1_RECVBUF_LEN  256           //"USART1���ջ�������С",
#define CFG_USART1_DMABUF_LEN   64            //"USART1DMA��������С",
//%$#@enum,true,false,
#define CFG_UART0_ENABLE        true          //"�Ƿ�����UART0",
#define CFG_UART1_ENABLE        false         //"�Ƿ�����UART1",
#define CFG_USART0_ENABLE       false         //"�Ƿ�����USART0",
#define CFG_USART1_ENABLE       false         //"�Ƿ�����USART1",
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

// =============================================================================
//base address
#define CN_UART0_BASE   ((u32)UART0)
#define CN_UART1_BASE   ((u32)UART1)
#define CN_USART0_BASE  ((u32)USART0)
#define CN_USART1_BASE  ((u32)USART1)

static u32 TxByteTime;                    //��������һ���ֽ�����Ҫ��ʱ��
static tagUartReg *PutStrDirectReg;     //����printk���͵Ĵ��ڼĴ���
static tagUartReg *GetCharDirectReg;     //����ֱ�ӽ��յĴ��ڼĴ���
static u32 TxDirectPort;                  //����printk���͵Ĵ��ں�
static u32 RxDirectPort;                  //����ֱ�ӽ��յĴ��ں�
//==============================================================================
static tagUartReg volatile * const tg_UART_Reg[] = {(tagUartReg *)CN_UART0_BASE,
                                                    (tagUartReg *)CN_UART1_BASE,
                                                    (tagUartReg *)CN_USART0_BASE,
                                                    (tagUartReg *)CN_USART1_BASE};
bool_t s_UART_DmaUsed[4]={false,false,false,false};

//#define CFG_UART0_SENDBUF_LEN  256
//#define CFG_UART0_RECVBUF_LEN  256
//#define CFG_UART0_DMABUF_LEN  64
static u32 s_UART0_DmaRcvLen = 1;
uint8_t UART0_DmaSendBuf[CFG_UART0_DMABUF_LEN];
uint8_t UART0_DmaRecvBuf[CFG_UART0_DMABUF_LEN];


//#define CFG_UART1_SENDBUF_LEN  256
//#define CFG_UART1_RECVBUF_LEN  256
//#define CFG_UART1_DMABUF_LEN  64
static u32 s_UART1_DmaRcvLen = 1;
uint8_t UART1_DmaSendBuf[CFG_UART1_DMABUF_LEN];
uint8_t UART1_DmaRecvBuf[CFG_UART1_DMABUF_LEN];

//#define CFG_USART0_SENDBUF_LEN  256
//#define CFG_USART0_RECVBUF_LEN  256
//#define CFG_USART0_DMABUF_LEN  64
static u32 s_USART0_DmaRcvLen = 1;
uint8_t USART0_DmaSendBuf[CFG_USART0_DMABUF_LEN];
uint8_t USART0_DmaRecvBuf[CFG_USART0_DMABUF_LEN];

//#define CFG_USART1_SENDBUF_LEN  256
//#define CFG_USART1_RECVBUF_LEN  256
//#define CFG_USART1_DMABUF_LEN  64
static u32 s_USART1_DmaRcvLen = 1;
uint8_t USART1_DmaSendBuf[CFG_USART1_DMABUF_LEN];
uint8_t USART1_DmaRecvBuf[CFG_USART1_DMABUF_LEN];

ptu32_t UART_ISR(ptu32_t IntLine);

static struct Object *pUartCB[CN_UART_NUM];

extern bool_t Board_UartGpioInit(u8 SerialNo);

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
//      DmaIsUsed,�Ƿ�ʹ��DMA
//����: ��
//-----------------------------------------------------------------------------
void __UART_RecvIntEnable(tagUartReg volatile *reg,u32 DmaIsUsed)
{
    if(DmaIsUsed == cn_dma_unused)
        reg->UART_IER = (1<<0);//rxrdy int
    else
        reg->UART_IER = (1<<3);//endrx int
}

//----��ֹ�����ж�-------------------------------------------------------------
//����: ��ֹuart�Ľ����ж�,DMA�ж�ʹ�õ���endrx
//����: reg,�������ļĴ�����ָ��
//      DmaIsUsed,�Ƿ�ʹ��DMA
//����: ��
//-----------------------------------------------------------------------------
void __UART_RecvIntDisable(tagUartReg volatile *reg,u32 DmaIsUsed)
{
    if(DmaIsUsed == cn_dma_unused)
        reg->UART_IDR = (1<<0);//rxrdy int
    else
        reg->UART_IDR = (1<<3);//endrx int
}

//----ʹ�ܷ����ж�-------------------------------------------------------------
//����: ʹ��uart�ķ����ж�,DMA�ж�ʹ�õ���endtx �ж�
//����: reg,�������ļĴ�����ָ��
//      DmaIsUsed,�Ƿ�ʹ��DMA
//����: ��
//-----------------------------------------------------------------------------
void __UART_SendIntEnable(tagUartReg volatile *reg,u32 DmaIsUsed)
{
    if(DmaIsUsed == cn_dma_unused)
        reg->UART_IER = (1<<9);//tx empty int
    else
        reg->UART_IER = (1<<11);//txbufe int
}

//----��ֹ�����ж�-------------------------------------------------------------
//����: ��ֹuart�ķ����ж�,DMA�ж�ʹ�õ���endtx �ж�
//����: reg,�������ļĴ�����ָ��
//      DmaIsUsed,�Ƿ�ʹ��DMA
//����: ��
//-----------------------------------------------------------------------------
void __UART_SendIntDisable(tagUartReg volatile *reg,u32 DmaIsUsed)
{
    if(DmaIsUsed == cn_dma_unused)
        reg->UART_IDR = (1<<9);//tx empty int
    else
        reg->UART_IDR = (1<<11);//txbufe int
}

//----��鴮�ڷ��ͼĴ����Ƿ��-------------------------------------------------
//����: ��鷢�ͼĴ�����״̬������վͷ���true����Ϊ�գ�����λ�Ĵ�����thr��Ϊ��
//�������ޡ�
//����: true = �������գ�false = �ǿ�
//-----------------------------------------------------------------------------
bool_t __UART_TxTranEmpty(volatile tagUartReg *reg)
{
    return ((reg->UART_SR & (1<<9)) && (reg->UART_SR & (1<<11)));//TXR EMPTY
}

// =============================================================================
// ����: �����ռĴ�����״̬�������ݾͷ���true�����򷵻�false
// ������reg,�������ļĴ�����ָ��
// ����: true = �����ݣ�false = ������
// =============================================================================
static bool_t __UART_RxHadChar(tagUartReg volatile *reg)
{
    return (1 == (reg->UART_SR & (1<<0)));  //RXRDY,At least one byte in buf
}

//----����baud-----------------------------------------------------------------
//����: ���ô���baud
//����: reg,�������ļĴ�����ָ��
//����: ��
//-----------------------------------------------------------------------------
void __UART_BaudSet(tagUartReg volatile *reg,u32 baud)
{
    reg->UART_BRGR = (CN_CFG_MCLK/baud)/16;
    if(tg_UART_Reg[TxDirectPort] == reg)
    {
        TxByteTime = 11000000/baud;     //1���ֽڴ���ʱ�䣬��10bit��+10%����
    }
}

//----��ʼ��uart��Ӧ��io��-----------------------------------------------------
//����: ��ʼ��uart��Ӧ��io��Ϊ��������
//������SerialNo,���ں�
//����: ��
//-----------------------------------------------------------------------------
void __UART_GpioInit(u32 SerialNo)
{
    Board_UartGpioInit(SerialNo);
}

//----��ʼ��uart��Ӧ���ж�-----------------------------------------------------
//����: ��ʼ��uart��Ӧ���ж��ߣ�����ʼ���ж���ں���
//������SerialNo,���ں�
//����: ��
//-----------------------------------------------------------------------------
void __UART_IntInit(u32 SerialNo)
{
    uint32_t int_line;
    if(SerialNo == CN_UART0)
        int_line = CN_INT_LINE_UART0;
    else if(SerialNo == CN_UART1)
        int_line = CN_INT_LINE_UART1;
    else if(SerialNo == CN_USART0)
        int_line = CN_INT_LINE_USART0;
    else
        int_line = CN_INT_LINE_USART1;

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
void __UART_PmcInit(u32 SerialNo)
{
    if(SerialNo == CN_UART0)
        PMC_EnablePeripheral(ID_UART0);
    else if(SerialNo == CN_UART1)
        PMC_EnablePeripheral(ID_UART1);
    else if(SerialNo == CN_USART0)
        PMC_EnablePeripheral(ID_USART0);
    else
        PMC_EnablePeripheral(ID_USART1);
}

// =============================================================================
// ����: ���ô��ڲ���DMA��ʽʱ���������ã����� DMA���մ�С��ָ��
// ������reg,�Ĵ���ָ��
//      SerialNo,���ں�
//      DataLen,���ݴ�С
// ����: ��
// =============================================================================
void __UART_dma_recv_config(tagUartReg* reg,u32 SerialNo,u32 DataLen)
{
    u32 length;
    switch(SerialNo)
    {

    case CN_UART0:
        length = CFG_UART0_DMABUF_LEN;
        length = (DataLen/length)?length:DataLen%length;
        s_UART0_DmaRcvLen = DataLen;                //ʣ����
        reg->UART_RCR = length;
        reg->UART_RPR = (vu32)UART0_DmaRecvBuf;
        break;

    case CN_UART1:
        length = CFG_UART1_DMABUF_LEN;
        length = (DataLen/length)?length:DataLen%length;
        s_UART1_DmaRcvLen = DataLen;                //ʣ����
        reg->UART_RCR = length;
        reg->UART_RPR = (vu32)UART1_DmaRecvBuf;
        break;

    case CN_USART0:
        length = CFG_USART0_DMABUF_LEN;
        length = (DataLen/length)?length:DataLen%length;
        s_USART0_DmaRcvLen = DataLen;               //ʣ����
        reg->UART_RCR = length;
        reg->UART_RPR = (vu32)USART0_DmaRecvBuf;
        break;

    case CN_USART1:
        length = CFG_USART1_DMABUF_LEN;
        length = (DataLen/length)?length:DataLen%length;
        s_USART1_DmaRcvLen = DataLen;               //ʣ����
        reg->UART_RCR = length;
        reg->UART_RPR = (vu32)USART1_DmaRecvBuf;
        break;
    default:
        break;
    }
}

// =============================================================================
// ����: ����uartΪDMA��ʽ������Ϊ��DMA��ʽʱ�����ø��Ĵ�����
// ������reg,���ڼĴ���ָ��
//       cmd,�����
//       Port,�˿ںţ������ں�
// ����: ��
// =============================================================================
void __UART_DMA_Config(tagUartReg* Reg,u32 cmd,u32 Port)
{
    //��������صļĴ���reset�����ܽ��պͷ���
    Reg->UART_CR = ((1<<2)|(1<<3)|(1<<5)|
                (1<<7)|(1<<8));         //���շ��Ͷ�������

    if(cmd == CN_UART_DMA_UNUSED)//��ʹ��DMA
    {
        s_UART_DmaUsed[Port] = false;

         //�رշ�dmaʱʹ�õ��жϣ���ʼdmaʹ�õ�uart�����ж�
         //�����ж�ֻ����������ʱ����
        __UART_RecvIntDisable(Reg, cn_dma_used);
        __UART_SendIntDisable(Reg, cn_dma_used);
        __UART_RecvIntEnable(Reg, cn_dma_unused);
        Reg->UART_PTCR = ((1<<1)|(1<<9));//disable DMA rx tx
        Reg->UART_CR = ((1<<4)|(1<<6));// tx rx enable
    }
    else    //ʹ��DMA
    {
        s_UART_DmaUsed[Port] = true;

        //�رշ�dmaʱʹ�õ��жϣ���ʼdmaʹ�õ�uart�����ж�
        __UART_RecvIntEnable(Reg, cn_dma_used);
        __UART_RecvIntDisable(Reg, cn_dma_unused);
        __UART_SendIntDisable(Reg, cn_dma_unused);

        //����dma����
        Reg->UART_TCR   = 0;
        Reg->UART_TNCR  = 0;
        Reg->UART_RCR   = 0;
        Reg->UART_RNCR  = 0;

        __UART_dma_recv_config(Reg,Port,1);

        Reg->UART_PTCR = ((1<<0)|(1<<8));//enable DMA rx tx
        Reg->UART_CR = ((1<<4)|(1<<6));// tx rx enable
    }
}

// =============================================================================
// ����: ����Ӳ�����������������ʡ�ֹͣλ��У��λ������λ��Ĭ�������:
//      ������:115200  �� ֹͣλ:1 ; У��:żУ�� ; ����λ:8bit
// ����: reg,�������ļĴ�����ָ��
// ����: ��
// =============================================================================
void __UART_HardInit(u8 SerialNo)
{
    tagUartReg *reg;
    if(SerialNo > CN_UART_MAX)
        return;

    __UART_PmcInit(SerialNo);
    __UART_GpioInit(SerialNo);

    reg = (tagUartReg *)tg_UART_Reg[SerialNo];
    __UART_BaudSet(reg,115200);            //����������
    reg->UART_MR = 0x000;                  //żУ�飬����ģʽ
    reg->UART_CR = ((1<<2)|(1<<3)|(1<<5)|
                    (1<<7)|(1<<8));         //���շ��Ͷ�������

    reg->UART_MR = 0x00C0;//ֹͣλ:1 ��żУ��:ż
    reg->UART_CR = ((1<<4)|(1<<6));// tx rx enable
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
    switch((u32)Reg)
    {
    case CN_UART0_BASE: Port = CN_UART0;break;
    case CN_UART1_BASE: Port = CN_UART1;break;
    case CN_USART0_BASE: Port = CN_USART0;break;
    case CN_USART1_BASE: Port = CN_USART1;break;
    default:return 0;
    }

    switch(cmd)
    {
        case CN_DEV_CTRL_START:
            __UART_RecvIntEnable(Reg,s_UART_DmaUsed[Port]);
            __UART_SendIntEnable(Reg,s_UART_DmaUsed[Port]);
            break;
        case CN_DEV_CTRL_STOP:
            __UART_RecvIntDisable(Reg,s_UART_DmaUsed[Port]);
            __UART_SendIntDisable(Reg,s_UART_DmaUsed[Port]);
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
//        case CN_UART_RX_PAUSE:      //��ͣ����
//            __UART_RecvIntEnable(Reg,s_UART_DmaUsed[Port]);
//            break;
//        case CN_UART_RX_RESUME:      //�ָ�����
//            __UART_RecvIntDisable(Reg,s_UART_DmaUsed[Port]);
//            break;
        case CN_UART_RECV_HARD_LEVEL:    //��ΪUARTû��FIFO���������DMA����
        {
            u32 data;
            data = va_arg(*arg0, u32);
            __UART_dma_recv_config(Reg,Port,data);
        }
            break;
//        case CN_UART_HALF_DUPLEX_SEND:
//            Board_UartHalfDuplexSend(Port);
//            break;
//        case CN_UART_HALF_DUPLEX_RECV:
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
// ����: �����ֱ��д���ں��������ᾭ���¼�����
// ����: Reg,UART�ļĴ�����ַ.
//      send_buf,�����͵Ļ�������
//      len,���͵������ֽ���
//      timeout,��ʱʱ�䣬΢��
// ����: ���͵ĸ���
// =============================================================================
//u32 __UART_SendDirectly(tagUartReg* Reg,u8 *send_buf,u32 len,u32 timeout)
//{
//    u32  result,Port;
//    switch((u32)Reg)
//    {
//    //UART0��UART1��FIFO���Ϊ8����������Ϊ1
//    case CN_UART0_BASE:Port = CN_UART0; break;
//    case CN_UART1_BASE:Port = CN_UART1;break;
//    case CN_USART0_BASE:Port = CN_USART0;break;
//    case CN_USART1_BASE:Port = CN_USART1; break;
//    default:return 0;
//    }
//
//    __UART_SendIntDisable(Reg,s_UART_DmaUsed[Port]);
////    __UART_half_duplex_send(Port);//Ӳ��ʹ�ܷ���
//
//    if(s_UART_DmaUsed[Port] == false)
//    {
//        for(result=0; result < len; result ++)
//        {
//            while((0 == __UART_TxTranEmpty(Reg))
//                && (timeout > 0))//��ʱ���߷��ͻ���Ϊ��ʱ�˳�
//            {
//                timeout--;
//                DJY_DelayUs(1);
//            }
//            if(timeout == 0)
//                break;
//            Reg->UART_THR = send_buf[result];
//        }
//
////        //�ȴ��������ٽ�485ͨ��תΪ����
////        while((0 == __UART_TxTranEmpty(Reg))
////                && (timeout > 0))//��ʱ���߷��ͻ���Ϊ��ʱ�˳�
////        {
////            timeout--;
////            DJY_DelayUs(1);
////        }
////        if(timeout == 0)
////            result = 0;
//    }
//    else
//    {
//        Reg->UART_PTCR = (1<<9);//disable dma send first
//      if((Reg->UART_TCR==0)&&(Reg->UART_TNCR==0))
//      {
//          Reg->UART_TPR  = (uint32_t)send_buf;
//          Reg->UART_TCR  = len;
//          Reg->UART_PTCR = (1<<8);//dma tx enbale
//      }
//      else
//            result = 0;
//        //ֱ�ӷ��ͷ�ʽ������������DMA����
//        while((!__UART_TxTranEmpty(Reg)) && (timeout > 0))
//        {
//            timeout--;
//            DJY_DelayUs(1);
//        }
//        if(timeout == 0)
//            result = 0;
//    }
//    __UART_SendIntEnable(Reg,s_UART_DmaUsed[Port]);
////    __UART_half_duplex_recv(Port);//Ӳ��ʹ�ܽ���
//    return result;
//}

// =============================================================================
// ����: �������ڷ��ͣ�ֱ�ӷ���FIFO��С�����ݣ����������Ϳ��жϣ����ж��н��������е�
//      ��������������ݵķ��ͣ��൱��������һ�η��͹��ܣ���ΪDMA��ʽ�ͷ�DMA��ʽ
// ����: Reg,�������Ĵ��ڼĴ�����ַ
//       timeout,��ʱʱ�䣬΢��
// ����: ���͵��ֽ���
// =============================================================================
u32 __UART_SendStart(tagUartReg *Reg)
{
    u8 Port;

    switch((u32)Reg)
    {
    //UART0��UART1��FIFO���Ϊ8����������Ϊ1
    case CN_UART0_BASE: Port = CN_UART0; break;
    case CN_UART1_BASE:Port = CN_UART1;break;
    case CN_USART0_BASE:Port = CN_USART0;break;
    case CN_USART1_BASE:Port = CN_USART1; break;
    default:return 0;
    }
    Board_UartHalfDuplexSend(Port);
    __UART_SendIntEnable(Reg,s_UART_DmaUsed[Port]);
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
    tagUartReg *Reg;
    s32 timeout = 1000;
    uint8_t ch,*puart_dma_send_buf,*puart_dma_recv_buf;
    uint32_t IIR=0,Port,DmaBufLen,DmaRcvLen,num;

    switch(IntLine)
    {
        case CN_INT_LINE_UART0:
            Port = CN_UART0;
            UCB = pUartCB[Port];
            Reg =  (tagUartReg *)CN_UART0_BASE;
            DmaBufLen = CFG_UART0_DMABUF_LEN;
            DmaRcvLen = s_UART0_DmaRcvLen;
            puart_dma_send_buf = (uint8_t*)UART0_DmaSendBuf;
            puart_dma_recv_buf = (uint8_t*)UART0_DmaRecvBuf;
            break;

        case CN_INT_LINE_UART1:
            Port = CN_UART1;
            UCB = pUartCB[Port];
            Reg =  (tagUartReg *)CN_UART1_BASE;
            DmaBufLen = CFG_UART1_DMABUF_LEN;
            DmaRcvLen = s_UART1_DmaRcvLen;
            puart_dma_send_buf = (uint8_t*)UART1_DmaSendBuf;
            puart_dma_recv_buf = (uint8_t*)UART1_DmaRecvBuf;
            break;

        case CN_INT_LINE_USART0:
            Port = CN_USART0;
            UCB = pUartCB[Port];
            Reg =  (tagUartReg *)CN_UART1_BASE;
            DmaBufLen = CFG_USART0_DMABUF_LEN;
            DmaRcvLen = s_USART0_DmaRcvLen;
            puart_dma_send_buf = (uint8_t*)USART0_DmaSendBuf;
            puart_dma_recv_buf = (uint8_t*)USART0_DmaRecvBuf;
            break;

        case CN_INT_LINE_USART1:
            Port = CN_USART1;
            UCB = pUartCB[Port];
            Reg =  (tagUartReg *)CN_UART1_BASE;
            DmaBufLen = CFG_USART1_DMABUF_LEN;
            DmaRcvLen = s_USART1_DmaRcvLen;
            puart_dma_send_buf = (uint8_t*)USART1_DmaSendBuf;
            puart_dma_recv_buf = (uint8_t*)USART1_DmaRecvBuf;
            break;
        default:
            return 0;
    }

    IIR = Reg->UART_SR;

    if(s_UART_DmaUsed[Port] == cn_dma_unused)//��DMA��ʽ���ͺͽ���
    {
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
                Board_UartHalfDuplexRecv(Port);
                __UART_SendIntDisable(Reg,s_UART_DmaUsed[Port]);
            }
        }
    }
    else            //DMA��ʽ���ͺͽ���
    {
        if((IIR & (1<<3)) && (Reg->UART_IMR &(1<<3)))//endrx int
        {
            if(DmaRcvLen > DmaBufLen)                   //�����DMA BUF�ж���������
                num = DmaBufLen;
            else
                num = DmaRcvLen;
            UART_PortWrite(UCB,puart_dma_recv_buf,num);

            DmaRcvLen = DmaRcvLen - num;
            if(DmaRcvLen > DmaBufLen)                   //�����´�DMA����������
                Reg->UART_RCR  = DmaBufLen;
            else if(DmaRcvLen > 1)
                Reg->UART_RCR = DmaRcvLen;
            else                                        //����DMA����Ϊ1
            {
                Reg->UART_RCR = 1;
                DmaRcvLen = 1;
            }

            Reg->UART_RPR = (vu32)puart_dma_recv_buf;
        }
        if((IIR & (1<<11)) && (Reg->UART_IMR &(1<<11)))//txbufe int
        {
            while(!(Reg->UART_SR & (1<<9)));//wait for empty

            num = UART_PortRead(UCB,puart_dma_send_buf, DmaBufLen);
            if(num > 0)
            {
                Reg->UART_PTCR = (1<<9);//diable dma tx
                if((Reg->UART_TCR==0)&&(Reg->UART_TNCR==0))
                {
//                    num = UART_PortRead(UCB,puart_dma_send_buf,
//                          DmaBufLen,0);
                    Reg->UART_TPR  = (uint32_t)puart_dma_send_buf;
                    Reg->UART_TCR  = num;
                }
                Reg->UART_PTCR = (1<<8);//enable dma tx
            }
            else
            {
                //�Ƿ����ֹ dma tx int
                while(!__UART_TxTranEmpty(Reg));
                __UART_SendIntDisable(Reg,s_UART_DmaUsed[Port]);
            }
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
    case CN_UART0://����1
        UART_Param.Name         = "UART0";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = (ptu32_t)CN_UART0_BASE;
        UART_Param.TxRingBufLen = CFG_UART0_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART0_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART1://����2
        UART_Param.Name         = "UART1";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = (ptu32_t)CN_UART1_BASE;
        UART_Param.TxRingBufLen = CFG_UART1_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART1_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_USART0://����1
        UART_Param.Name         = "USART0";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = (ptu32_t)CN_USART0_BASE;
        UART_Param.TxRingBufLen = CFG_USART0_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_USART0_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_USART1://����2
        UART_Param.Name         = "USART1";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = (ptu32_t)CN_USART1_BASE;
        UART_Param.TxRingBufLen = CFG_USART1_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_USART1_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    default:
        return 0;
    }
    UART_Param.mode = CN_UART_GENERAL;
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

    CR_Bak = PutStrDirectReg->UART_IER;             //Save INT
    __UART_SendIntDisable(PutStrDirectReg,0);       //disable send INT
    __UART_SendIntDisable(PutStrDirectReg,1);       //disable send INT when dma
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
        PutStrDirectReg->UART_THR = str[result];
    }
    PutStrDirectReg->UART_IER = CR_Bak;             //restore send INT
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

    CR_Bak = PutStrDirectReg->UART_IER;             //Save INT
    __UART_SendIntDisable(PutStrDirectReg,0);       //disable send INT
    __UART_SendIntDisable(PutStrDirectReg,1);       //disable send INT when dma
    while(__UART_RxHadChar(GetCharDirectReg) == false) ;
    result = GetCharDirectReg->UART_RHR;
    PutStrDirectReg->UART_IER = CR_Bak;             //restore send INT
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
