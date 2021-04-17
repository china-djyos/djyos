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
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <djyos.h>
#include "./stm32_usb_host_library/class/custom/inc/usbh_custom.h"
#include "./stm32_usb_host_library/class/hub/inc/usbh_hub.h"
#include "./stm32_usb_host_library/class/hid/inc/usbh_hid_touch.h"
#include "usb.h"

//
//
//
#define SPACE_LENGTH                       100
#define COMMAND_MAX                        10

//
//
//
static char shareBufferA[SPACE_LENGTH];
static char shareBufferB[SPACE_LENGTH];
static u8 pollExit; // 3 -- 正在运行; 0 -- 没有运行

char *AT_COMMAND_TABLE[] =
{
    NULL,
    "AT +CGMI",
    "AT +CGMM",
    "ATE",
    "ATI",
    "AT&D0",
    "AT+CSCLK = 1",
    "AT+CSCLK = 0",
    "AT +RESET",
    "AT ^RESET"
};

char *COMMAND_END = " \r";

//
//
//
static bool_t SH_USB_Send_AT(char *Param);
static bool_t SH_USB_Reveive_AT(char *Param);
static bool_t SH_USB_Send_MODEM(char *Param);
static bool_t SH_USB_Reveive_MODEM(char *Param);
static bool_t SH_USB_Send_DEBUG(char *Param);
static bool_t SH_USB_Reveive_DEBUG(char *Param);
static bool_t SH_USB_Send_DEBUG_Timeout(char *Param);
static bool_t SH_USB_Reveive_DEBUG_Timeout(char *Param);
extern bool_t SH_USB_Log(char *pParam);
static bool_t SH_USB_Debug(char *pParam);
static bool_t SH_USB_Force(char *pParam);
static u32 USBH_TouchPollThread(void);
static void SH_USB_UpDate(char *pParam);
extern s32 ModuleInstall_USB_IAP(u8 bArgC, ...);

extern bool_t GetRunMode(void);
//
//
//

extern u32 CUSTOM_WriteAT(ptu32_t pUSB, u8 *pBuf, u32 dwLen, u32 dwOffset, bool_t dwBlock, u32 dwTimeout);
extern u32 CUSTOM_ReadAT(ptu32_t pUSB, u8 *pBuf, u32 dwLen, u32 wdOffset, u32 dwTimeout);
extern u32 CUSTOM_WriteMODEM(ptu32_t pUSB, u8 *pBuf, u32 dwLen, u32 dwOffset, bool_t dwBlock, u32 dwTimeout);
extern u32 CUSTOM_ReadMODEM(ptu32_t pUSB, u8 *pBuf, u32 dwLen, u32 dwOffset, u32 dwTimeout);
// ============================================================================
// 功能：获取SHELL输入的命令行的某个参数
// 参数：pCommandLine -- 命令行；
//      bIndex -- 第几个参数（从零计）；
//      pArg -- 参数缓冲；
//      bArgLen -- 参数缓冲长度；
// 返回：0 -- 成功获取参数；-1 -- 不存在；>0 -- pArg参数空间不够；
// 备注：是将参数拷贝出来，而非直接修改，pCommandLine的输入不会以' '开头
// ============================================================================
s32 Sh_GetArgCopy(const char *pCommandLine, u8 bIndex, char *pArg, u8 bArgLen)
{
    s32 i = 0, j = 0;
    u8 args = 0;

    if((!pCommandLine) || (!pArg))
        return (-1);

    do
    {
        i += j; // 当前参数的起始位置
        j = 0;

        // 检索到一个参数或者结束符退出
        while(('\0' != pCommandLine[i+j]) &&
              (!((pCommandLine[i+j] == ' ') && (pCommandLine[i+j+1] != ' '))))
            j++;

        if(('\0' == pCommandLine[i+j]) || ('\0' == pCommandLine[i+j+1]))
            break;

    }while((++args < bIndex) && (j < 256)); // 单个命令参数小于256个字符，防止跑飞

    if(args < bIndex)
        return (-1);

    while((pCommandLine[i+j] == ' ') && (j > 1)) // j至少保留一个字符
        j--; // 去除多余的空格

    if((j+1) > bArgLen)
        return (j); // 参数缓冲不够大，j+1是考虑字符串结束符

    memcpy(pArg, pCommandLine+i, j);
    pArg[j] = '\0';
    return (0);
}

