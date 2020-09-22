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
#include <typedef.h>
#include "cpu_peri.h"
#include <device/include/uart.h>
#include "stdlib.h"
#include "gpio_pub.h"
#include "icu_pub.h"
#include "i2s/i2s.h"
#include "dbug.h"
#include "i2s_pub.h"
#include <arm_arch.h>
#include <general_dma_pub.h>
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern int ModuleInstall_I2S(void);
//    ModuleInstall_I2S();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip i2s"//
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"none"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_I2S == false )
//#warning  " myname 组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_I2S    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,262144,
#define CFG_I2S_RX_BUFFER_SIZE         8192       //接收环形缓冲区大小。
#define CFG_I2S_TX_BUFFER_SIZE         8192       //发送环形缓冲区大小。
//%$#@enum,GDMA_CHANNEL_0,GDMA_CHANNEL_1,GDMA_CHANNEL_2,GDMA_CHANNEL_3,GDMA_CHANNEL_4,GDMA_CHANNEL_5,GDMA_CHANNEL_MAX
#define CFG_I2S_RX_DMA_CHANNEL          GDMA_CHANNEL_4         //DMA通道数值越大的通道，优先级越高
#define CFG_I2S_TX_DMA_CHANNEL          GDMA_CHANNEL_3         //DMA通道数值越大的通道，优先级越高。
//%$#@enum,I2S_MODE,I2S_LEFT_JUSTIFIED,I2S_RIGHT_JUSTIFIED,I2S_RESERVE,I2S_SHORT_FRAME_SYNC,I2S_LONG_FRAME_SYNC,I2S_NORMAL_2B_D,I2S_DELAY_2B_D
#define CFG_I2S_MODE                    I2S_MODE               //I2S的工作模式。
//%$#@enum,I2S_LRCK_NO_TURN,I2S_LRCK_TURN
#define CFG_I2S_LRCK_SET                I2S_LRCK_NO_TURN       //I2S_LRCK_NO_TURN：低电平表示右声道，高电平表示左声道。I2S_LRCK_TURN相反
//%$#@enum,I2S_SCK_NO_TURN,I2S_SCK_TURN
#define CFG_I2S_SCK_SET                 I2S_SCK_NO_TURN        //I2S_SCK_NO_TURN: SCLK的上升沿采样，输出数据在SCLK的下降沿变化.I2S_SCK_TURN相反
//%$#@enum,I2S_MSB_FIRST,I2S_LSB_FIRST
#define CFG_I2S_MSB_LSB_SET             I2S_MSB_FIRST
//%$#@num,0,7,
#define CFG_I2S_SYNCLEN         0       //Sync长度（仅Long Frame Sync模式下有效）。1~7分别对应2~8个Bit Clock Cycle
#define CFG_I2S_PCM_DLEN        0       //2B+D的PCM模式中D的长度（单位是SCK）。
//%$#@select
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure


struct IIS_Device g_i2s;

//#define I2S_RX_BUFFER_SIZE     8192
//#define I2S_TX_BUFFER_SIZE     8192
//#define I2S_RX_DMA_CHANNEL     GDMA_CHANNEL_4       //DMA通道数值越大的通道，优先级越高
//#define I2S_TX_DMA_CHANNEL     GDMA_CHANNEL_3

#define I2S_DEFAULT_MODE (CFG_I2S_MODE| CFG_I2S_LRCK_SET| CFG_I2S_SCK_SET| CFG_I2S_MSB_LSB_SET| (CFG_I2S_SYNCLEN<<I2S_SYNC_LENGTH_BIT)| (CFG_I2S_PCM_DLEN<<I2S_PCM_DATA_LENGTH_BIT))

bool_t I2S_RX_CALLBACK = false;       //是否注册了I2S的接收回调函数
bool_t I2S_TX_CALLBACK = false;       //是否注册了I2S的发送回调函数

