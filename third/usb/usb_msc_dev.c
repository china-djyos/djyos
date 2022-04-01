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
#include <stdlib.h>
#include <device.h>
#include <port/drivers/fat_drivers.h>
#include <ff11/src/diskio.h>
#include "./stm32_usb_host_library/class/msc/inc/usbh_msc.h"
#include "usb.h"
#include <dbug.h>
#include <djyfs/filesystems.h>

int USB_Status(void);
int USB_Initialize(void);
int USB_Read(BYTE *Buff, DWORD Sector, UINT Count);
int USB_Write(BYTE *Buff, DWORD Sector, UINT Count);
int USB_Ioctl( BYTE Cmd, void *Buff);

extern struct Object *s_ptDeviceRoot;
static USBH_HandleTypeDef *pMSC;

struct FatDrvFuns USB_Drv =
{
    .DrvStatus     = USB_Status,
    .DrvInitialize = USB_Initialize,
    .DrvRead       = USB_Read,
    .DrvWrite      = USB_Write,
    .DrvIoctl      = USB_Ioctl
};

// ============================================================================
// 功能: USB MSC状态
// 参数:
// 输出:
// 返回:
// ============================================================================
s32 __USB_Status(void)
{
    s32 res = 1; //RES_ERROR;

    if(USBH_MSC_UnitIsReady (pMSC, 0))
    {
        res = 0; //RES_OK;
    }
    else
    {
        res = 1; //RES_ERROR;
    }

    return res;
}

// ============================================================================
// 功能: USB初始化
// 参数:
// 返回:
// 备注: 可以将设备初始化放在这里;
// ============================================================================
s32 __USB_Initialize(void)
{
    static BYTE Done = 0;

    if(1 == Done)
        return (0);

    Done = 1;
    return (0);
}

