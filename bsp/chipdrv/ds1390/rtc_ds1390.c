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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
//所属模块：RTC时钟模块
//作者：hm
//版本：V0.1.0
//文件描述：硬件RTC连接及RTC底层驱动
//其他说明：
//修订历史：
//1. 日期: 2014-04-12
//   作者：
//   新版本号：V0.1.0
//   修改说明：原始版本
//------------------------------------------------------
#include "stdint.h"
#include "cpu_peri.h"
#include "stdio.h"
#include "rtc_ds1390.h"
#include "board_config.h"
#include "spibus.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern ptu32_t ModuleInstall_RTC(void)
//    ModuleInstall_RTC();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"RTC chip ds1390"//spi接口的rtc
//parent:"spi bus"   //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"spi bus","time","cpu onchip spi"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_RTC_CHIP_DS1390 == false )
//#warning  " RTC_chip_ds1390  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_RTC_CHIP_DS1390    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_DS1390_BUS_NAME              "SPI0"            //"SPI总线名称",DS1390使用的SPI总线名称
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure


#define HexToBcd(x) ((((x) / 10) <<4) + ((x) % 10))            //将16进制转换成BCD码
#define BcdToHex(x) ((((x) & 0xF0) >>4) * 10 + ((x) & 0x0F))   //将BCD码转换成16进制

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
//-----读RTC时钟---------------------------------------------------------------
//功能：读RTC时钟模块的相应寄存器，如读秒、分、时、年、月、日等
//参数：reg,相应的寄存器号，根据ds1390pdf文档可知，读命令与写命令相差0x80，即，
//          例如，读秒命令为0x01时，写秒命令为0x81,此参数只需传递0x01即可
//返回：获取的数值，为BCD码
//-----------------------------------------------------------------------------
static unsigned char __rtc_read (unsigned char reg)
{
    unsigned char ret;

    _ds1390_cs_active();
    _ds1390_TxRx(&reg,1,&ret,1);
    _ds1390_cs_inactive();

    return (int)ret< 0 ? 0 : ret;
}

//-----写RTC时钟---------------------------------------------------------------
//功能：写RTC时钟模块的相应寄存器
//参数：reg,相应的寄存器号，根据ds1390pdf文档可知，读命令与写命令相差0x80，即，
//          例如，读秒命令为0x01时，写秒命令为0x81,此参数只需传递0x01即可
//      val,写到寄存器中的数值，写入的数据为BCD码
//返回：无
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

//-----更新RTC时钟---------------------------------------------------------------
//功能：将年月日时分秒写入到RTC时钟，更新RTC，由于RTC芯片存放的时间信息是基于BCD
//      编码格式，所以在写入时间前，需要将时间信息转化为相应的BCD格式
//参数：DateTime,rtc_tm结构类型的变量，里面存放着需更新RTC的时间信息
//返回：无
//-----------------------------------------------------------------------------
static bool_t rtc_time_get(struct tm *DateTime)
{
    uint32_t sec, min, hour, mday, wday, mon, year;

    //从RTC读时间
    sec  = __rtc_read (RTC_CMD_SECONDS);
    min  = __rtc_read (RTC_CMD_MINUTES);
    hour = __rtc_read (RTC_CMD_HOURS);
    mday = __rtc_read (RTC_CMD_DATE_OF_MONTH);
    wday = __rtc_read (RTC_CMD_DAY_OF_WEEK);
    mon  = __rtc_read (RTC_CMD_MONTH);
    year = __rtc_read (RTC_CMD_YEAR);

    //将BCD格式转化为正常模式
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
//-----更新RTC时钟---------------------------------------------------------------
//功能：将年月日时分秒写入到RTC时钟，更新RTC，由于RTC芯片存放的时间信息是基于BCD
//      编码格式，所以在写入时间前，需要将时间信息转化为相应的BCD格式
//参数：time,存放着需更新RTC的时间信息
//返回：无
//-----------------------------------------------------------------------------
bool_t RTC_SetTime(s64 time)
{
    struct tm DateTime;

    time = time/1000000;
    Time_LocalTime_r(&time,&DateTime);

    __rtc_write (RTC_CMD_SECONDS,     HexToBcd (DateTime.tm_sec));
    __rtc_write (RTC_CMD_MINUTES,     HexToBcd (DateTime.tm_min));
    __rtc_write (RTC_CMD_HOURS,       HexToBcd (DateTime.tm_hour));
    __rtc_write (RTC_CMD_DAY_OF_WEEK, HexToBcd (DateTime.tm_wday + 1));//星期几
    __rtc_write (RTC_CMD_DATE_OF_MONTH, HexToBcd (DateTime.tm_mday));
    __rtc_write (RTC_CMD_MONTH,       HexToBcd (DateTime.tm_mon));
    __rtc_write (RTC_CMD_YEAR,        HexToBcd (DateTime.tm_year- 2000));

    return true;
}

//----初始化rtc实时时钟模块------------------------------------------------------
//功能：初始化RTC模块，主要进行外部中断1的初始化
//参数：模块初始化函数没有参数
//返回：true = 成功初始化，false = 初始化失败
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

