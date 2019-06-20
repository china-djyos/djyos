//----------------------------------------------------
// Copyright (c) 2018, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2018，著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
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
//所属模块:
//作者：
//版本：V1.0.0
//文件描述:
//其他说明:
//修订历史:
//2. ...
//1. 日期:
//   作者:
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------

#include <typedef.h>
#include "drv_model_pub.h"
#include "sys_ctrl_pub.h"
#include "saradc_pub.h"

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip adc"       //adc组件
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"cpu onchip gpio"  //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_ADC == false )
//#warning  "   ADC组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_ADC    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,flase,
//%$#@string,1,10,
//%$#select,        ***定义无值的宏，仅用于第三方组件
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束

//@#$%component end configure


static saradc_desc_t tmp_single_desc;
static UINT16 tmp_single_buff[10];//ADC_TEMP_BUFFER_SIZE];
static volatile DD_HANDLE tmp_single_hdl = DD_HANDLE_UNVALID;


void djy_adc_mode(uint32_t channel, uint32_t mode)
{
//    switch (mode)
//    {
//    case PIN_MODE_INPUT:
//        bk_gpio_config_input(pin);
//        break;
//
//    case PIN_MODE_INPUT_PULLUP:
//        bk_gpio_config_input_pup(pin);
//        break;
//
//    case PIN_MODE_INPUT_PULLDOWN:
//        bk_gpio_config_input_pdwn(pin);
//        break;
//
//    case PIN_MODE_OUTPUT:
//        bk_gpio_config_output(pin);
//        break;
//    }
}

float djy_adc_read(uint16_t pin)
{
//    return saradc_calculate(pin);
//    UINT32 cmd;
//    UINT8 mode;
//
//    cmd = SARADC_CMD_SET_MODE;
//    mode = 3;
//    UINT32 retDdev = ddev_control(tmp_single_hdl, cmd, &mode);
//
//    cmd = SARADC_CMD_SET_CHANNEL;
//    saradc_chan_t chnl;
//    chnl.enable = 1;
//    chnl.channel = 1;
//    retDdev = ddev_control(tmp_single_hdl, cmd, &chnl);
//
//    cmd = SARADC_CMD_RUN_OR_STOP_ADC;
//    UINT8 run_stop = 1;
//    retDdev = ddev_control(tmp_single_hdl, cmd, &run_stop);
//
//
//    printf("Can't open saradc, have you register this device?\r\n");



    UINT32 status;
    UINT32 cmd;
    UINT8 run_stop;

    os_memset(&tmp_single_desc, 0x00, sizeof(saradc_desc_t));
    tmp_single_desc.channel = 1;
    tmp_single_desc.data_buff_size = 10;
    tmp_single_desc.mode = (ADC_CONFIG_MODE_CONTINUE << 0)
                           | (ADC_CONFIG_MODE_8CLK_DELAY << 2);

    tmp_single_desc.has_data                = 0;
    tmp_single_desc.current_read_data_cnt   = 0;
    tmp_single_desc.current_sample_data_cnt = 0;
    tmp_single_desc.pre_div = 0x10;
    tmp_single_desc.samp_rate = 0x20;
    memset(tmp_single_buff, 0, sizeof(tmp_single_buff));
    tmp_single_desc.pData = &tmp_single_buff[0];

    tmp_single_hdl = ddev_open(SARADC_DEV_NAME, &status, (UINT32)&tmp_single_desc);

    cmd = SARADC_CMD_RUN_OR_STOP_ADC;
    run_stop = 1;
    ddev_control(tmp_single_hdl, cmd, &run_stop);

    while (1)
    {
        if (tmp_single_desc.current_sample_data_cnt == tmp_single_desc.data_buff_size)
        {
            ddev_close(tmp_single_hdl);
            break;
        }
    }

    printf("buff:%p,%d,%d,%d,%d,%d\r\n", tmp_single_desc.pData,
                   tmp_single_desc.pData[0], tmp_single_desc.pData[1],
                   tmp_single_desc.pData[2], tmp_single_desc.pData[3],
                   tmp_single_desc.pData[4]);

    float voltage = saradc_calculate(tmp_single_desc.pData[4]);

    printf("voltage is [%f]\r\n", voltage);



    return SARADC_SUCCESS;
//    extern UINT32 ddev_control(DD_HANDLE handle, UINT32 cmd, VOID *param);
}

