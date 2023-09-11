#ifndef __cpu_peri_gpio_h_
#define __cpu_peri_gpio_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <at32f435_437_gpio.h>

#define PIO_LISTSIZE(pPins)    (sizeof(pPins) / sizeof( Pin))


//GPIO�˿�����
enum GPIO_PORT_NUM
{
    GPIO_A = 0,
    GPIO_B = 1,
    GPIO_C = 2,
    GPIO_D = 3,
    GPIO_E = 4,
    GPIO_F = 5,
    GPIO_G = 6,
    GPIO_H = 7,

    GPIO_NULL = 8,  //��Ч�˿ں�
};

//GPIO���Ŷ���
enum GPIO_PIN_NUM
{
    PIN0 = GPIO_PINS_0, 
    PIN1 = GPIO_PINS_1, 
    PIN2 = GPIO_PINS_2, 
    PIN3 = GPIO_PINS_3, 
    PIN4 = GPIO_PINS_4, 
    PIN5 = GPIO_PINS_5, 
    PIN6 = GPIO_PINS_6, 
    PIN7 = GPIO_PINS_7, 
    PIN8 = GPIO_PINS_8, 
    PIN9 = GPIO_PINS_9, 
    PIN10 = GPIO_PINS_10,
    PIN11 = GPIO_PINS_11,
    PIN12 = GPIO_PINS_12,
    PIN13 = GPIO_PINS_13,
    PIN14 = GPIO_PINS_14,
    PIN15 = GPIO_PINS_15,

    PIN_NULL = GPIO_PINS_15 + 1,    //��Ч���ź�
};

enum GPIO_MODE
{
    GPIO_IN_MODE      = GPIO_MODE_INPUT,        //��ͨ����ģʽ
    GPIO_OUT_MODE     = GPIO_MODE_OUTPUT,        //��ͨ���ģʽ
    GPIO_MUX_MODE     = GPIO_MODE_MUX ,        //MUX����ģʽ
    GPIO_ANALOG_MODE  = GPIO_MODE_ANALOG,        //ģ������/���ģʽ

    GPIO_NULL_MODE    = 0xff,         //��Ч�Ĺ���ģʽ
};

enum GPIO_PUPD
{
    GPIO_PUPD_NONE = GPIO_PULL_NONE,       //����������
    GPIO_PUPD_PU   = GPIO_PULL_UP,          //����
    GPIO_PUPD_PD   = GPIO_PULL_DOWN,       //����

    GPIO_PUPD_NULL = 0xff,                  //��Ч��������
};

enum GPIO_DRIVE
{
    GPIO_DRIVE_SS = GPIO_DRIVE_STRENGTH_STRONGER,       //����ǿ��
    GPIO_DRIVE_SM = GPIO_DRIVE_STRENGTH_MODERATE,       //�е�ǿ��

    GPIO_DRIVE_NULL = 0xff,                             //��Ч��ǿ��
};

enum GPIO_OTYPE
{
    GPIO_OTYPE_PP = GPIO_OUTPUT_PUSH_PULL,       //�������
    GPIO_OTYPE_OD = GPIO_OUTPUT_OPEN_DRAIN,      //��©���

    GPIO_OTYPE_NULL = 0xff,                      //��Ч���������
};

enum GPIO_MUX
{
    MUX_0   = GPIO_MUX_0,
    MUX_1   = GPIO_MUX_1,
    MUX_2   = GPIO_MUX_2,
    MUX_3   = GPIO_MUX_3,
    MUX_4   = GPIO_MUX_4,
    MUX_5   = GPIO_MUX_5,
    MUX_6   = GPIO_MUX_6,
    MUX_7   = GPIO_MUX_7,
    MUX_8   = GPIO_MUX_8,
    MUX_9   = GPIO_MUX_9,
    MUX_10  = GPIO_MUX_10,
    MUX_11  = GPIO_MUX_11,
    MUX_12  = GPIO_MUX_12,
    MUX_13  = GPIO_MUX_13,
    MUX_14  = GPIO_MUX_14,
    MUX_15  = GPIO_MUX_15,
    MUX_NUll= 0xff,        //����ģʽΪ����Ϊ��ͨI/O��ʹ�á�
};


