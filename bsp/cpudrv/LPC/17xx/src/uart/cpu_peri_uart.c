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
#include "string.h"
#include <device/djy_uart.h>
#include "stdio.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern ptu32_t ModuleInstall_UART(ptu32_t SerialNo);
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
//    if( ModuleInstall_UART(CN_UART3);
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
//dependence:"uart device file","device file system","heap"//������������������������none����ʾ�������������
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

#define CFG_UART3_SENDBUF_LEN            32                 //"UART3���ͻ��λ�������С",
#define CFG_UART3_RECVBUF_LEN            32                 //"UART3���ջ��λ�������С",
//%$#@enum,true,false,
#define CFG_UART0_ENABLE                 true               //"�Ƿ�����ʹ��UART0",
#define CFG_UART1_ENABLE                 false              //"�Ƿ�����ʹ��UART1",
#define CFG_UART2_ENABLE                 false              //"�Ƿ�����ʹ��UART2",
#define CFG_UART3_ENABLE                 false              //"�Ƿ�����ʹ��UART3",
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

extern bool_t Board_UartGpioInit(u8 SerialNo);
extern void Board_UartHalfDuplexSend(u8 SerialNo);
extern void Board_UartHalfDuplexRecv(u8 SerialNo);
// =============================================================================
#define tagUartReg              LPC_UART_TypeDef
#define CN_UART0_BASE           LPC_UART0_BASE
#define CN_UART1_BASE           LPC_UART1_BASE
#define CN_UART2_BASE           LPC_UART2_BASE
#define CN_UART3_BASE           LPC_UART3_BASE

static u32 TxByteTime;                    //��������һ���ֽ�����Ҫ��ʱ��
static tagUartReg *PutStrDirectReg;     //����printk���͵Ĵ��ڼĴ���
static tagUartReg *GetCharDirectReg;     //����ֱ�ӽ��յĴ��ڼĴ���
static u32 TxDirectPort;                  //����printk���͵Ĵ��ں�
static u32 RxDirectPort;                  //����ֱ�ӽ��յĴ��ں�
// =============================================================================
static tagUartReg volatile * const tg_UART_Reg[] = {(tagUartReg *)CN_UART0_BASE,
                                                    (tagUartReg *)CN_UART1_BASE,
                                                    (tagUartReg *)CN_UART2_BASE,
                                                    (tagUartReg *)CN_UART3_BASE};
//#define CFG_UART0_SENDBUF_LEN            32
//#define CFG_UART0_RECVBUF_LEN            32
//
//#define CFG_UART1_SENDBUF_LEN            32
//#define CFG_UART1_RECVBUF_LEN            32
//
//#define CFG_UART2_SENDBUF_LEN            32
//#define CFG_UART2_RECVBUF_LEN            32
//
//#define CFG_UART3_SENDBUF_LEN            32
//#define CFG_UART3_RECVBUF_LEN            32

static struct UartGeneralCB *pUartCB[CN_UART_NUM];
//���ڱ�ʶ�����Ƿ��ʼ����ǣ���0λ��ʾUART0����һλ��UART1....
//�������ƣ�1��ʾ��ʼ����0��ʾδ��ʼ��
static u8 sUartInited = 0;
// =============================================================================
static ptu32_t UART_ISR(ptu32_t IniLine);

// ----ʹ�ܽ����ж�-------------------------------------------------------------
// ����: ʹ��uart�Ľ����ж�
// ����: reg,�������ļĴ�����ָ��
// ����: ��
//-----------------------------------------------------------------------------
static void __UART_RxIntEnable(tagUartReg volatile *reg)
{
    reg->IER |= (0x01 << 0);
}

// ----��ֹ�����ж�-------------------------------------------------------------
// ����: ��ֹuart�Ľ����ж�
// ����: reg,�������ļĴ�����ָ��
// ����: ��
//-----------------------------------------------------------------------------
static void __UART_RxIntDisable(tagUartReg volatile *reg)
{
    reg->IER &= ~(0x01 << 0);
}

// ----ʹ�ܷ����ж�-------------------------------------------------------------
// ����: ʹ��uart�ķ����ж�
// ����: reg,�������ļĴ�����ָ��
// ����: ��
// -----------------------------------------------------------------------------
static void __UART_TxIntEnable(tagUartReg volatile *reg)
{
    reg->IER |= (0x01 << 1);
}

