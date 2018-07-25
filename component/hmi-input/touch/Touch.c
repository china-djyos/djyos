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
//����ģ��:��׼����--������ģ��
//����:  ������.
//�汾��V1.1.0
//�ļ�����: �������豸��������
//����˵��:
//�޶���ʷ:
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "hmi-input.h"
#include "object.h"
#include "djyos.h"
#include "Touch.h"
#include "string.h"
#include "systime.h"

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_Touch(void);
//    ModuleInstall_Touch();    //��ʼ���˻���������ģ��
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"touch"        //������ģ��
//parent:"HmiInput"             //��д������ĸ�������֣�none��ʾû�и����
//attribute:�������             //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                   //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//grade:init                    //��ʼ��ʱ������ѡֵ��none��init��main��none��ʾ�����ʼ����
                                //init��ʾ�ڵ���main֮ǰ��main��ʾ��main�����г�ʼ��
//dependence:"HmiInput"         //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������������������������none����ʾ�������������
                                //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
//%$#@end configue  ****�������ý���
//@#$%component end configure


//----������ɨ������-----------------------------------------------------------
//����: �����Եص���Ӳ��ɨ����򣬻�ȡ�û��û�������Ϣ�����ѻ�õ���Ϣ���뵽��׼
//      ���뻺����(���ú���stdin_input_msg)��ע�⣬̧�����ʺ󣬻�Ҫ����һ����
//      �����뿪����Ϣ
//����: my_event�����̷����¼����͡�
//����: ��
//----------------------------------------------------------------------------
ptu32_t Touch_Scan(void)
{
    struct HMI_InputDeviceObj *TouchObj,*StdinObj;
    struct SingleTouchPrivate *touch_pr;
    struct SingleTouchMsg touch_temp = {0,0,0,0,0};

    StdinObj = (struct HMI_InputDeviceObj *)OBJ_SearchChild(OBJ_Root(),"stdin input device");
    while(1)
    {
        TouchObj = StdinObj;
        while(1)
        {
            TouchObj = (struct HMI_InputDeviceObj*)
                            OBJ_TraveScion(StdinObj->HostObj,TouchObj->HostObj);
            if(TouchObj == NULL)
                break;
            if(TouchObj->input_type != EN_HMIIN_SINGLE_TOUCH)
                continue;
            touch_pr = (struct SingleTouchPrivate*)TouchObj->stdin_private;
            if(touch_pr->read_touch(&touch_temp) != 0)
            {
                if((touch_temp.x != touch_pr->touch_loc.x)
                    ||(touch_temp.y != touch_pr->touch_loc.y)
                    ||(touch_temp.z != touch_pr->touch_loc.z))
                {
                    touch_temp.time = DjyGetSysTimeBase();
                    HmiIn_InputMsg(TouchObj->device_id,(u8*)&touch_temp,
                                                        sizeof(touch_temp));
                    touch_pr->touch_loc = touch_temp;
                    touch_pr->status = true;
                }
            }
            else
            {
                if(touch_pr->status == true)
                {
                    touch_pr->touch_loc.time = DjyGetSysTimeBase();
                    touch_pr->touch_loc.z = 0;
                    HmiIn_InputMsg(TouchObj->device_id,
                                    (u8*)&touch_pr->touch_loc,
                                    sizeof(touch_pr->touch_loc));
                    touch_pr->status = false;
                }
            }
        }
        Djy_EventDelay(10*mS);
    }
}


s32 Touch_InstallDevice(char *touch_name,struct SingleTouchPrivate *touch_pr)
{
    s32  ret = 0;
    ret =  HmiIn_InstallDevice(touch_name, EN_HMIIN_SINGLE_TOUCH, touch_pr);
    if(-1 == ret)
        touch_pr->status = false;
    return ret;
}
//----��ʼ��������ģ��---------------------------------------------------------
//����: ��ʼ��������ģ�飬����:��װ��������׼�����豸���ǼǴ�����ɨ���¼�
//      ���ͣ�pop������ɨ���¼�������ɨ��(ʵ��ɨ�蹤��Ҫ�ȶ��߳�������ſ�ʼ)
//����: ��
//����: false=ʧ�ܣ�true=�ɹ�
//----------------------------------------------------------------------------
bool_t ModuleInstall_Touch(void)
{
    s16 touch_scan_evtt;
    if(!OBJ_SearchChild(OBJ_Root( ),"stdin input device"))      //��׼�����豸δ��ʼ��
        return false;
    touch_scan_evtt = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_REAL,0,0,
                            Touch_Scan,NULL,2048,"touch");
    if(touch_scan_evtt == CN_EVTT_ID_INVALID)
    {
        return false;
    }
    if(Djy_EventPop(touch_scan_evtt, NULL,0,0,0,0)
                        == (uint16_t)CN_EVENT_ID_INVALID)
    {
        Djy_EvttUnregist(touch_scan_evtt);
        return false;
    }
    return true;
}

