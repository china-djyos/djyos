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
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
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

#define DMA_WAIT_TIME (2*mS)
//DMAx�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMAͨ��ѡ��,��Χ:0~7
//par:�����ַ
//mar:�洢����ַ

// =============================================================================
// ���ܣ�DMAx�ĸ�ͨ������
// ������DMA_Streamx,DMA������,DMA1_Stream0~7/DMA2_Stream0~7
//      chx:DMAͨ��ѡ��,��Χ:0~7
//      par:�����ַ
//      mar:�洢����ַ
//      dir,���򣬴����赽�ڴ桢�ڴ浽�ڴ棬���赽����
//      msize,
//      psize,
//      ndtr,
// ���أ�
// =============================================================================
u32 DMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u8 chx,u32 par,bool_t MultiBuffer,u32 mar0,u32 mar1,
        u8 dir,u8 msize,u8 psize,u16 ndtr)
{
    DMA_TypeDef *DMAx;
    u8 streamx;
    u32 WaitTime = DMA_WAIT_TIME;

    if( ((u32)DMA_Streamx < (u32)DMA1) || ((u32)DMA_Streamx > (u32)DMA2_Stream7)
        || (chx > 7) || (dir > DMA_DIR_M2M) || ( msize > DMA_DATABITS_32)
        || (psize > DMA_DATABITS_32) )
        return false;

    if((u32)DMA_Streamx>(u32)DMA2)//�õ���ǰstream������DMA2����DMA1
    {
        DMAx=DMA2;
        RCC->AHB1ENR|=1<<22;//DMA2ʱ��ʹ��
    }else
    {
        DMAx=DMA1;
        RCC->AHB1ENR|=1<<21;//DMA1ʱ��ʹ��
    }

    while(DMA_Streamx->CR&0X01)                             //�ȴ�DMA������
    {
        if(WaitTime > 50)
        {
            DJY_DelayUs(50);
            WaitTime = WaitTime - 50;
        }
        else
        {
            if(DMAx == DMA2)
                RCC->AHB1ENR &= ~(1<<22);                   //DMA2ʱ��ʹ��
            else
                RCC->AHB1ENR &= ~(1<<21);
            return false;
        }
    }

    streamx=(((u32)DMA_Streamx-(u32)DMAx)-0X10)/0X18;       //�õ�streamͨ����
    if(streamx>=6)
        DMAx->HIFCR|=0X3D<<(6*(streamx-6)+16);              //���֮ǰ��stream�ϵ������жϱ�־
    else if(streamx>=4)
        DMAx->HIFCR|=0X3D<<6*(streamx-4);
    else if(streamx>=2)
        DMAx->LIFCR|=0X3D<<(6*(streamx-2)+16);
    else
        DMAx->LIFCR|=0X3D<<6*streamx;

    DMA_Streamx->PAR=par;           //DMA�����ַ
    DMA_Streamx->M0AR=mar0;         //DMA �洢��0��ַ
    DMA_Streamx->M1AR=mar1;         //DMA �洢��0��ַ
    DMA_Streamx->NDTR=ndtr;         //DMA ����������
    DMA_Streamx->CR=0;              //��ȫ����λCR�Ĵ���ֵ
    if(MultiBuffer)
    {
        DMA_Streamx->CR|=1<<18;
        DMA_Streamx->CR|=(1<<4);//��������ж�
    }
    else
        DMA_Streamx->CR|=(dir == DMA_DIR_P2M)?(1<<3):(1<<4);//�봫���ж�
    DMA_Streamx->CR &=~(1<<19);
    DMA_Streamx->CR|= (dir == DMA_DIR_P2M) ? (0<<6) : (1<<6);       //���赽�洢��ģʽ
    DMA_Streamx->CR|=0<<8;          //��ѭ��ģʽ(��ʹ����ͨģʽ)
    DMA_Streamx->CR|=0<<9;          //���������ģʽ
    DMA_Streamx->CR|=1<<10;         //�洢������ģʽ
    DMA_Streamx->CR|=psize<<11;     //�������ݳ���:8λ
    DMA_Streamx->CR|=msize<<13;     //�洢�����ݳ���:8λ
    DMA_Streamx->CR|=1<<16;         //�е����ȼ�
    DMA_Streamx->CR|=0<<21;         //����ͻ�����δ���
    DMA_Streamx->CR|=0<<23;         //�洢��ͻ�����δ���
    DMA_Streamx->CR|=(u32)chx<<25;  //ͨ��ѡ��

//  DMA_Streamx->FCR=0X21;      //FIFO���ƼĴ���//

    return true;
}
void DMA_SetIntModeComplete(DMA_Stream_TypeDef *DMA_Streamx)
{
    DMA_Streamx->CR &=~(3<<3);
    DMA_Streamx->CR |=(1<<4);
}
// =============================================================================
// ���ܣ�����һ��DMA����
// ������DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
//       ndtr:���ݴ�����
// ���أ���
// =============================================================================
void DMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u32 mar,u16 ndtr)
{
    DMA_TypeDef *DMAx;
    u8 streamx;

    if((u32)DMA_Streamx>(u32)DMA2)//�õ���ǰstream������DMA2����DMA1
    {
        DMAx=DMA2;
        RCC->AHB1ENR|=1<<22;//DMA2ʱ��ʹ��
    }else
    {
        DMAx=DMA1;
        RCC->AHB1ENR|=1<<21;//DMA1ʱ��ʹ��
    }

    streamx=(((u32)DMA_Streamx-(u32)DMAx)-0X10)/0X18;       //�õ�streamͨ����

    DMA_Streamx->CR&=~(1<<0);       //�ر�DMA����
    while(DMA_Streamx->CR&0X1);     //ȷ��DMA���Ա�����

    if(streamx>=6)
        DMAx->HIFCR|=0X3D<<(6*(streamx-6)+16);              //���֮ǰ��stream�ϵ������жϱ�־
    else if(streamx>=4)
        DMAx->HIFCR|=0X3D<<6*(streamx-4);
    else if(streamx>=2)
        DMAx->LIFCR|=0X3D<<(6*(streamx-2)+16);
    else
        DMAx->LIFCR|=0X3D<<6*streamx;

    DMA_Streamx->NDTR=ndtr;         //DMA ������
    DMA_Streamx->M0AR=mar;          //DMA �洢��0��ַ

    DMA_Streamx->CR|= (1<<0);           //����DMA����
}

