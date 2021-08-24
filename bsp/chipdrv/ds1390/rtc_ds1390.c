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
//����ģ�飺RTCʱ��ģ��
//���ߣ�hm
//�汾��V0.1.0
//�ļ�������Ӳ��RTC���Ӽ�RTC�ײ�����
//����˵����
//�޶���ʷ��
//1. ����: 2014-04-12
//   ���ߣ�
//   �°汾�ţ�V0.1.0
//   �޸�˵����ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "cpu_peri.h"
#include "stdio.h"
#include "rtc_ds1390.h"
#include "board_config.h"
#include "spibus.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern ptu32_t ModuleInstall_RTC(void)
//    ModuleInstall_RTC();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"RTC chip ds1390"//spi�ӿڵ�rtc
//parent:"spi bus"   //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"spi bus","time","cpu onchip spi"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_RTC_CHIP_DS1390 == false )
//#warning  " RTC_chip_ds1390  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_RTC_CHIP_DS1390    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_DS1390_BUS_NAME              "SPI0"            //"SPI��������",DS1390ʹ�õ�SPI��������
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure


#define HexToBcd(x) ((((x) / 10) <<4) + ((x) % 10))            //��16����ת����BCD��
#define BcdToHex(x) ((((x) & 0xF0) >>4) * 10 + ((x) & 0x0F))   //��BCD��ת����16����

#define RTC_CMD_SECONDS         (0x01)
#define RTC_CMD_MINUTES         (0x02)
#define RTC_CMD_HOURS           (0x03)
#define RTC_CMD_DAY_OF_WEEK     (0x04)
#define RTC_CMD_DATE_OF_MONTH   (0x05)
#define RTC_CMD_MONTH           (0x06)
#define RTC_CMD_YEAR            (0x07)

static struct SPI_Device *sptDS1390Dev;
#define DS1390_SPI_SPEED      (10*1000*1000)
#define DS1390_SPI_TIMEOUT    (10*1000)


static void _ds1390_cs_active(void)
{
    SPI_CsActive(sptDS1390Dev,DS1390_SPI_TIMEOUT);
}
static void _ds1390_cs_inactive(void)
{
    SPI_CsInactive(sptDS1390Dev);
}
static u32 _ds1390_TxRx(u8* sdata,u32 slen,u8* rdata, u32 rlen)
{
    struct SPI_DataFrame data;
    s32 result;

    data.RecvBuf = rdata;
    data.RecvLen = rlen;
    data.RecvOff = slen;
    data.SendBuf = sdata;
    data.SendLen = slen;

    result = SPI_Transfer(sptDS1390Dev,&data,true,DS1390_SPI_TIMEOUT);
    if(result != CN_SPI_EXIT_NOERR)
        return 0;
    return 1;
}
//-----��RTCʱ��---------------------------------------------------------------
//���ܣ���RTCʱ��ģ�����Ӧ�Ĵ���������롢�֡�ʱ���ꡢ�¡��յ�
//������reg,��Ӧ�ļĴ����ţ�����ds1390pdf�ĵ���֪����������д�������0x80������
//          ���磬��������Ϊ0x01ʱ��д������Ϊ0x81,�˲���ֻ�贫��0x01����
//���أ���ȡ����ֵ��ΪBCD��
//-----------------------------------------------------------------------------
static unsigned char __rtc_read (unsigned char reg)
{
    unsigned char ret;

    _ds1390_cs_active();
    _ds1390_TxRx(&reg,1,&ret,1);
    _ds1390_cs_inactive();

    return (int)ret< 0 ? 0 : ret;
}

//-----дRTCʱ��---------------------------------------------------------------
//���ܣ�дRTCʱ��ģ�����Ӧ�Ĵ���
//������reg,��Ӧ�ļĴ����ţ�����ds1390pdf�ĵ���֪����������д�������0x80������
//          ���磬��������Ϊ0x01ʱ��д������Ϊ0x81,�˲���ֻ�贫��0x01����
//      val,д���Ĵ����е���ֵ��д�������ΪBCD��
//���أ���
//-----------------------------------------------------------------------------
static void __rtc_write (unsigned char reg, unsigned char val)
{
    unsigned char dout[2];  /* SPI Output Data Bytes */

    dout[0] = 0x80 | reg;
    dout[1] = val;

    _ds1390_cs_active();
    _ds1390_TxRx(dout,2,NULL,0);
    _ds1390_cs_inactive();
}

