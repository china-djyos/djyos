#include "stdint.h"
#include "os.h"
#include "time.h"
#include "cpu_peri_rtc.h"

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    ModuleInstall_RTC(0);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip rtc"//CPU��rtc��������
//parent:"rtc"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                      //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                   //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                     //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium                   //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                     //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"time"  //������������������������none����ʾ�������������
                                     //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                     //����������������������г����á�,���ָ�
//weakdependence:"none"              //��������������������������none����ʾ�������������
                                     //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                     //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                     //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_RTC == false )
//#warning  " cpu_onchip_rtc  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_RTC    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure


#define HexToBcd(x) ((((x) / 10) <<4) + ((x) % 10))
#define BcdToHex(x) ((((x) & 0xF0) >>4) * 10 + ((x) & 0x0F))

//// =============================================================================
//// ���ܣ�RTC�жϣ�ʹ��RTCģ���TICK������������Ϊÿһ���ж�һ�Σ������ж��ж�ȡRTC
////       ʱ�ӣ�������ϵͳʱ��
//// �������ж��ߺ�
//// ���أ�0
//// =============================================================================
//u32 RTC_ISR(ufast_t rtc_int_line)
//{
//    struct tm dtm;
//
//    rRTCCON = 1 ;       //RTC read and write enable
//    dtm.tm_year = BcdToHex(rBCDYEAR) + 2000;
//    dtm.tm_mon  = BcdToHex(rBCDMON);
//    dtm.tm_mday = BcdToHex(rBCDDATE);
//    dtm.tm_hour = BcdToHex(rBCDHOUR);
//    dtm.tm_min  = BcdToHex(rBCDMIN);
//    dtm.tm_sec  = BcdToHex(rBCDSEC);
//    rRTCCON &= ~1 ;     //RTC read and write disable
//
//    Time_SetDateTime(&dtm); // ��OS��Clockģ�鴫���µ�ʱ��ֵ
//    return 0;
//}

// =============================================================================
// ���ܣ�RTCʱ�Ӹ��£���ϵͳʱ����µ�RTCģ�飬����ʱ��ģ�����洢�����ֵΪ99�����
//       ��RTCģ����ƴ�2000Ϊ��ʼֵ����2014����RTCģ���rBCDYEAR�洢����14
// ������dtm������ʱ��
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
bool_t RTC_SetTime(s64 time)
{
    bool_t result = false;
    struct tm dtm;
    s64 time_s;

    time_s = time/1000000;
    Time_LocalTime_r(&time_s,&dtm);

    if((dtm.tm_year > 2000) && (dtm.tm_year < 2099))
    {
        rRTCCON = 1 ;       //RTC read and write enable
        rBCDYEAR = HexToBcd(dtm.tm_year - 2000);
        rBCDMON  = HexToBcd(dtm.tm_mon);
        rBCDDATE = HexToBcd(dtm.tm_mday);
        rBCDHOUR = HexToBcd(dtm.tm_hour);
        rBCDMIN  = HexToBcd(dtm.tm_min);
        rBCDSEC  = HexToBcd(dtm.tm_sec);
        rRTCCON &= ~1 ;     //RTC read and write disable
        result = true;
    }
    return result;
}

// =============================================================================
// ���ܣ���ȡRTCʱ�ӣ���ȡRTCģ���ʱ�䣬����ʱ��ģ�����洢�����ֵΪ99������轫��
//       ������ݼӻ�����2000���磬��RTCģ���rBCDYEARΪ14����Ϊ2014��
// ������dtm������ʱ��
// ���أ�true
// =============================================================================
bool_t RTC_GetTime(s64 *time)
{
    struct tm dtm;

    rRTCCON = 1 ;       //RTC read and write enable
    dtm.tm_year = BcdToHex(rBCDYEAR) + 2000;
    dtm.tm_mon  = BcdToHex(rBCDMON);
    dtm.tm_mday = BcdToHex(rBCDDATE);
    dtm.tm_hour = BcdToHex(rBCDHOUR);
    dtm.tm_min  = BcdToHex(rBCDMIN);
    dtm.tm_sec  = BcdToHex(rBCDSEC);
    dtm.tm_us   = 0;
    rRTCCON &= ~1 ;     //RTC read and write disable

    *time = 1000000 * Time_MkTime(&dtm);
    return true;
}

// =============================================================================
// ���ܣ���ʼ��RTCģ��.��ʱ�Ӽ�������λ����������Ĭ��ʱ�䣬�����RTCʱ�䣬������ϵͳ
//       ʱ��
// ������ģ���ʼ������û�в���
// ���أ�true = �ɹ���ʼ����false = ��ʼ��ʧ��
// =============================================================================
ptu32_t ModuleInstall_RTC(ptu32_t para)
{
    struct tm DateTime;
    bool_t RtcIsReset = false;

    //�ж�ʱ�Ӽ������Ƿ񱻸�λ�����Ƿ�ϵ��
    rRTCCON = 1 ;
    RtcIsReset = rRTCCON & (1<<3);
    rRTCCON &= ~1 ;

    if(RtcIsReset == true)
    {
        DateTime.tm_us = 0;
        DateTime.tm_sec = 0;
        DateTime.tm_min = 0;
        DateTime.tm_hour = 0;
        DateTime.tm_mday = 1;
        DateTime.tm_mon  = 1;
        DateTime.tm_year = 2000;
        RTC_SetTime(&DateTime);
        Time_SetDateTime(&DateTime);
    }
    Rtc_RegisterDev(RTC_GetTime,RTC_SetTime);
    return true;
}
