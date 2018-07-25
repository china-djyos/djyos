#include "cpu_peri.h"
#include <djyos.h>
#include <Legacy/stm32_hal_legacy.h>
#include <stddef.h>
#include <stdint.h>
#include <stm32f7xx_hal_conf.h>
#include <systime.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//  u8 PCF8574_Init(void);
//  PCF8574_Init();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"PCF8574"      //iic�ӿڵ�8λIO��չ
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp���             //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                   //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//grade:init                    //��ʼ��ʱ������ѡֵ��none��init��main��none��ʾ�����ʼ����
                                //init��ʾ�ڵ���main֮ǰ��main��ʾ��main�����г�ʼ��
//dependence:"cpu_peri_gpio"    //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������������������������none����ʾ�������������
                                //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
//%$#@end configue  ****�������ý���
//@#$%component end configure

#include "../../../../djysrc/bsp/arch/arm/arm32_stdint.h"

#define SDA_IN()  {GPIOH->MODER&=~(3<<(5*2));GPIOH->MODER|=0<<5*2;} //PH5����ģʽ
#define SDA_OUT() {GPIOH->MODER&=~(3<<(5*2));GPIOH->MODER|=1<<5*2;} //PH5���ģʽ
//IO����
#define IIC_SCL(n)  (n?HAL_GPIO_WritePin(GPIOH,GPIO_PIN_4,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOH,GPIO_PIN_4,GPIO_PIN_RESET)) //SCL
#define IIC_SDA(n)  (n?HAL_GPIO_WritePin(GPIOH,GPIO_PIN_5,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOH,GPIO_PIN_5,GPIO_PIN_RESET)) //SDA
#define READ_SDA    HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_5)  //����SDA



void IIC_pcf8574_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    GPIO_PowerOn(GPIO_H);   //ʹ��GPIOHʱ��
    //PH4,5��ʼ������
    GPIO_Initure.Pin=GPIO_PIN_4|GPIO_PIN_5;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //����
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);

    IIC_SDA(1);
    IIC_SCL(1);
}

//����IIC��ʼ�ź�
void IIC_Start(void)
{
    SDA_OUT();     //sda�����
    IIC_SDA(1);
    IIC_SCL(1);
    Djy_DelayUs(4);
    IIC_SDA(0);//START:when CLK is high,DATA change form high to low
    Djy_DelayUs(4);
    IIC_SCL(0);//ǯסI2C���ߣ�׼�����ͻ��������
}
//����IICֹͣ�ź�
void IIC_Stop(void)
{
    SDA_OUT();//sda�����
    IIC_SCL(0);
    IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
    Djy_DelayUs(4);
    IIC_SCL(1);
    Djy_DelayUs(4);
    IIC_SDA(1);//����I2C���߽����ź�
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
    u8 ucErrTime=0;
    SDA_IN();      //SDA����Ϊ����
    IIC_SDA(1);Djy_DelayUs(1);
    IIC_SCL(1);Djy_DelayUs(1);
    while(READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_SCL(0);//ʱ�����0
    return 0;
}
//����ACKӦ��
void IIC_Ack(void)
{
    IIC_SCL(0);
    SDA_OUT();
    IIC_SDA(0);
    Djy_DelayUs(2);
    IIC_SCL(1);
    Djy_DelayUs(2);
    IIC_SCL(0);
}
//������ACKӦ��
void IIC_NAck(void)
{
    IIC_SCL(0);
    SDA_OUT();
    IIC_SDA(1);
    Djy_DelayUs(2);
    IIC_SCL(1);
    Djy_DelayUs(2);
    IIC_SCL(0);
}
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void IIC_Send_Byte(u8 txd)
{
    u8 t;
    SDA_OUT();
    IIC_SCL(0);//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {
        IIC_SDA((txd&0x80)>>7);
        txd<<=1;
        Djy_DelayUs(2);   //��TEA5767��������ʱ���Ǳ����
        IIC_SCL(1);
        Djy_DelayUs(2);
        IIC_SCL(0);
        Djy_DelayUs(2);
    }
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
u8 IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
    {
        IIC_SCL(0);
        Djy_DelayUs(2);
        IIC_SCL(1);
        receive<<=1;
        if(READ_SDA)receive++;
        Djy_DelayUs(1);
    }
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK
    return receive;
}

