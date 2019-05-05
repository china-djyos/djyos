/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

#define CFG_RUNMODE_BAREAPP    1
//*******************************  Configure devfile  ******************************************//
#define    CFG_DEVFILE_LIMIT               10                //�����豸����
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40                // "��������",�������������������ź����ͻ�����
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true            // "ȫ���ܶ�̬����",��ʹѡfalse��Ҳ����ʹ��malloc-free�����ڴ棬��ʹ���в����� �������û��ֲᡷ�ڴ�����½�
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_FLOAT_PRINT       true               // "֧�ָ����ӡ"
#define CFG_STDIO_STDIOFILE         true               // "֧�ֱ�׼IO�ļ�"
#define     CFG_STDIO_RUN_MODE               (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW)          // "STDIOģʽ",
#define     CFG_STDIO_IN_NAME                "/dev/UART1"                // "��׼�����豸��",
#define     CFG_STDIO_OUT_NAME               "/dev/UART1"                // "��׼����豸��",
#define     CFG_STDIO_ERR_NAME               "/dev/UART1"                // "��׼err����豸��",
//*******************************  Configure shell  ******************************************//
#define CFG_SHELL_STACK            0x1000             // "ִ��shell�����ջ�ߴ�"
#define     CFG_ADD_ROUTINE_SHELL            true                      // "�Ƿ���ӳ���shell����"
#define     CFG_ADD_EXPAND_SHELL             true                      // "�Ƿ������չshell����"
#define     CFG_ADD_GLOBAL_FUN               false                     // "���ȫ�ֺ�����shell"
#define     CFG_SHOW_ADD_SHEELL              true                      // "��ʾ�ڱ��봰����ӵ�shell����"
#define  CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure timer  ******************************************//
#define CFG_TIMER_SOUCE     1                 // "ʱ��Դ",1=��Ӳ����ʱ���ṩʱ��Դ��0=��tick�ṩʱ��Դ
#define CFG_TIMERS_LIMIT    5                 // "��ʱ������",�ɴ����Ķ�ʱ��������������ͼ�ν���Ķ�ʱ����
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096              // "��ʼ��ջ�ռ�",�����ʼ������ʹ�õ�ջ�ռ䣬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         15                // "�¼�������ֵ",�¼�����
#define CFG_EVENT_TYPE_LIMIT    15                // "�¼���������ֵ",�¼�������
#define CFG_MAINSTACK_LIMIT     4096              // "main����ջ�ߴ�",main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024              // "IDLE�¼�ջ�ߴ�",IDLE�¼����������е�ջ�ߴ磬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_OS_TINY             false             // "tiny���ں�",true=������Դ��ȱ�ĳ��ϣ��ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
//*******************************  Configure Objfile  ******************************************//
#define CFG_OBJECT_LIMIT        8          // "��������ʼֵ"��������Զ�����
#define CFG_HANDLE_LIMIT        8          // "�������ʼֵ"��������Զ�����
//*******************************  Configure MemoryPool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10                // "�ڴ��������ֵ",
//*******************************  Configure cpu_peri_uart  ******************************************//
#define     CFG_UART1_SENDBUF_LEN            32                          // "UART1���ͻ��λ�������С",
#define     CFG_UART1_RECVBUF_LEN            32                          // "UART1���ջ��λ�������С",
#define     CFG_UART1_DMABUF_LEN             32                          // "UART1 DMA��������С",
#define     CFG_UART2_SENDBUF_LEN            32                          // "UART2���ͻ��λ�������С",
#define     CFG_UART2_RECVBUF_LEN            32                          // "UART2���ջ��λ�������С",
#define     CFG_UART2_DMABUF_LEN             32                          // "UART2 DMA��������С",
#define     CFG_UART3_SENDBUF_LEN            32                          // "UART3���ͻ��λ�������С",
#define     CFG_UART3_RECVBUF_LEN            32                          // "UART3���ջ��λ�������С",
#define     CFG_UART3_DMABUF_LEN             32                          // "UART3 DMA��������С",
#define     CFG_UART4_SENDBUF_LEN            32                          // "UART4���ͻ��λ�������С",
#define     CFG_UART4_RECVBUF_LEN            32                          // "UART4���ջ��λ�������С",
#define     CFG_UART4_DMABUF_LEN             32                          // "UART4 DMA��������С",
#define     CFG_UART5_SENDBUF_LEN            32                          // "UART5���ͻ��λ�������С",
#define     CFG_UART5_RECVBUF_LEN            32                          // "UART5���ջ��λ�������С",
#define     CFG_UART5_DMABUF_LEN             32                          // "UART5 DMA��������С",
#define     CFG_UART6_SENDBUF_LEN            32                          // "UART6���ͻ��λ�������С",
#define     CFG_UART6_RECVBUF_LEN            32                          // "UART6���ջ��λ�������С",
#define     CFG_UART6_DMABUF_LEN             32                          // "UART6 DMA��������С",
#define     CFG_UART1_ENABLE                 true                        // "�Ƿ�ʹ��UART1",
#define     CFG_UART2_ENABLE                 false                       // "�Ƿ�ʹ��UART2",
#define     CFG_UART3_ENABLE                 false                       // "�Ƿ�ʹ��UART3",
#define     CFG_UART4_ENABLE                 false                       // "�Ƿ�ʹ��UART4",
#define     CFG_UART5_ENABLE                 false                       // "�Ƿ�ʹ��UART5",
#define     CFG_UART6_ENABLE                 false                       // "�Ƿ�ʹ��UART6",
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (168*Mhz)         //��Ƶ���ں�Ҫ�ã����붨��

#endif
