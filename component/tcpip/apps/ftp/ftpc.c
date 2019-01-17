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
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <os.h>
#include <shell.h>
#include "../../component_config_tcpip.h"

#include "ftp.h"
#include "dbug.h"
//------------------------------------------------------------------------------
//功能：字符串地址转整数IPV4地址，字符串可以是点分十进制的，也可以是主机名。
//参数：string，IPV4 地址，或者主机名
//     addr，返回结果，网络字节序
//返回：true=成功，false= 失败
//------------------------------------------------------------------------------
static bool_t  __String2Ip(const char *string,struct in_addr *addr)
{
    bool_t ret = false;
    struct hostent* hent;
    if(0 == inet_aton(string,addr))
    {
        hent = gethostbyname(string);
        if (hent)
        {
            memcpy(addr, hent->h_addr, hent->h_length);
            ret = true;
        }
    }
    else
    {
        ret = true;
    }
    return ret;
}

//initialize the client
static bool_t __InitClient(tagFtpClient *client,const char *host,const char *port,const char *user,const char *passwd)
{
    bool_t ret = false;
    memset(client,0,sizeof(tagFtpClient));
    //do the initialize
    client->buflen = CN_FTPCLIENT_BUFLEN;
    client->cchannel.s = -1;
    client->dchannel.saccept = -1;
    client->dchannel.sconnect = -1;
    client->dchannel.slisten = -1;
    client->dchannel.ispasv = true;
    //set the config
    strncpy(client->ipadr,host,CN_FTP_NAMELEN);
    if(__String2Ip(client->ipadr,&client->cchannel.ipaddr))
    {
        strncpy(client->port,port,CN_FTP_NAMELEN);
        client->cchannel.port = strtol(client->port,NULL,0);
        client->cchannel.port = htons(client->cchannel.port);
        strncpy(client->user,user,CN_FTP_NAMELEN);
        strncpy(client->passwd,passwd,CN_FTP_NAMELEN);
        ret = true;
    }
    return ret;
}



//get the response code (0 failed)
//the response format:code space string
static int __GetRespCode(const char *string)
{
    char c;
    int value;
    int ret = 0;
    int scale = 1;
    char *strspace;
    strspace = strchr(string,' ');
    if(NULL != strspace)
    {
        strspace--;
        while((unsigned int)strspace >= (unsigned int)string)
        {
            c = *strspace;
            strspace--;
            if(isdigit((int)c))
            {
                value = (int)(c-'0');
                value = value*scale;
                ret += value;
                scale =scale*10;
            }
            else
            {
                break;
            }
        }
    }
    return ret;
}
//return the response code and the message in the buf
static int __RcvResponse(int sock,u8 *buf,int len)
{
    int msglen;
    int code = 0;
    msglen = FtpRcvLine(sock,buf,len);
    if(msglen > 0)
    {
        code = __GetRespCode((const char *)buf);
        info_printf("ftpc","%s:rcvcode:%d",__FUNCTION__,code);
    }
    else
    {
        error_printf("ftpc","rcv err");
    }
    return code;
}
//send the command
static bool_t __SndCmd(int sock,const char *cmd,const char *para,u8 *buf,int len)
{
    bool_t ret = false;
    memset(buf,0,len);

    if(sock <=0)
    {
        return ret;
    }
    if(NULL != para)
    {
        snprintf((char *)buf,len,"%s %s\r\n",cmd,para);
    }
    else
    {
        snprintf((char *)buf,len,"%s \r\n",cmd);
    }
    len = strlen((char *)buf);
    ret = sendexact(sock,buf,len);
    if(ret == false)
    {
        error_printf("ftpc","Send Cmd:%s Para:%s FAILED",cmd,para);
    }
    else
    {
        info_printf("ftpc","%s:Send Cmd:%s Para:%s OK",__FUNCTION__,cmd,para);
    }
    return ret;
}


//flush the socket,before we send any command,we should flush all the content in the socket
//cache
static void __FlushRcvChannle(int s)
{
    u8 buf[1];
    u32 timeo = 0;  //DO THE TIME OUT SET
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeo, sizeof(timeo));
    int len;
    do{
        len = recv(s,buf,1,0);
    }while(len > 0);

    //recover the socket timeout
    timeo = CN_FTPCLIENT_TIMEOUT;  //DO THE TIME OUT SET
    setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, &timeo, sizeof(timeo));
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeo, sizeof(timeo));
    return;
}

