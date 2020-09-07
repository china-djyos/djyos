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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "djyos.h"
#include "int.h"
#include "systime.h"
#include "cpu_peri.h"
#include "uartctrl.h"
#include <device/djy_uart.h>
#include <silan_syscfg.h>
#include <silan_uart.h>
#include "cpu_peri_isr.h"

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
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip uart"//CPU��uart��������
//parent:"uart device file"      //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"device file system","lock","uart device file","uart device fileheap","cpu onchip dma"//������������������������none����ʾ�������������
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
//%$#@enum,true,false,
#define CFG_UART1_ENABLE                 false               //"�Ƿ�ʹ��UART1",
#define CFG_UART2_ENABLE                 true              //"�Ƿ�ʹ��UART2",
#define CFG_UART3_ENABLE                 false              //"�Ƿ�ʹ��UART3",
#define CFG_UART4_ENABLE                 false              //"�Ƿ�ʹ��UART4",
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

typedef struct __UART_REG
{
    vu32 DR;
    vu32 RSR_ECR;
    vu32 REV0[4];
    vu32 FR;
    vu32 REV1;
    vu32 ILPR;
    vu32 IBRD;
    vu32 FBRD;
    vu32 LCR_H;
    vu32 CR;
    vu32 IFLS;
    vu32 IMSC;
    vu32 RIS;
    vu32 MIS;
    vu32 ICR;
    vu32 DMACR;
}tagUartReg;

#define UART_INTR_EN_MCU4    (0x42030000 + 0x610)
#define UART_INTR_STS_MCU4   (0x42030000 + 0x510)

extern const char *gc_pCfgStdinName;    //��׼�����豸
extern const char *gc_pCfgStdoutName;   //��׼����豸
//extern const char *gc_pCfgStderrName;   //��׼��������豸

// =============================================================================
static const tagUartReg volatile *sUartReg[CN_UART_NUM] = {
                                            (tagUartReg volatile *)0X41000000,
                                            (tagUartReg volatile *)0X41010000,
                                            (tagUartReg volatile *)0X41020000,
                                            (tagUartReg volatile *)0X41030000,
};
static const char *sUartName[CN_UART_NUM] = {
        "UART1","UART2","UART3","UART4"
};
static const u8 sUartFifo[]={
        64,16,16,16
};
// =============================================================================

u16 UART_SndBufLen;
u16 UART_RxBufLen;

static u32 TxByteTime;                    //��������һ���ֽ�����Ҫ��ʱ��
static u32 TxDirectPort;                  //����printk���͵Ĵ��ں�
static u32 RxDirectPort;                  //����ֱ�ӽ��յĴ��ں�
// =============================================================================

static struct UartCB *pUartCB[CN_UART_NUM];

// =============================================================================
static ptu32_t UART_ISR(ptu32_t IniLine);
__attribute__((weak)) void Board_UartHalfDuplexSend(u8 port)
{
    //user added if need out of the file
}
__attribute__((weak)) void Board_UartHalfDuplexRecv(u8 port)
{
    //user added if need out of the file
}

static void __UART_Enable(tagUartReg *Reg,u8 Enable)
{
    if(Enable)
    {
        Reg->CR |= (1<<0);
    }
    else
    {
        Reg->CR &= ~(1<<0);
    }
}
static void __UART_RxEnable(tagUartReg *Reg,u8 Enable)
{
    if(Enable)
    {
        Reg->CR |= (1<<9);
    }
    else
    {
        Reg->CR &= ~(1<<9);
    }
}
static void __UART_TxEnable(tagUartReg *Reg,u8 Enable)
{
    if(Enable)
    {
        Reg->CR |= (1<<8);
    }
    else
    {
        Reg->CR &= ~(1<<8);
    }
}

static void __UART_TxIntEnable(tagUartReg *Reg,u8 Enable)
{
    if(Enable)
    {
        Reg->IMSC |= (1<<5);
    }
    else
    {
        Reg->IMSC &= ~(1<<5);
    }
}

