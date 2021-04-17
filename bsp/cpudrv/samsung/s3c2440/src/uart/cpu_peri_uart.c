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
//����ģ��: uart��������
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: ����2440��uart
//����˵��:
//�޶���ʷ:
//1. ����:2009-03-10
//   ����:lst
//   �°汾�ţ�1.0.0
//   �޸�˵��: ��ֲ��44b0��1.0.1��
//------------------------------------------------------

#include "stdint.h"
#include "string.h"
#include "errno.h"
#include "systime.h"
#include "cpu_peri.h"
#include "int.h"
#include "djyos.h"
#include <device/djy_uart.h>
#include "string.h"
#include "cpu_peri_uart.h"
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
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip uart"//CPU��uart��������
//parent:"uart device file"      //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                       //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                    //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                      //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium                    //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                      //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"device file system","lock","uart device file"//������������������������none����ʾ�������������
                                      //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                      //����������������������г����á�,���ָ�
//weakdependence:"none"               //��������������������������none����ʾ�������������
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
#define CFG_UART0_SENDBUF_LEN            256               //"UART0���ͻ��λ�������С",
#define CFG_UART0_RECVBUF_LEN            256               //"UART0���ջ��λ�������С",

#define CFG_UART1_SENDBUF_LEN            256               //"UART1���ͻ��λ�������С",
#define CFG_UART1_RECVBUF_LEN            256               //"UART1���ջ��λ�������С",

#define CFG_UART2_SENDBUF_LEN            256               //"UART2���ͻ��λ�������С",
#define CFG_UART2_RECVBUF_LEN            256               //"UART2���ջ��λ�������С",
//%$#@enum,true,false,
#define CFG_UART0_ENABLE                 true              //"�����Ƿ�ʹ��UART0",
#define CFG_UART1_ENABLE                 false             //"�����Ƿ�ʹ��UART1",
#define CFG_UART2_ENABLE                 false             //"�����Ƿ�ʹ��UART2",
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

// =============================================================================
#define CN_UART0_BASE   (0x50000000)
#define CN_UART1_BASE   (0x50004000)
#define CN_UART2_BASE   (0x50008000)
// =============================================================================
//static tagUartReg volatile * const tg_UART_Reg[] = {(tagUartReg *)CN_UART0_BASE,
//                                                  (tagUartReg *)CN_UART1_BASE,
//                                                  (tagUartReg *)CN_UART2_BASE};
//#define UART0_SendBufLen            256
//#define UART0_RecvBufLen            256

//#define UART1_SendBufLen            256
//#define UART1_RecvBufLen            256

//#define UART2_SendBufLen            256
//#define UART2_RecvBufLen            256

static struct UartGeneralCB *pUartCB[CN_UART_NUM];

//���ڱ�ʶ�����Ƿ��ʼ����ǣ���0λ��ʾUART0����һλ��UART1....
//�������ƣ�1��ʾ��ʼ����0��ʾδ��ʼ��
static u8 sUartInited = 0;

u32 UART0_Int(ptu32_t IntLine);
u32 UART1_Int(ptu32_t IntLine);
u32 UART2_Int(ptu32_t IntLine);

//----����uart���ж�Դ---------------------------------------------------------
//����: 2410�У�uart�жϿ��ܱ��ա�����err����Դ������������ѡ���Ե���������һ��
//      �������ж�Դ��������rsc_msk�б���1��λȷ����
//����: port����ʹ�ܵĴ��ں�
//      flag, ��ǣ���Ϊ���ա����͡�����
//����: ��
//-----------------------------------------------------------------------------
void __UART_SubIntEnable(u32 port, u32 flag)
{
    pg_int_reg->INTSUBMSK &= ~(BIT_SUB_INT(port,flag) & BIT_ALL_UART);
}