//do the snd and recv the response,and the rcv buf is the respbuf
//return the recv code here
static int __SndAndRcvResp(int sock,const char *cmd,const char *para,u8 *buf,int len)
{
    int code = CN_SNDCMDERR;
    bool_t sndret;

    //flush the sock
    __FlushRcvChannle(sock);

    sndret = __SndCmd(sock,cmd,para,buf,len);
    if(sndret)
    {
        code = __RcvResponse(sock,buf,len);
    }
    return code;
}
//create the data socket
static bool_t  __PortCmd(tagFtpClient *client)
{
    bool_t  ret = false;
    int     lsn_sock;
    int     len;
    int     retcode;
    struct  sockaddr_in sin;
    u16  port;
    u32  hostip;
    char    para[64];

    struct hostent *h;
    gethostname(para,64);
    h = gethostbyname(para);
    if(NULL == h)
    {
        return ret;
    }
    memcpy(&hostip,h->h_addr,sizeof(hostip));

    lsn_sock = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
    if(lsn_sock <= 0)
    {
        error_printf("ftpc","socket err");
        return ret;
    }
    if(getsockname(lsn_sock, (struct sockaddr *)&sin, (socklen_t *)&len ) == -1 )
    {
        error_printf("ftpc","getsockname err");
        closesocket( lsn_sock );
        return ret;
    }
    sin.sin_addr.s_addr = hostip;
    port = sin.sin_port;
    if(bind(lsn_sock, (struct sockaddr *)&sin, sizeof(sin)) == -1 )
    {
        error_printf("ftpc","bind err");
        closesocket( lsn_sock );
        return ret;
    }

    if( listen(lsn_sock, 1) == -1 )
    {
        error_printf("ftpc","listen err");
        closesocket( lsn_sock );
        return ret;
    }

    //make the message of port
    snprintf(para, 64,"%d,%d,%d,%d,%d,%d",
            (hostip>>0)&0x000000FF,
            (hostip>>8)&0x000000FF,
            (hostip>>16)&0x000000FF,
            (hostip>>24)&0x000000FF,
            (port>>0)&0xff,
            (port>>8)&0xff);
    retcode = __SndAndRcvResp(client->cchannel.s,"PORT",para,client->buf,client->buflen);
    if(retcode != CN_CMDSUCCESS)
    {
        closesocket(lsn_sock);
        lsn_sock = -1;
    }
    else
    {
        client->dchannel.ipaddr.s_addr = hostip;
        client->dchannel.port = port;
        client->dchannel.slisten = lsn_sock;
        client->dchannel.ispasv = false;
        ret = true;
    }
    return ret;
}

//connect to the pasv socket
static bool_t __Pasv(tagFtpClient *client)
{
    bool_t ret = false;
    int     addr[6];
    struct in_addr  ipaddr;
    u16             port;
    char  ipaddrstr[INET_ADDRSTRLEN];
    int coderet;
    int s;
    char *start;

    coderet = __SndAndRcvResp(client->cchannel.s,"PASV",NULL,client->buf,client->buflen);
    if(coderet == CN_ENTERPASSIVE)
    {
        start = strchr((char*)client->buf,'(');
        if(NULL != start)
        {
            sscanf(start, "(%d,%d,%d,%d,%d,%d)",&addr[0],&addr[1],&addr[2],&addr[3],&addr[4],&addr[5]);
            snprintf(ipaddrstr,INET_ADDRSTRLEN,"%d.%d.%d.%d",addr[0],addr[1],addr[2],addr[3]);
            inet_aton(ipaddrstr,&ipaddr);
            port = htons((u16)(addr[4]*256+addr[5]));
            info_printf("ftpc","%s:IP:%s Port:%d",__FUNCTION__,ipaddrstr,ntohs(port));
            s = FtpConnect(&ipaddr,port);
            if(s > 0)
            {
                client->dchannel.ipaddr = ipaddr;
                client->dchannel.port = port;
                client->dchannel.ispasv = true;
                client->dchannel.sconnect = s;
                ret = true;
            }
            else
            {
                error_printf("ftpc","connect err");
            }
        }
        else
        {
            info_printf("ftpc","%s:no ip and port get",__FUNCTION__);
        }
    }
    return ret;
}
//do the connect to the ftp server
static bool_t __ConnectServer(tagFtpClient *client)
{
    bool_t    ret = false;
    int       respcode;
    int       s;

    if(client->cchannel.s != -1)
    {
        info_printf("ftpc","%s:sockfdexist:sockfd:%d",__FUNCTION__,client->cchannel.s);
        return ret;
    }
    s = FtpConnect(&client->cchannel.ipaddr,client->cchannel.port);
    if(s > 0)
    {
        respcode = __RcvResponse(s,client->buf,client->buflen);
        if(respcode == CN_SERVERREADY)
        {
            client->cchannel.s = s;
            info_printf("ftpc","%s:connect server ok",__FUNCTION__);
            ret = true;
        }
        else
        {
            info_printf("ftpc","%s:server not ready:code:%d",__FUNCTION__,respcode);
            closesocket(s);
        }
    }
    else
    {
        error_printf("ftpc","connect server err");
    }
    return ret;
}

