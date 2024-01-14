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
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
// =============================================================================

// 文件名     ：cpu_peri_spi.c
// 模块描述: SPI模块底层硬件驱动模块
// 模块版本:
// 创建人员:
// 创建时间:
// =============================================================================
#include "string.h"
#include "djyos.h"
#include "cpu_peri.h"
#include "int.h"
#include "spibus.h"
#include "dbug.h"
#include "cpu_peri_spi.h"
#include "cpu_peri_io.h"
#include "set-cache.h"
#include "sys_clock.h"
#include "sys_dma.h"
#include "math.h"
// =============================================================================
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。
//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    bool_t ModuleInstall_SPI(u8 SPI_port);
//    #if CFG_SPI0_ENABLE==1
//    ModuleInstall_SPI(CN_SPI0);
//    #endif
//    #if CFG_SPI1_ENABLE==1
//    ModuleInstall_SPI(CN_SPI1);
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
//dependence:"spi bus"//该组件的依赖组件名（可以是none，表示无依赖组件），
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
#define CFG_SPI0_ENABLE               false     //"是否使用SPI0",
#define CFG_SPI1_ENABLE               false     //"是否使用SPI1",
#define CFG_SPI0_DMA_ENABLE           true      //"SPI0是否使用DMA传输",
#define CFG_SPI1_DMA_ENABLE           false     //"SPI1是否使用DMA传输",
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束
//@#$%component end configure

// =============================================================================
#define CN_TIMEOUT  (200*1000)

//定义中断中需使用的静态量结构体
struct SPI_IntParamSet
{
    u32 SendDataLen;
    u32 RecvDataLen;
    u32 RecvOffset;
};

#define delay_ms(x) DJY_DelayUs(x * 1000)
#define u64_t u64
#define u32_t u32
#define u16_t u16
#define u8_t  u8
#define sysprintf  printf
// =============================================================================
//定义SPI控制块和中断静态量
//#define CFG_SPI_BUF_LEN              128
//#define CN_SPI_DMA_BUF_LEN          128
static struct SPI_CB *s_ptSpi0_CB;
static u8 s_Spi0_Buf[CFG_SPI_BUF_LEN];
struct SPI_IntParamSet IntParamset0;

static struct SPI_CB *s_ptSpi1_CB;
static u8 s_Spi1_Buf[CFG_SPI_BUF_LEN];
struct SPI_IntParamSet IntParamset1;
/*---------------------------------------------------
修改记录
2020-04-13
    1.128x512字节写不正常，sys_spi.c中SPI_Flash_Write函数的参数unsigned short NumByteToWrite改为unsigned int NumByteToWrite
2020-08-19
    1.增加DMA读写模式
2020-12-17
    1.增加32K块，64K块擦除函数
    2.增加读UID(唯一ID),设备ID函数
2021-06-10
    1.DMA与中断开启的情况读写失败：增加一个虚拟缓存virtual_buff替换读写函数的NULL代入参数。
2021-07-07
    1.virtual_buff改为使用malloc分配，如果有使用系统,不能使用C库的malloc,请使用固定内存分配或系统的内存管理。
2021-07-09
    1.增加对16MB以上型号读写的支持,主要增加进入4地址模式与4地址读写-W25Q256测试正常。
    需要注意，存储程序的SPI NOR FLASH使用4地址时，使用完后与退出程序后应该恢复2地址模式，因为不断电的情况下FLASH一直默认4地址，
    而芯片则以2地址模式读取程序，会造成复位后程序不启动一直进入下载模式的问题。
2021-07-14
    1.增加对CLK的频率限制与最大频率设置函数,避免频率太高而读写错误问题。
2022-05-19
    1.增加软件CS脚控制-这样一个SPI可以挂多个SPI设备。
2022-07-03
    1.增加从设备支持-支持CPU+DMA模式-测试收发65K正常。
2022-08-19
    1.增加两线读，读写模式由原来8位改为32位，增加读写速度。
----------------------------------------------------*/

int rx_dual_en=0;//0=RX单线读 1=RX双线读

#if (CFG_SPI0_DMA_ENABLE == true) || (CFG_SPI1_DMA_ENABLE ==true)
#define  SPI_DMA_EN
#endif

#if SPI_DMA_EN  //DMA配置
    DMA * SPI_TXdma;
    DMA * SPI_RXdma;
#endif

//SPI 设备模式标志
unsigned int spi_slave_mode_en=0;//=0主机模式，=1从机模式
//HZ 最大时钟限制
int spi_max_clk=105000000;//默认最大时钟频率


