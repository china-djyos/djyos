#include "stdio.h"
#include "stddef.h"
#include "cpu_peri.h"
#include "iicbus.h"
#include "endian.h"
#include "lock.h"
#include "int.h"
#include "djyos.h"
#include "board-config.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_I2C(u8 port);
//    #if CFG_I2C1_ENABLE==1
//    ModuleInstall_I2C(CN_I2C1);
//    #endif
//
//    #if CFG_I2C2_ENABLE==1
//    ModuleInstall_I2C(CN_I2C2);
//    #endif
//
//    #if CFG_I2C3_ENABLE==1
//    ModuleInstall_I2C(CN_I2C3);
//    #endif
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip iic"//CPU的iic总线驱动
//parent:"iicbus"    //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                     //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                  //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                    //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early                   //初始化时机，可选值：early，medium，later, pre-main。
                                    //表示初始化时间，分别是早期、中期、后期
//dependence:"iicbus","int","lock"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                    //选中该组件时，被依赖组件将强制选中，
                                    //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"             //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                    //选中该组件时，被依赖组件不会被强制选中，
                                    //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                    //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_IIC == false )
//#warning  " cpu_onchip_iic  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_IIC    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,16,512,
#define CFG_I2C_BUF_LEN         128         //"IIC缓冲区大小",
//%$#@enum,true,false,
#define CFG_I2C1_ENABLE         false       //"是否配置使用IIC1",
#define CFG_I2C2_ENABLE         false       //"是否配置使用IIC2",
#define CFG_I2C3_ENABLE         false       //"是否配置使用IIC3",
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure
// =============================================================================

#define tagI2CReg  I2C_TypeDef
static tagI2CReg volatile * const tg_I2CReg[] = {(tagI2CReg *)I2C1_BASE,
                                                (tagI2CReg *)I2C2_BASE,
                                                (tagI2CReg *)I2C3_BASE};


static struct IIC_CB *s_ptIIC1_CB;
static struct IIC_CB *s_ptIIC2_CB;
static struct IIC_CB *s_ptIIC3_CB;

//#define CFG_I2C_BUF_LEN  128

// =============================================================================
// ========================IO模拟IIC接口=========================================
// =============================================================================
//#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
//#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
//#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))
//
//#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414
//#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410
//
//#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //out
//#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //in

//IO方向设置
//#define SDA_IN()  {GPIOB->MODER&=~(3<<(6*2));GPIOB->MODER|=0<<6*2;}   //PB9输入模式
//#define () {GPIOB->MODER&=~(3<<(6*2));GPIOB->MODER|=1<<6*2;} //PB9输出模式
//IO操作函数
//#define IIC_SCL    PBout(7) //SCL
//#define IIC_SDA    PBout(6) //SDA
//#define READ_SDA   PBin(6)  //输入SDA
extern bool_t Board_IicGpioInit(u8 I2Cx);
extern void IIC_SetDaIn(u8 IICx);
extern void IIC_SetDaOut(u8 IICx);
extern void IIC_Scl(u8 IICx,u8 level);
extern void IIC_Sda(u8 IICx,u8 level);
extern u8 IIC_ReadSda(u8 IICx);

// =============================================================================
// 功能：IIC硬件初始化，主要是GPIO的时钟和配置
// 参数： 无
// 返回：无
// =============================================================================
static void IIC_Init(u8 I2Cx)
{
    Board_IicGpioInit(I2Cx);
    IIC_Scl(I2Cx,1);
    IIC_Sda(I2Cx,1);
}

// =============================================================================
// 功能：产生IIC起始信号
// 参数： 无
// 返回：无
// =============================================================================
static void IIC_Start(u8 I2Cx)
{
    IIC_SetDaOut(I2Cx);     //sda线输出
    IIC_Sda(I2Cx,1);
    IIC_Scl(I2Cx,1);
    DJY_DelayUs(4);
    IIC_Sda(I2Cx,0);//START:when CLK is high,DATA change form high to low
    DJY_DelayUs(4);
    IIC_Scl(I2Cx,0);//钳住I2C总线，准备发送或接收数据
}

