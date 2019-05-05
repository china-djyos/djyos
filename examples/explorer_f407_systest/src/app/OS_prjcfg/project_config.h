/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

#define CFG_RUNMODE_BAREAPP    1
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40                  // "��������",�������������������ź����ͻ�����
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true              // "ȫ���ܶ�̬����",��ʹѡfalse��Ҳ����ʹ��malloc-free�����ڴ棬��ʹ���в����� �������û��ֲᡷ�ڴ�����½�
//*******************************  Configure DevFile  ******************************************//
#define    CFG_DEVFILE_LIMIT               10                 // "�豸����",�����豸����
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_STDIN_MULTI      true     // "�Ƿ�֧�ֶ��������豸",
#define CFG_STDIO_STDOUT_FOLLOW    true     // "stdout�Ƿ����stdin",
#define CFG_STDIO_STDERR_FOLLOW    true     // "stderr�Ƿ����stdin",
#define CFG_STDIO_FLOAT_PRINT       true                 // "֧�ָ����ӡ"
#define CFG_STDIO_STDIOFILE         true                 // "֧�ֱ�׼IO�ļ�"
#define CFG_STDIO_IN_NAME              "/dev/UART1"     // "��׼�����豸��",
#define CFG_STDIO_OUT_NAME             "/dev/UART1"     // "��׼����豸��",
#define CFG_STDIO_ERR_NAME             "/dev/UART1"     // "��׼err����豸��",
//*******************************  Configure shell  ******************************************//
#define CFG_SHELL_STACK            0x1000               // "ִ��shell�����ջ�ߴ�"
#define     CFG_ADD_ROUTINE_SHELL            true                        // "�Ƿ���ӳ���shell����"
#define     CFG_ADD_EXPAND_SHELL             true                        // "�Ƿ������չshell����"
#define     CFG_ADD_GLOBAL_FUN               false                       // "���ȫ�ֺ�����shell"
#define     CFG_SHOW_ADD_SHEELL              true                        // "��ʾ�ڱ��봰����ӵ�shell����"
#define  CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure dhcp  ******************************************//
#define    CFG_DHCPD_ENABLE                false             //
#define    CFG_DHCPC_ENABLE                true              //
#define    CFG_DHCP_RENEWTIME              3600              //����
#define    CFG_DHCP_REBINDTIME             3600              //����
#define    CFG_DHCP_LEASETIME              3600              //����
#define    CFG_DHCPD_IPNUM                 0x40              //64
#define    CFG_DHCPD_IPSTART               "192.168.0.2"     //
#define    CFG_DHCPD_SERVERIP              "192.168.0.253"   //
#define    CFG_DHCPD_ROUTERIP              "192.168.0.253"   //
#define    CFG_DHCPD_NETIP                 "255.255.255.0"   //
#define    CFG_DHCPD_DNS                   "192.168.0.253"   //
#define    CFG_DHCPD_DOMAINNAME            "domain"          //
#define  CFG_MODULE_ENABLE_DHCP          true
//*******************************  Configure tcp  ******************************************//
#define     CFG_TCP_REORDER             true    //��Դ����Ŵ�
#define     CFG_TCP_CCBNUM              10      //ռһ�� ָ�� �� struct ClienCB
#define     CFG_TCP_SCBNUM              5       //ռһ�� ָ�� �� struct ServerCB
#define  CFG_MODULE_ENABLE_TCP           true
//*******************************  Configure tpl  ******************************************//
#define CFG_TCP_REORDER  false
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
//*******************************  Configure ftp  ******************************************//
#define    CFG_FTPD_ENABLE                 false             //��δʵ��
#define    CFG_FTPC_ENABLE                 true              //
#define  CFG_MODULE_ENABLE_FTP           true
//*******************************  Configure arp  ******************************************//
#define    CFG_ARP_HASHLEN                 32                //ռ��һ��ָ��
#define  CFG_MODULE_ENABLE_ARP           true
//*******************************  Configure udp  ******************************************//
#define    CFG_UDP_CBNUM                   10                //ռ��һ�� tagUdpCB �ṹ
#define    CFG_UDP_HASHLEN                 4                 //ռ��һ��ָ��
#define    CFG_UDP_PKGMSGLEN               1472              //udp��������
#define    CFG_UDP_BUFLENDEFAULT           0x2000            //8KB
#define  CFG_MODULE_ENABLE_UDP           true
//*******************************  Configure timer  ******************************************//
#define CFG_TIMERS_LIMIT        5        // "��ʱ������",�ɴ����Ķ�ʱ��������������ͼ�ν���Ķ�ʱ����
#define CFG_TIMER_SOUCE_HARD    true     // "Ӳ����ʱ���ṩʱ��Դ",ѡ��ר��Ӳ������tick/tickless��ʱ��Դ
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096                // "��ʼ��ջ�ռ�",�����ʼ������ʹ�õ�ջ�ռ䣬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         15                  // "�¼�������ֵ",�¼�����
#define CFG_EVENT_TYPE_LIMIT    15                  // "�¼���������ֵ",�¼�������
#define CFG_MAINSTACK_LIMIT     4096                // "main����ջ�ߴ�",main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024                // "IDLE�¼�ջ�ߴ�",IDLE�¼����������е�ջ�ߴ磬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_OS_TINY             false               // "tiny���ں�",true=������Դ��ȱ�ĳ��ϣ��ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
//*******************************  Configure Objfile  ******************************************//
#define CFG_OBJECT_LIMIT        8            // "��������ʼֵ"��������Զ�����
#define CFG_HANDLE_LIMIT        8            // "�������ʼֵ"��������Զ�����
//*******************************  Configure MemoryPool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10                  // "�ڴ��������ֵ",
//*******************************  Configure cpu_peri_uart  ******************************************//
#define     CFG_UART1_SENDBUF_LEN            32                            // "UART1���ͻ��λ�������С",
#define     CFG_UART1_RECVBUF_LEN            32                            // "UART1���ջ��λ�������С",
#define     CFG_UART1_DMABUF_LEN             32                            // "UART1 DMA��������С",
#define     CFG_UART2_SENDBUF_LEN            32                            // "UART2���ͻ��λ�������С",
#define     CFG_UART2_RECVBUF_LEN            32                            // "UART2���ջ��λ�������С",
#define     CFG_UART2_DMABUF_LEN             32                            // "UART2 DMA��������С",
#define     CFG_UART3_SENDBUF_LEN            32                            // "UART3���ͻ��λ�������С",
#define     CFG_UART3_RECVBUF_LEN            32                            // "UART3���ջ��λ�������С",
#define     CFG_UART3_DMABUF_LEN             32                            // "UART3 DMA��������С",
#define     CFG_UART4_SENDBUF_LEN            32                            // "UART4���ͻ��λ�������С",
#define     CFG_UART4_RECVBUF_LEN            32                            // "UART4���ջ��λ�������С",
#define     CFG_UART4_DMABUF_LEN             32                            // "UART4 DMA��������С",
#define     CFG_UART5_SENDBUF_LEN            32                            // "UART5���ͻ��λ�������С",
#define     CFG_UART5_RECVBUF_LEN            32                            // "UART5���ջ��λ�������С",
#define     CFG_UART5_DMABUF_LEN             32                            // "UART5 DMA��������С",
#define     CFG_UART6_SENDBUF_LEN            32                            // "UART6���ͻ��λ�������С",
#define     CFG_UART6_RECVBUF_LEN            32                            // "UART6���ջ��λ�������С",
#define     CFG_UART6_DMABUF_LEN             32                            // "UART6 DMA��������С",
#define     CFG_UART1_ENABLE                 true                          // "�Ƿ�ʹ��UART1",
#define     CFG_UART2_ENABLE                 false                         // "�Ƿ�ʹ��UART2",
#define     CFG_UART3_ENABLE                 false                         // "�Ƿ�ʹ��UART3",
#define     CFG_UART4_ENABLE                 false                         // "�Ƿ�ʹ��UART4",
#define     CFG_UART5_ENABLE                 false                         // "�Ƿ�ʹ��UART5",
#define     CFG_UART6_ENABLE                 false                         // "�Ƿ�ʹ��UART6",
//*******************************  Configure netconfig  ******************************************//
#define    CFG_STATIC_IP                   true              //
#define    CFG_SELECT_NETCARD              "STM32F4_ETH"     //������ѡ�е��������������õ�������ͬ
#define    CFG_MY_IPV4                     "192.168.0.179"   //
#define    CFG_MY_SUBMASK                  "255.255.255.0"   //
#define    CFG_MY_GATWAY                   "192.168.0.1"     //
#define    CFG_MY_DNS                      "192.168.0.1"     //
//*******************************  Configure lan8720  ******************************************//
#define    CFG_LAN8720_PHY_ADDRESS         0                 //phy�������ַ��Ӳ��һ�����Ϊ 0
//*******************************  Configure cpu_peri_eth  ******************************************//
#define    CFG_ETH_NETCARD_NAME            "STM32F4_ETH"     //
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
#define  CFG_CORE_MCLK                   (168*Mhz)         //��Ƶ���ں�Ҫ�ã����붨��

#endif