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
#include <device/djy_uart.h>
#include "stdlib.h"
#include "gpio_pub.h"
#include "icu_pub.h"
#include "spi/spi.h"
#include "dbug.h"
#include "spi_pub.h"
#include <arm_arch.h>
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern int ModuleInstall_SPI(void);
//    ModuleInstall_SPI();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip spi"//spi接口的norflash
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:pre-main               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"lock","spi bus","heap"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_SPI == false )
//#warning  " myname 组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_SPI    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,30000000,
#define CFG_SPI_CLK                    30000000       //SPI 时钟。
//%$#@num,0,1,
#define CFG_SPI_CPOL                     1           //spi时钟极性（1：SCK在空闲状态处于高电平。0：反之）。
#define CFG_SPI_CPHA                     1           //spi时钟相位（1：在SCK周期的第二个边沿采样数据。0：在SCK周期的第一个边沿采样数据）
//%$#@enum,true,false,
#define CFG_SPI_FLASH_RAM_POWER         true         //是否打开flash和ram的电源。
#define CFG_SPI_WORK_MODE_INTE          true        //设置SPI的工作模式，true为中断模式通信，false为普通模式。
//%$#@select
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure


//spi的物理脚和QSPI重合，这里的设置是关闭QSPI的IO2和IO3
static void Spi_Init_Extral_Gpio(void)
{
    bk_gpio_config_output(SPI_FLASH_WP_GPIO_NUM);
    bk_gpio_output(SPI_FLASH_WP_GPIO_NUM, GPIO_INT_LEVEL_HIGH);

    bk_gpio_config_output(SPI_FLASH_HOLD_GPIO_NUM);
    bk_gpio_output(SPI_FLASH_HOLD_GPIO_NUM, GPIO_INT_LEVEL_HIGH);
}

//打开flash和ram的电源
static void SPI_Flash_Enable_Voltage(void)
{
    UINT32 param;

    param = BLK_BIT_MIC_QSPI_RAM_OR_FLASH;
    sddev_control(SCTRL_DEV_NAME, CMD_SCTRL_BLK_ENABLE, &param);

    param = QSPI_IO_3_3V;
    sddev_control(SCTRL_DEV_NAME, CMD_QSPI_IO_VOLTAGE, &param);

    param = PSRAM_VDD_3_3V_DEF;
    sddev_control(SCTRL_DEV_NAME, CMD_QSPI_VDDRAM_VOLTAGE, &param);
}

//关闭flash和ram的电源
static void SPI_Flash_Disable_Voltage(void)
{
    UINT32 param;

    param = BLK_BIT_MIC_QSPI_RAM_OR_FLASH;
    sddev_control(SCTRL_DEV_NAME, CMD_SCTRL_BLK_DISABLE, &param);
}

void SPI_Flash_Deinit(void)
{
    SPI_Flash_Disable_Voltage();
    bk_spi_master_deinit();
}

