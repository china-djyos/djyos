/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure NlsCharset  ******************************************//
#define     CFG_LOCAL_CHARSET                "gb2312"               // "�����ַ���",
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          100           // "��������",�������������������ź����ͻ�����
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true       // "ȫ���ܶ�̬����",��ʹѡfalse��Ҳ����ʹ��malloc-free�����ڴ棬��ʹ���в����� �������û��ֲᡷ�ڴ�����½�
//*******************************  Configure DevFile  ******************************************//
#define CFG_DEVFILE_LIMIT       20           // "�豸����",�����豸����
//*******************************  Configure font  ******************************************//
#define     CFG_FONT_DEFAULT                 CN_FONT_GB2312_SONG_16     // "Ĭ������",��������include/fontĿ¼����
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
#define CFG_DESKTOP_WIDTH       480                // "������",����ߴ磨����������ȣ�0=��ʾ�����
#define CFG_DESKTOP_HEIGHT      272                // "����߶�",����ߴ磨���������߶ȣ�0=��ʾ���߶�
#define CFG_DISPLAY_NAME        "LCD_DISPLAY"              // "��ʾ����",����bsp����ʾ������ģ�����õ���ʾ��������ͬ
#define CFG_DESKTOP_NAME        "Desktop"              // "������"
#define CFG_INPUTDEV_NAME       "TOUCH_FT5406"              // "�����豸����",ʹ��bsp�������豸�����õ����֣��������豸�Ļ���ÿ���豸���ö��Ÿ���
#define CFG_DESKTOP_FORMAT      CN_SYS_PF_RGB565         // "���ظ�ʽ",���洰�����ظ�ʽ��������gkernel.h�ж��壬һ��ʹ������ʾ����ͬ��ɫ
#define CFG_GRAY_BASE_COLOR     CN_COLOR_WHITE           // "�ҶȻ�ɫ",���ظ�ʽ��Ϊ�Ҷ�ʱ����Ҫ���õġ�������ɫ������gkernel.h���ҵ�������ɫ����
#define CFG_FILL_COLOR          CN_COLOR_BLUE            // "���ɫ",��������ʱ�����ɫ����888��ʽ������gkernel.h���ҵ�������ɫ����
//*******************************  Configure HmiInput  ******************************************//
#define     CFG_HMIIN_DEV_LIMIT              2                      // "name",�˻����������豸����������̡�����
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_RUN_MODE            (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW)      // "STDIOģʽ",
#define CFG_STDIO_IN_NAME              "/dev/UART1"     // "�����豸��",
#define CFG_STDIO_OUT_NAME             "/dev/UART1"     // "����豸��",
#define CFG_STDIO_ERR_NAME             "/dev/UART1"     // "err����豸��",
//*******************************  Configure cpu_peri_uart  ******************************************//
#define CFG_UART1_SENDBUF_LEN    64                  // "UART1���ͻ����С",
#define CFG_UART1_RECVBUF_LEN    64                  // "UART1���ջ����С",
#define CFG_UART2_SENDBUF_LEN    64                  // "UART2���ͻ����С",
#define CFG_UART2_RECVBUF_LEN    64                  // "UART2���ջ����С",
#define CFG_UART3_SENDBUF_LEN    64                  // "UART3���ͻ����С",
#define CFG_UART3_RECVBUF_LEN    64                  // "UART3���ջ����С",
#define CFG_UART4_SENDBUF_LEN    64                  // "UART4���ͻ����С",
#define CFG_UART4_RECVBUF_LEN    64                  // "UART4���ջ����С",
#define CFG_UART5_SENDBUF_LEN    64                  // "UART5���ͻ����С",
#define CFG_UART5_RECVBUF_LEN    64                  // "UART5���ջ����С",
#define CFG_UART6_SENDBUF_LEN    64                  // "UART6���ͻ����С",
#define CFG_UART6_RECVBUF_LEN    64                  // "UART6���ջ����С",
#define CFG_UART7_SENDBUF_LEN    64                  // "UART7���ͻ����С",
#define CFG_UART7_RECVBUF_LEN    64                  // "UART7���ջ����С",
#define CFG_UART8_SENDBUF_LEN    64                  // "UART8���ͻ����С",
#define CFG_UART8_RECVBUF_LEN    64                  // "UART8���ջ����С",
#define CFG_UART1_ENABLE         true                // "����ʹ��UART1",
#define CFG_UART2_ENABLE         false               // "����ʹ��UART2",
#define CFG_UART3_ENABLE         false               // "����ʹ��UART3",
#define CFG_UART4_ENABLE         false               // "����ʹ��UART4",
#define CFG_UART5_ENABLE         false               // "����ʹ��UART5",
#define CFG_UART6_ENABLE         false               // "����ʹ��UART6",
#define CFG_UART7_ENABLE         false               // "����ʹ��UART7",
#define CFG_UART8_ENABLE         false               // "����ʹ��UART8",
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096         // "��ʼ��ջ�ռ�",�����ʼ������ʹ�õ�ջ�ռ䣬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         50           // "�¼�������ֵ",�¼�����
#define CFG_EVENT_TYPE_LIMIT    50           // "�¼���������ֵ",�¼�������
#define CFG_MAINSTACK_LIMIT     4096         // "main����ջ�ߴ�",main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024         // "IDLE�¼�ջ�ߴ�",IDLE�¼����������е�ջ�ߴ磬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_OS_TINY             false        // "tiny���ں�",true=������Դ��ȱ�ĳ��ϣ��ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
//*******************************  Configure MemoryPool  ******************************************//
#define CFG_MEMPOOL_LIMIT       20           // "�ڴ������",������10���ڴ��
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8           // "ʱ��",����ʱ���Ƕ�8��
//*******************************  Configure loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN          // EN_DIRECT_RUN=ֱ�Ӵ�flash�����У�EN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define  CFG_IBOOT_CRC   EN_NO_APP_CRC           // �Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define CFG_APP_FILENAME        ""                   // CFG_APP_RUNMODE==EN_FORM_FILE�����������ļ���
#define CFG_IBOOT_VERSION       "V1.0.0"             // Iboot�����汾��
#define CFG_IBOOT_UPDATE_NAME   "iboot.bin"          // iboot�����ļ������˶����ã��Ժ�ĵ�
#define CFG_IBOOT_UPDATE_PATH   "\\"          // iboot����·�������˶����ã��Ժ�ĵ�
//*******************************  Configure ft5406  ******************************************//
#define CFG_FT5406_RT_I2C_ADDRESS   0x38                     // "�豸��ַ",Ӳ������IIC�豸��ַ
#define CFG_FT5406_RT_I2C_CLK_FRE   (100*1000)                // "�����ٶ�",��λHz
#define CFG_FT5406_BUS_NAME          "IIC1"             // "IIC��������",����оƬʹ�õ�IIC��������
#define CFG_FT5406_TOUCH_NAME        "TOUCH_FT5406"     // "����������",����оƬ������
#define CFG_FT5406_DESKTOP_NAME      "Desktop"                 // "��������",��������ʹ�õ����������
//*******************************  Configure cpu_peri_iic  ******************************************//
#define     CFG_IIC1_BUF_LEN                 128                // "IIC1��������С",
#define     CFG_IIC2_BUF_LEN                 128                // "IIC2��������С",
#define     CFG_IIC3_BUF_LEN                 128                // "IIC3��������С",
#define     CFG_IIC4_BUF_LEN                 128                // "IIC4��������С",
#define     CFG_IIC1_ENABLE                  true               // "�Ƿ�����ʹ��IIC1",
#define     CFG_IIC2_ENABLE                  false              // "�Ƿ�����ʹ��IIC2",
#define     CFG_IIC3_ENABLE                  false              // "�Ƿ�����ʹ��IIC3",
#define     CFG_IIC4_ENABLE                  false              // "�Ƿ�����ʹ��IIC4",
//*******************************  Configure cpu_peri_lcd  ******************************************//
#define     CFG_LCD_DISPLAY_NAME             "LCD_DISPLAY"          // "��ʾ������",
#define     CFG_LCD_HEAP_NAME                "sdram"                  // "��ʾʹ�õĶ�����",
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (600*Mhz)          //��Ƶ���ں�Ҫ�ã����붨��

#endif