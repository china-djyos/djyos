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
// 文件名     ：iicbus.c
// 模块描述: djybus模块的一个子结点IIC总线，该文件主要包含IIC类型的增加，总线和器件
//           增加以及对IIC总线的操作，即相应的IIC的操作API函数。
// 模块版本: V1.00
// 创建人员: hm
// 创建时间: 17/06.2014
// =============================================================================

#include "stdint.h"
#include "stdio.h"
#include "object.h"
#include "lock.h"
#include "stdlib.h"
#include <device.h>
#include "string.h"
#include "djybus.h"
#include "iicbus.h"
#include "djyos.h"
#include "dbug.h"

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_IICBus(void);
//    ModuleInstall_IICBus ( );
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"iicbus"//iicbus
//parent:"djybus"    //填写该组件的父组件名字，none表示没有父组件
//attribute:system              //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"djybus"           //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_IICBUS == false )
//#warning  " iicbus  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_IICBUS    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

//IIC总线器件结构体
struct IIC_Device
{
    struct Object         *HostObj;                 //宿主对象

    u8 DevAddr;                 //七位的器件地址,最低的0~2bit可能是器件内部地址。
    u8 BitOfMemAddrInDevAddr;   //器件地址中内部地址所占比特位数
    u8 BitOfMemAddr;            //器件内部地址寻址总bit数，包含了BitOfMemAddrInDevAddr
};

//IIC缓冲区结构体
struct IICBuf
{
    u32    Offset;              //缓冲区指针,指向下一次读的位置
    u32    MaxLen;              //缓冲区最大长度,元素个数.
    u8     *pBuf;               //缓冲区指针,用户自己保证所开辟的缓冲区是否与设定
};
//IIC总线控制块结构体,本模块可见
struct IIC_CB
{
    struct Object           *HostObj;               //宿主对象
    struct IICBuf           IIC_Buf;                //缓冲区,用于异步发送
    struct SemaphoreLCB     *IIC_BusSemp;           //IIC总线保护信号量
    struct SemaphoreLCB     *IIC_BufSemp;           //简易缓冲区保护信号量
    u32                     Counter;                //发送/接收计数器
    u16                     ErrorPopEvtt;           //出错处理事件的类型
    u8                      *pBuf;                  //发送/接收缓冲区指针
    u8                      Flag;                   //标记，按位，包括 读/方式和轮询/中断两种
    ptu32_t                 SpecificFlag;           //个性标记
    WriteReadPoll           pWriteReadPoll;
    WriteStartFunc          pGenerateWriteStart;
    ReadStartFunc           pGenerateReadStart;
    GenerateEndFunc         pGenerateEnd;
    IICBusCtrlFunc          pBusCtrl;
};

static struct Object *s_ptIICBusType;

//ICB的成员FLAG的位标记
#define CN_IIC_FLAG_R    (1<<0)         //读写标志位
#define CN_IIC_FLAG_POLL    (1<<1)      //轮询中断标志位

// =============================================================================
// 功能：将IIC总线类型结点挂接到DjyBus根结点
// 参数：无
// 返回：true = 成功，false = 失败
// =============================================================================
bool_t ModuleInstall_IICBus(void)
{
    s_ptIICBusType = DjyBus_BusTypeAdd("IICBus");
    if(NULL != s_ptIICBusType)
    {
        info_printf("bus","i2c installed successful.");
        return true;
    }
    else
    {
        error_printf("bus","i2c installed failed.");
        return false;
    }

}

// =============================================================================
// 功能：新增IIC总线结点到IIC总线类型结点,并初始化IIC_CB控制块结构体
// 参数：NewIICParam,新增总线参数，参数说明详细请参照IIC_Param结构体
// 返回：返回建立的资源结点指针，失败时返回NULL
// =============================================================================
struct IIC_CB *IIC_BusAdd(struct IIC_Param *NewIICParam)
{
    struct Object *IICDev;
    struct IIC_CB *NewIIC;
    if(NULL == NewIICParam)
        goto exit_from_param;

    //避免重复建立同名的IIC总线
    if(NULL != OBJ_SearchChild(s_ptIICBusType,(const char*)(NewIICParam->BusName)))
        goto exit_from_readd;

    NewIIC = (struct IIC_CB *)M_Malloc(sizeof(struct IIC_CB),0);
    if(NewIIC == NULL)
        goto exit_from_malloc;

