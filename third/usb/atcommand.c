//-----------------------------------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

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
// 这份授权条款，在使用者符合以下三条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。
// 3. 本软件作为都江堰操作系统的组成部分，未获事前取得的书面许可，不允许移植到非
//    都江堰操作系统环境下运行。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <device.h>
#include <stdio.h>

#define LOCAL_PRINT                 printk
#define LOCAL_RESPONSE_TIMEOUT      50
char *AT_COMMANDS_SIMCOM[] = {"AT+CSQ","AT+CICCID"};
char *AT_COMMANDS_HUAWEI[] = {"AT+CSQ","AT^ICCID?"};
char *AT_END = " \r";
char *AT_OK = "OK";
char *AT_ERROR = "ERROR";
char ICCID[21];
u8 AT_Output[256];
char *CUSTOM_GetInfos(void);

// ============================================================================
//功能：命令确认。
//参数：pCommand -- 确认的命令；
//     bLogic -- 确认逻辑。
//返回：-1 -- 失败； 0 -- OK； 1 -- ERROR。
//备注：
// ============================================================================
//s32 __GetResponse(void *pHandle, char *pCommand, u8 *pResp, u32 dwLimit)
s32 __GetResponse(s32 handle, char *pCommand, u8 *pResp, u32 dwLimit)
{
    u32 i, j, res, wait = LOCAL_RESPONSE_TIMEOUT;
    u32 responsed;
    char response = 0;
    u32 cLen = strlen(pCommand);
    u8 checkCommand = 1, checkStatus = 1, log = 1;

    i = 0;
    responsed = 0;
    while(checkCommand)
    {
        //res = Device_Read(pHandle, (u8*)&response, 1, 0, 0);
        res = read(handle, (u8*)&response, 1);
        if(!res)
        {
            if(!wait--)
            {
                if(log)
                    LOCAL_PRINT("AT : error : \"%s\" has %s response.\r\n", pCommand, (responsed?"error":"no"));

                return (-1);
            }

            DJY_EventDelay(1000);
            continue;
        }

        wait = LOCAL_RESPONSE_TIMEOUT;
        responsed++;
        if(pCommand[i] == response)
            i++;
        else
            i = 0; // 重置

        if(i == cLen)
            break;
    }

    i = 0;
    j = 0;
    responsed = 0;
    while(checkStatus)
    {
        if(AT_OK[i] == response)
        {
            if(++i == strlen(AT_OK))
                break;

        }
        else if(AT_ERROR[j] == response)
        {
            if(++j == strlen(AT_ERROR))
                return (1);
        }
        else
        {
            i = 0; // 重置
            j = 0;
        }

        //res = Device_Read(pHandle, (u8*)&response, 1, 0, 0);
        res = read(handle, (u8*)&response, 1);
        if(!res)
        {
            if(!wait--)
            {
                if(log)
                    LOCAL_PRINT("AT : error : \"%s\" has %s response.\r\n", pCommand, (responsed?"error":"no"));

                return (-1);
            }

            DJY_EventDelay(1000);
            continue;
        }

        wait = LOCAL_RESPONSE_TIMEOUT;

        if(responsed < dwLimit)
            pResp[responsed] = response;

        responsed++;
    }

    return (0);
}

// ============================================================================
//功能：命令发送。
//参数：pCommand -- 命令；
//     dwLen -- 命令字符长度；
//返回：实际发送的名字字符数。
//备注：
// ============================================================================
//u32 __Command(void *pHandle, char *pCommand, u32 dwLen)
//{
//    return (Driver_WriteDevice(pHandle, (u8*)pCommand, dwLen, 0, 0, 2000000));
//}
u32 __Command(s32 handle, char *pCommand, u32 dwLen)
{
    return (write(handle, (u8*)pCommand, dwLen));
}
// ============================================================================
//功能：获取AT命令。
//参数：function -- 需要AT命令集中的第几个命令；
//返回：该命令
//备注：
// ============================================================================
char *GetAT_Command(u32 function)
{
    char *receive;
    char *name[] = {"SIMCOM","HUAWEI"};
    receive = CUSTOM_GetInfos();     //获取4G模块的型号
    if(receive == NULL)
    {
        printf("AT : error : no found 4G module .\r\n");
        return NULL;
    }

    if(strstr(receive,name[0]) == NULL)     //根据获取到的型号选择使用的指令集
    {
        if(strstr(receive,name[1]) == NULL)
        {
            printf("AT : error : unsupported modules.\r\n");
            return NULL;
        }
        else
        {
           return AT_COMMANDS_HUAWEI[function];
        }
    }
    else
    {
        return AT_COMMANDS_SIMCOM[function];
    }
}

