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
#include "cpu_peri.h"
#include <device/djy_uart.h>
#include "stdlib.h"
#include "uart/uart.h"
#include <icu_pub.h>
#include "arm_arch.h"
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
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip uart"//CPU��uart��������
//parent:"uart device file"      //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"device file system","lock","uart device file","heap"//������������������������none����ʾ�������������
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
//%$#@num,0,4096,
#define CFG_UART1_SENDBUF_LEN       256      //"UART1���ͻ��λ�������С",
#define CFG_UART1_RECVBUF_LEN       256      //"UART1���ջ��λ�������С",

#define CFG_UART2_SENDBUF_LEN       256      //"UART2���ͻ��λ�������С",
#define CFG_UART2_RECVBUF_LEN       256      //"UART2���ջ��λ�������С",
//%$#@enum,true,false
#define CFG_UART1_ENABLE           true        //"�Ƿ�ʹ��UART1",
#define CFG_UART2_ENABLE           true       //"�Ƿ�ʹ��UART2",
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������
//@#$%component end configure
// =============================================================================

#define REG_READ(addr)          (*((volatile UINT32 *)(addr)))
#define REG_WRITE(addr, _data)  (*((volatile UINT32 *)(addr)) = (_data))

#if (CN_BEKEN_SDK_V3 == 1)
extern void uart_hw_set_change(UINT8 uport, bk_uart_config_t *uart_config);
static bk_uart_config_t djybsp_uart[CN_UART_NUM] = {
    {
        .baud_rate = UART_BAUDRATE_115200,
        .data_width = BK_DATA_WIDTH_8BIT,
        .parity = BK_PARITY_NO,
        .stop_bits = BK_STOP_BITS_1,
        .flow_control = FLOW_CTRL_DISABLED,
    },
    {
       .baud_rate = UART_BAUDRATE_115200,
       .data_width = BK_DATA_WIDTH_8BIT,
       .parity = BK_PARITY_NO,
       .stop_bits = BK_STOP_BITS_1,
       .flow_control = FLOW_CTRL_DISABLED,
    },
};
#else
extern void uart_hw_set_change(UINT8 uport, uart_config_t *uart_config);
static uart_config_t djybsp_uart[CN_UART_NUM] = {
    {
        .baud_rate = UART_BAUDRATE_115200,
        .data_width = DATA_WIDTH_8BIT,
        .parity = BK_PARITY_NO,
        .stop_bits = BK_STOP_BITS_1,
        .flow_control = FLOW_CTRL_DISABLED,
    },
    {
       .baud_rate = UART_BAUDRATE_115200,
       .data_width = DATA_WIDTH_8BIT,
       .parity = BK_PARITY_NO,
       .stop_bits = BK_STOP_BITS_1,
       .flow_control = FLOW_CTRL_DISABLED,
    },
};
#endif

static u16 UART_SndBufLen = 0;
static u16 UART_RxBufLen = 0;
static u8 TxDirectPort = CN_LIMIT_UINT8;        //����printk���͵Ĵ��ں�
static u8 RxDirectPort = CN_LIMIT_UINT8;        //����ֱ�ӽ��յĴ��ں�
static struct UartGeneralCB *pUartCB[CN_UART_NUM];
static const char *sUartName[CN_UART_NUM] = {
        "UART1","UART2",
};

static const uint8_t volatile *sUartReg[CN_UART_NUM] = {
                                            CN_UART1,
                                            CN_UART2,
};
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
static ptu32_t __UART_Ctrl(uint8_t port,u32 cmd, va_list *arg0);

__attribute__((weak))  void Board_UartHalfDuplexSend(u8 SerialNo)
{
    return;
}
__attribute__((weak))  void Board_UartHalfDuplexRecv(u8 SerialNo)
{
    return ;
}

static void __UART_BaudSet(uint8_t port,uint32_t data)
{
    if((data == 0) || (port > CN_UART2))
        return;
    djybsp_uart[port].baud_rate = data;
        uart_hw_set_change(port,&djybsp_uart[port]);
}
// =============================================================================
// ����: �ش����ж�
// ����: SerialNo,���ںţ�0,1
// ����: ��
// =============================================================================
static void __UART_CloseInte(uint8_t SerialNo)
{
    u32 param;
    if(SerialNo == CN_UART1)
    {
        param = IRQ_UART1_BIT;
        sddev_control(ICU_DEV_NAME, CMD_ICU_INT_DISABLE, &param);
    }
    else if(SerialNo == CN_UART2)
    {
        param = IRQ_UART2_BIT;
        sddev_control(ICU_DEV_NAME, CMD_ICU_INT_DISABLE, &param);
    }
}

// =============================================================================
// ����: �������ж�
// ����: SerialNo,���ںţ�0,1
// ����: ��
// =============================================================================
static void __UART_OpenInte(uint8_t SerialNo)
{
    u32 param;
    if(SerialNo == CN_UART1)
    {
        param = IRQ_UART1_BIT;
        sddev_control(ICU_DEV_NAME, CMD_ICU_INT_ENABLE, &param);
    }
    else if(SerialNo == CN_UART2)
    {
        param = IRQ_UART2_BIT;
        sddev_control(ICU_DEV_NAME, CMD_ICU_INT_ENABLE, &param);
    }
}

