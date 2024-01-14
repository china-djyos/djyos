#include "sys_dma.h"
#include "sys_clock.h"
#include "cpu_peri_io.h"
//#include <stdlib.h>
//-----------------------
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//  extern void DMA_Init(void);
//  DMA_Init();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu_dma"      //LCD��ʾ����
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:required               //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"             //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#if ( CFG_MODULE_ENABLE_DMA == false )
//#warning  " keyboard_hard_driver  �������δ���ã�ʹ��Ĭ������"
#define CFG_MODULE_ENABLE_DMA    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@enum,true,false,
//%$#@num,,,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#@select,1
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

#define delay_ms(x) DJY_DelayUs(x * 1000)
#define u64_t u64
#define u32_t u32
#define u16_t u16
#define u8_t  u8

/*
    Normal DMA(4·) ���һ�δ���128KB;
    Dedicated DMA(4·) ���һ�δ���0x1000000;
*/

/*DMA-�Ĵ�����ַ*/
#define DMA_Base_Address (0x01C02000)
#define write_update_bits(addr,bit,Clear,Value) write32(addr,read32(addr) & (~((u64_t)(Clear)<<bit)) | ((u64_t)(Value)<<bit)    )

#define CCU_Base_Address                (u32_t)0x01C20000
#define CCU_BUS_CLK_GATING_REG0         (u32_t)CCU_Base_Address+0x0060
#define CCU_BUS_CLK_GATING_REG1         (u32_t)CCU_Base_Address+0x0064
#define CCU_BUS_CLK_GATING_REG2         (u32_t)CCU_Base_Address+0x0068
#define CCU_BUS_SOFT_RST_REG0           (u32_t)CCU_Base_Address+0x02C0
#define CCU_BUS_SOFT_RST_REG1           (u32_t)CCU_Base_Address+0x02C4
#define CCU_BUS_SOFT_RST_REG2           (u32_t)CCU_Base_Address+0x02D0
/////////////////////////////////////////////
/*DMA��ʼ��*/
void DMA_Init(void)
{
    Open_Dev_Clock(DMA_CLOCK);
}
/*DMA�˳�*/
void DMA_Exit(void)
{
    Close_Dev_Clock(DMA_CLOCK);
}
/*dma enable*/
void DMA_Enable(DMA* dma)
{
    S_BIT(dma->addr+0x0,31);
}
/*dma disable*/
void DMA_Disable(DMA* dma)
{
    C_BIT(dma->addr+0x0,31);
}
/*Normal DMA dma_ch Half Transfer Interrupt Pending (n=0~3)*/
int DMA_Get_Half_TIP(DMA* dma)
{
    if(read32(DMA_Base_Address+0x4)&((0x1<<(2*dma->Ch+0))<<(dma->Type==NDMA?0:16)))
    {
        S_BIT(DMA_Base_Address+0x4,((2*dma->Ch+0)<<(dma->Type==NDMA?0:16)));//���ж�
        return 1;
    }
    return 0;
}
/*Normal DMA n Full Transfer Interrupt Pending (n=0~3)*/
int DMA_Get_Full_TIP(DMA* dma)
{
    if(read32(DMA_Base_Address+0x4)&((0x1<<(2*dma->Ch+1))<<(dma->Type==NDMA?0:16)))
    {
        S_BIT(DMA_Base_Address+0x4,((2*dma->Ch+1)<<(dma->Type==NDMA?0:16)));//���ж�
        return 1;
    }
    return 0;
}
/*�ж�ʹ�� (n=0~3)*/
int DMA_Interrupt_Control_Half(DMA* dma,int c)
{
    if(dma->Type)//��ʼ����
    {
        if(c)S_BIT(DMA_Base_Address+0x0,((2*dma->Ch+0)+((dma->Type==NDMA)?0:16)));
        else C_BIT(DMA_Base_Address+0x0,((2*dma->Ch+0)+((dma->Type==NDMA)?0:16)));
    }else
    {
        sysprintf("Init err...\r\n");
        return -1;
    }
    return 0;
}
/*�ж�ʹ�� (n=0~3)*/
int DMA_Interrupt_Control_Full(DMA* dma,int c)
{
    if(dma->Type)//��ʼ����
    {
        if(c)S_BIT(DMA_Base_Address+0x0,((2*dma->Ch+1)+((dma->Type==NDMA)?0:16)));
        else C_BIT(DMA_Base_Address+0x0,((2*dma->Ch+1)+((dma->Type==NDMA)?0:16)));
    }else
    {
        sysprintf("Init err...\r\n");
        return -1;
    }
    return 0;
}
/*DMA��ȡʣ��byte����ֵ
*/
unsigned int dma_read_bcnt(DMA* dma)
{
     if(dma->Read_Byte_Counter_Enable==1)
     {
            return (read32(dma->addr+0xC),0x1ffffff);
     }else
     {
            sysprintf("ERR:Read_Byte_Counter_Enable==0\r\n");
            return 0;
     }
}
/*
NDMA_Config
*/
int DMA_Config(DMA* dma)
{
    if(dma->Byte_Counter>(dma->Type==NDMA ? NDMA_MAX_LEN : DDMA_MAX_LEN))
    {
        sysprintf("ERR:Byte_Counter ERR \r\n");
        return -1;
    }
    //���ص�ַ
    dma->addr=(DMA_Base_Address+dma->Type+dma->Ch*0x20);
    //
    if(dma->Continuous_Mode_Enable==1)S_BIT(DMA_Base_Address+0x8,16);//����ģʽ��Ҫ����Ϊ1
    else C_BIT(DMA_Base_Address+0x8,16);//����Ϊ0
    write32(dma->addr+0xC,dma->Byte_Counter);//����BCNT

    write_update_bits(dma->addr+0x0,29,0x1,dma->Continuous_Mode_Enable);//����DMA����ʹ��
    write_update_bits(dma->addr+0x0,15,0x1,dma->Read_Byte_Counter_Enable);//������ֵʹ��
  //����Դ
    write32(dma->addr+0x4,(u32_t)dma->Source_Address);//����Դ��ַ
    write_update_bits(dma->addr+0x0,0,0x1F,dma->Source_DRQ_Type);//����Դ����
    write_update_bits(dma->addr+0x0,8,0x3,dma->Source_Data_Width);//����Դ���Ϊ16λ
    write_update_bits(dma->addr+0x0,5,0x3,dma->Source_Address_Type);//����Դ��ַģʽ
    write_update_bits(dma->addr+0x0,7,0x1,dma->Source_Burst_Length);//Դ Burst Length
  //����Ŀ��
    write32(dma->addr+0x8,(u32_t)dma->Destination_Address);//����Ŀ���ַ
    write_update_bits(dma->addr+0x0,16,0x1F,dma->Destination_DRQ_Type);//����Ŀ������
    write_update_bits(dma->addr+0x0,24,0x3,dma->Destination_Data_Width);//����Ŀ����Ϊ16λ
    write_update_bits(dma->addr+0x0,21,0x3,dma->Destination_Address_Type);//����Ŀ���ַģʽ
    write_update_bits(dma->addr+0x0,23,0x1,dma->Destination_Burst_Length);//Ŀ�� Burst Length

    return 0;
}

