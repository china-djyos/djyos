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

// �ļ���     ��cpu_peri_uart.c
// ģ������: DJYOS����ģ��ĵײ��������֣���Ҫʵ�ּĴ�������Ĳ��������жϵ�
// ģ��汾: V1.10
// ������Ա:
// ����ʱ��:
// =============================================================================

#include <djyos.h>
#include <stdint.h>
#include "cpu_peri.h"
#include "cpu_peri_temprate.h"
// =============================================================================
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern  bool_t Temprate_Init(void);
//    Temprate_Init( );
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip temprate"//CPU�Ĳ�����������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                          //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                       //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                         //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium                       //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                         //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"             //������������������������none����ʾ�������������
                                         //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                         //����������������������г�
//weakdependence:"none"                  //��������������������������none����ʾ�������������
                                         //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                         //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                         //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_TEMPRATE == false )
//#warning  " cpu_onchip_temprate  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_TEMPRATE    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

#define ADC_CH_TEMP     18

//ADʱ�ӷ�Ƶϵ��2,4,6,8.  PCLK2/ADC_CLK_DIV=CLk
#define ADC_CLK_DIV   4

// =============================================================================
// ����: ��ʼ��CPU�ڲ��¶ȴ�����
// ����: ��
// ����: ��
// =============================================================================

bool_t Temprate_Init(void)
{

    RCC->APB2ENR|=1<<8;     //ʹ��ADC1ʱ��
    RCC->APB2RSTR|=1<<8;    //ADCs��λ
    RCC->APB2RSTR&=~(1<<8); //��λ����
    ADC->CCR=((ADC_CLK_DIV>>1)-1)<<16;//ADʱ������
    ADC->CCR|=1<<23;        //ʹ���ڲ��¶ȴ�����

    ADC1->CR1=0;            //CR1��������
    ADC1->CR2=0;            //CR2��������
    ADC1->CR1|=0<<24;       //12λģʽ
    ADC1->CR1|=0<<8;        //��ɨ��ģʽ

    ADC1->CR2&=~(1<<1);     //����ת��ģʽ
    ADC1->CR2&=~(1<<11);    //�Ҷ���
    ADC1->CR2|=0<<28;       //�������

    ADC1->SQR1&=~(0XF<<20);
    ADC1->SQR1|=0<<20;      //1��ת���ڹ��������� Ҳ����ֻת����������1

    ADC1->SMPR1&=~(7<<(3*(18-10))); //���ͨ��18ԭ��������
    ADC1->SMPR1|=7<<(3*(18-10));    //ͨ��18 480����,��߲���ʱ�������߾�ȷ��

    ADC1->CR2|=1<<0;                //����ADת����
    return true;
}

// =============================================================================
// ����: ��ȡAD������ֵ
// ����: ͨ����
// ����: ADֵ
// =============================================================================
u16 Get_Adc(u8 ch)
{
    u16 timeout=0;
    //����ת������
    ADC1->SQR3&=0XFFFFFFE0;//��������1 ͨ��ch
    ADC1->SQR3|=ch;
    ADC1->CR2|=1<<30;       //��������ת��ͨ��
    while(!(ADC1->SR&1<<1))//�ȴ�ת������
    {
        timeout++;
        if(timeout>=1000000)
            return 0;//��ʱ����0
    }
    return ADC1->DR;
}

// =============================================================================
// ����:��ȡAD������ֵ
// ����: AD ͨ����
//      ��������
// ����: ��
// =============================================================================
static u16 Get_Adc_Average(u8 ch,u8 times)
{
    u32 temp_val=0;
    u8 i;
    for(i=0;i<times;i++)
    {
        temp_val+=Get_Adc(ch);
        DJY_DelayUs(5);
    }
    return temp_val/times;
}

// =============================================================================
// ����: ��ȡ�¶ȵ�ֵ
// ����: ��
// ע :����ֱ�ӷ��ص����Ȼ���˫���ȵĲ������غ���ֵ�����,
//   ���������ﲻ�ܲ���ֱ�ӷ��صķ�ʽ��ȡ�¶�
// ����: �¶�ֵ
// =============================================================================
float Get_Temprate(void)
{
    u32 adcx;
    float temperate;

    adcx=Get_Adc_Average(ADC_CH_TEMP,Temprate_num);

    temperate=(float)adcx*(3.3/4096);

    temperate=(temperate-0.76)/0.0025+25;

    return temperate;
}





