// =============================================================================

// 文件名     ：cpu_peri_eth.c
// 模块描述: GMAC模块底层硬件驱动，为DJYIP的协议栈特定的驱动
// 模块版本: V1.00
// 创建人员: huzb
// 创建时间: 02/14.2019
// =============================================================================
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <sys/socket.h>
#include <cpu_peri.h>
#include <board-config.h>
#include <netbsp.h>
#include "dbug.h"
#include "SCM6XX_ALL.h"
#include "shell.h"

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_ETH(const char *devname, bool_t loop,u32 loopcycle);
//    ModuleInstall_ETH(CFG_GMAC_NAME,CFG_GMAC_LOOP_MODE,CFG_GMAC_LOOP_CYCLE);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip MAC"//CPU的mac驱动
//parent:"tcpip"     //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"int","tcpip","heap","lock"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_MAC == false )
//#warning  " cpu_onchip_MAC  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_MAC    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,10000,
#define CFG_GMAC_LOOP_MODE         1            //"是否使用loop模式",
#define CFG_GMAC_LOOP_CYCLE        1000         //"loop的时间",
#define CFG_GMAC_MAC_ADDR0         00           //"MAC ADDR0",
#define CFG_GMAC_MAC_ADDR1         00           //"MAC ADDR1",
#define CFG_GMAC_MAC_ADDR2         00           //"MAC ADDR2",
#define CFG_GMAC_MAC_ADDR3         00           //"MAC ADDR3",
#define CFG_GMAC_MAC_ADDR4         00           //"MAC ADDR4",
#define CFG_GMAC_MAC_ADDR5         00           //"MAC ADDR5",
//%$#@enum,true,false,
//%$#@string,1,32,
#define CFG_GMAC_NAME              ("scm6xx")   //"网卡设备名称",
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
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

static  ETH_TypeDef *sgpt_Gmac[CN_GMAC_MAX] =
{
    ( ETH_TypeDef *)CN_ETH0_BASE,
    ( ETH_TypeDef *)CN_ETH1_BASE,
};

#define  ETH_ERROR              ((uint32_t)0)
#define  ETH_SUCCESS            ((uint32_t)1)
#define  ETH_DMARxDesc_FrameLengthShift           16

/* Global pointers on Tx and Rx descriptor used to track transmit and receive descriptors */

extern ETH_DMADESCTypeDef  *DMATxDescToSet;
extern ETH_DMADESCTypeDef  *DMARxDescToGet;


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
    struct NetDev           *devhandle;        //returned by the tcpip stack
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

#define EthRxDescs      5           //接收缓冲器的数量
#define EthTxDescs      5           //发送缓冲器的数量

#define EthRxBufSize    1520        //ETH_HEADER + ETH_EXTRA + MAX_ETH_PAYLOAD + ETH_CRC
#define EthTxBufSize    1520        //ETH_HEADER + ETH_EXTRA + MAX_ETH_PAYLOAD + ETH_CRC

ETH_DMADESCTypeDef  DMARxDscrTab[EthRxDescs];
ETH_DMADESCTypeDef  DMATxDscrTab[EthTxDescs];

u8 Rx_Buff[EthRxDescs][EthRxBufSize];
u8 Tx_Buff[EthTxDescs][EthTxBufSize];
#endif
static u8 gTxBuffer[EthRxBufSize];      //for sending copy frame
extern ETH_TypeDef *g_eth;
extern void Ethernet_Configuration(ETH_TypeDef *ETH);

