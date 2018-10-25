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
//����ģ��:�ж�ģ��
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: STM32F407�����жϺ�
//����˵��:
//�޶���ʷ:
//
//------------------------------------------------------
#ifndef __CPU_PERI_INT_LINE_H__
#define __CPU_PERI_INT_LINE_H__

#ifdef __cplusplus
extern "C" {
#endif

#define CN_INT_LINE_BOD             (0x00)  //0: Brown Out detection
#define CN_INT_LINE_IRC             (0x01)  //1: Internal RC
#define CN_INT_LINE_PWRWU           (0x02)  //2: Power down wake up
#define CN_INT_LINE_RAMPE           (0x03)  //3: RAM parity error
#define CN_INT_LINE_CKFAIL          (0x04)  //4: Clock detection fail
#define CN_INT_LINE_Default0        (0x05)  //5: Reserved
#define CN_INT_LINE_RTC             (0x06)  //6: Real Time Clock
#define CN_INT_LINE_TAMPER          (0x07)  //7: Tamper detection
#define CN_INT_LINE_WDT             (0x08)  //8: Watchdog timer
#define CN_INT_LINE_WWDT            (0x09)  //9: Window watchdog timer
#define CN_INT_LINE_EXTI0           (0x0A)  //10: External Input 0
#define CN_INT_LINE_EINT1           (0x0B)  //11: External Input 1
#define CN_INT_LINE_EINT2           (0x0C)  //12: External Input 2
#define CN_INT_LINE_EINT3           (0x0D)  //13: External Input 3
#define CN_INT_LINE_EINT4           (0x0E)  //14: External Input 4
#define CN_INT_LINE_EINT5           (0x0F)  //15: External Input 5
#define CN_INT_LINE_GPA             (0x10)  //16: GPIO Port A
#define CN_INT_LINE_GPB             (0x11)  //17: GPIO Port B
#define CN_INT_LINE_GPC             (0x12)  //18: GPIO Port C
#define CN_INT_LINE_GPD             (0x13)  //19: GPIO Port D
#define CN_INT_LINE_GPE             (0x14)  //20: GPIO Port E
#define CN_INT_LINE_GPF             (0x15)  //21: GPIO Port F
#define CN_INT_LINE_QSPI0           (0x16)  //22: QSPI0
#define CN_INT_LINE_SPI0            (0x17)  //23: SPI0
#define CN_INT_LINE_BRAKE0          (0x18)  //24:
#define CN_INT_LINE_EPWM0P0         (0x19)  //25:
#define CN_INT_LINE_EPWM0P1         (0x1A)  //26:
#define CN_INT_LINE_EPWM0P2         (0x1B)  //27:
#define CN_INT_LINE_BRAKE1          (0x1C)  //28:
#define CN_INT_LINE_EPWM1P0         (0x1D)  //29:
#define CN_INT_LINE_EPWM1P1         (0x1E)  //30:
#define CN_INT_LINE_EPWM1P2         (0x1F)  //31:
#define CN_INT_LINE_TMR0            (0x20)  //32: Timer 0
#define CN_INT_LINE_TMR1            (0x21)  //33: Timer 1
#define CN_INT_LINE_TMR2            (0x22)  //34: Timer 2
#define CN_INT_LINE_TMR3            (0x23)  //35: Timer 3
#define CN_INT_LINE_UART0           (0x24)  //36: UART0
#define CN_INT_LINE_UART1           (0x25)  //37: UART1
#define CN_INT_LINE_I2C0            (0x26)  //38: I2C0
#define CN_INT_LINE_I2C1            (0x27)  //39: I2C1
#define CN_INT_LINE_PDMA            (0x28)  //40: Peripheral DMA
#define CN_INT_LINE_DAC             (0x29)  //41: DAC
#define CN_INT_LINE_ADC00           (0x2A)  //42: ADC0 interrupt source 0
#define CN_INT_LINE_ADC01           (0x2B)  //43: ADC0 interrupt source 1
#define CN_INT_LINE_ACMP01          (0x2C)  //44: ACMP0 and ACMP1
#define CN_INT_LINE_Default1        (0x2D)  //45: Reserved
#define CN_INT_LINE_ADC02           (0x2E)  //46: ADC0 interrupt source 2
#define CN_INT_LINE_ADC03           (0x2F)  //47: ADC0 interrupt source 3
#define CN_INT_LINE_UART2           (0x30)  //48: UART2
#define CN_INT_LINE_UART3           (0x31)  //49: UART3
#define CN_INT_LINE_Default2        (0x32)  //50: Reserved
#define CN_INT_LINE_SPI1            (0x33)  //51: SPI1
#define CN_INT_LINE_SPI2            (0x34)  //52: SPI2
#define CN_INT_LINE_USBD            (0x35)  //53: USB device
#define CN_INT_LINE_OHCI            (0x36)  //54: OHCI
#define CN_INT_LINE_USBOTG          (0x37)  //55: USB OTG
#define CN_INT_LINE_CAN0            (0x38)  //56: CAN0
#define CN_INT_LINE_CAN1            (0x39)  //57: CAN1
#define CN_INT_LINE_SC0             (0x3A)  //58:
#define CN_INT_LINE_SC1             (0x3B)  //59:
#define CN_INT_LINE_SC2             (0x3C)  //60:
#define CN_INT_LINE_Default3        (0x3D)  //61:
#define CN_INT_LINE_SPI3            (0x3E)  //62: SPI3
#define CN_INT_LINE_Default4        (0x3F)  //63:
#define CN_INT_LINE_SDH0            (0x40)  //64: SDH0
#define CN_INT_LINE_USBD20          (0x41)  //65: USBD20
#define CN_INT_LINE_EMAC_TX         (0x42)  //66: EMAC_TX
#define CN_INT_LINE_EMAC_RX         (0x43)  //67: EMAX_RX
#define CN_INT_LINE_I2S0            (0x44)  //68: I2S0
#define CN_INT_LINE_Default5        (0x45)  //69: Add description to this Interrupt
#define CN_INT_LINE_OPA0            (0x46)  //70: OPA0
#define CN_INT_LINE_CRYPTO          (0x47)  //71: CRYPTO
#define CN_INT_LINE_GPG             (0x48)  //72:
#define CN_INT_LINE_EINT6           (0x49)  //73:
#define CN_INT_LINE_UART4           (0x4A)  //74: UART4
#define CN_INT_LINE_UART5           (0x4B)  //75: UART5
#define CN_INT_LINE_USCI0           (0x4C)  //76: USCI0
#define CN_INT_LINE_USCI1           (0x4D)  //77: USCI1
#define CN_INT_LINE_BPWM0           (0x4E)  //78: BPWM0
#define CN_INT_LINE_BPWM1           (0x4F)  //79: BPWM1
#define CN_INT_LINE_SPIM            (0x50)  //80: SPIM
#define CN_INT_LINE_Default6        (0x51)  //81: Add description to this Interrupt
#define CN_INT_LINE_I2C2            (0x52)  //82: I2C2
#define CN_INT_LINE_Default7        (0x53)  //83:
#define CN_INT_LINE_QEI0            (0x54)  //84: QEI0
#define CN_INT_LINE_QEI1            (0x55)  //85: QEI1
#define CN_INT_LINE_ECAP0           (0x56)  //86: ECAP0
#define CN_INT_LINE_ECAP1           (0x57)  //87: ECAP1
#define CN_INT_LINE_GPH             (0x58)  //88:
#define CN_INT_LINE_EINT7           (0x59)  //89:
#define CN_INT_LINE_SDH1            (0x5A)  //90: SDH1
#define CN_INT_LINE_Default8        (0x5B)  //91:
#define CN_INT_LINE_EHCI            (0x5C)  //92: EHCI
#define CN_INT_LINE_USBOTG20        (0x5D)  //93:
#define CN_INT_LINE_LAST            (0x5D)

#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_INT_LINE_H__

