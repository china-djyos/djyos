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
//����ģ��:�û�����ģ��
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
#ifndef __STDIN_DEV_H__
#define __STDIN_DEV_H__
#include "object.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct MsgQueue * tpInputMsgQ;

enum _STDIN_INPUT_TYPE_
{
    EN_HMIIN_INVALID,           //�Ƿ��豸
    EN_HMIIN_KEYBOARD,          //����
    EN_HMIIN_MOUSE_2D,          //2d���
    EN_HMIIN_MOUSE_3D,          //3d���
    EN_HMIIN_SINGLE_TOUCH,      //���㴥����
    EN_HMIIN_MUTI_TOUCH,        //��㴥����
    EN_HMIIN_AREA_TOUCH,        //��������
};


//������Ϣ���ݽṹ
struct KeyBoardMsg
{
    s64 time;              //�����¼�����ʱ��,US��
    u8 key_value[2];       //��ֵ,ÿ������2�������
};

//2ά������ݽṹ����Ŀǰ��õ����
struct Mouse2D_Msg
{
    s64 time;                       //����¼�����ʱ��,US��
    //ָ������¼���Ӧ����Ļ��NULL��ʾĬ����ʾ�������ֻ��һ����Ҳ����ΪNULL��
    struct DisplayObj *display;
    s32 x,y;                        //��ʾ���ָ��λ��
    u16 key_no;                     //������������
};

//3ά������ݽṹ
struct Mouse3D_Msg
{
    s64 time;                       //�����¼�����ʱ��,US��
    //ָ������¼���Ӧ����Ļ��NULL��ʾĬ����ʾ�������ֻ��һ����Ҳ����ΪNULL��
    struct DisplayObj *display;
    s32 x,y,z;                      //��ʾ���ָ��λ��
    u16 key_no;                     //������������
};

//���㴥����������õĴ�����
struct SingleTouchMsg
{
    s64 time;               //�����¼�����ʱ��,US��
    //ָ�������¼���Ӧ����Ļ��NULL��ʾĬ����ʾ�������ֻ��һ����Ҳ����ΪNULL��
    struct DisplayObj *display;
    s32 x,y,z;              //x,y��ʾ����λ�ã�z>0��־����ѹ����0��־δ����
    s16 MoveX,MoveY;        //ÿms�ƶ���΢����
};

//��㴥����
struct MultiTouchMsg
{
    s64 time;              //�����¼�����ʱ��,US��
};

//������������ʾ����������һ������
struct AreaTouchMsg
{
    s64 time;              //�����¼�����ʱ��,US��
};

union un_input_data
{
    struct KeyBoardMsg key_board;
    struct Mouse2D_Msg  mouse_2d;
    struct Mouse3D_Msg  mouse_3d;
    struct SingleTouchMsg  SingleTouchMsg;
    struct MultiTouchMsg   muti_touch;
    struct AreaTouchMsg    area_touch;
};

struct InputDeviceMsg
{
    enum _STDIN_INPUT_TYPE_ input_type;         //������Ϣ���ͣ�
    s32 device_id;          //�����豸id
    union un_input_data input_data;
};

struct HMI_InputDeviceObj
{
    struct Object *HostObj;
    enum _STDIN_INPUT_TYPE_ input_type;   //�����豸����
    s32 device_id;                      //�����豸id
    u32 input_counter;                  //�������豸�ۼ������˶�����Ϣ
    tpInputMsgQ FocusMsgQ;             //���ձ������豸��������Ϣ����
    ptu32_t stdin_private;              //�����豸��˽�����ݽṹ��
};

bool_t ModuleInstall_HmiIn(void);
s32 HmiIn_InstallDevice(const char *device_name,enum _STDIN_INPUT_TYPE_ stdin_type,
                        void *myprivate);
bool_t HmiIn_UnInstallDevice(const char *device_name);
tpInputMsgQ HmiIn_CreatInputMsgQ(u32 MsgNum);
bool_t HmiIn_DeleteInputMsgQ(tpInputMsgQ InputMsgQ);
bool_t HmiIn_ReadMsg(tpInputMsgQ InputMsgQ,
                     struct InputDeviceMsg *MsgBuf,u32 TimeOut);
bool_t HmiIn_ReadDefaultMsg(struct InputDeviceMsg *MsgBuf,u32 TimeOut);
bool_t HmiIn_SetFocus(const char *device_name,tpInputMsgQ FocusMsgQ);
enum _STDIN_INPUT_TYPE_ HmiIn_CheckDevType(const char *device_name);
void HmiIn_SetFocusDefault(tpInputMsgQ FocusMsgQ);
tpInputMsgQ HmiIn_GetFocusDefault(void);
bool_t HmiIn_InputMsg(s32 stdin_id,u8 *msg_data);

#ifdef __cplusplus
}
#endif

#endif //__STDIN_DEV_H__

