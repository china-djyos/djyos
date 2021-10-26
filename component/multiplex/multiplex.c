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
//所属模块: multiplex模块
//作者:  罗侍田.
//版本：V1.0.0
//文件描述:实现多路复用的功能
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2014-06-24
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "stdint.h"
#include "stddef.h"
#include "systime.h"
#include "object.h"
#include "objhandle.h"
#include "int.h"
#include "pool.h"
#include "lock.h"
#include <djyos.h>
#include "multiplex.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_Multiplex(void);
//    ModuleInstall_Multiplex ();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"multiplex"//多路复用
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:system              //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"lock"  //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_MULTIPLEX == false )
//#warning  " multiplex  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_MULTIPLEX    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure


struct MultiplexSetsCB
{
    struct MultiplexObjectCB *ObjectQ;   // 指向未触发的对象链表
    struct MultiplexObjectCB *ActiveQ;   // 指向已触发的对象链表
    u32 ObjectSum;          // 对象集中包含的对象数。
    u32 ActiveLevel;        // 触发水平，被触发对象数量达到ActiveLevel，将触发
                            // 对象集，大于ObjectSum 表示全触发
    u32 Actived;            // 已经触发的对象数
    bool_t SetsActived;     // 对象集是否已经触发。
    struct SemaphoreLCB Lock;       // 等待Sets被触发用的锁。
};

struct MultiplexObjectCB
{
    struct MultiplexObjectCB *NextObject;//纵向双向链表，用于连接一个MultiplexSets
                                         //包含的多个object
    struct MultiplexObjectCB *PreObject;
    struct MultiplexObjectCB *NextSets; //横向单向链表，用于一个object被多个
                                        //MultiplexSets包含的情况
    struct MultiplexSetsCB *MySets;     //指向主控制块
    s32 Fd;                             //被MultiplexSets等待的文件
    ptu32_t ObjectID;                   //被MultiplexSets等待的对象
    u32 ET_SaveBit;                     // ET模式触发 wait 函数返回的触发位。
    u32 PendingBit;                     //bit0~23：对象中已经触发的bit，
                                        //bit31：1=对象已激活。
    u32 SensingBit;                     //bit0~23：敏感位标志
                                        //bit24~31表示敏感位检测类型，参见
                                        //CN_MULTIPLEX_SENSINGBIT_MODE定义
};

static struct MemCellPool *g_ptMultiplexSetsPool;
static struct MemCellPool *g_ptMultiplexObjectPool;
static struct MutexLCB MultiplexMutex;

//把Object从*SrcList队列取出，放到*TarList队列头上去
static void __Multiplex_ChangeList(struct MultiplexObjectCB **SrcList,
                        struct MultiplexObjectCB **TarList,
                        struct MultiplexObjectCB *Object)
{
    Object->NextObject->PreObject = Object->PreObject;
    Object->PreObject->NextObject = Object->NextObject;
    if (*SrcList == Object)
    {
        if (Object->NextObject == Object)
        {
            *SrcList = NULL;
        } else
        {
            *SrcList = Object->NextObject;
        }
    }
    //把Object放到Active队列中
    if (*TarList == NULL)
    {
        *TarList = Object;
        Object->NextObject = Object;
        Object->PreObject = Object;
    } else
    {
        Object->PreObject = (*TarList)->PreObject;
        Object->NextObject = *TarList;
        (*TarList)->PreObject->NextObject = Object;
        (*TarList)->PreObject = Object;

        *TarList = Object;
    }
}

//判断一个Object是否已经触发
bool_t __Multiplex_ObjectIsActived(u32 Pending, u32 Sensing, u32 Mode)
{
    if (Mode & CN_MULTIPLEX_SENSINGBIT_OR)
    {
        if ((Pending & Sensing) != 0)
            return true;
        else
            return false;
    }
    else
    {
        if ((Pending & Sensing) == Sensing)
            return true;
        else
            return false;
    }
}

