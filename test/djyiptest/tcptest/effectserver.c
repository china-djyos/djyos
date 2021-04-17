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

// ģ������: TCP����
// ģ��汾: V1.00
// ������Ա: Administrator
// ����ʱ��: 9:23:45 AM/Jul 7, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <endian.h>

#include <os.h>
#include <sys/socket.h>

#define CN_TCP_SERVERPORT  2048

static u32 sgSndTimes = 0;

#define CN_SNDBUF_LEN 0x1000*20
#define CN_RCVBUF_LEN 0x1000*20   //80KB
ptu32_t tcptstserver(void)
{
    int sockfd;
    int clientfd;
    int multiid;
    int addrlen;
    int i =0;
    struct sockaddr_in serveraddr;
    struct sockaddr_in clientaddr;
    char *sndbuf;
    char *rcvbuf;
    int sndlen;
    s64 sndtotal;
    unsigned int sndprint =0;
    int sndopt;
    u32 sndtimestart;
    u32 sndtimetest;
    int sndspeed;
    int sndkbytes;


    int rcvlen;
    int rcvtotal;
    int rcvtimes;
    s64 rcvtimestart;
    s64 rcvtimeend;
    u32 nrcvtime;

    u32 activebits;
    struct MultiplexSetsCB *writesets;
    u32 badmultiwrite =0;

    struct MultiplexSetsCB *acceptsets;

    //�������ͼ���
    writesets = Multiplex_Create(1);
    if(NULL==writesets)
    {
        printf("Create WriteSets failed!\n\r");
    }
    else
    {
        printf("Create WriteSets success!\n\r");
    }

    //�������տͻ��˼���
    acceptsets = Multiplex_Create(1);
    if(NULL==acceptsets)
    {
        printf("Create acceptsets failed!\n\r");
    }
    else
    {
        printf("Create acceptsets success!\n\r");
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sockfd)
    {
        printf("socket failed!\n\r");
        return true;
    }
    else
    {
        printf("socket success!\n\r");
    }

    serveraddr.sin_port = htons(CN_TCP_SERVERPORT);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(-1==bind(sockfd, &serveraddr, sizeof(serveraddr)))
    {
        printf("bind failed!\n\r");
        return true;
    }
    else
    {
        printf("bind success!\n\r");
    }

    if(-1 == listen(sockfd, 100))
    {
        printf("listen failed!\n\r");
        return true;
    }
    else
    {
        printf("listen success!\n\r");
    }

    //����MULTIIO�����ϵļ��ӷ�������
    //����Ϊ������
//  sndopt = 1;
//  if(0 == setsockopt(sockfd,SOL_SOCKET ,SO_NONBLOCK,&sndopt,4))
//  {
//      printf("Server:Set server noblock success!\n\r");
//  }
//  else
//  {
//      printf("Sever:Set server noblock failed!\n\r");
//  }

    if(Multiplex_AddObject(acceptsets,sockfd,CN_SOCKET_IOACCEPT))
    {
        printf("add server to acceptsets success!\n\r");
    }
    else
    {
        printf("add server to acceptsets failed!\n\r");
    }
    while(1)
    {
        if(sockfd != Multiplex_Wait(acceptsets,NULL, CN_TIMEOUT_FOREVER))
        {
            printf("MultiIo activated err!\n\r");
        }
        else
        {
            printf("MultiIo activated success!\n\r");
        }

        clientfd = accept(sockfd, &clientaddr, &addrlen);
        if(clientfd != -1)
        {
            printf("Got an client:ip = %08x  port = %04x\n\r",\
                         ntohl(clientaddr.sin_addr.s_addr),ntohs(clientaddr.sin_port));
            //�ر�NAGLE
            sndopt = 1;
            if(0 == setsockopt(clientfd, IPPROTO_TCP,TCP_NODELAY,&sndopt,4))
            {
                printf("Client:close nagle success!\n\r");
            }
            else
            {
                printf("Client:close nagle  failed!\n\r");
            }

            //���ý��ջ�����
            sndopt = CN_RCVBUF_LEN;
            if(0 == setsockopt(clientfd,SOL_SOCKET ,SO_RCVBUF,&sndopt,4))
            {
                printf("Client:set client rcvbuf success!\n\r");
            }
            else
            {
                printf("Client:set client rcvbuf failed!\n\r");
            }
            //TEST RCV
//          rcvtotal = 0;
//          rcvbuf = M_Malloc(CN_RCVBUF_LEN,CN_TIMEOUT_FOREVER);
//          rcvtimestart = DJY_GetSysTime();
//          while(1)
//          {
//              rcvlen = recv(clientfd,rcvbuf,CN_RCVBUF_LEN,0);
//              if(rcvlen>0)
//              {
//                  rcvtotal+=rcvlen;
//                  rcvtimes++;
//              }
//              else
//              {
//                  printf("Rcv Failed!\n\r");
//              }
//              if(0==rcvtimes%1000)
//              {
//                  rcvtimeend = DJY_GetSysTime();
//                  nrcvtime = (u32)(rcvtimeend - rcvtimestart);
//                  printf("Rcv: Len =0x%08x MBytes,Time = 0x%08x us\n\r",\
//                                rcvtotal/1024/1024,nrcvtime);
//              }
//          }
            //TEST SND
            //���÷��ͻ�����
            sndopt = CN_SNDBUF_LEN;
            if(0 == setsockopt(clientfd,SOL_SOCKET ,SO_SNDBUF,&sndopt,4))
            {
                printf("Client:set client sndbuf success!\n\r");
            }
            else
            {
                printf("Client:set client sndbuf failed!\n\r");
            }
            sndbuf = M_Malloc(CN_SNDBUF_LEN,CN_TIMEOUT_FOREVER);
            for(i = 0; i <CN_SNDBUF_LEN; i++)
            {
                sndbuf[i] = 'a'+i%27;
            }
            while(1)
            {
                sndlen = send(clientfd,sndbuf,CN_SNDBUF_LEN,0);
                sgSndTimes++;

                DJY_EventDelay(1000*mS);
            }


            //���ý��ջ�����16KB
            sndopt = 0x4000;
            if(0 == setsockopt(clientfd,SOL_SOCKET ,SO_RCVBUF,&sndopt,4))
            {
                printf("Client:set client rcvbuf success!\n\r");
            }
            else
            {
                printf("Client:set client rcvbuf  failed!\n\r");
            }
            //���÷��͵�ˮλ,���͵�ˮƽΪ28K
            sndopt = 0x7000;
            if(0 == setsockopt(clientfd,SOL_SOCKET ,SO_SNDLOWAT,&sndopt,4))
            {
                printf("Client:set client sndbuf trig level success!\n\r");
            }
            else
            {
                printf("Client:set client sndbuf trig level  failed!\n\r");
            }
            //���ý���ˮλ��4KB
            sndopt = 0x1000;
            if(0 == setsockopt(clientfd,SOL_SOCKET ,SO_RCVLOWAT,&sndopt,4))
            {
                printf("Client:set client rcvbuf trig level success!\n\r");
                printf("Client:Begin Data Snd Test!\n\r");
            }
            else
            {
                printf("Client:set client rcvbuf trig level  failed!\n\r");
            }
            //����Ϊ������
            sndopt = 1;
            if(0 == setsockopt(clientfd,SOL_SOCKET ,SO_NONBLOCK,&sndopt,4))
            {
                printf("Client:Set noblock success!\n\r");
            }
            else
            {
                printf("Client:set noblock failed!\n\r");
            }
            //
            if(Multiplex_AddObject(writesets,clientfd,CN_SOCKET_IOWRITE))
            {
                printf("add client to writesets success!\n\r");
            }
            else
            {
                printf("add client to writesets failed!\n\r");
            }

            sndtotal = 0;
            sndtimestart = (u32)DJY_GetSysTime();
            while(1)
            {
                multiid = Multiplex_Wait(writesets,NULL, CN_TIMEOUT_FOREVER);
                if(clientfd == multiid)
                {
                    sndlen = send(clientfd,sndbuf,CN_SNDBUF_LEN,0);
                    sndprint++;
                    if(sndlen >0)
                    {
                        sndtotal += sndlen;
                        if(0 == sndprint%10000)
                        {
                            sndkbytes = sndtotal /1024;
                            sndtimetest = (u32)DJY_GetSysTime();
                            sndtimetest -= sndtimestart;
                            sndspeed = (sndtotal*1000)/sndtimetest;
                            printf("Send Msg:%d kbytes--speed = %d KB/S\n\r",\
                                           sndkbytes,sndspeed);
                        }
//                      DJY_EventDelay(1000*mS);
                    }
                    else
                    {
//                      printf("Client SndSet trigged but could not write!\n\r");
                    }
                }
                else
                {
                    badmultiwrite++;
                }
            }
        }
        else
        {
            printf("Bad Accept!\n\r");
        }
    }

    closesocket(sockfd);

    return 0;
}


bool_t TcpEffectTest(char *param)
{
   u16   evtt_id = CN_EVTT_ID_INVALID;
   evtt_id = DJY_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS-4, 0, 1,
        (ptu32_t (*)(void))tcptstserver,NULL, 0x1000, "TCPEffectServer");
    if (evtt_id != CN_EVTT_ID_INVALID)
    {
        DJY_EventPop(evtt_id, NULL, 0, NULL, 0, 0);
    }
    return true;
}


