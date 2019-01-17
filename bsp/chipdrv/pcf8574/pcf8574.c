#include "cpu_peri.h"
#include <djyos.h>
#include <Legacy/stm32_hal_legacy.h>
#include <stddef.h>
#include <stdint.h>
#include <stm32f7xx_hal_conf.h>
#include <systime.h>
#include "iicbus.h"
#include "board.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//  extern bool_t ModuleInstall_PCF8574(char *BusName);
//  ModuleInstall_PCF8574(CFG_PCF8574_BUS_NAME);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"PCF8574"      //iic�ӿڵ�8λIO��չ
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"ioiicbus"         //������������������������none����ʾ�������������
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


#define PCF8574_ADDR    0X40    //PCF8574��ַ(������һλ)

static struct IIC_Device *ps_PCF8574_Dev = NULL;

__attribute__((weak))  void PCF8574_Pin_Init(void)
{
    return;
}

//��PCF8574д��8λIOֵ
//DataToWrite:Ҫд�������
void PCF8574_WriteOneByte(u8 DataToWrite)
{
//    IIC_Start();
//    IIC_Send_Byte(PCF8574_ADDR|0X00);   //����������ַ0X40,д����
//    IIC_Wait_Ack();
//    IIC_Send_Byte(DataToWrite);         //�����ֽ�
//    IIC_Wait_Ack();
//    IIC_Stop();                         //����һ��ֹͣ����
//    Djy_DelayUs(10*mS);

    IIC_Write(ps_PCF8574_Dev,PCF8574_ADDR|0X00,&DataToWrite,1,true,CN_TIMEOUT_FOREVER);
}

//��ȡPCF8574��8λIOֵ
//����ֵ:����������
u8 PCF8574_ReadOneByte(void)
{
    u8 temp=0;
//    IIC_Start();
//    IIC_Send_Byte(PCF8574_ADDR|0X01);   //�������ģʽ
//    IIC_Wait_Ack();
//    temp=IIC_Read_Byte(0);
//    IIC_Stop();                         //����һ��ֹͣ����

    IIC_Read(ps_PCF8574_Dev,PCF8574_ADDR|0X01,&temp,1,CN_TIMEOUT_FOREVER);

    return temp;
}

//����PCF8574ĳ��IO�ĸߵ͵�ƽ
//bit:Ҫ���õ�IO���,0~7
//sta:IO��״̬;0��1
void PCF8574_WriteBit(u8 bit,u8 sta)
{
    u8 data;
    data=PCF8574_ReadOneByte(); //�ȶ���ԭ��������
    if(sta==0)
        data&=~(1<<bit);
    else
        data|=1<<bit;
    PCF8574_WriteOneByte(data); //д���µ�����
}

//��ȡPCF8574��ĳ��IO��ֵ
//bit��Ҫ��ȡ��IO���,0~7
//����ֵ:��IO��ֵ,0��1
u8 PCF8574_ReadBit(u8 bit)
{
    u8 data;
    data=PCF8574_ReadOneByte(); //�ȶ�ȡ���8λIO��ֵ
    if(data&(1<<bit))
        return 1;
    else
        return 0;
}


bool_t ModuleInstall_PCF8574(char *BusName)
{
    static struct IIC_Device* s_PCF8574_Dev;

    //���FT5X26��IIC����
    s_PCF8574_Dev = IIC_DevAdd(BusName,"IIC_Dev_PCF8574",PCF8574_ADDR>>1,0,8);
    if(NULL != s_PCF8574_Dev)
    {
        IIC_BusCtrl(s_PCF8574_Dev,CN_IIC_SET_CLK,100*1000,0);
        ps_PCF8574_Dev = s_PCF8574_Dev;

        PCF8574_Pin_Init();

        return true;
    }

    return false;
}

bool_t ETH_RESE(void)
{
    PCF8574_Pin_Init();
    PCF8574_WriteBit(ETH_RESET_IO,1);
    Djy_DelayUs(100*mS);
    PCF8574_WriteBit(ETH_RESET_IO,0);
    Djy_DelayUs(100*mS);
    return true;
}








