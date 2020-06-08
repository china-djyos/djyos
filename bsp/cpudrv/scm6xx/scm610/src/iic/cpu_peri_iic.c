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
//-----------------------------------------------------------------------------
// =============================================================================

// 文件名     ：cpu_peri_iic.c
// 模块描述: IIC模块底层硬件驱动，寄存器级别的操作
// 模块版本: V1.00
// 创建人员: huzb
// 创建时间: 29/09.2015
// =============================================================================

#include <stdint.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <cpu_peri_iic.h>
#include "cpu_peri.h"
#include "cpu_peri_uart.h"
#include "stdlib.h"
#include "iicbus.h"
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


typedef struct
{
    volatile u32 CFGR;
    volatile u32 CLKR;
    volatile u32 ADR;
    volatile u32 CMDR;
    volatile u32 SR;
    volatile u32 BUFR;
}tagI2CReg;


#define CN_I2C0_BASE (0x40200000UL)
#define CN_I2C1_BASE (0x40210000UL)
#define CN_I2C2_BASE (0x41220000UL)

static volatile tagI2CReg *sgpt_I2cReg[] =
{
    (volatile tagI2CReg*)CN_I2C0_BASE,
    (volatile tagI2CReg*)CN_I2C1_BASE,
    (volatile tagI2CReg*)CN_I2C2_BASE,
};

#define CN_IIC_NUM (3)

static u32 IIC_BUF_LEN[CN_IIC_NUM]={128,128,128};

//iic缓冲区定数组
static u8 s_IIC1Buf[128];
static u8 s_IIC2Buf[128];
static u8 s_IIC3Buf[128];

static u8* s_IICBuf[CN_IIC_NUM]=
{
    s_IIC1Buf,
    s_IIC2Buf,
    s_IIC3Buf,
};

struct IIC_IntParamSet
{
    struct SemaphoreLCB *pDrvPostSemp;   //信号量
    u32 TransCount;       //传输数据量计数器
    u32 TransTotalLen;//接收的数据总量
};

static struct IIC_IntParamSet IntParamset[CN_IIC_NUM];
static struct IIC_CB* sgpt_IIC_CB[CN_IIC_NUM];
const char *IIC_Name[CN_IIC_NUM]= {"IIC1","IIC2","IIC3"};

__attribute__((weak))  void IIC_InLowPowerPinCfg(u8 iic_port)
{
    return;
}
__attribute__((weak))  void IIC_OutLowPowerPinCfg(u8 iic_port)
{
    return ;
}


// =============================================================================
// 功能： 进入或退出低功耗模式
// 参数： iic_port IIC 端口号
//     flag 进入还是退出低功耗模式
// 返回：true/false
// =============================================================================
//

bool_t IIC_LowPowerConfig(u8 iic_port,u8 flag)
{

   return true;
}

// =============================================================================
// 功能：IIC检查是否有错误
// 参数： reg iic基地址
// 返回：true/false
// =============================================================================

static bool_t  _IIC_Chek(tagI2CReg *reg)
{
    if(reg->SR & (1<<13))
    {
       return false;
    }

    return true;
}

// =============================================================================
// 功能: 使能iic中断,接收与发送共用一个中断源。
// 参数: reg,被操作的寄存器组指针
// 返回: 无
// =============================================================================

static void __IIC_IntEnable(tagI2CReg *reg)
{
    //总中断开关使能
    reg->CFGR |= (1<<5);
    //发送中断 、接收中断使能
    reg->CFGR |= (1<<10);
}

// =============================================================================
// 功能: 禁止iic中断,接收与发送共用一个中断源。
// 参数: reg,被操作的寄存器组指针
// 返回: 无
// =============================================================================

static void __IIC_IntDisable(tagI2CReg *reg)
{
    reg->CFGR &= ~(1<<5);
}

// =============================================================================
// 功能: 传输速率设置
// 参数: iic_port iic编号，
//
//
//
// 返回: 无
// =============================================================================

static bool_t __IIC_TransportBaurtClkSet(u8 iic_port,u32 speed)
{
    u32 temp;

    if(iic_port > CN_IIC_NUM)
    {
        return false;
    }

    switch(speed)
    {
        case CN_IIC_SPEED_100KHZ:
            sgpt_I2cReg[iic_port]->CLKR = CN_IIC_SPEED_100KHZ;
            break;
        case CN_IIC_SPEED_400KHZ:
            sgpt_I2cReg[iic_port]->CLKR = CN_IIC_SPEED_400KHZ;
            break;
        case CN_IIC_SPEED_1000KHZ:
            sgpt_I2cReg[iic_port]->CLKR = CN_IIC_SPEED_1000KHZ;
            break;
        default:;break;
    }
    return true;
}

