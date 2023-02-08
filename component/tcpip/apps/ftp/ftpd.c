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

#include <sys/time.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <misc/misc.h>

#include <dirent.h>
#include "component_config_ftp.h"
#include "dbug.h"
#include "ftp.h"
#include <shell.h>

#define CN_FTP_SPORT         21    //THIS IS THE SERVER BIND PORT
#define CN_FTP_ROOT          "/"   //USE THIS AS THE ROOT ABSOLUTE PATH
#define CN_FTP_LISTENMAX      1    //COULD LISTEN 1 CLIENT AT THE SAME TIME
#define CN_FTP_WELCOMMSG     "DJYOS FTP SERVER READY."
#define CN_FTPD_INDEX        "FTPD"

static bool_t  gFtpdDebugSwitch = false;
static tagFtpClient* pFtpClient = NULL;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

//static bool_t __FtpdDebug(char *para)
bool_t ftpd(char *para)
{

    int argc = 3;
    char *argv[3];
    if((NULL == para)||(0 == strcmp(para,"help")))
    {
        debug_printf("ftpd","usage:help\r\n"
                "     debug on/off\r\n"
                "     status\r\n");
    }
    else
    {
        string2arg(&argc,argv,para);
        if(0 == strcmp(argv[0],"status"))
        {
            static tagFtpClient* client = NULL;
            client = pFtpClient;
            while(NULL != client)
            {
                FTP_ShowClient(client);
                client = client->nxt;
            }
        }
        else if(0 == strcmp(argv[0],"debug"))
        {
            if(argc == 2)
            {
                if(0 == strcmp(argv[1],"on"))
                {
                    gFtpdDebugSwitch = true;
                }
                else
                {
                    gFtpdDebugSwitch = false;
                }
            }
            else
            {
                debug_printf("ftpd","parafmt:debug on/off\r\n");
            }

        }
        else
        {
            debug_printf("ftpd","%s:cmd:%s not incomplete yet\r\n",__FUNCTION__,argv[0]);
        }
    }

    return true;
}

//we use this function to get the file size
static int __FTP_DomainFileSize(char * file_name)
{
    struct stat s;
    int len = 0;
    if (stat(file_name, &s) == 0)
    {
        len = s.st_size;
    }
    else
    {
        len = -1;
    }
    return len;
}

//we use the curdir and the file name to get the absolute path
static int __FTP_DomainMkPath(tagFtpClient* client, char* path, char* new_path, size_t size)
{
    int curdirlen;
    if(path[0] == '/')
    {
        //its an absolute path
        snprintf((char *)new_path,size, "%s",path);
    }
    else
    {
        //its an relative path
        curdirlen = strlen(client->curdir);
        if(client->curdir[curdirlen-1] == '/')  //end with '/' then cat it to the tail
        {
            snprintf((char *)new_path,size, "%s%s", client->curdir, path);
        }
        else
        {
            snprintf((char *)new_path,size, "%s/%s", client->curdir, path);
        }
    }

    return 0;
}

//we use this function to send the message to the ftp client
//usually this is the response message
static bool_t  __FTP_DomainSndResponse(tagFtpClient *client,int responsecode,const char *msg)
{
    bool_t ret = false;
    memset(client->buf,0,client->buflen);
    client->datalen = snprintf((char *)client->buf,client->buflen,"%d  %s\r\n",responsecode,msg);
    if(client->datalen > 0)
    {
        if(gFtpdDebugSwitch)
        {
            time_t indextime;
            indextime = time(NULL);
            debug_printf("ftpd","%s[%s]S:%s\r\n",CN_FTPD_INDEX,ctime(&indextime),(char *)client->buf);
        }
        ret = sendexact(client->cchannel.s,client->buf,client->datalen);
    }
    return ret;
}

//use this function to match the command, the return value is 0 match others not match
static bool_t __FTP_DomainMatchCmd(char* src, char* match)
{
    bool_t ret = false;
    if(0 == strcmp(src,match))
    {
        ret = true;
    }
    return ret;
}

//we use this function to check if the user and password is legal login
static bool_t __FTP_DomainCheckUser(tagFtpClient* client)
{
    return true;
}


//close the data channel
static void __FTP_DomainCloseDataChannel(tagFtpClient *client)
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

