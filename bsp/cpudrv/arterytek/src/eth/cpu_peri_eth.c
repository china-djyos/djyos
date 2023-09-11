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
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <cpu_peri.h>
#include <board-config.h>
#include <sys/socket.h>
#include <netbsp.h>
#include <dbug.h>
#include "shell.h"
#include <at32f435_437.h>
#include <at32f435_437_crm.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_ETH(void);
//    ModuleInstall_ETH( );
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip ETH"//CPU��mac����
//parent:"tcpip"       //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"tcpip","lock"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_MAC == false )
//#warning  " cpu_onchip_MAC  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_MAC    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@string,1,32,
#define CFG_ETH_NETCARD_NAME    "AT32F437_ETH"   //"��������",
//%$#@num,1000,10000,
#define CFG_ETH_LOOP_CYCLE      1000         //"������ѯ����(uS)",�ж�ģʽ������д
//%$#@enum,true,false,
#define CFG_ETH_LOOP_ENABLE     true            //"���������Ƿ���ѯ",
#define CFG_ETH_HARD_MAC_ADDR   true            //"Ӳ������Mac��ַ",
//%$#@num,0,255,
#define CFG_ETH_MAC_ADDR0      00           //"MAC ADDR0",��ѡ��"Ӳ������Mac��ַ",��������д
#define CFG_ETH_MAC_ADDR1      02           //"MAC ADDR1",��ѡ��"Ӳ������Mac��ַ",��������д
#define CFG_ETH_MAC_ADDR2      03           //"MAC ADDR2",��ѡ��"Ӳ������Mac��ַ",��������д
#define CFG_ETH_MAC_ADDR3      04           //"MAC ADDR3",��ѡ��"Ӳ������Mac��ַ",��������д
#define CFG_ETH_MAC_ADDR4      05           //"MAC ADDR4",��ѡ��"Ӳ������Mac��ַ",��������д
#define CFG_ETH_MAC_ADDR5      06           //"MAC ADDR5",��ѡ��"Ӳ������Mac��ַ",��������д
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure
// =============================================================================

bool_t MacReset(char *param);
#define CN_DEVNAME_LEN 32
// //this is the mac receive hook, if any hook rcv,then the data will goto the hook
// //the receive data goto the stack default
typedef bool_t (*fnRcvDeal)(u8 *buf, u16 len);

#define EMAC_DMARxDesc_FrameLengthShift  16

#define EMAC_RXBUFNB        6
#define EMAC_TXBUFNB        6
emac_dma_desc_type  DMARxDscrTab[EMAC_RXBUFNB], DMATxDscrTab[EMAC_TXBUFNB];/* Ethernet Rx & Tx DMA Descriptors */
uint8_t Rx_Buff[EMAC_RXBUFNB][EMAC_MAX_PACKET_LENGTH], Tx_Buff[EMAC_TXBUFNB][EMAC_MAX_PACKET_LENGTH];/* Ethernet buffers */

// ��at32f435_437_emac.c�ж���ġ�ָ��DMATxDscrTab��DMARxDscrTab
extern emac_dma_desc_type  *dma_tx_desc_to_set;
extern emac_dma_desc_type  *dma_rx_desc_to_get;

typedef struct
{
    vu32  Ints;                     //the int counter
    vu32  rcvInts;                  //rcv ints counter
    vu32  dmaerr;                   //dmaerr ins counter
    vu32  rxoverInts;               //rcv over ints counter
    vu32  rcvPkgTimes;              //rcv package times
    vu32  sndTimes;                 //the snd times counter
    vu32  sndOkTimes;               //the send success counter
    vu32  sndnobdCnt;               //not enough bd rings
    vu32  sndPkgTooLongCnt;         //snd pkg too long ounter
    vu32  sndInts;                  //the send ints counter
    vu32  rxTaskRun;                //the rx task run counter
    vu32  rsttimes;                 //the reset counter
    vu32  nopkgCnts;                //the no pkg free
}tagMacDebug;

// ETH_HandleTypeDef sEthHandle;
typedef struct
{
    //os member
    struct SemaphoreLCB     *rcvsync;          //activate the receive task
    struct MutexLCB         *protect;          //protect the device
    struct NetDev           *devhandle;        //returned by the tcpip stack
    char                    devname[CN_DEVNAME_LEN];
    //hardware
    // ETH_HandleTypeDef       *EthHandle;
    //which used to descript the device or control the device
    struct devstat
    {
        vu32 pm:1;        //1 copy all the frame ,no addr filter
        vu32 bfd:1;       //1 broadcast frames disable
        vu32 duplex:1;    //1 full while 0 half
        vu32 speed:1;     //1 means 100Mb while 0 10 Mb
        vu32 mii:1;       //1 means mii mode while 0 rmii mode
    }macstat;
    u8            macaddr[CN_MACADDR_LEN];    //storage the mac address
    //dev method
    u8 loop;          //1 use the loop mode while 0 interrupt mode
    u32 loopcycle;    //unit:uS
    fnRcvDeal     fnrcvhook;                  //rcv hook function
    tagMacDebug   debuginfo;                  //debug info
}tagMacDriver;
static tagMacDriver  gMacDriver;

emac_control_config_type mac_control_para;

static u8 gTxBuffer[EMAC_MAX_PACKET_LENGTH];      //for sending copy frame

// ============================================================================
// ���ܣ���λPHYоƬ�Ľӿڣ���board��ʵ��
// ��������
// ���أ���
// ============================================================================
__attribute__((weak))  void Board_ResetPHY(void)
{
    return;
}