bool_t SPI_TxRx(u8* sdata,u32 slen,u8* rdata, u32 rlen, u32 recvoff)
{
    s32 result;
    u32 i, len_limit, value;
    u8 ch;

    struct spi_message spi_msg;

    if( (!sdata) || ((rlen != 0) && (!rdata)))
        return false;

    if(CFG_SPI_WORK_MODE_INTE)
    {
        spi_msg.recv_buf = rdata;
        spi_msg.recv_len = rlen;
        spi_msg.send_buf = sdata;
        spi_msg.send_len = slen;

        result = bk_spi_master_xfer(&spi_msg);
        if(result != (s32)rlen)
        {
            return false;
        }
    }
    else
    {
        if((rlen == 0) && (rdata == NULL))
        {
            for (i=0; i < slen; i++)
            {
                while(1)
                {
                    value = REG_READ(SPI_STAT);

                    if((value & TXFIFO_FULL) == 0)
                    {
                        REG_WRITE(SPI_DAT, sdata[i]);
                       break;
                    }
                    DJY_EventDelay(2);
                }
            }
            while(spi_read_rxfifo(&ch) == 1);
        }
        else
        {
            while(spi_read_rxfifo(&ch) == 1);
            len_limit = MAX(slen, rlen + recvoff);
            for (i=0; i < len_limit; i++)
            {
                if (i < slen)
                {
                    while(spi_write_txfifo(sdata[i]) == 0)
                    {
                        DJY_EventDelay(2);
                    }
                }
                else if((i >= slen)&&(i < len_limit))
                {
                    while(spi_write_txfifo(0xff) == 0)
                    {
                        DJY_EventDelay(2);
                    }
                }

                if((rdata) && (i>=recvoff))
                {

                    while(spi_read_rxfifo(&ch) == 0)
                    {
                        DJY_EventDelay(2);
                    }
                    rdata[i-recvoff] = ch;
                }
                else
                {
                    while(spi_read_rxfifo(&ch) == 0)
                    {
                        DJY_EventDelay(2);
                    }
                }
            }
        }
    }

    return true;
}

static void Spi_Configure(UINT32 rate, UINT32 mode)
{
    UINT32 param;

    /* data bit width */
    param = 0;
    spi_ctrl(CMD_SPI_SET_BITWIDTH, (void *)&param);

    /* baudrate */
//    printf("max_hz = %d \n", rate);
    spi_ctrl(CMD_SPI_SET_CKR, (void *)&rate);

    /* mode */
    if (mode & BK_SPI_CPOL)
    {
        param = 1;
        spi_ctrl(CMD_SPI_SET_CKPOL, (void *)&param);
    }
    else
    {
        param = 0;
        spi_ctrl(CMD_SPI_SET_CKPOL, (void *)&param);
    }

    /* CPHA */
    if (mode & BK_SPI_CPHA)
    {
        param = 1;
        spi_ctrl(CMD_SPI_SET_CKPHA, (void *)&param);
    }
    else
    {
        param = 0;
        spi_ctrl(CMD_SPI_SET_CKPHA, (void *)&param);
    }

    /* Master */
    param = 1;
    spi_ctrl(CMD_SPI_SET_MSTEN, (void *)&param);
    param = 3;
    spi_ctrl(CMD_SPI_SET_NSSID, (void *)&param);
    param = 1;
    spi_ctrl(CMD_SPI_INIT_MSTEN, (void *)&param);

    param = PWD_SPI_CLK_BIT;
    sddev_control(ICU_DEV_NAME, CMD_CLK_PWR_UP, &param);

    param = (IRQ_SPI_BIT);
    sddev_control(ICU_DEV_NAME, CMD_ICU_INT_ENABLE, &param);

    param = GFUNC_MODE_SPI;
    sddev_control(GPIO_DEV_NAME, CMD_GPIO_ENABLE_SECOND, &param);


    /* enable spi */
    param = 1;
    spi_ctrl(CMD_SPI_UNIT_ENABLE, (void *)&param);

//    printf("[CTRL]:0x%08x \n", REG_READ(SPI_CTRL));
}


// =============================================================================
// 功能：初始化SPI模块
// 参数：无
// 返回：1=成功，0=失败
// =============================================================================
int ModuleInstall_SPI(void)
{
    u8 mode;

    spi_init();
    if(CFG_SPI_FLASH_RAM_POWER)
        SPI_Flash_Enable_Voltage();

//    Spi_Init_Extral_Gpio();

    mode = (CFG_SPI_CPOL | (CFG_SPI_CPHA << 1));

    if(CFG_SPI_WORK_MODE_INTE)
    {
        if(bk_spi_master_init(CFG_SPI_CLK, mode))
        {
            info_printf("SPI","SPI init fail.\r\n");
                return 0;
        }
    }
    else
        Spi_Configure(CFG_SPI_CLK, mode);

    return 1;
}


