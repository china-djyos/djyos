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

// �ļ���     ��ksz8061.c
// ģ������: ksz8061 PHY�������򣬵�����GMAC������MDIO�ӿ�
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 11/12.2015
// =============================================================================
#if 0
#include "stdint.h"
#include "stddef.h"
#include "ksz8061.h"
#include "cpu_peri.h"
#include "string.h"

#ifdef DEBUG
#define PHY_TRACE  printk
#else
#define PHY_TRACE
#endif


/** Default max retry count */
#define GMACB_RETRY_MAX            300000

/** Default max retry count */
//#define GACB_RETRY_MAX            1000000

/*---------------------------------------------------------------------------
 *         Local functions
 *---------------------------------------------------------------------------*/


/**
 * Wait PHY operation complete.
 * Return 1 if the operation completed successfully.
 * May be need to re-implemented to reduce CPU load.
 * \param retry: the retry times, 0 to wait forever until complete.
 */
static uint8_t GMACB_WaitPhy( Gmac *pHw, uint32_t retry )
{
    volatile uint32_t retry_count = 0;

    while (!GMAC_IsIdle(pHw))
    {
        if(retry == 0)
            continue;
        retry_count ++;
        if (retry_count >= retry)
        {
            return 0;
        }
    }
    return 1;
}

/**
 * Read PHY register.
 * Return 1 if successfully, 0 if timeout.
 * \param pHw HW controller address
 * \param PhyAddress PHY Address
 * \param Address Register Address
 * \param pValue Pointer to a 32 bit location to store read data
 * \param retry The retry times, 0 to wait forever until complete.
 */
static uint8_t GMACB_ReadPhy(Gmac *pHw,
        uint8_t PhyAddress,
        uint8_t Address,
        uint32_t *pValue,
        uint32_t retry)
{
    GMAC_PHYMaintain(pHw, PhyAddress, Address, 1, 0);
    if ( GMACB_WaitPhy(pHw, retry) == 0 )
    {
        PHY_TRACE("TimeOut GMACB_ReadPhy\n\r");
        return 0;
    }
    *pValue = GMAC_PHYData(pHw);
    return 1;
}

/**
 * Write PHY register
 * Return 1 if successfully, 0 if timeout.
 * \param pHw HW controller address
 * \param PhyAddress PHY Address
 * \param Address Register Address
 * \param Value Data to write ( Actually 16 bit data )
 * \param retry The retry times, 0 to wait forever until complete.
 */
static uint8_t GMACB_WritePhy(Gmac *pHw,
        uint8_t PhyAddress,
        uint8_t Address,
        uint32_t Value,
        uint32_t retry)
{
    GMAC_PHYMaintain(pHw, PhyAddress, Address, 0, Value);
    if ( GMACB_WaitPhy(pHw, retry) == 0 )
    {
        PHY_TRACE("TimeOut GMACB_WritePhy\n\r");
        return 0;
    }
    return 1;
}

/*---------------------------------------------------------------------------
 *         Exported functions
 *---------------------------------------------------------------------------*/

/**
 * \brief Find a valid PHY Address ( from 0 to 31 ).
 * \param pMacb Pointer to the MACB instance
 * \return 0xFF when no valid PHY Address found.
 */
