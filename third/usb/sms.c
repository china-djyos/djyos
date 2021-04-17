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

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <device.h>
#include <stdio.h>


char *SMS_CMDS[] = {
        "AT+CMGF",
        "AT+CPMS",
        "AT+CMGD",
        "AT+CSCS",
        "AT+CMGR"};

char *SMS_INIT_ARGS[] = {
        "=1",
        "=\"SM\",\"SM\",\"SM\"",
        "=0, 4",
        "=\"GSM\""};

char *SMS_END = " \r";
char *SMS_OK = "OK";
char *SMS_ERROR = "ERROR";
#define LOCAL_PRINT                 printk
#define LOCAL_RESPONSE_TIMEOUT      50
static char smsDev[10];
// ============================================================================
//���ܣ�����ȷ�ϡ�
//������pCommand -- ȷ�ϵ����
//     bLogic -- ȷ���߼���
//���أ�-1 -- ʧ�ܣ� 0 -- OK�� 1 -- ERROR��
//��ע��
// ============================================================================
s32 __SMS_CheckResponse(s32 pHandle, char *pCommand, u8 bLogic)
{
    u32 i, j, res, wait = LOCAL_RESPONSE_TIMEOUT;
    u32 responsed;
    char response = 0;
    u32 cLen = strlen(pCommand); // ȥ��β��һЩ�ַ�
    u8 checkCommand = 1, checkStatus = 1, log = 1;

    if(0x1 & bLogic)
    {
        checkStatus = 0;
    }

    if(0x2 & bLogic)
    {
        checkCommand = 0;
    }

    if(0x4 & bLogic)
    {
        log = 0;
    }

    i = 0;
    responsed = 0;
    while(checkCommand)
    {
        res = read(pHandle, (u8*)&response, 1);
        if(!res)
        {
            if(!wait--)
            {
                if(log)
                    LOCAL_PRINT("SMS : error : \"%s\" has %s response.\r\n", pCommand, (responsed?"error":"no"));

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
            i = 0; // ����

        if(i == cLen)
            break;
    }

    i = 0;
    j = 0;
    while(checkStatus)
    {
        if(SMS_OK[i] == response)
        {
            if(++i == strlen(SMS_OK))
                break;

        }
        else if(SMS_ERROR[j] == response)
        {
            if(++j == strlen(SMS_ERROR))
                return (1);
        }
        else
        {
            i = 0; // ����
            j = 0;
        }

        res = read(pHandle, (u8*)&response, 1);
        if(!res)
        {
            if(!wait--)
            {
                if(log)
                    LOCAL_PRINT("SMS : error : \"%s\" has %s response.\r\n", pCommand, (responsed?"error":"no"));

                return (-1);
            }

            DJY_EventDelay(1000);
            continue;
        }

        wait = LOCAL_RESPONSE_TIMEOUT;
        responsed++;
    }

    return (0);
}

// ============================================================================
//���ܣ�����͡�
//������pCommand -- ���
//     dwLen -- �����ַ����ȣ�
//���أ�ʵ�ʷ��͵������ַ�����
//��ע��
// ============================================================================
u32 __SMS_Command(s32 pHandle, char *pCommand, u32 dwLen)
{
    // return (Device_Write(pHandle, (u8*)pCommand, dwLen, 0, 2000000));
    return (write(pHandle, (u8*)pCommand, dwLen));
}

// ============================================================================
//���ܣ�SMS���ܳ�ʼ����
//������bArgC -- ����������
//     �豸������ѡ����
//���أ�0 -- �ɹ��� -1 -- ʧ�ܡ�
//��ע��
// ============================================================================
s32 SMS_Init(u8 bArgC, ...)
{
    va_list ap;
    u32 len, res;
    s32 ret;
    u8 i;
    s32 handle;
    char command[25] = {0};
    char *name = NULL;
    const char *defaultName = "uat";

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

    handle = open(name, O_RDWR, 0);
    if(handle == -1)
    {
        LOCAL_PRINT("SMS : error : \"%s\" does not exist. initialization failed.\r\n", name);
        return (-1);
    }

    // ����SMS����,����ģʽ�����Ŵ洢��ɾ�����ж���
    for(i = 0; i < 4; i++)
    {
        memset(command, 0x0, 25);
        strcat(command, SMS_CMDS[i]);
        strcat(command, SMS_INIT_ARGS[i]);
        strcat(command, SMS_END);
        len = strlen(command)+1;
        res = __SMS_Command(handle, command, len);
        if(res != len)
        {
            LOCAL_PRINT("SMS : error : \"%s%s\" cannot sent during initialization.\r\n", SMS_CMDS[i], SMS_INIT_ARGS[i]);
            ret = -1;
            goto INIT_EXIT;
        }

        if(__SMS_CheckResponse(handle,  SMS_CMDS[i], 0))
        {
            LOCAL_PRINT("SMS : error : initialization failed.\r\n");
            ret = -1;
            goto INIT_EXIT;
        }
    }

    if(strlen(name) > (sizeof(smsDev) - 1))
    {
        LOCAL_PRINT("SMS : error : device name is out of memory, initialization failed.\r\n");
        ret = -1;
    }
    else
    {
        strcpy(smsDev, name);
        ret = 0;
    }



INIT_EXIT:
    close(handle);
    return (ret);

}

// ============================================================================
//���ܣ���ѯ�Ƿ��ж��š�
//�������ޣ�
//���أ�-1 -- �޶��ţ����� -- ���źţ�
//��ע��
// ============================================================================
s32 SMS_Query(void)
{
    char *label[] = {"+CMTI:", "\"SM\"", ","};
    u32 res, wait = LOCAL_RESPONSE_TIMEOUT;
    s32 ret;
    u8 i = 0;
    char index[4];
    char *dev = smsDev;
    s32 handle;

    handle = open(dev, O_RDWR, 0);
    if(handle == -1)
    {
        LOCAL_PRINT("SMS : error : \"%s\" does not exist. Query quit.\r\n", dev);
        return (-1);
    }

    res = __SMS_CheckResponse(handle, label[0], 5);
    if(res)
    {
        ret = -1;
        goto QUERY_EXIT;
    }

    res = __SMS_CheckResponse(handle, label[1], 5);
    if(res)
    {
        ret = -1;
        goto QUERY_EXIT;
    }

    res = __SMS_CheckResponse(handle, label[2], 5);
    if(res)
    {
        ret = -1;
        goto QUERY_EXIT;
    }

    while(1)
    {
        res = Device_Read(handle, (u8*)&index[i], 1, 0, 0);
        if(!res)
        {
            if(i)
                break; // �ѻ�ȡ��

            if(!wait--) // û�л�ȡ��index
            {
                LOCAL_PRINT("SMS : error : unknown format for \"+CMTI:\"SM\",\".\r\n");
                ret = -1;
                goto QUERY_EXIT;
            }

            DJY_EventDelay(1000);
            continue;
        }

        wait = LOCAL_RESPONSE_TIMEOUT;
        if((index[i] >= '0') && (index[i] <= '9'))
        {
            if(i > 3) // index��Χֻ֧�ֵ�999
            {
                LOCAL_PRINT("SMS : error : unknown format for \"+CMTI:\"SM\",\".\r\n");
                ret = -1;
                goto QUERY_EXIT;
            }

            index[++i] = '\0';
        }
        else if(i)
        {
            break; // �ѻ�ȡ��index.
        }
    }

    ret = atoi(index);

QUERY_EXIT:
    close(handle);
    return (ret);
}

// ============================================================================
//���ܣ�������
//������bArgC -- ����������
//     buf -- �����壻 len -- �����峤�ȣ�index -- ������λ�ã�ȱʡΪ1����
//���أ�-1 -- ʧ�ܣ����� -- ���ض��ŵ�ʵ�ʳ��ȣ�������ڻ��峤�ȣ���
//��ע��
// ============================================================================
s32 SMS_Read(u8 bArgC, ...)
{
    va_list ap;
    u8 i;
    u32 len, cLen, res, responsed, index = 0;
    s32 ret;
    u8 *buf = NULL;
    char command[14] = {0};
    char arg[4] = "=0";
    char *dev = smsDev;
    s32 handle;

    va_start(ap, bArgC);
    for(i = 0; i < bArgC; i++)
    {
        switch(i)
        {
            case 0 : buf = va_arg(ap, u8*); break;
            case 1 : len = va_arg(ap, u32); break;
            case 2 : index = va_arg(ap, u32); break;
            default : break;
        }
    }
    va_end(ap);

    if(!buf)
    {
        LOCAL_PRINT("SMS : error : buffer is not set for read.\r\n");
        return (-1);
    }

    if(index > 999)
    {
        LOCAL_PRINT("SMS : error : index is out of range, should not exceed 999.\r\n");
        return (-1);
    }

    if(index)
        itoa(index, &arg[1], 10);

    handle = open(dev, O_RDWR, 0);
    if(handle == -1)
    {
        LOCAL_PRINT("SMS : error : \"%s\" does not exist. read quit.\r\n", dev);
        return (-1);
    }

    strcat(command, SMS_CMDS[4]);
    strcat(command, arg);
    strcat(command, SMS_END);
    cLen = strlen(command)+1;
    res = __SMS_Command(handle, command, cLen);
    if(res != cLen)
    {
        LOCAL_PRINT("SMS : error : \"%s%s\" cannot sent for read.\r\n",  SMS_CMDS[4], arg);
        ret = -1;
        goto READ_EXIT;
    }

    if(__SMS_CheckResponse(handle, SMS_CMDS[4], 1))
    {
        LOCAL_PRINT("SMS : error : \"%s%s\" has no response.\r\n",  SMS_CMDS[4], arg);
        ret = -1;
        goto READ_EXIT;
    }

    __SMS_CheckResponse(handle, arg, 1); // ���˵�arg

    responsed = 0;
    for(i = 0; i < 100; i++)
    {
        // res = Device_Read(handle, buf, len, 0, 100000);
        res = read(handle, buf, len);
        if(res)
        {
            buf += res;
            len -= res;
            responsed += res;
            continue;
        }

        if(responsed)
            break; // ��ȡ����
    }

    ret = responsed;

READ_EXIT:
    close(handle);
    return (ret);
}

// ============================================================================
//���ܣ����Ͷ��š�
//������bArgC -- ����������
//     to -- Ŀ���ֻ��ţ� message -- �������ݣ�len -- ���ų��ȣ�
//���أ�0 -- �ɹ���-1 -- ʧ�ܡ�
//��ע��
// ============================================================================
s32 SMS_Write(u8 bArgC, ...)
{
    va_list ap;
    char *to = NULL;
    char *message = NULL;
    u32 len, cLen, res;
    s32 ret;
    u8 i;
    char command[30] = "AT+CMGS=\"";
    char end[4] = "\"";
    char messageEnd = 0x1A;
    char *response[2] = {">", "+CMGS"};
    char *dev = smsDev;
    s32 handle;

    va_start(ap, bArgC);
    for(i = 0; i < bArgC; i++)
    {
        switch(i)
        {
        case 0 : to = va_arg(ap, char*); break;
        case 1 : message = va_arg(ap, char*); break;
        case 2 : len = va_arg(ap, u32); break;
        default : break;
        }
    }
    va_end(ap);

    if((!message) || (!to))
    {
        LOCAL_PRINT("SMS : error : bad parameters.\r\n");
        return (-1);
    }

    if(strlen(to) > 14)
    {
        LOCAL_PRINT("SMS : error : bad number format.\r\n");
        return (-1);
    }

    handle = open(dev, O_RDWR, 0);
    if(handle == -1)
    {
        LOCAL_PRINT("SMS : error : \"%s\" does not exist. Query quit.\r\n", dev);
        return (-1);
    }

    strcat(command, to);
    strcat(command, end);
    strcat(command, SMS_END);
    cLen = strlen(command)+1;
    res = __SMS_Command(handle, command, cLen);
    if(res != cLen)
    {
        command[cLen-2] = '\0';
        LOCAL_PRINT("SMS : error : \"%s\" cannot sent for write.\r\n", command);
        ret = -1;
        goto WRITE_EXIT;
    }

    if(__SMS_CheckResponse(handle, response[0], 1))
    {
        LOCAL_PRINT("SMS : error : \"%s\" has no response.\r\n",  command);
        ret = -1;
        goto WRITE_EXIT;
    }

    res = __SMS_Command(handle, message, len);
    if(res != len)
    {
        LOCAL_PRINT("SMS : error : message cannot sent for write.\r\n");
        ret = -1;
        goto WRITE_EXIT;
    }

    res = __SMS_Command(handle, &messageEnd, 1);
    if(res != 1)
    {
        LOCAL_PRINT("SMS : error : message end cannot sent for write.\r\n");
        ret = -1;
        goto WRITE_EXIT;
    }

    DJY_EventDelay(5000000);

    if(__SMS_CheckResponse(handle, response[1], 0))
    {
        LOCAL_PRINT("SMS : error : message write failed.\r\n");
        ret = -1;
        goto WRITE_EXIT;
    }

    ret = 0;

WRITE_EXIT:
    close(handle);
    return (ret);
}

// ============================================================================
//���ܣ�ɾ�����š�
//������dwIndex -- ���źš�
//���أ�0 -- �ɹ���-1 -- ʧ�ܡ�
//��ע��
// ============================================================================
s32 SMS_Del(u32 dwIndex)
{
    char command[16] = {0};
    char arg[5] = "=0,4";
    u32 len, res;
    s32 ret;
    char *dev = smsDev;
    s32 handle;

    if(-1 != dwIndex)
    {
        // ɾ����������
        if(dwIndex > 999)
        {
            LOCAL_PRINT("SMS : error : index out of range, should not exceed 999.\r\n");
            ret = -1;
            goto DEL_EXIT;
        }

        itoa(dwIndex, &arg[1], 10);
    }

    handle = open(dev, O_RDWR, 0);
    if(handle == -1)
    {
        LOCAL_PRINT("SMS : error : \"%s\" does not exist. Query quit.\r\n", dev);
        return (-1);
    }

    strcat(command, SMS_CMDS[2]);
    strcat(command, arg);
    strcat(command, SMS_END);
    len = strlen(command)+1;
    res = __SMS_Command(handle, command, len);
    if(res != len)
    {
        LOCAL_PRINT("SMS : error : \"%s%s\" cannot sent. Deletion failed.\r\n", SMS_CMDS[2], arg);
        return (-1);
    }

    res = __SMS_CheckResponse(handle, SMS_CMDS[2], 0);
    if(res)
    {
        LOCAL_PRINT("SMS : error : deletion failed.\r\n");
    }


    ret = 0;

DEL_EXIT:

    return (ret);
}
