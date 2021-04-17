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
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 文件名     ：cpu_peri_int.c
// 模块描述:
// 模块版本: V1.00
// 创建人员:
// 创建时间:
// =============================================================================
#ifdef CFG_CPU_ZQ12XX_CK
#include <string.h>
#include "cpu_peri.h"
#include "int.h"

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip isr"//CPU的isr外设驱动
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:required              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"int","slian"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_ISR == false )
//#warning  " cpu_onchip_isr  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_ISR    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
#endif
//%$#@end configue  ****参数配置结束
//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束
//@#$%component end configure
// =============================================================================
#define DJYBSP_ISR_SPDIF_HDL_NUM      2
static djybsp_isr_hdl_t djybsp_spdif_hdl[DJYBSP_ISR_SPDIF_HDL_NUM];

#define DJYBSP_ISR_DMAC_HDL_NUM       8
static djybsp_isr_hdl_t djybsp_sdmac_hdl[DJYBSP_ISR_DMAC_HDL_NUM];
static djybsp_isr_hdl_t djybsp_admac_hdl[DJYBSP_ISR_DMAC_HDL_NUM];

#define DJYBSP_ISR_GPIO_HDL_NUM       32
static djybsp_isr_hdl_t djybsp_gpio1_hdl[DJYBSP_ISR_GPIO_HDL_NUM];
static djybsp_isr_hdl_t djybsp_gpio2_hdl[DJYBSP_ISR_GPIO_HDL_NUM];

#define DJYBSP_ISR_TIMER_HDL_NUM      7
static djybsp_isr_hdl_t djybsp_timer_hdl[DJYBSP_ISR_TIMER_HDL_NUM];

#define DJYBSP_ISR_MISC_HDL_NUM       10
static djybsp_isr_hdl_t djybsp_misc_hdl[DJYBSP_ISR_MISC_HDL_NUM];

#define DJYBSP_ISR_PMU_HDL_NUM        11
static djybsp_isr_hdl_t djybsp_pmu_hdl[DJYBSP_ISR_PMU_HDL_NUM];

struct djybsp_isr_t {
    djybsp_isr_hdl_t isr;
    uint32_t param;
}static djybsp_isr[CN_INT_LINE_LAST+1];

static void djybsp_isr_do_hdl(uint32_t stat, djybsp_isr_hdl_t *hdl,uint32_t param)
{
    int subid = 0;

    while (stat) {
        if (stat & 0x1) {
            if(hdl[subid])
            {
                if(param==0)
                    (*hdl[subid])(subid);
                else
                    (*hdl[subid])(param);
            }
        }
        stat >>= 1;
        subid++;
    }
}

static uint32_t djybsp_isr_hdl_spdif(uint32_t param)
{
    uint32_t stat;

    stat = INTR_STS_MSK(PIC_IRQID_SPDIF);
    djybsp_isr_do_hdl(stat, djybsp_spdif_hdl, param);
    return stat;
}

static uint32_t djybsp_isr_hdl_sdmac(uint32_t param)
{
    uint32_t stat;

    stat = INTR_STS_MSK(PIC_IRQID_SDMAC);
    djybsp_isr_do_hdl(stat, djybsp_sdmac_hdl, param);
    return stat;
}

static uint32_t djybsp_isr_hdl_admac(uint32_t param)
{
    uint32_t stat;

    stat = INTR_STS_MSK(PIC_IRQID_ADMAC);
    djybsp_isr_do_hdl(stat, djybsp_admac_hdl, param);
    return stat;
}

static uint32_t djybsp_isr_hdl_gpio1(uint32_t param)
{
    uint32_t stat;

    stat = INTR_STS_MSK(PIC_IRQID_GPIO1);
    djybsp_isr_do_hdl(stat, djybsp_gpio1_hdl, param);
    return stat;
}

static uint32_t djybsp_isr_hdl_gpio2(uint32_t param)
{
    uint32_t stat;

    stat = INTR_STS_MSK(PIC_IRQID_GPIO2);
    djybsp_isr_do_hdl(stat, djybsp_gpio2_hdl, param);
    return stat;
}

static uint32_t djybsp_isr_hdl_misc(uint32_t param)
{
    uint32_t stat;

    stat = INTR_STS_MSK(PIC_IRQID_MISC);
    djybsp_isr_do_hdl(stat, djybsp_misc_hdl, param);
    return stat;
}