// =============================================================================
// ����: Ӳ���������úͼĴ����ĳ�ʼ�������������ʡ�ֹͣλ��У��λ������λ��Ĭ�������:
//       ������:115200  �� ֹͣλ:1 ; У��:�� ; ����λ:8bit
// ����: SerialNo,���ںţ�0~3
// ����: ��
// =============================================================================
static void __UART_HardInit(u8 SerialNo)
{
    switch(SerialNo)
    {
        case CN_UART1:
            uart1_init();
//            __UART_BaudSet(SerialNo, 115200);
            break;
        case CN_UART2:
            uart2_init();
            break;
    }
}

// =============================================================================
// ����: ʹ����ֹͣ����������ʱ�Ӻ�uartģ��
// ����: port,���ں�
// ����: ��
// =============================================================================
static void __UART_Disable(u32 port)
{
    switch(port)
    {
        case CN_UART1:  uart1_exit();  break;
        case CN_UART2:  uart2_exit(); break;
        default:break;
    }
}

// =============================================================================
// ����: �Դ��ڴ���������ã����������ʡ���żУ�顢����λ��ֹͣλ
// ����:  Reg,�������ļĴ�����ָ��
//        port,���ں�
//        data,�ṹ��tagCOMParam���͵�ָ����ֵ
// ����: ��
// =============================================================================
static void __UART_ComConfig(u32 port,struct COMParam *COM)
{
//  struct COMParam *COM;
    if((COM == 0) || (port > CN_UART2))
        return;
//    COM = (struct COMParam *)data;
    djybsp_uart[port].baud_rate = COM->BaudRate;

    switch(COM->DataBits)               // data bits
    {
#if (CN_BEKEN_SDK_V3 == 1)
        case CN_UART_DATABITS_7:
            djybsp_uart[port].data_width = BK_DATA_WIDTH_7BIT;
            break;

        case CN_UART_DATABITS_8:
            djybsp_uart[port].data_width = BK_DATA_WIDTH_8BIT;
            break;
#else
        case CN_UART_DATABITS_7:
            djybsp_uart[port].data_width = DATA_WIDTH_7BIT;
            break;

        case CN_UART_DATABITS_8:
            djybsp_uart[port].data_width = DATA_WIDTH_8BIT;
            break;
#endif
        default:break;
    }

    switch(COM->StopBits)  //todu:F7�� 0.5��ֹͣλû��д����
    {
        case CN_UART_STOPBITS_1:    djybsp_uart[port].stop_bits = BK_STOP_BITS_1;break;
        case CN_UART_STOPBITS_2:    djybsp_uart[port].stop_bits = BK_STOP_BITS_2;break;
        default:break;
    }
    uart_hw_set_change(port,&djybsp_uart[port]);
}

// =============================================================================
// ����: �������ڷ��ͣ��������������ʹ��DMA��ʹ��DMA
// ����: Reg,�������Ĵ��ڼĴ���ָ��.
// ����: ���͵ĸ���
// =============================================================================
static u32 __UART_SendStart (ptu32_t port)
{

    u8 val[256];
    u32 i = 0,len = 0;
    if(port>CN_UART2)
       return 0;
    Board_UartHalfDuplexSend((u8)port);//�л�������
    uart_set_tx_fifo_needwr_int((u8)port,1);

    __UART_CloseInte((u8)port);
    uart_set_tx_stop_end_int((u8)port, 1);

    if(uart_is_tx_fifo_empty((u8)port) == 1)
    {
        len = UART_PortRead((struct UartGeneralCB *)pUartCB[port],val,256);
        if(0 != len)
        {
            for(i = 0; i < len; i++)
            {
                uart_write_byte(port,val[i]);
            }
        }

    }

    __UART_OpenInte((u8)port);
    return 1;
}

// =============================================================================
// ���ܣ�UART�ж�,��Ϊidle�жϣ����DMA�������ж������ݣ�����������DMA���������HAL�ж�
//       �����������ջ���õ�HAL_UART_XXXXCallback����
// ���������ں�
// ���أ�1=�ɹ���0=ʧ��
// =============================================================================
static void djybsp_uart_rx_isr(s32 port, void *param)
{
    uint8_t val = 0;
    uint8_t fifo[256];
    uint32_t fifo_status_reg = 0;
    u32 num = 0;

    if(port>CN_UART2)
        return ;

    if(UART1_PORT == port)
        fifo_status_reg = REG_UART1_FIFO_STATUS;
    else
        fifo_status_reg = REG_UART2_FIFO_STATUS;
    while(REG_READ(fifo_status_reg) & FIFO_RD_READY)
    {
        UART_READ_BYTE(port, val);
        fifo[num++] = val;
        if(num >= sizeof(fifo))
            break;
    }
    if(num > 0)
    {
        if(num != UART_PortWrite((struct UartGeneralCB *)pUartCB[port],fifo,num) && (port ==1))
        {
            UART_ErrHandle((struct UartGeneralCB *)pUartCB[port],CN_UART_BUF_OVER_ERR);
        }
    }
    return ;
}

