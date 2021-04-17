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

// �ļ���     ��cpu_peri_gmac.c
// ģ������: GMACģ��ĳ�ʼ���Ĳ���������PHY��ʼ����MII��ʼ��
//           ��DJYOS����̫��Э��ʵ��Ҫ��ʵ�ֵ������ײ�����
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 25/11.2015
// =============================================================================
#if 0 // ǰ�������޸�����̫��Э�飬�����������δ���ü��޸�

#include "cpu_peri.h"
#include "stdio.h"
#include "string.h"
#include <tcpip/netdev.h>
#include <tcpip/rout.h>
#include <tcpip/tcpip_cdef.h>
#include "ksz8051mnl.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_GMAC(ptu32_t para);
//   ModuleInstall_GMAC(0);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip GMAC"//CPU��mac��������
//parent:"tcpip"     //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                                                                  //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                                                               //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                                                                 //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium                                                               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                                                                 //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"int","tcpip","heap","lock","AtmelSam4E","cpu drver Power management"//������������������������none����ʾ�������������
                                                                                 //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                                                                 //����������������������г����á�,���ָ�
//weakdependence:"none"                                                          //��������������������������none����ʾ�������������
                                                                                 //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                                                                 //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                                                                 //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_GMAC == false )
//#warning  " cpu_onchip_GMAC  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header              //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_GMAC    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

// =============================================================================
#if(DEBUG == 1)
#define GMAC_DBG   printk
#else
#define GMAC_DBG(...)
#endif

#define RAW_GMAC

#define enableCAF GMAC_CAF_ENABLE
#define enableNBC GMAC_NBC_DISABLE          //�����ι㲥����
// =============================================================================
#define CN_HARD_BOARD            0x03
static u8  sgNetHardMac[6] = {0x00,0x01, 0x02, 0x03, 0x04, CN_HARD_BOARD};
static u32  sgNetHardIpAddrMain = 0xC0A80100;
static u32  sgNetHardIpMsk = 0xFFFFFF00 ; //255.255.255.0
static u32  sgNetHardGateWay = 0xC0A80101; //192.168.1.1
static struct tagSemaphoreLCB GMAC_RcvSemp;

#define CN_PKG_MAX_LEN  1500
static tagRoutTab  *ptNetRout = NULL;
static tagNetDev   *ptNetDev = NULL;

// =============================================================================
// ��ָ���ʼ������8�ֽڶ����BD��
#ifdef __GNUC__
static volatile GRxTd GrxTd __attribute__((aligned(8)));
static volatile GTxTd GtxTd __attribute__((aligned(8)));
static volatile u8 RxBuffer[GRX_BUFFERS * GMAC_RX_UNITSIZE] __attribute__((aligned(8)));
static volatile u8 TxBuffer[GMAC_TX_UNITSIZE] __attribute__((aligned(8)));
#else
#error "bd must be 8-byte aligned"
#endif

#define FRAME_START_BUF     (1<<0)          //�װ����
#define FRAME_END_BUF       (1<<1)          //β�����

#define  SendPkgWaitMs  1000

#define NEW_PRO 0
/* Return count up to the end of the buffer. */
/* Carefully avoid accessing head and tail more than once, */
/* so they can change underneath us without returning inconsistent results */
#define GCIRC_CNT_TO_END(head, tail, size) \
    ({int end = (size) - (tail); \
      int n = ((head) + end) & ((size) - 1); \
      n < end ? n : end; } \
    )

/* Return space available up to the end of the buffer */
#define GCIRC_SPACE_TO_END(head, tail, size) \
    ({int end = (size) - 1 - (head); \
      int n = (end + (tail)) & ((size) - 1); \
      n <= end ? n : end + 1; } \
    )

/* Increment head or tail */
#define GCIRC_INC(headortail, size) \
    headortail++; \
    if (headortail >= size) { \
        headortail = 0; \
    }

#define GCIRC_EMPTY(circ)     ((circ)->head == (circ)->tail)
#define GCIRC_CLEAR(circ)     ((circ)->head = (circ)->tail = 0)
/* Return count in buffer */
#define GCIRC_CNT(head, tail, size) (((head) - (tail)) & ((size) - 1))

/* Return space available, 0..size-1 */
/* We always leave one free char as a completely full buffer */
/* has head == tail, which is the same as empty */
#define GCIRC_SPACE(head, tail, size) GCIRC_CNT((tail), ((head) + 1), (size))

static GMacb gMacb;

static void gmac_disable_txrx(void)
{
    GMAC->GMAC_NCR &=  ~(GMAC_NCR_RXEN | GMAC_NCR_TXEN);
}

/**
 * \brief Gmac, enable receiver
 */
static void gmac_enable_txrx(void)
{
    GMAC->GMAC_NCR |=  (GMAC_NCR_RXEN | GMAC_NCR_TXEN);
}

// =============================================================================
// ���ܣ������ȴ�PHY
// ������retry�����Դ���
// ���أ�1���ɹ���0ʧ��
// =============================================================================
static u8 __GMAC_WaitPhy( u32 retry )
{
    volatile uint32_t ul_retry_count = 0;

    while ((GMAC->GMAC_NSR & GMAC_NSR_IDLE) == 0)
    {
        /* Dead LOOP! */
        if (retry == 0)
        {
            continue;
        }

        /* Timeout check */
        ul_retry_count++;
        if (ul_retry_count >= retry)
        {
            GMAC_DBG("Error: Wait PHY time out\n\r");
            return 0;
        }
    }

    return 1;
}