/*---------------------------------------------------
SPI DMA传输时的等待
1.如果查询DMA状态太密集，会让DMA速度降低，所以延时是必要的。
2.多任务时可替换成系统诞时(设置1ms延时)执行其它任务，提高多任务执行效率。
---------------------------------------------------*/
void spi_dma_delay(void)
{
    DJY_DelayUs(10);
}
//最大时钟限制
void set_max_clk(int clk)
{
    spi_max_clk=clk;
}
/*---------------------------------------------------
配置SPI为设备模式-初始化要早于spi_spi_confing()
---------------------------------------------------*/
void spi_set_slave_mode(void)
{
    spi_slave_mode_en=1;
}
/*---------------------------------------------------
退出SPI设备模式
---------------------------------------------------*/
void spi_exit_slave_mode(void)
{
    spi_slave_mode_en=0;
}
/*---------------------------------------------------
读取接收缓存计数值
----------------------------------------------------*/
int read_rx_count(int SPI)
{
virtual_addr_t addr =spi_get_addr(SPI);
  int len=read32(addr + SPI_FSR) & 0xff;
    return len;
}
//时钟模式
#define Select_Clock_Divide_Rate_1_EN 0 //1=Clock_Divide_Rate_1   0=Clock_Divide_Rate_2

/*---------------------------------------------------
SPI配置
---------------------------------------------------*/
void spi_spi_confing(int SPI)
{
    virtual_addr_t addr=spi_get_addr(SPI);
    u32_t val;

    /*
    if CPU_CLK=600MHZ
    AHB_CLK=CPU_CLK/2=600/2=300
    SPI_CLK=AHB_CLK / (2*(n + 1));
    [n=2 SPI_CLK=50MHZ]
    [n=1 SPI_CLK=75MHZ]
    [n=0 SPI_CLK=150MHZ]
    */

    /*
    if CPU_CLK=408MHZ
    AHB_CLK=CPU_CLK/2=408/2=204
    SPI_CLK=AHB_CLK / (2*(n + 1));


  [n=5 SPI_CLK=17MHZ]
    [n=4 SPI_CLK=20.4MHZ]
    [n=3 SPI_CLK=25.5MHZ]
    [n=2 SPI_CLK=34MHZ]
    [n=1 SPI_CLK=51MHZ]
    [n=0 SPI_CLK=102MHZ]
    */
    int spi_clk=0;
    if(spi_slave_mode_en==0)//设备模式不用初始化时钟
    {
        int n=0;
        int ahb_clk=get_ahb_frequency();
        sysprintf("ahb_clk= %d hz\r\n",ahb_clk);
        while(n<0xff)
        {
#if Select_Clock_Divide_Rate_1_EN
            spi_clk=ahb_clk / (1<<(n+1));
#else
            spi_clk=ahb_clk / (2*(n + 1));
#endif
            if(spi_clk>spi_max_clk)//SPI 时钟频率限制
            {
                n++;
            }else
            {
                break;
            }
        }
        sysprintf("CLK n=%d\r\n",n);
        sysprintf("SPI%d initial CLK[%dHZ]...\r\n",SPI,spi_clk);
            /* Set spi clock rate control register */
        #if Select_Clock_Divide_Rate_1_EN
            write32(addr + SPI_CCR, 0x00000000|(n<<8));
        #else
            write32(addr + SPI_CCR, 0x00001000|n);
        #endif
    }
    /* Enable spi and do a soft reset */
    val = read32(addr + SPI_GCR);
    val |= ((u64_t)1 << 31) | (1 << 7) | (1 << 1) | (1 << 0);
    if(spi_slave_mode_en==1)val &= ~( 1 << 1);//设备模式
    write32(addr + SPI_GCR, val);
    while(read32(addr + SPI_GCR) & ((u64_t)1 << 31)){};
  //--------------
  if(spi_slave_mode_en==0)//设备模式不用初始化
  {
        val = read32(addr + SPI_TCR);
        //采样配置
        val &= ~((0x1 << 13)|(0x1 << 11));//SDM(bit13)  SDC(bit11)  清0
        if(spi_clk>=80000000)val |=((0x0 << 13)|(0x1 << 11));//delay one cycle sample  0  1  >=80 MHz
        else if(spi_clk<=24000000)val |=((0x1 << 13)|(0x0 << 11));//normal sample  1  0  <=24 MHz
        else if(spi_clk<=40000000)val |=((0x0 << 13)|(0x0 << 11));//delay half cycle sample  0  0  <=40 MHz

        val &= ~(0x3 << 0);
        val |= (1 << 6) | (1 << 2);
//        if(spi_software_cs_en==1)val &= ~( 1 << 6);//软件CS脚使能
        write32(addr + SPI_TCR, val);
    }
  //FIFO复位
    val = read32(addr + SPI_FCR);
    val |= ((u64_t)1 << 31) | (1 << 15);
    write32(addr + SPI_FCR, val);
}
/*---------------------------------------------------
SPI FIFO DMAC触发等级设置
tx_level/rx_level = 0x0-0x40;
---------------------------------------------------*/
void sys_spi_set_fifo_request_trigger_level(int SPI,int tx_level,int rx_level)
{
  virtual_addr_t addr = spi_get_addr(SPI);
  u32_t val;
  val = read32(addr + SPI_FCR);
  val &=~( (0XFF << 16) | (0XFF << 0));//清 TX RX
  val |= ((tx_level<< 16) | (rx_level << 0));//设置触发值
  write32(addr + SPI_FCR, val);
}

