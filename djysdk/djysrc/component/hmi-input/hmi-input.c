//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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
//����ģ�飺��׼IO�豸����
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ��ꡢ���̵������豸����
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2011-01-24
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: �����ļ�
//------------------------------------------------------

#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "object.h"
#include "pool.h"
#include "hmi-input.h"
#include "msgqueue.h"
#include "djyos.h"
#include "core_config.h"

//����δ���ý���������豸��Ĭ���Դ�Ϊ����
tpInputMsgQ tg_pDefaultFocusMsgQ = NULL;
struct HMI_InputDeviceRsc *g_ptStdinDevice;
struct MemCellPool *g_ptHmiInDevicePool;
s32 g_s32NextId = 0;       //ÿ�ΰ�װ�����豸ʱ���Ա�����Ϊ�豸ID��Ȼ�󱾱���++
                            //ɾ���豸��ID���ջء�
static struct HMI_InputDeviceRsc *s_ptHmiInDeviceRscTree;
tpInputMsgQ tg_pHmiInputMsgQ;    //  ��׼�����豸����Ϣ����

//----��ʼ����׼����ģ��-------------------------------------------------------
//����: ��ʼ����׼����ģ�飬ִ�г�ʼ���󣬿��Դ��������豸��
//����: ��
//����: true = �ɹ�,false= ʧ��.
//-----------------------------------------------------------------------------
bool_t ModuleInstall_HmiIn(void)
{
    static struct HMI_InputDeviceRsc root;

    tg_pHmiInputMsgQ = HmiIn_CreatInputMsgQ(10,"StdInDev");
    if(tg_pHmiInputMsgQ == NULL)
        return false;
    g_ptStdinDevice = M_Malloc(gc_u32CfgStdinDeviceLimit
                                * sizeof(struct HMI_InputDeviceRsc),0);
    if(g_ptStdinDevice == NULL)
    {
        HmiIn_DeleteInputMsgQ(tg_pHmiInputMsgQ);
        return false;
    }
    s_ptHmiInDeviceRscTree = (struct HMI_InputDeviceRsc *)
                        OBJ_AddTree(&root.stdin_device_node,
                        sizeof(struct HMI_InputDeviceRsc),RSC_STDIN_OUT,"stdin input device");
    //��ʼ���豸���ƿ��ڴ��
    g_ptHmiInDevicePool = Mb_CreatePool((void*)g_ptStdinDevice,
                                    gc_u32CfgStdinDeviceLimit,
                                    sizeof(struct HMI_InputDeviceRsc),
                                    2,10,
                                    "�����豸���ƿ��ڴ��");
    HmiIn_SetFocusDefault(tg_pHmiInputMsgQ);
    return true;
}

//----��װ�����豸-------------------------------------------------------------
//����: �������豸�Ǽǵ���Դ�����
//����: device_name��Ŀ���豸����Դ��
//����: �����豸ID,-1��ʾʧ��
//-----------------------------------------------------------------------------
s32 HmiIn_InstallDevice(const char *device_name,enum _STDIN_INPUT_TYPE_ stdin_type,
                         void *myprivate)
{
    struct HMI_InputDeviceRsc *Djy_HmiIn;

    if(OBJ_SearchChild(&s_ptHmiInDeviceRscTree->stdin_device_node,device_name))
    {
        return -1;
    }
    else
    {
        Djy_HmiIn = Mb_Malloc(g_ptHmiInDevicePool,0);
        if(Djy_HmiIn != NULL)
        {
            OBJ_AddChild(&s_ptHmiInDeviceRscTree->stdin_device_node,
                    &Djy_HmiIn->stdin_device_node,sizeof(struct HMI_InputDeviceRsc),
                    RSC_STDIN_OUT,device_name);
            Djy_HmiIn->input_type = stdin_type;
            Djy_HmiIn->stdin_private = (ptu32_t)myprivate;
            Djy_HmiIn->input_counter = 0;
            Djy_HmiIn->FocusMsgQ = tg_pDefaultFocusMsgQ;
            g_s32NextId += 1;
            Djy_HmiIn->device_id = g_s32NextId;
            return Djy_HmiIn->device_id;
        }
        else
        {
            Djy_SaveLastError(EN_MEM_TRIED);
            printf("�ڴ治��\n\r");
            return -1;
        }

    }
}

//----����������Ϣ����--------------------------------------------------------
//����: ����������Ϣ����, Ӧ�ó���Ӷ����н��������¼���Ϣ.
//����: MsgNum,��Ϣ�����п����ɵ�������Ϣ����
//      Name, ������Ϣ���е�����
//����: �´�����������Ϣ����ָ��
//----------------------------------------------------------------------------
tpInputMsgQ HmiIn_CreatInputMsgQ(u32 MsgNum,const char *Name)
{
    return MsgQ_Create(MsgNum,sizeof(struct InputDeviceMsg),CN_MSGQ_TYPE_FIFO);
}


