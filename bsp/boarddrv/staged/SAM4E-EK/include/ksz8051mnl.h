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

// �ļ���     ��ksz8051.c
// ģ������: PHYоƬ��������ͷ�ļ���صĶ���
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 26/11.2015
// =============================================================================

#ifndef __GMII_DEFINE_H__
#define __GMII_DEFINE_H__

#include "stdint.h"

/* IEEE defined Registers */
#define GMII_BMCR        0x00   /* Basic Control */
#define GMII_BMSR        0x01   /* Basic Status */
#define GMII_PHYID1      0x02   /* PHY Idendifier 1 */
#define GMII_PHYID2      0x03   /* PHY Idendifier 2 */
#define GMII_ANAR        0x04   /* Auto_Negotiation Advertisement */
#define GMII_ANLPAR      0x05   /* Auto_negotiation Link Partner Ability */
#define GMII_ANER        0x06   /* Auto-negotiation Expansion */
#define GMII_ANNPR       0x07   /* Auto-negotiation Next Page */
#define GMII_ANLPNPAR    0x08   /* Link Partner Next Page Ability */
/* 0x09 - 0x10: reserved */
#define GMII_AFECR1      0x11   /* AFE Control 1 */
/* 0x12 - 0x14: reserved */
#define GMII_RXERCR      0x15   /* RXER Counter */
#define GMII_OMSOR       0x16   /* Operation Mode Strap Override */
#define GMII_OMSSR       0x17   /* Operation Mode Strap Status */
#define GMII_ECR         0x18   /* Expanded Control */
/* 0x19 - 0x1A: reserved */
#define GMII_ICSR        0x1B   /* Interrupt Control/Status */
/* 0x1C: reserved */
#define GMII_LCSR        0x1D   /* LinkMD Control/Status */
#define GMII_PCR1        0x1E   /* PHY Control 1 */
#define GMII_PCR2        0x1F   /* PHY Control 2 */


/* Bit definitions: GMII_BMCR 0x00 Basic Control */
#define GMII_RESET             (1 << 15) /* 1= Software Reset; 0=Normal Operation */
#define GMII_LOOPBACK          (1 << 14) /* 1=loopback Enabled; 0=Normal Operation */
#define GMII_SPEED_SELECT      (1 << 13) /* 1=100Mbps; 0=10Mbps */
#define GMII_AUTONEG           (1 << 12) /* Auto-negotiation Enable */
#define GMII_POWER_DOWN        (1 << 11) /* 1=Power down 0=Normal operation */
#define GMII_ISOLATE           (1 << 10) /* 1 = Isolates 0 = Normal operation */
#define GMII_RESTART_AUTONEG   (1 << 9)  /* 1 = Restart auto-negotiation */
#define GMII_DUPLEX_MODE       (1 << 8)  /* 1 = Full duplex operation */
#define GMII_COLLISION_TEST    (1 << 7)  /* 1 = Enable COL test; 0 = Disable COL test */
/*      Reserved                6 to 0      Read as 0, ignore on write */

/* Bit definitions: GMII_BMSR 0x01 Basic Status */
#define GMII_100BASE_T4        (1 << 15) /* 100BASE-T4 Capable */
#define GMII_100BASE_TX_FD     (1 << 14) /* 100BASE-TX Full Duplex Capable */
#define GMII_100BASE_T4_HD     (1 << 13) /* 100BASE-TX Half Duplex Capable */
#define GMII_10BASE_T_FD       (1 << 12) /* 10BASE-T Full Duplex Capable */
#define GMII_10BASE_T_HD       (1 << 11) /* 10BASE-T Half Duplex Capable */
/*      Reserved                10 to7     Read as 0, ignore on write */
#define GMII_MF_PREAMB_SUPPR   (1 << 6)  /* MII Frame Preamble Suppression */
#define GMII_AUTONEG_COMP      (1 << 5)  /* Auto-negotiation Complete */
#define GMII_REMOTE_FAULT      (1 << 4)  /* Remote Fault */
#define GMII_AUTONEG_ABILITY   (1 << 3)  /* Auto Configuration Ability */
#define GMII_LINK_STATUS       (1 << 2)  /* Link Status */
#define GMII_JABBER_DETECT     (1 << 1)  /* Jabber Detect */
#define GMII_EXTEND_CAPAB      (1 << 0)  /* Extended Capability */