/*---------------------------------------------------
使能DMA_DRQ
---------------------------------------------------*/
void sys_spi_enable_drq(int SPI,int tx_drq_en,int rx_drq_en)
{
    virtual_addr_t addr =spi_get_addr(SPI);

    if(tx_drq_en==0)C_BIT(addr+SPI_FCR,24);//TX_DRQ
    else S_BIT(addr+SPI_FCR,24);

    if(rx_drq_en==0)C_BIT(addr+SPI_FCR,8);//RX_DRQ
    else S_BIT(addr+SPI_FCR,8);
}
/*---------------------------------------------------
SPI开始
---------------------------------------------------*/
void sys_spi_start(int SPI)
{
    virtual_addr_t addr =spi_get_addr(SPI);
    S_BIT(addr + SPI_TCR,31);
}
/*---------------------------------------------------
设置计数值
---------------------------------------------------*/
void sys_spi_set_count(int SPI,int txc,int len)
{
  virtual_addr_t addr =spi_get_addr(SPI);
  write32(addr + SPI_MBC, len & 0xffffff);
  write32(addr + SPI_MTC, txc & 0xffffff);
  write32(addr + SPI_BCC, (txc & 0xffffff) | ((rx_dual_en&0xf)<<28) );
}
/*---------------------------------------------------
SPI SPI CPU传输
----------------------------------------------------*/
int sys_spi_cpu_transfer(int SPI,void * txbuf, void * rxbuf, int len)
{
  virtual_addr_t addr =spi_get_addr(SPI);
  u32_t * tx32 = (u32_t *)txbuf;
  u32_t * rx32 = (u32_t *)rxbuf;
  int n, i;
  int txc=len;
  int rxc=len;
  if(!txbuf)txc=0;
  if(rx_dual_en>0xf0)txc=0;//两线接收时不发送
  if(spi_slave_mode_en==0)
    {
        sys_spi_set_count(SPI,txc,len);
        sys_spi_start(SPI);
    }
  while(!((txc==0)&&(rxc==0)))
  {
    //*************发送数据*************
    if(txc>0)
    {
#define CPU_TX_LEN 32  //CPU发送一次的长度，0-64
      if(txc >= CPU_TX_LEN)//4Byte模式发送
      {
        n=CPU_TX_LEN;
        if(((read32(addr + SPI_FSR)>>16) & 0xff)<=(64-n))
        {
          u32_t * buf32 = tx32;
          for(i = n; i > 0; i-=4)write32(addr + SPI_TXD, *buf32++);
          txc-=n;
          tx32+=n/4;
        }
      }else//1Byte模式发送
      {
        n=txc;
        if(((read32(addr + SPI_FSR)>>16) & 0xff)<=(64-n))
        {
          u8_t * buf8 =(u8_t *)tx32;
          for(i = n; i > 0; i--)write8(addr + SPI_TXD, *buf8++);
          txc-=n;
        }
      }
    }
    //*************接收数据*************
    if(rxc>0)
    {
#define CPU_RX_LEN 32  //CPU接收一次的长度，0-64
      if(rxc >= CPU_RX_LEN)//4Byte模式接收
      {
        n=CPU_RX_LEN;
        if((read32(addr + SPI_FSR) & 0xff)>=n)
        {
          u32_t val32;
          for(i = n; i > 0;i-=4)
          {
            val32 = read32(addr + SPI_RXD);
            if(rx32)*rx32++ = val32;
          }
          rxc -= n;
        }
      }else//1Byte模式接收
      {
        n=rxc;
        if((read32(addr + SPI_FSR) & 0xff)>=n)
        {
          u8_t *rx=(u8_t *)rx32;
          u8_t val8;
          for(i = n; i > 0;i--)
          {
            val8 = read8(addr + SPI_RXD);
            *rx++ = val8;
          }
          rxc -= n;
        }
      }
    }
  }

  if(spi_slave_mode_en==0)
    {
        //等待空闲- 低速时必须加这个判断-DMA有FLUSH过程可省略。
        int timeout=10000;
        while( (read32(addr + SPI_TCR)&0x80000000) && ((--timeout)>0) );
        if(timeout==0)
        {
            sysprintf("ERR:sys_spi_cpu_transfer timeout.\r\n");
            return -1;
        }
    }

  return len;
}