static void __UART_RxIntEnable(tagUartReg *Reg,u8 Enable)
{
    if(Enable)
    {
        Reg->IMSC |= (1<<4)|(1<<6);
    }
    else
    {
        Reg->IMSC &= ~((1<<4)|(1<<6));
    }
}
// =============================================================================
// ����: ���ô���baud,PCLKΪ25M��CPU��ƵΪ216M����,������ֻ�ṩ9600��19200��57600��
//       115200���ֲ���������
// ����: Reg,�������ļĴ�����ָ��
//       port,���ں�
//       baud,������
// ����: ��
// =============================================================================
static void __UART_BaudSet(tagUartReg *Reg,u8 port,u32 baudrate)
{
    u32 temp, remainder, divider, fraction;
    u32 uart_clk;

    __UART_Enable(Reg,0);
    uart_clk = get_sysclk_val_settled();
    temp = 16 * baudrate;
    divider = uart_clk / temp;
    remainder = uart_clk % temp;
    temp = (8 * remainder) / baudrate;
    fraction = (temp >> 1) + (temp & 1);
    Reg->IBRD = divider;
    Reg->FBRD = fraction;
    if(TxDirectPort == port)
    {
        TxByteTime = 11000000/baudrate;     //1���ֽڴ���ʱ�䣬��10bit��+10%����
    }
    __UART_Enable(Reg,1);
}
// =============================================================================
// ����: �йش��ڲ����ĳ�ʼ���������ʣ�ֹͣλ����żУ��λ
// ������port,���ں�
//      data������
// ����: ��
// =============================================================================
static void __UART_ComConfig(tagUartReg volatile *Reg,u32 port,struct COMParam *COM)
{
    struct COMParam *COM;
    if((COM == NULL) || (Reg == NULL))
        return;
    __UART_BaudSet(Reg,port,COM->BaudRate);

    __UART_Enable(Reg,0);
    switch(COM->DataBits)               // data bits
    {
        Reg->LCR_H &= ~(0x03<<5);
        case CN_UART_DATABITS_5:
            Reg->LCR_H |= (0<<5);
            break;
        case CN_UART_DATABITS_6:
            Reg->LCR_H |= (1<<5);
            break;
        case CN_UART_DATABITS_7:
            Reg->LCR_H |= (2<<5);
            break;
        case CN_UART_DATABITS_8:
            Reg->LCR_H |= (3<<5);
            break;
        default:
            Reg->LCR_H |= (3<<5);
            break;
    }

    switch(COM->Parity)        //
    {
        case CN_UART_PARITY_NONE:
            Reg->LCR_H &= ~(1<<1);
            break;
        case CN_UART_PARITY_ODD:
            Reg->LCR_H |= (1<<1);
            break;
        case CN_UART_PARITY_EVEN:
            Reg->LCR_H |= (3<<1);
            break;
        default:break;
    }

    switch(COM->StopBits)  //todu:F7�� 0.5��ֹͣλû��д����
    {
        case CN_UART_STOPBITS_1:
            Reg->LCR_H &= ~(1<<3);
            break;
        case CN_UART_STOPBITS_2:
            Reg->LCR_H |= (1<<3);
            break;
        default:break;
    }
    __UART_Enable(Reg,1);
}

static bool_t __UART_TxFIFO_Empty(tagUartReg *Reg)
{
    return (bool_t)(Reg->FR & UART_FR_TXFE);
}