//返回值 0-成功；其它-失败
static s32 IIS_SetTX_DMA_PauseAddr(UINT32 addr)
{
    GDMA_CFG_ST en_cfg;
    s32 ret = 0;

    memset(&en_cfg, 0, sizeof(GDMA_CFG_ST));

    en_cfg.channel = CFG_I2S_TX_DMA_CHANNEL;
    en_cfg.param = addr;
    ret = sddev_control(GDMA_DEV_NAME, CMD_GDMA_SET_SRC_PAUSE_ADDR, &en_cfg);
    return ret;
}
//返回值 0-成功；其它-失败
static s32 IIS_SetRX_DMA_PauseAddr(UINT32 addr)
{
    GDMA_CFG_ST en_cfg;
    s32 ret = 0;

    memset(&en_cfg, 0, sizeof(GDMA_CFG_ST));

    en_cfg.channel = CFG_I2S_RX_DMA_CHANNEL;
    en_cfg.param = addr;
    ret = sddev_control(GDMA_DEV_NAME, CMD_GDMA_SET_DST_PAUSE_ADDR, &en_cfg);
    return ret;
}
// =============================================================================
// 功能：I2S DMA主从模式设置
// 参数：mode - 1：主模式；0：从模式
// 返回：0-成功；其它-失败
// =============================================================================
static s32 IIS_DMA_SetMasterSlave(u32 mode)
{
    s32 ret = 0;
    ret = sddev_control(I2S_DEV_NAME, I2S_CMD_DMA_MASTER_ENABLE, (void *)&mode);
    return ret;
}

// =============================================================================
// 功能：I2S 使能
// 参数：enable - 是否使能，1：使能；0：失能
// 返回：0-成功；其它-失败
// =============================================================================
static s32 IIS_Enable(u32 enable)
{
    s32 ret = 0;
    if (enable)
    {
        ret = sddev_control(I2S_DEV_NAME, I2S_CMD_UNIT_ENABLE, NULL);
    }
    else
    {
        ret = sddev_control(I2S_DEV_NAME, I2S_CMD_DISABLE_I2S, NULL);
    }
    return ret;
}

// =============================================================================
// 功能：I2S DMA发送使能
// 参数：enable - 是否使能，1：使能；0：失能
// 返回：0-成功；其它-失败
// =============================================================================
static s32 IIS_DMA_TxEnable(u32 enable)
{
    GDMA_CFG_ST en_cfg;
    s32 ret = 0;
    if(g_i2s.tx_enable == enable)
    {
        notice_printf("IIS","Tx current state is already %d\r\n",enable);
        return -1;
    }

    en_cfg.channel = CFG_I2S_TX_DMA_CHANNEL;
    if (enable)
        en_cfg.param = 1;
    else
        en_cfg.param = 0;
    ret = sddev_control(GDMA_DEV_NAME, CMD_GDMA_SET_DMA_ENABLE, &en_cfg);

    g_i2s.tx_enable = enable ? 1 : 0;

    return ret;
}
// =============================================================================
// 功能：I2S DMA接收使能
// 参数：enable - 是否使能，1：使能；0：失能
// 返回：无
// =============================================================================
static s32 IIS_DMA_RxEnable(u32 enable)
{
    GDMA_CFG_ST en_cfg;
    s32 ret = 0;
    if(g_i2s.rx_enable == enable)
    {
        notice_printf("IIS","Rx current state is already %d\r\n",enable);
        return -1;
    }

    en_cfg.channel = CFG_I2S_RX_DMA_CHANNEL;
    if (enable)
        en_cfg.param = 1;
    else
        en_cfg.param = 0;
    ret = sddev_control(GDMA_DEV_NAME, CMD_GDMA_SET_DMA_ENABLE, &en_cfg);

    g_i2s.rx_enable = enable ? 1 : 0;
    return ret;
}

#if I2S_RX_CALLBACK
void i2s_dma_rx_half_handler(UINT32 flag)
{
    int result;
    struct rt_i2s_bus_device *i2s = &g_i2s_bus;

    rt_kprintf("%s:%d\r\n", __FUNCTION__, __LINE__);
    //rt_kprintf("%s:%d PCM_CTRL=0x%x,PCM_CN=0x%x,PCM_STAT=0x%x\r\n", __FUNCTION__, __LINE__, REG_READ(PCM_CTRL), REG_READ(PCM_CN), REG_READ(PCM_STAT));
#ifdef PAUSE_EN
    //i2s_dma_rx_pause_addr_set((UINT32)i2s.rx_fifo + (CFG_I2S_RX_BUFFER_SIZE -4));
#endif

    int index;
    int *ptr = (int *)i2s.rx_fifo;
    for (index = 0; index < CFG_I2S_RX_BUFFER_SIZE / 8; index++, ptr++)
    {
        if (*ptr != 0)
        {
            rt_kprintf("rx[%d]=0x%x\r\n", index, *ptr);
        }
    }
}