// =============================================================================
// ���ܣ�MII�ӿ�д������ͨ��MDIO��PHYоƬͨ��
// ������phy_addr, PHY��ַ
//       reg_addr,�Ĵ�����ַ
//       data,д�������
// ���أ�1,�������أ�0,ʧ��
// =============================================================================
u32 GMAC_WritePhy(u32 PhyAddress, u32 Address, u32 Value,u32 retry)
{
    GMAC->GMAC_MAN = GMAC_MAN_WTN(GMAC_MAN_CODE_VALUE)
            | GMAC_MAN_CLTTO
            | GMAC_MAN_PHYA(PhyAddress)
            | GMAC_MAN_REGA(Address)
            | GMAC_MAN_OP((0 ? GMAC_MAN_RW_TYPE : GMAC_MAN_READ_ONLY))
            | GMAC_MAN_DATA(Value);

    if (__GMAC_WaitPhy(retry) == 0)
    {
        GMAC_DBG("TimeOut GMAC_WritePhy\n\r");
        return 0;
    }

    return 1;
}

// =============================================================================
// ���ܣ�MII�ӿڶ����ݣ�ͨ��MDIO�ӿ���PHYоƬͨ��
// ������PhyAddress, PHY��ַ
//       Address,�Ĵ�����ַ
//       pValue,��ȡ��������ָ��
//       retry,ʧ�ܳ��ԵĴ���
// ���أ�1,�������أ�0,ʧ��
// =============================================================================
u32 GMAC_ReadPhy(u32 PhyAddress, u32 Address, u32 *pValue,u32 retry)
{
    /* PHY Maintenance Register */
    GMAC->GMAC_MAN = ((~GMAC_MAN_WZO) & GMAC_MAN_CLTTO )
                            | GMAC_MAN_OP(0x02)        /* read */
                            | GMAC_MAN_WTN(0x02)       /* must be */
                            | GMAC_MAN_PHYA(PhyAddress)
                            | GMAC_MAN_REGA(Address);

    if (__GMAC_WaitPhy(retry) == 0)
    {
        GMAC_DBG("TimeOut GMAC_ReadPhy\n\r");
        return 0;
    }

    *pValue = (GMAC->GMAC_MAN & 0x0000ffff);
    return 1;
}

// =============================================================================
// ���ܣ�MDIO�������ߵ�ʹ�ܺͽ���
// ��������
// ���أ���
// =============================================================================
void GMAC_MdioEnable(void)
{
    gmac_disable_txrx();
    GMAC->GMAC_NCR |= GMAC_NCR_MPE;
    gmac_enable_txrx();
}

void GMAC_MdioDisable(void)
{
    gmac_disable_txrx();
    GMAC->GMAC_NCR &= ~GMAC_NCR_MPE;
    gmac_enable_txrx();
}
void GMAC_EnableMii(void)
{
    gmac_disable_txrx();
    GMAC->GMAC_UR  |= GMAC_UR_RMIIMII;
    gmac_enable_txrx();
}

// =============================================================================
// ���ܣ�����GMAC�������ٶ�:speed 100M/10M and Full/Half duplex
// ������speed, 0 for 10M, 1 for 100M, 2 for 1000M
//      fullduplex ,1 for Full Duplex mode
// ���أ�CRC����ϣ�㷨�����CRCֵ��8����ֵ
// =============================================================================
void GMAC_SetLinkSpeed(u8 speed, u8 fullduplex)
{
    unsigned int ncfgr;
    gmac_disable_txrx();

    ncfgr = GMAC->GMAC_NCFGR;
    ncfgr &= ~(GMAC_NCFGR_SPD | GMAC_NCFGR_FD);
    if (speed) {
        ncfgr |= GMAC_NCFGR_SPD;
    }

    if (fullduplex) {
        ncfgr |= GMAC_NCFGR_FD;
    }

    GMAC->GMAC_NCFGR = ncfgr;

    GMAC->GMAC_NCR |=  (GMAC_NCR_RXEN | GMAC_NCR_TXEN);     // Enable Tx Rx
}

// =============================================================================
// ���ܣ�MAC��ַ���ú͵�ַhash�����ã�ȡ����Ĺ�ϣCRC��ߵ�6����
// ������pa,MAC��ַ����ָ��
// ���أ���
// =============================================================================
static void __GMAC_AddrSet (u8 *pa)
{
    GMAC->GMAC_SA[0].GMAC_SAB = ( (pa[3] << 24) | (pa[2] << 16)
                                    | (pa[1] << 8)  |  pa[0] );

    GMAC->GMAC_SA[0].GMAC_SAT = ( (pa[5] << 8) | pa[4] );
}

// =============================================================================
// ���ܣ�Buffer Descriptor��ʼ�������ڷ��ͺͽ������ݣ������ֲᣬBD����밴8�ֽڶ���
//       ����dataָ����ָ��ĵ�ַҲ�����ǰ�8�ֽڶ���
// ��������
// ���أ���
// =============================================================================
static void __GMAC_RxBD_Init(void)
{
    unsigned int Index;
    unsigned int Address;

    /* Disable RX */
    GMAC->GMAC_NCR &= ~GMAC_NCR_RXEN;
    /* Setup the RX descriptors. */
    GrxTd.idx = 0;
    for (Index = 0; Index < GRX_BUFFERS; Index++)
    {
        Address = (unsigned int)(&(RxBuffer[Index * GMAC_RX_UNITSIZE]));
        /* Remove GMAC_RX_OWNERSHIP_BIT and GMAC_RX_WRAP_BIT */
        GrxTd.td[Index].addr.val = Address & 0xFFFFFFFC;
        GrxTd.td[Index].status.val = 0;
    }
    GrxTd.td[GRX_BUFFERS - 1].addr.bm.b_wrap = 1;
    /* Receive Buffer Queue Pointer Register */
    GMAC->GMAC_RBQB = (unsigned int)(GrxTd.td);
}

