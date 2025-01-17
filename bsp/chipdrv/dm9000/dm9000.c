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
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
// 文件名     ：dm9000.c
// 模块描述: DM9000网卡驱动，按DJYOS的以太网协议实现要求实现的网卡底层驱动
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 18/09.2014
// =============================================================================

#include "stdint.h"
#include <tcpip/netdev.h>
#include "rout.h"
#include "dm9000.h"
#include "cpu_peri.h"
#include "stdio.h"
#include "string.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t DM9000_ModuleInit(ptu32_t para);
//    DM9000_ModuleInit(0);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"ethernet dm9000"//dm9000
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                     //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                  //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                    //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium                  //初始化时机，可选值：early，medium，later, pre-main。
                                    //表示初始化时间，分别是早期、中期、后期
//dependence:"lock","heap","tcpip"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                    //选中该组件时，被依赖组件将强制选中，
                                    //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"             //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                    //选中该组件时，被依赖组件不会被强制选中，
                                    //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                    //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_ETHERNET_DM9000 == false )
//#warning  " ethernet_dm9000  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_ETHERNET_DM9000    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#define CFG_DMA9000_ADDR          0x20000300//"地址口"，dm9000地址口
#define CFG_DMA9000_DATA          0x20000304//"数据口"，dm9000数据口
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

// =============================================================================
//#define DM_ADDR_PORT (*((volatile unsigned short *) 0x20000300)) //地址口
//#define DM_DATA_PORT (*((volatile unsigned short *) 0x20000304)) //数据口

#define DM_ADDR_PORT (*((volatile unsigned short *) CFG_DMA9000_ADDR)) //地址口
#define DM_DATA_PORT (*((volatile unsigned short *) CFG_DMA9000_ADDR)) //数据口

#define DM9000_DBG   printk
#define CN_MAC_SEMP_TIMEOUT    (100*mS)
#define CN_PHY_INTIT_TIMEOUT   (3000*mS)
// =============================================================================
#define CN_HARD_BOARD            0x02
static u8  sgNetHardMac[6] = {0x3C,0x97, 0x0E, 0x44, 0x07, 0XBD};
static u32  sgNetHardIpAddrMain = 0xC0A80100;
static u32  sgNetHardIpMsk = 0xFFFFFF00 ; //255.255.255.0
static u32  sgNetHardGateWay = 0xC0A80101; //192.168.1.1
static tagRoutTab  *ptNetRout = NULL;
static struct NetDev   *ptNetDev = NULL;
#define CN_PKG_MAX_LEN   1500
// =============================================================================

//写DM9000寄存器
static void dm_reg_write(unsigned char reg, unsigned char data)
{
    DM_ADDR_PORT = reg;            //将寄存器地址写到地址端口
    DM_DATA_PORT = data;            //将数据写到数据端口
}

//读DM9000寄存器
static unsigned char dm_reg_read(unsigned char reg)
{
    DM_ADDR_PORT = reg;
    return DM_DATA_PORT;             //将数据从数据端口读出
}

//读DM9000PHY寄存器
static u16 dm_reg_read_phy(u16 reg)
{
    u16 data;
    dm_reg_write(DM9000_EPAR, reg|0x40);
    dm_reg_write(DM9000_EPCR, 0x0c);            //phy读命令
    data = dm_reg_read(DM9000_EPDRH);           //读phy
    data = (data<<8) | dm_reg_read(DM9000_EPDRL);
    DJY_DelayUs(20);
    dm_reg_write(DM9000_EPCR, 0x08);            //清除读phy操作
    return data;
}
//向DM9000PHY寄存器写数据
static void dm_reg_write_phy(u16 reg, u16 data)
{
    dm_reg_write(DM9000_EPAR, reg|0x40);
    dm_reg_write(DM9000_EPDRH, (data>>8)&0xff);
    dm_reg_write(DM9000_EPDRL, data&0xff);
    dm_reg_write(DM9000_EPCR, 0x0a);            //phy写命令
    DJY_DelayUs(20);
    dm_reg_write(DM9000_EPCR, 0x08);            //清除PHY读操作
}

