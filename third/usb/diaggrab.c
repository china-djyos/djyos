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
#include <stdio.h>
#include <fcntl.h>
#include <stat.h>
#include <djyos.h>
#include <ctype.h>
#include "./usb.h"
#include "./stm32_usb_host_library/class/custom/inc/usbh_custom.h"

#define RW_SIZE                 512
#define LINE_MAX                1280

typedef enum {
    IDLE,
    FLUSH,
    CONFIG,
    CONFIG_DONE,
    LOG,
    QUIT,
}tagLogStatus;

typedef enum {
    WAIT,
    REQUEST,
    RESPOND,
}tagConfigDecode;

static tagLogStatus gStatus = IDLE;
u32 dwLineNum; // 记录已处理的行号

static u32 USB_LogThread(void);

// ============================================================================
// 功能:
// 参数:
// 返回:
// 备注:
// ============================================================================
bool_t SH_USB_Log(const char *pParam)
{
    if(!pParam)
        return (FALSE);

    if(!strcmp(pParam, "on"))
    {
        u16 thread;

        if(IDLE != gStatus)
        {
            USBH_UsrLog("\r\nUSB log service has been turned on\r\n");
            return (TRUE);
        }

        gStatus = IDLE;
        thread = DJY_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 0,
                            USB_LogThread, NULL, 0x2000, "USB log service");
        if(CN_EVTT_ID_INVALID != thread)
        {
            DJY_EventPop(thread, NULL, 0, 0, 0, 0);
            return (TRUE);
        }

        return (FALSE);
    }

    if(!strcmp(pParam, "flush") && (IDLE == gStatus))
        gStatus = FLUSH;

    if(!strcmp(pParam, "config") && (IDLE == gStatus))
        gStatus = CONFIG;

    if(!strcmp(pParam, "log") && (CONFIG_DONE == gStatus))
        gStatus = LOG;

    if(!strcmp(pParam, "quit"))
        gStatus = QUIT;

    return (TRUE);
}



// ============================================================================
// 功能:
// 参数:
// 返回:
// 备注:
// ============================================================================
void __Request(char *pCommand)
{
    char high, low;
    USBH_HandleTypeDef  *host;
    u32 i, left, len = 0;
    u8 command[LINE_MAX];

    memset(command, 0x0, LINE_MAX);

    host = USBH_GetHandle("custom"); // #2设USB用于无线模块
    if(!host)
    {
        USBH_UsrLog("\r\nLOG SERVICE: USB service is not available\r\n");
        return;
    }

    // 字符成对处理，将字符'76'转成0x76
    while(*pCommand != '\0' && *(pCommand + 1) != '\0')
    {
        high = tolower((s32)*pCommand); // 字符作为command某个字符的高4位
        low = tolower((s32)*(pCommand + 1)); // 字符作为command某个字符的低4位

        if ((('0' <= high && '9' >= high) || ('a' <= high && 'f' >= high)) &&
            (('0' <= low && '9' >= low) || ('a' <= low && 'f' >= low))) // 只处理‘0’-‘9’，‘a’ - 'f'之间的数据
        {
            if ('0' <= high && '9' >= high)
            {
                command[len] = (high - '0') << 4;
            }
            else
            {
                command[len] = (0x0a + high - 'a') << 4;
            }

            if ('0' <= low && '9' >= low)
            {
                command[len] |= (low - '0');
            }
            else
            {
                command[len] |= (0x0a + low - 'a');
            }

            len++;
            pCommand += 2;
        }
        else
        {
            pCommand++;
        }
    }

    if(!len)
    {
        USBH_UsrLog("\r\nLOG SERVICE: command cannot be decoded\r\n");
        USBH_UsrLog("\r\ncommand: %s", pCommand);
        return;
    }

    left = len;
    if(USBH_CUSTOM_DEBUG_Transmit(host, command, &left, 300000))
    {
        USBH_UsrLog("\r\nLOG SERVICE: failed to send REQ (%d bytes delivered)", (len - left));
        USBH_UsrLog("<");
        for(i = 0; i < len; i++)
        {
            USBH_UsrLog("%02x", command[i]);
        }
        USBH_UsrLog(">hex\r\nmemset(respond, 0x0, LINE_MAX);");

    }

    return;

}