// ============================================================================
//功能：读取信号强度
//参数：bArgC -- 参数数量；第一个参数为设备名，缺省为“uat”
//返回：-1 -- 读取失败；其他 -- 信号强度；
//备注：
// ============================================================================
s32 SignalStrength(u8 bArgC, ...)
{
    // const char *defaultName = "uat";
    const char *defaultName = "/dev/usb-at";
    char *name = NULL;
    char command[10] = {0};
    char *at_command;
    u8 response[50];
    va_list ap;
    // void *handle;
    s32 handle;
    u32 len, res;
    u8 i;
    s32 strength;
    u32 good = 0;

    at_command = GetAT_Command(0);
    if(at_command == NULL)
    {
        return (-1);
    }

    va_start(ap, bArgC);
    for(i = 0; i < bArgC; i++)
    {
        switch(i)
        {
        case 0 : name = va_arg(ap, char*); break;
        default: break;
        }
    }
    va_end(ap);

    if(!name)
        name = (char*)defaultName;

    // handle = (void*)Device_Open(name, O_RDWR, 0);
    // if(!handle)
    handle = open(name, O_RDWR);
    if(-1 == handle)
    {
        LOCAL_PRINT("AT : error : \"%s\" does not exist.\r\n", name);
        return (-1);
    }

    memset(command, 0x0, sizeof(command));
    memset(response, 0x0, sizeof(response));
    strcat(command, at_command);
    strcat(command, AT_END);
    len = strlen(command)+1;
    res = __Command(handle, command, len);
    if(res != len)
    {
        LOCAL_PRINT("AT : error : \"%s\" cannot sent for signal strength.\r\n", at_command);
        // Driver_CloseDevice(handle);
        close(handle);
        return (-1);
    }
    DJY_EventDelay(1000*1000);
    if(__GetResponse(handle, at_command, response, sizeof(response)))
    {
        // Driver_CloseDevice(handle);
        close(handle);
        return (-1);
    }

#if 0
    res = sscanf((const char*)response, "+CSQ: %d", &strength);
    if(-1 == res)
        return (-1);
#else

    for(i = 0; i < (sizeof(response)-5); i++)
    {
        if((response[i] == '+') && (response[i+1] == 'C') &&
           (response[i+2] == 'S') && (response[i+3] == 'Q') &&
           (response[i+4] == ':') && (response[i+5] == ' '))
        {
            good = i+6;
            break;
        }
    }

    if(!good)
    {
        // Driver_CloseDevice(handle);
        close(handle);
        return (-1);
    }

    i = good;
    for(; i < (sizeof(response)-i); i++)
    {
        if(response[i] < '0' || response [i] > '9')
        {
            response[i] = '\0';
            break;
        }
    }

    if(i == (sizeof(response)-i))
    {
        // Driver_CloseDevice(handle);
        close(handle);
        return (-1);
    }

    strength = atoi((char*)(response+good));
#endif

    // Driver_CloseDevice(handle);
    close(handle);
    return (strength);
}