//----模块初始化---------------------------------------------------------------
//功能: 初始化Multiplex模块。创建互斥量，初始化内存池。
//参数：para，无效。
//返回：true=成功，false=失败。
//-----------------------------------------------------------------------------
bool_t ModuleInstall_Multiplex(void)
{
    Lock_MutexCreate_s(&MultiplexMutex, "Multiplex control block");
    //创建MultiplexSets控制块内存池，内存池的初始状态是空的。
    g_ptMultiplexSetsPool = Mb_CreatePool(NULL, 0, sizeof(struct MultiplexSetsCB),
        5, CN_MULTIPLEX_MAX_SETS, "Multiplex control block");
    if (g_ptMultiplexSetsPool == NULL)
        return false;
    //创建Multiplex对象控制块内存池，内存池的初始状态是空的。
    g_ptMultiplexObjectPool = Mb_CreatePool(NULL, 0, sizeof(struct MultiplexObjectCB),
        5, CN_MULTIPLEX_MAX_OBJECTS, "Multiplex control block");
    if (g_ptMultiplexObjectPool == NULL)
    {
        Mb_DeletePool(g_ptMultiplexObjectPool);
        return false;
    }
    return true;
}

//----创建MultiplexSets----------------------------------------------------------
//功能：创建一个MultiplexSets，并且设定属性。
//参数：ActiveLevel, 触发水平,该集合被触发的对象达到此数后,触发集合.
//返回：新创建的MultiplexSets指针
//----------------------------------------------------------------------------
struct MultiplexSetsCB* Multiplex_Create(u32 ActiveLevel)
{
    struct MultiplexSetsCB *SetsQ;
    if (ActiveLevel == 0)       //触发水平必须大于0
        return NULL;
    SetsQ = (struct MultiplexSetsCB *)Mb_Malloc(g_ptMultiplexSetsPool,
        CN_TIMEOUT_FOREVER);
    if (SetsQ != NULL)
    {
        SetsQ->ObjectQ = NULL;
        SetsQ->ActiveQ = NULL;
        SetsQ->ObjectSum = 0;
        SetsQ->ActiveLevel = ActiveLevel;
        SetsQ->Actived = 0;
        SetsQ->SetsActived = false;
        Lock_SempCreate_s(&SetsQ->Lock, 1, 0, CN_BLOCK_FIFO, NULL);
        return SetsQ;
    } else
        return NULL;
}

//----添加对象到MultiplexSets--------------------------------------------------
//功能: MultiplexSets中添加一个对象。如果该Object的初始状态是已经触发，则加入到
//      ActiveQ队列，否则加入ObjectQ队列。
//参数: Sets，被操作的MultiplexSets指针
//      Fd，待加入Sets的文件描述符。
//      SensingBit，bit0~23，24个敏感bit，设为1表示本对象对这个bit标志敏感，参见
//                          CN_MULTIPLEX_SENSINGBIT_READ 的定义
//                  bit24~31表示模式，参见 CN_MULTIPLEX_SENSINGBIT_OR 的定义
//返回: true=成功，false=失败。
//-----------------------------------------------------------------------------
bool_t Multiplex_AddObject(struct MultiplexSetsCB *Sets,s32 Fd, u32 SensingBit)
{
    struct objhandle *Kfp;
    struct MultiplexObjectCB *temp;
    struct MultiplexObjectCB **TargetQ;
    u32 InitStatus;
    bool_t repeat = false;
    bool_t IsActived = 0;

    Kfp = fd2Handle(Fd);
    if ((NULL == Sets) || (NULL == Kfp) )
        return false;

    Lock_MutexPend(&MultiplexMutex, CN_TIMEOUT_FOREVER);
    temp = __Handle_GetMultiplexHead(Kfp);
    InitStatus = Handle_MultiEvents(Kfp);
    //循环检查一个Object是否重复加入同一个MultiplexSets
    //如果ObjectHead=NULL,检查结果是不重复，后续处理能够正确运行。
    while (temp != NULL)
    {
        if (temp->MySets != Sets)
            temp = temp->NextSets;
        else
        {
            repeat = true;
            break;
        }
    }
    Lock_MutexPost(&MultiplexMutex);

    if (repeat == false)
    {
        InitStatus &= ~CN_MULTIPLEX_MODEMSK;      //清除模式位
        //下面检查新加入的Object是否已经触发，以决定加入到MultiplexSets的哪个队列中
        if (__Multiplex_ObjectIsActived(InitStatus,
                SensingBit & CN_MULTIPLEX_STATUSMSK,
                SensingBit & CN_MULTIPLEX_MODEMSK))
        {
            TargetQ = &Sets->ActiveQ;
            IsActived = true;
        }
        else
            TargetQ = &Sets->ObjectQ;

        temp = (struct MultiplexObjectCB *)Mb_Malloc(g_ptMultiplexObjectPool, CN_TIMEOUT_FOREVER);
        if (temp != NULL)
        {
            Sets->ObjectSum++;
            temp->ET_SaveBit = 0;
            temp->SensingBit = SensingBit;
            temp->PendingBit = InitStatus & CN_MULTIPLEX_STATUSMSK;
            temp->Fd = Fd;
//          temp->ObjectID = ObjectID;
            Lock_MutexPend(&MultiplexMutex, CN_TIMEOUT_FOREVER);
            temp->MySets = Sets;            //设定对象所属MultiplexSets
                                            //同一个MultiplexSets包含多个对象，NextObject把这些对象链接起来。
            if (*TargetQ == NULL)
            {
                *TargetQ = temp;
                temp->NextObject = temp;
                temp->PreObject = temp;
            } else
            {
                //新加入MultiplexSets的对象插入队列头部
                temp->PreObject = (*TargetQ)->PreObject;
                temp->NextObject = *TargetQ;
                (*TargetQ)->PreObject->NextObject = temp;
                (*TargetQ)->PreObject = temp;

                (*TargetQ) = temp;
            }
            //同一个对象被多个MultiplexSets包含，用NextSets链接。
            //NextSets是单向链表，新对象插入链表头部
            temp->NextSets = __Handle_GetMultiplexHead(Kfp);
            __Handle_SetMultiplexHead(Kfp, temp);
            Lock_MutexPost(&MultiplexMutex);
            if (IsActived)
            {
                Sets->Actived += 1;
                temp->PendingBit |= CN_MULTIPLEX_OBJECT_ACTIVED;
                if ((Sets->Actived >= Sets->ActiveLevel)
                    || (Sets->Actived >= Sets->ObjectSum))
                {
                    if (false == Sets->SetsActived)
                    {
                        Sets->SetsActived = true;
                        Lock_SempPost(&Sets->Lock);
                    }
                }
            }
        } else
            return false;
    } else
    {
        //重复加入，无须做任何处理
    }
    return true;
}

