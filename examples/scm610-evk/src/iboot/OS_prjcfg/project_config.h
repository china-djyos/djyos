/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure component lock  ******************************************//
#define CFG_LOCK_LIMIT          40          // "��������",�������������������ź����ͻ�����
//*******************************  Configure component heap  ******************************************//
#define CFG_DYNAMIC_MEM true      // "ȫ���ܶ�̬����",��ʹѡfalse��Ҳ����ʹ��malloc-free�����ڴ棬��ʹ���в����� �������û��ֲᡷ�ڴ�����½�
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10          // "�豸����",�����豸����
//*******************************  Configure component stdio  ******************************************//
#define     CFG_STDIO_RUN_MODE               (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW) // "STDIOģʽ",
#define     CFG_STDIO_IN_NAME                "/dev/UART2"       // "��׼�����豸��",
#define     CFG_STDIO_OUT_NAME               "/dev/UART2"       // "��׼����豸��",
#define     CFG_STDIO_ERR_NAME               "/dev/UART2"       // "��׼err����豸��",
//*******************************  Configure cpu_peri_component uart  ******************************************//
#define     CFG_UART1_SENDBUF_LEN            32                 // "UART1���ͻ��λ�������С",
#define     CFG_UART1_RECVBUF_LEN            32                 // "UART1���ջ��λ�������С",
#define     CFG_UART1_DMABUF_LEN             32                 // "UART1 DMA��������С",
#define     CFG_UART2_SENDBUF_LEN            32                 // "UART2���ͻ��λ�������С",
#define     CFG_UART2_RECVBUF_LEN            32                 // "UART2���ջ��λ�������С",
#define     CFG_UART2_DMABUF_LEN             32                 // "UART2 DMA��������С",
#define     CFG_UART3_SENDBUF_LEN            32                 // "UART3���ͻ��λ�������С",
#define     CFG_UART3_RECVBUF_LEN            32                 // "UART3���ջ��λ�������С",
#define     CFG_UART3_DMABUF_LEN             32                 // "UART3 DMA��������С",
#define     CFG_UART4_SENDBUF_LEN            32                 // "UART4���ͻ��λ�������С",
#define     CFG_UART4_RECVBUF_LEN            32                 // "UART4���ջ��λ�������С",
#define     CFG_UART4_DMABUF_LEN             32                 // "UART4 DMA��������С",
#define     CFG_UART5_SENDBUF_LEN            32                 // "UART5���ͻ��λ�������С",
#define     CFG_UART5_RECVBUF_LEN            32                 // "UART5���ջ��λ�������С",
#define     CFG_UART5_DMABUF_LEN             32                 // "UART5 DMA��������С",
#define     CFG_UART6_SENDBUF_LEN            32                 // "UART6���ͻ��λ�������С",
#define     CFG_UART6_RECVBUF_LEN            32                 // "UART6���ջ��λ�������С",
#define     CFG_UART6_DMABUF_LEN             32                 // "UART6 DMA��������С",
#define     CFG_UART7_SENDBUF_LEN            32                 // "UART7���ͻ��λ�������С",
#define     CFG_UART7_RECVBUF_LEN            32                 // "UART7���ջ��λ�������С",
#define     CFG_UART7_DMABUF_LEN             32                 // "UART7 DMA��������С",
#define     CFG_UART8_SENDBUF_LEN            32                 // "UART8���ͻ��λ�������С",
#define     CFG_UART8_RECVBUF_LEN            32                 // "UART8���ջ��λ�������С",
#define     CFG_UART8_DMABUF_LEN             32                 // "UART8 DMA��������С",
#define     CFG_UART1_ENABLE                 false              // "�Ƿ�ʹ��UART1",
#define     CFG_UART2_ENABLE                 true               // "�Ƿ�ʹ��UART2",
#define     CFG_UART3_ENABLE                 false              // "�Ƿ�ʹ��UART3",
#define     CFG_UART4_ENABLE                 false              // "�Ƿ�ʹ��UART4",
#define     CFG_UART5_ENABLE                 false              // "�Ƿ�ʹ��UART5",
#define     CFG_UART6_ENABLE                 false              // "�Ƿ�ʹ��UART6",
#define     CFG_UART7_ENABLE                 false              // "�Ƿ�ʹ��UART7",
#define     CFG_UART8_ENABLE                 false              // "�Ƿ�ʹ��UART8",
//*******************************  Configure shell  ******************************************//
#define     CFG_ADD_ROUTINE_SHELL            true                  // "�Ƿ���ӳ���shell����"
#define     CFG_ADD_EXPAND_SHELL             true                  // "�Ƿ������չshell����"
#define     CFG_ADD_GLOBAL_FUN               false                 // "���ȫ�ֺ�����shell"
#define     CFG_SHOW_ADD_SHEELL              true                  // "��ʾ�ڱ��봰����ӵ�shell����"
#define     CFG_MODULE_ENABLE_SHELL          true
#define     CFG_SHELL_STACK                  (0x1000)
//*******************************  Configure component kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096        // "��ʼ��ջ�ռ�",�����ʼ������ʹ�õ�ջ�ռ䣬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         15          // "�¼�������ֵ",�¼�����
#define CFG_EVENT_TYPE_LIMIT    15          // "�¼���������ֵ",�¼�������
#define CFG_MAINSTACK_LIMIT     4096        // "main����ջ�ߴ�",main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024        // "IDLE�¼�ջ�ߴ�",IDLE�¼����������е�ջ�ߴ磬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_OS_TINY             false       // "tiny���ں�",true=������Դ��ȱ�ĳ��ϣ��ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
//*******************************  Configure os memory poll  ******************************************//
#define     CFG_MEMPOOL_LIMIT                10                 // "�ڴ������",������10���ڴ��
//*******************************  Configure loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN     //EN_DIRECT_RUN=ֱ�Ӵ�flash�����У�EN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL      //�Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define CFG_IBOOT_VERSION       01        //Iboot�����汾��
//*******************************  Configure filesystem  ******************************************//
#define     CFG_CLIB_BUFFERSIZE              512               //
//*******************************  Configure yaf2filesystem  ******************************************//
#define     CFG_YAF_ECC                      0                 //YAF�ļ�ϵͳ�ļ�ʹ���豸ECC���ܡ�0��ʹ��ecc��YAF2_ENABLE_DEVICE_ECCʹ��ecc
#define     CFG_YAF_INSTALL_OPTION           true              //YAF�ļ�ϵͳ��װѡ���װʱ�Ƿ��ʽ�������ļ�ϵͳ��
#define     CFG_YAF_MOUNT_POINT              "yaf2"            //YAF�ļ�ϵͳ��װĿ¼
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (150*Mhz)         //��Ƶ���ں�Ҫ�ã����붨��

#endif
