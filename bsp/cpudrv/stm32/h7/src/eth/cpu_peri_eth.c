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
#include "djyos.h"

#include <cpu_peri.h>
#include <board-config.h>
#include <sys/socket.h>
#include <netbsp.h>
#include "shell.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_eth.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//  extern bool_t ModuleInstall_ETH(const char *devname, u8 *macaddress,\
//                          bool_t loop,u32 loopcycle,\
//                          bool_t (*rcvHook)(u8 *buf, u16 len));
//   static u8 mac_addr[]={CFG_MAC_ADDR0,CFG_MAC_ADDR1,CFG_MAC_ADDR2,CFG_MAC_ADDR3,CFG_MAC_ADDR4,CFG_MAC_ADDR5};
//   ModuleInstall_ETH(CFG_ETH_DEV_NAME,mac_addr,CFG_ETH_LOOP_ENABLE,CFG_ETH_LOOP_CYCLE,NULL);
//%$#@end initcode  ****��ʼ���������
//%$#@describe      ****���������ʼ
//component name:"cpu_peri_eth" //CPU��mac����
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"djyip","lock","int","tcpip",      //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������������������������none����ʾ�������������
                                //����������������������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header    //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef cpu_peri_eth   //****�������Ƿ��Ѿ����ú�
#warning    cpu_peri_eth�������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,0,255,
#define CFG_MAC_ADDR0           00           //"������ַ0",
#define CFG_MAC_ADDR1           00           //"������ַ1",
#define CFG_MAC_ADDR2           00           //"������ַ2",
#define CFG_MAC_ADDR3           00           //"������ַ3",
#define CFG_MAC_ADDR4           00           //"������ַ4",
#define CFG_MAC_ADDR5           00           //"������ַ5",
//%$#@num,1000,10000,
#define CFG_ETH_LOOP_CYCLE      1000         //"������ѯ��������",
//%$#@enum,true,false,
#define CFG_ETH_LOOP_ENABLE     true        //"���������Ƿ���ѯ",
//%$#@string,1,32,
#define CFG_ETH_DEV_NAME        "STM32H7_ETH"//"��������",
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure
// =============================================================================

#define CN_DEVNAME_LEN 32
//this is the mac receive hook, if any hook rcv,then the data will goto the hook
//the receive data goto the stack default
typedef bool_t (*fnRcvDeal)(u8 *buf, u16 len);

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

ETH_HandleTypeDef sEthHandle;
typedef struct
{
    //os member
    struct SemaphoreLCB     *rcvsync;          //activate the receive task
    struct MutexLCB         *protect;          //protect the device
    void*                   devhandle;        //returned by the tcpip stack
    char                    devname[CN_DEVNAME_LEN];
    //hardware
    ETH_HandleTypeDef       *EthHandle;
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
    vu32 loop:1;          //1 use the loop mode while 0 interrupt mode
    vu32 loopcycle:31;    //unit:ms
    fnRcvDeal     fnrcvhook;                  //rcv hook function
    tagMacDebug   debuginfo;                  //debug info
}tagMacDriver;
static tagMacDriver  gMacDriver;

#ifdef __GNUC__

//define the descriptors and buffers
#define EthRxDescs      ETH_RX_DESC_CNT
#define EthTxDescs      ETH_TX_DESC_CNT
#define EthRxBufSize    (1528UL)
#define EthTxBufSize    (1528UL)
//��̫���������ͻ�����
ETH_DMADescTypeDef DMARxDscrTab[EthRxDescs] __attribute__((section(".nocacheram.DMARxDscrTab"))); /* Ethernet Rx DMA Descriptors */
ETH_DMADescTypeDef DMATxDscrTab[EthTxDescs] __attribute__((section(".nocacheram.DMATxDscrTab")));   /* Ethernet Tx DMA Descriptors */
uint8_t Rx_Buff[EthRxDescs][EthRxBufSize] __attribute__((section(".nocacheram.Rx_Buff"))); /* Ethernet Receive Buffers */
#endif
static u8 gTxBuffer[EthTxBufSize]__attribute__((section(".nocacheram.TxBuffer")));;      //for sending copy frame
static ETH_TxPacketConfig TxConfig;

