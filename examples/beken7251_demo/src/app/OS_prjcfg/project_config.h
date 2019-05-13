/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

#define CFG_RUNMODE_BAREAPP    1
//*******************************  Configure System: lock  ******************************************//
#define CFG_LOCK_LIMIT          40       // "��������",�������������������ź����ͻ�����
//*******************************  Configure System: heap  ******************************************//
#define CFG_DYNAMIC_MEM true   // "ȫ���ܶ�̬����",��ʹѡfalse��Ҳ����ʹ��malloc-free�����ڴ棬��ʹ���в����� �������û��ֲᡷ�ڴ�����½�
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10       // "�豸����",�����豸����
//*******************************  Configure System: stdio  ******************************************//
#define     CFG_STDIO_RUN_MODE               (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW) // "STDIOģʽ",
#define     CFG_STDIO_IN_NAME                "/dev/UART2"       // "��׼�����豸��",
#define     CFG_STDIO_OUT_NAME               "/dev/UART2"       // "��׼����豸��",
#define     CFG_STDIO_ERR_NAME               "/dev/UART2"       // "��׼err����豸��",
//*******************************  Configure cpu_peri_System: uart  ******************************************//
#define CFG_UART1_SENDBUF_LEN       64       // "UART1���ͻ��λ�������С",
#define CFG_UART1_RECVBUF_LEN       64       // "UART1���ջ��λ�������С",
#define CFG_UART1_DMABUF_LEN        64       // "UART1 DMA���λ�������С",
#define CFG_UART2_SENDBUF_LEN       64       // "UART2���ͻ��λ�������С",
#define CFG_UART2_RECVBUF_LEN       64       // "UART2���ջ��λ�������С",
#define CFG_UART2_DMABUF_LEN        64       // "UART2 DMA���λ�������С",
#define CFG_UART3_SENDBUF_LEN       64       // "UART3���ͻ��λ�������С",
#define CFG_UART3_RECVBUF_LEN       64       // "UART3���ջ��λ�������С",
#define CFG_UART3_DMABUF_LEN        64       // "UART3 DMA���λ�������С",
#define CFG_UART4_SENDBUF_LEN       64       // "UART4���ͻ��λ�������С",
#define CFG_UART4_RECVBUF_LEN       64       // "UART4���ջ��λ�������С",
#define CFG_UART4_DMABUF_LEN        64       // "UART4 DMA���λ�������С",
#define CFG_UART1_ENABLE           false         // "�Ƿ�ʹ��UART1",
#define CFG_UART2_ENABLE           true        // "�Ƿ�ʹ��UART2",
#define CFG_UART3_ENABLE           false        // "�Ƿ�ʹ��UART3",
#define CFG_UART4_ENABLE           false        // "�Ƿ�ʹ��UART4",
//*******************************  Configure shell  ******************************************//
#define     CFG_ADD_ROUTINE_SHELL            true               // "�Ƿ���ӳ���shell����"
#define     CFG_ADD_EXPAND_SHELL             true               // "�Ƿ������չshell����"
#define     CFG_ADD_GLOBAL_FUN               false              // "���ȫ�ֺ�����shell"
#define     CFG_SHOW_ADD_SHEELL              true               // "��ʾ�ڱ��봰����ӵ�shell����"
//*******************************  Configure System: tcpip  ******************************************//
#define     CFG_NETPKG_MEMSIZE          0x4000  //
#define     CFG_ARP_HASHLEN             32      //ռ��һ��ָ��
#define     CFG_TPL_PROTONUM            5       //ռ��һ�� tagTplProtoItem �ṹ
#define     CFG_UDP_ENABLE              true    //
#define     CFG_UDP_CBNUM               10      //ռ��һ�� tagUdpCB �ṹ
#define     CFG_UDP_HASHLEN             4       //ռ��һ��ָ��
//#define     CFG_TCP_ENABLE              true    //
//#define     CFG_TCP_REORDER             true    //��Դ����Ŵ�
//#define     CFG_TCP_CCBNUM              10      //ռһ�� ָ�� �� tagCCB
//#define     CFG_TCP_SCBNUM              5       //ռһ�� ָ�� �� tagSCB
#define     CFG_SOCKET_NUM              10      //ռһ�� tagItem �ṹ
#define     CFG_TELNET_ENABLE           true    //
#define     CFG_TELNETD_ENABLE          true    //
#define     CFG_TELNETC_ENABLE          false   //
#define     CFG_FTP_ENABLE              true    //
#define     CFG_FTPD_ENABLE             true    //
#define     CFG_FTPC_ENABLE             false   //
#define     CFG_TFTP_ENABLE             true    //
#define     CFG_TFTPD_ENABLE            false   //��δʵ��
#define     CFG_TFTPC_ENABLE            true    //
#define     CFG_TFTP_PATHDEFAULT       "/"   //TFTPĬ�Ϲ���Ŀ¼
#define     CFG_DHCP_ENABLE             true    //
#define     CFG_DHCPD_ENABLE            false   //
#define     CFG_DHCPC_ENABLE            true    //
#define     CFG_DHCP_RENEWTIME          3600    //����
#define     CFG_DHCP_REBINDTIME         3600    //����
#define     CFG_DHCP_LEASETIME          3600    //����
#define     CFG_DHCPD_IPNUM             0x40    //64
#define     CFG_DHCPD_IPSTART           "192.168.0.2"    //
#define     CFG_DHCPD_SERVERIP          "192.168.0.253"  //
#define     CFG_DHCPD_ROUTERIP          "192.168.0.253"  //
#define     CFG_DHCPD_NETIP             "255.255.255.0"  //
#define     CFG_DHCPD_DNS               "192.168.0.253"  //
#define     CFG_DHCPD_DOMAINNAME       "domain"        //
#define     CFG_PPP_ENABLE             false         //�Ƿ���Ҫ ppp Э��
//*******************************  Configure System: kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096     // "��ʼ��ջ�ռ�",�����ʼ������ʹ�õ�ջ�ռ䣬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         20       // "�¼�������ֵ",�¼�����
#define CFG_EVENT_TYPE_LIMIT    20       // "�¼���������ֵ",�¼�������
#define CFG_MAINSTACK_LIMIT     4096     // "main����ջ�ߴ�",main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024     // "IDLE�¼�ջ�ߴ�",IDLE�¼����������е�ջ�ߴ磬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_OS_TINY             false    // "tiny���ں�",true=������Դ��ȱ�ĳ��ϣ��ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
//*******************************  Configure System: memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10       // "�ڴ������",������10���ڴ��
//*******************************  Configure System: loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN      // EN_DIRECT_RUN=ֱ�Ӵ�flash�����У�EN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL       // �Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define CFG_IBOOT_VERSION       01         // Iboot�����汾��
#define CFG_IBOOT_UPDATE_NAME      "/yaf2/iboot.bin"            // "Iboot����"
//*******************************  Configure cpu onchip MAC  ******************************************//
#define CFG_MAC_ADDR0           00           //
#define CFG_MAC_ADDR1           00           //
#define CFG_MAC_ADDR2           00           //
#define CFG_MAC_ADDR3           00           //
#define CFG_MAC_ADDR4           00           //
#define CFG_MAC_ADDR5           00           //
#define CFG_ETH_LOOP_CYCLE      1000         //
#define CFG_ETH_LOOP_ENABLE     true        //
#define CFG_ETH_DEV_NAME        "STM32F7_ETH"//
//*******************************  Configure network config  ******************************************//
#define     CFG_STATIC_IP                    false             //true=ʹ�þ�̬IPfalse=��̬IP
#define     CFG_NETCARD_NAME                 "BK7251_WIFI"     //
#define     CFG_MY_IPV4                      "192.168.0.179"   //
#define     CFG_MY_SUBMASK                   "255.255.255.0"   //
#define     CFG_MY_GATWAY                    "192.168.0.1"     //
#define     CFG_MY_DNS                       "192.168.0.1"     //
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (180*Mhz)         //��Ƶ���ں�Ҫ�ã����붨��
#define  CFG_BARE_APP                    (1U)
#endif
