//----------------------------------------------------
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

#ifndef __BOARD__H__
#define __BOARD__H__

#include "./include/ad_sr2711.h"
#include "./include/kiko.h"
#include "./include/powerctrl.h"

#ifdef __cplusplus
extern "C" {
#endif

void UART1_GpioInit(void);
void UART3_GpioInit(void);
void UART4_GpioInit(void);
void UART5_GpioInit(void);
void UART6_GpioInit(void);
void Set_UartTransportChannel(u8 SerialNo,u8 CsNo);
void Board_SetCsSend(u8 SerialNo,u8 CsNo);
void Board_SetCsRecv(u8 SerialNo,u8 CsNo);
void Board_UartHalfDuplexSend(u8 SerialNo);
void Board_UartHalfDuplexRecv(u8 SerialNo);
void Board_UartGpioInit(void);
void Eth_GpioInit(void);
void DS3232M_GpioInit(void);
bool_t Board_SpiCsCtrl(u8 SPIPort,u8 cs,u8 level);
void GMAC_InLowPowerPinCfg(void);
void GMAC_OutLowPowerPinCfg(void);
void UART_InLowPowerPinCfg(u8 SerialNo);
void UART_OutLowPowerPinCfg(u8 SerialNo);
void SPI_InLowPowerPinCfg(u8 port);
void SPI_OutLowPowerPinCfg(u8 port);
void IIC_InLowPowerPinCfg(u8 iic_port);
void IIC_OutLowPowerPinCfg(u8 iic_port);
void Board_GpioInit(void);
void M4G_PowerSet(void);
void M4G_PowerClr(void);

bool_t ModuleInstall_ExpBkpsram(ptu32_t para);
bool_t ModuleInstall_ExpBkpsramTest(ptu32_t para);

ptu32_t EXTI_WakeUpIrq(u8 irqline);
void SR2711_LowPowerCfg(void);

void HAL_LPTIM_CompareMatchCallback(LPTIM_HandleTypeDef *hlptim);
void HAL_LPTIM_AutoReloadMatchCallback(LPTIM_HandleTypeDef *hlptim);
void HAL_LPTIM_MspInit(LPTIM_HandleTypeDef *hlptim);

ptu32_t LP_TIM_Shell_Module_Install(void);

bool_t ModemReset(void);

bool_t HAL_SetUpdateFlag(void);
bool_t HAL_ClrUpdateFlag(void);
bool_t HAL_CheckUpdateFlag(void);

void USB_IOs(void);
void USB_DeviceReset(u8 bHostID, u8 bDevID);
void USB_UserInstall(u8 bHostID);
void USB_UserSuspend(u16 wHostID);
void USB_UserResume(u16 wHostID);
void USB_FS_GPIO_Init(void);
void USB_HS_GPIO_Init(void);

ptu32_t Wwdg_Shell_Module_Install(void);
bool_t BrdWdt_FeedDog(void);
u32 FeedDog_Isr(ptu32_t intline);
bool_t BrdBoot_FeedStart(u32 bootfeedtime);
bool_t BrdBoot_FeedEnd(void);
void WWDG_Init(void);
void ModuleInstall_BrdWdt(void);

#ifdef __cplusplus
}
#endif
#endif