//this function used to deal with the user name
static int __FTP_DomainCmdUserDeal(tagFtpClient* client,char *user)
{
    char *paras;
    strncpy(client->user,user,CN_FTP_NAMELEN-1);
    paras = "Need Passwd";
    __FTP_DomainSndResponse(client,CN_NEEDPASS,paras);
    return 0;
}
//this function used to deal with the user pass word
static int __CmdPassDeal(tagFtpClient* client,char *passwd)
{
    int result = 0;
    char *paras;
    strncpy(client->passwd,passwd,CN_FTP_NAMELEN-1);
    if(__FTP_DomainCheckUser(client))
    {
        paras = "LOGIN OK";
        __FTP_DomainSndResponse(client,CN_LOGIN,paras);
        result = 0;
    }
    else
    {
        paras = "LOGIN FAILED:user or passwd not matched";
        __FTP_DomainSndResponse(client,CN_LOGOUT,paras);
        result= -1;
    }
    return result;
}

//this function for the path build
static int __FTP_DomainCmdPwdDeal(tagFtpClient *client,char *para)
{
    char newpath[CN_PATH_LENGTH];

    client->datalen = snprintf(newpath,CN_PATH_LENGTH,\
            "\"%s\" is current directory.",client->curdir);
    __FTP_DomainSndResponse(client,CN_PATHBUILD,newpath);
    return 0;
}
//this function for the type message
static int __FTP_DomainCmdTypeDeal(tagFtpClient *client,char *para)
{
    if(strcmp(para, "I")==0)
    {
        __FTP_DomainSndResponse(client,CN_CMDSUCCESS,"TYPE BINARY SELECTED");
        client->dchannel.type='I';
    }
    else
    {
        __FTP_DomainSndResponse(client,CN_CMDSUCCESS,"TYPE A SELECTED");
        client->dchannel.type='A';
    }
    return 0;
}
//this function is for the port message
static int __FTP_DomainCmdPortDeal(tagFtpClient *client,char *para)
{
    int i;
    int portcom[6];
    char tmpip[INET_ADDRSTRLEN];
    i=0;
    portcom[i++]=atoi(strtok(para, ".,;()"));
    for(;i<6;i++)
        portcom[i]=atoi(strtok(0, ".,;()"));
    sprintf(tmpip, "%d.%d.%d.%d", portcom[0], portcom[1], portcom[2], portcom[3]);
    client->dchannel.ipaddr.s_addr = inet_addr(tmpip);
    client->dchannel.port = htons(portcom[4] * 256 + portcom[5]);
    client->dchannel.ispasv = false;
    __FTP_DomainSndResponse(client,CN_CMDSUCCESS,"port command success");
    return 0;
}

//this function for the type message
static int __FTP_DomainCmdPasvDeal(tagFtpClient *client,char *para)
{
    bool_t  ret = false;
    int     lsn_sock;
    int     len;
    struct  sockaddr_in sin;
    u16  port;
    u32  hostip;
    char    paras[64];

    lsn_sock = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
    if(lsn_sock <= 0)
    {
        error_printf("ftpd","socket err\r\n");
        return ret;
    }
    if(getsockname(lsn_sock, (struct sockaddr *)&sin, (socklen_t *)&len ) == -1 )
    {
        error_printf("ftpd","getsockname err\r\n");
        closesocket( lsn_sock );
        return ret;
    }
    if(bind(lsn_sock, (struct sockaddr *)&sin, sizeof(sin)) == -1 )
    {
        error_printf("ftpd","bind err\r\n");
        closesocket( lsn_sock );
        return ret;
    }

    if( listen(lsn_sock, 1) == -1 )
    {
        error_printf("ftpd","listen err\r\n");
        closesocket( lsn_sock );
        return ret;
    }

    u32 timeo = CN_FTPCLIENT_TIMEOUT;  //DO THE TIME OUT SET
    setsockopt(lsn_sock, SOL_SOCKET, SO_SNDTIMEO, &timeo, sizeof(timeo));
    setsockopt(lsn_sock, SOL_SOCKET, SO_RCVTIMEO, &timeo, sizeof(timeo));

    port = sin.sin_port; //save the port here
    len = sizeof( struct sockaddr );
    if (getsockname(client->cchannel.s, (struct sockaddr *)&sin, (socklen_t *)&len ) == -1 )
    {
        error_printf("ftpd","get host ip err\r\n");
        closesocket( lsn_sock );
        return ret;
    }
    hostip = sin.sin_addr.s_addr;
    snprintf(paras, 64,"Entering passive mode(%d,%d,%d,%d,%d,%d)",
            (hostip>>0)&0x000000FF,
            (hostip>>8)&0x000000FF,
            (hostip>>16)&0x000000FF,
            (hostip>>24)&0x000000FF,
            (port>>0)&0xff,
            (port>>8)&0xff);
    __FTP_DomainSndResponse(client,CN_ENTERPASSIVE,paras);
    client->dchannel.ispasv = true;
    client->dchannel.slisten = lsn_sock;
    client->dchannel.ipaddr.s_addr =hostip;
    client->dchannel.port = port;
    return ret;
}

