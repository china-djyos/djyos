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
#include "pt32z192xx_uart.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern ptu32_t ModuleInstall_UART(u32 serial_no);
//
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
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip uart"//CPU��uart��������
//parent:"uart device file"      //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"uart device file" //������������������������none����ʾ�������������
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
#define CFG_UART0_SENDBUF_LEN            32                 //"UART0���ͻ��λ�������С",
#define CFG_UART0_RECVBUF_LEN            32                 //"UART0���ջ��λ�������С",

#define CFG_UART1_SENDBUF_LEN            32                 //"UART1���ͻ��λ�������С",
#define CFG_UART1_RECVBUF_LEN            32                 //"UART1���ջ��λ�������С",

#define CFG_UART2_SENDBUF_LEN            32                 //"UART2���ͻ��λ�������С",
#define CFG_UART2_RECVBUF_LEN            32                 //"UART2���ջ��λ�������С",
//%$#@enum,true,false,
#define CFG_UART0_ENABLE                 false              //"�Ƿ�����ʹ��UART0",
#define CFG_UART1_ENABLE                 false              //"�Ƿ�����ʹ��UART1",
#define CFG_UART2_ENABLE                 true               //"�Ƿ�����ʹ��UART2",
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

static u32 TxByteTime;                    //��������һ���ֽ�����Ҫ��ʱ��
static CMSDK_UART_TypeDef *PutStrDirectReg;     //����printk���͵Ĵ��ڼĴ���
static CMSDK_UART_TypeDef *GetCharDirectReg;     //����ֱ�ӽ��յĴ��ڼĴ���
static u32 TxDirectPort;                  //����printk���͵Ĵ��ں�
static u32 RxDirectPort;                  //����ֱ�ӽ��յĴ��ں�
// =============================================================================
static CMSDK_UART_TypeDef volatile * const tg_UART_Reg[] = {UART0, UART1, UART2};

static struct UartGeneralCB *pUartCB[CN_UART_NUM];

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
// ����: ��ȡuart�˿ڵ��ж�ʹ�ܿ���λ
// ����: port,���ںţ�1~5
// ����: ��
// =============================================================================
u32 __UART_GetIEReg(u8 port)
{
    switch(port)
    {
    case CN_UART0:
        return UART0->IER;
        break;
    case CN_UART1:
        return UART1->IER;
        break;
    case CN_UART2:
        return UART2->IER;
        break;
    default:
        break;
    }
    return 0;
}

// =============================================================================
// ����: ��ֹuart�ķ����жϡ�
// ����: port,���ںţ�1~5
// ����: ��
// �ر�˵����PT192��UART_IER_TOIDLEIE�жϲ���������ʲ����ã�ֻ��ʹ�� UART_IER_RXNEIE�ж�
// =============================================================================
void __UART_RxIntEnable(u8 port)
{
    switch(port)
    {
    case CN_UART0:
        UART_ITConfig(UART0, UART_IER_RXNEIE,ENABLE);
        break;
    case CN_UART1:
        UART_ITConfig(UART1, UART_IER_RXNEIE,ENABLE);
        break;
    case CN_UART2:
        UART_ITConfig(UART2, UART_IER_RXNEIE,ENABLE);
        break;
    default:
        break;
    }
}

// =============================================================================
// ����: ��ֹuart�Ľ����ж�
// ����: port,���ں�
// ����: ��
// =============================================================================
void __UART_RxIntDisable(u8 port)
{
    switch(port)
    {
    case CN_UART0:
        UART_ITConfig(UART0, UART_IER_RXNEIE,DISABLE);
        break;
    case CN_UART1:
        UART_ITConfig(UART1, UART_IER_RXNEIE,DISABLE);
        break;
    case CN_UART2:
        UART_ITConfig(UART2, UART_IER_RXNEIE,DISABLE);
        break;
    default:
        break;
    }
}

// =============================================================================
// ����: ʹ��uart�ķ����ж�
// ����: port,���ں�
// ����: ��
// =============================================================================
static void __UART_TxIntEnable(u8 port)
{
    switch(port)
    {
    case CN_UART0:
        UART_ITConfig(UART0, UART_IER_TXEIE+UART_IER_TXHEIE+UART_IER_TXENDIE,ENABLE);
        break;
    case CN_UART1:
        UART_ITConfig(UART1, UART_IER_TXEIE+UART_IER_TXHEIE+UART_IER_TXENDIE,ENABLE);
        break;
    case CN_UART2:
        UART_ITConfig(UART2, UART_IER_TXEIE+UART_IER_TXHEIE+UART_IER_TXENDIE,ENABLE);
        break;
    default:
        break;
    }
}