static void __GMAC_TxBD_Init(void)
{
    unsigned int Index;
    unsigned int Address;

    /* Disable TX */
    GMAC->GMAC_NCR &= ~GMAC_NCR_TXEN;
    /* Setup the TX descriptors. */
    GCIRC_CLEAR(&GtxTd);
    for (Index = 0; Index < GTX_BUFFERS; Index++)
    {
        Address = 0;                                //�ȷ��͵�ʱ��������
        GtxTd.td[Index].addr = Address;
        GtxTd.td[Index].status.bm.b_used = 1;
    }
    GtxTd.td[GTX_BUFFERS - 1].status.bm.b_wrap = 1;
    GtxTd.td[GTX_BUFFERS - 1].status.bm.b_used = 1;
    /* Transmit Buffer Queue Pointer Register */
    GMAC->GMAC_TBQB = (unsigned int)(GtxTd.td);
}

// =============================================================================
// ���ܣ�GMAC��PORT�ڳ�ʼ������Ҫ�Ƕ�ʱ�ӵ�ʹ�ܣ�GPIO���ã�����RMII��MII
//       �ֱ����ò�ͬ��IO���Ź���
// ��������
// ���أ���
// =============================================================================
static void __GMAC_PortInit(void)
{
    extern void Board_PhyIoInit(void);
    Board_PhyIoInit();
}

// =============================================================================
// ���ܣ�GMAC�жϳ�ʼ����ʹ���жϵķ�ʽ�������ݣ����������жϼĴ���
// ��������
// ���أ���
// =============================================================================
static void __GMAC_IntInit(void)
{
    u32 GMAC_IntHandler(ufast_t IntLine);
    Int_Register(CN_INT_LINE_GMAC);
    Int_IsrConnect(CN_INT_LINE_GMAC,GMAC_IntHandler);
    Int_SettoAsynSignal(CN_INT_LINE_GMAC);
    Int_ClearLine(CN_INT_LINE_GMAC);
    Int_ContactLine(CN_INT_LINE_GMAC);
}

// =============================================================================
// ���ܣ�PHYоƬ��ʼ����������ʹ������Ӧ��ʽ��ͨ����ȡPHYоƬ��SPEED��˫������ؼĴ�
//       �������������MAC����ز���
// ��������
// ���أ�true,link�ɹ���false,linkʧ��
// =============================================================================
static bool_t __PHY_Init(void)
{
    /* Init MAC PHY driver */
    ksz8051mnl_init(&gMacb, BOARD_GMAC_PHY_ADDR);

    if (!ksz8051mnl_init_phy(&gMacb, CN_CFG_MCLK))
    {
        GMAC_DBG("PHY Initialize ERROR!\r");
        return false;
    }

    /* Auto Negotiate, work in MII mode */
    if (!ksz8051mnl_auto_negotiate(&gMacb/*, BOARD_GMAC_PHY_ADDR*/))
    {
        GMAC_DBG("Auto Negotiate ERROR!\r");
        return false;
    }

    /* Establish ethernet link */
//  while(GMAC_OK != ksz8051mnl_set_link(&gMacb, BOARD_GMAC_PHY_ADDR, 1))
//  {
//      GMAC_DBG("Set link ERROR!\r");
//  }
    return true;
}

