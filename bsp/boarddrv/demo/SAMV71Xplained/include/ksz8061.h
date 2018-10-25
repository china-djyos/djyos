// ---------------------------------------------
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

// �ļ���     ��ksz8051.c
// ģ������: PHYоƬ��������ͷ�ļ���صĶ���
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 26/11.2015
// =============================================================================

#ifndef __GMII_DEFINE_H__
#define __GMII_DEFINE_H__

#include "cpu_peri_gmac.h"

//IEEE defined Registers
#define GMII_BMCR        0x0   // Basic Mode Control Register
#define GMII_BMSR        0x1   // Basic Mode Status Register
#define GMII_PHYID1R     0x2   // PHY Identifier Register 1
#define GMII_PHYID2R     0x3   // PHY Identifier Register 2
#define GMII_ANAR        0x4   // Auto_Negotiation Advertisement Register
#define GMII_ANLPAR      0x5   // Auto_negotiation Link Partner Ability Register
#define GMII_ANER        0x6   // Auto-negotiation Expansion Register
#define GMII_ANNPR       0x7   // Auto-negotiation Next Page Register
#define GMII_ANLPNPAR    0x8   // Auto_negotiation Link Partner Next Page Ability Register
#define GMII_AFEC0R      0x11  // AFE Control 0 Register
#define GMII_AFEC3R      0x14  // AFE Control 3 Register
#define GMII_RXERCR      0x15  // RXER Counter Register
#define GMII_OMSSR       0x17  // Operation Mode Strap Status Register
#define GMII_ECR         0x18  // Expanded Control Register
#define GMII_ICSR        0x1B  // Interrupt Control/Status Register
#define GMII_FC          0x1C  // Function Control
#define GMII_LCSR        0x1D  // LinkMD?Control/Status Register
#define GMII_PC1R        0x1E  // PHY Control 1 Register
#define GMII_PC2R        0x1F  // PHY Control 2 Register


// PHY ID Identifier Register
#define GMII_LSB_MASK           0x0U
// definitions: MII_PHYID1
#define GMII_OUI_MSB            0x0022
// definitions: MII_PHYID2
#define GMII_OUI_LSB            0x1572          // KSZ8061 PHY Id2

// Basic Mode Control Register (BMCR)
// Bit definitions: MII_BMCR
#define GMII_RESET             (1 << 15) // 1= Software Reset; 0=Normal Operation
#define GMII_LOOPBACK          (1 << 14) // 1=loopback Enabled; 0=Normal Operation
#define GMII_SPEED_SELECT_LSB  (1 << 13) // 1,0=1000Mbps 0,1=100Mbps; 0,0=10Mbps
#define GMII_AUTONEG           (1 << 12) // Auto-negotiation Enable
#define GMII_POWER_DOWN        (1 << 11) // 1=Power down 0=Normal operation
#define GMII_ISOLATE           (1 << 10) // 1 = Isolates 0 = Normal operation
#define GMII_RESTART_AUTONEG   (1 << 9)  // 1 = Restart auto-negotiation 0 = Normal operation
#define GMII_DUPLEX_MODE       (1 << 8)  // 1 = Full duplex operation 0 = Normal operation
//      Reserved                7        // Read as 0, ignore on write
#define GMII_SPEED_SELECT_MSB  (1 << 6)  //
//      Reserved                5 to 0   // Read as 0, ignore on write


// Basic Mode Status Register (BMSR)
// Bit definitions: MII_BMSR
#define GMII_100BASE_T4        (1 << 15) // 100BASE-T4 Capable
#define GMII_100BASE_TX_FD     (1 << 14) // 100BASE-TX Full Duplex Capable
#define GMII_100BASE_T4_HD     (1 << 13) // 100BASE-TX Half Duplex Capable
#define GMII_10BASE_T_FD       (1 << 12) // 10BASE-T Full Duplex Capable
#define GMII_10BASE_T_HD       (1 << 11) // 10BASE-T Half Duplex Capable
//      Reserved                10 to 9  // Read as 0, ignore on write
#define GMII_EXTEND_STATUS     (1 << 8)  // 1 = Extend Status Information In Reg 15
//      Reserved                7
#define GMII_MF_PREAMB_SUPPR   (1 << 6)  // MII Frame Preamble Suppression
#define GMII_AUTONEG_COMP      (1 << 5)  // Auto-negotiation Complete
#define GMII_REMOTE_FAULT      (1 << 4)  // Remote Fault
#define GMII_AUTONEG_ABILITY   (1 << 3)  // Auto Configuration Ability
#define GMII_LINK_STATUS       (1 << 2)  // Link Status
#define GMII_JABBER_DETECT     (1 << 1)  // Jabber Detect
#define GMII_EXTEND_CAPAB      (1 << 0)  // Extended Capability

// Auto-negotiation Advertisement Register (ANAR)
// Auto-negotiation Link Partner Ability Register (ANLPAR)
// Bit definitions: MII_ANAR, MII_ANLPAR
#define GMII_NP               (1 << 15) // Next page Indication
//      Reserved               7
#define GMII_RF               (1 << 13) // Remote Fault
//      Reserved               12       // Write as 0, ignore on read
#define GMII_PAUSE_MASK       (3 << 11) // 0,0 = No Pause 1,0 = Asymmetric Pause(link partner)
                                        // 0,1 = Symmetric Pause 1,1 = Symmetric&Asymmetric Pause(local device)
#define GMII_T4               (1 << 9)  // 100BASE-T4 Support
#define GMII_TX_FDX           (1 << 8)  // 100BASE-TX Full Duplex Support
#define GMII_TX_HDX           (1 << 7)  // 100BASE-TX Support
#define GMII_10_FDX           (1 << 6)  // 10BASE-T Full Duplex Support
#define GMII_10_HDX           (1 << 5)  // 10BASE-T Support
//      Selector                 4 to 0   // Protocol Selection Bits
#define GMII_AN_IEEE_802_3      0x00001

/** The reset length setting for external reset configuration */
#define GMACB_RESET_LENGTH         0xD

typedef struct _GMacb
{
    /**< Driver */
    sGmacd *pGmacd;
    /** The retry & timeout settings */
    uint32_t retryMax;
    /** PHY address ( pre-defined by pins on reset ) */
    uint8_t phyAddress;
} GMacb;

/*---------------------------------------------------------------------------
 *         Exported functions
 *---------------------------------------------------------------------------*/
extern void GMACB_SetupTimeout(GMacb *pMacb, uint32_t toMax);

extern void GMACB_Init(GMacb *pMacb, sGmacd *pGmacd, uint8_t phyAddress);

extern uint8_t GMACB_InitPhy(
                GMacb *pMacb,
                uint32_t mck
//              const Pin *pResetPins,
//              uint32_t nbResetPins,
//              const Pin *pEmacPins,
//              uint32_t nbEmacPins
                );

extern uint8_t GMACB_AutoNegotiate(GMacb *pMacb);

extern uint8_t GMACB_GetLinkSpeed(GMacb *pMacb, uint8_t applySettings);

extern uint8_t GMACB_Send(GMacb *pMacb, void *pBuffer, uint32_t size);

extern uint32_t GMACB_Poll(GMacb *pMacb, uint8_t *pBuffer, uint32_t size);

extern void GMACB_DumpRegisters(GMacb *pMacb);

extern uint8_t GMACB_ResetPhy(GMacb *pMacb);

extern bool_t GMACB_PhyIsLinkUp(GMacb *pMacb);

#endif // #ifndef _GMACB_H