//----从MultiplexSets删除对象-----------------------------------------------------
//功能: MultiplexAdd的逆函数
//参数: Sets，被操作的MultiplexSets指针
//      Fd，被删除的对象
//返回: true=成功，false=失败。
//-----------------------------------------------------------------------------
bool_t Multiplex_DelObject(struct MultiplexSetsCB *Sets,s32 Fd)
{
    struct objhandle *Kfp;
    struct MultiplexObjectCB *Object,*following;

    Kfp = fd2Handle(Fd);
    if ((Sets == NULL) || (Kfp == NULL))
        return false;
    Lock_MutexPend(&MultiplexMutex, CN_TIMEOUT_FOREVER);
    Object = __Handle_GetMultiplexHead(Kfp);
    following = NULL;
    while (Object != NULL)
    {       //查找被删除的对象控制块
        if (Object->MySets != Sets)
        {
            following = Object;
            Object = Object->NextSets;
        }
        else
        {
            break;
        }
    }

    if (Object != NULL)
    {
        //下面检查被删除的Object是否已经触发，
        if (__Multiplex_ObjectIsActived(Object->PendingBit,
                Object->SensingBit & CN_MULTIPLEX_STATUSMSK,
                Object->SensingBit & CN_MULTIPLEX_MODEMSK))
        {
            if (Sets->Actived != 0)
            {
                Sets->Actived--;
                if (Sets->Actived == 0)
                    Sets->SetsActived = false;
            }
        }

        Sets->ObjectSum--;
        if (Object->NextObject != Object)
        {    //链表中不止一个结点
            Object->NextObject->PreObject = Object->PreObject;
            Object->PreObject->NextObject = Object->NextObject;
            if (Object == Sets->ObjectQ)
            {
                Sets->ObjectQ = Object->NextObject;
            }
            if (Object == Sets->ActiveQ)
            {
                Sets->ActiveQ = Object->NextObject;
            }
        } else {                                //链表只有一个结点
            if (Object == Sets->ObjectQ)
            {
                Sets->ObjectQ = NULL;
            }
            if (Object == Sets->ActiveQ)
            {
                Sets->ActiveQ = NULL;
            }
        }
        if(following == NULL)       // Fd是链表头
            __Handle_SetMultiplexHead(Kfp, Object->NextSets);
        else
            following->NextSets = Object->NextSets;
        Mb_Free(g_ptMultiplexObjectPool, Object);
    }
    Lock_MutexPost(&MultiplexMutex);
    return true;
}

