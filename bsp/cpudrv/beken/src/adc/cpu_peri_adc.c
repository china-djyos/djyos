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
#include "icu_pub.h"
#include "intc_pub.h"
#include "power_save_pub.h"
#include "saradc/saradc.h"
#include <lock.h>
#include <string.h>
#include <stdio.h>
#include <systime.h>
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//extern void ModuleInstall_ADC(void);
//  ModuleInstall_ADC( );
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip adc"       //adc组件
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
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

#define REG_READ(addr)          (*((volatile UINT32 *)(addr)))
#define REG_WRITE(addr, _data)  (*((volatile UINT32 *)(addr)) = (_data))

DD_HANDLE djy_adc_open(uint8_t channel, saradc_desc_t *adcDesc, uint8_t cntOfDataBuff)
{
    u32 status;
    DD_HANDLE retHdl = DD_HANDLE_UNVALID;

    if (channel > 7) // TODO merlin 到底有多少个ADC？ 有12个？不确定，所以现在写死了先  SARADC_ADC_CHNL_MAX)
    {
        return 0;
    }

    if (0 == adcDesc)
    {
        return 0;
    }

    adcDesc->channel = channel;
    adcDesc->data_buff_size = cntOfDataBuff;
    adcDesc->mode = (ADC_CONFIG_MODE_CONTINUE << 0) | (ADC_CONFIG_MODE_8CLK_DELAY << 2);

    adcDesc->has_data                = 0;
    adcDesc->current_read_data_cnt   = 0;
    adcDesc->current_sample_data_cnt = 0;
    adcDesc->pre_div = 0x10;
    adcDesc->samp_rate = 0x20;
//    memset(adcDesc->pData, 0, cntOfDataBuff*sizeof(*adcDesc->pData));

    retHdl = ddev_open(SARADC_DEV_NAME, &status, (UINT32)adcDesc);

    return retHdl;
}

void djy_adc_fill_buffer(DD_HANDLE handle, saradc_desc_t *adcDescOpened, uint32_t delayUs)
{
//    s64 startTimeUs = 0;
//    s64 tmpDelayUs = delayUs;
//    uint32_t hasDelayMs = 0;
    uint32_t cmd;
    uint8_t run_stop;

    if (0 == adcDescOpened)
    {
        return ;
    }

    adcDescOpened->current_read_data_cnt   = 0;
    adcDescOpened->current_sample_data_cnt = 0;

    cmd = SARADC_CMD_RUN_OR_STOP_ADC;
    run_stop = 1;
    ddev_control(handle, cmd, &run_stop);

//    startTimeUs = DJY_GetSysTime();
//    while ((DJY_GetSysTime()-startTimeUs) < tmpDelayUs)
//    {
//        if (adcDescOpened->current_sample_data_cnt == adcDescOpened->data_buff_size)
//        {
//            ddev_close(handle);
//            break;
//        }
//    }
}

void djy_adc_close(DD_HANDLE handle)
{
    ddev_close(handle);
}




// 这个ADC系统不能重入，同一时刻只能只有一个ADC通道在运行
//#define ADC_TEMP_BUFFER_SIZE 2
//static saradc_desc_t tmp_single_desc;
//static UINT16 tmp_single_buff[ADC_TEMP_BUFFER_SIZE];//ADC_TEMP_BUFFER_SIZE];
//static volatile DD_HANDLE tmp_single_hdl = DD_HANDLE_UNVALID;

struct MutexLCB *pg_tadc_mutex = NULL;