// ============================================================================
// ���ܣ����������ٶȣ��Լ�����ģʽ��ȫ˫�������ǰ�˫����
// ������nego���Ƿ��Զ�Э�̲�����mode������ģʽ��speed���ٶ�
// ���أ�SUCCESS -- �ɹ���ERROR -- ʧ��
// ============================================================================
error_status emac_speed_config(emac_auto_negotiation_type nego, emac_duplex_type mode, emac_speed_type speed)
{
    uint16_t data = 0;
    uint32_t timeout = 0;

    if(nego == EMAC_AUTO_NEGOTIATION_ON)    //�Զ�Э�̲���
    {
        do
        {
            timeout++;
            if(emac_phy_register_read(PHY_ADDRESS, PHY_STATUS_REG, &data) == ERROR)
            {
                return ERROR;
            }
        } while(!(data & PHY_LINKED_STATUS_BIT) && (timeout < PHY_TIMEOUT));

        if(timeout == PHY_TIMEOUT)
        {
            return ERROR;
        }

        timeout = 0;

        if(emac_phy_register_write(PHY_ADDRESS, PHY_CONTROL_REG, PHY_AUTO_NEGOTIATION_BIT) == ERROR)
        {
            return ERROR;
        }


        do
        {
            timeout++;
            if(emac_phy_register_read(PHY_ADDRESS, PHY_STATUS_REG, &data) == ERROR)
            {
                return ERROR;
            }
        } while(!(data & PHY_NEGO_COMPLETE_BIT) && (timeout < PHY_TIMEOUT));

        if(timeout == PHY_TIMEOUT)
        {
            return ERROR;
        }

        if(emac_phy_register_read(PHY_ADDRESS, PHY_SPECIFIED_CS_REG, &data) == ERROR)
        {
            return ERROR;
        }

        if(data & PHY_FULL_DUPLEX_100MBPS_BIT)
        {
            emac_fast_speed_set(EMAC_SPEED_100MBPS);
            emac_duplex_mode_set(EMAC_FULL_DUPLEX);
        }
        else if(data & PHY_HALF_DUPLEX_100MBPS_BIT)
        {
            emac_fast_speed_set(EMAC_SPEED_100MBPS);
            emac_duplex_mode_set(EMAC_HALF_DUPLEX);
        }
        else if(data & PHY_FULL_DUPLEX_10MBPS_BIT)
        {
            emac_fast_speed_set(EMAC_SPEED_10MBPS);
            emac_duplex_mode_set(EMAC_FULL_DUPLEX);
        }
        else if(data & PHY_HALF_DUPLEX_10MBPS_BIT)
        {
            emac_fast_speed_set(EMAC_SPEED_10MBPS);
            emac_duplex_mode_set(EMAC_HALF_DUPLEX);
        }
    }
    else
    {
        if(emac_phy_register_write(PHY_ADDRESS, PHY_CONTROL_REG, (uint16_t)((mode << 8) | (speed << 13))) == ERROR)
        {
            return ERROR;
        }
        if(speed == EMAC_SPEED_100MBPS)
        {
            emac_fast_speed_set(EMAC_SPEED_100MBPS);
        }
        else
        {
            emac_fast_speed_set(EMAC_SPEED_10MBPS);
        }
        if(mode == EMAC_FULL_DUPLEX)
        {
            emac_duplex_mode_set(EMAC_FULL_DUPLEX);
        }
        else
        {
            emac_duplex_mode_set(EMAC_HALF_DUPLEX);
        }
    }

  return SUCCESS;
}

// ============================================================================
// ���ܣ�PHYоƬ�ļĴ�������
// ��������
// ���أ�FALSE -- ʧ�ܣ�TRUE -- �ɹ�
// ============================================================================
bool_t emac_phy_register_reset(void)
{
    uint16_t data = 0;
    uint32_t timeout = 0;
    uint32_t i = 0;

    if(emac_phy_register_write(PHY_ADDRESS, PHY_CONTROL_REG, PHY_RESET_BIT) == ERROR)
    {
        return FALSE;
    }

    for(i = 0; i < 0x000FFFFF; i++);

    do
    {
        timeout++;
        if(emac_phy_register_read(PHY_ADDRESS, PHY_CONTROL_REG, &data) == ERROR)
        {
            return FALSE;
        }
    } while((data & PHY_RESET_BIT) && (timeout < PHY_TIMEOUT));

    for(i = 0; i < 0x00FFFFF; i++);

    if(timeout == PHY_TIMEOUT)
    {
        return FALSE;
    }
    return TRUE;
}

// ============================================================================
// ���ܣ�PHYоƬ�ĳ�ʼ��
// ��������
// ���أ�FALSE -- ʧ�ܣ�TRUE -- �ɹ�
// ============================================================================
bool_t emac_phy_init(emac_control_config_type *control_para)
{
    emac_clock_range_set();   //ʱ������
    if(emac_phy_register_reset() == FALSE)
    {
        return FALSE;
    }

    emac_control_config(control_para);
    return TRUE;
}

// ============================================================================
// ���ܣ��ȴ�PHYоƬ���ӳɹ����������ù���ģʽ
// ��������
// ���أ�FALSE -- ʧ�ܣ�TRUE -- �ɹ�
// ============================================================================
bool_t emac_phy_set(void)
{
    uint16_t link_data = 0;
    uint16_t link_state = 0;
    u32 timeout = 5;
    bool_t start = true;
    u32 i = 0;
    while (timeout --)
    {
        link_data = 0;
        if(emac_phy_register_read(PHY_ADDRESS, PHY_STATUS_REG, &link_data) == SUCCESS)
        {
            link_state = (link_data & PHY_LINKED_STATUS_BIT)>>2;
        }

        if (link_state != 0)
        {
            emac_speed_config(mac_control_para.auto_nego, mac_control_para.duplex_mode, mac_control_para.fast_ethernet_speed);
            for(i = 0; i < 0x000FFFFF; i++);    //������������⣬�����������ʱһ��
            emac_start();
            break;
        }
        else
        {
            if (start)
            {
                emac_stop(); 
                start = false;
            }
        }
        DJY_DelayUs(5000);
    }

    if (0 == link_state)
    {
        emac_speed_config(mac_control_para.auto_nego, mac_control_para.duplex_mode, mac_control_para.fast_ethernet_speed);
        for(i = 0; i < 0x000FFFFF; i++);    //������������⣬�����������ʱһ��
        emac_start();
    }

    return link_state != 0;
}