// ============================================================================
// 功能:
// 参数:
// 返回:
// 备注:
// ============================================================================
void __Respond(char *pRespond)
{
    u8 check[LINE_MAX];
    u8 respond[LINE_MAX];
    u32 left, i;
    USBH_HandleTypeDef  *host;
    u8 retry;
    char high, low;
    u8 *current;
    u32 len = 0;
    u32 received;

    host = USBH_GetHandle("custom"); // #2设USB用于无线模块
    if(!host)
    {
        USBH_UsrLog("\r\nLOG SERVICE: USB service is not available\r\n");
        return;
    }

    memset(check, 0x0, LINE_MAX);
    memset(respond, 0x0, LINE_MAX);

    // 字符成对处理，将字符'76'转成0x76
    while(*pRespond != '\0' && *(pRespond + 1) != '\0')
    {
        high = tolower((s32)*pRespond); // 字符作为command某个字符的高4位
        low = tolower((s32)*(pRespond + 1)); // 字符作为command某个字符的低4位

        if ((('0' <= high && '9' >= high) || ('a' <= high && 'f' >= high)) &&
            (('0' <= low && '9' >= low) || ('a' <= low && 'f' >= low))) // 只处理‘0’-‘9’，‘a’ - 'f'之间的数据
        {
            if ('0' <= high && '9' >= high)
            {
                check[len] = (high - '0') << 4;
            }
            else
            {
                check[len] = (0x0a + high - 'a') << 4;
            }

            if ('0' <= low && '9' >= low)
            {
                check[len] |= (low - '0');
            }
            else
            {
                check[len] |= (0x0a + low - 'a');
            }

            len++;
            pRespond += 2;
        }
        else
        {
            pRespond++;
        }
    }

    left = len;
    retry = 5;
    while(retry--) // 尝试5次
    {
        USBH_CUSTOM_DEBUG_Receive(host, respond, &left, 300000);
        if(!left)
            break; // 读完
    }

    if((!retry) && (left)) //尝试5次后，依然没有获取应有数据
    {
        USBH_UsrLog("\r\nLOG SERVICE: %d more bytes need to be responded\r\n", left);
        USBH_UsrLog("\r\nin fact(%d bytes):\r\n", (len - left));

        for(i = 0; i < (len - left); i++)
        {
            USBH_UsrLog("%02x", respond[i]);
            if(0x0F == (i & 0x0F))
            {
                USBH_UsrLog("\r\n"); // 打印16个，添加一次换行
            }
        }
    }

    for(i = 0; i < len; i++)
    {
        if(respond[i] != check[i])
        {
            USBH_UsrLog("\r\nLOG SERVICE: bad respond <line: %d>", dwLineNum);
            USBH_UsrLog("\r\nshould be(%d bytes):\r\n", len);

            for(i = 0; i < len; i++)
            {
                USBH_UsrLog("%02x", check[i]);
                if(0x0F == (i & 0x0F))
                {
                    USBH_UsrLog("\r\n"); // 打印16个，添加一次换行
                }
            }

            USBH_UsrLog("\r\nin fact");
            while(1)
            {
                // 数据错误，尝试把数据读完，反之数据对不上
                received = len - left; // 已读数据
                current = respond + received;
                len = left = LINE_MAX - received;
                USBH_CUSTOM_DEBUG_Receive(host, current, &left, 300000);
                received += (len - left);
                USBH_UsrLog("(%d bytes):\r\n", received);
                for(i = 0; i < received; i++)
                {
                    USBH_UsrLog("%02x", respond[i]);
                    if(0x0F == (i & 0x0F))
                    {
                        USBH_UsrLog("\r\n"); // 打印16个，添加一次换行
                    }
                }

                if(!left) // 可能还有数据，继续尝试
                {
                    USBH_UsrLog("more");
                    len = LINE_MAX;
                    left = LINE_MAX;
                    memset(respond, 0x0, LINE_MAX);
                }
                else // 全部读完
                {
                    USBH_UsrLog("\r\n");
                    return;
                }
            }
        }
    }

    return;

}

// ============================================================================
// 功能:
// 参数:
// 返回:
// 备注:
// ============================================================================
static void UploadLog(void)
{
    s32 log;
    s32 res;
    u8 buffer[LINE_MAX];
    u32 len;
    USBH_HandleTypeDef  *host;
    char *path = "/fat/log.bin";


    host = USBH_GetHandle("custom"); // #2设USB用于无线模块
    if(!host)
    {
        USBH_UsrLog("\r\nLOG SERVICE: USB service is not available\r\n");
        return;
    }

    log = open(path, O_CREAT | O_RDWR);
    if(-1 == log)
    {
        USBH_UsrLog("\r\nLOG SERVICE: log cannot be create\r\n");
        return;
    }

    USBH_UsrLog("\r\nLOG SERVICE: log will upload to <%s>\r\n", path);

    while(LOG == gStatus)
    {
        USBH_CUSTOM_DEBUG_Receive(host, buffer, &len, 300000);
        len = LINE_MAX - len;

        if(len)
        {
            res = write(log, buffer, len);
            if(res != len)
            {
                USBH_UsrLog("\r\nLOG SERVICE: write log error, quit now\r\n");
                break; // 写错误，退出
            }

            memset(buffer, 0x0, LINE_MAX);
        }
    }

    if(close(log))
    {
        USBH_UsrLog("\r\nLOG SERVICE: close log error\r\n");
    }

    USBH_UsrLog("\r\nLOG SERVICE: log upload exist\r\n");
}

