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
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ��������ɣ���������ֲ����
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
// =============================================================================

// ģ������: TCP���������Ͳ���
// ģ��汾: V1.00
// ������Ա: Administrator
// ����ʱ��: 9:23:45 AM/Jul 7, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
#include "stdint.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "endian.h"

#include "os.h"

#include <tcpip/comport/sys/socket.h>

#define CN_TCP_BLOCKSERVERPORT  2048
#define CN_SNDBUF_LEN           0x1000    //4KB
#define CN_SNDBUF_STACKLEN      (CN_SNDBUF_LEN*64)
u8   sBlockSendBuf[CN_SNDBUF_LEN];

static s64   apptimestart;
static s64   apptimeend;
static u32   apptimeused;
static u32   appsndtimes = 0;
ptu32_t TcpServer_BlockSend(void)
{
    int server;
    int client;
    int addrlen;
    struct sockaddr_in addr;
    int sndlen;
    s64 sndtotal;
    int sndopt;
    //INITIAL LIZE THE DATA
    printk("\n\r%s:TASK BEGIN\n\r",__FUNCTION__);

    for(sndlen = 0; sndlen <CN_SNDBUF_LEN; sndlen ++ )
    {
        sBlockSendBuf[sndlen] = (u8)(sndlen%0x100);
    }

    server = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == server)
    {
        printk("%s:socket:err!\n\r",__FUNCTION__);
        return 0;
    }
    else
    {
        printk("%s:socket:ok!\n\r",__FUNCTION__);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(CN_TCP_BLOCKSERVERPORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(-1==bind(server, &addr, sizeof(addr)))
    {
        printk("%s:bind:err!\n\r",__FUNCTION__);
        return 0;
    }
    else
    {
        printk("%s:bind:ok!\n\r",__FUNCTION__);
    }

    if(-1 == listen(server, 5))
    {
        printk("%s:listen err!\n\r",__FUNCTION__);
        return 0;
    }
    else
    {
        printk("%s:listen ok!\n\r",__FUNCTION__);
    }

    while(1)
    {
        client = accept(server, &addr, &addrlen);
        if(client != -1)
        {
            printk("%s:ACCEPT OK: CLIEN :ip = %08x  port = %04x\n\r",__FUNCTION__,\
                    ntohl(addr.sin_addr.s_addr),ntohs(addr.sin_port));
            //���÷��ͻ�����
            sndopt = CN_SNDBUF_STACKLEN;
            if(0 == setsockopt(client,SOL_SOCKET ,SO_SNDBUF,&sndopt,sizeof(sndopt)))
            {
                printk("%s:setsockopt:sndbuf = %d  OK\n\r",__FUNCTION__,sndopt);

            }
            else
            {
                printk("%s:setsockopt:sndbuf = %d  failed\n\r",__FUNCTION__,sndopt);
                closesocket(client);
                continue;
            }

            while(1)
            {
//              apptimestart = DJY_GetSysTime();
                sndlen = send(client,sBlockSendBuf,CN_SNDBUF_LEN,0);
                if(sndlen > 0)
                {
                    sndtotal += sndlen;
                }
                else
                {
                    printk("%s:send: len = %d  failed\n\r",__FUNCTION__,CN_SNDBUF_LEN);
//                  closesocket(client);
                    break;
                }
//              apptimeend = DJY_GetSysTime();
//              apptimeused = (u32)(apptimeend - apptimestart);
//              printk("%s:apptime:No.%d used = %d us\n\r",__FUNCTION__,\
//                      appsndtimes++,apptimeused);
//              DJY_EventDelay(1000*mS);
            }
        }
        else
        {
            printk("%s:accept:bad!\n\r",__FUNCTION__);
        }
    }
    closesocket(server);
    printk("%s:TASK END\n\r",__FUNCTION__);

    return 0;
}


bool_t TcpServer_BlockSendTest(char *param)
{
    u16  evttID;
    u16  eventID;
    evttID = DJY_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 1,\
        (ptu32_t (*)(void))TcpServer_BlockSend,NULL, 0x1000, "TcpServer_BlockSend");
    if (evttID != CN_EVTT_ID_INVALID)
    {
        eventID = DJY_EventPop(evttID, NULL, 0, 0, 0, 0);
        if(eventID == CN_EVENT_ID_INVALID)
        {
            DJY_EvttUnregist(evttID);
            printk("%s:BlockSendTask  Create Failed\n\r",__FUNCTION__);
        }
        else
        {
            printk("%s:BlockSendTask  Create SUCCESS\n\r",__FUNCTION__);
        }
    }
    return true;
}




