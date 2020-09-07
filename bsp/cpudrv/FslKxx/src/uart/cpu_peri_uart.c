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

#include "cpu_peri.h"
#include "stdio.h"
#include <device/djy_uart.h>
#include "string.h"
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
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"device file system","lock","uart device file","cpu onchip xdma","heap"//������������������������none����ʾ�������������
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
#define CFG_UART0_SENDBUF_LEN    32             //"UART0���ͻ�������С",
#define CFG_UART0_RECVBUF_LEN    32             //"UART0���ջ�������С",

#define CFG_UART1_SENDBUF_LEN    32             //"UART1���ͻ����С",
#define CFG_UART1_RECVBUF_LEN    32             //"UART1���ջ����С",

#define CFG_UART2_SENDBUF_LEN    32             //"UART2���ͻ����С",
#define CFG_UART2_RECVBUF_LEN    32             //"UART2���ջ����С",

#define CFG_UART3_SENDBUF_LEN    32             //"UART3���ͻ����С",
#define CFG_UART3_RECVBUF_LEN    32             //"UART3���ջ����С",

#define CFG_UART4_SENDBUF_LEN    32             //"UART4���ͻ����С",
#define CFG_UART4_RECVBUF_LEN    32             //"UART4���ջ����С",

#define CFG_UART5_SENDBUF_LEN    32             //"UART5���ͻ����С",
#define CFG_UART5_RECVBUF_LEN    32             //"UART5���ջ����С",


//%$#@enum,true,false,
#define CFG_UART0_ENABLE         true           //"�Ƿ�����UART0",
#define CFG_UART1_ENABLE         false          //"�Ƿ�����UART1",
#define CFG_UART2_ENABLE         false          //"�Ƿ�����UART2",
#define CFG_UART3_ENABLE         false          //"�Ƿ�����UART3",
#define CFG_UART4_ENABLE         false          //"�Ƿ�����UART4",
#define CFG_UART5_ENABLE         false          //"�Ƿ�����UART4",
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

// =============================================================================
#define tagUartReg     UART_Type

static tagUartReg volatile * const tg_UART_Reg[] = {(tagUartReg *)CN_UART0_BASE,
                                                    (tagUartReg *)CN_UART1_BASE,
                                                    (tagUartReg *)CN_UART2_BASE,
                                                    (tagUartReg *)CN_UART3_BASE,
                                                    (tagUartReg *)CN_UART4_BASE,
                                                    (tagUartReg *)CN_UART5_BASE};

static u32 TxByteTime;                    //��������һ���ֽ�����Ҫ��ʱ��
static tagUartReg *PutStrDirectReg;     //����printk���͵Ĵ��ڼĴ���
static tagUartReg *GetCharDirectReg;     //����ֱ�ӽ��յĴ��ڼĴ���
static u32 TxDirectPort;                  //����printk���͵Ĵ��ں�
static u32 RxDirectPort;                  //����ֱ�ӽ��յĴ��ں�
// =============================================================================

//#define UART0_SendBufLen            32
//#define UART0_RecvBufLen            32

//#define UART1_SendBufLen            32
//#define UART1_RecvBufLen            32

//#define UART2_SendBufLen            32
//#define UART2_RecvBufLen            32

//#define UART3_SendBufLen            32
//#define UART3_RecvBufLen            32

//#define UART4_SendBufLen            32
//#define UART4_RecvBufLen            32

//#define UART5_SendBufLen            32
//#define UART5_RecvBufLen            32
static struct UartGeneralCB *pUartCB[CN_UART_NUM];

//���ڱ�ʶ�����Ƿ��ʼ����ǣ���0λ��ʾUART0����һλ��UART1....
//�������ƣ�1��ʾ��ʼ����0��ʾδ��ʼ��
static u8 sUartInited = 0;
// =============================================================================
static ptu32_t UART_ISR(ptu32_t IniLine);

__attribute__((weak))  void Board_UartHalfDuplexSend(u8 SerialNo)
{
    return;
}
__attribute__((weak))  void Board_UartHalfDuplexRecv(u8 SerialNo)
{
    switch (SerialNo)
    {
        case CN_UART0:
            break;
        default:
            break;
    }
}
// ----ʹ�ܽ����ж�-------------------------------------------------------------
// ����: ʹ��UART�Ľ����ж�
// ����: reg,�������ļĴ�����ָ��
// ����: ��
//-----------------------------------------------------------------------------
static void __UART_RxIntEnable(tagUartReg volatile *reg)
{
    reg->C2 |= UART_C2_RIE_MASK;
}