// =============================================================================
// ����: ��ʼ��UART��Ӧ���ж��ߣ�����ʼ���ж���ں���
// ������SerialNo,���ں�
// ����: ��
// =============================================================================
static void __UART_IntInit(u32 port)
{
    u8 IntLine;
    u8 subIntLine;

    switch(port)
    {
        case CN_UART1:
             subIntLine  = CN_SUBID_MISC_UART1;
             break;
        case CN_UART2:
             subIntLine  = CN_SUBID_MISC_UART2;
             break;
        case CN_UART3:
             subIntLine  = CN_SUBID_MISC_UART3;
             break;
        case CN_UART4:
             subIntLine  = CN_SUBID_MISC_UART4;
             break;
        default:subIntLine  = CN_SUBID_MISC_UART2;break;
    }

    IntLine = CN_INT_LINE_UART;

    *(u32*)UART_INTR_EN_MCU4 |= (1<<port);

    djybsp_isr_hdl_register(IntLine,subIntLine,UART_ISR,0);

}
// =============================================================================
// ����: Ӳ���������úͼĴ����ĳ�ʼ�������������ʡ�ֹͣλ��У��λ������λ��Ĭ�������:
//       ������:115200  �� ֹͣλ:1 ; У��:�� ; ����λ:8bit
// ����: SerialNo,���ںţ�0~3
// ����: ��
// =============================================================================
static void __UART_HardInit(u8 port)
{
    if(port < CN_UART_NUM)
    {
        silan_module_clkon(CLK_UART1+port, CLK_ON);
        silan_uart_sys_init(UART_1+port, BAUDRATE_115200);

        __UART_RxIntEnable(sUartReg[port],1);   //rx int enable
        __UART_RxEnable(sUartReg[port],1);
        __UART_Enable(sUartReg[port],1);
    }
}

// =============================================================================
// ����: �������ڷ��ͣ��������������ʹ��DMA��ʹ��DMA
// ����: Reg,�������Ĵ��ڼĴ���ָ��.
// ����: ���͵ĸ���
// =============================================================================
static u32 __UART_SendStart (tagUartReg *Reg)
{
    u8 len=0,i,port;
    u8 fifo[64];

    for(port = CN_UART1;port < CN_UART_NUM;port ++)
    {
        if(Reg == sUartReg[port])
            break;
    }
    if(port < CN_UART_NUM)
    {
        if(__UART_TxFIFO_Empty(Reg))
        {
            __UART_TxIntEnable(Reg,0);
            len = UART_PortRead(pUartCB[port],fifo,sUartFifo[port]);
            for(i = 0; i < len; i++)
            {
                Reg->DR = fifo[i];
            }

            __UART_TxIntEnable(Reg,1);
            __UART_TxEnable(Reg,1);
        }
    }
    return len;
}

// =============================================================================
// ����: �����豸�Ŀ��ƺ���,������Ӳ���Ĵ����������
// ����: Reg,�������Ĵ��ڼĴ���ָ��.
//       cmd,��������
//       data1,data2,������cmd����
// ����: ������.
// =============================================================================
static ptu32_t __UART_Ctrl(tagUartReg *Reg,u32 cmd,va_list *arg0)
{
    ptu32_t result = 0;
    u32 port;
    s32 timeout = 100000;
    for(port = CN_UART1;port < CN_UART_NUM;port ++)
    {
        if(Reg == sUartReg[port])
            break;
    }
    if(port < CN_UART_NUM)
    {
        switch(cmd)
        {
//        case CN_UART_START:
//          __UART_Enable(Reg,1);
//          break;
//        case CN_UART_STOP:
//          __UART_Enable(Reg,0);
//            break;
        case CN_UART_SET_BAUD:
        {
            u32 data;
            data = va_arg(*arg0, u32);
            __UART_BaudSet(Reg,port, data);
        }
            break;
        case CN_UART_COM_SET:
        {
            struct COMParam *COM;
            COM = va_arg(*arg0, void *);
            __UART_ComConfig(Reg,port,COM);
        }
            break;
        case CN_UART_HALF_DUPLEX_SEND: //��������
            Board_UartHalfDuplexSend(port);
            break;
        case CN_UART_HALF_DUPLEX_RECV: //��������
            while(__UART_TxFIFO_Empty(Reg) && (timeout > 10))
            {
                timeout -=10;
                DJY_DelayUs(10);
            }
            Board_UartHalfDuplexRecv(port);
            break;
        default:
            break;
        }
    }
    return result;
}