// ============================================================================
// 功能:
// 参数:
// 返回:
// 备注:
// ============================================================================
static s32 DownloadConfig(void)
{
    s32 res;
    s32 config;
    struct stat configInfo;
    char buffer[RW_SIZE];
    char command[LINE_MAX];
    char respond[LINE_MAX];
    char line[LINE_MAX];
    char *path = "/fat/config.dat";
    tagConfigDecode current = WAIT, next = WAIT;
    u32 i, j = 0;
    u8 new = 0;

    config = open(path, O_RDONLY);
    if(-1 == config)
    {
        USBH_UsrLog("\r\nLOG SERVICE: cannot open configure <%s>", path);
        return (-1);
    }

    res = fstat(config, &configInfo);
    if(res)
    {
        USBH_UsrLog("\r\nLOG SERVICE: cannot statistics configure <%s>.\r\n", path);
        return (-1);
    }

    memset(command, 0x0, LINE_MAX);
    memset(respond, 0x0, LINE_MAX);
    dwLineNum = 0;
    while(configInfo.st_size)
    {
        memset(buffer, 0x0, RW_SIZE);
        res = read(config, buffer, RW_SIZE);
        if((!res) && (configInfo.st_size)) // 仍有数据可读，但返回零
        {
            USBH_UsrLog("\r\nLOG SERVICE: read configure error\r\n");
            return (-1);
        }

        configInfo.st_size -= res;

        // 文件逐行解析
        for(i = 0; i < res;)
        {
            line[j] = buffer[i];

            if('\n' == line[j])// 查找换行， 将换行替换成'\0'
            {
                line[j] = '\0';
                dwLineNum++;

                if(LINE_MAX <= j)
                {
                    USBH_UsrLog("\r\nLOG SERVICE: line buffer overflow <j: 0x%x>\r\n", j); // 容错打印
                }

                if(strstr(line, "pkt rsp"))
                    next = RESPOND;

                if(strstr(line, "pkt req"))
                    next = REQUEST;

                if(strstr(line, "pkt end"))
                {
                    USBH_UsrLog("\r\nLOG SERVICE: configure done.\r\n");
                    return (0);
                }

                switch(current)
                {
                case WAIT :
                    if(REQUEST == next)
                    {
                        current = REQUEST;
                    }
                    else
                    {
                        USBH_UsrLog("\r\nLOG SERVICE: unknown logic in state <IDLE>\r\n");
                    }
                    break;

                case REQUEST :
                    if(RESPOND == next)
                    {
                        __Request(command); // 发送命令
                        memset(command, 0x0, LINE_MAX); // 清空，预备下次命令
                        current = RESPOND;
                        break;
                    }

                    strcat(command, line); // 提取本行命令
                    break;

                case RESPOND :
                    if(REQUEST == next)
                    {
                        __Respond(respond); // 接受回复
                        memset(respond, 0x0, LINE_MAX); // 清空，预备下次回复
                        current = REQUEST;
                    }

                    strcat(respond, line); // 提取本行命令
                    break;


                default :
                    break;
                }

                new = 1; // 本行解析完，换下一行
            }

            i++;
            j++;

            if(new) // 新的一行，指针归零
            {
                j = 0;
                new = 0;
            }
        }
    }

    return (0);
}

// ============================================================================
// 功能:
// 参数:
// 返回:
// 备注:
// ============================================================================
s32 CleanBuffer(void)
{
    USBH_HandleTypeDef  *host;
    u8 buffer[16];
    u32 len = 16;

    host = USBH_GetHandle("custom"); // #2设USB用于无线模块
    if(!host)
    {
        USBH_UsrLog("\r\nLOG SERVICE: USB service is not available\r\n");
        return (-1);
    }

    while(len) // 读debug接口，直至读空
    {
        if(USBH_CUSTOM_DEBUG_Receive(host, buffer, &len, 300000))
            break;

        len = 16;
    }
    USBH_UsrLog("\r\nLOG SERVICE: flush done\r\n");
    return (0);
}

// ============================================================================
// 功能:
// 参数:
// 返回:
// 备注:
// ============================================================================
static u32 USB_LogThread(void)
{
    s32 res;
    USBH_UsrLog("\r\nUSB log service start to work...\r\n");

    while(1)
    {
        switch(gStatus)
        {
        case IDLE:
        case CONFIG_DONE:
            DJY_EventDelay(10000000); //
            break;

        case FLUSH:
            res = CleanBuffer();
            if(!res)
                gStatus = IDLE;
            break;

        case CONFIG :
            res = DownloadConfig();
            if(!res)
                gStatus = CONFIG_DONE;
            break;

        case LOG :
            UploadLog();
            break;

        case QUIT :
            break;

        default :
            break;

        }
    }

    return (0);
}
