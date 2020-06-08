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
// 这份授权条款，在使用者符合以下二条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、此三条件表列，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、此三条件表列，以及下述
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

// 文件名     ：cpu_peri_spi.c
// 模块描述: SPI模块底层硬件驱动模块
// 模块版本: V1.00
// 创建人员: huzb
// 创建时间: 2/14/2018
// =============================================================================


#include "string.h"
#include "djyos.h"
#include "cpu_peri.h"
#include "int.h"
#include "spibus.h"
#include "math.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_SPI(u8 port);
//    #if CFG_SPI1_ENABLE==1
//    ModuleInstall_SPI(CN_SPI1);
//    #endif
//
//    #if CFG_SPI2_ENABLE==1
//    ModuleInstall_SPI(CN_SPI2);
//    #endif
//
//    #if CFG_SPI3_ENABLE==1
//    ModuleInstall_SPI(CN_SPI3);
//    #endif
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip spi"//SPI总线驱动
//parent:"spi bus"   //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"spi bus","int","lock"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_SPI == false )
//#warning  " cpu_onchip_spi  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_SPI    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,32,512,
#define CFG_SPI_BUF_LEN             128                //"SPI缓冲区大小",
#define CFG_SPI_DMA_BUF_LEN         128                //"SPI DMA缓冲区大小",
//%$#@enum,true,false,
#define CFG_SPI1_ENABLE             false              //"是否配置使用SPI1",
#define CFG_SPI2_ENABLE             false              //"是否配置使用SPI2",
#define CFG_SPI3_ENABLE             false              //"是否配置使用SPI3",
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure
// =============================================================================

//本驱动只考虑Cpu 当主机的情况

typedef struct
{
    volatile u32 SPCR0;
    volatile u32 SPCR1;
    volatile u32 SPCR2;
    volatile u32 SPCR3;
    volatile u32 SPSR;
    volatile u32 SPWAR;
    volatile u32 SPDR;
    volatile u32 SPNUMR;
    volatile u32 SPINTSR;
    volatile u32 SPI_SCKDIV;
}tagSpiReg;

#define CN_SPI1_BASE     (0x40600000UL)
#define CN_SPI2_BASE     (0x40610000UL)
#define CN_SPI3_BASE     (0x41620000UL)
#define CN_SPI4_BASE     (0x41630000UL)

static volatile tagSpiReg *sgpt_SpiReg[] = {

        (volatile tagSpiReg *)CN_SPI1_BASE,
        (volatile tagSpiReg *)CN_SPI2_BASE,
        (volatile tagSpiReg *)CN_SPI3_BASE,
        (volatile tagSpiReg *)CN_SPI4_BASE,
};

#define CN_TIMEOUT  (20*1000)

//定义中断中需使用的静态量结构体
struct SPI_IntParamSet
{
    u32 SendDataLen;
    u32 RecvDataLen;
    u32 RecvOffset;
};


// =============================================================================
//定义SPI控制块和中断静态量
#define CN_SPI_BUF_LEN              128
#define CN_SPI_DMA_BUF_LEN          128
static struct SPI_CB *sgpt_Spi1_CB;
static u8 s_Spi1_Buf[CN_SPI_BUF_LEN];
struct SPI_IntParamSet IntParamset1;

static struct SPI_CB *sgpt_Spi2_CB;
static u8 s_Spi2_Buf[CN_SPI_BUF_LEN];
struct SPI_IntParamSet IntParamset2;

static struct SPI_CB *sgpt_Spi3_CB;
static u8 s_Spi3_Buf[CN_SPI_BUF_LEN];
struct SPI_IntParamSet IntParamset3;

static struct SPI_CB *sgpt_Spi4_CB;
static u8 s_Spi4_Buf[CN_SPI_BUF_LEN];
struct SPI_IntParamSet IntParamset4;


// =============================================================================
// 功能: 中断使能和失能函数
// 参数: tpSPI,被操作的寄存器组指针
// 返回: 无
// =============================================================================
static void __SPI_IntEnable(volatile tagSpiReg *Reg,u32 IntSrc)
{
    Reg->SPCR0 |= IntSrc;
}
static void __SPI_IntDisable(volatile tagSpiReg *Reg,u32 IntSrc)
{
    Reg->SPCR0 &= ~IntSrc;
}