// =============================================================================
// 功能：读取DM9000的ID，若读到的ID不正确，则报警，并打印信息，该函数返回false，则
//       初始化网卡失败，模块直接返回
// 参数：无
// 返回：true,成功;false,失败
// 说明：该函数返回成功，实质说明了CPU与硬件之间的通信正常
// =============================================================================
bool_t DM9000_Probe(void)
{
    unsigned int id_val;

    id_val  = dm_reg_read(DM9000_VIDL);
    id_val |= dm_reg_read(DM9000_VIDH) << 8;
    id_val |= dm_reg_read(DM9000_PIDL) << 16;
    id_val |= dm_reg_read(DM9000_PIDH) << 24;
    if (id_val == DM9000_ID)
    {
        DM9000_DBG("dm9000 i/o:  id: 0x%x \r\n",id_val);
        return true;
    }
    else
    {
        DM9000_DBG("dm9000 not found !\r\n");
        return false;
    }
}

// =============================================================================
// 功能：DM9000PHY芯片复位，并配置为自适应模式
// 参数：无
// 返回：无
// =============================================================================
bool_t DM9000_PhyInit(void)
{
    u32 timeout = CN_PHY_INTIT_TIMEOUT;
    dm_reg_write(DM9000_GPCR,1);            //设置GPIO0为输出
    dm_reg_write(DM9000_GPR,0);             //激活内部PHY
    DJY_DelayUs(3 * mS);

    dm_reg_write_phy(DM9000_BMCR,0x8000);
    DJY_DelayUs(30);
    while(dm_reg_read_phy(DM9000_BMCR) & 0x8000)
    {
        DJY_DelayUs(10*mS);
        timeout = timeout - 10*mS;
        if(timeout == 0)
        {
            DM9000_DBG("phy init failed !\r\n");
            return false;
        }
    }

    DM9000_DBG("Phy Reset Success!\r\n");

    dm_reg_write_phy(DM9000_BMCR,0x1200);//auto negotiation
    DJY_DelayUs(30);
    while(!(dm_reg_read_phy(DM9000_BMSR) & 0x20))
    {
        DJY_DelayUs(10*mS);
        timeout = timeout - 10*mS;
        if(timeout == 0)
        {
            DM9000_DBG("phy init failed !\r\n");
            return false;
        }
    }
    DM9000_DBG("auto negotiation completed!\r\n");

    while(!(dm_reg_read_phy(DM9000_BMSR) & 0x04))
    {
        DJY_DelayUs(10*mS);
        timeout = timeout - 10*mS;
        if(timeout == 0)
        {
            DM9000_DBG("phy init failed !\r\n");
            return false;
        }
    }
    DM9000_DBG("phy linked!\r\n");
    return true;
}

// =============================================================================
// 功能：DM9000网卡芯片复位
// 参数：无
// 返回：无
// =============================================================================
void DM9000_reset(void)
{
    DM9000_DBG("resetting\n");
    dm_reg_write(DM9000_NCR, NCR_RST);
    DJY_DelayUs(3000);      /* delay 3ms */
    dm_reg_write(DM9000_NCR, 0x00);

    dm_reg_write(DM9000_NCR, NCR_RST);
    DJY_DelayUs(3000);      /* delay 3ms */
    dm_reg_write(DM9000_NCR, 0x00);
}

// =============================================================================
// 功能：DM9000以太网网卡硬件初始化，包括复位网卡、清发送和接收状态、配置MAC地址、
//      使能接收中断信息输出（若需产生中断，还需配置中断线）
// 参数：无
// 返回：
// =============================================================================
bool_t DM9000_HardInit(void)
{
    dm_reg_write(DM9000_NCR,1);             //软件复位DM9000
    DJY_DelayUs(30);                        //延时至少20μs
    dm_reg_write(DM9000_NCR,0);             //清除复位位

    dm_reg_write(DM9000_NCR,1);             //为了确保复位正确，再次复位
    DJY_DelayUs(30);
    dm_reg_write(DM9000_NCR,0);

    if(false == DM9000_PhyInit())                       //PHY芯片初始化
        return false;

    dm_reg_write(DM9000_NSR,0x2c);           //清TX状态
    dm_reg_write(DM9000_ISR,0xf);            //清中断状态

    dm_reg_write(DM9000_RCR,0x39);           //设置RX控制
    dm_reg_write(DM9000_TCR,0);              //设置TX控制
    dm_reg_write(DM9000_BPTR,0x3f);
    dm_reg_write(DM9000_FCTR,0x3a);
    dm_reg_write(DM9000_FCR,0xff);
    dm_reg_write(DM9000_SMCR,0x00);

    dm_reg_write(DM9000_PAR+0,sgNetHardMac[0]);  //设置MAC地址
    dm_reg_write(DM9000_PAR+1,sgNetHardMac[1]);
    dm_reg_write(DM9000_PAR+2,sgNetHardMac[2]);
    dm_reg_write(DM9000_PAR+3,sgNetHardMac[3]);
    dm_reg_write(DM9000_PAR+4,sgNetHardMac[4]);
    dm_reg_write(DM9000_PAR+5,sgNetHardMac[5]);

    dm_reg_write(DM9000_NSR,0x2c);           //再次清TX状态
    dm_reg_write(DM9000_ISR,0xf);            //再次清中断状态

    dm_reg_write(DM9000_IMR,0x81);           //打开接受数据中断

    while(!(dm_reg_read(DM9000_NSR) & NSR_LINKST));

    DM9000_DBG("DM9000 Linked!\r\n");

    return true;
}

