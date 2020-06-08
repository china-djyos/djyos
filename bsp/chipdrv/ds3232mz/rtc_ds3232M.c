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
// =============================================================================

// �ļ���     ��RTC_DS3232M.c
// ģ������: RTCоƬDS3232M����
// ģ��汾: V1.00
// ������Ա: zhb
// ����ʱ��: 18/08.2017
// =============================================================================
/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/
#include "stdint.h"
#include <string.h>
#include <stdlib.h>
#include "os.h"
#include "cpu_peri.h"
#include "iicbus.h"
#include "rtc_ds3232M.h"
#include "board-config.h"
#include <time.h>
#include "shell.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//   extern ptu32_t ModuleInstall_RTCDS3232M(void);
//   ModuleInstall_RTCDS3232M( );
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"RTC chip ds3232M"//spi�ӿڵ�rtcоƬ
//parent:"none"                       //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"lock","cpu onchip iic"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_RTC_CHIP_DS3232M == false )
//#warning  " RTC_chip_ds3232M  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_RTC_CHIP_DS3232M    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@enum,true,false,
//%$#@string,1,32,
#define CFG_DS3232M_BUS_NAME              "IIC2"      //"SPI��������",DS3232Mʹ�õ�SPI��������
//%$#@num,0,255,
#define CFG_DS3232MZ_ADDR   (0xD0>>1)       //"������ַ"��
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

bool_t rtcst(char *param);
bool_t rtcgt(void);

#define HexToBcd(x) ((((x) / 10) <<4) + ((x) % 10))            //��16����ת����BCD��
#define BcdToHex(x) ((((x) & 0xF0) >>4) * 10 + ((x) & 0x0F))   //��BCD��ת����16����



static struct IIC_Device *s_ptDS3232MZ_Dev=NULL;
u32 timeout = 1000*mS;                 //1000*mS;
#define DS3232MZ_CLK_FRE             (200*1000)      //�����ٶȣ���λHz
static struct MutexLCB *  ptSemID_RTC;       // IIC���������־
static struct SemaphoreLCB *pRtcSemp  = NULL;
static s64  UpdateTime = 0;                     //����µ�ʱ��


#define DS3232M_TIME_HOUR_MODE24_MASK   0x3F
#define DS3232M_TIME_HOUR_MODE12_MASK   0x7F

#define DS3232M_ALARM_HOUR_MODE24_MASK  0xBF
#define DS3232M_ALARM_HOUR_MODE12_MASK  0xFF
#define DS3232M_ALARM_DATE_MODE_MASK    0xBF
#define DS3232M_ALARM_DAY_MODE_MASK     0xC7


//������д�ļĴ�������
#define DS3232_TIME_REG_LEN         7       //DS1390=8��msλ
#define DS3232_ALARM_REG_LEN        5       //DS1390=5��msλ,DS3232M��Alarm2.minλ��msλ


#define DS3232_TIME_ADDR    0x00

#define DS3232_REG_SECONDS  0x00
#define DS3232_REG_MINUTES  0x01
#define DS3232_REG_HOURS    0x02
#define DS3232_REG_AMPM     0x02
#define DS3232_REG_DAY      0x03
#define DS3232_REG_DATE     0x04
#define DS3232_REG_MONTH    0x05
#define DS3232_REG_CENTURY  0x05
#define DS3232_REG_YEAR     0x06
#define DS3232_REG_ALARM1   0x07    /* Alarm 1 BASE */
#define DS3232_REG_ALARM2   0x0B    /* Alarm 2 BASE */
#define DS3232_REG_CR       0x0E    /* Control register */

#define DS3232_REG_CR_nEOSC 0x80
#define DS3232_REG_CR_INTCN 0x04
#define DS3232_REG_CR_A2IE  0x02
#define DS3232_REG_CR_A1IE  0x01
#define DS3232_REG_SR       0x0F    /* control/status register */
#define DS3232_REG_SR_OSF   0x80
#define DS3232_REG_SR_BSY   0x04
#define DS3232_REG_SR_A2F   0x02
#define DS3232_REG_SR_A1F   0x01


