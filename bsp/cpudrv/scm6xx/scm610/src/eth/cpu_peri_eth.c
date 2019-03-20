// =============================================================================

// �ļ���     ��cpu_peri_eth.c
// ģ������: GMACģ��ײ�Ӳ��������ΪDJYIP��Э��ջ�ض�������
// ģ��汾: V1.00
// ������Ա: huzb
// ����ʱ��: 02/14.2019
// =============================================================================
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <sys/socket.h>
#include <cpu_peri.h>
#include <board-config.h>
#include "dbug.h"
#include "shell.h"

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_ETH(const char *devname, bool_t loop,u32 loopcycle);
//    ModuleInstall_ETH(CFG_GMAC_NAME,CFG_GMAC_LOOP_MODE,CFG_GMAC_LOOP_CYCLE);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu_peri_eth"  //CPU��mac����
//parent:"djyip"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"int","djyip","heap","lock"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������������������������none����ʾ�������������
                                //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef CFG_GMAC_LOOP_MODE  //****�������Ƿ��Ѿ����ú�
#warning    cpu_peri_eth�������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,0,10000,
#define CFG_GMAC_LOOP_MODE         1            //"�Ƿ�ʹ��loopģʽ",
#define CFG_GMAC_LOOP_CYCLE        1000         //"loop��ʱ��",
#define CFG_GMAC_MAC_ADDR0         00           //"MAC ADDR0",
#define CFG_GMAC_MAC_ADDR1         00           //"MAC ADDR1",
#define CFG_GMAC_MAC_ADDR2         00           //"MAC ADDR2",
#define CFG_GMAC_MAC_ADDR3         00           //"MAC ADDR3",
#define CFG_GMAC_MAC_ADDR4         00           //"MAC ADDR4",
#define CFG_GMAC_MAC_ADDR5         00           //"MAC ADDR5",
//%$#@enum,true,false,
//%$#@string,1,32,
#define CFG_GMAC_NAME              ("STM32F4_ETH")   //"�����豸����",
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

//number of gmac

#define CN_GMAC_MAX (2)

/*********************************************************
 * There are two GMAC:
 *      GMAC0 and GMAC1
 *      sgpt_Gmac is the base address of the GMAC
 *
 *********************************************************/

#define CN_ETH0_BASE (0x50200000UL)
#define CN_ETH1_BASE (0x50210000UL)

static volatile ETH_TypeDef *sgpt_Gmac[CN_GMAC_MAX] =
{
    (volatile ETH_TypeDef *)CN_ETH0_BASE,
    (volatile ETH_TypeDef *)CN_ETH1_BASE,
};

#define  ETH_ERROR              ((uint32_t)0)
#define  ETH_SUCCESS            ((uint32_t)1)
#define  ETH_DMARxDesc_FrameLengthShift           16

/* Global pointers on Tx and Rx descriptor used to track transmit and receive descriptors */

extern ETH_DMADESCTypeDef  *DMATxDescToSet;
extern ETH_DMADESCTypeDef  *DMARxDescToGet;

//frame struct define

typedef struct
{
    u32 length;                      //frame len
    u32 buffer;                      //buffer address
    ETH_DMADESCTypeDef *descriptor;  //point to current tx or rx Dma descriptor

}FrameTypeDef;


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

volatile ETH_TypeDef *sgpt_EthReg = NULL;

