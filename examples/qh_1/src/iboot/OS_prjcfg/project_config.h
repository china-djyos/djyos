/****************************************************
 *  Automatically-generated file. Do not edit!  *
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40      //�����������������ź����ͻ�����
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true  //��ʹѡfalseҲ����ʹ��malloc-free�����ڴ浫ʹ���в����� �������û��ֲᡷ�ڴ�����½�
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8   //������Զ�����
#define CFG_HANDLE_LIMIT        8   //������Զ�����
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10      //
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096    //�����ʼ������ʹ�õ�ջ�ռ�һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         15      //�¼�����
#define CFG_EVENT_TYPE_LIMIT    15      //�¼�������
#define CFG_MAINSTACK_LIMIT     4096    //main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024    //IDLE�¼����������е�ջ�ߴ�һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_OS_TINY             false   //true=������Դ��ȱ�ĳ����ں˻�ü������¼����������¼�����ʱ��ͳ�ơ�
//*******************************  Configure cpu onchip iic  ******************************************//
#define    CFG_I2C1_BUF_LEN                128               //
#define    CFG_I2C2_BUF_LEN                128               //
#define    CFG_I2C1_ENABLE                 false             //
#define    CFG_I2C2_ENABLE                 false             //
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10      //�����豸����
//*******************************  Configure cpu onchip uart  ******************************************//
#define    CFG_UART1_SENDBUF_LEN           32                //
#define    CFG_UART1_RECVBUF_LEN           32                //
#define    CFG_UART1_DMABUF_LEN            32                //
#define    CFG_UART2_SENDBUF_LEN           32                //
#define    CFG_UART2_RECVBUF_LEN           32                //
#define    CFG_UART2_DMABUF_LEN            32                //
#define    CFG_UART3_SENDBUF_LEN           32                //
#define    CFG_UART3_RECVBUF_LEN           32                //
#define    CFG_UART3_DMABUF_LEN            32                //
#define    CFG_UART4_SENDBUF_LEN           32                //
#define    CFG_UART4_RECVBUF_LEN           32                //
#define    CFG_UART5_SENDBUF_LEN           32                //
#define    CFG_UART5_RECVBUF_LEN           32                //
#define    CFG_UART1_ENABLE                true              //
#define    CFG_UART2_ENABLE                false             //
#define    CFG_UART3_ENABLE                false             //
#define    CFG_UART4_ENABLE                false             //
#define    CFG_UART5_ENABLE                false             //
//*******************************  Configure graphical kernel  ******************************************//
#define CFG_GKERNEL_CMD_DEEP        1024    //gdd��gkernel��������Ļ��������ȣ��ֽ�����
//*******************************  Configure graphical decorate development(gdd)  ******************************************//
#define CFG_DESKTOP_WIDTH       0           //����ߴ磨�����������0=��ʾ�����
#define CFG_DESKTOP_HEIGHT      0           //����ߴ磨���������߶�0=��ʾ���߶�
#define CFG_DISPLAY_NAME        "DISPLAY_NAME"         //����bsp����ʾ������ģ�����õ���ʾ��������ͬ
#define CFG_DESKTOP_NAME        "DESKTOP_NAME"         //
#define CFG_INPUTDEV_NAME       "INPUTDEV_NAME"         //ʹ��bsp�������豸�����õ����ֶ������豸�Ļ�ÿ���豸���ö��Ÿ���
#define CFG_DESKTOP_FORMAT      CN_SYS_PF_RGB565    //���洰�����ظ�ʽ������gkernel.h�ж���һ��ʹ������ʾ����ͬ��ɫ
#define CFG_GRAY_BASE_COLOR     CN_COLOR_WHITE      //���ظ�ʽ��Ϊ�Ҷ�ʱ����Ҫ���õġ�������ɫ����gkernel.h���ҵ�������ɫ����
#define CFG_FILL_COLOR          CN_COLOR_BLUE       //��������ʱ�����ɫ��888��ʽ����gkernel.h���ҵ�������ɫ����
//*******************************  Configure human machine interface  ******************************************//
#define CFG_HMIIN_DEV_LIMIT     2       //�˻����������豸��������̡�����
//*******************************  Configure stmpe811  ******************************************//
#define    CFG_CRT_CLK_FRE                 (100*1000)        //�����ٶȵ�λHz
#define    CFG_STMPE811_DEVADDR            0x41              //IIC�����ϵ��豸��ַ
#define    CFG_STMPE811_TOUCH_DEV_NAME     "TOUCH_STMPE811"  //�����豸������
#define    CFG_STMPE811_DESKTOP_NAME       "LCD_DESKTOP_STMPE811"//�������������������
//*******************************  Configure LCD driver ili9325  ******************************************//
#define    CFG_LCD_XSIZE                   240               //
#define    CFG_LCD_YSIZE                   128               //
#define    CFG_ILI9325_DISPLAY_NAME        "lcdili9325"      //����Һ����ʾ������
#define    CFG_ILI9325_HEAP_NAME           "extram"          //����Һ��������ʹ�õĶ�����
//*******************************  Configure font  ******************************************//
#define    CFG_FONT_DEFAULT                "gb2312_song_16"  //��������include/fontĿ¼����
//*******************************  Configure ascii dot font  ******************************************//
#define    CFG_ASCII_8X8                   false             //8*8����ascii����
#define    CFG_ASCII_6X12                  false             //6*12����ascii����
#define    CFG_ASCII_8X16_SONG             true              //8*16����ascii����_����
#define    CFG_ASCII_8X16_YUAN             false             //8*16����ascii����_Բ��
#define    CFG_ASCII_8X16_KAI              false             //8*16����ascii����_����
#define    CFG_ASCII_8X16_HEI              false             //8*16����ascii����_����
#define    CFG_ASCII_8X16_FANG             false             //8*16����ascii����_����
#define    CFG_ASCII_12X24_SONG            false             //12*24����ascii����_����
#define    CFG_ASCII_12X24_YUAN            false             //12*24����ascii����_Բ��
#define    CFG_ASCII_12X24_KAI             false             //12*24����ascii����_����
#define    CFG_ASCII_12X24_HEI             false             //12*24����ascii����_����
#define    CFG_ASCII_12X24_FANG            false             //12*24����ascii����_����
#define    CFG_ASCII_16X32_YUAN            false             //16*32����ascii����_Բ��
#define    CFG_ASCII_16X32_KAI             false             //16*32����ascii����_����
#define    CFG_ASCII_16X32_HEI             false             //16*32����ascii����_����
#define    CFG_ASCII_16X32_FANG            false             //16*32����ascii����_����
//*******************************  Configure loader  ******************************************//
#define    CFG_APP_RUNMODE                 CN_DIRECT_RUN     //CN_DIRECT_RUN=ֱ�Ӵ�flash�����У�CN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define    CFG_APP_VERIFICATION            VERIFICATION_NULL //�Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define    CFG_IBOOT_VERSION               01                //Iboot�����汾��
#define    CFG_IBOOT_UPDATE_NAME           "/yaf2/iboot.bin" //
//*******************************  Configure Nls Charset  ******************************************//
#define    CFG_LOCAL_CHARSET               "gb2312"          //
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_STDIN_MULTI      true         //
#define CFG_STDIO_STDOUT_FOLLOW    true         //
#define CFG_STDIO_STDERR_FOLLOW    true         //
#define CFG_STDIO_FLOAT_PRINT      true         //
#define CFG_STDIO_STDIOFILE        true         //
#define CFG_STDIO_IN_NAME              "/dev/UART1"    //
#define CFG_STDIO_OUT_NAME             "/dev/UART1"    //
#define CFG_STDIO_ERR_NAME             "/dev/UART1"    //
//*******************************  Configure shell  ******************************************//
#define    CFG_SHELL_STACK                 0x1000            //
#define    CFG_ADD_ROUTINE_SHELL           true              //
#define    CFG_ADD_EXPAND_SHELL            true              //
#define    CFG_ADD_GLOBAL_FUN              false             //
#define    CFG_SHOW_ADD_SHEELL             true              //
#define  CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure time  ******************************************//
#define    CFG_LOCAL_TIMEZONE              8                 //����ʱ���Ƕ�8��
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (168.0*Mhz)       //��Ƶ���ں�Ҫ�ã����붨��

#endif
