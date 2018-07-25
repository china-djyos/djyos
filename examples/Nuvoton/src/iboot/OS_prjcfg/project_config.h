/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//******************************* Configure DevFile ******************************************//
#define CFG_DEVFILE_LIMIT       10      //�����豸����
//******************************* Configure kernel ******************************************//
#define CFG_INIT_STACK_SIZE     4096    //�����ʼ������ʹ�õ�ջ�ռ�һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         10      //�¼�����
#define CFG_EVENT_TYPE_LIMIT    10      //�¼�������
#define CFG_MAINSTACK_LIMIT     4096    //main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024    //IDLE�¼����������е�ջ�ߴ�һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_OS_TINY             false   //true=������Դ��ȱ�ĳ����ں˻�ü������¼����������¼�����ʱ��ͳ�ơ�
//******************************* Configure lock ******************************************//
#define CFG_LOCK_LIMIT          40      //�����������������ź����ͻ�����
//******************************* Configure MemoryPool ******************************************//
#define CFG_MEMPOOL_LIMIT       10      //������10���ڴ��
//******************************* Configure loader ******************************************//
#define  CFG_DJY_IBOOT_VERSION          "0.0.0"
#define  CFG_DJY_IBOOT_COMPILE_DATE     __DATE__
#define  CFG_DJY_IBOOT_COMPILE_TIME     __TIME__
#define  CFG_IBOOT_TYPE                 1//EN_DIRECT_RUN   //�Ƿ�ֱ�Ӵӳ���洢���м��أ���Mcuģʽ��
#define  CFG_IBOOT_CRC                  1//EN_USE_CRC      //�Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define  CFG_APP_UPDATE_NAME            ""
#define  CFG_APP_UPDATE_PATH            ""
#define  CFG_IBOOT_UPDATE_NAME          "libos_iboot_Debug.bin"
#define  CFG_IBOOT_UPDATE_FS
//******************************* Configure heap ******************************************//
#define CFG_DYNAMIC_MEM 1  //1 = ֧�ֶ�̬�ڴ���伴ʹ��֧��Ҳ����ʹ��malloc-free�����ڴ浫ʹ���в������������û��ֲᡷ�ڴ�����½�
//******************************* Configure stdio ******************************************//
#define CFG_STDIO_RUN_MODE            (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW) //
#define CFG_STDIO_IN_NAME              "/dev/UART0"//
#define CFG_STDIO_OUT_NAME             "/dev/UART0"//
#define CFG_STDIO_ERR_NAME             "/dev/UART0"//
//******************************* Configure time ******************************************//
#define CFG_LOCAL_TIMEZONE      8      //����ʱ���Ƕ�8��
//******************************* Configure cpu_peri_uart ******************************************//
#define CFG_UART0_SENDBUF_LEN            32                 //
#define CFG_UART0_RECVBUF_LEN            32                 //
#define CFG_UART0_DMABUF_LEN             32                 //
#define CFG_UART1_SENDBUF_LEN            32                 //
#define CFG_UART1_RECVBUF_LEN            32                 //
#define CFG_UART1_DMABUF_LEN             32                 //
#define CFG_UART2_SENDBUF_LEN            32                 //
#define CFG_UART2_RECVBUF_LEN            32                 //
#define CFG_UART2_DMABUF_LEN             32                 //
#define CFG_UART3_SENDBUF_LEN            32                 //
#define CFG_UART3_RECVBUF_LEN            32                 //
#define CFG_UART3_DMABUF_LEN             32                 //
#define CFG_UART4_SENDBUF_LEN            32                 //
#define CFG_UART4_RECVBUF_LEN            32                 //
#define CFG_UART4_DMABUF_LEN             32                 //
#define CFG_UART5_SENDBUF_LEN            32                 //
#define CFG_UART5_RECVBUF_LEN            32                 //
#define CFG_UART5_DMABUF_LEN             32                 //
#define CFG_UART0_ENABLE         		 true               //
#define CFG_UART1_ENABLE         		 false              //
#define CFG_UART2_ENABLE         		 false              //
#define CFG_UART3_ENABLE         		 false              //
#define CFG_UART4_ENABLE         		 false              //
#define CFG_UART5_ENABLE         		 false              //
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (192*Mhz)         //��Ƶ���ں�Ҫ�ã����붨��

#endif
