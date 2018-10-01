//-----------------------------------------------------------------------------
// Copyright (c) 2018,Open source team. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ�ɶ����߲���ϵͳ��Դ�Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
#include <stdint.h>
#include <stddef.h>
#include <cpu_peri_gpio.h>
#include <stm32f7xx_hal.h>
#include <systime.h>


const Pin USB_FS[] = {
    {GPIO_A, PIN11, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_VH, GPIO_PUPD_NONE, AF10},
    {GPIO_A, PIN12, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_VH, GPIO_PUPD_NONE, AF10},
};

const Pin USB_HS[] = {
    {GPIO_H, PIN2, GPIO_MODE_IN, GPIO_OTYPE_PP, GPIO_SPEED_H, GPIO_PUPD_NONE}, // over current
    {GPIO_H, PIN3, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_H, GPIO_PUPD_NONE}, // power switch
    {GPIO_B, PIN13, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_VH, GPIO_PUPD_NONE, AF12}, // VBUS_FS2
    {GPIO_B, PIN14, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_VH, GPIO_PUPD_NONE, AF12}, // DM
    {GPIO_B, PIN15, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_VH, GPIO_PUPD_NONE, AF12}, // DP
};


//USB����״ָ̬ʾ��

const Pin USB_Power[1]={
    {GPIO_H, PIN3, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_PU, AF_NUll},
};
const Pin USB_LED1[]={
    {GPIO_C, PIN3, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},
};


const Pin M4G_RST[1]={
    {GPIO_I,PIN10, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_PU, AF_NUll},
};
// ============================================================================
// ���ܣ�USB IAP״̬����
// ������
// ���أ�
// ===========================================================================
void USB_IOs(void)
{
    __HAL_RCC_GPIOE_CLK_ENABLE();
    PIO_Configure(USB_LED1, PIO_LISTSIZE(USB_LED1));  //USB LED1

    __HAL_RCC_GPIOB_CLK_ENABLE();
    PIO_Configure(M4G_RST, PIO_LISTSIZE(M4G_RST));
    PIO_Set(&M4G_RST[0]);
}

// ============================================================================
// ���ܣ�USB IAP״̬����
// ������
// ���أ�
// ============================================================================
void USB_IAP_TurnOnLED(void)
{
    PIO_Set(USB_LED1);
}

// ============================================================================
// ���ܣ�USB IAP״̬����
// ������
// ���أ�
// ============================================================================
void USB_IAP_TurnOffLED(void)
{
    PIO_Clear(USB_LED1);
}

// ============================================================================
// ���ܣ�USB�豸��λ�߼�
// ������
// ���أ�
// ============================================================================
void USB_UDiskReset(void)
{
    PIO_Set(&USB_Power[0]);
    Djy_EventDelay(800*mS);

    PIO_Clear(&USB_Power[0]);
    Djy_EventDelay(5000*mS);
}
void USB_DeviceReset(u8 bHostID, u8 bDevID)
{
    if((1 == bHostID) && (0 == bDevID))
    {
        // 4Gģ�鸴λ
        //Djy_EventDelay(10000*mS);
        PIO_Clear(&M4G_RST[0]); // reset device
        Djy_EventDelay(1000*mS);
        PIO_Set(&M4G_RST[0]); // release the reset
        Djy_EventDelay(20000*mS); // wait for device ready
    }
    else if(2 == bHostID)
    {
        USB_UDiskReset();
    }
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ============================================================================
void USB_UserInstall(u8 bHostID)
{
    if(0x1 == bHostID)
    {
        extern s32 USBH_ResigerDevMSC(void);
        USBH_ResigerDevMSC();
    }
}
// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
void USB_FS_GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    PIO_Configure(USB_FS, PIO_LISTSIZE(USB_FS));
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
void USB_HS_GPIO_Init(void)
{
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    PIO_Configure(USB_HS, PIO_LISTSIZE(USB_HS));
    PIO_Configure(USB_Power, PIO_LISTSIZE(USB_Power));

}
