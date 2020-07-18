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
#define    CFG_RUNMODE                CN_RUNMODE_APP                //��IBOOT���ص�APP
//*******************************  Configure black box  ******************************************//
#define    CFG_MODULE_ENABLE_BLACK_BOX     true
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
//*******************************  Configure cpu onchip gpio  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO  true
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
//*******************************  Configure kernel  ******************************************//
#define    CFG_INIT_STACK_SIZE             4096              //�����ʼ������ʹ�õ�ջ�ռ�һ�㰴Ĭ��ֵ�Ϳ�����
#define    CFG_EVENT_LIMIT                 15                //�¼�����
#define    CFG_EVENT_TYPE_LIMIT            15                //�¼�������
#define    CFG_MAINSTACK_LIMIT             4096              //main�������������ջ�ߴ�
#define    CFG_IDLESTACK_LIMIT             1024              //IDLE�¼����������е�ջ�ߴ�һ�㰴Ĭ��ֵ�Ϳ�����
#define    CFG_IDLE_MONITOR_CYCLE          30                //����IDLE�¼���������1/16 CPUռ�ȵ�ʱ��������0=������
#define    CFG_OS_TINY                     false             //true=������Դ��ȱ�ĳ����ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
#define    CFG_MODULE_ENABLE_KERNEL        true
//*******************************  Configure board config  ******************************************//
#define    CFG_MODULE_ENABLE_BOARD_CONFIG  true
//*******************************  Configure misc  ******************************************//
#define    CFG_MODULE_ENABLE_MISC          true
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8      //����ʱ���Ƕ�8��
#define    CFG_MODULE_ENABLE_TIME          true
//*******************************  Configure graphical kernel  ******************************************//
#define CFG_GKERNEL_CMD_DEEP        1024     // "�����������",�ϲ�Ӧ�ã�����gdd����gkernel��������Ļ��������ȣ��ֽ�����
#define CFG_USER_REQUEST_DEEP       128     //gkernel���ϲ���������Ļ��������ȣ��ֽ�����
#define    CFG_MODULE_ENABLE_GRAPHICAL_KERNEL  true
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true   // "ȫ���ܶ�̬����",��ʹѡfalse��Ҳ����ʹ��malloc-free�����ڴ棬��ʹ���в����� �������û��ֲᡷ�ڴ�����½�
#define    CFG_MODULE_ENABLE_HEAP          true
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