//this function used to deal with list message
static int __FTP_DomainCmdListDeal(tagFtpClient* client,char *details)
{
    DIR* dirp;
    struct dirent* entry;
    struct stat s;
    int sock;
    if(client->dchannel.ispasv== false) //PORT MODE
    {
        sock = FTP_Connect(&client->dchannel.ipaddr,client->dchannel.port);
        if(sock == -1)
        {
            __FTP_DomainSndResponse(client,CN_OPENDATACONNECTIONFAILED,"open the data connection failed");
        }
        else
        {
            client->dchannel.sconnect = sock;
            __FTP_DomainSndResponse(client,CN_OPENCONNECTION,"open ASCII mode data connection for directory list");
        }
    }
    else
    {
        sock = FTP_Accept(client->dchannel.slisten,&client->dchannel.ipaddr,&client->dchannel.port);
        if(sock == -1)
        {
            __FTP_DomainSndResponse(client,CN_OPENDATACONNECTIONFAILED,"open the data connection failed");
        }
        else
        {
            __FTP_DomainSndResponse(client,CN_OPENCONNECTION,"open ASCII mode data connection for directory list");
            client->dchannel.saccept = sock;
        }
    }
    if(sock != -1)
    {
        dirp = opendir(client->curdir);
        if (dirp == NULL)
        {
            __FTP_DomainSndResponse(client,CN_LOCALERR,"open dir err");
        }
        else
        {
            while (1)
            {
                entry = readdir(dirp);
                if (entry == NULL)
                    break;
                if(details)
                {
                    __FTP_DomainMkPath(client,entry->d_name,(char *)client->buf,client->buflen);
                    if (stat((const char *)client->buf, &s) == 0)
                    {
                        if (s.st_mode & S_IFDIR)
                            client->datalen = snprintf((char *)client->buf,client->buflen,\
                                    "drw-r--r-- 1 admin admin %lld Jan 1 2000 %s\r\n", s.st_size, entry->d_name);
                        else
                            client->datalen = snprintf((char *)client->buf,client->buflen,\
                                    "-rw-r--r-- 1 admin admin %lld Jan 1 2000 %s\r\n",  s.st_size, entry->d_name);
                    }
                    else
                    {
                        continue;
                    }
                }
                else
                {
                    client->datalen = snprintf((char *)client->buf,client->buflen,\
                            "%s\r\n",entry->d_name);
                }
                if(false == sendexact(sock,client->buf,client->datalen))
                {
                    break;
                }
            }
            closedir(dirp);
        }
        __FTP_DomainSndResponse(client,CN_CLOSEDATACONNECTION,"CLOSE DATA CONNECTION");
    }
    __FTP_DomainCloseDataChannel(client);
    return 0;
}
//this function used for the retr message
static int __FTP_DomainCmdRetrDeal(tagFtpClient *client,char *para)
{
    char file_name[CN_PATH_LENGTH];
    int sock;
    int fd;

    s64  time1;
    s64  time2;
    s64  time3;
    u32  time_all = 0;
    u32  time_net = 0;
    u32  time_fs = 0;
    u32  file_len = 0;

    __FTP_DomainMkPath(client, para, file_name, CN_PATH_LENGTH);
    fd = open(file_name, O_RDONLY, 0);
    if (fd < 0)
    {
        __FTP_DomainSndResponse(client,CN_FILEINVALID,"file unavailable");
        return 0;
    }
    if(client->dchannel.ispasv== false) //PORT MODE
    {
        sock = FTP_Connect(&client->dchannel.ipaddr,client->dchannel.port);
        if(sock == -1)
        {
            __FTP_DomainSndResponse(client,CN_OPENDATACONNECTIONFAILED,"open the data connection failed");
        }
        else
        {
            client->dchannel.sconnect = sock;
            __FTP_DomainSndResponse(client,CN_OPENCONNECTION,"open ASCII mode data connection for directory list");
        }
    }
    else
    {
        sock = FTP_Accept(client->dchannel.slisten,&client->dchannel.ipaddr,&client->dchannel.port);
        if(sock == -1)
        {
            __FTP_DomainSndResponse(client,CN_OPENDATACONNECTIONFAILED,"open the data connection failed");
        }
        else
        {
            __FTP_DomainSndResponse(client,CN_OPENCONNECTION,"open ASCII mode data connection for directory list");
            client->dchannel.saccept = sock;
        }
    }

    if(sock > 0)
    {
        while(1)
        {
            time1 = DJY_GetSysTime();
            client->datalen = read(fd, client->buf, client->buflen);
            time2 = DJY_GetSysTime();

            if(client->datalen > 0)
            {
                if(false == sendexact(sock, client->buf, client->datalen))
                {
                    break; //maybe any send err happend
                }
                time3 = DJY_GetSysTime();
                time_fs += (u32)(time2-time1);
                time_net+=(u32)(time3-time2);
                file_len += client->datalen;
            }
            else
            {
                break;
            }
        }
        time_all = time_net + time_fs;
        __FTP_DomainSndResponse(client,CN_CLOSEDATACONNECTION,"close data connection");
        info_printf("ftpd","DOWNLOAD:%s:size:%d nettime:%d(ms) fstime:%d(ms) totaltime:%d(ms)\r\n",\
                file_name,file_len,time_net/1000,time_fs/1000,time_all/1000);
    }
    close(fd);
    __FTP_DomainCloseDataChannel(client);
    return 0;
}

