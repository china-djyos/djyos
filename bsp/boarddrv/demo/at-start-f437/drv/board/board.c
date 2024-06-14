#include <stdint.h>
#include <stdbool.h>

#include <stddef.h>
#include <os.h>
#include <cpu_peri.h>
#include <uartctrl.h>
#include <cpu_peri_gpio.h>
#include "project_config.h"

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���

//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"board config"//�����
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:required               //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"kernel","stm32f7","cpu onchip gpio"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_BOARD_CONFIG == false )
//#warning  " board_config  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_BOARD_CONFIG    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure



#include "at32f435_437.h"

/* 
 * https://www.arterytek.com/download/RM/RM_AT32F435_437_CH_V2.03.pdf
 */

//DEBUG��
static const Pin uart1_pin[] = {
        {GPIO_A,PIN10,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_7},   //RX
        {GPIO_A,PIN9,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_7},    //TX
};

static const Pin uart2_pin[] = {
        {GPIO_D,PIN5,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_7},    //TX
        {GPIO_A,PIN3,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_7},    //RX
};

static const Pin uart3_pin[] = {
        {GPIO_B,PIN10,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_7},   //TX
        {GPIO_B,PIN11,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_7},   //RX
};

static const Pin uart4_pin[] = {
        {GPIO_C,PIN10,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_8},   //TX
        {GPIO_C,PIN11,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_8},   //RX
};

static const Pin uart5_pin[] = {
        {GPIO_B,PIN8,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_8},    //RX
        {GPIO_B,PIN9,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_8},    //TX
        // {GPIO_C,PIN12,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_8},   //TX
        // {GPIO_D,PIN2,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_PU,MUX_8},   //RX
};

static const Pin uart6_pin[] = {
        {GPIO_A,PIN4,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_8},    //TX
        {GPIO_A,PIN5,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_8},    //RX
        // {GPIO_C,PIN6,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_8},   //TX
        // {GPIO_C,PIN7,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_8},   //RX
};

static const Pin uart7_pin[] = {
        {GPIO_E,PIN7,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_8},    //RX
        {GPIO_E,PIN8,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_8},    //TX
};

static const Pin uart8_pin[] = {
        {GPIO_C,PIN2,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_8},    //TX 
        {GPIO_C,PIN3,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_8},    //RX
};

static const Pin EthRmii_pins[] = {
        {GPIO_A,PIN2,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_11},  //mdio
        {GPIO_C,PIN1,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_11},  //mdc
        {GPIO_G,PIN11,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_11},  //tx_d0
        {GPIO_G,PIN13,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_11},  //tx_d1
        {GPIO_G,PIN14,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_11},  //tx_en
        {GPIO_D,PIN8,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_11},  //rx_dv
        {GPIO_D,PIN9,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_11},  //rx_d0
        {GPIO_D,PIN10,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_11},  //rx_d1
        {GPIO_A,PIN1,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_11},  //clk
        {GPIO_A,PIN8,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_0},   //Ӧ���Ǹ�phyоƬ�ṩʱ���õ�
};

static const Pin ResetPHY_pins[] = {
        {GPIO_E,PIN15,GPIO_MODE_OUTPUT,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_NUll},
        {GPIO_G,PIN15,GPIO_MODE_OUTPUT,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_NUll},
};

// ============================================================================
// ���ܣ����ô��ڱ��ΪSerialNo�Ĵ���Ϊ��˫�����͹���
// ������SerialNo,���ں�
// ���أ���
// ============================================================================
void Board_UartHalfDuplexSend(u8 SerialNo)
{

}

// ============================================================================
// ���ܣ����ô��ڱ��ΪSerialNo�Ĵ���Ϊ��˫�����չ���
// ������SerialNo,���ں�
// ���أ���
// ============================================================================
void Board_UartHalfDuplexRecv(u8 SerialNo)
{

}

// ============================================================================
// ���ܣ���λPHYоƬ
// ��������
// ���أ���
// ============================================================================
void Board_ResetPHY(void)
{
   /* exit power down mode */
   PIO_Clear(&ResetPHY_pins[1]);

   /*reset phy */
   PIO_Clear(&ResetPHY_pins[0]);
   DJY_DelayUs(20000);
   PIO_Set(&ResetPHY_pins[0]);
   DJY_DelayUs(20000);
}



void Board_Init(void)
{
    PIO_Configure(uart1_pin, PIO_LISTSIZE(uart1_pin));
    PIO_Configure(uart2_pin, PIO_LISTSIZE(uart2_pin));
    PIO_Configure(uart3_pin, PIO_LISTSIZE(uart3_pin));
    PIO_Configure(uart4_pin, PIO_LISTSIZE(uart4_pin));
    PIO_Configure(uart5_pin, PIO_LISTSIZE(uart5_pin));
    PIO_Configure(uart6_pin, PIO_LISTSIZE(uart6_pin));
    PIO_Configure(uart7_pin, PIO_LISTSIZE(uart7_pin));
    PIO_Configure(uart8_pin, PIO_LISTSIZE(uart8_pin));
    PIO_Configure(EthRmii_pins, PIO_LISTSIZE(EthRmii_pins));
    PIO_Configure(ResetPHY_pins, PIO_LISTSIZE(ResetPHY_pins));

}