//����������ʹ�õ�0X30040000��ram�ڴ汣��
void NETMPU_Config(void)
{
    MPU_Region_InitTypeDef MPU_InitStruct;

    HAL_MPU_Disable();
    MPU_InitStruct.Enable=MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress=0x30040000;
    MPU_InitStruct.Size=MPU_REGION_SIZE_256B;
    MPU_InitStruct.AccessPermission=MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable=MPU_ACCESS_BUFFERABLE;
    MPU_InitStruct.IsCacheable=MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsShareable=MPU_ACCESS_SHAREABLE;
    MPU_InitStruct.Number=MPU_REGION_NUMBER5;
    MPU_InitStruct.TypeExtField=MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable=0x00;
    MPU_InitStruct.DisableExec=MPU_INSTRUCTION_ACCESS_ENABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

// HAL���е����˸ú���
void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Ethernett MSP init: RMII Mode */

    /* Enable GPIOs clocks */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

  /* Ethernet pins configuration ************************************************/
    /*
          RMII_REF_CLK ----------------------> PA1
          RMII_MDIO -------------------------> PA2
          RMII_MDC --------------------------> PC1
          RMII_MII_CRS_DV -------------------> PA7
          RMII_MII_RXD0 ---------------------> PC4
          RMII_MII_RXD1 ---------------------> PC5
          RMII_MII_RXER ---------------------> PG2
          RMII_MII_TX_EN --------------------> PG11
          RMII_MII_TXD0 ---------------------> PG13
          RMII_MII_TXD1 ---------------------> PB13
    */

    /* Configure PA1, PA2 and PA7 */
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Alternate = GPIO_AF11_ETH;
    GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure PB13 */
    GPIO_InitStructure.Pin = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Configure PC1, PC4 and PC5 */
    GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Configure PG2, PG11, PG13 and PG14 */
    GPIO_InitStructure.Pin =  GPIO_PIN_2 | GPIO_PIN_11 | GPIO_PIN_13;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

    /* Enable Ethernet clocks */
    __HAL_RCC_ETH1MAC_CLK_ENABLE();
    __HAL_RCC_ETH1TX_CLK_ENABLE();
    __HAL_RCC_ETH1RX_CLK_ENABLE();
}

static void __macbitsset(vu32 *reg,u32 bits)
{
    vu32 value;
    value = *reg;
    value |=bits;
    *reg = value;
    return;
}
static void __macbitsclear(vu32 *reg,u32 bits)
{
    vu32 value;
    value = *reg;
    value &=(~bits);
    *reg = value;
    return;
}