//----Multiplex设置状态--------------------------------------------------------
//功能：告知MultiplexSets中被监控对象的当前状态。
//参数: ObjectHead，被操作的Object队列头指针
//      Status，Object的当前状态
//返回: true=成功，false=失败。
//-----------------------------------------------------------------------------
bool_t __Multiplex_Set(s32 Fd, u32 Status)
{
    struct objhandle *Kfp;
    struct MultiplexObjectCB *Object;
    struct MultiplexSetsCB *Sets;
    u32 Sensing, Type, NewPendsing,MaskPending;
    u32 OldPend;
    bool_t ActivedFlag;
    Kfp = fd2Handle(Fd);
    if (Kfp == NULL)
        return false;
//  Lock_MutexPend(&MultiplexMutex, CN_TIMEOUT_FOREVER);
    Object = __Handle_GetMultiplexHead(Kfp);
    while (Object != NULL)
    {
        Int_SaveAsynSignal();
        OldPend = Object->PendingBit;
        Sets = Object->MySets;
        ActivedFlag = Sets->SetsActived;
        Sensing = Object->SensingBit & CN_MULTIPLEX_STATUSMSK;
        Type = Object->SensingBit & CN_MULTIPLEX_MODEMSK;
        NewPendsing = Status & Sensing;
        Object->PendingBit = (OldPend & CN_MULTIPLEX_OBJECT_ACTIVED)|NewPendsing;
        if (Object->SensingBit & CN_MULTIPLEX_SENSINGBIT_ET)    //ET（边沿）模式
        {
            MaskPending = (~(OldPend&Object->ET_SaveBit)) & NewPendsing;
            //这个逻辑比较复杂，参看相关设计文档
            Object->ET_SaveBit = Object->ET_SaveBit & OldPend & NewPendsing;
        }
        else
            MaskPending = NewPendsing;
        if (Object->PendingBit & CN_MULTIPLEX_OBJECT_ACTIVED)
        {                                           //调用前，Object已触发
            if (!__Multiplex_ObjectIsActived(MaskPending, Sensing, Type))
            {
                //调用Multiplex_Set导致对象变成未触发
                //把Object从Sets->ActiveQ队列拿出，放到ObjectQ队列中
                __Multiplex_ChangeList(&(Sets->ActiveQ), &(Sets->ObjectQ), Object);
                Object->PendingBit &= ~CN_MULTIPLEX_OBJECT_ACTIVED;
                if (Sets->Actived != 0)
                    Sets->Actived--;
                if (Sets->Actived == 0)
                    ActivedFlag = false;
            }
        } else
        {                                           //调用前，Object未触发
            if (__Multiplex_ObjectIsActived(MaskPending, Sensing, Type))
            {
                //调用Multiplex_Set导致对象被触发

                //把Object从Sets->ObjectQ队列拿出，放到ActiveQ队列中
                __Multiplex_ChangeList(&(Sets->ObjectQ), &(Sets->ActiveQ), Object);
                Object->PendingBit |= CN_MULTIPLEX_OBJECT_ACTIVED;
                if (Sets->Actived < Sets->ObjectSum)
                    Sets->Actived++;
                //须释放信号量
                if ((Sets->Actived >= Sets->ActiveLevel)
                    || (Sets->Actived >= Sets->ObjectSum))
                {
                    ActivedFlag = true;
                }
            }
        }

        if (ActivedFlag == true)
        {
            if(false == Sets->SetsActived)
            {
                Sets->SetsActived = ActivedFlag;
                Int_RestoreAsynSignal();
                Lock_SempPost(&Sets->Lock);
            }
            else
            {
                Sets->SetsActived = ActivedFlag;
                Int_RestoreAsynSignal();
            }
        }
//      else if((ActivedFlag == false) && (true == Sets->SetsActived))
//      {
//          Sets->SetsActived = ActivedFlag;
//          Lock_SempPend(&Sets->Lock,0);
//          Int_RestoreAsynSignal();
//      }
        else
        {
            if(true == Sets->SetsActived)
            {
                Sets->SetsActived = ActivedFlag;
                Lock_SempPend(&Sets->Lock, 0);
                Int_RestoreAsynSignal();
            }
            else
            {
                Sets->SetsActived = ActivedFlag;
                Int_RestoreAsynSignal();
            }
        }

        Object = Object->NextSets;
    }
//  Lock_MutexPost(&MultiplexMutex);
    return true;
}

