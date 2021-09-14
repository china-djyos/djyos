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
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <version.h>
#include <time.h>
#include <systime.h>
#include "../include/shell.h"

// ============================================================================
// 功能：显示os的版本
// 参数：
// 返回：
// 备注：
// ============================================================================

bool_t djyos_ver(char *param)
{
    printf("\r\n%s\r\n", djyos_kernel_version);
    if(param)
        return true;
    return true;

}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
// ============================================================================
// 功能：显示或者设置日期
// 参数：
// 返回：
// 备注：
// ============================================================================
bool_t date(char *param)
{
    s64 nowtime;
    struct tm dtm;
    char command[20];
    char buf[12];
    int res;

    nowtime = Time_Time(NULL);
    Time_LocalTime_r(&nowtime,&dtm);
    Time_AscTime(&dtm,command);

    printf("\r\n当前时间：%10.10s %s",command, g_cTmWdays[dtm.tm_wday]);
    printf("\r\n输入新日期：");

    fgets(buf,11,stdin);
    if(strlen(buf) != 0)
    {
        memcpy(command,buf,10);
        res = Time_SetDateTimeStr(command);
        switch (res)
        {
        case EN_CLOCK_YEAR_ERROR:
            printf("年份错误。");
            break;
        case EN_CLOCK_MON_ERROR:
            printf("月份错误。");
            break;
        case EN_CLOCK_DAY_ERROR:
            printf("日期错误。");
            break;
        case EN_CLOCK_FMT_ERROR:
            printf("格式错误。");
            break;
        default:
            break;
        }
    }

    printf("\r\n");
    return true;
}

// ============================================================================
// 功能：显示当前时间或者设置输入新时间
// 参数：
// 返回：
// 备注：
// ============================================================================
bool_t __time(char *param)
{
    s64 nowtime;
    struct tm dtm;
    char command[20];
    int res;

    nowtime = Time_Time(NULL);
    Time_LocalTime_r(&nowtime,&dtm);
    Time_AscTime(&dtm,command);

    printf("\r\n当前时间：%s",command+11);
    printf("\r\n输入新时间：");

    fgets(command+11,9,stdin);
    if(strlen(command+11) != 0)
    {
        res = Time_SetDateTimeStr(command);
        switch (res)
        {
        case EN_CLOCK_HOUR_ERROR:
            printf("小时错误。");
            break;
        case EN_CLOCK_MIN_ERROR:
            printf("分钟错误。");
            break;
        case EN_CLOCK_SEC_ERROR:
            printf("秒钟错误。");
            break;
        case EN_CLOCK_FMT_ERROR:
            printf("格式错误。");
            break;
        default:
            break;
        }
    }

    printf("\r\n");
    return true;
}
#pragma GCC diagnostic pop

// ============================================================================
// 功能：显示内存，显示某地址开始的一段内存，每行显示16个单元，只显示，不能修改
// 参数：param -- 参数串，本命令需要三个参数，用空格隔开
//               参数1:起始地址，10进制或16进制(0x开头)
//               参数2:显示的单元数
//               参数3:每单元字节数，合法值是1、2、4，其他数值将返回错误
//               参数4:待填充的内容
// 返回：true=正常显示，false=错误
// 备注：
// ============================================================================

