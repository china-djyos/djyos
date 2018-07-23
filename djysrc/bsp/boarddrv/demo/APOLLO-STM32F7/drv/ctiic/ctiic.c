#include <ctiic.h>
#include <djyos.h>
#include "cpu_peri.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_myname.h****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern void CT_IIC_Init(void);
//    CT_IIC_Init();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"ctiic"        //��д�����������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp���             //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
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
//%$#@string,,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
//%$#@end configue  ****�������ý���
//@#$%component end configure

//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//���ݴ�����-IIC ��������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/12/28
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved
//********************************************************************************
//��
//////////////////////////////////////////////////////////////////////////////////

//����I2C�ٶȵ���ʱ
void CT_Delay(void)
{
    Djy_DelayUs(2);
}
//���ݴ���оƬIIC�ӿڳ�ʼ��
void CT_IIC_Init(void)
{
    GPIO_PowerOn(GPIO_H);
    GPIO_PowerOn(GPIO_I);
    GPIO_CfgPinFunc(GPIO_H,PIN6 ,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU);
    GPIO_CfgPinFunc(GPIO_I,PIN3 ,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU);

    GPIO_CfgPinFunc(GPIO_H,PIN7 ,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU);
    GPIO_CfgPinFunc(GPIO_I,PIN8 ,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU);

}
//����IIC��ʼ�ź�
void CT_IIC_Start(void)
{
    CT_SDA_OUT();     //sda�����
    CT_IIC_SDA(1);
    CT_IIC_SCL(1);
    Djy_DelayUs(30);
    CT_IIC_SDA(0);//START:when CLK is high,DATA change form high to low
    CT_Delay();
    CT_IIC_SCL(0);//ǯסI2C���ߣ�׼�����ͻ��������
}
//����IICֹͣ�ź�
void CT_IIC_Stop(void)
{
    CT_SDA_OUT();//sda�����
    CT_IIC_SCL(1);
    Djy_DelayUs(30);
    CT_IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
    CT_Delay();
    CT_IIC_SDA(1);//����I2C���߽����ź�
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 CT_IIC_Wait_Ack(void)
{
    u8 ucErrTime=0;
    CT_SDA_IN();      //SDA����Ϊ����
    CT_IIC_SDA(1);
    CT_IIC_SCL(1);
    CT_Delay();
    while(CT_READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            CT_IIC_Stop();
            return 1;
        }
        CT_Delay();
    }
    CT_IIC_SCL(0);//ʱ�����0
    return 0;
}
//����ACKӦ��
void CT_IIC_Ack(void)
{
    CT_IIC_SCL(0);
    CT_SDA_OUT();
    CT_Delay();
    CT_IIC_SDA(0);
    CT_Delay();
    CT_IIC_SCL(1);
    CT_Delay();
    CT_IIC_SCL(0);
}
//������ACKӦ��
void CT_IIC_NAck(void)
{
    CT_IIC_SCL(0);
    CT_SDA_OUT();
    CT_Delay();
    CT_IIC_SDA(1);
    CT_Delay();
    CT_IIC_SCL(1);
    CT_Delay();
    CT_IIC_SCL(0);
}
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void CT_IIC_Send_Byte(u8 txd)
{
    u8 t;
    CT_SDA_OUT();
    CT_IIC_SCL(0);//����ʱ�ӿ�ʼ���ݴ���
    CT_Delay();
    for(t=0;t<8;t++)
    {
        CT_IIC_SDA((txd&0x80)>>7);
        txd<<=1;
        CT_IIC_SCL(1);
        CT_Delay();
        CT_IIC_SCL(0);
        CT_Delay();
    }
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
u8 CT_IIC_Read_Byte(unsigned char ack)
{
    u8 i,receive=0;
    CT_SDA_IN();//SDA����Ϊ����
    Djy_DelayUs(30);
    for(i=0;i<8;i++ )
    {
        CT_IIC_SCL(0);
        CT_Delay();
        CT_IIC_SCL(1);
        receive<<=1;
        if(CT_READ_SDA)receive++;
    }
    if (!ack)CT_IIC_NAck();//����nACK
    else CT_IIC_Ack(); //����ACK
    return receive;
}




























