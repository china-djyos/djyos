/****************************************************
 *  Automatically-generated file. Do not edit!  *
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure System: heap  ******************************************//
#define CFG_DYNAMIC_MEM true  //��ʹѡfalseҲ����ʹ��malloc-free�����ڴ浫ʹ���в����� �������û��ֲᡷ�ڴ�����½�
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8   //������Զ�����
#define CFG_HANDLE_LIMIT        8   //������Զ�����
//*******************************  Configure System: memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10      //
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10      //�����豸����
//*******************************  Configure netconfig  ******************************************//
#define    CFG_STATIC_IP                   true              //true=ʹ�þ�̬IPfalse=��̬IP
#define    CFG_NETCARD_NAME                "SCM610_ETH"
#define    CFG_MY_IPV4                     "192.168.0.179"   //
#define    CFG_MY_SUBMASK                  "255.255.255.0"   //
#define    CFG_MY_GATWAY                   "192.168.0.1"     //
#define    CFG_MY_DNS                      "192.168.0.1"     //
//*******************************  Configure emflash insatall xip  ******************************************//
#define    CFG_EFLASH_XIP_PART_START       64                //������ʼ����д��ţ���Ŵ�0��ʼ����
#define    CFG_EFLASH_XIP_PART_END         192                //����������-1��ʾ���һ��
#define    CFG_EFLASH_XIP_PART_FORMAT      true             //����ѡ��,�Ƿ���Ҫ��ʽ���÷�����
//*******************************  Configure cpu_peri_emflash  ******************************************//
#define    CFG_EFLASH_PART_FORMAT          false             //����ѡ��,�Ƿ���Ҫ������оƬ��
//*******************************  Configure cpu peri uart  ******************************************//
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
#define    CFG_UART5_SENDBUF_LEN           64                //
#define    CFG_UART5_RECVBUF_LEN           64                //
#define    CFG_UART5_DMABUF_LEN            64                //
#define    CFG_UART6_SENDBUF_LEN           64                //
#define    CFG_UART6_RECVBUF_LEN           64                //
#define    CFG_UART6_DMABUF_LEN            64                //
#define    CFG_UART7_SENDBUF_LEN           64                //
#define    CFG_UART7_RECVBUF_LEN           64                //
#define    CFG_UART7_DMABUF_LEN            64                //
#define    CFG_UART8_SENDBUF_LEN           64                //
#define    CFG_UART8_RECVBUF_LEN           64                //
#define    CFG_UART8_DMABUF_LEN            64                //
#define    CFG_UART1_ENABLE                false             //
#define    CFG_UART2_ENABLE                true              //
#define    CFG_UART3_ENABLE                false             //
#define    CFG_UART4_ENABLE                false             //
#define    CFG_UART5_ENABLE                false             //
#define    CFG_UART6_ENABLE                false             //
#define    CFG_UART7_ENABLE                false             //
#define    CFG_UART8_ENABLE                false             //
//*******************************  Configure cpu onchip MAC  ******************************************//
#define    CFG_GMAC_LOOP_MODE              1                 //
#define    CFG_GMAC_LOOP_CYCLE             1000              //
#define    CFG_GMAC_MAC_ADDR0              11                //
#define    CFG_GMAC_MAC_ADDR1              22                //
#define    CFG_GMAC_MAC_ADDR2              33                //
#define    CFG_GMAC_MAC_ADDR3              44                //
#define    CFG_GMAC_MAC_ADDR4              55                //
#define    CFG_GMAC_MAC_ADDR5              66                //
#define    CFG_GMAC_NAME                   ("scm6xx")        //
//*******************************  Configure cpu onchip iic  ******************************************//
#define    CFG_I2C_BUF_LEN                 128               //
#define    CFG_I2C1_ENABLE                 false             //
#define    CFG_I2C2_ENABLE                 false             //
#define    CFG_I2C3_ENABLE                 false             //
//*******************************  Configure System: file system  ******************************************//
#define    CFG_CLIB_BUFFERSIZE             512               //
//*******************************  Configure shell  ******************************************//
#define    CFG_SHELL_STACK                 0x1000            //
#define    CFG_ADD_ROUTINE_SHELL           true              //
#define    CFG_ADD_EXPAND_SHELL            true              //
#define    CFG_ADD_GLOBAL_FUN              false             //
#define    CFG_SHOW_ADD_SHEELL             true              //
#define  CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure System: stdio  ******************************************//
#define    CFG_STDIO_STDIN_MULTI           true              //
#define    CFG_STDIO_STDOUT_FOLLOW         true              //
#define    CFG_STDIO_STDERR_FOLLOW         true              //
#define    CFG_STDIO_FLOAT_PRINT           true              //
#define    CFG_STDIO_STDIOFILE             true              //
#define    CFG_STDIO_IN_NAME               ("/dev/UART2")    //
#define    CFG_STDIO_OUT_NAME              ("/dev/UART2")    //
#define    CFG_STDIO_ERR_NAME              ("/dev/UART2")    //
//*******************************  Configure ftp  ******************************************//
#define    CFG_FTPD_ENABLE                 false             //��δʵ��
#define    CFG_FTPC_ENABLE                 true              //
#define  CFG_MODULE_ENABLE_FTP           true
//*******************************  Configure telnet  ******************************************//
#define    CFG_TELNETD_ENABLE              true              //
#define    CFG_TELNETC_ENABLE              false             //
#define  CFG_MODULE_ENABLE_TELNET        true
//*******************************  Configure tftp  ******************************************//
#define    CFG_FTPD_ENABLE                 true              //
#define    CFG_FTPC_ENABLE                 false             //
#define    CFG_TFTP_PATHDEFAULT            "/"               //TFTPĬ�Ϲ���Ŀ¼
#define  CFG_MODULE_ENABLE_TFTP          true
//*******************************  Configure arp  ******************************************//
#define    CFG_ARP_HASHLEN                 32                //ռ��һ��ָ��
#define  CFG_MODULE_ENABLE_ARP           true
//*******************************  Configure icmp  ******************************************//
#define  CFG_MODULE_ENABLE_ICMP          true
//*******************************  Configure sock  ******************************************//
#define CFG_TCP_REORDER  false
#define    CFG_SOCKET_NUM                  10                //ռһ�� tagItem �ṹ
//*******************************  Configure router  ******************************************//
#define    CFG_IP_STRMAX                   20                //���·����Ŀ��
#define  CFG_MODULE_ENABLE_ROUTER        true
//*******************************  Configure System: tcp  ******************************************//
#define    CFG_TCP_REORDER                 true              //��Դ����Ŵ�
#define    CFG_TCP_CCBNUM                  10                //ռһ�� ָ�� �� struct ClienCB
#define    CFG_TCP_SCBNUM                  5                 //ռһ�� ָ�� �� struct ServerCB
#define  CFG_MODULE_ENABLE_TCP           true
//*******************************  Configure tpl  ******************************************//
#define    CFG_TPL_PROTONUM                5                 //ռ��һ�� tagTplProtoItem �ṹ
//*******************************  Configure udp  ******************************************//
#define    CFG_UDP_CBNUM                   10                //ռ��һ�� tagUdpCB �ṹ
#define    CFG_UDP_HASHLEN                 4                 //ռ��һ��ָ��
#define    CFG_UDP_PKGMSGLEN               1472              //udp��������
#define    CFG_UDP_BUFLENDEFAULT           0x2000            //8KB
#define  CFG_MODULE_ENABLE_UDP           true
//*******************************  Configure System: tcpip  ******************************************//
#define    CFG_NETPKG_MEMSIZE              0x4000            //
#define  CFG_MODULE_ENABLE_TCPIP         true
#define  CFG_MODULE_ENABLE_TCPIP         true
//*******************************  Configure System: kernel  ******************************************//
#define    CFG_INIT_STACK_SIZE             4096              //�����ʼ������ʹ�õ�ջ�ռ�һ�㰴Ĭ��ֵ�Ϳ�����
#define    CFG_EVENT_LIMIT                 15                //�¼�����
#define    CFG_EVENT_TYPE_LIMIT            15                //�¼�������
#define    CFG_MAINSTACK_LIMIT             4096              //main�������������ջ�ߴ�
#define    CFG_IDLESTACK_LIMIT             1024              //IDLE�¼����������е�ջ�ߴ�һ�㰴Ĭ��ֵ�Ϳ�����
#define    CFG_OS_TINY                     false             //true=������Դ��ȱ�ĳ����ں˻�ü������¼����������¼�����ʱ��ͳ�ơ�
//*******************************  Configure System: ymodem  ******************************************//
#define    CFG_YMODEM_BUF_NUM              32                //ÿ��1024�ֽ�
#define    CFG_YMODEM_PKG_TIMEOUT          (15*1000*1000)    //΢��
#define    CFG_YMODEM_TIMEOUT              (300*1000*1000)   //΢��
#define    CFG_YMODEM_DEVNAME              "std"             //"ymodem�����豸��std��ʾʹ�ñ�׼��������豸","ymodem�����豸��std��ʾʹ�ñ�׼��������豸"
//*******************************  Configure System: lock  ******************************************//
#define    CFG_LOCK_LIMIT                  40                //�����������������ź����ͻ�����
//*******************************  Configure System: time  ******************************************//
#define    CFG_LOCAL_TIMEZONE              8                 //����ʱ���Ƕ�8��
//*******************************  Configure System: loader  ******************************************//
#define    CFG_APP_RUNMODE                 CN_DIRECT_RUN     //CN_DIRECT_RUN=ֱ�Ӵ�flash�����У�CN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define    CFG_APP_VERIFICATION            VERIFICATION_NULL //�Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define    CFG_IBOOT_VERSION               01                //Iboot�����汾��
#define    CFG_IBOOT_UPDATE_NAME           "/yaf2/iboot.bin" //
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (150*Mhz)         //��Ƶ���ں�Ҫ�ã����붨��

#endif