// =============================================================================
// 功能: 根据APB2的时钟和Fre计算BR寄存器的写入值
// 参数: Fre,想要配置的SPI时钟
// 返回: BR值
// =============================================================================

#define CN_SYS_SPI_FRQ (37500000)

static u8 __SPI_BrCal(u32 Fre)
{
    u8 tempDiv;
    u8 loop;

    tempDiv = CN_SYS_SPI_FRQ/Fre;

    //Spi的分频系数-只接收偶数
    //判断分频系数是否合法
    loop = 1;
    while(loop <= 254/2)
    {
        if(loop*2 == tempDiv)
        {
            break;
        }
        loop++;
    }


    return tempDiv;
}

// =============================================================================
// 功能：SPI时钟配置函数，时钟来源为50M，经SCR和CPSR分频得到时钟，时钟计算公式为：
//       SCK =  PCLK / (CPSDVSR *[SCR+1])
// 参数：spi_dev，设备句柄
//      spiclk，欲配置的时钟速度，单位为Hz
// 返回：true=成功，false=失败
// 说明：此驱动固定SCR = 1;而根据手册，CPSDVSR必须为2-254的偶数，因此，频率范围为
//       12.5M ~ 100kHz
// =============================================================================
static void __SPI_SetClk(volatile tagSpiReg *Reg,u32 Fre)
{
    u8 div;
    div = (u8)__SPI_BrCal(Fre);
    Reg->SPI_SCKDIV = 0;
    Reg->SPI_SCKDIV = div;
}

// =============================================================================
// 功能：SPI控制寄存器参数配置，如PHA和CPOL、时钟等，根据各种寄存器而异
// 参数：tpSPI,SPI控制器基址
//       ptr,参数指针
// 返回：无
// =============================================================================

#define CN_MODE0 (0)
#define CN_MODE1 (1)
#define CN_MODE2 (2)
#define CN_MODE3 (3)
static bool_t __SPI_BusCsActive(tagSpiReg *Reg, u8 cs);
static void __SPI_Config(volatile tagSpiReg *Reg,tagSpiConfig *ptr)
{
     bool_t Spi_CHPA = false;
     bool_t Spi_CPOL = false;
     u32 SpiPin;

     SpiPin = (u32)Reg;

    __SPI_SetClk(Reg,ptr->Freq);

    //四种模式 空闲时高低电平 哪个边沿收发数据
    //Mode 0 CPOL = 0 CPHA =0
    //Mode 1 CPOL = 0 CPHA =1
    //Mode 2 CPOL = 1 CPHA =0
    //Mode 2 CPOL = 1 CPHA =1
    //在使用的过程中可能被服用做其他功能，这里进行重新初始化
    switch(SpiPin)
    {
        case 0x41630000:          //SPI3
            SPI3_InitPad(DISABLE);
            break;
        case 0x41620000:          //SPI2
            SPI2_InitPad();
            break;
        case 0x41610000:          //SPI1
            SPI1_InitPad();
            break;
        case 0x41600000:          //SPI0
            SPI0_InitPad();
            break;
        default:;break;
    }

    //set the PHA
    //判断哪种模式

    switch(ptr->Mode)
    {
        case CN_MODE0:                  //Model 0 00
             Reg->SPCR1 &= ~(0x3<<1);
             Reg->SPCR1 |= (0x0<<1);
             break;
        case CN_MODE1:                  //Model 1 01
             Reg->SPCR1 &= ~(0x3<<1);
             Reg->SPCR1 |= (0x1<<1);
             break;
        case CN_MODE2:                  //Model 2 10
             Reg->SPCR1 &= ~(0x3<<1);
             Reg->SPCR1 |= (0x2<<1);
             break;
        case CN_MODE3:                  //Model 3 11
             Reg->SPCR1 &= ~(0x3<<1);
             Reg->SPCR1 |= (0x3<<1);
             break;
        default:;break;
    }


//    Reg->SPCR3 &= ~(1<<0);
    //Spi 使能 启动自动传输模式
//    Reg->SPCR3 |= (1<<1) | (1<<4);

}

