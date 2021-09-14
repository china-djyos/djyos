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
#if 0
#include <stdio.h>
#include <string.h>
#include <djyos.h>
#include <wdt_soft.h>
#include <dirent.h>
#include <stdlib.h>
#include <blackbox.h>

#define RW_SIZE                      512
#define APPLICATION_FILE_NAME        "PRS-7573.bin"

typedef enum{
    INIT = 0,
    RUN,
    DONE,
    ERROR
}USB_IAP_STATUS;

static USB_IAP_STATUS gRunningStatus;
static u32 dwDeamonInspect;
#define __IAP_USB_BUFFER_SIZE                         (1024*8)
//extern void USB_UDiskReset(void);
extern void Amend_IBootFlag_RunIBoot(void);
extern void Amend_IBootFlag_RunAPP(void);
extern enStartMode GetBootMethodSoft(void);
extern s32 MSC_DeviceReady(u8 bID);
extern bool_t  Sh_RunAPP(char *param);
extern bool_t HAL_ClrUpdateFlag(void);
extern bool_t HAL_SetUpdateFlag(void);
extern bool_t HAL_ClrUpdateFlag(void);
extern void RunIboot(void);
extern void CPU_Reset(u32 key);
extern s32 IAP_SetMethod(u32 dwMethod);
extern s32 IAP_SetPath(char *pPath);
extern u32 IAP_GetMethod(void);
extern char *IAP_GetPath(void);
const tagVmMemItem *gMemTab[]={
    {0x20000000,(512-32)*1024},\
    NULL,\
};

// ============================================================================
// 功能： 灯亮
// 参数：
// 返回：
// 备注： 由驱动实现,TODO,改为fcntl逻辑；
// ============================================================================
__attribute__((weak)) void USB_IAP_TurnOnLED(void)
{
    ;
}

// ============================================================================
// 功能： 灯灭
// 参数：
// 返回：
// 备注： 由驱动实现,TODO,改为fcntl逻辑；
// ============================================================================
__attribute__((weak)) void USB_IAP_TurnOffLED(void)
{
    ;
}


