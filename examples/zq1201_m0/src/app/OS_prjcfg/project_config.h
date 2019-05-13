/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"
#include "xip.h"
//*******************************  Configure System: lock  ******************************************//
#define CFG_LOCK_LIMIT          40         // "��������",�������������������ź����ͻ�����
//*******************************  Configure System: heap  ******************************************//
#define CFG_DYNAMIC_MEM true     // "ȫ���ܶ�̬����",��ʹѡfalse��Ҳ����ʹ��malloc-free�����ڴ棬��ʹ���в����� �������û��ֲᡷ�ڴ�����½�
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10         // "�豸����",�����豸����
//*******************************  Configure System: stdio  ******************************************//
#define     CFG_STDIO_RUN_MODE               (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW)   // "STDIOģʽ",
#define     CFG_STDIO_IN_NAME                "/dev/UART2"         // "��׼�����豸��",
#define     CFG_STDIO_OUT_NAME               "/dev/UART2"         // "��׼����豸��",
#define     CFG_STDIO_ERR_NAME               "/dev/UART2"         // "��׼err����豸��",
//*******************************  Configure cpu_peri_System: uart  ******************************************//
#define CFG_UART1_SENDBUF_LEN            32                    // "UART1���ͻ��λ�������С",
#define CFG_UART1_RECVBUF_LEN            32                    // "UART1���ջ��λ�������С",
#define CFG_UART1_DMABUF_LEN             32                    // "UART1 DMA��������С",
#define CFG_UART2_SENDBUF_LEN            32                    // "UART2���ͻ��λ�������С",
#define CFG_UART2_RECVBUF_LEN            32                    // "UART2���ջ��λ�������С",
#define CFG_UART2_DMABUF_LEN             32                    // "UART2 DMA��������С",
#define CFG_UART3_SENDBUF_LEN            32                    // "UART3���ͻ��λ�������С",
#define CFG_UART3_RECVBUF_LEN            32                    // "UART3���ջ��λ�������С",
#define CFG_UART3_DMABUF_LEN             32                    // "UART3 DMA��������С",
#define CFG_UART4_SENDBUF_LEN            32                    // "UART4���ͻ��λ�������С",
#define CFG_UART4_RECVBUF_LEN            32                    // "UART4���ջ��λ�������С",
#define CFG_UART4_DMABUF_LEN             32                    // "UART4 DMA��������С",
#define CFG_UART1_ENABLE                 false                  // "�Ƿ�ʹ��UART1",
#define CFG_UART2_ENABLE                 true                 // "�Ƿ�ʹ��UART2",
#define CFG_UART3_ENABLE                 false                 // "�Ƿ�ʹ��UART3",
#define CFG_UART4_ENABLE                 false                 // "�Ƿ�ʹ��UART4",
//*******************************  Configure shell  ******************************************//
#define     CFG_ADD_ROUTINE_SHELL            true                 // "�Ƿ���ӳ���shell����"
#define     CFG_ADD_EXPAND_SHELL             true                 // "�Ƿ������չshell����"
#define     CFG_ADD_GLOBAL_FUN               false                // "���ȫ�ֺ�����shell"
#define     CFG_SHOW_ADD_SHEELL              true                 // "��ʾ�ڱ��봰����ӵ�shell����"
//*******************************  Configure System: Software Timers  ******************************************//
#define CFG_TIMER_SOUCE     1          // "ʱ��Դ",1=��Ӳ����ʱ���ṩʱ��Դ��0=��tick�ṩʱ��Դ
#define CFG_TIMERS_LIMIT    5          // "��ʱ������",�ɴ����Ķ�ʱ��������������ͼ�ν���Ķ�ʱ����
//*******************************  Configure System: ymodem  ******************************************//
#define     CFG_YMODEM_BUF_NUM               32                   // "����ymodem������",ÿ��1024�ֽ�
#define     CFG_YMODEM_PKG_TIMEOUT           (15*1000*1000)       // "�������ʱʱ��",΢��
#define     CFG_YMODEM_TIMEOUT               (300*1000*1000)      // "ymodem�����ܳ�ʱʱ��",΢��
#define     CFG_YMODEM_DEVNAME               "std"                // "ymodem�����豸��std��ʾʹ�ñ�׼��������豸"
//#define CFG_YMODEM_TIMEOUT (600*1000*1000)
//*******************************  Configure System: kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096       // "��ʼ��ջ�ռ�",�����ʼ������ʹ�õ�ջ�ռ䣬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         10         // "�¼�������ֵ",�¼�����
#define CFG_EVENT_TYPE_LIMIT    10         // "�¼���������ֵ",�¼�������
#define CFG_MAINSTACK_LIMIT     4096       // "main����ջ�ߴ�",main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024       // "IDLE�¼�ջ�ߴ�",IDLE�¼����������е�ջ�ߴ磬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_OS_TINY             false      // "tiny���ں�",true=������Դ��ȱ�ĳ��ϣ��ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
//*******************************  Configure System: memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10         // "�ڴ������",������10���ڴ��
//*******************************  Configure System: loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN        // EN_DIRECT_RUN=ֱ�Ӵ�flash�����У�EN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL         // �Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define CFG_IBOOT_VERSION       01          // Iboot�����汾��
//*******************************  Configure cpu drive inner flash  ******************************************//
#define     CFG_EFLASH_PART_START            0x80                  // ������ʼ
#define     CFG_EFLASH_PART_END              -1                 // ��������
#define     CFG_EFLASH_PART_FORMAT           false              // ����ѡ��,�Ƿ���Ҫ��ʽ���÷�����
#define     CFG_EFLASH_FSMOUNT_NAME          EN_XIP_APP_TARGET  // �谲װ���ļ�ϵͳ��mount�����֣�NULL��ʾ��flash�������ļ�ϵͳ
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (12*Mhz)          //��Ƶ���ں�Ҫ�ã����붨��

#endif
