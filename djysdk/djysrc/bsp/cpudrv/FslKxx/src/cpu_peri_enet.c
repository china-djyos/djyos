//----------------------------------------------------
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
// Copyright (c) 2014 ����Ȩ�����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��cpu_peri_enet.c
// ģ������: ENETģ��ĳ�ʼ���Ĳ���������PHY��ʼ����MII��ʼ��
//           ��DJYOS����̫��Э��ʵ��Ҫ��ʵ�ֵ������ײ�����
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 28/07.2014
// =============================================================================
#if 1
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <sys/socket.h>

#include "cpu_peri.h"

// =============================================================================
#define CN_SYS_CLK_MHZ          120         // ϵͳʱ��Ƶ��MHZ
#define CN_PHY_ADDR             0x01        // PHY��������ַ
#define CN_PHY_IDR1             0X02        // PHY��ID�Ĵ���1
#define CN_PHY_IDR2             0X03        // PHY��ID�Ĵ���2

#define MII_TIMEOUT             0x1FFFF     // MII��ʱʱ��������Ǿ�׼��
#define MII_LINK_TIMEOUT        0x1FFFF     // MII��LINK��ʱ�������Ǿ�׼��

#if(DEBUG == 1)
#define ENET_DBG   printk
#else
#define ENET_DBG
#endif
// =============================================================================
// ����MAC�������ã�Ĭ������ΪRMII������Ӧ��100M��ȫ˫��
static tagEnetCfg s_EnetConfig = {"ENET",MAC_RMII,AUTONEG_ON,MII_100BASET,
                                    MII_FDX,NO_LOOPBACK,EXTERNAL_NONE,
                                    0,{0x00,0x01,0x02,0x03,0x04,0x02}};

#define CN_PKG_MAX_LEN  1522
// =============================================================================
static struct SemaphoreLCB *pEnetRcvSync;
static struct MutexLCB     *pEnetSndSync;
static ptu32_t              gEnetHandle = NULL;
// ��ָ���ʼ������16�ֽڶ����BD��
#ifdef __GNUC__
static u8 txbd[(sizeof(NBUF) * NUM_TXBDS)] __attribute__ ((aligned(16)));
static u8 rxbd[(sizeof(NBUF) * NUM_RXBDS)] __attribute__ ((aligned(16)));
static u8 RxBuffer[(RX_BUFFER_SIZE * NUM_RXBDS)] __attribute__ ((aligned(16)));
static u8 TxBuffer[(TX_BUFFER_SIZE * NUM_TXBDS)] __attribute__ ((aligned(16)));
#else
#error "bd must be 16-byte aligned"
#endif

// ��ָ���ʼ������16�ֽڶ����BD��
static NBUF *spTxBdTab;
static NBUF *spRxBdTab;

// ��ʶ��һ��BD����ʶ������һ�����õ�BD��
static int sNextTxBd=0;
static int sNextRxBd=0;

// =============================================================================
// ���ܣ���32λvalue�ߵ��ֽ�ת������Ҫ���ڴ�С�˱任
// ������value,ת��ֵ
// ���أ�result,ת�����
// =============================================================================
static u32 ENET_REV(u32 value)
{
  u32 result;

  __asm__ volatile ("rev %0, %1" : "=r" (result) : "r" (value) );

  return(result);
}

// =============================================================================
// ���ܣ���16λ��value�ߵ��ֽ�ת������Ҫ���ڴ�С�˱任
// ������value,ת��ֵ
// ���أ�result,ת�����
// =============================================================================
static int32_t ENET_REVSH(int32_t value)
{
  u32 result;

  __asm__ volatile ("revsh %0, %1" : "=r" (result) : "r" (value) );

  return(result);
}

// =============================================================================
// ���ܣ�MII���нӿڵ�ʱ�ӳ�ʼ����MDC��ʱ��Ƶ�ʸ�ֵ�������2.5MHz��MSCR_MII_SPEED��
//       ���㹫ʽΪ 25 MHz / ((4 + 1) x 2) = 2.5 MHz
// ������value,ת��ֵ
// ���أ�result,ת�����
// =============================================================================
static void __MII_Init(int sys_clk_mhz)
{
    ENET->MSCR = 0
#ifdef TSIEVB/*TSI EVB requires a longer hold time than default 10 ns*/
                      | ENET_MSCR_HOLDTIME(2)
#endif
                      | ENET_MSCR_MII_SPEED((2*sys_clk_mhz/5)+1)
                      ;
}

