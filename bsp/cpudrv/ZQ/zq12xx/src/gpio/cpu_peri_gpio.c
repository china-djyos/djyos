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

// �ļ���     ��cpu_peri_gpio.h
// ģ������: STM32F7xx��GPIO����
// ģ��汾:
// ������Ա:
// ����ʱ��: 10/28.2016
// =============================================================================
#ifdef CFG_CPU_ZQ12XX_M0
#include "stdint.h"
#include "silan_iomux.h"
#include "cpu_peri_isr.h"
#include "cpu_peri_gpio.h"
#include "cpu_peri.h"

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���

//%$#@end initcode  ****��ʼ���������
//%$#@describe      ****���������ʼ
//component name:"cpu onchip gpio"//gpio����������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                  //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable               //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                 //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:none                 //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                 //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence                   //������������������������none����ʾ�������������
                                 //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                 //����������������������г�
//weakdependence:"none"          //��������������������������none����ʾ�������������
                                 //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                 //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                 //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO == false )
//#warning  " cpu_onchip_gpio  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

/*Gpio Module*/

typedef struct
{
    volatile u32 GPIO_DATA;//0x000
}tagRegGpioData;


typedef struct
{
   volatile u32 GPIO_DIR; //0x400
   volatile u32 GPIO_IS;  //0x404
   volatile u32 GPIO_IBE; //0x408
   volatile u32 GPIO_IEV; //0x40C
   volatile u32 GPIO_IE;  //0x410
   volatile u32 GPIO_RIS; //0x414
   volatile u32 GPIO_MIS; //0x418
   volatile u32 GPIO_IC;  //0x41C
   volatile u32 GPIOAFSEL;//0x420
   volatile u32 GPIOFILSEL;//0x424
   volatile u32 GPIODIV;  //0x428

}tagRegGpio;

#define  GPIO1_REG_BASE ((volatile tagRegGpio*)(0x41070000 + 0x400))
#define  GPIO1_REG_DATA ((volatile u32*)(0x41070000 + 0x00))

#define  GPIO2_REG_BASE (volatile tagRegGpio*)(0x41080000 + 0x400)
#define  GPIO2_REG_DATA ((volatile u32 *)(0x41080000 + 0x00))

typedef struct
{
   tagRegGpioData *pRegGpioData;//data Register
   tagRegGpio     *pRegGpioBase;
}tagRegBaseInfo;

#define CN_GPIO_GROUP_NUM   (2)

#define CN_GPIO1_MAX_NUM    22
#define CN_GPIO2_MAX_NUM    10

/*get gpio pin group location*/

#define GET_GPIO_PIN_GROUP(gpio)    (gpio / (CN_GPIO1_MAX_NUM+1))

/*get gpio pin register location*/

#define GET_GPIO_REG_OFFSET(gpio)    (gpio % (CN_GPIO1_MAX_NUM+1))

/*turn to gpio pin*/

#define IO_TO_GPIO_PIN(x)    ((x>=8) ? (x-8) : (34+x))


/*interrupt Control Register 6 and 7*/

#define REG_INT_CTRL_INT_LINE6 (0x42030000 + 0x618)
#define REG_INT_CTRL_INT_LINE7 (0x42030000 + 0x61C)

static volatile u32 *spg_RegIntEnable[2] = {
    (volatile u32*)(REG_INT_CTRL_INT_LINE6),
    (volatile u32*)(REG_INT_CTRL_INT_LINE7),
};


/*Gpio Module Register Info*/

static volatile tagRegBaseInfo s_RegGpioInfo[CN_GPIO_GROUP_NUM] =
{
    {   //Gpio Group 1
        (volatile u32 *)GPIO1_REG_DATA,
        (volatile tagRegGpio*)GPIO1_REG_BASE,
    },
       //Gpio Group 2
    {
        (volatile u32 *)GPIO2_REG_DATA,
        (volatile tagRegGpio*)GPIO2_REG_BASE,
    }
};



// =============================================================================
// Function Describe:
// Config io to input
//
// ���ö�Ӧ ��IOΪ����ģʽ
// =============================================================================

void Gpio_Input_Config(u8 io)
{
   u8 byPinRegLoc;
   u8 byPinGroupLoc;
   u8 byGpio;
   volatile tagRegGpio* GpioRegBase  = NULL;

   //turn to gpio
   byGpio = IO_TO_GPIO_PIN(io);

   //get Pin which Group
   byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

   //get offence
   byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);

   GpioRegBase = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioBase;

   if(GpioRegBase != NULL)
   {
       GpioRegBase->GPIO_DIR &= ~(1 << byPinRegLoc);
   }

}


