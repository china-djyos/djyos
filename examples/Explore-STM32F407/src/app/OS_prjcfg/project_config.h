/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure NlsCharset  ******************************************//
#define CFG_LOCAL_CHARSET       "gb2312"        //
//*******************************  Configure DevFile  ******************************************//
#define CFG_DEVFILE_LIMIT       10      //�����豸����
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true  //��ʹѡfalseҲ����ʹ��malloc-free�����ڴ浫ʹ���в����� �������û��ֲᡷ�ڴ�����½�
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40      //�����������������ź����ͻ�����
//*******************************  Configure font  ******************************************//
#define CFG_FONT_DEFAULT  CN_FONT_GB2312_SONG_16      //��������include/fontĿ¼����
//*******************************  Configure gkernel  ******************************************//
#define CFG_GKERNEL_CMD_DEEP        1024    //gdd��gkernel��������Ļ��������ȣ��ֽ�����
//*******************************  Configure HmiInput  ******************************************//
#define CFG_HMIIN_DEV_LIMIT     2       //�˻����������豸��������̡�����
//*******************************  Configure stdio  ******************************************//
#define     CFG_STDIO_RUN_MODE               (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW)//
#define     CFG_STDIO_IN_NAME                "/dev/UART2"      //
#define     CFG_STDIO_OUT_NAME               "/dev/UART2"      //
#define     CFG_STDIO_ERR_NAME               "/dev/UART2"      //
//*******************************  Configure timer  ******************************************//
#define CFG_TIMER_SOUCE     1       //1=��Ӳ����ʱ���ṩʱ��Դ0=��tick�ṩʱ��Դ
#define CFG_TIMERS_LIMIT    5       //�ɴ����Ķ�ʱ��������������ͼ�ν���Ķ�ʱ����
//*******************************  Configure wdt  ******************************************//
#define CFG_WDT_LIMIT           10      //������������
#define CFG_WDTMSG_LIMIT        3       //�������Ź�����Ϣ���е���󳤶�
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096    //�����ʼ������ʹ�õ�ջ�ռ�һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         10      //�¼�����
#define CFG_EVENT_TYPE_LIMIT    10      //�¼�������
#define CFG_MAINSTACK_LIMIT     4096    //main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024    //IDLE�¼����������е�ջ�ߴ�һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_OS_TINY             false   //true=������Դ��ȱ�ĳ����ں˻�ü������¼����������¼�����ʱ��ͳ�ơ�
//*******************************  Configure MemoryPool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10      //������10���ڴ��
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8      //����ʱ���Ƕ�8��
//*******************************  Configure loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN     //EN_DIRECT_RUN=ֱ�Ӵ�flash�����У�EN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define  CFG_IBOOT_CRC   EN_NO_APP_CRC      //�Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define CFG_APP_FILENAME        ""              //CFG_APP_RUNMODE==EN_FORM_FILE�����������ļ���
#define CFG_IBOOT_VERSION       "V1.0.0"        //Iboot�����汾��
#define CFG_IBOOT_UPDATE_NAME   "iboot.bin"     //iboot�����ļ������˶����ã��Ժ�ĵ�
#define CFG_IBOOT_UPDATE_PATH   "iboot.bin"     //iboot����·�������˶����ã��Ժ�ĵ�
//*******************************  Configure at24c02  ******************************************//
#define CFG_AT24_TIMEOUT           (-1)   //-1��ʾ����
#define CFG_AT24_ADDRESS            0xA0                //Ӳ������AT24��IIC�豸��ַ
#define CFG_AT24_CLK_FRE           (100*1000)           //��λHz
#define CFG_AT24C02_BUS_NAME       "I2C0"        //AT24ʹ�õ�����
//*******************************  Configure cpu_peri_iic  ******************************************//
#define CFG_I2C_BUF_LEN         128         //
#define CFG_I2C1_ENABLE         false       //
#define CFG_I2C2_ENABLE         false       //
#define CFG_I2C3_ENABLE         false       //
//*******************************  Configure cpu_peri_spi  ******************************************//
#define CFG_SPI_BUF_LEN             128                //
#define CFG_SPI_DMA_BUF_LEN         128                //
#define CFG_SPI1_ENABLE             false              //
#define CFG_SPI2_ENABLE             false              //
#define CFG_SPI3_ENABLE             false              //
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
#define     CFG_UART4_DMABUF_LEN             32                //
#define     CFG_UART5_SENDBUF_LEN            32                //
#define     CFG_UART5_RECVBUF_LEN            32                //
#define     CFG_UART5_DMABUF_LEN             32                //
#define     CFG_UART6_SENDBUF_LEN            32                //
#define     CFG_UART6_RECVBUF_LEN            32                //
#define     CFG_UART6_DMABUF_LEN             32                //
#define     CFG_UART1_ENABLE                 true              //
#define     CFG_UART2_ENABLE                 true              //
#define     CFG_UART3_ENABLE                 false             //
#define     CFG_UART4_ENABLE                 false             //
#define     CFG_UART5_ENABLE                 false             //
#define     CFG_UART6_ENABLE                 false             //
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (168*Mhz)         //��Ƶ���ں�Ҫ�ã����붨��

#endif