static uint32_t djybsp_isr_hdl_pmu(uint32_t param)
{
    uint32_t stat;

    stat = INTR_STS_MSK(PIC_IRQID_PMU);
    djybsp_isr_do_hdl(stat, djybsp_pmu_hdl, param);
    return stat;
}

static uint32_t djybsp_isr_hdl_timer(uint32_t param)
{
    uint32_t stat;

    stat = INTR_STS_MSK(PIC_IRQID_TIMER);
    djybsp_isr_do_hdl(stat, djybsp_timer_hdl, param);
    return stat;
}

static void djybsp_isr_register(uint32_t id, djybsp_isr_hdl_t isr,uint32_t param)
{
    /*?ú2?μ÷ó?oˉêy￡?2??ì2é2?êy*/
    switch(id)
    {
        case CN_INT_LINE_SPDIF:
            djybsp_isr[id].isr = djybsp_isr_hdl_spdif;
            break;
        case CN_INT_LINE_SDMAC:
            djybsp_isr[id].isr = djybsp_isr_hdl_sdmac;
            break;
        case CN_INT_LINE_ADMAC:
            djybsp_isr[id].isr = djybsp_isr_hdl_admac;
            break;
        case CN_INT_LINE_GPIO1:
            djybsp_isr[id].isr = djybsp_isr_hdl_gpio1;
            break;
        case CN_INT_LINE_GPIO2:
            djybsp_isr[id].isr = djybsp_isr_hdl_gpio2;
            break;
        case CN_INT_LINE_TIMER:
            djybsp_isr[id].isr = djybsp_isr_hdl_timer;
            break;
        case CN_INT_LINE_MISC:
            djybsp_isr[id].isr = djybsp_isr_hdl_misc;
            break;
        case CN_INT_LINE_PMU:
            djybsp_isr[id].isr = djybsp_isr_hdl_pmu;
            break;
        default:
            djybsp_isr[id].isr = isr;
            break;
    }
    djybsp_isr[id].param = param;

    Int_Register(id);
    Int_SetClearType(id,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(id,djybsp_isr_hdl_dispatch);
    Int_SetIsrPara(id,id);
    Int_SettoAsynSignal(id);
    Int_ClearLine(id);
    Int_RestoreAsynLine(id);
}

int djybsp_isr_hdl_register(uint32_t id, int subid, djybsp_isr_hdl_t hdl,uint32_t param)
{
    if ((id > CN_INT_LINE_LAST) || (!hdl)) {
        return -1;
    }

    INTR_CTR_MSK(id) &= ~(1 << (subid));/* 1 */

    if ((id == CN_INT_LINE_SPDIF) && (subid != CN_SUBID_SOLO)) {
        if (subid >= DJYBSP_ISR_SPDIF_HDL_NUM) {
            return -1;
        }
        if (!djybsp_spdif_hdl[subid]) {
            djybsp_spdif_hdl[subid] = hdl;
            INTR_CTR_MSK(id) |= (1 << (subid)); /* 2 */
        }
        else if (djybsp_spdif_hdl[subid] != hdl) {
            return -1;
        }
    }
    else if ((id == CN_INT_LINE_SDMAC) && (subid != CN_SUBID_SOLO)) {
        if (subid >= DJYBSP_ISR_DMAC_HDL_NUM) {
            return -1;
        }

        if (!djybsp_sdmac_hdl[subid]) {
            djybsp_sdmac_hdl[subid] = hdl;
            INTR_CTR_MSK(id) |= (1 << (subid));
        }
        else if (djybsp_sdmac_hdl[subid] != hdl) {
            return -1;
        }
    }
    else if ((id == CN_INT_LINE_ADMAC) && (subid != CN_SUBID_SOLO)) {
        if (subid >= DJYBSP_ISR_DMAC_HDL_NUM) {
            return -1;
        }

        if (!djybsp_admac_hdl[subid]) {
            djybsp_admac_hdl[subid] = hdl;
            INTR_CTR_MSK(id) |= (1 << (subid));
        }
        else if (djybsp_admac_hdl[subid] != hdl) {
            return -1;
        }
    }
    else if ((id == CN_INT_LINE_GPIO1) && (subid != CN_SUBID_SOLO)) {
        if (subid >= DJYBSP_ISR_GPIO_HDL_NUM) {
            return -1;
        }

        if (!djybsp_gpio1_hdl[subid]) {
            djybsp_gpio1_hdl[subid] = hdl;
            INTR_CTR_MSK(id) |= (1 << (subid));
        }
        else if (djybsp_gpio1_hdl[subid] != hdl) {
            return -1;
        }
    }
    else if ((id == CN_INT_LINE_GPIO2) && (subid != CN_SUBID_SOLO)) {
        if (subid >= DJYBSP_ISR_GPIO_HDL_NUM) {
            return -1;
        }

        if (!djybsp_gpio2_hdl[subid]) {
            djybsp_gpio2_hdl[subid] = hdl;
            INTR_CTR_MSK(id) |= (1 << (subid));
        }
        else if (djybsp_gpio2_hdl[subid] != hdl) {
            return -1;
        }
    }
    else if ((id == CN_INT_LINE_TIMER) && (subid != CN_SUBID_SOLO)) {
        if (subid >= DJYBSP_ISR_TIMER_HDL_NUM) {
            return -1;
        }

        if (!djybsp_timer_hdl[subid]) {
            djybsp_timer_hdl[subid] = hdl;
            INTR_CTR_MSK(id) |= (1 << (subid));
        } else if (djybsp_timer_hdl[subid] != hdl) {
            return -1;
        }
    }
    else if ((id == CN_INT_LINE_MISC) && (subid != CN_SUBID_SOLO)) {
        if (subid >= DJYBSP_ISR_MISC_HDL_NUM) {
            return -1;
        }

        if (!djybsp_misc_hdl[subid]) {
            djybsp_misc_hdl[subid]   = hdl;
            INTR_CTR_MSK(id) |= (1 << (subid));
        }
        else if (djybsp_misc_hdl[subid] != hdl) {
            return -1;
        }
    }
    else if ((id == CN_INT_LINE_PMU) && (subid != CN_SUBID_SOLO)) {
        if (subid >= DJYBSP_ISR_PMU_HDL_NUM) {
            return -1;
        }

        if (!djybsp_pmu_hdl[subid]) {
            djybsp_pmu_hdl[subid]   = hdl;
            INTR_CTR_MSK(id) |= (1 << (subid));
        } else if (djybsp_pmu_hdl[subid] != hdl) {
            return -1;
        }
    }
    if(djybsp_isr[id].isr==NULL)
        djybsp_isr_register(id,hdl,param);
    return 0;
}

void djybsp_isr_hdl_unregister(uint32_t id, int subid)
{
    return;
}

void djybsp_isr_init(void)
{
    memset(djybsp_spdif_hdl, 0, DJYBSP_ISR_SPDIF_HDL_NUM * sizeof(djybsp_isr_hdl_t));
    memset(djybsp_sdmac_hdl, 0, DJYBSP_ISR_DMAC_HDL_NUM  * sizeof(djybsp_isr_hdl_t));
    memset(djybsp_admac_hdl, 0, DJYBSP_ISR_DMAC_HDL_NUM  * sizeof(djybsp_isr_hdl_t));
    memset(djybsp_gpio1_hdl, 0, DJYBSP_ISR_GPIO_HDL_NUM  * sizeof(djybsp_isr_hdl_t));
    memset(djybsp_gpio2_hdl, 0, DJYBSP_ISR_GPIO_HDL_NUM  * sizeof(djybsp_isr_hdl_t));
    memset(djybsp_timer_hdl, 0, DJYBSP_ISR_TIMER_HDL_NUM * sizeof(djybsp_isr_hdl_t));
    memset(djybsp_misc_hdl,  0, DJYBSP_ISR_MISC_HDL_NUM * sizeof(djybsp_isr_hdl_t));
    memset(djybsp_pmu_hdl,   0, DJYBSP_ISR_PMU_HDL_NUM * sizeof(djybsp_isr_hdl_t));

    memset(djybsp_isr,0,sizeof(djybsp_isr));
}

uint32_t djybsp_isr_hdl_dispatch(uint32_t line)
{
    if(djybsp_isr[line].isr!=NULL)
        djybsp_isr[line].isr(djybsp_isr[line].param);
    return 0;
}
#endif