// ============================================================================
// 功能: USB读数据
// 参数:
// 返回:
// 备注:
// ============================================================================
s32 __USB_Read(u8 *buff, u32 sector, u32 count)
{
    u8 pdrv = 0;
    s32 res = 1; // RES_ERROR;
    MSC_LUNTypeDef info;
    USBH_StatusTypeDef  status = USBH_OK;
    u32 scratch [_MAX_SS / 4];

    if ((u32)buff & 3) // DMA Alignment issue, do single up to aligned buffer
    {
        while ((count--)&&(status == USBH_OK))
        {
            status = USBH_MSC_Read(pMSC, pdrv, sector + count, (uint8_t *)scratch, 1);
            if(status == USBH_OK)
            {
                memcpy(&buff[count * _MAX_SS], scratch, _MAX_SS);
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        status = USBH_MSC_Read(pMSC, pdrv, sector, buff, count);
    }

    if(status == USBH_OK)
    {
        res = 0; // RES_OK;
    }
    else
    {
        USBH_MSC_GetLUNInfo(pMSC, pdrv, &info);

        switch (info.sense.asc)
        {
            case SCSI_ASC_LOGICAL_UNIT_NOT_READY:
            case SCSI_ASC_MEDIUM_NOT_PRESENT:
            case SCSI_ASC_NOT_READY_TO_READY_CHANGE:
                printf("\r\n: info : usb : disk is not ready!");
                res = 3; // RES_NOTRDY;
                break;

            default:
                res = 1; // RES_ERROR;
                break;
        }
    }

    return res;
}

// ============================================================================
// 功能: 写数据
// 参数: buff -- 写入内容。
//      sector -- 目标页号。
//      count -- 页数量。
// 返回: 0 -- 正确;-1 -- 错误;
// 备注:
// ============================================================================
s32 __USB_Write(u8 *buff, u32 sector, u32 count)
{
    u8 pdrv = 0;
    s32 res = 0; // RES_ERROR;
    MSC_LUNTypeDef info;
    USBH_StatusTypeDef  status = USBH_OK;
    u32 scratch [_MAX_SS / 4];

    if ((u32)buff & 3) /* DMA Alignment issue, do single up to aligned buffer */
    {
        while (count--)
        {
            memcpy (scratch, &buff[count * _MAX_SS], _MAX_SS);

            status = USBH_MSC_Write(pMSC, pdrv, sector + count, (BYTE *)scratch, 1);
            if(status == USBH_FAIL)
            {
                break;
            }
        }
    }
    else
    {
        status = USBH_MSC_Write(pMSC, pdrv, sector, (BYTE *)buff, count);
    }

    if(status == USBH_OK)
    {
        res = 0; // RES_OK;
    }
    else
    {
        USBH_MSC_GetLUNInfo(pMSC, pdrv, &info);

        switch (info.sense.asc)
        {
            case SCSI_ASC_WRITE_PROTECTED:
                USBH_UsrLog("USB Disk is Write protected!");
                res = 2; // RES_WRPRT;
                break;

            case SCSI_ASC_LOGICAL_UNIT_NOT_READY:
            case SCSI_ASC_MEDIUM_NOT_PRESENT:
            case SCSI_ASC_NOT_READY_TO_READY_CHANGE:
                USBH_UsrLog("USB Disk is not ready!");
                res = 3; // RES_NOTRDY;
                break;

            default:
                res = 1; // RES_ERROR;
                break;
        }
    }

    return (res);
}

// ============================================================================
// 功能:
// 参数:
// 返回:
// 备注:
// ============================================================================
s32 __USB_Ioctl( u8 cmd, void *buff)
{
    u8 pdrv = 0;
    DRESULT res = 0; // RES_OK;
    MSC_LUNTypeDef info;

    switch (cmd) {
        /* Make sure that no pending write process */
        case CTRL_SYNC:
            res = 0; // RES_OK;
            break;

        /* Get number of sectors on the disk (DWORD) */
        case GET_SECTOR_COUNT:
            if(USBH_MSC_GetLUNInfo(pMSC, pdrv, &info) == USBH_OK)
            {
                *(u32*)buff = info.capacity.block_nbr;
                res =  0; // RES_OK;
            }
            else
            {
                res =  1; // RES_ERROR;
            }
            break;

        case GET_SECTOR_SIZE: /* Get R/W sector size (WORD) */
            if(USBH_MSC_GetLUNInfo(pMSC, pdrv, &info) == USBH_OK)
            {
                *(u32*)buff = info.capacity.block_size;
                res = 0; // RES_OK;
            }
            else
            {
                res = 1; // RES_ERROR;
            }
            break;

        /* Get erase block size in unit of sector (DWORD) */
        case GET_BLOCK_SIZE:
            if(USBH_MSC_GetLUNInfo(pMSC, pdrv, &info) == USBH_OK)
            {
                *(u32*)buff = info.capacity.block_size;
                res = 0; // RES_OK;
            }
            else
            {
                res = 1; // RES_ERROR;
            }
            break;

        default:
            res = 4; // RES_PARERR;
    }

    return res;
}

// ============================================================================
// 功能:
// 参数:
// 返回:
// 备注:
// ============================================================================
int USB_Ioctl( u8 Cmd, void *Buff)
{
    if(!pMSC)
    {
        printf("\r\n: erro : usb    : MSC class's device is not ready.");
        return (1);
    }

    return(__USB_Ioctl((BYTE)Cmd, Buff));
}

// ============================================================================
// 功能:
// 参数:
// 返回:
// 备注:
// ============================================================================
int USB_Status(void)
{
    if(!pMSC)
    {
        printf("\r\n: erro : usb    : MSC class's device is not ready.");
        return (1);
    }

    return (__USB_Status());
}

// ============================================================================
// 功能:
// 参数:
// 返回:
// 备注:
// ============================================================================
int USB_Initialize(void)
{
    return (__USB_Initialize());
}

// ============================================================================
// 功能:
// 参数:
// 返回:
// 备注:
// ============================================================================
int USB_Read(BYTE *Buff, DWORD Sector, UINT Count)
{
    if(!pMSC)
    {
        printf("\r\n: erro : usb    : MSC class's device is not ready.");
        return (1);
    }

    return(__USB_Read((u8*)Buff, (u32)Sector, (u32)Count));
}

// ============================================================================
// 功能:
// 参数:
// 返回:
// 备注:
// ============================================================================
int USB_Write(BYTE *Buff, DWORD Sector, UINT Count)
{
    if(!pMSC)
    {
        printf("\r\n: erro : usb    : MSC class's device is not ready.");
        return (1);
    }

    return(__USB_Write((u8*)Buff, (u32)Sector, (u32)Count));
}

// ============================================================================
// 功能: 查询U盘是否可用
// 参数:
// 返回:
// 备注:
// ============================================================================
s32 MSC_DeviceReady(u8 bID)
{
    USBH_HandleTypeDef *pHost;

    if(0 == bID)
        pHost = pMSC;

    return (USBH_Query(pHost));
}

// ============================================================================
// 功能: 注册USB设备MSC
// 参数: fs -- 需要挂载的文件系统
// 返回:
// 备注:
// ============================================================================
s32 USBH_ResigerDevMSC(const char *fs)
{
    static char *name = "msc";
    char *FullPath,*notfind;
    struct Object *targetobj;
    struct FsCore *super;

    if(!Device_Create((const char*)name, NULL, NULL, NULL, NULL, NULL, (ptu32_t)name))
    {
        printf("\r\n: erro : usb    : register device(MSC) failed.");
        return (-1); // register failure
    }
    if(fs != NULL)
    {
        targetobj = OBJ_MatchPath(fs, &notfind);
        if(notfind)
        {
            error_printf("usb"," not found need to install file system.");
            return -1;
        }
        super = (struct FsCore *)OBJ_GetPrivate(targetobj);
        super->MediaInfo = name;
        if(strcmp(super->pFsType->pType, "FAT") == 0)      //这里的"FAT"为文件系统的类型名，在文件系统的filesystem结构中
        {
            super->MediaDrv = &USB_Drv;
        }
        else
        {
            super->MediaDrv = 0;
            error_printf("usb","  install file system type not FAT");
            return -1;
        }

        FullPath = malloc(strlen(name)+strlen(s_ptDeviceRoot->name));  //获取msc的完整路径
        sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,name);
        File_BeMedia(FullPath,fs);     //在msc上挂载文件系统
        free(FullPath);
    }
    else
    {
        warning_printf("usb", "  No file system is installed");
    }

    printf("\r\n: info : usb    : registered device(MSC).");
    return (0); //
}

// ============================================================================-
// 功能: 注册USB设备
// 参数:
// 返回:
// 备注:
// ============================================================================
void USBH_SetMSC(USBH_HandleTypeDef *pHost)
{
    pMSC = pHost;
}

// ============================================================================
// 功能: 注销USB设备
// 参数:
// 返回:
// 备注:
// ============================================================================
void USBH_ResetMSC(USBH_HandleTypeDef *pHost)
{
    if(pHost == pMSC)
        pMSC = NULL;
}