// =============================================================================
// 功能: I/O模拟的方式 释放没有复位和使能引脚的IIC器件
// 参数: port I/O口 如 CN_GPIO_B
//      TS_SDA:引脚号
//      TS_SCK:引脚号
// 返回: true/false
// =============================================================================

bool_t IIC_Busfree(u32 port,u32 sda_pin,u32 sck_pin)
{


    return true;
}

// =============================================================================
// 功能: 硬件GPIO初始化，包括时钟选择和iic初始化参数
// 参数: IIC_NO,编号
// 返回: true/false
// =============================================================================
static bool_t __IIC_GpioConfig(u8 iic_port)
{
    //congig i2c clock
    u8 I2cClockBit;

    switch(iic_port)
    {
       case CN_I2C1:
            I2cClockBit = 14;
            break;
       case CN_I2C2:
            I2cClockBit = 15;
            break;
       case CN_I2C3:
            I2cClockBit = 29;
            break;
       default:;break;
    }

    CLK_Enable_Peripheral_Clk(I2cClockBit);

    sgpt_I2cReg[iic_port]->CLKR = 40000000 / (4*10);
    u32 value = sgpt_I2cReg[iic_port]->CLKR;
    //设置不去毛刺
    sgpt_I2cReg[iic_port]->CFGR &= ~(3<<3);
    sgpt_I2cReg[iic_port]->CFGR |= (0x0<<3);

    //配置7Bit 从机地址
    sgpt_I2cReg[iic_port]->ADR &= ~(0x1<<15);

    //使能I2c
    sgpt_I2cReg[iic_port]->CFGR |= (0x1<<0);

    return true;
}

// =============================================================================
// 功能: IIC中断配置函数
// 参数: IntLine,中断线
//       isr,中断服务函数指针
// 返回: 无
// =============================================================================
static void __IIC_IntConfig(u8 iic_port,u32 (*isr)(ptu32_t))
{
    ufast_t IntLine;
    switch (iic_port)
    {
      case CN_I2C1:
          IntLine=CN_INT_LINE_IIC0;
          break;
      case CN_I2C2:
          IntLine=CN_INT_LINE_IIC1;
          break;
      case CN_I2C3:
          IntLine=CN_INT_LINE_IIC2;
          break;
          default:return;
     }

     Int_Register(IntLine);
     Int_IsrConnect(IntLine,isr);
     Int_SettoAsynSignal(IntLine);
     Int_ClearLine(IntLine);     //清掉初始化产生的发送fifo空的中断
     Int_RestoreLine(IntLine);   //清掉初始化产生的发送fifo空的中断
}

// =============================================================================
// 功能: 产生停止时序，结束本次与外器件之间的通信
// 参数: reg,iic外设寄存器基址
// 返回: 无
// =============================================================================

static void __IIC_GenerateStop(tagI2CReg *reg)
{


}

// =============================================================================
// 功能：IIC写
// 参数： reg,寄存器基址
//        devaddr,器件物理地址，最后一bit区分读写
//         buf_addr地址写缓冲
//       len_addr 地址长度
//         buf写缓冲
//       len 长度
// 返回：true/false
// =============================================================================

static bool_t __IIC_Write(tagI2CReg *reg,u8 devaddr, u32 memaddr, u32 addr_len,u8 *buf, u32 len)
{
    u32 timeout=0;
    u32 Time;
    u32 i=0;
    u32 datLoop = 0;
    u8  addressLoop;
    u32 internalDevAddress = 0;
    u32 datToSend = 0;

    //get device internal address
    //from hight to low
    internalDevAddress =  memaddr;

    while(datToSend < len)
    {

        //1、Write Dev Address
        reg->BUFR = (devaddr & 0xfe);//Write Into Device Address
        //Set Cpu Master Mode and Get Start Singnal To Send
        reg->CMDR = (1<<0);
        //Wait To Send Int Status being Set
        while(!(reg->SR & (1<<10)));
        //Must Clear It For Continue To Send
        reg->SR = 1<<10;
        //Clear Start Condition Flag
        reg->SR = 1<<8;


        //2、First to send internal dev address

        while(i < addr_len)
        {
            u8 interPartDevAddress = (internalDevAddress >> ((addr_len - 1 -i)*8)) & 0xff;
            *((volatile u8 *)&reg->BUFR) = interPartDevAddress;
            i++;
            reg->CMDR = (1<<0);
            while(!(reg->SR & (1<<10)));
            reg->SR = 1<<10;
            reg->SR = 1<<8;
        }

        //clear var i cnt for next time
        i = 0;

        //Here to send Data and generate stop signal
        reg->BUFR = buf[datLoop++];
        while(!(reg->SR & (1<<10)));
        reg->SR = 1<<10;
        reg->CMDR = (0<<0);
        while(!(reg->SR & (1<<9)));//Wait stop signal
        reg->SR = 1<<9;
        //add address
        internalDevAddress++;
        datToSend++;
//        while(!(reg->SR & (1<<9)));
//      delay(10);
//        delay_ms(3);
        DJY_DelayUs(3000);
    }
    return true;
}