ptu32_t Rtc_UpdateTime(void);
static bool_t RTC_GetTime(s64 *time);



// =============================================================================
// ���ܣ����������ṩ�������������ҽӵ�IIC���߽����,�������ṩIIC_Device���ƿ�
// ������BusName,�����������ҽӵ���������
//       DevName,������������
//       NewDev,��������ָ��
// ���أ����ƿ���ָ�룬���ʧ��ʱ����NULL
// =============================================================================
static s32 __rtc_ReadData(uint8_t byAddress,uint8_t *pbyBuf,uint8_t byLen)
{
    s32 ret;
    ret=IIC_Read(s_ptDS3232MZ_Dev,byAddress,pbyBuf,byLen,timeout);

    return ret;
}


// =============================================================================
// ���ܣ����������ṩ�������������ҽӵ�IIC���߽����,�������ṩIIC_Device���ƿ�
// ������BusName,�����������ҽӵ���������
//       DevName,������������
//       NewDev,��������ָ��
// ���أ����ƿ���ָ�룬���ʧ��ʱ����NULL
// =============================================================================
static s32 __rtc_WriteData(uint8_t byAddress,uint8_t *pbyBuf,uint8_t byLen)
{
    s32 ret;
    ret=IIC_Write(s_ptDS3232MZ_Dev,byAddress,pbyBuf,byLen,true,timeout);
    return ret;
}

// =============================================================================
// ���ܣ����������ṩ�������������ҽӵ�IIC���߽����,�������ṩIIC_Device���ƿ�
// ������BusName,�����������ҽӵ���������
//       DevName,������������
//       NewDev,��������ָ��
// ���أ����ƿ���ָ�룬���ʧ��ʱ����NULL
// =============================================================================
bool_t RTC_DS3232MZ_UpdateTime(struct tm *ptm)
{
    s32 ret;
    uint8_t dat[DS3232_TIME_REG_LEN];
    if(ptm==NULL)
    {
        printf("%s para is invalid.\r\n",__FUNCTION__);
        return false;
    }
    dat[0]=HexToBcd(ptm->tm_sec);
    dat[1]=HexToBcd(ptm->tm_min);
    dat[2]=HexToBcd(ptm->tm_hour);
    dat[3]=HexToBcd(ptm->tm_wday);
    dat[4]=HexToBcd(ptm->tm_mday);
    dat[5]=HexToBcd(ptm->tm_mon);
    dat[6]=HexToBcd(ptm->tm_year-2000);
    ret=__rtc_WriteData(DS3232_TIME_ADDR,dat,DS3232_TIME_REG_LEN);
    if(ret==-1)
        return false;
    return true;

}

// =============================================================================
// ���ܣ����������ṩ�������������ҽӵ�IIC���߽����,�������ṩIIC_Device���ƿ�
// ������BusName,�����������ҽӵ���������
//       DevName,������������
//       NewDev,��������ָ��
// ���أ����ƿ���ָ�룬���ʧ��ʱ����NULL
// =============================================================================
bool_t RTC_DS3232MZ_GetTime(struct tm *ptm)
{
    s32 ret;
    uint8_t dat[DS3232_TIME_REG_LEN];
    if(ptm==NULL)
    {
        printf("%s para is invalid.\r\n",__FUNCTION__);
        return false;
    }
    ret=__rtc_ReadData(DS3232_TIME_ADDR,dat,DS3232_TIME_REG_LEN);
    if(ret==-1)
        return false;
     //��BCD��ʽת��Ϊ����ģʽ
    ptm->tm_sec    = BcdToHex(dat[0] & 0x7F);
    ptm->tm_min    = BcdToHex(dat[1] & 0x7F);
    ptm->tm_hour   = BcdToHex(dat[2]);
    ptm->tm_wday   = BcdToHex(dat[3] & 0x07);
    ptm->tm_mday   = BcdToHex(dat[4] & 0x3F);
    ptm->tm_mon    = BcdToHex(dat[5] & 0x1F);
    ptm->tm_year   = BcdToHex(dat[6]) + 2000;
    return true;
}


