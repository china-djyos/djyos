/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure component lock  ******************************************//
#define CFG_LOCK_LIMIT          40      //定义锁的数量包含信号量和互斥量
//*******************************  Configure component heap  ******************************************//
#define CFG_DYNAMIC_MEM true  //即使选false也允许使用malloc-free分配内存但使用有差别详见 《……用户手册》内存分配章节
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10      //定义设备数量
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
#define     CFG_YMODEM_BUF_NUM               32                //每包1024字节
#define     CFG_YMODEM_PKG_TIMEOUT           (15*1000*1000)    //微秒
#define     CFG_YMODEM_TIMEOUT               (300*1000*1000)   //微秒
#define     CFG_YMODEM_DEVNAME               "std"             //"           //","ymodem传输设备，std表示使用标准输入输出设备"
//*******************************  Configure component kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096    //定义初始化过程使用的栈空间一般按默认值就可以了
#define CFG_EVENT_LIMIT         15      //事件数量
#define CFG_EVENT_TYPE_LIMIT    15      //事件类型数
#define CFG_MAINSTACK_LIMIT     4096    //main函数运行所需的栈尺寸
#define CFG_IDLESTACK_LIMIT     1024    //IDLE事件处理函数运行的栈尺寸一般按默认值就可以了
#define CFG_OS_TINY             false   //true=用于资源紧缺的场合内核会裁剪掉：事件类型名字事件处理时间统计。
//*******************************  Configure os memory poll  ******************************************//
#define CFG_MEMPOOL_LIMIT       10      //允许建立10个内存池
//*******************************  Configure component loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN     //EN_DIRECT_RUN=直接从flash中运行；EN_FORM_FILE=从文件系统加载到内存运行，
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL      //是否对APP程序进行CRC校验，需要极快速启动才不需要CRC校验
#define CFG_IBOOT_VERSION       01        //Iboot发布版本号
//*******************************  Configure cpu driver iic  ******************************************//
#define CFG_IIC_BUF_LEN           32       //配置IIC缓冲区大小
#define CFG_IIC1_ENABLE           false     //
#define CFG_IIC2_ENABLE           false     //
#define CFG_IIC3_ENABLE           false     //
#define CFG_IIC4_ENABLE           false     //
//*******************************  Configure component tcpip  ******************************************//
#define     CFG_NETPKG_MEMSIZE               0x4000            //
#define     CFG_ARP_HASHLEN                  32                //占用一个指针
#define     CFG_TPL_PROTONUM                 5                 //占用一个 tagTplProtoItem 结构
#define     CFG_UDP_ENABLE                   true              //
#define     CFG_UDP_CBNUM                    10                //占用一个 tagUdpCB 结构
#define     CFG_UDP_HASHLEN                  4                 //占用一个指针
#define     CFG_TCP_ENABLE                   true              //
#define     CFG_TCP_REORDER                  true              //资源充足才打开
#define     CFG_TCP_CCBNUM                   10                //占一个 指针 和 tagCCB
#define     CFG_TCP_SCBNUM                   5                 //占一个 指针 和 tagSCB
#define     CFG_SOCKET_NUM                   10                //占一个 tagItem 结构
#define     CFG_TELNET_ENABLE                true              //
#define     CFG_TELNETD_ENABLE               true              //
#define     CFG_TELNETC_ENABLE               false             //
#define     CFG_FTP_ENABLE                   true              //
#define     CFG_FTPD_ENABLE                  true              //
#define     CFG_FTPC_ENABLE                  false             //
#define     CFG_TFTP_ENABLE                  true              //
#define     CFG_TFTPD_ENABLE                 false             //
#define     CFG_TFTPC_ENABLE                 true              //
#define     CFG_TFTP_PATHDEFAULT                "/"         //TFTP默认工作目录
#define     CFG_DHCP_ENABLE                  true              //
#define     CFG_DHCPD_ENABLE                 false             //
#define     CFG_DHCPC_ENABLE                 true              //
#define     CFG_DHCP_RENEWTIME               3600              //秒数
#define     CFG_DHCP_REBINDTIME              3600              //秒数
#define     CFG_DHCP_LEASETIME               3600              //秒数
#define     CFG_DHCPD_IPNUM                  0x40              //64
#define     CFG_DHCPD_IPSTART                "192.168.0.2"     //
#define     CFG_DHCPD_SERVERIP               "192.168.0.253"   //"  //",
#define     CFG_DHCPD_ROUTERIP               "192.168.0.253"   //"  //",
#define     CFG_DHCPD_NETIP                  "255.255.255.0"   //"  //",
#define     CFG_DHCPD_DNS                    "192.168.0.253"   //"  //",
#define     CFG_DHCPD_DOMAINNAME             "domain"          //"        //",
#define     CN_PPP_ENABLE                    true              //是否需要 ppp 协议
#define     CFG_MYCONFIG6                                      //我的配置
#define     CFG_MYCONFIG7                    CN_CONFIG         //我的配置
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
#define     CFG_STATIC_IP                    false              //true=使用静态IPfalse=动态IP
#define     CFG_NETCARD_NAME                 "NUCLEO-F767ZI_ETH"//
#define     CFG_MY_IPV4                      "192.168.0.179"   //
#define     CFG_MY_SUBMASK                   "255.255.255.0"   //
#define     CFG_MY_GATWAY                    "192.168.0.1"     //
#define     CFG_MY_DNS                       "192.168.0.1"     //
//*******************************  Configure cpu drive inner flash  ******************************************//
#define     CFG_EFLASH_PART_START            6                 //分区起始
#define     CFG_EFLASH_PART_END              -1                //分区结束
#define     CFG_EFLASH_PART_OPTION           0                 //分区选项
#define     CFG_EFLASH_FSMOUNT_NAME          "XIP-APP"       //需安装的文件系统的mount的名字
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (216*Mhz)         //主频，内核要用，必须定义

#endif
