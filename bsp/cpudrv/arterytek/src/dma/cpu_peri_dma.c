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

// �ļ���     ��cpu_peri_dma.c
// ģ������: DMA����
// ģ��汾: V1.10
// ������Ա: hm
// ����ʱ��: 14/07.2015
// =============================================================================

#include "stddef.h"
#include "cpu_peri.h"
#include "systime.h"
#include "djyos.h"
#include "cpu_peri_dma.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip dma"//CPU��dma����
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:none                //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"             //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_DMA == false )
//#warning  " cpu_onchip_dma  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_DMA    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

#define DMA_WAIT_TIME (2*mS)
//DMAx�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_Channel:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMAͨ��ѡ��,��Χ:0~7
//par:�����ַ
//mar:�洢����ַ

bool_t DMA_ClearIntFlag(dma_channel_type *DMA_Channel);

// =============================================================================
// ���ܣ����ͨ���Ƿ�Ϸ�
// ������DMA_Channel:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
// ���أ�true -- �Ϸ���false -- ���Ϸ�
// =============================================================================
static bool_t DMA_CheckChannel(dma_channel_type *DMA_Channel)
{
    if ((DMA_Channel == DMA1_CHANNEL1) || (DMA_Channel == DMA1_CHANNEL2) || (DMA_Channel == DMA1_CHANNEL3) 
        || (DMA_Channel == DMA1_CHANNEL4) || (DMA_Channel == DMA1_CHANNEL5) || (DMA_Channel == DMA1_CHANNEL6)
        || (DMA_Channel == DMA1_CHANNEL7) || (DMA_Channel == DMA2_CHANNEL1) || (DMA_Channel == DMA2_CHANNEL2) 
        || (DMA_Channel == DMA2_CHANNEL3) || (DMA_Channel == DMA2_CHANNEL4) || (DMA_Channel == DMA2_CHANNEL5) 
        || (DMA_Channel == DMA2_CHANNEL6) || (DMA_Channel == DMA2_CHANNEL7)) 
    {
        return true;
    }
    return false;
}

// =============================================================================
// ���ܣ�����ͨ����ȡͨ����MUX�Ĵ�����ַ
// ������DMA_Channel:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
// ���أ���Ӧͨ����MUX��ַ
// =============================================================================
static dmamux_channel_type *DMA_GetChannelMUX(dma_channel_type *DMA_Channel)
{
    dmamux_channel_type *MUX = NULL;

    switch ((u32)DMA_Channel)
    {
        case DMA1_CHANNEL1_BASE:     MUX = DMA1MUX_CHANNEL1;break;
        case DMA1_CHANNEL2_BASE:     MUX = DMA1MUX_CHANNEL2;break;
        case DMA1_CHANNEL3_BASE:     MUX = DMA1MUX_CHANNEL3;break;
        case DMA1_CHANNEL4_BASE:     MUX = DMA1MUX_CHANNEL4;break;
        case DMA1_CHANNEL5_BASE:     MUX = DMA1MUX_CHANNEL5;break;
        case DMA1_CHANNEL6_BASE:     MUX = DMA1MUX_CHANNEL6;break;
        case DMA1_CHANNEL7_BASE:     MUX = DMA1MUX_CHANNEL7;break;
        case DMA2_CHANNEL1_BASE:     MUX = DMA2MUX_CHANNEL1;break;
        case DMA2_CHANNEL2_BASE:     MUX = DMA2MUX_CHANNEL2;break;
        case DMA2_CHANNEL3_BASE:     MUX = DMA2MUX_CHANNEL3;break;
        case DMA2_CHANNEL4_BASE:     MUX = DMA2MUX_CHANNEL4;break;
        case DMA2_CHANNEL5_BASE:     MUX = DMA2MUX_CHANNEL5;break;
        case DMA2_CHANNEL6_BASE:     MUX = DMA2MUX_CHANNEL6;break;
        case DMA2_CHANNEL7_BASE:     MUX = DMA2MUX_CHANNEL7;break;

    default:
        break;
    }

    return MUX;
}