static void ETH_HardDrvInit(tagMacDriver *pDrive)
{
    ETH_HandleTypeDef *heth = NULL;

    heth = pDrive->EthHandle;
    if(heth==NULL)
        return;
    // configure ethernet peripheral (GPIOs, clocks, MAC, DMA)
    HAL_ETH_Init(heth);

//    HAL_ETH_SetMDIOClockRange(heth);

    for(int idx = 0; idx < ETH_RX_DESC_CNT; idx ++)
    {
        HAL_ETH_DescAssignMemory(heth, idx, Rx_Buff[idx], NULL);
    }
    memset(&TxConfig, 0 , sizeof(ETH_TxPacketConfig));
    TxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
    TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
    TxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;
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

static tagNetPkg *__MacRcv(void* devhandle)
{
    tagNetPkg         *pkg = NULL;
    tagMacDriver      *pDrive;
    ETH_HandleTypeDef *EthHandle;
    ETH_BufferTypeDef  RxBuff;
    volatile ETH_DMADescTypeDef *dmarxdesc;
    u16 CopyBytes,len;
    u8   *dst,*src;

    pDrive = &gMacDriver;
    EthHandle = pDrive->EthHandle;
    if(EthHandle==NULL)
        return NULL;
    // get received frame
//    if(HAL_ETH_IsRxDataAvailable(EthHandle)==0)
//        return NULL;
    SCB_CleanInvalidateDCache();    //��Ч���������Dcache
    if(HAL_ETH_GetRxDataBuffer(EthHandle, &RxBuff) != HAL_OK)
        return NULL;
//    HAL_ETH_GetRxDataBuffer(EthHandle,&RxBuff);
//    HAL_ETH_GetRxDataLength(EthHandle,&len);
    len = RxBuff.len;
    HAL_ETH_BuildRxDescriptors(EthHandle);
    CopyBytes = len;

    if(len > 0)
    {
        pkg = PkgMalloc(len,CN_PKLGLST_END);
        if(NULL == pkg)
            pDrive->debuginfo.nopkgCnts++;
    }

    if(NULL != pkg)
    {
        dst = (u8 *)(pkg->buf +pkg->offset);
        src = (u8 *)RxBuff.buffer;
        if(CopyBytes > EthRxBufSize)
        {
            CopyBytes = EthRxBufSize;
        }
        memcpy( dst, src, CopyBytes);
        pkg->datalen = len;
    }
    else        //if malloc pkg failed
    {
        return NULL;
    }

    return pkg;
}

static bool_t MacSnd(void* handle,tagNetPkg * pkg,u32 framelen, u32 netdevtask)
{
    bool_t             result;
    tagMacDriver      *pDrive;
    ETH_HandleTypeDef *EthHandle;
//    ETH_DMADescTypeDef *DmaTxDesc;
    tagNetPkg         *tmppkg;
    u8                *dst,*src;
    u16                len=0;
    ETH_BufferTypeDef Txbuffer[ETH_TX_DESC_CNT];

    result = false;
    pDrive = &gMacDriver;
    EthHandle = pDrive->EthHandle;
    pDrive->debuginfo.sndTimes++;
    memset(Txbuffer, 0 , ETH_TX_DESC_CNT*sizeof(ETH_BufferTypeDef));
    if((NULL == handle)||(NULL == pkg))
        return result;

    if(Lock_MutexPend(pDrive->protect,CN_TIMEOUT_FOREVER))
    {
        //copy datas to static frame buffer
        tmppkg = pkg;
        dst      = &gTxBuffer[0];
        do
        {
            src = (tmppkg->buf + tmppkg->offset);
            memcpy(dst,src,tmppkg->datalen);
            dst      += tmppkg->datalen;
            len      += tmppkg->datalen;
            if(PKG_ISLISTEND(tmppkg))
            {
                tmppkg = NULL;
                break;
            }
            else
            {
                tmppkg = tmppkg->partnext;
            }
        }while(NULL != tmppkg );

        if(len < EthTxBufSize)
        {
            src = &gTxBuffer[0];
            Txbuffer[0].buffer = src;
            Txbuffer[0].len = len;
            TxConfig.Length=len;
            TxConfig.TxBuffer=Txbuffer;
            SCB_CleanInvalidateDCache();    //��Ч�������Dcache
            //Prepare transmit descriptors to give to DMA
            if(HAL_OK == HAL_ETH_Transmit_IT(EthHandle,&TxConfig))
            {
                pDrive->debuginfo.sndOkTimes++;
                result = true;
            }
        }
        else
        {
            pDrive->debuginfo.sndPkgTooLongCnt++;
        }

        Lock_MutexPost(pDrive->protect);
    }

    return result;
}

u32 ETH_SendData(u8 *buf,u32 len)
{
    tagNetPkg          pkg;
    tagMacDriver      *pDrive;

    pDrive = &gMacDriver;

    pkg.partnext = NULL;
    pkg.pkgflag  = (1<<0);  //ֻ��һ����
    pkg.offset   = 0;
    pkg.datalen  = len;
    pkg.buf      = buf;
    if(MacSnd(pDrive->devhandle,&pkg,len,0))
    {
        return len;
    }
    else
    {
        return 0;
    }
}
//This is the interrut handler
u32 ETH_IntHandler(ufast_t IntLine)
{
    tagMacDriver *pDrive;
    ETH_HandleTypeDef *EthHandle;

    pDrive = &gMacDriver;
    EthHandle = pDrive->EthHandle;

    pDrive->debuginfo.Ints++;
    /* Packet received */
    if (__HAL_ETH_DMA_GET_IT(EthHandle, ETH_DMACSR_RI) && __HAL_ETH_DMA_GET_IT_SOURCE(EthHandle, ETH_DMACIER_RIE))
    {
      /* Call this function to update handle fields */
      HAL_ETH_IsRxDataAvailable(EthHandle);

      /* Receive complete callback */
      pDrive->debuginfo.rcvInts++;
      Lock_SempPost(pDrive->rcvsync);

      /* Clear the Eth DMA Rx IT pending bits */
      __HAL_ETH_DMA_CLEAR_IT(EthHandle, ETH_DMACSR_RI | ETH_DMACSR_NIS);
    }

    /* Packet transmitted */
    if (__HAL_ETH_DMA_GET_IT(EthHandle, ETH_DMACSR_TI) && __HAL_ETH_DMA_GET_IT_SOURCE(EthHandle, ETH_DMACIER_TIE))
    {
      /* Transfer complete callback */
//      HAL_ETH_TxCpltCallback(EthHandle);

      /* Clear the Eth DMA Tx IT pending bits */
      __HAL_ETH_DMA_CLEAR_IT(EthHandle, ETH_DMACSR_TI | ETH_DMACSR_NIS);

      EthHandle->ErrorCode = HAL_ETH_ERROR_NONE;
      EthHandle->gState = HAL_ETH_STATE_READY;
    }


    /* ETH DMA Error */
    if(__HAL_ETH_DMA_GET_IT(EthHandle, ETH_DMACSR_AIS) && __HAL_ETH_DMA_GET_IT_SOURCE(EthHandle, ETH_DMACIER_AIE))
    {
      EthHandle->ErrorCode |= HAL_ETH_ERROR_DMA;

      /* if fatal bus error occured */
      if (__HAL_ETH_DMA_GET_IT(EthHandle, ETH_DMACSR_FBE) && __HAL_ETH_DMA_GET_IT_SOURCE(EthHandle, ETH_DMACIER_FBEE))
      {
        /* Get DMA error code  */
        EthHandle->DMAErrorCode = __HAL_ETH_DMA_GET_IT(EthHandle, (ETH_DMACSR_FBE | ETH_DMACSR_TPS | ETH_DMACSR_RPS));

        /* Disable all interrupts */
        __HAL_ETH_DMA_DISABLE_IT(EthHandle, ETH_DMACIER_NIE | ETH_DMACIER_AIE);

        /* Set HAL state to ERROR */
        EthHandle->gState = HAL_ETH_STATE_ERROR;
      }
      else
      {
        /* Get DMA error status  */
        EthHandle->DMAErrorCode = __HAL_ETH_DMA_GET_IT(EthHandle, (ETH_DMACSR_CDE | ETH_DMACSR_ETI | ETH_DMACSR_RWT |
                                                         ETH_DMACSR_RBU | ETH_DMACSR_AIS));

        /* Clear the interrupt summary flag */
        __HAL_ETH_DMA_CLEAR_IT(EthHandle, (ETH_DMACSR_CDE | ETH_DMACSR_ETI | ETH_DMACSR_RWT |
                                      ETH_DMACSR_RBU | ETH_DMACSR_AIS));
      }

      /* Ethernet Error callback */
      HAL_ETH_DMAErrorCallback(EthHandle);
    }

    /* ETH MAC Error IT */
    if(__HAL_ETH_MAC_GET_IT(EthHandle, (ETH_MACIER_RXSTSIE | ETH_MACIER_TXSTSIE)))
    {
      /* Get MAC Rx Tx status and clear Status register pending bit */
      EthHandle->MACErrorCode = READ_REG(EthHandle->Instance->MACRXTXSR);

      EthHandle->gState = HAL_ETH_STATE_ERROR;

      /* Ethernet PMT callback */
      HAL_ETH_MACErrorCallback(EthHandle);

      EthHandle->MACErrorCode = (uint32_t)(0x0U);
    }

    /* ETH PMT IT */
    if(__HAL_ETH_MAC_GET_IT(EthHandle, ETH_MAC_PMT_IT))
    {
      /* Get MAC Wake-up source and clear the status register pending bit */
        EthHandle->MACWakeUpEvent = READ_BIT(EthHandle->Instance->MACPCSR, (ETH_MACPCSR_RWKPRCVD | ETH_MACPCSR_MGKPRCVD));

      /* Ethernet PMT callback */
      HAL_ETH_PMTCallback(EthHandle);

      EthHandle->MACWakeUpEvent = (uint32_t)(0x0U);
    }

    /* ETH EEE IT */
    if(__HAL_ETH_MAC_GET_IT(EthHandle, ETH_MAC_LPI_IT))
    {
      /* Get MAC LPI interrupt source and clear the status register pending bit */
        EthHandle->MACLPIEvent = READ_BIT(EthHandle->Instance->MACPCSR, 0x0000000FU);

      /* Ethernet EEE callback */
      HAL_ETH_EEECallback(EthHandle);

      EthHandle->MACLPIEvent = (uint32_t)(0x0U);
    }

    /* check ETH WAKEUP exti flag */
    if(__HAL_ETH_WAKEUP_EXTI_GET_FLAG(ETH_WAKEUP_EXTI_LINE) != RESET)
    {
      /* Clear ETH WAKEUP Exti pending bit */
      __HAL_ETH_WAKEUP_EXTI_CLEAR_FLAG(ETH_WAKEUP_EXTI_LINE);
      /* ETH WAKEUP interrupt user callback */
      HAL_ETH_WakeUpCallback(EthHandle);
    }
    return 0;
}
//mac control function
#define EN_NETDEV_ADDRFILTER (EN_NETDEV_CMDLAST + 1)
static bool_t MacCtrl(void *devhandle,u8 cmd,ptu32_t para)
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
//                    __macbitsclear(&pDrive->EthHandle->Instance->MACFFR,ETH_MACFFR_BFD);
                    printk("mac broad enable\n\r");
                    pDrive->macstat.bfd = 0;
                }
                else
                {
//                    __macbitsset(&pDrive->EthHandle->Instance->MACFFR,ETH_MACFFR_BFD);
                    printk("mac broad disable\n\r");
                    pDrive->macstat.bfd = 1;
                }
                result = true;
                break;
            case EN_NETDEV_SETPOINT:
                if(para)
                {
//                    __macbitsclear(&pDrive->EthHandle->Instance->MACFFR,
//                            ETH_MACFFR_HU);//���Ե���ģʽ
                }
                else
                {
//                    __macbitsset(&pDrive->EthHandle->Instance->MACFFR,
//                            ETH_MACFFR_HU);//ɢ�е���ģʽ
                }
                result = true;
                break;
            case EN_NETDEV_SETMULTI:
                if(para)
                {
//                   __macbitsset(&pDrive->EthHandle->Instance->MACFFR,
//                           ETH_MACFFR_PAM);//pass all multicast
                }
                else
                {
//                    __macbitsclear(&pDrive->EthHandle->Instance->MACFFR,
//                            ETH_MACFFR_PAM);//multicast filt
                }
               result = true;
                break;
            case EN_NETDEV_SETRECV:
                if(para)
                {
//                    HAL_ETH_StartReceive(pDrive->EthHandle);
                }
                else
                {
//                    HAL_ETH_StopReceive(pDrive->EthHandle);
                }
                result = true;
                break;
            case EN_NETDEV_SETSEND:
                if(para)
                {
//                    HAL_ETH_StartTransmit(pDrive->EthHandle);
                }
                else
                {
//                    HAL_ETH_StopTransmit(pDrive->EthHandle);
                }
                break;
            case EN_NETDEV_SETMAC:
                memcpy(pDrive->macaddr,(u8 *)para, CN_MACADDR_LEN);
                ETH_HardDrvInit(pDrive);
                pDrive->debuginfo.rsttimes++;
                result = true;
                break;
            case EN_NETDEV_SETMULTIMAC:
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
            case (EN_NETDEV_ADDRFILTER):    //����һ���Ƿ�������а�
                if(para)
                {
//                    __macbitsclear(&pDrive->EthHandle->Instance->MACFFR,
//                            ETH_MACFFR_RA);//addr filter
                }
                else
                {
//                    __macbitsset(&pDrive->EthHandle->Instance->MACFFR,
//                                               ETH_MACFFR_RA);//pass all pkg
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
    tagNetPkg *pkg=NULL;
    void      *handle;
    u8        *rawbuf;
    u16        len;
    tagMacDriver      *pDrive;
    pDrive = &gMacDriver;



    u32 *addr;
    u32 value;
    u32 resettimes= 0;
    time_t printtime;
    bool_t MacReset(char *param);

    Djy_GetEventPara((ptu32_t *)&handle,NULL);

    addr = (u32 *)((u32)ETH + 0x194);
    value =*addr;
    if(value > 0)
    {
        printtime = time(NULL);
        printf("[MACRESET:%s Num:0x%08x] CRCERRORCONTER:0x%08x start\n\r",\
                ctime(&printtime),resettimes++,value);
        MacReset(NULL);
        Djy_EventDelay(10*mS);
    }

    while(1)
    {
        Lock_SempPend(pDrive->rcvsync,pDrive->loopcycle);
        pDrive->debuginfo.rxTaskRun++;
        while(1)
        {
            if(Lock_MutexPend(pDrive->protect,CN_TIMEOUT_FOREVER))
            {
                pkg = __MacRcv(handle);
                Lock_MutexPost(pDrive->protect);
            }
            if(NULL != pkg)
            {
                //maybe we have another method like the hardware
//              NetDevFlowCounter(handle,NetDevFrameType(pkg->buf+ pkg->offset,pkg->datalen));
                //you could alse use the soft method
                if(NULL != pDrive->fnrcvhook)
                {
                    rawbuf = pkg->buf + pkg->offset;
                    len = pkg->datalen;
                    pDrive->fnrcvhook(rawbuf,len);
                }
                else
                {
                    NetDevPush(handle,pkg);
                }
                PkgTryFreePart(pkg);
                pDrive->debuginfo.rcvPkgTimes++;
            }
            else
            {
                //here we still use the counter to do the time state check
//              NetDevFlowCounter(handle,EN_NETDEV_FRAME_LAST);
                break;
            }
        }

        //check if any crc error get then reset the mac
        addr = (u32 *)((u32)ETH + 0x194);
        value =*addr;
        if(value > 0)
        {
            printtime = time(NULL);
            printf("[MACRESET:%s Num:0x%08x] CRCERRORCONTER:0x%08x running\n\r",\
                    ctime(&printtime),resettimes++,value);
            MacReset(NULL);
            Djy_EventDelay(10*mS);
        }

    }
    return 0;
}
//create the receive task
static bool_t __CreateRcvTask(ptu32_t handle)
{
    bool_t result = false;
    u16 evttID;
    u16 eventID;

    evttID = Djy_EvttRegist(EN_CORRELATIVE, CN_PRIO_REAL, 0, 1,
        (ptu32_t (*)(void))__MacRcvTask,NULL, 0x800, "GMACRcvTask");
    if (evttID != CN_EVTT_ID_INVALID)
    {
        eventID=Djy_EventPop(evttID, NULL,  0,(ptu32_t)handle, 0, 0);
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

//show the gmac status
//bool_t macdebuginfo(char *param)
bool_t mac(char *param)
{
    s64  time;
    u32  timeS;
    tagMacDriver      *pDrive;
    pDrive = &gMacDriver;

    time = DjyGetSysTime();
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
    printf("rcvnopkgCnt    :%d\n\r",pDrive->debuginfo.nopkgCnts);
    printf("rxTaskRun     :%d\n\r",pDrive->debuginfo.rxTaskRun);
    printf("rcvSpeed      :%d\n\r",rcvSpeed);
    printf("intSpeed      :%d\n\r",intSpeed);
    printf("rstTimes      :%d\n\r",pDrive->debuginfo.rsttimes);
    printf("**********************************\n\r");
    return true;
}

#define CN_GMAC_REG_BASE   ((u32)ETH)
bool_t macreg(char *param)
{
    vu32    i;
    vu32   *addr;
    vu32    value;
    u32     num;

    printf("%-10s%-10s\n\r",\
            "Addr(Hex)","Value(Hex)");
    addr = (u32 *)CN_GMAC_REG_BASE;
    num = 8;
    for(i=0;i < num;i++)
    {
        value =*addr;
        printf("%08x  %08x\n\r",(u32)addr,value);
        addr++;
    }

    addr = (u32 *)(CN_GMAC_REG_BASE + 0x34);
    num = 11;
    for(i=0;i < num;i++)
    {
        value =*addr;
        printf("%08x  %08x\n\r",(u32)addr,value);
        addr++;
    }

    printf("ETH_MMC CR/SR Register Below:\r\n");

    printf("%-10s%-10s\n\r",\
            "Addr(Hex)","Value(Hex)");
    addr = (u32 *)(CN_GMAC_REG_BASE + 0x100);
    num = 7;
    for(i=0;i < num;i++)
    {
        value =*addr;
        printf("%08x  %08x\n\r",(u32)addr,value);
        addr++;
    }

    printf("ETH_MMC Statistics Register Below:\r\n");
    addr = (u32 *)(CN_GMAC_REG_BASE + 0x14C);
    value =*addr;
    printf("%08x  %08x\n\r",(u32)addr,value);
    addr = (u32 *)(CN_GMAC_REG_BASE + 0x150);
    value =*addr;
    printf("%08x  %08x\n\r",(u32)addr,value);
    addr = (u32 *)(CN_GMAC_REG_BASE + 0x168);
    value =*addr;
    printf("%08x  %08x  Transmitted good frames \n\r",(u32)addr,value);
    addr = (u32 *)(CN_GMAC_REG_BASE + 0x194);
    value =*addr;
    printf("%08x  %08x  Receive frames with CRC error\n\r",(u32)addr,value);
    addr = (u32 *)(CN_GMAC_REG_BASE + 0x198);
    value =*addr;
    printf("%08x  %08x  Receive frames with alignment error\n\r",(u32)addr,value);
    addr = (u32 *)(CN_GMAC_REG_BASE + 0x1C4);
    value =*addr;
    printf("%08x  %08x  Received good unicast frames\n\r",(u32)addr,value);

    printf("ETH_DMA Register Below:\r\n");
    printf("%-10s%-10s\n\r",\
            "Addr(Hex)","Value(Hex)");
    addr = (u32 *)(CN_GMAC_REG_BASE + 0x1000);
    num = 10;
    for(i=0;i < num;i++)
    {
        value =*addr;
        if((u32)addr == CN_GMAC_REG_BASE + 0x101c)
        {
            printf("%08x  %08x  ETH_DMAIER Register\n\r",(u32)addr,value);
        }
        else if((u32)addr == CN_GMAC_REG_BASE + 0x1020)
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
    tagMacDriver   *pDrive = &gMacDriver;

    __MacReset(pDrive);
    return true;
}

bool_t macreset(char *param)
{
    tagMacDriver   *pDrive = &gMacDriver;

    __MacReset(pDrive);
    return true;
}

bool_t macsnden(char *param)
{
    tagMacDriver      *pDrive;


    pDrive = &gMacDriver;
    if(Lock_MutexPend(pDrive->protect,CN_TIMEOUT_FOREVER))
    {
        HAL_ETH_Start(pDrive->EthHandle);
        Lock_MutexPost(pDrive->protect);
    }
    return true;
}
bool_t macsnddis(char *param)
{
    tagMacDriver      *pDrive;

    pDrive = &gMacDriver;
    if(Lock_MutexPend(pDrive->protect,CN_TIMEOUT_FOREVER))
    {
        HAL_ETH_Stop(pDrive->EthHandle);
        Lock_MutexPost(pDrive->protect);
    }
    return true;
}

bool_t macfilten(char *param)
{
    tagMacDriver      *pDrive;

    pDrive = &gMacDriver;
    MacCtrl(pDrive->devhandle,EN_NETDEV_ADDRFILTER,1);//��ͨ��ַ���˹���
    return true;
}
bool_t macfiltdis(char *param)
{
    tagMacDriver      *pDrive;

    pDrive = &gMacDriver;
    MacCtrl(pDrive->devhandle,EN_NETDEV_ADDRFILTER,0);//�رյ�ַ���˹���
    return true;
}

void ETH_GetMACConfig(ETH_MACConfigTypeDef *macconf)
{
    HAL_ETH_GetMACConfig(&sEthHandle, macconf);
}

void ETH_SetMACConfig(ETH_MACConfigTypeDef *macconf)
{
    HAL_ETH_SetMACConfig(&sEthHandle, macconf);
}

void ETH_Start(void)
{
    HAL_ETH_Start_IT(&sEthHandle);
}

/*******************************************************************************
                       PHI IO Functions
*******************************************************************************/
/**
  * @brief  Initializes the MDIO interface GPIO and clocks.
  * @param  None
  * @retval 0 if OK, -1 if ERROR
  */
int32_t ETH_PHY_IO_Init(void)
{
  /* We assume that MDIO GPIO configuration is already done
     in the ETH_MspInit() else it should be done here
  */

  /* Configure the MDIO Clock */
  HAL_ETH_SetMDIOClockRange(&sEthHandle);

  return 0;
}

/**
  * @brief  De-Initializes the MDIO interface .
  * @param  None
  * @retval 0 if OK, -1 if ERROR
  */
int32_t ETH_PHY_IO_DeInit (void)
{
  return 0;
}

/**
  * @brief  Read a PHY register through the MDIO interface.
  * @param  DevAddr: PHY port address
  * @param  RegAddr: PHY register address
  * @param  pRegVal: pointer to hold the register value
  * @retval 0 if OK -1 if Error
  */
int32_t ETH_PHY_IO_ReadReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t *pRegVal)
{
  if(HAL_ETH_ReadPHYRegister(&sEthHandle, DevAddr, RegAddr, pRegVal) != HAL_OK)
  {
    return -1;
  }

  return 0;
}

/**
  * @brief  Write a value to a PHY register through the MDIO interface.
  * @param  DevAddr: PHY port address
  * @param  RegAddr: PHY register address
  * @param  RegVal: Value to be written
  * @retval 0 if OK -1 if Error
  */
int32_t ETH_PHY_IO_WriteReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t RegVal)
{
  if(HAL_ETH_WritePHYRegister(&sEthHandle, DevAddr, RegAddr, RegVal) != HAL_OK)
  {
    return -1;
  }

  return 0;
}

/**
  * @brief  Get the time in millisecons used for internal PHY driver process.
  * @retval Time value
  */
int32_t ETH_PHY_IO_GetTick(void)
{
  return HAL_GetTick();
}

// =============================================================================
// ���ܣ�GMAC������DJYIP������ʼ������
// ������para
// ����ֵ  ��true�ɹ�  falseʧ�ܡ�
// =============================================================================
bool_t ModuleInstall_ETH(const char *devname, u8 *macaddress,\
                          bool_t loop,u32 loopcycle,\
                          bool_t (*rcvHook)(u8 *buf, u16 len))
{
    tagMacDriver   *pDrive = &gMacDriver;
    tagNetDevPara   devpara;

    memset((void *)pDrive,0,sizeof(tagMacDriver));
    //copy the config para to the pDrive
    memcpy(pDrive->devname,devname,CN_DEVNAME_LEN-1);
    memcpy((void *)pDrive->macaddr,macaddress,CN_MACADDR_LEN);
    if(loop)
    {
        pDrive->loop = 1;
    }
    pDrive->loopcycle = loopcycle;
    pDrive->fnrcvhook = rcvHook;
    //set the work mode and speed
    pDrive->macstat.pm = 0;         //primmiscuous mode:= 1,no MAC addr filte
    pDrive->macstat.duplex = 1;    //duplex full
    pDrive->macstat.speed = 1;     //100 Mb
    pDrive->macstat.bfd = 0;       //broadcast frames disable
    pDrive->macstat.mii = 0;       //use RMII mode

    pDrive->EthHandle = &sEthHandle;
    pDrive->EthHandle->Instance = ETH;
    pDrive->EthHandle->Init.MACAddr = pDrive->macaddr;
    pDrive->EthHandle->Init.MediaInterface = HAL_ETH_RMII_MODE;
    pDrive->EthHandle->Init.RxDesc=DMARxDscrTab;                 //����������
    pDrive->EthHandle->Init.TxDesc=DMATxDscrTab;                 //����������
    pDrive->EthHandle->Init.RxBuffLen=EthRxBufSize;       //���ճ���
    NETMPU_Config();
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
    devpara.devfunc = CN_IPDEV_NONE;
    memcpy(devpara.mac,macaddress,6);
    devpara.name = (char *)pDrive->devname;
    devpara.private = 0;
    devpara.mtu = 1528;
    devpara.private = (ptu32_t)pDrive;
    pDrive->devhandle = NetDevInstall(&devpara);
    if(NULL == pDrive->devhandle)
    {
        goto NetInstallFailed;
    }
    if(false == __CreateRcvTask(pDrive->devhandle))
    {
        goto RcvTaskFailed;
    }

    if(false == loop)
    {
        Int_Register(CN_INT_LINE_ETH);
        Int_SettoAsynSignal(CN_INT_LINE_ETH);
        Int_ClearLine(CN_INT_LINE_ETH);
        Int_IsrConnect(CN_INT_LINE_ETH,ETH_IntHandler);
        Int_ContactLine(CN_INT_LINE_ETH);
    }
    printf("%s:Install Net Device %s success\n\r",__FUNCTION__,devname);
    return true;

RcvTaskFailed:
    NetDevUninstall(devname);
NetInstallFailed:
    Lock_MutexDelete(pDrive->protect);
    pDrive->protect = NULL;
DEVPROTECT_FAILED:
    Lock_SempDelete(pDrive->rcvsync);
    pDrive->rcvsync = NULL;
RCVSYNC_FAILED:
    printf("%s:Install Net Device %s failed\n\r",__FUNCTION__,devname);
    return false;
}


u8 GMAC_MdioR(u8 dev,u8 reg, u16 *value)
{
    u32 rValue;
    tagMacDriver   *pDrive = &gMacDriver;
    if(pDrive->EthHandle==NULL)
        return 0;
    if(HAL_OK != HAL_ETH_ReadPHYRegister(pDrive->EthHandle,dev,reg,&rValue))
        return 0;
    *value = (u16)rValue;
    return 1;
}
u8 GMAC_MdioW(u8 dev,u8 reg, u16 value)
{
    tagMacDriver   *pDrive = &gMacDriver;
    if(pDrive->EthHandle==NULL)
        return 0;
    if(HAL_OK != HAL_ETH_WritePHYRegister(pDrive->EthHandle,dev,reg,value))
        return 0;
    return 1;
}
extern void GMAC_InLowPowerPinCfg(void);
extern void GMAC_OutLowPowerPinCfg(void);

// =============================================================================
// ���ܣ� ������˳��͹���ģʽ
//     ʱ��ʹ�� ������ʹ�� GPIO������Ӧ��״̬
// ������ port ���ںţ��� CN_UART1
//     flag ����͹��� ���˳��͹��ı�־���� UART_IN_LOWPOWER
// ���أ�true/false
// =============================================================================
bool_t GMAC_LowPowerConfig(u8 flag)
{
    if(flag == InLowPower)
    {
        RCC->AHB1ENR&=~(1<<25);  //����ETHMACʱ��
        GMAC_InLowPowerPinCfg();
        return true;
    }
    else if (flag == OutLowPower)
    {
         RCC->AHB1ENR|=1<<25;  //ʹ��ETHMACʱ��
         GMAC_OutLowPowerPinCfg();
         MacReset(0);
        return true;
    }
    else
        return false;
}
ADD_TO_ROUTINE_SHELL(macfiltdis,macfiltdis,"usage:MacAddrDis, receive all frame(don't care MAC filter)");
ADD_TO_ROUTINE_SHELL(macfilten,macfilten,"usage:MacAddrEn, don't receive all frame(MAC filter)");
ADD_TO_ROUTINE_SHELL(macsnden,macsnden,"usage:MacSndEn");
ADD_TO_ROUTINE_SHELL(macsnddis,macsnddis,"usage:MacSndDis");
ADD_TO_ROUTINE_SHELL(macreset,macreset,"usage:reset gmac");
ADD_TO_ROUTINE_SHELL(macreg,macreg,"usage:MacReg");
ADD_TO_ROUTINE_SHELL(mac,mac,"usage:gmac");
