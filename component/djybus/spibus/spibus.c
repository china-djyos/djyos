// =============================================================================
// Copyright (C) 2012-2020 都江堰操作系统研发团队  All Rights Reserved
// 文件名     ：spibus.c
// 模块描述: djybus模块的一个子结点SPI总线，该文件主要包含SPI类型的增加，总线和器件
//           增加以及对SPI总线的操作，即相应的SPI的操作API函数。
// 模块版本: V1.00
// 创建人员: hm
// 创建时间: 02/07.2014
// =============================================================================

#include "stdint.h"
#include "stdio.h"
#include "object.h"
#include "lock.h"
#include "stdlib.h"
#include <device.h>
#include "string.h"
#include "djybus.h"
#include "spibus.h"
#include "systime.h"
#include "djyos.h"
#include "dbug.h"

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_SPIBus(void);
//    ModuleInstall_SPIBus();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"spi bus"//spibus
//parent:"djybus"    //填写该组件的父组件名字，none表示没有父组件
//attribute:system              //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"djybus"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_SPI_BUS == false )
//#warning  " spi_bus  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_SPI_BUS    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

static struct Object *s_ptSPIBusType;

#define CN_SPI_FLAG_POLL        (1<<0)          //轮询中断方式标记

//SPI总线器件结构体
struct SPI_Device
{
    struct Object *HostObj;
    u8 Cs;                                  //片选信号
    bool_t AutoCs;                          //自动片选
    u8 CharLen;                             //数据长度
    u8 Mode;                                //模式选择
    u8 ShiftDir;                            //MSB or LSB
    u32 Freq;                               //速度,Hz
};


struct SPI_Buf
{
    u32    Offset;         //缓冲区指针,指向下一次读的位置
    u32    MaxLen;        //缓冲区最大长度,元素个数.
    u8     *pBuf;             //缓冲区指针,用户自己保证所开辟的缓冲区是否与设定
};
//SPI总线控制块结构体,本模块可见
struct SPI_CB
{
    struct Object           *HostObj;              //宿主对象
    struct SPI_Buf          SPI_Buf;               //缓冲区,用于异步发送
    struct SemaphoreLCB     *SPI_BusSemp;           //SPI总线保护信号量
    struct SemaphoreLCB     *SPI_BlockSemp;         //简易缓冲区保护信号量
    struct SPI_DataFrame    Frame;
    struct SPI_Device       *CurrentDev;           //占有当前总线的设备
    u16                     ErrorPopEvtt;       //出错处理事件的类型
    bool_t                  MultiCsReg;         //是否具有多套CS寄存器
    u8                      BlockOption;        //当前总线操作是否阻塞
    u8                      Flag;               //轮询中断标记
    ptu32_t                 SpecificFlag;       //个性标记
    TransferFunc            pTransferTxRx;
    TransferPoll            pTransferPoll;
    CsActiveFunc            pCsActive;
    CsInActiveFunc          pCsInActive;
    SPIBusCtrlFunc          pBusCtrl;
};

// =============================================================================
// 功能：将SPI总线类型结点挂接到DjyBus根结点
// 参数：无
// 返回：true = 成功，false = 失败
// =============================================================================
bool_t ModuleInstall_SPIBus(void)
{
    s_ptSPIBusType = DjyBus_BusTypeAdd("SPIBus");
    if(NULL != s_ptSPIBusType)
    {
        info_printf("bus","spi bus install successful.");
        return true;
    }
    else
    {
        error_printf("bus","spi bus install failed.");
        return false;
    }
}

// =============================================================================
// 功能：新增SPI总线结点到SPI总线类型结点,并初始化tagSPI_CB控制块结构体
// 参数：NewSPIParam,新增总线参数，参数说明详细请参照tagSPI_Param结构体
// 返回：返回建立的资源结点指针，失败时返回NULL
// =============================================================================
struct SPI_CB *SPI_BusAdd(struct SPI_Param *NewSPIParam)
{
    struct Object *SpiDev;
    struct SPI_CB *NewSPI;
    if(NULL == NewSPIParam)
        goto exit_from_param;

