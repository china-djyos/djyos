/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure System: lock  ******************************************//
#define CFG_LOCK_LIMIT          40        // "��������",�������������������ź����ͻ�����
//*******************************  Configure System: heap  ******************************************//
#define CFG_DYNAMIC_MEM true    // "ȫ���ܶ�̬����",��ʹѡfalse��Ҳ����ʹ��malloc-free�����ڴ棬��ʹ���в����� �������û��ֲᡷ�ڴ�����½�
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10        // "�豸����",�����豸����
//*******************************  Configure System: stdio  ******************************************//
#define CFG_STDIO_FLOAT_PRINT       true         // "֧�ָ����ӡ"
#define CFG_STDIO_STDIOFILE         true         // "֧�ֱ�׼IO�ļ�"
#define CFG_STDIO_RUN_MODE            (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW)   // "STDIOģʽ",
#define CFG_STDIO_IN_NAME              "/dev/UART1"      // "��׼�����豸��",
#define CFG_STDIO_OUT_NAME             "/dev/UART1"      // "��׼����豸��",
#define CFG_STDIO_ERR_NAME             "/dev/UART1"      // "��׼err����豸��",
//*******************************  Configure System: kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096      // "��ʼ��ջ�ռ�",�����ʼ������ʹ�õ�ջ�ռ䣬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         15        // "�¼�������ֵ",�¼�����
#define CFG_EVENT_TYPE_LIMIT    15        // "�¼���������ֵ",�¼�������
#define CFG_MAINSTACK_LIMIT     4096      // "main����ջ�ߴ�",main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024      // "IDLE�¼�ջ�ߴ�",IDLE�¼����������е�ջ�ߴ磬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_OS_TINY             false     // "tiny���ں�",true=������Դ��ȱ�ĳ��ϣ��ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8    // "��������ʼֵ"��������Զ�����
#define CFG_HANDLE_LIMIT        8    // "�������ʼֵ"��������Զ�����
//*******************************  Configure System: memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10        // "�ڴ��������ֵ",
//*******************************  Configure System: loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN       // EN_DIRECT_RUN=ֱ�Ӵ�flash�����У�EN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL        // �Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define CFG_IBOOT_VERSION       01          // Iboot�����汾��
#define CFG_IBOOT_UPDATE_NAME      "/yaf2/iboot.bin"            // "Iboot����"
//*******************************  Configure cpu_peri_System: uart  ******************************************//
#define CFG_UART1_SENDBUF_LEN       64        // "UART1���ͻ��λ�������С",
#define CFG_UART1_RECVBUF_LEN       64        // "UART1���ջ��λ�������С",
#define CFG_UART1_DMABUF_LEN        64        // "UART1 DMA���λ�������С",
#define CFG_UART2_SENDBUF_LEN       64        // "UART2���ͻ��λ�������С",
#define CFG_UART2_RECVBUF_LEN       64        // "UART2���ջ��λ�������С",
#define CFG_UART2_DMABUF_LEN        64        // "UART2 DMA���λ�������С",
#define CFG_UART3_SENDBUF_LEN       64        // "UART3���ͻ��λ�������С",
#define CFG_UART3_RECVBUF_LEN       64        // "UART3���ջ��λ�������С",
#define CFG_UART3_DMABUF_LEN        64        // "UART3 DMA���λ�������С",
#define CFG_UART4_SENDBUF_LEN       64        // "UART4���ͻ��λ�������С",
#define CFG_UART4_RECVBUF_LEN       64        // "UART4���ջ��λ�������С",
#define CFG_UART4_DMABUF_LEN        64        // "UART4 DMA���λ�������С",
#define CFG_UART5_SENDBUF_LEN       64        // "UART5���ͻ��λ�������С",
#define CFG_UART5_RECVBUF_LEN       64        // "UART5���ջ��λ�������С",
#define CFG_UART5_DMABUF_LEN        64        // "UART5 DMA���λ�������С",
#define CFG_UART6_SENDBUF_LEN       64        // "UART6���ͻ��λ�������С",
#define CFG_UART6_RECVBUF_LEN       64        // "UART6���ջ��λ�������С",
#define CFG_UART6_DMABUF_LEN        64        // "UART6 DMA���λ�������С",
#define CFG_UART7_SENDBUF_LEN       64        // "UART7���ͻ��λ�������С",
#define CFG_UART7_RECVBUF_LEN       64        // "UART7���ջ��λ�������С",
#define CFG_UART7_DMABUF_LEN        64        // "UART7 DMA���λ�������С",
#define CFG_UART8_SENDBUF_LEN       64        // "UART8���ͻ��λ�������С",
#define CFG_UART8_RECVBUF_LEN       64        // "UART8���ջ��λ�������С",
#define CFG_UART8_DMABUF_LEN        64        // "UART8 DMA���λ�������С",
#define CFG_UART1_ENABLE           true          // "�Ƿ�ʹ��UART1",
#define CFG_UART2_ENABLE           false         // "�Ƿ�ʹ��UART2",
#define CFG_UART3_ENABLE           false         // "�Ƿ�ʹ��UART3",
#define CFG_UART4_ENABLE           false         // "�Ƿ�ʹ��UART4",
#define CFG_UART5_ENABLE           false         // "�Ƿ�ʹ��UART5",
#define CFG_UART6_ENABLE           false         // "�Ƿ�ʹ��UART6",
#define CFG_UART7_ENABLE           false         // "�Ƿ�ʹ��UART7",
#define CFG_UART8_ENABLE           false         // "�Ƿ�ʹ��UART8",
//*******************************  Configure shell  ******************************************//
#define    CFG_ADD_ROUTINE_SHELL           true              //
#define    CFG_ADD_EXPAND_SHELL            true              //
#define    CFG_ADD_GLOBAL_FUN              false             //
#define    CFG_SHOW_ADD_SHEELL             true              //
#define  CFG_MODULE_ENABLE_SHELL         true
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (216*Mhz)         //��Ƶ���ں�Ҫ�ã����붨��

#endif