static int __CreateDataChannel(tagFtpClient *client)
{
    int dsock = -1;

    if(client->dchannel.ispasv)
    {
        dsock =client->dchannel.sconnect;
    }
    else   //must wait for the client to connect
    {
        client->dchannel.saccept = FtpAccept(client->dchannel.slisten,NULL,0);
        dsock = client->dchannel.saccept;
    }
    if(dsock > 0)
    {
        //set the receive and send timeout limit
        u32 timeo = CN_FTPCLIENT_TIMEOUT;  //DO THE TIME OUT SET
        setsockopt(dsock, SOL_SOCKET, SO_SNDTIMEO, &timeo, sizeof(timeo));
        setsockopt(dsock, SOL_SOCKET, SO_RCVTIMEO, &timeo, sizeof(timeo));
    }
    return dsock;
}
//close the data channel
static void __CloseDataChannel(tagFtpClient *client)
{
    if(client->dchannel.saccept > 0)
    {
        closesocket(client->dchannel.saccept);
        client->dchannel.saccept = -1;
    }
    if(client->dchannel.slisten > 0)
    {
        closesocket(client->dchannel.slisten);
        client->dchannel.slisten = -1;
    }
    if(client->dchannel.sconnect > 0)
    {
        closesocket(client->dchannel.sconnect);
        client->dchannel.sconnect = -1;
    }
    return;
}


//__LogIn the server
static bool_t __LogIn(tagFtpClient *client)
{
    bool_t ret = false;
    int code;
    code = __SndAndRcvResp(client->cchannel.s,"USER",client->user,client->buf,client->buflen);
    if(code == CN_LOGIN)
    {
        ret = true;
    }
    else if(code == CN_NEEDPASS)//need the passwd
    {
        code = __SndAndRcvResp(client->cchannel.s,"PASS",client->passwd,client->buf,client->buflen);
        if(code == CN_LOGIN)
        {
            ret = true;
        }
        else
        {
            ret = false;
        }
    }
    else //err
    {
        ret = false;
    }
    return ret;
}

//expression type
static bool_t __Type(tagFtpClient *client,char mode)
{
    bool_t ret= false;
    int code;
    char para[2];
    para[0]=mode;
    para[1]='\0';
    code = __SndAndRcvResp(client->cchannel.s,"TYPE",para,client->buf,client->buflen);
    if(code == CN_CMDSUCCESS)
    {
        ret = true;
    }
    return ret;
}
static bool_t __Pwd(tagFtpClient *client)
{
    bool_t ret= false;
    int code;
    code = __SndAndRcvResp(client->cchannel.s,"PWD",NULL,client->buf,client->buflen);
    if(code == CN_PATHBUILD)
    {
        ret = true;
    }
    return ret;
}

//change the path
static bool_t __Cwd(tagFtpClient *client,const char *path)
{
    bool_t ret= false;
    int code;
    code = __SndAndRcvResp(client->cchannel.s,"CWD",path,client->buf,client->buflen);
    if(code == CN_FILECOMPLETE)
    {
        ret = true;
    }
    return ret;
}

//goto back directory
static bool_t __Cdup(tagFtpClient *client)
{
    bool_t ret= false;
    int code;
    code = __SndAndRcvResp(client->cchannel.s,"CDUP",NULL,client->buf,client->buflen);
    if(code == CN_FILECOMPLETE)
    {
        ret = true;
    }
    return ret;
}

//create a directory
static bool_t __Mkd(tagFtpClient *client,const char *path)
{
    bool_t ret= false;
    int code;
    code = __SndAndRcvResp(client->cchannel.s,"MKD",path,client->buf,client->buflen);
    if(code == CN_FILECOMPLETE)
    {
        ret = true;
    }
    return ret;
}