// ============================================================================
// 功能： 灯灭
// 参数：
// 返回：
// 备注： 由驱动实现,TODO,改为fcntl逻辑；
// ============================================================================
__attribute__((weak)) void USB_UDiskReset(void)
{
    ;
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注： 函数500微秒被调用一次
// ============================================================================
u32 USB_IAP_StatusDeamon(struct Wdt *reg)
{
    static u8 toggle;

    switch(gRunningStatus)
    {
    case INIT:
            USB_IAP_TurnOffLED();

            break;

    case RUN:
            // 500微秒周期进行亮灭
            if(toggle)
            {
                toggle = 0;
                USB_IAP_TurnOnLED();
            }
            else
            {
                toggle = 1;
                USB_IAP_TurnOffLED();
            }

            if(dwDeamonInspect++ > 20)
            {
                dwDeamonInspect = 0;
                printf("\r\nIAP : DEAMON : unknown state during USB update, reset all.\r\n");
                DJY_EventDelay(1000000);
                CPU_Reset(CN_BOOT_LEGALKEY);
            }
            break;

    case DONE:
            // 常亮
            USB_IAP_TurnOnLED();
            break;

    case ERROR:
            // 一长两短的亮灯
            if(toggle < 4) USB_IAP_TurnOnLED();
            else if(toggle < 5) USB_IAP_TurnOffLED();
            else if(toggle < 6) USB_IAP_TurnOnLED();
            else if(toggle < 7) USB_IAP_TurnOffLED();
            else if(toggle < 8) USB_IAP_TurnOnLED();
            else if(toggle < 9) USB_IAP_TurnOffLED();
            else if(toggle < 10) toggle = 0;

            toggle++;
//          必须大于10秒，因为在升级过程中，拔掉U盘的话，USB协议栈里会等10秒钟来给电容放电，需要等这个时间过了USB才能重新进入IDLE状态。
//          所以这里等10秒，等USB协议栈里的10秒延时过了之后，再来重启HUB。
//          如果没大于10秒，就重启HUB的话，那么在HUB掉电的时候，中断检测到HUB断开后，USB协议栈还在等待那10秒的电容放电，
//          USB协议栈还没来得及把USB模块进入IDLE状态，而这时HUB上电的话，中断检测到端口状态发生变化，进行连接。但是USB模块的状态还是disconnected
//          状态，未进入IDLE状态。这时连接HUB会失败。所以需要第二次重启HUB才能重启成功。
            if(dwDeamonInspect++ > 20)
            {
                dwDeamonInspect = 0;
                if(!MSC_DeviceReady(0))
                {
                    printf("\r\nIAP : DEAMON : unknown state during USB update, reset all.\r\n");
                    DJY_EventDelay(1000000);
                    USB_UDiskReset();
                }
                else
                {
                    printf("\r\nIAP : DEAMON : Enter the error state and do not detect the usb drive.\r\n");
                }
            }
            break;
    default:
        break;
    }

    return (0);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
void USB_IAP_StatusInit(void)
{

    if(!Wdt_Create("usb iap status", 500000, USB_IAP_StatusDeamon, EN_BLACKBOX_DEAL_IGNORE, 0, 0))
    {
        printf("\r\n: erro : iapusb : status initial failed\r\n");
        gRunningStatus = ERROR;
        do
        {
            USB_IAP_StatusDeamon(NULL);   //这里最好别加死循环吧，万一狗没申请成功，程序就死在这里了

        }while(1);
    }

    gRunningStatus = INIT;
}
// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
s32 __UpdateThroughUSB(char *pAPP, u32 dwOpt)
{
    DIR *dir;
    u8 updatestate = 1;
    struct dirent *structDirent;
    char *path;
    char *buffer;
    u32 bit[2];
    s32 res;
    struct stat sourceInfo;
    u8 i = 0;
    u8 found = 0;
    s32 source = -1;
    s32 destination = -1;
    u32 resetflag = 0;
    char *program = pAPP;

    u32 dbgCount = 0;

    for(i=0; i<2; i++)
    {
        bit[i] = dwOpt & 0x1;
        dwOpt = dwOpt >> 1;
    }

    gRunningStatus = INIT;

    do{
        if(!MSC_DeviceReady(0))
            break; // USB设备已准备好

        DJY_EventDelay(1000000); // 1s跑一次

        if((bit[0]) && (++i > 60))
        {
            if(resetflag)
            {
                printf("\r\n: info : iapusb : no U disk was found, switch to application mode in 10 seconds.\r\n\r\n\r\n\r\n\r\n");
                DJY_EventDelay(10000000); // 延时10s，以保证打印能输出
                CPU_Reset(CN_BOOT_LEGALKEY);
                return (-1); // 未检测到USB设备
            }
            resetflag = 1;
            i = 0;
            USB_UDiskReset();
        }
    }while(1);

    buffer = malloc(__IAP_USB_BUFFER_SIZE);
    if(!buffer)
    {
        printf("\r\nIAP : USB : memory out.");
        return (-1);
    }

    gRunningStatus = RUN;
    dwDeamonInspect = 0;
    dir = opendir("/fat");
    if(!dir)
    {
        printf("\r\n: info : iapusb : directory \"fat\" is not exist! maybe \"FAT\" is not installed!\r\n");
        gRunningStatus = ERROR;
        return (-1);
    }

    dwDeamonInspect = 0;
    while((structDirent = readdir(dir)) != NULL)
    {
        if(0 == strcmp(structDirent->d_name, program))
        {
            printf("\r\n: info : iapusb : file \"%s\" will be programmed.",
                    structDirent->d_name);
            found = 1;
            break;
        }
    }

    if(!found)
    {
        printf("\r\n: info : iapusb : file \"%s\" is not found.", program);
        gRunningStatus = ERROR;
        updatestate = 0;
        goto __UPGRADE_ERROR;
    }

    path = buffer;
    strcpy(path, "/fat/");
    strcpy(path+5, structDirent->d_name);
    dwDeamonInspect = 0;
    source = open(path, O_RDONLY);
    if(-1 == source)
    {
        printf("\r\n: info : iapusb : cannot open source file \"%s\".", path);
        gRunningStatus = ERROR;
        updatestate = 0;
        goto __UPGRADE_ERROR;
    }

    dwDeamonInspect = 0;
    res = fstat(source, &sourceInfo);
    if(res)
    {
        printf("\r\n: info : iapusb : cannot statistics source file \"%s\".", path);
        gRunningStatus = ERROR;
        updatestate = 0;
        goto __UPGRADE_ERROR;
    }

    strcpy(path, "/iboot/");
    strcpy(path+7, structDirent->d_name);
    dwDeamonInspect = 0;
    destination = open(path, O_RDWR | O_CREAT);
    if(-1 == destination)
    {
        printf("\r\n: info : iapusb : cannot open destination file \"%s\"!", path);
        gRunningStatus = ERROR;
        updatestate = 0;
        goto __UPGRADE_ERROR;
    }

    printf("\r\nIAP : USB : application is updating   ");
    while(1)
    {
        dwDeamonInspect = 0;
        dbgCount++;
        if(9 == dbgCount)
        {
            DJY_EventDelay(1000);
        }

        res = read(source, buffer, __IAP_USB_BUFFER_SIZE);
        if((!res) || (-1 == res))
        {
            printf("\b\b\b error<read source, %d>.", dbgCount);
            dwDeamonInspect = 0;
            gRunningStatus = ERROR;
            updatestate = 0;
            break; // 没有数据可读
        }

        sourceInfo.st_size -= res; // 剩余

        dwDeamonInspect = 0;
        if(res != write(destination, buffer, res))
        {
            printf("\b\b\b error<write destination>.");
            gRunningStatus = ERROR;
            updatestate = 0;
            break;
        }

        if(!sourceInfo.st_size)
        {
            printf("\b\b\b\b\b\bed successfully.");
            gRunningStatus = DONE;
            HAL_ClrUpdateFlag();
            break; // 全部读完
        }

        switch(i)
        {
        case 0 : printf("\b\b\b.  "); i = 1; break;
        case 1 : printf("\b\b\b.. "); i = 2; break;
        case 2 : printf("\b\b\b..."); i = 0; break;
        default: i = 0;break;
        }

    }

__UPGRADE_ERROR:
    closedir(dir);
    printf("\r\n: info : iapusb : thread exit ...");
    if(-1 != source)
    {
        res = close(source);
        if(res)
        {
            printf("\r\n: info : iapusb : close source file failed.");
        }
    }

    if(-1 != destination)
    {
        res = close(destination);
        if(res)
        {
            printf("\r\n: info : iapusb : close destination file failed.");
        }
    }

    printf("\r\n");

    if((bit[1]) && (gRunningStatus == DONE))
    {
        printf("\r\nIAP : USB : application update succeed, switch to application mode in 10 seconds.\r\n\r\n\r\n\r\n\r\n");
        DJY_EventDelay(10000000); // 延时10s，以保证打印能输出
        Sh_RunAPP(NULL);
        return (0); // 未检测到USB设备
    }
    if(updatestate)
    {
        return (0);
    }
    else
    {
        return (-1);
    }
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
void UpdateThroughUSB_T(void)
{
    char *file;
    u32 bits;

    DJY_GetEventPara((ptu32_t*)&file, (ptu32_t*)&bits);

    __UpdateThroughUSB(file, bits);
}

// ============================================================================
// 功能：监视U盘是否插上，如果插上而且U盘里有升级文件则把IbootFlag修改为Runiboot,否则IbootFlag为Runapp
//     并且把升级的方式和目标文件名传输给Iboot
// 参数：
// 返回：
// 备注：
// ============================================================================
void __SniffUSB(char *pFile,u32 dwOpt)
{
    DIR *dir;
    u8 UDisk_Flag = 1;
    u32 methodBackup;
    char *pathBackup;
    struct dirent *structDirent;
    char *program = pFile;

    methodBackup = IAP_GetMethod();
    pathBackup = IAP_GetPath();
    HAL_ClrUpdateFlag();
    while(1)
    {
        if((UDisk_Flag == 1) && (!MSC_DeviceReady(0)))
        {
            Amend_IBootFlag_RunAPP();
            dir = opendir("/fat");
            if(!dir)
            {
                printf("USB : directory \"fat\" is not exist! maybe \"FAT\" is not installed!\r\n");
            }

            while((!MSC_DeviceReady(0)) && (structDirent = readdir(dir)) != NULL)
            {
                if(0 == strcmp(structDirent->d_name, program))
                {
                    HAL_SetUpdateFlag();
                    IAP_SetMethod(3);
                    IAP_SetPath(pFile);
                    UDisk_Flag = 0;
                    if(dwOpt == 1)
                    {
                        ModuleInstall_OsBoot(gMemTab,NULL,GetBootMethodSoft);
                        Amend_IBootFlag_RunIBoot();
                        printf("\r\nIf you need to upgrade, please enter \"reset 0xaa55aa55\".\r\n");
                    }
                    else
                    {
                        RunIboot();
                    }

                    break;
                }
            }

        }

        if((UDisk_Flag == 0) && (MSC_DeviceReady(0) != 0))
        {
            // 没有执行升级逻辑，则将升级状态复原。
            Amend_IBootFlag_RunAPP();
            HAL_ClrUpdateFlag();
            IAP_SetMethod(methodBackup);
            IAP_SetPath(pathBackup);
            UDisk_Flag = 1;
        }

        DJY_EventDelay(1000000);
    }
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
void SniffUSB_T(void)
{
    char *file;
    u32 bits;

    DJY_GetEventPara((ptu32_t*)&file, (ptu32_t*)&bits);

    __SniffUSB(file,bits);
}

// ============================================================================
// 功能：安装U盘升级功能
// 参数：bArgC -- 参数个数；
//     第1个参数 -- 功能选项（1， USB升级；0，USB监视）。
//     第2个参数 -- 升级或监视的文件名；
//     第3个参数 -- USB监视功能：1(表示需要手动输入命令才执行升级)；0（表示不需要手动输入命令，当插上U盘后会自动执行升级）；
//              USB升级功能：bit1(表示iboot60秒没检测到U盘的话会重启)；bit2(表示升级完成后自动运行APP)；
// 返回： 成功（0）; 失败（-1）;
// 备注：
// ============================================================================
s32 ModuleInstall_USB_IAP(u8 bArgC, ...)
{
    u8 i;
    u16 thread;
    va_list ap;
    u32 arg = 0, func = 0;
    char *file = NULL;
    const char *func0 = "IAP: sniff USB";
    const char *func1 = "IAP: update through USB";

    va_start(ap, bArgC);
    for(i = 0; i < bArgC; i++)
    {
        switch(i)
        {
            case 0: func = va_arg(ap, u32);break;
            case 1: file = (char*)va_arg(ap, u32);break;
            case 2: arg = va_arg(ap, u32);break;
            default: break;
        }
    }
    va_end(ap);

    if(0 == func)
    {
        thread = DJY_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 0,
                                SniffUSB_T, NULL, 0x1000, (char*)func0);

        if(thread != CN_EVTT_ID_INVALID)
        {
            DJY_EventPop(thread, NULL, 0, (ptu32_t)file, arg, 0);
            return (0);
        }
    }
    else if(1 == func)
    {
        USB_IAP_StatusInit();
        thread = DJY_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 0,
                                UpdateThroughUSB_T, NULL, 0x1000, (char*)func1);

        if(thread != CN_EVTT_ID_INVALID)
        {
            DJY_EventPop(thread, NULL, 0, (ptu32_t)file, arg, 0);
            return (0);
        }
    }

    return (-1);
}
#endif