// =============================================================================
// ���ܣ�MII�ӿ�д������ͨ��MDIO��PHYоƬͨ��
// ������phy_addr, PHY��ַ
//       reg_addr,�Ĵ�����ַ
//       data,д�������
// ���أ�0,�������أ�1,ʧ��
// =============================================================================
static u32 __MII_Write(u32 phy_addr, u32 reg_addr, u32 data)
{
    int timeout;

    /* Clear the MII interrupt bit */
    ENET->EIR = ENET_EIR_MII_MASK;

    /* Initiatate the MII Management write */
    ENET->MMFR = ENET_MMFR_ST(0x01)
                | ENET_MMFR_OP(0x01)
                | ENET_MMFR_PA(phy_addr)
                | ENET_MMFR_RA(reg_addr)
                | ENET_MMFR_TA(0x02)
                | ENET_MMFR_DATA(data);

    /* Poll for the MII interrupt (interrupt should be masked) */
    for (timeout = 0; timeout < MII_TIMEOUT; timeout++)
    {
        if (ENET->EIR & ENET_EIR_MII_MASK)
            break;
    }

    if(timeout == MII_TIMEOUT)
        return 1;

    /* Clear the MII interrupt bit */
    ENET->EIR = ENET_EIR_MII_MASK;

    return 0;
}

// =============================================================================
// ���ܣ�MII�ӿڶ����ݣ�ͨ��MDIO�ӿ���PHYоƬͨ��
// ������phy_addr, PHY��ַ
//       reg_addr,�Ĵ�����ַ
//       data,��ȡ��������ָ��
// ���أ�0,�������أ�1,ʧ��
// =============================================================================
static u32 __MII_Read(u32 phy_addr, u32 reg_addr, u32 *data)
{
    int timeout;

    /* Clear the MII interrupt bit */
    ENET->EIR = ENET_EIR_MII_MASK;

    /* Initiatate the MII Management read */
    ENET->MMFR = ENET_MMFR_ST(0x01)
                | ENET_MMFR_OP(0x2)
                | ENET_MMFR_PA(phy_addr)
                | ENET_MMFR_RA(reg_addr)
                | ENET_MMFR_TA(0x02);

    /* Poll for the MII interrupt (interrupt should be masked) */
    for (timeout = 0; timeout < MII_TIMEOUT; timeout++)
    {
        if (ENET->EIR & ENET_EIR_MII_MASK)
            break;
    }

    if(timeout == MII_TIMEOUT)
        return 1;

    /* Clear the MII interrupt bit */
    ENET->EIR = ENET_EIR_MII_MASK;

    *data = ENET->MMFR & 0x0000FFFF;

    return 0;
}

// =============================================================================
// ���ܣ���λPHYоƬ�������MII�ӿں���
// ������phy_addr, PHY��ַ
// ���أ�0,�������أ�1,ʧ��
// =============================================================================
static int __PHY_Reset(int phy_addr)
{
#if MII_CHECK_TIMEOUT
    int timeout;
#endif
    u32 settings;

    /* Reset the PHY */
    if (__MII_Write(phy_addr, PHY_BMCR, PHY_BMCR_RESET))
        return 1;
    /* Wait for reset to complete */
#if MII_CHECK_TIMEOUT
    for (timeout = 0; timeout < MII_LINK_TIMEOUT; ++timeout)
#endif
    while(1)
    {
      /* Read back the contents of the CTRL register and verify
       * that RESET is not set - this is a sanity check to ensure
       * that we are talking to the PHY correctly. RESET should
       * always be cleared. */
      if (!(__MII_Read(phy_addr, PHY_BMCR, &settings)) && !(settings & PHY_BMCR_RESET))
          break;
    }
#if MII_CHECK_TIMEOUT
    if (timeout == MII_LINK_TIMEOUT || (settings & PHY_BMCR_RESET))
        return 1;
    else
#endif
        return 0;
}