// =============================================================================
// Function Describe:
// Config io to output
//
//
// =============================================================================

void Gpio_Output_Config(u8 io)
{
   u8 byPinRegLoc;
   u8 byPinGroupLoc;
   u8 byGpio;
   volatile tagRegGpio* GpioRegBase  = NULL;

   //turn to gpio
   byGpio = IO_TO_GPIO_PIN(io);

   //get Pin which Group
   byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

   //get offence
   byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);

   GpioRegBase = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioBase;

   if(GpioRegBase != NULL)
   {
       GpioRegBase->GPIO_DIR |= (1 << byPinRegLoc);
   }
}


// =============================================================================
// Function Describe:
// Config io input and output derection config
// arg0:io
// arg1:bDerection TRUE - output False :input
// =============================================================================

void Gpio_Derection_Config(u8 io,bool_t bDerection)
{
    if(bDerection)
    {
        Gpio_Output_Config(io);
    }else
    {
        Gpio_Input_Config(io);
    }
}


// =============================================================================
// Function Describe:
// Set io Level Hight
// arg0:io
// arg1:bDerection TRUE - output False :input
// =============================================================================

void Gpio_Set_Value_Hight(u8 io)
{
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpioData* GpioRegData  = NULL;

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);

    GpioRegData = (volatile tagRegGpioData*)s_RegGpioInfo[byPinGroupLoc].pRegGpioData;

    if(GpioRegData != NULL)
    {
        GpioRegData->GPIO_DATA |= (1 << byPinRegLoc);
    }
}


// =============================================================================
// Function Describe:
// Set io Level Low
// arg0:io
// arg1:bDerection TRUE - output False :input
// =============================================================================

void Gpio_Set_Value_Low(u8 io)
{
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpioData* GpioRegData  = NULL;

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);

    GpioRegData = (volatile tagRegGpioData*)s_RegGpioInfo[byPinGroupLoc].pRegGpioData;

    if(GpioRegData != NULL)
    {
        GpioRegData->GPIO_DATA &= ~(1 << byPinRegLoc);
    }
}


// =============================================================================
// Function Describe:
// Set io Level Low
// arg0:io
// arg1:bDerection TRUE - output False :input
// =============================================================================

u8 Gpio_Get_Level(u8 io)
{
    u8 byRet;
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpioData* GpioRegData  = NULL;

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);

    GpioRegData = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioData;

    if(GpioRegData != NULL)
    {
        if(GpioRegData->GPIO_DATA & (1 << byPinRegLoc))

           byRet = 1;
        else
           byRet = 0;

    }

    return byRet;
}

// =============================================================================
// Function Describe:
// Enable Int
// arg0:io
// arg1:
// =============================================================================

void Gpio_Enable_Int(u8 io)
{
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpio* GpioRegBase  = NULL;

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);

    GpioRegBase = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioBase;

    if(GpioRegBase != NULL)
    {
        GpioRegBase->GPIO_IE |= (1 << byPinRegLoc);
        //int Ctrl Register
        *(spg_RegIntEnable[byPinRegLoc]) = 1 << byPinRegLoc;
    }


}


// =============================================================================
// Function Describe:
// Disable Int
// arg0:io
// arg1:
// =============================================================================

void Gpio_Disable_Int(u8 io)
{
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpio* GpioRegBase  = NULL;

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);

    GpioRegBase = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioBase;

    if(GpioRegBase != NULL)
    {
        GpioRegBase->GPIO_IE &= ~(1 << byPinRegLoc);
        *(spg_RegIntEnable[byPinRegLoc]) &= ~(1 << byPinRegLoc);
    }
}



// =============================================================================
// Function Describe:
// Clear Irq
// arg0:io
// arg1:
// =============================================================================

void Gpio_Clear_Irq(u8 io)
{
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpio* GpioRegBase  = NULL;

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);

    GpioRegBase = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioBase;

    if(GpioRegBase != NULL)
    {
        GpioRegBase->GPIO_IC |= (1 << byPinRegLoc);
    }
}