//----����������Ϣ����--------------------------------------------------------
//����: ����������Ϣ����, Ӧ�ó�������Ҫ������Ϣʱ,ɾ���������.
//      ɾ������Ҳ����ն��������е���Ϣ.
//����: MsgNum,��Ϣ�����п����ɵ�������Ϣ����
//      Name, ������Ϣ���е�����
//����: �´�����������Ϣ����ָ��
//----------------------------------------------------------------------------
bool_t HmiIn_DeleteInputMsgQ(tpInputMsgQ InputMsgQ)
{
    return MsgQ_Delete(InputMsgQ);
}

//----�������뽹��------------------------------------------------------------
//����: ���������豸�����뽹�㣬����������Ϣ(������̻����������)�����ĸ�����
//      ��Ϣ����
//����: device_name��Ŀ���豸����Դ��
//      FocusMsgQ���µĽ���
//����: true =�ɹ���false=�����豸�Ҳ���
//----------------------------------------------------------------------------
bool_t HmiIn_SetFocus(const char *device_name, tpInputMsgQ FocusMsgQ)
{
    struct HMI_InputDeviceRsc *result;

    result = (struct HMI_InputDeviceRsc *)OBJ_SearchChild
                    (&s_ptHmiInDeviceRscTree->stdin_device_node,device_name);
    if(result != NULL)
    {
        result->FocusMsgQ = FocusMsgQ;
        return true;
    }
    else
    {
        return false;
    }
}

//----��ѯ�豸����------------------------------------------------------------
//����: ��ѯ��Ϊname���豸�����͡�
//����: device_name��Ŀ���豸����Դ��
//����: �豸������
//----------------------------------------------------------------------------
enum _STDIN_INPUT_TYPE_ HmiIn_CheckDevType(const char *device_name)
{
    struct HMI_InputDeviceRsc *InputDev;

    InputDev = (struct HMI_InputDeviceRsc *)OBJ_SearchChild
                    (&s_ptHmiInDeviceRscTree->stdin_device_node,device_name);
    if(InputDev != NULL)
    {
        return InputDev->input_type;
    }
    else
    {
        return EN_HMIIN_INVALID;
    }
}

//----����Ĭ�����뽹��---------------------------------------------------------
//����: ���������豸�����н��㣬���豸����������ʱ�����ĸ��¼����ͷ���Ϣ��δ�趨
//      ������豸��ʹ��Ĭ�Ͻ��㣬���Ĭ�Ͻ�����CN_INVALID_EVTT_ID����δ���ý���
//      ���豸��û�н���
//����: focus_evtt��Ĭ�Ͻ��㣬
//����: ��
//-----------------------------------------------------------------------------
void HmiIn_SetFocusDefault(tpInputMsgQ FocusMsgQ)
{
    tg_pDefaultFocusMsgQ = FocusMsgQ;
}

//----ȡĬ����Ϣ����-----------------------------------------------------------
//���ܣ���ȡĬ����Ϣ���У�����û��ָ����Ϣ���е������豸����������Ϣ����Ĭ����Ϣ
//      ���С�
//��������
//���أ�Ĭ����Ϣ��Ŷָ�롣
//-----------------------------------------------------------------------------
tpInputMsgQ HmiIn_GetFocusDefault(void)
{
   return tg_pDefaultFocusMsgQ;
}

//----���������豸���뽹��-----------------------------------------------------
//����: �����������豸�Ľ�����Ϊָ��ֵ
//����: focus_evtt���µĽ��㽹��
//����: ��
//-----------------------------------------------------------------------------
//void HmiIn_SetFocusAll(u16 focus_evtt)
//{
//    //������������������豸����"input device"��Դ��������Դ���������ǵ�focus_evtt
//    struct  Object  *current,*start,*target;
//    start = &(s_ptHmiInDeviceRscTree->stdin_device_node);
//    current = start;
//    while((target =
//        OBJ_TraveScion(start,current))!=NULL)
//    {
//        ((struct StdinDeviceRsc *)target)->focus_evtt = focus_evtt;
//        current = target;
//    }
//}

