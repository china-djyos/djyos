// =============================================================================

// �ļ���     ��cpu_peri_eth.c
// ģ������: GMACģ��ײ�Ӳ��������ΪDJYIP��Э��ջ�ض�������
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 02/13.2017
// =============================================================================
#if 0
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <sys/socket.h>
#include <cpu_peri.h>
#include <board-config.h>

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_eth.h"

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
    ptu32_t                 devhandle;        //returned by the tcpip stack
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
#define EthRxDescs      8
#define EthTxDescs      8
#define EthRxBufSize    1524
#define EthTxBufSize    1524
ETH_DMADescTypeDef  DMARxDscrTab[EthRxDescs];
ETH_DMADescTypeDef  DMATxDscrTab[EthTxDescs];

u8 Rx_Buff[EthRxDescs][EthRxBufSize];
u8 Tx_Buff[EthTxDescs][EthTxBufSize];
#endif
static u8 gTxBuffer[EthRxBufSize];      //for sending copy frame
extern bool_t Board_EthGpioInit(void);

// HAL���е����˸ú���
void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
      __HAL_RCC_ETH_CLK_ENABLE();
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
    ETH_HandleTypeDef *heth;

    heth = pDrive->EthHandle;

    // configure ethernet peripheral (GPIOs, clocks, MAC, DMA)
    HAL_ETH_Init(heth);

    // Initialize Tx/Rx Descriptors list: Chain Mode
    HAL_ETH_DMATxDescListInit(heth, DMATxDscrTab, &Tx_Buff[0][0], EthTxDescs);
    HAL_ETH_DMARxDescListInit(heth, DMARxDscrTab, &Rx_Buff[0][0], EthRxDescs);

    /* Enable MAC and DMA transmission and reception */
    HAL_ETH_Start(heth);
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

static struct NetPkg *__MacRcv(ptu32_t devhandle)
{
    struct NetPkg         *pkg = NULL;
    tagMacDriver      *pDrive;
    ETH_HandleTypeDef *EthHandle;
    volatile ETH_DMADescTypeDef *dmarxdesc;
    u16 CopyBytes,len;
    u8   *dst,*src;

    pDrive = &gMacDriver;
    EthHandle = pDrive->EthHandle;
    // get received frame
    if(HAL_ETH_GetReceivedFrame_IT(EthHandle) != HAL_OK)
        return NULL;

    len = EthHandle->RxFrameInfos.length;
    CopyBytes = len;

    if(len > 0)
    {
        pkg = PkgMalloc(len,CN_PKLGLST_END);
        if(NULL == pkg)
            pDrive->debuginfo.nopkgCnts++;
    }

    if(NULL != pkg)
    {
        dmarxdesc = EthHandle->RxFrameInfos.FSRxDesc;
        dst = (u8 *)(pkg->buf +pkg->offset);
        src = (u8 *)EthHandle->RxFrameInfos.buffer;
        while(CopyBytes > EthRxBufSize)
        {
            memcpy( dst, src, EthRxBufSize);
            CopyBytes -= EthRxBufSize;
            dst += EthRxBufSize;

            dmarxdesc->Status |= ETH_DMARXDESC_OWN;     //release Des to DMA
            dmarxdesc = (ETH_DMADescTypeDef *)(dmarxdesc->Buffer2NextDescAddr);
            src = (u8 *)(dmarxdesc->Buffer1Addr);
        }
        memcpy( dst, src, CopyBytes);
        dmarxdesc->Status |= ETH_DMARXDESC_OWN;

        pkg->datalen = len;
        EthHandle->RxFrameInfos.SegCount =0;
    }

    // When Rx Buffer unavailable flag is set: clear it and resume reception
    if ((EthHandle->Instance->DMASR & ETH_DMASR_RBUS) != (uint32_t)RESET)
    {
        // Clear RBUS ETHERNET DMA flag
        EthHandle->Instance->DMASR = ETH_DMASR_RBUS;
        // Resume DMA reception
        EthHandle->Instance->DMARPDR = 0;

        pDrive->debuginfo.rxoverInts++;
    }

    return pkg;
}