bool_t RTC_DS3232MZ_SetAlarm1Time(struct tm *ptm)
{
    s32 ret;
    uint8_t dat[DS3232_ALARM_REG_LEN];
    if(ptm==NULL)
    {
        printf("%s para is invalid.\r\n",__FUNCTION__);
        return false;
    }
    dat[0]=HexToBcd(ptm->tm_sec)&0x7F;
    dat[1]=HexToBcd(ptm->tm_min)&0x7F;
    dat[2]=HexToBcd(ptm->tm_hour&DS3232M_ALARM_HOUR_MODE24_MASK);
    dat[3]=HexToBcd(ptm->tm_mday&DS3232M_ALARM_DATE_MODE_MASK);
    ret=__rtc_WriteData(DS3232_REG_ALARM1,dat,DS3232_ALARM_REG_LEN);
    if(ret==-1)
        return false;
    return true;
}

bool_t RTC_DS3232MZ_SetAlarm2Time(struct tm *ptm)
{
    s32 ret;
    uint8_t dat[DS3232_ALARM_REG_LEN];
    if(ptm==NULL)
    {
        printf("%s para is invalid.\r\n",__FUNCTION__);
        return false;
    }
    dat[0]=HexToBcd(ptm->tm_sec)&0x7F;;
    dat[1]=HexToBcd(ptm->tm_min)&0x7F;;
    dat[2]=HexToBcd(ptm->tm_hour&DS3232M_ALARM_HOUR_MODE24_MASK);
    dat[3]=HexToBcd(ptm->tm_mday&DS3232M_ALARM_DATE_MODE_MASK);
    ret=__rtc_WriteData(DS3232_REG_ALARM2,dat,DS3232_ALARM_REG_LEN);
    if(ret==-1)
        return false;
    return true;
}


bool_t RTC_DS3232MZ_GetAlarm1Time(struct tm *ptm)
{
    s32 ret;
    uint8_t dat[DS3232_ALARM_REG_LEN];
    if(ptm==NULL)
    {
        printf("%s para is invalid.\r\n",__FUNCTION__);
        return false;
    }
    ret=__rtc_ReadData(DS3232_REG_ALARM1,dat,DS3232_ALARM_REG_LEN);
    if(ret==-1)
        return false;
     //��BCD��ʽת��Ϊ����ģʽ
    ptm->tm_sec    = BcdToHex(dat[0] & 0x7F);
    ptm->tm_min    = BcdToHex(dat[1] & 0x7F);
    ptm->tm_hour   = BcdToHex(dat[2]);
    ptm->tm_mday   = BcdToHex(dat[3] & 0x3F);
    return true;
}

bool_t RTC_DS3232MZ_GetAlarm2Time(struct tm *ptm)
{
    s32 ret;
    uint8_t dat[DS3232_ALARM_REG_LEN];
    if(ptm==NULL)
    {
        printf("%s para is invalid.\r\n",__FUNCTION__);
        return false;
    }
    ret=__rtc_ReadData(DS3232_REG_ALARM2,dat,DS3232_ALARM_REG_LEN);
    if(ret==-1)
        return false;
     //��BCD��ʽת��Ϊ����ģʽ
    ptm->tm_sec    = BcdToHex(dat[0] & 0x7F);
    ptm->tm_min    = BcdToHex(dat[1] & 0x7F);
    ptm->tm_hour   = BcdToHex(dat[2]);
    ptm->tm_mday   = BcdToHex(dat[3] & 0x3F);
    return true;
}


static bool_t RTC_DS3232MZ_ClearAlarmSta(void)
{
    s32 ret;
    u8 dat;

    ret=__rtc_ReadData(DS3232_REG_SR,&dat,1);
    if(ret==-1)
        return false;

    dat &= (u8)(~(DS3232_REG_SR_A1F | DS3232_REG_SR_A1F));//clear;
    ret=__rtc_WriteData(DS3232_REG_SR,&dat,1);
    if(ret==-1)
        return false;
    return true;
}

