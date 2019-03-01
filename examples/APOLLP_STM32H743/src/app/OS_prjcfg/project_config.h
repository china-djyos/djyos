/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40           // "��������",�������������������ź����ͻ�����
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true       // "ȫ���ܶ�̬����",��ʹѡfalse��Ҳ����ʹ��malloc-free�����ڴ棬��ʹ���в����� �������û��ֲᡷ�ڴ�����½�
//*******************************  Configure DevFile  ******************************************//
#define CFG_DEVFILE_LIMIT       10           // "�豸����",�����豸����
//*******************************  Configure ioiicbus  ******************************************//
#define IO_IIC_USER_TAG      0       // �û��Լ��ı�ǩ�����ڻص��û��ṩ��IO���ƺ����ĵڶ�������
#define IO_IIC_BUS_NAME "IoIic"     // ��IIC���ߵ�����
#define IIC_IOCTRL_FUN   IIC_IoCtrlFunc   // �û�ʵ�ֵ�IO���ƺ�����
//*******************************  Configure filesystem  ******************************************//
#define CFG_CLIB_BUFFERSIZE            512           // "C���ļ��û�̬�������ߴ�"
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_RUN_MODE            (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW)      // "STDIOģʽ",
#define CFG_STDIO_IN_NAME              "/dev/UART1"         // "��׼�����豸��",
#define CFG_STDIO_OUT_NAME             "/dev/UART1"         // "��׼����豸��",
#define CFG_STDIO_ERR_NAME             "/dev/UART1"         // "��׼err����豸��",
//*******************************  Configure cpu_peri_uart  ******************************************//
#define CFG_UART_SENDBUF_LEN            32                       // "���ڷ��ͻ��λ�������С",
#define CFG_UART_RECVBUF_LEN            32                       // "���ڽ��ջ��λ�������С",
#define CFG_UART1_ENABLE                true                     // "�����Ƿ�ʹ��UART1",
#define CFG_UART2_ENABLE                false                    // "�����Ƿ�ʹ��UART2",
#define CFG_UART3_ENABLE                false                    // "�����Ƿ�ʹ��UART3",
#define CFG_UART4_ENABLE                false                    // "�����Ƿ�ʹ��UART4",
#define CFG_UART5_ENABLE                false                    // "�����Ƿ�ʹ��UART5",
#define CFG_UART6_ENABLE                false                    // "�����Ƿ�ʹ��UART6",
#define CFG_UART7_ENABLE                false                    // "�����Ƿ�ʹ��UART7",
#define CFG_UART8_ENABLE                false                    // "�����Ƿ�ʹ��UART8",
//*******************************  Configure shell  ******************************************//
#define     CFG_ADD_ROUTINE_SHELL            true                   // "�Ƿ���ӳ���shell����"
#define     CFG_ADD_EXPAND_SHELL             true                   // "�Ƿ������չshell����"
#define     CFG_ADD_GLOBAL_FUN               false                  // "���ȫ�ֺ�����shell"
#define     CFG_SHOW_ADD_SHEELL              true                   // "��ʾ�ڱ��봰����ӵ�shell����"
//*******************************  Configure tcp  ******************************************//
#define     CFG_TCP_REORDER                  true                   // "TCP��������ʹ��",��Դ����Ŵ�
#define     CFG_TCP_CCBNUM                   10                     // "tcp �ͻ�������ֵ"��ռһ�� ָ�� �� struct ClienCB
#define     CFG_TCP_SCBNUM                   5                      // "tcp ����������ֵ"��ռһ�� ָ�� �� struct ServerCB
//*******************************  Configure tcpip  ******************************************//
#define     CFG_NETPKG_MEMSIZE               0x4000                 // "���ݰ��������ߴ�"
#define     CFG_ARP_HASHLEN                  32                     // "ARP��ϣ����"��ռ��һ��ָ��
#define     CFG_TPL_PROTONUM                 5                      // "֧�ֵĴ���Э����"��ռ��һ�� tagTplProtoItem �ṹ
#define     CFG_UDP_ENABLE                   true                   // "udpЭ��ʹ��"
#define     CFG_UDP_CBNUM                    10                     // "UDP socket����ֵ"��ռ��һ�� tagUdpCB �ṹ
#define     CFG_UDP_HASHLEN                  4                      // "udp socket ��ϣ����"��ռ��һ��ָ��
#define     CFG_TCP_ENABLE                   true                   // #define     CFG_TCP_ENABLE              true    
#define     CFG_TCP_REORDER                  true                   // #define     CFG_TCP_REORDER             true    
#define     CFG_TCP_CCBNUM                   10                     // #define     CFG_TCP_CCBNUM              10      
#define     CFG_TCP_SCBNUM                   5                      // #define     CFG_TCP_SCBNUM              5       
#define     CFG_SOCKET_NUM                   10                     // "socket����ֵ"��ռһ�� tagItem �ṹ
#define     CFG_TELNET_ENABLE                true                   // "telnet ʹ��"
#define     CFG_TELNETD_ENABLE               true                   // "telnet ������ʹ��"
#define     CFG_TELNETC_ENABLE               false                  // "telnet �ͻ���ʹ��"
#define     CFG_FTP_ENABLE                   true                   // "ftp ʹ��"
#define     CFG_FTPD_ENABLE                  true                   // "ftp ������ʹ��"
#define     CFG_FTPC_ENABLE                  false                  // "ftp �ͻ���ʹ��"
#define     CFG_TFTP_ENABLE                  true                   // "tftp ʹ��"
#define     CFG_TFTPD_ENABLE                 false                  // "tftp ������ʹ��",��δʵ��
#define     CFG_TFTPC_ENABLE                 true                   // "tftp �ͻ���ʹ��"
#define     CFG_TFTP_PATHDEFAULT             "/"                       // TFTPĬ�Ϲ���Ŀ¼
#define     CFG_DHCP_ENABLE                  true                   // "DHCP ʹ��"
#define     CFG_DHCPD_ENABLE                 false                  // "DHCP ������ʹ��"
#define     CFG_DHCPC_ENABLE                 true                   // "DHCP �ͻ���ʹ��"
#define     CFG_DHCP_RENEWTIME               3600                   // "renew timer",����
#define     CFG_DHCP_REBINDTIME              3600                   // "rebind timer",����
#define     CFG_DHCP_LEASETIME               3600                   // "lease timer",����
#define     CFG_DHCPD_IPNUM                  0x40                   // "IP�سߴ�",64
#define     CFG_DHCPD_IPSTART                "192.168.0.2"          // "DHCP��ʼIP",
#define     CFG_DHCPD_SERVERIP               "192.168.0.253"        // "DHCP SERVER IP"
#define     CFG_DHCPD_ROUTERIP               "192.168.0.253"        // "DHCP ROUTER SERVER IP"
#define     CFG_DHCPD_NETIP                  "255.255.255.0"        // "DHCP MASK IP"
#define     CFG_DHCPD_DNS                    "192.168.0.253"        // "DHCP DNSSERVER IP"
#define     CFG_DHCPD_DOMAINNAME             "domain"               // "DHCP domain name"
#define     CFG_PPP_ENABLE                   false                  // �Ƿ���Ҫ ppp Э��
//*******************************  Configure timer  ******************************************//
#define CFG_TIMER_SOUCE     1            // "ʱ��Դ",1=��Ӳ����ʱ���ṩʱ��Դ��0=��tick�ṩʱ��Դ
#define CFG_TIMERS_LIMIT    5            // "��ʱ������",�ɴ����Ķ�ʱ��������������ͼ�ν���Ķ�ʱ����
//*******************************  Configure ymodem  ******************************************//
#define     CFG_YMODEM_BUF_NUM               32                     // "����ymodem������",ÿ��1024�ֽ�
#define     CFG_YMODEM_PKG_TIMEOUT           (15*1000*1000)         // "�������ʱʱ��",΢��
#define     CFG_YMODEM_TIMEOUT               (300*1000*1000)        // "ymodem�����ܳ�ʱʱ��",΢��
#define     CFG_YMODEM_DEVNAME               "/dev/UART1"                  // "ymodem�����豸��std��ʾʹ�ñ�׼��������豸"
#define     CFG_YMODEM_TIMEOUT               (600*1000*1000)        // #define CFG_YMODEM_TIMEOUT (600*1000*1000)
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096         // "��ʼ��ջ�ռ�",�����ʼ������ʹ�õ�ջ�ռ䣬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         15           // "�¼�������ֵ",�¼�����
#define CFG_EVENT_TYPE_LIMIT    15           // "�¼���������ֵ",�¼�������
#define CFG_MAINSTACK_LIMIT     4096         // "main����ջ�ߴ�",main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024         // "IDLE�¼�ջ�ߴ�",IDLE�¼����������е�ջ�ߴ磬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_OS_TINY             false        // "tiny���ں�",true=������Դ��ȱ�ĳ��ϣ��ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
//*******************************  Configure MemoryPool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10           // "�ڴ������",������10���ڴ��
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8           // "ʱ��",����ʱ���Ƕ�8��
//*******************************  Configure loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN          // EN_DIRECT_RUN=ֱ�Ӵ�flash�����У�EN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL           // �Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define CFG_IBOOT_VERSION       01             // Iboot�����汾��
//*******************************  Configure cpu_peri_eth  ******************************************//
#define     CFG_MAC_ADDR0                    00                 // "������ַ0",
#define     CFG_MAC_ADDR1                    00                 // "������ַ1",
#define     CFG_MAC_ADDR2                    00                 // "������ַ2",
#define     CFG_MAC_ADDR3                    00                 // "������ַ3",
#define     CFG_MAC_ADDR4                    00                 // "������ַ4",
#define     CFG_MAC_ADDR5                    00                 // "������ַ5",
#define     CFG_ETH_LOOP_CYCLE               1000               // "������ѯ��������",
#define     CFG_ETH_LOOP_ENABLE              true               // "���������Ƿ���ѯ",
#define     CFG_ETH_DEV_NAME                 "STM32H7_ETH"      // "��������",
//*******************************  Configure lan8720  ******************************************//
#define CFG_LAN8720_PHY_ADDRESS     0        // phy�������ַ��Ӳ��һ�����Ϊ 0
//*******************************  Configure netconfig  ******************************************//
#define     CFG_STATIC_IP                    true                   // "IP����",true=ʹ�þ�̬IP��false=��̬IP
#define     CFG_NETCARD_NAME                 "NUCLEO_H743_ETH"      // "������",
#define     CFG_MY_IPV4                      "192.168.0.179"        // "��̬IP",
#define     CFG_MY_SUBMASK                   "255.255.255.0"        // "��������",
#define     CFG_MY_GATWAY                    "192.168.0.1"          // "����",
#define     CFG_MY_DNS                       "192.168.0.1"          // "DNS",
//*******************************  Configure PCF8574  ******************************************//
#define     CFG_PCF8574_BUS_NAME             "IoIic"              // "name"
//*******************************  Configure cpu_peri_emflash  ******************************************//
#define     CFG_EFLASH_PART_START            0                   // ������ʼ
#define     CFG_EFLASH_PART_END              4                   // ����������-1�������洢ģ�������ַ
#define     CFG_EFLASH_PART_OPTION           0                   // ����ѡ��
#define     CFG_EFLASH_FSMOUNT_NAME          EN_XIP_IBOOT_TARGET  // �谲װ���ļ�ϵͳ��mount�����֣�NULL��ʾ��flash�������ļ�ϵͳ
//*******************************  Configure yaf2filesystem  ******************************************//
#define     CFG_YAF_ECC                      0                     // YAF�ļ�ϵͳ�ļ�ʹ���豸ECC���ܡ�0��ʹ��ecc��YAF2_ENABLE_DEVICE_ECCʹ��ecc
#define     CFG_YAF_INSTALL_OPTION           false                 // YAF�ļ�ϵͳ��װѡ���װʱ�Ƿ��ʽ�������ļ�ϵͳ��
#define     CFG_YAF_MOUNT_POINT              "yaf2"                // "name",YAF�ļ�ϵͳ��װĿ¼
//*******************************  Configure cpu_peri_nand  ******************************************//
#define     CFG_NFLASH_FSMOUNT_NAME          "yaf2"         // �谲װ���ļ�ϵͳ��mount������
#define     CFG_NFLASH_PART_START            0                 // ������ʼ
#define     CFG_NFLASH_PART_END              -1                // ��������
#define     CFG_NFLASH_PART_OPTION           0                 // ����ѡ��
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (400*Mhz)         //��Ƶ���ں�Ҫ�ã����붨��

#endif