typedef struct
{
    //os member
    struct SemaphoreLCB     *rcvsync;          //activate the receive task
    struct MutexLCB         *protect;          //protect the device
    ptu32_t                 devhandle;        //returned by the tcpip stack
    char                    devname[CN_DEVNAME_LEN];
    //hardware
    ETH_TypeDef            *EthHandle;
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

#define EthRxDescs      5           //���ջ�����������
#define EthTxDescs      5           //���ͻ�����������

#define EthRxBufSize    1520        //ETH_HEADER + ETH_EXTRA + MAX_ETH_PAYLOAD + ETH_CRC
#define EthTxBufSize    1520        //ETH_HEADER + ETH_EXTRA + MAX_ETH_PAYLOAD + ETH_CRC

ETH_DMADESCTypeDef  DMARxDscrTab[EthRxDescs];
ETH_DMADESCTypeDef  DMATxDscrTab[EthTxDescs];

u8 Rx_Buff[EthRxDescs][EthRxBufSize];
u8 Tx_Buff[EthTxDescs][EthTxBufSize];
#endif
static u8 gTxBuffer[EthRxBufSize];      //for sending copy frame
extern bool_t Board_EthGpioInit(void);

//Get Eth Fram

FrameTypeDef Eth_RxGetFramInfo(void)
{
    ETH_TypeDef *EthHandle;

    EthHandle = gMacDriver.EthHandle;

    u32 framelength = 0;
    FrameTypeDef frame = {0,0};

    if((DMARxDescToGet->Status & ETH_DMARxDesc_OWN) != (u32)RESET)
    {
        frame.length = ETH_ERROR;

    if ((EthHandle->DMASR & ETH_DMASR_RBUS) != (u32)RESET)
    {
        EthHandle->DMASR = ETH_DMASR_RBUS;
        EthHandle->DMARPDR = 0;
    }

        return frame;
    }

    if(((DMARxDescToGet->Status & ETH_DMARxDesc_ES) == (u32)RESET) &&
        ((DMARxDescToGet->Status & ETH_DMARxDesc_LS) != (u32)RESET) &&
        ((DMARxDescToGet->Status & ETH_DMARxDesc_FS) != (u32)RESET))
    {

        framelength = ((DMARxDescToGet->Status & ETH_DMARxDesc_FL) >> ETH_DMARxDesc_FrameLengthShift) - 4;

        frame.buffer = DMARxDescToGet->Buffer1Addr;
    }
    else
    {
        framelength = ETH_ERROR;
    }

    frame.length = framelength;

    frame.descriptor = DMARxDescToGet;
    //Affter Return The Frame ,Point To Next pRecDesFd
    DMARxDescToGet = (ETH_DMADESCTypeDef*) (DMARxDescToGet->Buffer2NextDescAddr);

    return (frame);
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
    ETH_TypeDef  *EthHandle = NULL;

    if(pDrive != NULL)
    {
        EthHandle = pDrive->EthHandle;
        Ethernet_Configuration(EthHandle);
        //Set Mac Address
        ETH_MACAddressConfig(pDrive->EthHandle, ETH_MAC_Address0,pDrive->macaddr);
        //Set DMADesTxTab
        ETH_DMATxDescChainInit(EthHandle, DMATxDscrTab, &Tx_Buff[0][0], EthTxDescs);
        //Set DMADesRxTab
        ETH_DMARxDescChainInit(EthHandle, DMARxDscrTab, &Rx_Buff[0][0], EthRxDescs);

    }else
    {
        return;
    }

}

static void __MacReset(tagMacDriver *pDrive)
{

    if(Lock_MutexPend(pDrive->protect,CN_TIMEOUT_FOREVER))
    {
        ETH_HardDrvInit(pDrive);
        //Set Mac Address
        ETH_MACAddressConfig(pDrive->EthHandle, ETH_MAC_Address0,pDrive->macaddr);

        pDrive->debuginfo.rsttimes++;
        Lock_MutexPost(pDrive->protect);
    }
    return ;
}

//�ҽ���Buffer��ַ ���Լ�����֡���� ��������Pkg���ϴ���

static tagNetPkg *__MacRcv(ptu32_t devhandle)
{
    tagNetPkg         *pkg = NULL;
    tagMacDriver      *pDrive;
    ETH_TypeDef       *EthHandle;

    u16 CopyBytes,len;
    u8   *dst,*src;

    pDrive = &gMacDriver;
    EthHandle = pDrive->EthHandle;

    //Lenth to copy to Pkg
    len = ETH_GetRxPktSize();
    len = len - 4;          //��ȥ4�ֽڵ�crc

    //copy to Pkg
    CopyBytes = len;

    //Get Rev Fram Info
    //Here Return DMARxDescToGet Point To Next DMARxDescToGet
    FrameTypeDef frame = Eth_RxGetFramInfo();

    if(len > 0)
    {
        pkg = PkgMalloc(len,CN_PKLGLST_END);
        if(NULL == pkg)
            pDrive->debuginfo.nopkgCnts++;
    }

    if(NULL != pkg)
    {
        dst = (u8 *)(pkg->buf +pkg->offset);       //Pkg Buf Address
        src = (u8 *)frame.buffer;                  //Get Buffer Address

//      while(CopyBytes > EthRxBufSize)
        {
            memcpy( dst, src, len);
//          CopyBytes -= len;
//          dst += len;
            //Set Own The Rev Description DMA
            //Set Current descriptor To Be Used -point to DMARxDescToGet in fact
            frame.descriptor->Status = ETH_DMARxDesc_OWN;
        }

        pkg->datalen = len;
//      EthHandle->RxFrameInfos.SegCount =0;
    }

    // When Rx Buffer unavailable flag is set: clear it and resume reception

    if ((EthHandle->DMASR & ETH_DMASR_RBUS) != (u32)RESET)
    {
        EthHandle->DMASR = ETH_DMASR_RBUS; //������ջ����������ñ�־
        EthHandle->DMARPDR = 0;            //�ָ�DMA����
        pDrive->debuginfo.rxoverInts++;
    }


    return pkg;
}

//Get Send Buffer

u32 ETH_GetCurrentTxBuffer(void)
{
  return (DMATxDescToSet->Buffer1Addr);
}

//ETH Start To Send

u32 ETH_StartSndPkg(u16 len)
{
    ETH_TypeDef *EthHandle;
    u16 FrameLength;

    FrameLength = len;

    EthHandle = gMacDriver.EthHandle;

    if((DMATxDescToSet->Status & ETH_DMATxDesc_OWN) != (u32)RESET)
    {
        return ETH_ERROR;
    }

    DMATxDescToSet->ControlBufferSize |= ETH_DMATxDesc_IC; //zhangyx

    DMATxDescToSet->ControlBufferSize &= ~ETH_DMATxDesc_TBS1;
    DMATxDescToSet->ControlBufferSize |= (FrameLength & ETH_DMATxDesc_TBS1);

    DMATxDescToSet->ControlBufferSize |= ETH_DMATxDesc_LS | ETH_DMATxDesc_FS;

    DMATxDescToSet->Status |= ETH_DMATxDesc_OWN;


    if ((EthHandle->DMASR & ETH_DMASR_TBUS) != (u32)RESET)
    {
        EthHandle->DMASR = ETH_DMASR_TBUS;
        EthHandle->DMATPDR = 0;
    }

    DMATxDescToSet = (ETH_DMADESCTypeDef*) (DMATxDescToSet->Buffer2NextDescAddr);


    return ETH_SUCCESS;
}


static bool_t MacSnd(ptu32_t handle,tagNetPkg * pkg,u32 framelen, u32 netdevtask)
{
    bool_t             result;
    tagMacDriver      *pDrive;
    ETH_TypeDef       *EthHandle;
    tagNetPkg         *tmppkg;
    u8                *dst,*src;
    u16                len=0;

    result = false;
    pDrive = &gMacDriver;
    EthHandle = pDrive->EthHandle;
    pDrive->debuginfo.sndTimes++;

    if((0 == handle)||(NULL == pkg))
        return result;

    if(Lock_MutexPend(pDrive->protect,CN_TIMEOUT_FOREVER))
    {

        //Is this buffer available? If not, goto error
        if((DMATxDescToSet->Status & ETH_DMATxDesc_OWN) != (uint32_t)RESET)
        {
            pDrive->debuginfo.sndnobdCnt ++;
            goto NODESCERROR;
        }

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
            dst = (u8 *)ETH_GetCurrentTxBuffer();

            memcpy( dst,src ,len );
            // start to send
            ETH_StartSndPkg(len);
            //Prepare transmit descriptors to give to DMA
            //if(HAL_OK == HAL_ETH_TransmitFrame(EthHandle, len))
            {
                pDrive->debuginfo.sndOkTimes++;
                result = true;
            }
        }
        else
        {
            pDrive->debuginfo.sndPkgTooLongCnt++;
        }

NODESCERROR:
        // When Transmit Underflow flag is set, clear it and issue a Transmit
        //Poll Demand to resume transmission
        if ((EthHandle->DMASR & ETH_DMASR_TUS) != (uint32_t)RESET)
        {
            // Clear TUS ETHERNET DMA flag
            EthHandle->DMASR = ETH_DMASR_TUS;

            //Resume DMA transmission
            EthHandle->DMATPDR = 0;
        }
        Lock_MutexPost(pDrive->protect);
    }

    return result;
}
//
//u32 ETH_SendData(u8 *buf,u32 len)
//{
//    tagNetPkg          pkg;
//    tagMacDriver      *pDrive;
//
//    pDrive = &gMacDriver;
//
//    pkg.partnext = NULL;
//    pkg.pkgflag  = (1<<0);  //ֻ��һ����
//    pkg.offset   = 0;
//    pkg.datalen  = len;
//    pkg.buf      = buf;
//
//    if(MacSnd(pDrive->devhandle,&pkg,len,0))
//    {
//        return len;
//    }
//    else
//    {
//        return 0;
//    }
//}