void i2s_dma_rx_finish_handler(UINT32 flag)
{
    int result;
    struct rt_i2s_bus_device *i2s = &g_i2s_bus;

    rt_kprintf("%s:%d\r\n", __FUNCTION__, __LINE__);
    //rt_kprintf("%s:%d PCM_CTRL=0x%x,PCM_CN=0x%x,PCM_STAT=0x%x\r\n", __FUNCTION__, __LINE__, REG_READ(PCM_CTRL), REG_READ(PCM_CN), REG_READ(PCM_STAT));
#ifdef PAUSE_EN
    //i2s_dma_rx_pause_addr_set((UINT32)i2s.rx_fifo + (CFG_I2S_RX_BUFFER_SIZE / 2 -4));
#endif

    int index;
    int *ptr = (int *)i2s.rx_fifo + CFG_I2S_RX_BUFFER_SIZE / 8;
    for (index = CFG_I2S_RX_BUFFER_SIZE / 8; index < CFG_I2S_RX_BUFFER_SIZE / 4; index++, ptr++)
    {
        if (*ptr != 0)
        {
            rt_kprintf("rx[%d]=0x%x\r\n", index, *ptr);
        }
    }
}
#endif
#if I2S_TX_CALLBACK
void i2s_dma_tx_half_handler(UINT32 flag)
{
    int result;
    struct rt_i2s_bus_device *i2s = &g_i2s_bus;

    rt_kprintf("%s:%d\r\n", __FUNCTION__, __LINE__);
    //rt_kprintf("%s:%d PCM_CTRL=0x%x,PCM_CN=0x%x,PCM_STAT=0x%x\r\n", __FUNCTION__, __LINE__, REG_READ(PCM_CTRL), REG_READ(PCM_CN), REG_READ(PCM_STAT));
    result = rt_data_node_is_empty(i2s->tx_list);   //这是rtt的接口用来判断是否有数据需要发送
    if (result)
    {
        //没有数据需要发送
#ifdef PAUSE_EN
        i2s_dma_tx_pause_addr_set((UINT32)i2s->tx_fifo + (CFG_I2S_TX_BUFFER_SIZE -4));
        i2s->tx_paused = 1;
        //i2s_dma_master_enable(!i2s->tx_paused);
        i2s->tx_fill_pos = (UINT32)i2s->tx_fifo;
        i2s->tx_fill_size = CFG_I2S_TX_BUFFER_SIZE / 2;
#endif

        memset(i2s->tx_fifo, 0, CFG_I2S_TX_BUFFER_SIZE / 2);
    }
    else
    {
        //有数据需要发送
        memset(i2s->tx_fifo, 0, CFG_I2S_TX_BUFFER_SIZE / 2);
        result = rt_data_node_read(i2s->tx_list, i2s->tx_fifo, CFG_I2S_TX_BUFFER_SIZE / 2);     //获取需要发送的数据，rtt的接口

#ifdef PAUSE_EN
        if (result < (CFG_I2S_TX_BUFFER_SIZE / 2))
        {
            i2s_dma_tx_pause_addr_set((UINT32)i2s->tx_fifo + (CFG_I2S_TX_BUFFER_SIZE -4));
            i2s->tx_paused = 1;
            //i2s_dma_master_enable(!i2s->tx_paused);
            i2s->tx_fill_pos = (UINT32)i2s->tx_fifo + result;
            i2s->tx_fill_size = CFG_I2S_TX_BUFFER_SIZE / 2 - result;
        }
#endif
    }
}