// =============================================================================
// ���ܣ�DMAx�ĸ�ͨ������
// ������DMA_Channel,DMA������,DMA1_Stream0~7/DMA2_Stream0~7
//      req: ��;
//      par:�����ַ
//      mar:�洢����ַ
//      dir,���򣬴����赽�ڴ桢�ڴ浽�ڴ棬���赽����
//      msize,
//      psize,
//      ndtr,
// ���أ�true -- �ɹ���false -- ʧ��
// =============================================================================
bool_t DMA_Config(dma_channel_type *DMA_Channel, dmamux_requst_id_sel_type req, u32 par, u32 mar,
        u16 dir, u8 msize, u8 psize, u16 ndtr)
{
    dma_init_type dma_init_struct;
    dma_type *DMAx = NULL;
    // DMAͨ���Ϸ�

    if (DMA_CheckChannel(DMA_Channel) && ((dir == DMA_DIR_M2M) || (dir == DMA_DIR_M2P) || (dir == DMA_DIR_P2M))
        && ((msize == DMA_MEM_DATABITS_8) || (msize == DMA_MEM_DATABITS_16) || (msize == DMA_MEM_DATABITS_32))
        && ((psize == DMA_PER_DATABITS_8) || (psize == DMA_PER_DATABITS_16) || (psize == DMA_PER_DATABITS_32))
        )
    {
        // ����Ҫ��ʱ�ӣ���Ȼ�п���DMA�ļĴ��������ò��ɹ�
        if (((u32)DMA_Channel >= (u32)DMA2_CHANNEL1) && ((u32)DMA_Channel <= (u32)DMA2_CHANNEL7))//�õ���ǰstream������DMA2����DMA1
        {
            // RCC->AHB1ENR|=1<<22;//DMA2ʱ��ʹ��
            DMAx=DMA2;
            crm_periph_clock_enable(CRM_DMA2_PERIPH_CLOCK, TRUE);   //DMA2ʱ��ʹ��
        }else
        {
            // RCC->AHB1ENR|=1<<21;//DMA1ʱ��ʹ��
            DMAx=DMA1;
            crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);   //DMA1ʱ��ʹ��
        }
        // ����DMA�Ĵ���֮ǰҪ��dma������
        dma_channel_enable(DMA_Channel, FALSE);

//        DMA_ClearIntFlag(DMA_Channel);
        dma_reset(DMA_Channel);
        dma_default_para_init(&dma_init_struct);
        dma_init_struct.buffer_size = ndtr;
        dma_init_struct.direction = dir;
        dma_init_struct.memory_base_addr = (uint32_t)mar;
        dma_init_struct.memory_data_width = msize;
        dma_init_struct.memory_inc_enable = TRUE;
        dma_init_struct.peripheral_base_addr = par;
        dma_init_struct.peripheral_data_width = psize;
        dma_init_struct.peripheral_inc_enable = FALSE;
        dma_init_struct.priority = DMA_PRIORITY_MEDIUM;
        dma_init_struct.loop_mode_enable = FALSE;
        dma_init(DMA_Channel, &dma_init_struct);

        dmamux_enable(DMAx, TRUE);
        dmamux_init(DMA_GetChannelMUX(DMA_Channel), req);
    }
    else
    {
        return false;
    }


    return true;
}