//this function used for the stor message
static int __FTP_DomainCmdStorDeal(tagFtpClient *client,char *para)
{
    char file_name[CN_PATH_LENGTH];
    int sock;
    int fd;
    int wlen;

    s64  time1;
    s64  time2;
    s64  time3;
    u32  time_all = 0;
    u32  time_net = 0;
    u32  time_fs = 0;
    u32  file_len = 0;

    __FTP_DomainMkPath(client, para, file_name, CN_PATH_LENGTH);
    fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0);
    if (fd < 0)
    {
        __FTP_DomainSndResponse(client,CN_FILEINVALID,"file unavailable");
        return 0;
    }
    if(client->dchannel.ispasv== false) //PORT MODE
    {
        sock = FTP_Connect(&client->dchannel.ipaddr,client->dchannel.port);
        if(sock == -1)
        {
            __FTP_DomainSndResponse(client,CN_OPENDATACONNECTIONFAILED,"open the data connection failed");
        }
        else
        {
            client->dchannel.sconnect = sock;
            __FTP_DomainSndResponse(client,CN_OPENCONNECTION,"open ASCII mode data connection for directory list");
        }
    }
    else
    {
        sock = FTP_Accept(client->dchannel.slisten,&client->dchannel.ipaddr,&client->dchannel.port);
        if(sock == -1)
        {
            __FTP_DomainSndResponse(client,CN_OPENDATACONNECTIONFAILED,"open the data connection failed");
        }
        else
        {
            __FTP_DomainSndResponse(client,CN_OPENCONNECTION,"open ASCII mode data connection for directory list");
            client->dchannel.saccept = sock;
        }
    }

    if(sock > 0)
    {
        while(1)
        {
            time1 = DJY_GetSysTime();
            client->datalen=recv(sock, client->buf, client->buflen, 0);
            time2 = DJY_GetSysTime();
            if(client->datalen > 0)
            {
                wlen = write(fd, client->buf, client->datalen);
                if(wlen != client->datalen)
                {
                    debug_printf("ftpd","%s:write err:datalen:%d wlen:%d\r\n",__FUNCTION__,client->datalen,wlen);
                    break;
                }
            }
            else if(client->datalen < 0)
            {
                //may be timeout, just another time
            }
            else
            {
                //this is closed
                break;
            }
            time3 = DJY_GetSysTime();
            time_net += (u32)(time2-time1);
            time_fs +=(u32)(time3-time2);
            file_len += client->datalen;
        }
        time_all = time_net + time_fs;
        __FTP_DomainSndResponse(client,CN_CLOSEDATACONNECTION,"close data connection");
        info_printf("ftpd","UPLOAD:%s:size:%d nettime:%d(ms) fstime:%d(ms) totaltime:%d(ms)\r\n",\
                file_name,file_len,time_net/1000,time_fs/1000,time_all/1000);
    }
    close(fd);
    __FTP_DomainCloseDataChannel(client);
    return 0;
}


