//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//------------------------------------------------------------------------------
// =============================================================================
//------------------------------------------------------------------------------
// 文件名     ：ioiicbus.c
// 模块描述:模块用于用GPIO模拟IIC时序，并将该IIC添加到IIC总线上。
// 模块版本: V1.00
// 创建人员: czz
// 创建时间: 2018.07.17
// =============================================================================
#include "IoIicBus.h"
#include "djyos.h"
#include "stdint.h"
#include "stdio.h"
#include "iicbus.h"
#include "endian.h"
#include "lock.h"
#include "int.h"
#include "stddef.h"
#include "stdlib.h"


#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"io analog iic bus"//填写该组件的名字
//parent:"iicbus"    //填写该组件的父组件名字，none表示没有父组件
//attribute:system              //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"iicbus"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_IO_ANALOG_IIC_BUS == false )
//#warning  " io_analog_iic_bus  组件参数未配置，使用默认配置"
//%$#@target = header      //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_IO_ANALOG_IIC_BUS    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,
//%$#@string,1,32,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure


struct IIC_Io_CB
{
   u32            delay_us;  //根据频率算出的高低电平时间用作延时
   IIC_IoCtrl     IOCtrl ;   //gpio控制回调函数指针
   u32            tag;       //用户附加的标签
};

// =============================================================================
// 功能：产生IIC起始信号
// 参数：IO模拟的IIC控制块结构体指针
// 返回：
// =============================================================================
static void IO_IIC_Start(struct IIC_Io_CB * ICB)
{
    ICB->IOCtrl(sda_set_out,ICB->tag);//sda线输出
    ICB->IOCtrl(scl_set_High,ICB->tag);//时钟设置高电平
    DJY_DelayUs(ICB->delay_us);
    ICB->IOCtrl(sda_set_Low,ICB->tag);//START:when CLK is high,DATA change form high to low
    DJY_DelayUs(ICB->delay_us);
}

// =============================================================================
// 功能：产生IIC停止信号
// 参数：IO模拟的IIC控制块结构体指针
// 返回：
// =============================================================================
static void IO_IIC_Stop(struct IIC_Io_CB * ICB)
{
    ICB->IOCtrl(sda_set_out,ICB->tag);//sda线输出
    ICB->IOCtrl(scl_set_Low,ICB->tag);
    ICB->IOCtrl(sda_set_Low,ICB->tag);//STOP:when CLK is high DATA change form low to high
    DJY_DelayUs(ICB->delay_us);
    ICB->IOCtrl(scl_set_High,ICB->tag);
    DJY_DelayUs(ICB->delay_us);
    ICB->IOCtrl(sda_set_High,ICB->tag);//发送I2C总线结束信号
}

// =============================================================================
// 功能：等待从器件应答
// 等待应答信号到来
// 返回值：false，接收应答失败
//      true，接收应答成功
// =============================================================================
static bool_t IO_IIC_WaitAck(struct IIC_Io_CB * ICB)
{
    u8 ucErrTime=0;
    ICB->IOCtrl(sda_set_in,ICB->tag);      //SDA设置为输入
    ICB->IOCtrl(sda_set_Low,ICB->tag);      //SDA设置为输入
    ICB->IOCtrl(scl_set_High,ICB->tag);
    DJY_DelayUs(ICB->delay_us);
    while(ICB->IOCtrl(sda_get,ICB->tag))
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            return false;
        }
    }
    ICB->IOCtrl(scl_set_Low,ICB->tag);//时钟输出0
    ICB->IOCtrl(sda_set_out,ICB->tag);
    return true;
}

// =============================================================================
// 功能：产生IIC 应答信号
// 参数：IO模拟的IIC控制块结构体指针
// 返回：
// =============================================================================
static void IO_IIC_Ack(struct IIC_Io_CB * ICB)
{
    ICB->IOCtrl(scl_set_Low,ICB->tag);
    ICB->IOCtrl(sda_set_out,ICB->tag);
    ICB->IOCtrl(sda_set_Low,ICB->tag);
    DJY_DelayUs(ICB->delay_us);
    ICB->IOCtrl(scl_set_High,ICB->tag);
    DJY_DelayUs(ICB->delay_us);
    ICB->IOCtrl(scl_set_Low,ICB->tag);
}

// =============================================================================
// 功能：产生NACK应答
// 参数：IO模拟的IIC控制块结构体指针
// 返回：
// =============================================================================
static void IO_IIC_NAck(struct IIC_Io_CB * ICB)
{
    ICB->IOCtrl(scl_set_Low,ICB->tag);
    ICB->IOCtrl(sda_set_out,ICB->tag);
    ICB->IOCtrl(sda_set_Low,ICB->tag);
    DJY_DelayUs(ICB->delay_us);
    ICB->IOCtrl(sda_set_High,ICB->tag);
    DJY_DelayUs(ICB->delay_us);
    ICB->IOCtrl(scl_set_High,ICB->tag);
    DJY_DelayUs(ICB->delay_us);
    ICB->IOCtrl(scl_set_Low,ICB->tag);
    DJY_DelayUs(ICB->delay_us);
    ICB->IOCtrl(sda_set_Low,ICB->tag);
}

