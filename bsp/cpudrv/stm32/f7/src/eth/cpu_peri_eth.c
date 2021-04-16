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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
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
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_eth.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_ETH(void);
//    ModuleInstall_ETH( );
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip ETH"//CPU的mac驱动
//parent:"tcpip"       //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"int","tcpip","lock"//该组件的依赖组件名（可以是none，表示无依赖组件），
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
//%$#@string,1,32,
#define CFG_ETH_NETCARD_NAME    "STM32F7_ETH"   //"网卡名称",
//%$#@num,1000,10000,
#define CFG_ETH_LOOP_CYCLE      1000         //"网卡轮询周期(uS)",中断模式无须填写
//%$#@enum,true,false,
#define CFG_ETH_LOOP_ENABLE     true            //"网卡接收是否轮询",
#define CFG_ETH_HARD_MAC_ADDR   true            //"硬件生成Mac地址",
//%$#@num,0,255,
#define CFG_ETH_MAC_ADDR0      00           //"MAC ADDR0",若选中"硬件生成Mac地址",则无须填写
#define CFG_ETH_MAC_ADDR1      02           //"MAC ADDR1",若选中"硬件生成Mac地址",则无须填写
#define CFG_ETH_MAC_ADDR2      03           //"MAC ADDR2",若选中"硬件生成Mac地址",则无须填写
#define CFG_ETH_MAC_ADDR3      04           //"MAC ADDR3",若选中"硬件生成Mac地址",则无须填写
#define CFG_ETH_MAC_ADDR4      05           //"MAC ADDR4",若选中"硬件生成Mac地址",则无须填写
#define CFG_ETH_MAC_ADDR5      06           //"MAC ADDR5",若选中"硬件生成Mac地址",则无须填写
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束

//@#$%component end configure
// =============================================================================

bool_t MacReset(char *param);
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
    struct NetDev           *devhandle;        //returned by the tcpip stack
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
    u8 loop;          //1 use the loop mode while 0 interrupt mode
    u32 loopcycle;    //unit:uS
    fnRcvDeal     fnrcvhook;                  //rcv hook function
    tagMacDebug   debuginfo;                  //debug info
}tagMacDriver;
static tagMacDriver  gMacDriver;

#ifdef __GNUC__

//define the descriptors and buffers
#define EthRxDescs      8
#define EthTxDescs      8
#define EthRxBufSize    (1528UL)
#define EthTxBufSize    (1528UL)
//以太网描述符和缓冲区
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

static struct NetPkg *__MacRcv(struct NetDev *devhandle)
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
        dst = PkgGetCurrentBuffer(pkg);
//      dst = (u8 *)(pkg->buf +pkg->offset);
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

        PkgSetDataLen(pkg, len);
//      pkg->datalen = len;
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

static bool_t MacSnd(struct NetDev* handle,struct NetPkg * pkg, u32 netdevtask)
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
        len = PkgFrameDataCopy(tmppkg, dst);
//        do
//        {
//            src = (tmppkg->buf + tmppkg->offset);
//            memcpy(dst,src,PkgGetDataLen(tmppkg));
//            dst      += PkgGetDataLen(tmppkg);
//            len      += PkgGetDataLen(tmppkg);
////          memcpy(dst,src,tmppkg->datalen);
////          dst      += tmppkg->datalen;
////          len      += tmppkg->datalen;
//            if(PkgIsBufferEnd(tmppkg))
//            {
//                tmppkg = NULL;
//                break;
//            }
//            else
//            {
//                tmppkg = PkgGetNextUnit(tmppkg);
//            }
//        }while(NULL != tmppkg );

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

__attribute__((weak)) void GMAC_InLowPowerPinCfg(void)
{
    printf("\r\n请在板件驱动中提供进入低功耗时配置网卡引脚的函数。");
}
__attribute__((weak))  void GMAC_OutLowPowerPinCfg(void)
{
    printf("\r\n请在板件驱动中提供离开低功耗时配置网卡引脚的函数。");
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
                    RCC->AHB1ENR&=~(1<<25);  //禁能ETHMAC时钟
                    GMAC_InLowPowerPinCfg();
                }
                else
                {
                     RCC->AHB1ENR|=1<<25;  //使能ETHMAC时钟
                     GMAC_OutLowPowerPinCfg();
                     MacReset(0);
                }
                result = true;
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



    u32 value;
    u32 resettimes= 0;
    time_t printtime;

    DJY_GetEventPara((ptu32_t *)&handle,NULL);
    value = pDrive->EthHandle->Instance->MMCRFCECR;
