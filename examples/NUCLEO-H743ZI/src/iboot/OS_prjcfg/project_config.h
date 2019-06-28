/****************************************************
 *  Automatically-generated file. Do not edit!  *
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10      //�����豸����
//*******************************  Configure file system  ******************************************//
#define CFG_CLIB_BUFFERSIZE            512      //
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40         // "��������",�������������������ź����ͻ�����
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true     // "ȫ���ܶ�̬����",��ʹѡfalse��Ҳ����ʹ��malloc-free�����ڴ棬��ʹ���в����� �������û��ֲᡷ�ڴ�����½�
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096       // "��ʼ��ջ�ռ�",�����ʼ������ʹ�õ�ջ�ռ䣬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         15         // "�¼�������ֵ",�¼�����
#define CFG_EVENT_TYPE_LIMIT    15         // "�¼���������ֵ",�¼�������
#define CFG_MAINSTACK_LIMIT     4096       // "main����ջ�ߴ�",main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024       // "IDLE�¼�ջ�ߴ�",IDLE�¼����������е�ջ�ߴ磬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_OS_TINY             false      // "tiny���ں�",true=������Դ��ȱ�ĳ��ϣ��ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8   //������Զ�����
#define CFG_HANDLE_LIMIT        8   //������Զ�����
//*******************************  Configure loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN        // EN_DIRECT_RUN=ֱ�Ӵ�flash�����У�EN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL         // �Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define CFG_IBOOT_VERSION       01           // Iboot�����汾��
#define CFG_IBOOT_UPDATE_NAME      "/yaf2/iboot.bin"            // "Iboot����"
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10      //
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_STDIN_MULTI      true     // "�Ƿ�֧�ֶ��������豸",
#define CFG_STDIO_STDOUT_FOLLOW    true     // "stdout�Ƿ����stdin",
#define CFG_STDIO_STDERR_FOLLOW    true     // "stderr�Ƿ����stdin",
#define CFG_STDIO_FLOAT_PRINT       true         // "֧�ָ����ӡ"
#define CFG_STDIO_STDIOFILE         true         // "֧�ֱ�׼IO�ļ�"
#define CFG_STDIO_IN_NAME              "/dev/UART1"     // "��׼�����豸��",
#define CFG_STDIO_OUT_NAME             "/dev/UART1"     // "��׼����豸��",
#define CFG_STDIO_ERR_NAME             "/dev/UART1"     // "��׼err����豸��",
//*******************************  Configure shell  ******************************************//
#define CFG_SHELL_STACK            0x1000       // "ִ��shell�����ջ�ߴ�"
#define CFG_ADD_ROUTINE_SHELL      true         // "�Ƿ���ӳ���shell����"
#define CFG_ADD_EXPAND_SHELL       true         // "�Ƿ������չshell����"
#define CFG_ADD_GLOBAL_FUN         false        // "���ȫ�ֺ�����shell"
#define CFG_SHOW_ADD_SHEELL        true         // "��ʾ�ڱ��봰����ӵ�shell����"
#define  CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure tcpip  ******************************************//
#define     CFG_NETPKG_MEMSIZE               0x4000              // "���ݰ��������ߴ�"
#define  CFG_MODULE_ENABLE_TCPIP         true
#define  CFG_MODULE_ENABLE_TCPIP         true
//*******************************  Configure ymodem  ******************************************//
#define     CFG_YMODEM_BUF_NUM               32                   // "����ymodem������",ÿ��1024�ֽ�
#define     CFG_YMODEM_PKG_TIMEOUT           (15*1000*1000)       // "�������ʱʱ��",΢��
#define     CFG_YMODEM_TIMEOUT               (300*1000*1000)      // "ymodem�����ܳ�ʱʱ��",΢��
#define     CFG_YMODEM_DEVNAME               "std"                // "ymodem�����豸��std��ʾʹ�ñ�׼��������豸"
//*******************************  Configure cpu onchip uart  ******************************************//
#define CFG_UART_SENDBUF_LEN            32                  //
#define CFG_UART_RECVBUF_LEN            32                  //
#define CFG_UART1_ENABLE                true                //
#define CFG_UART2_ENABLE                false               //
#define CFG_UART3_ENABLE                false               //
#define CFG_UART4_ENABLE                false               //
#define CFG_UART5_ENABLE                false               //
#define CFG_UART6_ENABLE                false               //
#define CFG_UART7_ENABLE                false               //
#define CFG_UART8_ENABLE                false               //
//*******************************  Configure network config  ******************************************//
#define CFG_STATIC_IP       true            //
#define CFG_SELECT_NETCARD  "NUCLEO_H743_ETH"   //������ѡ�е��������������õ�������ͬ
#define CFG_MY_IPV4         "192.168.0.179" //
#define CFG_MY_SUBMASK      "255.255.255.0" //
#define CFG_MY_GATWAY       "192.168.0.1"   //
#define CFG_MY_DNS          "192.168.0.1"   //
//*******************************  Configure cpu onchip MAC  ******************************************//
#define    CFG_ETH_NETCARD_NAME            "STM32H7_ETH"     //
#define    CFG_ETH_LOOP_CYCLE              1000              //�ж�ģʽ������д
#define    CFG_ETH_LOOP_ENABLE             true              //
#define    CFG_ETH_HARD_MAC_ADDR           true              //
#define    CFG_ETH_MAC_ADDR0               00                //��ѡ��"Ӳ������Mac��ַ"��������д
#define    CFG_ETH_MAC_ADDR1               02                //��ѡ��"Ӳ������Mac��ַ"��������д
#define    CFG_ETH_MAC_ADDR2               03                //��ѡ��"Ӳ������Mac��ַ"��������д
#define    CFG_ETH_MAC_ADDR3               04                //��ѡ��"Ӳ������Mac��ַ"��������д
#define    CFG_ETH_MAC_ADDR4               05                //��ѡ��"Ӳ������Mac��ַ"��������д
#define    CFG_ETH_MAC_ADDR5               06                //��ѡ��"Ӳ������Mac��ַ"��������д
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (400*Mhz)         //��Ƶ���ں�Ҫ�ã����붨��

#endif