#if SPI_DMA_EN
/*---------------------------------------------------
SPI DMA transfe
----------------------------------------------------*/
int sys_spi_dma_transfer(int SPI,DMA * TXdma,DMA * RXdma,void * txbuf, void * rxbuf, int len)
{
  virtual_addr_t addr =spi_get_addr(SPI);

  int len1=len;
    int dma_data_width=DMA_DATA_WIDTH_32;
    /*1.设备模式,len为4的倍数时可以32位数据宽度,其它为8位数据,否则收发出错。
      2.一次数据不能多次传输，因为时钟是主机发出，从机不能中途暂停，
      数据长度必须len<=NDMA_MAX_LEN。*/
    if(spi_slave_mode_en==1)
    {
        if(len1%4)dma_data_width=DMA_DATA_WIDTH_8;
        if(len1>NDMA_MAX_LEN)
        {
            sysprintf("ERR:sys_spi_dma_transfer()->len>NDMA_MAX_LEN\r\n");
            return 0;
        }
    }
    //---------
    int trlen=len1>NDMA_MAX_LEN?NDMA_MAX_LEN:len1;
    unsigned int txbuf1=(unsigned int)txbuf;
    unsigned int rxbuf1=(unsigned int)rxbuf;
RESTART:
  if(txbuf!=0)
    {
        TXdma->Type=NDMA;//类型
        TXdma->Ch=0;//通道
        TXdma->Byte_Counter=trlen;//Byte计数
        TXdma->Continuous_Mode_Enable=0;//连续模式
        TXdma->Read_Byte_Counter_Enable=0;//读计数值使能
        //---------
        TXdma->Source_Address=(unsigned int)txbuf1;//源地址
        TXdma->Source_Address_Type=DMA_ADDRESS_TYEP_LINEER;//源地址类型
        TXdma->Source_DRQ_Type=NDMAS_DRQ_Type_SDRAM_Memory;//源类型
        TXdma->Source_Data_Width=dma_data_width;//源数据宽度
        TXdma->Source_Burst_Length=DMA_BURST_LENGTH_1;//BURST
        //-----------
        TXdma->Destination_Address=(unsigned int)(addr + SPI_TXD);//目标地址
        TXdma->Destination_Address_Type=DMA_ADDRESS_TYEP_IO;//目标地址类型
        TXdma->Destination_DRQ_Type=(SPI==0?NDMAD_DRQ_Type_SPI0_Tx:NDMAD_DRQ_Type_SPI1_Tx);//目标类型
        TXdma->Destination_Data_Width=dma_data_width;//目标数据宽度
        TXdma->Destination_Burst_Length=DMA_BURST_LENGTH_1;//BURST
        //-----------
        //sysprintf("发送数据大小: %d \r\n",TXdma->Byte_Counter);
        DMA_Config(TXdma);
        DMA_Enable(TXdma);
    }
    //---------------------------------------------------------------------------------------
    RXdma->Type=NDMA;//类型
    RXdma->Ch=1;//通道
    RXdma->Byte_Counter=trlen;//Byte计数
    RXdma->Continuous_Mode_Enable=0;//连续模式
    RXdma->Read_Byte_Counter_Enable=0;//读计数值使能
    //---------
    RXdma->Source_Address=(unsigned int)(addr + SPI_RXD);//源地址
    RXdma->Source_Address_Type=DMA_ADDRESS_TYEP_IO;//源地址类型
    RXdma->Source_DRQ_Type=(SPI==0?NDMAS_DRQ_Type_SPI0_Rx:NDMAS_DRQ_Type_SPI1_Rx);//源类型
    RXdma->Source_Data_Width=dma_data_width;//源数据宽度
    RXdma->Source_Burst_Length=DMA_BURST_LENGTH_1;//BURST
    //-----------
    RXdma->Destination_Address=(unsigned int)rxbuf1;//目标地址
    RXdma->Destination_Address_Type=DMA_ADDRESS_TYEP_LINEER;//目标地址类型
    RXdma->Destination_DRQ_Type=NDMAD_DRQ_Type_SDRAM_Memory;//目标类型
    RXdma->Destination_Data_Width=dma_data_width;//目标数据宽度
    RXdma->Destination_Burst_Length=DMA_BURST_LENGTH_1;//BURST
    //-----------
    //sysprintf("接收数据大小: %d \r\n",RXdma->Byte_Counter);
    DMA_Config(RXdma);
    DMA_Enable(RXdma);
  //FIFO DMA 触发等级配置
    if(dma_data_width==DMA_DATA_WIDTH_32)
        sys_spi_set_fifo_request_trigger_level(SPI,60,4);
    else
        sys_spi_set_fifo_request_trigger_level(SPI,64,1);
    //开启DMA_DRQ
    sys_spi_enable_drq(SPI,txbuf==0?0:1,1);
  if(spi_slave_mode_en==0)
    {
        //设置计数值
        sys_spi_set_count(SPI,txbuf==0?0:trlen,trlen);
        //SPI开始
        sys_spi_start(SPI);
    }
    while(1)
    {
        if(DMA_Get_Full_TIP(RXdma))
        {
            //sysprintf("Transfer complete...\r\n");
            break;
        }
        //延时或作务
        spi_dma_delay();
    }
    DMA_Disable(RXdma);
  if(txbuf!=0)DMA_Disable(TXdma);
    //大于128K继续传输
    len1-=trlen;
    if(len1>0)
    {
        txbuf1+=trlen;
        rxbuf1+=trlen;
        trlen=len1>NDMA_MAX_LEN?NDMA_MAX_LEN:len1;
        goto RESTART;
    }
    //写通CACHE
  //sysInvalidCache();
//  sysFlushCache(D_CACHE);
    Cache_InvalidData();
    return len;
}
#endif