// =============================================================================
// ���ܣ�����һ��DMA����
// ������DMA_Channel:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
//       ndtr:���ݴ�����
// ���أ�true -- �ɹ���false -- ʧ��
// =============================================================================
bool_t DMA_Enable(dma_channel_type *DMA_Channel, u32 mar, u16 ndtr)
{
//    dma_type *DMAx;
//    u8 streamx;

    if (false == DMA_CheckChannel(DMA_Channel))
    {
        return false;
    }

    if (((u32)DMA_Channel >= (u32)DMA2_CHANNEL1) && ((u32)DMA_Channel <= (u32)DMA2_CHANNEL7))//�õ���ǰstream������DMA2����DMA1
    {
//        DMAx=DMA2;
        // RCC->AHB1ENR|=1<<22;//DMA2ʱ��ʹ��
        crm_periph_clock_enable(CRM_DMA2_PERIPH_CLOCK, TRUE);   //DMA2ʱ��ʹ��
    }else
    {
//        DMAx=DMA1;
        // RCC->AHB1ENR|=1<<21;//DMA1ʱ��ʹ��
        crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);   //DMA1ʱ��ʹ��
    }
    // ����DMA�Ĵ���֮ǰҪ��dma������
    dma_channel_enable(DMA_Channel, FALSE);

    // dma_reset(DMA_Channel);
    DMA_ClearIntFlag(DMA_Channel);

    DMA_Channel->maddr = mar;
    DMA_Channel->dtcnt_bit.cnt = ndtr;

    dma_channel_enable(DMA_Channel, TRUE); //��DMA����

    // if((u32)DMA_Channel>(u32)DMA2)//�õ���ǰstream������DMA2����DMA1
    // {
    //     DMAx=DMA2;
    //     RCC->AHB1ENR|=1<<22;//DMA2ʱ��ʹ��
    // }else
    // {
    //     DMAx=DMA1;
    //     RCC->AHB1ENR|=1<<21;//DMA1ʱ��ʹ��
    // }

    // streamx=(((u32)DMA_Channel-(u32)DMAx)-0X10)/0X18;       //�õ�streamͨ����

    // DMA_Channel->CR&=~(1<<0);       //�ر�DMA����
    // while(DMA_Channel->CR&0X1);     //ȷ��DMA���Ա�����

    // if(streamx>=6)
    //     DMAx->HIFCR|=0X3D<<(6*(streamx-6)+16);              //���֮ǰ��stream�ϵ������жϱ�־
    // else if(streamx>=4)
    //     DMAx->HIFCR|=0X3D<<6*(streamx-4);
    // else if(streamx>=2)
    //     DMAx->LIFCR|=0X3D<<(6*(streamx-2)+16);
    // else
    //     DMAx->LIFCR|=0X3D<<6*streamx;

    // DMA_Channel->NDTR=ndtr;         //DMA ������
    // DMA_Channel->M0AR=mar;          //DMA �洢��0��ַ

    // DMA_Channel->CR|= (1<<0);           //����DMA����

    return true;
}

// =============================================================================
// ���ܣ���ֹʹ����Ӧ��DMA��
// ������DMA_Channel:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
// ���أ�true -- �ɹ���false -- ʧ��
// =============================================================================
bool_t DMA_Disable(dma_channel_type *DMA_Channel)
{
    if (false == DMA_CheckChannel(DMA_Channel))
    {
        return false;
    }
    // DMA_Channel->CR&=~(1<<0);       //�ر�DMA����
    // while(DMA_Channel->CR&0X1);     //ȷ��DMA���Ա�����
    // ����DMA�Ĵ���֮ǰҪ��dma������
    dma_channel_enable(DMA_Channel, FALSE);

    return true;
}

