/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40      //�����������������ź����ͻ�����
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true  //��ʹѡfalseҲ����ʹ��malloc-free�����ڴ浫ʹ���в����� �������û��ֲᡷ�ڴ�����½�
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8   //������Զ�����
#define CFG_HANDLE_LIMIT        8   //������Զ�����
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10      //
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10      //�����豸����
//*******************************  Configure tpl  ******************************************//
#define     CFG_TPL_PROTONUM            5       //ռ��һ�� tagTplProtoItem �ṹ
//*******************************  Configure tcpip  ******************************************//
#define     CFG_NETPKG_MEMSIZE          0x4000  //
#define  CFG_MODULE_ENABLE_TCPIP         true
#define  CFG_MODULE_ENABLE_TCPIP         true
//*******************************  Configure icmp  ******************************************//
#define  CFG_MODULE_ENABLE_ICMP          true
//*******************************  Configure router  ******************************************//
#define CFG_IP_STRMAX           20 //���·����Ŀ��
#define  CFG_MODULE_ENABLE_ROUTER        true
//*******************************  Configure cpu onchip MAC  ******************************************//
#define    CFG_MAC_ADDR0                   00                //
#define    CFG_MAC_ADDR1                   00                //
#define    CFG_MAC_ADDR2                   00                //
#define    CFG_MAC_ADDR3                   00                //
#define    CFG_MAC_ADDR4                   00                //
#define    CFG_MAC_ADDR5                   00                //
#define    CFG_ETH_LOOP_CYCLE              1000              //
#define    CFG_ETH_LOOP_ENABLE             true              //
#define    CFG_ETH_DEV_NAME                "STM32F7_ETH"     //
//*******************************  Configure cpu onchip uart  ******************************************//
#define    CFG_UART1_SENDBUF_LEN           64                //
#define    CFG_UART1_RECVBUF_LEN           64                //
#define    CFG_UART1_DMABUF_LEN            64                //
#define    CFG_UART2_SENDBUF_LEN           64                //
#define    CFG_UART2_RECVBUF_LEN           64                //
#define    CFG_UART2_DMABUF_LEN            64                //
#define    CFG_UART3_SENDBUF_LEN           64                //
#define    CFG_UART3_RECVBUF_LEN           64                //
#define    CFG_UART3_DMABUF_LEN            64                //
#define    CFG_UART4_SENDBUF_LEN           64                //
#define    CFG_UART4_RECVBUF_LEN           64                //
#define    CFG_UART4_DMABUF_LEN            64                //
#define    CFG_UART1_ENABLE                true              //
#define    CFG_UART2_ENABLE                true              //
#define    CFG_UART3_ENABLE                false             //
#define    CFG_UART4_ENABLE                false             //
//*******************************  Configure network config  ******************************************//
#define    CFG_STATIC_IP                   false             //true=ʹ�þ�̬IPfalse=��̬IP
#define    CFG_NETCARD_NAME                "BK7251_WIFI"     //
#define    CFG_MY_IPV4                     "192.168.0.179"   //
#define    CFG_MY_SUBMASK                  "255.255.255.0"   //
#define    CFG_MY_GATWAY                   "192.168.0.1"     //
#define    CFG_MY_DNS                      "192.168.0.1"     //
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_STDIN_MULTI      true         //
#define CFG_STDIO_STDOUT_FOLLOW    true         //
#define CFG_STDIO_STDERR_FOLLOW    true         //
#define CFG_STDIO_FLOAT_PRINT      true         //
#define CFG_STDIO_STDIOFILE        true         //
#define CFG_STDIO_IN_NAME              "/dev/UART1"    //
#define CFG_STDIO_OUT_NAME             "/dev/UART1"    //
#define CFG_STDIO_ERR_NAME             "/dev/UART1"    //
//*******************************  Configure shell  ******************************************//
#define    CFG_SHELL_STACK                 0x1000            //
#define    CFG_ADD_ROUTINE_SHELL           true              //
#define    CFG_ADD_EXPAND_SHELL            true              //
#define    CFG_ADD_GLOBAL_FUN              false             //
#define    CFG_SHOW_ADD_SHEELL             true              //
#define  CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure kernel  ******************************************//
#define    CFG_INIT_STACK_SIZE             4096              //�����ʼ������ʹ�õ�ջ�ռ�һ�㰴Ĭ��ֵ�Ϳ�����
#define    CFG_EVENT_LIMIT                 15                //�¼�����
#define    CFG_EVENT_TYPE_LIMIT            15                //�¼�������
#define    CFG_MAINSTACK_LIMIT             4096              //main�������������ջ�ߴ�
#define    CFG_IDLESTACK_LIMIT             1024              //IDLE�¼������������е�ջ�ߴ�һ�㰴Ĭ��ֵ�Ϳ�����
#define    CFG_OS_TINY                     false             //true=������Դ��ȱ�ĳ����ں˻�ü������¼����������¼�����ʱ��ͳ�ơ�
//*******************************  Configure loader  ******************************************//
#define    CFG_APP_RUNMODE                 EN_DIRECT_RUN     //EN_DIRECT_RUN=ֱ�Ӵ�flash�����У�EN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define    CFG_APP_VERIFICATION            VERIFICATION_NULL //�Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define    CFG_IBOOT_VERSION               01                //Iboot�����汾��
#define    CFG_IBOOT_UPDATE_NAME           "/yaf2/iboot.bin" //
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (180.0*Mhz)       //��Ƶ���ں�Ҫ�ã����붨��

#endif