// ============================================================================
// 功能：
// 参数：（0）发送内容；（1）发送TIMEOUT(可选)
// 返回：
// 备注：
// ============================================================================
static bool_t SH_USB_Send_AT(char *pParam)
{
    char *command = shareBufferA;
    char *arg = shareBufferB;
    u32 len, index = 0;
    USBH_HandleTypeDef *host;
    s32 timeout = 0;
    s32 res;

    if(!pParam)
        return (FALSE);

    memset(command, 0x0, SPACE_LENGTH);
    res = Sh_GetArgCopy(pParam, 0, command, SPACE_LENGTH);
    if(!res)
    {
        index = atoi(command);
        if((0 != index) && (index < COMMAND_MAX))
        {
            strcpy(command, AT_COMMAND_TABLE[index]);
        }

        strcat(command, COMMAND_END);
        len = strlen((char*)command) + 1;
    }
    else
    {
        printf("unsupported parameters.\r\n");
        return (TRUE);
    }

    res = Sh_GetArgCopy(pParam, 1, arg, SPACE_LENGTH);
    if(!res)
    {
        timeout = atoi(arg);
    }


    host = USBH_GetHandle("custom");
    if(!host)
    {
        printf("device \"custom\" is not installed\r\n");
        return (TRUE);
    }


    res = CUSTOM_WriteAT((ptu32_t)host, (u8*)command, len, 0, 0, timeout);
    if(res != len)
    {
        command[len-2] = '\0';
        printf("AT : error : \"%s\" sent error, return %d\r\n", command, res);
    }

    return (TRUE);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static bool_t SH_USB_Reveive_AT(char *pParam)
{
    USBH_HandleTypeDef *host;
    u32 len, i;
    u8 *buf = (u8*)shareBufferA;;
    u8 format = 0;

    if(!strcmp(pParam, "hex"))
    {
        format = 1;
    }

    memset(buf, 0x0, SPACE_LENGTH);
    host = USBH_GetHandle("custom");
    if(!host)
    {
        printf("device \"custom\" is not installed\r\n");
        return (TRUE);
    }

    len = CUSTOM_ReadAT((ptu32_t)host, (u8*)buf, SPACE_LENGTH, 0, 0);
    if(len)
    {
        printf("received %d bytes\r\n", len);
        if(!format)
        {
            for(i = 0; i < len; i++)
                printf("%c", buf[i]);
        }
        else
        {
            for(i = 0; i < len; i++)
                printf("%02x", buf[i]);
        }
    }

    return (TRUE);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static bool_t SH_USB_Send_MODEM(char *pParam)
{
    char *command = shareBufferA;
    char *arg = shareBufferB;
    u32 len, index = 0;
    USBH_HandleTypeDef *host;
    s32 timeout = 0;
    s32 res;

    if(!pParam)
        return (FALSE);

    memset(command, 0x0, SPACE_LENGTH);
    res = Sh_GetArgCopy(pParam, 0, command, SPACE_LENGTH);
    if(!res)
    {
        index = atoi(command);
        if((0 != index) && (index < COMMAND_MAX))
        {
            strcpy(command, AT_COMMAND_TABLE[index]);
        }

        strcat(command, COMMAND_END);
        len = strlen((char*)command) + 1;
    }
    else
    {
        printf("unsupported parameters.\r\n");
        return (TRUE);
    }

    res = Sh_GetArgCopy(pParam, 1, arg, SPACE_LENGTH);
    if(!res)
    {
        timeout = atoi(arg);
    }

    host = USBH_GetHandle("custom");
    if(!host)
    {
        printf("device \"custom\" is not installed\r\n");
        return (TRUE);
    }


    res = CUSTOM_WriteMODEM((ptu32_t)host, (u8*)command, len, 0, 0, timeout);
    if(res != len)
    {
        command[len-2] = '\0';
        printf("AT : error : \"%s\" sent error, return %d\r\n", command, res);
    }

    return (TRUE);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static bool_t SH_USB_Reveive_MODEM(char *pParam)
{
    USBH_HandleTypeDef *host;
    u32 i, len;
    u8 *buf = (u8*)shareBufferA;
    u8 format = 0;

    if(!strcmp(pParam, "hex"))
    {
        format = 1;
    }

    memset(buf, 0x0, SPACE_LENGTH);
    host = USBH_GetHandle("custom");
    if(!host)
    {
        printf("device \"custom\" is not installed\r\n");
        return (TRUE);
    }

    len = CUSTOM_ReadMODEM((ptu32_t)host, (u8*)buf, SPACE_LENGTH, 0, 0);
    if(len)
    {
        printf("received %d bytes\r\n", len);
        if(!format)
        {
            for(i = 0; i < len; i++)
                printf("%c", buf[i]);
        }
        else
        {
            for(i = 0; i < len; i++)
                printf("%02x", buf[i]);
        }
    }

    return (TRUE);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注： diaggrab.c使用
// ============================================================================
static bool_t SH_USB_Send_DEBUG(char *pParam)
{
    u8 *command;
    u32 len;
    char high, low;
    USBH_HandleTypeDef *host;
    u8 *buf = (u8*)shareBufferA;

    if(!pParam)
        return (FALSE);

    if(('-' == pParam[0]) && ('h' == pParam[1]) && (' ' == pParam[2])) // 将字符串格式命令转为HEX格式
    {
        pParam += 3;
        len = strlen(pParam);
        if(len > COMMAND_MAX)
        {
            printf("\r\ncommand parameter is too long(max:%d)\r\n", COMMAND_MAX);
            return (TRUE);
        }

        while(*pParam != '\0' && *(pParam + 1) != '\0')
        {
            high = tolower((s32)*pParam); // 字符作为command某个字符的高4位
            low = tolower((s32)*(pParam + 1)); // 字符作为command某个字符的低4位

            if ((('0' <= high && '9' >= high) || ('a' <= high && 'f' >= high)) &&
                (('0' <= low && '9' >= low) || ('a' <= low && 'f' >= low))) // 只处理‘0’-‘9’，‘a’ - 'f'之间的数据
            {
                if ('0' <= high && '9' >= high)
                {
                    buf[len] = (high - '0') << 4;
                }
                else
                {
                    buf[len] = (0x0a + high - 'a') << 4;
                }

                if ('0' <= low && '9' >= low)
                {
                    buf[len] |= (low - '0');
                }
                else
                {
                    buf[len] |= (0x0a + low - 'a');
                }

                len++;
                pParam += 2;
            }
            else
            {
                pParam++;
            }
        }
    }
    else // 字符串格式的命令
    {
        len = strlen(pParam);
        memset(buf, 0x0, len);
        strcpy((char*)buf, pParam);
        buf[len] = '\r';
        len += 1;
        buf[len] = '\0';
    }

    command = (u8*)buf;
    host = USBH_GetHandle("custom");
    if(!host)
    {
        printf("device is not installed\r\n");
        return (TRUE);
    }

    if(USBH_CUSTOM_DEBUG_Transmit(host, (u8*)command, &len, -1))
        printf("DEBUG channel commmand :\"%s\" error\r\n", command);

    return (TRUE);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注： diaggrab.c使用
// ============================================================================
static bool_t SH_USB_Send_DEBUG_Timeout(char *pParam)
{
    u8 *command;
    u32 len;
    char high, low;
    USBH_HandleTypeDef *host;
    char *buf = shareBufferA;

    if(!pParam)
        return (FALSE);

    if(('-' == pParam[0]) && ('h' == pParam[1]) && (' ' == pParam[2])) // 将字符串格式命令转为HEX格式
    {
        pParam += 3;
        len = strlen(pParam);
        if(len > COMMAND_MAX)
        {
            printf("\r\ncommand parameter is too long(max:%d)\r\n", COMMAND_MAX);
            return (TRUE);
        }

        while(*pParam != '\0' && *(pParam + 1) != '\0')
        {
            high = tolower((s32)*pParam); // 字符作为command某个字符的高4位
            low = tolower((s32)*(pParam + 1)); // 字符作为command某个字符的低4位

            if ((('0' <= high && '9' >= high) || ('a' <= high && 'f' >= high)) &&
                (('0' <= low && '9' >= low) || ('a' <= low && 'f' >= low))) // 只处理‘0’-‘9’，‘a’ - 'f'之间的数据
            {
                if ('0' <= high && '9' >= high)
                {
                    buf[len] = (high - '0') << 4;
                }
                else
                {
                    buf[len] = (0x0a + high - 'a') << 4;
                }

                if ('0' <= low && '9' >= low)
                {
                    buf[len] |= (low - '0');
                }
                else
                {
                    buf[len] |= (0x0a + low - 'a');
                }

                len++;
                pParam += 2;
            }
            else
            {
                pParam++;
            }
        }
    }
    else // 字符串格式的命令
    {
        len = strlen(pParam);
        memset(buf, 0x0, len);
        strcpy(buf, pParam);
        buf[len] = '\r';
        len += 1;
        buf[len] = '\0';
    }

    command = (u8*)buf;
    host = USBH_GetHandle("custom");
    if(!host)
    {
        printf("device is not installed\r\n");
        return (TRUE);
    }

    if(USBH_CUSTOM_DEBUG_Transmit(host, command, &len, 30000))
        printf("DEBUG channel commmand :\"%s\" error\r\n", command);

    return (TRUE);
}
// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注： diaggrab.c使用
// ============================================================================
static bool_t SH_USB_Reveive_DEBUG(char *pParam)
{
    u32 len, left;
    u32 i;
    USBH_HandleTypeDef *host;
    u8 *buf = (u8*)shareBufferA;

    if(!pParam)
        len = SPACE_LENGTH; // default length
    else
        len = atoi(pParam);

    left = len;
    memset(buf, 0x0, SPACE_LENGTH);
    host = USBH_GetHandle("custom");
    if(!host)
    {
        printf("device is not installed\r\n");
        return (TRUE);
    }

    if(USBH_CUSTOM_DEBUG_Receive(host, (u8*)buf, &left, -1)) // wait forever
    {
        printf("DEBUG channel receive error\r\n");
    }

    len -= left;
    if(len)
        printf("\r\nreceived %d bytes\r\n", len);

    for(i = 0; i < len; i++)
        printf("%c", buf[i]);

    return (TRUE);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：diaggrab.c使用
// ============================================================================
static bool_t SH_USB_Reveive_DEBUG_Timeout(char *pParam)
{
    u32 len, left;
    u32 i;
    USBH_HandleTypeDef *host;
    u8 *buf = (u8*)shareBufferA;

    if(!pParam)
        len = SPACE_LENGTH; // default length
    else
        len = atoi(pParam);

    left = len;
    memset(buf, 0x0, SPACE_LENGTH);
    host = USBH_GetHandle("custom");
    if(!host)
    {
        printf("device is not installed\r\n");
        return (TRUE);
    }

    if(USBH_CUSTOM_DEBUG_Receive(host, (u8*)buf, &left, 30000)) // wait 30 seconds
    {
        printf("DEBUG channel receive timeout\r\n");
    }

    len -= left;
    if(len)
        printf("\r\nreceived %d bytes\r\n", len);

    for(i = 0; i < len; i++)
        printf("%c", buf[i]);

    return (TRUE);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注： TODO： 功能尚未完全实现。命令格式 "uforce disconnect <devname>"
// ============================================================================
static bool_t SH_USB_Force(char *pParam)
{
    USBH_HandleTypeDef *host;
    extern void USBH_Force(USBH_HandleTypeDef *pHost, u32 dwOps, void *pParams);

    if(!strncmp(pParam, "disconnect ", 11))
    {
        char *name = pParam + 11;
        host = USBH_GetHandle(name);
        if(!host)
        {
            printf("host %s is not installed\r\n", name);
            return (TRUE);
        }

        USBH_Force(host, 1, 0);

        return (TRUE);

    }


    if(!strncmp(pParam, "connect ", 8))
    {
        u32 parameters = 1;
        char *name = pParam + 8;
        host = USBH_GetHandle(name);
        if(!host)
        {
            printf("host %s is not installed\r\n", name);
            return (TRUE);
        }

        USBH_Force(host, 2, &parameters);

        return (TRUE);
    }

    if(!strncmp(pParam, "attach ", 7))
    {
        u32 parameters = 2;
        char *name = pParam + 7;

        host = USBH_GetHandle(name);
        if(!host)
        {
            printf("host %s is not installed\r\n", name);
            return (TRUE);
        }

        USBH_Force(host, 2, &parameters);

        return (TRUE);
    }

    if(!strncmp(pParam, "reset ", 6))
    {
        u8 index = (u8)atoi(pParam + 6);
        USB_DeviceReset(index, 0);
        return (TRUE);

    }

    return (TRUE);
}

// ============================================================================
// 功能： udebug
// 参数： <status><hub/port><>
//      <clear><hub/port><>
// 返回：
// 备注：
// ============================================================================
static bool_t SH_USB_Debug(char *pParam)
{
    USBH_StatusTypeDef status;
    USBH_HandleTypeDef  *host;
    char *cur;
    u16 feature;
    u8 i;
    u16 num;

    if(!strncmp(pParam, "status ", 7))
    {
        host = USBH_GetHandle("hub");
        cur = pParam + 7;

        if(!strcmp(cur, "hub"))
        {
            tagHubStatus res;
            while(1)
            {
                status = USBH_HUB_GetHubStatus(host, &res);
                if(USBH_OK == status)
                {
                    printf("0x%04x\r\n", res.dw);
                    break;
                }
                else if(USBH_TIMEOUT == status)
                {
                    printf("timeout.\r\n");
                    break;
                }
            }
        }

        if(!strncmp(cur, "port ", 5))
        {
            tagPortStatus res;
            cur += 5;
            num = (u16)atoi(cur); // 获取port号

            while(1)
            {
                status = USBH_HUB_GetPortStatus(host, num, &res);
                if(USBH_OK == status)
                {
                    printf("0x%04x\r\n", res.dw);
                    break;
                }
                else if(USBH_TIMEOUT == status)
                {
                    printf("timeout.\r\n");
                    break;
                }
            }
        }
    }

    if(!strncmp(pParam, "clear ", 6))
    {
        host = USBH_GetHandle("hub");
        cur = pParam + 6;

        if(!strncmp(cur, "hub ", 4))
        {
            cur += 4;

            if(!strcmp(cur, "local power"))
                feature = C_HUB_LOCAL_POWER;

            if(!strcmp(cur, "over current"))
                feature = C_HUB_OVER_CURRENT;

            while(1)
            {
                status = USBH_HUB_ClearHubFeature(host, feature);
                if(USBH_OK == status)
                {
                    printf("done\r\n");
                    break;
                }
                else if(USBH_TIMEOUT == status)
                {
                    printf("timeout.\r\n");
                    break;
                }
            }
        }

        if(!strncmp(cur, "port ", 5))
        {
            cur += 5;
            num = strlen(cur); // 剩余字符串长度
            for(i = 0; i < num; i++) // 查找第一个空格符
            {
                if(' ' == cur[i])
                {
                    cur[i] = '\0';
                    break;
                }
            }
            num = (u16)atoi(cur);
            cur[i]= ' ';
            cur += (i + 1);

            if(!strncmp(cur, "enable", 5))
                feature = PORT_ENABLE;

            if(!strncmp(cur, "suspend", 6))
                feature = PORT_SUSPEND;

            if(!strncmp(cur, "power", 5))
                feature = PORT_POWER;

            if(!strncmp(cur, "indicator", 8))
                feature = PORT_INDICATOR;

            if(!strncmp(cur, "change connection", 16))
                feature = C_PORT_CONNECTION;

            if(!strncmp(cur, "change reset", 12))
                feature = C_PORT_RESET;

            if(!strncmp(cur, "change enable", 13))
                feature = C_PORT_ENABLE;

            if(!strncmp(cur, "change suspend", 14))
                feature = C_PORT_SUSPEND;

            if(!strncmp(cur, "change over current", 19))
                feature = C_PORT_OVER_CURRENT;

            while(1)
            {
                status = USBH_HUB_ClearPortFeature(host, num, feature);
                if(USBH_OK == status)
                {
                    printf("done\r\n");
                    break;
                }
                else if(USBH_TIMEOUT == status)
                {
                    printf("timeout.\r\n");
                    break;
                }
            }

        }
    }

    if(!strncmp(pParam, "set ", 4))
    {
        host = USBH_GetHandle("hub");
        cur = pParam + 4;

        if(!strncmp(cur, "hub ", 4))
        {
            cur += 4;

            if(!strcmp(cur, "local power"))
                feature = C_HUB_LOCAL_POWER;

            if(!strcmp(cur, "over current"))
                feature = C_HUB_OVER_CURRENT;

            while(1)
            {
                status = USBH_HUB_SetHubFeature(host, feature);
                if(USBH_OK == status)
                {
                    printf("done\r\n");
                    break;
                }
                else if(USBH_TIMEOUT == status)
                {
                    printf("timeout.\r\n");
                    break;
                }
            }
        }

        if(!strncmp(cur, "port ", 5))
        {
            cur += 5;
            num = strlen(cur); // 剩余字符串长度
            for(i = 0; i < num; i++) // 查找第一个空格符
            {
                if(' ' == cur[i])
                {
                    cur[i] = '\0';
                    break;
                }
            }

            num = (u16)atoi(cur);
            cur[i]= ' ';
            cur += (i + 1);

            if(!strcmp(cur, "reset"))
                feature = PORT_RESET; // 相当于ENABLE

            if(!strcmp(cur, "suspend"))
                feature = PORT_SUSPEND;

            if(!strcmp(cur, "power")) // 相当于掉电
                feature = PORT_POWER;

            if(!strcmp(cur, "indicator"))
                feature = PORT_INDICATOR;

            while(1)
            {
                status = USBH_HUB_SetPortFeature(host, num, feature);
                if(USBH_OK == status)
                {
                    printf("done\r\n");
                    break;
                }
                else if(USBH_TIMEOUT == status)
                {
                    printf("timeout.\r\n");
                    break;
                }
            }
        }
    }

    if(!strncmp(pParam, "change", 6))
    {
        u8 change;
        host = USBH_GetHandle("hub");

        while(1)
        {
            status = HUB_StatusChange(host, &change, 1);
            if(USBH_OK == status)
            {
                printf("0x%x\r\n", change);
                break;
            }
            else if(USBH_TIMEOUT == status)
            {
                printf("timeout.\r\n");
                break;
            }
        }
    }

    if(!strcmp(pParam, "hub descriptor"))
    {
        tagDescHUB description = {0};
        static u8 retry = 0;
        host = USBH_GetHandle("hub");

        while(1)
        {
            status = USBH_HUB_GetDescriptor(host, (u8*)&description, 1);
            if(USBH_OK == status)
            {
                printf("\r\ndescription total length:%d\r\n", description.bLen);
                break;
            }
            else if(USBH_TIMEOUT == status)
            {
                printf("timeout. retry %d\r\n", ++retry);
                if(retry > 10)
                    break;
            }
        }

        if(description.bLen)
        {
            while(1)
            {
                status = USBH_HUB_GetDescriptor(host, (u8*)&description, description.bLen);
                if(USBH_OK == status)
                {
                    printf("hub's description\r\n");
                    printf("      length: %d\r\n", description.bLen);
                    printf("      type: %xh\r\n", description.bType);
                    printf("      ports: %d\r\n", description.bPorts);
                    printf("      characteristics: %xh\r\n", description.wCharacteristics);
                    printf("      power on 2 power good: %xh\r\n", description.bPwrOn2PwrGood);
                    printf("      control current: %xh\r\n", description.bPwrOn2PwrGood);
                    printf("      device removable: %xh\r\n", description.bContrCurrent);
                    printf("      more data: ");
                    for(i = 0; i < (description.bLen - 7); i++)
                    {
                        printf("%x", description.bData[i]);
                        if(0 == ((i + 1) % 16))
                            printf("\r\n           ");
                    }
                    printf("\r\n");
                    break;
                }
                else if(USBH_TIMEOUT == status)
                {
                    printf("timeout.\r\n");
                    break;
                }
            }
        }
    }

    if(!strncmp(pParam, "touch", 5))
    {
        u8 button;
        u32 axisX;
        u32 axisY;
        extern s32 USBH_HID_TouchGetData(USBH_HandleTypeDef *pHost, u8 *pButton, u32 *pX, u32 *pY);

        cur = pParam + 5;

        if('\0' == *cur)
        {
            host = USBH_GetHandle("touch");
            if(!USBH_HID_TouchGetData(host, &button, &axisX, &axisY))
            {
                printf("button: %02xh,", button);
                printf("axis x: %04xh,", axisX);
                printf("axis y: %04xh\r\n", axisY);
            }
            else
            {
                printf("no touch");
            }
            printf("\r\n");
        }
        else if(!strncmp(cur, " poll on ", 8))
        {
            u32 period;
            cur += 8;
            period = atoi(cur);

            // 建立一个独立的线程，用于不断扫描touch
            if(3 == pollExit)
            {
                printf("touch poll is already running.\r\n");
            }
            else
            {
                u16 thread;

                pollExit = 3;

                thread = DJY_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 0,
                                        USBH_TouchPollThread, NULL, 0x1000, "USB touch poll");
                if(thread != CN_EVTT_ID_INVALID)
                {
                    DJY_EventPop(thread, NULL, 0, period, 0, 0);
                    return (TRUE);
                }

                printf("touch poll thread cannot register.");
                return (FALSE);

            }
        }
        else if(!strcmp(cur, " poll off"))
        {
            pollExit = 0;
        }
    }

    if(!strncmp(pParam, "reset host ", 11))
    {
        u8 hostIndex, devIndex = 0;
        cur = pParam + 11;
        num = strlen(cur); // 剩余字符串长度
        for(i = 0; i < num; i++) // 查找第一个空格符
        {
            if(' ' == cur[i])
            {
                cur[i] = '\0';
                break;
            }
        }

        num = (u16)atoi(cur);
        cur[i]= ' ';
        cur += (i + 1);
        hostIndex = (u8)num;

        if(!strncmp(cur, "dev ", 4))
        {
            cur += 4;
            num = strlen(cur); // 剩余字符串长度
            for(i = 0; i < num; i++) // 查找第一个空格符
            {
                if(' ' == cur[i])
                {
                    cur[i] = '\0';
                    break;
                }
            }

            num = (u16)atoi(cur);
            cur[i]= ' ';
            devIndex = (u8)num;
        }

        USB_DeviceReset(hostIndex, devIndex);
    }

    if(!strcmp(pParam, "iap"))
    {
//        extern u32 USB_IAP_Thread(void);
//        USB_IAP_Thread();
    }

    if(!strncmp(pParam, "test ", 5))
    {
        static int res;
        cur = pParam + 5;

        if(!strcmp(cur, "1"))
        {

            res = open("/dev/uat", O_RDONLY);
            if(-1 == res)
            {
                printf("cannot open </dev/uat>. ");
            }
        }

        if(!strcmp(cur, "2"))
        {
            u8 temp[2];
            write(res, temp, 2);
        }
    }

    if(!strncmp(pParam, "sleep ", 6))
    {
        cur = pParam + 6;

        if(!strncmp(cur, "custom ", 7))
        {
#if 0
            extern s32 CUSTOM_PermitWorkMode(u8 bArgC, ...);
            extern s32 CUSTOM_SetWorkMode(u8 bArgC, ...);

            CUSTOM_PermitWorkMode(1, 1);
            cur += 7;

            if(!strcmp(cur, "enable"))
            {
                CUSTOM_ChangeToMode(1);
                printf("custom sleep enable.\r\n");
            }

            if(!strcmp(cur, "disable"))
            {
                CUSTOM_ChangeToMode(0);
                printf("custom sleep disable.\r\n");
            }
#endif
        }
    }

    return (TRUE);
}

// ============================================================================
// 功能：执行USB升级功能
// 参数：pParam：目标文件名和升级功能参数，默认为升级完成后自动运行app。
// 返回：
// 备注：
// ============================================================================
static void SH_USB_UpDate(char *pParam)
{
    char *FileName,*NextParam,*WordTrail;
    u32 dwOpt = 2;

    if(pParam != NULL)
    {
        FileName = shell_inputs(pParam,&NextParam);
        WordTrail = shell_inputs(NextParam,&NextParam);
        if(WordTrail != NULL)
        {
            dwOpt = strtol(WordTrail, (char **)NULL, 0);
        }
        if(GetRunMode())
        {
            ModuleInstall_USB_IAP(3,1,FileName,dwOpt);
        }
        else
        {
            printf("\r\nThis command is available only in the iboot mode\r\n.");
        }
    }
    else
    {
        printf("\r\nPlease add the target file name after the shell command\r\n.");
    }
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
u32 USBH_TouchPollThread(void)
{
    u8 button = 0;
    u32 axisX = 0;
    u32 axisY = 0;
    u32 period = 0;
    extern s32 TOUCH_DirectRead(u8 *pButton, u32 *pX, u32 *pY);

    DJY_GetEventPara((ptu32_t*)&period, NULL);
    if(!period)
        period = 10000; // 默认10ms

    printf("touch poll thread <period.%d.us> start...\r\n", period);

    while(1)
    {
        if(TOUCH_DirectRead(&button, &axisX, &axisY))
        {
            printf("button: %02xh, ", button);
            printf("axis x: %04xh, ", axisX);
            printf("axis y: %04xh\r\n", axisY);
        }
        else
        {
            printf("no touch\r\n");
        }

        if(!pollExit)
        {
            printf("\r\n touch poll thread exit...\r\n");
            break;
        }

        DJY_EventDelay(period);
    }

    return (0);
}

// ============================================================================
// 功能： USB相关SHELL命令安装
// 参数：
// 返回：
// 备注：
// ============================================================================
void USB_ShellInstall(void)
{

}

