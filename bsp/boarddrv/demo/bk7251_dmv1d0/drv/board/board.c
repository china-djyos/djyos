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


#include "stdint.h"
#include "stddef.h"
#include <int.h>
#include "mem_pub.h"
#include "wdt_pub.h"
#include <shell.h>
#include "cpu_peri.h"
#include "board.h"
#include "icu_pub.h"
#include "general_dma_pub.h"
#include "bk_timer_pub.h"
#include "pwm_pub.h"
#include "mac_phy_bypass_pub.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"board config"//组件名
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:required               //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"cpu onchip gpio"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_BOARD_CONFIG == false )
//#warning  " board_config  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_BOARD_CONFIG    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure



// =============================================================================
// 功能：Board_GpioInit板上所有用到的GPIO初始化
//    在这里将所有可能用到的复用模式的gpio定义在这里需要用取消注释即可
//    建议不要删除
// 参数：无
// 返回：无
// =============================================================================
//
//static struct SemaphoreLCB* p7_sem=0;
//void p7_sem_init()
//{
//    if (p7_sem==0) {
//        p7_sem = semp_init(1,1,"p7_sem");
//    }
//}
//
//void p7_isr_hdr(void *args)
//{
//    if (p7_sem) {
//         Lock_SempPost(p7_sem);
//    }
//}
//
//int pend_p7_down(unsigned int timeout)
//{
//    int ret = 0;
//    ret = Lock_SempPend(p7_sem, timeout);
//    return ret;
//}
//
//电容触摸芯片IIC接口初始化
void FT6236_Pin_Init(void)
{
    djy_gpio_mode(GPIO4,PIN_MODE_OUTPUT);        //SCL
    djy_gpio_mode(GPIO2,PIN_MODE_OUTPUT);        //SDA

    djy_gpio_mode(GPIO6,PIN_MODE_INPUT_PULLUP);  //RT_INT
    djy_gpio_mode(GPIO7,PIN_MODE_OUTPUT);        //RT_RST
}

u32 IIC_IoCtrlFunc(enum IIc_Io IO,u32 tag)
{
    switch(tag)
    {
        case 1 :
            switch(IO)
            {
            case scl_set_High : CT_IIC_SCL(1); break;
            case scl_set_Low  : CT_IIC_SCL(0); break;
            case scl_set_out  : CT_SCL_OUT();  break;
            case sda_set_High : CT_IIC_SDA(1); break;
            case sda_get      : return CT_READ_SDA;
            case sda_set_Low  : CT_IIC_SDA(0); break;
            case sda_set_out  : CT_SDA_OUT();  break;
            case sda_set_in   : CT_SDA_IN();   break;
            default:
                break;
            }
        break;
    }
    return 0;
}
void OpenBackLight()
{
    djy_gpio_write(GPIO10,1);
}

void CloseBackLight()
{
    djy_gpio_write(GPIO10,0);
}

void OpenScreen()
{
    djy_gpio_write(GPIO11,1);
    djy_gpio_write(GPIO10,1);
    FT_RST(1);
}

void CloseScreen()
{
    djy_gpio_write(GPIO10,0);
    FT_RST(0);
    djy_gpio_write(GPIO11,0);
}

static enum SpeakerState Speaker = Speaker_off;
void CloseSpeaker()
{
    djy_gpio_write(GPIO9,0);
    Speaker = Speaker_off;
}
void OpenSpeaker()
{
    djy_gpio_write(GPIO9,1);
    Speaker = Speaker_on;
}
enum SpeakerState GetSpeakerState()
{
    return Speaker;
}

// =============================================================================
// 功能：用于设置系统时钟为180M，该函数在initcpu.S中调用，
// 参数：无
// 返回：无
// =============================================================================
extern UINT32 sctrl_ctrl(UINT32 cmd, void *param);
extern void sctrl_set_cpu_clk_dco(void);
extern void sctrl_dco_cali(UINT32 speed);
void InitOsClk_180M(void)
{
    u32 param;
    param = BLK_BIT_26M_XTAL | BLK_BIT_DPLL_480M | BLK_BIT_XTAL2RF | BLK_BIT_DCO;
    sctrl_ctrl(CMD_SCTRL_BLK_ENABLE, &param);

    sctrl_dco_cali(5);      //#define DCO_CALIB_180M          0x5
    sctrl_set_cpu_clk_dco();
}