bool_t d(char *param)
{
    ptu32_t addr;
    uint32_t unit_num,unit_bytes,len;
    char *word_addr,*word_un,*word_ub,*word_trail,*next_param;

    //提取3个参数
    word_addr = shell_inputs(param,&next_param);
    word_un = shell_inputs(next_param,&next_param);
    word_ub = shell_inputs(next_param,&next_param);
    word_trail = shell_inputs(next_param,&next_param);
    if((word_addr == NULL)||(word_un == NULL)
            ||(word_ub == NULL)||(word_trail != NULL))
    {
        printf("\r\n格式错误，正确格式是：\r\n>d 地址 单元数 每单元字节数\r\n");
        return false;
    }
    addr = strtoul(word_addr, (char **)NULL, 0);
    unit_num = strtol(word_un, (char **)NULL, 0);
    unit_bytes = strtol(word_ub, (char **)NULL, 0);
#if (CN_BYTE_BITS == 8)  //字节位宽=8，最常见的情况
    switch(unit_bytes)
    {
        case 1:
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%02x ", *(uint8_t*)addr);
                addr ++;
                if(addr %8 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
            break;
        case 2:
            addr &= ~(ptu32_t)1;//只能从偶数开始
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%04x ", *(u16*)addr);
                addr +=2;
                if(addr %16 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
            break;
        case 4:
            addr &= ~(ptu32_t)3;//只能从4字节对齐开始
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%08x ", *(uint32_t*)addr);
                addr +=4;
                if(addr %16 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
            break;
        case 8:
            addr &= ~(ptu32_t)7;//只能从8字节对齐开始
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%08x", *(uint32_t*)addr);
                addr +=4;
                printf("%08x ", *(uint32_t*)addr);
                addr +=4;
                if(addr %32 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
            break;
        default :
            printf("\r\n参数错误\r\n");
            return false;
            break;
    }
#elif (CN_BYTE_BITS == 16)  //字节位宽=16
    switch(unit_bytes)
    {
        case 1:
        {
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%04x ", *(u16*)addr);
                addr ++;
                if(addr %8 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
        } break;
        case 2:
        {
            addr &= ~(ptu32_t)1;
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%08x ", *(uint32_t*)addr);
                addr +=2;
                if(addr %16 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
        } break;
        case 4:
        {
            addr &= ~(ptu32_t)3;
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%08x ", *(uint32_t*)addr);
                addr +=2;
                printf("%08x ", *(uint32_t*)addr);
                putchar(' ');
                addr +=2;
                if(addr %16 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
        } break;
        default :
        {
            printf("\r\n参数错误\r\n");
            return false;
        } break;
    }
#elif (CN_BYTE_BITS == 32)  //字节位宽=32
    switch(unit_bytes)
    {
        case 1:
        {
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%08x ", *(uint32_t*)addr);
                addr ++;
                if(addr %8 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
        } break;
        case 2:
        {
            addr &= ~(ptu32_t)1;
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%08x ", *(uint32_t*)addr);
                addr +=2;
                printf("%08x ", *(uint32_t*)addr);
                putchar(' ');
                addr +=2;
                if(addr %16 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
        } break;
        default :
        {
            printf("\r\n参数错误\r\n");
            return false;
        } break;
    }
#endif
    printf("\r\n");
    return true;
}

// ============================================================================
// 功能：写入某地址开始的一段内存
// 参数：param -- 参数串，本命令需要三个参数，用空格隔开
//               参数1:起始地址，10进制或16进制(0x开头)
//               参数2:显示的单元数
//               参数3:每单元字节数，合法值是1、2、4，其他数值将返回错误
//               参数4:待填充的内容
// 返回：true=正常显示，false=错误
// 备注：
// ============================================================================

bool_t f(char *param)
{
    ptu32_t addr;
    uint32_t unit_num,unit_bytes,len,data;
    char *word,*next_param;

    word = shell_inputs(param,&next_param);
    addr = strtoul(word, (char **)NULL, 0);
    word = shell_inputs(next_param,&next_param);
    unit_num = strtol(word, (char **)NULL, 0);
    word = shell_inputs(next_param,&next_param);
    unit_bytes = strtol(word, (char **)NULL, 0);
    word = shell_inputs(next_param,&next_param);
    data = strtoul(word, (char **)NULL, 0);

#if (CN_BYTE_BITS == 8)  //字节位宽=8，最常见的情况
    switch(unit_bytes)
    {
        case 1:
            printf("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                *(uint8_t*)addr = (u8)data;
                addr ++;
            }
            break;
        case 2:
            addr &= ~(ptu32_t)1;
            printf("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                *(u16*)addr = (u16)data;
                addr +=2;
            }
            break;
        case 4:
            addr &= ~(ptu32_t)3;
            printf("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                *(u32*)addr = (u32)data;
                addr +=4;
            }
            break;
        default :
            printf("\r\n参数错误\r\n");
            return false;
            break;
    }
#elif (CN_BYTE_BITS == 16)  //字节位宽=16
    switch(unit_bytes)
    {
        case 1:
        {
            printf("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                *(u16*)addr = (u16)data;
                addr ++;
            }
        } break;
        case 2:
        {
            addr &= ~(ptu32_t)1;
            printf("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                *(u32*)addr = (u32)data;
                addr +=2;
            }
        } break;
        default :
        {
            printf("\r\n参数错误\r\n");
            return false;
        } break;
    }
#elif (CN_BYTE_BITS == 32)  //字节位宽=32
    switch(unit_bytes)
    {
        case 1:
        {
            printf("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                *(u32*)addr = (u32)data;
                addr ++;
            }
        } break;
        default :
        {
            printf("\r\n参数错误\r\n");
            return false;
        } break;
    }
#endif
    return true;
}

ADD_TO_ROUTINE_SHELL(f,f,"写数据到内存,命令格式：f 起始地址  单元数 每单元字节数 填充内容");
ADD_TO_ROUTINE_SHELL(ver,djyos_ver,"显示os的版本");
ADD_TO_ROUTINE_SHELL(date,date,"显示或者设置日期");
ADD_TO_ROUTINE_SHELL(d,d,"读取内存里的数据,命令格式:d 地址 单元数 每单元字节数");
ADD_TO_ROUTINE_SHELL(time,time,"显示当前时间或者设置输入新时间");
