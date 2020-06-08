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
// 这份授权条款，在使用者符合以下三条件的情形下，授予使用者使用及再散播本
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
//所属模块: 消息队列模块
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 提供消息队列管理功能。djyos的消息队列模块与常见的RTOS的实现有一些不
//          同。删除消息队列时，如果有事件阻塞在该消息队列中，常见RTOS的做法，是
//          唤醒所有阻塞的线程；而djyos的做法，是不允许删除。djyos更接近POSIX的
//          要求。
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2014-07-25
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "stdint.h"
#include "stdlib.h"
#include "stddef.h"
#include "string.h"
#include "systime.h"
#include "int.h"
#include "lock.h"
#include <djyos.h>
#include "msgqueue.h"
#include "dbug.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_MsgQ(void);
//    ModuleInstall_MsgQ ( );
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"message queue"//消息队列
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
#if ( CFG_MODULE_ENABLE_MESSAGE_QUEUE == false )
//#warning  " message_queue  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_MESSAGE_QUEUE    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100
//%$#@enum,true,false
//%$#@string,1,10
//%$#@select
//%$#@free
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure


static struct Object *s_ptMsgQ_Dir;
static struct MutexLCB s_tMsgQ_Mutex;

//----初始化-------------------------------------------------------------------
//功能: 初始化消息队列组件，添加消息队列根资源结点，初始化互斥量
//参数: para，无意义
//返回: true = 正确，false = 出错
//-----------------------------------------------------------------------------
bool_t ModuleInstall_MsgQ (void)
{
    s_ptMsgQ_Dir = OBJ_NewChild(OBJ_GetRoot(), (fnObjOps)-1, 0, "message queue");
    if(!s_ptMsgQ_Dir)
    {
        error_printf("module","message queue");
        return (FALSE);
    }

    Lock_MutexCreate_s(&s_tMsgQ_Mutex, "消息队列");
    return (TRUE);
}

//----创建一个消息队列---------------------------------------------------------
//功能: 创建消息队列，并初始化相关参数。消息队列控制块和保存消息的内存，均是动态
//      分配的，函数失败(返回NULL)的原因，只可能是内存不足。
//参数: MaxMsgs，队列的消息容量。
//      MsgLength，单条消息最大尺寸。
//      Options，选项，CN_MSGQ_TYPE_FIFO or CN_MSGQ_TYPE_PRIO，决定接收消息的事
//          件被阻塞时的排队方式。
//返回: 新创建的消息队列指针，失败则返回NULL.
//-----------------------------------------------------------------------------
struct MsgQueue *MsgQ_Create( s32 MaxMsgs,u32  MsgLength,u32 Options)
{
    struct MsgQueue *MQ;
    struct Object *MQ_Obj;
    //分配内存，同时分配消息队列控制块和存储消息的内存。
    MQ = M_Malloc(sizeof(struct MsgQueue)+MsgLength*MaxMsgs,0);
    if(MQ != NULL)
    {
        MQ_Obj = OBJ_NewChild(s_ptMsgQ_Dir, (fnObjOps)-1, (ptu32_t)MQ, NULL);
        if(MQ_Obj)
        {
            MQ->HostObj = MQ_Obj;
            if( (Options & CN_MSGQ_TYPE_MASK) == CN_MSGQ_TYPE_PRIO)
            {
                Lock_SempCreate_s(&(MQ->MsgSendSemp),MaxMsgs,MaxMsgs,
                                    CN_BLOCK_PRIO,"消息队列");
                Lock_SempCreate_s(&(MQ->MsgRecvSemp),MaxMsgs,0,
                                    CN_BLOCK_PRIO,"消息队列");
            }
            else
            {
                Lock_SempCreate_s(&(MQ->MsgSendSemp),MaxMsgs,MaxMsgs,
                                    CN_BLOCK_FIFO,"消息队列");
                Lock_SempCreate_s(&(MQ->MsgRecvSemp),MaxMsgs,0,
                                    CN_BLOCK_FIFO,"消息队列");
            }
            MQ->MsgSize = MsgLength;
            MQ->MsgUsed = 0;
            MQ->ReadOffset = 0;
            MQ->buf = (u8*)(MQ + 1);
        }
        else
        {
            free(MQ);
            MQ = NULL;
            error_printf("msgque","create failed(add to obj).");
        }
    }
    else
    {
            error_printf("msgque","create failed(no space).");
    }

    return MQ;
}

//----删除消息队列-------------------------------------------------------------
//功能: 删除消息队列，须特别注意的是，如果有事件阻塞在队列中，删除将失败，而不是
//      激活队列中的事件。这跟常见的RTOS有区别，与POSIX标准一致。
//      POSIX的规定是合理的，队列作为公共设施，应该只有在空闲时才可以拆除。
//参数: pMsgQ，被删除的消息队列。
//返回: true=成功删除，false=失败
//-----------------------------------------------------------------------------
bool_t MsgQ_Delete(struct MsgQueue *pMsgQ)
{
    bool_t result;
    Lock_MutexPend(&s_tMsgQ_Mutex,CN_TIMEOUT_FOREVER);
    if(pMsgQ != NULL)
    {
        if(Lock_SempCheckBlock(&(pMsgQ->MsgSendSemp))
                || Lock_SempCheckBlock(&(pMsgQ->MsgRecvSemp)) )
        {
            result = false;
        }
        else
        {
            OBJ_Delete(pMsgQ->HostObj);          //删除信号量结点
            Lock_SempDelete_s(&(pMsgQ->MsgSendSemp));
            Lock_SempDelete_s(&(pMsgQ->MsgRecvSemp));
            free(pMsgQ);
            result = true;
        }
    }
    else
    {
        result = false;
    }
    Lock_MutexPost(&s_tMsgQ_Mutex);
    return result;
}


