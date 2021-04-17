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

// 文件名     ：cpu_peri_iic.c
// 模块描述: IIC模块底层硬件驱动，寄存器级别的操作
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 05/12.2014
// =============================================================================
#include "cpu_peri.h"
#include "cpu_peri_iic.h"
#include "SysTime.h"
#include <string.h>
#include "endian.h"
#include "iicbus.h"
#include "int.h"
#include "djyos.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    #if CFG_IIC0_ENABLE==1
//    extern bool_t ModuleInstall_IIC(u8 port);
//    ModuleInstall_IIC(0);
//    #endif
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip iic"//CPU的iic总线驱动
//parent:"iicbus"    //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                         //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                      //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                        //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium                      //初始化时机，可选值：early，medium，later, pre-main。
                                        //表示初始化时间，分别是早期、中期、后期
//dependence:"iicbus","int","lock"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                        //选中该组件时，被依赖组件将强制选中，
                                        //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"                 //该组件的弱依赖组件名（可以是none，表示无依赖组件），
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
#define CFG_IIC0_BUF_LEN         128           //"IIC0缓冲区大小",
//%$#@enum,true,false,
#define CFG_IIC0_ENABLE          false        //"配置是否使用IIC0",
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure
//==============================================================================

#define CN_IIC_TIMEOUT  (1000 * mS)
#define CN_IIC_REGISTER_BADDR0        0x54000000

//中断中使用的数据类型结构体
struct IIC_IntParamSet
{
    struct SemaphoreLCB *pDrvPostSemp;   //信号量
    u32 TransCount;                     //传输数据量计数器
    u32 TransTotalLen;
};

//定义静态变量
static struct IIC_CB *s_ptIIC0_CB;
//#define IIC0_BUF_LEN        128
static u8 s_IIC0Buf[CFG_IIC0_BUF_LEN];
static struct IIC_IntParamSet IntParamset0;

// =============================================================================
// 功能: 使能iic中断,接收与发送共用一个中断源。
// 参数: reg,被操作的寄存器组指针
//       IntSrc,中断源
// 返回: 无
// =============================================================================
static void __IIC_IntDisable(volatile tagI2CReg *reg)
{
    reg->IICCON &= ~IICCON_INT_MASK;
}

// =============================================================================
// 功能: 禁止iic中断
// 参数: reg,被操作的寄存器组指针
//       IntSrc,操作的中断源
// 返回: 无
// =============================================================================
static void __IIC_IntEnable(volatile tagI2CReg *reg)
{
    reg->IICCON |= IICCON_INT_MASK;
}

static void __IIC_IntPendingBitClear(volatile tagI2CReg *reg)
{
    reg->IICCON &= ~(IICCON_INTPENDINGFLAG_MASK);
}

// =============================================================================
// 功能: 禁止iic中断,接收与发送共用一个中断源。
// 参数: reg,被操作的寄存器组指针
// 返回: 无
// =============================================================================
static void __IIC_GenerateStop(volatile tagI2CReg *reg)
{
    u32 timeout = CN_IIC_TIMEOUT;
    if((reg->IICSTAT & IICSTAT_MS(3)) == IICSTAT_MS_MT_MODE)
        reg->IICSTAT = 0xD0;
    else if((reg->IICSTAT & IICSTAT_MS(3)) == IICSTAT_MS_MR_MODE)
        reg->IICSTAT = 0x90;

    while((reg->IICSTAT & IICSTAT_BSY_MASK) && (timeout > 0))
    {
        DJY_EventDelay(100);
        timeout -= 100;
    }
}