//----������Ϣ-----------------------------------------------------------------
//����: �����豸����µ������(�����µļ��̻������������)�����ñ����������µ�
//      ��Ϣ����ϵͳ�������¼������in_device->focus_evtt�ѳ�ʼ�������¼�������
//      in_device->focus_evtt�����δ��ʼ�������¼�������u16g_default_focus��
//      ���u16g_default_focusҲû�г�ʼ�����򲻵����¼�
//����: in_device��Ŀ�������豸����Դ�ӵ�
//      msg_data���������������
//      msg_size��������Ϣ�ĳ��ȣ��ֽ���
//����: true=�ɹ���false=ʧ�ܣ�һ������Ϊ�����豸δ��װ��
//-----------------------------------------------------------------------------
bool_t HmiIn_InputMsg(s32 stdin_id,u8 *msg_data, u32 msg_size)
{
    struct  Object  *current,*start,*target;
    struct InputDeviceMsg input_msg;
    tpInputMsgQ InputMsgQ;

    msg_size = msg_size;        //�����������澯

    start = &s_ptHmiInDeviceRscTree->stdin_device_node;
    current = &s_ptHmiInDeviceRscTree->stdin_device_node;

    //����Դ�����в���stdin_id��Ӧ�������豸
    while((target = OBJ_TraveScion(start,current))!=NULL)
    {
        if(((struct HMI_InputDeviceRsc *)target)->device_id == stdin_id)
            break;
        current = target;
    }
    if(target == NULL)        //δ�ҵ������豸
        return false;

    InputMsgQ = ((struct HMI_InputDeviceRsc *)target)->FocusMsgQ;
    ((struct HMI_InputDeviceRsc *)target)->input_counter++;
    if(InputMsgQ == NULL)      //���豸�����뽹��δ��ʼ��
    {
        InputMsgQ = tg_pDefaultFocusMsgQ;      //ȡĬ�Ͻ���(����Ҳδ��ʼ��)
    }

    if(InputMsgQ != NULL)
    {
        input_msg.input_data = *(union un_input_data *)msg_data;
        input_msg.input_type = ((struct HMI_InputDeviceRsc *)target)->input_type;
        input_msg.device_id = ((struct HMI_InputDeviceRsc *)target)->device_id;
        MsgQ_Send(InputMsgQ,(u8*)&input_msg,sizeof(struct InputDeviceMsg),
                    CN_TIMEOUT_FOREVER,CN_MSGQ_PRIO_NORMAL);
    }
    return true;
}

//----��ȡ��Ϣ-----------------------------------------------------------------
//����: ��������Ϣ�����ж�ȡ������Ϣ
//����: InputMsgQ, ������Ϣ����Ϣ����
//      MsgBuf, ������յ�����Ϣ�Ļ�����
//      TimeOut, �����Ϣ���п�, �Ⱥ���Ϣ��ʱ��
//����: true=�ɹ���false=ʧ�ܣ�һ������Ϊ�����豸δ��װ��
//-----------------------------------------------------------------------------
bool_t HmiIn_ReadMsg(tpInputMsgQ InputMsgQ,
                     struct InputDeviceMsg *MsgBuf,u32 TimeOut)
{
    return MsgQ_Receive(InputMsgQ,(u8*)MsgBuf,sizeof(struct InputDeviceMsg),TimeOut);
}

//----��Ĭ�϶��ж�ȡ��Ϣ-------------------------------------------------------
//����: ��Ĭ��������Ϣ�����ж�ȡ������Ϣ
//����: MsgBuf, ������յ�����Ϣ�Ļ�����
//      TimeOut, �����Ϣ���п�, �Ⱥ���Ϣ��ʱ��
//����: true=�ɹ���false=ʧ�ܣ�һ������Ϊ�����豸δ��װ��
//-----------------------------------------------------------------------------
bool_t HmiIn_ReadDefaultMsg(struct InputDeviceMsg *MsgBuf,u32 TimeOut)
{
    return MsgQ_Receive(tg_pHmiInputMsgQ,(u8*)MsgBuf,sizeof(struct InputDeviceMsg),TimeOut);
}

//----ж�������豸-------------------------------------------------------------
//����: �������豸����Դ�ӵ�ɾ������"input device"��Դ������Դ�У��ҵ�
//      "device_name"��Դ�ӵ㣬ɾ��֮�������ڴ��ͷŵ�pg_stdin_device_pool
//      �ڴ����
//����: device_name��Ŀ���豸����Դ��
//����: true = �ɹ�ɾ����0=ʧ�ܡ�
//-----------------------------------------------------------------------------
bool_t HmiIn_UnInstallDevice(const char *device_name)
{
    struct  Object *temp;
    struct HMI_InputDeviceRsc *Djy_HmiIn;
    Djy_HmiIn = (struct HMI_InputDeviceRsc *)OBJ_SearchChild(
                 &s_ptHmiInDeviceRscTree->stdin_device_node,device_name);
    if(Djy_HmiIn == NULL)
        return false;

    temp = &Djy_HmiIn->stdin_device_node;
    if(OBJ_Del(temp) != NULL)
    {
        Mb_Free(g_ptHmiInDevicePool,Djy_HmiIn);
        return true;
    }
    else
        return false;
}

