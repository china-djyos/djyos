//-----------------------------------------------------------------------------

// ģ��汾: V1.00
// ������Ա:yt
// ����ʱ��:2017
// =============================================================================

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "os.h"
#include "cpu_peri.h"
#include "time.h"
#include <silan_syscfg.h>
#include <silan_rtc.h>

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//  extern ptu32_t ModuleInstall_CpuRtc(ptu32_t para);
//  ModuleInstall_CpuRtc(0);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip rtc"//CPU��rtc��������
//parent:"rtc"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"time","int"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_RTC == false )
//#warning  " cpu_onchip_rtc  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header    //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_RTC    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������
//@#$%component end configure

#ifdef CFG_CPU_ZQ12XX_M0

#if 0
typedef struct _RTC_
{
    vu32 TIMER;
    vu32 TMCON;
    vu32 CLKOUT_CON;
    vu32 WEEK_ALARM;
    vu32 DAY_ALARM;
    vu32 HOUR_ALARM;
    vu32 MIN_ALARM;
    vu32 YEARL;
    vu32 MON;
    vu32 WEEK;
    vu32 DAY;
    vu32 HOUR;
    vu32 MIN;
    vu32 SEC;
    vu32 YEARH;
    vu32 RTC_CS0;
    vu32 RTC_CS1;
    vu32 SCOUNTERLOADH;
    vu32 SCOUNTERLOADL;
}tagRTCReg;

#endif
#define HexToBcd(x) ((((x) / 10) <<4) + ((x) % 10))  //16����ת����BCD
#define BcdToHex(x) ((((x) & 0xF0) >>4) * 10 + ((x) & 0x0F)) //BCDת����16����

// =============================================================================
// ���ܣ���RTC�豸�ж�ȡRTCʱ�䣬��λ΢��,ȡ��1970��1��1��0:0:0�����ڵ�ʱ���
// ������time, ʱ��ֵ���������ʱ��ת����1970��1��1��0:0:0�����ڵ�ʱ���
// ���أ�true,�����������������
// =============================================================================
bool_t RTC_GetTime(s64 *time)
{
    struct tm dtm;
    u32 yearh,yearl,month,date,hour,min,sec;

    sec = silan_rtc_reg_get(0x134);
    min = silan_rtc_reg_get(0x130);
    hour = silan_rtc_reg_get(0x12C);
    date = silan_rtc_reg_get(0x128);
    month = silan_rtc_reg_get(0x120);
    yearl = silan_rtc_reg_get(0x11C);
    yearh = silan_rtc_reg_get(0x138);

    dtm.tm_year  = BcdToHex(yearl & 0xFF);
    dtm.tm_year += BcdToHex(yearh & 0xFF) * 100;
    dtm.tm_mon  = BcdToHex(month & 0x1F);
    dtm.tm_mday = BcdToHex(date & 0x3F);
    dtm.tm_hour = BcdToHex(hour & 0x3F);
    dtm.tm_min  = BcdToHex(min & 0x7F);
    dtm.tm_sec  = BcdToHex(sec & 0x7F);

    *time = (s64)(1000000 * Time_MkTime(&dtm));
    return true;
}

// =============================================================================
// ���ܣ�����RTC�豸RTCʱ�䣬��λ΢�룬��ʱ���1970��1��1��0:0:0�����ڵ�ʱ���
// ������time, ʱ��ֵ
// ���أ�true,�����������������
// =============================================================================
static bool_t RTC_SetTime(s64 time)
{
    struct tm dtm;
    u8 rtccs0,year;
    time = time/1000000;
    Time_LocalTime_r(&time,&dtm);

    rtccs0 = silan_rtc_reg_get(0x13c);
    silan_rtc_reg_set(0x13C,rtccs0 | (1<<5));//��ʱ�ر�ʱ��

    silan_rtc_reg_set(0x134,(u8)(HexToBcd(dtm.tm_sec)));
    silan_rtc_reg_set(0x130,(u8)(HexToBcd(dtm.tm_min)));
    silan_rtc_reg_set(0x12C,(u8)(HexToBcd(dtm.tm_hour)));

    silan_rtc_reg_set(0x128,(u8)(HexToBcd(dtm.tm_mday)));
    silan_rtc_reg_set(0x120,(u8)(HexToBcd(dtm.tm_mon)));

    year = (u8)(dtm.tm_year % 100);
    silan_rtc_reg_set(0x11C,(u8)(HexToBcd(year)));
    year = (dtm.tm_year/100);
    silan_rtc_reg_set(0x138,(u8)(HexToBcd(year)));

    silan_rtc_reg_set(0x124,(u8)(HexToBcd(dtm.tm_wday)));

    silan_rtc_reg_set(0x13C,rtccs0);//��ʱ�ر�ʱ��

    return true;
}

// =============================================================================
// ���ܣ�RTCӲ����ʼ��
//     ���ȼ��LSE�Ƿ��ܿ���,���������ʹ��LSIʱ��
// ������void
// ���أ�true,�����������������
// =============================================================================
static bool_t RTC_Configuration(void)
{
    silan_rtc_init();
    return true;
}
// =============================================================================
// ���ܣ�RTCʱ��ע�ἰ��ʼ��
// ������time, ʱ��ֵ
// ���أ�true,�����������������
// =============================================================================
ptu32_t ModuleInstall_CpuRtc(ptu32_t para)
{
    s64 rtc_time;
    struct timeval tv;

    RTC_Configuration();    //����RTC

    RTC_GetTime(&rtc_time);  //��RTC�豸�ж�ȡRTCʱ�䣬��λ��us,
                             //��ȡ��ֵ�ŵ�&rtc_time��

    tv.tv_sec  = rtc_time/1000000;   //�Ѷ�ȡ��ʱ����s�Ĳ�������
    tv.tv_usec = rtc_time%1000000;   //�Ѷ�ȡ��ʱ����us�Ĳ�������(����)

    settimeofday(&tv,NULL);

    //ע��RTCʱ��
    if(!Rtc_RegisterDev(NULL,RTC_SetTime))
        return false;
    return true;
}
#endif