//-----����RTCʱ��---------------------------------------------------------------
//���ܣ���������ʱ����д�뵽RTCʱ�ӣ�����RTC������RTCоƬ��ŵ�ʱ����Ϣ�ǻ���BCD
//      �����ʽ��������д��ʱ��ǰ����Ҫ��ʱ����Ϣת��Ϊ��Ӧ��BCD��ʽ
//������DateTime,rtc_tm�ṹ���͵ı������������������RTC��ʱ����Ϣ
//���أ���
//-----------------------------------------------------------------------------
static bool_t rtc_time_get(struct tm *DateTime)
{
    uint32_t sec, min, hour, mday, wday, mon, year;

    //��RTC��ʱ��
    sec  = __rtc_read (RTC_CMD_SECONDS);
    min  = __rtc_read (RTC_CMD_MINUTES);
    hour = __rtc_read (RTC_CMD_HOURS);
    mday = __rtc_read (RTC_CMD_DATE_OF_MONTH);
    wday = __rtc_read (RTC_CMD_DAY_OF_WEEK);
    mon  = __rtc_read (RTC_CMD_MONTH);
    year = __rtc_read (RTC_CMD_YEAR);

    //��BCD��ʽת��Ϊ����ģʽ
    DateTime->tm_sec    = BcdToHex(sec & 0x7F);
    DateTime->tm_min    = BcdToHex(min & 0x7F);
    DateTime->tm_hour   = BcdToHex(hour);
    DateTime->tm_mday   = BcdToHex(mday & 0x3F);
    DateTime->tm_wday   = BcdToHex(wday & 0x07) - 1;
    DateTime->tm_mon    = BcdToHex(mon & 0x1F);
    DateTime->tm_year   = BcdToHex(year) + 2000;

/*---------------------test use only----------------------*/
    printf("Get RTC year: %04d mon: %02d mday: %02d wday: %02d "
           "hr: %02d min: %02d sec: %02d\r\n",
           DateTime->tm_year, DateTime->tm_mon, DateTime->tm_mday,
           DateTime->tm_wday, DateTime->tm_hour, DateTime->tm_min,
           DateTime->tm_sec);
/*---------------------test use only----------------------*/

    return 1;
}
//-----����RTCʱ��---------------------------------------------------------------
//���ܣ���������ʱ����д�뵽RTCʱ�ӣ�����RTC������RTCоƬ��ŵ�ʱ����Ϣ�ǻ���BCD
//      �����ʽ��������д��ʱ��ǰ����Ҫ��ʱ����Ϣת��Ϊ��Ӧ��BCD��ʽ
//������time,����������RTC��ʱ����Ϣ
//���أ���
//-----------------------------------------------------------------------------
bool_t RTC_SetTime(s64 time)
{
    struct tm DateTime;

    time = time/1000000;
    Time_LocalTime_r(&time,&DateTime);

    __rtc_write (RTC_CMD_SECONDS,     HexToBcd (DateTime.tm_sec));
    __rtc_write (RTC_CMD_MINUTES,     HexToBcd (DateTime.tm_min));
    __rtc_write (RTC_CMD_HOURS,       HexToBcd (DateTime.tm_hour));
    __rtc_write (RTC_CMD_DAY_OF_WEEK, HexToBcd (DateTime.tm_wday + 1));//���ڼ�
    __rtc_write (RTC_CMD_DATE_OF_MONTH, HexToBcd (DateTime.tm_mday));
    __rtc_write (RTC_CMD_MONTH,       HexToBcd (DateTime.tm_mon));
    __rtc_write (RTC_CMD_YEAR,        HexToBcd (DateTime.tm_year- 2000));

    return true;
}

//----��ʼ��rtcʵʱʱ��ģ��------------------------------------------------------
//���ܣ���ʼ��RTCģ�飬��Ҫ�����ⲿ�ж�1�ĳ�ʼ��
//������ģ���ʼ������û�в���
//���أ�true = �ɹ���ʼ����false = ��ʼ��ʧ��
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_RTC(void)
{
    struct tm DateTime;
    struct timeval tv;

    if(sptDS1390Dev = SPI_DevAdd(CFG_DS1390_BUS_NAME,"RTC_DS1390",0,8,\
            SPI_MODE_0,SPI_SHIFT_MSB,DS1390_SPI_SPEED,false))
    {
        SPI_BusCtrl(s_ptAT45_Dev,CN_SPI_SET_POLL,0,0);
    }
    else
    {
        printf("\r\n: error  : spi device : DS1390 init failed.\n\r");
        return false;
    }

    if(true == rtc_time_get(&DateTime))
    {
        tv.tv_sec  = DateTime.tm_sec ;
        tv.tv_usec = DateTime.tm_us ;

        settimeofday(&tv,NULL);
        if(!Rtc_RegisterDev(NULL,RTC_SetTime))
            return false;
        else
            return true;
    }

    return 1;
}

