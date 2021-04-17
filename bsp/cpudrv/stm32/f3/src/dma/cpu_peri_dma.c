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
#include "stm32f303xc.h"

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
#define DMA_WAIT_TIME (2*mS)

// =============================================================================
// ���ܣ�DMAx�ĸ�ͨ������
// ������DMA_Channelx��DMA1_Channel_1~7/DMA2_Channel_1~5
//      par:�����ַ
//      mar:�洢����ַ
//      dir,���򣬴����赽�ڴ桢�ڴ浽�ڴ棬���赽����
//      msize,
//      psize,
//      ndtr,
// ���أ�
// =============================================================================
u32 DMA_Config(DMA_Channel_TypeDef *DMA_Channelx,u32 par,u32 mar,    //OK
        u8 dir,u8 msize,u8 psize,u16 ndtr)
{
    DMA_TypeDef *DMAx;
    u8 channelx;  //ͨ����
    u32 WaitTime = DMA_WAIT_TIME;


    //�ж���������Ƿ����Ҫ��
    if( ((u32)DMA_Channelx < (u32)DMA1) || ((u32)DMA_Channelx > (u32)DMA2_Channel5)
            || (dir > DMA_DIR_M2M) || ( msize > DMA_DATABITS_32)|| (psize > DMA_DATABITS_32) )
        return false;


    if((u32)DMA_Channelx>(u32)DMA2) //�õ���ǰChannel������DMA2����DMA1
    {
        DMAx=DMA2;
        RCC->AHBENR|=1<<1;//DMA2ʱ��ʹ��
    }else
    {
        DMAx=DMA1;
        RCC->AHBENR|=1<<0;//DMA1ʱ��ʹ��
    }

    DMA_Channelx->CCR&=~(1<<0);     //�ر�DMA����,�Լ���ӣ���������
    while(DMA_Channelx->CCR&0X01)   //�ȴ�DMA������,CCR��ENλ��1
    {
        if(WaitTime > 50)
        {
            DJY_DelayUs(50);
            WaitTime = WaitTime - 50;
        }
        else
        {
            if(DMAx == DMA2)
                RCC->AHBENR &= ~(1<<1); //DMA2ʱ��ʧ��
            else
                RCC->AHBENR &= ~(1<<0); //DMA1ʱ��ʧ��
            return false;
        }
    }
    channelx=((((u32)DMA_Channelx-(u32)DMAx)-0X08)/0X14)+1;    //�õ�ͨ����

    //���֮ǰ��channel�ϵ������жϱ�־
    if(channelx==1)
        DMAx->IFCR|=0x0F;    //ͨ��1,ǰ4λ��1���㣬��������
    else if(channelx==2)
        DMAx->IFCR|=0x0F0;
    else if(channelx==3)
        DMAx->IFCR|=0x0F00;
    else if(channelx==4)
        DMAx->IFCR|=0x0F000;
    else if(channelx==5)
        DMAx->IFCR|=0x0F0000;
    else if(channelx==6)
        DMAx->IFCR|=0x0F00000;
    else if(channelx==7)
        DMAx->IFCR|=0x0F000000;

    DMA_Channelx->CPAR=par;         //DMA�����ַ
    DMA_Channelx->CMAR=mar;         //DMA �洢����ַ
    DMA_Channelx->CNDTR=ndtr;       //DMA ����������
    DMA_Channelx->CCR=0;            //��ȫ����λCCR�Ĵ���ֵ

    DMA_Channelx->CCR|= (dir == DMA_DIR_P2M) ? (0<<4) : (1<<4); //���赽�洢��ģʽ
    DMA_Channelx->CCR|=0<<5;            //��ѭ��ģʽ(��ʹ����ͨģʽ)
    DMA_Channelx->CCR|=0<<6;            //���������ģʽ
    DMA_Channelx->CCR|=1<<7;            //�洢������ģʽ
    DMA_Channelx->CCR|=psize<<8;        //�������ݳ���:8λ
    DMA_Channelx->CCR|=msize<<10;       //�洢�����ݳ���:8λ
    DMA_Channelx->CCR|=1<<12;           //�е����ȼ�

//  DMA_Channelx->CCR|=0<<21;           //����ͻ�����δ���     //F3������ͻ�����δ�����洢��ͻ�����δ���
//  DMA_Channelx->CCR|=0<<23;           //�洢��ͻ�����δ���
    DMA_Channelx->CCR|=(dir == DMA_DIR_P2M)?(1<<2):(1<<1);//�봫���ж�

    return true;
}

