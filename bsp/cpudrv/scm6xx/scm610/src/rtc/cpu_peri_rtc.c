//-----------------------------------------------------------------------------

// ģ��汾: V1.00
// ������Ա:huzb
// ����ʱ��:2018
// =============================================================================

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "os.h"
#include "cpu_peri.h"
#include "time.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern ptu32_t ModuleInstall_CpuRtc(ptu32_t para);
//    ModuleInstall_CpuRtc(0);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip rtc"//CPU��rtc��������
//parent:"rtc"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                     //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                  //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                    //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium                  //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                    //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"time","lock"//������������������������none����ʾ�������������
                                    //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                    //����������������������г����á�,���ָ�
//weakdependence:"none"             //��������������������������none����ʾ�������������
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

//��ǰ��������-20 ����

#define CN_CUURENT_CENTURY_CONFIG (2000)

typedef struct
{
    volatile u32 Time0;
    volatile u32 Time1;
    volatile u32 CurTime0;
    volatile u32 CurTime1;
    volatile u32 AlarmTime0;
    volatile u32 AlarmTime1;
    volatile u32 RESERVED0[2];
    volatile u32 Control;
    volatile u32 Status;
}tagRtcReg;

static volatile tagRtcReg *sgpt_RtcReg = (volatile tagRtcReg *)0x40900000UL;


#define LSE_Flag_Reg 0xA5A5   //��־���������֮��
#define LSI_Flag_Reg 0xA5A0   //��־

#define BAK_Reg   2  //??

#define HexToBcd(x) ((((x) / 10) <<4) + ((x) % 10))  //16����ת����BCD
#define BcdToHex(x) ((((x) & 0xF0) >>4) * 10 + ((x) & 0x0F)) //BCDת����16����

static struct SemaphoreLCB *pRtcSemp  = NULL;
static s64  UpdateTime = 0;                     //����µ�ʱ��

// =============================================================================
// ���ܣ�RTC��������SRAM
// ������BKPx:�����Ĵ������,��Χ:0~19
// ���أ�����������
// =============================================================================

u32 BKP_ReadBackupRegister(u32 BKRx)
{

}

// =============================================================================
// ���ܣ�RTCд�������SRAM
// ������BKPx:�����Ĵ������,��Χ:0~19
//     data:Ҫд�������,32λ����
// ���أ�true,�����������������
// =============================================================================

void BKP_WriteBackupRegister(u32 BKRx,u32 data)
{

}
// =============================================================================
// ���ܣ��ȴ�RSFͬ������ֹ��ͬ��ʱ��ȡ������
// ������void
// ���أ�
// =============================================================================
bool_t RTC_Wait_Rsf(void)
{

}