    //将总线结点挂接到总线类型结点的子结点
    IICDev = OBJ_NewChild(s_ptIICBusType, (fnObjOps)-1, (ptu32_t)NewIIC,
                                    (const char*)(NewIICParam->BusName));
    if(IICDev == NULL)
        goto exit_from_add_node;

    //创建总线信号量和缓冲区信号量
    NewIIC->IIC_BusSemp= Lock_SempCreate(1,1,CN_BLOCK_FIFO,"iic bus semp");
    if(NewIIC->IIC_BusSemp== NULL)
        goto exit_from_iic_bus_semp;
    NewIIC->IIC_BufSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"iic buf semp");
    if(NewIIC->IIC_BufSemp== NULL)
        goto exit_from_iic_buf_semp;

    //IIC_CB控制块初始化
    NewIIC->HostObj             = IICDev;
    NewIIC->ErrorPopEvtt        = CN_EVTT_ID_INVALID;
    NewIIC->SpecificFlag        = NewIICParam->SpecificFlag;
    NewIIC->pWriteReadPoll      = NewIICParam->pWriteReadPoll;
    NewIIC->pGenerateWriteStart = NewIICParam->pGenerateWriteStart;
    NewIIC->pGenerateReadStart  = NewIICParam->pGenerateReadStart;
    NewIIC->pGenerateEnd        = NewIICParam->pGenerateEnd;
    NewIIC->pBusCtrl            = NewIICParam->pBusCtrl;

    //缓冲区初始化
    NewIIC->IIC_Buf.pBuf     = NewIICParam->IICBuf;
    NewIIC->IIC_Buf.MaxLen   = NewIICParam->IICBufLen;
    NewIIC->IIC_Buf.Offset   = 0;
    //标志初始化，包括 读写标志和轮询中断方式
    NewIIC->Flag =0;

    info_printf("i2cbus","%s added.",NewIICParam->BusName);

    return NewIIC;

exit_from_iic_buf_semp:
    Lock_SempDelete(NewIIC->IIC_BusSemp);
exit_from_iic_bus_semp:
    OBJ_Delete(NewIIC->HostObj);
exit_from_add_node:
    free(NewIIC);
exit_from_malloc:
exit_from_readd:
exit_from_param:
    error_printf("i2cbus","%s add failed.",NewIICParam->BusName);
    return NULL;
}

// =============================================================================
// 功能：删除在IIC总线类型上的IIC结点，若被删除的总线上有器件结点，则删除会不成功,需
//       释放新增结点时malloc的控制块内存
// 参数：DelIIC,删除IIC控制块指针
// 返回：true,删除成功;false,删除失败
// =============================================================================
bool_t IIC_BusDelete(struct IIC_CB *DelIIC)
{
    bool_t result;
    if(NULL == DelIIC)
        return false;
    if(OBJ_Delete(DelIIC->HostObj))
    {
        result = false;
    }
    else
    {
        result = true;
        free(DelIIC);
    }
    return result;
}

// =============================================================================
// 功能：查找IIC总线类型结点的子结点中是否存被查找名称的总线
// 参数：BusName,查找的总线名称
// 返回：查找的控制块结点指针，未找到时返回NULL
// =============================================================================
struct IIC_CB *IIC_BusFind(const char *BusName)
{
    struct Object *IIC_Obj;
    IIC_Obj = OBJ_SearchChild(s_ptIICBusType,BusName);
    if(IIC_Obj)
        return (struct IIC_CB *)OBJ_GetPrivate(IIC_Obj);
    else
        return NULL;
}