// ----��ֹ�����ж�-------------------------------------------------------------
// ����: ��ֹuart�ķ����ж�
// ����: reg,�������ļĴ�����ָ��
// ����: ��
// -----------------------------------------------------------------------------
static void __UART_TxIntDisable(tagUartReg volatile *reg)
{
    reg->IER &= ~(0x01 << 1);
}

// ----��鴮�ڷ��ͼĴ����Ƿ��--------------------------------------------------
// ����: ��鷢�ͼĴ�����״̬������վͷ���true����Ϊ�գ����򷵻�false
// ������reg,�������ļĴ�����ָ��
// ����: true = �������գ�false = �ǿ�
// -----------------------------------------------------------------------------
static bool_t __UART_TxTranEmpty(tagUartReg volatile *reg)
{
    return (0 != (reg->LSR & (1<<5)));
}

// =============================================================================
// ����: �����ռĴ�����״̬�������ݾͷ���true�����򷵻�false
// ������reg,�������ļĴ�����ָ��
// ����: true = �����ݣ�false = ������
// =============================================================================
static bool_t __UART_RxHadChar(tagUartReg volatile *reg)
{
    return (reg->LSR & 0x01);
}

// =============================================================================
// ����: ���ô���baud,PCLKΪ25M��CPU��ƵΪ100M����,������ֻ�ṩ9600��19200��57600��
//       115200���ֲ���������,ʱ�Ӹĳ�100M�������ֲ�318ҳ��ʾ��
//       2*PCLK/(16*2^15) <= BAUD <= PCLK/(16*(2+data bits+parity bits +stop bits))
//       ���������PCLKΪ100Mʱ��������ʲ�����560K
// ����: Reg,�������ļĴ�����ָ��
//       baud,������
// ����: ��
// =============================================================================
static void __UART_BaudSet(tagUartReg volatile *Reg,u32 baud)
{
    u8 DLL,DLM;
    u32 temp;
    if(Reg == NULL)
        return;

    temp = ( CN_CFG_CCLK / 16 ) / baud ;    /*baud rate */
    DLM = temp / 256;
    DLL = temp % 256;

    Reg->LCR |= (1<<7);         //  DLAB = 1
    Reg->DLL  = DLL;            // 115200
    Reg->DLM  = DLM;
    Reg->LCR &= ~(1<<7);        // DLAB = 0

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
    //UART ��GPIO��ʼ��Ӧ���ݰ����������˸ú�����boarddrv�ļ��е�board.c��
    Board_UartGpioInit(SerialNo);
}

