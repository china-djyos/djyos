//-----------------------------------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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

extern const Pin USB_FS[2];
extern const Pin USB_HS[2];
extern const Pin HUB_RST[1];
extern const Pin USB_LED1[1]; // USB����״ָ̬ʾ��
extern const Pin M4G_RST[1];
extern const Pin M4G_POWER[1];
extern const Pin HUB_POWER[1];
extern const Pin TOUCH_POWER[1];
extern void RunIboot(void);

// ============================================================================
// ���ܣ�USB IAP״̬����
// ������
// ���أ�
// ===========================================================================
void USB_IOs(void)
{
    PIO_Configure(USB_LED1, PIO_LISTSIZE(USB_LED1));  //USB LED1

    PIO_Configure(M4G_RST, PIO_LISTSIZE(M4G_RST));
    PIO_Set(&M4G_RST[0]);

    PIO_Configure(M4G_POWER, PIO_LISTSIZE(M4G_POWER));
    PIO_Set(&M4G_POWER[0]);

    PIO_Configure(HUB_RST, PIO_LISTSIZE(HUB_RST));
    PIO_Set(&HUB_RST[0]);

    PIO_Configure(HUB_POWER, PIO_LISTSIZE(HUB_POWER));
    PIO_Clear(&HUB_POWER[0]);
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
    PIO_Clear(&HUB_RST[0]); // reset
    PIO_Set(&HUB_POWER[0]);
    Djy_EventDelay(1000*mS);

    PIO_Clear(&HUB_POWER[0]);
    PIO_Set(&HUB_RST[0]); // release the reset
    Djy_EventDelay(30*mS);

    RunIboot();
}
void USB_DeviceReset(u8 bHostID, u8 bDevID)
{
    if((USB_4G_PORT == bHostID) && (0 == bDevID))
    {
        // 4Gģ�鸴λ
        //Djy_EventDelay(10000*mS);
        PIO_Clear(&M4G_POWER[0]); // reset device
        Djy_EventDelay(1000*mS);
        PIO_Set(&M4G_POWER[0]); // release the reset
        Djy_EventDelay(20000*mS); // wait for device ready
    }
    else if((USB_HUB_PORT == bHostID))
    {

        if(2 == bDevID)
        {
            PIO_Set(&TOUCH_POWER[0]); //
            Djy_EventDelay(10000*mS);
            PIO_Clear(&TOUCH_POWER[0]); //
            Djy_EventDelay(100*mS); //
            return;
        }
        else if (0 == bDevID)
        {
            // HUB��λ
            #if 0
            PIO_Clear(&HUB_RST[0]); // reset
            Djy_EventDelay(10*mS);
            PIO_Set(&HUB_RST[0]); // release the reset
            Djy_EventDelay(100*mS); // wait for device ready
            #elif 0
            PIO_Set(&HUB_POWER[0]); //
            Djy_EventDelay(1000*mS);
            PIO_Clear(&HUB_POWER[0]); //
            Djy_EventDelay(100*mS); //
            #else
            PIO_Clear(&HUB_RST[0]); // reset
            PIO_Set(&HUB_POWER[0]);
            Djy_EventDelay(1000*mS);

            PIO_Clear(&HUB_POWER[0]);
            Djy_EventDelay(30*mS);
            PIO_Set(&HUB_RST[0]); // release the reset
            #endif
        }
    }
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ============================================================================
void USB_UserInstall(u8 bHostID)
{
    if(USB_HUB_PORT == bHostID) // HUB
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
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
void M4G_PowerSet(void)
{
     PIO_Set(&M4G_POWER[0]);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
void M4G_PowerClr(void)
{
    PIO_Clear(&M4G_POWER[0]);
}
