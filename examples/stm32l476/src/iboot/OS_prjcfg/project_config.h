/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40        // "��������",�������������������ź����ͻ�����
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true    // "ȫ���ܶ�̬����",��ʹѡfalse��Ҳ����ʹ��malloc-free�����ڴ棬��ʹ���в����� �������û��ֲᡷ�ڴ�����½�
//*******************************  Configure stdio  ******************************************//
#define     CFG_STDIO_RUN_MODE               (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW)//
#define     CFG_STDIO_IN_NAME                "/dev/USART2"     //
#define     CFG_STDIO_OUT_NAME               "/dev/USART2"     //
#define     CFG_STDIO_ERR_NAME               "/dev/USART2"     //
//*******************************  Configure cpu_peri_uart  ******************************************//
#define     CFG_UART_SENDBUF_LEN             256               //
#define     CFG_UART_RECVBUF_LEN             256               //
#define     CFG_USART1_ENABLE                false             //
#define     CFG_USART2_ENABLE                true              //
#define     CFG_USART3_ENABLE                false             //
#define     CFG_UART4_ENABLE                 false             //
#define     CFG_UART5_ENABLE                 false             //
#define     CFG_LPUART1_ENABLE               false             //
//*******************************  Configure wdt  ******************************************//
#define     CFG_WDT_LIMIT                    10                  // "���Ź�����",������������
#define     CFG_WDTMSG_LIMIT                 3                   // "��Ϣ���г���"���������Ź�����Ϣ���е���󳤶�
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096      // "��ʼ��ջ�ռ�",�����ʼ������ʹ�õ�ջ�ռ䣬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         10        // "�¼�������ֵ",�¼�����
#define CFG_EVENT_TYPE_LIMIT    10        // "�¼���������ֵ",�¼�������
#define CFG_MAINSTACK_LIMIT     4096      // "main����ջ�ߴ�",main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024      // "IDLE�¼�ջ�ߴ�",IDLE�¼����������е�ջ�ߴ磬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_OS_TINY             false     // "tiny���ں�",true=������Դ��ȱ�ĳ��ϣ��ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
//*******************************  Configure MemoryPool  ******************************************//
#define     CFG_MEMPOOL_LIMIT                10                  // "�ڴ������",������10���ڴ��
//*******************************  Configure loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN       // EN_DIRECT_RUN=ֱ�Ӵ�flash�����У�EN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define  CFG_IBOOT_CRC   EN_NO_APP_CRC        // �Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define CFG_APP_FILENAME        ""                // CFG_APP_RUNMODE==EN_FORM_FILE�����������ļ���
#define CFG_IBOOT_VERSION       "V1.0.0"          // Iboot�����汾��
#define CFG_IBOOT_UPDATE_NAME   "iboot.bin"       // iboot�����ļ������˶����ã��Ժ�ĵ�
#define CFG_IBOOT_UPDATE_PATH   "\\"       // iboot����·�������˶����ã��Ժ�ĵ�
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (72*Mhz)          //��Ƶ���ں�Ҫ�ã����붨��

#endif
