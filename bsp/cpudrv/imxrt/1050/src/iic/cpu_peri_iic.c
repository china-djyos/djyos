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
// 模块描述: MIMXRT1052的iic驱动
// 模块版本:
// 创建人员:    chj
// 创建时间: 5/4.2018
// =============================================================================
#include "cpu_peri_iic.h"
#include "fsl_clock.h"
#include "fsl_lpi2c.h"
#include "fsl_iomuxc.h"

#include "cpu_peri_int_line.h"

#include "djyos.h"
#include "int.h"
#include "iicbus.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t IIC_Init(u8 iic_port);
//    #if (CFG_IIC1_ENABLE == 1)
//    IIC_Init(CN_IIC1);
//    #endif
//    #if (CFG_IIC2_ENABLE == 1)
//    IIC_Init(CN_IIC2);
//    #endif
//    #if (CFG_IIC3_ENABLE == 1)
//    IIC_Init(CN_IIC3);
//    #endif
//    #if (CFG_IIC4_ENABLE == 1)
//    IIC_Init(CN_IIC4);
//    #endif
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip iic"//CPU的iic总线驱动
//parent:"iicbus"    //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                  //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable               //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                 //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early                //初始化时机，可选值：early，medium，later, pre-main。
                                 //表示初始化时间，分别是早期、中期、后期
//dependence:"iicbus","int"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                 //选中该组件时，被依赖组件将强制选中，
                                 //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"          //该组件的弱依赖组件名（可以是none，表示无依赖组件），
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
//%$#@num,32,512,
#define CFG_IIC1_BUF_LEN              128           //"IIC1缓冲区大小",
#define CFG_IIC2_BUF_LEN              128           //"IIC2缓冲区大小",
#define CFG_IIC3_BUF_LEN              128           //"IIC3缓冲区大小",
#define CFG_IIC4_BUF_LEN              128           //"IIC4缓冲区大小",
//%$#@enum,true,false,
#define CFG_IIC1_ENABLE               false         //"是否配置使用IIC1",
#define CFG_IIC2_ENABLE               false         //"是否配置使用IIC2",
#define CFG_IIC3_ENABLE               false         //"是否配置使用IIC3",
#define CFG_IIC4_ENABLE               false         //"是否配置使用IIC4",
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure
// =============================================================================

/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_DIVIDER (5U)

#define LPI2C_CLOCK_FREQUENCY ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (LPI2C_CLOCK_SOURCE_DIVIDER + 1U))

typedef LPI2C_Type  tagI2CReg;

static tagI2CReg volatile * const tg_I2CReg[] = {(tagI2CReg *)LPI2C1_BASE,
                                                (tagI2CReg *)LPI2C2_BASE,
                                                (tagI2CReg *)LPI2C3_BASE,
                                                (tagI2CReg *)LPI2C4_BASE};

//struct IIC_IntParamSet
//{
//    struct SemaphoreLCB *pDrvPostSemp;   //信号量
//    u32 TransCount;       //传输数据量计数器
//    u32 TransTotalLen;//接收的数据总量
//};

//static struct IIC_IntParamSet IntParamset[CN_IIC_NUM];
static u32 IIC_BaudRate[CN_IIC_NUM]={100000,100000,100000,100000};
const char *IIC_Name[CN_IIC_NUM]=
         {"IIC1","IIC2","IIC3","IIC4"};

static u32 IIC_BUF_LEN[CN_IIC_NUM]={CFG_IIC1_BUF_LEN,CFG_IIC2_BUF_LEN,\
        CFG_IIC3_BUF_LEN,CFG_IIC4_BUF_LEN};
//iic缓冲区定数组
static u8 s_IIC1Buf[CFG_IIC1_BUF_LEN];
static u8 s_IIC2Buf[CFG_IIC2_BUF_LEN];
static u8 s_IIC3Buf[CFG_IIC1_BUF_LEN];
static u8 s_IIC4Buf[CFG_IIC1_BUF_LEN];

static struct IIC_CB *s_ptIIC_CB[CN_IIC_NUM];
static u8* s_IICBuf[CN_IIC_NUM]={s_IIC1Buf,
                                 s_IIC2Buf,
                                 s_IIC3Buf,
                                 s_IIC4Buf,};

// =============================================================================
// 功能: 主机使能iic中断,接收与发送共用一个中断源。
// 参数: reg,被操作的寄存器组指针
// 返回: 无
// =============================================================================
static void __IIC_MasterIntEnable(tagI2CReg *reg)
{
    /* 发送和接收中断同时使能*/
    LPI2C_MasterEnableInterrupts(reg,kLPI2C_MasterTxReadyFlag|kLPI2C_MasterRxReadyFlag  );
}

