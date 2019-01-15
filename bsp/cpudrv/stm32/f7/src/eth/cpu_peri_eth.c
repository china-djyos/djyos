// =============================================================================

// 文件名     ：cpu_peri_eth.c
// 模块描述: GMAC模块底层硬件驱动，为DJYIP的协议栈特定的驱动
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 02/13.2017
// =============================================================================
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <cpu_peri.h>
#include <board-config.h>
#include <sys/socket.h>
#include <netbsp.h>
#include "shell.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_eth.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//  extern bool_t ModuleInstall_ETH(const char *devname, u8 *macaddress,\
//                          bool_t loop,u32 loopcycle,\
//                          bool_t (*rcvHook)(u8 *buf, u16 len));
//   static u8 mac_addr[]={CFG_MAC_ADDR0,CFG_MAC_ADDR1,CFG_MAC_ADDR2,CFG_MAC_ADDR3,CFG_MAC_ADDR4,CFG_MAC_ADDR5};
//   ModuleInstall_ETH(CFG_ETH_DEV_NAME,mac_addr,CFG_ETH_LOOP_ENABLE,CFG_ETH_LOOP_CYCLE,NULL);
//%$#@end initcode  ****初始化代码结束
//%$#@describe      ****组件描述开始
//component name:"cpu_peri_eth" //CPU的mac驱动
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"djyip","lock","int","tcpip",      //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //如果依赖多个组件，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
//%$#@target = header    //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#ifndef cpu_peri_eth   //****检查参数是否已经配置好
#warning    cpu_peri_eth组件参数未配置，使用默认值
//%$#@num,0,255,
#define CFG_MAC_ADDR0           00           //"网卡地址0",
#define CFG_MAC_ADDR1           00           //"网卡地址1",
#define CFG_MAC_ADDR2           00           //"网卡地址2",
#define CFG_MAC_ADDR3           00           //"网卡地址3",
#define CFG_MAC_ADDR4           00           //"网卡地址4",
#define CFG_MAC_ADDR5           00           //"网卡地址5",
//%$#@num,1000,10000,
#define CFG_ETH_LOOP_CYCLE      1000         //"网卡轮询接收周期",
//%$#@enum,true,false,
#define CFG_ETH_LOOP_ENABLE     true        //"网卡接收是否轮询",
//%$#@string,1,32,
#define CFG_ETH_DEV_NAME        "STM32F7_ETH"//"网卡名称",
//%$#select,        ***定义无值的宏，仅用于第三方组件
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束

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
#define EthRxDescs      8
#define EthTxDescs      8
#define EthRxBufSize    1524
#define EthTxBufSize    1524
ETH_DMADescTypeDef  DMARxDscrTab[EthRxDescs] __attribute__((section(".nocacheram")));
ETH_DMADescTypeDef  DMATxDscrTab[EthTxDescs] __attribute__((section(".nocacheram")));

u8 Rx_Buff[EthRxDescs][EthRxBufSize] __attribute__((section(".nocacheram")));
u8 Tx_Buff[EthTxDescs][EthTxBufSize] __attribute__((section(".nocacheram")));
#endif
static u8 gTxBuffer[EthRxBufSize];      //for sending copy frame


/*
      RMII_REF_CLK ----------------------> PA1
      RMII_MDIO -------------------------> PA2
      RMII_MDC --------------------------> PC1
      RMII_MII_CRS_DV -------------------> PA7
      RMII_MII_RXD0 ---------------------> PC4
      RMII_MII_RXD1 ---------------------> PC5
      RMII_MII_RXER ---------------------> PG2  //(not used)
      RMII_MII_TX_EN --------------------> PB11
      RMII_MII_TXD0 ---------------------> PG13
      RMII_MII_TXD1 ---------------------> PG14
*/
//static const Pin EthRmiiPins[] = {
//  {GPIO_A,PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_NONE,AF11},
//  {GPIO_A,PIN2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_NONE,AF11},
//  {GPIO_A,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_NONE,AF11},
//  {GPIO_C,PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_NONE,AF11},
//  {GPIO_C,PIN4,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_NONE,AF11},
//  {GPIO_C,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_NONE,AF11},
//  {GPIO_B,PIN11,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_NONE,AF11},
//  {GPIO_G,PIN13,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_NONE,AF11},
//  {GPIO_G,PIN14,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_NONE,AF11},
//};

