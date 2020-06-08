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

// 文件名     ：stm32_iic_type1.c
// 作者         ：czz
// 模块描述: IIC模块底层硬件驱动，寄存器级别的操作
// 创建时间: 2018/08/16
// =============================================================================
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
//==============================================================================
//=========================DIDE config==========================================
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//bool_t ModuleInstall_IIC(ptu32_t port);
//    #if CFG_IIC1_ENABLE== true
//     ModuleInstall_IIC(CN_IIC1);
//    #endif
//    #if CFG_IIC2_ENABLE== true
//    ModuleInstall_IIC(CN_IIC2);
//    #endif
//    #if CFG_IIC3_ENABLE== true
//    ModuleInstall_IIC(CN_IIC3);
//    #endif
//    #if CFG_IIC4_ENABLE== true
//    ModuleInstall_IIC(CN_IIC4);
//    #endif
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip iic"//CPU的iic总线驱动
//parent:"iicbus","cpu onchip iic"//填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                          //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                       //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                         //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early                        //初始化时机，可选值：early，medium，later, pre-main。
                                         //表示初始化时间，分别是早期、中期、后期
//dependence:"iicbus"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                         //选中该组件时，被依赖组件将强制选中，
                                         //如果依赖多个组件，则依次列出
//weakdependence:"none"                  //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                         //选中该组件时，被依赖组件不会被强制选中，
                                         //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                         //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束
//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_IIC == false )
//#warning  " cpu_onchip_iic  组件参数未配置，使用默认配置"
//%$#@target = header   //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_IIC    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,16,512,
#define CFG_IIC_BUF_LEN           32       //"IIC缓冲区大小",配置IIC缓冲区大小
//%$#@enum,true,false,
#define CFG_IIC1_ENABLE           false     //"是否使用IIC1",
#define CFG_IIC2_ENABLE           false     //"是否使用IIC2",
#define CFG_IIC3_ENABLE           false     //"是否使用IIC3",
#define CFG_IIC4_ENABLE           false     //"是否使用IIC4",

//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,

#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束

//@#$%component end configure
//=====================DIDE config end ======================================================

// =============================================================================
// 功能: 使串口停止工作，包括时钟和uart模块
// 参数: port,串口号
// 返回: 无
// =============================================================================
//============================================================================================================
//============================================================================================================
typedef struct
{
  vu32 CR1;      /*!< I2C Control register 1,            Address offset: 0x00 */
  vu32 CR2;      /*!< I2C Control register 2,            Address offset: 0x04 */
  vu32 OAR1;     /*!< I2C Own address 1 register,        Address offset: 0x08 */
  vu32 OAR2;     /*!< I2C Own address 2 register,        Address offset: 0x0C */
  vu32 TIMINGR;  /*!< I2C Timing register,               Address offset: 0x10 */
  vu32 TIMEOUTR; /*!< I2C Timeout register,              Address offset: 0x14 */
  vu32 ISR;      /*!< I2C Interrupt and status register, Address offset: 0x18 */
  vu32 ICR;      /*!< I2C Interrupt clear register,      Address offset: 0x1C */
  vu32 PECR;     /*!< I2C PEC register,                  Address offset: 0x20 */
  vu32 RXDR;     /*!< I2C Receive data register,         Address offset: 0x24 */
  vu32 TXDR;     /*!< I2C Transmit data register,        Address offset: 0x28 */
} I2CType;
//============bitdef===========
/* STM32F7 I2C control 1 */
#define TYPE1_I2C_CR1_RXDMAEN         (1<<15)
#define TYPE1_I2C_CR1_TXDMAEN         (1<<14)
#define TYPE1_I2C_CR1_ANFOFF          (1<<12)
#define TYPE1_I2C_CR1_ERRIE           (1<<7)
#define TYPE1_I2C_CR1_TCIE            (1<<6)
#define TYPE1_I2C_CR1_STOPIE          (1<<5)
#define TYPE1_I2C_CR1_NACKIE          (1<<4)
#define TYPE1_I2C_CR1_ADDRIE          (1<<3)
#define TYPE1_I2C_CR1_RXIE            (1<<2)
#define TYPE1_I2C_CR1_TXIE            (1<<1)
#define TYPE1_I2C_CR1_PE              (1<<0)
#define TYPE1_I2C_ALL_IRQ_MASK      ( TYPE1_I2C_CR1_ERRIE \
                                    | TYPE1_I2C_CR1_TCIE \
                                    | TYPE1_I2C_CR1_STOPIE \
                                    | TYPE1_I2C_CR1_NACKIE \
                                    | TYPE1_I2C_CR1_RXIE \
                                    | TYPE1_I2C_CR1_TXIE)

/* STM32F7 I2C control 2 */
#define TYPE1_I2C_CR2_RELOAD          (1<<24)
#define TYPE1_I2C_CR2_AUTOEND          (1<<25)
#define TYPE1_I2C_CR2_NBYTES_MASK     (0xff<<16)
#define TYPE1_I2C_CR2_NBYTES(n)       (((n) & 0xff) << 16)
#define TYPE1_I2C_CR2_NACK            (1<<15)
#define TYPE1_I2C_CR2_STOP            (1<<14)
#define TYPE1_I2C_CR2_START           (1<<13)
#define TYPE1_I2C_CR2_HEAD10R         (1<<12)
#define TYPE1_I2C_CR2_ADD10           (1<<11)
#define TYPE1_I2C_CR2_RD_WRN          (1<<10)
#define TYPE1_I2C_CR2_SADD7_MASK      (0xfe)
#define TYPE1_I2C_CR2_SADD7(n)        (((n) & 0x7f) << 1)

/* STM32F7 I2C Interrupt Status */
#define TYPE1_I2C_ISR_BUSY            (1<<15)
#define TYPE1_I2C_ISR_ARLO            (1<<9)
#define TYPE1_I2C_ISR_BERR            (1<<8)
#define TYPE1_I2C_ISR_TCR             (1<<7)
#define TYPE1_I2C_ISR_TC              (1<<6)
#define TYPE1_I2C_ISR_STOPF           (1<<5)
#define TYPE1_I2C_ISR_NACKF           (1<<4)
#define TYPE1_I2C_ISR_RXNE            (1<<2)
#define TYPE1_I2C_ISR_TXIS            (1<<1)
#define TYPE1_I2C_ISR_ERRORS     (TYPE1_I2C_ISR_BERR|TYPE1_I2C_ISR_ARLO)

/* STM32F7 I2C Interrupt Clear */
#define TYPE1_I2C_ICR_ARLOCF          (1<<9)
#define TYPE1_I2C_ICR_BERRCF          (1<<8)
#define TYPE1_I2C_ICR_STOPCF          (1<<5)
#define TYPE1_I2C_ICR_NACKCF          (1<<4)

/* STM32F7 I2C Timing */
#define TYPE1_I2C_TIMINGR_PRESC(n)        (((n) & 0xf) << 28)
#define TYPE1_I2C_TIMINGR_SCLDEL(n)       (((n) & 0xf) << 20)
#define TYPE1_I2C_TIMINGR_SDADEL(n)       (((n) & 0xf) << 16)
#define TYPE1_I2C_TIMINGR_SCLH(n)         (((n) & 0xff) << 8)
#define TYPE1_I2C_TIMINGR_SCLL(n)         ((n) & 0xff)

#define TYPE1_I2C_MAX_LEN         0xff
#define TYPE1_I2C_DNF_DEFAULT     0
#define TYPE1_I2C_DNF_MAX         16

#define TYPE1_I2C_ANALOG_FILTER_ENABLE    1
#define TYPE1_I2C_ANALOG_FILTER_DELAY_MIN 50  /* ns */
#define TYPE1_I2C_ANALOG_FILTER_DELAY_MAX 260 /* ns */

#define TYPE1_I2C_RISE_TIME_DEFAULT       25  /* ns */
#define TYPE1_I2C_FALL_TIME_DEFAULT       10  /* ns */

#define TYPE1_PRESC_MAX           (1<<4)
#define TYPE1_SCLDEL_MAX          (1<<4)
#define TYPE1_SDADEL_MAX          (1<<4)
#define TYPE1_SCLH_MAX            (1<<8)
#define TYPE1_SCLL_MAX            (1<<8)

