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
//          �޸���stm32H7��ɾ����DMA��ز���
// ģ��汾: V1.10
// ������Ա:
// ����ʱ��:
// =============================================================================
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <systime.h>
#include <device/djy_uart.h>
#include "int.h"
#include "djyos.h"
#include "cpu_peri_uart.h"
#include "cpu_peri_int_line.h"
#include "stdlib.h"
#include "cpu_peri_io.h"
#include "dbug.h"
//#include "cpu_peri_dma.h"
// =============================================================================
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern ptu32_t ModuleInstall_UART(ptu32_t SerialNo);
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

//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip uart"//CPU��uart��������
//parent:"uart device file"      //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                    //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                 //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                   //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                  //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                   //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"uart device file"    //������������������������none����ʾ�������������
                                   //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                   //����������������������г�
//weakdependence:"none"            //��������������������������none����ʾ�������������
                                   //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                   //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                   //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_UART == false )
//#warning  " cpu_onchip_uart  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header    //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_UART    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,512,
#define CFG_UART0_SENDBUF_LEN       64      //"UART0���ͻ��λ�������С",
#define CFG_UART0_RECVBUF_LEN       64      //"UART0���ջ��λ�������С",
#define CFG_UART1_SENDBUF_LEN       64      //"UART1���ͻ��λ�������С",
#define CFG_UART1_RECVBUF_LEN       64      //"UART1���ջ��λ�������С",
#define CFG_UART2_SENDBUF_LEN       64      //"UART2���ͻ��λ�������С",
#define CFG_UART2_RECVBUF_LEN       64      //"UART2���ջ��λ�������С",
//%$#@enum,true,false
#define CFG_UART0_ENABLE           false       //"�Ƿ�ʹ��UART0",
#define CFG_UART1_ENABLE           false        //"�Ƿ�ʹ��UART1",
#define CFG_UART2_ENABLE           false       //"�Ƿ�ʹ��UART2",
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������
//@#$%component end configure
// =============================================================================

#define CN_UART0_BASE UART0_ADDR
#define CN_UART1_BASE UART1_ADDR
#define CN_UART2_BASE UART2_ADDR

static u32 TxByteTime;                    //��������һ���ֽ�����Ҫ��ʱ��
static struct tagUartReg *PutStrDirectReg;     //����printk���͵Ĵ��ڼĴ���
static struct tagUartReg *GetCharDirectReg;     //����ֱ�ӽ��յĴ��ڼĴ���
static u32 TxDirectPort;                  //����printk���͵Ĵ��ں�
static u32 RxDirectPort;                  //����ֱ�ӽ��յĴ��ں�
// =============================================================================
struct tagUartReg volatile * const tg_UART_Reg[] = {(struct tagUartReg *)CN_UART0_BASE,
                                                    (struct tagUartReg *)CN_UART1_BASE,
                                                    (struct tagUartReg *)CN_UART2_BASE,
                                                    };

static struct UartGeneralCB *pUartCB[CN_UART_NUM];

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

__attribute__((weak))  void Board_UartHalfDuplexSend(u8 SerialNo)
{
    return;
}
__attribute__((weak))  void Board_UartHalfDuplexRecv(u8 SerialNo)
{
    return ;
}
__attribute__((weak))  void UART_InLowPowerPinCfg(u8 SerialNo)
{
    return;
}
__attribute__((weak))  void UART_OutLowPowerPinCfg(u8 SerialNo)
{
    return ;
}
#pragma GCC diagnostic pop

// =============================================================================
static ptu32_t UART_ISR(ptu32_t port);

// =============================================================================
// ���ܣ� ������˳��͹���ģʽ
//     ʱ��ʹ�� ������ʹ�� GPIO������Ӧ��״̬
// ������ port ���ںţ��� CN_UART1
//     flag ����͹��� ���˳��͹��ı�־���� UART_IN_LOWPOWER
// ���أ�true/false
// =============================================================================
bool_t UART_LowPowerConfig(u8 port,u8 flag)
{
   return false;
}

// =============================================================================
// ����: ��ֹuart�Ľ����жϡ�
// ����: flag,�Ƿ�ʹ��dma����
//       port,���ںţ�1~8
// ����: ��
// =============================================================================
void __UART_RxIntEnable(u8 port)
{
    tg_UART_Reg[port]->UART_DLH_IER |= (1<<UART_REG_IER_ERBFI_BIT);//�ж�ʹ��
}

