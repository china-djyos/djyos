//-----------------------------------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

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
// 这份授权条款，在使用者符合以下三条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。
// 3. 本软件作为都江堰操作系统的组成部分，未获事前取得的书面许可，不允许移植到非
//    都江堰操作系统环境下运行。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <sys/socket.h>
#include "djyos.h"
#include "lan8720.h"
//#include "pcf8574.h"
#include "cpu_peri.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//  extern void ModuleInstall_InitNet( );
//    ModuleInstall_InitNet( );
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"netconfig"    //网络配置
//parent:"tcpip"                //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"cpu_peri_eth","lan8720","tcpip"     //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //如果依赖多个组件，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#ifndef CFG_STATIC_IP   //****检查参数是否已经配置好
#warning    “网络配置”组件参数未配置，使用默认值
//%$#@num,0,100
//%$#@enum,true,false
#define CFG_STATIC_IP       true            //"IP属性",true=使用静态IP，false=动态IP
//%$#@string,1,16
#define CFG_NETCARD_NAME    "NUCLEO_H743_ETH"    //"网卡名",
//%$#@string,7,15
#define CFG_MY_IPV4         "192.168.0.179" //"静态IP",
#define CFG_MY_SUBMASK      "255.255.255.0" //"子网掩码",
#define CFG_MY_GATWAY       "192.168.0.1"   //"网关",
#define CFG_MY_DNS          "192.168.0.1"   //"DNS",
//%$#@select
//%$#@free
#endif
//%$#@end configue  ****参数配置结束


static u8   gc_NetMac[CN_MACADDR_LEN] ={0x00,0x02,0x03,0x04,0x05,0x06};
static lan8720_IOCtx_t  LAN8720_IOCtx = {ETH_PHY_IO_Init,
                                         ETH_PHY_IO_DeInit,
                                         ETH_PHY_IO_WriteReg,
                                         ETH_PHY_IO_ReadReg};

__attribute__((weak)) void GetCpuSignature(void *buf,int len)
{
    //this is for the stm32f7
    vu8   *base;
    int i = 0;
    base = (vu8 *)0x1FF0F420;   //THE ADDRESS OF F7 F4 F1 IS NOT THE SAME
    for(i = 0;i < len;i++)
    {
        *((vu8 *)buf +i)=*(base+i);
    }
    return;
}

// HAL库中调用了该函数
void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOA_CLK_ENABLE();           //开启GPIOA时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();           //开启GPIOB时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();           //开启GPIOC时钟
    __HAL_RCC_GPIOG_CLK_ENABLE();           //开启GPIOG时钟
    __HAL_RCC_ETH1MAC_CLK_ENABLE();         //使能ETH1 MAC时钟
    __HAL_RCC_ETH1TX_CLK_ENABLE();          //使能ETH1发送时钟
    __HAL_RCC_ETH1RX_CLK_ENABLE();          //使能ETH1接收时钟

    GPIO_Initure.Pin=GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_7;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //推挽复用
    GPIO_Initure.Pull=GPIO_NOPULL;              //不带上下拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //高速
    GPIO_Initure.Alternate=GPIO_AF11_ETH;       //复用为ETH功能
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);         //初始化

    //PB11
    GPIO_Initure.Pin=GPIO_PIN_11;               //PB11
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);         //始化

    //PC1,4,5
    GPIO_Initure.Pin=GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5; //PC1,4,5
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);         //初始化

    //PG13,14
    GPIO_Initure.Pin=GPIO_PIN_13|GPIO_PIN_14;   //PG13,14
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);         //初始化
}

bool_t PHY_Init(void)
{
    uint32_t duplex, speed = 0;
    int32_t PHYLinkState;
    ETH_MACConfigTypeDef MACConf;
    /* Set PHY IO functions */
//    Djy_DelayUs(500*1000);
//    PCF8574_Init();                         //初始化PCF8574
    LAN8720_RegisterBusIO(&LAN8720_IOCtx);
    Djy_DelayUs(1000);
    PCF8574_WriteBit(ETH_RESET_IO,1);       //硬件复位
    Djy_DelayUs(50*1000);
    PCF8574_WriteBit(ETH_RESET_IO,0);       //复位结束
    Djy_DelayUs(50*1000);
    LAN8720_ResetInit();
    PHYLinkState = LAN8720_GetLinkState();
    switch(PHYLinkState)
    {
        case LAN8720_STATUS_100MBITS_FULLDUPLEX:
            duplex = ETH_FULLDUPLEX_MODE;
            speed = ETH_SPEED_100M;
            break;
        case LAN8720_STATUS_100MBITS_HALFDUPLEX:
            duplex = ETH_HALFDUPLEX_MODE;
            speed = ETH_SPEED_100M;
                break;
        case LAN8720_STATUS_10MBITS_FULLDUPLEX:
            duplex = ETH_FULLDUPLEX_MODE;
            speed = ETH_SPEED_10M;
                break;
        case LAN8720_STATUS_10MBITS_HALFDUPLEX:
            duplex = ETH_HALFDUPLEX_MODE;
            speed = ETH_SPEED_10M;
                break;
        default:
              duplex = ETH_FULLDUPLEX_MODE;
              speed = ETH_SPEED_100M;
              break;
    }
    djybsp_eth_get_mac_config(&MACConf);
    MACConf.DuplexMode = duplex;
    MACConf.Speed = speed;
    djybsp_eth_set_mac_config( &MACConf);
    djybsp_eth_start();

    return true;
}

