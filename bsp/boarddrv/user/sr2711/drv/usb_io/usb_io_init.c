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
#include <int.h>
#include <string.h>

// �ܽŶ�����board.c֮��
extern const Pin USB_FS[2];
extern const Pin USB_HS[2];
extern const Pin M4G_RST[1];
extern const Pin M4G_WAKEUP[1];
extern const Pin M4G_POWER[1];
extern const Pin M4G_WAKEUP_INPUT[1];

extern char *CUSTOM_GetInfos(void);
void __M4G_Wakeup(void);
void __M4G_Sleep(void);


#define SUPPORTS                    4 // Ŀǰ֧�ֵ�Ʒ��

struct __sleepCommands{
    char *pBrand;
    u32 dwCount;
    char *pCommand[2];
};

struct __sleepCommands brands[] = {
    {"HUAWEI", 2, {"AT^WAKEUPCFG=0,0,0 \r", "AT^CURC=0 \r"}},
    {"SIMCOM", 2, {"AT+CSCLK=1 \r", "AT&D0 \r"}},
    {"FORGE", 1, {"AT+SLEEPEN=1 \r"}},
    {"QUECTEL", 1, {"AT+QSCLK=1 \r"}}
};

static u8 configLowPower = 1; // ÿ�θ�λ��ϵͳ���½���͹���ʱ����Ҫ��ǰ����


// ============================================================================
// ���ܣ�USB IO�����߼�
// ������
// ���أ�
// ��ע��
// ===========================================================================
void USB_IOs(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    PIO_Configure(M4G_RST, PIO_LISTSIZE(M4G_RST));
    PIO_Set(&M4G_RST[0]);

    __HAL_RCC_GPIOK_CLK_ENABLE();
    PIO_Configure(M4G_POWER, PIO_LISTSIZE(M4G_POWER));
    PIO_Set(&M4G_POWER[0]);

    __HAL_RCC_GPIOH_CLK_ENABLE();
    PIO_Configure(M4G_WAKEUP, PIO_LISTSIZE(M4G_WAKEUP));
    PIO_Clear(&M4G_WAKEUP[0]);
}

// ============================================================================
// ���ܣ�USB�豸��λ�߼�
// ������
// ���أ�
// ��ע��ͨ����λ�ܽ�
// ============================================================================
void USB_DeviceReset(u8 bHostID, u8 bDevID)
{
    if((1 == bHostID) && (0 == bDevID))
    {
        // 4Gģ�鸴λ
        //Djy_EventDelay(10000*mS);
#if 0
        PIO_Clear(&M4G_RST[0]); // reset device
        Djy_EventDelay(1000*mS);
        PIO_Set(&M4G_RST[0]); // release the reset
        Djy_EventDelay(20000*mS); // wait for device ready
#else
        PIO_Clear(&M4G_POWER[0]);
        Djy_EventDelay(1000*mS);
        PIO_Set(&M4G_POWER[0]);
        Djy_EventDelay(20000*mS); // wait for device ready
#endif
        configLowPower = 1;
    }
    else if((2 == bHostID) && (0 == bDevID))
    {
        // USB_FSû�и�λ�߼�
    }
}

// ============================================================================
// ���ܣ�USB�豸����MCU���жϷ���
// ������
// ���أ�
// ��ע��δʹ�ã��޷�ʵ�ֹ���
// ============================================================================
static ptu32_t __M4G_IrqService(u8 bLine)
{
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_13) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);
    }

    Int_ClearLine(CN_INT_LINE_EXTI15_10);

    return (1);
}

// ============================================================================
// ���ܣ�USB�豸����MCU���жϳ�ʼ��
// ������
// ���أ�
// ��ע��δʹ�ã��޷�ʵ�ֹ���
// ============================================================================
static void __M4G_IrqInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_13; // PC13Ϊ4Gģ�黽�ѵ�Ƭ��

    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed= GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    Int_Register(CN_INT_LINE_EXTI15_10);
    Int_SetClearType(CN_INT_LINE_EXTI15_10, CN_INT_CLEAR_AUTO);
    Int_IsrConnect(CN_INT_LINE_EXTI15_10, (uint32_t(*)(ptu32_t))__M4G_IrqService);
    Int_SettoReal(CN_INT_LINE_EXTI15_10);
    Int_ClearLine(CN_INT_LINE_EXTI15_10);
    Int_RestoreRealLine(CN_INT_LINE_EXTI15_10);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