    //避免重复建立同名的SPI总线
    if(NULL != obj_search_child(s_ptSPIBusType,(const char*)(NewSPIParam->BusName)))
        goto exit_from_readd;

    NewSPI = (struct SPI_CB *)M_Malloc(sizeof(struct SPI_CB),0);
    if(NewSPI == NULL)
        goto exit_from_malloc;

    //将总线结点挂接到总线类型结点的子结点
    SpiDev = obj_newchild(s_ptSPIBusType, (fnObjOps)-1,(ptu32_t)NewSPI,(const char*)(NewSPIParam->BusName));
    if(SpiDev == NULL)
        goto exit_from_add_node;

    //创建总线信号量和阻塞信号量
    NewSPI->SPI_BusSemp= Lock_SempCreate(1,1,CN_BLOCK_FIFO,"spi bus semp");
    if(NewSPI->SPI_BusSemp== NULL)
        goto exit_from_spi_bus_semp;
    NewSPI->SPI_BlockSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"spi block semp");
    if(NewSPI->SPI_BlockSemp== NULL)
        goto exit_from_spi_buf_semp;

    //tagSPI_CB控制块初始化
    NewSPI->HostObj           = SpiDev;
    NewSPI->ErrorPopEvtt      = CN_EVTT_ID_INVALID;
    NewSPI->SpecificFlag      = NewSPIParam->SpecificFlag;
    NewSPI->MultiCsReg        = NewSPIParam->MultiCSRegFlag;
    NewSPI->pTransferTxRx     = NewSPIParam->pTransferTxRx;
    NewSPI->pCsActive         = NewSPIParam->pCsActive;
    NewSPI->pCsInActive       = NewSPIParam->pCsInActive;
    NewSPI->pBusCtrl          = NewSPIParam->pBusCtrl;
    NewSPI->pTransferPoll     = NewSPIParam->pTransferPoll;

    //缓冲区初始化
    NewSPI->SPI_Buf.pBuf     = NewSPIParam->SPIBuf;
    NewSPI->SPI_Buf.MaxLen   = NewSPIParam->SPIBufLen;
    NewSPI->SPI_Buf.Offset   = 0;

    info_printf("spibus","%s added.", NewSPIParam->BusName);
    return NewSPI;

exit_from_spi_buf_semp:
    Lock_SempDelete(NewSPI->SPI_BusSemp);
exit_from_spi_bus_semp:
    obj_Delete(SpiDev);
exit_from_add_node:
    free(NewSPI);
exit_from_malloc:
exit_from_readd:
exit_from_param:
    error_printf("spibus","%s add failed.", NewSPIParam->BusName);
    return NULL;
}

// =============================================================================
// 功能：删除在SPI总线类型上的SPI结点，若被删除的总线上有器件结点，则删除会不成功,需
//       释放新增结点时malloc的控制块内存
// 参数：DelSPI,删除SPI控制块指针
// 返回：true,删除成功;false,删除失败
// =============================================================================
bool_t SPI_BusDelete(struct SPI_CB *DelSPI)
{
    bool_t result;
    if(NULL == DelSPI)
        return false;
    if(obj_Delete(DelSPI->HostObj))
    {
        result = false;
    }
    else
    {
        result = true;
        free(DelSPI);
    }
    return result;
}


// =============================================================================
// 功能：查找SPI总线类型结点的子结点中是否存被查找名称的总线
// 参数：BusName,查找的总线名称
// 返回：查找的控制块结点指针，未找到时返回NULL
// =============================================================================
struct SPI_CB *SPI_BusFind(const char *BusName)
{
    struct Object *SPI_BusObj;
    SPI_BusObj = obj_search_child(s_ptSPIBusType,BusName);
    if(SPI_BusObj)
    {
        return (struct SPI_CB *)obj_GetPrivate(SPI_BusObj);
    }
    else
        return NULL;
}