// =============================================================================
// ���ܣ���̫��ģ��MAC��ʼ������Ҫ��MAC�Ĵ�����BD��ĳ�ʼ�����¼������裺
//       1.����ʱ��ʹ�ܣ���ֹ���ͽ��պ��жϣ����־������MII��RMIIģʽ��
//       2.��ʼ��BD���������ͺͽ��գ�����TCR��RCR�������ͽ��չ��ܣ�
//       3.MAC��ַ��ʼ����дMAC���Ĵ��������ù㲥������ַ���˵ȣ�
//       4.�����ж�ʹ�ܣ����ʹ��GMAC��
// ��������
// ���أ���
// =============================================================================
static void __GMAC_Init(void)
{
    // Enable GMAC clock
    PMC_EnablePeripheral(ID_GMAC);

    // Establish ethernet link
    GMAC->GMAC_NCR = 0;             // Disable TX & RX and more
    GMAC->GMAC_IDR = ~0;            // Disable all INT

    // MII MODE
    GMAC_EnableMii();

    // Receiver Buffer List Init
    __GMAC_RxBD_Init();
    // Transmit Buffer List Init
    __GMAC_TxBD_Init();
    // Set MAC Address
    __GMAC_AddrSet((u8*)&sgNetHardMac);

    GMAC->GMAC_NCR = GMAC_NCR_CLRSTAT;

    /* Clear all status bits in the receive status register. */
    GMAC->GMAC_RSR = (GMAC_RSR_RXOVR | GMAC_RSR_REC
            | GMAC_RSR_BNA | GMAC_RSR_HNO);

    /* Clear all status bits in the transmit status register */
    GMAC->GMAC_TSR = (GMAC_TSR_UBR | GMAC_TSR_COL | GMAC_TSR_RLE
            | GMAC_TSR_TXGO | GMAC_TSR_TFC | GMAC_TSR_TXCOMP
            | GMAC_TSR_UND | GMAC_TSR_HRESP );

    /* Clear interrupts */
    GMAC->GMAC_ISR;

    /* Enable the copy of data into the buffers */
    /* ignore broadcasts, and don't copy FCS. */
    GMAC->GMAC_NCFGR |= GMAC_NCFGR_FD; /* Full Duplex */

    /* Enable Non-VLAN FRAMES */
    GMAC->GMAC_NCFGR &= ~(GMAC_NCFGR_DNVLAN);

    if (enableCAF == GMAC_CAF_ENABLE)
    {
        /* When set to logic one, all valid frames will be accepted. */
        GMAC->GMAC_NCFGR |= GMAC_NCFGR_CAF;
    }

    if (enableNBC == GMAC_NBC_ENABLE)
    {
      /* When set to logic one, frames addressed to the broadcast address
       * of all ones will not be accepted. */
        GMAC->GMAC_NCFGR |= GMAC_NCFGR_NBC;
    }

    /* Setup the interrupts for TX (and errors) */
    GMAC->GMAC_IER = GMAC_IER_PDRSFT | GMAC_IER_PDRQFT | GMAC_IER_PDRSFR |
                    GMAC_IER_PDRQFR | GMAC_IER_SFT   | GMAC_IER_DRQFT |
                    GMAC_IER_SFR    | GMAC_IER_DRQFR | GMAC_IER_EXINT |
                    GMAC_IER_PFTR   | GMAC_IER_PTZ   | GMAC_IER_PFNZ  |
                    GMAC_IER_HRESP  | GMAC_IER_ROVR  | GMAC_IER_TCOMP |
                    GMAC_IER_TFC    | GMAC_IER_RLEX  | GMAC_IER_TUR   |
                    GMAC_IER_TXUBR  | GMAC_IER_RXUBR | GMAC_IER_RCOMP |
                    GMAC_IER_MFS;

    /* Enable Rx and Tx, plus the stats register. */
    GMAC->GMAC_NCR |= (GMAC_NCR_TXEN | GMAC_NCR_RXEN);
}

// =============================================================================
// ���ܣ�GMAC��̫����MAC�㷢���жϷ����������������������ݡ��������������ݺͷ��������
// ������int_line,�ж��ߺ�
// ���أ�0
// =============================================================================
u32 GMAC_IntHandler(ufast_t IntLine)
{
    u32 isr,rsr,tsr,rxStatusFlag,txStatusFlag;
//  volatile gmac_tx_descriptor_t *pTxTd;

    isr = GMAC->GMAC_ISR;
    rsr = GMAC->GMAC_RSR;
    tsr = GMAC->GMAC_TSR;
    isr &= ~(GMAC->GMAC_IMR | 0xF8030300);

    if((isr & GMAC_ISR_RCOMP) || (rsr & GMAC_RSR_REC))
    {
        rxStatusFlag = GMAC_RSR_REC;

        /* Check OVR */
        if (rsr & GMAC_RSR_RXOVR)
        {
            rxStatusFlag |= GMAC_RSR_RXOVR;
//          GmacStatistics.rx_ovrs++;
        }

        /* Check BNA */
        if (rsr & GMAC_RSR_BNA)
        {
            rxStatusFlag |= GMAC_RSR_BNA;
//          GmacStatistics.rx_bnas++;
        }

        /* Check HNO */
        if (rsr & GMAC_RSR_HNO)
        {
            rxStatusFlag |= GMAC_RSR_HNO;
//          GmacStatistics.rx_ovrs++;
        }
        /* Clear status */
        GMAC->GMAC_RSR |= rxStatusFlag;
        Lock_SempPost(&GMAC_RcvSemp);
    }

    /* TX packet */
    if ((isr & GMAC_ISR_TCOMP) || (tsr & GMAC_TSR_TXCOMP))
    {
        txStatusFlag = GMAC_TSR_TXCOMP;
//      GmacStatistics.tx_comp++;

        /* A frame transmitted */
        /* Check RLE = Retry limit exceeded */
        if (tsr & GMAC_TSR_RLE)
        {
            /* Status RLE & Number of discarded buffers */
            txStatusFlag = GMAC_TSR_RLE
                    | GCIRC_CNT(GtxTd.head, GtxTd.tail,GTX_BUFFERS);
            __GMAC_TxBD_Init();
            GMAC_DBG("Tx RLE!!\n\r");
            GMAC->GMAC_NCR |= GMAC_NCR_TXEN;
//          GmacStatistics.tx_errors++;
        }

        /* Check COL */
        if (tsr & GMAC_TSR_COL)
        {
            txStatusFlag |= GMAC_TSR_COL;
//          GmacStatistics.collisions++;
        }

        /* Check TFC */
        if (tsr & GMAC_TSR_TFC)
        {
            txStatusFlag |= GMAC_TSR_TFC;
//          GmacStatistics.tx_exausts++;
        }
        /* Check UND */
        if (tsr & GMAC_TSR_UND)
        {
            txStatusFlag |= GMAC_TSR_UND;
//          GmacStatistics.tx_underruns++;
        }

        /* Check HRESP */
        if (tsr & GMAC_TSR_HRESP)
        {
            txStatusFlag |= GMAC_TSR_HRESP;
//          GmacStatistics.tx_hresp++;
        }

        if (tsr & GMAC_TSR_RLE)
        {

        }
    }

    // Error Handler
//  if()
//  {
//
//  }

    return 0;
}