//----��ֹuart���ж�Դ---------------------------------------------------------
//����: 2410�У�uart�жϿ��ܱ��ա�����err����Դ������������ѡ���Եؽ�ֹ����һ��
//      �������ж�Դ��������rsc_msk�б���1��λȷ����
//����: port���ر��жϵĴ��ں�
//      flag, ��ǣ���Ϊ���ա����͡�����
//����: ��
//-----------------------------------------------------------------------------
void __UART_SubIntDisable(u32 port, u32 flag)
{
    pg_int_reg->INTSUBMSK |= (BIT_SUB_INT(port,flag) & BIT_ALL_UART);
}

//----��鴮�ڷ��ͻ������Ƿ���-------------------------------------------------
//����: ��鷢��fifo��������״̬�����fifo���ͷ���true
//�������ޡ�
//����: true = ����������false = ����
//-----------------------------------------------------------------------------
bool_t __UART_TxFifoFull(tagUartReg *Reg)
{
    return (Reg->UFSTAT &0x4000);
}

//----��鴮�ڷ���fifo�Ƿ��---------------------------------------------------
//����: ��鷢��fifo��������״̬�����fifo�վͷ���true
//�������ޡ�
//����: true = �������գ�false = �ǿ�
//-----------------------------------------------------------------------------
u32 __UART_GetTxFifoNum(tagUartReg *Reg)
{
    return (Reg->UFSTAT &0x3f00)>> 8;
}

//----��鴮�ڷ��ͼĴ����Ƿ��-------------------------------------------------
//����: ��鷢�ͼĴ�����״̬������վͷ���true
//�������ޡ�
//����: true = �������գ�false = �ǿ�
//-----------------------------------------------------------------------------
bool_t __UART_TxTranEmpty(volatile tagUartReg *Reg)
{
    return (0!=(Reg->UTRSTAT & 0x04));
}

//----����baud-----------------------------------------------------------------
//����: ���ô���baud
//����: reg,�������ļĴ�����ָ��
//����: ��
//-----------------------------------------------------------------------------
void __UART_BaudSet(volatile tagUartReg *Reg,u32 baud)
{
    Reg->UBRDIV = ((CN_CFG_PCLK<<2)/baud -32)>>6;
    Reg->UERSTAT;     //��һ���������״̬
}

//----����Ӳ������-------------------------------------------------------------
//����: ����Ӳ�����������������ʡ�ֹͣλ��У��λ������λ,������Ĭ��115200
//����: uart_reg,�������ļĴ�����ָ��
//����: ��
//-----------------------------------------------------------------------------
void __UART_DefaultConfig(volatile tagUartReg *Reg)
{
    //ϵͳ��ʼ��ʱ�Ѿ�ʹ�жϴ��ڽ�ֹ״̬�������ٽ�ֹ������жϡ�
    //��ʼ��uartӲ���������ݽṹ
    Reg->ULCON = 0x3;
    Reg->UCON =  0xc5;
    Reg->UFCON = 0x17;   //bit0=1ʹ��fifo.
    Reg->UMCON = 0x0;
    Reg->UBRDIV = ((CN_CFG_PCLK<<2)/115200 -32)>>6;
    Reg->UERSTAT;     //��һ���������״̬

}

//----���ô��ڵ�io��-----------------------------------------------------------
//����: ���ô�����ʹ�õ�io�ڣ��������������
//����: port,���ںţ���Ϊ1��2��3
//����: ��
//-----------------------------------------------------------------------------
void __UART_GPIOConfig(u32 port)
{
    u32 temp;
    //Ports  :  GPH10 GPH9 GPH8 GPH7 GPH6 GPH5 GPH4 GPH3 GPH2 GPH1  GPH0
    //Signal :   xx    xx   xx   xx   xx   xx   xx  RXD0 TXD0 nRTS0 nCTS0
    //Binary :   xx    xx   xx   xx   xx   xx   xx   xx   xx   xx   xx
    switch(port)
    {
    case CN_UART0:
        temp = pg_gpio_reg->GPHCON;
        temp &= ~0xff;
        temp |= 0xaa;
        pg_gpio_reg->GPHCON = temp;
        // The pull up function is disabled GPH[3:0]
        pg_gpio_reg->GPHUP |= 0xf;
        break;
    case CN_UART1:
        temp = pg_gpio_reg->GPHCON;
        temp &= ~0xf00;
        temp |= 0xa00;
        pg_gpio_reg->GPHCON = temp;
        // The pull up function is disabled GPH[5:4]
        pg_gpio_reg->GPHUP |= 0x30;
        break;
    case CN_UART2:
        temp = pg_gpio_reg->GPHCON;
        temp &= ~0xf000;
        temp |= 0xa000;
        pg_gpio_reg->GPHCON = temp;
        // The pull up function is disabled GPH[7:6]
        pg_gpio_reg->GPHUP |= 0xc0;
        break;
    default:
        break;
    }
}

