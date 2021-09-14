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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
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
// ���ܣ���ȡ�ļ���CRCֵ
// ������pPath -- �ļ�·����pCRC -- �ļ�CRCУ��ֵ
// ���أ��ɹ���0����ʧ�ܣ�-1��
// ��ע��
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
// ���ܣ�ͨ���ļ�ϵͳ�ķ�ʽ��������
// ������pPath -- �ļ�·����
// ���أ��ɹ���0����ʧ�ܣ�-1��
// ��ע��
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
        file = srcPath; // ·������ֻ���ļ���
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
// ���ܣ������г�����豸����iboot�ļ�ϵͳ
// ������pDevPath -- �豸·��
// ���أ�0 -- �ɹ��� -1 -- ʧ�ܣ�
// ��ע���³����в�������ʹ��
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

    // ��ȡ�豸��Ϣ
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

    // ��ȡ�豸�и����ļ���ͷ����Ϣ
    start = 0; // Ҫ�ø�������ʽ�Ļ�iap�ļ�ϵͳ����ʼ������0��ʼ
    res = um->mread(start++, buf, opt);
    if(0 != res)
    {
        error_printf("iap","cannot read the source device(%s).", pDevPath);
        res = -1;
        goto __ERROR_OIU;
    }

    head = (struct headFormat*)buf;
    info_printf("iap","latest file in device is <%s>, size <%d>", head->name, head->size);

    path = malloc(strlen(head->name) + 8); // ��"/iboot/xxxx"�Ŀռ�
    if(!path)
    {
        error_printf("iap","memory out.\r\n");
        res = -1;
        goto __ERROR_OIU;
    }

    // �ж�iboot�е��ļ��ǲ������µ�
    memcpy(path, update, 7);
    memcpy(path+7, head->name, (strlen(head->name)+1));
    // ���豸�еĳ�����µ�iboot
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
// ���ܣ�����application
// ������bArgC -- ����������
//      ����1 -- method;
//      methodΪ1����ʾͨ���ļ�ϵͳ��ʽ�����������2��ʾ�ļ�·����
//      methodΪ2����ʾͨ���豸����������2��ʾ�豸·��(����Ǿ��߼������豸�ϰ�װ����IAP)
//      methodΪ3����ʾͨ��U������������2��ʾ�µ��ļ���
//      ����2 -- param2��Ŀ���ļ�·��
//      ����3 -- dwOpt��ֻ����methodΪ3ʱ��Ч��
//      USB�������ܣ�bit1(��ʾiboot60��û��⵽U�̵Ļ�������)��bit2(��ʾ������ɺ��Զ�����APP)��
//      Ĭ�ϣ�optΪ3
// ���أ�0 -- �ɹ��� ���� -- ʧ�ܣ�
// ��ע��TODO:�����������߼����ൽ�˺���
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