// =============================================================================
// 功能：在SPI总线结点上增加器件，即总线上挂接的器件，并初始化器件所对应的参数配置寄
//       存器,该器件属于总线结点的子结点
// 参数：BusName,新增子器件挂接的总线名称
//       DevName,器件名称
//       cs,该器件所属的片选序号
//       charlen,器件的字符长度，比特位数
//       mode,配置从器件模式,CHPA和CPOL组合成成的模式
//       freq,SPI总线的时钟频率，单位Hz
//       autocs,新增加设备是否自动片选
// 返回：控制块结点指针，添加失败时返回NULL
// =============================================================================
struct SPI_Device *SPI_DevAdd(const char *BusName ,const char *DevName,u8 cs,u8 charlen,
                                u8 mode,u8 shiftdir,u32 freq,u8 autocs)
{
    struct SPI_CB     *SPI;
    struct SPI_Device *NewDev;
    struct Object *NewIicObj;
    tagSpiConfig spicfg;

    //查询是否该总线存在
    SPI = SPI_BusFind(BusName);
    if(NULL == SPI)
        return NULL;

    //避免建立同名的SPI器件
    if(NULL != obj_search_child(s_ptSPIBusType, DevName))
        return NULL;

    //为新的器件结点动态分配内存
    NewDev = (struct SPI_Device *)M_Malloc(sizeof(struct SPI_Device),0);
    if(NULL == NewDev)
        return NULL;

    //为新结点初始化
    NewDev->Cs  = cs;
    NewDev->HostObj = obj_newchild(SPI->HostObj, (fnObjOps)-1, (ptu32_t)NewDev,  DevName);
    if(!NewDev->HostObj)
    {
        free(NewDev);
        return NULL;
    }

    NewDev->CharLen    = charlen;
    NewDev->Cs         = cs;
    NewDev->AutoCs     = autocs;
    NewDev->Mode       = mode;
    NewDev->Freq       = freq;
    NewDev->ShiftDir   = shiftdir;

    spicfg.CharLen = charlen;
    spicfg.Mode    = mode;
    spicfg.Freq    = freq;
    spicfg.ShiftDir= shiftdir;

    //如果SPI控制器有多套CS寄存器，则配置它，否则不作配置
    if(true == SPI->MultiCsReg)
    {
        SPI_BusCtrl(NewDev,CN_SPI_CS_CONFIG,(ptu32_t)&spicfg,(ptu32_t)cs);
    }

    info_printf("spibus","device \"%s\" added to \"%s\".", DevName, BusName);
    return NewDev;
}

// =============================================================================
// 功能：删除SPI总线上的器件，从总线子结点中删除
// 参数：DelDev,删除的器件指针
// 返回：true,删除成功;false,删除失败
// =============================================================================
bool_t SPI_DevDelete(struct SPI_Device *DelDev)
{
    bool_t result;
    if(NULL == DelDev)
        return false;

    if(obj_Delete(DelDev->HostObj))
    {
        result = false;
    }
    else
    {
        free(DelDev);
        result = true;
    }
    return result;
}

// =============================================================================
// 功能：删除SPI总线上的器件，从总线子结点中删除
// 参数：DelDev,删除的器件指针
// 返回：true,删除成功;false,删除失败
// =============================================================================
bool_t SPI_DevDelete_s(struct SPI_Device *DelDev)
{
    bool_t result;
    if(NULL == DelDev)
        return false;

    if(obj_Delete(DelDev->HostObj))
    {
        result = false;
    }
    else
    {
        result = true;
    }
    return result;
}

// =============================================================================
// 功能：查找器件结点，该结点必然是挂接在相应的SPI总线结点上
// 参数：BusName,被搜索的SPI总线名称
//       DevName,器件名称
// 返回：结点附属结构体指针
// =============================================================================
struct SPI_Device *SPI_DevFind(const char *BusName ,const char *DevName)
{
    struct Object *SPI_DevObj;
    struct SPI_CB *SPI_Bus;

    SPI_Bus = SPI_BusFind(BusName);
    if(NULL == SPI_Bus)
        return NULL;

    //通过SPI类型结点，向下搜索后代结点
    SPI_DevObj = obj_search_child(SPI_Bus->HostObj, DevName);
    if(SPI_DevObj)
        return ((struct SPI_Device*)obj_GetPrivate(SPI_DevObj));
    else
        return NULL;
}