// =============================================================================
// ���ܣ�����һ��DMA����
// ������DMA_Channelx:DMAͨ��������,DMA1_Channel1~7/DMA2_Channel1~5
//       ndtr:���ݴ�����
// ���أ���
// =============================================================================
void DMA_Enable(DMA_Channel_TypeDef *DMA_Channelx,u32 mar,u16 ndtr)   //OK
{
    DMA_TypeDef *DMAx;
    u8 channelx;

    if((u32)DMA_Channelx>(u32)DMA2) //�õ���ǰchannel������DMA2����DMA1
    {
        DMAx=DMA2;
        RCC->AHBENR|=1<<1;//DMA2ʱ��ʹ��
    }else
    {
        DMAx=DMA1;
        RCC->AHBENR|=1<<0;//DMA1ʱ��ʹ��
    }


    DMA_Channelx->CCR&=~(1<<0);         //�ر�DMA����
    while(DMA_Channelx->CCR&0X1);       //ȷ��DMA���Ա�����

    channelx=((((u32)DMA_Channelx-(u32)DMAx)-0X08)/0X14)+1;    //�õ�ͨ����
    //���֮ǰ��channel�ϵ������жϱ�־
    if(channelx==1)
        DMAx->IFCR|=0x0F;    //ǰ4λ��1���㣬��������
    else if(channelx==2)
        DMAx->IFCR|=0x0F0;
    else if(channelx==3)
        DMAx->IFCR|=0x0F00;
    else if(channelx==4)
        DMAx->IFCR|=0x0F000;
    else if(channelx==5)
        DMAx->IFCR|=0x0F0000;
    else if(channelx==6)
        DMAx->IFCR|=0x0F00000;
    else if(channelx==7)
        DMAx->IFCR|=0x0F000000;

    DMA_Channelx->CNDTR=ndtr;       //DMA ������
    DMA_Channelx->CMAR=mar;         //DMA �洢����ַ

    DMA_Channelx->CCR|= (1<<0);     //����DMA����
}
bool_t DMA_SetIntModeComplete(DMA_Channel_TypeDef *DMA_Channelx)
{
    DMA_Channelx->CCR |=(1<<4);
    return true;
}

// =============================================================================
// ���ܣ���ֹʹ����Ӧ��DMA��
// ������DMA_Channelx:DMAͨ��������,DMA1_Channel1~7/DMA2_Channel1~5
// ���أ���
// =============================================================================
void DMA_Disable(DMA_Channel_TypeDef *DMA_Channelx)        //OK
{
    DMA_Channelx->CCR&=~(1<<0);         //�ر�DMA����
    while(DMA_Channelx->CCR&0X1);       //ȷ��DMA���Ա�����
}

// =============================================================================
// ���ܣ������Ӧ���ϵ������жϱ�־λ
// ������DMA_Channelx:DMA������,DMA1_Channel1~7/DMA2_Channel1~5
// ���أ���
// =============================================================================
void DMA_ClearIntFlag(DMA_Channel_TypeDef *DMA_Channelx)  //OK
{
    DMA_TypeDef *DMAx;
    u8 channelx;

    if((u32)DMA_Channelx>(u32)DMA2)//�õ���ǰstream������DMA2����DMA1
    {
        DMAx=DMA2;
        RCC->AHBENR|=1<<1;//DMA2ʱ��ʹ��
    }else
    {
        DMAx=DMA1;
        RCC->AHBENR|=1<<0;//DMA1ʱ��ʹ��
    }

    channelx=((((u32)DMA_Channelx-(u32)DMAx)-0X08)/0X14)+1;    //�õ�ͨ����

    //���֮ǰ��channel�ϵ������жϱ�־
    if(channelx==1)
        DMAx->IFCR|=0x0F;     //ǰ4λ��1���㣬��������
    else if(channelx==2)
        DMAx->IFCR|=0x0F0;
    else if(channelx==3)
        DMAx->IFCR|=0x0F00;
    else if(channelx==4)
        DMAx->IFCR|=0x0F000;
    else if(channelx==5)
        DMAx->IFCR|=0x0F0000;
    else if(channelx==6)
        DMAx->IFCR|=0x0F00000;
    else if(channelx==7)
        DMAx->IFCR|=0x0F000000;
}

// =============================================================================
// ���ܣ�ʹ����Ӧ���ϵ��ж�
// ������DMA_Channelx:DMA������,DMA1_Channel1~7/DMA2_Channel1~5
//     SrcInt,�ж�λ���룬��DMA_INT_HTIE��Ϊ(1<<3)
// ���أ�
// =============================================================================
void DMA_IntEnable(DMA_Channel_TypeDef *DMA_Channelx,u8 SrcInt)    //OK
{
    DMA_Channelx->CCR&=~(1<<0);         //�ر�DMA����
    while(DMA_Channelx->CCR&0X1);       //ȷ��DMA���Ա�����

    DMA_Channelx->CCR |= SrcInt;  //�ж�λ����SrcInt��������
}

// =============================================================================
// ���ܣ��ж���Ӧ�����ж�
// ������DMA_Streamx:DMA������,DMA1_Channel1~7/DMA2_Channel1~5
//     SrcInt,�ж�λ���룬��DMA_INT_HTIE��Ϊ(1<<3)
// ���أ���
// =============================================================================
void DMA_IntDisable(DMA_Channel_TypeDef *DMA_Channelx,u8 SrcInt)  //OK
{
    DMA_Channelx->CCR&=~(1<<0);         //�ر�DMA����
    while(DMA_Channelx->CCR&0X1);       //ȷ��DMA���Ա�����

    DMA_Channelx->CCR &= ~SrcInt;
}