//please refers the following function in the module-trim in proper place.
//make sure that that os kernel has been initialize ok and the interrupt system intialize ok
//make sure that that board net hardware has initialized and could work ok
void ModuleInstall_InitNet(void)   //static ip example
{
    //GET THE SIGNATURE OF THE DEVICE
    u32  signature[4];
    memset(signature,0,sizeof(signature));
    GetCpuSignature((u8 *)signature,sizeof(signature));
    printk("CPU SIGNATURE:%08X-%08X-%08X-%08X\n\r",signature[0],signature[1],signature[2],signature[3]);
    //use the signature as the mac address
    signature[0] = signature[1]+signature[2]+signature[3];
    memcpy(gc_NetMac,&signature[0],CN_MACADDR_LEN);
    gc_NetMac[0]=0x00;      //根据mac的规定，第一字节某位置为1表示广播或者组播
    //install the net device you used,you could use more than one, but they
    //has different names and macs
    //use the corresponding net device install function you use
//    extern bool_t ModuleInstall_TcpIp(void);
//    ModuleInstall_TcpIp();

    extern bool_t ModuleInstall_ETH(const char *devname, u8 *mac,\
            bool_t loop,u32 loopcycle,\
            bool_t (*rcvHook)(u8 *buf, u16 len));
    ModuleInstall_ETH(CFG_NETCARD_NAME,gc_NetMac,false,1*mS,NULL);
    if(PHY_Init()==false)
    {
        return;
    }
#if CFG_STATIC_IP == 1
    tagHostAddrV4  ipv4addr;
    //we use the static ip we like
    memset((void *)&ipv4addr,0,sizeof(ipv4addr));
    ipv4addr.ip      = inet_addr(CFG_MY_IPV4);
    ipv4addr.submask = inet_addr(CFG_MY_SUBMASK);
    ipv4addr.gatway  = inet_addr(CFG_MY_GATWAY);
    ipv4addr.dns     = inet_addr(CFG_MY_DNS);
    ipv4addr.broad   = inet_addr("192.168.0.255");
    if(RoutCreate(CFG_NETCARD_NAME,EN_IPV_4,(void *)&ipv4addr,CN_ROUT_NONE))
    {
        printk("%s:CreateRout:%s:%s success\r\n",__FUNCTION__,CFG_NETCARD_NAME,inet_ntoa(ipv4addr.ip));
    }
    else
    {
        printk("%s:CreateRout:%s:%s failed\r\n",__FUNCTION__,CFG_NETCARD_NAME,inet_ntoa(ipv4addr.ip));
    }
    //下一个路由，用于生产测试用，利用CPU ID 随机生成主机地址，网络地址用 192.168.1
    //WE WILL ADD A ROUT DIFFERENT FOR EACH DEVICE USE THE CPU SIGNATURE
    //USE THE NET:192.168.1.xx
    u8 value8 = 0;
    value8 = +((u8)signature[0]>>0)+((u8)signature[0]>>8)+((u8)signature[0]>>16)+((u8)signature[0]>>24);
    if((value8==0)||(value8==1)||(value8==255))
    {
        value8=253;
    }
    u32 value32 = 0;
    memset((void *)&ipv4addr,0,sizeof(ipv4addr));
    value32 = inet_addr("192.168.1.0");
    value32 = ntohl(value32);
    value32 =(value32&0xffffff00) + value8;
    ipv4addr.ip      = htonl(value32);
    ipv4addr.submask = inet_addr("255.255.255.0");
    ipv4addr.gatway  = inet_addr("192.168.1.1");
    ipv4addr.dns     = inet_addr("192.168.1.1");
    ipv4addr.broad   = inet_addr("192.168.1.255");
    if(RoutCreate(CFG_NETCARD_NAME,EN_IPV_4,(void *)&ipv4addr,CN_ROUT_NONE))
    {
        printk("%s:CreateRout:%s:%s success\r\n",__FUNCTION__,CFG_NETCARD_NAME,inet_ntoa(ipv4addr.ip));
    }
    else
    {
        printk("%s:CreateRout:%s:%s failed\r\n",__FUNCTION__,CFG_NETCARD_NAME,inet_ntoa(ipv4addr.ip));
    }

#else
    if(DhcpAddClientTask(CFG_NETCARD_NAME))
    {
       printk("%s:Add %s success\r\n",__FUNCTION__,CFG_NETCARD_NAME);
    }
    else
    {
        printk("%s:Add %s failed\r\n",__FUNCTION__,CFG_NETCARD_NAME);
    }

#endif
    //do the lan8720 chip set

//    bool_t ETH_RESE(void);
//    ETH_RESE( );//网口复位
//
//    bool_t lan8720Init(void);
//    lan8720Init( );

    return;
}

