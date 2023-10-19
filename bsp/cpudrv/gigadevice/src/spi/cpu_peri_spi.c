#include <string.h>

#include <djyos.h>
#include <cpu_peri.h>
#include <spibus.h>
#include <int.h>

#include <cpu_peri_int_line.h>
#include <at32f435_437_spi.h>

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_SPI(u8 port);
//    #if CFG_SPI1_ENABLE==1
//    ModuleInstall_SPI(CN_SPI1);
//    #endif
//
//    #if CFG_SPI2_ENABLE==1
//    ModuleInstall_SPI(CN_SPI2);
//    #endif
//
//    #if CFG_SPI3_ENABLE==1
//    ModuleInstall_SPI(CN_SPI3);
//    #endif
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip spi"//SPI总线驱动
//parent:"spi bus"   //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"spi bus","int","lock"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_SPI == false )
//#warning  " cpu_onchip_spi  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_SPI    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,32,512,
#define CFG_SPI_BUF_LEN             128                //"SPI缓冲区大小",
#define CFG_SPI_DMA_BUF_LEN         128                //"SPI DMA缓冲区大小",
//%$#@enum,true,false,
#define CFG_SPI1_ENABLE             false              //"是否配置使用SPI1",
#define CFG_SPI2_ENABLE             false              //"是否配置使用SPI2",
#define CFG_SPI3_ENABLE             false              //"是否配置使用SPI3",
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure
// =============================================================================

#define CN_TIMEOUT  (20*1000)



struct art43x_spi {
	spi_type *spi_x;
	struct SPI_CB spi_cb;
	union {
		ptu32_t IntLine;
		int irqno;
	};
	int port;
};

struct art43x_spi art43x_spi[3] = {
	.[CN_SPI1] = { .spi_x = SPI1,
		       .irqno = CN_INT_LINE_SPI1,
		       .port = CN_SPI1 },
	.[CN_SPI2] = { .spi_x = SPI2,
		       .irqno = CN_INT_LINE_SPI2_I2S2EXT,
		       .port = CN_SPI2 },
	.[CN_SPI3] = { .spi_x = SPI3,
		       .irqno = CN_INT_LINE_SPI3_I2S3EXT,
		       .port = CN_SPI3 }
};





typedef void *tagSpiReg;

struct SPI_IntParamSet
{
    u32 SendDataLen;
    u32 RecvDataLen;
    u32 RecvOffset;
};
// =============================================================================
//#define CFG_SPI_BUF_LEN              128
//#define CFG_SPI_DMA_BUF_LEN          128
static struct SPI_CB *s_ptSpi1_CB;
static u8 s_Spi1_Buf[CFG_SPI_BUF_LEN];
struct SPI_IntParamSet IntParamset1;

static struct SPI_CB *s_ptSpi2_CB;
static u8 s_Spi2_Buf[CFG_SPI_BUF_LEN];
struct SPI_IntParamSet IntParamset2;

static struct SPI_CB *s_ptSpi3_CB;
static u8 s_Spi3_Buf[CFG_SPI_BUF_LEN];
struct SPI_IntParamSet IntParamset3;

static void __SPI_IntEnable(volatile tagSpiReg *Reg,u32 IntSrc)
{
//    Reg->CR2 |= IntSrc;
}
static void __SPI_IntDisable(volatile tagSpiReg *Reg,u32 IntSrc)
{
//    Reg->CR2 &= ~IntSrc;
}

static u32 __SPI_BrCal(u32 Fre)
{
    u32 temp;
    temp = CN_CFG_PCLK2/Fre;
    if(temp >= 256)         temp = 7;
    else if(temp >= 128)    temp = 6;
    else if(temp >= 64)     temp = 5;
    else if(temp >= 32)     temp = 4;
    else if(temp >= 16)     temp = 3;
    else if(temp >= 8)      temp = 2;
    else if(temp >= 4)      temp = 1;
    else                    temp = 0;

    return temp;
}

static void __SPI_SetClk(volatile tagSpiReg *Reg,u32 Fre)
{
}

static void __SPI_Config(volatile tagSpiReg *Reg,tagSpiConfig *ptr)
{}

static void __SPI_HardConfig(u32 BaseAddr)
{
}

extern bool_t Board_SpiCsCtrl(u8 SPIPort,u8 cs,u8 level);
static bool_t __SPI_BusCsActive(tagSpiReg *Reg, u8 cs)
{
}

static bool_t __SPI_BusCsInActive(tagSpiReg *Reg, u8 cs)
{
}