// ============================================================================
//功能：读取ICCID
//参数：bArgC -- 参数数量；第一个参数为设备名，缺省为“uat”
//返回：NULL -- 读取失败；其他 -- 获取到的ICCID（以字符串形式）；
//备注：
// ============================================================================
char *ReadICCID(u8 bArgC, ...)
{
    const char *defaultName = "/dev/usb-at";
    char *name = NULL;
    char command[15] = {0};
    char *at_command;
    u8 response[50];
    va_list ap;
    s32 handle;
    u32 len, res;
    u8 i;
    u32 good = 0;

    at_command = GetAT_Command(1);
    if(at_command == NULL)
    {
        return NULL;
    }

    va_start(ap, bArgC);
    for(i = 0; i < bArgC; i++)
    {
        switch(i)
        {
        case 0 : name = va_arg(ap, char*); break;
        default: break;
        }
    }
    va_end(ap);

    if(!name)
        name = (char*)defaultName;

    handle = open(name, O_RDWR);
    if(-1 == handle)
    {
        LOCAL_PRINT("AT : error : \"%s\" does not exist.\r\n", name);
        return NULL;
    }

    memset(command, 0x0, sizeof(command));
    memset(response, 0x0, sizeof(response));
    strcat(command, at_command);
    strcat(command, AT_END);
    len = strlen(command)+1;
    res = __Command(handle, command, len);
    if(res != len)
    {
        LOCAL_PRINT("AT : error : \"%s\" cannot sent for read ICCID.\r\n", at_command);
        close(handle);
        return NULL;
    }
    DJY_EventDelay(1000*1000);
    if(__GetResponse(handle, at_command, response, sizeof(response)))
    {
        close(handle);
        return NULL;
    }

#if 0
    res = sscanf((const char*)response, "+ICCID: %d", &ICCID);
    if(-1 == res)
        return NULL;
#else

    for(i = 0; i < (sizeof(response) - 8); i++)
    {
        if((response[i] == '+') || (response[i] == '^'))
        {
            if((response[i+1] == 'I') &&(response[i+2] == 'C') &&
               (response[i+3] == 'C') &&(response[i+4] == 'I') &&
               (response[i+5] == 'D') &&(response[i+6] == ':') &&
               (response[i+7] == ' '))
            {
                good = i+8;
                break;
            }
        }
    }

    if(!good)
    {
        close(handle);
        return NULL;
    }

    i = good;
    for(; i < sizeof(response); i++)
    {
        if(response[i] < '0' || response [i] > '9')
        {
            if(response[i] < 'A' || response [i] > 'Z')
            {
                break;
            }
        }
    }

    len = i-good;
    if(len != 20)
    {
        close(handle);
        return NULL;
    }

    memset(ICCID, 0x0, sizeof(ICCID));
    for(i = 0; i < len; i++ )
    {
//        ICCID[i] = response[good + i] - '0';           //返回阿拉伯数字
        ICCID[i] = response[good + i] ;                  //返回ASCII
    }
    ICCID[len] = '\0';
#endif

    close(handle);
    return (ICCID);
}

// ============================================================================
//功能：通用AT命令接口
//参数：bArgC -- 参数数量；第一个参数为要输入的AT命令，第二个参数为设备名
//返回：NULL -- 读取失败；其他 -- 输出的结果（输出的结果是一个数组的首地址，该数组大小为256字节）；
//备注：AT命令的输入和输出最多不超过256个字节
// ============================================================================
u8 *AT_Command(char *at_command,char *name)
{
    const char *defaultName = "/dev/usb-at";
    char command[256] = {0};
    s32 handle;
    u32 len, res;

    if(!name)
        name = (char*)defaultName;

    handle = open(name, O_RDWR);
    if(-1 == handle)
    {
        LOCAL_PRINT("AT : error : \"%s\" does not exist.\r\n", name);
        return NULL;
    }

    memset(command, 0x0, sizeof(command));
    memset(AT_Output, 0x0, sizeof(AT_Output));
    strcat(command, at_command);
    strcat(command, AT_END);
    len = strlen(command)+1;
    res = __Command(handle, command, len);
    if(res != len)
    {
        LOCAL_PRINT("AT : error : \"%s\" cannot sent for read ICCID.\r\n", at_command);
        close(handle);
        return NULL;
    }
    DJY_EventDelay(1000*1000);
    if(__GetResponse(handle, at_command, AT_Output, sizeof(AT_Output)))
    {
        close(handle);
        return NULL;
    }

    close(handle);
    return (AT_Output);
}
