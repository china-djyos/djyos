/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40         // "��������",�������������������ź����ͻ�����
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true     // "ȫ���ܶ�̬����",��ʹѡfalse��Ҳ����ʹ��malloc-free�����ڴ棬��ʹ���в����� �������û��ֲᡷ�ڴ�����½�
//*******************************  Configure DevFile  ******************************************//
#define CFG_DEVFILE_LIMIT       10         // "�豸����",�����豸����
//*******************************  Configure stdio  ******************************************//
#define     CFG_STDIO_RUN_MODE               (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW)//
#define     CFG_STDIO_IN_NAME                "/dev/UART2"      //
#define     CFG_STDIO_OUT_NAME               "/dev/UART2"      //
#define     CFG_STDIO_ERR_NAME               "/dev/UART2"      //
//*******************************  Configure cpu_peri_uart  ******************************************//
#define     CFG_UART1_SENDBUF_LEN            32                //
#define     CFG_UART1_RECVBUF_LEN            32                //
#define     CFG_UART1_DMABUF_LEN             32                //
#define     CFG_UART2_SENDBUF_LEN            32                //
#define     CFG_UART2_RECVBUF_LEN            32                //
#define     CFG_UART2_DMABUF_LEN             32                //
#define     CFG_UART3_SENDBUF_LEN            32                //
#define     CFG_UART3_RECVBUF_LEN            32                //
#define     CFG_UART3_DMABUF_LEN             32                //
#define     CFG_UART4_SENDBUF_LEN            32                //
#define     CFG_UART4_RECVBUF_LEN            32                //
#define     CFG_UART4_DMABUF_LEN             32                //
#define     CFG_UART5_SENDBUF_LEN            32                //
#define     CFG_UART5_RECVBUF_LEN            32                //
#define     CFG_UART5_DMABUF_LEN             32                //
#define     CFG_UART6_SENDBUF_LEN            32                //
#define     CFG_UART6_RECVBUF_LEN            32                //
#define     CFG_UART6_DMABUF_LEN             32                //
#define     CFG_UART7_SENDBUF_LEN            32                //
#define     CFG_UART7_RECVBUF_LEN            32                //
#define     CFG_UART7_DMABUF_LEN             32                //
#define     CFG_UART8_SENDBUF_LEN            32                //
#define     CFG_UART8_RECVBUF_LEN            32                //
#define     CFG_UART8_DMABUF_LEN             32                //
#define     CFG_UART1_ENABLE                 false             //
#define     CFG_UART2_ENABLE                 true              //
#define     CFG_UART3_ENABLE                 false             //
#define     CFG_UART4_ENABLE                 false             //
#define     CFG_UART5_ENABLE                 false             //
#define     CFG_UART6_ENABLE                 false             //
#define     CFG_UART7_ENABLE                 false             //
#define     CFG_UART8_ENABLE                 false             //
//*******************************  Configure shell  ******************************************//
#define     CFG_ADD_ROUTINE_SHELL            true                 // "�Ƿ���ӳ���shell����"
#define     CFG_ADD_EXPAND_SHELL             true                 // "�Ƿ������չshell����"
#define     CFG_ADD_GLOBAL_FUN               false                // "���ȫ�ֺ�����shell"
#define     CFG_SHOW_ADD_SHEELL              true                 // "��ʾ�ڱ��봰����ӵ�shell����"
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096       // "��ʼ��ջ�ռ�",�����ʼ������ʹ�õ�ջ�ռ䣬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         15         // "�¼�������ֵ",�¼�����
#define CFG_EVENT_TYPE_LIMIT    15         // "�¼���������ֵ",�¼�������
#define CFG_MAINSTACK_LIMIT     4096       // "main����ջ�ߴ�",main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024       // "IDLE�¼�ջ�ߴ�",IDLE�¼����������е�ջ�ߴ磬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_OS_TINY             false      // "tiny���ں�",true=������Դ��ȱ�ĳ��ϣ��ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
//*******************************  Configure MemoryPool  ******************************************//
#define     CFG_MEMPOOL_LIMIT                10                //������10���ڴ��
//*******************************  Configure IBOOT  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN        // EN_DIRECT_RUN=ֱ�Ӵ�flash�����У�EN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL         // �Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define CFG_IBOOT_VERSION       01           // Iboot�����汾��
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (150*Mhz)         //��Ƶ���ں�Ҫ�ã����붨��

#endif