// =============================================================================
// 功能：片选拉低，该函数运行必须获得总线信号量，即SPI_BusSemp，然后根据CS是否具有
//       自己的配置寄存器标志，判断是否需要配置寄存器。若csctrl标志为false,则每次CS
//       使能时，都需要配置寄存器
// 参数：Dev,器件指针
//       timeout,请求总线信号量超时时间，us
// 返回：true,成功;false,失败
// =============================================================================
bool_t SPI_CsActive(struct SPI_Device *Dev,u32 timeout)
{
    struct SPI_CB *SPI;
    tagSpiConfig spicfg;
    if(NULL == Dev)
        return false;

    SPI = (struct SPI_CB *)obj_GetPrivate(obj_parent(Dev->HostObj));
    if(NULL == SPI)
        return false;

    if(Dev->AutoCs == false)                                //手动调用才有效
    {
        if(false == Lock_SempPend(SPI->SPI_BusSemp,timeout))    //需要等待总线空闲
            return false;

        //如果没有独立的CS参数配置寄存器，则每次操作都需要配置SPI参数配置寄存器，
        //因为多个CS共用同一套参数配置寄存器
        if((SPI->MultiCsReg == false) && (SPI->CurrentDev != Dev))
        {
            spicfg.CharLen = Dev->CharLen;
            spicfg.Mode    = Dev->Mode;
            spicfg.Freq    = Dev->Freq;
            spicfg.ShiftDir= Dev->ShiftDir;
            SPI->pBusCtrl(SPI->SpecificFlag,CN_SPI_CS_CONFIG,
                            (ptu32_t)&spicfg,(ptu32_t)Dev->Cs);
        }
        //如果每个CS都有独立的配置寄存器，则无需每次调用都配置，用户可通过调用pBusCtrl
        //控制CS相关的寄存器配置
        SPI->CurrentDev = Dev;
        SPI->pCsActive(SPI->SpecificFlag,Dev->Cs);
    }
    return true;
}

bool_t __SPI_AutoCsActive(struct SPI_CB *SPI,struct SPI_Device *Dev)
{
    tagSpiConfig spicfg;

    //如果没有独立的CS参数配置寄存器，则每次操作都需要配置SPI参数配置寄存器，
    //因为多个CS共用同一套参数配置寄存器
    if((SPI->MultiCsReg == false) && (SPI->CurrentDev != Dev))
    {
        spicfg.CharLen = Dev->CharLen;
        spicfg.Mode    = Dev->Mode;
        spicfg.Freq    = Dev->Freq;
        spicfg.ShiftDir= Dev->ShiftDir;
        SPI->pBusCtrl(SPI->SpecificFlag,CN_SPI_CS_CONFIG,
                        (ptu32_t)&spicfg,(ptu32_t)Dev->Cs);
    }
    //如果每个CS都有独立的配置寄存器，则无需每次调用都配置，用户可通过调用pBusCtrl
    //控制CS相关的寄存器配置
    SPI->CurrentDev = Dev;
    SPI->pCsActive(SPI->SpecificFlag,Dev->Cs);
    return true;
}

// =============================================================================
// 功能：片选拉高，若传输为阻塞方式，则由该函数释放总线信号量和拉高片选，否则，拉高总
//       线和释放信号量由底层驱动完成。因为非阻塞方式时，运行到该函数时，传输未必完成
// 参数：Dev,器件指针
//       block_option,阻塞选项，为true时，表明最后一次传输为阻塞方式，否则为非阻塞
// 返回：true,成功;false,失败
// =============================================================================
bool_t SPI_CsInactive(struct SPI_Device *Dev)
{
    struct SPI_CB *SPI;
    if(NULL == Dev)
        return false;

    SPI = (struct SPI_CB *)obj_GetPrivate(obj_parent(Dev->HostObj));
    if(NULL == SPI)
        return false;

    if(Dev->AutoCs == false)                        //自动片选时，手动调用无效
    {
        SPI->pCsInActive(SPI->SpecificFlag,Dev->Cs);
        Lock_SempPost(SPI->SPI_BusSemp);
    }
    else
    {
        ;
    }

    return true;
}

