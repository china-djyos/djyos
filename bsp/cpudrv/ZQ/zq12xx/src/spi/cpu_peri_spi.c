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
// 模块版本: V1spi
// 创建人员:
// 创建时间:
// =============================================================================

#include "djyos.h"
#include "int.h"
#include "cpu_peri.h"
#include "spibus.h"

// =============================================================================
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。
//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    bool_t SPI_Init(u8 SPI_port);
//    #if CFG_SPI1_ENABLE==1
//    ModuleInstall_SPI(CN_SPI1);
//    #endif
//    #if CFG_SPI2_ENABLE==1
//    ModuleInstall_SPI(CN_SPI2)
//    #endif
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip spi"//SPI总线驱动
//parent:"spi bus"   //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"spi bus","cpu onchip gpio"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_SPI == false )
//#warning  " cpu_onchip_spi  组件参数未配置，使用默认配置"
//%$#@target = header   //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_SPI    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,32,512,
#define CFG_SPI_BUF_LEN               128       //"SPI缓冲区大小",
//%$#@enum,true,false,
#define CFG_SPI1_ENABLE               false     //"是否使用SPI1",
#define CFG_SPI2_ENABLE               false     //"是否使用SPI2",
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束
//@#$%component end configure

// =============================================================================


// =============================================================================
#if(0)
typedef struct _SPI_CTRL_
{
    vu32 SPISR;
    vu32 SPICR;
    vu32 SPISSR;
    vu32 REV0;
    vu32 SPITR;
    vu32 SPIRR;
    vu32 SPITR1;
    vu32 SPIRR1;
}tagSpiReg;

#define CN_SPI1_BASE        (0x41040000+0x80)
#define CN_SPI2_BASE        (0x410A0000+0x80)
#define CN_FIFO_LEN         (32)

//定义中断中需使用的静态量结构体
struct SPI_IntParamSet
{
//  struct semaphore_LCB *pDrvPostSemp; //信号量
    u32 SendDataLen;
    u32 RecvDataLen;
    u32 RecvOffset;
//  u8 CurrentCS;
//  u8 block_option;
};
// =============================================================================
//定义SPI控制块和中断静态量
#define CN_SPI1_BUF_LEN      128
struct SPI_IntParamSet IntParamset1;

#define CN_SPI2_BUF_LEN      128
struct SPI_IntParamSet IntParamset2;

static struct SPI_CB *spSpiCb[CN_SPI_NUM];

#define SPI_INTR_EN_MCU4    (0x42030000 + 0x610)
#define SPI_INTR_STS_MCU4   (0x42030000 + 0x510)
// =============================================================================
__attribute__((weak)) bool_t Board_SpiCsCtrl(u8 port,u8 cs,u8 en)
{
    return true;    //accomplished in board.c if needed
}
// =============================================================================
// 功能: 中断使能和失能函数
// 参数: tpSPI,被操作的寄存器组指针
// 返回: 无
// =============================================================================
static void __Spi_IntEnable(volatile tagSpiReg *Reg)
{
    Reg->SPICR |= (1<<6);
}
static void __Spi_IntDisable(volatile tagSpiReg *Reg)
{
    Reg->SPICR &= ~(1<<6);
}