// =============================================================================
// 功能：IIC发送一个字节
// 参数：IO模拟的IIC控制块结构体指针
// 返回：
// =============================================================================
static void IO_IIC_SendByte(struct IIC_Io_CB * ICB,u8 txd)
{
    u8 t;
    ICB->IOCtrl(sda_set_out,ICB->tag);
    ICB->IOCtrl(scl_set_Low,ICB->tag);//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {
        if((txd&0x80))
            ICB->IOCtrl(sda_set_High,ICB->tag);
        else
            ICB->IOCtrl(sda_set_Low,ICB->tag);
        txd<<=1;
        DJY_DelayUs(ICB->delay_us);
        ICB->IOCtrl(scl_set_High,ICB->tag);
        DJY_DelayUs(ICB->delay_us);
        ICB->IOCtrl(scl_set_Low,ICB->tag);
    }
}

// =============================================================================
// 功能：IIC读1个字节
// 参数：ICB：IO模拟的IIC控制块结构体指针
//     ack，ack=0，发送nACK 其他值发送ACK
// 返回：读取的字节
// =============================================================================
static u8 IO_IIC_ReadByte(struct IIC_Io_CB * ICB,u8 ack)
{
    unsigned char i,receive=0;
    ICB->IOCtrl(sda_set_in,ICB->tag);//SDA设置为输入
    for(i=0;i<8;i++ )
    {
        ICB->IOCtrl(scl_set_Low,ICB->tag);
        DJY_DelayUs(ICB->delay_us);
        ICB->IOCtrl(scl_set_High,ICB->tag);
        receive<<=1;
        if(ICB->IOCtrl(sda_get,ICB->tag))
            receive++;
        DJY_DelayUs(ICB->delay_us);
    }
    if (!ack)
        IO_IIC_NAck(ICB);//发送nACK
    else
        IO_IIC_Ack(ICB); //发送ACK
    return receive;
}


// =============================================================================
// 功能：IIC总线控制回调函数，被上层调用，目前只实现对IIC时钟频率配置
// 参数：ICB,个性标记，本模块内即 IO模拟的IIC控制块结构体指针
//       cmd,命令
//       data1,data2,数据，与具体命令相关
// 返回：无
// =============================================================================
static s32 IO_IIC_BusCtrl(struct IIC_Io_CB *ICB,u32 cmd,u32 data1,u32 data2)
{

    switch(cmd)
    {
        case CN_IIC_SET_CLK: //设置时钟频率
            ICB->delay_us = (1000000/(2*data1));
            break;
        case CN_IIC_DMA_USED://使用dma传输
            break;
        case CN_IIC_DMA_UNUSED://禁止dma传输
            break;
        case CN_IIC_SET_POLL:           //使用轮询方式发送接收
            break;
        default:     return 0;
    }
    return 1;
}
// =============================================================================
// 功能：轮询方式读IIC从设备的数据
// 参数：ICB,个性标记，本模块内即 IO模拟的IIC控制块结构体指针
//     devaddr,器件物理地址，最后一bit区分读写
//     memaddr//寄存器地址
//     maddrlen的寄存器地址的长度
//     addr,访问的寄存器地址
//     buf, 存储缓冲区
//     len, 存储缓冲区的长度
// 返回：len,读取字节数;-1,读取失败
// =============================================================================
static s32 IO_IIC_ReadPoll(struct IIC_Io_CB * ICB,u8 devaddr,u32 memaddr,
                        u8 maddrlen, u8 *buf, u32 len)
{

    u8 mem_addr_buf[4];
    //将地址作大小端变换
    u8 i;
    for(i=0;i<maddrlen;i++)
    {
        mem_addr_buf[(maddrlen-1)-i]=((memaddr>>(8*i))&0xff);
    }

    IO_IIC_Start( ICB);//发送起始位

    IO_IIC_SendByte(ICB, ((devaddr<<1))); // 发送写器件地址
    if(false ==IO_IIC_WaitAck( ICB))
        return (-1);

    for(i=0;i<maddrlen;i++)//发送器件内部地址
    {
      IO_IIC_SendByte(ICB, mem_addr_buf[i]);
      if(false ==IO_IIC_WaitAck( ICB))
          return (-1);
    }
    IO_IIC_Stop( ICB); //发送起始位
    IO_IIC_Start( ICB); //发送起始位
    IO_IIC_SendByte(ICB, ((devaddr<<1)|0x01));//发送器件地址读
    if(false ==IO_IIC_WaitAck( ICB))
        return (-1);

    for(i=0;i<len;i++)
    {
        buf[i]=IO_IIC_ReadByte(ICB,i==(len-1)?0:1); //发数据
    }
    IO_IIC_Stop(ICB);

    return i;
}

