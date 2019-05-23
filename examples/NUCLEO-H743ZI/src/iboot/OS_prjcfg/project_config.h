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
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10       // "�豸����",�����豸����
//*******************************  Configure file system  ******************************************//
#define     CFG_CLIB_BUFFERSIZE              512                // "C���ļ��û�̬�������ߴ�"
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_RUN_MODE            (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW)  // "STDIOģʽ",
#define CFG_STDIO_IN_NAME              "/dev/UART3"     // "��׼�����豸��",
#define CFG_STDIO_OUT_NAME             "/dev/UART3"     // "��׼����豸��",
#define CFG_STDIO_ERR_NAME             "/dev/UART3"     // "��׼err����豸��",
//*******************************  Configure cpu peri uart  ******************************************//
#define CFG_UART_SENDBUF_LEN            32                   // "���ڷ��ͻ��λ�������С",
#define CFG_UART_RECVBUF_LEN            32                   // "���ڽ��ջ��λ�������С",
#define CFG_UART1_ENABLE                false                 // "�����Ƿ�ʹ��UART1",
#define CFG_UART2_ENABLE                false                // "�����Ƿ�ʹ��UART2",
#define CFG_UART3_ENABLE                true                // "�����Ƿ�ʹ��UART3",
#define CFG_UART4_ENABLE                false                // "�����Ƿ�ʹ��UART4",
#define CFG_UART5_ENABLE                false                // "�����Ƿ�ʹ��UART5",
#define CFG_UART6_ENABLE                false                // "�����Ƿ�ʹ��UART6",
#define CFG_UART7_ENABLE                false                // "�����Ƿ�ʹ��UART7",
#define CFG_UART8_ENABLE                false                // "�����Ƿ�ʹ��UART8",
//*******************************  Configure shell  ******************************************//
#define     CFG_SHELL_LEVEL                  SHELL_LEVEL_BASE   // "shell����ʵ�ּ���",���ڶ���shell֧�ֵ���������
#define     CFG_SHOW_ADD_SHEELL              true               // #define CFG_SHOW_ADD_SHEELL        true        
//*******************************  Configure tcpip  ******************************************//
#define     CFG_NETPKG_MEMSIZE               0x4000            //
#define     CFG_ARP_HASHLEN                  32                //ռ��һ��ָ��
#define     CFG_TPL_PROTONUM                 5                 //ռ��һ�� tagTplProtoItem �ṹ
#define     CFG_UDP_ENABLE                   true              //
#define     CFG_UDP_CBNUM                    10                //ռ��һ�� tagUdpCB �ṹ
#define     CFG_UDP_HASHLEN                  4                 //ռ��һ��ָ��
#define     CFG_TCP_ENABLE                   true              //
#define     CFG_TCP_REORDER                  true              //��Դ����Ŵ�
#define     CFG_TCP_CCBNUM                   10                //ռһ�� ָ�� �� tagCCB
#define     CFG_TCP_SCBNUM                   5                 //ռһ�� ָ�� �� tagSCB
#define     CFG_SOCKET_NUM                   10                //ռһ�� tagItem �ṹ
#define     CFG_TELNET_ENABLE                true              //
#define     CFG_TELNETD_ENABLE               true              //
#define     CFG_TELNETC_ENABLE               false             //
#define     CFG_FTP_ENABLE                   true              //
#define     CFG_FTPD_ENABLE                  true              //
#define     CFG_FTPC_ENABLE                  false             //
#define     CFG_TFTP_ENABLE                  true              //
#define     CFG_TFTPD_ENABLE                 false             //
#define     CFG_TFTPC_ENABLE                 true              //
#define     CFG_TFTP_PATHDEFAULT              "/"          // TFTPĬ�Ϲ���Ŀ¼
#define     CFG_DHCP_ENABLE                  true              //
#define     CFG_DHCPD_ENABLE                 false             //
#define     CFG_DHCPC_ENABLE                 true              //
#define     CFG_DHCP_RENEWTIME               3600              //����
#define     CFG_DHCP_REBINDTIME              3600              //����
#define     CFG_DHCP_LEASETIME               3600              //����
#define     CFG_DHCPD_IPNUM                  0x40              //64
#define     CFG_DHCPD_IPSTART                "192.168.0.2"     //
#define     CFG_DHCPD_SERVERIP               "192.168.0.253"   //"   // ",
#define     CFG_DHCPD_ROUTERIP               "192.168.0.253"   //"   // ",
#define     CFG_DHCPD_NETIP                  "255.255.255.0"   //"   // ",
#define     CFG_DHCPD_DNS                    "192.168.0.253"   //"   // ",
#define     CFG_DHCPD_DOMAINNAME             "domain"          //"         // ",
#define     CN_PPP_ENABLE                    null              //�Ƿ���Ҫ ppp Э��
//*******************************  Configure ymodem  ******************************************//
#define     CFG_YMODEM_BUF_NUM               32                 // "����ymodem������",ÿ��1024�ֽ�
#define     CFG_YMODEM_PKG_TIMEOUT           (15*1000*1000)     // "�������ʱʱ��",΢��
#define     CFG_YMODEM_TIMEOUT               (300*1000*1000)    // "ymodem�����ܳ�ʱʱ��",΢��
#define     CFG_YMODEM_DEVNAME               "std"              // "ymodem�����豸��std��ʾʹ�ñ�׼��������豸"
#define     CFG_YMODEM_TIMEOUT               (600*1000*1000)    // #define CFG_YMODEM_TIMEOUT (600*1000*1000)
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096     // "��ʼ��ջ�ռ�",�����ʼ������ʹ�õ�ջ�ռ䣬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         15       // "�¼�������ֵ",�¼�����
#define CFG_EVENT_TYPE_LIMIT    15       // "�¼���������ֵ",�¼�������
#define CFG_MAINSTACK_LIMIT     4096     // "main����ջ�ߴ�",main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024     // "IDLE�¼�ջ�ߴ�",IDLE�¼����������е�ջ�ߴ磬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_OS_TINY             false    // "tiny���ں�",true=������Դ��ȱ�ĳ��ϣ��ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10       // "�ڴ������",������10���ڴ��
//*******************************  Configure loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN      // EN_DIRECT_RUN=ֱ�Ӵ�flash�����У�EN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL       // �Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define CFG_IBOOT_VERSION       01         // Iboot�����汾��
//*******************************  Configure cpu onchip MAC  ******************************************//
#define CFG_MAC_ADDR0           00            // "������ַ0",
#define CFG_MAC_ADDR1           00            // "������ַ1",
#define CFG_MAC_ADDR2           00            // "������ַ2",
#define CFG_MAC_ADDR3           00            // "������ַ3",
#define CFG_MAC_ADDR4           00            // "������ַ4",
#define CFG_MAC_ADDR5           00            // "������ַ5",
#define CFG_ETH_LOOP_CYCLE      1000          // "������ѯ��������",
#define CFG_ETH_LOOP_ENABLE     true         // "���������Ƿ���ѯ",
#define CFG_ETH_DEV_NAME        "STM32H7_ETH" // "��������",
//*******************************  Configure network config  ******************************************//
#define     CFG_STATIC_IP                    true              //true=ʹ�þ�̬IPfalse=��̬IP
#define     CFG_NETCARD_NAME                 "NUCLEO_H743_ETH" //
#define     CFG_MY_IPV4                      "192.168.0.179"   //
#define     CFG_MY_SUBMASK                   "255.255.255.0"   //
#define     CFG_MY_GATWAY                    "192.168.0.1"     //
#define     CFG_MY_DNS                       "192.168.0.1"     //
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (400*Mhz)         //��Ƶ���ں�Ҫ�ã����붨��

#endif