//int djy_adc_attach_irq(GPIO_INDEX pin,uint32_t mode,
//                            void (*hdr)(void *args), void *args)
//{
//    if (pin >= GPIONUM)
//        return -1;
//
//    gpio_dev.irq_desc[pin].handler = hdr;
//    gpio_dev.irq_desc[pin].param   = args;
//    if (mode == PIN_IRQ_MODE_RISING)
//    {
//        gpio_dev.irq_desc[pin].mode = GMODE_INPUT_PULLDOWN;
//    }
//    else if (mode == PIN_IRQ_MODE_FALLING)
//    {
//        gpio_dev.irq_desc[pin].mode = GMODE_INPUT_PULLUP;
//    }
//    return 0;
//}
//
//
//int djy_adc_irq_enable( GPIO_INDEX pin, uint32_t enabled)
//{
//    if (pin >= GPIONUM)
//        return -1;
//
//    if (enabled)
//        gpio_int_enable(pin, gpio_dev.irq_desc[pin].mode, gpio_irq_dispatch);
//    else
//        gpio_int_disable(pin);
//    return 0;
//}

static void temp_single_detect_handler2(void)
{
    if(tmp_single_desc.current_sample_data_cnt >= tmp_single_desc.data_buff_size)
    {
        #if (CFG_SOC_NAME != SOC_BK7231)
        UINT32 sum = 0, sum1, sum2;
        //turnon_PA_in_temp_dect();
//        temp_single_get_disable();
        printf("buff:%p,%d,%d,%d,%d,%d\r\n", tmp_single_desc.pData,
                       tmp_single_desc.pData[0], tmp_single_desc.pData[1],
                       tmp_single_desc.pData[2], tmp_single_desc.pData[3],
                       tmp_single_desc.pData[4]);

        sum1 = tmp_single_desc.pData[1] + tmp_single_desc.pData[2];
        sum2 += tmp_single_desc.pData[3] + tmp_single_desc.pData[4];
        sum = sum1 / 2 + sum1 / 2;
        sum = sum / 2;
        tmp_single_desc.pData[0] = sum;
        #else
        turnon_PA_in_temp_dect();
        temp_single_get_disable();
        printf("buff:%p,%d,%d,%d,%d,%d\r\n", tmp_single_desc.pData,
                       tmp_single_desc.pData[0], tmp_single_desc.pData[1],
                       tmp_single_desc.pData[2], tmp_single_desc.pData[3],
                       tmp_single_desc.pData[4]);
        #endif // (CFG_SOC_NAME != SOC_BK7231)

//        rtos_set_semaphore(&tmp_single_semaphore);
    }
}

static void temp_single_get_desc_init(void)
{
//    os_memset(&tmp_single_buff[0], 0, sizeof(tmp_single_buff));
//
////    saradc_config_param_init(&tmp_single_desc);
//    tmp_single_desc.channel = ADC_TEMP_SENSER_CHANNEL;
//    tmp_single_desc.pData = &tmp_single_buff[0];
//    tmp_single_desc.data_buff_size = ADC_TEMP_BUFFER_SIZE;
//    tmp_single_desc.p_Int_Handler = temp_single_detect_handler;
}

void ModuleInstall_ADC(void)
{
//    GLOBAL_INT_DECLARATION();

//    temp_single_get_desc_init();
//
//    status = BLK_BIT_TEMPRATURE_SENSOR;
//    sddev_control(SCTRL_DEV_NAME, CMD_SCTRL_BLK_ENABLE, &status);
//
//#if (CFG_SOC_NAME == SOC_BK7231)
//    turnoff_PA_in_temp_dect();
//#endif // (CFG_SOC_NAME == SOC_BK7231)

//    GLOBAL_INT_DISABLE();
//    saradc_config_param_init(&tmp_single_desc);
//    tmp_single_desc.pData = &tmp_single_buff[0];
//    tmp_single_desc.data_buff_size = 10;
//    tmp_single_desc.p_Int_Handler = temp_single_detect_handler2;
//
//    tmp_single_hdl = ddev_open(SARADC_DEV_NAME, &status, (UINT32)&tmp_single_desc);
//    if(DD_HANDLE_UNVALID == tmp_single_hdl)
//    {
////        GLOBAL_INT_RESTORE();
//        printf("Can't open saradc, have you register this device?\r\n");
//        return SARADC_FAILURE;
//    }
//    GLOBAL_INT_RESTORE();
}

