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
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
//Copyright (C), 2013-2020,  �����߲���ϵͳ�����Ŷ�
//��Ȩ���� (C), 2013-2020,   �����߲���ϵͳ�����Ŷ�
//����ģ��: uart��������(������������)
//����:  ������.
//�汾��V1.0.0
//�ļ�����:
//����˵��:
//�޶���ʷ:
//1. ����:2013-11-10
//   ����:  ������.
//   �°汾�ţ�1.0.0
//   �޸�˵��: ��ֲ��44b0��1.0.1��
//------------------------------------------------------
#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#include <device/include/uart.h>
#include "cpu_peri_uart.h"

#include "os.h"
#include "int_hard.h"
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
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu_peri_uart"        //CPU��uart��������
//parent:"uart"                         //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                         //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                      //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                        //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium                      //��ʼ��ʱ������ѡֵ��early��medium��later��
                                        //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"uart","devfile","heap"    //������������������������none����ʾ�������������
                                        //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                        //����������������������г����á�,���ָ�
//weakdependence:"none"                 //��������������������������none����ʾ�������������
                                        //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                        //����������������������г����á�,���ָ�
//mutex:"none"                          //������������������������none����ʾ�������������
                                        //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef CFG_UART0_SENDBUF_LEN   //****�������Ƿ��Ѿ����ú�
#warning    cpu_peri_uart�������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,32,2048,
#define CFG_UART0_SENDBUF_LEN            2048                 //"UART0���ͻ��λ�������С",
#define CFG_UART0_RECVBUF_LEN            2048                 //"UART0���ջ��λ�������С",

#define CFG_UART1_SENDBUF_LEN            2048                 //"UART1���ͻ��λ�������С",
#define CFG_UART1_RECVBUF_LEN            2048                 //"UART1���ջ��λ�������С",
//%$#@enum,true,false,
#define CFG_UART0_ENABLE                 true                 //"�Ƿ�����ʹ��UART0",
#define CFG_UART1_ENABLE                 false                //"�Ƿ�����ʹ��UART1",
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

#define CN_UART0_BASE   (cn_uart_baddr)
#define CN_UART1_BASE   (cn_uart_baddr + 0x100)

//#define UART0_SendBufLen  2048
//#define UART0_RecvBufLen  2048

//#define UART1_SendBufLen  2048
//#define UART1_RecvBufLen  2048

static struct obj *pUartCB[CN_UART_NUM];
//���ڱ�ʶ�����Ƿ��ʼ����ǣ���0λ��ʾUART0����һλ��UART1....
//�������ƣ�1��ʾ��ʼ����0��ʾδ��ʼ��
static u8 sUartInited = 0;

//----ʹ�ܽ����ж�-------------------------------------------------------------
//����: ʹ��uart�Ľ����жϡ�
//����: reg,�������ļĴ�����ָ��
//����: ��
//-----------------------------------------------------------------------------
void __UART_RxIntEnable(tagUartReg volatile *reg)
{
    reg->ULCR &= ~(0x80);                //����ULCR[DLAB]=0
    reg->UDMB_IER |= (u8)0x01;
}

//----��ֹ�����ж�-------------------------------------------------------------
//����: ��ֹuart�Ľ����жϡ�
//����: reg,�������ļĴ�����ָ��
//����: ��
//-----------------------------------------------------------------------------
void __UART_RxIntDisable(tagUartReg volatile *reg)
{
    reg->ULCR &= ~(0x80);                //����ULCR[DLAB]=0
    reg->UDMB_IER &= ~(u8)0x01;
}

//----ʹ�ܷ����ж�-------------------------------------------------------------
//����: ʹ��uart�ķ����жϡ�
//����: reg,�������ļĴ�����ָ��
//����: ��
//-----------------------------------------------------------------------------
void __UART_TxIntEnable(tagUartReg volatile *reg)
{
    reg->ULCR &= ~(0x80);                //����ULCR[DLAB]=0
    reg->UDMB_IER |= (u8)0x02;
}