// =============================================================================
// 功能：DM9000中断初始化。DM9000中断引脚接到CPU的GPIO引脚，因此需将该引脚配置为外部
//       中断，并配置触发电平；同时需连接中断线到中断系统
// 参数：无
// 返回值  ：
// =============================================================================
u32 DM9000_ISR_Handler(ptu32_t IntLine);
void DM9000_IntInit(void)
{
    //配置外部引脚EINT7/GPF7
    pg_gpio_reg->GPFCON &= ~(3<<14);
    pg_gpio_reg->GPFCON |= (2<<14);     //配置为输入
    pg_gpio_reg->EXTINT0 &= ~(7<<28);
    pg_gpio_reg->EXTINT0  |= (4<<28);   //配置上升沿触发
    pg_gpio_reg->EINTPEND |= (1<<7);
    pg_gpio_reg->EINTMASK &= (~(01<<7));

    //DM9000的中断引脚输出中断信号到CPU的EINT7
    Int_Register(cn_int_line_eint4_7);
    Int_IsrConnect(cn_int_line_eint4_7,DM9000_ISR_Handler);
    Int_SettoAsynSignal(cn_int_line_eint4_7);
    Int_ClearLine(cn_int_line_eint4_7);
    Int_RestoreAsynLine(cn_int_line_eint4_7);
}

// =============================================================================
// 功能：DM9000发送数据包函数，将数据包发送到网卡，并启动发送，完成后返回，采用轮询
//       的方式发送
// 参数：无
// 返回值  ：
// =============================================================================
void DM9000_TxPacket(unsigned char *datas, int length)
{
    int i;

    dm_reg_write(DM9000_IMR, 0x80);                 //在发送数据过程中禁止网卡中断

    dm_reg_write(DM9000_TXPLH, (length>>8) & 0x0ff);//设置发送数据长度
    dm_reg_write(DM9000_TXPLL, length & 0x0ff);

    DM_ADDR_PORT = DM9000_MWCMD;                    //发送数据缓存赋予数据端口

    //发送数据
    for(i=0;i<length;i+=2)
    {
//      DJY_DelayUs(50);
        DM_DATA_PORT = datas[i]|(datas[i+1]<<8);  //8位数据转换为16位数据输出
    }

    dm_reg_write(DM9000_TCR, 0x01);                 //把数据发送到以太网上

    while((dm_reg_read(DM9000_NSR) & 0x0c) == 0)
    ;                                               //等待数据发送完成

//  DJY_DelayUs(50);

    dm_reg_write(DM9000_NSR, 0x2c);                 //清除TX状态
    dm_reg_write(DM9000_IMR, 0x81);                 //打开DM9000接收数据中断
}