bool_t __SPI_AutoCsInactive(struct SPI_CB *SPI,u8 CS)
{
    SPI->pCsInActive(SPI->SpecificFlag,CS);
    Lock_SempPost(SPI->SPI_BusSemp);
    return true;
}

// =============================================================================
// 功能：数据传送函数，完成数据的发送和接收。该函数完成的功能如下：
//       1.若器件驱动为了避免组包的麻烦，可先发命令再发送数据，分多次调用，多次调用前
//         后被CSActive和CsInactive函数包裹；
//       2.根据Dev查找所属SPI总线；
//       3.若缓冲区大于发送字节数，则直接将数据填入缓冲区；
//       4.若为阻塞发送，则等待总线信号量，若为非阻塞，则等待buf信号量；
//       5.发生超时或错误时，拉高CS并释放信号量
// 参数：Dev,器件指针
//       spidata,SPI数据结构体
//       block_option,阻塞选项，为true时，表明最后一次传输为阻塞方式，否则为非阻塞
//       timeout,超时参数，us
// 返回：返回发送状态，超时或错误或无错误
// =============================================================================
s32 SPI_Transfer(struct SPI_Device *Dev,struct SPI_DataFrame *spidata,
                u8 block_option,u32 timeout)
{
    struct SPI_CB *SPI;
//  struct semaphore_LCB *spi_semp;
    s32 result ;
    u32 written=0;
    u32 base_time = 0,rel_timeout = timeout;

    if(NULL == Dev)
        return CN_SPI_EXIT_PARAM_ERR;

    SPI = (struct SPI_CB *)obj_GetPrivate(obj_parent(Dev->HostObj));//查找该器件属于哪条总线
    if(NULL == SPI)
        return CN_SPI_EXIT_PARAM_ERR;

    base_time = (u32)DjyGetSysTime();
    //若配置需自动片选，则本函数内部需拉低片选
    if(Dev->AutoCs == true)
    {
        if(false == Lock_SempPend(SPI->SPI_BusSemp,timeout))    //需要等待总线空闲
        {
            result = CN_SPI_EXIT_TIMEOUT;
            goto exit_from_bus_timeout;
        }
        __SPI_AutoCsActive(SPI,Dev);
    }

    Lock_SempPend(SPI->SPI_BlockSemp,0);                        //相当于重置信号量

    //禁止调试或未登记pTransferTxRx，使用轮询方式通信
    if((Djy_QuerySch() == false) || (SPI->pTransferTxRx == NULL)
            || (SPI->Flag & CN_SPI_FLAG_POLL))
    {
        if(SPI->pTransferPoll != NULL)
        {
            SPI->pTransferPoll(SPI->SpecificFlag,spidata->SendBuf,spidata->SendLen,
                spidata->RecvBuf,spidata->RecvLen,spidata->RecvOff,Dev->Cs);

            if(Dev->AutoCs == true)
                __SPI_AutoCsInactive(SPI,Dev->Cs);
            result = CN_SPI_EXIT_NOERR;
        }
        goto exit_from_no_err;
    }

    //以下是中断方式收发
    if(spidata->RecvLen)
        block_option = true;                            //接收数据自动转为阻塞
    SPI->SPI_Buf.Offset = 0;                            //发送前先清空缓冲区
    SPI->Frame = *spidata;
    SPI->BlockOption = block_option;

    //如果非阻塞方式，且缓冲区够大，则直接写入缓冲区
    //若不是，则先发送调用者提供的缓冲区，直到剩余字节数能够填充到缓冲区
    if((!block_option) && (spidata->SendLen <= SPI->SPI_Buf.MaxLen))
    {
        memcpy(SPI->SPI_Buf.pBuf,spidata->SendBuf,spidata->SendLen);
        written = spidata->SendLen;
    }

    //调用启动时序的回调函数
    if(true == SPI->pTransferTxRx(SPI->SpecificFlag,
                                spidata->SendLen,
                                spidata->RecvLen,
                                spidata->RecvOff
                                ))
    {
        rel_timeout = (u32)DjyGetSysTime();
        if(rel_timeout - base_time < timeout)
            rel_timeout = timeout - (rel_timeout - base_time);
        else
        {
            result = CN_SPI_EXIT_TIMEOUT;
            goto exit_from_timeout;
        }

        //需要等待的情况：1.阻塞发送；2.数据未全部填到缓冲区
        if((true == block_option) || (written < spidata->SendLen))
        {
            //等待中断函数释放信号量
            if(!Lock_SempPend(SPI->SPI_BlockSemp,rel_timeout))
            {
                result = CN_SPI_EXIT_TIMEOUT;
                goto exit_from_timeout;
            }
        }

        result = CN_SPI_EXIT_NOERR;
        goto exit_from_no_err;
    }else
    {
        result = CN_SPI_EXIT_UNKNOW_ERR;
        goto exit_from_timeout;
    }

exit_from_timeout:
    if(Dev->AutoCs == true)                     //自动片选时，返回前需拉低片选
    {
        __SPI_AutoCsInactive(SPI,Dev->Cs);
    }
exit_from_bus_timeout:
exit_from_no_err:
    return result;
}

