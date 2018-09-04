/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure DevFile  ******************************************//
#define CFG_DEVFILE_LIMIT       20      //�����豸����
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          100      //�����������������ź����ͻ�����
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096    //�����ʼ������ʹ�õ�ջ�ռ�һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         50      //�¼�����
#define CFG_EVENT_TYPE_LIMIT    50      //�¼�������
#define CFG_MAINSTACK_LIMIT     4096    //main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024    //IDLE�¼����������е�ջ�ߴ�һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_OS_TINY             false   //true=������Դ��ȱ�ĳ����ں˻�ü������¼����������¼�����ʱ��ͳ�ơ�
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true  //��ʹѡfalseҲ����ʹ��malloc-free�����ڴ浫ʹ���в����� �������û��ֲᡷ�ڴ�����½�
//*******************************  Configure MemoryPool  ******************************************//
#define CFG_MEMPOOL_LIMIT       20      //������10���ڴ��
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8      //����ʱ���Ƕ�8��
//*******************************  Configure loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN     //EN_DIRECT_RUN=ֱ�Ӵ�flash�����У�EN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define  CFG_IBOOT_CRC   EN_NO_APP_CRC      //�Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define CFG_APP_FILENAME        ""              //CFG_APP_RUNMODE==EN_FORM_FILE�����������ļ���
#define CFG_IBOOT_VERSION       "V1.0.0"        //Iboot�����汾��
#define CFG_IBOOT_UPDATE_NAME   "iboot.bin"     //iboot�����ļ������˶����ã��Ժ�ĵ�
#define CFG_IBOOT_UPDATE_PATH   "\\"     //iboot����·�������˶����ã��Ժ�ĵ�
//*******************************  Configure cpu_peri_hal  ******************************************//
#define CFG_STM32_HAL_TIMER     TIM6//����HAL��ʹ�õĶ�ʱ��
//*******************************  Configure NlsCharset  ******************************************//
#define CFG_LOCAL_CHARSET       "gb2312"        //
//*******************************  Configure font  ******************************************//
#define CFG_FONT_DEFAULT  CN_FONT_GB2312_SONG_16      //��������include/fontĿ¼����
//*******************************  Configure ascii_font  ******************************************//
#define CFG_ASCII_8X8         0         //8*8����ascii����
#define CFG_ASCII_6X12        0         //6*12����ascii����
#define CFG_ASCII_8X16_SONG   1         //8*16����ascii����_����
#define CFG_ASCII_8X16_YUAN   0         //8*16����ascii����_Բ��
#define CFG_ASCII_8X16_KAI    0         //8*16����ascii����_����
#define CFG_ASCII_8X16_HEI    0         //8*16����ascii����_����
#define CFG_ASCII_8X16_FANG   0         //8*16����ascii����_����
#define CFG_ASCII_12X24_SONG  0         //12*24����ascii����_����
#define CFG_ASCII_12X24_YUAN  0         //12*24����ascii����_Բ��
#define CFG_ASCII_12X24_KAI   0         //12*24����ascii����_����
#define CFG_ASCII_12X24_HEI   0         //12*24����ascii����_����
#define CFG_ASCII_12X24_FANG  0         //12*24����ascii����_����
#define CFG_ASCII_16X32_YUAN  0         //16*32����ascii����_Բ��
#define CFG_ASCII_16X32_KAI   0         //16*32����ascii����_����
#define CFG_ASCII_16X32_HEI   0         //16*32����ascii����_����
#define CFG_ASCII_16X32_FANG  0         //16*32����ascii����_����
//*******************************  Configure fatfilesystem  ******************************************//
#define CFG_OPTIONS             0       //FAT�ļ�ϵͳ�ļ���װ�߼�
#define CFG_MOUNT_POINT         ""      //FAT�ļ�ϵͳ��װĿ¼
#define CFG_MOUNT_DEV           ""      //FAT�ļ�ϵͳ�����豸·��
//*******************************  Configure gkernel  ******************************************//
#define CFG_GKERNEL_CMD_DEEP        1024    //gdd��gkernel��������Ļ��������ȣ��ֽ�����
//*******************************  Configure HmiInput  ******************************************//
#define CFG_HMIIN_DEV_LIMIT     2       //�˻����������豸��������̡�����
//*******************************  Configure gdd  ******************************************//
#define     CFG_DESKTOP_WIDTH                0                 //����ߴ磨����������ȣ�0=��ʾ�����
#define     CFG_DESKTOP_HEIGHT               0                 //����ߴ磨���������߶ȣ�0=��ʾ���߶�
#define     CFG_DISPLAY_NAME                 "lcdili9325"      //����bsp����ʾ������ģ�����õ���ʾ��������ͬ
#define     CFG_DESKTOP_NAME                 "Desktop"         //
#define     CFG_INPUTDEV_NAME                "TOUCH_STMPE811"  //ʹ��bsp�������豸�����õ����֣��������豸�Ļ���ÿ���豸���ö��Ÿ���
#define     CFG_DESKTOP_FORMAT               CN_SYS_PF_RGB565  //���洰�����ظ�ʽ��������gkernel.h�ж��壬һ��ʹ������ʾ����ͬ��ɫ
#define     CFG_GRAY_BASE_COLOR              CN_COLOR_WHITE    //���ظ�ʽ��Ϊ�Ҷ�ʱ����Ҫ���õġ�������ɫ������gkernel.h���ҵ�������ɫ����
#define     CFG_FILL_COLOR                   CN_COLOR_BLUE     //��������ʱ�����ɫ����888��ʽ������gkernel.h���ҵ�������ɫ����
//*******************************  Configure stmpe811  ******************************************//
#define CFG_CRT_CLK_FRE                 (100*1000)      //�����ٶȵ�λHz
#define CFG_STMPE811_DEVADDR            0x41            //IIC�����ϵ��豸��ַ
#define CFG_STMPE811_TOUCH_DEV_NAME      "TOUCH_STMPE811"     //�����豸������
#define CFG_STMPE811_DESKTOP_NAME  "LCD_DESKTOP_STMPE811"     //�������������������
//*******************************  Configure stdio  ******************************************//
#define     CFG_STDIO_RUN_MODE               (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW)//
#define     CFG_STDIO_IN_NAME                "/dev/UART2"      //
#define     CFG_STDIO_OUT_NAME               "/dev/UART2"      //
#define     CFG_STDIO_ERR_NAME               "/dev/UART2"      //
//*******************************  Configure cpu_peri_uart  ******************************************//
#define     CFG_UART1_SENDBUF_LEN            32                //
#define     CFG_UART1_RECVBUF_LEN            32                //
#define     CFG_UART1_DMABUF_LEN             32                //
#define     CFG_UART2_SENDBUF_LEN            32                //
#define     CFG_UART2_RECVBUF_LEN            32                //
#define     CFG_UART2_DMABUF_LEN             32                //
#define     CFG_UART3_SENDBUF_LEN            32                //
#define     CFG_UART3_RECVBUF_LEN            32                //
#define     CFG_UART3_DMABUF_LEN             32                //
#define     CFG_UART4_SENDBUF_LEN            32                //
#define     CFG_UART4_RECVBUF_LEN            32                //
#define     CFG_UART5_SENDBUF_LEN            32                //
#define     CFG_UART5_RECVBUF_LEN            32                //
#define     CFG_UART1_ENABLE                 false             //
#define     CFG_UART2_ENABLE                 true              //
#define     CFG_UART3_ENABLE                 false             //
#define     CFG_UART4_ENABLE                 false             //
#define     CFG_UART5_ENABLE                 false             //
//*******************************  Configure timer  ******************************************//
#define CFG_TIMER_SOUCE     1       //1=��Ӳ����ʱ���ṩʱ��Դ0=��tick�ṩʱ��Դ
#define CFG_TIMERS_LIMIT    5       //�ɴ����Ķ�ʱ��������������ͼ�ν���Ķ�ʱ����
//*******************************  Configure wdt  ******************************************//
#define CFG_WDT_LIMIT           10      //������������
#define CFG_WDTMSG_LIMIT        3       //�������Ź�����Ϣ���е���󳤶�
//*******************************  Configure ili9325  ******************************************//
#define CFG_ILI9325_DISPLAY_NAME              "lcdili9325"    //����Һ����ʾ������
#define CFG_ILI9325_HEAP_NAME                 "extram"        //����Һ��������ʹ�õĶ�����
//*******************************  Configure cpu_peri_iic  ******************************************//
#define     CFG_I2C1_BUF_LEN                 128               //
#define     CFG_I2C2_BUF_LEN                 128               //
#define     CFG_I2C1_ENABLE                  true              //
#define     CFG_I2C2_ENABLE                  true              //
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (8*Mhz)           //��Ƶ���ں�Ҫ�ã����붨��

#endif