// =============================================================================
// ���ܣ���ֹʹ����Ӧ��DMA��
// ������DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
// ���أ���
// =============================================================================
void DMA_Disable(DMA_Stream_TypeDef *DMA_Streamx)
{
    DMA_Streamx->CR&=~(1<<0);       //�ر�DMA����
    while(DMA_Streamx->CR&0X1);     //ȷ��DMA���Ա�����
}

// =============================================================================
// ���ܣ������Ӧ���ϵ������жϱ�־λ
// ������DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
// ���أ���
// =============================================================================
void DMA_ClearIntFlag(DMA_Stream_TypeDef *DMA_Streamx)
{
    DMA_TypeDef *DMAx;
    u8 streamx;

    if((u32)DMA_Streamx>(u32)DMA2)//�õ���ǰstream������DMA2����DMA1
    {
        DMAx=DMA2;
        RCC->AHB1ENR|=1<<22;//DMA2ʱ��ʹ��
    }else
    {
        DMAx=DMA1;
        RCC->AHB1ENR|=1<<21;//DMA1ʱ��ʹ��
    }

    streamx=(((u32)DMA_Streamx-(u32)DMAx)-0X10)/0X18;       //�õ�streamͨ����

//  DMA_Streamx->CR&=~(1<<0);       //�ر�DMA����
//  while(DMA_Streamx->CR&0X1);     //ȷ��DMA���Ա�����

    if(streamx>=6)
        DMAx->HIFCR|=0X3D<<(6*(streamx-6)+16);              //���֮ǰ��stream�ϵ������жϱ�־
    else if(streamx>=4)
        DMAx->HIFCR|=0X3D<<6*(streamx-4);
    else if(streamx>=2)
        DMAx->LIFCR|=0X3D<<(6*(streamx-2)+16);
    else
        DMAx->LIFCR|=0X3D<<6*streamx;
}

// =============================================================================
// ���ܣ�ʹ����Ӧ���ϵ��ж�
// ������DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
//      SrcInt,�ж�λ���룬��DMA_INT_HTIE��Ϊ(1<<3)
// ���أ�
// =============================================================================
void DMA_IntEnable(DMA_Stream_TypeDef *DMA_Streamx,u8 SrcInt)
{
    DMA_Streamx->CR&=~(1<<0);       //�ر�DMA����
    while(DMA_Streamx->CR&0X1);     //ȷ��DMA���Ա�����

    DMA_Streamx->CR |= SrcInt;
}

// =============================================================================
// ���ܣ��ж���Ӧ�����ж�
// ������DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
//      SrcInt,�ж�λ���룬��DMA_INT_HTIE��Ϊ(1<<3)
// ���أ���
// =============================================================================
void DMA_IntDisable(DMA_Stream_TypeDef *DMA_Streamx,u8 SrcInt)
{
    DMA_Streamx->CR&=~(1<<0);       //�ر�DMA����
    while(DMA_Streamx->CR&0X1);     //ȷ��DMA���Ա�����

    DMA_Streamx->CR &= ~SrcInt;
}


