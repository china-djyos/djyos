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

// 文件名     ：
// 模块描述:
// 模块版本: V1.00
// 创建人员:
// 创建时间:
// =============================================================================
#ifdef CFG_CPU_ZQ12XX_M0
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
#include "cpu_peri_pwm.h"
#include "djyos.h"
#include "stdio.h"

//#include "silan_pwm_regs.h"
//#include "silan_types.h"
#include "silan_syscfg.h"
#include "silan_iomux.h"
#include "silan_iomux.h"

//Reg define
typedef struct
{
    vu32  PWMPSC;    //0x00
    vu32  PWM0D;      //0x04
    vu32  PWM1D;     //0x08
    vu32  PWM2D;     //0x0c
    vu32  PWM3D;     //0x10
    vu32  PWM4D;     //0x14
    vu32  PWM5D;     //0x18
    vu32  PWM6D;         //0x1c
    vu32  PWM7D;         //0x20
    vu32  PWMP01;    //0x24
    vu32  PWMP23;    //0x28
    vu32  PWMP45;    //0x2c
    vu32  PWMP67;    //0x30
    vu32  PWMCON;    //0x34
}tagPWMReg;

#define CN_PWM_CHANNEL_START     ( CN_PWM_CHANNEL0 )
#define CN_PWM_CHANNEL_END       ( CN_PWM_CHANNEL7 )

//channel number select
#define CHANNEL_NUM_GET(channelNum)     (1<< (channelNum+4) )

//base PWM base addr
#define BASE_PWM_ADDR  ( 0x410D0000 )

static tagPWMReg volatile *const sPWMReg = (tagPWMReg*) BASE_PWM_ADDR;

// =============================================================================
//功能: 打开对应的通道
//参数：
//返回：无
// =============================================================================
static void __CPU_PWM_ChannelOpen(u8 channelNum)
{
    sPWMReg->PWMCON  |= CHANNEL_NUM_GET(channelNum);
    return ;
}

// =============================================================================
//功能: 设置占空比
//参数：
//返回：无
// =============================================================================

static void __CPU_PWM_ChannelDutySet(u8 channelNum,u32 para)
{
    switch(channelNum)
    {
            case CN_PWM_CHANNEL0: sPWMReg->PWM0D = (para-1);break;
            case CN_PWM_CHANNEL1: sPWMReg->PWM1D = (para-1);break;
            case CN_PWM_CHANNEL2: sPWMReg->PWM2D = (para-1);break;
            case CN_PWM_CHANNEL3: sPWMReg->PWM3D = (para-1);break;
            case CN_PWM_CHANNEL4: sPWMReg->PWM4D = (para-1);break;
            case CN_PWM_CHANNEL5: sPWMReg->PWM5D = (para-1);break;
            case CN_PWM_CHANNEL6: sPWMReg->PWM6D = (para-1);break;
            case CN_PWM_CHANNEL7: sPWMReg->PWM7D = (para-1);break;
            default:printk("__CPU_ChannelParaSet func arg channelNum Err\r\n");
            return;
    }

}

// =============================================================================
//功能: 设置占空比周期
//参数：
//返回：无
// =============================================================================

static void __CPU_PWM_ChannelPeriodSet(u8 channelNum,u32 para)
{
    u8 channelSet;
    channelSet = (u8)(channelNum/2);

    switch(channelSet)
    {
        case CN_PWM_CHANNEL0:
             sPWMReg->PWMP01 = (para-1);
             break;
        case CN_PWM_CHANNEL1:
             sPWMReg->PWMP23 = (para-1);
             break;
        case CN_PWM_CHANNEL2:
             sPWMReg->PWMP45 = (para-1);
             break;
        case CN_PWM_CHANNEL3:
             sPWMReg->PWMP67 = (para-1);
             break;
        default:printk("__CPU_ChannelPeriodSet func arg channelNum Err\r\n");
        return;
    }
}

// =============================================================================
//功能: PWM 预分频设置
//参数： preDiv 分频值0~255
//返回：无
// =============================================================================

static void __CPU_PWM_PreDivSet(u8 preDiv)
{
    sPWMReg->PWMPSC = preDiv -1;
}

// =============================================================================
//功能: PWM 时钟源以及分配选择配置
//参数：
//返回：无
// =============================================================================

static void __CPU_PWM_ClkConfig(u8 clkSelect,u8 onOff)
{

    if(clkSelect > EN_PWM_RCL_CLK_DIV2)
    {
        printk("__CPU_PWM_ClkConfig clkSelect Err\r\n");
        return;
    }

    silan_pwm_cclk_config(clkSelect, onOff);
}

// =============================================================================
//功能: 配置IO驱动能力
//参数：pin:具备配置为pwm 输出的管脚号 drvLevel 驱动电流
//
//返回：无
// =============================================================================

static void __CPU_ConfigIODrvLevel(u32 pin,u32 drvLevel)
{
    silan_io_driver_config(pin,drvLevel);
}


// =============================================================================
//功能: 配置IO驱动能力
//参数：pin:具备配置为pwm 输出的管脚号 drvLevel 驱动电流
//
//返回：无
// =============================================================================

static void __CPU_PWM_IO_Config(u32 io, u32 x_para, u32 x_period)
{
    u32 channelNo =  io;
    channelNo = channelNo % 8;                          //管脚对应的通道号

    if(x_para == 0 || x_period == 0)
    {
        printk("__CPU_PWM_IO_Config func arg x_para or x_period Err\r\n");
        return;
    }else
    {
        __CPU_PWM_ChannelDutySet  (channelNo,x_para);   //设置 占空比

        __CPU_PWM_ChannelPeriodSet(channelNo,x_period); //设置周期

        __CPU_PWM_ChannelOpen(channelNo);         //打开对应的PWM 通道

        silan_io_func_config(io, IO_FUNC_PWM);          //启动PWM复用功能
    }

}


// =============================================================================
//功能: PWM 模块安装
//进行对该模块的一些默认初始化
//
//返回：无
// =============================================================================

#define CN_DEFAULT_PRE (60)

void PWM_ModuleInstall(void)
{
    __CPU_PWM_ClkConfig(EN_PWM_PLL_REF_CLK_DIV2,EN_PWM_CLK_ON);//默认设置PWM时钟源以及分频系数
    __CPU_PWM_PreDivSet(CN_DEFAULT_PRE);                       //设置默认预分频为60
    //配置默认输出管脚,以及对应的驱动能力
    __CPU_ConfigIODrvLevel(IO_CONFIG_PB8,EN_IO_DRV_8MS);
    __CPU_PWM_IO_Config(IO_CONFIG_PB8, 5, 100);
}

// =============================================================================
//功能: PWM 模块安装
//进行对该模块的一些默认初始化
//
//返回：无
// =============================================================================

bool_t PWM_Ctrl(u32 cmd,u32 para1,u32 para2,u32 para3)
{
    bool_t ret = false;

    switch(cmd)
    {
        case EN_PWM_PRE_DIV_SET:
            if(para1 > 255)
            {
                printk("para1 out of range\r\n");
                ret = false ;
            }else
                __CPU_PWM_PreDivSet((u8)para1);
            break;
        case EN_PWM_CLK_SET:
            __CPU_PWM_ClkConfig((u8)para1,(u8) para2);
            break;
        case EN_IO_DRV_SET:
            __CPU_ConfigIODrvLevel(para1,para2);
            break;
        case EN_PWM_PARA:
            __CPU_PWM_IO_Config(para1, para2, para3);
            break;
        default:;break;
    }

    return ret;
}

#endif



































































































































































































