#define DIV_ROUND_CLOSEST(x, divisor)(          \
{                           \
    typeof(x) __x = x;              \
    typeof(divisor) __d = divisor;          \
    (((typeof(x))-1) > 0 ||             \
     ((typeof(divisor))-1) > 0 || (__x) > 0) ?  \
        (((__x) + ((__d) / 2)) / (__d)) :   \
        (((__x) - ((__d) / 2)) / (__d));    \
}                           \
)
#define NSEC_PER_SEC 1000000000L

struct TYPE1_i2c_spec {
    u32 rate;//*@速率：I2C总线速度（Hz）
    u32 rate_min;//*@ rtIt min：I2C总线速度（Hz）的80%
    u32 rate_max;//*@ RATEXMAX：I2C总线速度（Hz）的100%
    u32 fall_max;//*@ FaluxMax：SDA和SCL信号（NS）的马克斯下降时间
    u32 rise_max;//*@ RISEXMAX：SDA和SCL信号（NS）的马克斯上升时间
    u32 hddat_min;//* @ HDDATA分钟：最小数据保持时间（NS）
    u32 vddat_max;//*@ VDDATYMAX：马克斯数据有效时间（NS）
    u32 sudat_min;//*@ SUDATYMIN：最小数据建立时间（NS）
    u32 l_min;//*@ Lmin Min：SCL时钟的最小低周期（ns）
    u32 h_min;//*@ Hmin Min：SCL时钟（NS）的min高周期
};


struct TYPE1_i2c_setup {
    u8 speed;//*@速度：I2C速度模式（标准，快速加）
    u32 speed_freq;//*@ SuffyFRIQ:I2C速度频率（Hz）
    u32 clock_src;//*@ CcLogySRC：I2C时钟源频率（Hz）
    u32 rise_time;//*@上升时间：上升时间（NS）
    u32 fall_time;//*@时间：下降时间（ns）
    u8 dnf;//*@ DNF：数字滤波系数（0～16）
    bool analog_filter;//*@类比过滤器：模拟滤波器延迟（开/关）
};

struct TYPE1_i2c_timings {
    u8 presc;//*PREC：预分频器值
    u8 scldel;//*sCdel:数据设置时间
    u8 sdadel;//*@ SDADEL：数据保持时间
    u8 sclh;//*@ SCLH：SCL高周期（主模式）
    u8 scll;//*@ SCLH：SCL低周期（主模式）
};

enum TYPE1_I2C_speed {
    TYPE1_I2C_SPEED_STANDARD, /* 100 kHz */
    TYPE1_I2C_SPEED_FAST, /* 400 kHz */
    TYPE1_I2C_SPEED_FAST_PLUS, /* 1 MHz */
    TYPE1_I2C_SPEED_END,/*其他频率*/
};

//==模块内部使用的数据结构
enum DmaState
{
    dma_sending,//正在发送
    dma_recveing,//正在接收
    dma_idle,   //空闲状态
    DmaUnused,  //未使用DMA模式
};
enum IicError
{
    noError,
    Bus_error,
    ARlocf_error,
    buf_over,
};
enum  IICstate
{
    idle_state,
    Tx_send_memoryaddr,
    Rx_send_memoryaddr,
    Tx_buf,
    Rx_buf,
};


struct stm32_iicConfig
{
    I2CType * iicBaseAddr;   //iic寄存器的基地址
    char* iicname  ;         //名字
    u32   speed;             //默认速度总线上的速度
    u16   iicbufLen;         //IICbus缓冲区长度
    u32    iicEvIntLine;     //IIC事件中断中断号
    u32    iicErIntLine;     //出错中断中断号
    u32    DmaRxIntLine;     //DMA接收中号
    u32    DmaTxIntLine;     //DMA发送中断号
    void* DmaRxTag;          //接收控制寄存器基地址
    void* DmaTxTag;          //发送控制寄存器基地址
    u16   DmaBufLen ;        //DMA缓冲区长度
    u8    DmaSendchsel;      //部分DMA控制器除流控还要设置通道号如果没有则无意义
    u8    DmaRecvchsel;      //部分DMA控制器除流控还要设置通道号如果没有则无意义
};

static const struct stm32_iicConfig config[] = {
[CN_IIC1]={
 .iicBaseAddr    = (I2CType *)I2C1,         //iic寄存器的基地址
 .iicname        = "IIC1",                  //名字
 .speed          = 400*1000,                //默认速度总线上的速度
 .iicbufLen      = CFG_IIC_BUF_LEN,         //IICbus缓冲区长度
 .iicEvIntLine   = CN_INT_LINE_I2C1_EV,     //IIC事件中断号
 .iicErIntLine   = CN_INT_LINE_I2C1_ER,     //IIC出错中断号
 .DmaRxIntLine   = CN_INT_LINE_DMA1_Stream0,//DMA接收中号
 .DmaTxIntLine   = CN_INT_LINE_DMA1_Stream6,//DMA发送中断号
 .DmaRxTag       = DMA1_Stream0,            //接收控制寄存器基地址
 .DmaTxTag       = DMA1_Stream6,            //发送控制寄存器基地址
 .DmaBufLen      = 32,                      //DMA缓冲区长度
 .DmaSendchsel   = 1,                       //部分DMA控制器除流控还要
 .DmaRecvchsel   = 1,                       //设置通道号如果没有则无意义
},
[CN_IIC2]={
 .iicBaseAddr    = (I2CType *)I2C2,         //iic寄存器的基地址
 .iicname        = "IIC2",                  //名字
 .speed          = 400*1000,                //默认速度总线上的速度
 .iicbufLen      = CFG_IIC_BUF_LEN,         //IICbus缓冲区长度
 .iicEvIntLine   = CN_INT_LINE_I2C2_EV,     //IIC事件中断号
 .iicErIntLine   = CN_INT_LINE_I2C2_ER,     //IIC出错中断号
 .DmaRxIntLine   = CN_INT_LINE_DMA1_Stream3,//DMA接收中号
 .DmaTxIntLine   = CN_INT_LINE_DMA1_Stream7,//DMA发送中断号
 .DmaRxTag       = DMA1_Stream3,            //接收控制寄存器基地址
 .DmaTxTag       = DMA1_Stream7,            //发送控制寄存器基地址
 .DmaBufLen      = 32,                      //DMA缓冲区长度
 .DmaSendchsel   = 7,                       //部分DMA控制器除流控还要
 .DmaRecvchsel   = 7,                       //设置通道号如果没有则无意义
},
[CN_IIC3]={
 .iicBaseAddr    = (I2CType *)I2C3,         //iic寄存器的基地址
 .iicname        = "IIC3",                  //名字
 .speed          = 400*1000,                //默认速度总线上的速度
 .iicbufLen      = CFG_IIC_BUF_LEN,         //IICbus缓冲区长度
 .iicEvIntLine   = CN_INT_LINE_I2C3_EV,     //IIC事件中断号
 .iicErIntLine   = CN_INT_LINE_I2C3_ER,     //IIC出错中断号
 .DmaRxIntLine   = CN_INT_LINE_DMA1_Stream1,//DMA接收中号
 .DmaTxIntLine   = CN_INT_LINE_DMA1_Stream4,//DMA发送中断号
 .DmaRxTag       = DMA1_Stream1,            //接收控制寄存器基地址
 .DmaTxTag       = DMA1_Stream4,            //发送控制寄存器基地址
 .DmaBufLen      = 32,                      //DMA缓冲区长度
 .DmaSendchsel   = 1,                       //部分DMA控制器除流控还要
 .DmaRecvchsel   = 3,                       //设置通道号如果没有则无意义
},
[CN_IIC4]={
 .iicBaseAddr    = (I2CType *)I2C4,         //iic寄存器的基地址
 .iicname        = "IIC4",                  //名字
 .speed          = 400*1000,                //默认速度总线上的速度
 .iicbufLen      = CFG_IIC_BUF_LEN,         //IICbus缓冲区长度
 .iicEvIntLine   = CN_INT_LINE_I2C4_EV,     //IIC事件中断号
 .iicErIntLine   = CN_INT_LINE_I2C4_ER,     //IIC出错中断号
 .DmaRxIntLine   = CN_INT_LINE_DMA1_Stream2,//DMA接收中号
 .DmaTxIntLine   = CN_INT_LINE_DMA1_Stream5,//DMA发送中断号
 .DmaRxTag       = DMA1_Stream2,            //接收控制寄存器基地址
 .DmaTxTag       = DMA1_Stream5,            //发送控制寄存器基地址
 .DmaBufLen      = 32,                      //DMA缓冲区长度
 .DmaSendchsel   = 2,                       //部分DMA控制器除流控还要
 .DmaRecvchsel   = 2,                       //设置通道号如果没有则无意义
}};
static struct
{
    struct SemaphoreLCB *pDrvPostSemp;   //信号量
    u8  DevAddr;                         //器件总线上地址
    u8  MemAddr[4];                      //器件内部地址长度
    u32 Addrlenbak;                      //器件内部地址长度备份
    u32 Addrlen;                         //器件内部地址长度计数运算时会改变
    u32 len;                             //收发数据总数计数
    enum  IICstate IICstate;             //IIC传输状态
    enum DmaState  pDmaState;            //DMA工作状态
    struct IIC_CB * pIIC_CB;             //指向IIC_CB句柄
    enum IicError error;                 //记录最后一次出错类型
    u8* Dmabuf;
    u32 DmaRcvlen;
} pStm32Icb[CN_IIC_NUM];
//==============================================================================

