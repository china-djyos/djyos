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
// 文件名     ：canbus.c
// 模块描述: 该文件将CANBUS挂到Djybus下，提供对CAN操作的统一标准接口。
// 模块版本: V1.00
// 创建人员: zhb
// 创建时间: 04/09.2018
// =============================================================================
#include "stddef.h"
#include "djybus.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "djyos.h"
#include "lock.h"
#include "canbus.h"
#include "object.h"
#include "msgqueue.h"
#include "ring.h"
#include "shell.h"
#include "dbug.h"
#include "systime.h"

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_CANBus(void);
//    ModuleInstall_CANBus ( );
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"can bus"//canbus
//parent:"djybus"    //填写该组件的父组件名字，none表示没有父组件
//attribute:system              //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
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
#if ( CFG_MODULE_ENABLE_CAN_BUS == false )
//#warning  " can_bus  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CAN_BUS    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,256,
#define CFG_CAN_BUS_MSGQ_NUM        200     //"消息队列长度",
#define CFG_CAN_BUS_TIMEOUT         200     //"总线超时(ms)",
#define CFG_CAN_BUS_RCV_RING_LEN    100     //"接收buffer消息数",
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

bool_t cbstat(char *param);

static struct Object *s_ptCANBusType;
static struct MsgQueue * gs_ptCanSndMsgQ;
static uint8_t CAN_BusSndStack[0x1000];
#define CN_CAN_BUS_MSGQ_LEN        18

#define CN_CAN_BUS_PRINTF_PREFIX     "          "

ptu32_t __CANBus_SndTask(void);
void __CAN_BusPrintfStat(struct CANBusCB * CANBus);
// =============================================================================
// 功能：将CAN总线类型结点挂接到DjyBus根结点。
// 参数：para,无实际意义。
// 返回：返回建立的资源结点指针，失败时返回NULL。
// =============================================================================
struct Object * ModuleInstall_CANBus()
{
    uint16_t evtt_id;
    struct Object *CANBusType = NULL;
    CANBusType = DjyBus_BusTypeAdd("CANBusType");
    if(CANBusType==NULL)
    {
        debug_printf("CANBUS","Type Added Failed!\r\n");
        return NULL;
    }
    //create a MsgQueen to save the Snd Frame.
    gs_ptCanSndMsgQ=MsgQ_Create(CFG_CAN_BUS_MSGQ_NUM,CN_CAN_BUS_MSGQ_LEN,CN_MSGQ_TYPE_FIFO);
    if(gs_ptCanSndMsgQ==NULL)
    {
        free(CANBusType);
        debug_printf("CANBUS","Type Added Failed!\r\n");
        return NULL;
    }
    //create a Task to SEND can frame asynchronously.
    evtt_id = DJY_EvttRegist(EN_CORRELATIVE,100,0,0,__CANBus_SndTask,
            CAN_BusSndStack,sizeof(CAN_BusSndStack),"CAN Bus Send Task");
    if(evtt_id!=CN_EVTT_ID_INVALID)
    {
        DJY_EventPop(evtt_id,NULL,0,0,0,0);
    }
    else
    {
        DJY_EvttUnregist(evtt_id);
        debug_printf("CANBUS","send task evtt pop failed.\r\n");
        free(CANBusType);
        debug_printf("CANBUS","Type Added Failed!\r\n");
        return NULL;
    }


    return CANBusType;
}