// =============================================================================
// ���ܣ���RTC�����ʼ��ģʽ
// ��������
// ���أ�true,�����������������
// =============================================================================
static bool_t RTC_Init_Mode(void)
{
    u32 tmp = 0;

    //�ر�Rtc
    sgpt_RtcReg->Control &= ~(1<<0);

    //�ȴ�Rtcʧ����Ч
    while(sgpt_RtcReg->Status & (1<<0));

    //�������Լ��������ж�
    sgpt_RtcReg->Status |= (1<<4)|(1<<5);

    return true;
}
// =============================================================================
// ���ܣ���RTC�豸�ж�ȡRTCʱ�䣬��λ΢��,ȡ��1970��1��1��0:0:0�����ڵ�ʱ���
// ������time, ʱ��ֵ���������ʱ��ת����1970��1��1��0:0:0�����ڵ�ʱ���
// ���أ�true,�����������������
// =============================================================================
bool_t RTC_GetTime(s64 *time)
{
    struct tm dtm;
    u32 temp1,temp2;
    u32 year,month,date,hour,min,sec;
    u8 secondU,secondD;
    u8 minuteU,minuteD;
    u8 hourU,hourD;
    u8 weekDay;
    u8 dayU,dayD;
    u8 monthU,monthD;
    u8 yearU,yearD;

    //��ȡ��ǰ�Ĵ���
    temp1 = sgpt_RtcReg->CurTime0;
    temp2 = sgpt_RtcReg->CurTime1;

    //��32timerΪʱ�����Լ�������Ϣ
    secondU =  temp1 & 0xf;                   //��ȡsec��Ϣ
    secondD = (temp1 & (0x7 << 0x4)) >> 0x4;

    minuteU = (temp1 & (0xf << 0x8)) >> 0x8;  //��ȡmin��Ϣ  0~6��Ч
    minuteD = (temp1 & (0x7 << 12)) >> 12;

    hourU   = (temp1 & (0xF << 16)) >> 16;
    hourD   = (temp1 & (0x3 << 20)) >> 20;    //��ȡhour��Ϣ

    weekDay = (temp1 & (0x7 << 24)) >> 24;    //��ȡ������Ϣ

    //��32Bit��¼����Լ��·� ����Ϣ
    dayU    =  temp2 & 0xf;
    dayD    = (temp2 & 0x3 << 4)   >>4;

    monthU  = (temp2 & (0xF << 8)) >>8;
    monthD  = (temp2 & (0x1 << 12))>>12;

    yearU   = (temp2 & (0xF << 16))>>16;
    yearD   = (temp2 & (0xF << 20))>>20;

    //��Ϊ��ȡ�����ʧ������������(20)�Ĳ���-�ʻ�ԭ�������ʱ��Ӧ�ü���������

    //��ʱ��ת��Ϊ�ꡢ�¡��ա�ʱ���֡��� - ����
    year   = yearD * 10  + yearU + CN_CUURENT_CENTURY_CONFIG;
    month  = monthD * 10 + monthU;
    date   = dayD * 10 + dayU;

    hour   = hourD * 10 + hourU;
    min    = minuteD * 10 + minuteU;
    sec    = secondD * 10 + secondU;

    u32 tim_us = 0;

    dtm.tm_year = year;
    dtm.tm_mon  = month;
    dtm.tm_mday = date;
    dtm.tm_hour = hour;
    dtm.tm_min  = min;
    dtm.tm_sec  = sec;

    *time = (s64)(1000000 * Time_MkTime(&dtm)+tim_us);
    return true;
}

// =============================================================================
// ���ܣ�����RTC�豸RTCʱ�䣬��λ΢�룬��ʱ���1970��1��1��0:0:0�����ڵ�ʱ���
// ������time, ʱ��ֵ
// ���أ�true,�����������������
// =============================================================================
bool_t RTC_SetTime(s64 time)
{
    atom_low_t  atom_bak;

    atom_bak = Int_LowAtomStart();
    UpdateTime = time;
    Int_LowAtomEnd(atom_bak);
    Lock_SempPost(pRtcSemp);  //����һ���źŵ�
    return true;
}

static bool_t __Rtc_SetTime(s64 time)
{
    struct tm dtm;
    s64 time_s;
    u8 tm_wday;
    u32 year,month,date,hour,min,sec;
    u8 secondU,secondD;
    u8 minuteU,minuteD;
    u8 hourU,hourD;
    u8 weekDay;
    u8 dayU,dayD;
    u8 monthU,monthD;
    u8 yearU,yearD;
    u32 timeValSet = 0;

    time_s = time/1000000;
    Time_LocalTime_r(&time_s,&dtm);



    weekDay = tm_wday=dtm.tm_wday;

    //�ر�Rtc
    sgpt_RtcReg->Control &= ~(1<<0);

    //�ȴ�Rtcʧ����Ч
    while(sgpt_RtcReg->Status & (1<<0));

    //�������Լ��������ж�
    sgpt_RtcReg->Status |= (1<<4)|(1<<5);

    year   = dtm.tm_year-CN_CUURENT_CENTURY_CONFIG;
    yearD  = year / 10;
    yearU  = year % 10;

    month  = dtm.tm_mon;
    monthD = dtm.tm_mon /10;
    monthU  = dtm.tm_mon %10;

    date   = dtm.tm_mday;
    dayD   = date /10;
    dayU   = date %10;

    hour   = dtm.tm_hour;
    hourD  = hour /10;
    hourU  = hour %10;

    min    = dtm.tm_min;
    minuteD= min /10;
    minuteU= min %10;

    sec    = dtm.tm_sec;
    secondD= sec /10;
    secondU= sec %10;

    //����д��λ���Ĵ���Time0
    timeValSet |= secondU;          //set sec_u
    timeValSet |= (secondD << 0x4);

    timeValSet |= (minuteU << 0x8); //set min_u
    timeValSet |= (minuteD << 12);

    timeValSet |= (hourU << 16);
    timeValSet |= (hourD << 20);      //set hour_u

    timeValSet |= (weekDay<<24);      //set weekday

    sgpt_RtcReg->Time0 = timeValSet;
    timeValSet = 0;

    //����д��λ���Ĵ���Time1
    timeValSet |= dayU;             //set day_u
    timeValSet |= (dayD<<4);

    timeValSet |= (monthU << 8);        //set month_u
    timeValSet |= (monthD << 12);

    timeValSet |= (yearU << 16);        //set year_u
    timeValSet |= (yearD << 20);

    sgpt_RtcReg->Time1 = timeValSet;

    //����ʹ��Rtc
    sgpt_RtcReg->Control |= (1<<0);

    //�ȴ�Rtc����
    while(!(sgpt_RtcReg->Status & (1<<0)));


    return true;
}

