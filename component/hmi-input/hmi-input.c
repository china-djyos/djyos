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
#include "dbug.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_HmiIn(void);
//    ModuleInstall_HmiIn();      //��ʼ���˻���������ģ��
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"HmiInput"               //�˻���������ӿ�
//parent:"none"                           //��д������ĸ�������֣�none��ʾû�и����
//attribute:system                        //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                        //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                          //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium                        //��ʼ��ʱ������ѡֵ��early��medium��later��
                                          //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"MsgQueue","gkernel"         //������������������������none����ʾ�������������
                                          //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                          //����������������������г����á�,���ָ�
//weakdependence:"none"                   //��������������������������none����ʾ�������������
                                          //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                          //����������������������г����á�,���ָ�
//mutex:"none"                            //������������������������none����ʾ�������������
                                          //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header                      //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef CFG_HMIIN_DEV_LIMIT                //****�������Ƿ��Ѿ����ú�
#warning    HmiInput�������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,0,100,
#define CFG_HMIIN_DEV_LIMIT     2       //"name",�˻����������豸����������̡�����
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

//����δ���ý���������豸��Ĭ���Դ�Ϊ����
tpInputMsgQ tg_pDefaultFocusMsgQ = NULL;
struct MemCellPool *g_ptHmiInDevicePool;
struct HMI_InputDeviceObj sg_ptStdinDeviceMem[CFG_HMIIN_DEV_LIMIT];
s32 g_s32NextId = 0;       //ÿ�ΰ�װ�����豸ʱ���Ա�����Ϊ�豸ID��Ȼ�󱾱���++
                            //ɾ���豸��ID���ջء�
static struct obj *s_ptHmiInDeviceDir;
tpInputMsgQ tg_pHmiInputMsgQ;    //  ��׼�����豸����Ϣ����

//----��ʼ����׼����ģ��-------------------------------------------------------
//����: ��ʼ����׼����ģ�飬ִ�г�ʼ���󣬿��Դ��������豸��
//����: ��
//����: true = �ɹ�,false= ʧ��.
//-----------------------------------------------------------------------------
bool_t ModuleInstall_HmiIn(void)
{
    static struct HMI_InputDeviceObj root;
    struct HMI_InputDeviceObj *StdinDeviceMem;

    tg_pHmiInputMsgQ = HmiIn_CreatInputMsgQ(10,"StdInDev");
    if(tg_pHmiInputMsgQ == NULL)
        goto ExitMsgQ;
    s_ptHmiInDeviceDir = obj_newchild(objsys_root(), (fnObjOps)-1, 0, (ptu32_t)&root, "stdin input device");
    if(s_ptHmiInDeviceDir == NULL)
    {
        goto ExitDir;
    }
    root.HostObj = s_ptHmiInDeviceDir;
    //��ʼ���豸���ƿ��ڴ��
    StdinDeviceMem = M_Malloc(sizeof( struct HMI_InputDeviceObj),0);
    if(StdinDeviceMem == NULL)
    {
        goto ExitDir;
    }
    g_ptHmiInDevicePool = Mb_CreatePool((void*)StdinDeviceMem,
                                    CFG_HMIIN_DEV_LIMIT,
                                    sizeof(struct HMI_InputDeviceObj),
                                    0,0,
                                    "�����豸���ƿ��ڴ��");
    if(s_ptHmiInDeviceDir == NULL)
    {
        goto ExitPool;
    }
    HmiIn_SetFocusDefault(tg_pHmiInputMsgQ);
    return true;

ExitPool:
    obj_del(s_ptHmiInDeviceDir);
ExitDir:
    free(StdinDeviceMem);
ExitMem:
    HmiIn_DeleteInputMsgQ(tg_pHmiInputMsgQ);
ExitMsgQ:
    return false;
}

//----��װ�����豸-------------------------------------------------------------
//����: �������豸�Ǽǵ���Դ�����
//����: device_name��Ŀ���豸����Դ��
//����: �����豸ID,-1��ʾʧ��
//-----------------------------------------------------------------------------
s32 HmiIn_InstallDevice(const char *device_name,enum _STDIN_INPUT_TYPE_ stdin_type,
                         void *myprivate)
{
    struct HMI_InputDeviceObj *Djy_HmiIn;

    if(obj_search_child(s_ptHmiInDeviceDir,device_name))
    {
        return -1;
    }
    else
    {
//        Djy_HmiIn = Mb_Malloc(g_ptHmiInDevicePool,0);
        Djy_HmiIn = M_Malloc(sizeof(struct HMI_InputDeviceObj),0);
        if(Djy_HmiIn != NULL)
        {
            Djy_HmiIn->HostObj = obj_newchild(s_ptHmiInDeviceDir, (fnObjOps)-1, 0, (ptu32_t)Djy_HmiIn, device_name);
            if(!Djy_HmiIn->HostObj)
            {
                Mb_Free(g_ptHmiInDevicePool, Djy_HmiIn);
                return (-1);
            }
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
            debug_printf("hmi-input","function %s �ڴ治��\n\r", __FUNCTION__);
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
    struct HMI_InputDeviceObj *result;
    struct obj *focus;

    focus = obj_search_child(s_ptHmiInDeviceDir,device_name);
    if(focus != NULL)
    {
        result = (struct HMI_InputDeviceObj *)obj_GetPrivate(focus);
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
    struct HMI_InputDeviceObj *InputDev;
    struct obj *input;

    input = obj_search_child(s_ptHmiInDeviceDir,device_name);
    if(input != NULL)
    {
        InputDev = (struct HMI_InputDeviceObj *)obj_GetPrivate(input);
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
//    struct  obj  *current,*start,*target;
//    start = &(s_ptHmiInDeviceDir->stdin_device_node);
//    current = start;
//    while((target =
//        obj_foreach_scion(start,current))!=NULL)
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
    struct  obj  *current;
    struct HMI_InputDeviceObj *InputDevice;
    struct InputDeviceMsg input_msg;
    tpInputMsgQ InputMsgQ;

    current = s_ptHmiInDeviceDir;

    //����Դ�����в���stdin_id��Ӧ�������豸
    while((current = obj_foreach_child(s_ptHmiInDeviceDir, current)) != NULL)
    {
        InputDevice = (struct HMI_InputDeviceObj *)obj_GetPrivate(current);
        if(InputDevice->device_id == stdin_id)
            break;
    }
    if(current == NULL)        //δ�ҵ������豸
        return false;

    InputMsgQ = InputDevice->FocusMsgQ;
    InputDevice->input_counter++;
    if(InputMsgQ == NULL)      //���豸�����뽹��δ��ʼ��
    {
        InputMsgQ = tg_pDefaultFocusMsgQ;      //ȡĬ�Ͻ���(����Ҳδ��ʼ��)
    }

    if(InputMsgQ != NULL)
    {
        input_msg.input_data = *(union un_input_data *)msg_data;
        input_msg.input_type = InputDevice->input_type;
        input_msg.device_id = InputDevice->device_id;
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
    struct  obj  *current;
    struct HMI_InputDeviceObj *Djy_HmiIn;
    current = obj_search_child(s_ptHmiInDeviceDir,device_name);
    if(current == NULL)
        return false;

    Djy_HmiIn = (struct HMI_InputDeviceObj *)obj_GetPrivate(current);
    if(!obj_del(current))
    {
        Mb_Free(g_ptHmiInDevicePool,Djy_HmiIn);
        return true;
    }
    else
        return false;
}

