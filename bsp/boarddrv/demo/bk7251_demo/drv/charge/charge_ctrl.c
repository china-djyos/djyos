
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
// Copyright (c) 2014 著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合以下二条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、此三条件表列，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、此三条件表列，以及下述
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
#include <cpu_peri_adc.h>
#include <djyos.h>

extern int Get_StabilizeVol(void);
char vol_to_percentage(int assign)
{
    int vol;
    char percentage = 0;
    static char critical = 0;
    if(assign == 0)
    {
        if(usb_is_pluged() != 0)
        {
            vol = Get_StabilizeVol();
        }
        else
        {
            vol = vbat_voltage_get();
        }
//        printf("测量到的Vbat = %d  \r\n", vol);    //显示当前的电池电压
    }
    else
    {
        vol = assign;
//        printf("指定的Vbat = %d  \r\n", vol);    //显示当前的电池电压
    }
    if(vol >= 3930)     //目前做了充电的临界。还没做放电的临界
    {
//        if(vol >= 4160)
//            percentage = 100;
//        else if(vol >= 4150)
//            percentage = 99;
//        else if(vol >= 4145)
//            percentage = 98;
//        else if(vol >= 4140)
//            percentage = 97;
//        else if(vol >= 4130)
//            percentage = 96;
//        else if(vol >= 4120)
//            percentage = 95;
//        else if(vol >= 4115)
//            percentage = 94;
//        else if(vol >= 4110)
//            percentage = 93;
//        else if(vol >= 4100)
//            percentage = 92;
//        else if(vol >= 4100)
//            percentage = 92;
//        else if(vol >= 4090)
//            percentage = 91;
//        else if(vol >= 4080)
//            percentage = 90;
//        else if(vol >= 4070)
//            percentage = 89;
//        else if(vol >= 4060)
//            percentage = 88;
//        else if(vol >= 4050)
//            percentage = 87;
//        else if(vol >= 4040)
//            percentage = 86;
//        else if(vol >= 4030)
//            percentage = 85;
//        else if(vol >= 4020)
//            percentage = 84;
//        else if(vol >= 4015)
//            percentage = 83;
//        else if(vol >= 4010)
//            percentage = 82;
//        else if(vol >= 4005)
//            percentage = 81;
//        else if(vol >= 3970)
//            percentage = 80;
//        else if(vol >= 3960)
//            percentage = 79;
//        else if(vol >= 3950)
//            percentage = 78;
//        else if(vol >= 3945)
//            percentage = 77;
//        else if(vol >= 3940)
//            percentage = 76;
//        else if(vol >= 3930)
//            percentage = 75;
        if(critical < 100)
        {
            if(vol > 4155)
            {
                percentage = 100;
                critical = 100;
                return percentage;
            }
        }
        else
        {
            percentage = 100;
            critical = 100;
            return percentage;
        }
    }
//    else if(vol >= 3790)
    if(vol >= 3790)
    {
//        if(vol >= 3925)
//            percentage = 74;
//        else if(vol >= 3920)
//            percentage = 73;
//        else if(vol >= 3915)
//            percentage = 72;
//        else if(vol >= 3908)
//            percentage = 71;
//        else if(vol >= 3900)
//            percentage = 70;
//        else if(vol >= 3890)
//            percentage = 69;
//        else if(vol >= 3885)
//            percentage = 68;
//        else if(vol >= 3880)
//            percentage = 67;
//        else if(vol >= 3875)
//            percentage = 66;
//        else if(vol >= 3870)
//            percentage = 65;
//        else if(vol >= 3865)
//            percentage = 64;
//        else if(vol >= 3860)
//            percentage = 63;
//        else if(vol >= 3855)
//            percentage = 62;
//        else if(vol >= 3848)
//            percentage = 61;
//        else if(vol >= 3840)
//            percentage = 60;
//        else if(vol >= 3835)
//            percentage = 59;
//        else if(vol >= 3830)
//            percentage = 58;
//        else if(vol >= 3825)
//            percentage = 57;
//        else if(vol >= 3818)
//            percentage = 56;
//        else if(vol >= 3810)
//            percentage = 55;
//        else if(vol >= 3805)
//            percentage = 54;
//        else if(vol >= 3800)
//            percentage = 53;
//        else if(vol >= 3797)
//            percentage = 52;
//        else if(vol >= 3793)
//            percentage = 51;
//        else if(vol >= 3790)
//            percentage = 50;
        if(critical < 75)
        {
            if(vol > 3848)
            {
                percentage = 75;
                critical = 75;
                return percentage;
            }
        }
        else
        {
            percentage = 75;
            critical = 75;
            return percentage;
        }
    }
//    else if(vol >= 3720)
    if(vol >= 3720)
    {
//        if(vol >= 3786)
//            percentage = 49;
//        else if(vol >= 3782)
//            percentage = 48;
//        else if(vol >= 3778)
//            percentage = 47;
//        else if(vol >= 3774)
//            percentage = 46;
//        else if(vol >= 3770)
//            percentage = 45;
//        else if(vol >= 3767)
//            percentage = 44;
//        else if(vol >= 3763)
//            percentage = 43;
//        else if(vol >= 3760)
//            percentage = 42;
//        else if(vol >= 3757)
//            percentage = 41;
//        else if(vol >= 3754)
//            percentage = 40;
//        else if(vol >= 3751)
//            percentage = 39;
//        else if(vol >= 3748)
//            percentage = 38;
//        else if(vol >= 3745)
//            percentage = 37;
//        else if(vol >= 3743)
//            percentage = 36;
//        else if(vol >= 3740)
//            percentage = 35;
//        else if(vol >= 3738)
//            percentage = 34;
//        else if(vol >= 3736)
//            percentage = 33;
//        else if(vol >= 3734)
//            percentage = 32;
//        else if(vol >= 3732)
//            percentage = 31;
//        else if(vol >= 3730)
//            percentage = 30;
//        else if(vol >= 3728)
//            percentage = 29;
//        else if(vol >= 3726)
//            percentage = 28;
//        else if(vol >= 3724)
//            percentage = 27;
//        else if(vol >= 3722)
//            percentage = 26;
//        else if(vol >= 3720)
//            percentage = 25;
        if(critical < 50)
        {
            if(vol > 3743)
            {
                percentage = 50;
                critical = 50;
                return percentage;
            }
        }
        else
        {
            percentage = 50;
            critical = 50;
            return percentage;
        }
    }
//    else if(vol >= 3630)
    if(vol >= 3630)
    {
//        if(vol >= 3718)
//            percentage = 24;
//        else if(vol >= 3716)
//            percentage = 23;
//        else if(vol >= 3714)
//            percentage = 22;
//        else if(vol >= 3712)
//            percentage = 21;
//        else if(vol >= 3710)
//            percentage = 20;
//        else if(vol >= 3706)
//            percentage = 19;
//        else if(vol >= 3702)
//            percentage = 18;
//        else if(vol >= 3698)
//            percentage = 17;
//        else if(vol >= 3694)
//            percentage = 16;
//        else if(vol >= 3690)
//            percentage = 15;
//        else if(vol >= 3680)
//            percentage = 14;
//        else if(vol >= 3670)
//            percentage = 13;
//        else if(vol >= 3660)
//            percentage = 12;
//        else if(vol >= 3655)
//            percentage = 11;
//        else if(vol >= 3650)
//            percentage = 10;
//        else if(vol >= 3645)
//            percentage = 9;
//        else if(vol >= 3640)
//            percentage = 8;
//        else if(vol >= 3637)
//            percentage = 7;
//        else if(vol >= 3633)
//            percentage = 6;
//        else if(vol >= 3630)
//            percentage = 5;
//        else if(vol >= 3620)
//            percentage = 4;
//        else if(vol >= 3610)
//            percentage = 3;
//        else if(vol >= 3600)
//            percentage = 2;
//        else if(vol >= 3590)
//            percentage = 1;
        if(critical < 25)
        {
            if(vol > 3694)
            {
                percentage = 25;
                critical = 25;
                return percentage;
            }
        }
        else
        {
            percentage = 25;
            critical = 25;
            return percentage;
        }
    }
//    else if(vol >= 3590)
    if(vol >= 3590)
    {
        percentage = 1;
    }
    else if((vol < 3310) && (usb_is_pluged() == 0))
    {
        if(vol >= 2700)     //防止一些异常电压导致休眠
        {
            printf("电量不足，准备关机\r\n");
            Djy_EventDelay(3000*1000);
            CloseScreen();
            LP_DeepSleep();
        }
        else
        {
            percentage = -1; //异常电压，返回这个，电量显示不做响应。
        }
    }
    return percentage;

}