// =============================================================================
// 功能：SPI时钟配置函数，时钟来源为25M，经SCR和CPSR分频得到时钟，时钟计算公式为：
//       SCK =  PCLK / (CPSDVSR *[SCR+1])
// 参数：spi_dev，设备句柄
//      spiclk，欲配置的时钟速度，单位为Hz
// 返回：true=成功，false=失败
// 说明：此驱动固定SCR = 1;而根据手册，CPSDVSR必须为2-254的偶数，因此，频率范围为
//       12.5M ~ 100kHz
// =============================================================================
static void __Spi_SetClk(volatile tagSpiReg *tpSPI,u32 spisck)
{
       u32 temp,i;
        if(tpSPI == NULL)
            return;

    switch(spisck)
    {
        //系统主频下来80M 进行分频
         case CN_SPI_SPEEK_40M:                  //2分频 000
             tpSPI->SPICR &= ~((3<<3) | (1<<8));//先清0,系统频率二分频直接设置清0设置000
             break;

         case CN_SPI_SPEEK_20M:                  //4分频 010
             tpSPI->SPICR &= ~((3<<3) | (1<<8)); //先清0
             tpSPI->SPICR |= 0x2 << 3;
             break;
         case CN_SPI_SPEEK_10M:              //8分频 011
             tpSPI->SPICR &= ~((3<<3) | (1<<8)); //先清0
             tpSPI->SPICR |= 0x3 << 3;
             break;

         case CN_SPI_SPEEK_5M:                  //16分频 100
             tpSPI->SPICR &= ~((3<<3) | (1<<8)); //先清0
             tpSPI->SPICR |= 0x1 << 8;
             break;

         case CN_SPI_SPEEK_2_5M:                //32分频 101
             tpSPI->SPICR &= ~((3<<3) | (1<<8)); //先清0
             tpSPI->SPICR |= (0x1 << 8) | (0x1 << 3) ;
             break;
         case CN_SPI_SPEEK_13_33M:              //6分频  110
             tpSPI->SPICR &= ~((3<<3) | (1<<8)); //先清0
             tpSPI->SPICR |=(0x1 << 8)|(0x2 <<3);
             break;

         case CN_SPI_SPEEK_6_67M:              //12 分频 111
             tpSPI->SPICR &= ~((3<<3) | (1<<8)); //先清0
             tpSPI->SPICR |=(0x1 << 8)|(0x3 <<3);
             break;
         default:
             printk("spisck arg set err\r\n");
             tpSPI->SPICR &= ~((3<<3) | (1<<8));//先清0,系统频率二分频直接设置清0设置000
             break;
    }
}

// =============================================================================
// 功能：SPI控制寄存器参数配置，如PHA和CPOL、时钟等，根据各种寄存器而异
// 参数：tpSPI,SPI控制器基址
//       ptr,参数指针
// 返回：无
// =============================================================================
static void __Spi_Config(volatile tagSpiReg *Reg,tagSpiConfig *ptr)
{
    __Spi_SetClk(Reg,ptr->Freq);

    //set the PHA
    if(ptr->Mode & SPI_CPHA)
    {
        Reg->SPICR |= 1<<2;
    }
    else
    {
        Reg->SPICR &= ~(1<<2);
    }
    //set the cpol
    if(ptr->Mode & SPI_CPOL)
    {
        Reg->SPICR |= 1<<1;
    }
    else
    {
        Reg->SPICR &= ~(1<<1);
    }

    if(ptr->CharLen <= 32 && ptr->CharLen >= 4)
    {
        Reg->SPICR &= ~(0x3F<<15);
        Reg->SPICR |= ptr->CharLen;
    }
}

// =============================================================================
// 功能: SPI默认硬件初始化配置，主要是时钟配置和GPIO写保护引脚配置
// 参数: RegBaseAddr,寄存器基址
// 返回: 无
// =============================================================================
static void __Spi_HardDefaultSet(u8 port)
{
    tagSpiReg * Reg;
    if(port == CN_SPI1)
        Reg = (tagSpiReg *)CN_SPI1_BASE;
    else if(port == CN_SPI2)
        Reg = (tagSpiReg *)CN_SPI2_BASE;
    else
        return;

    Reg->SPICR      = 0;
    __Spi_SetClk(Reg,5000*000);             //默认设置为5M
    Reg->SPICR      |= (1<<27);             //FLUSH FIFO
    Reg->SPICR      |= (1<<28) | (1<<29);   //FIFO EN
    Reg->SPICR      |= (8<<15);             //8bits,others default
    Reg->SPICR      |= (7<<24);             //DISABLE CS
}

