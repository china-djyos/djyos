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
// 模块版本: V1.0.0
// 创建人员: chj
// 创建时间:2018/5/29
// =============================================================================
#include "string.h"
#include "board.h"
#include "spibus.h"
#include "heap.h"
#include "int.h"
#include "cpu_peri.h"
#include "cpu_peri_spi.h"
#include "cpu_peri_int_line.h"

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "fsl_lpspi.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern s32 ModuleInstall_SPI(u8 Port);
//    #if (CFG_SPI1_ENABLE == 1)
//    ModuleInstall_SPI(CN_SPI1);
//    #endif
//    #if (CFG_SPI2_ENABLE == 1)
//    ModuleInstall_SPI(CN_SPI2);
//    #endif
//    #if (CFG_SPI3_ENABLE == 1)
//    ModuleInstall_SPI(CN_SPI3);
//    #endif
//    #if (CFG_SPI4_ENABLE == 1)
//    ModuleInstall_SPI(CN_SPI4);
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
//dependence:"spi bus","int"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:none           //该组件的弱依赖组件名（可以是none，表示无依赖组件），
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
#define CFG_SPI_BUF_LEN              128   //"配置SPI缓冲大小",
//#define CFG_SPI_DMA_BUF_LEN        128   //"SPI的DMA缓冲尺寸",
//%$#@enum,true,false,
#define CFG_SPI1_ENABLE              false //"是否配置使用SPI1",
#define CFG_SPI2_ENABLE              false //"是否配置使用SPI2",
#define CFG_SPI3_ENABLE              false //"是否配置使用SPI3",
#define CFG_SPI4_ENABLE              false //"是否配置使用SPI4",
//%$#@string,1,30,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure
// =============================================================================

#if !defined(LPSPI_CLK_SOURCE)
#define LPSPI_CLK_SOURCE         (1U) /* PLL3 PFD0 */
#endif
#if !defined(LPSPI_CLK_SOURCE_DIVIDER)
#define LPSPI_CLK_SOURCE_DIVIDER (8U) /* 8div */
#endif

#define EXAMPLE_LPSPI_CLOCK_FREQ (CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk) / (LPSPI_CLK_SOURCE_DIVIDER))

#define EXAMPLE_LPSPI_MASTER_CLOCK_FREQ     EXAMPLE_LPSPI_CLOCK_FREQ

#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)

#define TRANSFER_BAUDRATE 500000U /*! Transfer baudrate - 500k */
#define EXAMPLE_LPSPI_MASTER_BASEADDR (LPSPI1)
#define CN_TIMEOUT  (200*1000)
typedef LPSPI_Type      tagSpiReg;

static tagSpiReg volatile * const tg_SpiReg[] ={
                                                 (tagSpiReg *)LPSPI1_BASE,
                                                 (tagSpiReg *)LPSPI2_BASE,
                                                 (tagSpiReg *)LPSPI3_BASE,
                                                 (tagSpiReg *)LPSPI4_BASE,
                                                };

//定义中断中需使用的静态量结构体
struct SPI_IntParamSet
{
    u32 SendDataLen;
    u32 RecvDataLen;
    u32 RecvOffset;
};

// =============================================================================
//定义SPI控制块和中断静态量
//#define CN_SPI_BUF_LEN              128
//#define CN_SPI_DMA_BUF_LEN          128
static struct SPI_CB *s_ptSpi1_CB;
static u8 s_Spi1_Buf[CFG_SPI_BUF_LEN];
struct SPI_IntParamSet IntParamset1;

static struct SPI_CB *s_ptSpi2_CB;
static u8 s_Spi2_Buf[CFG_SPI_BUF_LEN];
struct SPI_IntParamSet IntParamset2;

static struct SPI_CB *s_ptSpi3_CB;
static u8 s_Spi3_Buf[CFG_SPI_BUF_LEN];
struct SPI_IntParamSet IntParamset3;

static struct SPI_CB *s_ptSpi4_CB;
static u8 s_Spi4_Buf[CFG_SPI_BUF_LEN];
struct SPI_IntParamSet IntParamset4;

static lpspi_master_config_t g_masterConfig[CN_SPI_NUM];

