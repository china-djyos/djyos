/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure NlsCharset  ******************************************//
#define     CFG_LOCAL_CHARSET                "gb2312"                 // "�����ַ���",
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40             // "��������",�������������������ź����ͻ�����
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true         // "ȫ���ܶ�̬����",��ʹѡfalse��Ҳ����ʹ��malloc-free�����ڴ棬��ʹ���в����� �������û��ֲᡷ�ڴ�����½�
//*******************************  Configure DevFile  ******************************************//
#define CFG_DEVFILE_LIMIT       10             // "�豸����",�����豸����
//*******************************  Configure ioiicbus  ******************************************//
#define     IO_IIC_USER_TAG                  1                 // �û��Լ��ı�ǩ�������ڻص��û��ṩ��IO���ƺ����ĵڶ�������
#define     IO_IIC_BUS_NAME                  "IoIic"           // ��IIC���ߵ�����
#define     IIC_IOCTRL_FUN                   IIC_IoCtrlFunc    // �û�ʵ�ֵ�IO���ƺ�����
//*******************************  Configure font  ******************************************//
#define     CFG_FONT_DEFAULT                 "gb2312_song_16"     // "Ĭ������",��������include/fontĿ¼����
//*******************************  Configure gb2312_dot  ******************************************//
#define     CFG_GB2312_12_SONG               zk_disable             // "12��������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"      // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_16_SONG              from_array                  // "16��������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_16_SONG_FILENAME "zk_gb2316_16song.bin"      // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_16_YUAN              zk_disable                  // "16����Բ��",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_16_YUAN_FILENAME "zk_gb2316_16yuan.bin"      // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_16_KAI              zk_disable                   // "16������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_16_KAI_FILENAME  "zk_gb2316_16kai.bin"       // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_16_HEI              zk_disable                   // "16�����",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_16_HEI_FILENAME  "zk_gb2316_16hei.bin"       // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_16_FANG              zk_disable                  // "16���������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_16_FANG_FILENAME "zk_gb2316_16fang.bin"      // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_24_SONG              zk_disable                  // "24��������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_24_SONG_FILENAME "zk_gb2316_24song.bin"      // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_24_YUAN              zk_disable                  // "24����Բ��",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_24_YUAN_FILENAME "zk_gb2316_24yuan.bin"      // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_24_KAI              zk_disable                   // "24������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_24_KAI_FILENAME  "zk_gb2316_24kai.bin"       // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_24_HEI              zk_disable                   // "24�������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_24_HEI_FILENAME  "zk_gb2316_24hei.bin"       // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_24_FANG              zk_disable                  // "24���������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_24_FANG_FILENAME "zk_gb2316_24fang.bin"      // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_32_SONG              zk_disable                  // "32��������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_32_SONG_FILENAME "zk_gb2316_32song.bin"      // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_32_YUAN              zk_disable                  // "32����Բ��",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_32_YUAN_FILENAME "zk_gb2316_32yuan.bin"      // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_32_KAI              zk_disable                   // "32������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_32_KAI_FILENAME  "zk_gb2316_32kai.bin"       // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_32_HEI              zk_disable                   // "32�������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_32_HEI_FILENAME  "zk_gb2316_32hei.bin"       // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_32_FANG              zk_disable                  // "32���������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_32_FANG_FILENAME "zk_gb2316_32fang.bin"      // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
//*******************************  Configure gkernel  ******************************************//
#define CFG_GKERNEL_CMD_DEEP        1024           // "����������",gdd��gkernel��������Ļ��������ȣ��ֽ�����
//*******************************  Configure gdd  ******************************************//
#define     CFG_DESKTOP_WIDTH                0                        // "������",����ߴ磨����������ȣ�0=��ʾ�����
#define     CFG_DESKTOP_HEIGHT               0                        // "����߶�",����ߴ磨���������߶ȣ�0=��ʾ���߶�
#define     CFG_DISPLAY_NAME                 "LCD_F7"                 // "��ʾ����",����bsp����ʾ������ģ�����õ���ʾ��������ͬ
#define     CFG_DESKTOP_NAME                 "desktop"                // "������"
#define     CFG_INPUTDEV_NAME                "FT5X26"                 // "�����豸����",ʹ��bsp�������豸�����õ����֣��������豸�Ļ���ÿ���豸���ö��Ÿ���
#define     CFG_DESKTOP_FORMAT               CN_SYS_PF_RGB565         // "���ظ�ʽ",���洰�����ظ�ʽ��������gkernel.h�ж��壬һ��ʹ������ʾ����ͬ��ɫ
#define     CFG_GRAY_BASE_COLOR              CN_COLOR_WHITE           // "�ҶȻ�ɫ",���ظ�ʽ��Ϊ�Ҷ�ʱ����Ҫ���õġ�������ɫ������gkernel.h���ҵ�������ɫ����
#define     CFG_FILL_COLOR                   CN_COLOR_BLUE            // "���ɫ",��������ʱ�����ɫ����888��ʽ������gkernel.h���ҵ�������ɫ����
//*******************************  Configure HmiInput  ******************************************//
#define     CFG_HMIIN_DEV_LIMIT              2                        // "name",�˻����������豸����������̡�����
//*******************************  Configure stdio  ******************************************//
#define     CFG_STDIO_RUN_MODE               (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW)  // "STDIOģʽ",
#define     CFG_STDIO_IN_NAME                "/dev/UART1"        // "��׼�����豸��",
#define     CFG_STDIO_OUT_NAME               "/dev/UART1"        // "��׼����豸��",
#define     CFG_STDIO_ERR_NAME               "/dev/UART1"        // "��׼err����豸��",
//*******************************  Configure cpu_peri_uart  ******************************************//
#define     CFG_UART1_SENDBUF_LEN            64                       // "UART1���ͻ��λ�������С",
#define     CFG_UART1_RECVBUF_LEN            64                       // "UART1���ջ��λ�������С",
#define     CFG_UART1_DMABUF_LEN             64                       // "UART1 DMA���λ�������С",
#define     CFG_UART2_SENDBUF_LEN            64                       // "UART2���ͻ��λ�������С",
#define     CFG_UART2_RECVBUF_LEN            64                       // "UART2���ջ��λ�������С",
#define     CFG_UART2_DMABUF_LEN             64                       // "UART2 DMA���λ�������С",
#define     CFG_UART3_SENDBUF_LEN            64                       // "UART3���ͻ��λ�������С",
#define     CFG_UART3_RECVBUF_LEN            64                       // "UART3���ջ��λ�������С",
#define     CFG_UART3_DMABUF_LEN             64                       // "UART3 DMA���λ�������С",
#define     CFG_UART4_SENDBUF_LEN            64                       // "UART4���ͻ��λ�������С",
#define     CFG_UART4_RECVBUF_LEN            64                       // "UART4���ջ��λ�������С",
#define     CFG_UART4_DMABUF_LEN             64                       // "UART4 DMA���λ�������С",
#define     CFG_UART5_SENDBUF_LEN            64                       // "UART5���ͻ��λ�������С",
#define     CFG_UART5_RECVBUF_LEN            64                       // "UART5���ջ��λ�������С",
#define     CFG_UART5_DMABUF_LEN             64                       // "UART5 DMA���λ�������С",
#define     CFG_UART6_SENDBUF_LEN            64                       // "UART6���ͻ��λ�������С",
#define     CFG_UART6_RECVBUF_LEN            64                       // "UART6���ջ��λ�������С",
#define     CFG_UART6_DMABUF_LEN             64                       // "UART6 DMA���λ�������С",
#define     CFG_UART7_SENDBUF_LEN            64                       // "UART7���ͻ��λ�������С",
#define     CFG_UART7_RECVBUF_LEN            64                       // "UART7���ջ��λ�������С",
#define     CFG_UART7_DMABUF_LEN             64                       // "UART7 DMA���λ�������С",
#define     CFG_UART8_SENDBUF_LEN            64                       // "UART8���ͻ��λ�������С",
#define     CFG_UART8_RECVBUF_LEN            64                       // "UART8���ջ��λ�������С",
#define     CFG_UART8_DMABUF_LEN             64                       // "UART8 DMA���λ�������С",
#define     CFG_UART1_ENABLE                 true                     // "�Ƿ�ʹ��UART1",
#define     CFG_UART2_ENABLE                 false                    // "�Ƿ�ʹ��UART2",
#define     CFG_UART3_ENABLE                 false                    // "�Ƿ�ʹ��UART3",
#define     CFG_UART4_ENABLE                 false                    // "�Ƿ�ʹ��UART4",
#define     CFG_UART5_ENABLE                 false                    // "�Ƿ�ʹ��UART5",
#define     CFG_UART6_ENABLE                 false                    // "�Ƿ�ʹ��UART6",
#define     CFG_UART7_ENABLE                 false                    // "�Ƿ�ʹ��UART7",
#define     CFG_UART8_ENABLE                 false                    // "�Ƿ�ʹ��UART8",
//*******************************  Configure shell  ******************************************//
#define     CFG_ADD_ROUTINE_SHELL            true                // "�Ƿ���ӳ���shell����"
#define     CFG_ADD_EXPAND_SHELL             true                // "�Ƿ������չshell����"
#define     CFG_ADD_GLOBAL_FUN               false               // "���ȫ�ֺ�����shell"
#define     CFG_SHOW_ADD_SHEELL              true                // "��ʾ�ڱ��봰����ӵ�shell����"
//*******************************  Configure tcpip  ******************************************//
#define     CFG_NETPKG_MEMSIZE               0x4000                  // "���ݰ��������ߴ�"
#define     CFG_ARP_HASHLEN                  32                      // "ARP��ϣ����"��ռ��һ��ָ��
#define     CFG_TPL_PROTONUM                 5                       // "֧�ֵĴ���Э����"��ռ��һ�� tagTplProtoItem �ṹ
#define     CFG_UDP_ENABLE                   true                    // "udpЭ��ʹ��"
#define     CFG_UDP_CBNUM                    10                      // "UDP socket����ֵ"��ռ��һ�� tagUdpCB �ṹ
#define     CFG_UDP_HASHLEN                  4                       // "udp socket ��ϣ����"��ռ��һ��ָ��
 // #define     CFG_TCP_ENABLE              true    
 // #define     CFG_TCP_REORDER             true    
 // #define     CFG_TCP_CCBNUM              10      
 // #define     CFG_TCP_SCBNUM              5       