// =============================================================================
// ����: ��ֹUART�ķ����ж�
// ����: port,���ں�
// ����: ��
// =============================================================================
static void __UART_TxIntDisable(u8 port)
{
    switch(port)
    {
    case CN_UART0:
        UART_ITConfig(UART0, UART_IER_TXEIE+UART_IER_TXHEIE+UART_IER_TXENDIE,DISABLE);
        break;
    case CN_UART1:
        UART_ITConfig(UART1, UART_IER_TXEIE+UART_IER_TXHEIE+UART_IER_TXENDIE,DISABLE);
        break;
    case CN_UART2:
        UART_ITConfig(UART2, UART_IER_TXEIE+UART_IER_TXHEIE+UART_IER_TXENDIE,DISABLE);
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
static bool_t __UART_TxTranEmpty(CMSDK_UART_TypeDef volatile *reg)
{
    return (0 != (reg->SR & UART_FLAG_TXE));
}

// =============================================================================
// ����: ��鷢�ͼĴ�����״̬������վͷ���true����Ϊ�գ����򷵻�false
// ������reg,�������ļĴ�����ָ��
// ����: true = �������գ�false = �ǿ�
// =============================================================================
static bool_t __UART_TxFifoFull(CMSDK_UART_TypeDef volatile *reg)
{
    return (0 != (reg->SR & UART_FLAG_TXF));
}

// =============================================================================
// ����: �����ռĴ�����״̬�������ݾͷ���true�����򷵻�false
// ������reg,�������ļĴ�����ָ��
// ����: true = �����ݣ�false = ������
// =============================================================================
static bool_t __UART_RxHadChar(CMSDK_UART_TypeDef volatile *reg)
{
    return (1 == (reg->SR & UART_FLAG_RXNE));
}

// =============================================================================
// ����: ���ô���baud,PCLKΪ25M��CPU��ƵΪ100M����,������ֻ�ṩ9600��19200��57600��
//       115200���ֲ���������
// ����: Reg,�������ļĴ�����ָ��
//       baud,������
// ����: ��
// =============================================================================
static void __UART_BaudSet(CMSDK_UART_TypeDef volatile *Reg,u32 baud)
{
    uint32_t  tmpreg = 0 ,divider = 0;

    divider = CN_CFG_PCLK2/(16*baud);
    tmpreg = CN_CFG_PCLK2%(16*baud);

    if (tmpreg > ((16/2) * baud))
    {
       Reg->BRR = (uint16_t) (divider + 1);
    }
    else
    {
        Reg->BRR = (uint16_t) divider;
    }
    Reg->BRR &= ~UART_BRR_SR;
    Reg->BRR |= UART_BRR_SRX16;

    if(Reg == PutStrDirectReg)
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
static void __UART_ComConfig(CMSDK_UART_TypeDef volatile *Reg,struct COMParam *COM)
{
    if((COM == NULL) || (Reg == NULL))
        return;
    __UART_BaudSet(Reg,COM->BaudRate);

    switch(COM->DataBits)               // data bits
    {
//  case CN_UART_DATABITS_8:    Reg->CR1 &= ~(1<<12);break;
//  case CN_UART_DATABITS_9:    Reg->CR1 |= (1<<12);;break;
    default:break;
    }

    switch(COM->Parity)                 // parity
    {
//  Reg->CR1 |= (1<<10);                // enable parity
//  case CN_UART_PARITY_ODD:    Reg->CR1 |= (1<<9);break;
//  case CN_UART_PARITY_EVEN:   Reg->CR1 &=~(1<<9);break;
    default:break;
    }

    switch(COM->StopBits)               // stop bits
    {
//  Reg->CR2 &= ~(3<<12);
//  case CN_UART_STOPBITS_1:    Reg->CR2 |= (0<<12);break;
//  case CN_UART_STOPBITS_1_5:  Reg->CR2 |= (1<<12);break;
//  case CN_UART_STOPBITS_2:    Reg->CR2 |= (2<<12);break;
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
    case CN_UART0:
        UART0->CR |=UART_CR_RXEN;
        break;
    case CN_UART1:
        UART1->CR |=UART_CR_RXEN;
        break;
    case CN_UART2:
        UART2->CR |=UART_CR_RXEN;
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
    case CN_UART0:
        UART0->CR &=~UART_CR_RXEN;
        break;
    case CN_UART1:
        UART1->CR &=~UART_CR_RXEN;
        break;
    case CN_UART2:
        UART2->CR &=~UART_CR_RXEN;
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
    if(SerialNo == CN_UART0)
        IntLine = CN_INT_LINE_USART0;
    else if(SerialNo == CN_UART1)
        IntLine = CN_INT_LINE_USART1;
    else if(SerialNo == CN_UART2)
        IntLine = CN_INT_LINE_USART2;
    else
        return;

    Int_Register(IntLine);
    Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(IntLine,UART_ISR);
    Int_SetIsrPara(IntLine,SerialNo);
    Int_SettoAsynSignal(IntLine);
    Int_ClearLine(IntLine);
    Int_RestoreAsynLine(IntLine);
    __UART_RxIntEnable(SerialNo);
}

// =============================================================================
// ����: Ӳ���������úͼĴ����ĳ�ʼ�������������ʡ�ֹͣλ��У��λ������λ��Ĭ�������:
//       ������:115200  �� ֹͣλ:1 ; У��:żУ�� ; ����λ:8bit
// ����: SerialNo,���ںţ�0~3
// ����: ��
// =============================================================================
static void __UART_HardInit(u8 SerialNo)
{
    CMSDK_UART_TypeDef *uart;
    if(SerialNo == CN_UART0)
        uart = UART0;
    else if(SerialNo == CN_UART1)
        uart = UART1;
    else if(SerialNo == CN_UART2)
        uart = UART2;
    else
        return;

    UART_InitTypeDef UART_InitStruct;
    UART_InitStruct.UART_BaudRate = 115200;
    UART_InitStruct.UART_WordLength = UARTM_8D;
    UART_InitStruct.UART_StopBit = UART1StopBit;
    UART_InitStruct.UART_Parity = UART_ODD_PARITY;
    UART_InitStruct.UART_RXEN = ENABLE;
    UART_InitStruct.UART_SampleRate = UART_SAMPLERATEX16;
    UART_InitStruct.FCPU = CN_CFG_AHBCLK;
    //�˺��� baud �Ƚϸ�ʱ�ƺ���׼������΢��ʾ��������ע��
    UART_Init(uart, &UART_InitStruct);
    UART_Cmd(uart, ENABLE);

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
static u32 __UART_SendStart (CMSDK_UART_TypeDef *Reg)
{
    u8 port;


    switch((u32)Reg)
    {
    case (u32)UART0:
        port = CN_UART0;
        break;
    case (u32)UART1:
        port = CN_UART1;
        break;
    case (u32)UART2:
        port = CN_UART2;
        break;
    default:
        return 0;
    }
    Board_UartHalfDuplexSend(port);//�л�������
//  __UART_TxIntDisable(port);
    //ֻҪʹ�ܷ��Ϳ��жϣ��򴥷��ж�
    __UART_TxIntEnable(port);
    return 1;
}

// =============================================================================
// ����: �����豸�Ŀ��ƺ���,������Ӳ���Ĵ����������
// ����: Reg,�������Ĵ��ڼĴ���ָ��.
//       cmd,��������
//       data1,data2,������cmd����
// ����: ������.
// =============================================================================
static ptu32_t __UART_Ctrl(CMSDK_UART_TypeDef *Reg,u32 cmd, va_list *arg0)
{
    ptu32_t result = 0;
    u32 port;
    if(Reg == NULL)
        return 0;

    switch((u32)Reg)
    {
        case (u32)UART0: port = CN_UART0;break;
        case (u32)UART1: port = CN_UART1;break;
        case (u32)UART2: port = CN_UART2;break;
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
            __UART_BaudSet(Reg, data);
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
            __UART_ComConfig(Reg,COM);
        }
            break;
        default: break;
    }
    return result;
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
    CMSDK_UART_TypeDef *Reg;
    u32 num;
    u8 ch;
    u8 revbuf[16];

    UCB = pUartCB[port];
    Reg = (CMSDK_UART_TypeDef *)tg_UART_Reg[port];

    num = 0;
    while( Reg->SR & UART_ISR_RXNE)         //�����ж�
    {
        revbuf[num++] = (u8)Reg->BR;
    }

    if(num != 0)
    {
        if(num != UART_PortWrite(UCB,revbuf,num))
        {
            UART_ErrHandle(UCB,CN_UART_BUF_OVER_ERR);
        }
    }

    if(Reg->SR & UART_ISR_TXEND)        //����ȫ������жϣ������жϷ���ʱ�����и�
                                        //����FIFO����жϱ�־����Ȼ��λ
    {
        num = UART_PortRead(UCB,&ch,1);
        if(num != 0)
        {
            Reg->BR = ch;
            while(!(Reg->SR & UART_ISR_TXF))        //ֱ��FIFOȫ��
            {
                num = UART_PortRead(UCB,&ch,1);
                if(num != 0)
                    Reg->BR = ch;
                else
                    break;
            }
        }
        else
        {
            __UART_TxIntDisable(port);
            Board_UartHalfDuplexRecv(port);
        }
    }
    else
    {
        while(!(Reg->SR & UART_ISR_TXF))        //ֱ��FIFOȫ��
        {
            num = UART_PortRead(UCB,&ch,1);
            if(num != 0)
                Reg->BR = ch;
            else
                break;
        }
    }
    if(Reg->SR & (UART_ISR_PERR+UART_ISR_FERR+UART_ISR_OVERR))        //���ִ���
    {
        ch = (u8)Reg->BR;
        Reg->SR = UART_ISR_PERR+UART_ISR_FERR+UART_ISR_OVERR;
        UART_ErrHandle(UCB,CN_UART_FIFO_OVER_ERR);
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
    case CN_UART0://����0
        UART_Param.Name         = "UART0";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = (ptu32_t)UART0;
        UART_Param.TxRingBufLen = CFG_UART0_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART0_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART1://����1
        UART_Param.Name         = "UART1";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = (ptu32_t)UART1;
        UART_Param.TxRingBufLen = CFG_UART1_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART1_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART2://����2
        UART_Param.Name         = "UART2";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = (ptu32_t)UART2;
        UART_Param.TxRingBufLen = CFG_UART2_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART2_RECVBUF_LEN;
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
    u32 timeout = TxByteTime * len+10;
    u8 port;
    for(port=0;port<CN_UART_NUM;port++)
    {
        if(PutStrDirectReg ==tg_UART_Reg[port])
            break;
    }
    Board_UartHalfDuplexSend(port);
    __UART_TxIntDisable(port);
    for(result=0; result <= len; result ++)
    {
        // ��ʱ���߷���FIFO����ʱ�˳�
        while((true == __UART_TxFifoFull(PutStrDirectReg)) && (timeout > TxByteTime))
        {
            timeout -=TxByteTime;
            DJY_DelayUs(TxByteTime);
        }
        if(timeout > TxByteTime)
            PutStrDirectReg->BR = str[result];
        else
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
//  u16 rxier;
    char result;
//
//  rxier = __UART_GetIEReg(port);
//  __UART_RxIntDisable(port);
    if(__UART_RxHadChar(GetCharDirectReg) == true)
        result = GetCharDirectReg->BR;
    else
        result = EOF;
//  PutStrDirectReg->CR1 = CR_Bak;                         //restore send INT
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
        PutStrDirectReg = UART0;
        TxDirectPort = CN_UART0;
    }
    else if(!strcmp(StdioOut,"/dev/UART1"))
    {
        PutStrDirectReg = UART1;
        TxDirectPort = CN_UART1;
    }
    else if(!strcmp(StdioOut,"/dev/UART2"))
    {
        PutStrDirectReg = UART2;
        TxDirectPort = CN_UART2;
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
        GetCharDirectReg = UART0;
        RxDirectPort = CN_UART0;
    }
    else if(!strcmp(StdioIn,"/dev/UART1"))
    {
        GetCharDirectReg = UART1;
        RxDirectPort = CN_UART1;
    }
    else if(!strcmp(StdioIn,"/dev/UART2"))
    {
        GetCharDirectReg = UART2;
        RxDirectPort = CN_UART2;
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