//----���ô����ж�-----------------------------------------------------------
//����: ���ô��ڵ��жϣ������жϷ������������ô����ж�Ϊ�첽�ź�
//����: port,���ںţ���Ϊ1��2��3
//����: ��
//-----------------------------------------------------------------------------
void __UART_IntConfig(u32 port)
{
    //�ж��ߵĳ�ʼ��
    //��ʼ��ͨ�ô������ݽṹ
    switch(port)
    {
    case CN_UART0:
     Int_Register(cn_int_line_uart0);
        Int_IsrConnect(cn_int_line_uart0,UART0_Int);
        Int_SettoAsynSignal(cn_int_line_uart0);
        pg_int_reg->SUBSRCPND = BIT_SUB_TXD0+BIT_SUB_RXD0+BIT_SUB_ERR0;
        Int_ClearLine(cn_int_line_uart0);       //�����ʼ�������ķ���fifo�յ��ж�
        Int_RestoreAsynLine(cn_int_line_uart0);
        break;
    case CN_UART1:
     Int_Register(cn_int_line_uart1);
        Int_IsrConnect(cn_int_line_uart1,UART1_Int);
        Int_SettoAsynSignal(cn_int_line_uart1);
        pg_int_reg->SUBSRCPND = BIT_SUB_TXD1+BIT_SUB_RXD1+BIT_SUB_ERR1;
        Int_ClearLine(cn_int_line_uart1);       //�����ʼ�������ķ���fifo�յ��ж�
        Int_RestoreAsynLine(cn_int_line_uart1);
        break;
    case CN_UART2:
     Int_Register(cn_int_line_uart2);
        Int_IsrConnect(cn_int_line_uart2,UART2_Int);
        Int_SettoAsynSignal(cn_int_line_uart2);
        pg_int_reg->SUBSRCPND = BIT_SUB_TXD2+BIT_SUB_RXD2+BIT_SUB_ERR2;
        Int_ClearLine(cn_int_line_uart2);       //�����ʼ�������ķ���fifo�յ��ж�
        Int_RestoreAsynLine(cn_int_line_uart2);
        break;
    default:
        break;
    }
}