//----��ֹ�����ж�-------------------------------------------------------------
//����: ��ֹuart�ķ����жϡ�
//����: reg,�������ļĴ�����ָ��
//����: ��
//-----------------------------------------------------------------------------
void __UART_TxIntDisable(tagUartReg volatile *reg)
{
    reg->ULCR &= ~(0x80);                //����ULCR[DLAB]=0
    reg->UDMB_IER &= ~(u8)0x02;
}

//----����baud-----------------------------------------------------------------
//����: ���ô���baud
//����: reg,�������ļĴ�����ָ��
//����: ��
//-----------------------------------------------------------------------------
void __UART_BaudSet(tagUartReg volatile *reg,u32 baud)
{
    u32 temp;

    reg->ULCR |= 0x80; //���ò�����ʱ������DLB��DMB������ULCR[DLAB]=1
    temp = (u32)(cfg_core_ccb_clk)/(baud*16);
    reg->URBR_THR_DLB = (u8)temp;
    reg->UDMB_IER = (u8)(temp>>8);
    reg->ULCR &= ~(0x80);                //����ULCR[DLAB]=0���ָ�Ĭ��
}
//----����Ӳ������-------------------------------------------------------------
//����: ����Ӳ�����������������ʡ�ֹͣλ��У��λ������λ
//����: reg,�������ļĴ�����ָ��
//����: ��
//-----------------------------------------------------------------------------
void __UART_DefaultSet(tagUartReg volatile *reg)
{
    __UART_BaudSet(reg,115200);            //����������
    reg->UIIR_FCR_AFR;                         //��һ����������жϱ�־
    reg->UIIR_FCR_AFR |= 0x01;              //����ʹ��FIFO
//    reg->UIIR_FCR_AFR   |= 0x08;            //DMAģʽѡ��Ϊ1
    reg->UIIR_FCR_AFR |= 0xc6;              //����14�ֽڴ����жϣ����շ�fifo
    reg->ULCR = 0x03 | (0<<2) | (000<<3);   //����8bit��ֹͣλ=1����У��
    reg->UMCR = 0;
    reg->UDMB_IER =  0x03;//ʹ���շ��жϣ���ʱ�ж��߿�����δʹ��
}
//----��鴮�ڷ��ͼĴ����Ƿ��-------------------------------------------------
//����: ��鷢�ͼĴ�����״̬������վͷ���true
//�������ޡ�
//����: true = �������գ�false = �ǿ�
//-----------------------------------------------------------------------------
bool_t __UART_TxTranEmpty(volatile tagUartReg *reg)
{
    return (0 != (reg->ULSR & 0x20));
}

//----�����ж�ϵͳ��ʼ��--------------------------------------------------------
//����: ��ʼ���ж�ϵͳ���������ڹ���һ���ж���
//���������ں�
//����: ��
//-----------------------------------------------------------------------------
extern uint32_t UART_ISR(ptu32_t IntLine);

void __UART_IntInit(ptu32_t SerialNo)
{
    u8 IntLine;
    IntLine = cn_int_line_duart;
    //�ж��ߵĳ�ʼ��
    Int_Register(IntLine);
    Int_IsrConnect(IntLine,UART_ISR);
    Int_SettoAsynSignal(IntLine);
    Int_SetLineTrigerType(IntLine,EN_INT_TRIGER_HIGHLEVEL);//set the uart active high
    Int_ClearLine(IntLine);     //�����ʼ�������ķ���fifo�յ��ж�
    Int_RestoreAsynLine(IntLine);
}