// =============================================================================
// 功能：IIC读
// 参数： reg,寄存器基址
//        devaddr 器件地址
//         buf收缓冲区
//       len 长度
// 返回：true/false
// =============================================================================

static bool_t __IIC_Read(tagI2CReg *reg,u8 devaddr, u32 memaddr, u32 addr_len,u8 *buf, u32 len)
{
    u32 internalDevAddress = 0;
    u32 datToRev = 0;
    u32 i = 0;
    u8 addressLoop;

    internalDevAddress =  memaddr;

    while(datToRev < len)
    {
        //1、Write Dev Address
        reg->BUFR = (devaddr & 0xfe);           //Write Into Device Address
        //Set Cpu Master Mode and Get Start Singnal To Send
        reg->CMDR = (1<<0);
        //Wait To Send Int Status being Set
        while(!(reg->SR & (1<<10)));
        //Must Clear It For Continue To Send
        reg->SR = 1<<10;
        //Clear Start Condition Flag
        reg->SR = 1<<8;


        //2、First to send internal dev address
        while(i < addr_len)
        {
            u8 interPartDevAddress = (internalDevAddress >> ((addr_len - 1 -i)*8)) & 0xff;
            *((volatile u8 *)&reg->BUFR) = interPartDevAddress;
            i++;
            reg->CMDR = (1<<0);
            while(!(reg->SR & (1<<10)));
            reg->SR = 1<<10;
            reg->SR = 1<<8;
        }

        //clear var i cnt for next time
        i = 0;

        reg->CMDR = 0x3;
        while(!(reg->SR & (1<<8) ));
        reg->SR = 0x100;
        devaddr &= ~0x01;
        devaddr |= 0x1;
        reg->BUFR = devaddr;
        while(!(reg->SR & (1<<10)));
        reg->SR |= 1<<10;

        //Waite to Rev Int
        while(!(reg->SR & (1<<10)));
        reg->SR |= ( 1<<10);
        buf[datToRev] = reg->BUFR;     //Get One Byte Data
        reg->CMDR = (1<<2) ;          //NACK+STOP
        while(!(reg->SR & (1<<9)));
        reg->SR |= (1<<9);

        datToRev++;
        internalDevAddress++;
    }

    return true;
}


// =============================================================================
// 功能：轮询方式读IIC从设备的数据
// 参数：reg,寄存器基址
//     devaddr,器件物理地址，最后一bit区分读写
//     memaddr//寄存器地址
//     maddrlen的寄存器地址的长度
//       addr,访问的寄存器地址
//       buf, 存储缓冲区
//       len, 存储缓冲区的长度
// 返回：len,读取成功;-1,读取失败
// =============================================================================

static s32 __IIC_ReadPoll(tagI2CReg *reg,u8 devaddr,u32 memaddr,
                        u8 maddrlen, u8 *buf, u32 len)
{


    if(false == __IIC_Read(reg, devaddr,memaddr, maddrlen,buf,len));
        return -1;

    return len;
}

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