// =============================================================================
// Function Describe:
// �����ⲿ�жϴ���ģʽ
// io:��Ҫ���õ�io
// trugleMethod:    ���ش��� ���� ��ƽ����
// edgeTrugleMethod:�����Ǹߡ��͵�ƽ�������������ػ����½��ش���
// =============================================================================

void Gpio_Set_Iqr_Mode(u8 io,u8 trugleMethod,u8 edgeTrugleMethod)
{
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpio* GpioRegBase  = NULL;

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);

    GpioRegBase = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioBase;

    switch(trugleMethod)
    {
        case EN_EDGE:   //���ش���
             GpioRegBase->GPIO_IS &= ~(1<<byPinRegLoc);
             if(edgeTrugleMethod == EN_BOTH_EDGE)           //˫���ش���
             {
                GpioRegBase->GPIO_IBE |= (1<<byPinRegLoc);  //˫���ش���
             }
             else if(edgeTrugleMethod == EN_RISING_EDGE)    //�����ش���
             {
                 //ѡ�������ػ����½��ش���
                 GpioRegBase->GPIO_IBE &= ~(1<<byPinRegLoc);
                 //ѡ��ߵ�ƽ���������ش���
                 GpioRegBase->GPIO_IEV |= (1<<byPinRegLoc);
             }else if(edgeTrugleMethod == EN_FALLING_EDGE)  //�½��ش���
             {
                 //ѡ�������ػ����½��ش���
                 GpioRegBase->GPIO_IBE &= ~(1<<byPinRegLoc);
                 //ѡ��͵�ƽ�����½��ش���
                 GpioRegBase->GPIO_IEV &= ~(1<<byPinRegLoc);
             }
             break;
        case EN_LEVEL:                                      //��ƽ����
             GpioRegBase->GPIO_IS |= (1<<byPinRegLoc);

             if(edgeTrugleMethod == EN_HIGHT_LAVEL_EDGE)
             {
                GpioRegBase->GPIO_IEV |= (1<<byPinRegLoc); //�ߵ�ƽ����
             }else if(edgeTrugleMethod == EN_LOW_LEVEL_EDGE)
             {
                GpioRegBase->GPIO_IEV &= ~(1<<byPinRegLoc);//�͵�ƽ����
             }
             break;
    }

}


// =============================================================================
// Function Describe:
// 滤波分频选择
//
//
// =============================================================================

void Gpio_Io_Filter_Config_Div(u8 io,u8 div)
{
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpio* GpioRegBase  = NULL;

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);

    GpioRegBase = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioBase;

    GpioRegBase->GPIODIV |= ((div&0xf)<<1)|0x1;

}


// =============================================================================
// Function Describe:
// Gpio ���ǲ�����
//
//
// =============================================================================


void Gpio_io_Filter_Open(u8 io)
{
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpio* GpioRegBase  = NULL;
    u8 intLine;

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);

    GpioRegBase = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioBase;

    GpioRegBase->GPIOFILSEL |= 1<<byPinRegLoc ;

}



// =============================================================================
// Function Describe:
// �ر� Gpio �˲�����
//
//
// =============================================================================

void Gpio_io_Filter_Close(u8 io)
{
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpio* GpioRegBase  = NULL;

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);

    GpioRegBase = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioBase;

    GpioRegBase->GPIOFILSEL &= ~(1<<byPinRegLoc) ;

}


// =============================================================================
// Function Describe:
// Gpio �ⲿ�ж�����
//
//
// =============================================================================

enum EN_GPIO_INTLINE{
    EN_GPIO1_INT_LINE = 0x00,
    EN_GPIO2_INT_LINE,
};

void Register_Gpio_Irq(u8 io,fnGpioHandle_t IsrHandle)
{
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpio* GpioRegBase  = NULL;
    u8 intLine;      //�ж���
    u8 subIntLine;   //���ж���

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);

    GpioRegBase = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioBase;

    //Gpio1 Ϊ6���ж��� Gpio2 Ϊ7�� �ж���
    switch(byPinGroupLoc)
    {
        case EN_GPIO1_INT_LINE:
             intLine    = 6;
             subIntLine = byPinRegLoc;
             break;
        case EN_GPIO2_INT_LINE:
             intLine = 7;
             subIntLine = byPinRegLoc;
             break;
        default:;break;
    }

    if(IsrHandle != NULL)
    {
        djybsp_isr_hdl_register(intLine, subIntLine,IsrHandle,0);
    }

}

#endif

