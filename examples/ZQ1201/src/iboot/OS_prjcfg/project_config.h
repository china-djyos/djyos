/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40               // "��������",�������������������ź����ͻ�����
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true           // "ȫ���ܶ�̬����",��ʹѡfalse��Ҳ����ʹ��malloc-free�����ڴ棬��ʹ���в����� �������û��ֲᡷ�ڴ�����½�
//*******************************  Configure DevFile  ******************************************//
#define CFG_DEVFILE_LIMIT       10               // "�豸����",�����豸����
//*******************************  Configure filesystem  ******************************************//
#define CFG_CLIB_BUFFERSIZE            512          // "C���ļ��û�̬�������ߴ�"
//*******************************  Configure stdio  ******************************************//
#define     CFG_STDIO_RUN_MODE               (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW) // "STDIOģʽ",
#define     CFG_STDIO_IN_NAME                "/dev/UART2"       // "��׼�����豸��",
#define     CFG_STDIO_OUT_NAME               "/dev/UART2"       // "��׼����豸��",
#define     CFG_STDIO_ERR_NAME               "/dev/UART2"       // "��׼err����豸��",
//*******************************  Configure cpu_peri_uart  ******************************************//
#define CFG_UART1_SENDBUF_LEN            32                          // "UART1���ͻ��λ�������С",
#define CFG_UART1_RECVBUF_LEN            32                          // "UART1���ջ��λ�������С",
#define CFG_UART1_DMABUF_LEN             32                          // "UART1 DMA��������С",
#define CFG_UART2_SENDBUF_LEN            32                          // "UART2���ͻ��λ�������С",
#define CFG_UART2_RECVBUF_LEN            32                          // "UART2���ջ��λ�������С",
#define CFG_UART2_DMABUF_LEN             32                          // "UART2 DMA��������С",
#define CFG_UART3_SENDBUF_LEN            32                          // "UART3���ͻ��λ�������С",
#define CFG_UART3_RECVBUF_LEN            32                          // "UART3���ջ��λ�������С",
#define CFG_UART3_DMABUF_LEN             32                          // "UART3 DMA��������С",
#define CFG_UART4_SENDBUF_LEN            32                          // "UART4���ͻ��λ�������С",
#define CFG_UART4_RECVBUF_LEN            32                          // "UART4���ջ��λ�������С",
#define CFG_UART4_DMABUF_LEN             32                          // "UART4 DMA��������С",
#define CFG_UART1_ENABLE                 false                        // "�Ƿ�ʹ��UART1",
#define CFG_UART2_ENABLE                 true                       // "�Ƿ�ʹ��UART2",
#define CFG_UART3_ENABLE                 false                       // "�Ƿ�ʹ��UART3",
#define CFG_UART4_ENABLE                 false                       // "�Ƿ�ʹ��UART4",
//*******************************  Configure timer  ******************************************//
#define     CFG_TIMER_SOUCE                  1                         // "ʱ��Դ",1=��Ӳ����ʱ���ṩʱ��Դ��0=��tick�ṩʱ��Դ
#define     CFG_TIMERS_LIMIT                 5                         // "��ʱ������",�ɴ����Ķ�ʱ��������������ͼ�ν���Ķ�ʱ����
//*******************************  Configure ymodem  ******************************************//
#define     CFG_YMODEM_BUF_NUM               32                    // "����ymodem������",ÿ��1024�ֽ�
#define     CFG_YMODEM_PKG_TIMEOUT           (15*1000*1000)        // "�������ʱʱ��",΢��
#define     CFG_YMODEM_TIMEOUT               (300*1000*1000)       // "ymodem�����ܳ�ʱʱ��",΢��
#define     CFG_YMODEM_DEVNAME               "std"                 // "ymodem�����豸��std��ʾʹ�ñ�׼��������豸"
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096             // "��ʼ��ջ�ռ�",�����ʼ������ʹ�õ�ջ�ռ䣬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         10               // "�¼�������ֵ",�¼�����
#define CFG_EVENT_TYPE_LIMIT    10               // "�¼���������ֵ",�¼�������
#define CFG_MAINSTACK_LIMIT     4096             // "main����ջ�ߴ�",main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024             // "IDLE�¼�ջ�ߴ�",IDLE�¼����������е�ջ�ߴ磬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_OS_TINY             false            // "tiny���ں�",true=������Դ��ȱ�ĳ��ϣ��ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
//*******************************  Configure MemoryPool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10               // "�ڴ������",������10���ڴ��
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8               // "ʱ��",����ʱ���Ƕ�8��
//*******************************  Configure loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN              // EN_DIRECT_RUN=ֱ�Ӵ�flash�����У�EN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define  CFG_IBOOT_CRC   EN_NO_APP_CRC               // �Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define CFG_APP_FILENAME        ""                       // CFG_APP_RUNMODE==EN_FORM_FILE�����������ļ���
#define CFG_IBOOT_VERSION       "V1.0.0"                 // Iboot�����汾��
#define CFG_IBOOT_UPDATE_NAME   "iboot.bin"              // iboot�����ļ������˶����ã��Ժ�ĵ�
#define CFG_IBOOT_UPDATE_PATH   "\\"              // iboot����·�������˶����ã��Ժ�ĵ�
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (12*Mhz)          //��Ƶ���ں�Ҫ�ã����붨��

#endif