#define PCF8574_INT  HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)//PCF8574 INT��

#define PCF8574_ADDR    0X40    //PCF8574��ַ(������һλ)

//PCF8574����IO�Ĺ���
#define BEEP_IO         0       //��������������   P0
#define AP_INT_IO       1       //AP3216C�ж�����   P1
#define DCMI_PWDN_IO    2       //DCMI�ĵ�Դ��������   P2
#define USB_PWR_IO      3       //USB��Դ�������� P3
#define EX_IO           4       //��չIO,�Զ���ʹ��    P4
#define MPU_INT_IO      5       //MPU9250�ж�����   P5
#define RS485_RE_IO     6       //RS485_RE����        P6
#define ETH_RESET_IO    7       //��̫����λ����       P7


//��PCF8574д��8λIOֵ
//DataToWrite:Ҫд�������
void PCF8574_WriteOneByte(u8 DataToWrite)
{
    IIC_Start();
    IIC_Send_Byte(PCF8574_ADDR|0X00);   //����������ַ0X40,д����
    IIC_Wait_Ack();
    IIC_Send_Byte(DataToWrite);         //�����ֽ�
    IIC_Wait_Ack();
    IIC_Stop();                         //����һ��ֹͣ����
    Djy_DelayUs(10*mS);
}

u8 PCF8574_Init(void)
{
    u8 temp=0;
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();           //ʹ��GPIOBʱ��

    GPIO_Initure.Pin=GPIO_PIN_12;           //PB12
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);     //��ʼ��
    IIC_pcf8574_Init();                             //IIC��ʼ��
    //���PCF8574�Ƿ���λ
    IIC_Start();
    IIC_Send_Byte(PCF8574_ADDR);            //д��ַ
    temp=IIC_Wait_Ack();                    //�ȴ�Ӧ��,ͨ���ж��Ƿ���ACKӦ��,���ж�PCF8574��״̬
    IIC_Stop();                             //����һ��ֹͣ����
    PCF8574_WriteOneByte(0XFF);             //Ĭ�����������IO����ߵ�ƽ
    return temp;
}

//��ȡPCF8574��8λIOֵ
//����ֵ:����������
u8 PCF8574_ReadOneByte(void)
{
    u8 temp=0;
    IIC_Start();
    IIC_Send_Byte(PCF8574_ADDR|0X01);   //�������ģʽ
    IIC_Wait_Ack();
    temp=IIC_Read_Byte(0);
    IIC_Stop();                         //����һ��ֹͣ����
    return temp;
}

//����PCF8574ĳ��IO�ĸߵ͵�ƽ
//bit:Ҫ���õ�IO���,0~7
//sta:IO��״̬;0��1
void PCF8574_WriteBit(u8 bit,u8 sta)
{
    u8 data;
    data=PCF8574_ReadOneByte(); //�ȶ���ԭ��������
    if(sta==0)data&=~(1<<bit);
    else data|=1<<bit;
    PCF8574_WriteOneByte(data); //д���µ�����
}

//��ȡPCF8574��ĳ��IO��ֵ
//bit��Ҫ��ȡ��IO���,0~7
//����ֵ:��IO��ֵ,0��1
u8 PCF8574_ReadBit(u8 bit)
{
    u8 data;
    data=PCF8574_ReadOneByte(); //�ȶ�ȡ���8λIO��ֵ
    if(data&(1<<bit))return 1;
    else return 0;
}


bool_t ETH_RESE(void)
{
    PCF8574_Init();
    PCF8574_WriteBit(ETH_RESET_IO,1);
    Djy_DelayUs(100*mS);
    PCF8574_WriteBit(ETH_RESET_IO,0);
    Djy_DelayUs(100*mS);
    return true;
}