// ============================================================================
// ���ܣ���λ����
// ������pDrive����������������
// ���أ���
// ============================================================================
static void ETH_SystemReset(tagMacDriver *pDrive)
{
    emac_dma_config_type dma_control_para;
    crm_periph_clock_enable(CRM_EMAC_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_EMACTX_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_EMACRX_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_SCFG_PERIPH_CLOCK, TRUE);
    if (pDrive->macstat.mii)
    {
        scfg_emac_interface_set(SCFG_EMAC_SELECT_MII);     //ʹ��RMIIģʽ
    }
    else
    {
        scfg_emac_interface_set(SCFG_EMAC_SELECT_RMII);     //ʹ��RMIIģʽ
    }
    crm_clock_out1_set(CRM_CLKOUT1_PLL);
    crm_clkout_div_set(CRM_CLKOUT_INDEX_1, CRM_CLKOUT_DIV1_5, CRM_CLKOUT_DIV2_2);
    /* reset phy */
    Board_ResetPHY();
    /* reset emac ahb bus */
    emac_reset();
    /* software reset emac dma */
    emac_dma_software_reset_set();

    while(emac_dma_software_reset_get() == SET);

    emac_control_para_init(&mac_control_para);
    mac_control_para.auto_nego = EMAC_AUTO_NEGOTIATION_ON;
    mac_control_para.ipv4_checksum_offload = TRUE;
    if (pDrive->macstat.speed)
    {
        mac_control_para.fast_ethernet_speed = EMAC_SPEED_100MBPS;
    }
    else
    {
        mac_control_para.fast_ethernet_speed = EMAC_SPEED_10MBPS;
    }
    if (pDrive->macstat.duplex)
    {
        mac_control_para.duplex_mode = EMAC_FULL_DUPLEX;
    }
    else
    {
        mac_control_para.duplex_mode = EMAC_HALF_DUPLEX;
    }

    if(emac_phy_init(&mac_control_para))
    {
        emac_dma_para_init(&dma_control_para);

        dma_control_para.rsf_enable = TRUE;
        dma_control_para.tsf_enable = TRUE;
        dma_control_para.osf_enable = TRUE;
        dma_control_para.aab_enable = TRUE;
        dma_control_para.usp_enable = TRUE;
        dma_control_para.fb_enable = TRUE;
        dma_control_para.flush_rx_disable = TRUE;
        dma_control_para.rx_dma_pal = EMAC_DMA_PBL_32;
        dma_control_para.tx_dma_pal = EMAC_DMA_PBL_32;
        dma_control_para.priority_ratio = EMAC_DMA_2_RX_1_TX;

        emac_dma_config(&dma_control_para);
        emac_dma_interrupt_enable(EMAC_DMA_INTERRUPT_NORMAL_SUMMARY, TRUE);
        emac_dma_interrupt_enable(EMAC_DMA_INTERRUPT_RX, TRUE);

        emac_local_address_set(pDrive->macaddr);   //MAC��ַ����

        if (pDrive->macstat.bfd)
        {
            emac_broadcast_frames_disable(true);
        }
        else
        {
            emac_broadcast_frames_disable(false);
        }

    }
    else
    {
        printk("phy initialized failed\r\n");
    }
}

// ============================================================================
// ���ܣ���ʼ������
// ������pDrive����������������
// ���أ���
// ============================================================================
static void ETH_HardDrvInit(tagMacDriver *pDrive)
{
    u32 i = 0;

    ETH_SystemReset(pDrive);

    /* Initialize Tx Descriptors list: Chain Mode */
    emac_dma_descriptor_list_address_set(EMAC_DMA_TRANSMIT, DMATxDscrTab, &Tx_Buff[0][0], EMAC_TXBUFNB);
    /* Initialize Rx Descriptors list: Chain Mode  */
    emac_dma_descriptor_list_address_set(EMAC_DMA_RECEIVE, DMARxDscrTab, &Rx_Buff[0][0], EMAC_RXBUFNB);

  /* Enable Ethernet Rx interrrupt */
    for(i=0; i < EMAC_RXBUFNB; i++)
    {
        emac_dma_rx_desc_interrupt_config(&DMARxDscrTab[i], TRUE);
    }
    for(i=0; i < EMAC_TXBUFNB; i++)
    {
        DMATxDscrTab[i].status |= EMAC_DMATXDESC_CIC_TUI_FULL;
    }

    emac_fast_speed_set(EMAC_SPEED_100MBPS);
    emac_duplex_mode_set(EMAC_FULL_DUPLEX);
    emac_start();

//    ������PYHоƬ�ˣ�����Ĭ�ϵĹ�����ʽ���У���Ϊû�����ߵĻ���PHYоƬ���ʲ��ˡ�
//    if (emac_phy_set() == FALSE)
//    {
//        printk("phy set failed\r\n");
//    }
}

static void __MacReset(tagMacDriver *pDrive)
{
    if(Lock_MutexPend(pDrive->protect,CN_TIMEOUT_FOREVER))
    {
        ETH_HardDrvInit(pDrive);
        pDrive->debuginfo.rsttimes++;
        Lock_MutexPost(pDrive->protect);
    }
    return ;
}

// ============================================================================
// ���ܣ�����������
// ������devhandle�������豸���
// ���أ�NULL -- ʧ�ܣ����� -- �������������ݰ�
// ============================================================================
static struct NetPkg *__MacRcv(struct NetDev *devhandle)
{
    struct NetPkg *pkg = NULL;
    tagMacDriver *pDrive = &gMacDriver;
    u16 CopyBytes = 0;
    u16 len = 0;
    u8 *dst = NULL;
    u8 *src = NULL;

    if (0 == emac_received_packet_size_get())
    {
        return pkg;
    }

