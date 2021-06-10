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
#include <stdio.h>
#include "hmi-input.h"
#include "object.h"
#include "djyos.h"
#include "Touch.h"
#include <gdd.h>
#include <gui/gk_display.h>
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
//parent:"human machine interface"      //��д������ĸ�������֣�none��ʾû�и����
//attribute:system              //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"human machine interface"  //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"graphical decorate development"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_TOUCH == false )
//#warning  " touch  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_TOUCH    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

u16 Touch_BinSqrt(u32 value)
{
    u16 root;
    u16 rem;
    s32 i;
    for(root = rem = i = 0; i < 16; ++i)
    {
        int if1;
        root <<= 1;
        rem = (rem << 2) | (value >> 30);
        value <<= 2;
        if1 = (root << 1) + 1;
        if(rem >= if1)
        {
            root |= 1;
            rem -= if1;
        }
    }
    return root;
}

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
    struct Object *ob;
    struct SingleTouchMsg touch_temp = {0,0,0,0,0,0,0};
    struct DisplayObj *display;

    ob = OBJ_SearchChild(OBJ_GetRoot(),"hmi input device");
    StdinObj = (struct HMI_InputDeviceObj *)OBJ_GetPrivate(ob);
    while(1)
    {
        TouchObj = StdinObj;
        while(1)
        {
            ob = OBJ_ForeachScion(StdinObj->HostObj,TouchObj->HostObj);
            if(ob == NULL)
                break;
            TouchObj = (struct HMI_InputDeviceObj*)OBJ_GetPrivate(ob);

            if(TouchObj->input_type != EN_HMIIN_SINGLE_TOUCH)
                continue;
            //ע�⣬�����ж������������Ӧ�����ʾ��
            touch_pr = (struct SingleTouchPrivate*)TouchObj->stdin_private;
            display = touch_pr->touch_loc.display;
            if(touch_pr->read_touch(&touch_temp) != 0)
            {
                if(touch_pr->TouchStatus == CN_NO_TOUCH)   //�ոսӴ�����������Ϣ������Ҫ��һ���ƶ�
                {
                    //���³�ʼ������
                    touch_pr->touch_loc.x = touch_temp.x;
                    touch_pr->touch_loc.y = touch_temp.y;
                    touch_pr->touch_loc.z = touch_temp.z;
//                  touch_pr->touch_loc.time = DJY_GetSysTime();
                    touch_pr->TouchStatus = CN_TOUCHING;
                }
                else   //�Ѿ��Ӵ����ж��Ƿ񻬶�
                {
                    s32 DeltaX,DeltaY,Distance;
                    DeltaX = touch_temp.x - touch_pr->touch_loc.x;  //�������ؾ���
                    DeltaY = touch_pr->touch_loc.y - touch_temp.y;  //�������ؾ��룬y������ϵ��£���֮��
                    Distance = Touch_BinSqrt(DeltaX*DeltaX + DeltaY*DeltaY);
                    //����������룬������������������εģ���x��y����ȼ�ࡣ
                    Distance = Distance * display->width_um / display->width;
                    touch_temp.time = DJY_GetSysTime();
                    if(Distance < 1000)     //���С��1mm������Ϊû���ƶ�,��������Ϣ
                    {
                        if(touch_pr->TouchStatus != CN_GOT_TOUCH)
                        {
                            touch_pr->TouchStatus = CN_GOT_TOUCH;
                            touch_temp.MoveX = 0;
                            touch_temp.MoveY = 0;
//                            printf("touch: x = %d ,y = %d \r\n",
//                                touch_temp.x, touch_temp.y);
                            //����������=0����Ϣ����������Ϣ
                            HmiIn_InputMsg(TouchObj->device_id,(u8*)&touch_temp);
                        }
                    }
                    else
                    {
                        touch_pr->TouchStatus = CN_MOVING;
                        touch_pr->touch_loc.x = touch_temp.x;
                        touch_pr->touch_loc.y = touch_temp.y;
                        touch_pr->touch_loc.z = touch_temp.z;
                        touch_temp.MoveX = DeltaX;
                        touch_temp.MoveY = DeltaY;
//                        printf("move: x = %d ,y = %d ,movx = %d ,movy = %d\r\n",
//                            touch_temp.x, touch_temp.y, touch_temp.MoveX, touch_temp.MoveY);
                        //�������������ݵ���Ϣ����������Ϣ
                        HmiIn_InputMsg(TouchObj->device_id,(u8*)&touch_temp);
                        //��⵽������200mS�ڲ���⣬�����������ٷ�������Ϣ
                        DJY_EventDelay(200*mS);
                    }
                }
            }
            else
            {
               if(touch_pr->TouchStatus != CN_NO_TOUCH)   //�ո�̧����ָ
                {
                    touch_pr->touch_loc.time = DJY_GetSysTime();
                    touch_pr->touch_loc.z = 0;
                    touch_pr->touch_loc.MoveX = 0;
                    touch_pr->touch_loc.MoveY = 0;
                    touch_pr->TouchStatus = CN_NO_TOUCH;
//                    printf("touch left: x = %d ,y = %d \r\n",
//                        touch_pr->touch_loc.x, touch_pr->touch_loc.y);
                    HmiIn_InputMsg(TouchObj->device_id,(u8*)&touch_pr->touch_loc);
                }
            }
        }
        DJY_EventDelay(20*mS);
    }
}


s32 Touch_InstallDevice(char *touch_name,struct SingleTouchPrivate *touch_pr)
{
    s32  ret = 0;

    ret =  HmiIn_InstallDevice(touch_name, EN_HMIIN_SINGLE_TOUCH, touch_pr);
    if(-1 == ret)
        touch_pr->TouchStatus = false;
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
    u16 touch_scan_evtt;
    if(!OBJ_SearchChild(OBJ_GetRoot( ),"hmi input device"))      //��׼�����豸δ��ʼ��
        return false;
    touch_scan_evtt = DJY_EvttRegist(EN_CORRELATIVE,CFG_GUI_RUN_PRIO,0,0,
                            Touch_Scan,NULL,2048,"touch");
    if(touch_scan_evtt == CN_EVTT_ID_INVALID)
    {
        return false;
    }
    if(DJY_EventPop(touch_scan_evtt, NULL,0,0,0,0)
                        == (uint16_t)CN_EVENT_ID_INVALID)
    {
        DJY_EvttUnregist(touch_scan_evtt);
        return false;
    }
    return true;
}