// =============================================================================
// 功能：添加CAN总线到CAN总线类型节点下，每个CAN控制器对应一条CAN总线。
// 参数：pNewCanDev,CAN Dev结构体指针。
// 返回：返回添加的CANBUS控制块结构体指针，失败时返回NULL。
// =============================================================================
struct CANBusCB *CAN_BusAdd(struct CanDev *pNewCanDev)
{
    struct CANBusCB *NewCAN;
    struct RingBuf *rxRingBuf;
    if(NULL == pNewCanDev)
        goto exit_from_param;
    //避免重复建立同名的CAN总线
    if(NULL != OBJ_SearchChild(s_ptCANBusType,pNewCanDev->ChipName))
        goto exit_from_readd;
    NewCAN = (struct CANBusCB *)M_Malloc(sizeof(struct CANBusCB),0);
    if(NewCAN == NULL)
        goto exit_from_malloc;
    //将总线结点挂接到总线类型结点的子结点
    //OBJ_AddChild(s_ptCANBusType,NULL,(ptu32_t)NewCAN,pNewCanDev->ChipName);
    NewCAN = OBJ_NewChild(s_ptCANBusType, (fnObjOps)-1, (ptu32_t)NewCAN,
                                    (const char*)(pNewCanDev->ChipName));
    if(&NewCAN->CAN_BusNode == NULL)
        goto exit_from_add_node;
    //创建总线信号量
    NewCAN->CAN_BusSemp= Lock_SempCreate(1,1,CN_BLOCK_FIFO,"CAN Bus semp");
    if(NewCAN->CAN_BusSemp== NULL)
        goto exit_from_can_bus_semp;
    rxRingBuf=(struct RingBuf *)malloc(sizeof(struct RingBuf));
    if(!rxRingBuf)
    {
        goto exit_from_can_bus_ring;
    }
    rxRingBuf->buf=(uint8_t *)malloc(CFG_CAN_BUS_RCV_RING_LEN*13);
    if(!rxRingBuf->buf)
    {
       goto exit_from_can_bus_ring_buf;
    }
    Ring_Init(rxRingBuf,rxRingBuf->buf,CFG_CAN_BUS_RCV_RING_LEN*13);
    NewCAN->RcvRing=rxRingBuf;
    //CAN_CB控制块初始化
    NewCAN->BusName=pNewCanDev->ChipName;
    NewCAN->BaudRate=pNewCanDev->BaudRate;
    NewCAN->CanCtrl=pNewCanDev->CanCtrl;
    NewCAN->PrivateTag=pNewCanDev->PrivateTag;
    NewCAN->CanWrite=pNewCanDev->CanWrite;
    memset(&NewCAN->CanStat,0,sizeof(CanStatDef));
    debug_printf("CANBUS","%s Added Succeeded!\r\n",pNewCanDev->ChipName);
    return NewCAN;


exit_from_can_bus_ring_buf:
    free(rxRingBuf);
exit_from_can_bus_ring:
    free(&NewCAN->CAN_BusSemp);
exit_from_can_bus_semp:
    OBJ_Delete(&NewCAN->CAN_BusNode);
exit_from_add_node:
    free(NewCAN);
exit_from_malloc:
exit_from_readd:
exit_from_param:
    return NULL;
}

// =============================================================================
// 功能：添加CAN总线到CAN总线类型节点下，每个CAN控制器对应一条CAN总线，CAN总线控制块
//       由调用方提供。
// 参数：pNewCanDev,CAN Dev结构体指针；
//       NewCAN,CANBUS控制块数据结构体。
// 返回：返回添加的CANBUS控制块结构体指针，失败时返回NULL。
// =============================================================================
struct CANBusCB * CAN_BusAdd_s(struct CanDev *pNewCanDev,\
                                   struct CANBusCB *NewCAN)
{
    struct RingBuf *rxRingBuf;
    if((NULL == NewCAN) || (NULL == pNewCanDev))
        goto exit_from_param;
    //避免重复建立同名的CAN总线
   if(NULL != OBJ_SearchChild(s_ptCANBusType,pNewCanDev->ChipName))
        goto exit_from_readd;
    //将总线结点挂接到总线类型结点的子结点上
    OBJ_AddChild(s_ptCANBusType,NULL,(ptu32_t)NewCAN,pNewCanDev->ChipName);
    if(&NewCAN->CAN_BusNode == NULL)
        goto exit_from_add_node;
    //创建总线信号量
    NewCAN->CAN_BusSemp= Lock_SempCreate(1,1,CN_BLOCK_FIFO,"CAN Bus semp");
    if(NewCAN->CAN_BusSemp== NULL)
        goto exit_from_can_bus_semp;

    rxRingBuf=(struct RingBuf *)malloc(sizeof(struct RingBuf));
    if(!rxRingBuf)
    {
        goto exit_from_can_bus_ring;
    }
    rxRingBuf->buf=(uint8_t *)malloc(CFG_CAN_BUS_RCV_RING_LEN*13);
    if(!rxRingBuf->buf)
    {
       goto exit_from_can_bus_ring_buf;
    }