// =============================================================================
// 功能：读发送缓冲区数据，由总线驱动调用，总线驱动读取到数据后将数据写入寄存器发送，
//       若阻塞发送，则直接读发送缓冲区指向的数据，若非阻塞发送，则需判断是否剩余数据
//       已达到缓冲区边界，若已到达缓冲区边界，则填写缓冲区，并释放阻塞信号量
// 参数：SPI,SPI控制块指针
//       buf,读数据缓冲区指针
//       len,读数据长度，字节单位
// 返回：字节数
// =============================================================================
s32 SPI_PortRead( struct SPI_CB *SPI,u8 *buf,u32 len)
{
    u32 Result = 0,RingLen,CpyLen = 0;
    u8 *pbuf;

    if((len > 0) && (SPI->Frame.SendLen > 0))
    {
        CpyLen = SPI->Frame.SendLen >= len ?len:SPI->Frame.SendLen;
        RingLen = SPI->SPI_Buf.MaxLen;

        if(SPI->BlockOption == true)                //阻塞发送直接读缓冲区
        {
            memcpy(buf,SPI->Frame.SendBuf,CpyLen);
            SPI->Frame.SendBuf += CpyLen;
        }
        else                                        //非阻塞发送，则需区别读取
        {
            //判断是否从缓冲区中读数据
            if(SPI->Frame.SendLen <= RingLen)       //从缓冲区中读数据
            {
                pbuf = &SPI->SPI_Buf.pBuf[SPI->SPI_Buf.Offset];
                SPI->SPI_Buf.Offset += CpyLen;
                memcpy(buf,pbuf,CpyLen);
            }
            else                                    //从pbuf中读数据
            {
                memcpy(buf,SPI->Frame.SendBuf,CpyLen);
                SPI->Frame.SendBuf += CpyLen;

                //将pbuf剩余数据写入缓冲,并释放阻塞信号量
                if(SPI->Frame.SendLen - CpyLen <= RingLen)
                {
                    memcpy(SPI->SPI_Buf.pBuf,SPI->Frame.SendBuf,
                            SPI->Frame.SendLen-CpyLen);
                    Lock_SempPost(SPI->SPI_BlockSemp);
                }
            }
        }
    }

    //只有发送接收都完成的时候才能拉高片选
    if(SPI->Frame.SendLen + SPI->Frame.RecvLen == 0)
    {
        if(SPI->BlockOption == true)
        {
            Lock_SempPost(SPI->SPI_BlockSemp);      //阻塞发送时，释放阻塞信号量
        }

        if(SPI->CurrentDev->AutoCs == true)
        {
            __SPI_AutoCsInactive(SPI,SPI->CurrentDev->Cs);
        }
    }

    SPI->Frame.SendLen -= CpyLen;
    Result = CpyLen;

    return Result;
}