// =============================================================================
// 功能：SPI片选使能，使片选有效
// 参数：Reg，本模块内即SPI寄存器基址
//       cs,片选线
// 返回：无
// 说明：
// =============================================================================
static bool_t __Spi_BusCsActive(tagSpiReg *Reg, u8 cs)
{
    u8 port = CN_SPI1;

    if((u32)Reg != CN_SPI1_BASE)
    {
        port = CN_SPI2;
    }

    if(cs < 4)
    {
        Reg->SPICR |= (7<<24);
        Reg->SPICR &= ~(1<<cs);
    }

    Board_SpiCsCtrl(port,cs,1);

    return true;
}

// =============================================================================
// 功能：SPI片选失能，使片选无效
// 参数：Reg，本模块内即SPI寄存器基址
//       cs,片选线
// 返回：无
// =============================================================================
static bool_t __Spi_BusCsInActive(tagSpiReg *Reg, u8 cs)
{
    u8 port = CN_SPI1;

    if((u32)Reg != CN_SPI1_BASE)
    {
        port = CN_SPI2;
    }

    if(cs < 4)
    {
        Reg->SPICR |= (7<<24);
    }

    Board_SpiCsCtrl(port,cs,0);
    return true;
}

// =============================================================================
// 功能：SPI总线控制回调函数，被上层调用，目前只实现对SPI时钟频率配置
// 参数：Reg,个性标记，本模块内即IIC寄存器基址
//       cmd,命令
//       data1,data2,数据，与具体命令相关
// 返回：无
// =============================================================================
static s32 __Spi_BusCtrl(tagSpiReg *Reg,u32 cmd,ptu32_t data1,ptu32_t data2)
{
    s32 result = 1;
    if(Reg == NULL)
        return 0;

    switch(cmd)
    {
    case CN_SPI_SET_CLK:
        __Spi_SetClk(Reg,data1);
        break;
    case CN_SPI_CS_CONFIG:
        __Spi_Config(Reg,(tagSpiConfig *)data1);
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

static u32 __Spi_Write(tagSpiReg *Reg,u8 Data)
{
    while(!(Reg->SPISR & (1<<3)));          //xmit empty

    Reg->SPITR1 = Data;
    Reg->SPICR |= (1<<5);                   //start
    while(!(Reg->SPISR & (1<<7)))   ;       //done

    while(!(Reg->SPISR & (1<<2)))   ;       //rcv full

    return (u32)(Reg->SPIRR1);
}
// =============================================================================
// 功能：轮询方式spi读写数据，接收偏移此处作了简化处理，直接认为与sendlen相同，即发送
//       完后再接收数据
// 参数：Reg,个性标记，本模块内即IIC寄存器基址
//       srcaddr,发送数据源
//       sendlen,发送数据长度，字节单位
//       destaddr,接收数据缓冲区
//       recvlen,接收数据长度，字节单位
//       recvoff,接收依稀字节，很多时候为0
//       Byte,发送的字节
// 返回：true,成功;false,失败
// =============================================================================
static bool_t __Spi_TxRxPoll(tagSpiReg *Reg,u8* srcAddr,u32 wrSize,
                            u8* destAddr,u32 rdSize,u32 recvoff,u8 cs)
{
    u32 i,data,len_limit;

    if(wrSize + rdSize == 0)
        return false;
    len_limit = (wrSize > (rdSize + recvoff))? wrSize:(rdSize + recvoff);

    __Spi_IntDisable(Reg);
    Reg->SPICR |= (1<<7);               //ENABLE SPI
    while(!(Reg->SPISR & (1<<17)))     //RXFIFO NOT EMPTY
    {
        i = Reg->SPIRR;                 //读空FIFO
    }
    Reg->SPICR |= (1<<28)|(1<<29);      //FIFO NOT NEEDED

    for (i=0;i<len_limit;)
    {
        if (i<wrSize)
        {
            __Spi_Write(Reg,srcAddr[i]);
        }
        else if((i>=wrSize)&&(i<len_limit))
        {
            data = __Spi_Write(Reg,0);
        }
        if((destAddr) && (i>=recvoff) )
        {
            destAddr[i-recvoff] = (u8)data;
        }
    }
    Reg->SPICR &= ~(1<<5);                  //disable start
    Reg->SPICR &= ~(1<<7);                  //disable spi
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
// 返回：true,无错误;false,失败
// =============================================================================
static bool_t __Spi_TransferTxRx(tagSpiReg *Reg,u32 sendlen,u32 recvlen,
                                u32 recvoff)
{
    u8 i,ch;
    struct SPI_CB *SPI_SCB;
    struct SPI_IntParamSet *Param=NULL;

    if((u32)Reg == CN_SPI1_BASE)
    {
        Param = &IntParamset1;
        SPI_SCB = spSpiCb[CN_SPI1];
    }

    if((u32)Reg == CN_SPI2_BASE)
    {
        Param = &IntParamset2;
        SPI_SCB = spSpiCb[CN_SPI2];
    }

    __Spi_IntDisable(Reg);
    Reg->SPICR |= (1<<7);                   //ENABLE SPI
    Reg->SPICR &= ~((1<<28)|(1<<29));       //FIFO ENABLE
    while(!(Reg->SPISR & (1<<17)))          //RXFIFO NOT EMPTY
    {
        i = Reg->SPIRR;                 //读空FIFO
    }

    Param->SendDataLen = sendlen;
    Param->RecvDataLen = recvlen;
    Param->RecvOffset  = recvoff;

    sendlen = (sendlen > CN_FIFO_LEN)?CN_FIFO_LEN:sendlen;
    for(i = 0; i < sendlen; i++)
    {
        if(SPI_PortRead(SPI_SCB,&ch,1))
            Reg->SPITR = ch;
        else
            break;
    }

    Param->SendDataLen -= sendlen;

    Reg->SPICR |= (1<<5);                   //START
    __Spi_IntEnable(Reg);

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
u32 Spi_ISR(ptu32_t IntLine)
{
    struct SPI_CB *SPI_SCB=NULL;
    struct SPI_IntParamSet *param;
    tagSpiReg *Reg;
    u32 status;
    u8 ch[32],i=0,trans;

    status = *(u32*)SPI_INTR_STS_MCU4;
    if(status & (1<<4))
    {
        SPI_SCB = spSpiCb[CN_SPI1];
        Reg = (tagSpiReg *)SPI_SCB->SpecificFlag;
        param = &IntParamset1;
    }
    else if(status & (1<<5))
    {
        SPI_SCB = spSpiCb[CN_SPI2];
        Reg = (tagSpiReg *)SPI_SCB->SpecificFlag;
        param = &IntParamset2;
    }
    else
    {
        return 0;
    }

    if(param->RecvDataLen > 0)
    {
        while(!(Reg->SPISR & (1<<17)))          //RXFIFO_EMPTY
        {
            ch[0] = Reg->SPIRR;
            if(param->RecvOffset > 0)
                param->RecvOffset--;
            else
            {
                if(param->RecvDataLen > 0)
                {
                    SPI_PortWrite(SPI_SCB,ch,1);
                    param->RecvDataLen--;
                }
                else
                {
                    break;
                }
            }
        }
    }

    if((param->SendDataLen > 0) || (param->RecvDataLen > 0))
    {
        if(Reg->SPISR & (1<<25))                    //TXFIFO_EMPTY
        {
            trans = SPI_PortRead(SPI_SCB,ch,32);
            if(trans >0)
            {
                for(i = 0; i < trans; i++)
                {
                    Reg->SPITR = ch[i];
                }
                param->SendDataLen -= trans;
            }
            else
            {
                trans = (param->RecvDataLen > 32)?32:param->RecvDataLen;
                for(i = 0; i < trans; i++)
                {
                    Reg->SPITR = 0x00;
                }
            }
        }
    }

    if(param->SendDataLen + param->RecvDataLen == 0)
    {
        __Spi_IntDisable(Reg);
        Reg->SPICR &= ~(1<<5);                  //disable start
        Reg->SPICR &= ~(1<<7);                  //disable spi
    }
    else
    {
        Reg->SPICR |= (1<<5);                   //start
    }

    return 0;
}

// =============================================================================
// 功能: SPI中断配置函数
// 参数: IntLine,中断线
// 返回: 无
// =============================================================================
static void __Spi_IntConfig(u8 port)
{
    u8 IntLine = CN_INT_LINE_SPI;

    if(port == CN_SPI1)
    {
        *(u32*)SPI_INTR_EN_MCU4 |= (1<<4);
    }
    else
    {
        *(u32*)SPI_INTR_EN_MCU4 |= (1<<5);
    }
    //中断线的初始化
    Int_Register(IntLine);
    Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(IntLine,Spi_ISR);
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
bool_t ModuleInstall_SPI1(ptu32_t para)
{
    struct SPI_Param Spi_Config;
    static struct SPI_CB s_Spi1_CB;
    static u8 s_SPI1_Buf[CN_SPI1_BUF_LEN];

    Spi_Config.BusName          = "SPI1";
    Spi_Config.SPIBuf           = (u8*)&s_SPI1_Buf;
    Spi_Config.SPIBufLen        = CN_SPI1_BUF_LEN;
    Spi_Config.SpecificFlag     = CN_SPI1_BASE;
    Spi_Config.MultiCSRegFlag   = false;
    Spi_Config.pTransferTxRx    = (TransferFunc)__Spi_TransferTxRx;
    Spi_Config.pCsActive        = (CsActiveFunc)__Spi_BusCsActive;
    Spi_Config.pCsInActive      = (CsInActiveFunc)__Spi_BusCsInActive;
    Spi_Config.pBusCtrl         = (SPIBusCtrlFunc)__Spi_BusCtrl;
    Spi_Config.pTransferPoll    = (TransferPoll)__Spi_TxRxPoll;
    spSpiCb[CN_SPI1] = &s_Spi1_CB;

    __Spi_HardDefaultSet(CN_SPI1);
    __Spi_IntConfig(CN_SPI1);

    if(NULL == SPI_BusAdd_s(&s_Spi1_CB,&Spi_Config))
        return 0;
    return 1;
}

bool_t ModuleInstall_SPI2(ptu32_t para)
{
    struct SPI_Param Spi_Config;
    static struct SPI_CB s_Spi2_CB;
    static u8 s_SPI2_Buf[CN_SPI2_BUF_LEN];

    Spi_Config.BusName          = "SPI2";
    Spi_Config.SPIBuf           = (u8*)&s_SPI2_Buf;
    Spi_Config.SPIBufLen        = CN_SPI2_BUF_LEN;
    Spi_Config.SpecificFlag     = CN_SPI2_BASE;
    Spi_Config.MultiCSRegFlag   = false;
    Spi_Config.pTransferTxRx    = (TransferFunc)__Spi_TransferTxRx;
    Spi_Config.pCsActive        = (CsActiveFunc)__Spi_BusCsActive;
    Spi_Config.pCsInActive      = (CsInActiveFunc)__Spi_BusCsInActive;
    Spi_Config.pBusCtrl         = (SPIBusCtrlFunc)__Spi_BusCtrl;
    Spi_Config.pTransferPoll    = (TransferPoll)__Spi_TxRxPoll;
    spSpiCb[CN_SPI2] = &s_Spi2_CB;

    __Spi_HardDefaultSet(CN_SPI2);
    __Spi_IntConfig(CN_SPI2);

    if(NULL == SPI_BusAdd_s(&s_Spi2_CB,&Spi_Config))
        return 0;
    return 1;
}
#endif