// =============================================================================
// 功能：IIC时钟配置函数，时钟来源为MCK，计算方法可参考技术手册
// 参数：reg，设备句柄
//      iicclk，欲配置的时钟速度，标准的时钟如50KHz,100KHz,200KHz
// 返回：true=成功，false=失败
// =============================================================================
static void __IIC_SetClk(volatile tagI2CReg *reg,u32 iicclk)
{
    u32 BaseClk = CN_CFG_PCLK/256,temp;

    if(iicclk < BaseClk)    //大于100K
    {
        reg->IICCON |= IICCON_IICLK_MASK;       //IICCLK = CN_CFG_PCLK/16
        temp = (BaseClk/iicclk) - 1;
        if(temp < 16)
        {
            reg->IICCON &= ~ (0xF << IICCON_TXCLK_OFFSET);
            reg->IICCON |= IICCON_TXCLK(temp);
        }
    }
}


// =============================================================================
// 功能：IIC引脚初始化，初始化为外设使用
// 参数：iic_no,IIC控制器号
// 返回：无
// =============================================================================
static void __IIC_GpioInit(void)
{
    u32 temp;
    temp = pg_gpio_reg->GPECON;
    temp &= ~(0x0F << 28);
    temp |= (0x0A << 28);       //配置IO口为SDA和SCL
    pg_gpio_reg->GPECON = temp;
    //pg_gpio_reg->GPHUP
}

// =============================================================================
// 功能: IIC默认硬件初始化配置，主要是时钟配置和GPIO写保护引脚配置
// 参数: RegBaseAddr,寄存器基址
//       iicno,IIC控制器号
// 返回: 无
// =============================================================================
static void __IIC_HardConfig(ptu32_t RegBaseAddr,u8 iicno)
{
    volatile tagI2CReg *reg;
    reg = (volatile tagI2CReg *)RegBaseAddr;

    __IIC_GpioInit();

    __IIC_SetClk(reg,CN_IIC_CLK_100K);

}

// =============================================================================
// 功能: IIC中断配置函数
// 参数: IntLine,中断线
//       ISR,中断服务函数指针
// 返回: 无
// =============================================================================
static void __IIC_IntConfig(u32 IntLine,u32 (*ISR)(ptu32_t))
{
    //中断线的初始化
    Int_Register(IntLine);
    Int_IsrConnect(IntLine,ISR);
    Int_SettoAsynSignal(IntLine);
    Int_ClearLine(IntLine);     //清掉初始化产生的发送fifo空的中断
    Int_RestoreAsynLine(IntLine);
}

// =============================================================================
// 功能：查询是否接收到ACK信号（2440是查询中断标志）
// 参数：寄存器基址指针
// 返回：true,成功;false,失败
// =============================================================================
static bool_t __IIC_AckReceived(volatile tagI2CReg *Reg)
{
    u32 timeout = CN_IIC_TIMEOUT;
//    while(Reg->IICSTAT & IICSTAT_ACK_MASK);                       //判断ACK
//    while(!(Reg->IICCON & IICCON_INTPENDINGFLAG_MASK));

    while((!(Reg->IICCON & IICCON_INTPENDINGFLAG_MASK)) && (timeout > 0))
    {
        DJY_DelayUs(1);
        timeout -=1;
    }
    if(timeout == 0)
    {
        return false;
    }
    return true;
}

// =============================================================================
// 功能: 启动写时序，启动写时序的过程为：器件地址（写）、存储地址（写），当存储地址完
//       成时，需打开中断，重新配置寄存器为接收模式，之后将会发生发送中断，在中断服务
//       函数中，每次发送一定的字节数，直到len数据量，post信号量iic_bus_semp，并产生
//       停止时序
// 参数: specific_flag,个性标记，本模块内即IIC寄存器基址
//      dev_addr,器件地址的前7比特，最后的三个比特位已经更新，该函数内部需将该地址左
//               移一位，并修改最后的读写比特位，最后一比特写0，表示写，最后一比特写
//               1,表示读;
//      mem_addr,存储器内部地址,该函数需发送低maddr_len字节到总线
//      maddr_len,存储器内部地址的长度，字节单位；
//      length,发送的数据总量，接收数据的倒数第一字节，即count-1，停止产生ACK信号，
//          当接收的字节数为count时，产生停止时序，并释放信号量iic_semp;
//      iic_semp,发送完成时驱动需释放的信号量。发送程序读IIC_PortRead时，若读不到数
//          则需释放该信号量，产生结束时序
// 返回: TRUE，启动读时序成功，FALSE失败
// =============================================================================
static bool_t __IIC_GenerateWriteStart(volatile tagI2CReg *Reg,
                                       u8 dev_addr,
                                       u32 mem_addr,
                                       u8 maddr_len,
                                       u32 length,
                                       struct SemaphoreLCB *iic_semp)
{
    struct IIC_IntParamSet *IntParam;
    u8 i;u32 timeout = CN_IIC_TIMEOUT;