// ----��ֹ�����ж�-------------------------------------------------------------
// ����: ��ֹUART�Ľ����ж�
// ����: reg,�������ļĴ�����ָ��
// ����: ��
//-----------------------------------------------------------------------------
static void __UART_RxIntDisable(tagUartReg volatile *reg)
{
    reg->C2 &= ~UART_C2_RIE_MASK;
}

// ----ʹ�ܷ����ж�-------------------------------------------------------------
// ����: ʹ��UART�ķ����ж�
// ����: reg,�������ļĴ�����ָ��
// ����: ��
// -----------------------------------------------------------------------------
static void __UART_TxIntEnable(tagUartReg volatile *reg)
{
    reg->C2 |= UART_C2_TIE_MASK;
}

// ----��ֹ�����ж�-------------------------------------------------------------
// ����: ��ֹUART�ķ����ж�
// ����: reg,�������ļĴ�����ָ��
// ����: ��
// -----------------------------------------------------------------------------
static void __UART_TxIntDisable(tagUartReg volatile *reg)
{
    reg->C2 &= ~UART_C2_TIE_MASK;
}

// ----��鴮�ڷ��ͼĴ����Ƿ��--------------------------------------------------
// ����: ��鷢�ͼĴ�����״̬������վͷ���true���෴���򷵻�false
// �������ޡ�
// ����: true = �������գ�false = �ǿ�
// -----------------------------------------------------------------------------
static bool_t __UART_TxTranEmpty(tagUartReg volatile *reg)
{
    return (0 != (reg->S1 & UART_S1_TDRE_MASK));
}

// =============================================================================
// ����: �����ռĴ�����״̬�������ݾͷ���true�����򷵻�false
// ������reg,�������ļĴ�����ָ��
// ����: true = �����ݣ�false = ������
// =============================================================================
static bool_t __UART_RxHadChar(tagUartReg volatile *reg)
{
    return reg->RCFIFO; //��������������������ݷ���true,����false
}

// =============================================================================
// ����: ���ô��ڴ��ڵĲ����ʣ�FreescaleKxx��UART0��UART1ʱ��ԴΪCORE��������ʱ��Դ
//       ΪBUSʱ��
// ����: Reg,�������ļĴ�����ָ��
//       baud,������
// ����: ��
// =============================================================================
static void __UART_BaudSet(tagUartReg volatile *Reg,u32 baud)
{
    u32 UartClk;
    u16 Sbr,Brfa;
    u8 temp;

    if(Reg == NULL)     return;

    if(((u32)Reg == CN_UART0_BASE) || ((u32)Reg == CN_UART1_BASE))
        UartClk = CN_CFG_MCLK;
    else
        UartClk = CN_CFG_MCLK/2;

    Sbr = (u16)(UartClk/(baud * 16));
    Brfa = (((UartClk*32)/(baud * 16)) - (Sbr * 32));
    //���ò�����
    temp = Reg->BDH & ~(UART_BDH_SBR(0x1F));
    Reg->BDH = temp | UART_BDH_SBR(((Sbr & 0x1F00) >> 8));
    Reg->BDL = (u8)(Sbr & UART_BDL_SBR_MASK);
    //����У��
    temp = Reg->C4 & ~(UART_C4_BRFA(0x1F));
    Reg->C4 = temp |  UART_C4_BRFA(Brfa);

    if(tg_UART_Reg[TxDirectPort] == Reg)
    {
        TxByteTime = 11000000/baud;     //1���ֽڴ���ʱ�䣬��10bit��+10%����
    }
}