// =============================================================================
// ���ܣ���λPHY�������������Ȳ������ʱ����ӦPHY��������������������PHY LINKUP�ж�ʱ����
// ��������
// ���أ���
// =============================================================================
void GMAC_PhyIntHandler(void)
{
    if(GMACB_PhyIsLinkUp(&gMacb))
    {
        __GMAC_Init();

        /* Establish ethernet link */
        while(GMAC_OK != ksz8051mnl_set_link(&gMacb, BOARD_GMAC_PHY_ADDR, 1))
        {
            GMAC_DBG("Set link ERROR!\r");
        }
    }
}

#ifndef RAW_GMAC
// =============================================================================
// ���ܣ���̫��ģ��������ݰ������ݰ����ֽ�������������Ϊ���¼������裺
//       1.�����MAC�Ƿ�����������д�����ݣ����Ƿ���b_ownership == 1��
//       2.��ѯ��������֡���װ����ڻ���BD��λ�ã���status.bm.b_sof == 1��
//       3.��ѯ��������λ��status.bm.b_eof����������������֡�����ݳ��ȣ�
//       4.����pkg�ռ䣬����С������GMAC_TX_UNITSIZE
//       5.��������������д��pkg�������b_ownership�����־
// ������packet,���յ����ݵ��׵�ַ
// ���أ����յ����ݰ����ȣ���󲻻ᳬ��1518�ֽ�
// =============================================================================
static struct NetPkg *GMAC_RcvPacket(tagNetDev *NetCard)
{
    struct NetPkg *pkg=NULL;
    u8 LastBuffer=0;
    u8 *rcvbuf = NULL;
    u8 tmpIdx,i;
    u16 totallen = 0,length = 0;
    u8 IsFrame = false;
    volatile gmac_rx_descriptor_t *pRxTd;

    //Ѱ�ҵ���һ������λ��
    for(i = 0;i < GRX_BUFFERS; i++)
    {
        pRxTd = GrxTd.td + GrxTd.idx;
        if(pRxTd->status.bm.b_sof)
            break;
        pRxTd->addr.bm.b_ownership = 0;
        GCIRC_INC(GrxTd.idx, GRX_BUFFERS);      //�ݴ���
    }

    if(i == GRX_BUFFERS)
    {
        return NULL;
    }

    // ����Ƿ������ݰ�
    tmpIdx = GrxTd.idx;
    pRxTd = GrxTd.td + tmpIdx;
    while(pRxTd->addr.bm.b_ownership)
    {
        if(pRxTd->status.bm.b_sof)                  //�ҵ���ǰ֡���װ�
        {
            while(GrxTd.idx != tmpIdx)                  //��������SOF֮��û��EOF
            {
                GCIRC_INC(GrxTd.idx, GRX_BUFFERS);
                pRxTd->addr.bm.b_ownership = 0;
                pRxTd = GrxTd.td + GrxTd.idx;
            }
            IsFrame = FRAME_START_BUF;
        }

        GCIRC_INC(tmpIdx, GRX_BUFFERS);

        if(IsFrame == FRAME_START_BUF)
        {
            if(tmpIdx == GrxTd.idx)         //δ�ҵ�EOF
            {
                do {
                    pRxTd = GrxTd.td + GrxTd.idx;
                    pRxTd->addr.bm.b_ownership = 0;
                    GCIRC_INC(GrxTd.idx, GRX_BUFFERS);
                } while(tmpIdx != GrxTd.idx);
                break;
            }
            if(pRxTd->status.bm.b_eof)      //�ҵ���һ��������֡
            {
                IsFrame |= (FRAME_END_BUF);
                totallen = pRxTd->status.bm.len;
                break;
            }
        }
        else
        {
            pRxTd->addr.bm.b_ownership = 0;
            GrxTd.idx = tmpIdx;
        }

        pRxTd = GrxTd.td + tmpIdx;
    }

    //���յ�����������֡����������ݿ���
    if( (IsFrame == (FRAME_START_BUF|FRAME_END_BUF)) &&
            (totallen <= CN_PKG_MAX_LEN) && (totallen > 0) )
    {
        pkg =PkgMalloc(totallen,CN_PKLGLST_END);
        if(NULL != pkg)
        {
            rcvbuf = (u8 *)(pkg->buf + pkg->offset);
            pkg->datalen = totallen;
            pkg->partnext= NULL;
        }
        else
        {
            GMAC_DBG("GMAC_RcvPacket() pkg_alloc failed!\r\n ");
        }

        LastBuffer = 0;
        while(!LastBuffer)                             //�Ƿ�Ϊ��Frame�����һ��BD��
        {
            pRxTd = GrxTd.td + GrxTd.idx;
            length = (totallen > GMAC_RX_UNITSIZE) ?
                    GMAC_RX_UNITSIZE : (totallen);

            if( (length > 0) && (NULL != rcvbuf))
            {
               memcpy(rcvbuf,(void *)(pRxTd->addr.val & 0xFFFFFFFC),length);
               rcvbuf = rcvbuf + length;
               totallen -= length;
            }

            LastBuffer = pRxTd->status.bm.b_eof;
            if(pRxTd->status.bm.b_eof)
            {
                pRxTd->status.bm.b_eof = 0;
                pRxTd->status.bm.len = 0;
            }

            if(pRxTd->status.bm.b_sof)
            {
                pRxTd->status.bm.b_sof = 0;
            }
            pRxTd->addr.bm.b_ownership = 0;
            GCIRC_INC(GrxTd.idx, GRX_BUFFERS);
        }
    }

