/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"
#include "board.h"
//*******************************  Configure NlsCharset  ******************************************//
#define     CFG_LOCAL_CHARSET                "gb2312"               // "�����ַ���",
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          100           // "��������",�������������������ź����ͻ�����
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true       // "ȫ���ܶ�̬����",��ʹѡfalse��Ҳ����ʹ��malloc-free�����ڴ棬��ʹ���в����� �������û��ֲᡷ�ڴ�����½�
//*******************************  Configure DevFile  ******************************************//
#define CFG_DEVFILE_LIMIT       12           // "�豸����",�����豸����
//*******************************  Configure ioiicbus  ******************************************//
#define IO_IIC_USER_TAG      1         // �û��Լ��ı�ǩ�����ڻص��û��ṩ��IO���ƺ����ĵڶ�������
#define IO_IIC_BUS_NAME "IoIic"       // ��IIC���ߵ�����
#define IIC_IOCTRL_FUN    IIC_IoCtrlFunc    // �û�ʵ�ֵ�IO���ƺ�����
//*******************************  Configure font  ******************************************//
#define CFG_FONT_DEFAULT  CN_FONT_GB2312_SONG_16           // "Ĭ������",��������include/fontĿ¼����
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
#define CFG_GKERNEL_CMD_DEEP        1024         // "����������",gdd��gkernel��������Ļ��������ȣ��ֽ�����
//*******************************  Configure gdd  ******************************************//
#define     CFG_DESKTOP_WIDTH                1024                     // "������",����ߴ磨����������ȣ�0=��ʾ�����
#define     CFG_DESKTOP_HEIGHT               600                     // "����߶�",����ߴ磨���������߶ȣ�0=��ʾ���߶�
#define     CFG_DISPLAY_NAME                 "LCD_F7"        // "��ʾ����",����bsp����ʾ������ģ�����õ���ʾ��������ͬ
#define     CFG_DESKTOP_NAME                 "desktop"        // "������"
#define     CFG_INPUTDEV_NAME                "FT5X26"       // "�����豸����",ʹ��bsp�������豸�����õ����֣��������豸�Ļ���ÿ���豸���ö��Ÿ���
#define     CFG_DESKTOP_FORMAT               CN_SYS_PF_RGB565      // "���ظ�ʽ",���洰�����ظ�ʽ��������gkernel.h�ж��壬һ��ʹ������ʾ����ͬ��ɫ
#define     CFG_GRAY_BASE_COLOR              CN_COLOR_WHITE        // "�ҶȻ�ɫ",���ظ�ʽ��Ϊ�Ҷ�ʱ����Ҫ���õġ�������ɫ������gkernel.h���ҵ�������ɫ����
#define     CFG_FILL_COLOR                   CN_COLOR_BLUE         // "���ɫ",��������ʱ�����ɫ����888��ʽ������gkernel.h���ҵ�������ɫ����
//*******************************  Configure stdio  ******************************************//
#define     CFG_STDIO_RUN_MODE               (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW)     // "STDIOģʽ",
#define     CFG_STDIO_IN_NAME                "/dev/UART2"           // "��׼�����豸��",
#define     CFG_STDIO_OUT_NAME               "/dev/UART2"           // "��׼����豸��",
#define     CFG_STDIO_ERR_NAME               "/dev/UART2"           // "��׼err����豸��",
//*******************************  Configure cpu_peri_uart  ******************************************//
#define CFG_UART_SENDBUF_LEN            32                       // "���ڷ��ͻ��λ�������С",
#define CFG_UART_RECVBUF_LEN            32                       // "���ڽ��ջ��λ�������С",
#define CFG_UART1_ENABLE                false                     // "�����Ƿ�ʹ��UART1",
#define CFG_UART2_ENABLE                true                    // "�����Ƿ�ʹ��UART2",
#define CFG_UART3_ENABLE                false                    // "�����Ƿ�ʹ��UART3",
#define CFG_UART4_ENABLE                false                    // "�����Ƿ�ʹ��UART4",
#define CFG_UART5_ENABLE                false                    // "�����Ƿ�ʹ��UART5",
#define CFG_UART6_ENABLE                false                    // "�����Ƿ�ʹ��UART6",
#define CFG_UART7_ENABLE                false                    // "�����Ƿ�ʹ��UART7",
#define CFG_UART8_ENABLE                false                    // "�����Ƿ�ʹ��UART8",
//*******************************  Configure shell  ******************************************//
#define     CFG_ADD_ROUTINE_SHELL            true                   // "�Ƿ���ӳ���shell����"
#define     CFG_ADD_EXPAND_SHELL             true                   // "�Ƿ������չshell����"
#define     CFG_ADD_GLOBAL_FUN               false                  // "���ȫ�ֺ�����shell"
#define     CFG_SHOW_ADD_SHEELL              true                   // "��ʾ�ڱ��봰����ӵ�shell����"
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096         // "��ʼ��ջ�ռ�",�����ʼ������ʹ�õ�ջ�ռ䣬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         50           // "�¼�������ֵ",�¼�����
#define CFG_EVENT_TYPE_LIMIT    50           // "�¼���������ֵ",�¼�������
#define CFG_MAINSTACK_LIMIT     4096         // "main����ջ�ߴ�",main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024         // "IDLE�¼�ջ�ߴ�",IDLE�¼����������е�ջ�ߴ磬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_OS_TINY             false        // "tiny���ں�",true=������Դ��ȱ�ĳ��ϣ��ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
//*******************************  Configure MemoryPool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10           // "�ڴ������",������10���ڴ��
//*******************************  Configure loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN          // EN_DIRECT_RUN=ֱ�Ӵ�flash�����У�EN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL           // �Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define CFG_IBOOT_VERSION       01             // Iboot�����汾��
//*******************************  Configure FT5X26  ******************************************//
#define     CT_MAX_TOUCH                     5                    // "������",֧�����5�㴥��
#define     CFG_FT5X26_BUS_NAME              "IoIic"              // "IIC��������",����оƬʹ�õ�IIC��������
#define     CFG_TOUCH_NAME                   "FT5X26"             // "����������",���ô���������
#define     CFG_DESKTOP_NAME                 "desktop"            // "��������",���ô�����������ʾ�����������
//*******************************  Configure cpu_peri_lcd  ******************************************//
#define     CFG_LCD_XSIZE                    1024                    // "LCDˮƽ���ؿ��",
#define     CFG_LCD_YSIZE                    600                    // "LCD��ֱ���ؿ��",
#define     CFG_DISPLAY_NAME                 "LCD_F7"               // "��ʾ��LCD����",
#define     CFG_LCD_HEAP_NAME                "extram"               // "LCDʹ�ö�����",
//*******************************  Configure HmiInput  ******************************************//
#define CFG_HMIIN_DEV_LIMIT     2       //�˻����������豸��������̡�����
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (400*Mhz)         //��Ƶ���ں�Ҫ�ã����붨��

#endif