    NewCAN->RcvRing=rxRingBuf;
    //CAN_CB控制块初始化
    NewCAN->BusName=pNewCanDev->ChipName;
    NewCAN->BaudRate=pNewCanDev->BaudRate;
    NewCAN->CanCtrl=pNewCanDev->CanCtrl;
    NewCAN->PrivateTag=pNewCanDev->PrivateTag;
    memset(&NewCAN->CanStat,0,sizeof(CanStatDef));
    NewCAN->CanWrite=pNewCanDev->CanWrite;
    debug_printf("CANBUS","%s Added Succeeded!\r\n",pNewCanDev->ChipName);
    return NewCAN;

exit_from_can_bus_ring_buf:
    free(rxRingBuf);
exit_from_can_bus_ring:
    free(&NewCAN->CAN_BusSemp);
exit_from_can_bus_semp:
      OBJ_Del(&NewCAN->CAN_BusNode);
exit_from_add_node:
exit_from_readd:
exit_from_param:
     return NULL;
}

// =============================================================================
// 功能：删除在CAN总线类型上的CANBUS指定结点。
// 参数：DelCAN,CANBUS控制块指针。
// 返回：true,删除成功;false,删除失败
// =============================================================================
bool_t CAN_BusDelete(struct CANBusCB *DelCAN)
{
    bool_t result;
    if(NULL == DelCAN)
         return false;
    if(OBJ_Delete(&DelCAN->CAN_BusNode))
        result = false;
    else
    {
        result = true;
        free(DelCAN);
    }
    return result;
}

// =============================================================================
// 功能：删除在CAN总线类型上的CANBUS指定结点。
// 参数：DelCAN,CANBUS控制块指针。
// 返回：true,删除成功;false,删除失败
// =============================================================================
bool_t CAN_BusDelete_s(struct CANBusCB *DelCAN)
{
    bool_t result;
    if(NULL == DelCAN)
        return false;
    if(OBJ_Delete(&DelCAN->CAN_BusNode))
        result = false;
    else
    {
        result = true;
    }
    return result;
}

// =============================================================================
// 功能：查找CAN总线类型结点的子结点中是否存被查找总线名称的CANBUS节点。
// 参数：BusName,待查找的CANBUS名称。
// 返回：成功找到则返回找到的CANBUS控制块结点指针，未找到时返回NULL。
// =============================================================================
struct CANBusCB* CAN_BusFind(char *BusName)
{
    if(BusName==NULL)
        return NULL;
    return (struct CANBusCB*)OBJ_SearchChild(s_ptCANBusType,BusName);
}


// =============================================================================
// 功能：CANBUS写一帧CAN报文函数，通过该函数，可将需要发送的一帧CAN报文通过指定CANBUS(指定的CAN控制器)发送到CAN总线上。
// 参数：CANBus,CANBUS控制块结构体指针;
//     Frame,CAN帧结构体指针;
// 返回：发送成功则返回true，若发送失败，则返回false.
// =============================================================================
bool_t CAN_BusSendFrame(struct CANBusCB * CANBus,CanFarmeDef *Frame,CanSendMode Mode)
{
    uint8_t i=0,len;
    bool_t Ret=false;
    uint32_t temp;
    uint8_t buf[CN_CAN_BUS_MSGQ_LEN];
    if((CANBus==NULL)||(Frame==NULL))
        return false;
    CANBus->CanStat.AppSndCnt++;
    //同步发送，则直接调用底层驱动钩子函数将CAN帧发送到CAN总线上.
    if(Mode==EN_CAN_BUS_SEND_SYN_MODE)
    {
         //需要等待总线空闲
         if(true == Lock_SempPend(CANBus->CAN_BusSemp,CFG_CAN_BUS_TIMEOUT*mS))
         {
            if(CANBus->CanWrite!=NULL)
                Ret=CANBus->CanWrite(CANBus,Frame);
            Lock_SempPost(CANBus->CAN_BusSemp);
            if(Ret)
            {
                CANBus->CanStat.HardSndCnt++;
            }
            else
            {
                CANBus->CanStat.SndPkgBadCnt++;
            }
            return Ret;
         }
         else
         {
             CANBus->CanStat.SendSempTimeoutCnt++;
             return false;
         }
    }

    //异步发送则将CAN报文填写到消息队列gs_ptCanSndMsgQ中.
    memset(buf,0,CN_CAN_BUS_MSGQ_LEN);
    temp=(uint32_t)CANBus;
    for(i=0;i<4;i++)
    {
        buf[i]=temp>>(8*i);
    }
    buf[4]=Frame->Type;
    len=Frame->DLC;
    buf[5]=len;
    temp=Frame->Id;
    for(i=0;i<4;i++)
    {
        buf[6+i]=temp>>(8*i);
    }
    memcpy(&buf[10],Frame->Data,len);
    Ret=MsgQ_Send(gs_ptCanSndMsgQ,buf,CN_CAN_BUS_MSGQ_LEN,0, CN_MSGQ_PRIO_NORMAL);
    if(!Ret)
    {
        CANBus->CanStat.LostSendFrameCnt++;
        debug_printf("CANBUS","CANBUS MsgQueen is full.\r\n");
        return false;
    }
    return true;

}