//static s64   timedriverstart;
//static s64   timedriverend;
//static u32   timedriverused;
// =============================================================================
// 函数功能：NetHard_Send
//          网卡发送数据包
// 输入参数：netdev,使用的网卡
//        pkg,待发送的数据包
//        netdevtask,网卡需要完成的工作
// 输出参数：
// 返回值  ：true发送成功  false发送失败。
// 说明    ：采用拷贝的方式发送，后续考虑使用链表发送
// =============================================================================
bool_t DM9000_Send(struct NetDev *dev,struct NetPkg *pkg,u32 netdevtask)
{
    bool_t  result;
    struct NetPkg *tmp;
    u16 *mysrc;
    u16 sndlen;
    u16 i;
    atom_low_t atom;

    result = false;
    if((ptNetDev == dev)&&(NULL != pkg))
    {
        sndlen = 0;
        tmp = pkg;
        //cout the len
        tmp = pkg;
        sndlen = 0;
        while(NULL != tmp)
        {
            sndlen +=PkgGetDataLen(tmp);
//          sndlen +=tmp->datalen;

//          if(tmp->pkgflag&CN_PKLGLST_END)
            if(PkgIsBufferEnd(tmp))
            {
                tmp = NULL;
            }
            else
            {
                tmp = PkgGetNextUnit(tmp);
            }
        }
        atom = Int_LowAtomStart();
//        timedriverstart = DJY_GetSysTime();
        //snd all the pkg
        tmp = pkg;
        //init the dm9000
        dm_reg_write(DM9000_IMR, 0x80);                 //在发送数据过程中禁止网卡中断

        dm_reg_write(DM9000_TXPLH, (sndlen>>8) & 0x0ff);//设置发送数据长度
        dm_reg_write(DM9000_TXPLL, sndlen & 0x0ff);

        DM_ADDR_PORT = DM9000_MWCMD;                    //发送数据缓存赋予数据端口


        while(NULL!= tmp)
        {
            sndlen = PkgGetDataLen(tmp);
//          sndlen = tmp->datalen;
            mysrc = (u16 *)PkgGetCurrentBuffer(tmp);
//          mysrc = (u16 *)(tmp->buf + tmp->offset);
            //发送数据
            for(i=0;i<sndlen;i+=2)
            {
                DM_DATA_PORT = *mysrc++;  //8位数据转换为16位数据输出
            }
            if(PkgIsBufferEnd(tmp))
            {
                tmp = NULL;
            }
            else
            {
                tmp = PkgGetNextUnit(tmp);
            }
        }
        //ok now start transfer;
        dm_reg_write(DM9000_TCR, 0x01);                 //把数据发送到以太网上

        while((dm_reg_read(DM9000_NSR) & 0x0c) == 0)
        ;                                               //等待数据发送完成
        dm_reg_write(DM9000_NSR, 0x2c);                 //清除TX状态
        dm_reg_write(DM9000_IMR, 0x81);                 //打开DM9000接收数据中断

//        timedriverend = DJY_GetSysTime();
        Int_LowAtomEnd(atom);
//        timedriverused = (u32)(timedriverend- timedriverstart);
//        printk("%s:TimeUsed = %d\n\r",__FUNCTION__, timedriverused);
        result = true;
    }

    return result;
}
// =============================================================================
// 函数功能：NetHard_Rcv
//          网卡读数据函数
// 输入参数：无
// 输出参数：
// 返回值  ：
// 说明    ：网卡读数据
// =============================================================================
struct NetPkg *DM9000_RcvPacket(struct NetDev *netdev)
{
    s32   rx_length,i;
    u8    rx_ready;
    u16 rx_status;
    struct NetPkg *pkg=NULL;

    u16 *dst;
    atom_low_t atom;
    atom = Int_LowAtomStart();

    rx_ready = dm_reg_read(DM9000_MRCMDX);      //先读取一个无效的数据
    rx_ready = (unsigned char)DM_DATA_PORT;     //真正读取到的数据包首字节

    if(rx_ready == 1)                           //判读首字节是否为1或0
    {
        DM_ADDR_PORT = DM9000_MRCMD;            //连续读取数据包内容
        rx_status = DM_DATA_PORT;                          //状态字节
        rx_length = DM_DATA_PORT;                          //数据长度
        rx_status &= 0xff;

        if((rx_length > 0) && (rx_length <= CN_PKG_MAX_LEN))
        {
            pkg =PkgMalloc(rx_length,0);
        }
        if(pkg != NULL)
        {
            dst = (u16 *)PkgGetCurrentBuffer(pkg);
//          dst = (u16 *)(pkg->buf + pkg->offset);
            if(!(rx_status & 0xbf00) && (rx_length < 10000))//判读数据是否符合要求
            {
                for(i=0; i<rx_length; i+=2)          //16位数据转换为8位数据存储
                {
                    *dst++ = DM_DATA_PORT;
                }
            }
            PkgSetDataLen(pkg, rx_length);
//          pkg->datalen = rx_length;
        }
    }
    Int_LowAtomEnd(atom);

    return pkg;
}