//list the directory
static bool_t __List(tagFtpClient *client,const char *path)
{
    int dsock = -1;
    bool_t ret = false;
    int len = 0;
    int timeout=0;

    ret = __Type(client,'A');
    if(ret == false)
    {
        return ret;
    }
    if(client->dchannel.ispasv)
    {
        ret = __Pasv(client);
    }
    else
    {
        ret = __PortCmd(client);
    }
    if(ret == false)
    {
        return ret;
    }
    int retcode;
    retcode = __SndAndRcvResp(client->cchannel.s,"LIST",path,client->buf,client->buflen);
    if((retcode == CN_OPENCONNECTION)||(retcode== CN_OPENDATACONNECTION125))
    {
        dsock = __CreateDataChannel(client);
        if(dsock > 0)
        {
            while(timeout < CN_FTPCLIENT_TRYTIMES)
            {
                memset(client->buf,0,client->buflen);
                len = recv(dsock,client->buf,client->buflen-1,0);
                if(len > 0)
                {
                    info_printf("ftpc","%s",client->buf);
                    timeout = 0;
                }
                else if(len == 0)
                {
                    break;
                }
                else
                {
                    timeout++;
                }
            }
            ret = true;
        }
        __CloseDataChannel(client);
    }
    retcode = __RcvResponse(client->cchannel.s,client->buf,client->buflen);
    if(retcode != CN_CLOSEDATACONNECTION)
    {
        debug_printf("ftpc","%s:DATA NOT COMPLETE\r\n",__FUNCTION__);
        ret = false;
    }
    return ret;
}