/* Bit definitions: GMII_PHYID1 0x02 PHY Idendifier 1 */
/* Bit definitions: GMII_PHYID2 0x03 PHY Idendifier 2 */
#define GMII_LSB_MASK           0x3F
#define GMII_OUI_MSB            0x0022
#define GMII_OUI_LSB            0x05

/* Bit definitions: GMII_ANAR   0x04 Auto_Negotiation Advertisement */
/* Bit definitions: GMII_ANLPAR 0x05 Auto_negotiation Link Partner Ability */
#define GMII_NP               (1 << 15) /* Next page Indication */
/*      Reserved               7 */
#define GMII_RF               (1 << 13) /* Remote Fault */
/*      Reserved               12       // Write as 0, ignore on read */
#define GMII_PAUSE_MASK       (3 << 11) /* 0,0 = No Pause 1,0 = Asymmetric
                                         * Pause(link partner) */
                                        /* 0,1 = Symmetric Pause 1,1 =
                                         * Symmetric&Asymmetric Pause(local
                                         * device) */
#define GMII_100T4             (1 << 9)  /* 100BASE-T4 Support */
#define GMII_100TX_FDX         (1 << 8)  /* 100BASE-TX Full Duplex Support */
#define GMII_100TX_HDX         (1 << 7)  /* 100BASE-TX Support */
#define GMII_10_FDX            (1 << 6)  /* 10BASE-T Full Duplex Support */
#define GMII_10_HDX            (1 << 5)  /* 10BASE-T Support */
/*      Selector               4 to 0    Protocol Selection Bits */
#define GMII_AN_IEEE_802_3      0x0001    /* [00001] = IEEE 802.3 */

/* Bit definitions: GMII_ANER 0x06 Auto-negotiation Expansion */
/*      Reserved                15 to 5   Read as 0, ignore on write */
#define GMII_PDF              (1 << 4) /* Local Device Parallel Detection Fault */
#define GMII_LP_NP_ABLE       (1 << 3) /* Link Partner Next Page Able */
#define GMII_NP_ABLE          (1 << 2) /* Local Device Next Page Able */
#define GMII_PAGE_RX          (1 << 1) /* New Page Received */
#define GMII_LP_AN_ABLE       (1 << 0) /* Link Partner Auto-negotiation Able */

/* Bit definitions: GMII_ANNPR       0x07 Auto-negotiation Next Page */
/* Bit definitions: GMII_ANLPNPAR    0x08 Link Partner Next Page Ability */
/* Bit definitions: GMII_ERCR        0x11 AFE Control 1 */
/* Bit definitions: GMII_EMSR        0x15 RXER Counter */
/* Bit definitions: GMII_OMSO        0x16 Operation Mode Strap Override */
/* Bit definitions: GMII_RLLMR       0x17 Operation Mode Strap Status */
/* Bit definitions: GMII_LMDCDR      0x18 Expanded Control */
/* Bit definitions: GMII_ICSR        0x1B Interrupt Control/Status */
/* Bit definitions: GMII_LCSR        0x1D LinkMD Control/Status */
/* Bit definitions: GMII_PCR1        0x1E PHY Control 1 */
/* Bit definitions: GMII_PCR2        0x1F PHY Control 2 */



/* ----------------------------------------------------------------------------- */
/*         Types */
/* ----------------------------------------------------------------------------- */

typedef struct _GMacb {
    /* The retry & timeout settings */
    unsigned int retryMax;

    /* PHY address ( pre-defined by pins on reset ) */
    unsigned char phyAddress;
} GMacb, *pGMacb;

/* ------------------------------------------------------------------------------ */
/*         Exported functions */
/* ------------------------------------------------------------------------------ */


void ksz8051mnl_set_local_loopBack(GMacb *pMacb);
void ksz8051mnl_set_remote_loopBack(GMacb *pMacb);
static unsigned char ksz8051mnl_find_valid_phy(GMacb *pMacb);
void ksz8051mnl_setup_timeout(GMacb *pMacb, unsigned int toMax);
void ksz8051mnl_init(GMacb *pMacb, unsigned char phyAddress);
unsigned char ksz8051mnl_reset_phy(GMacb *pMacb);
unsigned char ksz8051mnl_init_phy(GMacb *pMacb, unsigned int mck);
unsigned char ksz8051mnl_auto_negotiate(GMacb *pMacb);
uint8_t ksz8051mnl_set_link(GMacb *pMacb, uint8_t uc_phy_addr,
        uint8_t uc_apply_setting_flag);

#endif /* #ifndef _MII_DEFINE_H */