//----���з��ͷ���ֱ��д�նˣ�----------------------------------------------
//����: �����ֱ��д���ں��������ᾭ���¼�����
//����: uart_dev,�������Ĵ����豸ָ��.
//      send_buf,�����͵Ļ�������
//      len,���͵������ֽ���
//      timeout,��ʱ
//����: ���͵ĸ���
//-----------------------------------------------------------------------------
//bool_t __UART_SendDirectly(tagUartReg *Reg,u8 *send_buf,u32 len,u32 timeout)
//{
//    volatile u32  result,port;
//
//    if((u32)Reg == CN_UART0_BASE)
//        port = CN_UART0;
//    else if((u32)Reg == CN_UART1_BASE)
//        port = CN_UART1;
//    else if((u32)Reg == CN_UART2_BASE)
//        port = CN_UART2;
//    else
//        return false;
//
//    __UART_SubIntDisable(port,SUB_TXD_FLG);
//    for(result=0; result < len; result ++)
//    {
//        while((false == __UART_TxTranEmpty(Reg))&& (timeout > 0))//��ʱ���߷��ͻ���Ϊ��ʱ�˳�
//        {
//            timeout--;
//            DJY_DelayUs(1);
//        }
//        if(timeout == 0)
//            break;
//        Reg->UTXH = send_buf[result];
//    }
//    __UART_SubIntEnable(port,SUB_TXD_FLG);
//
//    return result;
//}
//----�������ڷ��ͺ���---------------------------------------------------------
//����: �������ڷ��ͣ�ֱ�ӷ���fifo��С�����ݣ����������Ϳ��жϣ����ж��н�
//      send_ring_buf�е���������������ݵķ��ͣ��൱��������һ�η��͹���
//����: uart_dev,�������Ĵ����豸ָ��.
//����: ���͵ĸ���
//-----------------------------------------------------------------------------
u32 __UART_SendStart(tagUartReg *Reg)
{
    u8 trans,num,ch[UART_TX_FIFO_SIZE],sending_num,port;
    struct Object *UCB;

    switch((u32)Reg)
    {
    case CN_UART0_BASE:     port = CN_UART0; break;
    case CN_UART1_BASE:     port = CN_UART1;break;
    case CN_UART2_BASE:     port = CN_UART2;break;
    default:return 0;
    }

    __UART_SubIntDisable(port,SUB_TXD_FLG);
    sending_num = __UART_GetTxFifoNum(Reg);
    if(sending_num <= UART_TX_FIFO_SIZE)//fifo bytes
    {
        trans = UART_PortRead(pUartCB[port],ch,UART_TX_FIFO_SIZE-sending_num);
        for(num = 0; num < trans; num++)
        {
            Reg->UTXH = ch[num];
        }
    }
    __UART_SubIntEnable(port,SUB_TXD_FLG);
    return trans;
}

//----�����豸���ƺ���---------------------------------------------------------
//����: �����豸�Ŀ��ƺ���,������Ӳ���Ĵ����������
//����: uart_dev,�������Ĵ����豸ָ��.
//      cmd,��������
//      data,������cmd����
//����: ������.
//-----------------------------------------------------------------------------
ptu32_t __UART_Ctrl(tagUartReg *Reg,u32 cmd, va_list *arg0)
{
    u32  port;

    if((u32)Reg == CN_UART0_BASE)
        port = CN_UART0;
    else if((u32)Reg == CN_UART1_BASE)
        port = CN_UART1;
    else if((u32)Reg == CN_UART2_BASE)
        port = CN_UART2;
    else
        return false;

    switch(cmd)
    {
        case CN_DEV_CTRL_START:
            __UART_SubIntEnable(port,SUB_RXD_FLG);
            __UART_SubIntEnable(port,SUB_TXD_FLG);
            break;
        case CN_DEV_CTRL_STOP:
            __UART_SubIntDisable(port,SUB_RXD_FLG);
            __UART_SubIntDisable(port,SUB_TXD_FLG);
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
            __UART_SubIntDisable(port,SUB_RXD_FLG);
            break;
        case CN_UART_RX_RESUME:      //�ָ�����
            __UART_SubIntEnable(port,SUB_RXD_FLG);
            break;
        case CN_UART_SEND_HARD_LEVEL:    //���÷���fifo����ˮƽ
        {
            u32 data;
            data = va_arg(*arg0, u32);
            Reg->UFCON &= 0x3f;
            switch (data)
            {
                case 0:
                    break;
                case 4:
                    Reg->UFCON |= 0x40;
                    break;
                case 8:
                    Reg->UFCON |= 0x80;
                    break;
                case 12:
                    Reg->UFCON |= 0xc0;
                    break;
                default :break;
            }
            break;
        }
        case CN_UART_RECV_HARD_LEVEL:    //���ý���fifo����ˮƽ
        {
            Reg->UFCON &= 0xcf;
            switch (data1)
            {
                case 0:
                    break;
                case 4:
                    Reg->UFCON |= 0x10;
                    break;
                case 8:
                    Reg->UFCON |= 0x20;
                    break;
                case 12:
                    Reg->UFCON |= 0x30;
                    break;
                default :break;
            }
            break;
        }
        default: break;
    }
    return 0;
}