    /* Check if the descriptor is owned by the ETHERNET DMA (when set) or CPU (when reset) */
    if((dma_rx_desc_to_get->status & EMAC_DMARXDESC_OWN) != (u32)RESET)
    {
        if(emac_dma_flag_get(EMAC_DMA_RBU_FLAG))
        {
            /* Clear RBUS ETHERNET DMA flag */
            emac_dma_flag_clear(EMAC_DMA_RBU_FLAG);
            /* Resume DMA reception */
            EMAC_DMA->rpd_bit.rpd = FALSE;
        }
    }
    else
    {
        if(((dma_rx_desc_to_get->status & EMAC_DMATXDESC_ES) == (u32)RESET) &&
            ((dma_rx_desc_to_get->status & EMAC_DMARXDESC_LS) != (u32)RESET) &&
            ((dma_rx_desc_to_get->status & EMAC_DMARXDESC_FS) != (u32)RESET))
        {
            /* Get the Frame Length of the received packet: substruct 4 bytes of the CRC */
            len = ((dma_rx_desc_to_get->status & EMAC_DMARXDESC_FL) >> EMAC_DMARxDesc_FrameLengthShift) - 4;
        }
        else
        {
            /* Return ERROR */
            len = 0;
        }

        CopyBytes = len;

        if(len > 0)
        {
            pkg = PkgMalloc(len,CN_PKLGLST_END);
            if(NULL == pkg)
                pDrive->debuginfo.nopkgCnts++;
        }

        if(NULL != pkg)
        {
            dst = PkgGetCurrentBuffer(pkg);
            src = (u8 *)dma_rx_desc_to_get->buf1addr;
            while(CopyBytes > EMAC_MAX_PACKET_LENGTH)
            {
                memcpy(dst, src, EMAC_MAX_PACKET_LENGTH);
                CopyBytes -= EMAC_MAX_PACKET_LENGTH;
                dst += EMAC_MAX_PACKET_LENGTH;

                dma_rx_desc_to_get->status |= EMAC_DMARXDESC_OWN;     //release Des to DMA
                dma_rx_desc_to_get = (emac_dma_desc_type *)(dma_rx_desc_to_get->buf2nextdescaddr);
                src = (u8 *)(dma_rx_desc_to_get->buf1addr);
            }
            memcpy(dst, src, CopyBytes);
            dma_rx_desc_to_get->status |= EMAC_DMARXDESC_OWN;
            dma_rx_desc_to_get = (emac_dma_desc_type *)(dma_rx_desc_to_get->buf2nextdescaddr);
            PkgSetDataLen(pkg, len);
        }
        else        //if malloc pkg failed
        {
            while(CopyBytes > EMAC_MAX_PACKET_LENGTH)
            {
                CopyBytes -= EMAC_MAX_PACKET_LENGTH;

                dma_rx_desc_to_get->status |= EMAC_DMARXDESC_OWN;     //release Des to DMA
                dma_rx_desc_to_get = (emac_dma_desc_type *)(dma_rx_desc_to_get->buf2nextdescaddr);
            }
            dma_rx_desc_to_get->status |= EMAC_DMARXDESC_OWN;
            dma_rx_desc_to_get = (emac_dma_desc_type *)(dma_rx_desc_to_get->buf2nextdescaddr);
        }
    }

    if(emac_dma_flag_get(EMAC_DMA_RBU_FLAG))
    {
        /* Clear RBUS ETHERNET DMA flag */
        emac_dma_flag_clear(EMAC_DMA_RBU_FLAG);
        /* Resume DMA reception */
        EMAC_DMA->rpd_bit.rpd = FALSE;
        pDrive->debuginfo.rxoverInts++;
    }

    return pkg;
}

// ============================================================================
// ���ܣ��������ݷ���
// ������handle�������豸�����pkg -- �����͵����ݰ���
// ���أ�false -- ʧ�ܣ�true -- �ɹ�
// ============================================================================
static bool_t MacSnd(struct NetDev* handle,struct NetPkg * pkg, u32 netdevtask)
{
    bool_t result = false;
    tagMacDriver *pDrive = &gMacDriver;
    struct NetPkg *tmppkg = NULL;
    u8 *dst = NULL;
    u8 *src = NULL;
    u16 len=0;

    pDrive->debuginfo.sndTimes++;
    if ((NULL == handle)||(NULL == pkg))
    {
        return result;
    }

    if(Lock_MutexPend(pDrive->protect, CN_TIMEOUT_FOREVER))
    {
        //Is this buffer available? If not, goto error
        if((dma_tx_desc_to_set->status & EMAC_DMATXDESC_OWN) != (uint32_t)RESET)
        {
            pDrive->debuginfo.sndnobdCnt ++;
            goto NODESCERROR;
        }

        //copy datas to static frame buffer
        tmppkg = pkg;
        dst      = &gTxBuffer[0];
        len = PkgFrameDataCopy(tmppkg, dst);
        if(len < EMAC_MAX_PACKET_LENGTH)
        {
            src = &gTxBuffer[0];
            // dst = (u8 *)(EthHandle->TxDesc->Buffer1Addr);
            dst = (u8 *)(dma_tx_desc_to_set->buf1addr);
            memcpy(dst, src , len);

            /* Setting the Frame Length: bits[12:0] */
            dma_tx_desc_to_set->controlsize = (len & EMAC_DMATXDESC_TBS1);
            /* Setting the last segment and first segment bits (in this case a frame is transmitted in one descriptor) */
            dma_tx_desc_to_set->status |= EMAC_DMATXDESC_LS | EMAC_DMATXDESC_FS;
            /* Set Own bit of the Tx descriptor Status: gives the buffer back to ETHERNET DMA */
            dma_tx_desc_to_set->status |= EMAC_DMATXDESC_OWN;

            /* When Tx Buffer unavailable flag is set: clear it and resume transmission */
            if(emac_dma_flag_get(EMAC_DMA_TBU_FLAG))
            {
                /* Clear TBUS ETHERNET DMA flag */
                emac_dma_flag_clear(EMAC_DMA_TBU_FLAG);
                /* Resume DMA transmission*/
                EMAC_DMA->tpd_bit.tpd = 0;
            }

            // Point to next descriptor
            dma_tx_desc_to_set = (emac_dma_desc_type *)(dma_tx_desc_to_set->buf2nextdescaddr);
            result = true;
        }
        else
        {
            pDrive->debuginfo.sndPkgTooLongCnt++;
        }

NODESCERROR:
        // When Transmit Underflow flag is set, clear it and issue a Transmit
        //Poll Demand to resume transmission
        // if ((EthHandle->Instance->DMASR & ETH_DMASR_TUS) != (uint32_t)RESET)
        if (emac_dma_flag_get(EMAC_DMA_UNF_FLAG) != (uint32_t)RESET)
        {
            // Clear TUS ETHERNET DMA flag
            emac_dma_flag_clear(EMAC_DMA_UNF_FLAG);

            //Resume DMA transmission
            emac_dma_poll_demand_set(EMAC_DMA_TRANSMIT, 0);
        }
        Lock_MutexPost(pDrive->protect);
    }

    return result;
}