static bool_t RTC_DS3232MZ_AlarmConfig(u8 alarm1,u8 alarm2)
{
    s32 ret;
    u8 dat;

    ret=__rtc_ReadData(DS3232_REG_CR,&dat,1);
    if(ret==-1)
        return false;

    dat &= ~(DS3232_REG_CR_INTCN | DS3232_REG_CR_A2IE |DS3232_REG_CR_A1IE );
    if(alarm1 || alarm2 )
    {
        dat |= DS3232_REG_CR_INTCN;
        if(alarm1)
            dat |= DS3232_REG_CR_A1IE;
        if(alarm2)
            dat |= DS3232_REG_CR_A2IE;
    }
    ret=__rtc_WriteData(DS3232_REG_CR,&dat,1);
    if(ret==-1)
        return false;

    return true;
}

void (*gfunRTC_TrigIoInit)(void) =  NULL;

bool_t RTC_SetAlarm(u32 timeout_s)
{
    bool_t result = false;
    static bool_t sAlarmSet = false;
    s64 time;
    struct tm dtm;

    if(true == RTC_DS3232MZ_ClearAlarmSta())
    {
        if(false == sAlarmSet)
        {
            RTC_DS3232MZ_AlarmConfig(1,0);
            sAlarmSet = true;
            if(NULL != gfunRTC_TrigIoInit)
                gfunRTC_TrigIoInit();
        }

        if(true == sAlarmSet)
        {
            if(true == RTC_DS3232MZ_GetTime(&dtm))
            {
                time = (s64)(Time_MkTime(&dtm) + timeout_s);

                Time_LocalTime_r(&time,&dtm);
                result = RTC_DS3232MZ_SetAlarm1Time(&dtm);

                RTC_DS3232MZ_GetAlarm1Time(&dtm);
            }
        }
    }

    return result;
}

bool_t RTC_AlarmConfig(void (*RTC_TrigIoInit)(void))
{
    bool_t result = false;
    if(NULL != RTC_TrigIoInit)
    {
        gfunRTC_TrigIoInit = RTC_TrigIoInit;
        result = true;
    }
    return result;
}

/* =============================================================================
 ���ܣ�DS3232MZоƬ��ʼ������ʼ���ͼ����豸����Ӧ��IIC����.
 ��������
 ���أ�true,�ɹ�;false,ʧ��
 =============================================================================*/

static bool_t DS3232MZ_Init(char *BusName)
{
    ptSemID_RTC = Lock_MutexCreate("DS3232MZ Lock");
    if(ptSemID_RTC==NULL)
    {
        printf("%s malloc failed.\r\n",__FUNCTION__);
        return false;
    }

    s_ptDS3232MZ_Dev = IIC_DevAdd(BusName,"DS3232MZ",CFG_DS3232MZ_ADDR,0,8);
    if(s_ptDS3232MZ_Dev)
    {
        IIC_BusCtrl(s_ptDS3232MZ_Dev,CN_IIC_SET_CLK,DS3232MZ_CLK_FRE,0);//����ʱ�Ӵ�С
        IIC_BusCtrl(s_ptDS3232MZ_Dev,CN_IIC_SET_POLL,0,0);       //ʹ���жϷ�ʽ����
        return true;
    }
    else
    {
        Lock_MutexDelete(ptSemID_RTC);
        return false;
    }
}


// =============================================================================
// ���ܣ���RTC�豸�ж�ȡRTCʱ�䣬��λ΢��,ȡ��1970��1��1��0:0:0�����ڵ�ʱ���
// ������time, ʱ��ֵ���������ʱ��ת����1970��1��1��0:0:0�����ڵ�ʱ���
// ���أ�true,�����������������
// =============================================================================
static bool_t RTC_GetTime(s64 *time)
{
    struct tm dtm;
    RTC_DS3232MZ_GetTime(&dtm);
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
    atom_low_t  atom_bak;

    atom_bak = Int_LowAtomStart();
    UpdateTime = time;
    Int_LowAtomEnd(atom_bak);
    Lock_SempPost(pRtcSemp);
    return true;
}

