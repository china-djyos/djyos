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
// =============================================================================

// �ļ���     ��cpu_peri_eth.c
// ģ������: MACģ��ײ�Ӳ��������ΪDJYIP��Э��ջ�ض�������
// ģ��汾: V1.00
// ������Ա: chj
// ����ʱ��: 05/14.2018
// =============================================================================
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <sys/socket.h>
#include <cpu_peri.h>
#include "cpu_peri_int_line.h"
#include <board-config.h>

#include "fsl_clock.h"
#include "cpu_peri_eth.h"
#include "dbug.h"
#include "project_config.h"    //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//  extern bool_t ModuleInstall_ETH(const char *devname, u8 *macaddress,\
//                       tagPhyMessage  *phy,
//                          bool_t loop,u32 loopcycle,\
//                          bool_t (*rcvHook)(u8 *buf, u16 len));
//    static u8 ImxMac[] = {CFG_MAC_ADDR0,CFG_MAC_ADDR1,CFG_MAC_ADDR2,CFG_MAC_ADDR3,\
//                          CFG_MAC_ADDR4,CFG_MAC_ADDR5};
//    static tagPhyMessage Phy = {CFG_IMX_PHY_SPEED,CFG_IMX_PHY_DUPLEX};
//    ModuleInstall_ETH(CFG_ETH_DEV_NAME,ImxMac,&Phy,CFG_LOOP_CYCLE,NULL);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip MAC"//CPU��mac����
//parent                       //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                     //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                  //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                    //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium                  //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                    //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"tcpip","lock","int"//������������������������none����ʾ�������������
                                    //����������������������г�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                    //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_MAC == false )
//#warning  " cpu_onchip_MAC  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header              //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_MAC    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,255,
#define CFG_MAC_ADDR0             00//"����MAC��ַ0",
#define CFG_MAC_ADDR1             00//"����MAC��ַ1",
#define CFG_MAC_ADDR2             00//"����MAC��ַ2",
#define CFG_MAC_ADDR3             00//"����MAC��ַ3",
#define CFG_MAC_ADDR4             00//"����MAC��ַ4",
#define CFG_MAC_ADDR5             00//"����MAC��ַ5",
//%$#@num,1000,10000,
#define CFG_LOOP_CYCLE            1000//"������loopʱ��",
//%$#@enum,1,0,
#define CFG_IMX_PHY_SPEED          1//"����phy�ٶ�",Ϊ1��ʾ100M��Ϊ0��ʾ10M
#define CFG_IMX_PHY_DUPLEX         1//"����ȫ/��˫��ģʽ",Ϊ1��ʾȫ˫����Ϊ0��ʾ��˫��
//%$#@string,1,30,
#define CFG_ETH_DEV_NAME          "ETH_DEV_IMXRT1052"//"��������",ע���������豸����
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

#define CN_DEVNAME_LEN 32

#define ETH_RING_NUM        (FSL_FEATURE_ENET_QUEUE)
#define CORE_CLK_FREQ       CLOCK_GetFreq(kCLOCK_AhbClk)
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

typedef struct
{
    ENET_Type*      base;
    enet_handle_t   handle;
    enet_config_t   config;
    enet_buffer_config_t    buffConfig[ETH_RING_NUM];
}tagEthHandle;

