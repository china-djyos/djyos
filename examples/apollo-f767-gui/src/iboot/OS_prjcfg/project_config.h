/****************************************************
 *  Automatically-generated file. Do not edit!  *
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10      //�����豸����
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40               // "��������",�������������������ź����ͻ�����
//*******************************  Configure io iic config  ******************************************//
#define CFG_IO_IIC_BUS_NAME   "Ioiic"
//*******************************  Configure font  ******************************************//
#define     CFG_FONT_DEFAULT                 "gb2312_song_16"       // "Ĭ������",��������include/fontĿ¼����
//*******************************  Configure gb2312 dot  ******************************************//
#define CFG_GB2312_12_SONG              zk_disable              //GB2312����zk_disable������Ҫfrom_array���������ȡfrom_file�����ļ���
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"  //�����ļ���ȡ�������ļ���
#define CFG_GB2312_16_SONG              from_array              //GB2312����zk_disable������Ҫfrom_array���������ȡfrom_file�����ļ���
    #define CFG_GB2312_16_SONG_FILENAME "zk_gb2316_16song.bin"  //�����ļ���ȡ�������ļ���
#define CFG_GB2312_16_YUAN              zk_disable              //GB2312����zk_disable������Ҫfrom_array���������ȡfrom_file�����ļ���
    #define CFG_GB2312_16_YUAN_FILENAME "zk_gb2316_16yuan.bin"  //�����ļ���ȡ�������ļ���
#define CFG_GB2312_16_KAI              zk_disable               //GB2312����zk_disable������Ҫfrom_array���������ȡfrom_file�����ļ���
    #define CFG_GB2312_16_KAI_FILENAME  "zk_gb2316_16kai.bin"   //�����ļ���ȡ�������ļ���
#define CFG_GB2312_16_HEI              zk_disable               //GB2312����zk_disable������Ҫfrom_array���������ȡfrom_file�����ļ���
    #define CFG_GB2312_16_HEI_FILENAME  "zk_gb2316_16hei.bin"   //�����ļ���ȡ�������ļ���
#define CFG_GB2312_16_FANG              zk_disable              //GB2312����zk_disable������Ҫfrom_array���������ȡfrom_file�����ļ���
    #define CFG_GB2312_16_FANG_FILENAME "zk_gb2316_16fang.bin"  //�����ļ���ȡ�������ļ���
#define CFG_GB2312_24_SONG              zk_disable              //GB2312����zk_disable������Ҫfrom_array���������ȡfrom_file�����ļ���
    #define CFG_GB2312_24_SONG_FILENAME "zk_gb2316_24song.bin"  //�����ļ���ȡ�������ļ���
#define CFG_GB2312_24_YUAN              zk_disable              //GB2312����zk_disable������Ҫfrom_array���������ȡfrom_file�����ļ���
    #define CFG_GB2312_24_YUAN_FILENAME "zk_gb2316_24yuan.bin"  //�����ļ���ȡ�������ļ���
#define CFG_GB2312_24_KAI              zk_disable               //GB2312����zk_disable������Ҫfrom_array���������ȡfrom_file�����ļ���
    #define CFG_GB2312_24_KAI_FILENAME  "zk_gb2316_24kai.bin"   //�����ļ���ȡ�������ļ���
#define CFG_GB2312_24_HEI              zk_disable               //GB2312����zk_disable������Ҫfrom_array���������ȡfrom_file�����ļ���
    #define CFG_GB2312_24_HEI_FILENAME  "zk_gb2316_24hei.bin"   //�����ļ���ȡ�������ļ���
#define CFG_GB2312_24_FANG              zk_disable              //GB2312����zk_disable������Ҫfrom_array���������ȡfrom_file�����ļ���
    #define CFG_GB2312_24_FANG_FILENAME "zk_gb2316_24fang.bin"  //�����ļ���ȡ�������ļ���
#define CFG_GB2312_32_SONG              zk_disable              //GB2312����zk_disable������Ҫfrom_array���������ȡfrom_file�����ļ���
    #define CFG_GB2312_32_SONG_FILENAME "zk_gb2316_32song.bin"  //�����ļ���ȡ�������ļ���
#define CFG_GB2312_32_YUAN              zk_disable              //GB2312����zk_disable������Ҫfrom_array���������ȡfrom_file�����ļ���
    #define CFG_GB2312_32_YUAN_FILENAME "zk_gb2316_32yuan.bin"  //�����ļ���ȡ�������ļ���
#define CFG_GB2312_32_KAI              zk_disable               //GB2312����zk_disable������Ҫfrom_array���������ȡfrom_file�����ļ���
    #define CFG_GB2312_32_KAI_FILENAME  "zk_gb2316_32kai.bin"   //�����ļ���ȡ�������ļ���
#define CFG_GB2312_32_HEI              zk_disable               //GB2312����zk_disable������Ҫfrom_array���������ȡfrom_file�����ļ���
    #define CFG_GB2312_32_HEI_FILENAME  "zk_gb2316_32hei.bin"   //�����ļ���ȡ�������ļ���
#define CFG_GB2312_32_FANG              zk_disable              //GB2312����zk_disable������Ҫfrom_array���������ȡfrom_file�����ļ���
    #define CFG_GB2312_32_FANG_FILENAME "zk_gb2316_32fang.bin"  //�����ļ���ȡ�������ļ���
//*******************************  Configure graphical kernel  ******************************************//
#define CFG_GKERNEL_CMD_DEEP        1024    //gdd��gkernel��������Ļ��������ȣ��ֽ�����
//*******************************  Configure graphical decorate development  ******************************************//
#define CFG_DESKTOP_WIDTH       0           //����ߴ磨�����������0=��ʾ�����
#define CFG_DESKTOP_HEIGHT      0           //����ߴ磨���������߶�0=��ʾ���߶�
#define CFG_DISPLAY_NAME        "DISPLAY_NAME"         //����bsp����ʾ������ģ�����õ���ʾ��������ͬ
#define CFG_DESKTOP_NAME        "DESKTOP_NAME"         //
#define CFG_INPUTDEV_NAME       "INPUTDEV_NAME"         //ʹ��bsp�������豸�����õ����ֶ������豸�Ļ�ÿ���豸���ö��Ÿ���
#define CFG_DESKTOP_FORMAT      CN_SYS_PF_RGB565    //���洰�����ظ�ʽ������gkernel.h�ж���һ��ʹ������ʾ����ͬ��ɫ
#define CFG_GRAY_BASE_COLOR     CN_COLOR_WHITE      //���ظ�ʽ��Ϊ�Ҷ�ʱ����Ҫ���õġ�������ɫ����gkernel.h���ҵ�������ɫ����
#define CFG_FILL_COLOR          CN_COLOR_BLUE       //��������ʱ�����ɫ��888��ʽ����gkernel.h���ҵ�������ɫ����
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true           // "ȫ���ܶ�̬����",��ʹѡfalse��Ҳ����ʹ��malloc-free�����ڴ棬��ʹ���в����� �������û��ֲᡷ�ڴ�����½�
//*******************************  Configure human machine interface  ******************************************//
#define CFG_HMIIN_DEV_LIMIT     2       //�˻����������豸��������̡�����
//*******************************  Configure kernel  ******************************************//
#define     CFG_INIT_STACK_SIZE              4096                      // "��ʼ��ջ�ռ�",�����ʼ������ʹ�õ�ջ�ռ䣬һ�㰴Ĭ��ֵ�Ϳ�����
#define     CFG_EVENT_LIMIT                  30                        // "�¼�������ֵ",�¼�����
#define     CFG_EVENT_TYPE_LIMIT             30                        // "�¼���������ֵ",�¼�������
#define     CFG_MAINSTACK_LIMIT              4096                      // "main����ջ�ߴ�",main�������������ջ�ߴ�
#define     CFG_IDLESTACK_LIMIT              1024                      // "IDLE�¼�ջ�ߴ�",IDLE�¼����������е�ջ�ߴ磬һ�㰴Ĭ��ֵ�Ϳ�����
#define     CFG_OS_TINY                      false                     // "tiny���ں�",true=������Դ��ȱ�ĳ��ϣ��ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8   //������Զ�����
#define CFG_HANDLE_LIMIT        8   //������Զ�����
//*******************************  Configure loader  ******************************************//
#define  CFG_APP_RUNMODE  CN_DIRECT_RUN              // CN_DIRECT_RUN=ֱ�Ӵ�flash�����У�CN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL               // �Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define CFG_IBOOT_VERSION       01                 // Iboot�����汾��
#define CFG_IBOOT_UPDATE_NAME      "/yaf2/iboot.bin"                    // "Iboot����"
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10      //
//*******************************  Configure Nls Charset  ******************************************//
#define CFG_LOCAL_CHARSET       "gb2312"        //
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_STDIN_MULTI      true     // "�Ƿ�֧�ֶ��������豸",
#define CFG_STDIO_STDOUT_FOLLOW    true     // "stdout�Ƿ����stdin",
#define CFG_STDIO_STDERR_FOLLOW    true     // "stderr�Ƿ����stdin",
#define CFG_STDIO_FLOAT_PRINT       true         // "֧�ָ����ӡ"
#define CFG_STDIO_STDIOFILE         true         // "֧�ֱ�׼IO�ļ�"
#define CFG_STDIO_IN_NAME              "/dev/UART1"     // "��׼�����豸��",
#define CFG_STDIO_OUT_NAME             "/dev/UART1"     // "��׼����豸��",
#define CFG_STDIO_ERR_NAME             "/dev/UART1"     // "��׼err����豸��",
//*******************************  Configure shell  ******************************************//
#define CFG_SHELL_STACK            0x1000       // "ִ��shell�����ջ�ߴ�"
#define     CFG_ADD_ROUTINE_SHELL            true                  // "�Ƿ���ӳ���shell����"
#define     CFG_ADD_EXPAND_SHELL             true                  // "�Ƿ������չshell����"
#define     CFG_ADD_GLOBAL_FUN               false                 // "���ȫ�ֺ�����shell"
#define     CFG_SHOW_ADD_SHEELL              true                  // "��ʾ�ڱ��봰����ӵ�shell����"
#define  CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure tcpip  ******************************************//
#define     CFG_NETPKG_MEMSIZE               0x4000                    // "���ݰ��������ߴ�"
#define  CFG_MODULE_ENABLE_TCPIP         true
#define  CFG_MODULE_ENABLE_TCPIP         true
//*******************************  Configure ethernet phy lan8720  ******************************************//
#define CFG_LAN8720_PHY_ADDRESS     0   //phy�������ַ��Ӳ��һ�����Ϊ 0
//*******************************  Configure network config  ******************************************//
#define CFG_STATIC_IP       true            //
#define CFG_SELECT_NETCARD  "STM32F7_ETH"   //������ѡ�е��������������õ�������ͬ
#define CFG_MY_IPV4         "192.168.0.179" //
#define CFG_MY_SUBMASK      "255.255.255.0" //
#define CFG_MY_GATWAY       "192.168.0.1"   //
#define CFG_MY_DNS          "192.168.0.1"   //
//*******************************  Configure tcp  ******************************************//
#define     CFG_TCP_REORDER                  true                      // "TCP��������ʹ��",��Դ����Ŵ�
#define     CFG_TCP_CCBNUM                   10                        // "tcp �ͻ�������ֵ"��ռһ�� ָ�� �� struct ClienCB
#define     CFG_TCP_SCBNUM                   5                         // "tcp ����������ֵ"��ռһ�� ָ�� �� struct ServerCB
#define  CFG_MODULE_ENABLE_TCP           true
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8               // "ʱ��",����ʱ���Ƕ�8��
//*******************************  Configure cpu onchip MAC  ******************************************//
#define CFG_ETH_NETCARD_NAME    "STM32F7_ETH"   //
#define CFG_ETH_LOOP_CYCLE      1000         //�ж�ģʽ������д
#define CFG_ETH_LOOP_ENABLE     true            //
#define CFG_ETH_HARD_MAC_ADDR   true            //
#define CFG_ETH_MAC_ADDR0      00           //��ѡ��"Ӳ������Mac��ַ"��������д
#define CFG_ETH_MAC_ADDR1      02           //��ѡ��"Ӳ������Mac��ַ"��������д
#define CFG_ETH_MAC_ADDR2      03           //��ѡ��"Ӳ������Mac��ַ"��������д
#define CFG_ETH_MAC_ADDR3      04           //��ѡ��"Ӳ������Mac��ַ"��������д
#define CFG_ETH_MAC_ADDR4      05           //��ѡ��"Ӳ������Mac��ַ"��������д
#define CFG_ETH_MAC_ADDR5      06           //��ѡ��"Ӳ������Mac��ַ"��������д
//*******************************  Configure cpu onchip uart  ******************************************//
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
#define CFG_UART3_ENABLE           false       //
#define CFG_UART4_ENABLE           false       //
#define CFG_UART5_ENABLE           false       //
#define CFG_UART6_ENABLE           false       //
#define CFG_UART7_ENABLE           false       //
#define CFG_UART8_ENABLE           false       //
//*******************************  Configure IIC to 8gpio pcf8574  ******************************************//
#define CFG_PCF8574_BUS_NAME       "IoIic"        //
//*******************************  Configure keyboard hard driver  ******************************************//
#define CFG_KEYBOARD_NAME              "KEYBOARD"        //���ü�������
//*******************************  Configure touchscreen FT5X26  ******************************************//
#define CT_MAX_TOUCH  5                         //֧�����5�㴥��
#define CFG_TOUCH_ADJUST_FILE   "/yaf2/touch_init.dat"  //���津���������������ļ�
#define CFG_FT5X26_BUS_NAME     "IoIic"        //����оƬʹ�õ�IIC��������
#define CFG_FT5X26_TOUCH_NAME   "FT5X26"       //���ô���������
#define CFG_DESKTOP_NAME        "desktop"      //���ô�����������ʾ�����������
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (216*Mhz)         //��Ƶ���ں�Ҫ�ã����붨��

#endif