//use this function to send the file size
static int __FTP_DomainCmdSizeDeal(tagFtpClient* client,char *para)
{
    char file_name[CN_PATH_LENGTH];
    int file_size;
    char paras[32];

    __FTP_DomainMkPath(client, para, file_name, CN_PATH_LENGTH);
    file_size = __FTP_DomainFileSize((char *)file_name);
    if( file_size == -1)
    {
        snprintf(paras,32,"%s","file invalid");
    }
    else
    {
        snprintf(paras,32,"%d",file_size);
    }
    __FTP_DomainSndResponse(client,CN_FILENOTVALID,paras);
    return 0;
}

//use this function cwd
static int __FTP_DomainCmdCwdDeal(tagFtpClient* client,char *para)
{
    char file_name[CN_PATH_LENGTH];
    char paras[CN_PATH_LENGTH];
    __FTP_DomainMkPath(client, para, file_name, CN_PATH_LENGTH);
    strncpy(client->curdir,file_name,CN_PATH_LENGTH);
    snprintf(paras,CN_PATH_LENGTH,"Changed to directory \"%s\"",client->curdir);
    __FTP_DomainSndResponse(client,CN_FILECOMPLETE,paras);
    return 0;
}

//use this function cdup
static int __FTP_DomainCmdCdupDeal(tagFtpClient* client,char *para)
{
    int i = 0;
    char paras[CN_PATH_LENGTH];
    if(0 !=strcmp(client->curdir,CN_FTP_ROOT)) //BACK TO THE PARENT DIR
    {
        i = strlen(client->curdir);
        for(;i>0;i--)
        {
            if(client->curdir[i] == '/')
            {
                client->curdir[i] ='\0';
                break;
            }
            else
            {
                client->curdir[i] ='\0';
            }
        }
    }
    snprintf(paras,CN_PATH_LENGTH,"Changed to directory \"%s\"",client->curdir);
    __FTP_DomainSndResponse(client,CN_PATHBUILD,paras);
    return 0;
}
//use this function rest
static int __FTP_DomainCmdRestDeal(tagFtpClient* client,char *para)
{
    __FTP_DomainSndResponse(client,CN_CMDFAILED,"rest cmd execute failed");
    return 0;
}
#pragma GCC diagnostic pop

//use this function mkd
static int __FTP_DomainCmdMkdDeal(tagFtpClient* client,char *para)
{
    char file_name[CN_PATH_LENGTH];

    __FTP_DomainMkPath(client, para, file_name, CN_PATH_LENGTH);
    if(mkdir((char *)file_name, 0) == -1)
    {
        __FTP_DomainSndResponse(client,CN_FILEINVALID,"Mkd fail");

    }
    else
    {
        __FTP_DomainSndResponse(client,CN_PATHBUILD,"Mkd success");
    }

    return 0;
}
//use this function delete
static int __FTP_DomainCmdDeleDeal(tagFtpClient* client,char *para)
{
    char file_name[CN_PATH_LENGTH];

    __FTP_DomainMkPath(client, para, file_name, CN_PATH_LENGTH);
    if(unlink((char *)file_name) == -1)
    {
        __FTP_DomainSndResponse(client,CN_FILEINVALID,"Delete fail");
    }
    else
    {
        __FTP_DomainSndResponse(client,CN_FILECOMPLETE,"Delete success");
    }

    return 0;
}

//use this function rmd
static int __FTP_DomainCmdRmdDeal(tagFtpClient* client,char *para)
{
    char file_name[CN_PATH_LENGTH];

    __FTP_DomainMkPath(client, para, file_name, CN_PATH_LENGTH);
    if(unlink((char *)file_name) == -1)
    {
        __FTP_DomainSndResponse(client,550,"Rmd fail");
    }
    else
    {
        __FTP_DomainSndResponse(client,257,"Rmd success");
    }

    return 0;
}