typedef struct
{
    //os member
    struct SemaphoreLCB     *rcvsync;          //activate the receive task
    struct MutexLCB         *protect;          //protect the device
    struct NetDev           *devhandle;        //returned by the tcpip stack
    char                    devname[CN_DEVNAME_LEN];
    //hardware
    tagEthHandle            *EthHandle;
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

//define the descriptors and buffers
#define EthRxDescs      (8U)
#define EthTxDescs      (8U)
#define EthRxBufSize    ENET_FRAME_MAX_FRAMELEN
#define EthTxBufSize    ENET_FRAME_MAX_FRAMELEN

/*! @brief Buffer descriptors should be in non-cacheable region and should be align to "ENET_BUFF_ALIGNMENT". */
AT_NONCACHEABLE_SECTION_ALIGN(enet_rx_bd_struct_t RxDscrTab[EthRxDescs], ENET_BUFF_ALIGNMENT);
AT_NONCACHEABLE_SECTION_ALIGN(enet_tx_bd_struct_t TxDscrTab[EthTxDescs], ENET_BUFF_ALIGNMENT);

SDK_ALIGN(u8 Rx_Buff[EthRxDescs][SDK_SIZEALIGN(EthRxBufSize, ENET_BUFF_ALIGNMENT)],\
                                                        ENET_BUFF_ALIGNMENT);
SDK_ALIGN(u8 Tx_Buff[EthTxDescs][SDK_SIZEALIGN(EthTxBufSize, ENET_BUFF_ALIGNMENT)],\
                                                        ENET_BUFF_ALIGNMENT);
static u8 gTxBuffer[EthRxBufSize];      //for sending copy frame
 tagEthHandle   sEthHandle={
    .buffConfig[0]={
            EthRxDescs,
            EthTxDescs,
            SDK_SIZEALIGN(EthRxBufSize, ENET_BUFF_ALIGNMENT),
            SDK_SIZEALIGN(EthTxBufSize, ENET_BUFF_ALIGNMENT),
            &RxDscrTab[0],
            &TxDscrTab[0],
            &Rx_Buff[0][0],
            &Tx_Buff[0][0],
    },
};

static void ETH_HardDrvInit(tagMacDriver *pDrive)
{
    // configure ethernet peripheral (GPIOs, clocks, MAC)
    ENET_Init(  pDrive->EthHandle->base,    \
                &pDrive->EthHandle->handle, \
                &pDrive->EthHandle->config, \
                pDrive->EthHandle->buffConfig,\
                pDrive->macaddr,            \
                CORE_CLK_FREQ);

    /* ��ʼ����Ĭ��ʹ�ܽ��գ������ڷ��ͺ�����ʹ�� */
    ENET_ActiveRead(pDrive->EthHandle->base);
}

void EthOutClkToPhy(ENET_Type* base,u32 freq)
{
    u8 loopdiv=0;
    clock_enet_pll_config_t config = {true, false, 0};
    switch(freq)
    {
        case (25*Mhz):
                loopdiv = 0;
                break;
        case (50*Mhz):
                loopdiv=1;
                break;
        case (100*Mhz):
                loopdiv=2;
                break;
        case (125*Mhz):
                loopdiv=3;
                break;
        default:
            return;
    }
    config.loopDivider = loopdiv;
    CLOCK_EnableClock(kCLOCK_Enet);
    CLOCK_InitEnetPll(&config);
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
    tagEthHandle *EthHandle;
    status_t status;
    enet_data_error_stats_t eErrStatic;
    u32  len=0;
    u8   *dst;

    pDrive = &gMacDriver;
    EthHandle = pDrive->EthHandle;
    // get received frame
    status = ENET_GetRxFrameSize(&EthHandle->handle, &len);
    //len = EthHandle->handle.rxBdCurrent[0]->length;
    if (status == kStatus_ENET_RxFrameError)
    //if(len==0)
    {
        /* Update the received buffer when error happened. */
        /* Get the error information of the received g_frame. */
        ENET_GetRxErrBeforeReadFrame(&EthHandle->handle, &eErrStatic);
        /* update the receive buffer. */
        ENET_ReadFrame(EthHandle->base, &EthHandle->handle, NULL, 0);
        return NULL;
    }

    if(len > 0)
    {
        pkg = PkgMalloc(len,CN_PKLGLST_END);
        if(NULL == pkg)
            pDrive->debuginfo.nopkgCnts++;
    }

    if(NULL != pkg)
    {
        dst = PkgGetCurrentBuffer(pkg);
//      dst = (u8 *)(pkg->buf +pkg->offset);
        status = ENET_ReadFrame(EthHandle->base, &EthHandle->handle, dst, len);
        PkgSetDataLen(pkg, len);
//      pkg->datalen = len;
    }

    return pkg;
}

static bool_t MacSnd(ptu32_t handle,struct NetPkg * pkg,u32 netdevtask)
{
    bool_t             result;
    tagMacDriver      *pDrive;
    tagEthHandle    *EthHandle;
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
        if(EthHandle->handle.txBdCurrent[0]->control & \
                ENET_BUFFDESCRIPTOR_TX_READY_MASK)
        {
            pDrive->debuginfo.sndnobdCnt ++;
            goto NODESCERROR;
        }

        //copy datas to static frame buffer
        tmppkg = pkg;
        dst      = &gTxBuffer[0];
        len = PkgFrameDataCopy(tmppkg,dst);
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
//      }while(NULL != tmppkg );

        if(len < EthTxBufSize)
        {
            if(ENET_SendFrame(EthHandle->base, &EthHandle->handle,\
                            &gTxBuffer[0], len)==kStatus_Success)
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
        Lock_MutexPost(pDrive->protect);
    }

    return result;
}

//u32 ETH_SendData(u8 *buf,u32 len)
//{
//    struct NetPkg     *pkg;
//    tagMacDriver      *pDrive;
//
//    pDrive = &gMacDriver;
//
//    PkgInit(pkg,CN_PKLGLST_END,0,len,buf);  //ֻ��һ����
////  pkg.partnext = NULL;
////  pkg.pkgflag  = (1<<0);
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

static bool_t MacCtrl(struct NetDev * devhandle,u8 cmd,ptu32_t para)
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
                    if((pDrive->EthHandle->config.macSpecialConfig & \
                            (~kENET_ControlRxBroadCastRejectEnable))!=0)
                    {
                        pDrive->EthHandle->config.macSpecialConfig &= \
                                                (~kENET_ControlRxBroadCastRejectEnable);
                        pDrive->EthHandle->base->RCR &= ~ENET_RCR_BC_REJ_MASK;
                    }
                    printk("mac broad enable\n\r");
                    pDrive->macstat.bfd = 0;
                }
                else
                {
                    pDrive->EthHandle->config.macSpecialConfig |= \
                                            kENET_ControlRxBroadCastRejectEnable;
                    pDrive->EthHandle->base->RCR |= ENET_RCR_BC_REJ_MASK;
                    printk("mac broad disable\n\r");
                    pDrive->macstat.bfd = 1;
                }
                result = true;
                break;
            case EN_NETDEV_SETPOINT:
                result = true;
                break;
            case EN_NETDEV_SETMULTI:
               result = true;
                break;
            case EN_NETDEV_SETRECV:
                if(para)
                {
                    pDrive->EthHandle->base->RDAR = ENET_RDAR_RDAR_MASK;
                }
                else
                {
                    pDrive->EthHandle->base->RDAR &= ~ENET_RDAR_RDAR_MASK;
                }
                result = true;
                break;
            case EN_NETDEV_SETSEND:
                if(para)
                {
                    ETH_HardDrvInit(pDrive);
                }
                else
                {
                    ENET_Deinit(pDrive->EthHandle->base);
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
                    ENET_Reset(pDrive->EthHandle->base);
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
}

