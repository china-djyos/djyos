/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40       // "��������",�������������������ź����ͻ�����
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true   // "ȫ���ܶ�̬����",��ʹѡfalse��Ҳ����ʹ��malloc-free�����ڴ棬��ʹ���в����� �������û��ֲᡷ�ڴ�����½�
//*******************************  Configure DevFile  ******************************************//
#define CFG_DEVFILE_LIMIT       10       // "�豸����",�����豸����
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_RUN_MODE            (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW)  // "STDIOģʽ",
#define CFG_STDIO_IN_NAME              "/dev/UART1" // "�����豸��",
#define CFG_STDIO_OUT_NAME             "/dev/UART1" // "����豸��",
#define CFG_STDIO_ERR_NAME             "/dev/UART1" // "err����豸��",
//*******************************  Configure cpu_peri_uart  ******************************************//
#define CFG_UART1_SENDBUF_LEN            32                  // "UART1���ͻ��λ�������С",
#define CFG_UART1_RECVBUF_LEN            32                  // "UART1���ջ��λ�������С",
#define CFG_UART1_DMABUF_LEN             32                  // "UART1 DMA��������С",
#define CFG_UART2_SENDBUF_LEN            32                  // "UART2���ͻ��λ�������С",
#define CFG_UART2_RECVBUF_LEN            32                  // "UART2���ջ��λ�������С",
#define CFG_UART2_DMABUF_LEN             32                  // "UART2 DMA��������С",
#define CFG_UART3_SENDBUF_LEN            32                  // "UART3���ͻ��λ�������С",
#define CFG_UART3_RECVBUF_LEN            32                  // "UART3���ջ��λ�������С",
#define CFG_UART3_DMABUF_LEN             32                  // "UART3 DMA��������С",
#define CFG_UART4_SENDBUF_LEN            32                   // "UART4���ͻ��λ�������С",
#define CFG_UART4_RECVBUF_LEN            32                   // "UART4���ջ��λ�������С",
#define CFG_UART5_SENDBUF_LEN            32                    // "UART5���ͻ��λ�������С",
#define CFG_UART5_RECVBUF_LEN            32                    // "UART5���ջ��λ�������С",
#define CFG_UART1_ENABLE                 true                  // "�Ƿ�����ʹ��UART1",
#define CFG_UART2_ENABLE                 false                 // "�Ƿ�����ʹ��UART2",
#define CFG_UART3_ENABLE                 false                 // "�Ƿ�����ʹ��UART3",
#define CFG_UART4_ENABLE                 false                 // "�Ƿ�����ʹ��UART4",
#define CFG_UART5_ENABLE                 false                 // "�Ƿ�����ʹ��UART5",
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096     // "��ʼ��ջ�ռ�",�����ʼ������ʹ�õ�ջ�ռ䣬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         10       // "�¼�������ֵ",�¼�����
#define CFG_EVENT_TYPE_LIMIT    10       // "�¼���������ֵ",�¼�������
#define CFG_MAINSTACK_LIMIT     4096     // "main����ջ�ߴ�",main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024     // "IDLE�¼�ջ�ߴ�",IDLE�¼������������е�ջ�ߴ磬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_OS_TINY             false    // "tiny���ں�",true=������Դ��ȱ�ĳ��ϣ��ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
//*******************************  Configure MemoryPool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10       // "�ڴ������",��������10���ڴ��
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8       // "ʱ��",����ʱ���Ƕ�8��
//*******************************  Configure loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN      // EN_DIRECT_RUN=ֱ�Ӵ�flash�����У�EN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define  CFG_IBOOT_CRC   EN_NO_APP_CRC       // �Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define CFG_APP_FILENAME        ""               // CFG_APP_RUNMODE==EN_FORM_FILE�����������ļ���
#define CFG_IBOOT_VERSION       "V1.0.0"         // Iboot�����汾��
#define CFG_IBOOT_UPDATE_NAME   "iboot.bin"      // iboot�����ļ������˶����ã��Ժ�ĵ�
#define CFG_IBOOT_UPDATE_PATH   "\\"      // iboot����·�������˶����ã��Ժ�ĵ�
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (168*Mhz)         //��Ƶ���ں�Ҫ�ã����붨��

#endif