//This is the interrut handler

u32 ETH_IntHandler(ufast_t IntLine)
{
    tagMacDriver *pDrive;
    ETH_TypeDef *EthHandle;

    pDrive = &gMacDriver;
    EthHandle = pDrive->EthHandle;

    pDrive->debuginfo.Ints++;

    pDrive->debuginfo.rcvInts++;

    //��DMA�жϱ�־�Լ�DMA�����жϱ�־
    ETH_DMAClearITPendingBit(SGCC_GMAC0_P, ETH_DMA_IT_R);
    ETH_DMAClearITPendingBit(SGCC_GMAC0_P, ETH_DMA_IT_NIS);
    Lock_SempPost(pDrive->rcvsync);

    return 0;
}

//mac control function

static bool_t MacCtrl(ptu32_t devhandle,u8 cmd,ptu32_t para)
{
    bool_t result = false;
    tagMacDriver   *pDrive;
#if 1
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
                    __macbitsclear(pDrive->EthHandle->MACFFR,ETH_MACFFR_BFD);
                    printk("mac broad enable\n\r");
                    pDrive->macstat.bfd = 0;
                }
                else
                {
                    __macbitsset(pDrive->EthHandle->MACFFR,ETH_MACFFR_BFD);
                    printk("mac broad disable\n\r");
                    pDrive->macstat.bfd = 1;
                }
                result = true;
                break;
            case EN_NETDEV_SETPOINT:
                if(para)//���Ե���ģʽ
                {
                    __macbitsclear(pDrive->EthHandle->MACFFR,ETH_MACFFR_HU);
                }
                else    //ɢ�е���ģʽ
                {
                    __macbitsset(pDrive->EthHandle->MACFFR,ETH_MACFFR_HU);
                }
                result = true;
                break;
            case EN_NETDEV_SETMULTI:
                if(para)
                {
                   __macbitsset(pDrive->EthHandle->MACFFR,ETH_MACFFR_PAM);//pass all multicast
                }
                else
                {
                    __macbitsclear(pDrive->EthHandle->MACFFR,ETH_MACFFR_PAM);//multicast filt
                }
               result = true;
                break;
            case EN_NETDEV_SETRECV:
                if(para)
                {
                     //enable to rev
                     ETH_MACReceptionCmd(pDrive->EthHandle, ENABLE);
                     ETH_DMAReceptionCmd(pDrive->EthHandle, ENABLE);
                }
                else
                {    //disable to rev
                     ETH_MACReceptionCmd(pDrive->EthHandle, DISABLE);
                     ETH_DMAReceptionCmd(pDrive->EthHandle, DISABLE);
                }
                result = true;
                break;
            case EN_NETDEV_SETSEND:
                if(para)
                {
                     /* Enable transmit state machine of the MAC for transmission on the MII */
                      ETH_MACTransmissionCmd(pDrive->EthHandle, ENABLE);
                      /* Flush Transmit FIFO */
                      ETH_FlushTransmitFIFO(pDrive->EthHandle);
                      /* Start DMA transmission */
                      ETH_DMATransmissionCmd(pDrive->EthHandle, ENABLE);
}
                else
                {
                      /* Disable transmit state machine of the MAC for transmission on the MII */
                      ETH_MACTransmissionCmd(pDrive->EthHandle, DISABLE);
                      /* Flush Transmit FIFO */
                      ETH_FlushTransmitFIFO(pDrive->EthHandle);

                      /* Disable DMA transmission */
                      ETH_DMATransmissionCmd(pDrive->EthHandle, DISABLE);

                }
                break;
            case EN_NETDEV_SETMAC://Set Mac Address
                memcpy(pDrive->macaddr,(u8 *)para, CN_MACADDR_LEN);
                ETH_HardDrvInit(pDrive);
                //Set Mac Address
                ETH_MACAddressConfig(pDrive->EthHandle, ETH_MAC_Address0,para);
                pDrive->debuginfo.rsttimes++;
                result = true;
                break;
            case EN_NETDEV_SETMULTIMAC:
                break;
            case EN_NETDEV_GTETMAC://Get Mac Address
                if((u8 *)para != NULL)
                {
                    ETH_GetMACAddress(pDrive->EthHandle, ETH_MAC_Address0, (u8 *)para);
                    memcpy(pDrive->macaddr,(u8 *)para,CN_MACADDR_LEN);
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
            default:
                result = false;
                break;
        }
        Lock_MutexPost(pDrive->protect);
    }

    return result;