    return pkg;
}

// =============================================================================
// �������ܣ�GMAC_SendPacket,��̫���ײ���������PKG���ݰ����������£�
//          1.���㵱ǰ����֡�ɶ��ٸ�pkg��ɣ�
//          2.���㵱ǰBD Ring�ж��ٿ���BD��
//          3.������BD��Ŀ��֡�����pkg��Ŀ�㹻��������BD�������ͣ�����������ȴ�
//          4.����ָ����ʱ����δ����㹻��bd����ֱ�ӷ���ʧ��
// ���������dev,ʹ�õ������豸
//          pkg,�����͵����ݰ�
// ���������
// ����ֵ  ��true���ͳɹ�  false����ʧ�ܡ�
// =============================================================================
static bool_t GMAC_SendPacket(tagNetDev *dev,struct NetPkg * pkg, u32 netdevtask)
{
    bool_t  result = false;
    struct NetPkg *tmp;
    u32 pkgnum = 0,temp = 0,i;
    u32 SendBdWaitTime = SendPkgWaitMs;
    volatile gmac_tx_descriptor_t *pTxTd;
    u16 totallen=0;
    u8 *SendBuf;

    if((NULL == dev)||(NULL == pkg))
        return result;

    pkgnum = 1;
    //��ѯ�Ƿ����㹻���BD��
    do
    {
        temp = GCIRC_SPACE(GtxTd.head, GtxTd.tail, GTX_BUFFERS);
        if(temp >= pkgnum)
            break;
        DJY_EventDelay(1*mS);
    }while(SendBdWaitTime-- > 0);

    if(SendBdWaitTime == 0)
        return result;
    //����һ���������ݰ��ɶ��ٸ�pkg���
    tmp = pkg;
    SendBuf = (u8*)TxBuffer;
    while(NULL != tmp)
    {
        if(totallen <= GMAC_TX_UNITSIZE)
        {
            memcpy(&SendBuf[totallen],(void*)(tmp->buf+tmp->offset),tmp->datalen);
            totallen += tmp->datalen;
            tmp = tmp->partnext;
        }
    }

    //����һ����˵�����㹻��Ŀ���BD���͸�֡����
    for (i = 0; i < pkgnum; i++)
    {
        /* Pointers to the current TxTd */
        pTxTd = GtxTd.td + GtxTd.head;

        pTxTd->status.bm.b_used = 0;
        pTxTd->status.bm.len = totallen & 0x3FFF;
        pTxTd->addr = (u32)SendBuf;//(u32)(tmp->buf + tmp->offset);
        if(i == pkgnum - 1)
        {
            // ��ǰ֡�����һ��
            pTxTd->status.bm.b_last_buffer = 1;
        }

        GCIRC_INC(GtxTd.head, GTX_BUFFERS);
        tmp = tmp->partnext;
    }

    // Indicate that Descriptors are ready to transmit
    GMAC->GMAC_NCR |= GMAC_NCR_TSTART;

    SendBdWaitTime = SendPkgWaitMs;
    // ��ʱ����������ٷ���
    do
    {
        if((GMAC->GMAC_TSR & GMAC_TSR_TXCOMP) )
        {
            break;
        }
        DJY_EventDelay(1*mS);
    }while(SendBdWaitTime-- > 0);

    pTxTd = GtxTd.td + GtxTd.tail;
    if (pTxTd->status.bm.b_used ==1)    //������ʱ��GMAC�Ὣb_used����Ϊ1
    {
        GCIRC_INC( GtxTd.tail, GTX_BUFFERS );
    }

    return result = true;
}

// =============================================================================
// ���ܣ������¼��ĺ����������ȴ����յ��������ݰ����ź����������ն�ȡ������������������ݰ�
// ��������
// ���أ���
// =============================================================================
static ptu32_t GMAC_Rcv(void)
{
    struct NetPkg *pkg;
    tagNetDev *NetCard = ptNetDev;

//    DJY_GetEventPara(NetCard,NULL);
    while(1)
    {
        Lock_SempPend(&GMAC_RcvSemp,CN_TIMEOUT_FOREVER);
        while((pkg = GMAC_RcvPacket(NetCard))!= NULL)//���϶�����ֱ��û���������ݰ�Ϊֹ
        {
            NetDev_PostPkg(NetCard,pkg);
            PkgTryFreePart(pkg);
        }
    }
    return 0;
}

// =============================================================================
// ���ܣ�GMAC�����������ݵ�����
// ����������
// ���أ�true
// =============================================================================
static bool_t GMAC_RcvTask(tagNetDev *NetCard)
{
    bool_t result = false;
    u16 evttID;
    u16 eventID;

    evttID = DJY_EvttRegist(EN_CORRELATIVE, CN_PRIO_CRITICAL, 0, 1,
        (ptu32_t (*)(void))GMAC_Rcv,NULL, 0x1000, "GMACRcvTask");
    if (evttID != CN_EVTT_ID_INVALID)
    {
        eventID=DJY_EventPop(evttID, NULL,  0,(ptu32_t)NetCard, 0, 0);
        if(eventID != CN_EVENT_ID_INVALID)
        {
            result = true;
            Lock_SempCreate_s(&GMAC_RcvSemp,1,0,CN_BLOCK_FIFO,NULL);
        }
        else
        {
            DJY_EvttUnregist(evttID);
        }
    }
    return result;
}