// =============================================================================
// ����: ���ö�ӦUART��IO�ڣ�����ʱ�Ӻ�IO����
// ����: SerialNo,���ں�
// ����: ��
// =============================================================================
static void __UART_GpioConfig(u8 SerialNo)
{
    GPIO_PowerOn(1);
    switch(SerialNo)
    {
    case CN_UART0:
        SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
        PORT_MuxConfig(PORT_PORT_A,PORT_PIN(1),PORT_PINMUX_ALT2);
        PORT_MuxConfig(PORT_PORT_A,PORT_PIN(2),PORT_PINMUX_ALT2);
        break;
    case CN_UART1:
        SIM->SCGC4 |= SIM_SCGC4_UART1_MASK;
        PORT_MuxConfig(PORT_PORT_E,PORT_PIN(0),PORT_PINMUX_ALT3);
        PORT_MuxConfig(PORT_PORT_E,PORT_PIN(1),PORT_PINMUX_ALT3);
        break;
    case CN_UART2:
        SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
        PORT_MuxConfig(PORT_PORT_E,PORT_PIN(16),PORT_PINMUX_ALT3);
        PORT_MuxConfig(PORT_PORT_E,PORT_PIN(17),PORT_PINMUX_ALT3);
        break;
    case CN_UART3:
        SIM->SCGC4 |= SIM_SCGC4_UART3_MASK;
        PORT_MuxConfig(PORT_PORT_E,PORT_PIN(4),PORT_PINMUX_ALT3);
        PORT_MuxConfig(PORT_PORT_E,PORT_PIN(5),PORT_PINMUX_ALT3);
        break;
    case CN_UART4:
        SIM->SCGC1 |= SIM_SCGC1_UART4_MASK;
        PORT_MuxConfig(PORT_PORT_E,PORT_PIN(24),PORT_PINMUX_ALT3);
        PORT_MuxConfig(PORT_PORT_E,PORT_PIN(25),PORT_PINMUX_ALT3);
        break;
    case CN_UART5:
        SIM->SCGC1 |= SIM_SCGC1_UART5_MASK;
        PORT_MuxConfig(PORT_PORT_E,PORT_PIN(8),PORT_PINMUX_ALT3);
        PORT_MuxConfig(PORT_PORT_E,PORT_PIN(9),PORT_PINMUX_ALT3);
        break;
    default:
        break;
    }
}