//----uart0�ж�---------------------------------------------------------------
//����: ����ǽ����źŴ������ж�:
//      1.������fifo�е�������.�����ݴӽ��ռĴ���ȫ��copy������㻺������.
//      2.�������㻺������.�ô��ڿ��ƿ�״̬�ֵ����λ,������uart�����¼�.
//      3.���ʹ��ڽ����¼�.�����¼�����״̬���¼�
//      ����Ƿ����ź��������ж�:
//      1.��鷢��fifo�ж��ٿ�λ
//      2.���豸���ֽӿڶ����������ݣ�
//      3.�Ѷ���������д�뷢��fifo
//      ����ǳ����ź������ж�:�������ڳ����¼�
//����: �жϺ���û�в���.
//����: �жϺ���û�з���ֵ.
//-----------------------------------------------------------------------------
u32 UART0_Int(ptu32_t IntLine)
{
    u32 recv_trans,num;
    u32 fifo_reg;
    u8 ch[UART_TX_FIFO_SIZE];
    struct UartGeneralCB *UCB;
    tagUartReg *Reg;

    UCB = pUartCB[CN_UART0];
    if(UCB == NULL)
        return 0;
    Reg = (tagUartReg *)CN_UART0_BASE;

    if(pg_int_reg->SUBSRCPND & BIT_SUB_RXD0)
    {
        pg_int_reg->SUBSRCPND = BIT_SUB_RXD0;
        fifo_reg = Reg->UFSTAT;
        recv_trans = fifo_reg & 0x3f;
        for(num = 0; num < recv_trans; num++)//fifo������������,ֱ���������
            ch[num] = Reg->URXH;
        UART_PortWrite(UCB,ch,recv_trans);
        if( fifo_reg & cn_rx_over)
        {
            Reg->UFCON |= cn_rx_fifo_reset;
            UART_ErrHandle(UCB,CN_UART_FIFO_OVER_ERR);
        }
    }
    if(pg_int_reg->SUBSRCPND & BIT_SUB_TXD0)
    {
        pg_int_reg->SUBSRCPND = BIT_SUB_TXD0;
        fifo_reg = Reg->UFSTAT;
        recv_trans = UART_TX_FIFO_SIZE-((fifo_reg>>8) & 0x3f);
        recv_trans = UART_PortRead(UCB,ch,recv_trans);
        for(num = 0; num < recv_trans; num++)
        {
            Reg->UTXH = ch[num];
        }
    }
    if(pg_int_reg->SUBSRCPND & BIT_SUB_ERR0)
    {
        pg_int_reg->SUBSRCPND = BIT_SUB_ERR0;
        UART_ErrHandle(UCB,CN_UART_HARD_COMM_ERR);
    }
    return 0;
}

//----uart1�ж�---------------------------------------------------------------
//����: ����ǽ����źŴ������ж�:
//      1.������fifo�е�������.�����ݴӽ��ռĴ���ȫ��copy������㻺������.
//      2.�������㻺������.�ô��ڿ��ƿ�״̬�ֵ����λ,������uart�����¼�.
//      3.���ʹ��ڽ����¼�.�����¼�����״̬���¼�
//      ����Ƿ����ź��������ж�:
//      1.��鷢��fifo�ж��ٿ�λ
//      2.���豸���ֽӿڶ����������ݣ�
//      3.�Ѷ���������д�뷢��fifo
//      ����ǳ����ź������ж�:�������ڳ����¼�
//����: �жϺ���û�в���.
//����: �жϺ���û�з���ֵ.
//-----------------------------------------------------------------------------
u32 UART1_Int(ptu32_t uart_int_line)
{
   u32 recv_trans,num;
    u32 fifo_reg;
    u8 ch[UART_TX_FIFO_SIZE];
    struct UartGeneralCB *UCB;
    tagUartReg *Reg;

    UCB = pUartCB[CN_UART1];
    if(UCB == NULL)
        return 0;
    Reg = (tagUartReg *)CN_UART1_BASE;

    if(pg_int_reg->SUBSRCPND & BIT_SUB_RXD1)
    {
        pg_int_reg->SUBSRCPND = BIT_SUB_RXD1;
        fifo_reg = Reg->UFSTAT;
        recv_trans = fifo_reg & 0x3f;
        for(num = 0; num < recv_trans; num++)
        //fifo������������,ֱ���������
            ch[num] = Reg->URXH;
        UART_PortWrite(UCB,ch,recv_trans);
        if( fifo_reg & cn_rx_over)
        {
            Reg->UFCON |= cn_rx_fifo_reset;
            UART_ErrHandle(UCB,CN_UART_FIFO_OVER_ERR);
       }
    }
    if(pg_int_reg->SUBSRCPND & BIT_SUB_TXD1)
    {
        pg_int_reg->SUBSRCPND = BIT_SUB_TXD1;
        fifo_reg = Reg->UFSTAT;
        recv_trans = UART_TX_FIFO_SIZE-((fifo_reg>>8) & 0x3f);
        recv_trans = UART_PortRead(UCB,ch,recv_trans);
        for(num = 0; num < recv_trans; num++)
        {
            Reg->UTXH = ch[num];
        }
    }
    if(pg_int_reg->SUBSRCPND & BIT_SUB_ERR1)
    {
        pg_int_reg->SUBSRCPND = BIT_SUB_ERR1;
        UART_ErrHandle(UCB,CN_UART_HARD_COMM_ERR);
    }
    return 0;
}