//----�����豸���ƺ���---------------------------------------------------------
//����: �����豸�Ŀ��ƺ���,������Ӳ���Ĵ����������
//����: uart_dev,�������Ĵ����豸ָ��.
//      cmd,��������
//      data,������cmd����
//����: ������.
//-----------------------------------------------------------------------------
ptu32_t __UART_Ctrl(tagUartReg *Reg,u32 cmd, u32 data1,u32 data2)
{
    if(Reg == NULL)
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
            __UART_BaudSet(Reg,data1);
            break;
        case CN_UART_RX_PAUSE:      //��ͣ����
            __UART_RxIntDisable(Reg);
            break;
        case CN_UART_RX_RESUME:      //�ָ�����
            __UART_RxIntEnable(Reg);
            break;
        case CN_UART_RECV_HARD_LEVEL:    //���ý���fifo����ˮƽ
        {
            (Reg)->UIIR_FCR_AFR &= 0x3f;
            switch (data1)
            {
                case 1:
                    (Reg)->UIIR_FCR_AFR |= 0x00;//1�ֽڴ���
                    break;
                case 4:
                    (Reg)->UIIR_FCR_AFR |= 0x10;//4�ֽڴ���
                    break;
                case 8:
                    (Reg)->UIIR_FCR_AFR |= 0x20;//8�ֽڴ���
                    break;
                case 14:
                    (Reg)->UIIR_FCR_AFR |= 0x30;//14�ֽڴ���
                    break;
                default :break;
            }
            break;
        }
        case CN_UART_COM_SET:
//          __UART_ComConfig(Reg,data1);todo
            break;
        default: break;
    }
    return 0;
}


//----���з��ͷ���ֱ��д�նˣ�----------------------------------------------
//����: �����ֱ��д���ں��������ᾭ���¼�����
//����: uart_dev,�������Ĵ����豸ָ��.
//      send_buf,�����͵Ļ�������
//      len,���͵������ֽ���
//      timeout,��ʱ
//����: ���͵ĸ���
//-----------------------------------------------------------------------------
//u32 __UART_SendDirectly(tagUartReg *Reg,u8 *send_buf,u32 len,u32 timeout)
//{
//    u32  result;
//
//    if(Reg == NULL)
//        return 0;
//    __UART_TxIntDisable(Reg);
//    for(result=0; result < len; result ++)
//    {
//        while((false == __UART_TxTranEmpty(Reg))
//            && (timeout > 0))//��ʱ���߷��ͻ���Ϊ��ʱ�˳�
//        {
//            timeout--;
//            Djy_DelayUs(1);
//        }
//        if(timeout == 0)
//            break;
//        Reg->URBR_THR_DLB = send_buf[result];
//    }
//    __UART_TxIntEnable(Reg);
//    return result;
//}

//----�������ڷ��ͺ���---------------------------------------------------------
//����: �������ڷ��ͣ�ֱ�ӷ���fifo��С�����ݣ����������Ϳ��жϣ����ж��н�
//      send_ring_buf�е���������������ݵķ��ͣ��൱��������һ�η��͹���
//����: uart_dev,�������Ĵ����豸ָ��.
//����: ���͵ĸ���
//-----------------------------------------------------------------------------
u32 __UART_SendStart(tagUartReg *Reg,u32 timeout)
{
    u8 trans,num,ch[16],port;

    if((u32)Reg == CN_UART0_BASE)
        port = CN_UART0;
    else if((u32)Reg == CN_UART1_BASE)
        port = CN_UART1;
    else
        return 0;

    __UART_TxIntDisable(Reg);
    if(__UART_TxTranEmpty(Reg))
    {
        trans = UART_PortRead(pUartCB[port],ch,16);
        for(num = 0; num < trans; num++)
        {
            (Reg)->URBR_THR_DLB = ch[num];
        }
    }
    __UART_TxIntEnable(Reg);
    return trans;
}
//----uart�ж�---------------------------------------------------------------
//����: ���գ���ȡfifo�е����ݲ�������д���豸
//      ���ͣ������ֶ����ݲ�����
//����: �жϺ�.
//����: 0.
//-----------------------------------------------------------------------------
uint32_t UART_ISR(ptu32_t IntLine)
{
    struct obj *UCB = NULL;
    tagUartReg *Reg;
    uint32_t recv_trans,num;
    uint8_t ch[20],IIR=0;

    Reg =  (tagUartReg *)CN_UART0_BASE;
    IIR = Reg->UIIR_FCR_AFR;
    UCB = pUartCB[CN_UART0];
    if(IIR & 0x01)//uart0���ж�
    {
        Reg =  (tagUartReg *)CN_UART1_BASE;
        IIR = Reg->UIIR_FCR_AFR;
        if(IIR & 0x01)//uart1���ж�
            return 0;
        UCB = pUartCB[CN_UART1];
    }
    IIR = IIR &0x7;
    switch(IIR)
    {
        case 2:     //UTHR�Ĵ���Ϊ��
        {
            recv_trans = UART_PortRead(UCB,ch,16);
            for(num = 0; num < recv_trans; num++)
            {
                Reg->URBR_THR_DLB = ch[num];
            }
        }break;
        case 4:     //���յ���Ч���ݻ�ʹ��FIFOʱ�����մ���ˮƽ��
        case 12:     //���ճ�ʱ��fifo��������һ�����ݣ�����4�ַ�ʱ����������
        {
            for(num = 0; num<20; num++)
            {
                if(Reg->ULSR & 0x01)    //�ж�ULSR��DRλ���ж��Ƿ������ݿɶ�
                    ch[num] = Reg->URBR_THR_DLB;
                else
                    break;
            }
            UART_PortWrite(UCB,ch,num);
        }break;
        case 6:     //������־��ֹͣλ����֡������������
        {
            IIR = Reg->ULSR;//���жϱ�־
        }break;
        default:
            break;
    }
    return 0;
}