// =============================================================================
// �������ܣ�NetHard_AddNetDev
//          ��Э��ջ���һ�������豸
// �����������
// �����������
// ����ֵ  ��true�ɹ�  falseʧ�ܡ�
// ˵��    ��1,����豸
//        2,���·��
// =============================================================================
tagNetDev *GMAC_AddNetDev(void)
{
    struct NetDevPara  devpara;
    tagHostIpv4Addr devaddr;

    devpara.ifsend = GMAC_SendPacket;
    devpara.iftype = EN_LINK_ETHERNET;
    devpara.devfunc = 0x00;//0xFF;//�������Զ�������
    memcpy(devpara.mac, sgNetHardMac,6);
    devpara.name = "SAM4E_MAC";
    devpara.private = 0;
    devpara.linklen = 14;
    devpara.pkglen = 1500;

    ptNetDev = NetDev_AddDev(&devpara);
    if(ptNetDev != NULL)
    {
        devaddr.ip = sgNetHardIpAddrMain;
        devaddr.gateway = sgNetHardGateWay;
        devaddr.ipmsk = sgNetHardIpMsk;
        ptNetRout = Rout_AddRout(ptNetDev, &devaddr);
        if(NULL != ptNetRout)
        {
            Dhcp_AddTask(ptNetRout);
        }
        if(false == GMAC_RcvTask(ptNetDev))
        {
            printk("%s:Create Rcv Task Failed\n\r",__FUNCTION__);
        }

    }
// NetDev_AddDev��Rout_AddRout��Dhcp_AddTask��Щ����ִ��ʧ����ô�죬���ﶼû�д���
    return ptNetDev;
}

// =============================================================================
// ���ܣ�GMAC������DJYIP������ʼ������
// ������para
// ����ֵ  ��true�ɹ�  falseʧ�ܡ�
// =============================================================================
bool_t ModuleInstall_GMAC(ptu32_t para)
{
    bool_t result = false;

    __GMAC_PortInit();                               // PORT��ʼ��
    __GMAC_Init();                                  // MAC��ʼ��
    __PHY_Init();                                   // PHY��ʼ��
    __GMAC_IntInit();                               // INT SYS��ʼ��

    if(NULL == GMAC_AddNetDev())
        result = false;

    return result;
}
#else
// =============================================================================
// ============================= FUNCTIONS API FOR RAW =========================
// =============================================================================

static u8 RecvBuffer[1500];
static bool_t sGmacIsInited = false;
// =============================================================================
// Function: GMAC API FOR SEND PACKET DIRECTLY BY GMAC, NO DJYIP STACK
// PARAM: BUF,THE BUFFER OF DATE TO BE SENDING
//        LEN��THE LENGTH OF BYTES TO BE SENDING
// RETURN: TRUE,SUCCESSED; OTHERWISE,FAILED
// NOTE: return after packet be sent.
// =============================================================================
bool_t GMAC_Send(u8 *buf,u32 len)
{
    u32 pkgnum = 0,temp = 0,i;
    u32 SendBdWaitTime = SendPkgWaitMs;
    volatile gmac_tx_descriptor_t *pTxTd;
    u16 totallen=len;
    u8 *SendBuf;

    if(sGmacIsInited == false)
        return false;

    if( (NULL == buf) || (len == 0) || ( len > GMAC_TX_UNITSIZE))
        return false;

    pkgnum = 1;
    //��ѯ�Ƿ����㹻���BD��
    do
    {
        temp = GCIRC_SPACE(GtxTd.head, GtxTd.tail, GTX_BUFFERS);
        if(temp >= pkgnum)
            break;
        DJY_EventDelay(1*mS);
    }while(SendBdWaitTime-- > 0);

    SendBuf = (u8*)TxBuffer;
    if(totallen <= GMAC_TX_UNITSIZE)
    {
        memcpy(SendBuf,(void*)buf,len);
    }
    else
        return false;

    //����һ����˵�����㹻��Ŀ���BD���͸�֡����
    for (i = 0; i < pkgnum; i++)
    {
        /* Pointers to the current TxTd */
        pTxTd = GtxTd.td + GtxTd.head;

        pTxTd->status.bm.b_used = 0;
        pTxTd->status.bm.len = totallen & 0x3FFF;
        pTxTd->addr = (u32)SendBuf;//(u32)(tmp->buf + tmp->offset);
        if(i == pkgnum - 1)
        {
            // ��ǰ֡�����һ��
            pTxTd->status.bm.b_last_buffer = 1;
        }

        GCIRC_INC(GtxTd.head, GTX_BUFFERS);
    }

    // Indicate that Descriptors are ready to transmit
    GMAC->GMAC_NCR |= GMAC_NCR_TSTART;
    SendBdWaitTime = SendPkgWaitMs;
    // ��ʱ����������ٷ���
    do
    {
        if((GMAC->GMAC_TSR & GMAC_TSR_TXCOMP) )
        {
            break;
        }
        DJY_EventDelay(1*mS);
    }while(SendBdWaitTime-- > 0);

    pTxTd = GtxTd.td + GtxTd.tail;
    if (pTxTd->status.bm.b_used ==1)    //������ʱ��GMAC�Ὣb_used����Ϊ1
    {
        GCIRC_INC( GtxTd.tail, GTX_BUFFERS );
    }
    return true;
}