// =============================================================================
// ���ܣ�����hash�㷨����CRCֵ��ȡ��8λ��Ϊ����ֵ
// ������addr, MAC��ַָ��
// ���أ�CRC����ϣ�㷨�����CRCֵ��8����ֵ
// =============================================================================
static u8 __Enet_HashAddr(const u8* addr)
{
    u32 crc;
    u8 byte;
    int i, j;

    crc = 0xFFFFFFFF;
    for(i=0; i<6; ++i)
    {
        byte = addr[i];
        for(j=0; j<8; ++j)
        {
            if((byte & 0x01)^(crc & 0x01))
            {
                crc >>= 1;
                crc = crc ^ 0xEDB88320;
            }
            else
                crc >>= 1;
            byte >>= 1;
        }
    }
    return (u8)(crc >> 26);
}

// =============================================================================
// ���ܣ�MAC��ַ���ú͵�ַhash�����ã�ȡ����Ĺ�ϣCRC��ߵ�6����
// ������pa,MAC��ַ����ָ��
// ���أ���
// =============================================================================
static void __Enet_MAC_AddrSet (const u8 *pa)
{
    u8 crc;

    // Set the Physical Address
    ENET->PALR = (u32)((pa[0]<<24) | (pa[1]<<16) | (pa[2]<<8) | pa[3]);
    ENET->PAUR = (u32)((pa[4]<<24) | (pa[5]<<16));

    // Calculate and set the hash for given Physical Address
    // in the  Individual Address Hash registers
    crc = __Enet_HashAddr(pa);
    if(crc >= 32)
        ENET->IAUR |= (u32)(1 << (crc - 32));
    else
        ENET->IALR |= (u32)(1 << crc);
}

// =============================================================================
// ���ܣ�Buffer Descriptor��ʼ�������ڷ��ͺͽ������ݣ������ֲᣬBD�����밴16�ֽڶ���
//       ����dataָ����ָ��ĵ�ַҲ�����ǰ�16�ֽڶ��룬��ַ��length����д�С��ת��
// ��������
// ���أ���
// =============================================================================
static void __Enet_BD_Init(void)
{
    int i;

    sNextTxBd = 0;
    sNextRxBd = 0;

    spTxBdTab = (NBUF *)(((u32)(txbd)) & 0xFFFFFFF0);
    spRxBdTab = (NBUF *)(((u32)(rxbd)) & 0xFFFFFFF0);

    // Initialize transmit descriptor ring
    for (i = 0; i < NUM_TXBDS; i++)
    {
        spTxBdTab[i].status = 0x0000;
        spTxBdTab[i].length = 0;
        spTxBdTab[i].data = (u8 *)ENET_REV((u32)&TxBuffer[i * TX_BUFFER_SIZE]);
    }

    // Initialize receive descriptor ring
    for (i = 0; i < NUM_RXBDS; i++)
    {
        spRxBdTab[i].status = RX_BD_E;
        spRxBdTab[i].length = 0;
        spRxBdTab[i].data = (u8 *)ENET_REV((u32)&RxBuffer[i * RX_BUFFER_SIZE]);
    }

    // Set the Wrap bit on the last one in the ring
    spRxBdTab[NUM_RXBDS - 1].status |= RX_BD_W;
    spTxBdTab[NUM_TXBDS - 1].status |= TX_BD_W;

    // Set Receive Buffer Size
    ENET->MRBR = (uint16_t)RX_BUFFER_SIZE;

    // Point to the start of the Tx buffer descriptor queue
    ENET->TDSR = (u32)spTxBdTab;
    // Point to the start of the circular Rx buffer descriptor queue
    ENET->RDSR = (u32)spRxBdTab;
}

