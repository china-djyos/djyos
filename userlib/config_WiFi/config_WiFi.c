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


//======================================================================
//  ��ҳ����ģʽĿǰֻ֧��bk7251��ƽ̨
//======================================================================


#include "config_Wifi.h"
#include <dbug.h>
#include <systime.h>
#include <cpu_peri_wifi.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�
extern int GetNetInfo(char *ssid, int ssid_len, char *passwd, int pwd_len);
extern bool_t web_config_event_init();

s32 config_net_modes = 0;

s32 NetcGetModes(void)
{
    return config_net_modes;
}

// ============================================================================
// ���ܣ���ʼ��������ȡwifi�˺ź�����
// ������para������������
// ���أ�0 -- �ɹ���-1 -- ʧ�ܣ�
// ��ע��
// ============================================================================
s32 NetcStart(struct NetcInfo *Netc)
{
    s32 ret = -1;
    switch(Netc->mode)
    {
        case NETC_USER_INPUT:
            warning_printf("netc", "KEY BOARD mode is temporarily not supported.\r\n");
        break;

        case NETC_AP:
            DjyWifi_ApOpen((char *)Netc->ap_ssid, (char *)Netc->ap_key);
            DJY_EventDelay(1000*mS);    //bk3.0�Ŀ��AP��ɨ��wifi��Ҫ��ʱ1�룬��Ȼ��core100%
            web_config_event_init();
            GetNetInfo((char *)Netc->ssid, Netc->ssid_len, (char *)Netc->pwd, Netc->pwd_len);
            DJY_EventDelay(100*mS);
            DjyWifi_ApClose();
            ret  = 0;
        break;

        case NETC_AIRKISS:
            warning_printf("netc", "AIRKISS mode is temporarily not supported.\r\n");
        break;

        case NETC_WAVE:
            warning_printf("netc", "WAVE mode is temporarily not supported.\r\n");
        break;

        default:
            warning_printf("netc", "unsupported mode.\r\n");
        break;
    }

    return ret;
}

// ============================================================================
// ���ܣ���������
// ������mode������ģʽ��
// ���أ�0 -- �ɹ���-1 -- ʧ�ܣ�
// ��ע��
// ============================================================================
s32 NetcEnd(s32 mode)
{
    s32 ret = -1;
    switch(mode)
    {
        case NETC_USER_INPUT:
            warning_printf("netc", "KEY BOARD mode is temporarily not supported.\r\n");
        break;

        case NETC_AP:
            ret  = 0;
        break;

        case NETC_AIRKISS:
            warning_printf("netc", "AIRKISS mode is temporarily not supported.\r\n");
        break;

        case NETC_WAVE:
            warning_printf("netc", "WAVE mode is temporarily not supported.\r\n");
        break;

        default:
            warning_printf("netc", "unsupported mode.\r\n");
        break;
    }

    return ret;
}

// =============================================================================
// ���ܣ���ʼ����������
// ��������
// ���أ�ture=�ɹ���false=ʧ��
// =============================================================================
bool_t ModuleInstall_ConfigNet(void)
{
    if(CFG_NETC_USER_INPUT_ENABLE == true)
        config_net_modes |= NETC_USER_INPUT;

    if(CFG_NETC_AP_ENABLE == true)
        config_net_modes |= NETC_AP;

    if(CFG_NETC_AIRKISS_ENABLE == true)
        config_net_modes |= NETC_AIRKISS;

    if(CFG_NETC_WAVE_ENABLE == true)
        config_net_modes |= NETC_WAVE;

    return true;
}
