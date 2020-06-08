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
// �ļ���     ��canbus.c
// ģ������: ���ļ���CANBUS�ҵ�Djybus�£��ṩ��CAN������ͳһ��׼�ӿڡ�
// ģ��汾: V1.00
// ������Ա: zhb
// ����ʱ��: 04/09.2018
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

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_CANBus(void);
//    ModuleInstall_CANBus ( );
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"can bus"//canbus
//parent:"djybus"    //��д������ĸ�������֣�none��ʾû�и����
//attribute:system              //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"djybus"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CAN_BUS == false )
//#warning  " can_bus  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CAN_BUS    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,256,
#define CFG_CAN_BUS_MSGQ_NUM        200     //"��Ϣ���г���",
#define CFG_CAN_BUS_TIMEOUT         200     //"���߳�ʱ(ms)",
#define CFG_CAN_BUS_RCV_RING_LEN    100     //"����buffer��Ϣ��",
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
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
// ���ܣ���CAN�������ͽ��ҽӵ�DjyBus����㡣
// ������para,��ʵ�����塣
// ���أ����ؽ�������Դ���ָ�룬ʧ��ʱ����NULL��
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
// ���ܣ����CAN���ߵ�CAN�������ͽڵ��£�ÿ��CAN��������Ӧһ��CAN���ߡ�
// ������pNewCanDev,CAN Dev�ṹ��ָ�롣
// ���أ�������ӵ�CANBUS���ƿ�ṹ��ָ�룬ʧ��ʱ����NULL��
// =============================================================================
struct CANBusCB *CAN_BusAdd(struct CanDev *pNewCanDev)
{
    struct CANBusCB *NewCAN;
    struct RingBuf *rxRingBuf;
    if(NULL == pNewCanDev)
        goto exit_from_param;
    //�����ظ�����ͬ����CAN����
    if(NULL != OBJ_SearchChild(s_ptCANBusType,pNewCanDev->ChipName))
        goto exit_from_readd;
    NewCAN = (struct CANBusCB *)M_Malloc(sizeof(struct CANBusCB),0);
    if(NewCAN == NULL)
        goto exit_from_malloc;
    //�����߽��ҽӵ��������ͽ����ӽ��
    //OBJ_AddChild(s_ptCANBusType,NULL,(ptu32_t)NewCAN,pNewCanDev->ChipName);
    NewCAN = OBJ_NewChild(s_ptCANBusType, (fnObjOps)-1, (ptu32_t)NewCAN,
                                    (const char*)(pNewCanDev->ChipName));
    if(&NewCAN->CAN_BusNode == NULL)
        goto exit_from_add_node;
    //���������ź���
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
    //CAN_CB���ƿ��ʼ��
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
// ���ܣ����CAN���ߵ�CAN�������ͽڵ��£�ÿ��CAN��������Ӧһ��CAN���ߣ�CAN���߿��ƿ�
//       �ɵ��÷��ṩ��
// ������pNewCanDev,CAN Dev�ṹ��ָ�룻
//       NewCAN,CANBUS���ƿ����ݽṹ�塣
// ���أ�������ӵ�CANBUS���ƿ�ṹ��ָ�룬ʧ��ʱ����NULL��
// =============================================================================
struct CANBusCB * CAN_BusAdd_s(struct CanDev *pNewCanDev,\
                                   struct CANBusCB *NewCAN)
{
    struct RingBuf *rxRingBuf;
    if((NULL == NewCAN) || (NULL == pNewCanDev))
        goto exit_from_param;
    //�����ظ�����ͬ����CAN����
   if(NULL != OBJ_SearchChild(s_ptCANBusType,pNewCanDev->ChipName))
        goto exit_from_readd;
    //�����߽��ҽӵ��������ͽ����ӽ����
    OBJ_AddChild(s_ptCANBusType,NULL,(ptu32_t)NewCAN,pNewCanDev->ChipName);
    if(&NewCAN->CAN_BusNode == NULL)
        goto exit_from_add_node;
    //���������ź���
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
    //CAN_CB���ƿ��ʼ��
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
// ���ܣ�ɾ����CAN���������ϵ�CANBUSָ����㡣
// ������DelCAN,CANBUS���ƿ�ָ�롣
// ���أ�true,ɾ���ɹ�;false,ɾ��ʧ��
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
// ���ܣ�ɾ����CAN���������ϵ�CANBUSָ����㡣
// ������DelCAN,CANBUS���ƿ�ָ�롣
// ���أ�true,ɾ���ɹ�;false,ɾ��ʧ��
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
// ���ܣ�����CAN�������ͽ����ӽ�����Ƿ�汻�����������Ƶ�CANBUS�ڵ㡣
// ������BusName,�����ҵ�CANBUS���ơ�
// ���أ��ɹ��ҵ��򷵻��ҵ���CANBUS���ƿ���ָ�룬δ�ҵ�ʱ����NULL��
// =============================================================================
struct CANBusCB* CAN_BusFind(char *BusName)
{
    if(BusName==NULL)
        return NULL;
    return (struct CANBusCB*)OBJ_SearchChild(s_ptCANBusType,BusName);
}


// =============================================================================
// ���ܣ�CANBUSдһ֡CAN���ĺ�����ͨ���ú������ɽ���Ҫ���͵�һ֡CAN����ͨ��ָ��CANBUS(ָ����CAN������)���͵�CAN�����ϡ�
// ������CANBus,CANBUS���ƿ�ṹ��ָ��;
//     Frame,CAN֡�ṹ��ָ��;
// ���أ����ͳɹ��򷵻�true��������ʧ�ܣ��򷵻�false.
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
    //ͬ�����ͣ���ֱ�ӵ��õײ��������Ӻ�����CAN֡���͵�CAN������.
    if(Mode==EN_CAN_BUS_SEND_SYN_MODE)
    {
         //��Ҫ�ȴ����߿���
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

    //�첽������CAN������д����Ϣ����gs_ptCanSndMsgQ��.
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
// ���ܣ�CANBUS��������ͨ���ú������ɶ�ȡָ��CANBUS(ָ��CAN������)�Ͻ��յ�һ֡CAN���ģ��ײ�CAN���������յ����ݺ��
//     �����һ�����λ�������.
// ������CANBus,CANBUS���ƿ�ṹ��ָ��;
//     Frame,CAN֡�ṹ��ָ��;
// ���أ����سɹ��������ݳ��ȣ�������ʧ�ܣ��򷵻�-1.
// =============================================================================
bool_t CAN_BusRecvFrame(struct CANBusCB * CANBus,CanFarmeDef *Frame)
{
    u32 ret=0;
    struct RingBuf *rxRing=NULL;
    uint8_t buf[13],i=0;
    uint32_t can_id=0;
    if((CANBus==NULL)||(Frame==NULL))
        return false;
    if(false == Lock_SempPend(CANBus->CAN_BusSemp,CFG_CAN_BUS_TIMEOUT*mS))    //��Ҫ�ȴ����߿���
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
// ���ܣ�CANBUS���ƺ�����ͨ���ú����ɶ�ָ��CANBUS(ָ��CAN������)���в������û���������������μ�canbus.h��
//     CAN_CTRL_CMD���塣
// ������CANBus,CANBUS���ƿ�ṹ��ָ��;
//     CtrlCmd,�����룬����μ�canbus.h��CanCtrlCmd����;
//     param1,�����������;
//     param2�������������.
// ���أ��ɹ������򷵻�true,ʧ���򷵻�false.
// =============================================================================
bool_t CAN_BusCtrl(struct CANBusCB * CANBus,uint8_t CtrlCmd,ptu32_t param1,ptu32_t param2)
{
   bool_t ret=false;
   if(CANBus==NULL)
       return false;
   if(false == Lock_SempPend(CANBus->CAN_BusSemp,CFG_CAN_BUS_TIMEOUT*mS))    //��Ҫ�ȴ����߿���
        return false;
   if(CANBus->CanCtrl!=NULL)
       ret=CANBus->CanCtrl(CANBus,CtrlCmd,param1,param2);
   Lock_SempPost(CANBus->CAN_BusSemp);
   return ret;
}



//----CANBUS�����߳�---------------------------------------------------------
//����: �첽����CAN���ݣ��ж��Ƿ�����Ϣ�������Ƿ������ݣ���������������ݷ��ͳ�ȥ��
//����: ��.
//����: ��.
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
         //��Ҫ�ȴ����߿���
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

//----CANBUS�����߳�------------------------------------------------------------
//����: ���ײ�CAN�������������յ�һ֡CAN����д��CANBUS�Ļ��λ������С�
//����: CANBus,CANBUS���ƿ�ṹ��ָ��;
//     Frame,CAN֡�ṹ��ָ��;
//����: ��.
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

//----CAN���󱨸�------------------------------------------------------------
//����: �ײ������ڼ�⵽CAN����󣬵��øú������ú������ڽ��ô���������м���������ͳ�Ƽ�����CANBUSͨ��״̬.
//����: CANBus,CANBUS���ƿ�ṹ��ָ��;
//     ErrType,CAN���ߴ������ͣ��ɲμ�canbus.h��CanErrType.
//����: ��.
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

//----CANBUS�ϲ��ȡCANBUS��ͳ������---------------------------------------------
//����: �ú�����ָ��CANBUS��ͳ�����ݽ���CANBUS�ϲ�
//����: CANBus,CANBUS���ƿ�ṹ��ָ��;
//     Canstat��CANBUSͳ�ƽṹ��ָ�롣
//����: ��.
//-------------------------------------------------------------------------
void CAN_BusGetStat(struct CANBusCB * CANBus,CanStatDef *CanStat)
{
   if((CANBus==NULL)||(CanStat==NULL))
       return;
   memcpy(CanStat,&(CANBus->CanStat),sizeof(CanStatDef));
}


/*******************************************************************************
����:����CANBUS��������������CANBUS���ߣ����ν���CANBUSͳ����Ϣ��stdout���.
����:�ޡ�
���:�ޡ�
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
����:CANBUS��ӡͳ����Ϣ.
����:CANBus,CANBUS���ƿ�ṹ��ָ��;
���:�ޡ�
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
ADD_TO_ROUTINE_SHELL(cbstat,cbstat,"COMMAND:canrst+CAN���������+enter");