// =============================================================================
// 功能: SPI默认硬件初始化配置，主要是时钟配置和GPIO写保护引脚配置
// 参数: RegBaseAddr,寄存器基址
// 返回: 无
// =============================================================================
static void __SPI_HardConfig(u32 BaseAddr)
{
    tagSpiReg *Reg;
    u32 temp = 0;

    Reg = (tagSpiReg *)BaseAddr;
    //配置SPI使用GPIO引脚
    //复位寄存器
    SPI_Reset_Reg(Reg);

    //设置为主模式
    Reg->SPCR1 |= (1<<0);
    //默认模式配置1
    Reg->SPCR1 |= (1<<1);

    //收发使能
    Reg->SPCR1 |= (1<<6)|(1<<7);

    //门限值设置
    Reg->SPCR2 |= 8<<4;
    Reg->SPCR2 |= 0<<0;

    //默认设置10分频
    Reg->SPI_SCKDIV = 10;

    //主模式下数据计数失能
    Reg->SPCR3 &= ~(1<<5);

    //主模式SSN使能
    Reg->SPCR3 |= (1<<0);

    //数据连续传输无间隔
    Reg->SPWAR = 0;

    //启动SPI
    Reg->SPCR3 |= (1<<4);

}

// =============================================================================
// 功能：SPI片选使能，使片选有效,若控制器有具体的配置片选寄存器，可直接返回
// 参数：Reg，本模块内即SPI寄存器基址
//       cs,片选线
// 返回：无
// 说明：SPI控制器上只引出了一个CS，若需要接多个CS，则必须应用程序自己控制CS
// =============================================================================
extern bool_t Board_SpiCsCtrl(u8 SPIPort,u8 cs,u8 level);
static bool_t __SPI_BusCsActive(tagSpiReg *Reg, u8 cs)
{
    u8 SPIPort;
    for(SPIPort = 0; SPIPort < CN_SPI_NUM; SPIPort++)
    {
        if(Reg ==  sgpt_SpiReg[SPIPort])
            break;
    }

    //设立使用spi的SSN信号控制，而非其他的IO引脚,用于主模式的片选信号
    //return Board_SpiCsCtrl(SPIPort,cs,0);
    Reg->SPCR3 &= ~(1<<0);

    Reg->SPCR3 |= (1<<1) | (1<<4);

    return true;
}

// =============================================================================
// 功能：SPI片选失能，使片选无效
// 参数：Reg，本模块内即SPI寄存器基址
//       cs,片选线
// 返回：无
// =============================================================================
static bool_t __SPI_BusCsInActive(tagSpiReg *Reg, u8 cs)
{
    u8 SPIPort;
    for(SPIPort = 0; SPIPort < CN_SPI_NUM; SPIPort++)
    {
        if(Reg ==  sgpt_SpiReg[SPIPort])
            break;
    }
    //使能片选
    Reg->SPCR3 |= (1<<0);
    //同时失能主动发送
    Reg->SPCR3 &= ~((1<<1) | (1<<4));

    return true;
}

// =============================================================================
// 功能：SPI总线控制回调函数，被上层调用，目前只实现对SPI时钟频率配置
// 参数：specific_flag,个性标记，本模块内即IIC寄存器基址
//       cmd,命令
//       data1,data2,数据，与具体命令相关
// 返回：无
// =============================================================================
static s32 __SPI_BusCtrl(tagSpiReg *Reg,u32 cmd,ptu32_t data1,ptu32_t data2)
{
    s32 result = 1;
    if( (Reg != sgpt_SpiReg[CN_SPI1]) && (Reg != sgpt_SpiReg[CN_SPI2])
            && (Reg != sgpt_SpiReg[CN_SPI3]) &&  (Reg != sgpt_SpiReg[CN_SPI4]))
        return 0;

    switch(cmd)
    {
    case CN_SPI_SET_CLK:
        __SPI_SetClk(Reg,data1);
        break;
    case CN_SPI_CS_CONFIG:
        __SPI_Config(Reg,(tagSpiConfig *)data1);
        break;
    case CN_SPI_SET_AUTO_CS_EN:
        // 硬件上如果有自动产生CS的配置
        break;
    case CN_SPI_SET_AUTO_CS_DIS:
        break;
    default:
        break;
    }
    return result;
}