// =============================================================================
// 功能：CANBUS读函数，通过该函数，可读取指定CANBUS(指定CAN控制器)上接收的一帧CAN报文，底层CAN控制器接收到数据后会
//     存放在一个环形缓冲区中.
// 参数：CANBus,CANBUS控制块结构体指针;
//     Frame,CAN帧结构体指针;
// 返回：返回成功接收数据长度，若接收失败，则返回-1.
// =============================================================================
bool_t CAN_BusRecvFrame(struct CANBusCB * CANBus,CanFarmeDef *Frame)
{
    u32 ret=0;
    struct RingBuf *rxRing=NULL;
    uint8_t buf[13],i=0;
    uint32_t can_id=0;
    if((CANBus==NULL)||(Frame==NULL))
        return false;
    if(false == Lock_SempPend(CANBus->CAN_BusSemp,CFG_CAN_BUS_TIMEOUT*mS))    //需要等待总线空闲
    {
        CANBus->CanStat.RecvSempTimeoutCnt++;
        return false;
    }
    memset(buf,0,13);
    rxRing=CANBus->RcvRing;
    ret=Ring_Read(rxRing,buf,13);
    Lock_SempPost(CANBus->CAN_BusSemp);
    if(ret!=13)
        return false;
     Frame->Type=buf[0]&0xF0;

     Frame->DLC=buf[0]&0x0F;
     for(i=0;i<4;i++)
     {
         can_id|=(uint32_t)(buf[1+i]<<(8*i));
     }
     Frame->Id=can_id;
     for(i=0;i<Frame->DLC;i++)
     {
         Frame->Data[i]=buf[5+i];
     }
     CANBus->CanStat.AppRcvCnt++;
    return true;
}

// =============================================================================
// 功能：CANBUS控制函数，通过该函数可对指定CANBUS(指定CAN控制器)进行参数设置或其它操作，具体参见canbus.h中
//     CAN_CTRL_CMD定义。
// 参数：CANBus,CANBUS控制块结构体指针;
//     CtrlCmd,控制码，具体参见canbus.h中CanCtrlCmd定义;
//     param1,控制所需参数;
//     param2，控制所需参数.
// 返回：成功操作则返回true,失败则返回false.
// =============================================================================
bool_t CAN_BusCtrl(struct CANBusCB * CANBus,uint8_t CtrlCmd,ptu32_t param1,ptu32_t param2)
{
   bool_t ret=false;
   if(CANBus==NULL)
       return false;
   if(false == Lock_SempPend(CANBus->CAN_BusSemp,CFG_CAN_BUS_TIMEOUT*mS))    //需要等待总线空闲
        return false;
   if(CANBus->CanCtrl!=NULL)
       ret=CANBus->CanCtrl(CANBus,CtrlCmd,param1,param2);
   Lock_SempPost(CANBus->CAN_BusSemp);
   return ret;
}