// =============================================================================
// 功能: 主机禁止iic中断,接收与发送共用一个中断源。
// 参数: reg,被操作的寄存器组指针
// 返回: 无
// =============================================================================
static void __IIC_MasterIntDisable(tagI2CReg *reg)
{
    LPI2C_MasterDisableInterrupts(reg,kLPI2C_MasterTxReadyFlag|kLPI2C_MasterRxReadyFlag );
}

// =============================================================================
// 功能: 从机使能iic中断,接收与发送共用一个中断源。
// 参数: reg,被操作的寄存器组指针
// 返回: 无
// =============================================================================
static void __IIC_SlaveIntEnable(tagI2CReg *reg)
{
    /* 发送和接收中断同时使能*/
    LPI2C_SlaveEnableInterrupts(reg,kLPI2C_MasterTxReadyFlag|kLPI2C_MasterRxReadyFlag   );
}

// =============================================================================
// 功能: 主机禁止iic中断,接收与发送共用一个中断源。
// 参数: reg,被操作的寄存器组指针
// 返回: 无
// =============================================================================
static void __IIC_SlaveIntDisable(tagI2CReg *reg)
{
    LPI2C_SlaveDisableInterrupts(reg,kLPI2C_MasterTxReadyFlag|kLPI2C_MasterRxReadyFlag  );
}

// =============================================================================
// 功能: 设置IIC时钟
// 返回: 无
// =============================================================================

static bool_t __IIC_ClkSet(void)
{
    CLOCK_SetMux(kCLOCK_Lpi2cMux, LPI2C_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, LPI2C_CLOCK_SOURCE_DIVIDER);
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
            case CN_IIC1:
                IntLine=CN_INT_LINE_LPI2C1;
                break;
            case CN_IIC2:
                IntLine=CN_INT_LINE_LPI2C2;
                break;
            case CN_IIC3:
                IntLine=CN_INT_LINE_LPI2C3;
                break;
            case CN_IIC4:
                IntLine=CN_INT_LINE_LPI2C4;
                break;
            default:return;
        }
        Int_Register(IntLine);
        Int_IsrConnect(IntLine,isr);
        Int_SettoAsynSignal(IntLine);
        Int_ClearLine(IntLine);     //清掉初始化产生的发送fifo空的中断
        Int_RestoreAsynLine(IntLine);
}

// =============================================================================
// 功能: 主机产生停止时序，结束本次与外器件之间的通信
// 参数: reg,iic外设寄存器基址
// 返回: 无
// =============================================================================
static void __IIC_MasterGenerateStop(tagI2CReg *reg)
{
    LPI2C_MasterStop(reg);//产生停止位
}

// =============================================================================
// 功能: 主机产生停止时序，结束本次与外器件之间的通信
// 参数: reg,iic外设寄存器基址
// 返回: 无
// =============================================================================
static void __IIC_SlaveGenerateStop(tagI2CReg *reg)
{
    reg->STAR = LPI2C_STAR_TXNACK_MASK;//产生停止位
}

// =============================================================================
// 功能：主机IIC写
// 参数： reg,寄存器基址
//        devaddr,器件物理地址，最后一bit区分读写
//         memaddr寄存器地址写缓冲
//       maddrlen 寄存器地址长度
//         buf写缓冲
//       len 长度
// 返回：true/false
// =============================================================================
static bool_t __IIC_MasterWrite(tagI2CReg *reg,u8 devaddr, u32 memaddr,u8 maddrlen,\
                                                u8 *buf, u32 len)
{
    lpi2c_master_transfer_t masterXfer = {0};
    status_t reVal = kStatus_Fail;

    if(maddrlen>4)
        return false;

    masterXfer.slaveAddress = devaddr & 0xFE;
    masterXfer.direction = kLPI2C_Write;
    masterXfer.subaddress = memaddr;
    masterXfer.subaddressSize = maddrlen;
    masterXfer.data = buf;
    masterXfer.dataSize = len;
    masterXfer.flags = kLPI2C_TransferDefaultFlag;

    reVal = LPI2C_MasterTransferBlocking(reg, &masterXfer);
    if(reVal==kStatus_Success)
        return true;
    else
        return false;
}