//This is the interrut handler
u32 ETH_IntHandler(ufast_t IntLine)
{
    tagMacDriver *pDrive = &gMacDriver;
    // ETH_HandleTypeDef *EthHandle;

    pDrive = &gMacDriver;
    // EthHandle = pDrive->EthHandle;

    pDrive->debuginfo.Ints++;
    // if (__HAL_ETH_DMA_GET_FLAG(EthHandle, ETH_DMA_FLAG_R))
    if (emac_dma_flag_get(EMAC_DMA_RI_FLAG))
    {
        pDrive->debuginfo.rcvInts++;
        Lock_SempPost(pDrive->rcvsync);
        // __HAL_ETH_DMA_CLEAR_IT(EthHandle, ETH_DMA_IT_R);    //Clear the Eth DMA Rx IT pending bits
        emac_dma_flag_clear(EMAC_DMA_RI_FLAG);      //Clear the Eth DMA Rx IT pending bits
        // EthHandle->State = HAL_ETH_STATE_READY;             //Set HAL State to Ready
        // __HAL_UNLOCK(EthHandle);
    }
    // else if (__HAL_ETH_DMA_GET_FLAG(EthHandle, ETH_DMA_FLAG_T))
    else if (emac_dma_flag_get(EMAC_DMA_TI_FLAG))
    {
        pDrive->debuginfo.sndInts++;
        // __HAL_ETH_DMA_CLEAR_IT(EthHandle, ETH_DMA_IT_T);    //Clear the Eth DMA Tx IT pending bits
        emac_dma_flag_clear(EMAC_DMA_TI_FLAG);      //Clear the Eth DMA Tx IT pending bits
        // EthHandle->State = HAL_ETH_STATE_READY;             //Set HAL State to Ready
        // __HAL_UNLOCK(EthHandle);
    }

    // __HAL_ETH_DMA_CLEAR_IT(EthHandle, ETH_DMA_IT_NIS);
    emac_dma_flag_clear(EMAC_DMA_NIS_FLAG);

    // if(__HAL_ETH_DMA_GET_FLAG(EthHandle, ETH_DMA_FLAG_AIS))
    if (emac_dma_flag_get(EMAC_DMA_AIS_FLAG))
    {
        pDrive->debuginfo.dmaerr++;
        // __HAL_ETH_DMA_CLEAR_IT(EthHandle, ETH_DMA_FLAG_AIS);
        emac_dma_flag_clear(EMAC_DMA_AIS_FLAG);
        // EthHandle->State = HAL_ETH_STATE_READY;

        // __HAL_UNLOCK(EthHandle);
    }
    return 0;
}

__attribute__((weak)) void GMAC_InLowPowerPinCfg(void)
{
    printf("\r\n���ڰ���������ṩ����͹���ʱ�����������ŵĺ�����");
}
__attribute__((weak))  void GMAC_OutLowPowerPinCfg(void)
{
    printf("\r\n���ڰ���������ṩ�뿪�͹���ʱ�����������ŵĺ�����");
}

//mac control function
static bool_t MacCtrl(struct NetDev *devhandle,u8 cmd,ptu32_t para)
{
    bool_t result = false;
    tagMacDriver   *pDrive;
    pDrive = &gMacDriver;
    if((pDrive->devhandle == devhandle)&&\
        (Lock_MutexPend(pDrive->protect,CN_TIMEOUT_FOREVER)))
    {
        switch(cmd)
        {
            case EN_NETDEV_SETNOPKG:
                if(para)
                {
                    ETH_HardDrvInit(pDrive);
                    pDrive->debuginfo.rsttimes++;
                }
                result = true;
                break;
            case EN_NETDEV_SETBORAD:
                if(para)
                {
                    emac_broadcast_frames_disable(false);
                    printk("mac broad enable\n\r");
                    pDrive->macstat.bfd = 0;
                }
                else
                {
                    emac_broadcast_frames_disable(true);
                    printk("mac broad disable\n\r");
                    pDrive->macstat.bfd = 1;
                }
                result = true;
                break;
            case EN_NETDEV_SETPOINT:
                if(para)
                {
                    emac_hash_unicast_set(false);   //���Ե���ģʽ
                }
                else
                {
                    emac_hash_unicast_set(true); //ɢ�е���ģʽ
                }
                result = true;
                break;
            case EN_NETDEV_SETMULTI:
                if(para)
                {
                    emac_pass_all_multicasting_set(false);  //pass all multicast
                }
                else
                {
                    emac_pass_all_multicasting_set(true);   //multicast filt
                }
               result = true;
                break;
            case EN_NETDEV_SETRECV:
                if(para)
                {
                    emac_start();
                }
                else
                {
                    emac_stop();
                }
                result = true;
                break;
            case EN_NETDEV_SETSEND:
                if(para)
                {
                    emac_start();
                }
                else
                {
                    emac_stop();
                }
                break;
            case EN_NETDEV_SETMAC:
                memcpy(pDrive->macaddr, (u8 *)para, CN_MACADDR_LEN);
                ETH_HardDrvInit(pDrive);
                pDrive->debuginfo.rsttimes++;
                result = true;
                break;
            case EN_NETDEV_SETMULTIMAC:
                break;
            case EN_NETDEV_SETHOOK:
                pDrive->fnrcvhook = (fnRcvDeal)para;
                break;
            case EN_NETDEV_GTETMAC:
                if((u8 *)para != NULL)
                {
                    memcpy((u8 *)para,pDrive->macaddr, CN_MACADDR_LEN);
                    result = true;
                }
                break;
            case EN_NETDEV_RESET:
                if(para)
                {
                    ETH_HardDrvInit(pDrive);
                    pDrive->debuginfo.rsttimes++;
                    result = true;
                }
                break;
            case EN_NETDEV_LOWPOWER:
                if(para == 1)
                {
                    crm_periph_clock_enable(CRM_EMAC_PERIPH_CLOCK, false); //����ETHMACʱ��
                    GMAC_InLowPowerPinCfg();
                }
                else
                {
                    crm_periph_clock_enable(CRM_EMAC_PERIPH_CLOCK, true);   //ʹ��ETHMACʱ��
                    GMAC_OutLowPowerPinCfg();
                    MacReset(0);
                }
                result = true;
                break;
            case (EN_NETDEV_ADDRFILTER):    //����һ���Ƿ�������а�
                if(para)
                {
                    emac_receive_all_set(false);
                }
                else
                {
                    emac_receive_all_set(true);
                }
                break;
            default:
                result = false;
                break;
        }
        Lock_MutexPost(pDrive->protect);
    }

    return result;
}