static s32 __IIC_WritePoll(tagI2CReg *reg,u8 devaddr,u32 memaddr,
                        u8 maddrlen, u8 *buf, u32 len)
{

    if( __IIC_Write(reg, devaddr, memaddr, maddrlen,buf,len)==false)//写存储器地址
       return -1;

    return len;

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
// 返回：
// =============================================================================

static bool_t __IIC_WriteReadPoll(tagI2CReg *reg,u8 DevAddr,u32 MemAddr,\
                                u8 MemAddrLen,u8* Buf, u32 Length,u8 WrRdFlag)
{
    static bool_t flag;

    __IIC_IntDisable(reg);

    if(WrRdFlag == CN_IIC_WRITE_FLAG)   //写
    {
        if(Length == __IIC_WritePoll(reg,DevAddr,MemAddr,MemAddrLen,Buf,Length))
            flag = true;
        else
            flag = false;
    }
    else                                //读
    {
        if(Length == __IIC_ReadPoll(reg,DevAddr,MemAddr,MemAddrLen,Buf,Length))
            flag = true;
        else
            flag = false;
    }


    return  flag ;
}


// =============================================================================
// 功能: 启动写时序，启动写时序的过程为：器件地址（写）、存储地址（写），当存储地址完
//       成时，需打开中断，之后将会发生发送中断，在中断服务
//       函数中，每次发送一定的字节数，直到len数据量，post信号量iic_bus_semp，并产生
//       停止时序
// 参数: SpecificFlag,个性标记，本模块内即IIC寄存器基址
//      DevAddr,器件地址的前7比特，已将内部地址所占的bit位更新，该函数需将该地址左
//               移一位增加增加最后一位读/写比特;
//      MemAddr,存储器内部地址，即发送到总线上的地址，该地址未包含放在器件地址上的
//               比特位；
//      MenAddrLen,存储器内部地址的长度，字节单位，未包含在器件地址里面的比特位；
//      Length,接收的数据总量，接收数据的倒数第一字节，即count-1，停止产生ACK信号，
//          当接收的字节数为count时，产生停止时序，并释放信号量iic_semp;
//      IIC_BusSemp,发送完成时驱动需释放的信号量。发送程序读IIC_PortRead时，若读不到数
//          则需释放该信号量，产生结束时序
// 返回: TRUE，启动读时序成功，FALSE失败
// =============================================================================

static bool_t __IIC_GenerateWriteStart(tagI2CReg *reg,
                                       u8 dev_addr,
                                       u32 mem_addr,
                                       u8 maddr_len,
                                       u32 length,
                                       struct SemaphoreLCB *iic_semp)
{
    u8 mem_addr_buf[4];
    u8 iic_port;
    //通过specific_flag区分是哪条总线
    if(reg==sgpt_I2cReg[CN_I2C1])
        iic_port=CN_I2C1;
    else if (reg==sgpt_I2cReg[CN_I2C2])
        iic_port=CN_I2C2;
    else if (reg==sgpt_I2cReg[CN_I2C3])
        iic_port=CN_I2C3;
    else
        return false;

    IntParamset[iic_port].TransTotalLen = length;
    IntParamset[iic_port].TransCount = 0;
    IntParamset[iic_port].pDrvPostSemp = iic_semp;           //iic_bus_semp


    __IIC_IntDisable(reg);                 //关中断
    return true;
}


// =============================================================================
// 功能: 启动读时序，启动读时序的过程为：器件地址（写）、存储地址（写）、器件地址（读）
//       当器件地址（读）完成时，需打开中断，重新配置寄存器为接收模式，之后将会发生
//       接收数据中断，在中断中将接收到的数据调用IIC_PortWrite写入缓冲，接收到len字
//       节数的数据后，释放信号量iic_semp
// 参数: SpecificFlag,个性标记，本模块内即IIC寄存器基址
//      DevAddr,器件地址的前7比特，已将内部地址所占的bit位更新，该函数需将该地址左
//               移一位增加增加最后一位读/写比特;
//     MemAddr,存储器内部地址，即发送到总线上的地址，该地址未包含放在器件地址上的
//               比特位；
//      MenAddrLen,存储器内部地址的长度，字节单位，未包含在器件地址里面的比特位；
//      Length,接收的数据总量，接收数据的倒数第一字节，即count-1，停止产生ACK信号，当接
//             收的字节数为count时，产生停止时序，并释放信号量iic_semp;
//     IIC_BusSemp,读完成时，驱动需释放的信号量（缓冲区信号量）
// 返回: TRUE，启动读时序成功，FALSE失败
// =============================================================================

static bool_t __IIC_GenerateReadStart( tagI2CReg *reg,
                                       u8 dev_addr,
                                       u32 mem_addr,
                                       u8 maddr_len,
                                       u32 length,
                                       struct SemaphoreLCB *iic_semp)
{
    u8 iic_port;
    if(reg==sgpt_I2cReg[CN_I2C1])
      iic_port=CN_I2C1;
    else if(reg==sgpt_I2cReg[CN_I2C2])
      iic_port=CN_I2C2;
    else if (reg==sgpt_I2cReg[CN_I2C3])
      iic_port=CN_I2C3;
    else
      return false;

  IntParamset[iic_port].TransTotalLen = length;
  IntParamset[iic_port].TransCount = 0;
  IntParamset[iic_port].pDrvPostSemp = iic_semp;               //iic_buf_semp
  __IIC_IntDisable(reg);//关中断

  return true;
}


// =============================================================================
// 功能: 结束本次读写回调函数，区分读写的不同停止时序，当属于发送时，则直接停止时序，
//      若为读，则先停止回复ACK，再停止
// 参数: reg,个性标记，本模块内即IIC寄存器基址
// 返回: 无
// =============================================================================

static void __IIC_GenerateEnd(tagI2CReg *reg)
{

}


//==============================================================================
// 功能：IIC总线控制回调函数，被上层调用，目前只实现对IIC时钟频率配置
// 参数：TIMINGR-->位 31:28 PRESC时序预分频因子
//                   位 23:20 SCLDEL[3:0]：数据建立时间
//                   位 19:16 SDADEL[3:0]：数据保持时间
//                   位 15:8 SCLH[7:0]：SCL 高电平周期
//                   位 7:0 SCLL[7:0]：SCL 低电平周期
//   reg    本模块内即IIC寄存器基址
// 返回：无
//==============================================================================

static void __IIC_ClkSet(tagI2CReg *reg,u32 data)
{

}


// =============================================================================
// 功能：IIC总线控制回调函数，被上层调用，目前只实现对IIC时钟频率配置
// 参数：reg,个性标记，本模块内即IIC寄存器基址
//       cmd,命令
//       data1,data2,数据，与具体命令相关
// 返回：无
// =============================================================================
static s32 __IIC_BusCtrl(tagI2CReg *reg,u32 cmd,u32 data1,u32 data2)
{
    if( reg == NULL )
           return 0;
       switch(cmd)
       {
           case CN_IIC_SET_CLK: //设置时钟频率
               __IIC_ClkSet(reg,data1);
                 break;
           case CN_IIC_DMA_USED://使用dma传输

               break;
           case CN_IIC_DMA_UNUSED://禁止dma传输

               break;
           case CN_IIC_SET_POLL:           //使用轮询方式发送接收
               __IIC_IntDisable(reg);
               break;
           default:
               return 0;
       }
       return 1;
}


// =============================================================================
// 功能：        IIC接收与发送中断服务函数。该函数实现的功能如下：
//       1.每发送与接收一个或若干字节发生一次中断；
//       2.若有多个中断使用同一个中断号，则需根据具体情况区分使用的是哪个中断；
//       3.清中断标志，并判断ACK信号，每读写字节，计数器都需相应修改；
//       4.接收达到倒数第一个字节时，需配置不发送ACK信号；
//       5.接收或发送完成时，需post信号量IntParam->pDrvPostSemp；
//       6.接收或发送完成时，需产生停止时序。
// 参数：i2c_int_line,中断号，本函数没用到
// 返回：true falst
// =============================================================================

static u32 __IIC_ISR(ufast_t i2c_int_line)
{


    return true;
}


// =============================================================================
// 功能：IIC底层驱动的初始化，完成整个IIC总线的初始化，其主要工作如下：
//       1.初始化总线控制块IIC_CB，回调函数和缓冲区的初始化赋值；
//       2.默认的硬件初始化，如GPIO或IIC寄存器等；
//       3.中断初始化，完成读写中断配置；
//       4.调用IICBusAdd或IICBusAdd_r增加总线结点；
// 参数：iic_port  IIC编号 CN_IIC1
//       Add_mode 寻址模式
// 返回：无
// =============================================================================

bool_t ModuleInstall_I2C(u8 iic_port)
{

    struct IIC_Param IIC_Config[CN_IIC_NUM];

    IIC_Config[iic_port].BusName             = (char *)IIC_Name[iic_port];
    IIC_Config[iic_port].IICBuf              = (u8*)s_IICBuf[iic_port];
    IIC_Config[iic_port].IICBufLen           = IIC_BUF_LEN[iic_port];
    IIC_Config[iic_port].SpecificFlag        = (ptu32_t)sgpt_I2cReg[iic_port];
    IIC_Config[iic_port].pGenerateWriteStart = (WriteStartFunc)__IIC_GenerateWriteStart;
    IIC_Config[iic_port].pGenerateReadStart  = (ReadStartFunc)__IIC_GenerateReadStart;
    IIC_Config[iic_port].pGenerateEnd        = (GenerateEndFunc)__IIC_GenerateEnd;
    IIC_Config[iic_port].pBusCtrl            = (IICBusCtrlFunc)__IIC_BusCtrl;
    IIC_Config[iic_port].pWriteReadPoll      = (WriteReadPoll)__IIC_WriteReadPoll;

    __IIC_GpioConfig(iic_port);
    __IIC_IntConfig(iic_port,__IIC_ISR);

    sgpt_IIC_CB[iic_port] = IIC_BusAdd(&IIC_Config);
    if(sgpt_IIC_CB[iic_port] == NULL)
    {
        return 0;
    }

    return 1;
}