int djy_adc_read(uint16_t channel) // 注意！！！ 不能再中断中使用！！！
{
#define ADC_TEMP_BUFFER_SIZE 2
    saradc_desc_t tmp_single_desc;
    UINT16 tmp_single_buff[ADC_TEMP_BUFFER_SIZE];//ADC_TEMP_BUFFER_SIZE];
    volatile DD_HANDLE tmp_single_hdl = DD_HANDLE_UNVALID;
    int tryTimes = 1000;
    UINT32 status;
    UINT32 cmd;
    UINT8 run_stop;
    u16 ad_date;
    int tmpData = 0xFFFFFFFF;
    float voltage = 0.0;

    if (channel > 7) // TODO merlin 到底有多少个ADC？ 有12个？不确定，所以现在写死了先  SARADC_ADC_CHNL_MAX)
    {
        return tmpData;
    }

    memset(&tmp_single_desc, 0x00, sizeof(saradc_desc_t));
    tmp_single_desc.channel = channel;
    tmp_single_desc.data_buff_size = ADC_TEMP_BUFFER_SIZE;
    tmp_single_desc.mode = (ADC_CONFIG_MODE_CONTINUE << 0) | (ADC_CONFIG_MODE_8CLK_DELAY << 2);
    tmp_single_desc.has_data                = 0;
    tmp_single_desc.current_read_data_cnt   = 0;
    tmp_single_desc.current_sample_data_cnt = 0;
    tmp_single_desc.pre_div = 0x10;
    tmp_single_desc.samp_rate = 0x20;
    memset(tmp_single_buff, 0, sizeof(tmp_single_buff));
    tmp_single_desc.pData = &tmp_single_buff[0];

    Lock_MutexPend(pg_tadc_mutex, 0xffffffff);

    tmp_single_hdl = ddev_open(SARADC_DEV_NAME, &status, (UINT32)&tmp_single_desc);

    cmd = SARADC_CMD_RUN_OR_STOP_ADC;
    run_stop = 1;
    ddev_control(tmp_single_hdl, cmd, &run_stop);

    while (tryTimes--)
    {
        DJY_EventDelay(10);
        if (tmp_single_desc.current_sample_data_cnt == tmp_single_desc.data_buff_size)
        {
            ddev_close(tmp_single_hdl);

            ad_date = 0;
            for (int i=0; i<tmp_single_desc.current_sample_data_cnt; i++)
            {
                ad_date += tmp_single_desc.pData[i];
            }
            ad_date /= tmp_single_desc.current_sample_data_cnt;
            voltage = saradc_calculate(ad_date);
            tmpData = voltage * 1000;
            break;
        }
    }
    if (tryTimes <= 0) {
        ddev_close(tmp_single_hdl);
    }
    Lock_MutexPost(pg_tadc_mutex);

    return tmpData;
}
#if 1