// =============================================================================
// 功能：将接收到的数据写入用户提供的缓冲区中，接收是阻塞方式，因此使用用户的缓冲区
// 参数：SPI,SPI控制块指针
//       buf,数据指针
//       len,数据长度，字节单位
// 返回：字节数
// =============================================================================
s32 SPI_PortWrite(struct SPI_CB *SPI,u8 *buf,u32 len)
{
    u32 cpylen;
    if((len > 0) && (SPI->Frame.RecvLen > 0))
    {
        cpylen = SPI->Frame.RecvLen >= len ?len:SPI->Frame.RecvLen;
        memcpy(SPI->Frame.RecvBuf,buf,cpylen);
        SPI->Frame.RecvBuf += cpylen;
        SPI->Frame.RecvLen -= cpylen;
    }

    //只有发送接收都完成的时候才能拉高片选
    if(SPI->Frame.SendLen + SPI->Frame.RecvLen == 0)
    {
        if(SPI->BlockOption == true)
        {
            Lock_SempPost(SPI->SPI_BlockSemp);      //阻塞发送时，释放阻塞信号量
        }
        if(SPI->CurrentDev->AutoCs == true)
        {
            __SPI_AutoCsInactive(SPI,SPI->CurrentDev->Cs);
        }
    }

    return cpylen;
}

// =============================================================================
// 功能：根据cmd命令，解析数据data1和Data2，控制SPI，硬件相关的命令需调用回调函数
//       由底层驱动完成
// 参数：Dev,器件结构体指针
//       cmd,命令
//       data1,data2,参数数据，根据具体命令定义不同
// 返回：-1=参数检查出错,否则由cmd决定,若需要调用用户的pBusCtrl,则是该函数返回值
// =============================================================================
s32 SPI_BusCtrl(struct SPI_Device *Dev,u32 cmd,ptu32_t data1,ptu32_t data2)
{
    struct SPI_CB *SPI;
    tagSpiConfig *spicfg;
    s32 result = -1;

    if(NULL == Dev)
        return -1;

    SPI = (struct SPI_CB *)obj_GetPrivate(obj_parent(Dev->HostObj));
    if(NULL == SPI)
        return -1;

    switch(cmd)
    {
    case CN_SPI_SET_CLK:
        result = SPI->pBusCtrl(SPI->SpecificFlag,cmd,data1,Dev->Cs);
        break;
    case CN_SPI_CS_CONFIG:
        spicfg = (tagSpiConfig *)data1;
        Dev->CharLen = spicfg->CharLen;
        Dev->Freq    = spicfg->Freq;
        Dev->Mode    = spicfg->Mode;
        Dev->ShiftDir= spicfg->ShiftDir;
        result = SPI->pBusCtrl(SPI->SpecificFlag,cmd,data1,Dev->Cs);
        break;
    case CN_SPI_SET_AUTO_CS_EN:
        SPI->CurrentDev->AutoCs = true;
        result = SPI->pBusCtrl(SPI->SpecificFlag,cmd,data1,Dev->Cs);
        break;
    case CN_SPI_SET_AUTO_CS_DIS:
        SPI->CurrentDev->AutoCs = false;
        result = SPI->pBusCtrl(SPI->SpecificFlag,cmd,data1,Dev->Cs);
        break;
    case CN_SPI_SET_ERROR_POP_EVENT:
        SPI->ErrorPopEvtt = data1;
        break;
    case CN_SPI_SET_POLL:
        result = SPI->pBusCtrl(SPI->SpecificFlag,cmd,data1,data2);
        SPI->Flag |=  CN_SPI_FLAG_POLL;
        break;
    case CN_SPI_SET_INT:
        SPI->Flag &= ~CN_SPI_FLAG_POLL;
        break;
    default:
        break;
    }
    return result;
}

// =============================================================================
// 功能：SPI错误弹出API函数，当发生错误时，调用该函数，弹出错误处理事件，并将错误号
//       以参数的形式传递给弹出的事件
// 参数：SPI,SPI控制块指针
//       ErrNo,发生错误序号，如未收到ACK信号CN_SPI_NO_ACK_ERR
// 返回：执行结果，-1参数错误，0无错误
// =============================================================================
s32 SPI_ErrPop(struct SPI_CB *SPI, u32 ErrNo)
{
    if(NULL == SPI)
        return -1;

    //弹出错误处理事件，并将错误号以参数形式传递
    Djy_EventPop(SPI->ErrorPopEvtt,NULL,0,ErrNo,0,0);

    return CN_SPI_EXIT_NOERR;
}