/*SPI传输
*/
int sys_spi_transfer(int SPI,void * txbuf, void * rxbuf, int len)
{
    if(len>0xffffff)//一次最大传输长度为0xffffff.
    {
        sysprintf("ERR:spi transfer max len 0xffffff,len=0x%08x\r\n",len);
        return -1;
    }
#if SPI_DMA_EN
    void *p=0;
    if(len>64)//>64 DMA模式-DMA模式时缓存指针不能为0
    {
        if(rxbuf==NULL)
        {
            p=malloc(len);
            if(p==0)
            {
                sysprintf("ERR:malloc() rxbuf==0 \r\n");
                return 0;
            }
            rxbuf=p;
        }
        if(((int)txbuf%4)!=0)//DMA缓存需要4字节对齐
        {
            sysprintf("ERR:malloc() txbuf alignment(%08x)\r\n",txbuf);
            return 0;
        }
        if(((int)rxbuf%4)!=0)//DMA缓存需要4字节对齐
        {
            sysprintf("ERR:malloc() rxbuf alignment(%08x)\r\n",rxbuf);
            return 0;
        }

        sys_spi_dma_transfer(SPI,SPI_TXdma,SPI_RXdma,txbuf,rxbuf,len);
        if(p)free(p);
    }else//CPU模式
    {
#endif
        sys_spi_cpu_transfer(SPI,txbuf,rxbuf,len);
#if SPI_DMA_EN
    }
#endif
    return len;
}

/*SPI write_then_read
*/
static int sys_spi_write_then_read(int SPI,void * txbuf, int txlen,
        void * rxbuf, int rxlen)
{

    if(sys_spi_transfer(SPI,txbuf, NULL, txlen) != txlen)
        return -1;
  if(rx_dual_en>0)rx_dual_en|=0xf0;//2线读
    if(sys_spi_transfer(SPI,NULL, rxbuf, rxlen) != rxlen)
        return -1;
    return 0;
}
/*SPI write_then_write
*/
static int sys_spi_write_then_write(int SPI,void * txbuf0, int txlen0, void * txbuf1, int txlen1)
{
    if(sys_spi_transfer(SPI,txbuf0, NULL, txlen0) != txlen0)
        return -1;
    if(sys_spi_transfer(SPI,txbuf1, NULL, txlen1) != txlen1)
        return -1;
    return 0;
}


/*SPI clock_初始化
*/
void spi_clock_init(int SPI)
{
    S_BIT(0x01c202c0,(20+SPI));/* Deassert reset */
    S_BIT(0x01c20060,(20+SPI));/* Open the bus gate */
}

// =============================================================================
// 功能: SPI默认硬件初始化配置，主要是时钟配置
// 参数: RegBaseAddr,寄存器基址
// 返回: 无
// =============================================================================
void spi_init(int SPI)
{
    spi_clock_init(SPI);
    spi_spi_confing(SPI);
}
//**********************************************************************************

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
static void __SPI_IntEnable(struct tagSpiReg *Reg,u32 IntSrc)
{
    Reg->SPI_IER |= IntSrc;
}
static void __SPI_IntDisable(struct tagSpiReg *Reg,u32 IntSrc)
{
    Reg->SPI_IER &= ~IntSrc;
}
// =============================================================================
// 功能：SPI时钟配置函数
// 参数：SPI，cpu的spi接口号
//      Freq，欲配置的时钟速度，单位为Hz
// 返回：true=成功，false=失败
// 说明：
// =============================================================================
static void __SPI_SetClk( struct tagSpiReg * Reg,u32 Freq)
{
    u32 n=0;
    u32 spi_clk=0;
    u32 val;
    u32 ahb_clk=get_ahb_frequency();
    if(Freq > spi_max_clk)
        Freq = spi_max_clk;
    while(n<0xff)
    {
#if Select_Clock_Divide_Rate_1_EN
        spi_clk=ahb_clk / (1<<(n+1));
#else
        spi_clk=ahb_clk / (2*(n + 1));
#endif
        if(spi_clk > Freq)      //SPI 时钟频率限制
        {
            n++;
        }else
        {
            break;
        }
    }
    /* Set spi clock rate control register */
#if Select_Clock_Divide_Rate_1_EN
//  write32(addr + SPI_CCR, 0x00000000|(n<<8));
    Reg->SPI_CCR = 0x00000000|(n<<8);
#else
//  write32(addr + SPI_CCR, 0x00001000|n);
    Reg->SPI_CCR = 0x00001000|n;
#endif
}