//  addr = (u32 *)((u32)ETH + 0x194);
//  value =*addr;
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
        //中断模式下，loopcycle = CN_TIMEOUT_FOREVER，信号量将被中断post。
        //轮询模式下，没人会post信号量，则固定按loopcycle为周期轮询网卡。
        //故，中断模式和轮询模式，源码是一样的。
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
//                  rawbuf = pkg->buf + pkg->offset;
                    len = PkgGetDataLen(pkg);
                    pDrive->fnrcvhook(rawbuf,len);
                }
                else
                {
                    Link_NetDevPush(handle,pkg);
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

        //check if any crc error get then reset the mac
        value = pDrive->EthHandle->Instance->MMCRFCECR;
//      addr = (u32 *)((u32)ETH + 0x194);
//      value =*addr;
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
static bool_t __CreateRcvTask(struct NetDev* handle)
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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

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
    printf("rcvnopkgCnt   :%d\n\r",pDrive->debuginfo.nopkgCnts);
    printf("rxTaskRun     :%d\n\r",pDrive->debuginfo.rxTaskRun);
    printf("rcvSpeed      :%d\n\r",rcvSpeed);
    printf("intSpeed      :%d\n\r",intSpeed);
    printf("rstTimes      :%d\n\r",pDrive->debuginfo.rsttimes);
    printf("**********************************\n\r");
    return true;
}

#define CN_ETH_REG_BASE   ((u32)ETH)
bool_t MacReg(char *param)
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

    addr = (u32 *)(CN_ETH_REG_BASE + 0x34);
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
    addr = (u32 *)(CN_ETH_REG_BASE + 0x100);
    num = 7;
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
    num = 10;
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
        HAL_ETH_StartTransmit(pDrive->EthHandle);
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
        HAL_ETH_StopTransmit(pDrive->EthHandle);
        Lock_MutexPost(pDrive->protect);
    }
    return true;
}

bool_t MacAddrFilterEn(char *param)
{
    tagMacDriver      *pDrive;

    pDrive = &gMacDriver;
    MacCtrl(pDrive->devhandle,EN_NETDEV_ADDRFILTER,1);//开通地址过滤功能
    return true;
}
bool_t MacAddrFilterDis(char *param)
{
    tagMacDriver      *pDrive;

    pDrive = &gMacDriver;
    MacCtrl(pDrive->devhandle,EN_NETDEV_ADDRFILTER,0);//关闭地址过滤功能
    return true;
}
#pragma GCC diagnostic pop


// =============================================================================
// 功能：GMAC网卡和DJYIP驱动初始化函数
// 参数：para
// 返回值  ：true成功  false失败。
// =============================================================================
bool_t ModuleInstall_ETH(void)
{
    tagMacDriver   *pDrive = &gMacDriver;
    struct NetDevPara   devpara;
    //GET THE SIGNATURE OF THE DEVICE
#if (CFG_ETH_HARD_MAC_ADDR == true)
    u32  signature[3];
    u8 gc_NetMac[CN_MACADDR_LEN];
    memset(signature,0,sizeof(signature));
    GetCpuID(&signature[0],&signature[1],&signature[2]);
    printk("CPU SIGNATURE:%08X-%08X-%08X\n\r",signature[0],signature[1],signature[2]);
    //use the signature as the mac address
    signature[0] = signature[0]^signature[1]^signature[2];
    //memcpy(gc_NetMac,&signature[0],CN_MACADDR_LEN);
    memcpy(&gc_NetMac[2], &signature[0], 4);
    gc_NetMac[1] = (u8)(signature[1]&0xFF);
    gc_NetMac[0] = 0x00;      //根据mac的规定，第一字节某位置为1表示广播或者组播
//    if (gc_NetMac[5]==0) {
//        gc_NetMac[5]=0xFF;
//    }
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
    printf("mac: %02x-%02x-%02x-%02x-%02x-%02x！\r\n",
            gc_NetMac[0], gc_NetMac[1],gc_NetMac[2], gc_NetMac[3],gc_NetMac[4], gc_NetMac[5]);
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
    memcpy(devpara.mac,gc_NetMac,CN_MACADDR_LEN);
    devpara.name = (char *)pDrive->devname;
    devpara.mtu = CN_ETH_MTU;
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

    if(false == CFG_ETH_LOOP_ENABLE)
    {
        Int_Register(CN_INT_LINE_ETH);
        Int_SettoAsynSignal(CN_INT_LINE_ETH);
        Int_ClearLine(CN_INT_LINE_ETH);
        Int_IsrConnect(CN_INT_LINE_ETH,ETH_IntHandler);
        Int_ContactLine(CN_INT_LINE_ETH);
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


ADD_TO_ROUTINE_SHELL(macfiltdis,MacAddrFilterDis,"usage:MacAddrDis, receive all frame(don't care MAC filter)");
ADD_TO_ROUTINE_SHELL(macfilten,MacAddrFilterEn,"usage:MacAddrEn, don't receive all frame(MAC filter)");
ADD_TO_ROUTINE_SHELL(macsnden,MacSndEn,"usage:MacSndEn");
ADD_TO_ROUTINE_SHELL(macsnddis,MacSndDis,"usage:MacSndDis");
ADD_TO_ROUTINE_SHELL(macreset,MacReset,"usage:reset gmac");
ADD_TO_ROUTINE_SHELL(macreg,MacReg,"usage:MacReg");
ADD_TO_ROUTINE_SHELL(mac,macdebuginfo,"usage:gmac");