// =============================================================================
// 功能：在IIC总线结点上增加器件，即总线上挂接的器件，该器件属于总线结点的子结点
// 参数：BusName,新增子器件挂接的总线名称
//       DevName,器件名称
//       DevAddr,器件地址，低7位有效，未包含读写标志位，如0x50
//       BitOfMaddrInDaddr,存储器地址在器件地址的比特位数
//       BitOfMaddr,器件存储地址的总位数
// 返回：控制块结点指针，添加失败时返回NULL
// =============================================================================
struct IIC_Device *IIC_DevAdd(const char *BusName ,const char *DevName, u8 DevAddr,
                                u8 BitOfMaddrInDaddr, u8 BitOfMaddr)
{
    struct IIC_CB      *IIC;
    struct IIC_Device *NewDev;

    //查询是否该总线存在
    IIC = IIC_BusFind(BusName);
    if(NULL == IIC)
        return NULL;

    //避免建立同名的IIC器件
    if(NULL != OBJ_SearchChild(IIC->HostObj, DevName))
        return NULL;

    //为新的器件结点动态分配内存
    NewDev = (struct IIC_Device *)M_Malloc(sizeof(struct IIC_Device),0);
    if(NULL == NewDev)
        return NULL;

    //为新结点初始化
    NewDev->DevAddr              = DevAddr;
    NewDev->BitOfMemAddrInDevAddr = BitOfMaddrInDaddr;
    NewDev->BitOfMemAddr          = BitOfMaddr;
    NewDev->HostObj = OBJ_NewChild(IIC->HostObj, (fnObjOps)-1, (ptu32_t)NewDev, DevName);
    if(NewDev->HostObj == NULL)
    {
        free(NewDev);
        return NULL;
    }

    info_printf("i2cbus","device \"%s\" added to \"%s\"", DevName, BusName);
    return NewDev;
}