//O_��ͷ���������Ҫ��ʼ����   I_  ��ͷ����������Ҫ��ʼ������
typedef struct PIN
{
   //GPIO_A....
   u32 PORT;
   //��PIN1....
   u32 Pinx;
   // GPIO �˿�ģʽ�Ĵ����� GPIO_MODE_IN
   u32 MODER;
   //���ģʽ��©���� �������
   u32 O_TYPER;
   //ǿ��
   u32 O_STRENGTH;
   //����������
   u32 PUPD;
   //MUX mode
   u32 MUX;
}Pin;


// =============================================================================
// @brief: GPIO�������ã��������ŵ�ģʽ���������͡��ٶȡ����������͵�
// @param: port����������port��ţ�����Ҫ����GPIOA,PIN5,��port=GPIO_A
// @param: Msk�����������룬���������GPIOA,PIN5����msk = (1<<5).
// @param: Mode,ģʽ����Ϊ���롢�����ģ�����롢AF���ù���
// @param: OutType,���������©���
// @param: Speed,�ٶȣ���GPIO_SPEED_50M
// @param: PUPD,����������
// @retval: ��
// =============================================================================
bool_t GPIO_CfgPinFunc(u32 port, u32 Msk, u32 Mode, u32 OutType, u32 Strength, u32 PUPD);

// =============================================================================
// @brief: �������ù��ܺ�
// @param: port����������port��ţ�����Ҫ����GPIOA,PIN5,��port=GPIO_A
// @param: pinnum,IO�ţ���GPIOA,PIN5,��pinnum = 5
// @param: af_no�����ù��ܺ�
// @retval: ��
// =============================================================================
u32 GPIO_AFSet(u32 port, u32 pinnum, u32 af_no);

// =============================================================================
// @brief: ��ĳgpio port�������
// @param: port����������port��ţ�����Ҫ����GPIOA,��port=GPIO_A
// @retval: ���õ�����
// =============================================================================
u32 GPIO_GetData(u32 port);

// =============================================================================
// @brief: ���������������ĳgpio port
// @param: port����������port��ţ�����Ҫ����GPIOA,��port=GPIO_A
// @param: data�������������
// @retval: ��
// =============================================================================
void GPIO_OutData(u32 port, u32 data);

// =============================================================================
// @brief: ��port��msk����1��bit��Ӧ��λ����ߵ�ƽ
// @param: port����������port��ţ�����Ҫ����GPIOA,��port=GPIO_A
// @param: mask��port����Ҫ����ߵ�ƽ��λ���룬�������GPIOA,PIN5��msk = (1<<5)
// @retval: ��
// =============================================================================
void GPIO_SettoHigh(u32 port, u32 msk);

// =============================================================================
// @brief: ��port��msk����1��bit��Ӧ��λ����͵�ƽ
// @param: port����������port��ţ�����Ҫ����GPIOA,��port=GPIO_A
// @param: mask��port����Ҫ����ߵ�ƽ��λ����,�������PIOA,PIN5��msk = (1<<5)
// @retval: ��
// =============================================================================
void GPIO_SettoLow(u32 port, u32 msk);

// =============================================================================
// @brief: ��GPIOA-GPIOI��ʱ�����
// @param: port,�˿ںţ���GPIO_A
// @retval: ��
// =============================================================================
void GPIO_PowerOn(u32 port);

// =============================================================================
// @brief: Ϊ�˽��ܣ����͹��ģ�ֱ�ӹر�GPIOA-GPIOI��ʱ�����
// @param: port,�˿ںţ���GPIO_A
// @retval: ��
// =============================================================================
void GPIO_PowerOff(u32 port);
// =============================================================================
// ����: ��ʼ��GPIO ����
// ������struct PIN
// ���أ�true/false
// =============================================================================
bool_t PIO_Configure(const Pin *Pin, u32 num);
void PIO_Set(const Pin *Pin);
void PIO_Clear(const Pin *Pin);
#ifdef __cplusplus
}
#endif

#endif /*__cpu_peri_gpio_h_*/