#endif
    return true;
}

//this is the receive task

static ptu32_t __MacRcvTask(void)
{
    tagNetPkg *pkg;
    ptu32_t    handle;
    u8        *rawbuf;
    u16        len;
    tagMacDriver      *pDrive;
    pDrive = &gMacDriver;

    Djy_GetEventPara(&handle,NULL);

    while(1)
    {
        //Wait to Rcv Data Pkg
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
                NetDevFlowCounter(handle,NetDevFrameType(pkg->buf+ pkg->offset,pkg->datalen));
                //you could alse use the soft method
                if(NULL != pDrive->fnrcvhook)
                {
                    rawbuf = pkg->buf + pkg->offset;
                    len = pkg->datalen;
                    pDrive->fnrcvhook(rawbuf,len);
                }
                else
                {
                    LinkPost(handle,pkg);
                }
                PkgTryFreePart(pkg);
                pDrive->debuginfo.rcvPkgTimes++;
            }
            else
            {
                  //here we still use the counter to do the time state check
                  NetDevFlowCounter(handle,EN_NETDEV_FRAME_LAST);
                break;
            }
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
    printf("rxTaskRun     :%d\n\r",pDrive->debuginfo.rxTaskRun);
    printf("rcvSpeed      :%d\n\r",rcvSpeed);
    printf("intSpeed      :%d\n\r",intSpeed);
    printf("rstTimes      :%d\n\r",pDrive->debuginfo.rsttimes);
    printf("**********************************\n\r");
    return true;
}