//this is the receive task
static ptu32_t __MacRcvTask(void)
{
    struct NetPkg *pkg = NULL;
    struct NetDev *handle = NULL;
    u8 *rawbuf = NULL;
    u16 len = 0;
    tagMacDriver *pDrive = &gMacDriver;
    u32 value = 0;
    u32 resettimes= 0;
    time_t printtime = 0;

    DJY_GetEventPara((ptu32_t *)&handle,NULL);
    value = emac_mmc_received_error_frames_get(MMC_RX_CRC_ERROR);
    if(value > 0)
    {
        printtime = time(NULL);
        printf("[MACRESET:%s Num:0x%08x] CRCERRORCONTER:0x%08x start\n\r",\
                ctime(&printtime),resettimes++,value);
        MacReset(NULL);
        DJY_EventDelay(10*mS);
    }

    while(1)
    {
        //�ж�ģʽ�£�loopcycle = CN_TIMEOUT_FOREVER���ź��������ж�post��
        //��ѯģʽ�£�û�˻�post�ź�������̶���loopcycleΪ������ѯ������
        //�ʣ��ж�ģʽ����ѯģʽ��Դ����һ���ġ�
        Lock_SempPend(pDrive->rcvsync, pDrive->loopcycle);
        pDrive->debuginfo.rxTaskRun++;
        while(1)
        {
            if(Lock_MutexPend(pDrive->protect, CN_TIMEOUT_FOREVER))
            {
                pkg = __MacRcv(handle);
                Lock_MutexPost(pDrive->protect);
            }
            if(NULL != pkg)
            {
                //maybe we have another method like the hardware
                NetDev_FlowCtrl(handle,NetDev_FrameType(PkgGetCurrentBuffer(pkg),
                                                      PkgGetDataLen(pkg)));
                //you could alse use the soft method
                if(NULL != pDrive->fnrcvhook)
                {
                    rawbuf = PkgGetCurrentBuffer(pkg);
                    len = PkgGetDataLen(pkg);
                    pDrive->fnrcvhook(rawbuf,len);
                }
                Link_NetDevPush(handle,pkg);
                PkgTryFreePart(pkg);
                pDrive->debuginfo.rcvPkgTimes++;
                pkg = NULL;
            }
            else
            {
                //here we still use the counter to do the time state check
                NetDev_FlowCtrl(handle,EN_NETDEV_FRAME_LAST);
                break;
            }
        }

        //check if any crc error get then reset the mac
        value = emac_mmc_received_error_frames_get(MMC_RX_CRC_ERROR);
        if(value > 0)
        {
            printtime = time(NULL);
            printf("[MACRESET:%s Num:0x%08x] CRCERRORCONTER:0x%08x running\n\r",\
                    ctime(&printtime),resettimes++,value);
            MacReset(NULL);
            DJY_EventDelay(10*mS);
        }

    }
    return 0;
}
//create the receive task
static bool_t __CreateRcvTask(struct NetDev *handle)
{
    bool_t result = false;
    u16 evttID = 0;
    u16 eventID = 0;

    evttID = DJY_EvttRegist(EN_CORRELATIVE, CN_PRIO_REAL, 0, 1,
                        (ptu32_t (*)(void))__MacRcvTask,NULL, 0x800, "GMACRcvTask");
    if (evttID != CN_EVTT_ID_INVALID)
    {
        eventID=DJY_EventPop(evttID, NULL,  0,(ptu32_t)handle, 0, 0);
        if(eventID != CN_EVENT_ID_INVALID)
        {
            result = true;
        }
        else
        {
            DJY_EvttUnregist(evttID);
        }
    }
    return result;
}