//----CANBUS发送线程---------------------------------------------------------
//功能: 异步发送CAN数据，判断是否发送消息队列中是否有数据，如果有数据则将数据发送出去。
//参数: 无.
//返回: 无.
//-----------------------------------------------------------------------------
ptu32_t __CANBus_SndTask(void)
{
      struct CANBusCB * CANBus;
      uint8_t i;
      uint32_t id;
      uint32_t temp=0;
      uint8_t buf[CN_CAN_BUS_MSGQ_LEN];
      CanFarmeDef Frame;
      bool_t ret=false;
      while(1)
      {
         MsgQ_Receive(gs_ptCanSndMsgQ, buf, CN_CAN_BUS_MSGQ_LEN, CN_TIMEOUT_FOREVER);
         temp=0x0;
         memset(&Frame,0,sizeof(CanFarmeDef));
         for(i=0;i<4;i++)
         {
             temp|=(uint32_t)(buf[i]<<(8*i));
         }
         CANBus=(struct CANBusCB *)temp;
         Frame.Type=buf[4];
         Frame.DLC=buf[5];
         id=0x0;
         for(i=0;i<4;i++)
         {
             id|=(uint32_t)(buf[6+i]<<(8*i));
         }
         Frame.Id=id;
         for(i=0;i<Frame.DLC;i++)
         {
             Frame.Data[i]=buf[10+i];
         }
         //需要等待总线空闲
         if(true == Lock_SempPend(CANBus->CAN_BusSemp,CFG_CAN_BUS_TIMEOUT*mS))
         {
            if(CANBus->CanWrite!=NULL)
                ret=CANBus->CanWrite(CANBus,&Frame);
            Lock_SempPost(CANBus->CAN_BusSemp);
            if(ret)
            {
                CANBus->CanStat.HardSndCnt++;
            }
            else
            {
                CANBus->CanStat.SndPkgBadCnt++;
            }
         }
         else
         {
             CANBus->CanStat.SendSempTimeoutCnt++;
         }
      }
}

//----CANBUS发送线程------------------------------------------------------------
//功能: 将底层CAN控制器驱动接收到一帧CAN报文写进CANBUS的环形缓冲区中。
//参数: CANBus,CANBUS控制块结构体指针;
//     Frame,CAN帧结构体指针;
//返回: 无.
//-----------------------------------------------------------------------------
bool_t CAN_BusAddRcvFrame(struct CANBusCB * CANBus,CanFarmeDef *Frame)
{
    uint8_t i=0,buf[13],frame_len,tmp=0;
    uint32_t can_id,Len;
    if((CANBus==NULL)||(Frame==NULL))
        return false;
    struct RingBuf *rxRing=NULL;
    memset(buf,0,13);
    tmp=Frame->Type|Frame->DLC;
    buf[0]=tmp;
    frame_len=Frame->DLC;
    can_id=Frame->Id;
    for(i=0;i<4;i++)
    {
        buf[1+i]=can_id>>(8*i);
    }
    for(i=0;i<frame_len;i++)
    {
        buf[5+i]=Frame->Data[i];
    }
    rxRing=CANBus->RcvRing;
    Len=Ring_Write(rxRing,buf,13);
    if(Len!=13)
    {
        CANBus->CanStat.LostRecvFrameCnt++;
        return false;
    }
    else
    {
        CANBus->CanStat.HardRcvCnt++;
        return true;
    }
}

//----CAN错误报告------------------------------------------------------------
//功能: 底层驱动在检测到CAN错误后，调用该函数，该函数用于将该错误次数进行计数，便于统计及分析CANBUS通信状态.
//参数: CANBus,CANBUS控制块结构体指针;
//     ErrType,CAN总线错误类型，可参见canbus.h中CanErrType.
//返回: 无.
//-------------------------------------------------------------------------
void CAN_BusReportErr(struct CANBusCB * CANBus,CanErrType ErrType)
{
   if(CANBus==NULL)
       return;

   switch(ErrType)
   {
       case EN_CAN_BUS_BUS_OFF_ERR:
           CANBus->CanStat.CanErrStat.BusOffErrCnt++;
           break;
       case EN_CAN_BUS_CRC_ERR:
           CANBus->CanStat.CanErrStat.CrcErrCnt++;
           break;
       case EN_CAN_BUS_BIT_ERR:
           CANBus->CanStat.CanErrStat.BitErrCnt++;
           break;
       case EN_CAN_BUS_STUFF_ERR:
           CANBus->CanStat.CanErrStat.StuErrCnt++;
           break;
       case EN_CAN_BUS_FORMAT_ERR:
           CANBus->CanStat.CanErrStat.FormatErrCnt++;
           break;
       case EN_CAN_BUS_ACK_ERR:
           CANBus->CanStat.CanErrStat.ACKErrCnt++;
           break;
       case EN_CAN_BUS_LOSS_ARB_ERR:
           CANBus->CanStat.CanErrStat.LossArbCnt++;
           break;
       case EN_CAN_BUS_SEND_BAD:
           CANBus->CanStat.SndPkgBadCnt++;
           break;
       case EN_CAN_BUS_RST_FAILED:
           CANBus->CanStat.RstFailedCnt++;
           break;
       default:
           break;
   }
}

