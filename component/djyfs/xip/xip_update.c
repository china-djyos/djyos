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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <device/djy_flash.h>
#include <device.h>
#include "xip.h"
#include "dbug.h"
#include <device/unit_media.h>
#if 0
#define LOCAL_BUF_SIZE        512
// ============================================================================
// 功能：获取文件的CRC值
// 参数：pPath -- 文件路径；pCRC -- 文件CRC校验值
// 返回：成功（0）；失败（-1）
// 备注：
// ============================================================================
extern s32 crc32Init(u32 *pCRC);
extern s32 crc32Run(u32 *pCRC, u8 *pBuf, u32 dwLen);
extern s32 crc32Exit(u32 *pCRC);
s32 __IAP_GetCRC(char *pPath, u32 *pCRC)
{
    u32 crc, size;
    s32 res;
    struct stat info;
    u8 *buf = NULL;
    s32 file = -1;

    res = stat(pPath, &info);
    if(-1 == res)
    {
        error_printf("iap","cannot get <%s>'s information.\r\n", pPath);
        return (-1);
    }

    buf = malloc(512);
    if(!buf)
    {
        error_printf("iap","memory out.\r\n");
        return (-1);
    }

    file = open(pPath, O_RDONLY);
    if(-1 == file)
    {
        error_printf("iap","cannot open <%s>.\r\n", pPath);
        res = -1;
        goto __EXIT_IAP_GET_CRC;

    }

    if(!crc32Init(&crc))
    {
        error_printf("iap","cannot create CRC.\r\n");
        res = -1;
        goto __EXIT_IAP_GET_CRC;
    }

    do{
        res = read(file, buf, 512);
        if((!res) && info.st_size)
        {
            error_printf("iap","cannot read <%s>.\r\n", pPath);
            res = -1;
            goto __EXIT_IAP_GET_CRC;
        }

        crc32Run(&crc, buf, res);
        size += res;
    }while(size >= info.st_size);

    if(crc32Exit(&crc))
    {
        *pCRC = crc;
        res = 0;
    }
    else
    {
        res = -1;
    }

__EXIT_IAP_GET_CRC:

    free(buf);
    if(-1 != file)
        close(file);

    return (res);
}

// ============================================================================
// 功能：通过文件系统的方式升级程序
// 参数：pPath -- 文件路径；
// 返回：成功（0）；失败（-1）
// 备注：
// ============================================================================
s32 __UpdateThroughFS(char *pPath)
{
    s32 src, des, left, size, res;
    u8 buf[LOCAL_BUF_SIZE];
    struct stat info;
    char *desPath, *file, *srcPath = pPath;

    if(!pPath)
    {
        error_printf("iap","bad parameters for update.");
        return (-1);
    }

    if(stat(srcPath, &info))
    {
        error_printf("iap","cannot get the info of update file, maybe does not exist, <name:%s>.", srcPath);
        return (-1);
    }

    src = open(srcPath, O_RDONLY);
    if(-1 == src)
    {
        error_printf("iap","can not open update file, <name:%s>, <size:%lld>.", srcPath, info.st_size);
        return (-1);
    }

    desPath = (char*)buf;
    file = strrchr(srcPath, '/');
    if(!file)
    {
        file = srcPath; // 路径里面只有文件；
        sprintf(desPath, "%s%s", "/iboot/", file);
    }
    else
    {
        sprintf(desPath, "%s%s", "/iboot", file);
    }

    des = open(desPath, O_RDWR | O_CREAT);
    if(-1 == des)
    {
        error_printf("iap","cannot create file to update, <%s>.", desPath);
        return (-1);
    }

        debug_printf("iap","update method #0, <from:%s>, <size:%lld>.", srcPath, (info.st_size>>10));
    left = info.st_size;
    while(left)
    {

        if(left > LOCAL_BUF_SIZE)
            size = LOCAL_BUF_SIZE;
        else
            size = left;

        res = read(src, buf, size);
        if(size != res)
        {
            error_printf("iap","read file to update, <size:%lld>, <left:%xH>.", info.st_size, left);
            break;
        }

        res = write(des, buf, size);
        if(size != res)
        {
            error_printf("iap","write file to update, <size:%lld>, <left:%xH>.", info.st_size, left);
            break;
        }

        left -= res;
    }

    if(left)
        res = -1;
    else
        res = 0;

    close(src);
    close(des);
    return (res);

}

