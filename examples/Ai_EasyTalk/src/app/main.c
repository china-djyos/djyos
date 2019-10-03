
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
// Copyright (c) 2014 ����Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������¶������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�����������У���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�����������У��Լ�����
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
/*
 * app_main.c
 *
 *  Created on: 2014-5-28
 *      Author: Administrator
 */

#include "stdint.h"
#include "stddef.h"
#include <sys/socket.h>
#include <gpio_pub.h>
#include <sys/dhcp.h>
#include <app_flash.h>
#include "project_config.h"

extern  WIFI_CFG_T  LoadWifi;
extern  WIFI_CFG_T  LoadUser;

extern void airkissEnterProcess(void);

u32 Get_DhcpInitflag(void)
{
#if RC_DEBUG_OFF
    if(!WifiLoad())
    {
        return 1<<1;
    }
    return (1<<0);
#else
    return 1;
#endif
}

void log_time(char * log)
{
    static s64 time_djymain = 0;
    if(time_djymain == 0)
        time_djymain = DjyGetSysTime();
    printf("[====] %s time :%f ms\r\n ",log,(float)(DjyGetSysTime()-time_djymain)/(1000));
}


int cb_ip_get(u32 *ip)
{
    if (DhcpIp_InfoLoad(ip)) {
        char *ip_str = inet_ntoa(*((struct in_addr *)ip));
        if (ip_str) {
            printf("===== local ip: %s =====!\r\n", ip_str);
        }
        return 1;
    }
    return 0;
}

int cb_ip_set(u32 ip)
{
    char *ip_str = inet_ntoa(*((struct in_addr *)&ip));

    if (ip_str) {
        printf("===== dhcp ip: %s =====!\r\n", ip_str);
    }
    u32 ip_temp;
    if (DhcpIp_InfoLoad(&ip_temp)) {
        if (ip_temp == ip) {
            printf("===== Match: Do not Write! =====!\r\n");
            return 0;
        }
    }
    printf("===== Not match: Write to Flash! =====!\r\n");
    DhcpIp_InfoSave(&ip, sizeof(ip));
    return 1;
}

const unsigned char mp3_data[] = {
//#include "WeChatConnectError.dat"
#include "ConnectOk.dat"
};

ptu32_t Key_EventMain(void);
bool_t Key_event_init(void)
{
    u16 evtt_Key;
    evtt_Key = Djy_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 0, Key_EventMain, NULL, 0x1000, "Key_evevt");
    if(evtt_Key != CN_EVENT_ID_INVALID)
    {
        Djy_EventPop(evtt_Key, NULL, 0, NULL, 0, 0);
    }else
    {
        printf("\r\evtt_recorder Event Start Fail!\r\n");
    }

    return true;
}

//WAV¼��
ptu32_t Key_EventMain(void)
{
    mp3_module_init();
    while(1)
    {
        Mp3PlayData(mp3_data, sizeof(mp3_data), 80, 1000);
        Djy_EventDelay(1000*1000);
    }
    return 0;

}

typedef enum
{
    NO_KEY = 0,
    PAUSE_PLAY_KEY,
    VOL_UP_KEY,
    VOL_DOWN_KEY,
    RECORD_KEY,
    POWER_KEY,
} KEY_ID;

KEY_ID current_key;
extern int djy_adc_read(uint16_t channel);
void key_recognition(void)
{
    int Vol;
    Vol = djy_adc_read(2);
    if(Vol > 1550)
    {
        if(Vol < 2200)
            current_key = RECORD_KEY;
        else
            current_key = NO_KEY;
    }
    else if(Vol > 950)
        current_key = VOL_DOWN_KEY;
    else if(Vol > 300)
        current_key = VOL_UP_KEY;
    else if(Vol > 300)
        current_key = VOL_UP_KEY;
    else if(Vol >= 0)
        current_key = PAUSE_PLAY_KEY;

    if(!djy_gpio_read(13))
        current_key = POWER_KEY;

    printf("key = %d\r\n", current_key);
}
extern void usb_charge_check_cb(void);
extern void usb_plug_func_open(void);

ptu32_t djy_main(void)
{
    u32 j = 0;
    int Vbat;
    log_time("djy_main");
#if 1
    extern ptu32_t ModuleInstall_st7796s(const char *DisplayName,const char* HeapName);
    ModuleInstall_st7796s(CFG_ST7796S_DISPLAY_NAME,CFG_ST7796S_HEAP_NAME);

    extern void ModuleInstall_Gdd_AND_Desktop(void);
    ModuleInstall_Gdd_AND_Desktop();
//
    extern struct GkWinObj;
    extern bool_t ModuleInstall_FT6236(struct GkWinObj *desktop);
    struct GkWinObj *desktop;
    desktop = GK_GetDesktop(CFG_DISPLAY_NAME);
    if(NULL == desktop)
    {
        printf("Desktop Not Exist !\r\n");
    }
    else
    {
        ModuleInstall_FT6236(desktop);
    }
    extern bool_t GDD_AddInputDev(const char *InputDevName);
    GDD_AddInputDev(CFG_FT6236_TOUCH_NAME);
#endif
#if 1
    log_time("git ssid key");
//=========================����wifi============================================
//    if (!WifiLoad())
#if 1
    if(0)
    {
        log_time("wifi Config");
        airkissEnterProcess ();
    }else
    {
//        strcpy(LoadWifi.WifiSsid,"DJYOSauto");
//        strcpy(LoadWifi.WifiPassWd,"1234567980");
        strcpy(LoadWifi.WifiSsid,"djyos-ap");
        strcpy(LoadWifi.WifiPassWd,"djyos12345");
        log_time("wifi Connecting");
        printf("ap_ssid= %s,ap_key = %s \r\n ",LoadWifi.WifiSsid,LoadWifi.WifiPassWd);
        DjyWifi_StaAdvancedConnect(LoadWifi.WifiSsid, LoadWifi.WifiPassWd);
        if(!PendDhcpDone(20*1000*1000))
        {
            printf("wifi Connect Fail!! \r\n");
            while(1);
        }

        DjyWifi_StaConnectDone();
        log_time("DjyWifi_StaConnectDone");
    }
#endif
    printf("hello world!\r\n");
//    mp3_module_init();
    Key_event_init();
//=======================================================================
#endif
#if 1
    GUI_StudyMachine();
#endif
    usb_plug_func_open();
    gpio_config(13, GMODE_INPUT_PULLUP);
	while(1)
	{
//	    Mp3PlayData(mp3_data, sizeof(mp3_data), 80, 1000);
	    key_recognition();
        if(j++ > 300)
        {
            Vbat = vbat_voltage_get(0);
            printf("Vbat = %d  \r\n", Vbat);
            usb_charge_check_cb();
            j = 0;
        }

		Djy_EventDelay(100*1000);
	}
	return 0;
}
