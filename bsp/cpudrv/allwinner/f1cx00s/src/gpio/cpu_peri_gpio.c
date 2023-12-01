#include "cpu_peri_io.h"
#include "cpu_peri_gpio.h"
#include "reg_ccu.h"

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���

//%$#@end initcode  ****��ʼ���������
//%$#@describe      ****���������ʼ
//component name:"cpu onchip gpio"//gpio����������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                  //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable               //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                 //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:none                 //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                 //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence                   //������������������������none����ʾ�������������
                                 //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                 //����������������������г�
//weakdependence:"none"          //��������������������������none����ʾ�������������
                                 //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                 //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                 //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO == false )
//#warning  " cpu_onchip_gpio  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure


/*
GPIO ��ʼ��
*/
void GPIO_Congif(
        GPIO_TypeDef * GPIOx, //GPIO
        unsigned int GPIO_Pin, //Pin
        GPIOMode_TypeDef GPIOMode,//ģʽ
        GPIOPuPd_TypeDef GPIO_PuPd//������
    )
{
    GPIOx->CFG[GPIO_Pin/8] &=~  (0xF<<(GPIO_Pin%8*4));//��0

    if(GPIOMode==GPIO_Mode_IN)
        GPIOx->CFG[GPIO_Pin/8] &= ~ (0x1<<(GPIO_Pin%8*4));//����ģʽ
    else if(GPIOMode==GPIO_Mode_OUT)
        GPIOx->CFG[GPIO_Pin/8] |=   (0x1<<(GPIO_Pin%8*4));//���ģʽ
    else
        GPIOx->CFG[GPIO_Pin/8] |=   (GPIOMode<<(GPIO_Pin%8*4));//����ģʽ

    //������
    GPIOx->PUL[GPIO_Pin/16] &= ~ (0x3<<(GPIO_Pin%16*2));//��0
    GPIOx->PUL[GPIO_Pin/16] |=  (GPIO_PuPd<<(GPIO_Pin%16*2));//д��
}

/*
GPIO���1
*/
void GPIO_SettoHigh(GPIO_TypeDef * GPIOx,unsigned int GPIO_Pin )
{
    GPIOx->DAT |=   (0x1<<GPIO_Pin);
}
/*
GPIO���0
*/
void GPIO_SettoLow(GPIO_TypeDef * GPIOx,unsigned int GPIO_Pin )
{
    GPIOx->DAT &= ~ (0x1<<GPIO_Pin);
}
/*
GPIO�� ����0-1
*/
u8 GPIO_READ(GPIO_TypeDef * GPIOx, unsigned int GPIO_Pin )
{
    if(GPIOx->DAT&((1)<<GPIO_Pin))return 1;
    else return 0;
}
/*
GPIO������������
Multi_Driving=0-3��0��ͣ�3���
*/
void GPIO_Multi_Driving
    (
        GPIO_TypeDef * GPIOx, //GPIO
        unsigned int GPIO_Pin, //Pin
        unsigned int Multi_driving //��������
    )
{
    if(Multi_driving>3)Multi_driving=3;
    GPIOx->DRV[GPIO_Pin/16]&= ~ (0x3<<(GPIO_Pin%16*2));//��0
    GPIOx->DRV[GPIO_Pin/16]|=  (Multi_driving<<(GPIO_Pin%16*2));//д��
}
/*
GPIO�ж�����
*/
#define PIO_Base_Address (0x01C20800)
void GPIO_External_Inerrupt_Config(int GPIOi,unsigned int GPIO_Pin,int mode)
{
    unsigned int n=GPIO_Pin/8;
    unsigned int y=GPIO_Pin%8;
    unsigned int addr=PIO_Base_Address+0x200+(GPIOi*0x20)+(n*0x4);

    // External Interrupt Configure Register
    write32(addr,read32(addr)&(~(0xf<<(y*4))));//��0
    write32(addr,read32(addr)|(mode<<(y*4)));//д��
//  sysprintf("GPIO_%08x...\r\n",addr);
//  sysprintf("GPIO_%08x...\r\n",read32(addr));
}
/*
GPIO�ж�ʹ��
*/
void GPIO_External_Inerrupt_Enable(int GPIOi,unsigned int GPIO_Pin)
{
    unsigned int addr=PIO_Base_Address+0x200+(GPIOi*0x20)+0x10;

    //External Interrupt Control Register
    S_BIT(addr,GPIO_Pin);
//  sysprintf("GPIO_%08x...\r\n",addr);
//  sysprintf("GPIO_%08x...\r\n",read32(addr));
}
/*
GPIO�ж�ʧ��
*/
void GPIO_External_Inerrupt_Disable(int GPIOi,unsigned int GPIO_Pin)
{
    unsigned int addr=PIO_Base_Address+0x200+(GPIOi*0x20)+0x10;

    //External Interrupt Control Register
    C_BIT(addr,GPIO_Pin);
}

//��ȡgpio���ж�״̬�����ж�
// Ϊ1��bit�����gpio�����ж�
unsigned int GPIO_External_Inerrupt_Status(int GPIOi)
{
    unsigned int addr=PIO_Base_Address+0x200+(GPIOi*0x20)+0x14;
    unsigned int val=read32(addr);
    write32(addr,val);//Write ��1�� to clear
    return val;
}