#define TRANSFER_SIZE 64U

uint8_t masterRxData[TRANSFER_SIZE] = {0U};
uint8_t masterTxData[TRANSFER_SIZE] = {0U};

volatile uint32_t masterTxCount;
volatile uint32_t masterRxCount;
uint8_t g_masterRxWatermark;
uint8_t g_masterFifoSize;

__attribute__((weak))  bool_t Board_SpiCsCtrl(u8 SPIPort,u8 cs,u8 level)
{
    return true;
}
__attribute__((weak))  void SPI_InLowPowerPinCfg(u8 port)
{
    return;
}
__attribute__((weak))  void SPI_OutLowPowerPinCfg(u8 port)
{
    return ;
}

// =============================================================================
// 功能: 中断使能和失能函数
// 参数: tpSPI,被操作的寄存器组指针
// 返回: 无
// =============================================================================
static void __SPI_IntEnable(volatile tagSpiReg *Reg,u32 IntSrc)
{
    LPSPI_EnableInterrupts((tagSpiReg*)Reg, IntSrc);
}
static void __SPI_IntDisable(volatile tagSpiReg *Reg,u32 IntSrc)
{
    LPSPI_DisableInterrupts((tagSpiReg*)Reg, IntSrc);
}

// =============================================================================
// 功能：SPI时钟配置函数，
// 参数：spi_dev，设备句柄
//      Fre，欲配置SCK的时钟速度，单位为Hz
// 返回：无
// =============================================================================
static void __SPI_SetClk(volatile tagSpiReg *Reg,u32 Fre)
{
    u32 base = (u32)Reg;
    switch(base)
    {
        case LPSPI1_BASE:
                g_masterConfig[0].baudRate = Fre;
                break;
        case LPSPI2_BASE:
                g_masterConfig[1].baudRate = Fre;
                break;
        case LPSPI3_BASE:
                g_masterConfig[2].baudRate = Fre;
                break;
        case LPSPI4_BASE:
                g_masterConfig[3].baudRate = Fre;
                break;
        default:
                break;
    }
}

// =============================================================================
// 功能：SPI控制寄存器参数配置，如PHA和CPOL、时钟等，根据各种寄存器而异
// 参数：tpSPI,SPI控制器基址
//       ptr,参数指针
// 返回：无
// =============================================================================
static void __SPI_Config(volatile tagSpiReg *Reg,tagSpiConfig *ptr)
{
    u8 port=0xff;
    u32 base=(u32)Reg;
    if(ptr->Freq > 40*1000*1000)
    {
        ptr->Freq = 40*1000*1000;
    }
    __SPI_SetClk(Reg,ptr->Freq);
    switch(base)
    {
        case LPSPI1_BASE:
                port=0;
                break;
        case LPSPI2_BASE:
                port=1;
                break;
        case LPSPI3_BASE:
                port=2;
                break;
        case LPSPI4_BASE:
                port=3;
                break;
        default:
                return;
    }

    if(ptr->CharLen!=8 && ptr->CharLen!=16 && ptr->CharLen!=32)
        return;

    //set the PHA
    if(ptr->Mode & SPI_CPHA)
    {
        g_masterConfig[port].cpha = kLPSPI_ClockPhaseSecondEdge;
    }
    else
    {
        g_masterConfig[port].cpha = kLPSPI_ClockPhaseFirstEdge;
    }
    //set the cpol
    if(ptr->Mode & SPI_CPOL)
    {
        g_masterConfig[port].cpol = kLPSPI_ClockPolarityActiveLow;
    }
    else
    {
        g_masterConfig[port].cpol = kLPSPI_ClockPolarityActiveHigh;
    }
    g_masterConfig[port].bitsPerFrame = ptr->CharLen;

    if(ptr->ShiftDir == SPI_SHIFT_LSB)
    {
        g_masterConfig[port].direction = kLPSPI_LsbFirst;
    }
    else
    {
        g_masterConfig[port].direction = kLPSPI_MsbFirst;
    }
}

