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
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
#include <sys/socket.h>
#include "tftplib.h"
#include "dbug.h"
#include <shell.h>
char  gTftpWorkSpace[CN_PATH_LENLIMIT];
static char *gTftpErrMsg[] =
{
    "TFTP OK",
    "FILE NOT FOUND",
    "FILE CREATE FAILED",
    "FILE STAT ERR",
    "INVALID OPERATION",
    "TIMEOUT",
    "REMOTE SHUTDOWN",
    "LOCAL MEM FAILED",
    "SOCKET ERR",
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

//get the err message
char *TFTP_ErrMsg(u16 errcode)
{
    if(errcode < EN_TFTPERR_LAST)
    {
        return gTftpErrMsg[errcode];
    }
    else
    {
        return NULL;
    }
}

//bool_t TftpSetWorkSpace(char *path)
//{
//    if(NULL != path)
//    {
//        strncpy(gTftpWorkSpace,path,CN_PATH_LENLIMIT);
//        gTftpWorkSpace[CN_PATH_LENLIMIT-1] = '\0';
//    }
//    return true;
//}

//bool_t TftpSetWorkSpace(char *path)
bool_t TFTP_PathSet(char *path)
{
    if(NULL != path)
    {
        strncpy(gTftpWorkSpace,path,CN_PATH_LENLIMIT);
        gTftpWorkSpace[CN_PATH_LENLIMIT-1] = '\0';
    }
    return true;
}

//bool_t TftpWorkSpaceShow(char *param)
bool_t TFTP_Path(char *param)
{
    info_printf("tftp","%s:%s\n\r",__FUNCTION__,gTftpWorkSpace);
    return true;
}

#pragma GCC diagnostic pop

//make the request message
int TFTP_MakeRequestMsg(u8 *buf, int buflen,u16 opcode,char *filename,char *mode,\
                   int blksize,int timeout,int tsize)
{
    int    len = 0;
    int    result = 0;
    char   optionbuf[20];
    char  *string;

    opcode = htons(opcode);
    len = sizeof(opcode);
    memcpy((void *)buf,(void *)&opcode,len);
    buf+=len;
    result += len;

    len = strlen(filename) +1;
    memcpy((void *)buf,(void *)filename,len);
    buf+=len;
    result += len;

    len = strlen(mode)+1;
    memcpy((void *)buf,(void *)mode,len);
    buf+=len;
    result += len;

    string = "blksize";
    len = strlen(string)+1;
    memcpy((void *)buf,(void *)string,len);
    buf+=len;
    result += len;

    sprintf(optionbuf,"%d",blksize);
    len = strlen(optionbuf)+1;
    memcpy((void *)buf,(void *)optionbuf,len);
    buf+=len;
    result += len;

    string = "timeout";
    len = strlen(string)+1;
    memcpy((void *)buf,(void *)string,len);
    buf+=len;
    result += len;

    sprintf(optionbuf,"%d",timeout);
    len = strlen(optionbuf)+1;
    memcpy((void *)buf,(void *)optionbuf,len);
    buf+=len;
    result += len;

    string = "tsize";
    len = strlen(string)+1;
    memcpy((void *)buf,(void *)string,len);
    buf+=len;
    result += len;

    sprintf(optionbuf,"%d",tsize);
    len = strlen(optionbuf)+1;
    memcpy((void *)buf,(void *)optionbuf,len);
    buf+=len;
    result += len;

    if(result > buflen)
    {
        info_printf("tftp","%s:buf overflow:buflen:%08d  result:%08d\n\r",\
                __FUNCTION__,buflen,result);
        result = buflen;  //don't send the exceeded msg
    }
    return result;
}

//decode the request message
int TFTP_DecodeRequestMsg(u8 *buf, int buflen, char **filename, char **mode,\
                     int *blksize,int *timeout, int *tsize)
{
    int i =0;
    int len;

    int otimeout = 0;
    int oblksize = 0;
    int otsize   = 0;

    i = 0;
    //file name
    *filename = (char *)buf;
    len = strlen((const char *)buf) +1;
    i += len;
    buf +=len;
    //mode
    *mode = (char *)buf;
    len = strlen((const char *)buf) +1;
    i += len;
    buf +=len;

    while(i < buflen)
    {
        if(0 == strcmp("timeout",(const char *)buf))
        {
            len = strlen((const char *)buf)+1;
            i += len;
            buf +=len;

            otimeout =atoi((const char *)buf);
            len = strlen((const char *)buf)+1;
            i += len;
            buf +=len;

            if(NULL != timeout)
            {
                *timeout = otimeout;
            }
        }
        else if(0 == strcmp("blksize",(const char *)buf))
        {
            len = strlen((const char *)buf)+1;
            i += len;
            buf +=len;

            oblksize =atoi((const char *)buf);
            len = strlen((const char *)buf)+1;
            i += len;
            buf +=len;

            if(NULL != blksize)
            {
                *blksize = oblksize;
            }
        }
        else if(0 == strcmp("tsize",(const char *)buf))
        {
            len = strlen((const char *)buf)+1;
            i += len;
            buf +=len;

            otsize =atoi((const char *)buf);
            len = strlen((const char *)buf)+1;
            i += len;
            buf +=len;

            if(NULL != tsize)
            {
                *tsize = otsize;
            }
        }
        else
        {
            len = strlen((const char *)buf)+1;
            i += len;
            buf +=len;
        }
    }

    return 0;
}

//make the optional message
int TFTP_MakeOptAckMsg(u8 *buf, int buflen,int blksize,int timeout,int tsize)
{
    u16    opcode;
    int    len = 0;
    int    result = 0;
    char   optionbuf[20];
    char  *string;

    opcode = TFTP_OACK;
    opcode = htons(opcode);
    len = sizeof(opcode);
    memcpy((void *)buf,(void *)&opcode,len);
    buf+=len;
    result += len;

    string = "blksize";
    len = strlen(string)+1;
    memcpy((void *)buf,(void *)string,len);
    buf+=len;
    result += len;

    sprintf(optionbuf,"%d",blksize);
    len = strlen(optionbuf)+1;
    memcpy((void *)buf,(void *)optionbuf,len);
    buf+=len;
    result += len;

    string = "timeout";
    len = strlen(string)+1;
    memcpy((void *)buf,(void *)string,len);
    buf+=len;
    result += len;

    sprintf(optionbuf,"%d",timeout);
    len = strlen(optionbuf)+1;
    memcpy((void *)buf,(void *)optionbuf,len);
    buf+=len;
    result += len;

    string = "tsize";
    len = strlen(string)+1;
    memcpy((void *)buf,(void *)string,len);
    buf+=len;
    result += len;

    sprintf(optionbuf,"%d",tsize);
    len = strlen(optionbuf)+1;
    memcpy((void *)buf,(void *)optionbuf,len);
    buf+=len;
    result += len;

    if(result > buflen)
    {
        info_printf("tftp","%s:buf overflow:buflen:%08d  result:%08d\n\r",\
                __FUNCTION__,buflen,result);
        result = buflen;  //don't send the exceeded msg
    }
    return result;
}

//decode the optional message
int TFTP_DecodeOptAckMsg(u8 *buf, int buflen,int *blksize,int *timeout, int *tsize)
{
    int i =0;
    int len;

    int otimeout=0;
    int oblksize=0;
    int otsize=0;

    i =0;
    while(i < buflen)
    {
        if(0 == strcmp("timeout",(const char *)buf))
        {
            len = strlen((const char *)buf)+1;
            i += len;
            buf +=len;

            otimeout =atoi((const char *)buf);
            len = strlen((const char *)buf)+1;
            i += len;
            buf +=len;

            if(NULL != timeout)
            {
                *timeout = otimeout;
            }
        }
        else if(0 == strcmp("blksize",(const char *)buf))
        {
            len = strlen((const char *)buf)+1;
            i += len;
            buf +=len;

            oblksize =atoi((const char *)buf);
            len = strlen((const char *)buf)+1;
            i += len;
            buf +=len;

            if(NULL != blksize)
            {
                *blksize = oblksize;
            }
        }
        else if(0 == strcmp("tsize",(const char *)buf))
        {
            len = strlen((const char *)buf)+1;
            i += len;
            buf +=len;

            otsize =atoi((const char *)buf);
            len = strlen((const char *)buf)+1;
            i += len;
            buf +=len;

            if(NULL != tsize)
            {
                *tsize = otsize;
            }
        }
        else
        {
            len = strlen((const char *)buf)+1;
            i += len;
            buf +=len;
        }
    }

    info_printf("tftp","OACK:blksize:%04d timeout:%04d tsize:%04d\n\r",oblksize,otimeout,otsize);


    return 0;
}

//make the ack message
int TFTP_MakeAckMsg(u8 *buf, int buflen,u16 block)
{
    int len = 0;
    int result = 0;
    u16 opcode;

    opcode = TFTP_ACK;
    opcode = htons(opcode);
    len = sizeof(opcode);
    memcpy((void *)buf,(void *)&opcode,len);
    buf+=len;
    result += len;

    block = htons(block);
    len = sizeof(block);
    memcpy((void *)buf,(void *)&block,len);
    buf+=len;
    result += len;

    if(result > buflen)
    {
        info_printf("tftp","%s:buf overflow:buflen:%08d  result:%08d\n\r",\
                __FUNCTION__,buflen,result);
        result = buflen;  //don't send the exceeded msg
    }
    return result;
}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

//decode the ack message
int TFTP_DecodeAckMsg(u8 *buf, int buflen,u16 *block)
{
    u16 blockmsg;
    int result;

    result = 0;
    memcpy((void *)&blockmsg,buf,sizeof(blockmsg));
    blockmsg = ntohs(blockmsg);
    if(NULL != block)
    {
        *block = blockmsg;
        result = sizeof(blockmsg);
    }

    return result;
}

//make the err message
int TFTP_MakeErrMsg(u8 *buf,int buflen,u16 errcode,char *msg)
{
    int len = 0;
    int result = 0;
    u16 opcode;

    opcode = TFTP_ERR;
    opcode = htons(opcode);
    len = sizeof(opcode);
    result += len;
    memcpy((void *)buf,(void *)&opcode,len);
    buf+=len;

    errcode = htons(errcode);
    len = sizeof(errcode);
    result += len;
    memcpy((void *)buf,(void *)&errcode,len);
    buf+=len;

    len = strlen(msg)+1;
    result += len;
    memcpy((void *)buf,(void *)msg,len);
    buf+=len;

    if(result > buflen)
    {
        info_printf("tftp","%s:buf overflow:buflen:%08d  result:%08d\n\r",\
                __FUNCTION__,buflen,result);
        result = buflen;  //don't send the exceeded msg
    }
    return result;
}

//decode the err message
int TFTP_DecodeErrMsg(u8 *buf, int buflen,tagTftpClient *client)
{
    u16 errcode;
    memcpy((void *)&errcode,(void *)buf,sizeof(errcode));
    errcode = ntohs(errcode);
    client->errcode = errcode;
    buf += sizeof(errcode);
    strncpy(client->errmsg,(const char *)buf,CN_STRING_LEN);
    client->errmsg[CN_STRING_LEN -1] = '\0';
    return 0;
}

//make the date message
int TFTP_MakeDataMsg(tagTftpClient  *client)
{
    u8 *buf;
    int len = 0;
    int result = 0;
    u16 opcode;
    u16 block;
    buf = client->buf;

    opcode = TFTP_DATA;
    opcode = htons(opcode);
    len = sizeof(opcode);
    memcpy((void *)buf,(void *)&opcode,len);
    buf+=len;
    result += len;

    block = client->block;
    block = htons(block);
    len = sizeof(block);
    memcpy((void *)buf,(void *)&block,len);
    buf+=len;
    result += len;

    if(NULL != client->readdata)
    {
        len = client->readdata(client,buf,client->oblksize);
        if(len != client->oblksize)
        {
            client->lastblock = true;
        }
        buf+=len;
        result += len;
    }

    return result;
}

//decode the data message
int  TFTP_DecodeDataMsg(u8 *buf,int buflen,u16 *block)
{
    u16 blocknum;
    int result;

    result =0;
    memcpy((void *)&blocknum,buf,sizeof(blocknum));
    blocknum = ntohs(blocknum);
    if(NULL != block)
    {
        *block = blocknum;
        result = sizeof(blocknum);
    }

    return result;
}
#pragma GCC diagnostic pop

//show the client stat
bool_t TFTP_ClientShow(tagTftpClient *client)
{
    info_printf("tftp","Result:\n\r");
    info_printf("tftp","FileName    :%s   MsgMode:%s\n\r",client->filename,client->mode);
    info_printf("tftp","Reqmode     :%08d (get=%d put=%d)\n\r",client->reqmode,TFTP_RRQ,TFTP_WRQ);
    info_printf("tftp","Timout      :%08d TimeoutNegotiation  :%08d (unit:second)\n\r",client->timeout,client->otimeout);
    info_printf("tftp","TimeoutTimes:%08d TimeoutTimesLimit   :%08d (unit:times)\n\r",client->timeoutTimes,client->timeoutLimit);
    info_printf("tftp","Blksize     :%08d BlksizeNegotiation  :%08d (unit:byte)\n\r",client->blksize,client->oblksize);
    info_printf("tftp","Tszie       :%08d TsizeNegotiation    :%08d (unit:byte)\n\r",client->tsize,client->otsize);
    info_printf("tftp","SndLen      :%08d Speed               :%08d (unit:KBytes/Second)\n\r",client->sntlen,client->sndspeed);
    info_printf("tftp","RcvLen      :%08d Speed               :%08d (unit:KBytes/Second)\n\r",client->rcvlen,client->rcvspeed);
    info_printf("tftp","ErrCode     :%08d ErrMsg              :%s\n\r",client->errcode,client->errmsg);
    info_printf("tftp","LastBlockNum:%08d %s\n\r",client->block,client->lastblock==true?"Yes":"No");
//    printf("\n\r");
    return true;
}

//write data to the file system
// =============================================================================
// FUNCTION:
// PARA  IN：
// PARA OUT：
// RETURN  :The request package len
// INSTRUCT:Rcv an file, the data will be dealt by the hook
// =============================================================================
static int __TFTP_WriteData(struct TftpClient *client,u8 *buf, int count)
{
    int result;
    result =write(client->fd,buf,count);

    return result;
}

//read data from the file system
static int __TFTP_ReadData(struct TftpClient *client,u8 *buf, int count)
{
    int ret = -1;
    u32 offset;
    u32 whence;
    u32 block;

    block = client->block;
    if(block > 0)
    {
        offset= client->oblksize*(block-1);
    }
    else
    {
        offset = 0;
    }
    whence = SEEK_SET;
    if(-1 == lseek(client->fd,offset,whence))
    {
        info_printf("tftp","%s:Seek offset:%08d  whence:%08d failed\n\r",__FUNCTION__,offset,whence);
        return ret;
    }
    ret = read(client->fd,buf,count);
    if(ret < 0)
    {
        ret = 0;
    }
    return ret;
}


//create the client, including all the resource needed by the client
int TFTP_CreateClient(char *filename,char *mode,u16 reqmod,struct sockaddr_in *netaddr,\
                 int oblksize,int otimeout,int otsize,bool_t server,tagTftpClient **tftpclient)
{
    int errcode;
    errcode = EN_TFTPOK;

    tagTftpClient      *client = NULL;
    int                 fileflag = 0;
    unsigned int        filemode = 0;
    struct stat         filestat;
    u32                 timeout;
    char                filepath[CN_PATH_LENLIMIT];

    client = (tagTftpClient *)net_malloc(sizeof(tagTftpClient));
    if(NULL == client)
    {
        error_printf("tftp","%s:net_malloc client failed\n\r",__FUNCTION__);
        errcode = EN_TFTPERR_LOCALMEM;
        goto EXIT_CLIENT;
    }
    //initialize the client struct
    memset((void *)client,0,sizeof(tagTftpClient));
    //create an udp socket
    client->sock = socket(AF_INET,SOCK_DGRAM,0);
    if(client->sock < 0)
    {
        error_printf("tftp","%s:client socket err\n\r",__FUNCTION__);
        errcode= EN_TFTPERR_SOCKET;
        goto EXIT_CLIENTSOCK;
    }

    timeout = CN_TFTP_TIMEOUT_DEFAULT*1000*1000; //(the client unit is second, and the block unit is micro second)
    if(0 != setsockopt(client->sock,SOL_SOCKET,SO_RCVTIMEO,\
                      (const void *)&timeout,sizeof(timeout)))
    {
        error_printf("tftp","%s:set receive timeout err\n\r",__FUNCTION__);
        errcode= EN_TFTPERR_SOCKET;
        goto EXIT_CLIENTSOCKOPT;
    }

    if(NULL != netaddr)
    {
        client->netaddr = *netaddr;
    }
    client->addrlen = sizeof(struct sockaddr_in);

    //cpy the filename  and mode
    strncpy(client->filename,filename,CN_STRING_LEN);
    client->filename[CN_STRING_LEN-1] = '\0';
    strncpy(client->mode,mode,CN_STRING_LEN);
    client->mode[CN_STRING_LEN-1] = '\0';

    client->blksize = oblksize;
    client->timeout = otimeout;
    client->tsize  = otsize;
    client->oblksize = oblksize;
    client->otimeout = otimeout;
    client->otsize   = otsize;

    client->timeoutLimit = CN_TFTP_TIMEOUTLIMIT_DEFAULT;
    client->buflen       = client->blksize+4;
    client->buf          = (u8 *)net_malloc(client->buflen);
    if(NULL == client->buf)
    {
        errcode = EN_TFTPERR_LOCALMEM;
        goto EXIT_BUF;
    }
    //for the file system
    client->reqmode = reqmod;
    client->server = server;
    strncpy(filepath,gTftpWorkSpace,CN_STRING_LEN);
    strcat(filepath,client->filename);

    if(((reqmod == TFTP_RRQ)&&(server ==true))||\
       ((reqmod == TFTP_WRQ)&&(server ==false)))
    {
        fileflag = O_RDWR;
        filemode = 0;

        client->fd = open(filepath, fileflag , filemode);
        if(client->fd < 0)
        {
            error_printf("tftp","%s:open the file:%s failed\n\r",__FUNCTION__,client->filename);
            errcode = EN_TFTPERR_FILENOTFOUND;
            goto EXIT_FILEOPEN;
        }
        if(0 != stat(filepath,&filestat))
        {
            error_printf("tftp","%s:stat the file:%s failed\n\r",__FUNCTION__,filename);
            errcode = EN_TFTPERR_FILESTATERR;
            goto EXIT_FILESTAT;
        }
        client->tsize = filestat.st_size;
    }
    else
    {
        fileflag = O_RDWR|O_CREAT;
        filemode = 0;

        client->fd = open(filepath, fileflag , filemode);
        if(client->fd < 0)
        {
            error_printf("tftp","%s:open the file:%s failed\n\r",__FUNCTION__,client->filename);
            errcode = EN_TFTPERR_FILECREATFAILED;
            goto EXIT_FILEOPEN;
        }
    }

    client->readdata=__TFTP_ReadData;
    client->writedata =__TFTP_WriteData;
    goto EXIT_CLIENT;              //create the client success

EXIT_FILESTAT:
    close(client->fd);
EXIT_FILEOPEN:
    net_free((void *)client->buf);
EXIT_BUF:
EXIT_CLIENTSOCKOPT:
    closesocket(client->sock);
EXIT_CLIENTSOCK:
     net_free(client);
     client = NULL;
EXIT_CLIENT:
    *tftpclient = client;
    return errcode;
}

//delete the client and net_free all the resource in the client
bool_t TFTP_DeleteClient(tagTftpClient *client)
{
    if(NULL != client)
    {
        net_free((void *)client->buf);     //net_free the client buf
        close(client->fd);             //close the file handle
        closesocket(client->sock);     //close the transaction socket
        net_free((void *)client);          //net_free the client memory
    }
    return true;
}

int TFTP_TransEngine(tagTftpClient *client)
{
    u16  block     = 0;
    u16  opcode    = 0;
    int  sndmsglen = 0;
    int  rcvmsglen = 0;
    int  len       = 0;
    bool_t  rcvloop = true;
    u8     *buf;
    u32     timeouttimes = 0;
    s64     tmp;

    client->timestart = DJY_GetSysTime();
    //OK, let us do the client engine now,
    //first send the optionnal
    if(client->server)
    {
        //as the server, we should send the optional ack first
        sndmsglen = TFTP_MakeOptAckMsg(client->buf,client->buflen,client->blksize,client->timeout,client->tsize);
    }
    else
    {
        //as the client, we should send the request first
        sndmsglen = TFTP_MakeRequestMsg(client->buf,client->buflen,client->reqmode,client->filename,client->mode,\
                                    client->blksize,client->timeout,client->tsize);
    }
    sendto(client->sock,client->buf,sndmsglen,0,(struct sockaddr *)&client->netaddr,client->addrlen);
    //loop until the transaction finished
    while(rcvloop)
    {
        rcvmsglen = recvfrom(client->sock,client->buf,client->buflen,0,\
                             (struct sockaddr *)&client->netaddr,&client->addrlen);
        if(rcvmsglen > 0)           //OK, the recved data
        {
            buf = client->buf;

            len = sizeof(opcode);
            memcpy((void *)&opcode,buf,len);
            buf += len;
            rcvmsglen -= len;
            opcode = ntohs(opcode);
            switch(opcode)
            {
                case TFTP_RRQ:
                    sndmsglen = TFTP_MakeErrMsg(client->buf,client->buflen,\
                            EN_TFTPERR_INVALIDOPERATION,TFTP_ErrMsg(EN_TFTPERR_INVALIDOPERATION));
                    sendto(client->sock,client->buf,sndmsglen,0,\
                           (struct sockaddr *)&client->netaddr,client->addrlen);
                    rcvloop = false;
                    error_printf("tftp","%s:err:got rrq from server\n\r",__FUNCTION__);
                    break;

                case TFTP_WRQ:
                    sndmsglen = TFTP_MakeErrMsg(client->buf,client->buflen,\
                            EN_TFTPERR_INVALIDOPERATION,TFTP_ErrMsg(EN_TFTPERR_INVALIDOPERATION));
                    sendto(client->sock,client->buf,sndmsglen,0,\
                           (struct sockaddr *)&client->netaddr,client->addrlen);
                    rcvloop = false;
                    error_printf("tftp","%s:err:got wrq from server\n\r",__FUNCTION__);
                    break;

                case TFTP_DATA:
                    if(((client->reqmode == TFTP_RRQ)&&(client->server == false))||\
                       ((client->reqmode == TFTP_WRQ)&&(client->server == true)))
                    {
                        len = TFTP_DecodeDataMsg(buf,rcvmsglen,&block);
                        buf += len;
                        rcvmsglen -= len;
                        if(block == (u16)(client->block +1))
                        {
                            client->block++;
                            if(NULL != client->writedata)
                            {
                                client->writedata(client,buf,rcvmsglen);
                            }
                            client->rcvlen += rcvmsglen;

                            if(rcvmsglen < client->oblksize)
                            {
                                client->lastblock = true;
                                rcvloop = false;
                            }
                            sndmsglen = TFTP_MakeAckMsg(client->buf,client->buflen,block);
                            sendto(client->sock,client->buf,sndmsglen,0,\
                                   (struct sockaddr *)&client->netaddr,client->addrlen);
                        }
                        else
                        {
                            block = (u16)client->block;
                            sndmsglen = TFTP_MakeAckMsg(client->buf,client->buflen,block);
                            sendto(client->sock,client->buf,sndmsglen,0,\
                                   (struct sockaddr *)&client->netaddr,client->addrlen);
                        }
                    }
                    else
                    {
                        //for we are the request to upload data,not receive data
                        sndmsglen = TFTP_MakeErrMsg(client->buf,client->buflen,\
                                EN_TFTPERR_INVALIDOPERATION,TFTP_ErrMsg(EN_TFTPERR_INVALIDOPERATION));
                        sendto(client->sock,client->buf,sndmsglen,0,\
                               (struct sockaddr *)&client->netaddr,client->addrlen);
                        rcvloop = false;
                        error_printf("tftp","%s:err:got data from server but we are wrq\n\r",__FUNCTION__);
                    }
                    break;
                case TFTP_ACK:
                    if(((client->reqmode == TFTP_WRQ)&&(client->server == false))||\
                       ((client->reqmode == TFTP_RRQ)&&(client->server == true)))
                    {
                        len = TFTP_DecodeAckMsg(buf,rcvmsglen,&block);
                        buf +=len;
                        rcvmsglen -=len;

                        if(block == (u16)client->block)
                        {
                            if(client->lastblock)
                            {
                                rcvloop = false;   //acked the last block ,so transmition finished
                            }
                            else
                            {
                                //send more data;
                                client->block++;
                                sndmsglen = TFTP_MakeDataMsg(client);
                                sendto(client->sock,client->buf,sndmsglen,0,\
                                       (struct sockaddr *)&client->netaddr,client->addrlen);
                                client->sntlen += (sndmsglen-4);
                                client->retrans=0;
                            }
                        }
                        else
                        {
                            //the remote did not receive the last frame. so retrans it
                            sndmsglen = TFTP_MakeDataMsg(client);
                            sendto(client->sock,client->buf,sndmsglen,0,\
                                   (struct sockaddr *)&client->netaddr,client->addrlen);
                            client->retrans++;
                        }
                    }
                    else
                    {
                        //for we are the request to upload data,not receive data
                        sndmsglen = TFTP_MakeErrMsg(client->buf,client->buflen,\
                                EN_TFTPERR_INVALIDOPERATION,TFTP_ErrMsg(EN_TFTPERR_INVALIDOPERATION));
                        sendto(client->sock,client->buf,sndmsglen,0,\
                               (struct sockaddr *)&client->netaddr,client->addrlen);
                        rcvloop = false;
                        error_printf("tftp","%s:err:got ack from server but we are rrq\n\r",__FUNCTION__);
                    }
                    break;

                case TFTP_ERR:
                    TFTP_DecodeErrMsg(buf,rcvmsglen,client);
                    rcvloop = false;
                    error_printf("tftp","%s:got errcode:%d:msg:%s\n\r",__FUNCTION__,client->errcode,client->errmsg);
                    break;
                case TFTP_OACK:  //the server may do this:specified the blksize
                    TFTP_DecodeOptAckMsg(buf,rcvmsglen,&client->oblksize,&client->otimeout,&client->otsize);
                    if(client->blksize<client->oblksize)
                    {
                        //if this happens, so exit
                        rcvloop = false;
                        error_printf("tftp","%s:oackfailed:blksize:%08d:oblksie:%08d\n\r",\
                                __FUNCTION__,client->blksize,client->oblksize);
                    }
                    if(((client->server==true)&&(client->reqmode == TFTP_WRQ))||\
                       ((client->server==false)&&(client->reqmode == TFTP_RRQ)))
                    {
                        block = (u16)client->block;
                        sndmsglen = TFTP_MakeAckMsg(client->buf,client->buflen,block);
                        sendto(client->sock,client->buf,sndmsglen,0,\
                               (struct sockaddr *)&client->netaddr,client->addrlen);
                    }
                    else if((client->server==false)&&(client->reqmode == TFTP_WRQ))
                    {
                        //send the data now
                        client->block =1;
                        sndmsglen = TFTP_MakeDataMsg(client);
                        sendto(client->sock,client->buf,sndmsglen,0,\
                               (struct sockaddr *)&client->netaddr,client->addrlen);
                        client->sntlen += (sndmsglen-4);
                    }
                    break;
                default:
                    sndmsglen = TFTP_MakeErrMsg(client->buf,client->buflen,\
                            EN_TFTPERR_INVALIDOPERATION,TFTP_ErrMsg(EN_TFTPERR_INVALIDOPERATION));
                    sendto(client->sock,client->buf,sndmsglen,0,\
                           (struct sockaddr *)&client->netaddr,client->addrlen);
                    rcvloop = false;
                    error_printf("tftp","%s:got err opcode:%d\n\r",__FUNCTION__,opcode);
                    break;
            }
            timeouttimes = 0;
        }
        else if(rcvmsglen == 0)  //the remote shutdown
        {
            error_printf("tftp","%s:the remote shutdown:will exit\n\r",__FUNCTION__);
            rcvloop = false;
        }
        else  //maybe an timeout
        {
            sendto(client->sock,client->buf,sndmsglen,0,\
                   (struct sockaddr *)&client->netaddr,client->addrlen);
            client->timeoutTimes++;
            timeouttimes++;
        }

        if(timeouttimes >= client->timeoutLimit)
        {
            error_printf("tftp","%s:TimeoutExcceed:%d/%d\n\r",__FUNCTION__,timeouttimes,client->timeoutLimit);
            rcvloop = false;
        }
        if(client->retrans >= CN_TFTP_RETRANSLIMIT)
        {
            error_printf("tftp","%s:RetransExcceed:%d/%d\n\r",__FUNCTION__,client->retrans,CN_TFTP_RETRANSLIMIT);
            rcvloop = false;
        }
    }
    client->timestop = DJY_GetSysTime();
    client->timeused = (u32)(client->timestop - client->timestart);
    if(((client->reqmode == TFTP_RRQ)&&(client->server==false))||\
       ((client->reqmode == TFTP_WRQ)&&(client->server==true))    )
    {
        tmp = client->rcvlen*1000;
        client->rcvspeed = (u32)(tmp/(client->timeused+1));
    }
    else
    {
        tmp = client->sntlen*1000;
        client->sndspeed = (u32)(tmp/(client->timeused+1));
    }
    TFTP_ClientShow(client);
    //now delete the client
    TFTP_DeleteClient(client);

    return 0;
}

ADD_TO_ROUTINE_SHELL(tftppath,TFTP_Path,"usage:tftppath");
ADD_TO_ROUTINE_SHELL(tftppathset,TFTP_PathSet,"usage:tftppathset workpath");