// =============================================================================
// ����: ��ֹuart�Ľ����ж�
// ����: port,���ں�
// ����: ��
// =============================================================================
void __UART_RxIntDisable(u8 port)
{
    tg_UART_Reg[port]->UART_DLH_IER &= ~(1<<UART_REG_IER_ERBFI_BIT);
}

// =============================================================================
// ����: ʹ��uart�ķ����ж�
// ����: port,���ں�
// ����: ��
// =============================================================================
static void __UART_TxIntEnable(u8 port)
{
   tg_UART_Reg[port]->UART_DLH_IER |= 1<<UART_REG_IER_ETBEI_BIT;
}

// =============================================================================
// ����: ��ֹUART�ķ����ж�
// ����: port,���ں�
// ����: ��
// =============================================================================
static void __UART_TxIntDisable(u8 port)
{
    tg_UART_Reg[port]->UART_DLH_IER &= ~(1<<UART_REG_IER_ETBEI_BIT);//��ֹ���Ϳ��ж�
}

// =============================================================================
// ����: ��鷢�ͼĴ�����״̬��
// ������reg,�������ļĴ�����ָ��
// ����: true = �������գ�false = �ǿ�
// =============================================================================
static bool_t __UART_TxTranEmpty(struct tagUartReg *reg)
{
    return ((reg->UART_USR & (1<<UART_REG_USR_TFE_BIT)) !=0);
}

bool_t UART_WaitTxEnd(u8 port)
{
    s32 timeout = 10000;
    struct tagUartReg *Reg;
    Reg = (struct tagUartReg *)tg_UART_Reg[port];
    while((false == __UART_TxTranEmpty(Reg))&& (timeout > 10))
    {
        timeout -=10;
        DJY_DelayUs(10);
    }
    return true;
}

// =============================================================================
// ����: �����ռĴ�����״̬�������ݾͷ���true�����򷵻�false
// ������reg,�������ļĴ�����ָ��
// ����: true = �����ݣ�false = ������
// =============================================================================
static bool_t __UART_RxHadChar(struct tagUartReg *Reg)
{
    return ((Reg->UART_USR & (1<<UART_REG_USR_RFNE_BIT)) !=0);
}

void uart_fifo_reset(struct tagUartReg *Reg)
{
    u32 value;

    value = Reg->UART_IIR_FCR;
    SET_REG_BIT(value, UART_REG_FCR_XFIFOR_BIT);
    SET_REG_BIT(value, UART_REG_FCR_RFIFOR_BIT);
    Reg->UART_IIR_FCR = value;

    for (value = 0; value < 0xff; value++);
}

// =============================================================================
// ����: ���ô���baud,PCLKΪ25M��CPU��ƵΪ216M����,������ֻ�ṩ9600��19200��57600��
//       115200���ֲ���������
// ����: Reg,�������ļĴ�����ָ��
//       port,���ں�
//       baud,������
// ����: ��
// =============================================================================
static void __UART_BaudSet(struct tagUartReg *Reg,u32 port,u32 baud)
{
    u32 nApbClk;
    u32 divisor;
    u32 time,value;

    nApbClk = CN_CFG_MCLK/4;
    divisor = (nApbClk + (baud << 3)) / (baud << 4);    //�ο�melis2.0

    // clear fifo
    uart_fifo_reset(Reg);

    // check uart busy
    time = 0xffffff;
    do
    {
        value = Reg->UART_USR;
    } while (time-- && TEST_REG_BIT(value, UART_REG_USR_BUSY_BIT));
    if (0 == time)
    {
        error_printf("uart","uart%d set baudrate check busy timeout\n", port);
        return ;
    }

    Reg->UART_LCR |= 1<<UART_REG_LCR_DLAB_BIT;
    Reg->UART_RBR_THR_DLL = divisor & 0xff;
    Reg->UART_DLH_IER = (divisor>>8) & 0xff;
    Reg->UART_LCR &= ~(1<<UART_REG_LCR_DLAB_BIT);

    // clear fifo
    uart_fifo_reset(Reg);

    if(TxDirectPort == port)
    {
        TxByteTime = 11000000/baud;     //1���ֽڴ���ʱ�䣬��10bit��+10%����
    }
}