// ============================================================================
// 功能：将运行程序从设备拷入iboot文件系统
// 参数：pDevPath -- 设备路径
// 返回：0 -- 成功； -1 -- 失败；
// 备注：新程序中不允许再使用
// ============================================================================
s32 OBSOLETE_IAP_Update(char *pDevPath)
{
    u32 size, start;
    char *path;
    u8 *buf;
    struct umedia *um;
    struct uopt opt;
    struct headFormat *head;
    char *update = "/iboot/";
    s32 fd, res, new,flash_page_size;

    if(!strlen(pDevPath))
    {
        error_printf("iap","parameter is bad.");
        return (-1);
    }

    // 获取设备信息
    fd = open(pDevPath, O_RDONLY);
    if(-1 == fd)
    {
        error_printf("iap","device do not exist(%s).", pDevPath);
        return (-1);
    }

    res = fcntl(fd, F_GETDRVTAG, &um);
    if(-1 == res)
    {
        error_printf("iap","device has no tag(%s).", pDevPath);
        close(fd);
        return (-1);
    }
    opt = um->opt;
    flash_page_size = (1 << um->usz);
    buf = um->ubuf;
    if(!buf)
    {
        buf = (u8*)malloc(flash_page_size);
        if(!buf)
        {
            error_printf("iap","memory out.\r\n");
            close(fd);
            return (-1);
        }
    }

    // 获取设备中更新文件的头部信息
    start = 0; // 要用该升级方式的话iap文件系统的起始必须是0开始
    res = um->mread(start++, buf, opt);
    if(0 != res)
    {
        error_printf("iap","cannot read the source device(%s).", pDevPath);
        res = -1;
        goto __ERROR_OIU;
    }

    head = (struct headFormat*)buf;
    info_printf("iap","latest file in device is <%s>, size <%d>", head->name, head->size);

    path = malloc(strlen(head->name) + 8); // 即"/iboot/xxxx"的空间
    if(!path)
    {
        error_printf("iap","memory out.\r\n");
        res = -1;
        goto __ERROR_OIU;
    }

    // 判断iboot中的文件是不是最新的
    memcpy(path, update, 7);
    memcpy(path+7, head->name, (strlen(head->name)+1));
    // 将设备中的程序更新到iboot
    new = open(path, O_RDWR | O_CREAT);
    if(-1 == new)
    {
        error_printf("iap","open new failed.\r\n");
        res = -1;
        goto __ERROR_OIU;
    }

    size = flash_page_size - sizeof(struct headFormat);
    res = write(new, (buf+sizeof(struct headFormat)), size);
    if((s32)size!= res)
    {
        error_printf("iap","write failed.\r\n");
        res = -1;
        goto __ERROR_OIU;
    }

    size = head->size - size;
    while(size)
    {
        res = um->mread(start++, buf, opt);
        if(0 != res)
        {
            error_printf("iap","cannot read the source device.\r\n");
            res = -1;
            goto __ERROR_OIU;
        }

        res = flash_page_size;
        if(res > (s32)size)
            res = size;

        if(res != write(new, (buf), res))
        {
            error_printf("iap","write failed.\r\n");
            res = -1;
            goto __ERROR_OIU;
        }

        size -= res;
    }

    res = 0;

__ERROR_OIU:

    close(fd);
    close(new);

    if(!um->ubuf)
        free(buf);

    if(path)
        free(path);

    return (res);

}

// ============================================================================
// 功能：升级application
// 参数：bArgC -- 参数个数；
//      参数1 -- method;
//      method为1，表示通过文件系统方式升级，则参数2表示文件路径。
//      method为2，表示通过设备升级，参数2表示设备路径(这个是旧逻辑，此设备上安装的是IAP)
//      method为3，表示通过U盘升级，参数2表示新的文件名
//      参数2 -- param2：目标文件路径
//      参数3 -- dwOpt：只有在method为3时有效。
//      USB升级功能：bit1(表示iboot60秒没检测到U盘的话会重启)；bit2(表示升级完成后自动运行APP)。
//      默认：opt为3
// 返回：0 -- 成功； 其他 -- 失败；
// 备注：TODO:将所有升级逻辑到类到此函数
// ============================================================================
s32 IAP_Update(u8 bArgC, ...)
{
    u32 param2 = (u32)NULL, method = -1;
    s32 res = -1;
    u32 opt = 3;
    u8 i;
    va_list ap;

    extern u32 __UpdateThroughUSB(char *filename, u32 parameter);
    extern void USB_IAP_StatusInit(void);


    va_start(ap, bArgC);
    for(i = 0; i < bArgC; i++)
    {
        switch(i)
        {
            case 0 : method = va_arg(ap, u32); break;
            case 1 : param2 = va_arg(ap, u32); break;
            case 2 : opt = va_arg(ap, u32); break;
            default: break;
        }
    }
    va_end(ap);

    switch(method)
    {
        case 1 : res = __UpdateThroughFS((char*)param2); break;
        case 2 : res = OBSOLETE_IAP_Update((char*)param2);break;
        case 3 :
            USB_IAP_StatusInit();
            res = __UpdateThroughUSB((char*)param2, opt);break;
        default: break;
    }

    if(res)
    {
        info_printf("IAP","update failed(method:%xH)(path:%s)", method, (char*)param2);
    }
    else
    {
        info_printf("IAP","update successful.");
    }

    debug_printf("iap","\r\n");
    return (res);
}
#endif
