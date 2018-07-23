#include <ctiic.h>
#include <djyos.h>
#include "cpu_peri.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_myname.h****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern void CT_IIC_Init(void);
//    CT_IIC_Init();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"ctiic"        //填写该组件的名字
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp组件             //选填“第三方组件、核心组件、bsp组件、用户组件”，本属性用于在IDE中分组
//select:可选                //选填“必选、可选、不可选”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//grade:init                    //初始化时机，可选值：none，init，main。none表示无须初始化，
                                //init表示在调用main之前，main表示在main函数中初始化
//dependence:"cpu_peri_gpio"    //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //如果依赖多个组件，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,,1,10,
//%$#select,        ***定义无值的宏，仅用于第三方组件
//%$#@free,
//%$#@end configue  ****参数配置结束
//@#$%component end configure

//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//电容触摸屏-IIC 驱动代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/12/28
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved
//********************************************************************************
//无
//////////////////////////////////////////////////////////////////////////////////

//控制I2C速度的延时
void CT_Delay(void)
{
    Djy_DelayUs(2);
}
//电容触摸芯片IIC接口初始化
void CT_IIC_Init(void)
{
    GPIO_PowerOn(GPIO_H);
    GPIO_PowerOn(GPIO_I);
    GPIO_CfgPinFunc(GPIO_H,PIN6 ,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU);
    GPIO_CfgPinFunc(GPIO_I,PIN3 ,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU);

    GPIO_CfgPinFunc(GPIO_H,PIN7 ,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU);
    GPIO_CfgPinFunc(GPIO_I,PIN8 ,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU);

}
//产生IIC起始信号
void CT_IIC_Start(void)
{
    CT_SDA_OUT();     //sda线输出
    CT_IIC_SDA(1);
    CT_IIC_SCL(1);
    Djy_DelayUs(30);
    CT_IIC_SDA(0);//START:when CLK is high,DATA change form high to low
    CT_Delay();
    CT_IIC_SCL(0);//钳住I2C总线，准备发送或接收数据
}
//产生IIC停止信号
void CT_IIC_Stop(void)
{
    CT_SDA_OUT();//sda线输出
    CT_IIC_SCL(1);
    Djy_DelayUs(30);
    CT_IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
    CT_Delay();
    CT_IIC_SDA(1);//发送I2C总线结束信号
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 CT_IIC_Wait_Ack(void)
{
    u8 ucErrTime=0;
    CT_SDA_IN();      //SDA设置为输入
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
    CT_IIC_SCL(0);//时钟输出0
    return 0;
}
//产生ACK应答
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
//不产生ACK应答
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
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void CT_IIC_Send_Byte(u8 txd)
{
    u8 t;
    CT_SDA_OUT();
    CT_IIC_SCL(0);//拉低时钟开始数据传输
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
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
u8 CT_IIC_Read_Byte(unsigned char ack)
{
    u8 i,receive=0;
    CT_SDA_IN();//SDA设置为输入
    Djy_DelayUs(30);
    for(i=0;i<8;i++ )
    {
        CT_IIC_SCL(0);
        CT_Delay();
        CT_IIC_SCL(1);
        receive<<=1;
        if(CT_READ_SDA)receive++;
    }
    if (!ack)CT_IIC_NAck();//发送nACK
    else CT_IIC_Ack(); //发送ACK
    return receive;
}




