//show the gmac status
//bool_t macdebuginfo(char *param)
bool_t mac(char *param)
{
    s64  time;
    u32  timeS;
    tagMacDriver *pDrive = &gMacDriver;

    time = DJY_GetSysTime();
    timeS = time/(1000*1000);
    if(timeS == 0)
    {
        timeS =1;
    }
    u32 rcvSpeed = pDrive->debuginfo.rcvPkgTimes/timeS;
    u32 intSpeed = pDrive->debuginfo.Ints/timeS;

    printf("*********MAC DEBUG INFO***********\n\r");
    if(pDrive->loop)
    {
        printf("loop/int      :loop\n\r");
        printf("loopcycle     :%d mS\n\r",pDrive->loopcycle);
    }
    else
    {
        printf("loop/int      :int\n\r");
    }

    printf("EthInts       :%d\n\r",pDrive->debuginfo.Ints);
    printf("rcvInts       :%d\n\r",pDrive->debuginfo.rcvInts);
    printf("rcvPkgCnt     :%d\n\r",pDrive->debuginfo.rcvPkgTimes);
    printf("rxoInts       :%d\n\r",pDrive->debuginfo.rxoverInts);
    printf("dmaerrcnt     :%d\n\r",pDrive->debuginfo.dmaerr);
    printf("snd           :%d\n\r",pDrive->debuginfo.sndTimes);
    printf("sndInts       :%d\n\r",pDrive->debuginfo.sndInts);
    printf("sndTimes      :%d\n\r",pDrive->debuginfo.sndTimes);
    printf("sndOkTime     :%d\n\r",pDrive->debuginfo.sndOkTimes);
    printf("sndnobdCnt    :%d\n\r",pDrive->debuginfo.sndnobdCnt);
    printf("rcvnopkgCnt   :%d\n\r",pDrive->debuginfo.nopkgCnts);
    printf("rxTaskRun     :%d\n\r",pDrive->debuginfo.rxTaskRun);
    printf("rcvSpeed      :%d\n\r",rcvSpeed);
    printf("intSpeed      :%d\n\r",intSpeed);
    printf("rstTimes      :%d\n\r",pDrive->debuginfo.rsttimes);
    printf("**********************************\n\r");
    return true;
}

#define CN_ETH_REG_BASE   ((u32)EMAC)
bool_t macreg(char *param)
{
    vu32    i;
    vu32   *addr;
    vu32    value;
    u32     num;

    printf("%-10s%-10s\n\r",\
            "Addr(Hex)","Value(Hex)");
    addr = (u32 *)CN_ETH_REG_BASE;
    num = 8;
    for(i=0;i < num;i++)
    {
        value =*addr;
        printf("%08x  %08x\n\r",(u32)addr,value);
        addr++;
    }

    addr = (u32 *)(CN_ETH_REG_BASE + 0x38);
    num = 10;
    for(i=0;i < num;i++)
    {
        value =*addr;
        printf("%08x  %08x\n\r",(u32)addr,value);
        addr++;
    }

    printf("ETH_MMC CR/SR Register Below:\r\n");

    printf("%-10s%-10s\n\r",\
            "Addr(Hex)","Value(Hex)");
    addr = (u32 *)(CN_ETH_REG_BASE + 0x100);
    num = 5;
    for(i=0;i < num;i++)
    {
        value =*addr;
        printf("%08x  %08x\n\r",(u32)addr,value);
        addr++;
    }

    printf("ETH_MMC Statistics Register Below:\r\n");
    addr = (u32 *)(CN_ETH_REG_BASE + 0x14C);
    value =*addr;
    printf("%08x  %08x\n\r",(u32)addr,value);
    addr = (u32 *)(CN_ETH_REG_BASE + 0x150);
    value =*addr;
    printf("%08x  %08x\n\r",(u32)addr,value);
    addr = (u32 *)(CN_ETH_REG_BASE + 0x168);
    value =*addr;
    printf("%08x  %08x  Transmitted good frames \n\r",(u32)addr,value);
    addr = (u32 *)(CN_ETH_REG_BASE + 0x194);
    value =*addr;
    printf("%08x  %08x  Receive frames with CRC error\n\r",(u32)addr,value);
    addr = (u32 *)(CN_ETH_REG_BASE + 0x198);
    value =*addr;
    printf("%08x  %08x  Receive frames with alignment error\n\r",(u32)addr,value);
    addr = (u32 *)(CN_ETH_REG_BASE + 0x1C4);
    value =*addr;
    printf("%08x  %08x  Received good unicast frames\n\r",(u32)addr,value);

    printf("ETH_DMA Register Below:\r\n");
    printf("%-10s%-10s\n\r",\
            "Addr(Hex)","Value(Hex)");
    addr = (u32 *)(CN_ETH_REG_BASE + 0x1000);
    num = 9;
    for(i=0;i < num;i++)
    {
        value =*addr;
        if((u32)addr == CN_ETH_REG_BASE + 0x101c)
        {
            printf("%08x  %08x  ETH_DMAIER Register\n\r",(u32)addr,value);
        }
        else if((u32)addr == CN_ETH_REG_BASE + 0x1020)
        {
            printf("%08x  %08x  Missed frames and buffer overflow counter\n\r",(u32)addr,value);
        }
        else
        {
            printf("%08x  %08x\n\r",(u32)addr,value);
        }
        addr++;
    }

    return true;
}

bool_t MacReset(char *param)
{
    tagMacDriver *pDrive = &gMacDriver;

    __MacReset(pDrive);
    return true;
}

bool_t macsnden(char *param)
{
    tagMacDriver *pDrive = &gMacDriver;
    if(Lock_MutexPend(pDrive->protect,CN_TIMEOUT_FOREVER))
    {
        emac_start();
        Lock_MutexPost(pDrive->protect);
    }
    return true;
}
bool_t macsnddis(char *param)
{
    tagMacDriver *pDrive = &gMacDriver;

    if(Lock_MutexPend(pDrive->protect,CN_TIMEOUT_FOREVER))
    {
        emac_stop();
        Lock_MutexPost(pDrive->protect);
    }
    return true;
}

bool_t macfilten(char *param)
{
    tagMacDriver *pDrive = &gMacDriver;

    MacCtrl(pDrive->devhandle,EN_NETDEV_ADDRFILTER,1);//��ͨ��ַ���˹���
    return true;
}
bool_t macfiltdis(char *param)
{
    tagMacDriver *pDrive = &gMacDriver;

    MacCtrl(pDrive->devhandle,EN_NETDEV_ADDRFILTER,0);//�رյ�ַ���˹���
    return true;
}