static bool_t MacSnd(ptu32_t handle,struct NetPkg * pkg,u32 framelen, u32 netdevtask)
{
    bool_t             result;
    tagMacDriver      *pDrive;
    ETH_HandleTypeDef *EthHandle;
    ETH_DMADescTypeDef *DmaTxDesc;
    struct NetPkg         *tmppkg;
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
        DmaTxDesc = pDrive->EthHandle->TxDesc;
        //Is this buffer available? If not, goto error
        if((DmaTxDesc->Status & ETH_DMATXDESC_OWN) != (uint32_t)RESET)
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
            dst = (u8 *)(EthHandle->TxDesc->Buffer1Addr);
            memcpy( dst,src ,len );
            // Point to next descriptor
            DmaTxDesc = (ETH_DMADescTypeDef *)(DmaTxDesc->Buffer2NextDescAddr);
            //Prepare transmit descriptors to give to DMA
            if(HAL_OK == HAL_ETH_TransmitFrame(EthHandle, len))
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
        if ((EthHandle->Instance->DMASR & ETH_DMASR_TUS) != (uint32_t)RESET)
        {
            // Clear TUS ETHERNET DMA flag
            EthHandle->Instance->DMASR = ETH_DMASR_TUS;

            //Resume DMA transmission
            EthHandle->Instance->DMATPDR = 0;
        }
        Lock_MutexPost(pDrive->protect);
    }

    return result;
}