// =============================================================================
// 功能：轮询方式向IIC从设备的写数据
// 参数：    ICB,个性标记，本模块内即 IO模拟的IIC控制块结构体指针
//       devaddr,设备地址
//       memaddr,设备内部地址
//       maddrlen,设备内部地址长度
//       buf,存储数据
//       len,数据长度，字节
// 返回：len,读取成功;-1,读取失败
// =============================================================================
static s32 IO_IIC_WritePoll(struct IIC_Io_CB * ICB,u8 devaddr,u32 memaddr,
                        u8 maddrlen, u8 *buf, u32 len)
{
    u8 mem_addr_buf[4];
    u8 i;
    for(i=0;i<maddrlen;i++)
    {
        mem_addr_buf[(maddrlen-1)-i]=((memaddr>>(8*i))&0xff);
    }

    IO_IIC_Start(ICB);

    IO_IIC_SendByte(ICB,(devaddr<<1));    //发送器件地址
    if(false ==IO_IIC_WaitAck( ICB))
             return 0;

    for(i=0;i<maddrlen;i++)
    {
        IO_IIC_SendByte(ICB,mem_addr_buf[i]);    //发送器件内部地址
        if(false ==IO_IIC_WaitAck( ICB))
                 return 0;
    }

    for(i=0;i<len;i++)
    {
        IO_IIC_SendByte(ICB,buf[i]);   //发数据
        if(false == IO_IIC_WaitAck(ICB))
            break;
    }
    IO_IIC_Stop(ICB);                  //产生一个停止条件
    return i;
}

// =============================================================================
// 功能：轮询方式读写IIC设备
// 参数：    ICB,个性标记，本模块内即 IO模拟的IIC控制块结构体指针
//       DevAddr,设备地址
//       MemAddr,设备内部地址
//       MemAddrLen,设备内部地址长度
//       Buf,存储数据
//       Length,数据长度，字节
//       WrRdFlag,读写标记，为0时写，1时为读
// 返回：
// =============================================================================
static bool_t IO_IIC_WriteReadPoll( struct IIC_Io_CB * ICB,u8 DevAddr,u32 MemAddr,\
                                u8 MemAddrLen,u8* Buf, u32 Length,u8 WrRdFlag)
{
    static bool_t flag;


    if(WrRdFlag == CN_IIC_WRITE_FLAG)   //写
    {
        if(Length == IO_IIC_WritePoll(ICB,DevAddr,MemAddr,MemAddrLen,Buf,Length))
            flag = true;
        else
            flag = false;
    }
    else                                //读
    {
        if(Length == IO_IIC_ReadPoll(ICB,DevAddr,MemAddr,MemAddrLen,Buf,Length))
            flag = true;
        else
            flag = false;
    }
    return  flag ;
}

// =============================================================================
// 功能：添加IO模拟的IIC总线
// 参数： init,寄存器基址
//       *BusName      //总线名称，如IIC1
//       tag           //用户自己的标记
//       IICBufLen     //总线缓冲区大小，字节
//       Clk           //总线频率单位HZ
//       IIC_IoCtrl    //根据命令enum IIc_Io 和用户自己的标签设置IO的状态
// 返回：true/false
// =============================================================================
bool_t ModuleInstall_IO_IICBus(struct IO_IIC_Init * init)
{
    struct IIC_Io_CB * IicIoCb;
    struct IIC_Param IIC_Config;

    IicIoCb = (struct IIC_Io_CB *)M_Malloc(sizeof(struct IIC_Io_CB),0);
    if(IicIoCb == NULL)
    {
        free(IicIoCb);
        return false;
    }

    IicIoCb->delay_us              = (u32)(5);//默认值芯片添加时会设置
    IicIoCb->IOCtrl                = init->IIC_IoCtrl ;
    IicIoCb->tag                   = init->tag ;
//    IicIoCb->IOCtrl(sda_set_out,IicIoCb->tag);
//    IicIoCb->IOCtrl(scl_set_out,IicIoCb->tag);
//    IicIoCb->IOCtrl(sda_set_High,IicIoCb->tag);
//    IicIoCb->IOCtrl(scl_set_High,IicIoCb->tag);

    IIC_Config.BusName             = init->BusName;
    IIC_Config.IICBuf              = NULL;
    IIC_Config.IICBufLen           = 0;
    IIC_Config.SpecificFlag        = (ptu32_t)IicIoCb;
    IIC_Config.pGenerateWriteStart = (WriteStartFunc)NULL;
    IIC_Config.pGenerateReadStart  = (ReadStartFunc)NULL;
    IIC_Config.pGenerateEnd        = (GenerateEndFunc)NULL;
    IIC_Config.pBusCtrl            = (IICBusCtrlFunc)IO_IIC_BusCtrl;
    IIC_Config.pWriteReadPoll      = (WriteReadPoll)IO_IIC_WriteReadPoll;

    if(NULL != IIC_BusAdd(&IIC_Config))
        return true;
    return false;

}