static uint8_t GMACB_FindValidPhy(GMacb *pMacb)
{
    sGmacd *pDrv = pMacb->pGmacd;
    Gmac *pHw = pDrv->pHw;

    uint32_t  retryMax;
    uint32_t  value=0;
    uint8_t rc;
    uint8_t phyAddress;
    uint8_t cnt;

    PHY_TRACE("GMACB_FindValidPhy\n\r");

    GMAC_EnableMdio(pHw);
    phyAddress = pMacb->phyAddress;
    retryMax = pMacb->retryMax;

    /* Check current phyAddress */
    rc = phyAddress;
    if( GMACB_ReadPhy(pHw, phyAddress, GMII_PHYID1R, &value, retryMax) == 0 )
    {
        PHY_TRACE("GMACB PROBLEM\n\r");
    }
    PHY_TRACE("_PHYID1  : 0x%X, addr: %d\n\r", value, phyAddress);

    /* Find another one */
    if (value != GMII_OUI_MSB)
    {
        rc = 0xFF;
        for(cnt = 0; cnt < 32; cnt ++)
        {
            phyAddress = (phyAddress + 1) & 0x1F;
            if( GMACB_ReadPhy(pHw, phyAddress, GMII_PHYID1R, &value, retryMax)
                        == 0 )
            {
                PHY_TRACE("MACB PROBLEM\n\r");
            }
            PHY_TRACE("_PHYID1  : 0x%X, addr: %d\n\r", value, phyAddress);
            if (value == GMII_OUI_MSB)
            {
                rc = phyAddress;
                break;
            }
        }
    }
    if (rc != 0xFF)
    {
        PHY_TRACE("** Valid PHY Found: %d\n\r", rc);
        GMACB_ReadPhy(pHw, phyAddress, GMII_PHYID1R, &value, retryMax);
        PHY_TRACE("_PHYID1R  : 0x%X, addr: %d\n\r", value, phyAddress);
        GMACB_ReadPhy(pHw, phyAddress, GMII_PHYID2R, &value, retryMax);
        PHY_TRACE("_EMSR  : 0x%X, addr: %d\n\r", value, phyAddress);
    }
    GMAC_DisableMdio(pHw);
    return rc;
}


/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/


/**
 * \brief Dump all the useful registers.
 * \param pMacb          Pointer to the MACB instance
 */
void GMACB_DumpRegisters(GMacb *pMacb)
{
    sGmacd *pDrv = pMacb->pGmacd;
    Gmac *pHw = pDrv->pHw;

    uint8_t phyAddress;
    uint32_t retryMax;
    uint32_t value;

    PHY_TRACE("GMACB_DumpRegisters\n\r");

    GMAC_EnableMdio(pHw);
    phyAddress = pMacb->phyAddress;
    retryMax = pMacb->retryMax;

    PHY_TRACE("GMII MACB @ %d) Registers:\n\r", phyAddress);

    GMACB_ReadPhy(pHw, phyAddress, GMII_BMCR, &value, retryMax);
    PHY_TRACE(" _BMCR     : 0x%X\n\r", (unsigned)value);
    GMACB_ReadPhy(pHw, phyAddress, GMII_BMSR, &value, retryMax);
    PHY_TRACE(" _BMSR     : 0x%X\n\r", (unsigned)value);
    GMACB_ReadPhy(pHw, phyAddress, GMII_PHYID1R, &value, retryMax);
    PHY_TRACE(" _PHYID1     : 0x%X\n\r", (unsigned)value);
    GMACB_ReadPhy(pHw, phyAddress, GMII_PHYID2R, &value, retryMax);
    PHY_TRACE(" _PHYID2     : 0x%X\n\r", (unsigned)value);
    GMACB_ReadPhy(pHw, phyAddress, GMII_ANAR, &value, retryMax);
    PHY_TRACE(" _ANAR     : 0x%X\n\r", (unsigned)value);
    GMACB_ReadPhy(pHw, phyAddress, GMII_ANLPAR, &value, retryMax);
    PHY_TRACE(" _ANLPAR   : 0x%X\n\r", (unsigned)value);
    GMACB_ReadPhy(pHw, phyAddress, GMII_ANER, &value, retryMax);
    PHY_TRACE(" _ANER     : 0x%X\n\r", (unsigned)value);
    GMACB_ReadPhy(pHw, phyAddress, GMII_ANNPR, &value, retryMax);
    PHY_TRACE(" _ANNPR    : 0x%X\n\r", (unsigned)value);
    GMACB_ReadPhy(pHw, phyAddress, GMII_ANLPNPAR, &value, retryMax);
    PHY_TRACE(" _ANLPNPAR : 0x%X\n\r", (unsigned)value);

    PHY_TRACE(" \n\r");

    GMACB_ReadPhy(pHw, phyAddress, GMII_RXERCR, &value, retryMax);
    PHY_TRACE(" _RXERCR   : 0x%X\n\r", (unsigned)value);
    GMACB_ReadPhy(pHw, phyAddress, GMII_ICSR, &value, retryMax);
    PHY_TRACE(" _ICSR     : 0x%X\n\r", (unsigned)value);
    PHY_TRACE(" \n\r");

    GMAC_DisableMdio(pHw);
}

