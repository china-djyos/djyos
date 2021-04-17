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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��cpu_peri_int.c
// ģ������:
// ģ��汾: V1.00
// ������Ա:
// ����ʱ��:
// =============================================================================
#ifdef CFG_CPU_ZQ12XX_CK
#include <string.h>
#include "cpu_peri.h"
#include "int.h"

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip isr"//CPU��isr��������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:required              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"int","slian"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_ISR == false )
//#warning  " cpu_onchip_isr  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_ISR    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
#endif
//%$#@end configue  ****�������ý���
//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������
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
    /*?��2?�̡¨�?o����y��?2??��2��2?��y*/
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