// =============================================================================
// ���ܣ������Ӧ���ϵ������жϱ�־λ
// ������DMA_Channel:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
// ���أ�true -- �ɹ���false -- ʧ��
// =============================================================================
bool_t DMA_ClearIntFlag(dma_channel_type *DMA_Channel)
{
//    dma_type *DMAx;
//    u8 streamx;
    uint32_t temp = 0;

    if (false == DMA_CheckChannel(DMA_Channel))
    {
        return false;
    }

    if (((u32)DMA_Channel >= (u32)DMA2_CHANNEL1) && ((u32)DMA_Channel <= (u32)DMA2_CHANNEL7))//�õ���ǰstream������DMA2����DMA1
    {
//        DMAx=DMA2;
        // RCC->AHB1ENR|=1<<22;//DMA2ʱ��ʹ��
        crm_periph_clock_enable(CRM_DMA2_PERIPH_CLOCK, TRUE);   //DMA2ʱ��ʹ��
    }else
    {
//        DMAx=DMA1;
        // RCC->AHB1ENR|=1<<21;//DMA1ʱ��ʹ��
        crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);   //DMA1ʱ��ʹ��
    }
    // ����DMA�Ĵ���֮ǰҪ��dma������
    dma_channel_enable(DMA_Channel, FALSE);

    temp = (uint32_t)DMA_Channel;

    if((temp & 0x6FF) < 0x608)
    {
        /* dma1 channel */
        DMA1->clr |= (uint32_t)(0x0F << ((((temp & 0xFF) - 0x08) / 0x14) * 4));
    }
    else if((temp & 0x6FF) < 0x688)
    {
        /* dma2 channel */
        DMA2->clr |= (uint32_t)(0x0F << ((((temp & 0xFF) - 0x08) / 0x14) * 4));
    }

    // if((u32)DMA_Channel>(u32)DMA2)//�õ���ǰstream������DMA2����DMA1
    // {
    //     DMAx=DMA2;
    //     RCC->AHB1ENR|=1<<22;//DMA2ʱ��ʹ��
    // }else
    // {
    //     DMAx=DMA1;
    //     RCC->AHB1ENR|=1<<21;//DMA1ʱ��ʹ��
    // }

    // streamx=(((u32)DMA_Channel-(u32)DMAx)-0X10)/0X18;       //�õ�streamͨ����

    // DMA_Channel->CR&=~(1<<0);       //�ر�DMA����
    // while(DMA_Channel->CR&0X1);     //ȷ��DMA���Ա�����

    // if(streamx>=6)
    //     DMAx->HIFCR|=0X3D<<(6*(streamx-6)+16);              //���֮ǰ��stream�ϵ������жϱ�־
    // else if(streamx>=4)
    //     DMAx->HIFCR|=0X3D<<6*(streamx-4);
    // else if(streamx>=2)
    //     DMAx->LIFCR|=0X3D<<(6*(streamx-2)+16);
    // else
    //     DMAx->LIFCR|=0X3D<<6*streamx;

    return true;
}

// =============================================================================
// ���ܣ�ʹ����Ӧ���ϵ��ж�
// ������DMA_Channel:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
//      SrcInt,�ж�λ���룬��DMA_INT_HTIE
// ���أ�true -- �ɹ���false -- ʧ��
// =============================================================================
bool_t DMA_IntEnable(dma_channel_type *DMA_Channel, u8 SrcInt)
{
    // DMA_Channel->CR&=~(1<<0);       //�ر�DMA����
    // while(DMA_Channel->CR&0X1);     //ȷ��DMA���Ա�����

    // DMA_Channel->CR |= SrcInt;

    if (false == DMA_CheckChannel(DMA_Channel))
    {
        return false;
    }
    // ����DMA�Ĵ���֮ǰҪ��dma������
    dma_channel_enable(DMA_Channel, FALSE); //�ر�DMA����

    dma_interrupt_enable(DMA_Channel, SrcInt, TRUE);    //���ж�
    return true;
}

// =============================================================================
// ���ܣ��ж���Ӧ�����ж�
// ������DMA_Channel:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
//      SrcInt,�ж�λ���룬��DMA_INT_HTIE��Ϊ(1<<3)
// ���أ�true -- �ɹ���false -- ʧ��
// =============================================================================
bool_t DMA_IntDisable(dma_channel_type *DMA_Channel,u8 SrcInt)
{
    // DMA_Channel->CR&=~(1<<0);       //�ر�DMA����
    // while(DMA_Channel->CR&0X1);     //ȷ��DMA���Ա�����

    // DMA_Channel->CR &= ~SrcInt;

    if (false == DMA_CheckChannel(DMA_Channel))
    {
        return false;
    }
    // ����DMA�Ĵ���֮ǰҪ��dma������
    dma_channel_enable(DMA_Channel, FALSE); //�ر�DMA����

    dma_interrupt_enable(DMA_Channel, SrcInt, FALSE);    //���ж�

    return true;
}