// =============================================================================
// 功能：产生IIC停止信号
// 参数： 无
// 返回：无
// =============================================================================
static void IIC_Stop(u8 I2Cx)
{
    IIC_SetDaOut(I2Cx);//sda线输出
    IIC_Scl(I2Cx,0);
    IIC_Sda(I2Cx,0);//STOP:when CLK is high DATA change form low to high
    DJY_DelayUs(4);
    IIC_Scl(I2Cx,1);
    IIC_Sda(I2Cx,1);//发送I2C总线结束信号
    DJY_DelayUs(4);
}

// =============================================================================
// 功能：等待应答信号到来
// 参数： 无
// 返回：1，接收应答失败,0，接收应答成功
// =============================================================================
static u8 IIC_Wait_Ack(u8 I2Cx)
{
    u8 ucErrTime=0;
    IIC_SetDaIn(I2Cx);      //SDA设置为输入
    IIC_Sda(I2Cx,1);
    DJY_DelayUs(1);
    IIC_Scl(I2Cx,1);
    DJY_DelayUs(1);
    while(IIC_ReadSda(I2Cx))
    {
        ucErrTime++;
        DJY_DelayUs(1);
        if(ucErrTime>250)
        {
            IIC_Stop(I2Cx);
            return 1;
        }
    }
    IIC_Scl(I2Cx,0);//时钟输出0
    return 0;
}

// =============================================================================
// 功能：产生应答信号
// 参数： 无
// 返回：无
// =============================================================================
static void IIC_Ack(u8 I2Cx)
{
    IIC_Scl(I2Cx,0);
    IIC_SetDaOut(I2Cx);
    IIC_Sda(I2Cx,0);
    DJY_DelayUs(2);
    IIC_Scl(I2Cx,1);
    DJY_DelayUs(2);
    IIC_Scl(I2Cx,0);
}

// =============================================================================
// 功能：不产生ACK应答
// 参数： 无
// 返回：1，接收应答失败,0，接收应答成功
// =============================================================================
static void IIC_NAck(u8 I2Cx)
{
    IIC_Scl(I2Cx,0);
    IIC_SetDaOut(I2Cx);
    IIC_Sda(I2Cx,1);
    DJY_DelayUs(2);
    IIC_Scl(I2Cx,1);
    DJY_DelayUs(2);
    IIC_Scl(I2Cx,0);
}

// =============================================================================
// 功能：IIC发送一个字节,返回从机有无应答
// 参数： 无
// 返回：1，有应答,0，无应答
// =============================================================================
static void IIC_Send_Byte(u8 I2Cx,u8 txd)
{
    u8 t;
    IIC_SetDaOut(I2Cx);
    IIC_Scl(I2Cx,0);//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {
        IIC_Sda(I2Cx,(txd&0x80)>>7);
        txd<<=1;
        DJY_DelayUs(2);   //对TEA5767这三个延时都是必须的
        IIC_Scl(I2Cx,1);
        DJY_DelayUs(2);
        IIC_Scl(I2Cx,0);
        DJY_DelayUs(2);
    }
}

// =============================================================================
// 功能：读1个字节，ack=1时，发送ACK，ack=0，发送nACK
// 参数： 无
// 返回：1，有应答,0，无应答
// =============================================================================
static u8 IIC_Read_Byte(u8 I2Cx,unsigned char ack)
{
    unsigned char i,receive=0;
    IIC_SetDaIn(I2Cx);//SDA设置为输入
    for(i=0;i<8;i++ )
    {
        IIC_Scl(I2Cx,0);
        DJY_DelayUs(2);
        IIC_Scl(I2Cx,1);
        receive<<=1;
        if(IIC_ReadSda(I2Cx))
            receive++;
        DJY_DelayUs(1);
    }
    if (!ack)
        IIC_NAck(I2Cx);//发送nACK
    else
        IIC_Ack(I2Cx); //发送ACK
    return receive;
}