void i2s_dma_tx_finish_handler(UINT32 flag)
{
    int result;
    struct rt_i2s_bus_device *i2s = &g_i2s_bus;

    rt_kprintf("%s:%d\r\n", __FUNCTION__, __LINE__);
    //rt_kprintf("%s:%d PCM_CTRL=0x%x,PCM_CN=0x%x,PCM_STAT=0x%x\r\n", __FUNCTION__, __LINE__, REG_READ(PCM_CTRL), REG_READ(PCM_CN), REG_READ(PCM_STAT));
    i2s->tx_dma_irq_cnt ++;
    result = rt_data_node_is_empty(i2s->tx_list);   //这是rtt的接口用来判断是否有数据需要发送
    if (result)
    {
    #ifdef PAUSE_EN
        i2s_dma_tx_pause_addr_set((UINT32)i2s->tx_fifo + (CFG_I2S_TX_BUFFER_SIZE / 2 -4));
        i2s->tx_paused = 1;
        //i2s_dma_master_enable(!i2s->tx_paused);
        i2s->tx_fill_pos = (UINT32)i2s->tx_fifo + CFG_I2S_TX_BUFFER_SIZE / 2;
        i2s->tx_fill_size = CFG_I2S_TX_BUFFER_SIZE / 2;
    #endif
        //rt_kprintf("* ");
        memset(i2s->tx_fifo + (CFG_I2S_TX_BUFFER_SIZE / 2), 0, CFG_I2S_TX_BUFFER_SIZE / 2);
    }
    else
    {
        memset(i2s->tx_fifo + (CFG_I2S_TX_BUFFER_SIZE / 2), 0, CFG_I2S_TX_BUFFER_SIZE / 2);
        result = rt_data_node_read(i2s->tx_list, i2s->tx_fifo + (CFG_I2S_TX_BUFFER_SIZE / 2), CFG_I2S_TX_BUFFER_SIZE / 2);  //获取需要发送的数据，rtt的接口
    #ifdef PAUSE_EN
        if (result < (CFG_I2S_TX_BUFFER_SIZE / 2))
        {
            i2s_dma_tx_pause_addr_set((UINT32)i2s->tx_fifo + (CFG_I2S_TX_BUFFER_SIZE / 2 -4));
            i2s->tx_paused = 1;
            //i2s_dma_master_enable(!i2s->tx_paused);
            i2s->tx_fill_pos = (UINT32)i2s->tx_fifo + CFG_I2S_TX_BUFFER_SIZE / 2 + result;
            i2s->tx_fill_size = CFG_I2S_TX_BUFFER_SIZE / 2 - result;
        }
    #endif
    }
}
#endif