static s32 __SPI_BusCtrl(tagSpiReg *Reg,u32 cmd,ptu32_t data1,ptu32_t data2)
{
#if 0

    s32 result = 1;
    if( (Reg != tg_SpiReg[CN_SPI1]) && (Reg != tg_SpiReg[CN_SPI2])
            && (Reg != tg_SpiReg[CN_SPI3]) )
        return 0;

    switch(cmd)
    {
    case CN_SPI_SET_CLK:
        __SPI_SetClk(Reg,data1);
        break;
    case CN_SPI_CS_CONFIG:
        __SPI_Config(Reg,(tagSpiConfig *)data1);
        break;
    case CN_SPI_SET_AUTO_CS_EN:
        // 硬件上如果有自动产生CS的配置
        break;
    case CN_SPI_SET_AUTO_CS_DIS:
        break;
    default:
        break;
    }
    return result;
#endif
}

static s32 __SPI_Read( tagSpiReg * Reg )
{
#if 0
        u32 time = CN_TIMEOUT;
    while ( ( (Reg->SR & SPI_SR_RXNE) == 0 ) && (time > 0))
    {
        time --;
        DJY_DelayUs(1);
    }

    if(time == 0)
        return -1;
    return Reg->DR & 0xFFFF ;
#endif
}

static s32 __SPI_Write( tagSpiReg * Reg,uint16_t wData )
{
	uint16_t tx_data;
	spi_type *spi_x;
	flag_status status;
	int retry = CN_TIMEOUT;

        spi_x = SPI1;
	tx_data = wData;

        do {
		if (spi_i2s_flag_get(spi_x, SPI_I2S_TDBE_FLAG)) {
			spi_i2s_data_transmit(spi_x, tx_data);
			return 0;
		}
	} while (retry--);

        return -1;       
}

static bool_t __SPI_TxRxPoll(tagSpiReg *Reg,u8 *srcAddr,u32 wrSize, u8 *destAddr, u32 rdSize,u32 recvoff,u8 cs)
{
#if 0
    u32 i,len_limit;

    if( (!srcAddr) || ((rdSize != 0) && (!destAddr)))
        return false;

    Reg->CR1 |= SPI_CR1_SPE;
    len_limit = MAX(wrSize, rdSize + recvoff);
    for (i=0;i<len_limit;i++)
    {
        if (i<wrSize)
        {
            __SPI_Write(Reg,srcAddr[i]);
        }
        else if((i>=wrSize)&&(i<len_limit))
        {
          __SPI_Write(Reg,0);
        }
        if((destAddr) && (i>=recvoff) )
        {
            destAddr[i-recvoff] = __SPI_Read(Reg);
        }
        else
        {
            __SPI_Read(Reg);
        }
    }

    return true;
#endif
}

static bool_t __SPI_TransferTxRx(tagSpiReg *Reg,u32 sendlen,u32 recvlen,
                                u32 recvoff)
{
#if 0
    struct SPI_IntParamSet *Param=NULL;

    if(Reg == tg_SpiReg[CN_SPI1])
        Param = &IntParamset1;
    else if(Reg == tg_SpiReg[CN_SPI2])
        Param = &IntParamset2;
    else
        Param = &IntParamset3;

    Param->SendDataLen = sendlen;
    Param->RecvDataLen = recvlen;
    Param->RecvOffset  = recvoff;

    __SPI_IntEnable(Reg,SPI_CR2_TXEIE);
    Reg->CR1 |= SPI_CR1_SPE;

    return true;
#endif
}