void Board_Init(void)
{
    extern void os_clk_init(void);
    extern s32 Djy_GpioInit(void);
    Djy_GpioInit();
    os_meminit();
    drv_model_init();
//  g_dd_init();
    sctrl_init();
    icu_init();
    wdt_init();
    gpio_init();
    flash_init();       //本函数不能放到ModuleInstall_Flash中，sctrl_sta_ps_init调用时需要访问flash设备。
    gdma_init();
    bk_timer_init();
    mpb_init();                         //暂时先放这里
    sctrl_sta_ps_init();
    pwm_init();         //用于os_clk_init做CPU时钟

    intc_init();
    os_clk_init();

//    void ble_init(void);
//    ble_init();
#if 0
    djy_gpio_mode(GPIO7,PIN_MODE_INPUT_PULLUP);  //wifi一键配置
    djy_gpio_mode(GPIO3,PIN_MODE_INPUT_PULLUP);  //音效模式
    djy_gpio_mode(GPIO4,PIN_MODE_INPUT_PULLUP);  //变声模式

    djy_gpio_mode(GPIO13,PIN_MODE_INPUT_PULLUP);   //语音按键

    djy_gpio_mode(GPIO9,PIN_MODE_OUTPUT);        //LED
    djy_gpio_write(GPIO9,0);

    djy_gpio_mode(GPIO8,PIN_MODE_OUTPUT);        //WIFI连接状态
    djy_gpio_write(GPIO8,0);
#else
//    djy_gpio_mode(GPIO13,PIN_MODE_INPUT_PULLUP);  //wifi一键配置
//    djy_gpio_mode(GPIO10,PIN_MODE_INPUT_PULLUP);  //读取蓝牙状态

//    p7_sem_init();
//    djy_gpio_mode(GPIO7,PIN_MODE_INPUT_PULLUP);   //语音按键
//    djy_gpio_attach_irq(GPIO7, PIN_IRQ_MODE_FALLING, p7_isr_hdr, NULL);
//    djy_gpio_irq_enable( GPIO7, 1);


    djy_gpio_mode(GPIO13, GMODE_INPUT_PULLUP); //唤醒键
    djy_gpio_mode(GPIO10,PIN_MODE_OUTPUT);         //液晶背光
    djy_gpio_mode(GPIO8,PIN_MODE_INPUT_PULLUP);          //按键
    djy_gpio_mode(GPIO9,PIN_MODE_INPUT_PULLUP);          //按键
//    djy_gpio_write(GPIO10,1);

    djy_gpio_mode(GPIO11,PIN_MODE_OUTPUT);        //液晶+触摸屏电源控制管脚
    djy_gpio_write(GPIO11,1);

#endif
//    void uart1_exit(void);
//    uart1_exit();
}

void Init_Cpu(void);
void SoftReset(void)
{
    Init_Cpu();
}

//由于板件的jtag口被psram占用，导致出了问题无法插入仿真器，本函数禁止psram，使能jtag，
//关闭中断后进入死循环，允许仿真器插入调试，用法有两种：
//1、如果shell还有响应，可以执行jtag命令调用本函数。
//2、插入监视代码，当检测到错误时，主动调用本函数，等待仿真器插入。
void stub_debug(void)
{
    atom_high_t atom;
    volatile u32 t = 1;
    atom = Int_HighAtomStart();
    printk("+++++++++enable jtag，you can contact ICE debuger++++++++++++\r\n");
    EnJtag();
    if(t)
        while(1);
    Int_HighAtomEnd(atom);
}
bool_t init_jtag(char *param)
{
    (void)param;
    sddev_control(WDT_DEV_NAME, WCMD_POWER_DOWN, 0);
    stub_debug();
    return true;
}

ADD_TO_ROUTINE_SHELL(initjtag,init_jtag,"重新初始化 :COMMAND:init_jtag+enter");

void ST7789V_GpioInit(void)
{
    gpio_config(GPIO15, GMODE_OUTPUT);  //1=读写数据端口，0=读写命令端口
    gpio_output(GPIO15, 1);

    gpio_config(GPIO19, GMODE_OUTPUT);  //rst
    gpio_output(GPIO19, 1);

    gpio_config(GPIO3, GMODE_OUTPUT);   //CS
    gpio_output(GPIO3, 1);
}

void SPI_RST(u32 value)
{
    djy_gpio_write(GPIO19, value);
}

void SPI_RS(u32 value)
{
    djy_gpio_write(GPIO15, value);
}

void ST7789V_CsActive(void)
{
    gpio_output(GPIO3, 0);
}
void ST7789V_CsInactive(void)
{
    gpio_output(GPIO3, 1);
}