// HAL库中调用了该函数
void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
      __HAL_RCC_GPIOA_CLK_ENABLE();
      __HAL_RCC_GPIOB_CLK_ENABLE();
      __HAL_RCC_GPIOC_CLK_ENABLE();
      __HAL_RCC_GPIOG_CLK_ENABLE();

//    PIO_Configure(EthPins,PIO_LISTSIZE(EthRmiiPins));

      /* Enable ETHERNET clock  */
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
    HAL_ETH_DMATxDescListInit(heth, DMATxDscrTab, &Tx_Buff[0][0], EthTxDescs,EthTxBufSize);
    HAL_ETH_DMARxDescListInit(heth, DMARxDscrTab, &Rx_Buff[0][0], EthRxDescs,EthRxBufSize);

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

static tagNetPkg *__MacRcv(void* devhandle)
{
    tagNetPkg         *pkg = NULL;
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
    else        //if malloc pkg failed
    {
        dmarxdesc = EthHandle->RxFrameInfos.FSRxDesc;
        while(CopyBytes > EthRxBufSize)
        {
            CopyBytes -= EthRxBufSize;

            dmarxdesc->Status |= ETH_DMARXDESC_OWN;     //release Des to DMA
            dmarxdesc = (ETH_DMADescTypeDef *)(dmarxdesc->Buffer2NextDescAddr);
        }
        dmarxdesc->Status |= ETH_DMARXDESC_OWN;
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

static bool_t MacSnd(void* handle,tagNetPkg * pkg,u32 framelen, u32 netdevtask)
{
    bool_t             result;
    tagMacDriver      *pDrive;
    ETH_HandleTypeDef *EthHandle;
    ETH_DMADescTypeDef *DmaTxDesc;
    tagNetPkg         *tmppkg;
    u8                *dst,*src;
    u16                len=0;

    result = false;
    pDrive = &gMacDriver;
    EthHandle = pDrive->EthHandle;
    pDrive->debuginfo.sndTimes++;
    if((NULL == handle)||(NULL == pkg))
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

u32 ETH_SendData(u8 *buf,u32 len)
{
    tagNetPkg          pkg;
    tagMacDriver      *pDrive;

    pDrive = &gMacDriver;

    pkg.partnext = NULL;
    pkg.pkgflag  = (1<<0);  //只有一个包
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
#define EN_NETDEV_ADDRFILTER (EN_NETDEV_CMDLAST + 1)
static bool_t MacCtrl(void *devhandle,u8 cmd,ptu32_t para)
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
                            ETH_MACFFR_HU);//绝对单播模式
                }
                else
                {
                    __macbitsset(&pDrive->EthHandle->Instance->MACFFR,
                            ETH_MACFFR_HU);//散列单播模式
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
                    HAL_ETH_StartReceive(pDrive->EthHandle);
                }
                else
                {
                    HAL_ETH_StopReceive(pDrive->EthHandle);
                }
                result = true;
                break;
            case EN_NETDEV_SETSEND:
                if(para)
                {
                    HAL_ETH_StartTransmit(pDrive->EthHandle);
                }
                else
                {
                    HAL_ETH_StopTransmit(pDrive->EthHandle);
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
            case (EN_NETDEV_ADDRFILTER):    //增加一条是否接收所有包
                if(para)
                {
                    __macbitsclear(&pDrive->EthHandle->Instance->MACFFR,
                            ETH_MACFFR_RA);//addr filter
                }
                else
                {
                    __macbitsset(&pDrive->EthHandle->Instance->MACFFR,
                                               ETH_MACFFR_RA);//pass all pkg
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
ADD_TO_IN_SHELL_HELP(mac,"usage:gmac");
ADD_TO_IN_SHELL bool_t mac(char *param)

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
//bool_t MacReg(char *param)
ADD_TO_IN_SHELL_HELP(macreg,"usage:MacReg");
ADD_TO_IN_SHELL bool_t macreg(char *param)
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

//bool_t MacReset(char *param)
ADD_TO_IN_SHELL_HELP(macreset,"usage:reset gmac");
ADD_TO_IN_SHELL bool_t macreset(char *param)
{
    tagMacDriver   *pDrive = &gMacDriver;

    __MacReset(pDrive);
    return true;
}

//bool_t MacSndEn(char *param)
ADD_TO_IN_SHELL_HELP(macsnden,"usage:MacSndEn");
ADD_TO_IN_SHELL bool_t macsnden(char *param)
{
    tagMacDriver      *pDrive;


    pDrive = &gMacDriver;
    if(Lock_MutexPend(pDrive->protect,CN_TIMEOUT_FOREVER))
    {
        HAL_ETH_StartTransmit(pDrive->EthHandle);
        Lock_MutexPost(pDrive->protect);
    }
    return true;
}
//bool_t MacSndDis(char *param)
ADD_TO_IN_SHELL_HELP(macsnddis,"usage:MacSndDis");
ADD_TO_IN_SHELL bool_t macsnddis(char *param)
{
    tagMacDriver      *pDrive;

    pDrive = &gMacDriver;
    if(Lock_MutexPend(pDrive->protect,CN_TIMEOUT_FOREVER))
    {
        HAL_ETH_StopTransmit(pDrive->EthHandle);
        Lock_MutexPost(pDrive->protect);
    }
    return true;
}

//bool_t MacAddrFilterEn(char *param)
ADD_TO_IN_SHELL_HELP(macfilten,"usage:MacAddrEn, don't receive all frame(MAC filter)");
ADD_TO_IN_SHELL bool_t macfilten(char *param)
{
    tagMacDriver      *pDrive;

    pDrive = &gMacDriver;
    MacCtrl(pDrive->devhandle,EN_NETDEV_ADDRFILTER,1);//开通地址过滤功能
    return true;
}
//bool_t MacAddrFilterDis(char *param)
ADD_TO_IN_SHELL_HELP(macfiltdis,"usage:MacAddrDis, receive all frame(don't care MAC filter)");
ADD_TO_IN_SHELL bool_t macfiltdis(char *param)
{
    tagMacDriver      *pDrive;

    pDrive = &gMacDriver;
    MacCtrl(pDrive->devhandle,EN_NETDEV_ADDRFILTER,0);//关闭地址过滤功能
    return true;
}


// =============================================================================
// 功能：GMAC网卡和DJYIP驱动初始化函数
// 参数：para
// 返回值  ：true成功  false失败。
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
    MacCtrl(pDrive->devhandle,EN_NETDEV_ADDRFILTER,1);//开通地址过滤功能

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


extern void GMAC_InLowPowerPinCfg(void);
extern void GMAC_OutLowPowerPinCfg(void);

// =============================================================================
// 功能： 进入或退出低功耗模式
//     时钟使能 控制器使能 GPIO设置相应的状态
// 参数： port 串口号：如 CN_UART1
//     flag 进入低功耗 或退出低功耗标志：如 UART_IN_LOWPOWER
// 返回：true/false
// =============================================================================
bool_t GMAC_LowPowerConfig(u8 flag)
{
    if(flag == InLowPower)
    {
        RCC->AHB1ENR&=~(1<<25);  //禁能ETHMAC时钟
        GMAC_InLowPowerPinCfg();
        return true;
    }
    else if (flag == OutLowPower)
    {
         RCC->AHB1ENR|=1<<25;  //使能ETHMAC时钟
         GMAC_OutLowPowerPinCfg();
         MacReset(0);
        return true;
    }
    else
        return false;
}