static u32 Spi_GetFreq(void)
{
    u32 freq = 0;

    /* CLOCK_GetMux(kCLOCK_LpspiMux):
       00b: derive clock from PLL3 PFD1 720M
       01b: derive clock from PLL3 PFD0 720M
       10b: derive clock from PLL2      528M
       11b: derive clock from PLL2 PFD2 396M
    */
    switch(CLOCK_GetMux(kCLOCK_LpspiMux))
    {
    case 0:
        freq = CLOCK_GetFreq(kCLOCK_Usb1PllPfd1Clk);
        break;

    case 1:
        freq = CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk);
        break;

    case 2:
        freq = CLOCK_GetFreq(kCLOCK_SysPllClk);
        break;

    case 3:
        freq = CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk);
        break;
    }

    freq /= (CLOCK_GetDiv(kCLOCK_LpspiDiv) + 1U);

    return freq;
}

// =============================================================================
// 功能: SPI默认硬件初始化配置，主要是时钟配置和GPIO写保护引脚配置
// 参数: RegBaseAddr,寄存器基址
// 返回: 无
// =============================================================================
static void __SPI_HardConfig(u32 BaseAddr)
{
    u8 port=0;
    CLOCK_SetMux(kCLOCK_LpspiMux, LPSPI_CLK_SOURCE);
    CLOCK_SetDiv(kCLOCK_LpspiDiv, LPSPI_CLK_SOURCE_DIVIDER-1);

    switch(BaseAddr)
    {
        case LPSPI1_BASE:
                port=0;
                break;
        case LPSPI2_BASE:
                port=1;
                break;
        case LPSPI3_BASE:
                port=2;
                break;
        case LPSPI4_BASE:
                port=3;
                break;
        default:
                return;
    }
    //LPSPI_MasterGetDefaultConfig(&g_masterConfig[port]);

    g_masterConfig[port].baudRate = TRANSFER_BAUDRATE;
    g_masterConfig[port].bitsPerFrame = 8;
    g_masterConfig[port].cpol = kLPSPI_ClockPolarityActiveHigh;
   g_masterConfig[port].cpha = kLPSPI_ClockPhaseFirstEdge;
   g_masterConfig[port].direction = kLPSPI_MsbFirst;

   g_masterConfig[port].pcsToSckDelayInNanoSec = 1000000000 / g_masterConfig[port].baudRate;
   g_masterConfig[port].lastSckToPcsDelayInNanoSec = 1000000000 / g_masterConfig[port].baudRate;
   g_masterConfig[port].betweenTransferDelayInNanoSec = 1000000000 / g_masterConfig[port].baudRate;

   g_masterConfig[port].whichPcs = EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT;
   g_masterConfig[port].pcsActiveHighOrLow = kLPSPI_PcsActiveLow;

   g_masterConfig[port].pinCfg = kLPSPI_SdiInSdoOut;
   g_masterConfig[port].dataOutConfig = kLpspiDataOutRetained;

    LPSPI_MasterInit((tagSpiReg*)BaseAddr, &g_masterConfig[port], EXAMPLE_LPSPI_MASTER_CLOCK_FREQ);
    LPSPI_SetFifoWatermarks((tagSpiReg*)BaseAddr, 0, 0);
    //LPSPI_SetFifoWatermarks((tagSpiReg*)BaseAddr, txWatermark, g_masterRxWatermark);

    LPSPI_Enable((tagSpiReg*)BaseAddr, false);
    ((tagSpiReg*)BaseAddr)->CFGR1 &= (~LPSPI_CFGR1_NOSTALL_MASK);
    LPSPI_Enable((tagSpiReg*)BaseAddr, true);

    /*Flush FIFO , clear status , disable all the inerrupts.*/
    LPSPI_FlushFifo((tagSpiReg*)BaseAddr, true, true);
    LPSPI_ClearStatusFlags((tagSpiReg*)BaseAddr, kLPSPI_AllStatusFlag);
    LPSPI_DisableInterrupts((tagSpiReg*)BaseAddr, kLPSPI_AllInterruptEnable);

    ((tagSpiReg*)BaseAddr)->TCR =
            (((tagSpiReg*)BaseAddr)->TCR &
             ~(LPSPI_TCR_CONT_MASK | LPSPI_TCR_CONTC_MASK | LPSPI_TCR_RXMSK_MASK | LPSPI_TCR_PCS_MASK)) |
            LPSPI_TCR_CONT(0) | LPSPI_TCR_CONTC(0) | LPSPI_TCR_RXMSK(0) | LPSPI_TCR_TXMSK(0) | g_masterConfig[port].whichPcs;

}