// =============================================================================
// ���ܣ�Enet��PORT�ڳ�ʼ������Ҫ�Ƕ�ʱ�ӵ�ʹ�ܣ�MPUʧ�ܣ���GPIO���ã�����RMII��MII
//       �ֱ����ò�ͬ��IO���Ź���
// ��������
// ���أ���
// =============================================================================
static void Enet_PortInit(void)
{
    // ʹ��ENETʱ��Դ
    SIM->SCGC2 |= SIM_SCGC2_ENET_MASK;

    /*FSL: allow concurrent access to MPU controller.
     * Example: ENET uDMA to SRAM, otherwise bus error*/
    // MPUʧ�ܣ���ΪENET��Ҫ��uDMA�е������Ƶ�SRAM�����ʹ����
    // MPU�����������ߴ���
    MPU->CESR = 0;

    // ����MDC��MDIO��port�ڣ���PHYͨ��
    PORTB->PCR[0]   = (PORTB->PCR[0]  & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(4)
                      | PORT_PCR_ODE_MASK;
    PORTB->PCR[1]   = (PORTB->PCR[1]  & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(4);

    // ����ʹ�õĽӿڷ�ʽ��������ΪRMII��MII
    #ifdef CN_USE_MII_INTERFACE
    PORTA->PCR[12]  = (PORTA->PCR[12] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(4);
    PORTA->PCR[13]  = (PORTA->PCR[13] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(4);
    PORTA->PCR[14]  = (PORTA->PCR[14] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(4);
    PORTA->PCR[15]  = (PORTA->PCR[15] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(4);
    PORTA->PCR[16]  = (PORTA->PCR[16] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(4);
    PORTA->PCR[17]  = (PORTA->PCR[17] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(4);

    PORTA->PCR[11] = (PORTA->PCR[11] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(4);
    PORTA->PCR[25] = (PORTA->PCR[25] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(4);
    PORTA->PCR[9]  = (PORTA->PCR[9] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(4);
    PORTA->PCR[10] = (PORTA->PCR[10] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(4);
    PORTA->PCR[28] = (PORTA->PCR[28] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(4);
    PORTA->PCR[24] = (PORTA->PCR[24] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(4);
    PORTA->PCR[26] = (PORTA->PCR[26] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(4);
    PORTA->PCR[27] = (PORTA->PCR[27] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(4);
    PORTA->PCR[29] = (PORTA->PCR[29] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(4);
    #else
    PORTA->PCR[12]  = (PORTA->PCR[12] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(4);
    PORTA->PCR[13]  = (PORTA->PCR[13] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(4);
    PORTA->PCR[14]  = (PORTA->PCR[14] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(4);
    PORTA->PCR[15]  = (PORTA->PCR[15] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(4);
    PORTA->PCR[16]  = (PORTA->PCR[16] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(4);
    PORTA->PCR[17]  = (PORTA->PCR[17] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(4);
    #endif
}

// =============================================================================
// ���ܣ�Enet�жϳ�ʼ����ʹ���жϵķ�ʽ�������ݣ����������жϼĴ���
// ��������
// ���أ���
// =============================================================================
static void Enet_IntInit(ufast_t IntLine,u32 (*isr)(ufast_t))
{
    if(true == Int_Register(IntLine))
    {
        Int_Register(IntLine);
        Int_IsrConnect(IntLine,isr);
        Int_SettoAsynSignal(IntLine);
        Int_ClearLine(IntLine);
        Int_ContactLine(IntLine);
    }
}

// =============================================================================
// ���ܣ�PHYоƬ��ʼ����������ʹ������Ӧ��ʽ��ͨ����ȡPHYоƬ��SPEED��˫������ؼĴ�
//       �������������MAC����ز���
// ��������
// ���أ�true,link�ɹ���false,linkʧ��
// =============================================================================
static bool_t PHY_Init(void)
{
    u32 usData,NegTimes = 100;

    __PHY_Reset(CN_PHY_ADDR);
    ENET_DBG("PHY RESET DONE!\r\n");
    // MII��ʼ��
    __MII_Init(CN_SYS_CLK_MHZ);

    /* Can we talk to the PHY? */
//    do
//    {
//      Djy_DelayUs(500);
//      usData = 0xFFFF;
//      __MII_Read( CN_PHY_ADDR, CN_PHY_IDR1, &usData );
//    } while( usData == 0xFFFF );
    ENET_DBG("START TO NEGOTIATION!\r\n");
    /* Start auto negotiate. */
    __MII_Write(CN_PHY_ADDR, PHY_BMCR,PHY_BMCR_AN_ENABLE | PHY_BMCR_AN_RESTART);

    /* Wait for auto negotiate to complete. */
    do
    {
        Djy_DelayUs( 50000);
      __MII_Read( CN_PHY_ADDR, PHY_BMSR, &usData );
      NegTimes--;
    } while(( !( usData & PHY_BMSR_AN_COMPLETE ) ) && (NegTimes > 0));

    if(NegTimes == 0)
    {
        ENET_DBG("NEGOTIATION FAILED, PLEASE CHECK THE CONNECTION!\r\n");
    }

    /* When we get here we have a link - find out what has been negotiated. */
    usData = 0;
    __MII_Read(CN_PHY_ADDR, PHY_BMSR, &usData );
    ENET_DBG("START TO LINK!\r\n");
    // Is Linked ?
    if(usData & PHY_BMSR_LINK)
    {
        // Get the speed
        if ((usData & PHY_BMSR_100BT4)     ||
            (usData & PHY_BMSR_100BTX_FDX) ||
            (usData & PHY_BMSR_100BTX))
            s_EnetConfig.speed = MII_100BASET;
        else
            s_EnetConfig.speed = MII_10BASET;
        // Get the duplex
        if ((usData & PHY_BMSR_100BTX_FDX) ||
            (usData & PHY_BMSR_10BT_FDX))
            s_EnetConfig.duplex = MII_FDX;
        else
            s_EnetConfig.duplex = MII_HDX;
        return true;
    }
    ENET_DBG("PHY INIT DONE!\r\n");
    return false;
}


// =============================================================================
// ���ܣ�FSL��̫��ģ��MAC��ʼ������Ҫ��MAC�Ĵ�����BD���ĳ�ʼ�����¼������裺
//       1.��λ��̫��ģ�飬��Ҫ����صļĴ�����λ����λ��ȴ�һ��ʱ�䣻
//       2.��ʼ��BD��������TCR��RCR�������ͽ��չ��ܣ�
//       3.MAC��ַ��ʼ����дMAC���Ĵ�����������Ϊ�Ƿ�ǿ���滻���Ͱ��е�Դ��ַ��
//       4.ʹ��ENET��
// ��������
// ���أ���
// =============================================================================
static void Enet_MAC_Init(void)
{
    int i;

    // Set the Reset bit and clear the Enable bit
    ENET->ECR = ENET_ECR_RESET_MASK;

    // Wait at least 8 clock cycles
    for (i=0; i<10; ++i)
        asm( "NOP" );

    __Enet_BD_Init();                   // Init BD��

    // Clear the Individual and Group Address Hash registers
    ENET->IALR = 0;
    ENET->IAUR = 0;
    ENET->GALR = 0;
    ENET->GAUR = 0;

    __Enet_MAC_AddrSet(s_EnetConfig.mac);   // Set the Physical Address
    ENET->EIMR = ENET_EIMR_RXF_MASK         // Enable RX and err interrupts
                | ENET_EIMR_BABR_MASK
                | ENET_EIMR_BABT_MASK
                | ENET_EIMR_EBERR_MASK
                | ENET_EIMR_UN_MASK;
    ENET->EIR = 0xFFFFFFFF;                 // Clear all FEC interrupt events

    // Initialize the Receive Control Register
    ENET->RCR = ENET_RCR_MAX_FL(ETH_MAX_FRM)
//                | ENET_RCR_BC_REJ_MASK        // �����չ㲥֡
                | ENET_RCR_MII_MODE_MASK    // always
                //| ENET_RCR_CRCFWD_MASK;   // no CRC pad required
                ;

    if ( s_EnetConfig.interface == MAC_RMII )
    {
        ENET->RCR |= ENET_RCR_RMII_MODE_MASK;
        if( s_EnetConfig.speed == MII_10BASET ) // only set speed in RMII mode
        {
            ENET->RCR |= ENET_RCR_RMII_10T_MASK;
        }
    }
    // no need to configure MAC MII interface
    ENET->TCR = 0;
    // config the duplex
    if (s_EnetConfig.duplex == MII_HDX)
    {
        ENET->RCR |= ENET_RCR_DRT_MASK;
        ENET->TCR &= (u32)~ENET_TCR_FDEN_MASK;
    }
    else
    {
        ENET->RCR &= ~ENET_RCR_DRT_MASK;
        ENET->TCR |= ENET_TCR_FDEN_MASK;
    }

    if (s_EnetConfig.prom)
    {
        ENET->RCR |= ENET_RCR_PROM_MASK;        // All frames are accepted
    }

#ifdef ENHANCED_BD
    ENET_ECR = ENET_ECR_EN1588_MASK;
#else
    ENET->ECR = 0;                          //clear register
#endif

    if(s_EnetConfig.loopback == INTERNAL_LOOPBACK)
    {
        // seems like RMII internal loopback works,
        // even if it's not supported
        ENET->RCR |= ENET_RCR_LOOP_MASK;
    }

    // Enable FEC
    ENET->ECR |= ENET_ECR_ETHEREN_MASK;
    // Enable RX
    ENET->RDAR = ENET_RDAR_RDAR_MASK;
}

// =============================================================================
// ���ܣ�FSL��̫��ģ��������ݰ������ݰ����ֽ�������������Ϊ���¼������裺
//       1.BD��Ϊ���α��������ж��Ƿ���յ����ݣ���BD���Ƿ�Ϊ�գ�
//       2.��BD���е��������ݣ�ֱ��RX_BD_L��ţ�����֡���һ����
//       3.����BD����������ʹ�ܽ������ݣ�
// ������packet,���յ����ݵ��׵�ַ
// ���أ����յ����ݰ����ȣ���󲻻ᳬ��1518�ֽ�
// =============================================================================
static tagNetPkg *__Enet_RcvPacket(ptu32_t handle)
{
    tagNetPkg *pkg=NULL;
    int LastBuffer,length,len,RxBdIndex;
    u16 status;
    u8 *rcvbuf = NULL;
    atom_low_t atomop;

    LastBuffer = 0;
    length = 0;

    RxBdIndex = sNextRxBd;
    atomop = Int_LowAtomStart();
    if(spRxBdTab[RxBdIndex].status & RX_BD_E)             //BD���Ƿ�Ϊ��
    {
        Int_LowAtomEnd(atomop);
        return NULL;
    }
    Int_LowAtomEnd(atomop);

    //������յ����ݰ��м���BD����todo----�����ݴ���������û�н�����
    while(!LastBuffer)
    {
        length = ENET_REVSH(spRxBdTab[RxBdIndex].length);
        status = spRxBdTab[RxBdIndex].status;
        if(status & RX_BD_W)
        {
            RxBdIndex = 0;
        }
        else
        {
            RxBdIndex++;
        }
        LastBuffer = (status & RX_BD_L);
    }

    if(length >0)
    {
        pkg =PkgMalloc(length,CN_PKLGLST_END);
        if(NULL != pkg)
        {
            rcvbuf = (u8 *)(pkg->buf + pkg->offset);
            pkg->datalen = length;
            pkg->partnext= NULL;
        }
        else
        {
            ENET_DBG("Enet_RcvPacket() pkg_alloc failed!\r\n ");//todo
        }

    }

    LastBuffer = 0;
    RxBdIndex = sNextRxBd;
    while(!LastBuffer)                             //�Ƿ�Ϊ��Frame�����һ��BD��
    {
        status = spRxBdTab[RxBdIndex].status;
        len = ENET_REVSH(spRxBdTab[RxBdIndex].length);
        len = (length > len) ? len : length;
        length -= len;

        //--TODO, this code has something err, you could't memcpy when
        //        there is no pkg for you~!
        if(len > 0)
        {
            memcpy(rcvbuf,(uint8_t *)ENET_REV((u32)spRxBdTab[RxBdIndex].data),len);
            rcvbuf = rcvbuf + len;
        }
        if(status & RX_BD_W)
        {
            spRxBdTab[RxBdIndex].status = (RX_BD_W | RX_BD_E);
            RxBdIndex = 0;
        }
        else
        {
            spRxBdTab[RxBdIndex].status = RX_BD_E;
            RxBdIndex++;
        }
        LastBuffer = (status & RX_BD_L);
    }

    sNextRxBd = RxBdIndex;                     // ���¶�NEXT BD����־
    ENET->RDAR = ENET_RDAR_RDAR_MASK;           // ����RX BD�б�
    return pkg;
}

// =============================================================================
// ���ܣ�FSL��̫����MAC�㷢���жϷ����������������������ݡ��������������ݺͷ�������
// ������int_line,�ж��ߺ�
// ���أ�0
// =============================================================================
u32 Enet_RxISR(ufast_t IntLine)
{
    u32 event;

    event = ENET->EIR;         // ��ȡ�жϼĴ���
    ENET->EIR = ENET_EIR_RXF_MASK;                      // ���жϱ�־λ
    if(event & ENET_EIR_RXF_MASK)
    {
        Lock_SempPost(pEnetRcvSync);
    }
    return 0;
}

u32 Enet_TxISR(ufast_t IntLine)
{
    return 0;
}

u32 Enet_ErrISR(ufast_t IntLine)
{
    u32 event;

    event = ENET->EIR;                      // ��ȡ�жϼĴ���
    ENET->EIR = event;                      // ���жϱ�־λ

    ENET_DBG("ENET error occurred, event = %8x !\r\n" ,event);

    return 0;
}

// =============================================================================
// �������ܣ�Enet_SendPacket,��̫���ײ���������PKG���ݰ����������£�
//          1.�ͷ��Ѿ�������ϵ�bd��ʹ�õ�pkg��
//          2.���㵱ǰ����֡�ɶ��ٸ�pkg��ɣ�
//          3.���㵱ǰBD Ring�ж��ٿ���BD��
//          4.������BD��Ŀ��֡�����pkg��Ŀ�㹻��������BD�������ͣ�����������ȴ�
//          5.����ָ����ʱ����δ����㹻��bd����ֱ�ӷ���ʧ��
// ���������devindex,ʹ�õ�����
//          pkg,�����͵����ݰ�
// ���������
// ����ֵ  ��true���ͳɹ�  false����ʧ�ܡ�
// =============================================================================
static bool_t Enet_SendPacket(ptu32_t hanlde,tagNetPkg * pkglst,u32 framlen, u32 netdevtask)
{
    bool_t  result = false;
    tagNetPkg *tmppkg;
    u32 TxBdIndex;

    u8  *dst,*src;
    u32 len,pkglen;

    if((NULL == hanlde)||(NULL == pkglst))
        return result;

    if(Lock_MutexPend(pEnetSndSync,CN_TIMEOUT_FOREVER))
    {
        TxBdIndex = sNextTxBd;

        if( !(spTxBdTab[TxBdIndex].status & TX_BD_R) )      //��ǰBD��׼������
        {
            dst = (u8 *)ENET_REV((u32)spTxBdTab[TxBdIndex].data);

            pkglen = 0;
            len =0;
            tmppkg = pkglst;
            do{
                //��������
                src = (tmppkg->buf + tmppkg->offset);
                len = tmppkg->datalen;
                memcpy((void *)dst,(void *)src,len);
                dst += len;
                pkglen+=len;
                if(pkglen > TX_BUFFER_SIZE)
                {
                    break;//���
                }

                if(PKG_ISLISTEND(tmppkg))
                {
                    tmppkg = NULL;
                    break;
                }
                else
                {
                    tmppkg = tmppkg->partnext;
                }
            }while(tmppkg != NULL);

            spTxBdTab[TxBdIndex].status = TX_BD_TC | TX_BD_R | TX_BD_L;
            spTxBdTab[TxBdIndex].length = ENET_REVSH(pkglen);

            // Wrap if this was last TxBD
            if(++TxBdIndex == NUM_TXBDS)
            {
                spTxBdTab[NUM_TXBDS - 1].status |= TX_BD_W;
                TxBdIndex = 0;
            }

            // Update the global txbd index
            sNextTxBd = TxBdIndex;

            // Indicate that Descriptors are ready to transmit
            ENET->TDAR = ENET_TDAR_TDAR_MASK;

            result = true;
        }
        else
        {
            //no bd here
        }
        Lock_MutexPost(pEnetSndSync);
    }

    return result;
}
// =============================================================================
// ���ܣ������¼��ĺ����������ȴ����յ��������ݰ����ź����������ն�ȡ������������������ݰ�
// ��������
// ���أ���
// =============================================================================
static ptu32_t __GmacRcvTask(void)
{
    tagNetPkg *pkg;
    while(1)
    {
        Lock_SempPend(pEnetRcvSync,CN_TIMEOUT_FOREVER);
        while((pkg = __Enet_RcvPacket(gEnetHandle))!= NULL)//���϶�����ֱ��û���������ݰ�Ϊֹ
        {
            LinkPost(gEnetHandle,pkg);
            PkgTryFreePart(pkg);
        }
    }
    return 0;
}
// =============================================================================
// ���ܣ������������ݵ�����
// ����������
// ���أ�true
// =============================================================================
static bool_t __CreateRcvTask(ptu32_t nethandle)
{
    bool_t result = false;
    u16 evttID;
    u16 eventID;

    evttID = Djy_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 1,
        (ptu32_t (*)(void))__GmacRcvTask,NULL, 0x400, "ENETRcvTask");
    if (evttID != CN_EVTT_ID_INVALID)
    {
        eventID=Djy_EventPop(evttID, NULL,  0,(ptu32_t)nethandle, 0, 0);
        if(eventID != CN_EVENT_ID_INVALID)
        {
            result = true;
        }
        else
        {
            Djy_EvttUnregist(evttID);
        }
    }
    return result;
}

bool_t Enet_Ctrl(ptu32_t handle,u8 cmd, ptu32_t para)
{
    atom_low_t  atom;
    bool_t result = false;
    if(0 != handle)
    {
        switch(cmd)
        {
            case EN_NETDEV_SETMAC:
                memcpy(s_EnetConfig.mac,(u8 *)para, CN_MACADDR_LEN);
                atom = Int_LowAtomStart();
                Enet_MAC_Init();
                Int_LowAtomEnd(atom);
                result = true;
                break;
            case EN_NETDEV_GTETMAC:
                break;
            default:
                break;
        }
    }
    return result;
}
// =============================================================================
// �������ܣ�NetHard_AddNetDev
//          ��Э��ջ����һ�������豸
// �����������
// �����������
// ����ֵ  ��true�ɹ�  falseʧ�ܡ�
// ˵��    ��1,�����豸
//        2,����·��
// =============================================================================
static bool_t Enet_AddNetDev(void)
{
    tagNetDevPara   devpara;

    //�������ͺͽ���ͬ����
    pEnetRcvSync = Lock_SempCreate(1,1,CN_BLOCK_FIFO,NULL);
    if(NULL == pEnetRcvSync)
    {
        goto RcvSyncFailed;
    }
    pEnetSndSync = Lock_MutexCreate(NULL);
    if(NULL == pEnetSndSync)
    {
        goto SndSyncFailed;
    }

    //ע�ᵽ�����豸����
    devpara.ifctrl = Enet_Ctrl;
    devpara.ifsend = Enet_SendPacket;
    devpara.iftype = EN_LINK_ETHERNET;
    devpara.devfunc = 0;    //NO FUNC FOR THE DEV
    memcpy(devpara.mac, s_EnetConfig.mac,6);
    devpara.name = (char *)(s_EnetConfig.name);
    devpara.private = 0;
    devpara.mtu = CN_PKG_MAX_LEN;
    devpara.private = (u32)NULL;
    gEnetHandle = NetDevInstall(&devpara);
    if(0 == gEnetHandle)
    {
        goto NetInstallFailed;
    }
    if(false == __CreateRcvTask(gEnetHandle))
    {
        goto RcvTaskFailed;
    }
    return true;

RcvTaskFailed:
    NetDevUninstall(s_EnetConfig.name);
NetInstallFailed:
    Lock_MutexDelete(pEnetSndSync);
    pEnetSndSync = NULL;
SndSyncFailed:
    Lock_SempDelete(pEnetRcvSync);
    pEnetRcvSync = NULL;
RcvSyncFailed:
    printf("%s:Install gmac failed\n\r",__FUNCTION__);
    return false;
}


// =============================================================================
// ���ܣ�DM9000������DJYIP������ʼ������
// ������para
// ����ֵ  ��true�ɹ�  falseʧ�ܡ�
// =============================================================================
bool_t ModuleInstall_Enet(ptu32_t para)
{
    bool_t result = false;

    if(para == (u32)NULL)
        return false;

    memcpy(&s_EnetConfig,(u8*)para,sizeof(tagEnetCfg));

    Enet_PortInit();                                // PORT��ʼ��
    Enet_IntInit(CN_INT_LINE_ENET_RX,Enet_RxISR);   // INT��ʼ��
    Enet_IntInit(CN_INT_LINE_ENET_ERR,Enet_ErrISR); // INT��ʼ��

    PHY_Init();                         // PHY��ʼ��
    Enet_MAC_Init();                    // MAC��ʼ��

    result = Enet_AddNetDev();

    return result;
}
#endif