//----发送消息-----------------------------------------------------------------
//功能: 发送一条消息到消息队列
//参数: pMsgQ，目标消息队列
//      buffer，新消息缓冲区
//      nBytes，新消息的长度
//      Timeout，超时时间，us，如果消息队列非空，将阻塞，直到超时
//      priority，消息优先级，取值为CN_MSGQ_PRIO_NORMAL或CN_MSGQ_PRIO_URGENT，
//          决定新消息排在队列头还是尾部
//返回: true=成功发送，false=失败
//-----------------------------------------------------------------------------
bool_t MsgQ_Send(struct MsgQueue *pMsgQ,u8 *buffer,u32 nBytes,u32 Timeout,
                 bool_t priority)
{
    u32 WriteOffset,sum,used;
    atom_low_t atom_low;
    if((pMsgQ == NULL) || (buffer == NULL))
        return false;
    if(nBytes > pMsgQ->MsgSize)
        return false;
    // 是否存在被操作的队列被别人删除的问题呢? 理论上是有的,分以下几种情况:
    // 1.别人误操作,但别人得不到你的MsgQ指针,根本操作不到你.
    // 2.恶意破坏操作,你根本防不住.
    // 3.自己误操作
    //
    // 如果消息队列被删除,本地指针非空,但指向的消息队列却已经释放,此时访问这个指
    // 针,确实很危险,一般是用信号量或者互斥量来保护,但同样存在用来保护的信号量被
    // 删除的可能,是个无解过程.如果使用原子操作保护,保护一个会被阻塞的过程,要付出
    // 很大的代价,且还只能保护自己的小模块内的误操作,是不值得的.
    if( Lock_SempPend(&(pMsgQ->MsgSendSemp),Timeout) )  //看消息队列还有空位没
    {
        //取当前队列中的消息容量
        sum = Lock_SempQueryCapacital(&(pMsgQ->MsgSendSemp));
        atom_low = Int_LowAtomStart();            //进入原子操作状态
        used = pMsgQ->MsgUsed++;
        //以下if语句取新消息的保存位置
        if(priority == CN_MSGQ_PRIO_URGENT)
        {
            if(pMsgQ->ReadOffset== 0)
                WriteOffset = sum - 1;
            else
                WriteOffset = pMsgQ->ReadOffset-1;
            pMsgQ->ReadOffset = WriteOffset;
        }
        else
        {
            WriteOffset = used + pMsgQ->ReadOffset;
            if(WriteOffset >= sum)
                WriteOffset = WriteOffset - sum;
        }
        WriteOffset *= pMsgQ->MsgSize;
        memcpy(pMsgQ->buf + WriteOffset,buffer,nBytes);
        Int_LowAtomEnd(atom_low);
        Lock_SempPost(&(pMsgQ->MsgRecvSemp));
        return true;
    }
    else
        return false;
}

//----接收消息-----------------------------------------------------------------
//功能: 从消息队列中接收一条消息
//参数: pMsgQ，目标消息队列
//      buffer，接收消息的缓冲区
//      nBytes，消息的长度，不能超过消息队列的消息长度MsgSize
//      Timeout，超时时间，us，如果消息队列空，将阻塞，直到超时
//返回: true=成功发送，false=失败
//-----------------------------------------------------------------------------
bool_t MsgQ_Receive(struct MsgQueue *pMsgQ,u8 *buffer,u32 nBytes,u32 Timeout)
{
    u32 sum;
    atom_low_t atom_low;
    if((pMsgQ == NULL) || (buffer == NULL))
        return false;
    if(nBytes > pMsgQ->MsgSize)
        return false;
    if( Lock_SempPend(&(pMsgQ->MsgRecvSemp),Timeout) )
    {
        atom_low = Int_LowAtomStart();
        sum = Lock_SempQueryCapacital(&(pMsgQ->MsgSendSemp));
        memcpy(buffer,pMsgQ->buf+pMsgQ->ReadOffset * pMsgQ->MsgSize,nBytes);
        pMsgQ->MsgUsed--;
        if(pMsgQ->ReadOffset >= sum -1)
            pMsgQ->ReadOffset = 0;
        else
            pMsgQ->ReadOffset++;
        Int_LowAtomEnd(atom_low);
        Lock_SempPost(&(pMsgQ->MsgSendSemp));
        return true;
    }
    else
        return false;
}

//----取消息数-----------------------------------------------------------------
//功能: 取消息队列中的消息数
//参数: pMsgQ，目标消息队列
//返回: 消息数。
//-----------------------------------------------------------------------------
u32 MsgQ_NumMsgs(struct MsgQueue *pMsgQ)
{
    if(pMsgQ == NULL)
        return 0;
    else
        return pMsgQ->MsgUsed;
}