// =============================================================================
// 功能：读SPI接收寄存器，轮询方式,被__SPI_TxRxPoll调用
// 参数：Reg,寄存器基址
// 返回：读到的数据
// =============================================================================
static s32 __SPI_Read( tagSpiReg * Reg )
{
    u32 time = CN_TIMEOUT;

//    Reg->SPDR = 0xff;

    //判断接收缓存是否有数据可读
    while (((Reg->SPSR & 0xF) == 0 ) && (time > 0))
    {
        time --;
        DJY_DelayUs(1);
    }

    if(time == 0)
    {
        printk("Rev TimeOut\r\n");
        return -1;
    }

    return Reg->SPDR;
}

// =============================================================================
// 功能：写SPI发送寄存器，轮询方式,被__SPI_TxRxPoll调用
// 参数：Reg,寄存器基址；wData，写数据
// 返回：无
// =============================================================================
static s32 __SPI_Write( tagSpiReg * Reg,u8 wData )
{
    u32 time = CN_TIMEOUT;

    /* Send data */

    //默认初始化中发送FIFO门限值设置为8个,此时当Fifo中满8个时候还没有发出去那么,fifo不能再进去写了

    while ((((Reg->SPSR & 0xf0)>>4) == 8 ) && (time > 0) )
    {
        time --;
        DJY_DelayUs(1);
    }

    if(time == 0)
        return -1;

    Reg->SPDR = wData;

    return 0;
}

// =============================================================================
// 功能：轮询方式读写数据
// 参数：Reg,寄存器基址，srcAddr,数据源指针；wrSize，写数据量，destAddr，读到的数据
//       地址指针；rdSize，读数据量，recvoff,读偏移，暂未用
// 返回：true,正确；false,错误
// =============================================================================

static bool_t __SPI_TxRxPoll(tagSpiReg *Reg,u8 *srcAddr,u32 wrSize,
        u8 *destAddr, u32 rdSize,u32 recvoff,u8 cs)
{
    u32 i,len_limit;

    if( (!srcAddr) || ((rdSize != 0) && (!destAddr)))
        return false;

    Reg->SPCR3 |= (1<<4);

    len_limit = MAX(wrSize, rdSize + recvoff);

    //spi 通信一发一收
    for (i=0;i<len_limit;i++)
    {
        if (i<wrSize)
        {
            __SPI_Write(Reg,srcAddr[i]);
        }
        else if((i>=wrSize)&&(i<len_limit))
        {
          __SPI_Write(Reg,0);         //这里配合接收数据,发送无意义数据产生时钟接收
        }

        if((destAddr) && (i>=recvoff) )
        {
            destAddr[i-recvoff] = __SPI_Read(Reg);//Read Data We Need
        }
        else
        {
            __SPI_Read(Reg); //当写过程中，同时会收到数据 非需要数据 配合发送
        }
    }

    return true;
}