static int __FTP_DomainMsgDeal(tagFtpClient* client, char *buf)
{
    int  result;
    char *parameter_ptr, *ptr;
    /* remove \r\n */
    ptr = buf;
    while (*ptr)
    {
        if (*ptr == '\r' || *ptr == '\n') *ptr = 0;
        ptr ++;
    }

    /* get request parameter */
    parameter_ptr = strchr(buf, ' ');
    if (parameter_ptr != NULL)
    {
        *parameter_ptr++='\0';
    }
    result = 0;
    // debug:
    if(__FTP_DomainMatchCmd(buf, "USER"))
    {
        // login correct
        result = __FTP_DomainCmdUserDeal(client,parameter_ptr);
    }
    else if(__FTP_DomainMatchCmd(buf, "PASS"))
    {
        result = __CmdPassDeal(client,parameter_ptr);
    }
    else if(__FTP_DomainMatchCmd(buf, "LIST"))
    {
        result = __FTP_DomainCmdListDeal(client,"detail");
    }
    else if(__FTP_DomainMatchCmd(buf, "NLST"))
    {
        result = __FTP_DomainCmdListDeal(client,"simple");
    }
    else if(__FTP_DomainMatchCmd(buf, "PWD")|| __FTP_DomainMatchCmd(buf, "XPWD"))
    {
        result = __FTP_DomainCmdPwdDeal(client,parameter_ptr);
    }
    else if(__FTP_DomainMatchCmd(buf, "TYPE"))
    {
        result = __FTP_DomainCmdTypeDeal(client,parameter_ptr);
    }
    else if(__FTP_DomainMatchCmd(buf, "PASV"))
    {
        result = __FTP_DomainCmdPasvDeal(client,parameter_ptr);
    }
    else if(__FTP_DomainMatchCmd(buf, "PORT"))
    {
        result = __FTP_DomainCmdPortDeal(client,parameter_ptr);
    }
    else if (__FTP_DomainMatchCmd(buf, "RETR"))
    {
        result = __FTP_DomainCmdRetrDeal(client,parameter_ptr);
    }
    else if (__FTP_DomainMatchCmd(buf, "STOR"))
    {
        result = __FTP_DomainCmdStorDeal(client,parameter_ptr);
    }
    else if(__FTP_DomainMatchCmd(buf, "SIZE"))
    {
        result = __FTP_DomainCmdSizeDeal(client,parameter_ptr);
    }
    else if(__FTP_DomainMatchCmd(buf, "SYST"))
    {
        __FTP_DomainSndResponse(client,CN_SYSTYPE,"DJYOS");
        result = 0;
    }
    else if(__FTP_DomainMatchCmd(buf, "CWD"))
    {
        result = __FTP_DomainCmdCwdDeal(client,parameter_ptr);
    }
    else if(__FTP_DomainMatchCmd(buf, "CDUP"))
    {
        result = __FTP_DomainCmdCdupDeal(client,parameter_ptr);
    }
    else if(__FTP_DomainMatchCmd(buf, "REST"))
    {
        result = __FTP_DomainCmdRestDeal(client,parameter_ptr);
    }
    else if(__FTP_DomainMatchCmd(buf, "MKD"))
    {
        result = __FTP_DomainCmdMkdDeal(client,parameter_ptr);
    }
    else if(__FTP_DomainMatchCmd(buf, "DELE"))
    {
        result = __FTP_DomainCmdDeleDeal(client,parameter_ptr);
    }
    else if(__FTP_DomainMatchCmd(buf, "RMD"))
    {
        result = __FTP_DomainCmdRmdDeal(client,parameter_ptr);
    }
    else if(__FTP_DomainMatchCmd(buf, "noop"))
    {
        __FTP_DomainSndResponse(client,CN_CMDSUCCESS,"noop command successful");
        result = 0;
    }
    else if(__FTP_DomainMatchCmd(buf, "QUIT"))
    {
        __FTP_DomainSndResponse(client,CN_LOGOUT,"QUIT CMD,bye");
        result = -1;
    }
    else
    {
        __FTP_DomainSndResponse(client,CN_CMDINVALID,"this cmd is not supported yet");
        result = 0;
    }
    return result;
}

