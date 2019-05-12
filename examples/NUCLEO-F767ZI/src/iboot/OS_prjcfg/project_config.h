/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure component lock  ******************************************//
#define CFG_LOCK_LIMIT          40      //�����������������ź����ͻ�����
//*******************************  Configure component heap  ******************************************//
#define CFG_DYNAMIC_MEM true  //��ʹѡfalseҲ����ʹ��malloc-free�����ڴ浫ʹ���в����� �������û��ֲᡷ�ڴ�����½�
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10      //�����豸����
//*******************************  Configure component stdio  ******************************************//
#define     CFG_STDIO_RUN_MODE               (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW)//
#define     CFG_STDIO_IN_NAME                "/dev/UART3"      //
#define     CFG_STDIO_OUT_NAME               "/dev/UART3"      //
#define     CFG_STDIO_ERR_NAME               "/dev/UART3"      //
//*******************************  Configure cpu_peri_component uart  ******************************************//
#define CFG_UART1_SENDBUF_LEN       64      //
#define CFG_UART1_RECVBUF_LEN       64      //
#define CFG_UART1_DMABUF_LEN        64      //
#define CFG_UART2_SENDBUF_LEN       64      //
#define CFG_UART2_RECVBUF_LEN       64      //
#define CFG_UART2_DMABUF_LEN        64      //
#define CFG_UART3_SENDBUF_LEN       64      //
#define CFG_UART3_RECVBUF_LEN       64      //
#define CFG_UART3_DMABUF_LEN        64      //
#define CFG_UART4_SENDBUF_LEN       64      //
#define CFG_UART4_RECVBUF_LEN       64      //
#define CFG_UART4_DMABUF_LEN        64      //
#define CFG_UART5_SENDBUF_LEN       64      //
#define CFG_UART5_RECVBUF_LEN       64      //
#define CFG_UART5_DMABUF_LEN        64      //
#define CFG_UART6_SENDBUF_LEN       64      //
#define CFG_UART6_RECVBUF_LEN       64      //
#define CFG_UART6_DMABUF_LEN        64      //
#define CFG_UART7_SENDBUF_LEN       64      //
#define CFG_UART7_RECVBUF_LEN       64      //
#define CFG_UART7_DMABUF_LEN        64      //
#define CFG_UART8_SENDBUF_LEN       64      //
#define CFG_UART8_RECVBUF_LEN       64      //
#define CFG_UART8_DMABUF_LEN        64      //
#define CFG_UART1_ENABLE           true        //
#define CFG_UART2_ENABLE           false       //
#define CFG_UART3_ENABLE           true       //
#define CFG_UART4_ENABLE           false       //
#define CFG_UART5_ENABLE           false       //
#define CFG_UART6_ENABLE           false       //
#define CFG_UART7_ENABLE           false       //
#define CFG_UART8_ENABLE           false       //
//*******************************  Configure shell  ******************************************//
#define     CFG_ADD_ROUTINE_SHELL            true              //
#define     CFG_ADD_EXPAND_SHELL             true              //
#define     CFG_ADD_GLOBAL_FUN               false             //
#define     CFG_SHOW_ADD_SHEELL              true              //
//*******************************  Configure component ymodem  ******************************************//
#define     CFG_YMODEM_BUF_NUM               32                //ÿ��1024�ֽ�
#define     CFG_YMODEM_PKG_TIMEOUT           (15*1000*1000)    //΢��
#define     CFG_YMODEM_TIMEOUT               (300*1000*1000)   //΢��
#define     CFG_YMODEM_DEVNAME               "std"             //"           //","ymodem�����豸��std��ʾʹ�ñ�׼��������豸"
//*******************************  Configure component kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096    //�����ʼ������ʹ�õ�ջ�ռ�һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         15      //�¼�����
#define CFG_EVENT_TYPE_LIMIT    15      //�¼�������
#define CFG_MAINSTACK_LIMIT     4096    //main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024    //IDLE�¼����������е�ջ�ߴ�һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_OS_TINY             false   //true=������Դ��ȱ�ĳ����ں˻�ü������¼����������¼�����ʱ��ͳ�ơ�
//*******************************  Configure os memory poll  ******************************************//
#define CFG_MEMPOOL_LIMIT       10      //������10���ڴ��
//*******************************  Configure component loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN     //EN_DIRECT_RUN=ֱ�Ӵ�flash�����У�EN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL      //�Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define CFG_IBOOT_VERSION       01        //Iboot�����汾��
//*******************************  Configure cpu driver iic  ******************************************//
#define CFG_IIC_BUF_LEN           32       //����IIC��������С
#define CFG_IIC1_ENABLE           false     //
#define CFG_IIC2_ENABLE           false     //
#define CFG_IIC3_ENABLE           false     //
#define CFG_IIC4_ENABLE           false     //
//*******************************  Configure component tcpip  ******************************************//
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
#define     CFG_TFTP_PATHDEFAULT                "/"         //TFTPĬ�Ϲ���Ŀ¼
#define     CFG_DHCP_ENABLE                  true              //
#define     CFG_DHCPD_ENABLE                 false             //
#define     CFG_DHCPC_ENABLE                 true              //
#define     CFG_DHCP_RENEWTIME               3600              //����
#define     CFG_DHCP_REBINDTIME              3600              //����
#define     CFG_DHCP_LEASETIME               3600              //����
#define     CFG_DHCPD_IPNUM                  0x40              //64
#define     CFG_DHCPD_IPSTART                "192.168.0.2"     //
#define     CFG_DHCPD_SERVERIP               "192.168.0.253"   //"  //",
#define     CFG_DHCPD_ROUTERIP               "192.168.0.253"   //"  //",
#define     CFG_DHCPD_NETIP                  "255.255.255.0"   //"  //",
#define     CFG_DHCPD_DNS                    "192.168.0.253"   //"  //",
#define     CFG_DHCPD_DOMAINNAME             "domain"          //"        //",
#define     CN_PPP_ENABLE                    true              //�Ƿ���Ҫ ppp Э��
#define     CFG_MYCONFIG6                                      //�ҵ�����
#define     CFG_MYCONFIG7                    CN_CONFIG         //�ҵ�����
//*******************************  Configure cpu driver eth  ******************************************//
#define CFG_MAC_ADDR0           00           //
#define CFG_MAC_ADDR1           02           //
#define CFG_MAC_ADDR2           03           //
#define CFG_MAC_ADDR3           04           //
#define CFG_MAC_ADDR4           05           //
#define CFG_MAC_ADDR5           06           //
#define CFG_ETH_LOOP_CYCLE      1000         //
#define CFG_ETH_LOOP_ENABLE     true        //
#define CFG_ETH_DEV_NAME        "STM32F7_ETH"//
//*******************************  Configure board driver net config  ******************************************//
#define     CFG_STATIC_IP                    false              //true=ʹ�þ�̬IPfalse=��̬IP
#define     CFG_NETCARD_NAME                 "NUCLEO-F767ZI_ETH"//
#define     CFG_MY_IPV4                      "192.168.0.179"   //
#define     CFG_MY_SUBMASK                   "255.255.255.0"   //
#define     CFG_MY_GATWAY                    "192.168.0.1"     //
#define     CFG_MY_DNS                       "192.168.0.1"     //
//*******************************  Configure cpu drive inner flash  ******************************************//
#define     CFG_EFLASH_PART_START            6                 //������ʼ
#define     CFG_EFLASH_PART_END              -1                //��������
#define     CFG_EFLASH_PART_OPTION           0                 //����ѡ��
#define     CFG_EFLASH_FSMOUNT_NAME          "XIP-APP"       //�谲װ���ļ�ϵͳ��mount������
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (216*Mhz)         //��Ƶ���ں�Ҫ�ã����붨��

#endif
