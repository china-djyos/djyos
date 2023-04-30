#include "board-config.h"
#include <stdio.h>
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
//#include "cpu_peri_dma.h"
// =============================================================================
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
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip uart"//CPU��uart��������
//parent:"uart device file"        //��д������ĸ�������֣�none��ʾû�и����
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
//mutex:"none"                     //������Ļ����������������none����ʾ�޻����������
                                   //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue  	****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_UART == false )
//#warning  " cpu_onchip_uart  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header   		//header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_UART    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,512,
#define CFG_UART1_SENDBUF_LEN       64      //"UART1���ͻ��λ�������С",
#define CFG_UART1_RECVBUF_LEN       64      //"UART1���ջ��λ�������С",
#define CFG_UART1_DMABUF_LEN        64      //"UART1 DMA���λ�������С",

#define CFG_UART2_SENDBUF_LEN       64      //"UART2���ͻ��λ�������С",
#define CFG_UART2_RECVBUF_LEN       64      //"UART2���ջ��λ�������С",
#define CFG_UART2_DMABUF_LEN        64      //"UART2 DMA���λ�������С",

#define CFG_UART3_SENDBUF_LEN       64      //"UART3���ͻ��λ�������С",
#define CFG_UART3_RECVBUF_LEN       64      //"UART3���ջ��λ�������С",
#define CFG_UART3_DMABUF_LEN        64      //"UART3 DMA���λ�������С",

#define CFG_UART4_SENDBUF_LEN       64      //"UART4���ͻ��λ�������С",
#define CFG_UART4_RECVBUF_LEN       64      //"UART4���ջ��λ�������С",
#define CFG_UART4_DMABUF_LEN        64      //"UART4 DMA���λ�������С",

#define CFG_UART5_SENDBUF_LEN       64      //"UART5���ͻ��λ�������С",
#define CFG_UART5_RECVBUF_LEN       64      //"UART5���ջ��λ�������С",
#define CFG_UART5_DMABUF_LEN        64      //"UART5 DMA���λ�������С",

#define CFG_UART6_SENDBUF_LEN       64      //"UART6���ͻ��λ�������С",
#define CFG_UART6_RECVBUF_LEN       64      //"UART6���ջ��λ�������С",
#define CFG_UART6_DMABUF_LEN        64      //"UART6 DMA���λ�������С",

#define CFG_UART7_SENDBUF_LEN       64      //"UART7���ͻ��λ�������С",
#define CFG_UART7_RECVBUF_LEN       64      //"UART7���ջ��λ�������С",
#define CFG_UART7_DMABUF_LEN        64      //"UART7 DMA���λ�������С",

#define CFG_UART8_SENDBUF_LEN       64      //"UART8���ͻ��λ�������С",
#define CFG_UART8_RECVBUF_LEN       64      //"UART8���ջ��λ�������С",
#define CFG_UART8_DMABUF_LEN        64      //"UART8 DMA���λ�������С",

//%$#@enum,true,false
#define CFG_UART1_ENABLE           true        //"�Ƿ�ʹ��UART1",
#define CFG_UART1_ENABLE_DMA       false       //"UART1ʹ��DMA",
#define CFG_UART2_ENABLE           false       //"�Ƿ�ʹ��UART2",
#define CFG_UART2_ENABLE_DMA       false       //"UART2ʹ��DMA",
#define CFG_UART3_ENABLE           false       //"�Ƿ�ʹ��UART3",
#define CFG_UART3_ENABLE_DMA       false       //"UART3ʹ��DMA",
#define CFG_UART4_ENABLE           false       //"�Ƿ�ʹ��UART4",
#define CFG_UART4_ENABLE_DMA       false       //"UART4ʹ��DMA",
#define CFG_UART5_ENABLE           false       //"�Ƿ�ʹ��UART5",
#define CFG_UART5_ENABLE_DMA       false       //"UART5ʹ��DMA",
#define CFG_UART6_ENABLE           false       //"�Ƿ�ʹ��UART6",
#define CFG_UART6_ENABLE_DMA       false       //"UART6ʹ��DMA",
#define CFG_UART7_ENABLE           false       //"�Ƿ�ʹ��UART7",
#define CFG_UART7_ENABLE_DMA       false       //"UART7ʹ��DMA",
#define CFG_UART8_ENABLE           false       //"�Ƿ�ʹ��UART8",
#define CFG_UART8_ENABLE_DMA       false       //"UART8ʹ��DMA",
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������
//@#$%component end configure
// =============================================================================

//#define CFG_UART0_SENDBUF_LEN 		   256
//#define CFG_UART0_RECVBUF_LEN 		   256
//
//#define CFG_UART1_SENDBUF_LEN 		   256
//#define CFG_UART1_RECVBUF_LEN 		   256
//
//#define CFG_UART2_SENDBUF_LEN 		   256
//#define CFG_UART2_RECVBUF_LEN 		   256

static u32 imxStartSend(ptu32_t PrivateTag)
{
	return 0;
}

static ptu32_t imxUartCtrl(ptu32_t PrivateTag,u32 cmd, va_list *arg0)
{
	return 0;
}

struct imxUartPort {
	int port;
	struct UartGeneralCB *UartGeneralCB;
};

static struct imxUartPort imxUartPort[1] = {
	[0] = {
		.port = 0,
		.UartGeneralCB = NULL,
	},
};

static struct UartParam imxUartParam[1] = {
	[0] = {

		.Name          = "UART0",
		.TxRingBufLen  = 256,
		.RxRingBufLen  = 256,
		.Baud          = 115200,
		.mode          = CN_UART_GENERAL,
		.UartPortTag   = 0,
		.StartSend     = imxStartSend,
		.UartCtrl      = imxUartCtrl,
	},
};

ptu32_t ModuleInstall_UART(ptu32_t serial_no)
{
	struct UartParam *Param;
	struct imxUartPort *up;
	up->UartGeneralCB = UART_InstallGeneral(Param);
}

s32 imxPutStrDirect(const char *buf,u32 len)
{
    int c;
    while (len) {
        c = *buf++;
	/* write c to hardware. */
        len--;
    }
}
char imxGetCharDirect(void)
{
    int c;
    c = 'C'; /* read from hardware. */
    return c;
}

void Stdio_KnlInOutInit(char * StdioIn, char *StdioOut)
{
	PutStrDirect = imxPutStrDirect;
	GetCharDirect = imxGetCharDirect;
}