//----等待MultiplexSets-----------------------------------------------------------
//功能: 设置好MultiplexSets后，应用程序调用本函数等待MultiplexSets被触发，根据
//      Type值，等待方式分轮询和异步触发两种。
//参数: Sets，被操作的MultiplexSets指针
//      Status，返回对象当前状态的指针，如果应用程序不关心其状态，可以给NULL。
//      Timeout，阻塞等待的最长时间，uS。
//返回：如果MultiplexSets被触发，则返回MultiplexSets中一个被触发对象的Fd，
//      在规定时间内无触发行为或者条件错误等都会返回-1.
//注意: 调用本函数后,Sets的触发状态并没有改变,只有Multiplex_Set会修改Sets的触发
//      状态,Multiplex的运行过程是:
//      1.应用程序调用Multiplex_Wait,取得ObjectID返回
//      2.根据ObjectID访问相应的对象.
//      3.被访问的对象内部,如果本次访问导致了Sending Bit 变化,调用Multiplex_Set
//          "告知"Multiplex组件.
//      4.Multiplex_Set函数修改Sets的状态.
//      5.反复1~4步,直到全部活动的对象访问完,然后调用 Multiplex_Wait 会进入阻塞状态.
//      后,如果不对被select的对象做读写
//-----------------------------------------------------------------------------
s32 Multiplex_Wait(struct MultiplexSetsCB *Sets, u32 *Status, u32 Timeout)
{
    struct MultiplexObjectCB *Object;
    s32 ret = -1;
    if (Sets == NULL)
    {
        return ret;
    }
//    if ((Sets->ActiveQ == NULL) && (Sets->ObjectQ == NULL))
//        return -1;
//     上述语句必须注释掉，原因如下：
//     1,当我们创建sets的时候，可能有些任务已经调用了等待，然而此时并没有任何的等待对象加入
//     2,当等待对象从1变为0时，容易在次数死循环（使用sets wait的程序一般使用while(1)）
//       判断或等待直到MultiplexSets被触发。

    //判断或等待直到MultiplexSets被触发。
    if (!Sets->SetsActived)
    {
        Lock_SempPend(&Sets->Lock, Timeout);
        //下列代码注释原因：如果已经有fd被触发，但还没达到ActiveLevel，这种情况也要在超时
        //之时返回fd
//      if(false ==Lock_SempPend(&Sets->Lock, Timeout))
//      {
//          return ret; //没有触发行为且原来是未触发状态
//      }
    }
    Int_SaveAsynSignal();
    if (Sets->ActiveQ != NULL)
    {   //阻塞等待结束后，Actived非空即视为触发。Lock_SempPend超时返回，此时
        //ActiveQ可能NULL（无对象被触发），也可能非空（有对象触发，但未达到ActiveLevel，
        //此时要人工设为已触发）
        Sets->SetsActived = true;
        Object = Sets->ActiveQ;
        ret = Object->Fd;
        if(Object->SensingBit & CN_MULTIPLEX_SENSINGBIT_ET)
        {
            //把Object从Sets->ActiveQ队列拿出，放到ObjectQ队列中
            //取出上次触发时未触发，而本次变位为已触发的位
//            Object->PendingBit &= CN_MULTIPLEX_STATUSMSK;
//            Object->ET_SaveBit = (~Object->ET_SaveBit) & Object->PendingBit;
            if (Status != NULL)
                *Status = Object->PendingBit;
            Object->PendingBit = 0;
            __Multiplex_ChangeList(&(Sets->ActiveQ), &(Sets->ObjectQ), Object);
            if (Sets->Actived != 0)
                Sets->Actived--;
            if (Sets->Actived == 0)
            {
                Sets->SetsActived = false;
                Lock_SempPend(&Sets->Lock, 0);  //清除掉已经post的信号
            }
        }
        else
        {
            Sets->SetsActived = true;
            if (Status != NULL)
                *Status = Object->PendingBit;
        }
    }
    Int_RestoreAsynSignal();

    return ret;
}