// =============================================================================
// ����: �Դ��ڴ���������ã����������ʡ���żУ�顢����λ��ֹͣλ
// ����:  Reg,�������ļĴ�����ָ��
//        data,�ṹ��tagCOMParam���͵�ָ����ֵ
// ����: ��
// =============================================================================
static void __UART_ComConfig(tagUartReg volatile *Reg,struct COMParam *COM)
{
    struct COMParam *COM;
    if((COM == NULL) || (Reg == NULL))
        return;
    __UART_BaudSet(Reg,COM->BaudRate);

    switch(COM->DataBits)               // data bits
    {
    case CN_UART_DATABITS_5:    Reg->LCR &= ~0x3; Reg->LCR |= 0x00;break;
    case CN_UART_DATABITS_6:    Reg->LCR &= ~0x3; Reg->LCR |= 0x01;break;
    case CN_UART_DATABITS_7:    Reg->LCR &= ~0x3; Reg->LCR |= 0x02;break;
    case CN_UART_DATABITS_8:    Reg->LCR &= ~0x3; Reg->LCR |= 0x03;break;
    default:break;
    }

    switch(COM->Parity)                 // parity
    {
    case CN_UART_PARITY_ODD:    Reg->LCR &=~(0x03<<4);Reg->LCR |=0x00<<4;break;
    case CN_UART_PARITY_EVEN:   Reg->LCR &=~(0x03<<4);Reg->LCR |=0x01<<4;break;
    case CN_UART_PARITY_MARK:   Reg->LCR &=~(0x03<<4);Reg->LCR |=0x02<<4;break;
    case CN_UART_PARITY_SPACE:  Reg->LCR &=~(0x03<<4);Reg->LCR |=0x03<<4;break;
    default:break;
    }

    switch(COM->StopBits)               // stop bits
    {
    case CN_UART_STOPBITS_1:    Reg->LCR &= ~(0x01<<2);break;
    case CN_UART_STOPBITS_1_5:  break;
    case CN_UART_STOPBITS_2:    Reg->LCR |= (0x01<<2);break;
    default:break;
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
        IntLine = CN_INT_LINE_UART0;
    else if(SerialNo == CN_UART1)
        IntLine = CN_INT_LINE_UART1;
    else if(SerialNo == CN_UART2)
        IntLine = CN_INT_LINE_UART2;
    else if(SerialNo == CN_UART3)
        IntLine = CN_INT_LINE_UART3;
    else
        return;
    Int_Register(IntLine);
    Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(IntLine,UART_ISR);
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
    if(SerialNo > CN_UART3)
        return;
    __UART_GpioConfig(SerialNo);
    if(SerialNo == CN_UART0)
    {
        LPC_SC->PCONP |= (1<<3);
        LPC_SC->PCLKSEL0 &= ~(0x03<<6);
        LPC_SC->PCLKSEL0 |= (0x01<<6);  //��PCLK 1 ��Ƶ
    }
    else if(SerialNo == CN_UART1)
    {
        LPC_SC->PCONP |= (1<<4);
        LPC_SC->PCLKSEL0 &= ~(0x03<<8);
        LPC_SC->PCLKSEL0 |= (0x01<<8);
    }
    else if(SerialNo == CN_UART2)
    {
        LPC_SC->PCONP |= (1<<24);
        LPC_SC->PCLKSEL1 &= ~(0x03<<16);
        LPC_SC->PCLKSEL1 |= (0x01<<16);
    }
    else
    {
        LPC_SC->PCONP |= (1<<25);
        LPC_SC->PCLKSEL1 &= ~(0x03<<18);
        LPC_SC->PCLKSEL1 |= (0x01<<18);
    }
    __UART_BaudSet(tg_UART_Reg[SerialNo],115200);

    tg_UART_Reg[SerialNo]->IIR;             // ��һ����������жϱ�־
    tg_UART_Reg[SerialNo]->LCR |= 0x83;     // 8 bits, no Parity, 1 Stop bit
    tg_UART_Reg[SerialNo]->LCR  = 0x03;     // DLAB = 0
    tg_UART_Reg[SerialNo]->FCR  = (0x07 | (0x02<<6)); // ʹ��FIFO�������ô���ˮƽ8Bytes
    tg_UART_Reg[SerialNo]->TER  = 0x80;     // ����ʹ��
    tg_UART_Reg[SerialNo]->IER  = 0x01;     // ʹ�ܽ����ж�

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
//    u32  result = 0;
//
//    if(Reg == NULL)
//        return result;
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
//        Reg->THR = send_buf[result];
//    }
//    __UART_TxIntEnable(Reg);
//    return result;
//}

// =============================================================================
// ����: �������ڷ��ͣ���Ŀ���Ǵ����жϣ����жϷ�ʽ�������ݡ�
// ����: Reg,�������Ĵ��ڼĴ���ָ��.
// ����: ���͵ĸ���
// =============================================================================
static u32 __UART_SendStart (tagUartReg *Reg)
{
    u8 trans=0,num,ch[16],port;

    switch((u32)Reg)
    {
    case CN_UART0_BASE:   port = CN_UART0;   break;
    case CN_UART1_BASE:   port = CN_UART1;   break;
    case CN_UART2_BASE:   port = CN_UART2;   break;
    case CN_UART3_BASE:   port = CN_UART3;   break;
    default:return 0;
    }

    __UART_TxIntDisable(Reg);
    if(__UART_TxTranEmpty(Reg))
    {
        trans = UART_PortRead(pUartCB[port],ch,16);// UART��FIFO��СΪ16�ֽ�
        for(num = 0; num < trans; num++)
        {
            Reg->THR = ch[num];
        }
    }
    __UART_TxIntEnable(Reg);
    return trans;
}

//----�����豸���ƺ���---------------------------------------------------------
//����: �����豸�Ŀ��ƺ���,������Ӳ���Ĵ����������
//����: Reg,�������Ĵ��ڼĴ���ָ��.
//      cmd,��������
//      data,������cmd����
//����: ������.
//-----------------------------------------------------------------------------
static ptu32_t __UART_Ctrl(tagUartReg *Reg,u32 cmd, va_list *arg0)
{
    u8 i;
    for(i =0; i < CN_UART_NUM;i++)
    {
        if(tg_UART_Reg[i] == Reg)
            break;
    }
    if(i == CN_UART_NUM)
        return 0;
    switch(cmd)
    {
        case CN_DEV_CTRL_START:
            __UART_RxIntEnable(Reg);
            __UART_TxIntEnable(Reg);
            break;
        case CN_DEV_CTRL_STOP:
            __UART_RxIntDisable(Reg);
            __UART_TxIntDisable(Reg);
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
        {
            Reg->FCR &= ~(0x03<<6);
            switch (data1)
            {
                case 1:
                    Reg->FCR |= 0x00<<6;//1�ֽڴ���
                    break;
                case 4:
                    Reg->FCR |= 0x01<<6;//4�ֽڴ���
                    break;
                case 8:
                    Reg->FCR |= 0x02<<6;//8�ֽڴ���
                    break;
                case 14:
                    Reg->FCR |= 0x03<<6;//14�ֽڴ���
                    break;
                default :break;
            }
            break;
        }
        case CN_UART_COM_SET:
        {
            struct COMParam *COM;
            COM = va_arg(*arg0, void *);
            __UART_ComConfig(Reg,COM);
        }
            break;
        case CN_UART_HALF_DUPLEX_RECV:
            Board_UartHalfDuplexRecv(i);
            break;
        case CN_UART_HALF_DUPLEX_SEND:
            Board_UartHalfDuplexSend(i);
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
// ����: �жϺ�.
// ����: 0.
// =============================================================================
static u32 UART_ISR(ptu32_t IntLine)
{
    struct UartGeneralCB *UCB;
    volatile tagUartReg *Reg;
    u32 recv_trans,num,IIR;
    u8 ch[16],port;

    switch(IntLine)
    {
    case CN_INT_LINE_UART0:   port = CN_UART0;   break;
    case CN_INT_LINE_UART1:   port = CN_UART1;   break;
    case CN_INT_LINE_UART2:   port = CN_UART2;   break;
    case CN_INT_LINE_UART3:   port = CN_UART3;   break;
    default:return 0;
    }

    UCB = pUartCB[port];
    Reg =  tg_UART_Reg[port];
    while(1)
    {
        IIR = Reg->IIR;
        if(IIR & 0x01)  //���ж�
            return 0;
        IIR = (IIR>>1)&0x7;
        switch(IIR)
        {
            case 1:     //����fifo�Ĵ�����
            {
                recv_trans = UART_PortRead(UCB,ch,16);
                if(recv_trans != 0)
                {
                    for(num = 0; num < recv_trans; num++)
                    {
                        Reg->THR = ch[num];
                    }
                }
            }break;
            case 2:     //���մ���ˮƽ��
            case 6:     //���ճ�ʱ��fifo��������һ�����ݣ�����4�ַ�ʱ����������
            {
                for(num = 0; num<16; num++)
                {
                    if(Reg->LSR & 0x01)
                        ch[num] = Reg->RBR;
                    else
                        break;
                }
                UART_PortWrite(UCB,ch,num);
            }break;
            case 3:     //�����־
            {
                UART_ErrHandle(UCB,CN_UART_HARD_COMM_ERR);
            }break;
            default: return 0;
        }
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
    default:
        return 0;
    }

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

    CR_Bak = GetCharDirectReg->IER;                          //Save INT
    __UART_TxIntDisable(GetCharDirectReg);                   //disable send INT
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
        PutStrDirectReg->THR = str[result];
    }
    PutStrDirectReg->IER = CR_Bak;                         //restore send INT
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

    CR_Bak = GetCharDirectReg->IER;                          //Save INT
    __UART_TxIntDisable(GetCharDirectReg);                   //disable send INT
    while(__UART_RxHadChar(GetCharDirectReg) == false) ;
    result = GetCharDirectReg->RBR;
    PutStrDirectReg->IER = CR_Bak;                         //restore send INT
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