u32 SPI_ISR(ptu32_t IntLine)
{
#if 0
    struct SPI_CB *pSCB = NULL;
    struct SPI_IntParamSet *param;
    tagSpiReg *Reg;
    u32 spi_sr,spi_cr2;
    u8 ch,temp;
    static u8 firstrcv = 0;

    if(IntLine == CN_INT_LINE_SPI1)
    {
        pSCB = s_ptSpi1_CB;
        Reg = (tagSpiReg *)tg_SpiReg[CN_SPI1];
        param = &IntParamset1;
    }
    else if(IntLine == CN_INT_LINE_SPI2)
    {
        pSCB = s_ptSpi2_CB;
        Reg = (tagSpiReg *)tg_SpiReg[CN_SPI2];
        param = &IntParamset2;
    }
    else if(IntLine == CN_INT_LINE_SPI3)
    {
        pSCB = s_ptSpi3_CB;
        Reg = (tagSpiReg *)tg_SpiReg[CN_SPI3];
        param = &IntParamset3;
    }
    else
        return 0;

    spi_sr = Reg->SR;
    spi_cr2 = Reg->CR2;
    //�����ж�
    if( (spi_cr2 & SPI_CR2_TXEIE) && (spi_sr & SPI_SR_TXE)) //ʹ���˷����жϣ���TXRIS
    {
        temp = SPI_PortRead(pSCB,&ch,1);//�Ƿ���������Ҫ����

        if(temp >0)
        {
            param->SendDataLen --;
            param->RecvOffset  --;
            Reg->DR = ch;
        }
        else                                            //�������ͽ���
        {
            //�ط����ж�
            __SPI_IntDisable(Reg,SPI_CR2_TXEIE);
            param->RecvOffset -= param->SendDataLen;
            param->SendDataLen = 0;

            ch = Reg->DR;

            //��������ˣ��ж��Ƿ���Ҫ����
            if(param->RecvDataLen > 0)
            {
                //MASTERģʽ�£����ͺͽ���ͬʱ����
                Reg->DR = 0xAA;
                __SPI_IntEnable(Reg,SPI_CR2_RXNEIE);    //ʹ�ܽ����ж�
                firstrcv = 1;
            }
        }
    }
    else if((spi_cr2 & SPI_CR2_RXNEIE) && (spi_sr & SPI_SR_RXNE))    //�����ж�
    {
        ch = Reg->DR;
        //��������˶�������
        if(firstrcv != 1)
        {
            SPI_PortWrite(pSCB,&ch,1);
            param->RecvDataLen -= 1;
        }
        else
            firstrcv = 0;

        //�ж��Ƿ����������
        if(param->RecvDataLen > 0)
        {
            Reg->DR = 0xAA;
        }
        else                    //���������Ѿ����
        {
            param->RecvDataLen = 0;
            __SPI_IntDisable(Reg,SPI_CR2_RXNEIE);
        }
    }

    if(param->RecvDataLen + param->SendDataLen == 0)
    {
        Reg->CR1 &= ~SPI_CR1_SPE;
    }
#endif
    return 0;
}

static void __SPI_IntConfig(u8 IntLine)
{
    //�ж��ߵĳ�ʼ��
    Int_Register(IntLine);
    Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(IntLine,SPI_ISR);
    Int_SettoAsynSignal(IntLine);
    Int_ClearLine(IntLine);
    Int_RestoreAsynLine(IntLine);
}

bool_t SPI_Initialize(u8 port)
{
    struct SPI_Param SPI_Config;
    struct SPI_CB **pSpiCB;
    u8 IntLine;
#if 0
    switch(port)
    {
    case CN_SPI1:
        SPI_Config.BusName          = "SPI1";
        SPI_Config.SPIBuf           = (u8*)&s_Spi1_Buf;
        IntLine = CN_INT_LINE_SPI1;
        pSpiCB = &s_ptSpi1_CB;
        break;
    case CN_SPI2:
        SPI_Config.BusName          = "SPI2";
        SPI_Config.SPIBuf           = (u8*)&s_Spi2_Buf;
        IntLine = CN_INT_LINE_SPI2;
        pSpiCB = &s_ptSpi2_CB;
        break;
    case CN_SPI3:
        SPI_Config.BusName          = "SPI3";
        SPI_Config.SPIBuf           = (u8*)&s_Spi3_Buf;
        IntLine = CN_INT_LINE_SPI3;
        pSpiCB = &s_ptSpi3_CB;
        break;
    default:
        return (0);
    }
    SPI_Config.SpecificFlag     = (ptu32_t)tg_SpiReg[port];
    SPI_Config.SPIBufLen        = CFG_SPI_BUF_LEN;
    SPI_Config.MultiCSRegFlag   = false;
    SPI_Config.pTransferPoll    = (TransferPoll)__SPI_TxRxPoll;
    SPI_Config.pTransferTxRx    = (TransferFunc)__SPI_TransferTxRx;
    SPI_Config.pCsActive        = (CsActiveFunc)__SPI_BusCsActive;
    SPI_Config.pCsInActive      = (CsInActiveFunc)__SPI_BusCsInActive;
    SPI_Config.pBusCtrl         = (SPIBusCtrlFunc)__SPI_BusCtrl;


    __SPI_HardConfig((u32)SPI_Config.SpecificFlag); // SPI�Ĵ�������

    __SPI_IntConfig(IntLine); // ע���ж�

    if(*pSpiCB = SPI_BusAdd(&SPI_Config)) // ��SPI�˿���ӵ�SPI����
        return (1);
    else
        return (0);
#endif
}

s32 ModuleInstall_SPI(u8 Port)
{
    if(CN_SPI1 != Port) // ���ֻ֧�ֵ�SPI1
        return (-1);
    if(0 == SPI_Initialize(Port))
        return (-1);
    return (0);
}
