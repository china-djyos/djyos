//----------------------------------------------------
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
//author:zhangqf
//date  :����4:59:55/2017��1��4��
//usage :
//------------------------------------------------------

//standard includes
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <misc/misc.h>
#include <os.h>
//add your own specified header here
#include <sys/socket.h>
#include <device.h>
#include "iodev.h"
#include "dbug.h"
#include <shell.h>
typedef struct
{
    u8  idebug;
    u8  odebug;
}tagIoDebug;
static tagIoDebug  gIoDebug = {
        .idebug = 0,
        .odebug = 0,
};
//-----------------------------------------------------------------------------
//����:weuse,0xthis,0xfunction,0xto open the serial device
//����:
//����:NULL open failed else the device handle
//��ע:
//����:zhangqf@����4:55:51/2017��1��5��
//-----------------------------------------------------------------------------
s32 iodevopen(const char *name)
{
    s32 ret;

    ret = Device_Open(name,O_RDWR,0);
    if(-1==ret)
        return (0);

    return ret;
}
//#define CN_UART_RT_TIMEOUT (10*mS)
//-----------------------------------------------------------------------------
//����:we use this function to read data from the serial device
//����:dev,device handle;buf,data buf to read;len,data len
//����:-1 if the read abort others  the read len
//��ע:do the block read
//����:zhangqf@����4:57:17/2017��1��5��
//-----------------------------------------------------------------------------
s32 iodevread(s32 dev,u8 *buf,u32 buflen,u32 timeout)
{
    s32 len = -1;

    len = Device_Read(dev,buf,buflen,0,timeout);
    if((len > 0)&&(gIoDebug.idebug > 0))
    {
        time_t printtime;
        printtime = time(NULL);
        debug_printf("iodev","[RCV:%s:%d bytes]",ctime(&printtime),len);
        for(s32 i = 0;i < len;i++)
        {
            if(gIoDebug.idebug == 1)
            {
                debug_printf("iodev","%c",buf[i]);
            }
            else if(gIoDebug.idebug ==2)
            {
                debug_printf("iodev","%02x ",buf[i]);
            }
        }
        printf("\n\r");
    }

    return len;
}
//-----------------------------------------------------------------------------
//����:we use this function to write data to the serial device
//����:dev,device handle;buf,data buf to write;len,data len
//����:
//��ע:do the block send
//����:zhangqf@����4:57:17/2017��1��5��
//-----------------------------------------------------------------------------
s32 iodevwrite(s32 dev,u8 *buf,u32 len,u32 timeout)
{
    s32 ret = -1;
    u32 sentlen;
    sentlen  =0;
    u32 i;

    while(sentlen != len)
    {
        ret = Device_Write(dev,buf+sentlen,len-sentlen,0,timeout);
        if(ret > 0)
        {
            sentlen += ret;
        }
        else
        {
            break;
        }
    }
    ret = sentlen;

    if(gIoDebug.odebug > 0)
    {
        time_t printtime;
        printtime = time(NULL);
        debug_printf("iodev","[SND:%s:%d bytes]",ctime(&printtime),len);
        for(i = 0;i < len;i++)
        {
            if(gIoDebug.odebug == 1)
            {
                debug_printf("iodev","%c",buf[i]);
            }
            else if(gIoDebug.odebug ==2)
            {
                debug_printf("iodev","%02x ",buf[i]);
            }
        }
        printf("\n\r");
    }
    return ret;
}

//-----------------------------------------------------------------------------
//����:we use this function to flush the device,read all the exited data
//����:dev,device handle;buf,data buf to write;len,data len
//����:
//��ע:do the block send
//����:zhangqf@����4:57:17/2017��1��5��
//-----------------------------------------------------------------------------
void iodevflush(s32 dev)
{
    u8 buf[8];
    s32 len;
    while((len =iodevread(dev,buf,8,0))>0);
    return;
}


//-----------------------------------------------------------------------------
//����:use this function to close the sio device
//����:
//����:
//��ע:
//����:zhangqf@����5:05:27/2017��1��5��
//-----------------------------------------------------------------------------
bool_t iodevclose(s32 dev)
{
    bool_t ret = true;
    Device_Close(dev);
    return ret;
}
//usage:this function to set the sdev debug mode
//bool_t iodebugset(char *param)
bool_t iodebug(char *param)
{
    char *argv[2];
    s32   argc=2;
    s32   v1;

    string2arg(&argc,argv,param);
    if(argc == 2)
    {
        v1 = strtol(argv[0],NULL,0);
        if(v1 == 0)
        {
            gIoDebug.idebug = strtol(argv[1],NULL,0);
        }
        else
        {
            gIoDebug.odebug = strtol(argv[1],NULL,0);
        }
    }
    return true;
}

//-----------------------------------------------------------------------------
//����:this is the ppp main function here
//����:
//����:
//��ע:
//����:zhangqf@����4:06:57/2017��1��5��
//-----------------------------------------------------------------------------
bool_t PppIoInit(void)
{
        return (TRUE);
}
ADD_TO_ROUTINE_SHELL(iodebug,iodebug,"usage:iodebug type[0/rcv 1/snd] mode[0/nodebug 1/ascii 2/hex]");