//download the file
static int __DownLoad( tagFtpClient *client,const char *s,const char *d)
{
    int dsock = -1;
    bool_t retcmd = false;
    int    retcode;
    int    retlen = 0;
    int    timeout=0;
    int    len_write = 0;
    int    len_rcv = 0;
    int    fd;

    //compute the time used
    u32    time_netused = 0; //the time used by the network
    u32    time_fsused = 0 ; //the time used by the file system
    s64    time1;
    s64    time2;
    s64    time3;
    //打开本地文件
    fd = open( d,  O_WRONLY|O_CREAT|O_TRUNC);
    if (fd == -1 )
    {
        debug_printf("ftpc","%s:open %s failed\r\n",__FUNCTION__,d);
        return retlen;
    }

    retcmd = __Type(client,'I');
    if(retcmd == false)
    {
        close(fd);
        return retlen;
    }
    if(client->dchannel.ispasv)
    {
        retcmd = __Pasv(client);
    }
    else
    {
        retcmd = __PortCmd(client);
    }
    if(retcmd == false)
    {
        close(fd);
        return retlen;
    }
    retcode = __SndAndRcvResp(client->cchannel.s,"RETR",s,client->buf,client->buflen);
    if((retcode == CN_OPENCONNECTION)||(retcode== CN_OPENDATACONNECTION125))
    {
        dsock = __CreateDataChannel(client);
        if(dsock > 0)
        {
            while(timeout < CN_FTPCLIENT_TRYTIMES)
            {
                memset(client->buf,0,client->buflen);

                time1= DjyGetSysTime();
                len_rcv = recv(dsock,client->buf,client->buflen,0);
                if(len_rcv > 0)
                {
                    timeout = 0;
                    time2= DjyGetSysTime();
                    len_write = write(fd,client->buf,len_rcv);
                    time3= DjyGetSysTime();
                    if(len_write != len_rcv)
                    {
                        debug_printf("ftpc","%s:write %s err:len_rcv:%d len_write:%d\r\n",\
                                __FUNCTION__,d,len_rcv,len_write);
                        break;
                    }
                    else
                    {
                        retlen += len_write;
                        //compute the timeused
                        time_netused += (u32)(time2-time1);
                        time_fsused += (u32)(time3-time2);
                    }
                }
                else if(len_rcv == 0)
                {
                    break;
                }
                else
                {
                    timeout++;
                }
            }
        }
        __CloseDataChannel(client);
        retcode = __RcvResponse(client->cchannel.s,client->buf,client->buflen);
    }
    else
    {
        debug_printf("ftpc","%s:RcvCodeErr %d\r\n",__FUNCTION__,retcode);
    }
    close(fd);
    debug_printf("ftpc","%s:sfile:%s dfile:%s  size:%d(Bytes)  nettime:%d(ms) fstime:%d(ms)\r\n",\
            __FUNCTION__,s,d,retlen,time_netused/1000,time_fsused/1000);

    return retlen;
}
//upload the file
static int __UpLoad( tagFtpClient *client,const char *s,const char *d)
{
    int dsock = -1;
    bool_t retcmd = false;
    int    retcode;
    int    retlen = 0;
    int    timeout=0;
    int    len_read = 0;
    int    fd;

    //compute the time used
    u32    time_netused = 0; //the time used by the network
    u32    time_fsused = 0 ; //the time used by the file system
    s64    time1;
    s64    time2;
    s64    time3;

    //打开本地文件
    fd = open(s, O_RDONLY);
    if (fd == -1 )
    {
        debug_printf("ftpc","%s:open %s failed\r\n",__FUNCTION__,s);
        return retlen;
    }

    retcmd = __Type(client,'I');
    if(retcmd == false)
    {
        close(fd);
        return retlen;
    }
    if(client->dchannel.ispasv)
    {
        retcmd = __Pasv(client);
    }
    else
    {
        retcmd = __PortCmd(client);
    }
    if(retcmd == false)
    {
        close(fd);
        return retlen;
    }
    retcode = __SndAndRcvResp(client->cchannel.s,"STOR",d,client->buf,client->buflen);
    if((retcode == CN_OPENCONNECTION)||(retcode== CN_OPENDATACONNECTION125))
    {
        dsock = __CreateDataChannel(client);
        if(dsock > 0)
        {
            while(timeout < CN_FTPCLIENT_TRYTIMES)
            {
                time1= DjyGetSysTime();
                len_read = read(fd,client->buf,client->buflen);
                if(len_read > 0)
                {
                    time2= DjyGetSysTime();
                    if(sendexact(dsock,client->buf,len_read))
                    {
                        time3= DjyGetSysTime();
                        retlen += len_read;
                        //compute the timeused
                        time_netused += (u32)(time3-time2);
                        time_fsused += (u32)(time2-time1);
                    }
                    else
                    {
                        debug_printf("ftpc","%s:snderr:len:%d \r\n",__FUNCTION__,len_read);
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
        }
        __CloseDataChannel(client);
        retcode = __RcvResponse(client->cchannel.s,client->buf,client->buflen);
    }
    else
    {
        info_printf("ftpc","%s:RcvCodeErr %d\r\n",__FUNCTION__,retcode);
    }
    close(fd);
    info_printf("ftpc","%s:sfile:%s dfile:%s  size:%d(Bytes)  nettime:%d(ms) fstime:%d(ms)\r\n",\
            __FUNCTION__,s,d,retlen,time_netused/1000,time_fsused/1000);
    return retlen;
}

//rename a file
static bool_t   __RenameFile(tagFtpClient *client,const char *s,const char *d)
{
    bool_t ret = false;
    int retcode;
    retcode = __SndAndRcvResp(client->cchannel.s,"RNFR",s,client->buf,client->buflen);
    if(retcode == CN_FILEPAUSE)
    {
        retcode = __SndAndRcvResp(client->cchannel.s,"RNTO",d,client->buf,client->buflen);
        if(retcode == CN_FILECOMPLETE)
        {
            ret = true;
        }
    }
    return ret;
}
//del a file
static bool_t  __DelFile(tagFtpClient *client,const char *s)
{
    bool_t ret = false;
    int retcode;
    retcode = __SndAndRcvResp(client->cchannel.s,"DELE",s,client->buf,client->buflen);
    if(retcode == CN_FILECOMPLETE)
    {
        ret = true;
    }
    return ret;
}

//del a folder
static bool_t  __DelDir(tagFtpClient *client,const char *s)
{
    bool_t ret = false;
    int retcode;
    retcode = __SndAndRcvResp(client->cchannel.s,"RMD",s,client->buf,client->buflen);
    if(retcode == CN_FILECOMPLETE)
    {
        ret = true;
    }
    return ret;
}

//logout
static bool_t  __LogOut(tagFtpClient *client)
{
    bool_t ret = false;
    int retcode;
    retcode = __SndAndRcvResp(client->cchannel.s,"QUIT",NULL,client->buf,client->buflen);
    if(retcode == CN_SERVERREADY)
    {
        ret = true;
    }
    return ret;
}

//disconnect the server
static void __DisConnectServer(tagFtpClient *client)
{
    __CloseDataChannel(client);
    if(client->cchannel.s > 0)
    {
        closesocket(client->cchannel.s);
        client->cchannel.s = -1;
    }
    return ;
}

//this data for the ftp client debug
static tagFtpClient  gFtpClientDebug;
//host port user passwd sfile destfile filesize times
//only used by the developer
static bool_t __FtpcTest(int argc,const char *argv[])
{
    int times_total = 0;
    int times_success = 0;
    int times_failed = 0;
    u32 time_used = 0;
    u32 time_max = 0;
    int file_size = 0;
    int file_rcv;
    s64 timestart;
    s64 timestop;
    const char *cur;

    bool_t download = true;
    times_total = strtol(argv[8],NULL,0);
    file_size = strtol(argv[7],NULL,0);
    if(0 == strcmp(argv[0],"u"))
    {
        download = false;
    }
    else
    {
        download = true;
    }

    int i = 0;
    for(i = 0;i < times_total;i++)
    {
        timestart = DjyGetSysTime();
        if(download)
        {
            file_rcv= ftpdownload(argv[1],argv[2],argv[3],argv[4],argv[5],argv[6]);
        }
        else
        {
            file_rcv= ftpupload(argv[1],argv[2],argv[3],argv[4],argv[5],argv[6]);
        }
        timestop = DjyGetSysTime();
        time_used = (u32)((timestop - timestart)/1000);
        if(time_used > time_max)
        {
            time_max = time_used;
        }
        if(file_rcv == file_size)
        {
            times_success++;
            cur ="OK";
        }
        else
        {
            times_failed++;
            cur ="ERR";
        }
        info_printf("ftpc","No%04d:%s TimeUed:%d(ms):TimeMax:%d(ms) Failed:%d Success:%d\r\n",\
                i,cur,time_used,time_max,times_failed,times_success);
        Djy_EventDelay(5*1000*mS);
    }
    info_printf("ftpc","TestDone\r\n");
    return true;
}
//static bool_t __FtpcDebug(char *param)
ADD_TO_IN_SHELL_HELP(ftpc,"usage:ftpc subcmd [subpara]");
ADD_TO_IN_SHELL  bool_t ftpc(char *param)
{
    int len ;
    int argc =10;
    const char *argv[10];
    if((NULL == param)||(0 == strcmp(param,"help"))) //printf the usage
    {
        info_printf("ftpc","usage:help\r\n"
                "     config host port user passwd\r\n"
                "     connect\r\n"
                "     login\r\n"
                "     pwd\r\n"
                "     cwd path\r\n"
                "     cdup\r\n"
                "     mkd dir\r\n"
                "     list path\r\n"
                "     download sfile dfile\r\n"
                "     upload   sfile dfile\r\n"
                "     rename   sfile dfile\r\n"
                "     delfile  file\r\n"
                "     deldir   dir\r\n"
                "     logout\r\n"
                "     disconnect\r\n"
                "     test d/u host port user passwd sfile destfile filesize times\r\n");
    }
    else
    {
        string2arg(&argc,argv,param);
        if(0 == strcmp(argv[0],"config"))
        {
            if(argc == 5) //5 paras include the subcmd
            {

                if(__InitClient(&gFtpClientDebug,argv[1],argv[2],argv[3],argv[4]))
                {
                    info_printf("ftpc","%s:config:success\r\n",__FUNCTION__);
                }
                else
                {
                    debug_printf("ftpc","%s:config:ip could not resolved\r\n",__FUNCTION__);
                }
            }
            else if(argc == 1) //use the default config
            {
                __InitClient(&gFtpClientDebug,"192.168.0.100","21","djyos","zqf");
            }
            else
            {
                debug_printf("ftpc","%s:parafmt:config host port user passwd\r\n",__FUNCTION__);
            }
        }
        else if(0 == strcmp(argv[0],"connect"))
        {
            if(__ConnectServer(&gFtpClientDebug))
            {
                info_printf("ftpc","%s:connect:OK\r\n",__FUNCTION__);
            }
            else
            {
                error_printf("ftpc","connect:ERR");
            }
        }
        else if(0 == strcmp(argv[0],"disconnect"))
        {
            __DisConnectServer(&gFtpClientDebug);
            info_printf("ftpc","%s:disconnect:OK",__FUNCTION__);
        }
        else if(0 == strcmp(argv[0],"login"))
        {
            if(__LogIn(&gFtpClientDebug))
            {
                info_printf("ftpc","%s:login:OK",__FUNCTION__);
            }
            else
            {
                error_printf("ftpc","login:ERR");
            }
        }
        else if(0 == strcmp(argv[0],"logout"))
        {
            if(__LogOut(&gFtpClientDebug))
            {
                info_printf("ftpc","%s:logout:OK",__FUNCTION__);
            }
            else
            {
                error_printf("ftpc","%s:logout:ERR");
            }
        }
        else if(0 == strcmp(argv[0],"pwd"))
        {
            if(__Pwd(&gFtpClientDebug))
            {
                info_printf("ftpc","%s:pwd:OK",__FUNCTION__);
            }
            else
            {
                error_printf("ftpc","pwd:ERR");
            }
        }
        else if(0 == strcmp(argv[0],"cdup"))
        {
            if(__Cdup(&gFtpClientDebug))
            {
                info_printf("ftpc","%s:cdup:OK",__FUNCTION__);
            }
            else
            {
                error_printf("ftpc","cdup:ERR");
            }
        }
        else if(0 == strcmp(argv[0],"cwd"))
        {
            if(argc == 2)
            {
                if(__Cwd(&gFtpClientDebug,argv[1]))
                {
                    info_printf("ftpc","%s:cwd:%s OK",__FUNCTION__,argv[1]);
                }
                else
                {
                    error_printf("ftpc","cwd:%s ERR",argv[1]);
                }
            }
            else
            {
                info_printf("ftpc","%s:parafmt:cwd path",__FUNCTION__);
            }
        }
        else if(0 == strcmp(argv[0],"mkd"))
        {
            if(argc == 2)
            {
                if(__Mkd(&gFtpClientDebug,argv[1]))
                {
                    info_printf("ftpc","%s:mkd:%s OK",__FUNCTION__,argv[1]);
                }
                else
                {
                    error_printf("ftpc","mkd:%s ERR",argv[1]);
                }
            }
            else
            {
                info_printf("ftpc","%s:parafmt:mkd path",__FUNCTION__);
            }
        }
        else if(0 == strcmp(argv[0],"list"))
        {
            if(argc == 2)
            {
                if(__List(&gFtpClientDebug,argv[1]))
                {
                    info_printf("ftpc","%s:list:%s OK",__FUNCTION__,argv[1]);
                }
                else
                {
                    error_printf("ftpc","list:%s ERR",argv[1]);
                }
            }
            else
            {
                info_printf("ftpc","%s:parafmt:list path\r\n",__FUNCTION__);
            }
        }
        else if(0 == strcmp(argv[0],"delfile"))
        {
            if(argc == 2)
            {
                if(__DelFile(&gFtpClientDebug,argv[1]))
                {
                    info_printf("ftpc","%s:delfile:%s OK",__FUNCTION__,argv[1]);
                }
                else
                {
                    error_printf("ftpc","delfile:%s ERR",argv[1]);
                }
            }
            else
            {
                info_printf("ftpc","%s:parafmt:delfile file\r\n",__FUNCTION__);
            }
        }
        else if(0 == strcmp(argv[0],"deldir"))
        {
            if(argc == 2)
            {
                if(__DelDir(&gFtpClientDebug,argv[1]))
                {
                    info_printf("ftpc","%s:deldir:%s OK",__FUNCTION__,argv[1]);
                }
                else
                {
                    error_printf("ftpc","deldir:%s ERR",argv[1]);
                }
            }
            else
            {
                info_printf("ftpc","%s:parafmt:deldir dir\r\n",__FUNCTION__);
            }
        }
        else if(0 == strcmp(argv[0],"download"))
        {
            if(argc == 3)
            {
                len = __DownLoad(&gFtpClientDebug,argv[1],argv[2]);
                if(len)
                {
                    info_printf("ftpc","%s:download:%s %s OK:len:%d",__FUNCTION__,argv[1],argv[2],len);
                }
                else
                {
                    error_printf("ftpc","download:%s %s ERR:len:%d",argv[1],argv[2],len);
                }
            }
            else
            {
                info_printf("ftpc","%s:parafmt:download sfile dstfile\r\n",__FUNCTION__);
            }
        }
        else if(0 == strcmp(argv[0],"upload"))
        {
            if(argc == 3)
            {
                len = __UpLoad(&gFtpClientDebug,argv[1],argv[2]);
                if(len)
                {
                    info_printf("ftpc","%s:upload:%s %s OK:len:%d",__FUNCTION__,argv[1],argv[2],len);
                }
                else
                {
                    error_printf("ftpc","upload:%s %s ERR:len:%d",argv[1],argv[2],len);
                }
            }
            else
            {
                info_printf("ftpc","%s:parafmt:upload sfile dstfile",__FUNCTION__);
            }
        }
        else if(0 == strcmp(argv[0],"rename"))
        {
            if(argc == 3)
            {
                len = __RenameFile(&gFtpClientDebug,argv[1],argv[2]);
                if(len)
                {
                    info_printf("ftpc","%s:rename:%s %s OK:len:%d\r\n",__FUNCTION__,argv[1],argv[2],len);
                }
                else
                {
                    error_printf("ftpc","rename:%s %s ERR:len:%d\r\n",argv[1],argv[2],len);
                }
            }
            else
            {
                info_printf("ftpc","%s:parafmt:rename sfile dstfile",__FUNCTION__);
            }
        }
        else if(0 == strcmp(argv[0],"status"))
        {
            FtpShowClient(&gFtpClientDebug);
        }
        else if(0 == strcmp(argv[0],"test"))
        {
            if(argc == 1)
            {
                info_printf("ftpc","use default:192.168.0.100 21 djyos zqf sourcefile destfile 0x1000 100");
                argv[1] = "d";
                argv[2] = "192.168.0.100";
                argv[3] ="21";
                argv[4] ="djyos";
                argv[5] ="zqf";
                argv[6] ="sourcefile";
                argv[7] ="destfile";
                argv[8] ="0x4000";
                argv[9] ="100";
            }
            else if(argc != 10)
            {
                info_printf("ftpc","%s:parafmt:test d/u host port user passwd sfile destfile filesize times\r\n",__FUNCTION__);
                return true;
            }
            __FtpcTest(argc-1,&argv[1]);
        }
        else
        {
            info_printf("ftpc","%s:cmd:%s not incomplete yet\r\n",__FUNCTION__,argv[0]);
        }
    }
    return true;
}

//download api
int ftpdownload(const char *host,const char *port,const char *user,const char *passwd,const char *sfile,const char *dfile)
{
    int ret = 0;
    tagFtpClient *client;
    client = net_malloc(sizeof(tagFtpClient));
    if(NULL == client)
    {
        info_printf("ftpc","%s:no mem",__FUNCTION__);
        goto EXIT_MEM;
    }
    if(false == __InitClient(client,host,port,user,passwd))
    {
        error_printf("ftpc","init err(HOST:%s COULD NOT RESOLVE)",host);
        goto EXIT_INIT;
    }
    if(false == __ConnectServer(client))
    {
        error_printf("ftpc","connect err");
        goto EXIT_CONNECT;
    }
    if(false == __LogIn(client))
    {
        error_printf("ftpc","login err");
        goto EXIT_LOGIN;
    }
    ret = __DownLoad(client,sfile,dfile);
    //now we begin to exit
    __LogOut(client);
EXIT_LOGIN:
    __DisConnectServer(client);
EXIT_CONNECT:
EXIT_INIT:
    net_free(client);
EXIT_MEM:
    return ret;
}
//upload api
int ftpupload(const char *host,const char *port,const char *user,const char *passwd,const char *sfile,const char *dfile)
{
    int ret = 0;
    tagFtpClient *client;
    client = net_malloc(sizeof(tagFtpClient));
    if(NULL == client)
    {
        error_printf("ftpc","no mem");
        goto EXIT_MEM;
    }
    if(false == __InitClient(client,host,port,user,passwd))
    {
        error_printf("ftpc","init err(HOST:%s COULD NOT RESOLVE)",host);
        goto EXIT_INIT;
    }
    if(false == __ConnectServer(client))
    {
        error_printf("ftpc","connect err");
        goto EXIT_CONNECT;
    }
    if(false == __LogIn(client))
    {
        error_printf("ftpc","login err");
        goto EXIT_LOGIN;
    }
    ret = __UpLoad(client,sfile,dfile);
    //now we begin to exit
    __LogOut(client);
EXIT_LOGIN:
    __DisConnectServer(client);
EXIT_CONNECT:
EXIT_INIT:
    net_free(client);
EXIT_MEM:
    return ret;
}



// =============================================================================
// 功能：add the ftpclient debug to the system
// 参数：para
// 返回值  ：true成功  false失败。
// =============================================================================
bool_t ServiceFtpcInit(ptu32_t para)
{
    return true;
}