    if((u32)Reg != CN_IIC_REGISTER_BADDR0)
        return false;

    IntParam = &IntParamset0;
    IntParam->TransTotalLen = length;
    IntParam->TransCount = 0;
    IntParam->pDrvPostSemp = iic_semp;

    //关中断
    Int_CutLine(cn_int_line_iic);
    __IIC_IntEnable(Reg);

    while((Reg->IICSTAT & IICSTAT_BSY_MASK) &&(timeout > 0))    //check if busy
    {
        DJY_EventDelay(100);
        timeout -= 100;
    }
    if(timeout == 0)
    {
        return false;
    }
    Reg->IICCON |= IICCON_ACKEN_MASK;
    Reg->IICSTAT = IICSTAT_MS_MT_MODE | IICSTAT_TXRXEN_MASK; //配置为发送主模式
    Reg->IICDS = (dev_addr<<CN_IIC_ADDR_OFFSET) | CN_IIC_WRITE_BIT; //发设备地址
    __IIC_IntPendingBitClear(Reg);

    Reg->IICSTAT |= IICSTAT_START_STOP_MASK;                    //启动发送
    if(! __IIC_AckReceived(Reg))
    {
        __IIC_GenerateStop(Reg);
        return false;
    }

    for(i = 0; i < maddr_len; i ++)
    {
        Reg->IICDS = (u8) (mem_addr >> ((maddr_len - i - 1) *8));
        __IIC_IntPendingBitClear(Reg);
        if(! __IIC_AckReceived(Reg))
        {
            __IIC_GenerateStop(Reg);
            return false;
        }
    }
    Int_ContactLine(cn_int_line_iic);
    return true;
}

// =============================================================================
// 功能: 启动读时序，启动读时序的过程为：器件地址（写）、存储地址（写）、器件地址（读）
//       当器件地址（读）完成时，需打开中断，重新配置寄存器为接收模式，之后将会发生
//       接收数据中断，在中断中将接收到的数据调用IIC_PortWrite写入缓冲，接收到len字
//       节数的数据后，释放信号量iic_semp
// 参数: specific_flag,个性标记，本模块内即IIC寄存器基址
//      dev_addr,器件地址的前7比特，最后的三个比特位已经更新，该函数内部需将该地址左
//               移一位，并修改最后的读写比特位，最后一比特写0，表示写，最后一比特写
//               1,表示读;
//      mem_addr,存储器内部地址,该函数需发送低maddr_len字节到总线
//      maddr_len,存储器内部地址的长度，字节单位；
//      len,接收的数据总量，接收数据的倒数第一字节，即count-1，停止产生ACK信号，当接
//          收的字节数为count时，产生停止时序，并释放信号量iic_semp;
//      iic_semp,读完成时，驱动需释放的信号量（缓冲区信号量）
// 返回: TRUE，启动读时序成功，FALSE失败
// =============================================================================
static bool_t __IIC_GenerateReadStart( volatile tagI2CReg *Reg,
                                       u8 dev_addr,
                                       u32 mem_addr,
                                       u8 maddr_len,
                                       u32 length,
                                       struct SemaphoreLCB *iic_semp)
{
    struct IIC_IntParamSet *IntParam;
    u32 i,temp,timeout = CN_IIC_TIMEOUT;

    if((u32)Reg != CN_IIC_REGISTER_BADDR0)
        return false;