/**
  * All these values are coming from I2C Specification, Version 6.0, 4th of
  * April 2014.
  *
  * Table10. Characteristics of the SDA and SCL bus lines for Standard, Fast,
  * and Fast-mode Plus I2C-bus devices
  */
const static struct TYPE1_i2c_spec i2c_specs[] = {
    [TYPE1_I2C_SPEED_STANDARD] = {
        .rate = 100000,
        .rate_min = 80000,
        .rate_max = 100000,
        .fall_max = 300,
        .rise_max = 1000,
        .hddat_min = 0,
        .vddat_max = 3450,
        .sudat_min = 250,
        .l_min = 4700,
        .h_min = 4000,
    },
    [TYPE1_I2C_SPEED_FAST] = {
        .rate = 400000,
        .rate_min = 320000,
        .rate_max = 400000,
        .fall_max = 300,
        .rise_max = 300,
        .hddat_min = 0,
        .vddat_max = 900,
        .sudat_min = 100,
        .l_min = 1300,
        .h_min = 600,
    },
    [TYPE1_I2C_SPEED_FAST_PLUS] = {
        .rate = 1000000,
        .rate_min = 800000,
        .rate_max = 1000000,
        .fall_max = 100,
        .rise_max = 120,
        .hddat_min = 0,
        .vddat_max = 450,
        .sudat_min = 50,
        .l_min = 500,
        .h_min = 260,
    },
    [TYPE1_I2C_SPEED_END] = {
        .rate = 100000000,//*@速率：I2C总线速度（Hz）
        .rate_min = 100,//*@ rtIt min：I2C总线速度（Hz）的80%
        .rate_max = 100000000,//*@ RATEXMAX：I2C总线速度（Hz）的100%
        .fall_max = 300,//*@ FaluxMax：SDA和SCL信号（NS）的马克斯下降时间
        .rise_max = 120,//*@ RISEXMAX：SDA和SCL信号（NS）的马克斯上升时间
        .hddat_min = 0,//* @ HDDATA分钟：最小数据保持时间（NS）
        .vddat_max = 450,//*@ VDDATYMAX：马克斯数据有效时间（NS）
        .sudat_min = 50,//*@ SUDATYMIN：最小数据建立时间（NS）
        .l_min = 500,//*@ Lmin Min：SCL时钟的最小低周期（ns）
        .h_min = 260,//*@ Hmin Min：SCL时钟（NS）的min高周期
    },
};

//==============================================================================
static u32 iic_dmarx_isr(ptu32_t port);//IIC DMA接收中断服务函数
static u32 iic_dmatx_isr(ptu32_t port);//IIC DMA发送中断服务函数
static bool_t IIC_IntEnable(ptu32_t port);
//==============================================================================
//中断方式收发启动函数
//
//==============================================================================
static bool_t IIC_RxTxInt_Init(ptu32_t port)
{
    u32 cr1, cr2;
    bool_t Rdflag = false;
    u32 len;
    I2CType*reg = (I2CType*) config[port].iicBaseAddr;
    reg->CR1&=~TYPE1_I2C_CR1_PE;
    switch (pStm32Icb[port].IICstate)
    {
        case Tx_send_memoryaddr:
            len = pStm32Icb[port].Addrlen + pStm32Icb[port].len;
            break;
        case Rx_send_memoryaddr:
            len = pStm32Icb[port].Addrlen;
            break;
//        case Tx_buf:            break; //写的时候地址和数据一次发送
        case Rx_buf:
            len = pStm32Icb[port].len;
            Rdflag = true;
            break;
        default:   return false;
    }

    cr1 = reg->CR1;
    cr2 = reg->CR2;
    /* Set transfer direction */
    cr2 &= ~TYPE1_I2C_CR2_RD_WRN;
    if (Rdflag)
        cr2 |= TYPE1_I2C_CR2_RD_WRN;

    /* Set slave address */
    cr2 &= ~TYPE1_I2C_CR2_SADD7_MASK;
    cr2 |= TYPE1_I2C_CR2_SADD7(pStm32Icb[port].DevAddr);

    /* Set nb bytes to transfer and reload if needed */
    cr2 &= ~(TYPE1_I2C_CR2_NBYTES_MASK | TYPE1_I2C_CR2_RELOAD);
    if (len > TYPE1_I2C_MAX_LEN) {
        cr2 |= TYPE1_I2C_CR2_NBYTES(TYPE1_I2C_MAX_LEN);
        cr2 |= TYPE1_I2C_CR2_RELOAD;
    }
    else
    {
        cr2 |= TYPE1_I2C_CR2_NBYTES(len);
    }

    /* Enable NACK, STOP, error and transfer complete interrupts */
    cr1 |= TYPE1_I2C_CR1_ERRIE | TYPE1_I2C_CR1_TCIE |
        TYPE1_I2C_CR1_STOPIE | TYPE1_I2C_CR1_NACKIE;

    /* Clear TX/RX interrupt */
    cr1 &= ~(TYPE1_I2C_CR1_RXIE | TYPE1_I2C_CR1_TXIE);

    /* Enable RX/TX interrupt according to msg direction */
    if (Rdflag)
        cr1 |= TYPE1_I2C_CR1_RXIE;
    else
        cr1 |= TYPE1_I2C_CR1_TXIE;

    /* Write configurations registers */
    reg->CR1 = cr1;
    reg->CR2 = cr2;
    /* Configure Start/Repeated Start */
    reg->CR1 |=TYPE1_I2C_CR1_PE;
    reg->CR2 |= TYPE1_I2C_CR2_START;
    return true;
}

