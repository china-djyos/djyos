/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40                // "��������",�������������������ź����ͻ�����
//*******************************  Configure heap  ******************************************//
#define     CFG_DYNAMIC_MEM                  true                    // "ȫ���ܶ�̬����",��ʹѡfalse��Ҳ����ʹ��malloc-free�����ڴ棬��ʹ���в����� �������û��ֲᡷ�ڴ�����½�
//*******************************  Configure DevFile  ******************************************//
#define CFG_DEVFILE_LIMIT       10                // "�豸����",�����豸����
//*******************************  Configure filesystem  ******************************************//
#define CFG_CLIB_BUFFERSIZE            512            // "C���ļ��û�̬�������ߴ�"
//*******************************  Configure stdio  ******************************************//
#define     CFG_STDIO_RUN_MODE               (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW)     // "STDIOģʽ",
#define     CFG_STDIO_IN_NAME                "/dev/UART3"           // "��׼�����豸��",
#define     CFG_STDIO_OUT_NAME               "/dev/UART3"           // "��׼����豸��",
#define     CFG_STDIO_ERR_NAME               "/dev/UART3"           // "��׼err����豸��",
//*******************************  Configure cpu_peri_uart  ******************************************//
#define CFG_UART1_SENDBUF_LEN       64                // "UART1���ͻ��λ�������С",
#define CFG_UART1_RECVBUF_LEN       64                // "UART1���ջ��λ�������С",
#define CFG_UART1_DMABUF_LEN        64                // "UART1 DMA���λ�������С",
#define CFG_UART2_SENDBUF_LEN       64                // "UART2���ͻ��λ�������С",
#define CFG_UART2_RECVBUF_LEN       64                // "UART2���ջ��λ�������С",
#define CFG_UART2_DMABUF_LEN        64                // "UART2 DMA���λ�������С",
#define CFG_UART3_SENDBUF_LEN       64                // "UART3���ͻ��λ�������С",
#define CFG_UART3_RECVBUF_LEN       64                // "UART3���ջ��λ�������С",
#define CFG_UART3_DMABUF_LEN        64                // "UART3 DMA���λ�������С",
#define CFG_UART4_SENDBUF_LEN       64                // "UART4���ͻ��λ�������С",
#define CFG_UART4_RECVBUF_LEN       64                // "UART4���ջ��λ�������С",
#define CFG_UART4_DMABUF_LEN        64                // "UART4 DMA���λ�������С",
#define CFG_UART5_SENDBUF_LEN       64                // "UART5���ͻ��λ�������С",
#define CFG_UART5_RECVBUF_LEN       64                // "UART5���ջ��λ�������С",
#define CFG_UART5_DMABUF_LEN        64                // "UART5 DMA���λ�������С",
#define CFG_UART6_SENDBUF_LEN       64                // "UART6���ͻ��λ�������С",
#define CFG_UART6_RECVBUF_LEN       64                // "UART6���ջ��λ�������С",
#define CFG_UART6_DMABUF_LEN        64                // "UART6 DMA���λ�������С",
#define CFG_UART7_SENDBUF_LEN       64                // "UART7���ͻ��λ�������С",
#define CFG_UART7_RECVBUF_LEN       64                // "UART7���ջ��λ�������С",
#define CFG_UART7_DMABUF_LEN        64                // "UART7 DMA���λ�������С",
#define CFG_UART8_SENDBUF_LEN       64                // "UART8���ͻ��λ�������С",
#define CFG_UART8_RECVBUF_LEN       64                // "UART8���ջ��λ�������С",
#define CFG_UART8_DMABUF_LEN        64                // "UART8 DMA���λ�������С",
#define CFG_UART1_ENABLE           true                  // "�Ƿ�ʹ��UART1",
#define CFG_UART2_ENABLE           true                 // "�Ƿ�ʹ��UART2",
#define CFG_UART3_ENABLE           true                 // "�Ƿ�ʹ��UART3",
#define CFG_UART4_ENABLE           true                 // "�Ƿ�ʹ��UART4",
#define CFG_UART5_ENABLE           true                 // "�Ƿ�ʹ��UART5",
#define CFG_UART6_ENABLE           true                 // "�Ƿ�ʹ��UART6",
#define CFG_UART7_ENABLE           true                 // "�Ƿ�ʹ��UART7",
#define CFG_UART8_ENABLE           true                 // "�Ƿ�ʹ��UART8",
//*******************************  Configure tcpip  ******************************************//
#define     CFG_NETPKG_MEMSIZE               0x4000                 // "���ݰ��������ߴ�"
#define     CFG_ARP_HASHLEN                  32                     // "ARP��ϣ����"��ռ��һ��ָ��
#define     CFG_TPL_PROTONUM                 5                      // "֧�ֵĴ���Э����"��ռ��һ�� tagTplProtoItem �ṹ
#define     CFG_UDP_ENABLE                   true                   // "udpЭ��ʹ��"
#define     CFG_UDP_CBNUM                    10                     // "UDP socket����ֵ"��ռ��һ�� tagUdpCB �ṹ
#define     CFG_UDP_HASHLEN                  4                      // "udp socket ��ϣ����"��ռ��һ��ָ��
#define     CFG_TCP_ENABLE                   true                   // "tcpЭ��ʹ��"
#define     CFG_TCP_REORDER                  true                   // "TCP��������ʹ��",��Դ����Ŵ�
#define     CFG_TCP_CCBNUM                   10                     // "tcp �ͻ�������ֵ"��ռһ�� ָ�� �� tagCCB
#define     CFG_TCP_SCBNUM                   5                      // "tcp ����������ֵ"����ռһ�� ָ�� �� tagSCB
#define     CFG_SOCKET_NUM                   10                     // "socket����ֵ"��ռһ�� tagItem �ṹ
#define     CFG_TELNET_ENABLE                true                   // "telnet ʹ��"
#define     CFG_TELNETD_ENABLE               true                   // "telnet ������ʹ��"
#define     CFG_TELNETC_ENABLE               false                  // "telnet �ͻ���ʹ��"
#define     CFG_FTP_ENABLE                   true                   // "ftp ʹ��"
#define     CFG_FTPD_ENABLE                  true                   // "ftp ������ʹ��"
#define     CFG_FTPC_ENABLE                  false                  // "ftp �ͻ���ʹ��"
#define     CFG_TFTP_ENABLE                  true                   // "tftp ʹ��"
#define     CFG_TFTPD_ENABLE                 false                  // "tftp ������ʹ��"
#define     CFG_TFTPC_ENABLE                 true                   // "tftp �ͻ���ʹ��"
#define     CN_TFTP_PATHDEFAULT              "/efs/"                // TFTPĬ�Ϲ���Ŀ¼
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
#define     CFG_DHCPD_DOMAINNAME             ""                     // "DHCP domain name"
#define     CN_PPP_ENABLE                    1                      // �Ƿ���Ҫ ppp Э��
#define CFG_MYCONFIG6               // "name",�ҵ�����
#define     CFG_MYCONFIG7                    CN_CONFIG              // �ҵ�����
//*******************************  Configure timer  ******************************************//
#define CFG_TIMER_SOUCE     1           // "ʱ��Դ",1=��Ӳ����ʱ���ṩʱ��Դ��0=��tick�ṩʱ��Դ
#define CFG_TIMERS_LIMIT    5           // "��ʱ������",�ɴ����Ķ�ʱ��������������ͼ�ν���Ķ�ʱ����
//*******************************  Configure kernel  ******************************************//
#define     CFG_INIT_STACK_SIZE              4096                    // "��ʼ��ջ�ռ�",�����ʼ������ʹ�õ�ջ�ռ䣬һ�㰴Ĭ��ֵ�Ϳ�����
#define     CFG_EVENT_LIMIT                  20                      // "�¼�������ֵ",�¼�����
#define     CFG_EVENT_TYPE_LIMIT             20                      // "�¼���������ֵ",�¼�������
#define     CFG_MAINSTACK_LIMIT              4096                    // "main����ջ�ߴ�",main�������������ջ�ߴ�
#define     CFG_IDLESTACK_LIMIT              1024                    // "IDLE�¼�ջ�ߴ�",IDLE�¼����������е�ջ�ߴ磬һ�㰴Ĭ��ֵ�Ϳ�����
#define     CFG_OS_TINY                      false                   // "tiny���ں�",true=������Դ��ȱ�ĳ��ϣ��ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
//*******************************  Configure MemoryPool  ******************************************//
#define     CFG_MEMPOOL_LIMIT                10                      // "�ڴ������",������10���ڴ��
//*******************************  Configure time  ******************************************//
#define     CFG_LOCAL_TIMEZONE               8                       // "ʱ��",����ʱ���Ƕ�8��
//*******************************  Configure loader  ******************************************//
#define     CFG_APP_RUNMODE                  EN_DIRECT_RUN           // EN_DIRECT_RUN=ֱ�Ӵ�flash�����У�EN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define     CFG_IBOOT_CRC                    EN_NO_APP_CRC           // �Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define     CFG_APP_FILENAME                 ""                      // CFG_APP_RUNMODE==EN_FORM_FILE�����������ļ���
#define     CFG_IBOOT_VERSION                "V1.0.0"                // Iboot�����汾��
#define     CFG_IBOOT_UPDATE_NAME            "iboot.bin"             // iboot�����ļ������˶����ã��Ժ�ĵ�
#define     CFG_IBOOT_UPDATE_PATH            "\\"                    // iboot����·�������˶����ã��Ժ�ĵ�
//*******************************  Configure cpu_peri_iic  ******************************************//
#define CFG_IIC_BUF_LEN           32           // "IIC��������С",����IIC��������С
#define CFG_IIC1_ENABLE           false         // "�Ƿ�ʹ��IIC1",
#define CFG_IIC2_ENABLE           false         // "�Ƿ�ʹ��IIC2",
#define CFG_IIC3_ENABLE           false         // "�Ƿ�ʹ��IIC3",
#define CFG_IIC4_ENABLE           false         // "�Ƿ�ʹ��IIC4",
//*******************************  Configure cpu_peri_eth  ******************************************//
#define     CFG_MAC_ADDR0                    00                    // "������ַ0",
#define     CFG_MAC_ADDR1                    00                    // "������ַ1",
#define     CFG_MAC_ADDR2                    00                    // "������ַ2",
#define     CFG_MAC_ADDR3                    00                    // "������ַ3",
#define     CFG_MAC_ADDR4                    00                    // "������ַ4",
#define     CFG_MAC_ADDR5                    00                    // "������ַ5",
#define     CFG_ETH_LOOP_CYCLE               1000                  // "������ѯ��������",
#define     CFG_ETH_LOOP_ENABLE              true                  // "���������Ƿ���ѯ",
#define     CFG_ETH_DEV_NAME                 "STM32F7_ETH"         // "��������",
//*******************************  Configure netconfig  ******************************************//
#define     CFG_STATIC_IP                    true               // "IP����",true=ʹ�þ�̬IP��false=��̬IP
#define     CFG_NETCARD_NAME                 "NUCLEO-F767ZI_ETH" // "������",
#define     CFG_MY_IPV4                      "192.168.0.179"    // "��̬IP",
#define     CFG_MY_SUBMASK                   "255.255.255.0"    // "��������",
#define     CFG_MY_GATWAY                    "192.168.0.1"      // "����",
#define     CFG_MY_DNS                       "192.168.0.1"      // "DNS",
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (216*Mhz)         //��Ƶ���ں�Ҫ�ã����붨��

#endif