//----��ʼ��uart1ģ��----------------------------------------------------------
//���ܣ���ʼ��uart1ģ��,��װ�ش����豸
//���������ڳ�ʼ��ģ��Ĳ���Ϊ1��2��3...���ں�
//���أ�1=�ɹ���0=ʧ��
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_UART(ptu32_t serial_no)
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
    default:
        return 0;
    }

    //Ӳ����ʼ��
    __UART_DefaultSet((tagUartReg *)UART_Param.UartPortTag);
    __UART_IntInit(serial_no);

    sUartInited |= (0x01 << serial_no);
    pUartCB[serial_no] = UART_InstallPort(&UART_Param);
    if( pUartCB[serial_no] == NULL)
        return 0;
    else
        return 1;
}

static tagUartReg  *PutStrDirectReg = NULL;
static tagUartReg  *GetCharDirectReg = NULL;

//----���з��ͷ���ֱ��д�նˣ�-------------------------------------------------
//����: �����ֱ��д���ں��������ᾭ���¼�����
//����: ����Ҫ���͵��ַ�������Ȼ��ǰ�������ṩ��һ�����ַ���'\0'����
//����: ���͵ĸ���
//-----------------------------------------------------------------------------
s32 Uart_PutStrDirect(const char *str,u32 len)
{
    u32  result,timeout;
    tagUartReg *Reg;

    Reg = PutStrDirectReg;
    __UART_TxIntDisable(Reg);
    for(result=0; result < len; result ++)
    {
        timeout = 0;
        while((false == __UART_TxTranEmpty(Reg))
            && (timeout++ < 10000));//��ʱ���߷��ͻ���Ϊ��ʱ�˳�
        if(timeout >= 10000)
            break;
        Reg->URBR_THR_DLB = ((uint8_t*)str)[result];
    }
    __UART_TxIntEnable(Reg);
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
    char result = '\n';


    return result;
}

//----��ʼ���ں˼�IO-----------------------------------------------------------
//���ܣ���ʼ���ں˼��������������runtime����ָ�롣
//��������
//���أ���
//-----------------------------------------------------------------------------
void Stdio_KnlInOutInit(char * StdioIn, char *StdioOut)
{
    u32 TxDirectPort = 0;
    u32 RxDirectPort = 0;

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
    else
    {
        PutStrDirectReg = NULL ;
    }

    if(PutStrDirectReg != NULL)
    {
        __UART_DefaultSet(PutStrDirectReg);
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
    else
    {
        GetCharDirectReg = NULL ;
    }
    if(GetCharDirectReg != NULL)
    {
        if(TxDirectPort != RxDirectPort)
            __UART_DefaultSet(GetCharDirectReg);
        GetCharDirect = Uart_GetCharDirect;
    }
    return;
}