    IntParam = &IntParamset0;
    IntParam->TransTotalLen = length;
    IntParam->TransCount = 0;
    IntParam->pDrvPostSemp = iic_semp;

    //说明：此芯片IIC控制器，先将数据填入到CDS，启动发送便发送数据
    Int_CutLine(cn_int_line_iic);
    __IIC_IntEnable(Reg);           //此芯片必须开中断，才能判断是否发送接收完

    while((Reg->IICSTAT & IICSTAT_BSY_MASK) &&(timeout > 0))    //check if busy
    {
        DJY_EventDelay(100);
        timeout -= 100;
    }
    if(timeout == 0)
    {
        return false;
    }
    Reg->IICCON |= IICCON_ACKEN_MASK;   //此芯片必须开ACKEN位，否则会出问题
    Reg->IICSTAT = IICSTAT_MS_MT_MODE | IICSTAT_TXRXEN_MASK;        //发送主模式
    Reg->IICDS = (dev_addr<<CN_IIC_ADDR_OFFSET) | CN_IIC_WRITE_BIT; //发设备地址

    Reg->IICSTAT |= IICSTAT_START_STOP_MASK;                    //启动发送
    if(! __IIC_AckReceived(Reg))
    {
        __IIC_GenerateStop(Reg);
        return false;
    }

    __IIC_IntPendingBitClear(Reg);
    for(i = 0; i < maddr_len; i ++)
    {
        Reg->IICDS = (u8) (mem_addr >> ((maddr_len - i - 1) *8));
//      __IIC_IntPendingBitClear(Reg);
        if(! __IIC_AckReceived(Reg))
        {
            __IIC_GenerateStop(Reg);
            return false;
        }
    }
    //产生Restart
    Reg->IICDS = (dev_addr<<CN_IIC_ADDR_OFFSET) | CN_IIC_READ_BIT;  //发设备地址
    Reg->IICSTAT = IICSTAT_MS_MR_MODE | IICSTAT_TXRXEN_MASK
                        | IICSTAT_START_STOP_MASK;              //配置为发送主模式
    __IIC_IntPendingBitClear(Reg);
    if(! __IIC_AckReceived(Reg))
    {
        __IIC_GenerateStop(Reg);
        return false;
    }

//    temp = Reg->IICSTAT;              //此处不可省，相当于清状态
    temp = Reg->IICDS;
    temp = Reg->IICCON;
    temp = Reg->IICSTAT;
    __IIC_IntPendingBitClear(Reg);
    Int_ContactLine(cn_int_line_iic);

    return true;
}

// =============================================================================
// 功能: 结束本次读写回调函数，区分读写的不同停止时序，当属于发送时，则直接停止时序，
//      若为读，则先停止回复ACK，再停止
// 参数: specific_flag,个性标记，本模块内即IIC寄存器基址
// 返回: 无
// =============================================================================
static void __IIC_GenerateEnd(volatile tagI2CReg *Reg)
{
    if((u32)Reg != CN_IIC_REGISTER_BADDR0)
        return;

    __IIC_IntDisable(Reg);
    DJY_EventDelay(100);
    __IIC_GenerateStop(Reg);
}

// =============================================================================
// 功能：IIC总线控制回调函数，被上层调用，目前只实现对IIC时钟频率配置
// 参数：specific_flag,个性标记，本模块内即IIC寄存器基址
//       cmd,命令
//       data1,data2,数据，与具体命令相关
// 返回：无
// =============================================================================
static s32 __IIC_BusCtrl(volatile tagI2CReg *Reg,u32 cmd,u32 data1,u32 data2)
{
    if((u32)Reg != CN_IIC_REGISTER_BADDR0)
        return 0;

    switch(cmd)
    {
    case CN_IIC_SET_CLK:
        __IIC_SetClk(Reg,data1);
        break;
    default:
        break;
    }
    return 1;
}