// =============================================================================
// Function: GMAC API FOR RECV PACKET DIRECTLY BY GMAC, NO DJYIP STACK
// PARAM: *LEN��THE LENGTH OF BYTES RECEIVED
// RETURN: TRUE,SUCCESSED; OTHERWISE,FAILED
// NOTE: return after packet be sent.
// =============================================================================
bool_t GMAC_Recv(u32 *len)
{
    u8 *rcvbuf = NULL;
    u8 tmpIdx,i,result = false,IsFrame = false,LastBuffer=0;
    u16 totallen = 0,length = 0;
    volatile gmac_rx_descriptor_t *pRxTd;

    if(sGmacIsInited == false)
        return false;

    //Ѱ�ҵ���һ������λ��
    for(i = 0;i < GRX_BUFFERS; i++)
    {
        pRxTd = GrxTd.td + GrxTd.idx;
        if(pRxTd->status.bm.b_sof)
            break;
        pRxTd->addr.bm.b_ownership = 0;
        GCIRC_INC(GrxTd.idx, GRX_BUFFERS);      //�ݴ���
    }

    if(i == GRX_BUFFERS)
    {
        return result;
    }

    // ����Ƿ������ݰ�
    tmpIdx = GrxTd.idx;
    pRxTd = GrxTd.td + tmpIdx;
    while(pRxTd->addr.bm.b_ownership)
    {
        if(pRxTd->status.bm.b_sof)                  //�ҵ���ǰ֡���װ�
        {
            while(GrxTd.idx != tmpIdx)                  //��������SOF֮��û��EOF
            {
                GCIRC_INC(GrxTd.idx, GRX_BUFFERS);
                pRxTd->addr.bm.b_ownership = 0;
                pRxTd = GrxTd.td + GrxTd.idx;
            }
            IsFrame = FRAME_START_BUF;
        }

        GCIRC_INC(tmpIdx, GRX_BUFFERS);

        if(IsFrame == FRAME_START_BUF)
        {
            if(tmpIdx == GrxTd.idx)         //δ�ҵ�EOF
            {
                do {
                    pRxTd = GrxTd.td + GrxTd.idx;
                    pRxTd->addr.bm.b_ownership = 0;
                    GCIRC_INC(GrxTd.idx, GRX_BUFFERS);
                } while(tmpIdx != GrxTd.idx);
                break;
            }
            if(pRxTd->status.bm.b_eof)      //�ҵ���һ��������֡
            {
                IsFrame |= (FRAME_END_BUF);
                totallen = pRxTd->status.bm.len;
                break;
            }
        }
        else
        {
            pRxTd->addr.bm.b_ownership = 0;
            GrxTd.idx = tmpIdx;
        }

        pRxTd = GrxTd.td + tmpIdx;
    }

    //���յ�����������֡����������ݿ���
    if( (IsFrame == (FRAME_START_BUF|FRAME_END_BUF)) &&
            (totallen <= CN_PKG_MAX_LEN) && (totallen > 0) )
    {
        rcvbuf = RecvBuffer;
        LastBuffer = 0;
        while(!LastBuffer)                             //�Ƿ�Ϊ��Frame�����һ��BD��
        {
            pRxTd = GrxTd.td + GrxTd.idx;
            length = (totallen > GMAC_RX_UNITSIZE) ?
                    GMAC_RX_UNITSIZE : (totallen);

            if( (length > 0) && (NULL != rcvbuf))
            {
               memcpy(rcvbuf,(void *)(pRxTd->addr.val & 0xFFFFFFFC),length);
               rcvbuf = rcvbuf + length;
               totallen -= length;
            }

            LastBuffer = pRxTd->status.bm.b_eof;
            if(pRxTd->status.bm.b_eof)
            {
                pRxTd->status.bm.b_eof = 0;
                pRxTd->status.bm.len = 0;
            }

            if(pRxTd->status.bm.b_sof)
            {
                pRxTd->status.bm.b_sof = 0;
            }
            pRxTd->addr.bm.b_ownership = 0;
            GCIRC_INC(GrxTd.idx, GRX_BUFFERS);
        }
        result = true;
    }

    *len = totallen;

    return result;
}
u8* GMAC_ReceiveData(void)
{
    u32 len;
    Lock_SempPend(&GMAC_RcvSemp,CN_TIMEOUT_FOREVER);
    if(true == GMAC_Recv(&len))
    {
        //������ʾ�õĺ���
        //���ݷ���  RecvBuffer
        return RecvBuffer;
    }
    return NULL;
}

// =============================================================================
// ���ܣ�GMAC������DJYIP������ʼ������
// ������para
// ����ֵ  ��true�ɹ�  falseʧ�ܡ�
// =============================================================================
bool_t ModuleInstall_GMAC(ptu32_t para)
{
    bool_t result = false;

    __GMAC_PortInit();                               // PORT��ʼ��
    __GMAC_Init();                                  // MAC��ʼ��
    __PHY_Init();                                   // PHY��ʼ��
    __GMAC_IntInit();                               // INT SYS��ʼ��

    sGmacIsInited = true;
    Lock_SempCreate_s(&GMAC_RcvSemp,1,0,CN_BLOCK_FIFO,NULL);
    return result;
}

#endif
#endif
