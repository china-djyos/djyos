/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include <stdint.h>
#include <stddef.h>
//manual config start
//�˴���д�ֶ����ã�DIDE���������ļ�ʱ�����ᱻ�޸�
//manual config end

#define    CN_RUNMODE_IBOOT                0                //IBOOTģʽ����
#define    CN_RUNMODE_APP                2                //��IBOOT���ص�APP
#define    CFG_RUNMODE                CN_RUNMODE_IBOOT                //��IBOOT���ص�APP
//*******************************  Configure black box  ******************************************//
#define    CFG_MODULE_ENABLE_BLACK_BOX     true
//*******************************  Configure file system  ******************************************//
#define CFG_CLIB_BUFFERSIZE            512       // "C���ļ��û�̬�������ߴ�"
#define    CFG_MODULE_ENABLE_FILE_SYSTEM   true
//*******************************  Configure int  ******************************************//
#define    CFG_MODULE_ENABLE_INT           true
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8    // "��������ʼֵ"��������Զ�����
#define CFG_HANDLE_LIMIT        8    // "�������ʼֵ"��������Զ�����
#define    CFG_MODULE_ENABLE_KERNEL_OBJECT_SYSTEM  true
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40       // "��������",�������������������ź����ͻ�����
#define    CFG_MODULE_ENABLE_LOCK          true
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10       // "�ڴ��������ֵ",
#define    CFG_MODULE_ENABLE_MEMORY_POOL   true
//*******************************  Configure message queue  ******************************************//
#define    CFG_MODULE_ENABLE_MESSAGE_QUEUE true
//*******************************  Configure multiplex  ******************************************//
#define    CFG_MODULE_ENABLE_MULTIPLEX     true
//*******************************  Configure ring buffer and line buffer  ******************************************//
#define    CFG_MODULE_ENABLE_RING_BUFFER_AND_LINE_BUFFER  true
//*******************************  Configure xip app file system  ******************************************//
#define    CFG_MODULE_ENABLE_XIP_APP_FILE_SYSTEM  true
//*******************************  Configure cpu onchip timer  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_TIMER  true
//*******************************  Configure cpu onchip systime  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_SYSTIME  true
//*******************************  Configure cpu onchip dma  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_DMA  true
//*******************************  Configure cpu onchip gpio  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO  true
//*******************************  Configure cpu onchip random  ******************************************//
#define CFG_RNG_NUM_MAX      65535          // "��������ֵ",
#define CFG_RNG_NUM_MIN      1024           // "�������Сֵ",
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_RANDOM  true
//*******************************  Configure STM32 commond code  ******************************************//
#define    CFG_MODULE_ENABLE_STM32_COMMOND_CODE  true
//*******************************  Configure djybus  ******************************************//
#define    CFG_MODULE_ENABLE_DJYBUS        true
//*******************************  Configure can bus  ******************************************//
#define CFG_CAN_BUS_MSGQ_NUM        200      // "��Ϣ���г���",
#define CFG_CAN_BUS_TIMEOUT         200      // "���߳�ʱ(ms)",
#define CFG_CAN_BUS_RCV_RING_LEN    100      // "����buffer��Ϣ��",
#define    CFG_MODULE_ENABLE_CAN_BUS       true
//*******************************  Configure iicbus  ******************************************//
#define    CFG_MODULE_ENABLE_IICBUS        true
//*******************************  Configure io analog iic bus  ******************************************//
#define    CFG_MODULE_ENABLE_IO_ANALOG_IIC_BUS  true
//*******************************  Configure IIC to 8gpio pcf8574  ******************************************//
#define CFG_PCF8574_BUS_NAME       "IoIic"         // "IIC������"��
#define    CFG_MODULE_ENABLE_IIC_TO_8GPIO_PCF8574  true
//*******************************  Configure easy file system  ******************************************//
#define CFG_EFS_FILE_SIZE_LIMIT           4096                 //
#define CFG_EFS_MAX_CREATE_FILE_NUM       50                   //
#define CFG_EFS_MAX_OPEN_FILE_NUM         10                   //
#define CFG_EFS_MOUNT_POINT               "efs"       // "EFS�ļ�ϵͳ��װĿ¼"
#define CFG_EFS_INSTALL_OPTION            MS_INSTALLFORMAT       // "��װѡ��"��MS_INSTALLFORMAT,MS_INSTALLCREAT,MS_INSTALLUSE��һ�������ġ���
#define    CFG_MODULE_ENABLE_EASY_FILE_SYSTEM  true
//*******************************  Configure font  ******************************************//
#define CFG_FONT_DEFAULT  "gb2312_song_16"       // "Ĭ������",��������include/fontĿ¼����
#define    CFG_MODULE_ENABLE_FONT          true
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096     // "��ʼ��ջ�ռ�",�����ʼ������ʹ�õ�ջ�ռ䣬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         15       // "�¼�������ֵ",�¼�����
#define CFG_EVENT_TYPE_LIMIT    15       // "�¼���������ֵ",�¼�������
#define CFG_MAINSTACK_LIMIT     4096     // "main����ջ�ߴ�",main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024     // "IDLE�¼�ջ�ߴ�",IDLE�¼����������е�ջ�ߴ磬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_IDLE_MONITOR_CYCLE  30      //����IDLE�¼���������1/16 CPUռ�ȵ�ʱ��������0=������
#define CFG_OS_TINY             false    // "tiny���ں�",true=������Դ��ȱ�ĳ��ϣ��ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
#define    CFG_MODULE_ENABLE_KERNEL        true
//*******************************  Configure board config  ******************************************//
#define    CFG_MODULE_ENABLE_BOARD_CONFIG  true
//*******************************  Configure misc  ******************************************//
#define    CFG_MODULE_ENABLE_MISC          true
//*******************************  Configure Nls Charset  ******************************************//
#define CFG_LOCAL_CHARSET       "gb2312"         // "�����ַ���",
#define    CFG_MODULE_ENABLE_NLS_CHARSET   true
//*******************************  Configure gb2312 charset  ******************************************//
#define    CFG_MODULE_ENABLE_GB2312_CHARSET  true
//*******************************  Configure gb2312 dot  ******************************************//
#define CFG_GB2312_12_SONG              zk_disable               // "12��������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_16_SONG              from_array               // "16��������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_16_YAHEI             zk_disable              //GB2312����,zk_disable������Ҫfrom_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_16_YUAN              zk_disable               // "16����Բ��",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_16_KAI              zk_disable                // "16������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_16_HEI              zk_disable                // "16�����",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_16_FANG              zk_disable               // "16���������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_24_SONG              zk_disable               // "24��������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_24_YAHEI              zk_disable             // "24����΢���ź�",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   //  "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_24_YUAN              zk_disable               // "24����Բ��",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_24_KAI              zk_disable                // "24������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_24_HEI              zk_disable                // "24�������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_24_FANG              zk_disable               // "24���������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_32_SONG              zk_disable               // "32��������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_32_YUAN              zk_disable               // "32����Բ��",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_32_KAI              zk_disable                // "32������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_32_HEI              zk_disable                // "32�������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_32_FANG              zk_disable               // "32���������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define    CFG_MODULE_ENABLE_GB2312_DOT    true
//*******************************  Configure ascii charset  ******************************************//
#define    CFG_MODULE_ENABLE_ASCII_CHARSET true
//*******************************  Configure Software Timers  ******************************************//
#define CFG_TIMERS_LIMIT        5        // "��ʱ������",�ɴ����Ķ�ʱ��������������ͼ�ν���Ķ�ʱ����
#define CFG_TIMER_SOUCE_HARD    true     // "Ӳ����ʱ���ṩʱ��Դ",ѡ��ר��Ӳ������tick��ʱ��Դ
#define    CFG_MODULE_ENABLE_SOFTWARE_TIMERS  true
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8       // "ʱ��",����ʱ���Ƕ�8��
#define    CFG_MODULE_ENABLE_TIME          true
//*******************************  Configure board driver lcd config  ******************************************//
#define    CFG_MODULE_ENABLE_BOARD_DRIVER_LCD_CONFIG  true
//*******************************  Configure debug information  ******************************************//
#define    CFG_MODULE_ENABLE_DEBUG_INFORMATION  true
//*******************************  Configure graphical kernel  ******************************************//
#define CFG_GKERNEL_CMD_DEEP        1024     // "�����������",�ϲ�Ӧ�ã�����gdd����gkernel��������Ļ��������ȣ��ֽ�����
#define CFG_USER_REQUEST_DEEP       128     //gkernel���ϲ���������Ļ��������ȣ��ֽ�����
#define    CFG_MODULE_ENABLE_GRAPHICAL_KERNEL  true
//*******************************  Configure graphical decorate development  ******************************************//
#define CFG_DESKTOP_WIDTH       0           //����ߴ磨�����������0=��ʾ�����
#define CFG_DESKTOP_HEIGHT      0           //����ߴ磨���������߶�0=��ʾ���߶�
#define CFG_DESKTOP_BUF         true        //�ڴ�ϡ�ٵ�Ӳ���ɲ�������
#define CFG_DISPLAY_NAME        "DISPLAY_NAME"         //����bsp����ʾ������ģ�����õ���ʾ��������ͬ
#define CFG_DESKTOP_NAME        "DESKTOP_NAME"         //
#define CFG_DESKTOP_FORMAT      CN_SYS_PF_RGB565    //���洰�����ظ�ʽ������gkernel.h�ж��壬һ��ʹ������ʾ����ͬ��ɫ
#define CFG_GRAY_BASE_COLOR     CN_COLOR_WHITE      //���ظ�ʽ��Ϊ�Ҷ�ʱ����Ҫ���õġ�������ɫ����gkernel.h���ҵ�������ɫ����
#define CFG_FILL_COLOR          CN_COLOR_BLUE       //��������ʱ�����ɫ��888��ʽ������gkernel.h���ҵ�������ɫ����
#define    CFG_MODULE_ENABLE_GRAPHICAL_DECORATE_DEVELOPMENT  true
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true   // "ȫ���ܶ�̬����",��ʹѡfalse��Ҳ����ʹ��malloc-free�����ڴ棬��ʹ���в����� �������û��ֲᡷ�ڴ�����½�
#define    CFG_MODULE_ENABLE_HEAP          true
//*******************************  Configure cpu onchip lcd  ******************************************//
#define CFG_LCD_XSIZE                   600  //  "LCDˮƽ���ؿ��",
#define CFG_LCD_YSIZE                   480  //  "LCD��ֱ���ؿ��",
#define CFG_DISPLAY_NAME        "LCD_F7"     //  "��ʾ��LCD����",
#define CFG_LCD_HEAP_NAME       "extram"     //  "LCDʹ�ö�����",
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_LCD  true
//*******************************  Configure uart device file  ******************************************//
#define    CFG_MODULE_ENABLE_UART_DEVICE_FILE  true
//*******************************  Configure cpu onchip uart  ******************************************//
#define CFG_UART_SENDBUF_LEN            32                   //  "���ڷ��ͻ��λ�������С",
#define CFG_UART_RECVBUF_LEN            32                   //  "���ڽ��ջ��λ�������С",
#define CFG_UART1_ENABLE                true                 //  "�����Ƿ�ʹ��UART1",
#define CFG_UART2_ENABLE                false                //  "�����Ƿ�ʹ��UART2",
#define CFG_UART3_ENABLE                false                //  "�����Ƿ�ʹ��UART3",
#define CFG_UART4_ENABLE                false                //  "�����Ƿ�ʹ��UART4",
#define CFG_UART5_ENABLE                false                //  "�����Ƿ�ʹ��UART5",
#define CFG_UART6_ENABLE                false                //  "�����Ƿ�ʹ��UART6",
#define CFG_UART7_ENABLE                false                //  "�����Ƿ�ʹ��UART7",
#define CFG_UART8_ENABLE                false                //  "�����Ƿ�ʹ��UART8",
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_UART  true
//*******************************  Configure tcpip  ******************************************//
#define     CFG_NETPKG_MEMSIZE          0x4000   // "���ݰ��������ߴ�"
#define    CFG_MODULE_ENABLE_TCPIP         true
//*******************************  Configure ethernet phy lan8720  ******************************************//
#define CFG_LAN8720_PHY_ADDRESS     0    //  phy�������ַ��Ӳ��һ�����Ϊ 0
#define    CFG_MODULE_ENABLE_ETHERNET_PHY_LAN8720  true
//*******************************  Configure cpu onchip MAC  ******************************************//
#define CFG_ETH_NETCARD_NAME    "STM32H7_ETH"    //  "��������",
#define CFG_ETH_LOOP_CYCLE      1000          //  "������ѯ����(uS)",�ж�ģʽ������д
#define CFG_ETH_LOOP_ENABLE     true             //  "���������Ƿ���ѯ",
#define CFG_ETH_HARD_MAC_ADDR   true             //  "Ӳ������Mac��ַ",
#define CFG_ETH_MAC_ADDR0      00            //  "MAC ADDR0",��ѡ��"Ӳ������Mac��ַ",��������д
#define CFG_ETH_MAC_ADDR1      02            //  "MAC ADDR1",��ѡ��"Ӳ������Mac��ַ",��������д
#define CFG_ETH_MAC_ADDR2      03            //  "MAC ADDR2",��ѡ��"Ӳ������Mac��ַ",��������д
#define CFG_ETH_MAC_ADDR3      04            //  "MAC ADDR3",��ѡ��"Ӳ������Mac��ַ",��������д
#define CFG_ETH_MAC_ADDR4      05            //  "MAC ADDR4",��ѡ��"Ӳ������Mac��ַ",��������д
#define CFG_ETH_MAC_ADDR5      06            //  "MAC ADDR5",��ѡ��"Ӳ������Mac��ַ",��������д
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_MAC  true
//*******************************  Configure tcp  ******************************************//
#define     CFG_TCP_REORDER             true     // "TCP��������ʹ��",��Դ����Ŵ�
#define     CFG_TCP_CCBNUM              10       // "tcp �ͻ�������ֵ"��ռһ�� ָ�� �� struct ClienCB
#define     CFG_TCP_SCBNUM              5        // "tcp ����������ֵ"��ռһ�� ָ�� �� struct ServerCB
#define    CFG_MODULE_ENABLE_TCP           true
//*******************************  Configure human machine interface  ******************************************//
#define CFG_HMIIN_DEV_LIMIT     2        // "�����豸��������",�˻����������豸����������̡�����
#define    CFG_MODULE_ENABLE_HUMAN_MACHINE_INTERFACE  true
//*******************************  Configure touch  ******************************************//
#define    CFG_MODULE_ENABLE_TOUCH         true
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_STDIN_MULTI      true          // "�Ƿ�֧�ֶ��������豸",
#define CFG_STDIO_STDOUT_FOLLOW    true          // "stdout�Ƿ����stdin",
#define CFG_STDIO_STDERR_FOLLOW    true          // "stderr�Ƿ����stdin",
#define CFG_STDIO_FLOAT_PRINT      true          // "֧�ָ����ӡ"
#define CFG_STDIO_STDIOFILE        true          // "֧�ֱ�׼IO�ļ�"
#define CFG_STDIO_IN_NAME              "/dev/UART1"     // "��׼�����豸��",
#define CFG_STDIO_OUT_NAME             "/dev/UART1"     // "��׼����豸��",
#define CFG_STDIO_ERR_NAME             "/dev/UART1"     // "��׼err����豸��",
#define    CFG_MODULE_ENABLE_STDIO         true
//*******************************  Configure shell  ******************************************//
#define CFG_SHELL_STACK            0x1000       // "ִ��shell�����ջ�ߴ�",
#define CFG_ADD_ROUTINE_SHELL      true         // "�Ƿ���ӳ���shell����",
#define CFG_ADD_EXPAND_SHELL       true         // "�Ƿ������չshell����",
#define CFG_ADD_GLOBAL_FUN         false        // "���ȫ�ֺ�����shell",
#define CFG_SHOW_ADD_SHEELL        true         // "��ʾ�ڱ��봰����ӵ�shell����",
#define    CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure network config  ******************************************//
#define CFG_STATIC_IP       true             //  "ʹ�þ�̬IP?",
#define CFG_SELECT_NETCARD  "STM32H7_ETH"    //  "��������",������ѡ�е��������������õ�������ͬ
#define CFG_MY_IPV4         "192.168.0.179"  //  "��̬IP",
#define CFG_MY_SUBMASK      "255.255.255.0"  //  "��������",
#define CFG_MY_GATWAY       "192.168.0.1"    //  "����",
#define CFG_MY_DNS          "192.168.0.1"    //  "DNS",
#define    CFG_MODULE_ENABLE_NETWORK_CONFIG  true
//*******************************  Configure touchscreen FT5X26  ******************************************//
#define CT_MAX_TOUCH  5                          // "������",֧�����5�㴥��
#define CFG_TOUCH_ADJUST_FILE   "/yaf2/touch_init.dat"   // ���津���������������ļ�
#define CFG_FT5X26_BUS_NAME     "IoIic"         // "IIC��������",����оƬʹ�õ�IIC��������
#define CFG_FT5X26_TOUCH_NAME   "FT5X26"        // "�����豸����",
#define CFG_FT5X26_DISPLAY_NAME "display"      //
#define    CFG_MODULE_ENABLE_TOUCHSCREEN_FT5X26  true
//*******************************  Core Clock  ******************************************//
#define  CFG_CORE_MCLK                   (400*Mhz)         //��Ƶ���ں�Ҫ�ã����붨��
//*******************************  DjyosProduct Configuration  ******************************************//
#define    PRODUCT_MANUFACTURER_NAME       ""                //��������
#define    PRODUCT_PRODUCT_CLASSIFY        ""                //��Ʒ����
#define    PRODUCT_PRODUCT_MODEL           ""                //��Ʒ�ͺ�
#define    PRODUCT_VERSION_LARGE           0                 //�汾��,__.xx.xx
#define    PRODUCT_VERSION_MEDIUM          0                 //�汾��,xx.__.xx
#define    PRODUCT_VERSION_SMALL           0                 //�汾��,xx.xx.__
#define    PRODUCT_PRODUCT_MODEL_CODE      ""                //��Ʒ�ͺű���
#define    PRODUCT_PASSWORD                ""                //��Ʒ��Կ
#define    PRODUCT_OTA_ADDRESS             ""                //OTA��������ַ
#define    PRODUCT_BOARD_TYPE              ""                //�������
#define    PRODUCT_CPU_TYPE                ""                //CPU����



#endif