// =============================================================================
// 功能：SPI片选使能，使片选有效,若控制器有具体的配置片选寄存器，可直接返回
// 参数：Reg，本模块内即SPI寄存器基址
//       cs,片选线
// 返回：无
// 说明：SPI控制器上只引出了一个CS，若需要接多个CS，则必须应用程序自己控制CS
// =============================================================================
static bool_t __SPI_BusCsActive(tagSpiReg *Reg, u8 cs)
{
    u8 SPIPort;
    for(SPIPort = 0; SPIPort < CN_SPI_NUM; SPIPort++)
    {
        if(Reg == tg_SpiReg[SPIPort])
            break;
    }
//    switch(SPIPort)
//    {
//        case CN_SPI1:
//            GPIO_SettoLow(GPIO_F, PIN14);
//            break;
//        case CN_SPI2:
//
//            break;
//        case CN_SPI3:
//
//            break;
//        case CN_SPI4:
//
//            break;
//        case CN_SPI5:
//
//            break;
//        case CN_SPI6:
//
//            break;
//        default:
//            return false;
//    }
//    return true;
    return Board_SpiCsCtrl(SPIPort,cs,0);
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
        if(Reg == tg_SpiReg[SPIPort])
            break;
    }
//    switch(SPIPort)
//    {
//        case CN_SPI1:
//            GPIO_SettoHigh(GPIO_F, PIN14);
//            break;
//        case CN_SPI2:
//
//            break;
//        case CN_SPI3:
//
//            break;
//        case CN_SPI4:
//
//            break;
//        case CN_SPI5:
//
//            break;
//        case CN_SPI6:
//
//            break;
//        default:
//            return false;
//    }
//    return true;
    return Board_SpiCsCtrl(SPIPort,cs,1);
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
    if( (Reg != tg_SpiReg[CN_SPI1]) && (Reg != tg_SpiReg[CN_SPI2])
         && (Reg != tg_SpiReg[CN_SPI3])  && (Reg != tg_SpiReg[CN_SPI4])
        )
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

        case CN_SPI_SET_POLL:
               break;

        case CN_SPI_SET_INT:
               break;
        default:        break;
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
    u32 readData=0;
    while ( ( LPSPI_GetRxFifoCount(Reg) == 0 ) && (time > 0))
    {
        time --;
        DJY_DelayUs(1);
    }

    if(time == 0)
        return -1;

    readData = LPSPI_ReadData(Reg);

    return readData;
}