//----uart2�ж�---------------------------------------------------------------
//����: ����ǽ����źŴ������ж�:
//      1.������fifo�е�������.�����ݴӽ��ռĴ���ȫ��copy������㻺������.
//      2.�������㻺������.�ô��ڿ��ƿ�״̬�ֵ����λ,������uart�����¼�.
//      3.���ʹ��ڽ����¼�.�����¼�����״̬���¼�
//      ����Ƿ����ź��������ж�:
//      1.��鷢��fifo�ж��ٿ�λ
//      2.���豸���ֽӿڶ����������ݣ�
//      3.�Ѷ���������д�뷢��fifo
//      ����ǳ����ź������ж�:�������ڳ����¼�
//����: �жϺ���û�в���.
//����: �жϺ���û�з���ֵ.
//-----------------------------------------------------------------------------
u32 UART2_Int(ptu32_t uart_int_line)
{
    u32 recv_trans,num;
    u32 fifo_reg;
    u8 ch[UART_TX_FIFO_SIZE];
    struct UartGeneralCB *UCB;
    tagUartReg *Reg;

    UCB = pUartCB[CN_UART2];
    if(UCB == NULL)
        return 0;
    Reg = (tagUartReg *)CN_UART2_BASE;

    if(pg_int_reg->SUBSRCPND & BIT_SUB_RXD2)
    {
        pg_int_reg->SUBSRCPND = BIT_SUB_RXD2;
        fifo_reg = Reg->UFSTAT;
        recv_trans = fifo_reg & 0x3f;
        for(num = 0; num < recv_trans; num++)
        //fifo������������,ֱ���������
            ch[num]= Reg->URXH;
        UART_PortWrite(UCB,ch,recv_trans);
        if( fifo_reg & cn_rx_over)
        {
            Reg->UFCON |= cn_rx_fifo_reset;
            UART_ErrHandle(UCB,CN_UART_FIFO_OVER_ERR);
       }
    }
    if(pg_int_reg->SUBSRCPND & BIT_SUB_TXD2)
    {
        pg_int_reg->SUBSRCPND = BIT_SUB_TXD2;
        fifo_reg = Reg->UFSTAT;
        recv_trans = UART_TX_FIFO_SIZE-((fifo_reg>>8) & 0x3f);
        recv_trans = UART_PortRead(UCB,ch,recv_trans);
        for(num = 0; num < recv_trans; num++)
        {
            Reg->UTXH = ch[num];
        }
    }
    if(pg_int_reg->SUBSRCPND & BIT_SUB_ERR2)
    {
        pg_int_reg->SUBSRCPND = BIT_SUB_ERR2;
        UART_ErrHandle(UCB,CN_UART_HARD_COMM_ERR);
    }
    return 0;
}