//----CANBUS上层获取CANBUS的统计数据---------------------------------------------
//功能: 该函数将指定CANBUS的统计数据交给CANBUS上层
//参数: CANBus,CANBUS控制块结构体指针;
//     Canstat，CANBUS统计结构体指针。
//返回: 无.
//-------------------------------------------------------------------------
void CAN_BusGetStat(struct CANBusCB * CANBus,CanStatDef *CanStat)
{
   if((CANBus==NULL)||(CanStat==NULL))
       return;
   memcpy(CanStat,&(CANBus->CanStat),sizeof(CanStatDef));
}


/*******************************************************************************
功能:遍历CANBUS总线类型下所有CANBUS总线，依次将各CANBUS统计信息在stdout输出.
参数:无。
输出:无。
*********************************************************************************/
//static void Sh_CAN_BusStat(char *param)

bool_t cbstat(char *param)
{
    struct Object *Object=NULL;
    struct CANBusCB * CANBus;
    Object=OBJ_ForeachChild(s_ptCANBusType,s_ptCANBusType);
    if(Object==NULL)
    {
        debug_printf("CANBUS","NO CANBUS Added.\r\n");
        return false;
    }
    do
    {
        CANBus=(struct CANBusCB *)Object;
        __CAN_BusPrintfStat(CANBus);
        Object=OBJ_ForeachChild(s_ptCANBusType,Object);
    }while(Object!=NULL);
   return true;
}