// =============================================================================
// ����: ���ö�ӦUART��IO�ڣ�����ʱ�Ӻ�IO����
// ����: SerialNo,���ں�
// ע :�򴮿ڸ������Ų�ȷ��������Ҫ��ʼ����Ҫ������
// ����: ��
// =============================================================================
static void __UART_ClkConfig(u8 SerialNo)
{

    switch(SerialNo)
    {
    case CN_UART0:
        break;
    case CN_UART1:
        break;
    case CN_UART2:
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
static void __UART_ComConfig(struct tagUartReg *Reg,u32 port,struct COMParam *COM)
{
    if((COM == NULL) || (Reg == NULL))
        return;
    __UART_BaudSet(Reg,port,COM->BaudRate);

    switch(COM->DataBits)               // data bits
    {
        case CN_UART_DATABITS_5:
            Reg->UART_LCR &= ~(3<<UART_REG_LCR_DLS_BIT);
            Reg->UART_LCR |= 0<<UART_REG_LCR_DLS_BIT;
            break;
        case CN_UART_DATABITS_6:
            Reg->UART_LCR &= ~(3<<UART_REG_LCR_DLS_BIT);
            Reg->UART_LCR |= 1<<UART_REG_LCR_DLS_BIT;
            break;
        case CN_UART_DATABITS_7:
            Reg->UART_LCR &= ~(3<<UART_REG_LCR_DLS_BIT);
            Reg->UART_LCR |= 2<<UART_REG_LCR_DLS_BIT;
            break;
        case CN_UART_DATABITS_8:
        case CN_UART_DATABITS_9:    //��֧��9λ����ͬ��8λ����
            Reg->UART_LCR &= ~(3<<UART_REG_LCR_DLS_BIT);
            Reg->UART_LCR |= 3<<UART_REG_LCR_DLS_BIT;
            break;
            default:break;
    }
    switch(COM->Parity)
    {
        case CN_UART_PARITY_NONE:
            Reg->UART_LCR &= ~(1<<UART_REG_LCR_PEN_BIT);
            break;

        case CN_UART_PARITY_ODD:
            Reg->UART_LCR &= ~(3<<UART_REG_LCR_EPS_BIT);
            Reg->UART_LCR |= (0<<UART_REG_LCR_EPS_BIT);
            Reg->UART_LCR |= (1<<UART_REG_LCR_PEN_BIT);//ʹ����żУ��
            break;
        case CN_UART_PARITY_EVEN:
            Reg->UART_LCR &= ~(3<<UART_REG_LCR_EPS_BIT);
            Reg->UART_LCR |= (1<<UART_REG_LCR_EPS_BIT);
            Reg->UART_LCR |= (1<<UART_REG_LCR_PEN_BIT);//ʹ����żУ��
            break;
        default:break;
    }
    switch(COM->StopBits)
    {
        case CN_UART_STOPBITS_1:
            Reg->UART_LCR &= ~(1<<UART_REG_LCR_STOP_BIT);
            break;
        case CN_UART_STOPBITS_1_5:
        case CN_UART_STOPBITS_2:       //��֧��2λ����ͬ��1.5λ����
            Reg->UART_LCR |= (1<<UART_REG_LCR_STOP_BIT);
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
    switch(port)
    {
    case CN_UART0:
        break;
    case CN_UART1:
        break;
    case CN_UART2:
        break;
    default: break;
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
        break;
    case CN_UART1:
        break;
    case CN_UART2:
        break;
    default: break;
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
    switch (SerialNo)
    {
        case CN_UART0:
                    IntLine = CN_INT_LINE_UART0;break;
        case CN_UART1:
                    IntLine = CN_INT_LINE_UART1;break;
        case CN_UART2:
                    IntLine = CN_INT_LINE_UART2;break;
                default:
                    return;
    }
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
//       ������:115200  �� ֹͣλ:1 ; У��:�� ; ����λ:8bit
// ����: SerialNo,���ںţ�0~3
// ����: ��
// =============================================================================
static void __UART_HardInit(u8 SerialNo)
{
    u8 LW;
    u32 addr1,val;

    if(SerialNo >= CN_UART_NUM)
        return;
    switch(SerialNo)
    {
        case CN_UART0:
            LW=20;
            break;
        case CN_UART1:
            LW=21;
            break;
        case CN_UART2:
            LW=22;
            break;
    }

    //Open the clock gate for uart ʱ�ӿ�//
    addr1 = 0x01c20068;
    val = read32(addr1);
    val |= 1 << LW;
    write32(addr1, val);
    // Deassert uart reset ��λ//
    addr1 = 0x01c202d0;
    val = read32(addr1);
    val &=~( 1 << LW);
    write32(addr1, val);
    DJY_DelayUs(10000);
    // Deassert uart reset ��λ//
    addr1 = 0x01c202d0;
    val = read32(addr1);
    val |= 1 << LW;
    write32(addr1, val);

    __UART_ClkConfig(SerialNo);
    tg_UART_Reg[SerialNo]->UART_IIR_FCR = 0xb7; //ʹ��fifo

    //ϵͳ��ʼ��ʱ�Ѿ�ʹ�жϴ��ڽ�ֹ״̬�������ٽ�ֹ������жϡ�
   //��ʼ��uartӲ���������ݽṹ
    __UART_BaudSet(tg_UART_Reg[SerialNo],SerialNo,115200);

    val = tg_UART_Reg[SerialNo]->UART_LCR;
    val &= ~0x1f;
    val |= (0x3 << 0) | (0 << 2) | (0x0 << 3);
    tg_UART_Reg[SerialNo]->UART_LCR = val;

    tg_UART_Reg[SerialNo]->UART_DLH_IER |= 0x8f;    //ʹ�������ж�
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
// =============================================================================
// ����: �������ڷ��ͣ���Ŀ���Ǵ����жϣ����жϷ�ʽ�������ݡ�
// ����: Reg,�������Ĵ��ڼĴ���ָ��.
// ����: ���͵ĸ���
// =============================================================================
static u32 __UART_SendStart (ptu32_t MyReg)
{
    struct tagUartReg *Reg = (struct tagUartReg *)MyReg;
    u8 port;

    switch(MyReg)
    {
        case CN_UART0_BASE:        port = CN_UART0;    break;
        case CN_UART1_BASE:        port = CN_UART1;    break;
        case CN_UART2_BASE:        port = CN_UART2;    break;
        default:return 0;
    }

    Board_UartHalfDuplexSend(port);//�л�������
    Reg->UART_DLH_IER |= 1<<UART_REG_IER_ETBEI_BIT;    //ʹ�ܷ��Ϳ��ж�

    return 1;
}
#pragma GCC diagnostic pop

// =============================================================================
// ����: �����豸�Ŀ��ƺ���,������Ӳ���Ĵ����������
// ����: Reg,�������Ĵ��ڼĴ���ָ��.
//       cmd,��������
//       data1,data2,������cmd����
// ����: ������.
// =============================================================================
static ptu32_t __UART_Ctrl(ptu32_t MyReg,u32 cmd, va_list *arg0)
{
    struct tagUartReg *Reg = (struct tagUartReg *)MyReg;
    ptu32_t result = 0;
    u32 port;

    if(Reg == NULL)
        return 0;

    switch((u32)Reg)
    {
        case CN_UART0_BASE: port = CN_UART0;break;
        case CN_UART1_BASE: port = CN_UART1;break;
        case CN_UART2_BASE: port = CN_UART2;break;
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
            u32 baud = va_arg(*arg0, u32);
             __UART_BaudSet(Reg,port, baud);
            break;
        }
        case CN_UART_EN_RTS:
            Reg->UART_MCR |= 1<<UART_REG_MCR_RTS_BIT;
            break;
        case CN_UART_DIS_RTS:
            Reg->UART_MCR &= ~(1<<UART_REG_MCR_RTS_BIT);
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
            struct COMParam *COM = va_arg(*arg0, struct COMParam *);
            __UART_ComConfig(Reg,port,COM);
            break;
        }
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
    struct tagUartReg *Reg;
    u32 num,loop;
    u8 IIR;
    u8 fifobuf[64];

    UCB = pUartCB[port];
    Reg = (struct tagUartReg *)tg_UART_Reg[port];
    IIR = Reg->UART_IIR_FCR & 0x0f;

    switch(IIR)
    {
        case UART_INT_MS  :         // 0x00 modem status
            Reg->UART_MSR;
            break;
        case UART_INT_NIP :         // 0x01 no interrupt pending
            break;
        case UART_INT_THRE:         // 0x02 THR empty
//            Reg->UART_RBR_THR_DLL =0x31;
//            for(num = 0;num<10000000;num++)
//                if(Reg->UART_IIR_FCR & 0x0f == UART_INT_THRE)
//                    break;
            num = 32 - Reg->UART_TFL;    //ȡfifo��λ
            num = UART_PortRead(UCB,fifobuf,num);
            if(num == 0)
            {
                Reg->UART_DLH_IER &= ~(1<<UART_REG_IER_ETBEI_BIT);    //��ֹ���Ϳ��ж�
            }
            else
            {
                for(loop = 0; loop < num; loop++)
                {
                    Reg->UART_RBR_THR_DLL = fifobuf[loop];
                }
            }
            break;
        case UART_INT_RDA :         // 0x04 received data available
        case UART_INI_CT  :         // 0x0c character timeout
            num = Reg->UART_RFL;    //ȡfifo��������
            for(loop = 0; loop < num; loop++)
            {
                fifobuf[loop] = Reg->UART_RBR_THR_DLL;
            }
            if(num != 0)
            {
                num = UART_PortWrite(UCB,fifobuf,num);
                if(num != 1)
                {
                    UART_ErrHandle(UCB,CN_UART_BUF_OVER_ERR);
                }
            }

            break;
        case UART_INT_RLS :         // 0x06 receiver line status
            Reg->UART_LSR;          //clear int
            break;
        case UART_INT_BD  :         // 0x07 busy detect
            Reg->UART_USR;          //clear int
            break;
        default:break;
    }

//ע��F1Cϵ��CPU����û�з�������ж�
//  if((Reg->ISR & USART_ISR_TC)&&(Reg->CR1 & USART_CR1_TCIE))//TC
//  {
//      Reg->CR1 &=~USART_CR1_TCIE;    //��TC�ж�
//      Board_UartHalfDuplexRecv(port);     //������DMA�����жϴ��䣬�����������ݺ�������������״̬
//  }
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
    default:
        return 0;
    }
    UART_Param.mode = CN_UART_GENERAL;
    Board_UartHalfDuplexRecv(serial_no);
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
    s32 timeout = TxByteTime * len;
    u16 CR_Bak;
    u8 port;
    for(port=0;port<CN_UART_NUM;port++)
    {
        if(PutStrDirectReg ==tg_UART_Reg[port])
            break;
    }
    Board_UartHalfDuplexSend(port);
    CR_Bak = PutStrDirectReg->UART_DLH_IER;                          //Save INT
    PutStrDirectReg->UART_DLH_IER &= ~(1<<UART_REG_IER_ETBEI_BIT);//��ֹ���Ϳ��ж�
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
        PutStrDirectReg->UART_RBR_THR_DLL = str[result];
    }
    while((PutStrDirectReg->UART_LSR &(1<<UART_REG_LSR_TEMT_BIT))!=0)
    {
        timeout -=10;
        DJY_DelayUs(10);
        if(timeout < 10)
           break;
    }
    PutStrDirectReg->UART_DLH_IER = CR_Bak;                         //restore send INT
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

    CR_Bak = PutStrDirectReg->UART_DLH_IER;                          //Save INT
    PutStrDirectReg->UART_DLH_IER &= ~(1<<UART_REG_IER_ERBFI_BIT);//��ֹ�����ж�
    while(__UART_RxHadChar(GetCharDirectReg) == false)
        DJY_EventDelay(1000);

    result = GetCharDirectReg->UART_RBR_THR_DLL;
    PutStrDirectReg->UART_DLH_IER = CR_Bak;                         //restore send INT
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
        PutStrDirectReg = (struct tagUartReg*)CN_UART0_BASE;
        TxDirectPort = CN_UART0;
    }
    else if(!strcmp(StdioOut,"/dev/UART1"))
    {
        PutStrDirectReg = (struct tagUartReg*)CN_UART1_BASE;
        TxDirectPort = CN_UART1;
    }
    else if(!strcmp(StdioOut,"/dev/UART2"))
    {
        PutStrDirectReg = (struct tagUartReg*)CN_UART2_BASE;
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
        GetCharDirectReg = (struct tagUartReg*)CN_UART0_BASE;
        RxDirectPort = CN_UART0;
    }
    else if(!strcmp(StdioIn,"/dev/UART1"))
    {
        GetCharDirectReg = (struct tagUartReg*)CN_UART1_BASE;
        RxDirectPort = CN_UART1;
    }
    else if(!strcmp(StdioIn,"/dev/UART2"))
    {
        GetCharDirectReg = (struct tagUartReg*)CN_UART2_BASE;
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