/**
 * \brief Setup the maximum timeout count of the driver.
 * \param pMacb Pointer to the MACB instance
 * \param toMax Timeout maximum count.
 */
void GMACB_SetupTimeout(GMacb *pMacb, uint32_t toMax)
{
    pMacb->retryMax = toMax;
}

/**
 * \brief Initialize the MACB instance.
 * \param pMacb Pointer to the MACB instance
 * \param phyAddress   The PHY address used to access the PHY
 */
void GMACB_Init(GMacb *pMacb, sGmacd *pGmacd, uint8_t phyAddress)
{
    pMacb->pGmacd = pGmacd;
    pMacb->phyAddress = phyAddress;
    /* Initialize timeout by default */
    pMacb->retryMax = GMACB_RETRY_MAX;
}


/**
 * \brief Issue a SW reset to reset all registers of the PHY.
 * \param pMacb Pointer to the MACB instance
 * \return 1 if successfully, 0 if timeout.
 */
uint8_t GMACB_ResetPhy(GMacb *pMacb)
{
    sGmacd *pDrv = pMacb->pGmacd;
    Gmac *pHw = pDrv->pHw;
    uint32_t retryMax;
    uint32_t bmcr = GMII_RESET,icsr;
    uint8_t phyAddress;
    uint32_t timeout = 10;
    uint8_t ret = 1;

    PHY_TRACE("GMACB_ResetPhy\n\r");

    phyAddress = pMacb->phyAddress;
    retryMax = pMacb->retryMax;

    GMAC_EnableMdio(pHw);
    bmcr = GMII_RESET;
    GMACB_WritePhy(pHw, phyAddress, GMII_BMCR, bmcr, retryMax);

    do {
        GMACB_ReadPhy(pHw, phyAddress, GMII_BMCR, &bmcr, retryMax);
        timeout--;
    } while ((bmcr & GMII_RESET) && timeout);

    //����phy link up �ж�
    icsr = (1<<8);
    GMACB_WritePhy(pHw, phyAddress, GMII_ICSR, icsr, retryMax);

    GMAC_DisableMdio(pHw);

    if (!timeout)
    {
        ret = 0;
    }

    return( ret );
}
/**
 * \brief Do a HW initialize to the PHY ( via RSTC ) and set up clocks & PIOs
 * This should be called only once to initialize the PHY pre-settings.
 * The PHY address is reset status of CRS,RXD[3:0] (the emacPins' pullups).
 * The COL pin is used to select MII mode on reset (pulled up for Reduced MII)
 * The RXDV pin is used to select test mode on reset (pulled up for test mode)
 * The above pins should be predefined for corresponding settings in resetPins
 * The GMAC peripheral pins are configured after the reset done.
 * \param pMacb Pointer to the MACB instance
 * \param mck         Main clock setting to initialize clock
 * \param resetPins   Pointer to list of PIOs to configure before HW RESET
 *                       (for PHY power on reset configuration latch)
 * \param nbResetPins Number of PIO items that should be configured
 * \param emacPins    Pointer to list of PIOs for the EMAC interface
 * \param nbEmacPins  Number of PIO items that should be configured
 * \return 1 if RESET OK, 0 if timeout.
 */
