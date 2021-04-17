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

#ifndef __TFTP_H
#define __TFTP_H

#include <sys/socket.h>

enum __EN_TFTP_OPCODE
{
    TFTP_RRQ = 1,
    TFTP_WRQ,
    TFTP_DATA,
    TFTP_ACK,
    TFTP_ERR,
    TFTP_OACK,
};


#define CN_TFTP_SERVERPORT_DEFAULT    69               //socket port
#define CN_TFTP_BLKSIZE_DEFAULT       512              //unit:byte
#define CN_TFTP_TIMEOUT_DEFAULT       2                //unit:second
#define CN_TFTP_TIMEOUTLIMIT_DEFAULT  3                //timesout limit
#define CN_STRING_LEN                 32
#define CN_PATH_LENLIMIT              128
#define CN_TFTPSERVER_BUFLEN          0x80             //unit:byte
#define CN_TFTP_RETRANSLIMIT          20               //unit:times

typedef struct TftpClient
{
    char   filename[CN_STRING_LEN];    //file name to download or upload
    char   mode[CN_STRING_LEN];        //used to decode the option value
    u16    reqmode;          //request mode
    int    timeout;          //the timeout time level
    int    otimeout;         //the timeout time for oack
    int    blksize;          //the blksize,(each block has this length data)
    int    oblksize;         //the blksize for oack
    int    tsize;            //the file size
    int    otsize;           //the file size for oack
    u32    timeoutLimit;     //the receive timeout times limit
    u32    timeoutTimes;     //the receive timeout times

    u32    block;            //u16 maybe got overflow easily, so we use u32
    u16    errcode;
    char   errmsg[CN_STRING_LEN];
    u32    rcvlen;          //the data length has received
    u32    sntlen;          //the data length has sent
    u32    retrans;         //the retrans times
    bool_t lastblock;       //the last block
    bool_t server;
    u32    sndspeed;        //use to record the send speed
    u32    rcvspeed;        //use to record the receive speed
    u32    timeused;        //use to record the timeuse;
    s64    timestart;       //use to record the start time
    s64    timestop;        //use to record the stop time

    int  (*readdata)(struct TftpClient *client,u8 *buf, int len);
    int  (*writedata)(struct TftpClient *client,u8 *buf, int len);

    u8   *buf;              //used as the client buf
    u32   buflen;
    int   fd;               //file handle
    int   sock;             //the socket handle
    struct sockaddr_in netaddr;
    int     addrlen ;
}tagTftpClient;



enum __EN_TFTPERR
{
    EN_TFTPOK =0,
    EN_TFTPERR_FILENOTFOUND ,
    EN_TFTPERR_FILECREATFAILED,
    EN_TFTPERR_FILESTATERR,
    EN_TFTPERR_INVALIDOPERATION,
    EN_TFTPERR_TIMEOUT,
    EN_TFTPERR_SHUTDOWN,
    EN_TFTPERR_LOCALMEM,
    EN_TFTPERR_SOCKET,
    EN_TFTPERR_LAST,
};
char *TFTP_ErrMsg(u16 errcode);
int TFTP_MakeRequestMsg(u8 *buf, int buflen,u16 opcode,char *filename,char *mode,\
                   int blksize,int timeout,int tsize);
int TFTP_DecodeRequestMsg(u8 *buf, int buflen, char **filename, char **mode,\
                     int *blksize,int *timeout, int *tsize);
int TFTP_MakeOptAckMsg(u8 *buf, int buflen,int blksize,int timeout,int tsize);
int TFTP_DecodeOptAckMsg(u8 *buf, int buflen,int *blksize,int *timeout, int *tsize);
int TFTP_MakeAckMsg(u8 *buf, int buflen,u16 block);
int TFTP_DecodeAckMsg(u8 *buf, int buflen,u16 *block);
int TFTP_MakeErrMsg(u8 *buf,int buflen,u16 errcode,char *msg);
int TFTP_DecodeErrMsg(u8 *buf, int buflen,tagTftpClient *client);
int TFTP_MakeDataMsg(tagTftpClient  *client);
int TFTP_DecodeDataMsg(u8 *buf,int buflen,u16 *block);
bool_t TFTP_ClientShow(tagTftpClient *client);
int TFTP_CreateClient(char *filename,char *mode,u16 reqmod,struct sockaddr_in *netaddr,\
                 int oblksize,int otimeout,int otsize,bool_t server,tagTftpClient **tftpclient);
bool_t TFTP_DeleteClient(tagTftpClient *client);
int TFTP_TransEngine(tagTftpClient *client);

bool_t TFTP_Path(char *param);
bool_t TFTP_PathSet(char *path);



#endif