static bool_t __Rtc_SetTime(s64 time)
{
    struct tm dtm;
    s64 time_s;
    bool_t ret;
    time_s = time/1000000;
    Time_LocalTime_r(&time_s,&dtm);
    ret=RTC_DS3232MZ_UpdateTime(&dtm);
    return ret;
}

// =============================================================================
// ���ܣ�RTCʱ���������ר����һ����
//     ���ȼ���������Ϊ����RTC�����Է�ռ�������̵߳�CPUʱ��
// ��������
// ���أ���
// =============================================================================
ptu32_t Rtc_UpdateTime(void)
{

    while(1)
    {
        if(Lock_SempPend(pRtcSemp,CN_TIMEOUT_FOREVER))
        {
            __Rtc_SetTime(UpdateTime);
        }
    }
    return 0;
}


ptu32_t ModuleInstall_RTCDS3232M(void)
{
    u16 evtt;
    s64 rtc_time;
    struct timeval tv;

    DS3232MZ_Init(CFG_DS3232M_BUS_NAME);
    pRtcSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"RTC_SEMP");
    if(NULL == pRtcSemp)
         return -1;
    evtt = DJY_EvttRegist(EN_CORRELATIVE,CN_PRIO_REAL,0,0,
                               Rtc_UpdateTime,NULL,800,
                                   "RTC Update Event");
    if(evtt == CN_EVTT_ID_INVALID)
    {
        free(pRtcSemp);
        return false;
    }
    DJY_EventPop(evtt,NULL,0,NULL,0,0);

    RTC_GetTime(&rtc_time);

    tv.tv_sec  = rtc_time/1000000;
    tv.tv_usec = rtc_time%1000000;

    settimeofday(&tv,NULL);

//  RTC_Shell_Module_Install();
    //ע��RTCʱ��
    if(!Rtc_RegisterDev(NULL,RTC_SetTime))
        return false;
    else
        return true;
}



//void Sh_RTC_SetTime(char *param)
bool_t rtcst(char *param)
{
    char command[20];
    bool_t ret;
    struct tm ptDateTime;
    if(param!=NULL)
    {
         memcpy(command,param,20);
         sscanf(command,"%d/%d/%d,%d:%d:%d:%d",\
                     &ptDateTime.tm_year,&ptDateTime.tm_mon,&ptDateTime.tm_mday,\
                     &ptDateTime.tm_hour,&ptDateTime.tm_min,&ptDateTime.tm_sec);
         ret=RTC_DS3232MZ_UpdateTime(&ptDateTime);
         if(ret)
         {
             printf("Update Time OK.\r\n");
         }
         else
         {
             printf("Update Time failed.\r\n");
         }
    }

    return true;

}

//void Sh_RTC_GetTime(void)
bool_t rtcgt(void)
{
    struct tm ptDateTime;
    bool_t ret;
    ret=RTC_DS3232MZ_GetTime(&ptDateTime);
    if(!ret)
    {
         printf("Get RTC Time failed.\r\n");
    }
    else
    {
         printf("Get RTC Time:%04d/%02d/%02d,%02d:%02d:%02d.\r\n",
                ptDateTime.tm_year, ptDateTime.tm_mon, ptDateTime.tm_mday,
                ptDateTime.tm_hour, ptDateTime.tm_min,ptDateTime.tm_sec);
    }
   return true;
}


ADD_TO_ROUTINE_SHELL(rtcst,rtcst,"����RTCʱ��   ��ʽ:2017/08/20,21:00:00");
ADD_TO_ROUTINE_SHELL(rtcgt,rtcgt,"��ȡRTCʱ��");