//----��ʼ��uart0ģ��----------------------------------------------------------
//���ܣ���ʼ��uart0ģ��
//������ģ���ʼ������û�в���
//���أ�true = �ɹ���ʼ����false = ��ʼ��ʧ��
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_UART(ptu32_t serial_no)
{
    struct Object *UCB;
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
//        UART_Param.DirectlySend = (UartDirectSend)__UART_SendDirectly;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART1://����1
        UART_Param.Name         = "UART1";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART1_BASE;
        UART_Param.TxRingBufLen = CFG_UART1_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART1_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
//        UART_Param.DirectlySend = (UartDirectSend)__UART_SendDirectly;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART2://����2
        UART_Param.Name         = "UART2";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART2_BASE;
        UART_Param.TxRingBufLen = CFG_UART2_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART2_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
//        UART_Param.DirectlySend = (UartDirectSend)__UART_SendDirectly;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    default:
        return 0;
    }

    //Ӳ����ʼ��
    __UART_GPIOConfig(serial_no);
    __UART_DefaultConfig((tagUartReg *)UART_Param.UartPortTag);
    __UART_IntConfig(serial_no);

    sUartInited |= (0x01 << serial_no);
    pUartCB[serial_no] = UART_InstallGeneral(&UART_Param);
    if( pUartCB[serial_no] == NULL)
        return 0;
    else
        return 1;

}

// =============================================================================

#include <stdio.h>

static tagUartReg *reg_stdio=NULL;
static int port_stdio=0;

// =============================================================================
// ���ܣ��ַ��ն�ֱ�ӷ��ͺ�����������ѯ��ʽ��ֱ��д�Ĵ���������printk������stdout
//       û�г�ʼ��
// ������str�������ַ���ָ��
//      len�����͵��ֽ���
// ���أ�0����������result,�������ݳ��ȣ��ֽڵ�λ
// =============================================================================
s32 Uart_PutStrDirect(const char *str,u32 len)
{
    u32 result = 0,port,timeout = 100*mS;
    struct Object *UCB = NULL;
    tagUartReg *Reg;
    u32 BaseAddr;

    if(reg_stdio==NULL)
    {
        return 0;
    }

    Reg = (struct UART_REG *)reg_stdio;

    __UART_SubIntDisable(port,SUB_TXD_FLG);
    for(result=0; result < len; result ++)
    {
        // ��ʱ���߷��ͻ���Ϊ��ʱ�˳�
        while((false == __UART_TxTranEmpty(Reg))&& (timeout > 0))
        {
            timeout--;
            DJY_DelayUs(1);
        }
        if(timeout == 0)
            break;
        Reg->UTXH = str[result];
    }
    __UART_SubIntEnable(port,SUB_TXD_FLG);
    return result;
}

// =============================================================================
// ���ܣ��ַ��ն�ֱ�ӽ��պ�����������ѯ��ʽ��ֱ�Ӷ��Ĵ���������stdin��ʼ��ǰ
// ���أ�����
// =============================================================================
char Uart_GetCharDirect(void)
{
    while(!(reg_stdio->UTRSTAT & 0x1));
    return reg_stdio->URXH;
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
        reg_stdio = (struct UART_REG*)CN_UART0_BASE;
        port_stdio = CN_UART0;
    }
    else if(!strcmp(StdioOut,"/dev/UART1"))
    {
        reg_stdio = (struct UART_REG*)CN_UART1_BASE;
        port_stdio = CN_UART1;
    }
    else if(!strcmp(StdioOut,"/dev/UART2"))
    {
        reg_stdio = (struct UART_REG*)CN_UART2_BASE;
        port_stdio = CN_UART2;
    }
    else
    {
        reg_stdio = NULL ;
        port_stdio =0xFFFF;
    }

    if(reg_stdio != NULL)
    {
        __UART_GPIOConfig(port_stdio);
        __UART_IntConfig(port_stdio);

        __UART_DefaultConfig(reg_stdio);
        __UART_BaudSet(reg_stdio,115200);

        PutStrDirect = Uart_PutStrDirect;
        GetCharDirect = Uart_GetCharDirect;
    }

}