#define CN_GMAC_REG_BASE   ((u32)(gMacDriver.EthHandle))

//bool_t MacReg(char *param)
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

bool_t macsnden(char *param)
{
    tagMacDriver      *pDrive;

    pDrive = &gMacDriver;
    if(Lock_MutexPend(pDrive->protect,CN_TIMEOUT_FOREVER))
    {
         /* Enable transmit state machine of the MAC for transmission on the MII */
         ETH_MACTransmissionCmd(pDrive->EthHandle, ENABLE);
         /* Flush Transmit FIFO */
         ETH_FlushTransmitFIFO(pDrive->EthHandle);
         /* Start DMA transmission */
         ETH_DMATransmissionCmd(pDrive->EthHandle, ENABLE);
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
         ETH_MACTransmissionCmd(pDrive->EthHandle, DISABLE);
         /* Flush Transmit FIFO */
         ETH_FlushTransmitFIFO(pDrive->EthHandle);
         /* Start DMA transmission */
         ETH_DMATransmissionCmd(pDrive->EthHandle, DISABLE);
        Lock_MutexPost(pDrive->protect);
    }
    return true;
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
    u8 intLine;

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

    //Ĭ��ʹ��GMAC0
    pDrive->EthHandle  = sgpt_Gmac[0];

    //add to set MAC address
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

    //install the net device
    devpara.ifctrl = MacCtrl;
    devpara.ifsend = MacSnd;
    devpara.iftype = EN_LINK_ETHERNET;
    devpara.devfunc = CN_IPDEV_ALL;
    memcpy(devpara.mac,macaddress,6);
    devpara.name = (char *)pDrive->devname;
    devpara.private = 0;
    devpara.mtu = 1500;                     //Max Transmit Unit
    devpara.private = (ptu32_t)pDrive;
    pDrive->devhandle = NetDevInstall(&devpara);

    if(0 == pDrive->devhandle)
    {
        goto NetInstallFailed;
    }
    if(false == __CreateRcvTask(pDrive->devhandle))
    {
        goto RcvTaskFailed;
    }

    if(false == loop)
    {
        switch((u32)pDrive->EthHandle)
        {
            case CN_ETH0_BASE:
            intLine = CN_INT_LINE_GMAC0;
            break;
            case CN_ETH1_BASE:
            intLine = CN_INT_LINE_GMAC1;
            break;
            default:break;
        }

        Int_Register(intLine);
        Int_SettoAsynSignal(intLine);
        Int_ClearLine(intLine);
        Int_IsrConnect(intLine,ETH_IntHandler);
        Int_ContactLine(intLine);
    }

//    Sh_InstallCmd(gMacDebug,gMacDebugCmdRsc,CN_GMACDEBUG_NUM);
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


/*---------------------------------  PHY  ------------------------------------*/
/**
  * @brief  Read a PHY register
  * @param PHYAddress: PHY device address, is the index of one of supported 32 PHY devices.
  *   This parameter can be one of the following values: 0,..,31
  * @param PHYReg: PHY register address, is the index of one of the 32 PHY register.
  *   This parameter can be one of the following values:
  *     @arg PHY_BCR: Tranceiver Basic Control Register
  *     @arg PHY_BSR: Tranceiver Basic Status Register
  *     @arg PHY_SR : Tranceiver Status Register
  *     @arg More PHY register could be read depending on the used PHY
  * @retval ETH_ERROR: in case of timeout
  *         MAC MIIDR register value: Data read from the selected PHY register (correct read )
  */

u8 GMAC_MdioR(u8 dev,u8 reg, u16 *value)
{
    u16 rValue;
    tagMacDriver   *pDrive = &gMacDriver;

    rValue = ETH_ReadPHYRegister(pDrive->EthHandle, dev, reg);

    *value = (u16)rValue;

     return 1;
}

/**
  * @brief  Write to a PHY register
  * @param PHYAddress: PHY device address, is the index of one of supported 32 PHY devices.
  *   This parameter can be one of the following values: 0,..,31
  * @param PHYReg: PHY register address, is the index of one of the 32 PHY register.
  *   This parameter can be one of the following values:
  *     @arg PHY_BCR    : Tranceiver Control Register
  *     @arg More PHY register could be written depending on the used PHY
  * @param  PHYValue: the value to write
  * @retval ETH_ERROR: in case of timeout
  *         ETH_SUCCESS: for correct write
  */


u8 GMAC_MdioW(u8 dev,u8 reg, u16 value)
{
    tagMacDriver *pDrive = &gMacDriver;

    if( ETH_SUCCESS != ETH_WritePHYRegister(pDrive->EthHandle, dev, reg,value))
        return ETH_SUCCESS;

    return ETH_SUCCESS;
}

ADD_TO_ROUTINE_SHELL(mac,mac,"usage:gmac");
ADD_TO_ROUTINE_SHELL(macreg,macreg,"usage:MacReg");
ADD_TO_ROUTINE_SHELL(macreset,MacReset,"usage:reset gmac");
ADD_TO_ROUTINE_SHELL(macsnden,macsnden,"usage:MacSndEn");
ADD_TO_ROUTINE_SHELL(macsnddis,macsnddis,"usage:MacSndDis");