// =============================================================================
// ���ܣ�UART�ж�,��Ϊidle�жϣ����DMA�������ж������ݣ�����������DMA���������HAL�ж�
//       �����������ջ���õ�HAL_UART_XXXXCallback����
// ���������ں�
// ���أ�1=�ɹ���0=ʧ��
// =============================================================================
u32 UART_ISR(ptu32_t port)
{
    u32 num,status;
    tagUartReg volatile *Reg;
    u8 fifo[64];

//    status = (*(u32*)UART_INTR_STS_MCU4);
//
//    for(port = 0; port < CN_UART4;port++)
//    {
//      if(status & (1<<port))
//      {
//          break;
//      }
//    }

    if(port < CN_UART_NUM)
    {
        Reg = sUartReg[port];
        if( (Reg->RIS & (1<<4)) || (Reg->RIS & (1 << 6)))   //rx or idle int
        {
            Reg->ICR |= (1<<4)|(1<<6);
            num = 0;
            while(!(Reg->FR & UART_FR_RXFE))
            {
                if(num<sUartFifo[port])
                {
                    fifo[num++] = (u8)(Reg->DR);
                }
                else
                {
                    break;
                }
            }
            if(num > 0)
            {
                if(num != UART_PortWrite(pUartCB[port],fifo,num))
                {
                    UART_ErrHandle(pUartCB[port],CN_UART_BUF_OVER_ERR);
//                  printk("uart idle over!\r\n");
                }
            }
        }

        if(Reg->RIS & (1<<5))                   //tx int
        {
            Reg->ICR |= (1<<5);
            num = 0;
            while(!(Reg->FR & UART_FR_TXFF))
            {
                if(0 != UART_PortRead(pUartCB[port],&fifo[0],1))
                {
                    Reg->DR = fifo[0];
                }
                else
                {
                    break;
                }
            }
        }

        if(Reg->RIS & (0xF << 7))   //hard error int
        {
            if(Reg->RIS & (1<<7))   //frame error
            {
                printf("uart frame error!\r\n");
                Reg->ICR |= (1<<7);
            }
            if(Reg->RIS & (1<<8))   //parity error
            {
                printf("uart parity error!\r\n");
                Reg->ICR |= (1<<8);
            }
            if(Reg->RIS & (1<<9))   //BE
            {
                printf("uart be error!\r\n");
                Reg->ICR |= (1<<9);
            }
            if(Reg->RIS & (1<<10))  //over error
            {
                printf("uart over error!\r\n");
                Reg->ICR |= (1<<10);
            }
        }
    }
    return 1;
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
ptu32_t ModuleInstall_UART(u32 port)
{
    bool_t Ret = false;
    struct UartParam UART_Param;

    switch(port)
    {
        case CN_UART1:
            UART_SndBufLen = CFG_UART1_SENDBUF_LEN;
            UART_RxBufLen  = CFG_UART1_RECVBUF_LEN;
            break;
        case CN_UART2:
            UART_SndBufLen = CFG_UART2_SENDBUF_LEN;
            UART_RxBufLen  = CFG_UART2_RECVBUF_LEN;
            break;
        case CN_UART3:
            UART_SndBufLen = CFG_UART3_SENDBUF_LEN;
            UART_RxBufLen  = CFG_UART3_RECVBUF_LEN;
            break;
        case CN_UART4:
            UART_SndBufLen = CFG_UART4_SENDBUF_LEN;
            UART_RxBufLen  = CFG_UART4_RECVBUF_LEN;
            break;
        deault:printk("ModuleInstall_UART:port Err\r\n");break;
    }

    if(port < CN_UART_NUM)
    {
        UART_Param.Name         = sUartName[port];
        UART_Param.UartPortTag  = (ptu32_t)sUartReg[port];
        UART_Param.Baud         = 115200;
        UART_Param.TxRingBufLen = UART_SndBufLen;
        UART_Param.RxRingBufLen = UART_RxBufLen;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;

        pUartCB[port] = UART_InstallGeneral(&UART_Param);
        if( pUartCB[port] != NULL)
        {
            __UART_HardInit(port);              //Ӳ����ʼ��
            __UART_IntInit(port);
            Ret = true;
        }
    }
    return Ret;
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
    u32 timeout,i=0;
    tagUartReg *Reg;
    TxByteTime = 11000000/115200;

    timeout= TxByteTime * len;

    Reg = sUartReg[TxDirectPort];

    while(1)
    {
        if( !(Reg->FR & (1<<5)) )
        {
            Reg->DR = str[i++];
            if(i==len)
                break;
        }
        else
        {
//          DJY_EventDelay(1000);
            DJY_DelayUs(1000);
        }
    }

    return len;
}

// =============================================================================
// ���ܣ��ַ��ն�ֱ�ӽ��պ�����������ѯ��ʽ��ֱ�Ӷ��Ĵ���������stdin��ʼ��ǰ
// ������str�������ַ���ָ��
//      len�����͵��ֽ���
// ���أ�0����������result,�������ݳ��ȣ��ֽڵ�λ
// =============================================================================
char Uart_GetCharDirect(void)
{
    u8 result;
    tagUartReg *Reg;
    Reg = sUartReg[RxDirectPort];
    while(1)
    {
        if( !(Reg->FR & (1<<4)) )
        {
            result = Reg->DR;
            break;
        }
        printk("bsp uart\r\n");
        DJY_EventDelay(500);
    }

//  Reg->IMSC &= ~((1<<4)|(1<<6));
//  while(Reg->FR & (1<<4) == 0);
//  result = Reg->DR;
//  Reg->FR &= ~(1<<4);
//  Reg->IMSC |= (1<<4)|(1<<6);

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
        TxDirectPort = CN_UART1;
    }
    else if(!strcmp(StdioOut,"/dev/UART2"))
    {
        TxDirectPort = CN_UART2;
    }
    else if(!strcmp(StdioOut,"/dev/UART3"))
    {
        TxDirectPort = CN_UART3;
    }
    else if(!strcmp(StdioOut,"/dev/UART4"))
    {
        TxDirectPort = CN_UART4;
    }
    else
    {
        PutStrDirect = NULL ;
    }

    //if(PutStrDirect != NULL)
    if(TxDirectPort >= 0)
    {
        __UART_HardInit(TxDirectPort);
        TxByteTime = 95;      //��ʼ��Ĭ��115200������һ��byte��87uS,+10%����
        PutStrDirect = Uart_PutStrDirect;
    }

    if(!strcmp(StdioIn,"/dev/UART1"))
    {
        RxDirectPort = CN_UART1;
    }
    else if(!strcmp(StdioIn,"/dev/UART2"))
    {
        RxDirectPort = CN_UART2;
    }
    else if(!strcmp(StdioIn,"/dev/UART3"))
    {
        RxDirectPort = CN_UART3;
    }
    else if(!strcmp(StdioIn,"/dev/UART4"))
    {
        RxDirectPort = CN_UART4;
    }
    else
    {
        GetCharDirect = NULL ;
    }

   // if(GetCharDirect != NULL)
    if(RxDirectPort >= 0)
    {
        if(TxDirectPort != RxDirectPort)
            __UART_HardInit(RxDirectPort);
        GetCharDirect = Uart_GetCharDirect;
    }
    return;
}

#ifdef CFG_CPU_ZQ12XX_CK
#include "shell.h"
typedef void (*Ex_shell_func)(void);
void __asm_ExecuteCmd(union param *ptab,
       enum param_typr *pflag,Ex_shell_func fun,u32 num)
{

}
#endif