// =============================================================================
// 功能：写SPI发送寄存器，轮询方式,被__SPI_TxRxPoll调用
// 参数：Reg,寄存器基址；wData，写数据
// 返回：无
// =============================================================================
static s32 __SPI_Write( tagSpiReg * Reg,u8 wData )
{
    u32 time = CN_TIMEOUT;
    u32 fifoSize = LPSPI_GetTxFifoSize(Reg);
    /* Send data */
    while ( ( LPSPI_GetTxFifoCount(Reg) == fifoSize ) && (time > 0) )
    {
        time --;
        DJY_DelayUs(1);
    }
    if(time == 0)
        return -1;

    LPSPI_WriteData(Reg, wData);
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

    len_limit = MAX(wrSize, rdSize + recvoff);

    for (i=0;i<len_limit;i++)
    {
        if (i<wrSize)
        {
            __SPI_Write(Reg,srcAddr[i]);
        }
        else if((i>=wrSize)&&(i<len_limit))
        {
          __SPI_Write(Reg,0);
        }

        if((destAddr) && (i>=recvoff))
        {
            destAddr[i-recvoff] = __SPI_Read(Reg);
        }
        else
        {
            __SPI_Read(Reg);
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
    struct SPI_CB *pSCB=NULL;
    struct SPI_IntParamSet *Param=NULL;
    u32 fifo_size=0;
    u8 i=0;
    u8 ch=0,temp=0;

    if(Reg == tg_SpiReg[CN_SPI1])
    {
        pSCB = s_ptSpi1_CB;
        Param = &IntParamset1;
    }
    else if(Reg == tg_SpiReg[CN_SPI2])
    {
        pSCB = s_ptSpi2_CB;
        Param = &IntParamset2;
    }
    else if(Reg == tg_SpiReg[CN_SPI3])
    {
        pSCB = s_ptSpi3_CB;
        Param = &IntParamset3;
    }
    else if(Reg == tg_SpiReg[CN_SPI4])
    {
        pSCB = s_ptSpi4_CB;
        Param = &IntParamset4;
    }
    else
        return false;

    Param->SendDataLen = sendlen;
    Param->RecvDataLen = recvlen;
    Param->RecvOffset  = recvoff;
    fifo_size = LPSPI_GetTxFifoSize(Reg) - LPSPI_GetTxFifoCount(Reg);
    if(fifo_size>sendlen)//当缓冲区大小比要发送的数据还大时，没必要开发送中断，直接填到缓冲区里即可
    {
        for(i=0;i<sendlen;i++)
        {
            temp = SPI_PortRead(pSCB,&ch,1);//是否还有数据需要发送
            if(temp>1)
            {
                __SPI_Write(Reg, ch);
                Param->SendDataLen --;
                Param->RecvOffset  --;
            }
            else
                return false;//还没发完，但是已经从缓冲区中读不出数据了
        }
        if(recvlen>0)
        {
            __SPI_IntEnable(Reg,kLPSPI_RxInterruptEnable);
            __SPI_Write(Reg, 0XAA);
        }
    }
    else
    {
        __SPI_IntEnable(Reg,kLPSPI_TxInterruptEnable|kLPSPI_WordCompleteInterruptEnable     \
                            |kLPSPI_FrameCompleteInterruptEnable|kLPSPI_TransferCompleteInterruptEnable);

        for(i=0;i<fifo_size;i++)
        {
            temp = SPI_PortRead(pSCB,&ch,1);//是否还有数据需要发送
            if(temp>1)
            {
                __SPI_Write(Reg, ch);
                Param->SendDataLen --;
                Param->RecvOffset  --;
            }
            else
                return false;//还没发完，但是已经从缓冲区中读不出数据了
        }//系统会进入发送中断，继续发完剩下的
    }
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
    u32 sr=0;
    u32 fifo_size=0;
    u8 ch=0;
    s32 temp=0;

    static u8 firstrcv = 0;

    if(IntLine == CN_INT_LINE_LPSPI1)
    {
        pSCB = s_ptSpi1_CB;
        Reg = (tagSpiReg *)tg_SpiReg[CN_SPI1];
        param = &IntParamset1;
    }
    else if(IntLine == CN_INT_LINE_LPSPI2)
    {
        pSCB = s_ptSpi2_CB;
        Reg = (tagSpiReg *)tg_SpiReg[CN_SPI2];
        param = &IntParamset2;
    }
    else if(IntLine == CN_INT_LINE_LPSPI3)
    {
        pSCB = s_ptSpi3_CB;
        Reg = (tagSpiReg *)tg_SpiReg[CN_SPI3];
        param = &IntParamset3;
    }
    else if(IntLine == CN_INT_LINE_LPSPI4)
    {
        pSCB = s_ptSpi4_CB;
        Reg = (tagSpiReg *)tg_SpiReg[CN_SPI4];
        param = &IntParamset4;
    }
    else
        return 0;

    sr = LPSPI_GetStatusFlags(Reg);
    LPSPI_ClearStatusFlags(Reg, sr);
    if(sr&kLPSPI_TxDataRequestFlag)//发送中断
    {
        fifo_size = LPSPI_GetTxFifoSize(Reg) - LPSPI_GetTxFifoCount(Reg);
        temp = SPI_PortRead(pSCB,&ch,fifo_size);//是否还有数据需要发送
        if(temp>0)
        {
            while(temp--)
            {
                param->SendDataLen --;
                param->RecvOffset  --;
                LPSPI_WriteData(Reg, ch);
            }
        }
        else                                            //表明发送结束
        {
            //关发送中断
            __SPI_IntDisable(Reg,kLPSPI_TxInterruptEnable);
            param->RecvOffset -= param->SendDataLen;
            param->SendDataLen = 0;

            //发送完成了，判断是否需要接收
            if(param->RecvDataLen > 0)
            {
                __SPI_IntEnable(Reg,kLPSPI_RxInterruptEnable);    //使能接收中断
                //MASTER模式下，发送和接收同时进行
                LPSPI_WriteData(Reg, 0xAA);
                firstrcv = 1;
            }
        }
    }
    else if(sr&kLPSPI_RxDataReadyFlag)    //接收中断
    {
        ch=LPSPI_ReadData(Reg);
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
            LPSPI_WriteData(Reg, 0xAA);
        }
        else                    //表明接收已经完成
        {
            param->RecvDataLen = 0;
            __SPI_IntDisable(Reg,kLPSPI_RxInterruptEnable);
        }
    }
    return 0;
}

// =============================================================================
// 功能: SPI中断配置函数
// 参数: IntLine,中断线
// 返回: 无
// =============================================================================
static void __SPI_IntConfig(u32 IntLine)
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
// 功能： 进入或退出低功耗模式
//     时钟使能 控制器使能 GPIO设置相应的状态
// 参数： port 串口号：如 CN_SPI1
//     flag 进入低功耗 或退出低功耗标志：如 SPI_IN_LOWPOWER
// 返回：true/false
// =============================================================================

bool_t SPI_LowPowerConfig(u8 port,u8 flag)
{
    if(port>=CN_SPI_NUM)
        return false;
    if(flag == InLowPower)
    {
        SPI_InLowPowerPinCfg(port);
        return true;
    }
    else if (flag == OutLowPower)
    {
        SPI_OutLowPowerPinCfg(port);
        return true;
    }
   return false;
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
    struct SPI_CB **pSpiCB;
    u8 IntLine;

    switch(port)
    {
        case CN_SPI1:
            SPI_Config.BusName          = "SPI1";
            SPI_Config.SPIBuf           = (u8*)&s_Spi1_Buf;
            IntLine = CN_INT_LINE_LPSPI1;
            pSpiCB = &s_ptSpi1_CB;
            break;
        case CN_SPI2:
            SPI_Config.BusName          = "SPI2";
            SPI_Config.SPIBuf           = (u8*)&s_Spi2_Buf;
            IntLine = CN_INT_LINE_LPSPI2;
            pSpiCB = &s_ptSpi2_CB;
            break;
        case CN_SPI3:
            SPI_Config.BusName          = "SPI3";
            SPI_Config.SPIBuf           = (u8*)&s_Spi3_Buf;
            IntLine = CN_INT_LINE_LPSPI3;
            pSpiCB = &s_ptSpi3_CB;
            break;
        case CN_SPI4:
            SPI_Config.BusName          = "SPI4";
            SPI_Config.SPIBuf           = (u8*)&s_Spi4_Buf;
            IntLine = CN_INT_LINE_LPSPI4;
            pSpiCB = &s_ptSpi4_CB;
            break;
        default:
            return (0);
    }
    SPI_Config.SpecificFlag     = (ptu32_t)tg_SpiReg[port];
    SPI_Config.SPIBufLen        = CFG_SPI_BUF_LEN;
    SPI_Config.MultiCSRegFlag   = false;
    SPI_Config.pTransferPoll    = (TransferPoll)__SPI_TxRxPoll;
    SPI_Config.pTransferTxRx    = (TransferFunc)__SPI_TransferTxRx;
    SPI_Config.pCsActive        = (CsActiveFunc)__SPI_BusCsActive;
    SPI_Config.pCsInActive      = (CsInActiveFunc)__SPI_BusCsInActive;
    SPI_Config.pBusCtrl         = (SPIBusCtrlFunc)__SPI_BusCtrl;

    *pSpiCB = SPI_BusAdd(&SPI_Config);// 将SPI端口添加到SPI总线
    if(NULL == (*pSpiCB))
        return (0); // 失败

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
s32 ModuleInstall_SPI(u8 Port)
{
    if(0 == SPI_Initialize(Port))
        return (-1);
    return (0);
}
