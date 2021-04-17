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
#include <netbsp.h>
#include "dbug.h"
#include "SCM6XX_ALL.h"
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
//component name:"cpu onchip MAC"//CPU��mac����
//parent:"tcpip"     //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"int","tcpip","heap","lock"//������������������������none����ʾ�������������
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
#define CFG_GMAC_NAME              ("scm6xx")   //"�����豸����",
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
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
extern ETH_TypeDef *g_eth;
extern void Ethernet_Configuration(ETH_TypeDef *ETH);

#define  CHECKSUM_GEN_ICMP      (0)  //0-ʹ�����У�飬�ر�Ӳ��У��
#define PHY_ADDRESS             0x01  //DP83848��ַ�����ݺ��İ�Ӳ�����Ӿ���
//#define  MAC0_MII             (0)
ETH_TypeDef *g_eth;
// =============================================================================
// ���ܣ���̫������
// ������
// ����ֵ  ��
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
// ���ܣ���ȡ�������յ�������֡fram
// ������
// ����ֵ��
// =============================================================================
FrameTypeDef ETH_RxPkt_ChainMode(void)
{
    ETH_TypeDef *EthHandle;
    EthHandle = gMacDriver.EthHandle;

    u32 framelength = 0;
    FrameTypeDef frame = {0,0};

    if((DMARxDescToGet->Status & ETH_DMARxDesc_OWN) != (u32)RESET) //���DMAռ�÷�����
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
// ���ܣ�����ʱ�ӳ�ʼ��
// ������
// ����ֵ��
// =============================================================================
void ETH_CLK_ENABLE(void)
{
    CLK_Enable_Peripheral_Clk(GMAC0); //ʹ��GMAC0ʱ��
    g_eth = SGCC_GMAC0_P;  //��GMAC0�Ĵ�����ʼ��ַ����g_eth
}
// =============================================================================
// ���ܣ�����Ӳ����ʼ��
// ������
// ����ֵ��
// =============================================================================
static void ETH_HardDrvInit(tagMacDriver *pDrive)
{
    ETH_TypeDef  *ETH;

    ETH = pDrive->EthHandle;   //�Ĵ�����ַ����ETH

    if(pDrive != NULL)
    {

        Ethernet_Configuration(ETH); //����eth,�ܽŵ�
        ETH_MACAddressConfig(ETH, ETH_MAC_Address0,pDrive->macaddr); //����MAC��ַ����boardnetcfg.c�ж��壬ͨ������������

        ETH_DMATxDescChainInit(ETH, DMATxDscrTab, &Tx_Buff[0][0], EthTxDescs); //��ʼ������DMA����������  (from ethernetif.c)
        ETH_DMARxDescChainInit(ETH, DMARxDscrTab, &Rx_Buff[0][0], EthRxDescs); //��ʼ������DMA����������

        for(u8 i=0; i<EthRxDescs; i++) //����DMA�����������ж�
    {
            ETH_DMARxDescReceiveITConfig(&DMARxDscrTab[i], ENABLE);
        }
        #if  !CHECKSUM_GEN_ICMP  //�ж�Ӳ�����黹���������
             for(u8 i=0; i<EthTxDescs; i++) //����֡����
             {
                ETH_DMATxDescChecksumInsertionConfig(&DMATxDscrTab[i], ETH_DMATxDesc_ChecksumTCPUDPICMPFull); //TODO:�����Ƿ���Ҫȥ����
             }
        #endif
        ETH_Start(ETH); //����MAC��DMA
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
// ���ܣ������������ݺ���
// ˵�����ҽ���Buffer��ַ ���Լ�����֡���� ��������Pkg���ϴ���
// ������
// ����ֵ��
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
        len = len - 4;  //��ȡ��֡���Ȱ���4�ֽ�crc,Ȼ���ȥ4�ֽ�CRC
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
        EthHandle->DMASR = ETH_DMASR_RBUS; //������ջ����������ñ�־ Clear RBUS ETHERNET DMA flag
        EthHandle->DMARPDR = 0;            //�ָ�DMA���� Resume DMA reception
        pDrive->debuginfo.rxoverInts++;
    }
    return pkg;
}

// =============================================================================
// ���ܣ������ײ㷢������֡fram
// ������
// ����ֵ��
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
// ���ܣ���������������Э���
// ˵����
// ������
// ����ֵ��
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
            ETH_TxPkt_ChainMode(len); //�����ײ㷢��������Э���
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

//u32 ETH_SendData(u8 *buf,u32 len)
//{
//    struct NetPkg      pkg;
//    tagMacDriver      *pDrive;
//
//    pDrive = &gMacDriver;
//
//    PkgInit(pkg,CN_PKLGLST_END,0,len,buf);  //ֻ��һ����
////  pkg.partnext = NULL;
////  pkg.pkgflag  = (1<<0);  //ֻ��һ����
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

    ETH_DMAClearITPendingBit(SGCC_GMAC0_P, ETH_DMA_IT_NIS); //��DMA�жϱ�־λ

    if (ETH_GetDMAFlagStatus(ETH, ETH_DMA_FLAG_R))
    {
    pDrive->debuginfo.rcvInts++;
        Lock_SempPost(pDrive->rcvsync); //�ͷ��ź���
        ETH_DMAClearITPendingBit(SGCC_GMAC0_P, ETH_DMA_IT_R); //������жϱ�־λ
    }
//    if (ETH_GetDMAFlagStatus(ETH, ETH_DMA_FLAG_T))
//    {
//        pDrive->debuginfo.sndInts++;
//        ETH_DMAClearITPendingBit(SGCC_GMAC0_P, ETH_DMA_IT_T); //�巢���жϱ�־λ
//    }

    else if(ETH_GetDMAFlagStatus(ETH, ETH_DMA_FLAG_AIS))
    {
        pDrive->debuginfo.dmaerr++;
        ETH_DMAClearITPendingBit(SGCC_GMAC0_P, ETH_DMA_IT_AIS);
    }

    return 0;
}

// =============================================================================
// ���ܣ��������ƺ���
// ˵����
// ������
// ����ֵ  ��
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
// ���ܣ���������
// ˵����
// ������
// ����ֵ  ��
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
                    Link_NetDevPush(handle,pkg);  //����Э��ջ
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
// ���ܣ������������ݰ������߳�
// ˵����
// ������
// ����ֵ  ��
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
// ���ܣ�shell������
// ˵����how the gmac status
// ������
// ����ֵ  ��
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
// ���ܣ�SCM610������DJYIP������ʼ������
// ������para
// ����ֵ  ��true�ɹ�  falseʧ�ܡ�
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

    //Ĭ��ʹ��GMAC0
    pDrive->EthHandle  = sgpt_Gmac[0]; //MAC�Ĵ�����ʼ��ַ

    ETH_CLK_ENABLE(); //ETH�������ʱ�ӳ�ʼ��

    ETH_HardDrvInit(pDrive);  //MAC������Ӳ����ʼ��

    pDrive->rcvsync = Lock_SempCreate(1,1,CN_BLOCK_FIFO,NULL); //�ź���

    if(NULL == pDrive->rcvsync)
    {
        goto RCVSYNC_FAILED;
    }

    pDrive->protect = Lock_MutexCreate(NULL); //������
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


