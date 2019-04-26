/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure devfile  ******************************************//
#define CFG_DEVFILE_LIMIT       10      //�����豸����
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40       // "��������",�������������������ź����ͻ�����
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true   // "ȫ���ܶ�̬����",��ʹѡfalse��Ҳ����ʹ��malloc-free�����ڴ棬��ʹ���в����� �������û��ֲᡷ�ڴ�����½�
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_FLOAT_PRINT       true        //
#define CFG_STDIO_STDIOFILE         true        //
#define     CFG_STDIO_RUN_MODE               (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW) // "STDIOģʽ",
#define     CFG_STDIO_IN_NAME                "/dev/UART1"       // "��׼�����豸��",
#define     CFG_STDIO_OUT_NAME               "/dev/UART1"       // "��׼����豸��",
#define     CFG_STDIO_ERR_NAME               "/dev/UART1"       // "��׼err����豸��",
//*******************************  Configure shell  ******************************************//
#define     CFG_ADD_ROUTINE_SHELL            true               // "�Ƿ���ӳ���shell����"
#define     CFG_ADD_EXPAND_SHELL             true               // "�Ƿ������չshell����"
#define     CFG_ADD_GLOBAL_FUN               false              // "���ȫ�ֺ�����shell"
#define     CFG_SHOW_ADD_SHEELL              true               // "��ʾ�ڱ��봰����ӵ�shell����"
#define  CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure timer  ******************************************//
#define CFG_TIMER_SOUCE     1        // "ʱ��Դ",1=��Ӳ����ʱ���ṩʱ��Դ��0=��tick�ṩʱ��Դ
#define CFG_TIMERS_LIMIT    5        // "��ʱ������",�ɴ����Ķ�ʱ��������������ͼ�ν���Ķ�ʱ����
//*******************************  Configure kernel  ******************************************//
#define     CFG_INIT_STACK_SIZE              4096               // "��ʼ��ջ�ռ�",�����ʼ������ʹ�õ�ջ�ռ䣬һ�㰴Ĭ��ֵ�Ϳ�����
#define     CFG_EVENT_LIMIT                  15                 // "�¼�������ֵ",�¼�����
#define     CFG_EVENT_TYPE_LIMIT             15                 // "�¼���������ֵ",�¼�������
#define     CFG_MAINSTACK_LIMIT              4096               // "main����ջ�ߴ�",main�������������ջ�ߴ�
#define     CFG_IDLESTACK_LIMIT              1024               // "IDLE�¼�ջ�ߴ�",IDLE�¼����������е�ջ�ߴ磬һ�㰴Ĭ��ֵ�Ϳ�����
#define     CFG_OS_TINY                      false              // "tiny���ں�",true=������Դ��ȱ�ĳ��ϣ��ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
//*******************************  Configure Objfile  ******************************************//
#define CFG_OBJECT_LIMIT        8   //������Զ�����
#define CFG_HANDLE_LIMIT        8   //������Զ�����
//*******************************  Configure MemoryPool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10       // "�ڴ��������ֵ",
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8       // "ʱ��",����ʱ���Ƕ�8��
//*******************************  Configure loader  ******************************************//
#define     CFG_APP_RUNMODE                  EN_DIRECT_RUN      // EN_DIRECT_RUN=ֱ�Ӵ�flash�����У�EN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define     CFG_APP_VERIFICATION             VERIFICATION_NULL  // �Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define     CFG_IBOOT_VERSION                01                 // Iboot�����汾��
#define CFG_IBOOT_UPDATE_NAME      "/yaf2/iboot.bin"           //
//*******************************  Configure cpu_peri_uart  ******************************************//
#define     CFG_UART_SENDBUF_LEN             32                 // "���ڷ��ͻ��λ�������С",
#define     CFG_UART_RECVBUF_LEN             32                 // "���ڽ��ջ��λ�������С",
#define     CFG_UART1_ENABLE                 true               // "�����Ƿ�ʹ��UART1",
#define     CFG_UART2_ENABLE                 false              // "�����Ƿ�ʹ��UART2",
#define     CFG_UART3_ENABLE                 false              // "�����Ƿ�ʹ��UART3",
#define     CFG_UART4_ENABLE                 false              // "�����Ƿ�ʹ��UART4",
#define     CFG_UART5_ENABLE                 false              // "�����Ƿ�ʹ��UART5",
#define     CFG_UART6_ENABLE                 false              // "�����Ƿ�ʹ��UART6",
#define     CFG_UART7_ENABLE                 false              // "�����Ƿ�ʹ��UART7",
#define     CFG_UART8_ENABLE                 false              // "�����Ƿ�ʹ��UART8",
//*******************************  Configure tpl  ******************************************//
#define CFG_TCP_REORDER  false
#define     CFG_TPL_PROTONUM            5       //ռ��һ�� tagTplProtoItem �ṹ
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (400*Mhz)         //��Ƶ���ں�Ҫ�ã����붨��

#endif