// =============================================================================
// 功能：I2S设备接收数据初始化
// 参数：无
// 返回：0=成功，-1=失败
// =============================================================================
s32 IIS_DMA_RxInit(struct IIS_OperParam *param)
{
    GDMACFG_TPYES_ST init_cfg;
    GDMA_CFG_ST en_cfg;
    s32 res = 0;

    memset(&init_cfg, 0, sizeof(GDMACFG_TPYES_ST));
    memset(&en_cfg, 0, sizeof(GDMA_CFG_ST));

    init_cfg.dstdat_width = 32;                     //目标数据位宽
    init_cfg.srcdat_width = g_i2s.bits_width;       //源数据位宽
    init_cfg.dstptr_incr = 1;                       //目标指针地址设置为增量
    init_cfg.srcptr_incr = 0;                       //源指针地址不增量

    init_cfg.src_start_addr = (void *)PCM_DAT0;     //源数据发送起始地址
    init_cfg.dst_start_addr = g_i2s.rx_fifo;       //目标数据接收起始地址

    init_cfg.channel = CFG_I2S_RX_DMA_CHANNEL;
    init_cfg.prio = 0;                              //优先级
    //type1; loop src,    no loop dst,    no register
    //type2; no loop src, loop dst,    no register
    //type3;  loop src,    loop dst,    no register
    //type4; loop src,    no loop dst,    register
    //type5; no loop src, loop dst,    register
    //type6; loop src,    loop dst,    register
    init_cfg.u.type5.dst_loop_start_addr = g_i2s.rx_fifo;                        //目标缓冲区起始地址。
    init_cfg.u.type5.dst_loop_end_addr = g_i2s.rx_fifo + CFG_I2S_RX_BUFFER_SIZE;     //目标缓冲区结束地址。

//#if I2S_RX_CALLBACK
    if((param->i2s_rx_half_fin_handler != NULL) && (param->i2s_rx_fin_handler != NULL))
    {
        init_cfg.half_fin_handler = param->i2s_rx_half_fin_handler;     //注册回调函数
        init_cfg.fin_handler = param->i2s_rx_fin_handler;
        I2S_RX_CALLBACK = true;
    }
    else
        info_printf("IIS","RX not registered callback functions \r\n");
//#endif

    init_cfg.src_module = GDMA_X_SRC_I2S_RX_REQ;        //源模块为I2S接收
    init_cfg.dst_module = GDMA_X_DST_DTCM_WR_REQ;       //目标模块为DTCM写

    res = sddev_control(GDMA_DEV_NAME, CMD_GDMA_CFG_TYPE5, (void *)&init_cfg);
    if(res != 0)
    {
        error_printf("IIS"," IIS_DMA_RxInit fail\r\n");
        return -1;
    }
    en_cfg.channel = CFG_I2S_RX_DMA_CHANNEL;
    en_cfg.param = CFG_I2S_RX_BUFFER_SIZE; // dma 接收缓存大小
    res = sddev_control(GDMA_DEV_NAME, CMD_GDMA_SET_TRANS_LENGTH, (void *)&en_cfg);
    if(res != 0)
    {
        error_printf("IIS"," IIS_DMA_RxInit fail\r\n");
        return -1;
    }
//#if !I2S_RX_CALLBACK
    if(I2S_RX_CALLBACK == false)
        rb_init_dma_write(&g_i2s.rb_dma_wr, g_i2s.rx_fifo, CFG_I2S_RX_BUFFER_SIZE, CFG_I2S_RX_DMA_CHANNEL);
    else
        info_printf("IIS","RX not init ring buf \r\n");
//#endif

    return 0;
}
// =============================================================================
// 功能：I2S设备发送数据初始化
// 参数：无
// 返回：0=成功，-1=失败
// =============================================================================
s32 IIS_DMA_TxInit(struct IIS_OperParam *param)
{
    GDMACFG_TPYES_ST init_cfg;
    GDMA_CFG_ST en_cfg;
    s32 res = 0;

    memset(&init_cfg, 0, sizeof(GDMACFG_TPYES_ST));
    memset(&en_cfg, 0, sizeof(GDMA_CFG_ST));

    init_cfg.dstdat_width = g_i2s.bits_width;       //目标数据位宽
    init_cfg.srcdat_width = 32;                     //源数据位宽
    init_cfg.dstptr_incr = 0;                       //目标指针地址设置为不增量
    init_cfg.srcptr_incr = 1;                       //源指针地址不增量

    init_cfg.src_start_addr = g_i2s.tx_fifo;        //源数据发送起始地址
    init_cfg.dst_start_addr = (void *)PCM_DAT0;     //目标数据接收起始地址

    init_cfg.channel = CFG_I2S_TX_DMA_CHANNEL;
    init_cfg.prio = 0;                              //优先级
    init_cfg.u.type4.src_loop_start_addr = g_i2s.tx_fifo;                        //源缓冲区起始地址。
    init_cfg.u.type4.src_loop_end_addr = g_i2s.tx_fifo + CFG_I2S_TX_BUFFER_SIZE;     //源缓冲区结束地址。
//#if    I2S_TX_CALLBACK
    if((param->i2s_tx_fin_handler != NULL) && (param->i2s_tx_half_fin_handler != NULL))
    {
        init_cfg.half_fin_handler = param->i2s_tx_half_fin_handler;
        init_cfg.fin_handler = param->i2s_tx_fin_handler;
        I2S_TX_CALLBACK = true;
    }
    else
        info_printf("IIS","TX not registered callback functions \r\n");
//#endif
    init_cfg.src_module = GDMA_X_SRC_DTCM_RD_REQ;       //源模块为DTCM读
    init_cfg.dst_module = GDMA_X_DST_I2S_TX_REQ;        //目标模块为I2S发送

    res = sddev_control(GDMA_DEV_NAME, CMD_GDMA_CFG_TYPE4, (void *)&init_cfg);
    if(res != 0)
    {
        error_printf("IIS"," IIS_DMA_TxInit fail\r\n");
        return -1;
    }
    en_cfg.channel = CFG_I2S_TX_DMA_CHANNEL;
    en_cfg.param = CFG_I2S_TX_BUFFER_SIZE; // dma 发送缓存大小
    res = sddev_control(GDMA_DEV_NAME, CMD_GDMA_SET_TRANS_LENGTH, (void *)&en_cfg);
    if(res != 0)
    {
        error_printf("IIS"," IIS_DMA_TxInit fail\r\n");
        return -1;
    }
//#if !I2S_TX_CALLBACK
    if(I2S_TX_CALLBACK == false)
    {
        rb_init_dma_read(&g_i2s.wr_dma_rd, g_i2s.tx_fifo, CFG_I2S_RX_BUFFER_SIZE, CFG_I2S_TX_DMA_CHANNEL);
    }
    else
        info_printf("IIS","TX not init ring buf \r\n");
//#endif
    return res;
}