//==============================================================================
// DMA方式收发启动函数
//
//==============================================================================
static bool_t IIC_RxTxDMA_Init(ptu32_t port)
{
    u32 cr1, cr2;
    bool_t Rdflag = false;
    u32 len,num;
    I2CType*reg = (I2CType*) config[port].iicBaseAddr;
    reg->CR1&=~TYPE1_I2C_CR1_PE;
    switch (pStm32Icb[port].IICstate)
    {
        case Tx_send_memoryaddr:
            len = pStm32Icb[port].Addrlen + pStm32Icb[port].len;
            break;
        case Rx_send_memoryaddr:
            len = pStm32Icb[port].Addrlen;
            break;
        case Tx_buf:
            break;
        case Rx_buf:
            len = pStm32Icb[port].len;
            Rdflag = true;
            break;
        default:   return false;
    }

    cr1 = reg->CR1;
    cr2 = reg->CR2;
    /* Set transfer direction */
    cr2 &= ~TYPE1_I2C_CR2_RD_WRN;
    if (Rdflag)
        cr2 |= TYPE1_I2C_CR2_RD_WRN;

    /* Set slave address */
    cr2 &= ~TYPE1_I2C_CR2_SADD7_MASK;
    cr2 |= TYPE1_I2C_CR2_SADD7(pStm32Icb[port].DevAddr);

    /* Set nb bytes to transfer and reload if needed */
    cr2 &= ~(TYPE1_I2C_CR2_NBYTES_MASK | TYPE1_I2C_CR2_RELOAD);
    if (len > TYPE1_I2C_MAX_LEN) {
        cr2 |= TYPE1_I2C_CR2_NBYTES(TYPE1_I2C_MAX_LEN);
        cr2 |= TYPE1_I2C_CR2_RELOAD;
    }
    else
    {
        cr2 |= TYPE1_I2C_CR2_NBYTES(len);
    }

    /* Enable NACK, STOP, error and transfer complete interrupts */
    cr1 |= TYPE1_I2C_CR1_ERRIE | TYPE1_I2C_CR1_TCIE |TYPE1_I2C_CR1_STOPIE|\
           TYPE1_I2C_CR1_NACKIE|TYPE1_I2C_ISR_TCR|TYPE1_I2C_ISR_TC;

    /* Clear TX/RX interrupt */
    cr1 &= ~(TYPE1_I2C_CR1_RXIE | TYPE1_I2C_CR1_TXIE);

    /* Enable RX/TX DMA interrupt according to msg direction */
    if (Rdflag)
    {
        num = len>config[port].DmaBufLen ? \
                config[port].DmaBufLen:len;
        DMA_Enable(config[port].DmaRxTag,(u32)pStm32Icb[port].Dmabuf,num);
        pStm32Icb[port].pDmaState = dma_recveing;
        pStm32Icb[port].DmaRcvlen = num;
        pStm32Icb[port].len-=num;
        cr1 |= TYPE1_I2C_CR1_RXDMAEN;
    }
    else
    {
        if(pStm32Icb[port].Addrlen != 0)
        {
            memcpy(pStm32Icb[port].Dmabuf,&pStm32Icb[port].MemAddr,pStm32Icb[port].Addrlen);
        }
        if((pStm32Icb[port].IICstate == Rx_send_memoryaddr))
        {
            num = pStm32Icb[port].Addrlen;
        }
        else
        {
            num = IIC_PortRead(pStm32Icb[port].pIIC_CB ,\
                            (u8*)((u32)pStm32Icb[port].Dmabuf+pStm32Icb[port].Addrlen),\
                             config[port].DmaBufLen-pStm32Icb[port].Addrlen);
            if(num > 0)
            {
                pStm32Icb[port].len-=num;
                pStm32Icb[port].IICstate = Tx_buf;
                num+=pStm32Icb[port].Addrlen;
            }
        }
        pStm32Icb[port].pDmaState = dma_sending;
        DMA_Enable(config[port].DmaTxTag,(u32)pStm32Icb[port].Dmabuf,num);
        cr1 |= TYPE1_I2C_CR1_TXDMAEN;
    }

    /* Write configurations registers */
    reg->CR1 = cr1;
    reg->CR2 = cr2;
    reg->CR1 |=TYPE1_I2C_CR1_PE;
    reg->CR2 |= TYPE1_I2C_CR2_START;
    return true;
}

