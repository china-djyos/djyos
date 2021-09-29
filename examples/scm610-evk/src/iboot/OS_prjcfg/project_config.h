/****************************************************
 *  Automatically-generated file. Do not edit!  *
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure System: heap  ******************************************//
#define CFG_DYNAMIC_MEM true  //即使选false也允许使用malloc-free分配内存但使用有差别详见 《……用户手册》内存分配章节
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8   //用完会自动扩充
#define CFG_HANDLE_LIMIT        8   //用完会自动扩充
//*******************************  Configure System: memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10      //
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10      //定义设备数量
//*******************************  Configure netconfig  ******************************************//
#define    CFG_STATIC_IP                   true              //true=使用静态IPfalse=动态IP
#define    CFG_NETCARD_NAME                "SCM610_ETH"
#define    CFG_MY_IPV4                     "192.168.0.179"   //
#define    CFG_MY_SUBMASK                  "255.255.255.0"   //
#define    CFG_MY_GATWAY                   "192.168.0.1"     //
#define    CFG_MY_DNS                      "192.168.0.1"     //
//*******************************  Configure emflash insatall xip  ******************************************//
#define    CFG_EFLASH_XIP_PART_START       64                //分区起始，填写块号，块号从0开始计算
#define    CFG_EFLASH_XIP_PART_END         192                //分区结束，-1表示最后一块
#define    CFG_EFLASH_XIP_PART_FORMAT      true             //分区选项,是否需要格式化该分区。
//*******************************  Configure cpu_peri_emflash  ******************************************//
#define    CFG_EFLASH_PART_FORMAT          false             //分区选项,是否需要擦除该芯片。
//*******************************  Configure cpu peri uart  ******************************************//
#define    CFG_UART1_SENDBUF_LEN           64                //
#define    CFG_UART1_RECVBUF_LEN           64                //
#define    CFG_UART1_DMABUF_LEN            64                //
#define    CFG_UART2_SENDBUF_LEN           64                //
#define    CFG_UART2_RECVBUF_LEN           64                //
#define    CFG_UART2_DMABUF_LEN            64                //
#define    CFG_UART3_SENDBUF_LEN           64                //
#define    CFG_UART3_RECVBUF_LEN           64                //
#define    CFG_UART3_DMABUF_LEN            64                //
#define    CFG_UART4_SENDBUF_LEN           64                //
#define    CFG_UART4_RECVBUF_LEN           64                //
#define    CFG_UART4_DMABUF_LEN            64                //
#define    CFG_UART5_SENDBUF_LEN           64                //
#define    CFG_UART5_RECVBUF_LEN           64                //
#define    CFG_UART5_DMABUF_LEN            64                //
#define    CFG_UART6_SENDBUF_LEN           64                //
#define    CFG_UART6_RECVBUF_LEN           64                //
#define    CFG_UART6_DMABUF_LEN            64                //
#define    CFG_UART7_SENDBUF_LEN           64                //
#define    CFG_UART7_RECVBUF_LEN           64                //
#define    CFG_UART7_DMABUF_LEN            64                //
#define    CFG_UART8_SENDBUF_LEN           64                //
#define    CFG_UART8_RECVBUF_LEN           64                //
#define    CFG_UART8_DMABUF_LEN            64                //
#define    CFG_UART1_ENABLE                false             //
#define    CFG_UART2_ENABLE                true              //
#define    CFG_UART3_ENABLE                false             //
#define    CFG_UART4_ENABLE                false             //
#define    CFG_UART5_ENABLE                false             //
#define    CFG_UART6_ENABLE                false             //
#define    CFG_UART7_ENABLE                false             //
#define    CFG_UART8_ENABLE                false             //
//*******************************  Configure cpu onchip MAC  ******************************************//
#define    CFG_GMAC_LOOP_MODE              1                 //
#define    CFG_GMAC_LOOP_CYCLE             1000              //
#define    CFG_GMAC_MAC_ADDR0              11                //
#define    CFG_GMAC_MAC_ADDR1              22                //
#define    CFG_GMAC_MAC_ADDR2              33                //
#define    CFG_GMAC_MAC_ADDR3              44                //
#define    CFG_GMAC_MAC_ADDR4              55                //
#define    CFG_GMAC_MAC_ADDR5              66                //
#define    CFG_GMAC_NAME                   ("scm6xx")        //
//*******************************  Configure cpu onchip iic  ******************************************//
#define    CFG_I2C_BUF_LEN                 128               //
#define    CFG_I2C1_ENABLE                 false             //
#define    CFG_I2C2_ENABLE                 false             //
#define    CFG_I2C3_ENABLE                 false             //
//*******************************  Configure System: file system  ******************************************//
#define    CFG_CLIB_BUFFERSIZE             512               //
//*******************************  Configure shell  ******************************************//
#define    CFG_SHELL_STACK                 0x1000            //
#define    CFG_ADD_ROUTINE_SHELL           true              //
#define    CFG_ADD_EXPAND_SHELL            true              //
#define    CFG_ADD_GLOBAL_FUN              false             //
#define    CFG_SHOW_ADD_SHEELL             true              //
#define  CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure System: stdio  ******************************************//
#define    CFG_STDIO_STDIN_MULTI           true              //
#define    CFG_STDIO_STDOUT_FOLLOW         true              //
#define    CFG_STDIO_STDERR_FOLLOW         true              //
#define    CFG_STDIO_FLOAT_PRINT           true              //
#define    CFG_STDIO_STDIOFILE             true              //
#define    CFG_STDIO_IN_NAME               ("/dev/UART2")    //
#define    CFG_STDIO_OUT_NAME              ("/dev/UART2")    //
#define    CFG_STDIO_ERR_NAME              ("/dev/UART2")    //
//*******************************  Configure ftp  ******************************************//
#define    CFG_FTPD_ENABLE                 false             //暂未实现
#define    CFG_FTPC_ENABLE                 true              //
#define  CFG_MODULE_ENABLE_FTP           true
//*******************************  Configure telnet  ******************************************//
#define    CFG_TELNETD_ENABLE              true              //
#define    CFG_TELNETC_ENABLE              false             //
#define  CFG_MODULE_ENABLE_TELNET        true
//*******************************  Configure tftp  ******************************************//
#define    CFG_FTPD_ENABLE                 true              //
#define    CFG_FTPC_ENABLE                 false             //
#define    CFG_TFTP_PATHDEFAULT            "/"               //TFTP默认工作目录
#define  CFG_MODULE_ENABLE_TFTP          true
//*******************************  Configure arp  ******************************************//
#define    CFG_ARP_HASHLEN                 32                //占用一个指针
#define  CFG_MODULE_ENABLE_ARP           true
//*******************************  Configure icmp  ******************************************//
#define  CFG_MODULE_ENABLE_ICMP          true
//*******************************  Configure sock  ******************************************//
#define CFG_TCP_REORDER  false
#define    CFG_SOCKET_NUM                  10                //占一个 tagItem 结构
//*******************************  Configure router  ******************************************//
#define    CFG_IP_STRMAX                   20                //最大路由条目数
#define  CFG_MODULE_ENABLE_ROUTER        true
//*******************************  Configure System: tcp  ******************************************//
#define    CFG_TCP_REORDER                 true              //资源充足才打开
#define    CFG_TCP_CCBNUM                  10                //占一个 指针 和 struct ClienCB
#define    CFG_TCP_SCBNUM                  5                 //占一个 指针 和 struct ServerCB
#define  CFG_MODULE_ENABLE_TCP           true
//*******************************  Configure tpl  ******************************************//
#define    CFG_TPL_PROTONUM                5                 //占用一个 tagTplProtoItem 结构
//*******************************  Configure udp  ******************************************//
#define    CFG_UDP_CBNUM                   10                //占用一个 tagUdpCB 结构
#define    CFG_UDP_HASHLEN                 4                 //占用一个指针
#define    CFG_UDP_PKGMSGLEN               1472              //udp最大包长度
#define    CFG_UDP_BUFLENDEFAULT           0x2000            //8KB
#define  CFG_MODULE_ENABLE_UDP           true
//*******************************  Configure System: tcpip  ******************************************//
#define    CFG_NETPKG_MEMSIZE              0x4000            //
#define  CFG_MODULE_ENABLE_TCPIP         true
#define  CFG_MODULE_ENABLE_TCPIP         true
//*******************************  Configure System: kernel  ******************************************//
#define    CFG_INIT_STACK_SIZE             4096              //定义初始化过程使用的栈空间一般按默认值就可以了
#define    CFG_EVENT_LIMIT                 15                //事件数量
#define    CFG_EVENT_TYPE_LIMIT            15                //事件类型数
#define    CFG_MAINSTACK_LIMIT             4096              //main函数运行所需的栈尺寸
#define    CFG_IDLESTACK_LIMIT             1024              //IDLE事件处理函数运行的栈尺寸一般按默认值就可以了
#define    CFG_OS_TINY                     false             //true=用于资源紧缺的场合内核会裁剪掉：事件类型名字事件处理时间统计。
//*******************************  Configure System: ymodem  ******************************************//
#define    CFG_YMODEM_BUF_NUM              32                //每包1024字节
#define    CFG_YMODEM_PKG_TIMEOUT          (15*1000*1000)    //微秒
#define    CFG_YMODEM_TIMEOUT              (300*1000*1000)   //微秒
#define    CFG_YMODEM_DEVNAME              "std"             //"ymodem传输设备，std表示使用标准输入输出设备","ymodem传输设备，std表示使用标准输入输出设备"
//*******************************  Configure System: lock  ******************************************//
#define    CFG_LOCK_LIMIT                  40                //定义锁的数量包含信号量和互斥量
//*******************************  Configure System: time  ******************************************//
#define    CFG_LOCAL_TIMEZONE              8                 //北京时间是东8区
//*******************************  Configure System: loader  ******************************************//
#define    CFG_APP_RUNMODE                 CN_DIRECT_RUN     //CN_DIRECT_RUN=直接从flash中运行；CN_FORM_FILE=从文件系统加载到内存运行，
#define    CFG_APP_VERIFICATION            VERIFICATION_NULL //是否对APP程序进行CRC校验，需要极快速启动才不需要CRC校验
#define    CFG_IBOOT_VERSION               01                //Iboot发布版本号
#define    CFG_IBOOT_UPDATE_NAME           "/yaf2/iboot.bin" //
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (150*Mhz)         //主频，内核要用，必须定义

#endif