// =============================================================================
// 功能：打开I2S设备
// 参数：sample - 采样率， bit_width - 位宽，mode -打开模式，O_RDONLY、O_WRONLY、O_RDWR
// 返回：0=成功，-1=失败
// =============================================================================
s32 IIS_Open(struct IIS_OperParam *param, u32 mode)
{
    if(mode == 0)
    {
        error_printf("IIS"," mode is error. mode = %x\r\n", mode);
        return -1;
    }

    g_i2s.open_flag = mode;
    g_i2s.tx_paused = 1;
    g_i2s.tx_fill_pos = (u32)g_i2s.tx_fifo;
    g_i2s.tx_fill_size = CFG_I2S_TX_BUFFER_SIZE / 2;
    g_i2s.sample_rate = param->sample_rate;
    g_i2s.bits_width = param->bits_width;

    i2s_configure(FIFO_LEVEL_32, g_i2s.sample_rate, g_i2s.bits_width, I2S_DEFAULT_MODE);

    if(mode & O_RDONLY)
    {
        g_i2s.rx_fifo = malloc(CFG_I2S_RX_BUFFER_SIZE);   //申请i2s，DMA功能的接收缓冲区
        if (g_i2s.rx_fifo == NULL)
        {
            error_printf("IIS","malloc rx_fifo failed\r\n");
            return -1;
        }
        memset(g_i2s.rx_fifo, 0, CFG_I2S_RX_BUFFER_SIZE);
        if(IIS_DMA_RxInit(param) == 0)
        {
            if(IIS_SetRX_DMA_PauseAddr(0) != 0)
            {
                free(g_i2s.rx_fifo);
                return -1;
            }
        }
        else
        {
            free(g_i2s.rx_fifo);
            return -1;
        }
    }

    if(mode & O_WRONLY)
    {
        g_i2s.tx_fifo = malloc(CFG_I2S_TX_BUFFER_SIZE);    //申请i2s，DMA功能的发送缓冲区
        if (g_i2s.tx_fifo == NULL)
        {
            error_printf("IIS"," malloc tx_fifo failed\r\n");
            if(mode & O_RDONLY)
                free(g_i2s.rx_fifo);
            return -1;
        }
        memset(g_i2s.tx_fifo, 0, CFG_I2S_TX_BUFFER_SIZE);
        if(IIS_DMA_TxInit(param) == 0)
        {
            if(IIS_SetTX_DMA_PauseAddr((UINT32)g_i2s.tx_fifo) != 0)
            {
                free(g_i2s.tx_fifo);
                if(mode & O_RDONLY)
                    free(g_i2s.rx_fifo);
                return -1;
            }
        }
        else
        {
            free(g_i2s.tx_fifo);
            if(mode & O_RDONLY)
                free(g_i2s.rx_fifo);
            return -1;
        }
    }

    return 0;
}

// =============================================================================
// 功能：I2S设备接收
// 参数：buffer - 接收缓冲区；size - 缓冲区大小
// 返回：实际读取的大小
// =============================================================================
s32 IIS_Read(void *buffer, s32 size)
{
    s32 fill_size = 0;
    if(!(g_i2s.open_flag & O_RDONLY))
        return 0;

#if CFG_GENERAL_DMA
    if((I2S_RX_CALLBACK == false) && (g_i2s.rb_dma_wr.address) && (g_i2s.rb_dma_wr.capacity))
    {
        fill_size = rb_get_fill_size_dma_write(&g_i2s.rb_dma_wr);
        if(fill_size > size)
            fill_size = size;
        rb_read_dma_write(&g_i2s.rb_dma_wr, (UINT8 *)buffer, fill_size, 1);
    }
#endif

    return fill_size;
}