#define  CHECKSUM_GEN_ICMP      (0)  //0-使用软件校验，关闭硬件校验
#define PHY_ADDRESS             0x01  //DP83848地址，根据核心板硬件连接决定
//#define  MAC0_MII             (0)
ETH_TypeDef *g_eth;
// =============================================================================
// 功能：以太网配置
// 参数：
// 返回值  ：
// =============================================================================
void Ethernet_Configuration(ETH_TypeDef *ETH)
{
    ETH_InitTypeDef ETH_InitStructure;
    #ifndef MAC0_MII
    #define MAC0_MII
    #endif
    #ifdef MAC0_MII
        SCU_Configure_GMAC_Mode(SYS_CTRL_GMAC_MODE_MII) ;

        IO_Configure_PullDown(P_GPIO111, DISABLE);  //MCO pull-down disable
        MII0_ConfigDS(IO_DS_MII0_18);

        MII0_InitPad();
    #else
        SCU_Configure_GMAC_Mode(SYS_CTRL_GMAC_MODE_RMII) ;//

        IO_Configure_PullDown(P_GPIO111, DISABLE);  //
        MII0_ConfigDS(IO_DS_MII0_18);

        RMII0_InitPad();
    #endif

        MCO_InitPad();

        ETH_DeInit(ETH);

        ETH_SoftwareReset(ETH);

        while (ETH_GetSoftwareResetStatus(ETH) == SET);//

        ETH_StructInit(&ETH_InitStructure);

        ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable;
        ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;
        ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
        ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;
        ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Disable;
        ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;
        ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;
        ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;
        ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;

        #if  !CHECKSUM_GEN_ICMP
        ETH_InitStructure.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable;
        #endif

        ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable;
        ETH_InitStructure.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;
        ETH_InitStructure.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;

        ETH_InitStructure.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;
        ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;
        ETH_InitStructure.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;
        ETH_InitStructure.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;
        ETH_InitStructure.ETH_FixedBurst = ETH_FixedBurst_Enable;
        ETH_InitStructure.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;
        ETH_InitStructure.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;
        ETH_InitStructure.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;
        ETH_Init(ETH, &ETH_InitStructure,PHY_ADDRESS);

        ETH_DMAITConfig(ETH, ETH_DMA_IT_NIS|ETH_DMA_IT_R,ENABLE);
        ETH_DMAITConfig(ETH, ETH_DMA_IT_T,DISABLE);
}
// =============================================================================
// 功能：获取网卡接收到的数据帧fram
// 参数：
// 返回值：
// =============================================================================
FrameTypeDef ETH_RxPkt_ChainMode(void)
{
    ETH_TypeDef *EthHandle;
    EthHandle = gMacDriver.EthHandle;

    u32 framelength = 0;
    FrameTypeDef frame = {0,0};

    if((DMARxDescToGet->Status & ETH_DMARxDesc_OWN) != (u32)RESET) //如果DMA占用符成立
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

// =============================================================================
// 功能：网卡时钟初始化
// 参数：
// 返回值：
// =============================================================================
void ETH_CLK_ENABLE(void)
{
    CLK_Enable_Peripheral_Clk(GMAC0); //使能GMAC0时钟
    g_eth = SGCC_GMAC0_P;  //将GMAC0寄存器启始地址传给g_eth
}
// =============================================================================
// 功能：网卡硬件初始化
// 参数：
// 返回值：
// =============================================================================
static void ETH_HardDrvInit(tagMacDriver *pDrive)
{
    ETH_TypeDef  *ETH;

    ETH = pDrive->EthHandle;   //寄存器地址传给ETH

    if(pDrive != NULL)
    {

        Ethernet_Configuration(ETH); //配置eth,管脚等
        ETH_MACAddressConfig(ETH, ETH_MAC_Address0,pDrive->macaddr); //配置MAC地址，在boardnetcfg.c中定义，通过参数传进来

        ETH_DMATxDescChainInit(ETH, DMATxDscrTab, &Tx_Buff[0][0], EthTxDescs); //初始化发送DMA描述符链表  (from ethernetif.c)
        ETH_DMARxDescChainInit(ETH, DMARxDscrTab, &Rx_Buff[0][0], EthRxDescs); //初始化接收DMA描述符链表

        for(u8 i=0; i<EthRxDescs; i++) //开启DMA描述符接收中断
    {
            ETH_DMARxDescReceiveITConfig(&DMARxDscrTab[i], ENABLE);
        }
        #if  !CHECKSUM_GEN_ICMP  //判断硬件检验还是软件检验
             for(u8 i=0; i<EthTxDescs; i++) //发送帧检验
             {
                ETH_DMATxDescChecksumInsertionConfig(&DMATxDscrTab[i], ETH_DMATxDesc_ChecksumTCPUDPICMPFull); //TODO:这里是否需要去掉？
             }
        #endif
        ETH_Start(ETH); //开启MAC和DMA
        printf("ETH hard init ok!\n\r");
    }
    else
    {
        printf("ETH hard init filed!\n\r");
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

// =============================================================================
// 功能：网卡接收数据函数
// 说明：找接收Buffer地址 、以及数据帧长度 、拷贝到Pkg往上传输
// 参数：
// 返回值：
// =============================================================================
static struct NetPkg *__MacRcv(void* devhandle)
{
    struct NetPkg     *pkg = NULL;
    tagMacDriver      *pDrive;
    ETH_TypeDef       *EthHandle;
    FrameTypeDef      frame;

    u16 CopyBytes,len;
    u8   *dst,*src;

    pDrive = &gMacDriver;
    EthHandle = pDrive->EthHandle;

    //Lenth to copy to Pkg
    if((len = ETH_GetRxPktSize())==0)
    {
        len=0;
        return NULL;
    }
    else
    {
        len = len - 4;  //获取的帧长度包含4字节crc,然后减去4字节CRC
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
        //Here Return DMARxDescToGet Point To Next DMARxDescToGet
        frame = ETH_RxPkt_ChainMode();
        dst = PkgGetCurrentBuffer(pkg);       //Pkg Buf Address
        src = (u8 *)frame.buffer;                  //Get Buffer Address

        while(CopyBytes > EthRxBufSize)
        {
            memcpy(dst,src,EthRxBufSize);  //copy the first part
            CopyBytes -= EthRxBufSize;
            dst += EthRxBufSize;
            //Set Own The Current Rev Description DMA
            frame.descriptor->Status |= ETH_DMARxDesc_OWN;
            //get the rest data len
            frame = ETH_RxPkt_ChainMode();
            src = (u8 *)frame.buffer;
        }
        memcpy(dst, src, CopyBytes);
        frame.descriptor->Status |= ETH_DMARxDesc_OWN;
        PkgSetDataLen(pkg, len);
//      EthHandle->RxFrameInfos.SegCount =0;
    }
    else
    {
        DMARxDescToGet->Status |= ETH_DMARxDesc_OWN;
        printf("malloc pkg failed\n\r");
    }

    // When Rx Buffer unavailable flag is set: clear it and resume reception
    if ((EthHandle->DMASR & ETH_DMASR_RBUS) != (u32)RESET)
    {
        EthHandle->DMASR = ETH_DMASR_RBUS; //清除接收缓冲区不可用标志 Clear RBUS ETHERNET DMA flag
        EthHandle->DMARPDR = 0;            //恢复DMA接收 Resume DMA reception
        pDrive->debuginfo.rxoverInts++;
    }
    return pkg;
}

// =============================================================================
// 功能：网卡底层发送数据帧fram
// 参数：
// 返回值：
// =============================================================================
u32 ETH_TxPkt_ChainMode(u16 FrameLength)
{
    if((DMATxDescToSet->Status & ETH_DMATxDesc_OWN) != (u32)RESET)
    {
        return ETH_ERROR;
    }

    DMATxDescToSet->ControlBufferSize |= ETH_DMATxDesc_IC; //zhangyx

    DMATxDescToSet->ControlBufferSize &= ~ETH_DMATxDesc_TBS1;
    DMATxDescToSet->ControlBufferSize |= (FrameLength & ETH_DMATxDesc_TBS1);

    DMATxDescToSet->ControlBufferSize |= ETH_DMATxDesc_LS | ETH_DMATxDesc_FS;

    DMATxDescToSet->Status |= ETH_DMATxDesc_OWN;


    if ((g_eth->DMASR & ETH_DMASR_TBUS) != (u32)RESET)
    {
        g_eth->DMASR = ETH_DMASR_TBUS;
        g_eth->DMATPDR = 0;
    }

    DMATxDescToSet = (ETH_DMADESCTypeDef*) (DMATxDescToSet->Buffer2NextDescAddr);
    return ETH_SUCCESS;
}

//Get the next DMA Description

u32 ETH_GetCurrentTxBuffer(void)
{
  return (DMATxDescToSet->Buffer1Addr);
}


// =============================================================================
// 功能：网卡发送数据至协议层
// 说明：
// 参数：
// 返回值：
// =============================================================================
static bool_t MacSnd(void* handle,struct NetPkg * pkg, u32 netdevtask)
{
    bool_t             result;
    tagMacDriver      *pDrive;
    ETH_TypeDef       *EthHandle;
    struct NetPkg     *tmppkg;
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

        //Is this buffer available? If not, goto error
        if((DMATxDescToSet->Status & ETH_DMATxDesc_OWN) != (uint32_t)RESET)
        {
            pDrive->debuginfo.sndnobdCnt ++;
            goto NODESCERROR;
        }
        dst      = &gTxBuffer[0];
        //copy datas to static frame buffer
        tmppkg = pkg;
        len = PkgFrameDataCopy(tmppkg, dst);

        if(len < EthTxBufSize)
        {
            src = &gTxBuffer[0];
            //get current buffer point
            dst = (u8 *)ETH_GetCurrentTxBuffer();
            memcpy(dst,src ,len);
            // start to send
            ETH_TxPkt_ChainMode(len); //网卡底层发送数据至协议层
                pDrive->debuginfo.sndOkTimes++;
                result = true;
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
//    pkg.pkgflag  = (1<<0);  //只有一个包
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

//u32 ETH_SendData(u8 *buf,u32 len)
//{
//    struct NetPkg      pkg;
//    tagMacDriver      *pDrive;
//
//    pDrive = &gMacDriver;
//
//    PkgInit(pkg,CN_PKLGLST_END,0,len,buf);  //只有一个包
////  pkg.partnext = NULL;
////  pkg.pkgflag  = (1<<0);  //只有一个包
////  pkg.offset   = 0;
////  pkg.datalen  = len;
////  pkg.buf      = buf;
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
    ETH_TypeDef *ETH;

    pDrive = &gMacDriver;
    ETH = pDrive->EthHandle;
    pDrive->debuginfo.Ints++;

    ETH_DMAClearITPendingBit(SGCC_GMAC0_P, ETH_DMA_IT_NIS); //清DMA中断标志位

    if (ETH_GetDMAFlagStatus(ETH, ETH_DMA_FLAG_R))
    {
    pDrive->debuginfo.rcvInts++;
        Lock_SempPost(pDrive->rcvsync); //释放信号量
        ETH_DMAClearITPendingBit(SGCC_GMAC0_P, ETH_DMA_IT_R); //清接收中断标志位
    }
//    if (ETH_GetDMAFlagStatus(ETH, ETH_DMA_FLAG_T))
//    {
//        pDrive->debuginfo.sndInts++;
//        ETH_DMAClearITPendingBit(SGCC_GMAC0_P, ETH_DMA_IT_T); //清发送中断标志位
//    }

    else if(ETH_GetDMAFlagStatus(ETH, ETH_DMA_FLAG_AIS))
    {
        pDrive->debuginfo.dmaerr++;
        ETH_DMAClearITPendingBit(SGCC_GMAC0_P, ETH_DMA_IT_AIS);
    }

    return 0;
}

// =============================================================================
// 功能：网卡控制函数
// 说明：
// 参数：
// 返回值  ：
// =============================================================================
static bool_t MacCtrl(struct NetDev *devhandle,u8 cmd,ptu32_t para)
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
                if(para)//绝对单播模式
                {
                    __macbitsclear(pDrive->EthHandle->MACFFR,ETH_MACFFR_HU);
                }
                else    //散列单播模式
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
                ETH_MACAddressConfig(pDrive->EthHandle, ETH_MAC_Address0,(u8 *)para);
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

// =============================================================================
// 功能：接收任务
// 说明：
// 参数：
// 返回值  ：
// =============================================================================
static ptu32_t __MacRcvTask(void)
{
    struct NetPkg *pkg = NULL;
    struct NetDev *handle;
    u8        *rawbuf;
    u16        len;
    tagMacDriver      *pDrive;
    pDrive = &gMacDriver;

    DJY_GetEventPara((ptu32_t *)&handle,NULL);

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
                NetDev_FlowCtrl(handle,NetDev_FrameType(PkgGetCurrentBuffer(pkg),
                                                      PkgGetDataLen(pkg)));
                //you could alse use the soft method
                if(NULL != pDrive->fnrcvhook)
                {
                    rawbuf = PkgGetCurrentBuffer(pkg);
                    len = PkgGetDataLen(pkg);
                    pDrive->fnrcvhook(rawbuf,len);
                }
                else
                {
                    Link_NetDevPush(handle,pkg);  //丢给协议栈
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

// =============================================================================
// 功能：创建接收数据包任务线程
// 说明：
// 参数：
// 返回值  ：
// =============================================================================
static bool_t __CreateRcvTask(struct NetDev *handle)
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


// =============================================================================
// 功能：shell调试用
// 说明：how the gmac status
// 参数：
// 返回值  ：
// =============================================================================
//bool_t macdebuginfo(char *param)
bool_t mac(char *param)
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
// 功能：SCM610网卡和DJYIP驱动初始化函数
// 参数：para
// 返回值  ：true成功  false失败。
// =============================================================================
bool_t ModuleInstall_ETH(const char *devname, bool_t loop,u32 loopcycle)
{
    tagMacDriver   *pDrive = &gMacDriver;
    struct NetDevPara   devpara;
    u8 intLine;

    u8 macAddress[6] =
    {
         CFG_GMAC_MAC_ADDR0,CFG_GMAC_MAC_ADDR1,
         CFG_GMAC_MAC_ADDR2,CFG_GMAC_MAC_ADDR3,
         CFG_GMAC_MAC_ADDR4,CFG_GMAC_MAC_ADDR5
    };

    memset((void *)pDrive,0,sizeof(tagMacDriver));
    //copy the config para to the pDrive
    memcpy(pDrive->devname,devname,CN_DEVNAME_LEN-1);
    memcpy((void *)pDrive->macaddr,macAddress,CN_MACADDR_LEN);

    if(loop)
    {
        pDrive->loop = 1;
    }

    pDrive->loopcycle = loopcycle;
    pDrive->fnrcvhook = NULL;
    //set the work mode and speed
    pDrive->macstat.pm = 0;         //primmiscuous mode:= 1,no MAC addr filte
    pDrive->macstat.duplex = 1;    //duplex full
    pDrive->macstat.speed = 1;     //100 Mb
    pDrive->macstat.bfd = 0;       //broadcast frames disable
    pDrive->macstat.mii = 0;       //use RMII mode

    //默认使用GMAC0
    pDrive->EthHandle  = sgpt_Gmac[0]; //MAC寄存器启始地址

    ETH_CLK_ENABLE(); //ETH所有相关时钟初始化

    ETH_HardDrvInit(pDrive);  //MAC控制器硬件初始化

    pDrive->rcvsync = Lock_SempCreate(1,1,CN_BLOCK_FIFO,NULL); //信号量

    if(NULL == pDrive->rcvsync)
    {
        goto RCVSYNC_FAILED;
    }

    pDrive->protect = Lock_MutexCreate(NULL); //互斥量
    if(NULL == pDrive->protect)
    {
        goto DEVPROTECT_FAILED;
    }

    //install the net device
    devpara.ifctrl = MacCtrl;
    devpara.ifsend = MacSnd;
    devpara.iftype = EN_LINK_ETHERNET;
    devpara.devfunc = CN_IPDEV_ALL;
    memcpy(devpara.mac,macAddress,6);
    devpara.name = (char *)pDrive->devname;
    devpara.mtu = CN_ETH_MTU;                     //Max Transmit Unit
    devpara.Private = (ptu32_t)pDrive;
    pDrive->devhandle = NetDev_Install(&devpara);

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