/*********************************************************************************
功能:CANBUS打印统计信息.
参数:CANBus,CANBUS控制块结构体指针;
输出:无。
*********************************************************************************/
void __CAN_BusPrintfStat(struct CANBusCB * CANBus)
{
    uint32_t data[2];//used to print the s64 type
    u64 AppSndCnt,HardSndCnt,HardRcvCnt,AppRcvCnt;
    u64 BusOffCnt,CrcErrCnt,BitErrCnt,FoErrCnt,StErrCnt,ACKErrCnt;
    u64 SndTimeoutCnt,RcvTimeoutCnt,LossSndCnt,LossRcvCnt,SndBadCnt,RstFailedCnt;
    if(CANBus==NULL)
        return;
    AppSndCnt=CANBus->CanStat.AppSndCnt;
    HardSndCnt=CANBus->CanStat.HardSndCnt;
    AppRcvCnt=CANBus->CanStat.AppRcvCnt;
    HardRcvCnt=CANBus->CanStat.HardRcvCnt;
    BusOffCnt=CANBus->CanStat.CanErrStat.BusOffErrCnt;
    CrcErrCnt=CANBus->CanStat.CanErrStat.CrcErrCnt;
    BitErrCnt=CANBus->CanStat.CanErrStat.BitErrCnt;
    FoErrCnt=CANBus->CanStat.CanErrStat.FormatErrCnt;
    StErrCnt=CANBus->CanStat.CanErrStat.StuErrCnt;
    ACKErrCnt=CANBus->CanStat.CanErrStat.ACKErrCnt;
    SndTimeoutCnt=CANBus->CanStat.SendSempTimeoutCnt;
    RcvTimeoutCnt=CANBus->CanStat.RecvSempTimeoutCnt;
    LossSndCnt=CANBus->CanStat.LostSendFrameCnt;
    LossRcvCnt=CANBus->CanStat.LostRecvFrameCnt;
    SndBadCnt=CANBus->CanStat.SndPkgBadCnt;
    RstFailedCnt=CANBus->CanStat.RstFailedCnt;

    debug_printf("CANBUS","CANBUS Name:%s,CANBUS BaudRate:%d.\r\n",CANBus->BusName,CANBus->BaudRate);
    memcpy(data,&AppSndCnt,sizeof(data));
    debug_printf("CANBUS","%s APP Snd Cnt          :0x%08x 0x%08x \r\n",CN_CAN_BUS_PRINTF_PREFIX,data[1],data[0]);
    memcpy(data,&HardSndCnt,sizeof(data));
    debug_printf("CANBUS","%s Hard Snd Cnt         :0x%08x 0x%08x \r\n",CN_CAN_BUS_PRINTF_PREFIX,data[1],data[0]);
    memcpy(data,&HardRcvCnt,sizeof(data));
    debug_printf("CANBUS","%s Hard Rcv Cnt         :0x%08x 0x%08x \r\n",CN_CAN_BUS_PRINTF_PREFIX,data[1],data[0]);
    memcpy(data,&AppRcvCnt,sizeof(data));
    debug_printf("CANBUS","%s APP Rcv Cnt          :0x%08x 0x%08x \r\n",CN_CAN_BUS_PRINTF_PREFIX,data[1],data[0]);

    memcpy(data,&SndTimeoutCnt,sizeof(data));
    debug_printf("CANBUS","%s Send Timeout Cnt     :0x%08x 0x%08x \r\n",CN_CAN_BUS_PRINTF_PREFIX,data[1],data[0]);
    memcpy(data,&RcvTimeoutCnt,sizeof(data));
    debug_printf("CANBUS","%s Recv Timeout Cnt     :0x%08x 0x%08x \r\n",CN_CAN_BUS_PRINTF_PREFIX,data[1],data[0]);
    memcpy(data,&LossSndCnt,sizeof(data));
    debug_printf("CANBUS","%s Loss Send Cnt        :0x%08x 0x%08x \r\n",CN_CAN_BUS_PRINTF_PREFIX,data[1],data[0]);
    memcpy(data,&LossRcvCnt,sizeof(data));
    debug_printf("CANBUS","%s Loss Recv Cnt        :0x%08x 0x%08x \r\n",CN_CAN_BUS_PRINTF_PREFIX,data[1],data[0]);

    memcpy(data,&SndBadCnt,sizeof(data));
    debug_printf("CANBUS","%s Snd Bad Cnt          :0x%08x 0x%08x \r\n",CN_CAN_BUS_PRINTF_PREFIX,data[1],data[0]);
    memcpy(data,&RstFailedCnt,sizeof(data));
    debug_printf("CANBUS","%s Rst failed Cnt       :0x%08x 0x%08x \r\n",CN_CAN_BUS_PRINTF_PREFIX,data[1],data[0]);

    memcpy(data,&BusOffCnt,sizeof(data));
    debug_printf("CANBUS","%s Bus Off Cnt          :0x%08x 0x%08x \r\n",CN_CAN_BUS_PRINTF_PREFIX,data[1],data[0]);
    memcpy(data,&CrcErrCnt,sizeof(data));
    debug_printf("CANBUS","%s CRC Err Cnt          :0x%08x 0x%08x \r\n",CN_CAN_BUS_PRINTF_PREFIX,data[1],data[0]);
    memcpy(data,&BitErrCnt,sizeof(data));
    debug_printf("CANBUS","%s Bit Err Cnt          :0x%08x 0x%08x \r\n",CN_CAN_BUS_PRINTF_PREFIX,data[1],data[0]);
    memcpy(data,&FoErrCnt,sizeof(data));
    debug_printf("CANBUS","%s Format Err Cnt       :0x%08x 0x%08x \r\n",CN_CAN_BUS_PRINTF_PREFIX,data[1],data[0]);
    memcpy(data,&StErrCnt,sizeof(data));
    debug_printf("CANBUS","%s Stuff Err Cnt        :0x%08x 0x%08x \r\n",CN_CAN_BUS_PRINTF_PREFIX,data[1],data[0]);
    memcpy(data,&ACKErrCnt,sizeof(data));
    debug_printf("CANBUS","%s ACK Err Cnt          :0x%08x 0x%08x \r\n",CN_CAN_BUS_PRINTF_PREFIX,data[1],data[0]);

}
ADD_TO_ROUTINE_SHELL(cbstat,cbstat,"COMMAND:canrst+CAN控制器编号+enter");