// =============================================================================
// 功能：从机IIC写
// 参数： reg,寄存器基址
//        devaddr,器件物理地址，最后一bit区分读写
//         addr_buf寄存器地址写缓冲
//       addr_len 寄存器地址长度
//         buf写缓冲
//       len 长度
// 返回：true/false
// =============================================================================
//static bool_t __IIC_SlaveWrite(tagI2CReg *reg,u8 devaddr, u8 *addr_buf, u32 addr_len,u8 *buf, u32 len)
//{
//  lpi2c_master_transfer_t masterXfer = {0};
//  status_t reVal = kStatus_Fail;
//  u8 i=0;
//
//  if(addr_len>4)
//      return false;
//
//  masterXfer.slaveAddress = devaddr & 0xFE;
//  masterXfer.direction = kLPI2C_Write;
//  masterXfer.subaddress = 0;
//  for(i=0;i<addr_len;i++)
//  {
//      masterXfer.subaddress |= addr_buf[i]<<(8*i);
//  }
//  masterXfer.subaddressSize = addr_len;
//  masterXfer.data = buf;
//  masterXfer.dataSize = len;
//  masterXfer.flags = kLPI2C_TransferDefaultFlag;
//
//  reVal = LPI2C_MasterTransferBlocking(reg, &masterXfer);
//  if(reVal==kStatus_Success)
//      return true;
//  else
//      return false;
//}

// =============================================================================
// 功能：轮询方式读IIC从设备的数据
// 参数：reg,寄存器基址
//       devaddr,器件物理地址，最后一bit区分读写
//         memaddr//寄存器地址
//         maddrlen的寄存器地址的长度
//       addr,访问的寄存器地址
//       buf, 存储缓冲区
//       len, 存储缓冲区的长度
// 返回：len,读取成功;-1,读取失败
// =============================================================================
static bool_t __IIC_MasterRead(tagI2CReg *reg,u8 devaddr,u32 memaddr,u8 maddrlen,\
                                                u8 *buf, u32 len)
{
    lpi2c_master_transfer_t masterXfer = {0};
    status_t reVal = kStatus_Fail;

    if(maddrlen>4)
        return false;

    masterXfer.slaveAddress = devaddr & 0xFE;
    masterXfer.direction = kLPI2C_Read;
    masterXfer.subaddress = memaddr;
    masterXfer.subaddressSize = maddrlen;
    masterXfer.data = buf;
    masterXfer.dataSize = len;
    masterXfer.flags = kLPI2C_TransferDefaultFlag;
    reVal=LPI2C_MasterTransferBlocking(reg,&masterXfer);
    if(reVal==kStatus_Success)
        return true;
    else
        return false;
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
static bool_t __IIC_MasterWriteReadPoll(tagI2CReg *reg,u8 DevAddr,u32 MemAddr,\
                                u8 MemAddrLen,u8* Buf, u32 Length,u8 WrRdFlag)
{
    static bool_t flag;

    if(WrRdFlag == CN_IIC_WRITE_FLAG)   //写
    {
        if(true == __IIC_MasterWrite(reg,DevAddr,MemAddr,MemAddrLen,Buf,Length))
            flag = true;
        else
            flag = false;
    }
    else                                //读
    {
        if(true == __IIC_MasterRead(reg,DevAddr,MemAddr,MemAddrLen,Buf,Length))
            flag = true;
        else
            flag = false;
    }

    return  flag ;
}

// =============================================================================
// 功能：IIC总线控制回调函数，被上层调用，
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
              break;
        case CN_IIC_DMA_USED://使用dma传输

            break;
        case CN_IIC_DMA_UNUSED://禁止dma传输
            break;
        case CN_IIC_SET_POLL:           //使用轮询方式发送接收
            break;
        default:
            return 0;
    }
    return 1;
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
bool_t IIC_Init(u8 iic_port)
{
    struct IIC_Param IIC_Config;
    lpi2c_master_config_t masterConfig = {0};

    IIC_Config.BusName             = (char *)IIC_Name[iic_port];
    IIC_Config.IICBuf              = (u8*)s_IICBuf[iic_port];
    IIC_Config.IICBufLen           = IIC_BUF_LEN[iic_port];
    IIC_Config.SpecificFlag        = (ptu32_t)tg_I2CReg[iic_port];
    IIC_Config.pGenerateWriteStart = (WriteStartFunc)NULL;
    IIC_Config.pGenerateReadStart  = (ReadStartFunc)NULL;
    IIC_Config.pGenerateEnd        = (GenerateEndFunc)NULL;
    IIC_Config.pBusCtrl            = (IICBusCtrlFunc)NULL;
    IIC_Config.pWriteReadPoll      = (WriteReadPoll)__IIC_MasterWriteReadPoll;

    __IIC_ClkSet();

    LPI2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Hz = IIC_BaudRate[iic_port];
    LPI2C_MasterInit((tagI2CReg*)tg_I2CReg[iic_port], &masterConfig, LPI2C_CLOCK_FREQUENCY);
    s_ptIIC_CB[iic_port] = IIC_BusAdd(&IIC_Config);
        if(s_ptIIC_CB[iic_port])
            return false;
    return true;
}