// =============================================================================
// 功能：SPI底层驱动发送接收函数，本函数完成的功能如下：
//       1.保存结构静态变量，用于中断中使用；
//       2.配置相关的寄存器，使其处于发送接收数据状态;
//       3.开启中断，在中断服务函数中完成数据收发。
// 参数：specific_flag,个性标记，本模块内即IIC寄存器基址
//       sendlen,发送数据长度，字节
//       recvlen,发送数据长度，字节
//       recvoff,接收数据依稀，即从接收到的第几个字节开始保存数据，字节
//       block_opt,阻塞选项，true时，为阻塞，非阻塞方式需底层驱动释放信号量和拉高CS
//       spi_semp,底层可能需要释放的信号量，是否释放由block_opt决定
// 返回：true,无错误;false,失败
// =============================================================================
static bool_t __SPI_TransferTxRx(tagSpiReg *Reg,u32 sendlen,u32 recvlen,
                                u32 recvoff)
{
    struct SPI_IntParamSet *Param=NULL;

    if(Reg == sgpt_SpiReg[CN_SPI1])
        Param = &IntParamset1;
    else if(Reg == sgpt_SpiReg[CN_SPI2])
        Param = &IntParamset2;
    else if(Reg == sgpt_SpiReg[CN_SPI3])
        Param = &IntParamset3;
    else
        Param = &IntParamset4;


    Param->SendDataLen = sendlen;
    Param->RecvDataLen = recvlen;
    Param->RecvOffset  = recvoff;

    //Spi 使能
    Reg->SPCR3 |= (1<<4);

    //发送空中断使能
    __SPI_IntEnable(Reg,(1<<5));


    return true;
}
// =============================================================================
// 功能：SPI接收与发送中断服务函数。该函数实现的功能如下：
//       1.由于收发一体，因此发送空中断中也收到了数据；
//       2.SPI_PortRead()中读不到数据时，说明发送的数据已经完成，可关发送中断
//       3.若param->RecvDataLen大于0，即需要接收数据，则从接收到数据领衔offset字节
//         开始存储数据，即调用SPI_PortRead()
//       4.
// 参数：spi_int_line,中断号，本函数没用到
// 返回：无意义
// =============================================================================
u32 SPI_ISR(ptu32_t IntLine)
{
    struct SPI_CB *pSCB=NULL;
    struct SPI_IntParamSet *param;
    tagSpiReg *Reg;
    u32 spi_sr,spi_cr2;
    u8 ch,temp;
    static u8 firstrcv = 0;

    if(IntLine == CN_INT_LINE_SPI0)
    {
        pSCB = sgpt_Spi1_CB;
        Reg = (tagSpiReg *)sgpt_SpiReg[CN_SPI1];
        param = &IntParamset1;
    }
    else if(IntLine == CN_INT_LINE_SPI1)
    {
        pSCB = sgpt_Spi2_CB;
        Reg = (tagSpiReg *)sgpt_SpiReg[CN_SPI2];
        param = &IntParamset2;
    }
    else if(IntLine == CN_INT_LINE_SPI2)
    {
        pSCB = sgpt_Spi3_CB;
        Reg = (tagSpiReg *)sgpt_SpiReg[CN_SPI3];
        param = &IntParamset3;
    }
    else if(IntLine == CN_INT_LINE_SPI3)
    {
        pSCB = sgpt_Spi4_CB;
        Reg = (tagSpiReg *)sgpt_SpiReg[CN_SPI4];
        param = &IntParamset4;
    }
    else
        return 0;

    //发送空中断
    if((Reg->SPCR0 & (1<<5)) && (Reg->SPINTSR & (1<<5)))
    {
        temp = SPI_PortRead(pSCB,&ch,1);//是否还有数据需要发送

        if(temp >0)
        {
            param->SendDataLen --;
            param->RecvOffset  --;
            Reg->SPDR = ch;
        }
        else                                            //表明发送结束
        {
            //关发送中断
            __SPI_IntDisable(Reg,(1<<5));
            param->RecvOffset -= param->SendDataLen;
            param->SendDataLen = 0;

            ch = Reg->SPDR;

            //发送完成了，判断是否需要接收
            if(param->RecvDataLen > 0)
            {
                //MASTER模式下，发送和接收同时进行
                Reg->SPDR = 0xAA;
                //使能接收非空中断
                __SPI_IntEnable(Reg,(1<<2));    //使能接收中断
                firstrcv = 1;
            }
        }

    }

    //接收非空中断
    else if((Reg->SPCR0 & (1<<2)) && (Reg->SPINTSR  & (1<<2)))    //接收中断
    {
        ch = Reg->SPDR;
        //计算接收了多少数据
        if(firstrcv != 1)
        {
            SPI_PortWrite(pSCB,&ch,1);
            param->RecvDataLen -= 1;
        }
        else
            firstrcv = 0;

        //判断是否还需接收数据
        if(param->RecvDataLen > 0)
        {
            Reg->SPDR = 0xAA;
        }
        else                    //表明接收已经完成
        {
            param->RecvDataLen = 0;
            __SPI_IntDisable(Reg,(1<<2));
        }
    }

    if(param->RecvDataLen + param->SendDataLen == 0)
    {
        //spi 使能
        Reg->SPCR0 &= (1<<4);
    }

    return 0;
}