// =============================================================================
// 功能: 硬件GPIO初始化，包括电源等
// 参数: IIC_NO,编号
// 返回: 无
// =============================================================================
void __IIC_GpioConfig(u8 IIC_NO)
{
    switch(IIC_NO)
    {
    case CN_I2C1:
        IIC_Init(IIC_NO);
        break;
    case CN_I2C2:
        break;
    case CN_I2C3:
        break;
    default:
        break;
    }
}

// =============================================================================
// 功能：轮询方式读IIC从设备的数据
// 参数：reg,寄存器基址
//       devaddr,设备地址
//       addr,访问的寄存器地址
//       buf,存储数据
//       len,数据长度，字节
// 返回：len,读取成功;-1,读取失败
// =============================================================================
static u32 __IIC_ReadPoll(volatile tagI2CReg *reg,u8 devaddr,u32 memaddr,
                        u8 maddrlen, u8 *buf, u32 len)
{
    u32 i;
    u8 iic;

    for(i = 0; i < CN_I2C_NUM; i++)
    {
        if(reg == tg_I2CReg[i])
            break;
    }
    if(i == CN_I2C_NUM)
        return -1;

    iic = i;

    u8 mem_addr_buf[4];
    //将地址作大小端变换
    fill_little_32bit(mem_addr_buf,0,memaddr);

    u8 temp=0;
    temp = (devaddr<<1)| CN_IIC_WRITE_FLAG;
    IIC_Start(iic);

    IIC_Send_Byte(iic,temp);       //发送写命令
    IIC_Wait_Ack(iic);
    for(i = 0; i < maddrlen; i++)
    {
        IIC_Send_Byte(iic,mem_addr_buf[i]);   //发送低地址
        IIC_Wait_Ack(iic);
    }

    temp = (devaddr<<1)| CN_IIC_READ_FLAG;
    IIC_Start(iic);
    IIC_Send_Byte(iic,temp);           //进入接收模式
    IIC_Wait_Ack(iic);

    for(i = 0; i < len - 1; i ++)
    {
        buf[i]=IIC_Read_Byte(iic,1);
    }
    buf[i] = IIC_Read_Byte(iic,0);
    IIC_Stop(iic);//产生一个停止条件
    DJY_DelayUs(1000);
    return len;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// =============================================================================
// 功能：轮询方式向IIC从设备的写数据
// 参数：reg,寄存器基址
//       devaddr,设备地址
//       memaddr,设备内部地址
//       maddrlen,设备内部地址长度
//       buf,存储数据
//       len,数据长度，字节
// 返回：len,读取成功;-1,读取失败
// =============================================================================
static u32 __IIC_WritePoll(volatile tagI2CReg *reg,u8 devaddr,u32 memaddr,
                        u8 maddrlen, u8 *buf, u32 len)
{
    u8 mem_addr_buf[4];
    u8 iic;
    u32 i;

    for(i = 0; i < CN_I2C_NUM; i++)
    {
        if(reg == tg_I2CReg[i])
            break;
    }
    if(i == CN_I2C_NUM)
        return -1;

    iic = i;
    //将地址作大小端变换
    fill_little_32bit(mem_addr_buf,0,memaddr);

    IIC_Start(iic);

    IIC_Send_Byte(iic,0XA0);        //发送写命令
    IIC_Wait_Ack(iic);

    for(i = 0; i < maddrlen; i++)
    {
        IIC_Send_Byte(iic,mem_addr_buf[i]);   //发送低地址
        IIC_Wait_Ack(iic);
    }

    for(i = 0; i < len; i++)
    {
        IIC_Send_Byte(iic,buf[i]);     //发送字节
        IIC_Wait_Ack(iic);
    }
    IIC_Stop(iic);//产生一个停止条件
    DJY_DelayUs(1000);
    return len;
}
#pragma GCC diagnostic pop

// =============================================================================
// 功能: 禁止iic中断,接收与发送共用一个中断源。
// 参数: reg,被操作的寄存器组指针
// 返回: 无
// 注：F4的寄存器定义与F1/F3/F7/H7/L4均不一样。
// =============================================================================
static void __IIC_IntDisable(tagI2CReg *reg)
{
    reg->CR2 &= ~(I2C_IT_EVT);
}

// =============================================================================
// 功能：轮询方式读写IIC设备
// 参数：reg,寄存器基址
//       DevAddr,设备地址
//       MemAddr,设备内部地址
//       MemAddrLen,设备内部地址长度
//       Buf,存储数据
//       Length,数据长度，字节
//       WrRdFlag,读写标记，为0时写，1时为读
// 返回：len,读取成功;-1,读取失败
// =============================================================================
static bool_t __IIC_WriteReadPoll(ptu32_t reg,u8 DevAddr,u32 MemAddr,\
                                u8 MemAddrLen,u8* Buf, u32 Length,u8 WrRdFlag)
{
    __IIC_IntDisable((tagI2CReg *)reg);
    if(WrRdFlag == CN_IIC_WRITE_FLAG)   //写
    {
        if(Length == __IIC_WritePoll((tagI2CReg *)reg,DevAddr,MemAddr,MemAddrLen,Buf,Length))
            return true;
        else
            return false;
    }
    else                                //读
    {
        if(Length == __IIC_ReadPoll((tagI2CReg *)reg,DevAddr,MemAddr,MemAddrLen,Buf,Length))
            return true;
        else
            return false;
    }
}

// =============================================================================
// 功能：IIC底层驱动的初始化，完成整个IIC总线的初始化，其主要工作如下：
//       1.初始化总线控制块IIC_CB，回调函数和缓冲区的初始化赋值；
//       2.默认的硬件初始化，如GPIO或IIC寄存器等；
//       3.中断初始化，完成读写中断配置；
//       4.调用IICBusAdd或IICBusAdd_r增加总线结点；
// 参数：para,无具体意义
// 返回：无
// =============================================================================
bool_t ModuleInstall_I2C(u8 port)
{
    struct IIC_CB **ICB;
    struct IIC_Param IIC_Config;

    switch(port)
    {
    case CN_I2C1:
        IIC_Config.BusName = "IIC1";
        ICB                = &s_ptIIC1_CB;
        IIC_Config.IICBuf  = NULL;
        break;
    case CN_I2C2:
        IIC_Config.BusName = "IIC2";
        IIC_Config.IICBuf  = NULL;
        ICB                = &s_ptIIC2_CB;
        break;
    case CN_I2C3:
        IIC_Config.BusName = "IIC3";
        IIC_Config.IICBuf  = NULL;
        ICB                = &s_ptIIC3_CB;
        break;
    default: return false;
    }

    IIC_Config.IICBufLen           = CFG_I2C_BUF_LEN;
    IIC_Config.SpecificFlag        = (ptu32_t)tg_I2CReg[port];
    IIC_Config.pWriteReadPoll      = (WriteReadPoll)__IIC_WriteReadPoll;
    //stm32的IIC存在问题，故采用IO模拟方式，无须实现如下函数
    IIC_Config.pGenerateWriteStart = NULL;
    IIC_Config.pGenerateReadStart  = NULL;
    IIC_Config.pGenerateEnd        = NULL;
    IIC_Config.pBusCtrl            = NULL;

    __IIC_GpioConfig(port);
//    __IIC_HardDefaultSet(IIC_Config.SpecificFlag);
//    __IIC_IntConfig(intline,__IIC_ISR);

    *ICB = IIC_BusAdd(&IIC_Config);
    if(*ICB)
        return 0;
    else
        return 1;
}