static void djybsp_uart_tx_isr(s32 port, void *param)
{
    uint8_t val = 0;
    if(port>CN_UART2)
        return ;

    while(uart_is_tx_fifo_full(port)==0)
    {
        if(0 != UART_PortRead((struct UartGeneralCB *)pUartCB[port],&val,1))
        {
            uart_write_byte(port,val);
        }
        else
        {
            uart_set_tx_fifo_needwr_int(port,0);
            break;
        }
    }
    return ;
}

static void djybsp_uart_tx_end_isr(s32 port, void *param)
{
    if(port>CN_UART2)
        return ;

    Board_UartHalfDuplexRecv(port);
    return ;
}

void djybsp_uart_rx_over_isr(s32 port, void *param)
{
    if(port>CN_UART2)
        return ;
    UART_ErrHandle((struct UartGeneralCB *)pUartCB[port],CN_UART_FIFO_OVER_ERR);

    return ;
}
#pragma GCC diagnostic pop

// =============================================================================
// ����: ��ʼ��UART��Ӧ���ж��ߣ�����ʼ���ж���ں���
// ������SerialNo,���ں�
// ����: ��
// =============================================================================
static void __UART_IntInit(u32 port)
{
    if(port>CN_UART2)
       return ;
    uart_rx_callback_set(port, djybsp_uart_rx_isr, (void *)port);
    uart_tx_fifo_needwr_callback_set(port,djybsp_uart_tx_isr,(void *)port);
    uart_tx_end_callback_set(port,djybsp_uart_tx_end_isr,(void *)port);
    uart_rx_fifo_over_callback_set(port,djybsp_uart_rx_over_isr,(void *)port);
}

// =============================================================================
// ����: �����豸�Ŀ��ƺ���,������Ӳ���Ĵ����������
// ����: Reg,�������Ĵ��ڼĴ���ָ��.
//       cmd,��������
//       data1,data2,������cmd����
// ����: ������.
// =============================================================================
static ptu32_t __UART_Ctrl(uint8_t port,u32 cmd, va_list *arg0)
{
    ptu32_t result = 0;

    if(port>CN_UART2)
        return 0;

    switch(cmd)
    {
        case CN_DEV_CTRL_START:
            uart_reset(port);
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
            __UART_BaudSet(port, data);
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
            __UART_ComConfig(port,COM);
        }
            break;
        default: break;
    }
    return result;
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
            uart1_init();
            UART_SndBufLen = CFG_UART1_SENDBUF_LEN;
            UART_RxBufLen  = CFG_UART1_RECVBUF_LEN;
            break;
        case CN_UART2:
            uart2_init();
            UART_SndBufLen = CFG_UART2_SENDBUF_LEN;
            UART_RxBufLen  = CFG_UART2_RECVBUF_LEN;
            break;
        default:printk("ModuleInstall_UART:port Err\r\n");break;
    }

    if(port < CN_UART_NUM)
    {
        UART_Param.Name         = sUartName[port];
        UART_Param.UartPortTag  = (ptu32_t)sUartReg[port];
        UART_Param.Baud         = 115200;
        UART_Param.TxRingBufLen = UART_SndBufLen;
        UART_Param.RxRingBufLen = UART_RxBufLen;
        UART_Param.StartSend    = __UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;

        Board_UartHalfDuplexRecv(port);
        pUartCB[port] = UART_InstallGeneral(&UART_Param);
        if( pUartCB[port] != NULL)
        {
//            __UART_HardInit(port);              //Ӳ����ʼ��
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
    //bk_send_string(TxDirectPort,str);
    bk_send_string_len(TxDirectPort,str,len);
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
    while(1)
    {
        if( ((char)(result = uart_read_byte(RxDirectPort)))!=-1 )
        {
            break;
        }
        DJY_EventDelay(500);
    }
    return result;
}

__attribute__((weak)) void bk_printf(const char *fmt, ...)
{

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
    else
    {
        PutStrDirect = NULL ;
    }

    if(TxDirectPort != CN_LIMIT_UINT8)
    {
        __UART_HardInit(TxDirectPort);
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
    else
    {
        GetCharDirect = NULL ;
    }

    if((s32)RxDirectPort >= 0)
    {
        if(TxDirectPort != RxDirectPort)
            __UART_HardInit(RxDirectPort);
        GetCharDirect = Uart_GetCharDirect;
    }
    return;
}

#include "shell.h"
typedef void (*Ex_shell_func)(void);
void __asm_ExecuteCmd(union param *ptab,
       enum param_typr *pflag,Ex_shell_func fun,u32 num)
{

}

