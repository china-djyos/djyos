#include "cpu_peri.h"
#include <djyos.h>
#include <Legacy/stm32_hal_legacy.h>
#include <stddef.h>
#include <stdint.h>
#include <stm32f7xx_hal_conf.h>
#include <systime.h>

#include "../../../../djysrc/bsp/arch/arm/arm32_stdint.h"

#define SDA_IN()  {GPIOH->MODER&=~(3<<(5*2));GPIOH->MODER|=0<<5*2;} //PH5输入模式
#define SDA_OUT() {GPIOH->MODER&=~(3<<(5*2));GPIOH->MODER|=1<<5*2;} //PH5输出模式
//IO操作
#define IIC_SCL(n)  (n?HAL_GPIO_WritePin(GPIOH,GPIO_PIN_4,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOH,GPIO_PIN_4,GPIO_PIN_RESET)) //SCL
#define IIC_SDA(n)  (n?HAL_GPIO_WritePin(GPIOH,GPIO_PIN_5,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOH,GPIO_PIN_5,GPIO_PIN_RESET)) //SDA
#define READ_SDA    HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_5)  //输入SDA



void IIC_pcf8574_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    GPIO_PowerOn(GPIO_H);   //使能GPIOH时钟
    //PH4,5初始化设置
    GPIO_Initure.Pin=GPIO_PIN_4|GPIO_PIN_5;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //快速
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);
    
    IIC_SDA(1);
    IIC_SCL(1);  
}

//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA(1);	  	  
	IIC_SCL(1);
	Djy_DelayUs(4);
 	IIC_SDA(0);//START:when CLK is high,DATA change form high to low 
	Djy_DelayUs(4);
	IIC_SCL(0);//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL(0);
	IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
 	Djy_DelayUs(4);
	IIC_SCL(1); 
	Djy_DelayUs(4);
	IIC_SDA(1);//发送I2C总线结束信号				   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
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
	IIC_SCL(0);//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
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
//不产生ACK应答		    
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
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL(0);//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA((txd&0x80)>>7);
        txd<<=1; 	  
		Djy_DelayUs(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL(1);
		Djy_DelayUs(2);
		IIC_SCL(0);	
		Djy_DelayUs(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
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
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}

#define PCF8574_INT  HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)//PCF8574 INT脚

#define PCF8574_ADDR    0X40    //PCF8574地址(左移了一位)

//PCF8574各个IO的功能
#define BEEP_IO         0       //蜂鸣器控制引脚   P0
#define AP_INT_IO       1       //AP3216C中断引脚   P1
#define DCMI_PWDN_IO    2       //DCMI的电源控制引脚   P2
#define USB_PWR_IO      3       //USB电源控制引脚 P3
#define EX_IO           4       //扩展IO,自定义使用    P4
#define MPU_INT_IO      5       //MPU9250中断引脚   P5
#define RS485_RE_IO     6       //RS485_RE引脚        P6
#define ETH_RESET_IO    7       //以太网复位引脚       P7


//向PCF8574写入8位IO值
//DataToWrite:要写入的数据
void PCF8574_WriteOneByte(u8 DataToWrite)
{
    IIC_Start();
    IIC_Send_Byte(PCF8574_ADDR|0X00);   //发送器件地址0X40,写数据
    IIC_Wait_Ack();
    IIC_Send_Byte(DataToWrite);         //发送字节
    IIC_Wait_Ack();
    IIC_Stop();                         //产生一个停止条件
    Djy_DelayUs(10*mS);
}

u8 PCF8574_Init(void)
{
    u8 temp=0;
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();           //使能GPIOB时钟

    GPIO_Initure.Pin=GPIO_PIN_12;           //PB12
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);     //初始化
    IIC_pcf8574_Init();                             //IIC初始化
    //检查PCF8574是否在位
    IIC_Start();
    IIC_Send_Byte(PCF8574_ADDR);            //写地址
    temp=IIC_Wait_Ack();                    //等待应答,通过判断是否有ACK应答,来判断PCF8574的状态
    IIC_Stop();                             //产生一个停止条件
    PCF8574_WriteOneByte(0XFF);             //默认情况下所有IO输出高电平
    return temp;
}

//读取PCF8574的8位IO值
//返回值:读到的数据
u8 PCF8574_ReadOneByte(void)
{
    u8 temp=0;
    IIC_Start();
    IIC_Send_Byte(PCF8574_ADDR|0X01);   //进入接收模式
    IIC_Wait_Ack();
    temp=IIC_Read_Byte(0);
    IIC_Stop();                         //产生一个停止条件
    return temp;
}

//设置PCF8574某个IO的高低电平
//bit:要设置的IO编号,0~7
//sta:IO的状态;0或1
void PCF8574_WriteBit(u8 bit,u8 sta)
{
    u8 data;
    data=PCF8574_ReadOneByte(); //先读出原来的设置
    if(sta==0)data&=~(1<<bit);
    else data|=1<<bit;
    PCF8574_WriteOneByte(data); //写入新的数据
}

//读取PCF8574的某个IO的值
//bit：要读取的IO编号,0~7
//返回值:此IO的值,0或1
u8 PCF8574_ReadBit(u8 bit)
{
    u8 data;
    data=PCF8574_ReadOneByte(); //先读取这个8位IO的值
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








