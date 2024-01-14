#include "sys_dma.h"
#include "sys_clock.h"
#include "cpu_peri_io.h"
//#include <stdlib.h>
//-----------------------
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//  extern void DMA_Init(void);
//  DMA_Init();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu_dma"      //LCD显示驱动
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:required               //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"none"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#if ( CFG_MODULE_ENABLE_DMA == false )
//#warning  " keyboard_hard_driver  组件参数未配置，使用默认配置"
#define CFG_MODULE_ENABLE_DMA    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@enum,true,false,
//%$#@num,,,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#@select,1
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

#define delay_ms(x) DJY_DelayUs(x * 1000)
#define u64_t u64
#define u32_t u32
#define u16_t u16
#define u8_t  u8

/*
    Normal DMA(4路) 最大一次传输128KB;
    Dedicated DMA(4路) 最大一次传输0x1000000;
*/

/*DMA-寄存器地址*/
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
/*DMA初始化*/
void DMA_Init(void)
{
    Open_Dev_Clock(DMA_CLOCK);
}
/*DMA退出*/
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
        S_BIT(DMA_Base_Address+0x4,((2*dma->Ch+0)<<(dma->Type==NDMA?0:16)));//清中断
        return 1;
    }
    return 0;
}
/*Normal DMA n Full Transfer Interrupt Pending (n=0~3)*/
int DMA_Get_Full_TIP(DMA* dma)
{
    if(read32(DMA_Base_Address+0x4)&((0x1<<(2*dma->Ch+1))<<(dma->Type==NDMA?0:16)))
    {
        S_BIT(DMA_Base_Address+0x4,((2*dma->Ch+1)<<(dma->Type==NDMA?0:16)));//清中断
        return 1;
    }
    return 0;
}
/*中断使能 (n=0~3)*/
int DMA_Interrupt_Control_Half(DMA* dma,int c)
{
    if(dma->Type)//初始化过
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
/*中断使能 (n=0~3)*/
int DMA_Interrupt_Control_Full(DMA* dma,int c)
{
    if(dma->Type)//初始化过
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
/*DMA读取剩余byte计数值
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
    //返回地址
    dma->addr=(DMA_Base_Address+dma->Type+dma->Ch*0x20);
    //
    if(dma->Continuous_Mode_Enable==1)S_BIT(DMA_Base_Address+0x8,16);//连续模式下要设置为1
    else C_BIT(DMA_Base_Address+0x8,16);//设置为0
    write32(dma->addr+0xC,dma->Byte_Counter);//设置BCNT

    write_update_bits(dma->addr+0x0,29,0x1,dma->Continuous_Mode_Enable);//设置DMA连续使能
    write_update_bits(dma->addr+0x0,15,0x1,dma->Read_Byte_Counter_Enable);//读计数值使能
  //设置源
    write32(dma->addr+0x4,(u32_t)dma->Source_Address);//设置源地址
    write_update_bits(dma->addr+0x0,0,0x1F,dma->Source_DRQ_Type);//设置源类型
    write_update_bits(dma->addr+0x0,8,0x3,dma->Source_Data_Width);//设置源宽度为16位
    write_update_bits(dma->addr+0x0,5,0x3,dma->Source_Address_Type);//设置源地址模式
    write_update_bits(dma->addr+0x0,7,0x1,dma->Source_Burst_Length);//源 Burst Length
  //设置目标
    write32(dma->addr+0x8,(u32_t)dma->Destination_Address);//设置目标地址
    write_update_bits(dma->addr+0x0,16,0x1F,dma->Destination_DRQ_Type);//设置目标类型
    write_update_bits(dma->addr+0x0,24,0x3,dma->Destination_Data_Width);//设置目标宽度为16位
    write_update_bits(dma->addr+0x0,21,0x3,dma->Destination_Address_Type);//设置目标地址模式
    write_update_bits(dma->addr+0x0,23,0x1,dma->Destination_Burst_Length);//目标 Burst Length

    return 0;
}