//快速采样
//
s32 djy_adc_fast_read(u8 channel)
{
    UINT32 value = 0, reg;
    u8 mode = (ADC_CONFIG_MODE_CONTINUE << 0) | (ADC_CONFIG_MODE_4CLK_DELAY << 2);
    u8 pre_div = 0x10,samp_rate = 0x20,filter = 0;
    u16 ad_date;
    int voltage = 0xFFFFFFFF;

    if(channel > SARADC_ADC_CHNL_MAX){
        return SARADC_FAILURE;
    }

    reg = REG_READ(ICU_PERI_CLK_PWD);
    reg &= ~PWD_SARADC_CLK_BIT;
    REG_WRITE(ICU_PERI_CLK_PWD, reg);


    // clear fifo
    while((value & SARADC_ADC_FIFO_EMPTY) == 0) {
        REG_READ(SARADC_ADC_DATA);
        value = REG_READ(SARADC_ADC_CONFIG);
    }

    value = ((mode & SARADC_ADC_MODE_MASK) << SARADC_ADC_MODE_POSI)
        | SARADC_ADC_CHNL_EN
        | ((channel & SARADC_ADC_CHNL_MASK) << SARADC_ADC_CHNL_POSI)
        | (((mode>>2)&0x01) << SARADC_ADC_DELAY_CLK_POSI)
        | SARADC_ADC_INT_CLR
        | ((pre_div & SARADC_ADC_PRE_DIV_MASK) << SARADC_ADC_PRE_DIV_POSI)
        | ((samp_rate & SARADC_ADC_SAMPLE_RATE_MASK) << SARADC_ADC_SAMPLE_RATE_POSI)
        | ((filter & SARADC_ADC_FILTER_MASK)<< SARADC_ADC_FILTER_POSI);
    REG_WRITE(SARADC_ADC_CONFIG, value);

#if (CFG_SOC_NAME != SOC_BK7231)
    value = SARADC_ADC_SAT_ENABLE
       //| ((0x01 & SARADC_ADC_SAT_CTRL_MASK) << SARADC_ADC_SAT_CTRL_POSI);
       | ((0x03 & SARADC_ADC_SAT_CTRL_MASK) << SARADC_ADC_SAT_CTRL_POSI);
    REG_WRITE(SARADC_ADC_SATURATION_CFG, value);
#endif


    Lock_MutexPend(pg_tadc_mutex, 0xffffffff);

    value = REG_READ(SARADC_ADC_CONFIG);
    value &= ~(SARADC_ADC_CHNL_MASK << SARADC_ADC_CHNL_POSI);
    value |= (channel << SARADC_ADC_CHNL_POSI);
    value |= SARADC_ADC_CHNL_EN;
    REG_WRITE(SARADC_ADC_CONFIG, value);

    value = REG_READ(SARADC_ADC_CONFIG);
    while((value & SARADC_ADC_FIFO_EMPTY) != 0)
    {
        value = REG_READ(SARADC_ADC_CONFIG);
    }

    #if (CFG_SOC_NAME == SOC_BK7231)
    ad_date = REG_READ(SARADC_ADC_DATA)&0x03FF;
    #else
    ad_date = REG_READ(SARADC_ADC_DAT_AFTER_STA)&0xFFFF;
    #endif // (CFG_SOC_NAME == SOC_BK7231)
    voltage = saradc_calculate_fast(ad_date);
//    tmpData = voltage * 1000;

    value = REG_READ(SARADC_ADC_CONFIG);
    value &= ~(SARADC_ADC_MODE_MASK << SARADC_ADC_MODE_POSI);
    value &= ~(SARADC_ADC_CHNL_EN);
    REG_WRITE(SARADC_ADC_CONFIG, value);

    reg = REG_READ(ICU_PERI_CLK_PWD);
    reg |= PWD_SARADC_CLK_BIT;
    REG_WRITE(ICU_PERI_CLK_PWD, reg);

    Lock_MutexPost(pg_tadc_mutex);

    return voltage;
}
#endif
#if 0
static void temp_single_detect_handler2(void)
{
    if(tmp_single_desc.current_sample_data_cnt >= tmp_single_desc.data_buff_size)
    {
        #if (CFG_SOC_NAME != SOC_BK7231)
        UINT32 sum = 0, sum1, sum2=0;
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
#endif




int vbat_voltage_get(void)
{
    int i, j = 0, Vbat = 0, AverageVbat = 0;
    for(i = 0; i < 10; i++)
    {
        Vbat = djy_adc_read(0);
        Vbat = Vbat * 2;
        if((Vbat > 2700) && (Vbat < 4400))  //去掉一些错误电压
        {
            AverageVbat += Vbat;
            j++;
        }
    }
    AverageVbat = AverageVbat / j;
    return AverageVbat;
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
    UINT32 status;
    saradc_init();
    Int_SaveAsynLine(IRQ_SARADC);
    pg_tadc_mutex = Lock_MutexCreate("pg_tadc_mutex");
//  memset(&tmp_single_desc, 0x00, sizeof(saradc_desc_t));
//  tmp_single_desc.channel = 1;
//  tmp_single_desc.data_buff_size = ADC_TEMP_BUFFER_SIZE;
//  tmp_single_desc.mode = (ADC_CONFIG_MODE_CONTINUE << 0) | (ADC_CONFIG_MODE_4CLK_DELAY << 2);
//  tmp_single_desc.has_data                = 0;
//  tmp_single_desc.current_read_data_cnt   = 0;
//  tmp_single_desc.current_sample_data_cnt = 0;
//  tmp_single_desc.pre_div = 0x10;
//  tmp_single_desc.samp_rate = 0x20;
//  tmp_single_desc.pData = &tmp_single_buff[0];
//  tmp_single_hdl = ddev_open(SARADC_DEV_NAME, &status, (UINT32)&tmp_single_desc);

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