uint8_t GMACB_InitPhy(GMacb *pMacb, uint32_t mck)
{
    sGmacd *pDrv = pMacb->pGmacd;
    Gmac *pHw = pDrv->pHw;
    uint8_t rc = 1;
    uint8_t phy;

    /* Configure GMAC runtime pins */
    if (rc)
    {
        rc = GMAC_SetMdcClock(pHw, mck );
        if (!rc)
        {
            PHY_TRACE("No Valid MDC clock\n\r");
            return 0;
        }

        /* Check PHY Address */
        phy = GMACB_FindValidPhy(pMacb);
        if (phy == 0xFF)
        {
            PHY_TRACE("PHY Access fail\n\r");
            return 0;
        }
        if(phy != pMacb->phyAddress)
        {
            pMacb->phyAddress = phy;
            GMACB_ResetPhy(pMacb);
        }

    }
    else
    {
        PHY_TRACE("PHY Reset Timeout\n\r");
    }
    return rc;
}

/**
 * \brief Issue a Auto Negotiation of the PHY
 * \param pMacb Pointer to the MACB instance
 * \return 1 if successfully, 0 if timeout.
 */
uint8_t GMACB_AutoNegotiate(GMacb *pMacb)
{
    sGmacd *pDrv = pMacb->pGmacd;
    Gmac *pHw = pDrv->pHw;
    uint32_t retryMax;
    uint32_t value;
    uint32_t phyAnar;
    uint32_t phyAnalpar;
    uint32_t retryCount= 0;
    uint8_t phyAddress;
    uint8_t rc = 1;
    uint32_t duplex, speed;
    phyAddress = pMacb->phyAddress;
    retryMax = pMacb->retryMax;

    GMAC_EnableMdio(pHw);

    if (!GMACB_ReadPhy(pHw,phyAddress, GMII_PHYID1R, &value, retryMax))
    {
        PHY_TRACE("Pb GEMAC_ReadPhy Id1\n\r");
        rc = 0;
        goto AutoNegotiateExit;
    }
    PHY_TRACE("ReadPhy Id1 0x%X, address: %d\n\r", value, phyAddress);
    if (!GMACB_ReadPhy(pHw,phyAddress, GMII_PHYID2R, &phyAnar, retryMax))
    {
        PHY_TRACE("Pb GMACB_ReadPhy Id2\n\r");
        rc = 0;
        goto AutoNegotiateExit;
    }
    PHY_TRACE("ReadPhy Id2 0x%X\n\r", phyAnar);

    if( ( value == GMII_OUI_MSB )
            && ( ((phyAnar)&(~GMII_LSB_MASK)) == GMII_OUI_LSB ) )
    {
        PHY_TRACE("Vendor Number Model = 0x%X\n\r", ((phyAnar>>4)&0x3F));
        PHY_TRACE("Model Revision Number = 0x%X\n\r", (phyAnar&0xF));
    } else {
        PHY_TRACE("Problem OUI value\n\r");
    }

    /** Set the Auto_negotiation Advertisement Register, MII advertising for
    Next page 100BaseTxFD and HD, 10BaseTFD and HD, IEEE 802.3 */
    rc  = GMACB_ReadPhy(pHw, phyAddress, GMII_ANAR, &phyAnar, retryMax);
    if (rc == 0) {
        goto AutoNegotiateExit;
    }
    phyAnar = GMII_TX_FDX | GMII_TX_HDX |
        GMII_10_FDX | GMII_10_HDX | GMII_AN_IEEE_802_3;
    rc = GMACB_WritePhy(pHw,phyAddress, GMII_ANAR, phyAnar, retryMax);
    if (rc == 0)
    {
        goto AutoNegotiateExit;
    }

    /* Read & modify control register */
    rc  = GMACB_ReadPhy(pHw, phyAddress, GMII_BMCR, &value, retryMax);
    if (rc == 0)
    {
        goto AutoNegotiateExit;
    }

    /* Check AutoNegotiate complete */
    value |=  GMII_AUTONEG | GMII_RESTART_AUTONEG;
    rc = GMACB_WritePhy(pHw, phyAddress, GMII_BMCR, value, retryMax);
    if (rc == 0)
    {
        goto AutoNegotiateExit;
    }
    PHY_TRACE(" _BMCR: 0x%X\n\r", value);

    // Check AutoNegotiate complete
    while (1)
    {
        rc  = GMACB_ReadPhy(pHw, phyAddress, GMII_BMSR, &value, retryMax);
        if (rc == 0)
        {
            PHY_TRACE("rc==0\n\r");
            goto AutoNegotiateExit;
        }
        /* Done successfully */
        if (value & GMII_AUTONEG_COMP)
        {
            PHY_TRACE("AutoNegotiate complete\n\r");
            break;
        }
        /* Timeout check */
        if (retryMax)
        {
            if (++ retryCount >= retryMax)
            {
                GMACB_DumpRegisters(pMacb);
                PHY_TRACE("TimeOut\n\r");
                rc = 0;
                goto AutoNegotiateExit;
            }
        }
    }

    /*Set local link mode */
    while(1)
    {
        rc  = GMACB_ReadPhy(pHw, phyAddress, GMII_ANLPAR, &phyAnalpar, retryMax);
        if (rc == 0)
        {
            goto AutoNegotiateExit;
        }
        /* Set up the GMAC link speed */
        if ((phyAnar & phyAnalpar) & GMII_TX_FDX)
        {
            /* set RGMII for 1000BaseTX and Full Duplex */
            duplex = GMAC_DUPLEX_FULL;
            speed = GMAC_SPEED_100M;
            break;
        } else if ((phyAnar & phyAnalpar) & GMII_10_FDX)
        {
            /* set RGMII for 1000BaseT and Half Duplex*/
            duplex = GMAC_DUPLEX_FULL;
            speed = GMAC_SPEED_10M;
            break;
        } else if ((phyAnar & phyAnalpar) & GMII_TX_HDX)
        {
            /* set RGMII for 100BaseTX and half Duplex */
            duplex = GMAC_DUPLEX_HALF;
            speed = GMAC_SPEED_100M;
            break;
        } else if ((phyAnar & phyAnalpar) & GMII_10_HDX)
        {
            // set RGMII for 10BaseT and half Duplex
            duplex = GMAC_DUPLEX_HALF;
            speed = GMAC_SPEED_10M;
            break;
        }
    }
    PHY_TRACE("GMAC_EnableRGMII duplex %u, speed %u\n\r",(unsigned)duplex,(unsigned)speed);

    GMACB_ReadPhy(pHw,phyAddress, GMII_PC1R, &value, retryMax);
    GMACB_ReadPhy(pHw,phyAddress, GMII_PC2R, &value, retryMax);
    GMACB_ReadPhy(pHw,phyAddress, GMII_ICSR, &value, retryMax);
    /* Set up GMAC mode  */
    GMAC_EnableRGMII(pHw, duplex, speed);

AutoNegotiateExit:
    GMAC_DisableMdio(pHw);
    return rc;
}

// =============================================================================
// ���ܣ�
// ������
// ���أ�
// =============================================================================
bool_t GMACB_PhyIsLinkUp(GMacb *pMacb)
{
    u32 icsr ;

    GMAC_EnableMdio(pMacb->pGmacd->pHw);

    GMACB_ReadPhy(pMacb->pGmacd->pHw,
            pMacb->phyAddress,
            GMII_ICSR,
            &icsr,
            pMacb->retryMax);

    GMAC_DisableMdio(pMacb->pGmacd->pHw);

    if( ( icsr & (1<<8) )  &&  (icsr & (1<<0)) )
    {
        return true;
    }
    return false;
}

#endif













