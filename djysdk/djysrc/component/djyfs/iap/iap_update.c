//-----------------------------------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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
#include <driver/flash/flash.h>
#include <driver.h>
#include "iap.h"

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
        printf("\r\nIAP : error : cannot get <%s>'s information.\r\n", pPath);
        return (-1);
    }

    buf = malloc(512);
    if(!buf)
    {
        printf("\r\nIAP : error : memory out.\r\n");
        return (-1);
    }

    file = open(pPath, O_RDONLY);
    if(-1 == file)
    {
        printf("\r\nIAP : error : cannot open <%s>.\r\n", pPath);
        res = -1;
        goto __EXIT_IAP_GET_CRC;

    }

    if(!crc32Init(&crc))
    {
        printf("\r\nIAP : error : cannot create CRC.\r\n");
        res = -1;
        goto __EXIT_IAP_GET_CRC;
    }

    do{
        res = read(file, buf, 512);
        if((!res) && info.st_size)
        {
            printf("\r\nIAP : error : cannot read <%s>.\r\n", pPath);
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
        printf("\r\nIAP : error : bad parameters for update.");
        return (-1);
    }

    if(stat(srcPath, &info))
    {
        printf("\r\nIAP : error : cannot get the info of update file, maybe does not exist, <name:%s>.", srcPath);
        return (-1);
    }

    src = open(srcPath, O_RDONLY);
    if(-1 == src)
    {
        printf("\r\nIAP : error : can not open update file, <name:%s>, <size:%xH>.", srcPath, info.st_size);
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
        printf("\r\nIAP : error : cannot create file to update, <%s>.", desPath);
        return (-1);
    }

    printf("\r\nIAP : debug : update method #0, <from:%s>, <size:%dKB>.", srcPath, (info.st_size>>10));
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
            printf("\r\nIAP : error : read file to update, <size:%xH>, <left:%xH>.", info.st_size, left);
            break;
        }

        res = write(des, buf, size);
        if(size != res)
        {
            printf("\r\nIAP : error : write file to update, <size:%xH>, <left:%xH>.", info.st_size, left);
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
// ��ע��
// ============================================================================
s32 OBSOLETE_IAP_Update(char *pDevPath)
{
    u32 size;
    u32 start;
    char *path;
    u8 *buf;
    struct FileContext *fileCt;
    struct DjyDevice *dev;
    struct FlashChip *flash;
    struct HeadFormat *head;
#if 0
    struct stat info;
#endif
    s32 res = 0;
    char *root = "/iboot/";
    struct VFile *devFp = NULL;
    s32 new = -1;

    if(!strlen(pDevPath))
    {
        printf("\r\nIAP : error : parameter is bad.\r\n");
        return (-1);
    }

    // ��ȡ�豸��Ϣ
    devFp = __Open(pDevPath, M_READ | M_WRITE | M_OPEN);// ���豸
    if (!devFp)
    {
        printf("\r\nIAP : error : device do not exist.\r\n");
        return (-1);
    }

    fileCt = (struct FileContext *)(devFp->Private);
    dev = (struct DjyDevice *)(fileCt->Private);
    flash = (struct FlashChip *)(dev->PrivateTag);
    buf = flash->Buf;
    if(!buf)
    {
        buf = (u8*)malloc(flash->dwPageBytes);
        if(!buf)
        {
            printf("\r\nIAP : error : memory out.\r\n");
            return (-1);
        }
    }

    // ��ȡ�豸�и����ļ���ͷ����Ϣ
    start = flash->dwPagesReserved; // ��ǰ�߼��Ǵӱ���ҳ��ʼ
    res = flash->Ops.RdPage(start++, buf, 0);
    if(flash->dwPageBytes != res)
    {
        printf("\r\nIAP : error : cannot read the source device.\r\n");
        res = -1;
        goto FAILED;
    }

    head = (struct HeadFormat*)buf;
    printf("\r\nIAP : info : latest file in device name <%s>, size <%d>", head->Name, head->Size);

    path = malloc(strlen(head->Name) + 8); // ��"/iboot/xxxx"�Ŀռ�
    if(!path)
    {
        printf("\r\nIAP : error : memory out.\r\n");
        res = -1;
        goto FAILED;
    }

    // �ж�iboot�е��ļ��ǲ������µ�
    memcpy(path, root, 7);
    memcpy(path+7, head->Name, (strlen(head->Name)+1));
#if 0
    res = stat(path, &info);
    if(!res)
    {
        if(info.st_size == head->Size)
        {
            printf("\r\nIAP : info : running application is already the latest.\r\n");
            res = 0;
            goto FAILED;
        }
    }
#endif
    // ���豸�еĳ�����µ�iboot
    new = open(path, O_RDWR | O_CREAT);
    if(-1 == new)
    {
        printf("\r\nIAP : error : open new failed.\r\n");
        res = -1;
        goto FAILED;
    }

    size = flash->dwPageBytes - FILE_HEAD_SIZE;
    res = write(new, (buf+FILE_HEAD_SIZE), size);
    if(size!= res)
    {
        printf("\r\nIAP : error : write failed.\r\n");
        res = -1;
        goto FAILED;
    }

    size = head->Size - size;
    while(size)
    {
        res = flash->Ops.RdPage(start++, buf, 0);
        if(flash->dwPageBytes != res)
        {
            printf("\r\nIAP : error : cannot read the source device.\r\n");
            res = -1;
            goto FAILED;
        }

        if(res > size)
            res = size;

        if(res != write(new, (buf), res))
        {
            printf("\r\nIAP : error : write failed.\r\n");
            res = -1;
            goto FAILED;
        }

        size -= res;
    }

    res = 0;

FAILED:

    if(devFp)
        fclose(devFp);

    if((new) && (-1 != new))
        close(new);

    if(!flash->Buf)
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
// ���أ�0 -- �ɹ��� ���� -- ʧ�ܣ�
// ��ע��TODO:�����������߼����ൽ�˺���
// ============================================================================
s32 IAP_Update(u8 bArgC, ...)
{
    u32 param2 = (u32)NULL, method = -1;
    s32 res = -1;
    u8 i;
    va_list ap;

    va_start(ap, bArgC);
    for(i = 0; i < bArgC; i++)
    {
        switch(i)
        {
            case 0 : method = va_arg(ap, u32); break;
            case 1 : param2 = va_arg(ap, u32); break;
            default: break;
        }
    }
    va_end(ap);

    switch(method)
    {
        case 1 : res = __UpdateThroughFS((char*)param2); break;
        case 2 : res = OBSOLETE_IAP_Update((char*)param2);break;
        default: break;
    }

    if(res)
    {
        printf("\r\nIAP : info : update failed.<method:%xH><path:%s>", method, (char*)param2);
    }
    else
    {
        printf("\r\nIAP : info : update successful.");
    }

    printf("\r\n");
    return (res);
}

// ============================================================================
// ���ܣ���ȡIAP�ļ���CRCֵ
// ������pDevPath -- �豸·��; pCRC -- CRCֵ
// ���أ�0 -- �ɹ��� -1 -- ʧ�ܣ�
// ��ע��
// ============================================================================
s32 IAP_CRC(char *pDevPath, u32 *pCRC)
{
    u32 start;
    u8 *buf;
    struct FileContext *fileCt;
    struct DjyDevice *dev;
    struct FlashChip *flash;
    struct HeadFormat *head;
    s32 res = 0;
    struct VFile *devFp = NULL;


    if(!strlen(pDevPath) || (!pCRC))
    {
        printf("\r\nIAP : error : parameter is bad.\r\n");
        return (-1);
    }

    // ��ȡ�豸��Ϣ
    devFp = __Open(pDevPath, M_READ | M_WRITE | M_OPEN);// ���豸
    if (!devFp)
    {
        printf("\r\nIAP : error : device do not exist.\r\n");
        return (-1);
    }

    fileCt = (struct FileContext *)(devFp->Private);
    dev = (struct DjyDevice *)(fileCt->Private);
    flash = (struct FlashChip *)(dev->PrivateTag);
    buf = flash->Buf;
    if(!buf)
    {
        buf = (u8*)malloc(flash->dwPageBytes);
        if(!buf)
        {
            printf("\r\nIAP : error : memory out.\r\n");
            return (-1);
        }
    }

    // ��ȡ�豸�и����ļ���ͷ����Ϣ
    start = flash->dwPagesReserved; // ��ǰ�߼��Ǵӱ���ҳ��ʼ
    res = flash->Ops.RdPage(start, buf, 0);
    if(flash->dwPageBytes != res)
    {
        printf("\r\nIAP : error : cannot read the source device.\r\n");
        res = -1;
        goto FAILED;
    }

    head = (struct HeadFormat*)buf;
    if(S_APP_UPDATE_DONE == head->Signature)
    {
        *pCRC = head->CRC;
        res = 0;
    }
    else
    {
        printf("\r\nIAP : error : file's signature is bad<%x>.\r\n", head->Signature);
        res = -1;
    }

FAILED:

    if(devFp)
        fclose(devFp);

    if(!flash->Buf)
        free(buf);

    return (res);
}