#define     CFG_SOCKET_NUM              10       // "socket����ֵ"��ռһ�� tagItem �ṹ
#define     CFG_TELNET_ENABLE           true     // "telnet ʹ��"
#define     CFG_TELNETD_ENABLE          true     // "telnet ������ʹ��"
#define     CFG_TELNETC_ENABLE          false    // "telnet �ͻ���ʹ��"
#define     CFG_FTP_ENABLE              true     // "ftp ʹ��"
#define     CFG_FTPD_ENABLE             true     // "ftp ������ʹ��"
#define     CFG_FTPC_ENABLE             false    // "ftp �ͻ���ʹ��"
#define     CFG_TFTP_ENABLE             true     // "tftp ʹ��"
#define     CFG_TFTPD_ENABLE            false    // "tftp ������ʹ��",��δʵ��
#define     CFG_TFTPC_ENABLE            true     // "tftp �ͻ���ʹ��"
#define     CFG_TFTP_PATHDEFAULT       "/"    // TFTPĬ�Ϲ���Ŀ¼
#define     CFG_DHCP_ENABLE             true     // "DHCP ʹ��"
#define     CFG_DHCPD_ENABLE            false    // "DHCP ������ʹ��"
#define     CFG_DHCPC_ENABLE            true     // "DHCP �ͻ���ʹ��"
#define     CFG_DHCP_RENEWTIME          3600     // "renew timer",����
#define     CFG_DHCP_REBINDTIME         3600     // "rebind timer",����
#define     CFG_DHCP_LEASETIME          3600     // "lease timer",����
#define     CFG_DHCPD_IPNUM             0x40     // "IP�سߴ�",64
#define     CFG_DHCPD_IPSTART           "192.168.0.2"     // "DHCP��ʼIP",
#define     CFG_DHCPD_SERVERIP          "192.168.0.253"   // "DHCP SERVER IP"
#define     CFG_DHCPD_ROUTERIP          "192.168.0.253"   // "DHCP ROUTER SERVER IP"
#define     CFG_DHCPD_NETIP             "255.255.255.0"   // "DHCP MASK IP"
#define     CFG_DHCPD_DNS               "192.168.0.253"   // "DHCP DNSSERVER IP"
#define     CFG_DHCPD_DOMAINNAME       "domain"         // "DHCP domain name"
#define     CFG_PPP_ENABLE             false          // �Ƿ���Ҫ ppp Э��
//*******************************  Configure tcp  ******************************************//
#define     CFG_TCP_REORDER                  true                    // "TCP��������ʹ��",��Դ����Ŵ�
#define     CFG_TCP_CCBNUM                   10                      // "tcp �ͻ�������ֵ"��ռһ�� ָ�� �� struct ClienCB
#define     CFG_TCP_SCBNUM                   5                       // "tcp ����������ֵ"��ռһ�� ָ�� �� struct ServerCB
//*******************************  Configure timer  ******************************************//
#define CFG_TIMER_SOUCE     1              // "ʱ��Դ",1=��Ӳ����ʱ���ṩʱ��Դ��0=��tick�ṩʱ��Դ
#define CFG_TIMERS_LIMIT    5              // "��ʱ������",�ɴ����Ķ�ʱ��������������ͼ�ν���Ķ�ʱ����
//*******************************  Configure kernel  ******************************************//
#define     CFG_INIT_STACK_SIZE              4096                    // "��ʼ��ջ�ռ�",�����ʼ������ʹ�õ�ջ�ռ䣬һ�㰴Ĭ��ֵ�Ϳ�����
#define     CFG_EVENT_LIMIT                  30                      // "�¼�������ֵ",�¼�����
#define     CFG_EVENT_TYPE_LIMIT             30                      // "�¼���������ֵ",�¼�������
#define     CFG_MAINSTACK_LIMIT              4096                    // "main����ջ�ߴ�",main�������������ջ�ߴ�
#define     CFG_IDLESTACK_LIMIT              1024                    // "IDLE�¼�ջ�ߴ�",IDLE�¼����������е�ջ�ߴ磬һ�㰴Ĭ��ֵ�Ϳ�����
#define     CFG_OS_TINY                      false                   // "tiny���ں�",true=������Դ��ȱ�ĳ��ϣ��ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
//*******************************  Configure MemoryPool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10             // "�ڴ������",������10���ڴ��
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8             // "ʱ��",����ʱ���Ƕ�8��
//*******************************  Configure loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN            // EN_DIRECT_RUN=ֱ�Ӵ�flash�����У�EN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL             // �Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define CFG_IBOOT_VERSION       01               // Iboot�����汾��
#define CFG_IBOOT_UPDATE_NAME      "/yaf2/iboot.bin"                  // "ymodem�����豸��std��ʾʹ�ñ�׼��������豸"
//*******************************  Configure cpu_peri_eth  ******************************************//
#define CFG_MAC_ADDR0           00                  // "������ַ0",
#define CFG_MAC_ADDR1           00                  // "������ַ1",
#define CFG_MAC_ADDR2           00                  // "������ַ2",
#define CFG_MAC_ADDR3           00                  // "������ַ3",
#define CFG_MAC_ADDR4           00                  // "������ַ4",
#define CFG_MAC_ADDR5           00                  // "������ַ5",
#define CFG_ETH_LOOP_CYCLE      1000                // "������ѯ��������",
#define CFG_ETH_LOOP_ENABLE     true               // "���������Ƿ���ѯ",
#define CFG_ETH_DEV_NAME        "STM32F7_ETH"       // "��������",
//*******************************  Configure lan8720  ******************************************//
#define CFG_LAN8720_PHY_ADDRESS     0          // phy�������ַ��Ӳ��һ�����Ϊ 0
//*******************************  Configure netconfig  ******************************************//
#define     CFG_STATIC_IP                    true                     // "IP����",true=ʹ�þ�̬IP��false=��̬IP
#define     CFG_NETCARD_NAME                 "APOLLO_ETH"             // "������",
#define     CFG_MY_IPV4                      "192.168.0.179"          // "��̬IP",
#define     CFG_MY_SUBMASK                   "255.255.255.0"          // "��������",
#define     CFG_MY_GATWAY                    "192.168.0.1"            // "����",
#define     CFG_MY_DNS                       "192.168.0.1"            // "DNS",
//*******************************  Configure key_hard  ******************************************//
#define     CFG_KEYBOARD_NAME                "KEYBOARD"               // "name",���ü�������
//*******************************  Configure FT5X26  ******************************************//
#define     CT_MAX_TOUCH                     5                        // "������",֧�����5�㴥��
#define     CFG_TOUCH_ADJUST_FILE            "/yaf2/touch_init.dat"       // ���津���������������ļ�
#define     CFG_FT5X26_BUS_NAME              "IoIic"                  // "IIC��������",����оƬʹ�õ�IIC��������
#define     CFG_FT5X26_TOUCH_NAME            "FT5X26"                 // "����������",���ô���������
#define     CFG_DESKTOP_NAME                 "desktop"                // "��������",���ô�����������ʾ�����������
//*******************************  Configure PCF8574  ******************************************//
#define     CFG_PCF8574_BUS_NAME             "IoIic"                  // "name"
//*******************************  Configure cpu_peri_lcd  ******************************************//
#define     CFG_LCD_XSIZE                    1024               // "LCDˮƽ���ؿ��",
#define     CFG_LCD_YSIZE                    600                // "LCD��ֱ���ؿ��",
#define     CFG_DISPLAY_NAME                 "LCD_F7"           // "��ʾ��LCD����",
#define     CFG_LCD_HEAP_NAME                "extram"           // "LCDʹ�ö�����",
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (216*Mhz)         //��Ƶ���ں�Ҫ�ã����붨��

#endif