// =============================================================================
// 功能：SPI控制寄存器参数配置，如PHA和CPOL、时钟等，根据各种寄存器而异
// 参数：tpSPI,SPI控制器基址
//       ptr,参数指针
// 返回：无
// =============================================================================
static void __SPI_Config(struct tagSpiReg *Reg,tagSpiConfig *ptr)
{
    __SPI_SetClk(Reg,ptr->Freq);
//    Reg->CR1 &= ~SPI_CR1_SPE;

    //set the PHA
    if(ptr->Mode & SPI_CPHA)
    {
        Reg->SPI_TCR |= SPI_CTL_PHA;
    }
    else
    {
        Reg->SPI_TCR &= ~SPI_CTL_PHA;
    }
    //set the cpol
    if(ptr->Mode & SPI_CPOL)
    {
        Reg->SPI_TCR |= SPI_CTL_POL;
    }
    else
    {
        Reg->SPI_TCR &= ~SPI_CTL_POL;
    }

    if(ptr->CharLen == 16)  //C100似乎不支持16位传输。
    {
    }
    else
    {
    }
    if(ptr->ShiftDir == SPI_SHIFT_LSB)
    {
        Reg->SPI_TCR |= SPI_CTL_LMTF;
    }
    else
    {
        Reg->SPI_TCR &= ~SPI_CTL_LMTF;
    }
}

// =============================================================================
// 功能：SPI片选使能，使片选有效,若控制器有具体的配置片选寄存器，可直接返回
// 参数：Reg，本模块内即SPI寄存器基址
//       cs,片选线
// 返回：无
// =============================================================================
static bool_t __SPI_BusCsActive(u32 SPI, u8 cs)
{
    if(cs < 4)  //本CPU有4个标准CS引脚
    {
        virtual_addr_t addr =spi_get_addr(SPI);
        u32_t val;
        val = read32(addr + SPI_TCR);
        val &= ~((0x3 << 4) | (0x1 << 7));
        val |= ((0 & 0x3) << 4) | (0 << 7);
        write32(addr + SPI_TCR, val);
        return true;
    }
    else
    {
        //如果是gpio控制CS，则本驱动不知道所用的是哪个gpio，无从控制，应在boarddrv
        //中实现如下函数。
        return Board_SpiCsCtrl(SPI,cs,0);
    }
}

// =============================================================================
// 功能：SPI片选失能，使片选无效
// 参数：Reg，本模块内即SPI寄存器基址
//       cs,片选线
// 返回：无
// =============================================================================
static bool_t __SPI_BusCsInActive(u32 SPI, u8 cs)
{
    if(cs < 4)  //本CPU有4个标准CS引脚
    {
        virtual_addr_t addr =spi_get_addr(SPI);
        u32_t val;
        val = read32(addr + SPI_TCR);
        val &= ~((0x3 << 4) | (0x1 << 7));
        val |= ((0 & 0x3) << 4) | (0x1 << 7);
        write32(addr + SPI_TCR, val);
        return true;
    }
    else
    {
        //如果是gpio控制CS，则本驱动不知道所用的是哪个gpio，无从控制，应在boarddrv
        //中实现如下函数。
        return Board_SpiCsCtrl(SPI,cs,1);
    }
}