// =============================================================================
// 功能: 启动写时序，启动写时序的过程为：器件地址（写）、存储地址（写），当存储地址完
//       成时，需打开中断，重新配置寄存器为接收模式，之后将会发生发送中断，在中断服务
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
static bool_t IIC_WriteStartFunc(ptu32_t port,u8 DevAddr,\
                                u32 MemAddr,u8 MemAddrLen, u32 Length,\
                                struct SemaphoreLCB *IIC_BusSemp)
{
    pStm32Icb[port].pDrvPostSemp    = IIC_BusSemp;
    pStm32Icb[port].len             = Length;
    pStm32Icb[port].Addrlen         = MemAddrLen;
    pStm32Icb[port].Addrlenbak      = MemAddrLen;
    pStm32Icb[port].DevAddr         = DevAddr;
    for(u8 i=0;i<MemAddrLen;i++)
        pStm32Icb[port].MemAddr[(MemAddrLen-1)-i]=((MemAddr>>(8*i))&0xff);
    if(MemAddrLen!=0)
        pStm32Icb[port].IICstate = Tx_send_memoryaddr;
    else
        pStm32Icb[port].IICstate = Tx_buf;
    if(pStm32Icb[port].pDmaState ==  DmaUnused)
        return IIC_RxTxInt_Init(port);
    else
        return IIC_RxTxDMA_Init(port);
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
//          收的字节数为count时，产生停止时序，并释放信号量iic_semp;
//     IIC_BusSemp,读完成时，驱动需释放的信号量（缓冲区信号量）
// 返回: TRUE，启动读时序成功，FALSE失败
// =============================================================================
static  bool_t  IIC_ReadStartFunc (ptu32_t port,u8 DevAddr,\
                                u32 MemAddr,u8 MemAddrLen, u32 Length,\
                                struct SemaphoreLCB *IIC_BusSemp)
{
    pStm32Icb[port].pDrvPostSemp    = IIC_BusSemp;
    pStm32Icb[port].len             = Length;
    pStm32Icb[port].Addrlen         = MemAddrLen;
    pStm32Icb[port].Addrlenbak      = MemAddrLen;
    pStm32Icb[port].DevAddr         = DevAddr;
    for(u8 i=0;i<MemAddrLen;i++)
        pStm32Icb[port].MemAddr[(MemAddrLen-1)-i]=((MemAddr>>(8*i))&0xff);
    if(MemAddrLen!=0)
        pStm32Icb[port].IICstate = Rx_send_memoryaddr;
    else
        pStm32Icb[port].IICstate = Rx_buf;
    if(pStm32Icb[port].pDmaState == DmaUnused)
        return IIC_RxTxInt_Init( port);
    else
        return IIC_RxTxDMA_Init(port);
}

// =============================================================================
// 功能: 结束本次读写回调函数，区分读写的不同停止时序，当属于发送时，则直接停止时序，
//      若为读，则先停止回复ACK，再停止。
// 参数: SpecificFlag,个性标记，本模块内即IIC寄存器基址。
// 返回: 无
// =============================================================================
static  void IIC_GenerateEndFunc(ptu32_t port)
{
    I2CType*reg = (I2CType*) config[port].iicBaseAddr;
    switch (pStm32Icb[port].pDmaState)
    {
        case DmaUnused:
             pStm32Icb[port].IICstate = DmaUnused;
             reg->CR1 &=~TYPE1_I2C_ALL_IRQ_MASK;
             reg->CR1 &=~TYPE1_I2C_CR1_PE;
             break;
        case dma_sending:
            DMA_Disable(config[port].DmaTxTag);
            break;
        case dma_recveing:
            DMA_Disable(config[port].DmaRxTag);
            break;
        case dma_idle:            break;
        default:                 break;
    }

}

//==============================================================================
// 功能：根据要设置的时钟频率计算要设置的寄存器,遍历所有可设置的值取误差最小的值
// 参数：setup 输入参数
//     output 输出要设置寄存器的值
// 返回：true /false
//==============================================================================
static bool_t compute_timing(struct TYPE1_i2c_setup *setup,struct TYPE1_i2c_timings *output)
{
    u32 clk_error_prev, af_delay_min  ,af_delay_max ,dnf_delay;
    int sdadel_max,scldel_min ,sdadel_min;
    u32 clk_max,clk_min,tsync;

    bool_t flag  = false;
    u32 p_prev = TYPE1_PRESC_MAX;
    float i2cclk = DIV_ROUND_CLOSEST(NSEC_PER_SEC,
                       setup->clock_src);
    float i2cbus = DIV_ROUND_CLOSEST(NSEC_PER_SEC,
                       setup->speed_freq);

    if ((setup->rise_time > i2c_specs[setup->speed].rise_max) ||\
        (setup->fall_time > i2c_specs[setup->speed].fall_max) ||\
        (setup->dnf > TYPE1_I2C_DNF_MAX) ||\
        (setup->speed_freq > i2c_specs[setup->speed].rate))
        return false;

    clk_error_prev = i2cbus;
    /*  Analog and Digital Filters 模拟和数字滤波器 */
    af_delay_min =(setup->analog_filter ?TYPE1_I2C_ANALOG_FILTER_DELAY_MIN : 0);
    af_delay_max =(setup->analog_filter ?TYPE1_I2C_ANALOG_FILTER_DELAY_MAX : 0);
    dnf_delay = setup->dnf * i2cclk;

    //数据保持时间最小和最大值
    sdadel_min  =  (setup->fall_time - i2c_specs[setup->speed].hddat_min) ;
    sdadel_min -=  af_delay_min;
    sdadel_min -=  (setup->dnf + 3) * i2cclk;

    sdadel_max = i2c_specs[setup->speed].vddat_max - setup->rise_time ;
    sdadel_max-= af_delay_max;
    sdadel_max-= (setup->dnf + 4) * i2cclk;

    if (sdadel_min < 0)
        sdadel_min = 0;
    if (sdadel_max < 0)
        sdadel_max = 0;

    scldel_min = setup->rise_time + i2c_specs[setup->speed].sudat_min;

    clk_max = NSEC_PER_SEC / i2c_specs[setup->speed].rate_min;
    clk_min = NSEC_PER_SEC / i2c_specs[setup->speed].rate_max;
    tsync = af_delay_min + dnf_delay + (2 * i2cclk);

    /* Compute possible values for PRESC, SCLDEL and SDADEL*/
    for (u16 p = 0; p < TYPE1_PRESC_MAX; p++) ///预分频值
    {
        for (u16 l = 0; l < TYPE1_SCLDEL_MAX; l++)
        {
            u32 scldel = (l + 1) * (p + 1) * i2cclk;//数据建立时间
            if (scldel < scldel_min)
                continue;

            for (u16 a = 0; a < TYPE1_SDADEL_MAX; a++)
            {
                u32 sdadel = (a * (p + 1) + 1) * i2cclk;//数据保持时间
                if (((sdadel >= sdadel_min) &&(sdadel <= sdadel_max)) &&(p != p_prev))
                {
                    p_prev = p;
                    u32 prescaler = (p + 1) * i2cclk;//分频后时钟周期
                    for (u16 scll = 0; scll < TYPE1_SCLL_MAX; scll++)
                    {
                        u32 tscl_l = (scll + 1) * prescaler + tsync;//时钟低维持时间
                        if ((tscl_l < i2c_specs[setup->speed].l_min) ||\
                              (i2cclk >=((tscl_l - af_delay_min - dnf_delay) / 4)))
                            continue;
                        for (u16 sclh = 0; sclh < TYPE1_SCLH_MAX; sclh++)
                        {
                            u32 tscl_h = (sclh + 1) * prescaler + tsync;//时钟高维持时间
                            u32 tscl = tscl_l + tscl_h +\
                                    setup->rise_time + setup->fall_time;

                            if ((tscl_h >= i2c_specs[setup->speed].h_min) &&\
                                (tscl >= clk_min) && (tscl <= clk_max) &&\
                                (i2cclk < tscl_h))//允许不标准的频率
                            {
                                s32 clk_error = tscl - i2cbus;
                                if (clk_error < 0)
                                    clk_error = -clk_error;

                                if (clk_error < clk_error_prev)
                                {
                                    clk_error_prev = clk_error;
                                    output->presc = p;
                                    output->scll = scll;
                                    output->sclh = sclh;
                                    output->scldel = l;
                                    output->sdadel = a;
                                    flag  = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return flag;
}

//=============================================================================
// 功能: 设置时钟频率
// 参数: port,端口号，即IIC号，为1~3
//      clk_rate 要设置的频率
// 返回: true/false
// =============================================================================
static bool_t IIC_ClkSet(ptu32_t port,ptu32_t clk_rate)
{
    struct TYPE1_i2c_setup setup ;
    struct TYPE1_i2c_timings STiming;
    u32 timingr = 0;
    bool_t flag = false;
    I2CType*reg = (I2CType*) config[port].iicBaseAddr;

    setup.rise_time     = TYPE1_I2C_RISE_TIME_DEFAULT;
    setup.fall_time     = TYPE1_I2C_FALL_TIME_DEFAULT;
    setup.dnf           = TYPE1_I2C_DNF_DEFAULT;
    setup.analog_filter = TYPE1_I2C_ANALOG_FILTER_ENABLE;
    setup.analog_filter = TYPE1_I2C_ANALOG_FILTER_ENABLE;
    setup.speed_freq    = clk_rate;
    setup.clock_src     = CN_CFG_PCLK1;
    setup.speed = TYPE1_I2C_SPEED_STANDARD;
    do {
        flag = compute_timing(&setup,&STiming);
        if (flag == false)
        {
            if (setup.speed < TYPE1_I2C_SPEED_FAST_PLUS)
            {
                setup.speed ++;
            }
            else
            {
                break;
            }
        }
    } while (!flag);

    if(flag == true)
    {
        printf("IIC%d 设置时钟  %d hz成功 speed = %d ！！\n\r ",port+1,clk_rate,setup.speed);
        reg->CR1 &=~ TYPE1_I2C_CR1_PE;
        timingr |= TYPE1_I2C_TIMINGR_PRESC(STiming.presc);
        timingr |= TYPE1_I2C_TIMINGR_SCLDEL(STiming.scldel);
        timingr |= TYPE1_I2C_TIMINGR_SDADEL(STiming.sdadel);
        timingr |= TYPE1_I2C_TIMINGR_SCLH(STiming.sclh);
        timingr |= TYPE1_I2C_TIMINGR_SCLL(STiming.scll);
        reg->TIMINGR = timingr;

        if (setup.analog_filter) /* Enable I2C 模拟滤波*/
            reg->CR1 &=~ TYPE1_I2C_CR1_ANFOFF;
        else
            reg->CR1 |=  TYPE1_I2C_CR1_ANFOFF;
        reg->CR1 &=~ TYPE1_I2C_CR1_PE;
        return true;
    }
    printf("IIC%d 设置时钟  %d hz失败 ！！\n\r ",port+1,clk_rate);
    return false;
}

//=============================================================================
// 功能: 设置使用DMA，IIC使用DMA收发，每收发一个字节就必须中断一次，不建议在高速
//      连续通信中使用。
// 参数: port,端口号，即IIC号，为1~3
// 返回: 无
// =============================================================================
static bool_t Set_IIC_DMA_USED(ptu32_t port)
{
    struct HeapCB *heap;
    u32 DmaBufLen;
    ufast_t ufl_line;
    I2CType*reg = (I2CType*) config[port].iicBaseAddr;
    pStm32Icb[port].IICstate = idle_state;
    if (pStm32Icb[port].pDmaState != DmaUnused)//已经是DMA模式了直接返回
        return false;
    if(pStm32Icb[port].Dmabuf == NULL)
    {
        heap =Heap_FindHeap("nocache");
        if(heap==NULL)
        {
            heap =Heap_FindHeap("sys"); //如果没有“nocache”堆就使用系统堆
            printf("警告UART%d:设置DMA 模式没有找到 nocache Heap",port+1);;
        }
        DmaBufLen =config[port].DmaBufLen;

        //有些DMA访问需要32字节对齐
        pStm32Icb[port].Dmabuf = M_MallocHeap(DmaBufLen+32,heap,0);
        if(pStm32Icb[port].Dmabuf == NULL)
        {
            printf("内存不足设置DMA模式失败 \n\r");
            return false;
        }
        pStm32Icb[port].Dmabuf = (u8 *)((u32)(pStm32Icb[port].Dmabuf+32)&(~31));
    }
    reg->CR1 &= ~(TYPE1_I2C_CR1_TXIE|TYPE1_I2C_CR1_TXIE|TYPE1_I2C_CR1_ERRIE|\
                  TYPE1_I2C_CR1_TCIE|TYPE1_I2C_CR1_STOPIE|TYPE1_I2C_CR1_NACKIE);
    pStm32Icb[port].pDmaState = dma_idle;

    //配置接收DMA
    DMA_Config(config[port].DmaRxTag,config[port].DmaRecvchsel,\
                (u32)(&reg->RXDR),false,(u32)pStm32Icb[port].Dmabuf,0,DMA_DIR_P2M,\
                DMA_DATABITS_8,DMA_DATABITS_8,config[port].DmaBufLen);
    DMA_SetIntModeComplete(config[port].DmaRxTag);
    //发送
    DMA_Config(config[port].DmaTxTag,config[port].DmaSendchsel,\
                 (u32)(&reg->TXDR),false,(u32)pStm32Icb[port].Dmabuf,0,DMA_DIR_M2P,\
                 DMA_DATABITS_8,DMA_DATABITS_8,config[port].DmaBufLen);

    ufl_line = config[port].DmaRxIntLine;
    Int_Register(ufl_line);//注册发送DMA中断
    Int_SetClearType(ufl_line,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(ufl_line,iic_dmarx_isr);
    Int_SetIsrPara(ufl_line,port);
    Int_ClearLine(ufl_line);
    Int_SettoAsynSignal(ufl_line);
    Int_RestoreAsynLine(ufl_line);

    ufl_line = config[port].DmaTxIntLine;
    Int_Register(ufl_line);//注册接收DMA中断
    Int_SetClearType(ufl_line,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(ufl_line,iic_dmatx_isr);//关联中断
    Int_SetIsrPara(ufl_line,port);
    Int_ClearLine(ufl_line);
    Int_SettoAsynSignal(ufl_line);
    Int_RestoreAsynLine(ufl_line);

    return true;
}
// =============================================================================
// 功能: 设置IIC使用dma收发，根据stm32各IIC的收发dma通道配置寄存器。将重新初
//       始化所有dma寄存器，因为:
//       1、不使用dma期间可能有其他设备使用dma，从而更改了设置。
//       2、即使IIC使用dma期间，也可能软件模块配合的问题导致其他设备使用dma.
// 参数: port,端口号，即IIC号，从1~3
// 返回: 无
// =============================================================================
static bool_t Set_IIC_DMA_UNUSED(ptu32_t port)
{
    I2CType*reg = (I2CType*) config[port].iicBaseAddr;
    pStm32Icb[port].IICstate = idle_state;

    reg->CR1 &= ~(TYPE1_I2C_CR1_RXDMAEN|TYPE1_I2C_CR1_TXDMAEN);

    if(pStm32Icb[port].pDmaState != DmaUnused)
    {
        Int_SaveAsynLine(config[port].DmaRxIntLine);
        Int_SaveAsynLine(config[port].DmaTxIntLine);
        pStm32Icb[port].pDmaState = DmaUnused;
    }
    IIC_IntEnable(port);
    return true;
}
// =============================================================================
// 功能: 使IIC开始工作，时钟和IIC模块
// 参数: port,IIC号
// 返回: 无
// =============================================================================
static bool_t IIC_IntDisable(ptu32_t port)
{
    I2CType*reg = (I2CType*) config[port].iicBaseAddr;
    Int_SaveAsynLine(config[port].iicErIntLine);
    Int_SaveAsynLine(config[port].iicEvIntLine);
    reg->CR1 &= ~TYPE1_I2C_ALL_IRQ_MASK;
    pStm32Icb[port].IICstate = idle_state;
    return true;
}
// =============================================================================
// 功能: 使IIC开始工作，时钟和IIC模块
// 参数: port,IIC号
// 返回: 无
// =============================================================================
static bool_t IIC_IntEnable(ptu32_t port)
{
    I2CType*reg = (I2CType*) config[port].iicBaseAddr;
    Int_RestoreAsynLine(config[port].iicErIntLine);
    Int_RestoreAsynLine(config[port].iicEvIntLine);
    reg->CR1 |= TYPE1_I2C_ALL_IRQ_MASK;
    pStm32Icb[port].IICstate = idle_state;
    return true;
}

// =============================================================================
// 功能：IIC总线控制回调函数，被上层调用，目前只实现对IIC时钟频率配置
// 参数：SpecificFlag,个性标记，本模块内即IIC寄存器基址
//       cmd,命令
//       data1,data2,数据，与具体命令相关
// 返回：无
// =============================================================================
static s32 IIC_BusCtrlFunc (ptu32_t port,u32 cmd, ptu32_t data1,ptu32_t data2)
{
    switch(cmd)
    {
         case  CN_IIC_SET_CLK:
             IIC_ClkSet(port,data1);
             break;    //设置时钟频率
         case  CN_IIC_DMA_USED :
             Set_IIC_DMA_USED(port);
             break;    //使用dma传输
         case  CN_IIC_DMA_UNUSED:
             Set_IIC_DMA_UNUSED(port);
             break;    //禁止dma传输
         case  CN_IIC_SET_POLL  :
             IIC_IntDisable(port);
             break;    //使用轮询方式发送接收
         case  CN_IIC_SET_INT :
             IIC_IntEnable(port);
             break;     //使用中断方式发送接收
         case  CN_IIC_OS_USED :

             break;
         default:            return 0;
    }
    return 1;
}
// =============================================================================
// 功能：轮询方式向IIC从设备收发数据
// 参数：reg,寄存器基址
//       devaddr,设备地址
//       memaddr,设备内部地址
//       maddrlen,设备内部地址长度
//       buf,存储数据
//       len,数据长度，字节
// 返回：len,读取成功;-1,读取失败
// =============================================================================
static bool_t IIC_wait_flags(I2CType *reg,u32 flags,u32 *status)
{
    u32 timeout = 1000;
     *status = reg->ISR;
    while ((!(*status & flags))&&timeout)
    {
        timeout--;
        DJY_DelayUs(100);
        *status = reg->ISR;
    }
    if(timeout == 0)
        return false;
    return true;
}

// =============================================================================
// 功能：轮询方式向IIC从设备收发数据
// 参数：reg,寄存器基址
//       devaddr,设备地址
//       memaddr,设备内部地址
//       maddrlen,设备内部地址长度
//       buf,存储数据
//       len,数据长度，字节
// 返回：len,读取成功;-1,读取失败
// =============================================================================
//(I2CType *reg,u8 devaddr,u8 *buf, u32 len ,bool_t Rdflag)
static u32 IIC_RxTxPoll(ptu32_t port,u8 devaddr,u8 *buf)
{
    I2CType* reg = config[port].iicBaseAddr;
    u32  status,cr2,num,mask,bytes_to_rw,len;
    bool_t Rdflag;
    u8 * bufbak = buf;
    cr2 = reg->CR2;
    num = 0;

    switch (pStm32Icb[port].IICstate)
    {
        case Tx_send_memoryaddr:
            len = pStm32Icb[port].Addrlen + pStm32Icb[port].len;
            buf = pStm32Icb[port].MemAddr;
            Rdflag = false;
            break;
        case Rx_send_memoryaddr:
            len = pStm32Icb[port].Addrlen;
            buf = pStm32Icb[port].MemAddr;
            Rdflag = false;
            break;
        case Rx_buf:
            len = pStm32Icb[port].len;
            Rdflag = true;
            break;
        default:     return 0;
    }
    mask = Rdflag ? TYPE1_I2C_ISR_RXNE : (TYPE1_I2C_ISR_TXIS | TYPE1_I2C_ISR_NACKF);

    reg->ICR |=(TYPE1_I2C_ICR_STOPCF|TYPE1_I2C_ICR_NACKCF);
    /* 添加需要检测的标志位 */
    mask |= TYPE1_I2C_ISR_ERRORS;
    //清除相关位
    cr2 &= ~(TYPE1_I2C_CR2_RD_WRN|TYPE1_I2C_CR2_SADD7_MASK| TYPE1_I2C_CR2_ADD10|\
            TYPE1_I2C_CR2_NBYTES_MASK|TYPE1_I2C_CR2_RELOAD|TYPE1_I2C_CR2_AUTOEND);
    if(Rdflag)
        cr2 |=TYPE1_I2C_CR2_RD_WRN;
    //设置总线地址
    cr2 |= TYPE1_I2C_CR2_SADD7(devaddr);
    if (len > TYPE1_I2C_MAX_LEN)
    {
        bytes_to_rw = TYPE1_I2C_MAX_LEN;
        cr2 |= TYPE1_I2C_CR2_NBYTES(TYPE1_I2C_MAX_LEN);
        cr2 |= TYPE1_I2C_CR2_RELOAD;
    }
    else
    {
        bytes_to_rw = len;
        cr2 |= TYPE1_I2C_CR2_NBYTES(len);
    }
    reg->CR2 = cr2;
    reg->CR1 |= TYPE1_I2C_CR1_PE;
    reg->CR2 |= TYPE1_I2C_CR2_START;

    while (len)
    {
        if(IIC_wait_flags(reg,mask,&status)==false)
            break;

        if (status & (TYPE1_I2C_ISR_NACKF | TYPE1_I2C_ISR_ERRORS))
            break;
        if (status & TYPE1_I2C_ISR_RXNE) //
        {
            *buf++ = reg->RXDR;
             len--;
             num++;
             bytes_to_rw--;
        }
        if (status & TYPE1_I2C_ISR_TXIS)
        {
            *((volatile u8 *)&reg->TXDR) = *buf++;
            len--;
            num++;
            bytes_to_rw--;
            if(pStm32Icb[port].Addrlen == num)
            {
                pStm32Icb[port].IICstate =Tx_buf;
                buf = bufbak;
            }
        }
        if (!bytes_to_rw && len)
        {
            /* Wait until TCR flag is set */
            if(IIC_wait_flags(reg,TYPE1_I2C_ISR_TCR,&status)==false)
                break;
            bytes_to_rw = len > TYPE1_I2C_MAX_LEN ?TYPE1_I2C_MAX_LEN :len;
            mask = Rdflag ? TYPE1_I2C_ISR_RXNE : TYPE1_I2C_ISR_TXIS | TYPE1_I2C_ISR_NACKF;

            reg->CR2 &= ~TYPE1_I2C_CR2_NBYTES_MASK;
            if (len > TYPE1_I2C_MAX_LEN) {
                reg->CR2 |= TYPE1_I2C_CR2_NBYTES(TYPE1_I2C_MAX_LEN);
            }
            else
            {
                reg->CR2 &= ~TYPE1_I2C_CR2_RELOAD;
                reg->CR2 |= TYPE1_I2C_CR2_NBYTES(len);
            }
        }
        else if (!bytes_to_rw)
        {
            /* Wait until TC flag is set */
            if(IIC_wait_flags(reg,TYPE1_I2C_ISR_TC,&status)==false)
                break;
        }
    }
    reg->CR2 |=TYPE1_I2C_CR2_STOP;/* End of transfer, send stop condition */
    return num;
}

// =============================================================================
// 功能：轮询方式读写IIC设备
// 参数：SpecificFlag,寄存器基址
//       DevAddr,设备地址
//       MemAddr,设备内部地址
//       MemAddrLen,设备内部地址长度
//       Buf,存储数据
//       Length,数据长度，字节
//       WrRdFlag,读写标记，为0时写，1时为读
// 返回：len,读取成功;-1,读取失败
// =============================================================================
static bool_t IIC_WriteReadPoll(ptu32_t port,u8 DevAddr,u32 MemAddr,\
                                u8 MemAddrLen,u8* Buf, u32 Length,u8 WrRdFlag)
{
    u8 i;
    bool_t flag=false;
    I2CType* reg = config[port].iicBaseAddr;
    reg->CR1 &=~TYPE1_I2C_CR1_PE;
    for(i=0;i<MemAddrLen;i++)
        pStm32Icb[port].MemAddr[(MemAddrLen-1)-i]=((MemAddr>>(8*i))&0xff);

    pStm32Icb[port].Addrlen = MemAddrLen;
    pStm32Icb[port].len     = Length;

    //轮询方式先关中断
    reg->CR1 &= ~(TYPE1_I2C_CR1_ERRIE | TYPE1_I2C_CR1_TCIE |\
            TYPE1_I2C_CR1_STOPIE | TYPE1_I2C_CR1_NACKIE);
    if(!WrRdFlag)//写
    {
        pStm32Icb[port].IICstate = Tx_send_memoryaddr;
        if((MemAddrLen+Length) ==IIC_RxTxPoll(port,DevAddr,Buf))
            flag = true;
    }
    else  //读
    {
        pStm32Icb[port].IICstate = Rx_send_memoryaddr;
        if(MemAddrLen == IIC_RxTxPoll(port,DevAddr,Buf))
        {
            pStm32Icb[port].IICstate = Rx_buf;
            if(Length == IIC_RxTxPoll(port,DevAddr,Buf))
                flag = true;
        }
    }
    return flag;
}
// =============================================================================
// 功能:  IIC中断配置函数 底层硬件初始化 控制器 时钟 工作状态
// 参数: port端口号
// 返回: 无
// =============================================================================
static void IIC_HardInit(ptu32_t port)
{
    I2CType* reg = config[port].iicBaseAddr;
#if defined IICCLKOLD
    RCC->DCKCFGR2 &=~(3<<((2*port)+16));//清除原来的设置
    RCC->APB1LENR |=(1<<(21+port));//外设时钟使能
    reg->TIMINGR = 0x40953c38;//时序寄存器先写死
    reg->CR1     = TYPE1_I2C_CR1_PE;//PE使能
#else
    RCC->APB1LENR |=(1<<(21+port));//外设时钟使能
//    RCC->DCKCFGR2 &=~(3<<((2*port)+16));//清除原来的设置
//    RCC->DCKCFGR2 |=(0<<((2*port)+16));
    IIC_ClkSet(port,config[port].speed);
    reg->CR1     = TYPE1_I2C_CR1_PE;//PE使能
#endif
}

//
// =============================================================================
// 功能：IIC DMA接收中断服务函数
// 参数：port  IIC编号 CN_IIC1
//       Add_mode 寻址模式
// 返回：无
// =============================================================================
static u32 iic_dmarx_isr(ptu32_t port)
{
    u32 num;
    num = IIC_PortWrite(pStm32Icb[port].pIIC_CB,pStm32Icb[port].Dmabuf,\
                                            pStm32Icb[port].DmaRcvlen);

    num = pStm32Icb[port].len>config[port].DmaBufLen ? \
                config[port].DmaBufLen:pStm32Icb[port].len;
    DMA_ClearIntFlag(config[port].DmaRxTag);

    if(num>0)
    {
        DMA_Enable(config[port].DmaRxTag,(u32)pStm32Icb[port].Dmabuf,num);
        pStm32Icb[port].DmaRcvlen = num;
        pStm32Icb[port].len-=num;
    }
    else
    {
        pStm32Icb[port].pDmaState = dma_idle;
        DMA_Disable(config[port].DmaRxTag);
    }
    return 0;
}

// =============================================================================
// 功能：IIC DMA发送中断包括发送地址发送数据
// 参数：iic_port  IIC编号 CN_IIC1
//       Add_mode 寻址模式
// 返回：无
// =============================================================================
static u32 iic_dmatx_isr(ptu32_t port)
{
    u32 num;
    DMA_ClearIntFlag(config[port].DmaTxTag);
    if(pStm32Icb[port].IICstate == Rx_send_memoryaddr)
    {
        num=0;
    }
    else
    {
        num = pStm32Icb[port].len>config[port].DmaBufLen ? \
                config[port].DmaBufLen:pStm32Icb[port].len;
        num = IIC_PortRead(pStm32Icb[port].pIIC_CB ,pStm32Icb[port].Dmabuf,num);
    }

    if(num > 0)
    {
        DMA_Enable(config[port].DmaTxTag,(u32)pStm32Icb[port].Dmabuf,num);
        pStm32Icb[port].len-=num;
    }
    else
    {
        pStm32Icb[port].pDmaState = dma_idle;
        DMA_Disable(config[port].DmaTxTag);
    }

    return 0;
}
// =============================================================================
// 功能： 如果发送或接收的数据超过255字节会使用reload模式
//     这里会计算出需要再次传输的数据值，并填入寄存器。
// 参数： port  IIC编号 CN_IIC1
// 返回：无
// =============================================================================
static void iic_reload(ptu32_t port)
{
    u32 cr2;
    I2CType*reg = (I2CType*) config[port].iicBaseAddr;

    cr2 = reg->CR2;
    cr2 &= ~TYPE1_I2C_CR2_NBYTES_MASK;
    if (pStm32Icb[port].len > TYPE1_I2C_MAX_LEN)
    {
        cr2 |= TYPE1_I2C_CR2_NBYTES(TYPE1_I2C_MAX_LEN);
    }
    else
    {
        cr2 &= ~TYPE1_I2C_CR2_RELOAD;
        cr2 |= TYPE1_I2C_CR2_NBYTES(pStm32Icb[port].len);
    }
    reg->CR2 = cr2;
}

// =============================================================================
// 功能：I2C中断服务函数，包括事件中断和出错中断
// 参数：port  IIC编号 CN_IIC1
// 返回：无
// =============================================================================
static u32 iic_event_error_isr(ptu32_t port)
{
    I2CType* reg = (I2CType*) config[port].iicBaseAddr;
    s32 num;
    u8 ch;

    if (reg->ISR & TYPE1_I2C_ISR_TC) /* 传输完成 */
    {
        if((pStm32Icb[port].IICstate==Tx_buf)||\
           (pStm32Icb[port].IICstate==Rx_buf))//发送完内部地址发送数据
        {
            reg->CR2 |= TYPE1_I2C_CR2_STOP;
            pStm32Icb[port].IICstate = idle_state;
            Lock_SempPost(pStm32Icb[port].pDrvPostSemp);//结束本次传输
            if( pStm32Icb[port].pDmaState != DmaUnused)
                pStm32Icb[port].pDmaState =dma_idle;
        }
        else
        {
            if(pStm32Icb[port].IICstate == Rx_send_memoryaddr)
                pStm32Icb[port].IICstate = Rx_buf;
            if(pStm32Icb[port].pDmaState == DmaUnused)
                return IIC_RxTxInt_Init( port);
            else
                return IIC_RxTxDMA_Init(port);
        }
    }
    if (reg->ISR & TYPE1_I2C_ISR_TXIS) /* 发送寄存器空 */
    {
        if((pStm32Icb[port].IICstate==Tx_send_memoryaddr)||\
           (pStm32Icb[port].IICstate==Rx_send_memoryaddr))//发送完内部地址发送数据
        {
            ch = pStm32Icb[port].MemAddr[pStm32Icb[port].Addrlenbak-pStm32Icb[port].Addrlen];
            pStm32Icb[port].Addrlen--;
            *((volatile u8 *)&reg->TXDR) = ch;
            if(pStm32Icb[port].Addrlen==0 && pStm32Icb[port].IICstate==Tx_send_memoryaddr )
                pStm32Icb[port].IICstate=Tx_buf;
        }
        else
        {
            num = IIC_PortRead( pStm32Icb[port].pIIC_CB,&ch,1);
            if(num != 0)
            {
                pStm32Icb[port].len--;
                *((volatile u8 *)&reg->TXDR) = ch;
            }
            else
            {
                pStm32Icb[port].IICstate = idle_state;
                Lock_SempPost(pStm32Icb[port].pDrvPostSemp);
            }
        }
    }

    if (reg->ISR & TYPE1_I2C_ISR_RXNE)/* 接收寄存器非空 */
    {
        ch = reg->RXDR;
        pStm32Icb[port].len--;
        num = IIC_PortWrite(pStm32Icb[port].pIIC_CB,&ch,1);
        if(num != 1)
            pStm32Icb[port].error = buf_over;//缓冲区溢出
    }

    if (reg->ISR & TYPE1_I2C_ISR_NACKF)/* 接收到否定应答标志 */
    {
        reg->ICR |=TYPE1_I2C_ICR_NACKCF;//器件主动结束本次传输
        Lock_SempPost(pStm32Icb[port].pDrvPostSemp);
    }

    if (reg->ISR & TYPE1_I2C_ISR_STOPF)/* 停止位检测标志  */
    {
        pStm32Icb[port].IICstate = idle_state;
        /* Disable interrupts */
        reg->CR1 &=~TYPE1_I2C_ALL_IRQ_MASK;
        /* Clear STOP flag */
        reg->ICR |=TYPE1_I2C_ICR_STOPCF;//清停止位
    }

    /*
     * Transfer Complete Reload: 255 data bytes have been transferred
     * We have to prepare the I2C controller to transfer the remaining
     * data.
     */
    if (reg->ISR & TYPE1_I2C_ISR_TCR)/*传输完成等待重载*/
        iic_reload(port);

    if (reg->ISR & TYPE1_I2C_ISR_BERR) /* 总线错误 */
    {
        printf( "<%s>: Bus error\n", __func__);
        IIC_ErrPop(pStm32Icb[port].pIIC_CB,CN_IIC_POP_UNKNOW_ERR);
        reg->ICR |= TYPE1_I2C_ICR_BERRCF;
        pStm32Icb[port].error = Bus_error;
        Lock_SempPost(pStm32Icb[port].pDrvPostSemp);//结束本次传输
        reg->CR1 &=~TYPE1_I2C_CR1_PE;
    }

    if (reg->ISR & TYPE1_I2C_ISR_ARLO) /* 仲裁丢失*/
    {
        printf("<%s>: Arbitration loss\n", __func__);
        IIC_ErrPop(pStm32Icb[port].pIIC_CB,CN_IIC_POP_MAL_LOST_ERR);
        reg->ICR |= TYPE1_I2C_ICR_ARLOCF;
        pStm32Icb[port].error = ARlocf_error;
        Lock_SempPost(pStm32Icb[port].pDrvPostSemp);//结束本次传输
        reg->CR1 &=~TYPE1_I2C_CR1_PE;
    }
    return 0;
}

// =============================================================================
// 功能: IIC中断配置函数
// 参数: port端口号
// 返回: 无
// =============================================================================
static void IIC_Int_Init(ptu32_t port)
{
    u8 IntLine;
    //注册事件中断服务函数
    IntLine = config[port].iicEvIntLine;
    Int_Register(IntLine);
    Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(IntLine,iic_event_error_isr);
    Int_SettoAsynSignal(IntLine);
    Int_ClearLine(IntLine);
    Int_SetIsrPara(IntLine,port);
    Int_RestoreAsynLine(IntLine);

    IntLine = config[port].iicErIntLine;
    Int_Register(IntLine);
    Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(IntLine,iic_event_error_isr);
    Int_SettoAsynSignal(IntLine);
    Int_ClearLine(IntLine);
    Int_SetIsrPara(IntLine,port);
    Int_RestoreAsynLine(IntLine);
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
     u32 timeout=0;
     GPIO_PowerOn(port);
     GPIO_CfgPinFunc(port,sda_pin,GPIO_MODE_IN, //TS_SDA
                     GPIO_OTYPE_OD,GPIO_SPEED_100M,GPIO_PUPD_PU);
     GPIO_CfgPinFunc(port,sck_pin,GPIO_MODE_OUT,  //TS_SCK
                     GPIO_OTYPE_OD,GPIO_SPEED_100M,GPIO_PUPD_PU);
     while(1)
     {
          timeout++;
          GPIO_SettoLow(port,sck_pin);
          DJY_DelayUs(10);

          GPIO_SettoHigh(port,sck_pin);
          DJY_DelayUs(10);

          if(timeout>=100)
              return false;
          if( GPIO_GetData(port)&(sda_pin))
              break;
    }
     GPIO_CfgPinFunc(port,sda_pin,GPIO_MODE_OUT,  //TS_SCK
                         GPIO_OTYPE_OD,GPIO_SPEED_100M,GPIO_PUPD_PU);
    //产生停止信号 iic总线释放
    GPIO_SettoLow(port,sda_pin);
    DJY_DelayUs(10);
    GPIO_SettoHigh(port,sda_pin);
    DJY_DelayUs(10);

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
bool_t ModuleInstall_IIC(ptu32_t port)
{
    struct IIC_Param IIC_Config;
    u8 *buf;
    if(port >= CN_IIC_NUM)
        return false;

    buf = M_Malloc(config[port].iicbufLen,0);
    if(buf == NULL)
        return false;

    pStm32Icb[port].pDmaState  = DmaUnused;
    pStm32Icb[port].error      = noError;
    pStm32Icb[port].IICstate   = idle_state;
    pStm32Icb[port].Dmabuf     = NULL;
    pStm32Icb[port].IICstate   = idle_state;
    pStm32Icb[port].pDmaState  = DmaUnused;
    pStm32Icb[port].Dmabuf     = NULL;
    pStm32Icb[port].DmaRcvlen  = 0;;

    IIC_Config.BusName             = config[port].iicname;
    IIC_Config.IICBuf              = buf;
    IIC_Config.IICBufLen           = config[port].iicbufLen;
    IIC_Config.SpecificFlag        = (ptu32_t)port;
    IIC_Config.pGenerateWriteStart = IIC_WriteStartFunc;
    IIC_Config.pGenerateReadStart  = IIC_ReadStartFunc;
    IIC_Config.pGenerateEnd        = IIC_GenerateEndFunc;
    IIC_Config.pBusCtrl            = IIC_BusCtrlFunc;
    IIC_Config.pWriteReadPoll      = IIC_WriteReadPoll;

    IIC_HardInit(port);
    IIC_Int_Init(port);

    pStm32Icb[port].pIIC_CB = IIC_BusAdd(&IIC_Config);
    if(NULL ==  pStm32Icb[port].pIIC_CB )
         return false;
    return true;
}