// =============================================================================
// 功能：I2S设备发送
// 参数：buffer - 接收缓冲区；size - 缓冲区大小
// 返回：实际读取的大小
// =============================================================================
s32 IIS_Write(void *buffer, s32 size)
{
    s32 fill_size = 0;
    if(!(g_i2s.open_flag & O_WRONLY))
        return 0;

#if CFG_GENERAL_DMA
    if((I2S_TX_CALLBACK == false) && (g_i2s.wr_dma_rd.address) && (g_i2s.wr_dma_rd.capacity))
    {
        fill_size = rb_get_free_size_dma_read(&g_i2s.wr_dma_rd);
        if(fill_size > size)
            fill_size = size;
        rb_write_dma_read(&g_i2s.wr_dma_rd, (UINT8 *)buffer, fill_size, 1);
    }
#endif

    return fill_size;
}

// =============================================================================
// 功能：I2S设备关闭
// 参数：无
// 返回：0=成功，-1=失败
// =============================================================================
s32 IIS_Close(void)
{
    s32 ret = -1;
    if (g_i2s.open_flag)
    {
        if(g_i2s.open_flag & O_WRONLY)
        {
            free(g_i2s.tx_fifo);
            ret = IIS_DMA_TxEnable(0);
        }
        if(g_i2s.open_flag & O_RDONLY)
        {
            free(g_i2s.rx_fifo);
            ret = IIS_DMA_RxEnable(0);
        }
        if(ret == 0)
        {
            //i2s_dma_enable(0);
            g_i2s.open_flag = 0;
            g_i2s.tx_paused = 0;
            if(IIS_Enable(0) == 0)
                ret = 0;
        }
        memset(&g_i2s, 0, sizeof(g_i2s));
    }

    return ret;
}

// =============================================================================
// 功能：I2S设备控制
// 参数：无
// 返回：0=成功，其它=失败
// =============================================================================
s32 IIS_Cotrol(enum i2s_ctrl cmd, ptu32_t data)
{
    s32 ret = 0;
//    i2s_rate_t rate;
    switch (cmd)
    {
        case IIS_DMA_RX_ENABLE:
            ret = IIS_DMA_RxEnable(data);
            break;
        case IIS_DMA_TX_ENABLE:
            ret = IIS_DMA_TxEnable(data);
            break;
        case I2S_DMA_MASTER_SLAVE:
            ret = IIS_DMA_SetMasterSlave(data);
            break;
//        case I2S_SAMPLE_RATE_SET:
//        case I2S_BIT_WIDTH_SET:
//            if ((g_i2s.sample_rate != data) && (g_i2s.bits_width != data))
//            {
//                IIS_Close();
////                if(data > 32)
////                memset(&g_i2s, 0, sizeof(g_i2s));
//                    g_i2s.sample_rate = 44100;
////                else
//                    g_i2s.bits_width = 16;
//                IIS_Open(g_i2s.sample_rate, g_i2s.bits_width, 1);
//                IIS_Cotrol(I2S_DMA_MASTER_SLAVE, 1);
//                IIS_Cotrol(IIS_DMA_TX_ENABLE, 1);
////                ret = i2s_configure(FIFO_LEVEL_32, g_i2s.sample_rate, g_i2s.bits_width, I2S_DEFAULT_MODE);
////                if(ret == 0)
////                {
////                    if (g_i2s.open_flag & O_RDONLY)
////                    {
////                        ret = IIS_DMA_RxInit();
////                        if(ret == 0)
////                            ret = IIS_SetRX_DMA_PauseAddr(0);
////                    }
////                    if (g_i2s.open_flag & O_WRONLY)
////                    {
////                        ret = IIS_DMA_TxInit();
////                        if(ret == 0)
////                            ret = IIS_SetTX_DMA_PauseAddr(0);
////                    }
////                }
//            }
//            break;

        default:
            notice_printf("IIS", "unprocessed commands %d\r\n",cmd);
            break;
    }

    return ret;
}

// =============================================================================
// 功能：初始化I2S模块
// 参数：无
// 返回：1=成功，0=失败
// =============================================================================
int ModuleInstall_I2S(void)
{
    i2s_init(0);    //安装i2s模块，不使能i2s中断。
    return 1;
}

