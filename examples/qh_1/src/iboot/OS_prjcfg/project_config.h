/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure NlsCharset  ******************************************//
#define     CFG_LOCAL_CHARSET                "gb2312"            // "�����ַ���",
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          100        // "��������",�������������������ź����ͻ�����
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true    // "ȫ���ܶ�̬����",��ʹѡfalse��Ҳ����ʹ��malloc-free�����ڴ棬��ʹ���в����� �������û��ֲᡷ�ڴ�����½�
//*******************************  Configure DevFile  ******************************************//
#define CFG_DEVFILE_LIMIT       10        // "�豸����",�����豸����
//*******************************  Configure font  ******************************************//
#define     CFG_FONT_DEFAULT                 CN_FONT_GB2312_SONG_16  // "Ĭ������",��������include/fontĿ¼����
//*******************************  Configure ascii_font  ******************************************//
#define     CFG_ASCII_8X8                    0                   // "ASCII_8��8",8*8����ascii����
#define     CFG_ASCII_6X12                   0                   // "ASCII_6��12",6*12����ascii����
#define     CFG_ASCII_8X16_SONG              1                   // "ASCII_8��16����",8*16����ascii����_����
#define     CFG_ASCII_8X16_YUAN              0                   // "ASCII_8��16Բ��",8*16����ascii����_Բ��
#define     CFG_ASCII_8X16_KAI               0                   // "ASCII_8��16����",8*16����ascii����_����
#define     CFG_ASCII_8X16_HEI               0                   // "ASCII_8��16����",8*16����ascii����_����
#define     CFG_ASCII_8X16_FANG              0                   // "ASCII_8��16����",8*16����ascii����_����
#define     CFG_ASCII_12X24_SONG             0                   // "ASCII_12��24����",12*24����ascii����_����
#define     CFG_ASCII_12X24_YUAN             0                   // "ASCII_12��24Բ��",12*24����ascii����_Բ��
#define     CFG_ASCII_12X24_KAI              0                   // "ASCII_12��24����",12*24����ascii����_����
#define     CFG_ASCII_12X24_HEI              0                   // "ASCII_12��24����",12*24����ascii����_����
#define     CFG_ASCII_12X24_FANG             0                   // "ASCII_12��24����",12*24����ascii����_����
#define     CFG_ASCII_16X32_YUAN             0                   // "ASCII_16��32Բ��",16*32����ascii����_Բ��
#define     CFG_ASCII_16X32_KAI              0                   // "ASCII_16��32����",16*32����ascii����_����
#define     CFG_ASCII_16X32_HEI              0                   // "ASCII_16��32����",16*32����ascii����_����
#define     CFG_ASCII_16X32_FANG             0                   // "ASCII_16��32����",16*32����ascii����_����
//*******************************  Configure gkernel  ******************************************//
#define CFG_GKERNEL_CMD_DEEP        1024      // "����������",gdd��gkernel��������Ļ��������ȣ��ֽ�����
//*******************************  Configure gdd  ******************************************//
#define     CFG_DESKTOP_WIDTH                0                   // "�������",����ߴ磨�����������ȣ�0=��ʾ������
#define     CFG_DESKTOP_HEIGHT               0                   // "����߶�",����ߴ磨���������߶ȣ�0=��ʾ���߶�
#define     CFG_DISPLAY_NAME                 "lcdili9325"        // "��ʾ����",����bsp����ʾ������ģ�����õ���ʾ��������ͬ
#define     CFG_DESKTOP_NAME                 "Desktop"           // "������"
#define     CFG_INPUTDEV_NAME                "TOUCH_STMPE811"    // "�����豸����",ʹ��bsp�������豸�����õ����֣��������豸�Ļ���ÿ���豸���ö��Ÿ���
#define     CFG_DESKTOP_FORMAT               CN_SYS_PF_RGB565    // "���ظ�ʽ",���洰�����ظ�ʽ��������gkernel.h�ж��壬һ��ʹ������ʾ����ͬ��ɫ
#define     CFG_GRAY_BASE_COLOR              CN_COLOR_WHITE      // "�ҶȻ�ɫ",���ظ�ʽ��Ϊ�Ҷ�ʱ����Ҫ���õġ�������ɫ������gkernel.h���ҵ�������ɫ����
#define     CFG_FILL_COLOR                   CN_COLOR_BLUE       // "���ɫ",��������ʱ�����ɫ����888��ʽ������gkernel.h���ҵ�������ɫ����
//*******************************  Configure HmiInput  ******************************************//
#define     CFG_HMIIN_DEV_LIMIT              2                   // "name",�˻����������豸����������̡�����
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_RUN_MODE            (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW)   // "STDIOģʽ",
#define CFG_STDIO_IN_NAME              "/dev/UART2"  // "�����豸��",
#define CFG_STDIO_OUT_NAME             "/dev/UART2"  // "����豸��",
#define CFG_STDIO_ERR_NAME             "/dev/UART2"  // "err����豸��",
//*******************************  Configure cpu_peri_uart  ******************************************//
#define CFG_UART1_SENDBUF_LEN            32                   // "UART1���ͻ��λ�������С",
#define CFG_UART1_RECVBUF_LEN            32                   // "UART1���ջ��λ�������С",
#define CFG_UART1_DMABUF_LEN             32                   // "UART1 DMA��������С",
#define CFG_UART2_SENDBUF_LEN            32                   // "UART2���ͻ��λ�������С",
#define CFG_UART2_RECVBUF_LEN            32                   // "UART2���ջ��λ�������С",
#define CFG_UART2_DMABUF_LEN             32                   // "UART2 DMA��������С",
#define CFG_UART3_SENDBUF_LEN            32                   // "UART3���ͻ��λ�������С",
#define CFG_UART3_RECVBUF_LEN            32                   // "UART3���ջ��λ�������С",
#define CFG_UART3_DMABUF_LEN             32                   // "UART3 DMA��������С",
#define CFG_UART4_SENDBUF_LEN            32                    // "UART4���ͻ��λ�������С",
#define CFG_UART4_RECVBUF_LEN            32                    // "UART4���ջ��λ�������С",
#define CFG_UART5_SENDBUF_LEN            32                     // "UART5���ͻ��λ�������С",
#define CFG_UART5_RECVBUF_LEN            32                     // "UART5���ջ��λ�������С",
#define CFG_UART1_ENABLE                 false                   // "�Ƿ�����ʹ��UART1",
#define CFG_UART2_ENABLE                 true                  // "�Ƿ�����ʹ��UART2",
#define CFG_UART3_ENABLE                 false                  // "�Ƿ�����ʹ��UART3",
#define CFG_UART4_ENABLE                 false                  // "�Ƿ�����ʹ��UART4",
#define CFG_UART5_ENABLE                 false                  // "�Ƿ�����ʹ��UART5",
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096      // "��ʼ��ջ�ռ�",�����ʼ������ʹ�õ�ջ�ռ䣬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         50        // "�¼�������ֵ",�¼�����
#define CFG_EVENT_TYPE_LIMIT    50        // "�¼���������ֵ",�¼�������
#define CFG_MAINSTACK_LIMIT     4096      // "main����ջ�ߴ�",main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024      // "IDLE�¼�ջ�ߴ�",IDLE�¼������������е�ջ�ߴ磬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_OS_TINY             false     // "tiny���ں�",true=������Դ��ȱ�ĳ��ϣ��ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
//*******************************  Configure MemoryPool  ******************************************//
#define CFG_MEMPOOL_LIMIT       20        // "�ڴ������",��������10���ڴ��
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8        // "ʱ��",����ʱ���Ƕ�8��
//*******************************  Configure loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN       // EN_DIRECT_RUN=ֱ�Ӵ�flash�����У�EN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define  CFG_IBOOT_CRC   EN_NO_APP_CRC        // �Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define CFG_APP_FILENAME        ""                // CFG_APP_RUNMODE==EN_FORM_FILE�����������ļ���
#define CFG_IBOOT_VERSION       "V1.0.0"          // Iboot�����汾��
#define CFG_IBOOT_UPDATE_NAME   "iboot.bin"       // iboot�����ļ������˶����ã��Ժ�ĵ�
#define CFG_IBOOT_UPDATE_PATH   "\\"       // iboot����·�������˶����ã��Ժ�ĵ�
//*******************************  Configure stmpe811  ******************************************//
#define     CFG_CRT_CLK_FRE                  (100*1000)          // "IIC�����ٶ�"�������ٶȣ���λHz
#define     CFG_STMPE811_DEVADDR             0x41                // "�豸��ַ"��IIC�����ϵ��豸��ַ
#define     CFG_STMPE811_TOUCH_DEV_NAME      "TOUCH_STMPE811"    // "�����豸����",�����豸������
#define     CFG_STMPE811_DESKTOP_NAME        "LCD_DESKTOP_STMPE811"  // "������ʾ����",�������������������
//*******************************  Configure ili9325  ******************************************//
#define     CFG_ILI9325_DISPLAY_NAME         "lcdili9325"        // "��ʾ������",����Һ����ʾ������
#define     CFG_ILI9325_HEAP_NAME            "extram"            // "����ʹ�ö���",����Һ��������ʹ�õĶ�����
//*******************************  Configure cpu_peri_iic  ******************************************//
#define     CFG_I2C1_BUF_LEN                 128                 // "IIC1��������С",
#define     CFG_I2C2_BUF_LEN                 128                 // "IIC2��������С",
#define     CFG_I2C1_ENABLE                  true                // "�Ƿ�����ʹ��IIC1",
#define     CFG_I2C2_ENABLE                  true                // "�Ƿ�����ʹ��IIC2",
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (168*Mhz)         //��Ƶ���ں�Ҫ�ã����붨��

#endif