// =============================================================================
// 功能: SPI中断配置函数
// 参数: IntLine,中断线
// 返回: 无
// =============================================================================
static void __SPI_IntConfig(u8 IntLine)
{
    //中断线的初始化
    Int_Register(IntLine);
    Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(IntLine,SPI_ISR);
    Int_SettoAsynSignal(IntLine);
    Int_ClearLine(IntLine);
    Int_RestoreAsynLine(IntLine);
}

// =============================================================================
// 功能：SPI底层驱动的初始化，完成整个SPI总线的初始化，其主要工作如下：
//       1.初始化总线控制块SPI_CB，回调函数和缓冲区的初始化赋值；
//       2.默认的硬件初始化，如GPIO或SPI寄存器等；
//       3.中断初始化，完成读写中断配置；
//       4.调用SPIBusAdd或SPIBusAdd_r增加总线结点；
// 参数：无
// 返回：初始化成功返回1，失败返回0
// =============================================================================
bool_t SPI_Initialize(u8 port)
{
    struct SPI_Param SPI_Config;
    struct SPI_CB **pSpiCB = NULL;
    u8 IntLine;

    switch(port)
    {
    case CN_SPI1:
        SPI_Config.BusName          = "SPI1";
        SPI_Config.SPIBuf           = (u8*)&s_Spi1_Buf;
        IntLine = CN_INT_LINE_SPI0;
        pSpiCB = &sgpt_Spi1_CB;
        break;
    case CN_SPI2:
        SPI_Config.BusName          = "SPI2";
        SPI_Config.SPIBuf           = (u8*)&s_Spi2_Buf;
        IntLine = CN_INT_LINE_SPI1;
        pSpiCB = &sgpt_Spi2_CB;
        break;
    case CN_SPI3:
        SPI_Config.BusName          = "SPI3";
        SPI_Config.SPIBuf           = (u8*)&s_Spi3_Buf;
        IntLine = CN_INT_LINE_SPI2;
        pSpiCB = &sgpt_Spi3_CB;
        break;
    case CN_SPI4:
        SPI_Config.BusName          = "SPI4";
        SPI_Config.SPIBuf           = (u8*)&s_Spi4_Buf;
        IntLine = CN_INT_LINE_SPI3;
        pSpiCB = &sgpt_Spi4_CB;
        break;

    default:
        return (0);
    }
    SPI_Config.SpecificFlag     = (ptu32_t)sgpt_SpiReg[port];
    SPI_Config.SPIBufLen        = CN_SPI_BUF_LEN;
    SPI_Config.MultiCSRegFlag   = false;
    SPI_Config.pTransferPoll    = (TransferPoll)__SPI_TxRxPoll;
    SPI_Config.pTransferTxRx    = (TransferFunc)__SPI_TransferTxRx;
    SPI_Config.pCsActive        = (CsActiveFunc)__SPI_BusCsActive;
    SPI_Config.pCsInActive      = (CsInActiveFunc)__SPI_BusCsInActive;
    SPI_Config.pBusCtrl         = (SPIBusCtrlFunc)__SPI_BusCtrl;

    *pSpiCB = SPI_BusAdd(&SPI_Config);
    if(*pSpiCB == NULL)
    {
        return (0); // 失败
    }
    __SPI_HardConfig((u32)SPI_Config.SpecificFlag); // SPI寄存器设置

    __SPI_IntConfig(IntLine); // 注册中断

    return (1); // 成功
}
//-----------------------------------------------------------------------------
//功能: 安装SPI端口
//参数: SPI端口号
//返回: -1 -- 失败; 0 -- 成功;
//备注:
//-----------------------------------------------------------------------------
s32 ModuleInstall_SPIPort(u8 Port)
{
    if(CN_SPI_NUM < Port) // 板件只支持到SPI1
        return (-1);
    if(0 == SPI_Initialize(Port))
        return (-1);
    return (0);
}