void USB_UserInstall(u8 bHostID)
{
    if(0x1 == bHostID)
    {
        // __M4G_IrqInit(); // HS�ϵ�4Gģ��Ļ����ж��߼�.
    }
    else if(0x2 == bHostID)
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
void USB_UserSuspend(u16 wHostID)
{

    if(0x1 == (wHostID & 0xFF))
    {
        __M4G_Sleep();
    }
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
void USB_UserResume(u16 wHostID)
{
    if(0x1 == (wHostID & 0xFF))
    {
        __M4G_Wakeup();
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
//  __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    PIO_Configure(USB_HS, PIO_LISTSIZE(USB_HS));
}

// ============================================================================
// ���ܣ�����ظ����
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 __CheckResponse(char *pCommand)
{
    u32 i, j, res;
    u32 responsed;
    char response = 0;
    char buffer[128];
    u32 wait = 200;
    u32 cLen = strlen(pCommand);
    char OK[2] = {'O', 'K'};
    char ERROR[5] = {'E', 'R', 'R', 'O', 'R'};
    extern u32 __CUSTOM_DirectReadAT(u8 *pBuf, u32 dwLen, u32 dwTimeout);
    extern u32 __CUSTOM_SaveUnrelatedAT(u8 *pBuf, u32 dwLen);

    i = 0;
    responsed = 0;
    while(1)
    {
        res = __CUSTOM_DirectReadAT((u8*)&response, 1, 0);
        if(!res)
        {
            if(!wait--)
            {
                printk("\r\nUSB IO : error : \"%s\" has %s response.\r\n", pCommand, (responsed?"error":"no"));
                return (-1);
            }

            Djy_DelayUs(1000);
            continue;
        }

        responsed++;
        if(pCommand[i] == response)
        {
            buffer[i] = response;
            i++;
        }
        else
        {
            if(i)
            {
                __CUSTOM_SaveUnrelatedAT((u8*)buffer, i);
                i = 0; // ����
            }
            __CUSTOM_SaveUnrelatedAT((u8*)&response, 1);
        }

        if(i == cLen)
            break;
    }

    i = 0;
    j = 0;
    while(1)
    {
        if(OK[i] == response)
        {
            if(++i == 2)
                break;

        }
        else if(ERROR[j] == response)
        {
            if(++j == 5)
            {
                printk("\r\nUSB IO : error : \"%s\" has error response.\r\n", pCommand);
                return (1);
            }
        }
        else
        {
            i = 0; // ����
            j = 0;
        }

        res = __CUSTOM_DirectReadAT((u8*)&response, 1, 0);
        if(!res)
        {
            if(!wait--)
            {
                printk("\r\nUSB IO : error : \"%s\" has %s response.\r\n", pCommand, (responsed?"error":"no"));
                return (-1);
            }

            Djy_DelayUs(1000);
            continue;
        }

        responsed++;
    }

    return (0);
}

// ============================================================================
// ���ܣ�4Gģ���˳�˯��
// ������
// ���أ�
// ��ע�����Ѳ������ڴ���
// ============================================================================
void __M4G_Wakeup(void)
{
    ;// û��ʲôҪ����
}

// ============================================================================
// ���ܣ�4Gģ�����˯��
// ������
// ���أ�
// ��ע��˯�߲������ڴ���
// ============================================================================
void __M4G_Sleep(void)
{
    extern u32 __CUSTOM_DirectWriteAT(u8 *pBuf, u32 dwLen, u32 dwTimeout);
    u8 commandLen, i, j;
    u32 res;

    if(configLowPower)
    {
        char *workingBrand = CUSTOM_GetInfos();
        configLowPower = 0;

        if(!workingBrand)
            return ; // ģ��û����Ϣ

        for(i = 0; i < SUPPORTS; i++)
        {
            if(strstr(workingBrand, brands[i].pBrand))
            {
                break;
            }
        }

        if(i == SUPPORTS)
        {
            return; // Ŀǰ��֧�ָ�����
        }

        // ���ý���ģ�����͹����߼�
        for(j = 0; j < brands[i].dwCount; j++)
        {
            commandLen = (strlen(brands[i].pCommand[j]) + 1);
            res = __CUSTOM_DirectWriteAT((u8*)(brands[i].pCommand[j]), commandLen, 1000); //
            brands[i].pCommand[j][commandLen-2] = '\0';
            if(res != commandLen)
            {
                printk("\r\nUSB IO : error : \"%s\" set(send) failed during sleep.\r\n", brands[i].pCommand[j]);
            }

            res = __CheckResponse((brands[i].pCommand[j]));
            if(res)
            {
                printk("\r\n\"%s\" set(response) failed during sleep.\r\n", brands[i].pCommand[j]);
            }
        }
    }
}