// =============================================================================
// 功能：SPI总线控制回调函数，被上层调用，目前只实现对SPI时钟频率配置
// 参数：specific_flag,个性标记，本模块内即IIC寄存器基址
//       cmd,命令
//       data1,data2,数据，与具体命令相关
// 返回：无
// =============================================================================
static s32 __SPI_BusCtrl(u32 SPI,u32 cmd,ptu32_t data1,ptu32_t data2)
{
    s32 result = 1;
    struct tagSpiReg *Reg = (struct tagSpiReg *)spi_get_addr(SPI);

    switch(cmd)
    {
    case CN_SPI_SET_CLK:
        __SPI_SetClk(Reg,data1);
        break;

    case CN_SPI_CS_CONFIG:
//      __SPI_Config(Reg,(tagSpiConfig *)data1);    //有多套片选控制器才需要
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
static u8 __SPI_Read(struct tagSpiReg * Reg )
{
    u32 time = CN_TIMEOUT;
    while(((Reg->SPI_FSR & 0xff) == 0) && (time > 0))
    {
        time --;
        DJY_DelayUs(1);
    }
    if(time == 0)
    {
        *(volatile u8*)&(Reg->SPI_RXD);
        return -1;
    }
    else
        return  *(volatile u8*)&(Reg->SPI_RXD);
}

// =============================================================================
// 功能：写SPI发送寄存器，轮询方式,被__SPI_TxRxPoll调用
// 参数：Reg,寄存器基址；wData，写数据
// 返回：无
// =============================================================================
static s32 __SPI_Write(struct tagSpiReg * Reg,u8 wData )
{
    u32 time = CN_TIMEOUT;
    while((((Reg->SPI_FSR>>16) & 0xff) >= 64) && (time > 0))
    {
        time --;
        DJY_DelayUs(1);
    }
    if(time == 0)
        return -1;
    *(u8*)&(Reg->SPI_TXD) = wData;
    return 0;
}

// =============================================================================
// 功能：轮询方式读写数据
// 参数：Reg,寄存器基址，srcAddr,数据源指针；wrSize，写数据量，destAddr，读到的数据
//       地址指针；rdSize，读数据量，recvoff,读偏移
// 返回：true,正确；false,错误
// =============================================================================
static bool_t __SPI_TxRxPoll(u32 SPI,u8 *srcAddr,u32 wrSize,
        u8 *destAddr, u32 rdSize,u32 recvoff,u8 cs)
{
    u32 i,len_limit;
    volatile u32 k;
    u8 r;
    atom_low_t si;
    struct tagSpiReg *Reg = (struct tagSpiReg *)spi_get_addr(SPI);
    if( (!srcAddr) || ((rdSize != 0) && (!destAddr)))
        return false;
    len_limit = MAX(wrSize, rdSize + recvoff);

    Reg->SPI_MBC = len_limit;
    Reg->SPI_MTC = wrSize;
    Reg->SPI_BCC = wrSize;
    Reg->SPI_TCR |= SPI_CTL_XCH;
    for(i = 0; i < len_limit; i++)
    {
        if((k = i) < wrSize)
        {
//          if((Reg->SPI_BCC==0))
//              printf("spi send fifo pre error\r\n");
            __SPI_Write(Reg, srcAddr[i]);
        }
//        else if((k = i) < wrSize)
//        {
////          if((Reg->SPI_BCC==0))
////              printf("spi send fifo pre error\r\n");
//            __SPI_Write(Reg, srcAddr[i]);
//        }
//配置好了SPI_MBC、SPI_MTC、SPI_BCC，单纯读设备时无需写 txd 寄存器，CPU会自动发出读时钟
//      else
//      {
//          __SPI_Write(Reg, 0);
//      }
        r = __SPI_Read(Reg); //移位进来的数据一定会进入fifo，无论是否需要，都必须读出来
        if((i >= recvoff) && (i < (recvoff + rdSize)))
        {
            destAddr[i - recvoff] = r;
        }
//        else if((i >= recvoff) && (i < (recvoff + rdSize)))
//        {
//            destAddr[i - recvoff] = r;
//        }
//        extern u32 disint,enint;
//        si = Int_LowAtomStart();
//        if(!len_limit)
//            disint++;
//        Int_LowAtomEnd(si);
//        if(!len_limit)
//            enint++;
        if((Reg->SPI_FSR & SPI_FIFO_TXCNT) && (Reg->SPI_BCC==0))
            printf("spi send fifo error\r\n");
    }
extern u32 fileoffset,errspi;
static u32 wordoffset,resume = 100000,loop;
    if(rdSize == 0x1000)
    {
        if(fileoffset != resume)
        {
            resume = fileoffset;
            wordoffset = 0;
        }
        for(loop = 0;loop < 0x400; loop++)
        {
            if(((u32*)destAddr)[loop] != fileoffset*0x4000+wordoffset)
            {
                errspi++;
            }
            wordoffset++;
        }
    }
//    for (wr=0;((wr<wrSize)&&(wrSize != 0))||((rd < (recvoff+rdSize))&&(rdSize != 0)); )
//    {
//        if ((wr + 4 < wrSize) && (fiforoom < 60))
//        {
//            //srcAddr可能不是对齐的地址
//            Reg->SPI_TXD = srcAddr[wr] + (srcAddr[wr+1]<<8) + (srcAddr[wr+2]<<16) + (srcAddr[wr+3]<<24);
//            wr +=4;
//        }
//        else if((wr < wrSize) && (fiforoom < 64))
//        {
//            *(u8*)&(Reg->SPI_TXD) = srcAddr[wr];
//            wr++;
//        }
////配置好了SPI_MBC、SPI_MTC、SPI_BCC，读设备时无需写 txd 寄存器，CPU会自动发出读时钟
////      else if((i>=wrSize)&&(i<len_limit))
////      {
////          Reg->SPI_TXD = 0;
////      }
//        fiforoom = (Reg->SPI_FSR) & 0xff;   //接收fifo数据量
//        for(j = 0; j< fiforoom; j++)
//        {
//            if((rd < recvoff) || (rd > recvoff + rdSize))
//            {
//                *(volatile u8*)&(Reg->SPI_RXD);
//            }
//            else
//            {
//                destAddr[rd - recvoff] = *(volatile u8*)&(Reg->SPI_RXD);
//            }
//            rd++;
//        }
//    }
//
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
static bool_t __SPI_TransferTxRx(u32 SPI,u32 sendlen,u32 recvlen,
                                u32 recvoff)
{
    struct SPI_IntParamSet *Param=NULL;
    struct tagSpiReg *Reg = (struct tagSpiReg *)spi_get_addr(SPI);

    Param->SendDataLen = sendlen;
    Param->RecvDataLen = recvlen;
    Param->RecvOffset  = recvoff;

    __SPI_IntEnable(Reg,SPI_INTEN_TR);
    Reg->SPI_TCR |= SPI_CTL_XCH;

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
u32 SPI_ISRFUNC(ptu32_t IntLine)
{
#if 0
    struct SPI_CB *pSCB=NULL;
    struct SPI_IntParamSet *param;
    tagSpiReg *Reg;
    u32 spi_sr,spi_ier;
    u8 ch,temp;
    static u8 firstrcv = 0;

    if(IntLine == CN_INT_LINE_SPI0)
    {
        pSCB = s_ptSpi1_CB;
        Reg = (tagSpiReg *)tg_SpiReg[CN_SPI1];
        param = &IntParamset1;
    }
    else if(IntLine == CN_INT_LINE_SPI1)
    {
        pSCB = s_ptSpi2_CB;
        Reg = (tagSpiReg *)tg_SpiReg[CN_SPI2];
        param = &IntParamset2;
    }
    else
        return 0;

    spi_sr = Reg->SR;
    spi_ier = Reg->IER;
    //发送中断
    if( (spi_ier & SPI_IER_TXPIE) && (spi_sr & SPI_SR_TXP)) //使能了发送中断，且TXRIS
    {
        temp = SPI_PortRead(pSCB,&ch,1);//是否还有数据需要发送

        if(temp >0)
        {
            param->SendDataLen --;
            param->RecvOffset  --;
            *((__IO u8 *)&Reg->TXDR) = ch;
        }
        else                                            //表明发送结束
        {
            //关发送中断
            __SPI_IntDisable(Reg,SPI_INTEN_TR);
            param->RecvOffset -= param->SendDataLen;
            param->SendDataLen = 0;

            ch = *((__IO u8 *)&Reg->RXDR);

            //发送完成了，判断是否需要接收
            if(param->RecvDataLen > 0)
            {
                //MASTER模式下，发送和接收同时进行
                *((__IO u8 *)&Reg->TXDR) = 0xAA;
                __SPI_IntEnable(Reg,SPI_INTEN_RR);    //使能接收中断
                firstrcv = 1;
            }
        }
    }
    else if((spi_ier & SPI_IER_RXPIE) && (spi_sr & SPI_SR_RXP))    //接收中断
    {
        ch =*((__IO u8 *)&Reg->SPI_RXD);
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
            *((__IO u8 *)&Reg->TXDR) = 0xAA;
        }
        else                    //表明接收已经完成
        {
            param->RecvDataLen = 0;
            __SPI_IntDisable(Reg,SPI_INTEN_RR);
        }
    }

    if(param->RecvDataLen + param->SendDataLen == 0)
    {
        Reg->CR1 &= ~SPI_CR1_SPE;
    }

    return 0;
#endif
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
    Int_IsrConnect(IntLine,SPI_ISRFUNC);
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
    case CN_SPI0:
        SPI_Config.BusName          = "SPI0";
        SPI_Config.SPIBuf           = (u8*)&s_Spi0_Buf;
        IntLine = CN_INT_LINE_SPI0;
        pSpiCB = &s_ptSpi0_CB;
        break;
    case CN_SPI1:
        SPI_Config.BusName          = "SPI1";
        SPI_Config.SPIBuf           = (u8*)&s_Spi1_Buf;
        IntLine = CN_INT_LINE_SPI1;
        pSpiCB = &s_ptSpi1_CB;
        break;
    default:
        return (0);
    }
    SPI_Config.SpecificFlag     = port;
    SPI_Config.SPIBufLen        = CFG_SPI_BUF_LEN;
    SPI_Config.MultiCSRegFlag   = false;
    SPI_Config.pTransferPoll    = (TransferPoll)__SPI_TxRxPoll;
    SPI_Config.pTransferTxRx    = (TransferFunc)__SPI_TransferTxRx;
    SPI_Config.pCsActive        = (CsActiveFunc)__SPI_BusCsActive;
    SPI_Config.pCsInActive      = (CsInActiveFunc)__SPI_BusCsInActive;
    SPI_Config.pBusCtrl         = (SPIBusCtrlFunc)__SPI_BusCtrl;

    spi_init((u32)SPI_Config.SpecificFlag); // SPI寄存器设置

    __SPI_IntConfig(IntLine); // 注册中断
    *pSpiCB = SPI_BusAdd(&SPI_Config);
    if(*pSpiCB) // 将SPI端口添加到SPI总线
        return (1);
    else
        return (0);
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