static ptu32_t __MacRcvTask(void)
{
    struct NetPkg *pkg=NULL;
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
    }
    return 0;
}

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

    debug_printf("*********MAC DEBUG INFO***********\n\r");
    if(pDrive->loop)
    {
        debug_printf("loop/int      :loop\n\r");
        debug_printf("loopcycle     :%d mS\n\r",pDrive->loopcycle);
    }
    else
    {
        debug_printf("loop/int      :int\n\r");
    }

    debug_printf("EthInts       :%d\n\r",pDrive->debuginfo.Ints);
    debug_printf("rcvInts       :%d\n\r",pDrive->debuginfo.rcvInts);
    debug_printf("rcvPkgCnt     :%d\n\r",pDrive->debuginfo.rcvPkgTimes);
    debug_printf("rxoInts       :%d\n\r",pDrive->debuginfo.rxoverInts);
    debug_printf("dmaerrcnt     :%d\n\r",pDrive->debuginfo.dmaerr);
    debug_printf("snd           :%d\n\r",pDrive->debuginfo.sndTimes);
    debug_printf("sndInts       :%d\n\r",pDrive->debuginfo.sndInts);
    debug_printf("sndTimes      :%d\n\r",pDrive->debuginfo.sndTimes);
    debug_printf("sndOkTime     :%d\n\r",pDrive->debuginfo.sndOkTimes);
    debug_printf("sndnobdCnt    :%d\n\r",pDrive->debuginfo.sndnobdCnt);
    debug_printf("rxTaskRun     :%d\n\r",pDrive->debuginfo.rxTaskRun);
    debug_printf("rcvSpeed      :%d\n\r",rcvSpeed);
    debug_printf("intSpeed      :%d\n\r",intSpeed);
    debug_printf("rstTimes      :%d\n\r",pDrive->debuginfo.rsttimes);
    debug_printf("**********************************\n\r");
    return true;
}