// =============================================================================
// 功能：删除IIC总线上的器件，从总线子结点中删除
// 参数：DelDev,删除的器件指针
// 返回：true,删除成功;false,删除失败
// =============================================================================
bool_t IIC_DevDelete(struct IIC_Device *DelDev)
{
    bool_t result;
    if(NULL == DelDev)
        return false;

    if(OBJ_Delete(DelDev->HostObj))
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
// 功能：查找器件结点，该结点必然是挂接在相应的IIC总线结点上
// 参数：BusName,被搜索的IIC总线名称
//       DevName,器件名称
// 返回：结点附属结构体指针
// =============================================================================
struct IIC_Device *IIC_DevFind(const char *BusName ,const char *DevName)
{
    struct Object *IIC_DevObj;
    struct IIC_CB *IIC_Bus;

    IIC_Bus = IIC_BusFind(BusName);
    if(NULL == IIC_Bus)
        return NULL;

    //通过IIC类型结点，向下搜索后代结点
    IIC_DevObj = OBJ_SearchChild(IIC_Bus->HostObj, DevName);
    if(IIC_DevObj)
        return (struct IIC_Device *)OBJ_GetPrivate(IIC_DevObj);
    else
        return NULL;
}

// =============================================================================
// 功能：上层调用写API函数，该函数根据Dev结点获得其所在总线结点，调用总线回调函数完成
//       发送启动时序，并且根据阻塞选项，决定是阻塞等待发送完成还是直接返回。timeout
//       参数用于指定函数在确定的时间内完成，超时则强制返回，并停止时序，释放信号量
// 参数：DelDev,器件结构体指针
//       addr,发送地址，即指存储地址
//       buf,发送数据缓冲区指针
//       len,发送数据长度，字节单位
//       block_option,阻塞选项,true表示阻塞发送，false为非阻塞发送
//       timeout,超时时间，us
// 返回：发送的字节数
// =============================================================================
s32  IIC_Write(struct IIC_Device *Dev, u32 addr,u8 *buf,u32 len,
                bool_t block_option,u32 timeout)
{
    struct IIC_CB *IIC;
    u8 DevAddr,MemAddrLen;
    u32 written=0;
    s32 result = 0;
    u32 base_time = 0,rel_timeout = timeout;

    if(NULL == Dev)
        return CN_IIC_EXIT_PARAM_ERR;

    base_time = (u32)DJY_GetSysTime();
    //查找该器件属于哪条总线
    IIC = (struct IIC_CB*)OBJ_GetPrivate(OBJ_GetParent(Dev->HostObj));
    if(NULL == IIC)
        return CN_IIC_EXIT_PARAM_ERR;
    //需要等待总线空闲
    if(false == Lock_SempPend(IIC->IIC_BusSemp,timeout))
        return CN_IIC_EXIT_TIMEOUT;

    rel_timeout = timeout - ((u32)DJY_GetSysTime() - base_time);
    //DevAddr = Dev->DevAddr |
    //        ((u8)(addr >> (Dev->BitOfMemAddr - Dev->BitOfMemAddrInDevAddr))
    //        & ~(0xFF<<Dev->BitOfMemAddrInDevAddr));
    DevAddr = Dev->DevAddr |
              (((u8)(addr >> (Dev->BitOfMemAddr - Dev->BitOfMemAddrInDevAddr))
              & ~(0xFF<<Dev->BitOfMemAddrInDevAddr))<<1);//the lowest is read or write in dev address
    //计算发送地址字节数
    //change by lst
//    MemAddrLen = (Dev->BitOfMemAddr)/8 +
//            ((Dev->BitOfMemAddr - Dev->BitOfMemAddrInDevAddr)%8 ? 1:0);
    MemAddrLen = (Dev->BitOfMemAddr - Dev->BitOfMemAddrInDevAddr +7)/8;
    IIC->Flag &= ~CN_IIC_FLAG_R;
    //判断是否用轮询方式
    if((DJY_QuerySch() == false) || (IIC->pGenerateWriteStart == NULL)
            || (IIC->Flag & CN_IIC_FLAG_POLL))
    {
        if(IIC->pWriteReadPoll != NULL)
        {
            result = IIC->pWriteReadPoll(IIC->SpecificFlag,DevAddr,addr,
                                MemAddrLen,buf,len,CN_IIC_WRITE_FLAG);
            if(result)
                result = len;
            else
                result = 0;
        }
        else
        {
            result = CN_IIC_EXIT_FUN_NULL;
        }
        Lock_SempPost(IIC->IIC_BusSemp);
        return result;
    }

    IIC->IIC_Buf.Offset = 0;                            //发送前先清空缓冲区
    IIC->Counter = len;

    //如果发送字节数小于等于缓冲区大小，则直接填写缓冲区；
    //若不是，则先发送调用者提供的缓冲区，直到剩余字节数能够填充到缓冲区
    if(len <= IIC->IIC_Buf.MaxLen)
    {
        memcpy(IIC->IIC_Buf.pBuf,buf,len);                  //写缓冲区
        written = len;
    }
    else
    {
        IIC->pBuf = buf;                                        //指向用户缓冲区
    }

    //调用启动时序的回调函数
    if(true == IIC->pGenerateWriteStart(IIC->SpecificFlag,DevAddr,
                                        addr,MemAddrLen,len,IIC->IIC_BusSemp))
    {
        rel_timeout = (u32)DJY_GetSysTime();
        if(rel_timeout - base_time < timeout)
            rel_timeout = timeout - (rel_timeout - base_time);
        else
        {
            IIC->pGenerateEnd(IIC->SpecificFlag);
            return CN_IIC_EXIT_TIMEOUT;
        }

        if(true == block_option)                    //阻塞发送需等待bus信号量
        {
            if(Lock_SempPend(IIC->IIC_BusSemp,rel_timeout))//等待中断函数释放信号量
            {
                //阻塞发送时，会由本函数释放总线信号量
                Lock_SempPost(IIC->IIC_BusSemp);
                result = len;
            }else                                    //阻塞超时返回
            {
                IIC->pGenerateEnd(IIC->SpecificFlag);//超时退出，结束时序
                Lock_SempPost(IIC->IIC_BusSemp);         //释放总线信号量
                result = len - IIC->Counter;
            }
        }else                                       //非阻塞发送
        {
            if(written != len)                      //没有填到缓冲区
            {
                //IIC_PortRead将buf中剩余的数据填入简易缓冲区时，释放信号量
                if(false == Lock_SempPend(IIC->IIC_BufSemp,rel_timeout))
                {
                    IIC->pGenerateEnd(IIC->SpecificFlag);
                    return CN_IIC_EXIT_TIMEOUT;
                }else
                {
                    result = len - IIC->Counter;
                }
            }else                                   //填写了简易缓冲区
            {
                result = len;
            }
        }
    }else
    {
        IIC->pGenerateEnd(IIC->SpecificFlag);       //超时退出，结束时序
        Lock_SempPost(IIC->IIC_BusSemp);                //释放总线信号量
        return CN_IIC_EXIT_UNKNOW_ERR;
    }
    //正常情况下，非阻塞发送会由底层驱动释放总线信号量
    return result;
}

// =============================================================================
// 功能：读函数，该函数首先根据Dev查出其所属总线，然后调用总线读回调函数，启动读取。
//       产生读数据时序，并阻塞等待接收完成，当获得接收完成的信号量时，释放总线控制
//       信号量。超时参数timeout用于用户指定在确定时间内完成总线操作，超时则强制返回
// 参数：Dev,器件结构体指针
//       addr,接收地址，即指存储地址
//       buf,接收数据缓冲区指针
//       len,接收数据长度，字节单位
//       timeout,超时时间，us
// 返回：接收的字节数
// =============================================================================
s32  IIC_Read(struct IIC_Device *Dev,u32 addr,u8 *buf,u32 len,u32 timeout)
{
    struct IIC_CB *IIC;
    u8 DevAddr,MemAddrLen;
    s32 result = len;
    u32 base_time = 0,rel_timeout = timeout;

    if(NULL == Dev)
        return CN_IIC_EXIT_PARAM_ERR;

    base_time = (u32)DJY_GetSysTime();
    //查找该器件属于哪条总线
    IIC = (struct IIC_CB *)OBJ_GetPrivate(OBJ_GetParent(Dev->HostObj));
    if(NULL == IIC)
        return CN_IIC_EXIT_PARAM_ERR;

    if(false == Lock_SempPend(IIC->IIC_BusSemp,timeout))        //需要等待总线空闲
        return CN_IIC_EXIT_TIMEOUT;

    Lock_SempPend(IIC->IIC_BufSemp,0);                          //相当于清二值信号量

    rel_timeout = timeout - ((u32)DJY_GetSysTime() - base_time);
    //DevAddr = Dev->DevAddr |
    //        ((u8)(addr >> (Dev->BitOfMemAddr - Dev->BitOfMemAddrInDevAddr))
    //        & ~(0xFF<<Dev->BitOfMemAddrInDevAddr));
     DevAddr = Dev->DevAddr |
              (((u8)(addr >> (Dev->BitOfMemAddr - Dev->BitOfMemAddrInDevAddr))
              & ~(0xFF<<Dev->BitOfMemAddrInDevAddr))<<1);//the lowest is read or write in dev address

    //计算发送地址字节数
//    MemAddrLen = (Dev->BitOfMemAddr)/8 +
//            ((Dev->BitOfMemAddr - Dev->BitOfMemAddrInDevAddr)%8 ? 1:0);
    MemAddrLen = (Dev->BitOfMemAddr - Dev->BitOfMemAddrInDevAddr +7)/8;

    IIC->Flag |= CN_IIC_FLAG_R;
    //判断是否用轮询方式
    if((DJY_QuerySch() == false) || (IIC->pGenerateReadStart == NULL)
            || (IIC->Flag & CN_IIC_FLAG_POLL))
    {
        if(IIC->pWriteReadPoll != NULL)
        {
            result = IIC->pWriteReadPoll(IIC->SpecificFlag,DevAddr,addr,
                                MemAddrLen,buf,len,CN_IIC_READ_FLAG);
            if(result)
                result = len;
            else
                result = 0;
        }
        else
        {
            result = CN_IIC_EXIT_FUN_NULL;
        }
        Lock_SempPost(IIC->IIC_BusSemp);
        return result;
    }
    IIC->Counter = len;
    IIC->pBuf    = buf;

    //启动读数据时序
    if(IIC->pGenerateReadStart(IIC->SpecificFlag,DevAddr,addr,MemAddrLen,
                                len,IIC->IIC_BufSemp))
    {
        rel_timeout = (u32)DJY_GetSysTime();
        if(rel_timeout - base_time < timeout)           //未超时
        {
            rel_timeout = timeout - (rel_timeout - base_time);

            if(!Lock_SempPend(IIC->IIC_BufSemp,rel_timeout))//阻塞等待接收完成
            {
                result = CN_IIC_EXIT_TIMEOUT;
            }
            //及时正确接收到所有数据
        }
        else                                            //超时退出
        {
            result = CN_IIC_EXIT_TIMEOUT;
        }
    }

    if(result == CN_IIC_EXIT_TIMEOUT)
    {
        IIC->pGenerateEnd(IIC->SpecificFlag);   //超时退出结束总线
    }
    Lock_SempPost(IIC->IIC_BusSemp);                //释放总线信号量
    return len-IIC->Counter;                    //返回读到数据量
}

// =============================================================================
// 功能：读发送缓冲区数据，由总线驱动调用，总线驱动读取到数据后将数据写入寄存器发送
// 参数：IIC,IIC控制块指针
//       buf,读数据缓冲区指针
//       len,读数据长度，字节单位
// 返回：字节数
// =============================================================================
s32 IIC_PortRead( struct IIC_CB *IIC,u8 *buf,u32 len)
{
    u32 Result = 0,RingLen,CpyLen;
    u8 *pbuf;

    //容错处理，如果是读数据过程，则不该进入到此处
    if(IIC->Flag & CN_IIC_FLAG_R)
        return 0;

    if((len > 0) && (IIC->Counter > 0))
    {
        CpyLen = IIC->Counter >= len ?len:IIC->Counter;
        RingLen = IIC->IIC_Buf.MaxLen;
        //判断是否从简易缓冲区中读数据
        if(IIC->Counter <= RingLen)                     //从缓冲区中读数据
        {
            pbuf = &IIC->IIC_Buf.pBuf[IIC->IIC_Buf.Offset];
            IIC->IIC_Buf.Offset += CpyLen;
            memcpy(buf,pbuf,CpyLen);
        }
        else                                            //从pbuf中读数据
        {
            memcpy(buf,IIC->pBuf,CpyLen);
            IIC->pBuf += CpyLen;

            if(IIC->Counter - CpyLen <= RingLen)        //将pbuf剩余数据写入缓冲
            {
                memcpy(IIC->IIC_Buf.pBuf,IIC->pBuf,IIC->Counter-CpyLen);
                Lock_SempPost(IIC->IIC_BufSemp);
            }
        }
        IIC->Counter -= CpyLen;
        Result = CpyLen;
    }

    return Result;
}

// =============================================================================
// 功能：将接收到的数据写入用户提供的缓冲区中，接收是阻塞方式，因此使用用户的缓冲区
// 参数：IIC,IIC控制块指针
//       buf,数据指针
//       len,数据长度，字节单位
// 返回：字节数
// =============================================================================
s32 IIC_PortWrite(struct IIC_CB *IIC,u8 *buf,u32 len)
{
    u32 cpylen = 0;

    //容错处理，如果是写数据过程，则不该进入到此处
    if(!(IIC->Flag & CN_IIC_FLAG_R))
        return 0;
    if((len > 0) && (IIC->Counter > 0))
    {
        cpylen = IIC->Counter >= len ?len:IIC->Counter;
        memcpy(IIC->pBuf,buf,cpylen);
        IIC->pBuf += cpylen;
        IIC->Counter -= cpylen;
    }

    return cpylen;
}

// =============================================================================
// 功能：根据cmd命令，解析数据data1和Data2，控制IIC，硬件相关的命令需调用回调函数
//       由底层驱动完成
// 参数：Dev,器件结构体指针
//       cmd,命令
//       data1,data2,参数数据，根据具体命令定义不同
// 返回：-1=参数检查出错,否则由cmd决定,若需要调用用户的pBusCtrl,则是该函数返回值
// =============================================================================
s32 IIC_BusCtrl(struct IIC_Device *Dev,u32 cmd,ptu32_t data1,ptu32_t data2)
{
    struct IIC_CB *IIC;
    s32 result = 1;
    if(NULL == Dev)
        return -1;

    IIC = (struct IIC_CB *)OBJ_GetPrivate(OBJ_GetParent(Dev->HostObj));
    if(NULL == IIC)
        return -1;

    switch(cmd)
    {
    case CN_IIC_SET_CLK:
    case CN_IIC_DMA_USED:
    case CN_IIC_DMA_UNUSED:
        result = IIC->pBusCtrl(IIC->SpecificFlag,cmd,data1,data2);
        break;
    case CN_IIC_SET_ERROR_POP_EVENT:
        IIC->ErrorPopEvtt = data1;
        result = 1;
        break;
    case CN_IIC_SET_POLL:
        result = IIC->pBusCtrl(IIC->SpecificFlag,cmd,data1,data2);
        IIC->Flag |=  CN_IIC_FLAG_POLL;
        break;
    case CN_IIC_SET_INT:
        IIC->Flag &= ~CN_IIC_FLAG_POLL;
        break;
    default:
        break;
    }
    return result;
}

// =============================================================================
// 功能：IIC错误弹出API函数，当发生错误时，调用该函数，弹出错误处理事件，并将错误号
//       以参数的形式传递给弹出的事件
// 参数：IIC,IIC控制块指针
//       ErrNo,发生错误序号，如未收到ACK信号CN_IIC_NO_ACK_ERR
// 返回：执行结果，CN_EXIT_NO_ERR为无错误
// =============================================================================
s32 IIC_ErrPop(struct IIC_CB *IIC, u32 ErrNo)
{
    if(NULL == IIC)
        return CN_IIC_EXIT_PARAM_ERR;

    //弹出错误处理事件，并将错误号以参数形式传递
    DJY_EventPop(IIC->ErrorPopEvtt,NULL,0,ErrNo,0,0);

    return 0;
}
