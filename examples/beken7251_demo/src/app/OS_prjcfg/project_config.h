/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

#define CFG_RUNMODE_BAREAPP    1
//*******************************  Configure System: lock  ******************************************//
#define CFG_LOCK_LIMIT          40       // "锁的数量",定义锁的数量，包含信号量和互斥量
//*******************************  Configure System: heap  ******************************************//
#define CFG_DYNAMIC_MEM true   // "全功能动态分配",即使选false，也允许使用malloc-free分配内存，但使用有差别，详见 《……用户手册》内存分配章节
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10       // "设备数量",定义设备数量
//*******************************  Configure System: stdio  ******************************************//
#define     CFG_STDIO_RUN_MODE               (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW) // "STDIO模式",
#define     CFG_STDIO_IN_NAME                "/dev/UART2"       // "标准输入设备名",
#define     CFG_STDIO_OUT_NAME               "/dev/UART2"       // "标准输出设备名",
#define     CFG_STDIO_ERR_NAME               "/dev/UART2"       // "标准err输出设备名",
//*******************************  Configure cpu_peri_System: uart  ******************************************//
#define CFG_UART1_SENDBUF_LEN       64       // "UART1发送环形缓冲区大小",
#define CFG_UART1_RECVBUF_LEN       64       // "UART1接收环形缓冲区大小",
#define CFG_UART1_DMABUF_LEN        64       // "UART1 DMA环形缓冲区大小",
#define CFG_UART2_SENDBUF_LEN       64       // "UART2发送环形缓冲区大小",
#define CFG_UART2_RECVBUF_LEN       64       // "UART2接收环形缓冲区大小",
#define CFG_UART2_DMABUF_LEN        64       // "UART2 DMA环形缓冲区大小",
#define CFG_UART3_SENDBUF_LEN       64       // "UART3发送环形缓冲区大小",
#define CFG_UART3_RECVBUF_LEN       64       // "UART3接收环形缓冲区大小",
#define CFG_UART3_DMABUF_LEN        64       // "UART3 DMA环形缓冲区大小",
#define CFG_UART4_SENDBUF_LEN       64       // "UART4发送环形缓冲区大小",
#define CFG_UART4_RECVBUF_LEN       64       // "UART4接收环形缓冲区大小",
#define CFG_UART4_DMABUF_LEN        64       // "UART4 DMA环形缓冲区大小",
#define CFG_UART1_ENABLE           false         // "是否使用UART1",
#define CFG_UART2_ENABLE           true        // "是否使用UART2",
#define CFG_UART3_ENABLE           false        // "是否使用UART3",
#define CFG_UART4_ENABLE           false        // "是否使用UART4",
//*******************************  Configure shell  ******************************************//
#define     CFG_ADD_ROUTINE_SHELL            true               // "是否添加常规shell命令"
#define     CFG_ADD_EXPAND_SHELL             true               // "是否添加拓展shell命令"
#define     CFG_ADD_GLOBAL_FUN               false              // "添加全局函数到shell"
#define     CFG_SHOW_ADD_SHEELL              true               // "显示在编译窗口添加的shell命令"
//*******************************  Configure System: tcpip  ******************************************//
#define     CFG_NETPKG_MEMSIZE          0x4000  //
#define     CFG_ARP_HASHLEN             32      //占用一个指针
#define     CFG_TPL_PROTONUM            5       //占用一个 tagTplProtoItem 结构
#define     CFG_UDP_ENABLE              true    //
#define     CFG_UDP_CBNUM               10      //占用一个 tagUdpCB 结构
#define     CFG_UDP_HASHLEN             4       //占用一个指针
//#define     CFG_TCP_ENABLE              true    //
//#define     CFG_TCP_REORDER             true    //资源充足才打开
//#define     CFG_TCP_CCBNUM              10      //占一个 指针 和 tagCCB
//#define     CFG_TCP_SCBNUM              5       //占一个 指针 和 tagSCB
#define     CFG_SOCKET_NUM              10      //占一个 tagItem 结构
#define     CFG_TELNET_ENABLE           true    //
#define     CFG_TELNETD_ENABLE          true    //
#define     CFG_TELNETC_ENABLE          false   //
#define     CFG_FTP_ENABLE              true    //
#define     CFG_FTPD_ENABLE             true    //
#define     CFG_FTPC_ENABLE             false   //
#define     CFG_TFTP_ENABLE             true    //
#define     CFG_TFTPD_ENABLE            false   //暂未实现
#define     CFG_TFTPC_ENABLE            true    //
#define     CFG_TFTP_PATHDEFAULT       "/"   //TFTP默认工作目录
#define     CFG_DHCP_ENABLE             true    //
#define     CFG_DHCPD_ENABLE            false   //
#define     CFG_DHCPC_ENABLE            true    //
#define     CFG_DHCP_RENEWTIME          3600    //秒数
#define     CFG_DHCP_REBINDTIME         3600    //秒数
#define     CFG_DHCP_LEASETIME          3600    //秒数
#define     CFG_DHCPD_IPNUM             0x40    //64
#define     CFG_DHCPD_IPSTART           "192.168.0.2"    //
#define     CFG_DHCPD_SERVERIP          "192.168.0.253"  //
#define     CFG_DHCPD_ROUTERIP          "192.168.0.253"  //
#define     CFG_DHCPD_NETIP             "255.255.255.0"  //
#define     CFG_DHCPD_DNS               "192.168.0.253"  //
#define     CFG_DHCPD_DOMAINNAME       "domain"        //
#define     CFG_PPP_ENABLE             false         //是否需要 ppp 协议
//*******************************  Configure System: kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096     // "初始化栈空间",定义初始化过程使用的栈空间，一般按默认值就可以了
#define CFG_EVENT_LIMIT         20       // "事件数量限值",事件数量
#define CFG_EVENT_TYPE_LIMIT    20       // "事件类型数限值",事件类型数
#define CFG_MAINSTACK_LIMIT     4096     // "main函数栈尺寸",main函数运行所需的栈尺寸
#define CFG_IDLESTACK_LIMIT     1024     // "IDLE事件栈尺寸",IDLE事件处理函数运行的栈尺寸，一般按默认值就可以了
#define CFG_OS_TINY             false    // "tiny版内核",true=用于资源紧缺的场合，内核会裁剪掉：事件类型名字，事件处理时间统计。
//*******************************  Configure System: memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10       // "内存池数量",允许建立10个内存池
//*******************************  Configure System: loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN      // EN_DIRECT_RUN=直接从flash中运行；EN_FORM_FILE=从文件系统加载到内存运行，
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL       // 是否对APP程序进行CRC校验，需要极快速启动才不需要CRC校验
#define CFG_IBOOT_VERSION       01         // Iboot发布版本号
#define CFG_IBOOT_UPDATE_NAME      "/yaf2/iboot.bin"            // "Iboot保存"
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
#define     CFG_STATIC_IP                    false             //true=使用静态IPfalse=动态IP
#define     CFG_NETCARD_NAME                 "BK7251_WIFI"     //
#define     CFG_MY_IPV4                      "192.168.0.179"   //
#define     CFG_MY_SUBMASK                   "255.255.255.0"   //
#define     CFG_MY_GATWAY                    "192.168.0.1"     //
#define     CFG_MY_DNS                       "192.168.0.1"     //
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (180*Mhz)         //主频，内核要用，必须定义
#define  CFG_BARE_APP                    (1U)
#endif