// =============================================================================
// 功能：IIC接收与发送中断服务函数。该函数实现的功能如下：
//       1.每发送与接收一个或若干字节发生一次中断；
//       2.若有多个中断使用同一个中断号，则需根据具体情况区分使用的是哪个中断；
//       3.清中断标志，并判断ACK信号，每读写字节，计数器都需相应修改；
//       4.接收达到倒数第一个字节时，需配置不发送ACK信号；
//       5.接收或发送完成时，需post信号量IntParam->pDrvPostSemp；
//       6.接收或发送完成时，需产生停止时序。
// 参数：i2c_int_line,中断号，本函数没用到
// 返回：无意义
// =============================================================================
static u32 __IIC_ISR(ufast_t IntLine)
{
    static struct IIC_CB *ICB;
    static struct IIC_IntParamSet *IntParam;
    tagI2CReg *reg;

    u8 ch;
    if(IntLine != cn_int_line_iic)
        return 0;
    ICB = s_tIIC0_CB;
    IntParam = &IntParamset0;
    reg = (tagI2CReg *)CN_IIC_REGISTER_BADDR0;

    if(reg->IICCON & IICCON_INTPENDINGFLAG_MASK)        //是否有中断标志
    {
        if((reg->IICSTAT & IICSTAT_MS_MT_MODE) ==  IICSTAT_MS_MT_MODE)//发送
        {
            if(!(reg->IICSTAT & IICSTAT_ACK_MASK))      //是否收到ACK信号
            {
                if(IIC_PortRead(ICB,&ch,1))
                {
                    reg->IICDS = ch;
                    IntParam->TransCount ++;
                }
                else if(IntParam->TransCount == IntParam->TransTotalLen)
                {
                    __IIC_IntDisable(reg);
                    __IIC_IntPendingBitClear(reg);
                    __IIC_GenerateStop(reg);
                    Lock_SempPost(IntParam->pDrvPostSemp);
                }
            }
        }
        else if((reg->IICSTAT & IICSTAT_MS_MR_MODE) ==  IICSTAT_MS_MR_MODE)
        {
            ch = reg->IICDS;
            if(IIC_PortWrite(ICB,&ch,1))
                IntParam->TransCount ++;
            if(IntParam->TransCount == IntParam->TransTotalLen - 1)
            {
                reg->IICCON &= ~(IICCON_ACKEN_MASK);    //倒数第二个数，不发ACK
            }
            else if(IntParam->TransCount == IntParam->TransTotalLen)
            {
                __IIC_IntPendingBitClear(reg);
                __IIC_IntDisable(reg);
                __IIC_GenerateStop(reg);
                Lock_SempPost(IntParam->pDrvPostSemp);
            }
        }
    }
    else
    {
        //....
    }
    __IIC_IntPendingBitClear(reg);
    return 0;
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
bool_t IIC_Init(void)
{
    struct IIC_Param IIC0_Config;

    IIC0_Config.BusName            = "IIC0";
    IIC0_Config.IICBuf             = (u8*)&s_IIC0Buf;
    IIC0_Config.IICBufLen          = CFG_IIC0_BUF_LEN;
    IIC0_Config.SpecificFlag       = CN_IIC_REGISTER_BADDR0;
    IIC0_Config.pWriteReadPoll      = NULL;
    IIC0_Config.pGenerateWriteStart = (WriteStartFunc)__IIC_GenerateWriteStart;
    IIC0_Config.pGenerateReadStart  = (ReadStartFunc)__IIC_GenerateReadStart;
    IIC0_Config.pGenerateEnd        = (GenerateEndFunc)__IIC_GenerateEnd;
    IIC0_Config.pBusCtrl            = (IICBusCtrlFunc)__IIC_BusCtrl;

    __IIC_HardConfig(CN_IIC_REGISTER_BADDR0,0);
    __IIC_IntConfig(cn_int_line_iic,__IIC_ISR);

    if(s_ptIIC0_CB = IIC_BusAdd(&IIC0_Config))
        return 1;
    else
        return 0;
}