// =============================================================================
// ���ܣ�RTCʱ���������
//     ר����һ�������ȼ���������Ϊ����RTC�����Է�ռ�������̵߳�CPUʱ��
// ��������
// ���أ���
// =============================================================================
ptu32_t Rtc_UpdateTime(void)
{

    while(1)
    {
        if(Lock_SempPend(pRtcSemp,CN_TIMEOUT_FOREVER))  //����һյ�źŵ�
        {
            __Rtc_SetTime(UpdateTime);
        }
    }
    return 0;
}

// =============================================================================
// ���ܣ�RTCӲ����ʼ��
//     ���ȼ��LSE�Ƿ��ܿ���,���������ʹ��LSIʱ��
// ������void
// ���أ�true,�����������������
// =============================================================================

static bool_t RTC_Configuration(void)
{

    IO_Configure_PullDown(P_GPIO109, DISABLE);
    IO_Configure_PullDown(P_GPIO110, DISABLE);
    XTAL32_InitPad();
    EXTAL32_InitPad();
    CLK_Enable_XLOSC(ENABLE);

    //�ر�Rtc
    sgpt_RtcReg->Control &= ~(1<<0);

    //�ȴ�Rtcʧ����Ч
    while(sgpt_RtcReg->Status & (1<<0));

    //�������Լ��������ж�
    sgpt_RtcReg->Status |= (1<<4)|(1<<5);

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
    u16 evtt;

    RTC_Configuration();    //����RTC
    //��ʼ��Ӳ��

    pRtcSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"RTC_SEMP"); //����һ���ź���

    if(NULL == pRtcSemp)
        return false;

    evtt = DJY_EvttRegist(EN_CORRELATIVE,CN_PRIO_REAL,0,0,
                            Rtc_UpdateTime,NULL,800,
                                "RTC Update Event");
    //�Ǽ�һ���¼����ͣ�������ʱ�����ͣ����ȼ�130����ں���Rtc_UpdateTime��ջ�ߴ�800

    if(evtt == CN_EVTT_ID_INVALID)
    {
        free(pRtcSemp);
        return false;
    }

    DJY_EventPop(evtt,NULL,0,NULL,0,0);
    //�����¼�

    RTC_GetTime(&rtc_time);  //��RTC�豸�ж�ȡRTCʱ�䣬��λ��us,
                             //��ȡ��ֵ�ŵ�&rtc_time��

    tv.tv_sec  = rtc_time/1000000;   //�Ѷ�ȡ��ʱ����s�Ĳ�������
    tv.tv_usec = rtc_time%1000000;   //�Ѷ�ȡ��ʱ����us�Ĳ�������(����)

    settimeofday(&tv,NULL);      //????

    //ע��RTCʱ��
    if(!Rtc_RegisterDev(RTC_GetTime,RTC_SetTime))
        return false;
    return true;
}