//use this function to deal the connection
static bool_t __FTP_DomainClientEngine(tagFtpClient *client)
{
    time_t indextime;
    //ok, now use  the client processor to deal it
    //first we send the welcome message
    __FTP_DomainSndResponse(client,CN_SERVERREADY,CN_FTP_WELCOMMSG);
    //then do the while loop to do the interactive
    while(1)
    {
        //receive messages from the command sock
        client->datalen = FTP_RcvLine(client->cchannel.s,client->buf,client->buflen);
        if(client->datalen > 0)
        {
            if(gFtpdDebugSwitch)
            {
                indextime = time(NULL);
                info_printf("ftpd","%s[%s]R:%s\r\n",CN_FTPD_INDEX,ctime(&indextime),(char *)client->buf);
            }
            if(__FTP_DomainMsgDeal(client,(char *)client->buf)==-1)
            {
                break;
            }
        }
        else if(client->datalen < 0)
        {
            //may be time out,continue
        }
        else
        {
            break;
        }
    }
    //now its time to exit
    __FTP_DomainCloseDataChannel(client);
    closesocket(client->cchannel.s);
    return true;
}

//this is the ftp server main,always accept here
static ptu32_t __FTP_DomainServerMain()
{
    int sockserver;
    struct sockaddr_in local;
    int addr_len = sizeof(struct sockaddr);
    tagFtpClient  *client;
    time_t timeindex;


    local.sin_port=htons(CN_FTP_SPORT);
    local.sin_family=AF_INET;
    local.sin_addr.s_addr=INADDR_ANY;
    sockserver=socket(AF_INET, SOCK_STREAM, 0);
    if(sockserver < 0)
    {
        error_printf("ftpd","socket err\r\n");
        return 0;
    }
    if(0 != bind(sockserver, (struct sockaddr *)&local, addr_len))
    {
        error_printf("ftpd","bind err\r\n");
        goto SERVER_EXIT;
    }
    if(0 != listen(sockserver, CN_FTP_LISTENMAX))
    {
        error_printf("ftpd","listen err\r\n");
        goto SERVER_EXIT;
    }
    timeindex = time(NULL);
        info_printf("ftpd","FTPD:DEAMON AT:%s %d Time:%s\r\n",inet_ntoa(local.sin_addr),ntohs(local.sin_port),ctime(&timeindex));
    client = net_malloc(sizeof(tagFtpClient));
    if(NULL == client)
    {
        goto SERVER_EXIT;
    }
    //do some initialize
    memset(client,0,sizeof(tagFtpClient));
    client->nxt = NULL;
    pFtpClient = client;
    while(1) //always do the loop
    {
        client->cchannel.s = FTP_Accept(sockserver, &client->cchannel.ipaddr,
                                        &client->cchannel.port);
        if(client->cchannel.s >0)
        {
            //do the initialize
            strcpy(client->curdir,CN_FTP_ROOT);
            client->buflen = CN_FTPCLIENT_BUFLEN;
            client->datalen = 0;
            client->dchannel.saccept = -1;
            client->dchannel.sconnect = -1;
            client->dchannel.slisten = -1;
            //build a client a do the message deal
            timeindex = time(NULL);
            info_printf("ftpd","NewClient:IP:%s PORT:%d Time:%s\r\n",
                        inet_ntoa(client->cchannel.ipaddr),
                        ntohs(client->cchannel.port),ctime(&timeindex));
            __FTP_DomainClientEngine(client);
        }
    }
    timeindex = time(NULL);
            debug_printf("ftpd","%s:FTP SERVER QUIT:TIME:%s\r\n",__FUNCTION__,ctime(&timeindex));
    net_free(client);

SERVER_EXIT:
    closesocket(sockserver);
    return 0;
}

//THIS IS PING MODULE FUNCTION
bool_t ServiceFtpdInit(void)
{
    bool_t ret;
    ret = taskcreate("FTPD",0x1000,CN_PRIO_RRS,__FTP_DomainServerMain,NULL);
    if (ret == false) {
        error_printf("ftpd","FTPD:TASK CREATE ERR\r\n");
    }

    return ret;
}
ADD_TO_ROUTINE_SHELL(ftpd,ftpd,"usage:ftpd subcmd [subpara]");