static u16 sgDm9000RcvTaskID = CN_EVTT_ID_INVALID;
static struct MutexLCB *pDm9000RcvSync;
// =============================================================================
// 功能：DM9000中断服务函数，接收中断时，调用NetDev_PostDataRcvSignal，表示网卡接收
//       到数据信息
// 参数：中断线号
// 返回 ：
// =============================================================================
u32 DM9000_ISR_Handler(ptu32_t IntLine)
{
    u8 IntStatus;

    dm_reg_write(DM9000_IMR, 0x80);                 //首先禁止网卡中断，CPU端OS已经做了

    if(pg_gpio_reg->EINTPEND & (1<<7))              //DM9000外部中断标志
    {
        pg_gpio_reg->EINTPEND |= (1<<7);            //清外部中断标志

        IntStatus = dm_reg_read(DM9000_ISR);        //读取ISR
        if(IntStatus & ISR_PRS)                     //接收中断
        {
            Lock_SempPost(pDm9000RcvSync);
        }
        dm_reg_write(DM9000_ISR, IntStatus);       //清中断
    }

    dm_reg_write(DM9000_IMR, 0x81);                 //恢复网卡中断，CPU端OS已经做了

    return 0;
}


ptu32_t Dm9000Rcv(void)
{
    struct NetPkg *pkg;

    while(1)
    {
        if(Lock_SempPend(pDm9000RcvSync,CN_TIMEOUT_FOREVER))
        {
            while((pkg = DM9000_RcvPacket(ptNetDev))!= NULL)
            {
                NetDev_PostPkg(ptNetDev,pkg);
                PkgTryFreePart(pkg);
            }
        }
    }
    return 0;
}


bool_t Dm9000RcvTask()
{
    bool_t result = false;
    u16 evttID;
    u16 eventID;
    evttID = DJY_EvttRegist(EN_CORRELATIVE, CN_PRIO_CRITICAL, 0, 1,
        (ptu32_t (*)(void))Dm9000Rcv,NULL, 0x1000, "Dm900RcvTask");
    if (evttID != CN_EVTT_ID_INVALID)
    {
        eventID=DJY_EventPop(evttID, NULL, 0, 0, 0, 0);
        if(eventID != CN_EVENT_ID_INVALID)
        {
            result = true;
            sgDm9000RcvTaskID = evttID;
            pDm9000RcvSync = Lock_SempCreate(1,0,CN_BLOCK_FIFO,NULL);
        }
        else
        {
            DJY_EvttUnregist(evttID);
        }
    }
    return result;
}

// =============================================================================
// 函数功能：NetHard_AddNetDev
//          向协议栈添加一个网卡设备
// 输入参数：无
// 输出参数：无
// 返回值  ：true成功  false失败。
// 说明    ：1,添加设备
//        2,添加路由
// =============================================================================
bool_t DM9000_AddNetDev(void)
{
    bool_t  result;
    struct NetDevPara  devpara;
    tagHostIpv4Addr devaddr;

    devpara.ifsend = DM9000_Send;
    devpara.iftype = EN_LINK_ETHERNET;
    devpara.devfunc = 0x00;//0xFF;//所有属性都配置上
    memcpy(devpara.mac, sgNetHardMac,6);
    devpara.name = "DM9000NetDriver";
    devpara.Private = 0;
    devpara.linklen = 14;
    devpara.pkglen = 1500;

    if(false == Dm9000RcvTask())
    {
        printk("%s:Create Rcv Task Failed\n\r",__FUNCTION__);
    }

    result = false;
    ptNetDev = NetDev_AddDev(&devpara);
    if(ptNetDev != NULL)
    {
//        devaddr.ip = sgNetHardIpAddrMain|CN_HARD_BOARD;
//        devaddr.gateway = sgNetHardGateWay;
//        devaddr.ipmsk = sgNetHardIpMsk;
        devaddr.ip = 0;
        devaddr.gateway = 0;
        devaddr.ipmsk = 0;
        ptNetRout = Rout_AddRout(ptNetDev, &devaddr);
        if(NULL != ptNetRout)
        {
            Dhcp_AddTask(ptNetRout);
            result = true;
        }
    }

    return result;
}

// =============================================================================
// 功能：DM9000网卡和DJYIP驱动初始化函数
// 参数：para
// 返回值  ：true成功  false失败。
// =============================================================================
bool_t DM9000_ModuleInit(ptu32_t para)
{
    bool_t result = false;

    if(true == DM9000_Probe())  //网卡检测成功
    {
        if(true == DM9000_HardInit())
        {
            DM9000_IntInit();
            result = DM9000_AddNetDev();
        }
    }

    return result;
}

