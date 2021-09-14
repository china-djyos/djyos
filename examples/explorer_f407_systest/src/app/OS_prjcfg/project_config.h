/****************************************************
 *  Automatically-generated file. Do not edit!  *
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

#define CFG_RUNMODE_BAREAPP    1
//*******************************  Configure arp  ******************************************//
#define     CFG_ARP_HASHLEN             32      //ռ��һ��ָ��
#define  CFG_MODULE_ENABLE_ARP           true
//*******************************  Configure black box  ******************************************//
#define  CFG_MODULE_ENABLE_BLACK_BOX     true
//*******************************  Configure board config  ******************************************//
#define  CFG_MODULE_ENABLE_BOARD_CONFIG  true
//*******************************  Configure cpu drive inner flash  ******************************************//
#define CFG_EFLASH_PART_FORMAT     false      //����ѡ��,�Ƿ���Ҫ������оƬ��
#define  CFG_MODULE_ENABLE_CPU_DRIVE_INNER_FLASH  true
//*******************************  Configure cpu onchip dma  ******************************************//
#define  CFG_MODULE_ENABLE_CPU_ONCHIP_DMA  true
//*******************************  Configure cpu onchip iic  ******************************************//
#define CFG_I2C_BUF_LEN         128         //
#define CFG_I2C1_ENABLE         false       //
#define CFG_I2C2_ENABLE         false       //
#define CFG_I2C3_ENABLE         false       //
#define  CFG_MODULE_ENABLE_CPU_ONCHIP_IIC  true
//*******************************  Configure cpu onchip MAC  ******************************************//
#define CFG_ETH_NETCARD_NAME    "STM32F4_ETH"   //
#define CFG_ETH_LOOP_CYCLE      1000         //�ж�ģʽ������д
#define CFG_ETH_LOOP_ENABLE     true            //
#define CFG_ETH_HARD_MAC_ADDR   true            //
#define CFG_ETH_MAC_ADDR0      00           //��ѡ��"Ӳ������Mac��ַ"��������д
#define CFG_ETH_MAC_ADDR1      02           //��ѡ��"Ӳ������Mac��ַ"��������д
#define CFG_ETH_MAC_ADDR2      03           //��ѡ��"Ӳ������Mac��ַ"��������д
#define CFG_ETH_MAC_ADDR3      04           //��ѡ��"Ӳ������Mac��ַ"��������д
#define CFG_ETH_MAC_ADDR4      05           //��ѡ��"Ӳ������Mac��ַ"��������д
#define CFG_ETH_MAC_ADDR5      06           //��ѡ��"Ӳ������Mac��ַ"��������д
#define  CFG_MODULE_ENABLE_CPU_ONCHIP_MAC  true
//*******************************  Configure cpu onchip gpio  ******************************************//
#define  CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO  true
//*******************************  Configure cpu onchip timer  ******************************************//
#define  CFG_MODULE_ENABLE_CPU_ONCHIP_TIMER  true
//*******************************  Configure cpu onchip uart  ******************************************//
#define CFG_UART1_SENDBUF_LEN            32                 //
#define CFG_UART1_RECVBUF_LEN            32                 //
#define CFG_UART1_DMABUF_LEN             32                 //
#define CFG_UART2_SENDBUF_LEN            32                 //
#define CFG_UART2_RECVBUF_LEN            32                 //
#define CFG_UART2_DMABUF_LEN             32                 //
#define CFG_UART3_SENDBUF_LEN            32                 //
#define CFG_UART3_RECVBUF_LEN            32                 //
#define CFG_UART3_DMABUF_LEN             32                 //
#define CFG_UART4_SENDBUF_LEN            32                 //
#define CFG_UART4_RECVBUF_LEN            32                 //
#define CFG_UART4_DMABUF_LEN             32                 //
#define CFG_UART5_SENDBUF_LEN            32                 //
#define CFG_UART5_RECVBUF_LEN            32                 //
#define CFG_UART5_DMABUF_LEN             32                 //
#define CFG_UART6_SENDBUF_LEN            32                 //
#define CFG_UART6_RECVBUF_LEN            32                 //
#define CFG_UART6_DMABUF_LEN             32                 //
#define CFG_UART1_ENABLE                 true               //
#define CFG_UART2_ENABLE                 false              //
#define CFG_UART3_ENABLE                 false              //
#define CFG_UART4_ENABLE                 false              //
#define CFG_UART5_ENABLE                 false              //
#define CFG_UART6_ENABLE                 false              //
#define  CFG_MODULE_ENABLE_CPU_ONCHIP_UART  true
//*******************************  Configure debug information  ******************************************//
#define  CFG_MODULE_ENABLE_DEBUG_INFORMATION  true
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10      //�����豸����
#define  CFG_MODULE_ENABLE_DEVICE_FILE_SYSTEM  true
//*******************************  Configure dhcp  ******************************************//
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
#define  CFG_MODULE_ENABLE_DHCP          true
//*******************************  Configure djybus  ******************************************//
#define  CFG_MODULE_ENABLE_DJYBUS        true
//*******************************  Configure emflash insatall xip  ******************************************//
#define CFG_EFLASH_XIP_PART_START      6          //������ʼ����д��ţ���Ŵ�0��ʼ����
#define CFG_EFLASH_XIP_PART_END        -1         //����������-1��ʾ���һ��
#define CFG_EFLASH_XIP_PART_FORMAT     false      //����ѡ��,�Ƿ���Ҫ��ʽ���÷�����
#define  CFG_MODULE_ENABLE_EMFLASH_INSATALL_XIP  true
//*******************************  Configure ethernet phy lan8720  ******************************************//
#define CFG_LAN8720_PHY_ADDRESS     0   //phy�������ַ��Ӳ��һ�����Ϊ 0
#define  CFG_MODULE_ENABLE_ETHERNET_PHY_LAN8720  true
//*******************************  Configure file system  ******************************************//
#define CFG_CLIB_BUFFERSIZE            512      //
#define  CFG_MODULE_ENABLE_FILE_SYSTEM   true
//*******************************  Configure ftp  ******************************************//
#define     CFG_FTPD_ENABLE            false   //��δʵ��
#define     CFG_FTPC_ENABLE            true    //
#define  CFG_MODULE_ENABLE_FTP           true
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true  //��ʹѡfalseҲ����ʹ��malloc-free�����ڴ浫ʹ���в����� �������û��ֲᡷ�ڴ�����½�
#define  CFG_MODULE_ENABLE_HEAP          true
//*******************************  Configure icmp  ******************************************//
#define  CFG_MODULE_ENABLE_ICMP          true
//*******************************  Configure iicbus  ******************************************//
#define  CFG_MODULE_ENABLE_IICBUS        true
//*******************************  Configure int  ******************************************//
#define  CFG_MODULE_ENABLE_INT           true
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096    //�����ʼ������ʹ�õ�ջ�ռ�һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         15      //�¼�����
#define CFG_EVENT_TYPE_LIMIT    15      //�¼�������
#define CFG_MAINSTACK_LIMIT     4096    //main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024    //IDLE�¼����������е�ջ�ߴ�һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_OS_TINY             false   //true=������Դ��ȱ�ĳ����ں˻�ü������¼����������¼�����ʱ��ͳ�ơ�
#define  CFG_MODULE_ENABLE_KERNEL        true
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8   //������Զ�����
#define CFG_HANDLE_LIMIT        8   //������Զ�����
#define  CFG_MODULE_ENABLE_KERNEL_OBJECT_SYSTEM  true
//*******************************  Configure loader  ******************************************//
#define CFG_UPDATEIBOOT_EN      false       //
#define  CFG_APP_RUNMODE  CN_DIRECT_RUN     //CN_DIRECT_RUN=ֱ�Ӵ�flash�����У�CN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL      //�Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define CFG_IBOOT_VERSION       01        //Iboot�����汾��
#define CFG_IBOOT_UPDATE_NAME      "/yaf2/iboot.bin"           //
#define  CFG_MODULE_ENABLE_LOADER        true
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40      //�����������������ź����ͻ�����
#define  CFG_MODULE_ENABLE_LOCK          true
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10      //
#define  CFG_MODULE_ENABLE_MEMORY_POOL   true
//*******************************  Configure message queue  ******************************************//
#define  CFG_MODULE_ENABLE_MESSAGE_QUEUE true
//*******************************  Configure multiplex  ******************************************//
#define  CFG_MODULE_ENABLE_MULTIPLEX     true
//*******************************  Configure network config  ******************************************//
#define CFG_STATIC_IP       true            //
#define CFG_SELECT_NETCARD  "STM32F4_ETH"   //������ѡ�е��������������õ�������ͬ
#define CFG_MY_IPV4         "192.168.0.179" //
#define CFG_MY_SUBMASK      "255.255.255.0" //
#define CFG_MY_GATWAY       "192.168.0.1"   //
#define CFG_MY_DNS          "192.168.0.1"   //
#define  CFG_MODULE_ENABLE_NETWORK_CONFIG  true
//*******************************  Configure ring buffer and line buffer  ******************************************//
#define  CFG_MODULE_ENABLE_RING_BUFFER_AND_LINE_BUFFER  true
//*******************************  Configure router  ******************************************//
#define CFG_IP_STRMAX           20 //���·����Ŀ��
#define  CFG_MODULE_ENABLE_ROUTER        true
//*******************************  Configure shell  ******************************************//
#define CFG_SHELL_STACK            0x1000      //
#define CFG_ADD_ROUTINE_SHELL      true        //
#define CFG_ADD_EXPAND_SHELL       true        //
#define CFG_ADD_GLOBAL_FUN         false       //
#define CFG_SHOW_ADD_SHEELL        true        //
#define  CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure Software Timers  ******************************************//
#define CFG_TIMERS_LIMIT        5       //�ɴ����Ķ�ʱ��������������ͼ�ν���Ķ�ʱ����
#define CFG_TIMER_SOUCE_HARD    true    //ѡ��ר��Ӳ������tick/tickless��ʱ��Դ
#define  CFG_MODULE_ENABLE_SOFTWARE_TIMERS  true
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_STDIN_MULTI      true         //
#define CFG_STDIO_STDOUT_FOLLOW    true         //
#define CFG_STDIO_STDERR_FOLLOW    true         //
#define CFG_STDIO_FLOAT_PRINT      true         //
#define CFG_STDIO_STDIOFILE        true         //
#define CFG_STDIO_IN_NAME              "/dev/UART1"    //
#define CFG_STDIO_OUT_NAME             "/dev/UART1"    //
#define CFG_STDIO_ERR_NAME             "/dev/UART1"    //
#define  CFG_MODULE_ENABLE_STDIO         true
//*******************************  Configure STM32 commond code  ******************************************//
#define  CFG_MODULE_ENABLE_STM32_COMMOND_CODE  true
//*******************************  Configure tcp  ******************************************//
#define     CFG_TCP_REORDER             true    //��Դ����Ŵ�
#define     CFG_TCP_CCBNUM              10      //ռһ�� ָ�� �� struct ClienCB
#define     CFG_TCP_SCBNUM              5       //ռһ�� ָ�� �� struct ServerCB
#define  CFG_MODULE_ENABLE_TCP           true
//*******************************  Configure tcpip  ******************************************//
#define     CFG_NETPKG_MEMSIZE          0x4000  //
#define  CFG_MODULE_ENABLE_TCPIP         true
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8      //����ʱ���Ƕ�8��
#define  CFG_MODULE_ENABLE_TIME          true
//*******************************  Configure tpl  ******************************************//
#define     CFG_TPL_PROTONUM            5       //ռ��һ�� tagTplProtoItem �ṹ
#define  CFG_MODULE_ENABLE_TPL           true
//*******************************  Configure uart device file  ******************************************//
#define  CFG_MODULE_ENABLE_UART_DEVICE_FILE  true
//*******************************  Configure udp  ******************************************//
#define     CFG_UDP_CBNUM               10      //ռ��һ�� tagUdpCB �ṹ
#define     CFG_UDP_HASHLEN             4       //ռ��һ��ָ��
#define     CFG_UDP_PKGMSGLEN        1472       //udp��������
#define     CFG_UDP_BUFLENDEFAULT    0x2000     //8KB
#define  CFG_MODULE_ENABLE_UDP           true
//*******************************  Configure xip iboot file system  ******************************************//
#define  CFG_MODULE_ENABLE_XIP_IBOOT_FILE_SYSTEM  true
//*******************************  Configure yaf2 file system  ******************************************//
#define CFG_YAF_ECC                       0                //YAF�ļ�ϵͳ�ļ�ʹ���豸ECC���ܡ�0��ʹ��ecc��YAF2_ENABLE_DEVICE_ECCʹ��ecc
#define CFG_YAF_INSTALL_OPTION           MS_INSTALLCREAT            //YAF�ļ�ϵͳ��װѡ���װʱ�Ƿ��ʽ�������ļ�ϵͳ��
#define CFG_YAF_MOUNT_POINT              "yaf2"               //YAF�ļ�ϵͳ��װĿ¼
#define  CFG_MODULE_ENABLE_YAF2_FILE_SYSTEM  true
//*******************************  Configure STM32 commond code  ******************************************//
#define  CFG_MODULE_ENABLE_STM32_COMMOND_CODE  true
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (168*Mhz)         //��Ƶ���ں�Ҫ�ã����붨��

#endif