bool_t MacReset(char *param)
{
    tagMacDriver   *pDrive = &gMacDriver;

    __MacReset(pDrive);
    return true;
}

#include <shell.h>

 u32 ETH_IntHandler(ufast_t IntLine)
{
    tagMacDriver *pDrive;
    tagEthHandle *EthHandle;
    uint32_t mask = kENET_RxFrameInterrupt | kENET_RxBufferInterrupt;

    pDrive = &gMacDriver;
    EthHandle = pDrive->EthHandle;

    pDrive->debuginfo.Ints++;

     while (mask & EthHandle->base->EIR)
     {
         /* Clear the transmit interrupt event. */
         EthHandle->base->EIR = mask;
         Lock_SempPost(pDrive->rcvsync);
     }
    return 0;
}

// =============================================================================
// ���ܣ�MAC������DJYIP������ʼ������
// ������para
// ����ֵ  ��true�ɹ�  falseʧ�ܡ�
// =============================================================================
bool_t ModuleInstall_ETH(const char *devname, u8 *macaddress,\
                         tagPhyMessage  *phy,
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
    pDrive->macstat.duplex = (phy->duplex==CPU_ETH_FULL_DUPLEX ? 1:0);    //duplex full
    pDrive->macstat.speed = (phy->speed==CPU_ETH_SPEED_100M ? 1:0);     //100 Mb
    pDrive->macstat.bfd = 0;       //broadcast frames disable
    pDrive->macstat.mii = 0;       //use RMII mode

    pDrive->EthHandle = &sEthHandle;
    pDrive->EthHandle->base = ENET;
    ENET_GetDefaultConfig(&pDrive->EthHandle->config);
    pDrive->EthHandle->config.miiSpeed = (phy->speed==CPU_ETH_SPEED_100M ?\
                                                kENET_MiiSpeed100M:kENET_MiiSpeed10M);;
    pDrive->EthHandle->config.miiDuplex = (phy->duplex==CPU_ETH_FULL_DUPLEX ? \
                                                kENET_MiiFullDuplex:kENET_MiiHalfDuplex);
    pDrive->EthHandle->config.miiMode = kENET_RmiiMode;
//  pDrive->EthHandle->config.intCoalesceCfg = &intcoalesce;
//  pDrive->EthHandle->config.intCoalesceCfg->rxCoalesceFrameCount[0] = 10;
    pDrive->EthHandle->config.interrupt = kENET_RxBufferInterrupt|kENET_RxFrameInterrupt;
    pDrive->EthHandle->config.txAccelerConfig = \
                            kENET_TxAccelIpCheckEnabled|kENET_TxAccelProtoCheckEnabled;
    pDrive->EthHandle->config.rxAccelerConfig = \
            kENET_RxAccelIpCheckEnabled|kENET_RxAccelProtoCheckEnabled;
//  pDrive->EthHandle->config.txFifoWatermark =32;
//  pDrive->EthHandle->config.rxFifoFullThreshold = 32;
    ETH_HardDrvInit(pDrive);
    //all the configuration has set in the pDrive now,we need some sys assistant
    //application some semphore and mutex
    pDrive->rcvsync = Lock_SempCreate(1,0,CN_BLOCK_FIFO,NULL);
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
    devpara.mtu = CN_ETH_MTU;
    devpara.Private = (ptu32_t)pDrive;
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
        Int_Register(CN_INT_LINE_ENET);
        Int_SetClearType(CN_INT_LINE_ENET,CN_INT_CLEAR_AUTO);
        Int_IsrConnect(CN_INT_LINE_ENET,ETH_IntHandler);
        Int_SettoAsynSignal(CN_INT_LINE_ENET);
        Int_ClearLine(CN_INT_LINE_ENET);
        Int_RestoreAsynLine(CN_INT_LINE_ENET);
    }

    debug_printf("%s:Install Net Device %s success\n\r",__FUNCTION__,devname);
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
debug_printf("%s:Install Net Device %s failed\n\r",__FUNCTION__,devname);
    return false;
}