// =============================================================================
// ���ܣ�GMAC������DJYIP������ʼ������
// ������para
// ����ֵ  ��true�ɹ�  falseʧ�ܡ�
// =============================================================================
bool_t ModuleInstall_ETH(void)
{
    tagMacDriver *pDrive = &gMacDriver;
    struct NetDevPara   devpara;
    //GET THE SIGNATURE OF THE DEVICE
    u8 gc_NetMac[CN_MACADDR_LEN];
#if (CFG_ETH_HARD_MAC_ADDR == true)
    u32  signature[3];
    memset(signature,0,sizeof(signature));
    GetCpuID(&signature[0],&signature[1],&signature[2]);
    printk("CPU SIGNATURE:%08X-%08X-%08X\n\r",signature[0],signature[1],signature[2]);
    //use the signature as the mac address
    signature[0] = signature[1]+signature[2];
    memcpy(gc_NetMac,&signature[0],CN_MACADDR_LEN);
    gc_NetMac[0]=0x00;      //����mac�Ĺ涨����һ�ֽ�ĳλ��Ϊ1��ʾ�㲥�����鲥
#else
    u8 gc_NetMac[CN_MACADDR_LEN] ={CFG_ETH_MAC_ADDR0,CFG_ETH_MAC_ADDR1,
                                   CFG_ETH_MAC_ADDR2,CFG_ETH_MAC_ADDR3,
                                   CFG_ETH_MAC_ADDR4,CFG_ETH_MAC_ADDR5};
#endif

    memset((void *)pDrive,0,sizeof(tagMacDriver));
    //copy the config para to the pDrive
    memcpy(pDrive->devname,CFG_ETH_NETCARD_NAME,CN_DEVNAME_LEN-1);
    pDrive->devname[CN_DEVNAME_LEN-1] = '\0';
    memcpy((void *)pDrive->macaddr,gc_NetMac,CN_MACADDR_LEN);
    if(CFG_ETH_LOOP_ENABLE)
    {
        pDrive->loopcycle = CFG_ETH_LOOP_CYCLE;
    }
    else
        pDrive->loopcycle = CN_TIMEOUT_FOREVER;
    pDrive->fnrcvhook = NULL;
    //set the work mode and speed
    pDrive->macstat.pm = 0;         //primmiscuous mode:= 1,no MAC addr filte
    pDrive->macstat.duplex = 1;    //duplex full
    pDrive->macstat.speed = 1;     //100 Mb
    pDrive->macstat.bfd = 0;       //broadcast frames disable
    pDrive->macstat.mii = 0;       //use RMII mode

    ETH_HardDrvInit(pDrive);

    //all the configuration has set in the pDrive now,we need some sys assistant
    //application some semphore and mutex
    pDrive->rcvsync = Lock_SempCreate(1,1,CN_BLOCK_FIFO,NULL);
    if(NULL == pDrive->rcvsync)
    {
        goto RCVSYNC_FAILED;
    }
    pDrive->protect = Lock_MutexCreate(NULL);
    if(NULL == pDrive->protect)
    {
        goto DEVPROTECT_FAILED;
    }
    MacCtrl(pDrive->devhandle,EN_NETDEV_ADDRFILTER,1);//��ͨ��ַ���˹���

    //install the net device
    devpara.ifctrl = MacCtrl;
    devpara.ifsend = MacSnd;
    devpara.iftype = EN_LINK_ETHERNET;
    devpara.devfunc = CN_IPDEV_ALL;
    memcpy(devpara.mac, gc_NetMac, CN_MACADDR_LEN);
    devpara.name = (char *)pDrive->devname;
    devpara.mtu = CN_ETH_MTU;
    devpara.Private = (void *)pDrive;
    pDrive->devhandle = NetDev_Install(&devpara);
    if(NULL == pDrive->devhandle)
    {
        goto NetInstallFailed;
    }
    if(false == __CreateRcvTask(pDrive->devhandle))
    {
        goto RcvTaskFailed;
    }

    if(false == CFG_ETH_LOOP_ENABLE)
    {
        Int_Register(CN_INT_LINE_EMAC);
        Int_SettoAsynSignal(CN_INT_LINE_EMAC);
        Int_ClearLine(CN_INT_LINE_EMAC);
        Int_IsrConnect(CN_INT_LINE_EMAC,ETH_IntHandler);
        Int_ContactLine(CN_INT_LINE_EMAC);
    }
    info_printf("eth","%s:Install Net Device %s success\n\r",__FUNCTION__,pDrive->devname);
    return true;

RcvTaskFailed:
    NetDev_Uninstall(pDrive->devname);
NetInstallFailed:
    Lock_MutexDelete(pDrive->protect);
    pDrive->protect = NULL;
DEVPROTECT_FAILED:
    Lock_SempDelete(pDrive->rcvsync);
    pDrive->rcvsync = NULL;
RCVSYNC_FAILED:
    error_printf("bspETH","Install Net Device %s failed\n\r",pDrive->devname);
    return false;
}


u8 GMAC_MdioR(u8 dev, u8 reg, u16 *value)
{
    u16 rValue;
    if (emac_phy_register_read(dev, reg, &rValue) == ERROR)
    {
        return 0;
    }
    *value = (u16)rValue;
    return 1;
}
u8 GMAC_MdioW(u8 dev, u8 reg, u16 value)
{
    if (emac_phy_register_write(dev, reg, value) == ERROR)
    {
        return 0;
    }
    return 1;
}


ADD_TO_ROUTINE_SHELL(macfiltdis,macfiltdis,"usage:MacAddrDis, receive all frame(don't care MAC filter)");
ADD_TO_ROUTINE_SHELL(macfilten,macfilten,"usage:MacAddrEn, don't receive all frame(MAC filter)");
ADD_TO_ROUTINE_SHELL(macsnden,macsnden,"usage:MacSndEn");
ADD_TO_ROUTINE_SHELL(macsnddis,macsnddis,"usage:MacSndDis");
ADD_TO_ROUTINE_SHELL(macreset,MacReset,"usage:reset gmac");
ADD_TO_ROUTINE_SHELL(macreg,macreg,"usage:MacReg");
ADD_TO_ROUTINE_SHELL(mac,mac,"usage:gmac");