// =============================================================================
// ����: �Դ��ڴ���������ã����������ʡ���żУ�顢����λ��ֹͣλ
// ����:  Reg,�������ļĴ�����ָ��
//        data,�ṹ��tagCOMParam���͵�ָ����ֵ
// ����: ��
// =============================================================================
static void __UART_ComConfig(tagUartReg volatile *Reg,struct COMParam *COM)
{
    if((COM == NULL) || (Reg == NULL))
        return;
    __UART_BaudSet(Reg,COM->BaudRate);

    switch(COM->DataBits)               // data bits
    {
    case CN_UART_DATABITS_8:    Reg->C1 &= ~UART_C1_M_MASK;break;
    case CN_UART_DATABITS_9:    Reg->C1 = UART_C1_M_MASK; break;
    default:break;
    }

    switch(COM->Parity)                 // parity
    {
    Reg->C1 |= UART_C1_PE_MASK;
    case CN_UART_PARITY_ODD:    Reg->C1 |= UART_C1_PT_MASK;break;
    case CN_UART_PARITY_EVEN:   Reg->C1 &=~UART_C1_PT_MASK;break;
    default:break;
    }

    switch(COM->StopBits)               // stop bits
    {
    default:break;                      //k60û�ҵ�ֹͣλ����
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
    if(SerialNo == CN_UART0)
        IntLine = CN_INT_LINE_UART0_RX_TX;
    else if(SerialNo == CN_UART1)
        IntLine = CN_INT_LINE_UART1_RX_TX;
    else if(SerialNo == CN_UART2)
        IntLine = CN_INT_LINE_UART2_RX_TX;
    else if(SerialNo == CN_UART3)
        IntLine = CN_INT_LINE_UART3_RX_TX;
    else if(SerialNo == CN_UART4)
        IntLine = CN_INT_LINE_UART4_RX_TX;
    else if(SerialNo == CN_UART5)
        IntLine = CN_INT_LINE_UART5_RX_TX;
    else
        return;

    if(true == Int_Register(IntLine))
    {
        Int_IsrConnect(IntLine,UART_ISR);
        Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
        Int_SettoAsynSignal(IntLine);
        Int_ClearLine(IntLine);
        Int_RestoreAsynLine(IntLine);
    }
}

// =============================================================================
// ����: Ӳ���������úͼĴ����ĳ�ʼ�������������ʡ�ֹͣλ��У��λ������λ��Ĭ�������:
//       ������:115200  �� ֹͣλ:1 ; У��:żУ�� ; ����λ:8bit
// ����: SerialNo,���ںţ�0~5
// ����: ��
// =============================================================================
static void __UART_HardInit(u8 SerialNo)
{
    if(SerialNo > CN_UART5)
        return;

    //GPIO������
    __UART_GpioConfig(SerialNo);
    __UART_BaudSet(tg_UART_Reg[SerialNo],115200);

    tg_UART_Reg[SerialNo]->C2 |= UART_C2_TIE_MASK;      //ʹ��TIE�ж�
    tg_UART_Reg[SerialNo]->C2 |= UART_C2_RIE_MASK;      //ʹ��RIE�ж�
//  tg_UART_Reg[SerialNo]->C2 |= UART_C2_ILIE_MASK;     //ʹ��ILIE�ж�
    tg_UART_Reg[SerialNo]->PFIFO = UART_PFIFO_TXFE_MASK | UART_PFIFO_RXFE_MASK;
    tg_UART_Reg[SerialNo]->C2 |= UART_C2_RE_MASK;       //ʹ�ܽ���
    tg_UART_Reg[SerialNo]->C2 |= UART_C2_TE_MASK;       //ʹ�ܷ���
}

// =============================================================================
// ����: ֱ�ӷ��ͺ�����ʹ����ѯ��ʽ��
// ����: PrivateTag,�������Ĵ����豸ָ��.
//       send_buf,�������ݻ�����
//       len,���ݳ��ȣ��ֽ�
//       timeout,��ʱʱ�䣬us
// ����: �����ֽ���
// =============================================================================
//static u32 __UART_SendDirectly(tagUartReg *Reg,u8 *send_buf,u32 len,u32 timeout)
//{
//    u32  result;
//
//    if(NULL == Reg)     return 0;
//    __UART_TxIntDisable(Reg);
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
//        Reg->D = send_buf[result];
//    }
//    __UART_TxIntEnable(Reg);
//    return result;
//}

// =============================================================================
// ����: �������ڷ��ͣ���Ŀ���Ǵ����жϣ����жϷ�ʽ�������ݡ�
// ����: PrivateTag,�������Ĵ����豸ָ����ֵ.
// ����: ���͵ĸ���
// =============================================================================
static u32 __UART_SendStart (tagUartReg *Reg)
{
    u8 fifodep=1,num,ch[8],port;

    switch((u32)Reg)
    {
    //UART0��UART1��FIFO���Ϊ8����������Ϊ1
    case CN_UART0_BASE:   port = CN_UART0;   fifodep = 8;  break;
    case CN_UART1_BASE:   port = CN_UART1;   fifodep = 8;  break;
    case CN_UART2_BASE:   port = CN_UART2;   break;
    case CN_UART3_BASE:   port = CN_UART3;   break;
    case CN_UART4_BASE:   port = CN_UART4;   break;
    case CN_UART5_BASE:   port = CN_UART5;   break;
    default:return 0;
    }
    Board_UartHalfDuplexSend(port);
    __UART_TxIntDisable(Reg);
    if(__UART_TxTranEmpty(Reg))
    {
        fifodep = UART_PortRead(pUartCB[port],ch,fifodep);// UART��FIFO��СΪ8�ֽ�
        for(num = 0; num < fifodep; num++)
        {
            Reg->D = ch[num];
        }
    }
    __UART_TxIntEnable(Reg);
    return num;
}

//----�����豸���ƺ���---------------------------------------------------------
//����: �����豸�Ŀ��ƺ���,������Ӳ���Ĵ����������
//����: PrivateTag,�������Ĵ����豸ָ����ֵ.
//      cmd,��������
//      data,������cmd����
//����: ������.
//-----------------------------------------------------------------------------
static ptu32_t __UART_Ctrl(tagUartReg *Reg,u32 cmd, va_list *arg0)
{

    switch(cmd)
    {
        case CN_DEV_CTRL_START:
            Reg->C2 |= UART_C2_TE_MASK;
            Reg->C2 |= UART_C2_RE_MASK;
            break;
        case CN_DEV_CTRL_STOP:
            Reg->C2 &= ~UART_C2_TE_MASK;
            Reg->C2 &= ~UART_C2_RE_MASK;
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
        case CN_UART_RX_PAUSE:      //��ͣ����
            __UART_RxIntDisable(Reg);
            break;
        case CN_UART_RX_RESUME:      //�ָ�����
            __UART_RxIntEnable(Reg);
            break;
        case CN_UART_RECV_HARD_LEVEL:    //���ý���fifo����ˮƽ
            Reg->RWFIFO = data1;    //������ˮƽ
            Reg->TWFIFO = data2;    //д����ˮƽ
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
    return 0;
}

// =============================================================================
// ����: UART���жϷ��������������Ϳա����ա����������жϡ�
//      1.�����жϣ��ӽ���FIFO�ж����ݣ�������UART_PortWriteд����ջ������У�
//      2.�����жϣ�����UART_PortRead�����ͻ����������ݣ���д�뷢��FIFO��
//      3.�����жϣ����������¼�
// ����: �ж��ߺ�.
// ����: 0.
// =============================================================================
static u32 UART_ISR(ptu32_t IntLine)
{
    struct UartGeneralCB *UCB;
    volatile tagUartReg *Reg;
    u32 i,fifodep=1,num=0;
    u8 ch[16];
    u8 port;

    switch(IntLine)
    {
    case CN_INT_LINE_UART0_RX_TX:
    case CN_INT_LINE_UART0_ERR:
        UCB = pUartCB[CN_UART0];
        Reg = tg_UART_Reg[CN_UART0];
        fifodep = 8;
        port = CN_UART0;
        break;
    case CN_INT_LINE_UART1_RX_TX:
    case CN_INT_LINE_UART1_ERR:
        UCB = pUartCB[CN_UART1];
        Reg = tg_UART_Reg[CN_UART1];
        fifodep = 8;
        port = CN_UART1;
        break;
    case CN_INT_LINE_UART2_RX_TX:
    case CN_INT_LINE_UART2_ERR:
        UCB = pUartCB[CN_UART2];
        Reg = tg_UART_Reg[CN_UART2];
        port = CN_UART2;
        break;
    case CN_INT_LINE_UART3_RX_TX:
    case CN_INT_LINE_UART3_ERR:
        UCB = pUartCB[CN_UART3];
        Reg = tg_UART_Reg[CN_UART3];
        port = CN_UART3;
        break;
    case CN_INT_LINE_UART4_RX_TX:
    case CN_INT_LINE_UART4_ERR:
        UCB = pUartCB[CN_UART4];
        Reg = tg_UART_Reg[CN_UART4];
        port = CN_UART4;
        break;
    case CN_INT_LINE_UART5_RX_TX:
    case CN_INT_LINE_UART5_ERR:
        UCB = pUartCB[CN_UART5];
        Reg = tg_UART_Reg[CN_UART5];
        port = CN_UART5;
        break;
    default:return 0;
    }

    if(Reg->S1 & UART_S1_RDRF_MASK)         //�����жϱ�־
    {
        for(num = 0; num < fifodep; num++)
        {
            if(!(Reg->SFIFO & UART_SFIFO_RXEMPT_MASK))
                ch[num] = Reg->D;
            else
                break;
        }
        UART_PortWrite(UCB,ch,num);
    }

    if(Reg->S1 & UART_S1_TDRE_MASK)         //�����жϱ�־
    {
        num = UART_PortRead(UCB,ch,fifodep);
        if(num>0)
        {
            for(i = 0; i < num; i++)
                Reg->D = ch[i++];
        }
        else
        {
            __UART_TxIntDisable(Reg);
            Board_UartHalfDuplexRecv(port);
        }
    }

    if(Reg->S1 & (UART_S1_FE_MASK | UART_S1_PF_MASK))   //Ӳ������
    {
        UART_ErrHandle(UCB,CN_UART_HARD_COMM_ERR);
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
        UART_Param.UartPortTag  = CN_UART0_BASE;
        UART_Param.TxRingBufLen = CFG_UART0_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART0_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
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

    CR_Bak = GetCharDirectReg->C2;                          //Save INT
    __UART_TxIntDisable(GetCharDirectReg);                  //disable send INT
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
        PutStrDirectReg->D = str[result];
    }
    PutStrDirectReg->C2 = CR_Bak;                         //restore send INT
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

    CR_Bak = GetCharDirectReg->C2;                          //Save INT
    __UART_TxIntDisable(GetCharDirectReg);                  //disable send INT
    while(__UART_RxHadChar(GetCharDirectReg) == false) ;
    result = GetCharDirectReg->D;
    PutStrDirectReg->C2 = CR_Bak;                         //restore send INT
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

    if(!strcmp(StdioOut,"/dev/UART0"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART0_BASE;
        RxDirectPort = CN_UART0;
    }
    else if(!strcmp(StdioOut,"/dev/UART1"))
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