//u32 ETH_SendData(u8 *buf,u32 len)
//{
//    struct NetPkg          pkg;
//    tagMacDriver      *pDrive;
//
//    pDrive = &gMacDriver;
//
//    pkg.partnext = NULL;
//    pkg.pkgflag  = (1<<0);  //ֻ��һ����
//    pkg.offset   = 0;
//    pkg.datalen  = len;
//    pkg.buf      = buf;
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
    ETH_HandleTypeDef *EthHandle;

    pDrive = &gMacDriver;
    EthHandle = pDrive->EthHandle;

    pDrive->debuginfo.Ints++;
    if (__HAL_ETH_DMA_GET_FLAG(EthHandle, ETH_DMA_FLAG_R))
    {
        pDrive->debuginfo.rcvInts++;
        Lock_SempPost(pDrive->rcvsync);
        __HAL_ETH_DMA_CLEAR_IT(EthHandle, ETH_DMA_IT_R);    //Clear the Eth DMA Rx IT pending bits
        EthHandle->State = HAL_ETH_STATE_READY;             //Set HAL State to Ready
        __HAL_UNLOCK(EthHandle);
    }
    else if (__HAL_ETH_DMA_GET_FLAG(EthHandle, ETH_DMA_FLAG_T))
    {
        pDrive->debuginfo.sndInts++;
        __HAL_ETH_DMA_CLEAR_IT(EthHandle, ETH_DMA_IT_T);    //Clear the Eth DMA Tx IT pending bits
        EthHandle->State = HAL_ETH_STATE_READY;             //Set HAL State to Ready
        __HAL_UNLOCK(EthHandle);
    }

    __HAL_ETH_DMA_CLEAR_IT(EthHandle, ETH_DMA_IT_NIS);

    if(__HAL_ETH_DMA_GET_FLAG(EthHandle, ETH_DMA_FLAG_AIS))
    {
        pDrive->debuginfo.dmaerr++;
        __HAL_ETH_DMA_CLEAR_IT(EthHandle, ETH_DMA_FLAG_AIS);
        EthHandle->State = HAL_ETH_STATE_READY;

        __HAL_UNLOCK(EthHandle);
    }
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
                    __macbitsclear(&pDrive->EthHandle->Instance->MACFFR,ETH_MACFFR_BFD);
                    printk("mac broad enable\n\r");
                    pDrive->macstat.bfd = 0;
                }
                else
                {
                    __macbitsset(&pDrive->EthHandle->Instance->MACFFR,ETH_MACFFR_BFD);
                    printk("mac broad disable\n\r");
                    pDrive->macstat.bfd = 1;
                }
                result = true;
                break;
            case EN_NETDEV_SETPOINT:
                if(para)
                {
                    __macbitsclear(&pDrive->EthHandle->Instance->MACFFR,
                            ETH_MACFFR_HU);//���Ե���ģʽ
                }
                else
                {
                    __macbitsset(&pDrive->EthHandle->Instance->MACFFR,
                            ETH_MACFFR_HU);//ɢ�е���ģʽ
                }
                result = true;
                break;
            case EN_NETDEV_SETMULTI:
                if(para)
                {
                   __macbitsset(&pDrive->EthHandle->Instance->MACFFR,
                           ETH_MACFFR_PAM);//pass all multicast
                }
                else
                {
                    __macbitsclear(&pDrive->EthHandle->Instance->MACFFR,
                            ETH_MACFFR_PAM);//multicast filt
                }
               result = true;
                break;
            case EN_NETDEV_SETRECV:
                if(para)
                {
//                  HAL_ETH_StartReceive(pDrive->EthHandle);
                }
                else
                {
//                  HAL_ETH_StopReceive(pDrive->EthHandle);
                }
                result = true;
                break;
            case EN_NETDEV_SETSEND:
                if(para)
                {
                    HAL_ETH_Start(pDrive->EthHandle);
                }
                else
                {
                    HAL_ETH_Stop(pDrive->EthHandle);
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
    struct NetPkg *pkg;
    struct NetDev *handle;
    u8        *rawbuf;
    u16        len;
    tagMacDriver      *pDrive;
    pDrive = &gMacDriver;

    DJY_GetEventPara(&handle,NULL);
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
                NetDev_FlowCtrl(handle,NetDev_FrameType(PkgGetCurrentBuffer(pkg),
                                                      PkgGetDataLen(pkg)));
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
                  NetDev_FlowCtrl(handle,EN_NETDEV_FRAME_LAST);
                break;
            }
        }
    }
    return 0;
}
//create the receive task
static bool_t __CreateRcvTask(struct NetDev * handle)
{
    bool_t result = false;
    u16 evttID;
    u16 eventID;

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
bool_t macdebuginfo(char *param)
{
    s64  time;
    u32  timeS;
    tagMacDriver      *pDrive;
    pDrive = &gMacDriver;

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
    printf("rxTaskRun     :%d\n\r",pDrive->debuginfo.rxTaskRun);
    printf("rcvSpeed      :%d\n\r",rcvSpeed);
    printf("intSpeed      :%d\n\r",intSpeed);
    printf("rstTimes      :%d\n\r",pDrive->debuginfo.rsttimes);
    printf("**********************************\n\r");
    return true;
}

#define CN_GMAC_REG_BASE   ((u32)ETH)
bool_t MacReg(char *param)
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

bool_t MacSndEn(char *param)
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
bool_t MacSndDis(char *param)
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

#include <shell.h>
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
    struct NetDevPara   devpara;

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
    pDrive->EthHandle->Init.MACAddr = macaddress;
    pDrive->EthHandle->Init.AutoNegotiation = ETH_AUTONEGOTIATION_ENABLE;
    pDrive->EthHandle->Init.Speed = ETH_SPEED_100M;
    pDrive->EthHandle->Init.DuplexMode = ETH_MODE_FULLDUPLEX;
    pDrive->EthHandle->Init.MediaInterface = ETH_MEDIA_INTERFACE_RMII;
    pDrive->EthHandle->Init.RxMode = ETH_RXINTERRUPT_MODE;
    pDrive->EthHandle->Init.ChecksumMode = ETH_CHECKSUM_BY_HARDWARE;//ETH_CHECKSUM_BY_SOFTWARE;
    pDrive->EthHandle->Init.PhyAddress = 0x00;//not used;

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
    devpara.mtu = 1522;
    devpara.private = (ptu32_t)pDrive;
    pDrive->devhandle = NetDev_Install(&devpara);
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
        Int_Register(CN_INT_LINE_ETH);
        Int_SettoAsynSignal(CN_INT_LINE_ETH);
        Int_ClearLine(CN_INT_LINE_ETH);
        Int_IsrConnect(CN_INT_LINE_ETH,ETH_IntHandler);
        Int_ContactLine(CN_INT_LINE_ETH);
    }

    printf("%s:Install Net Device %s success\n\r",__FUNCTION__,devname);
    return true;

RcvTaskFailed:
    NetDev_Uninstall(devname);
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
    pDrive->EthHandle->Init.PhyAddress = dev;

    if(HAL_OK != HAL_ETH_ReadPHYRegister(pDrive->EthHandle,reg,&rValue))
        return 0;
    *value = (u16)rValue;
    return 1;
}
u8 GMAC_MdioW(u8 dev,u8 reg, u16 value)
{
    tagMacDriver   *pDrive = &gMacDriver;
    pDrive->EthHandle->Init.PhyAddress = dev;

    if(HAL_OK != HAL_ETH_WritePHYRegister(pDrive->EthHandle,reg,value))
        return 0;
    return 1;
}

#endif
