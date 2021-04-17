#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
#include <dbug.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���

//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip ct1711"       //adc���
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"cpu onchip gpio"  //������������������������none����ʾ�������������
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_CT1711 == false )
//#warning  "   ADC�������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_CT1711    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,flase,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure

//IO��������
#define CT1711_OUT() gpio_config(GPIO36, GMODE_OUTPUT);
#define CT1711_IN()  gpio_config(GPIO36, GMODE_INPUT)

//IO��������
#define CT1711_DATA(n) gpio_output(GPIO36,n)
#define CT1711_READ    gpio_input(GPIO36)


void CT1711_Init(void)
{
    CT1711_OUT();

    CT1711_DATA(0);
    DJY_DelayUs(400);
    CT1711_DATA(1);
}

u8 CT1711_Read_Bit(void)
{
    u8 data;
    atom_low_t atom_low;

    atom_low = Int_LowAtomStart();
//    CT1711_DATA(1);
    CT1711_OUT();
//    CT1711_DATA(0);
    DJY_DelayUs(1);
    CT1711_IN();
//    CT1711_DATA(1);
    DJY_DelayUs(20);

//    CT1711_IN();
    if(CT1711_READ)
    {
        data = 0x01;
    }else{
        data = 0x00;
    }
//    CT1711_IN();

//    CT1711_OUT();
//    CT1711_DATA(1);
    DJY_DelayUs(30);
    Int_LowAtomEnd(atom_low);
    return data;
}


u8 CT1711_Read_Byte(void)
{
    u8 byte = 0;
    int i;
    for(i=8;i > 0;i--)
    {
        byte <<=1;
        byte |= CT1711_Read_Bit();
    }

    return byte;
}

float CT1711_Read_Temp_Degree(void)
{
    float temp = 0.00;
    unsigned char  cc0,cc1,sign;
    char temp_byte0,temp_byte1;
    int temp_val;

    CT1711_Init();

//    CT1711_IN();
    DJY_DelayUs(150*1000);
//    DJY_EventDelay(130*1000);

    cc0 = CT1711_Read_Bit();
    DJY_DelayUs(10);
    cc1 = CT1711_Read_Bit();
//    printf("cc0 = %d,cc1 = %d\r\n",cc0,cc1);
    DJY_DelayUs(10);
    cc0 = cc0 & 0x01;
    cc1 = cc1 & 0x01;
    if((cc0 == 0x00) && (cc1 == 0x00))
    {
        sign = CT1711_Read_Bit();
//        printf("sign = %d\r\n",sign);
        DJY_DelayUs(10);
        temp_byte0 = CT1711_Read_Byte();
        DJY_DelayUs(10);
        temp_byte1 = CT1711_Read_Byte();
        DJY_DelayUs(10);
        temp_val = (temp_byte0 << 8) + temp_byte1;
        if(sign == 0x01)
        {
            temp_val =~ temp_val;
            temp_val &= 0xffff;
            temp_val ++;
            temp = (-3.90625*temp_val/1000);
        }else{
            temp =  ((3.90625*(float)temp_val)/1000);
        }
    }

    return temp;
}
