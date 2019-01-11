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
//*******************************  Configure DevFile  ******************************************//
#define CFG_DEVFILE_LIMIT       10      //�����豸����
//*******************************  Configure stdio  ******************************************//
#define     CFG_STDIO_RUN_MODE               (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW)//
#define     CFG_STDIO_IN_NAME                "/dev/UART3"      //
#define     CFG_STDIO_OUT_NAME               "/dev/UART3"      //
#define     CFG_STDIO_ERR_NAME               "/dev/UART1"      //
//*******************************  Configure cpu_peri_uart  ******************************************//
#define     CFG_UART1_SENDBUF_LEN            64                //
#define     CFG_UART1_RECVBUF_LEN            64                //
#define     CFG_UART1_DMABUF_LEN             64                //
#define     CFG_UART2_SENDBUF_LEN            64                //
#define     CFG_UART2_RECVBUF_LEN            64                //
#define     CFG_UART2_DMABUF_LEN             64                //
#define     CFG_UART3_SENDBUF_LEN            64                //
#define     CFG_UART3_RECVBUF_LEN            64                //
#define     CFG_UART3_DMABUF_LEN             64                //
#define     CFG_UART4_SENDBUF_LEN            64                //
#define     CFG_UART4_RECVBUF_LEN            64                //
#define     CFG_UART4_DMABUF_LEN             64                //
#define     CFG_UART5_SENDBUF_LEN            64                //
#define     CFG_UART5_RECVBUF_LEN            64                //
#define     CFG_UART5_DMABUF_LEN             64                //
#define     CFG_UART6_SENDBUF_LEN            64                //
#define     CFG_UART6_RECVBUF_LEN            64                //
#define     CFG_UART6_DMABUF_LEN             64                //
#define     CFG_UART7_SENDBUF_LEN            64                //
#define     CFG_UART7_RECVBUF_LEN            64                //
#define     CFG_UART7_DMABUF_LEN             64                //
#define     CFG_UART8_SENDBUF_LEN            64                //
#define     CFG_UART8_RECVBUF_LEN            64                //
#define     CFG_UART8_DMABUF_LEN             64                //
#define     CFG_UART1_ENABLE                 false             //
#define     CFG_UART2_ENABLE                 false             //
#define     CFG_UART3_ENABLE                 true              //
#define     CFG_UART4_ENABLE                 false             //
#define     CFG_UART5_ENABLE                 false             //
#define     CFG_UART6_ENABLE                 false             //
#define     CFG_UART7_ENABLE                 false             //
#define     CFG_UART8_ENABLE                 false             //
//*******************************  Configure tcpip  ******************************************//
#define     CFG_NETPKG_MEMSIZE          0x4000  //
#define     CFG_ARP_HASHLEN             32      //ռ��һ��ָ��
#define     CFG_TPL_PROTONUM            5       //ռ��һ�� tagTplProtoItem �ṹ
#define     CFG_UDP_ENABLE              true    //
#define     CFG_UDP_CBNUM               10      //ռ��һ�� tagUdpCB �ṹ
#define     CFG_UDP_HASHLEN             4       //ռ��һ��ָ��
#define     CFG_TCP_ENABLE              true    //
#define     CFG_TCP_REORDER             true    //��Դ����Ŵ�
#define     CFG_TCP_CCBNUM              10      //ռһ�� ָ�� �� tagCCB
#define     CFG_TCP_SCBNUM              5       //ռһ�� ָ�� �� tagSCB
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
#define     CFG_TFTP_PATHDEFAULT       "/efs/"   //TFTPĬ�Ϲ���Ŀ¼
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
#define     CFG_DHCPD_DOMAINNAME        "djyos.com"      //
#define     CN_PPP_ENABLE             false         //�Ƿ���Ҫ ppp Э��
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096    //�����ʼ������ʹ�õ�ջ�ռ�һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         20      //�¼�����
#define CFG_EVENT_TYPE_LIMIT    20      //�¼�������
#define CFG_MAINSTACK_LIMIT     4096    //main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024    //IDLE�¼����������е�ջ�ߴ�һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_OS_TINY             false   //true=������Դ��ȱ�ĳ����ں˻�ü������¼����������¼�����ʱ��ͳ�ơ�
//*******************************  Configure MemoryPool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10      //������10���ڴ��
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8      //����ʱ���Ƕ�8��
//*******************************  Configure loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN     //EN_DIRECT_RUN=ֱ�Ӵ�flash�����У�EN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define  CFG_IBOOT_CRC   EN_NO_APP_CRC      //�Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define CFG_APP_FILENAME        ""              //CFG_APP_RUNMODE==EN_FORM_FILE�����������ļ���
#define CFG_IBOOT_VERSION       "V1.0.0"        //Iboot�����汾��
#define CFG_IBOOT_UPDATE_NAME   "iboot.bin"     //iboot�����ļ������˶����ã��Ժ�ĵ�
#define CFG_IBOOT_UPDATE_PATH   "\\"     //iboot����·�������˶����ã��Ժ�ĵ�
//*******************************  Configure cpu_peri_iic  ******************************************//
#define CFG_IIC_BUF_LEN           32       //����IIC��������С
#define CFG_IIC1_ENABLE           false     //
#define CFG_IIC2_ENABLE           false     //
#define CFG_IIC3_ENABLE           false     //
#define CFG_IIC4_ENABLE           false     //
//*******************************  Configure cpu_peri_eth  ******************************************//
#define CFG_MAC_ADDR0           00           //
#define CFG_MAC_ADDR1           00           //
#define CFG_MAC_ADDR2           00           //
#define CFG_MAC_ADDR3           00           //
#define CFG_MAC_ADDR4           00           //
#define CFG_MAC_ADDR5           00           //
#define CFG_ETH_LOOP_CYCLE      1000         //
#define CFG_ETH_LOOP_ENABLE     true        //
#define CFG_ETH_DEV_NAME        "STM32F7_ETH"//
//*******************************  Configure netconfig  ******************************************//
#define     CFG_STATIC_IP                    true              //true=ʹ�þ�̬IP��false=��̬IP
#define     CFG_NETCARD_NAME                 "NUCLEO-F767ZI_ETH"//
#define     CFG_MY_IPV4                      "192.168.0.179"   //
#define     CFG_MY_SUBMASK                   "255.255.255.0"   //
#define     CFG_MY_GATWAY                    "192.168.0.1"     //
#define     CFG_MY_DNS                       "192.168.0.1"     //
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (216*Mhz)         //��Ƶ���ں�Ҫ�ã����붨��

#endif
