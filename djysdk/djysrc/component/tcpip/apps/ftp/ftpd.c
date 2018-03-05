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

#include <netdb.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include "../../tcpipconfig.h"
#include "ftp.h"

#define CN_FTP_SPORT	     21    //THIS IS THE SERVER BIND PORT
#define CN_FTP_ROOT		     "/"   //USE THIS AS THE ROOT ABSOLUTE PATH
#define CN_FTP_LISTENMAX	  1    //COULD LISTEN 1 CLIENT AT THE SAME TIME
#define CN_FTP_WELCOMMSG     "DJYOS FTP SERVER READY."
#define CN_FTPD_INDEX        "FTPD"

static bool_t  gFtpdDebugSwitch = false;
static tagFtpClient* pFtpClient = NULL;

static bool_t __FtpdDebug(char *para)
{

	int argc = 3;
	const char *argv[3];
	if((NULL == para)||(0 == strcmp(para,"help")))
	{
		printf("usage:help\r\n"
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
		    	FtpShowClient(client);
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
				printf("%s:parafmt:debug on/off\r\n",__FUNCTION__);
			}

		}
		else
		{
			printf("%s:cmd:%s not incomplete yet\r\n",__FUNCTION__,argv[0]);
		}
	}

    return true;
}

//we use this function to get the file size
static int __FileSize(char * file_name)
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
static int __MkPath(tagFtpClient* client, char* path, char* new_path, size_t size)
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
static bool_t  __SndResponse(tagFtpClient *client,int responsecode,const char *msg)
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
			printf("%s[%s]S:%s",CN_FTPD_INDEX,ctime(&indextime),(char *)client->buf);
		}
		ret = sendexact(client->cchannel.s,client->buf,client->datalen);
	}
	return ret;
}

//use this function to match the command, the return value is 0 match others not match
static bool_t __MatchCmd(char* src, char* match)
{
	bool_t ret = false;
	if(0 == strcmp(src,match))
	{
		ret = true;
	}
	return ret;
}

//we use this function to check if the user and password is legal login
static bool_t __CheckUser(tagFtpClient* client)
{
	return true;
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

//this function used to deal with the user name
static int __CmdUserDeal(tagFtpClient* client,char *user)
{
	char *paras;
	strncpy(client->user,user,CN_FTP_NAMELEN-1);
	paras = "Need Passwd";
	__SndResponse(client,CN_NEEDPASS,paras);
	return 0;
}
//this function used to deal with the user pass word
static int __CmdPassDeal(tagFtpClient* client,char *passwd)
{
	int result = 0;
	char *paras;
	strncpy(client->passwd,passwd,CN_FTP_NAMELEN-1);
	if(__CheckUser(client))
	{
		paras = "LOGIN OK";
		__SndResponse(client,CN_LOGIN,paras);
		result = 0;
	}
	else
	{
		paras = "LOGIN FAILED:user or passwd not matched";
		__SndResponse(client,CN_LOGOUT,paras);
		result= -1;
	}
	return result;
}

//this function for the path build
static int __CmdPwdDeal(tagFtpClient *client,char *para)
{
	char newpath[CN_PATH_LENGTH];

	client->datalen = snprintf(newpath,CN_PATH_LENGTH,\
			"\"%s\" is current directory.",client->curdir);
	__SndResponse(client,CN_PATHBUILD,newpath);
	return 0;
}
//this function for the type message
static int __CmdTypeDeal(tagFtpClient *client,char *para)
{
	if(strcmp(para, "I")==0)
	{
		__SndResponse(client,CN_CMDSUCCESS,"TYPE BINARY SELECTED");
		client->dchannel.type='I';
	}
	else
	{
		__SndResponse(client,CN_CMDSUCCESS,"TYPE A SELECTED");
		client->dchannel.type='A';
	}
	return 0;
}
//this function is for the port message
static int __CmdPortDeal(tagFtpClient *client,char *para)
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
    __SndResponse(client,CN_CMDSUCCESS,"port command success");
    return 0;
}

//this function for the type message
static int __CmdPasvDeal(tagFtpClient *client,char *para)
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
    	printf("%s:socket err\r\n",__FUNCTION__);
    	return ret;
    }
    if(getsockname(lsn_sock, (struct sockaddr *)&sin, (socklen_t *)&len ) == -1 )
    {
    	printf("%s:getsockname err\r\n",__FUNCTION__);
    	closesocket( lsn_sock );
        return ret;
    }
    if(bind(lsn_sock, (struct sockaddr *)&sin, sizeof(sin)) == -1 )
    {
    	printf("%s:bind err\r\n",__FUNCTION__);
        closesocket( lsn_sock );
        return ret;
    }

    if( listen(lsn_sock, 1) == -1 )
    {
    	printf("%s:listen err\r\n",__FUNCTION__);
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
    	printf("%s:get host ip err\r\n",__FUNCTION__);
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
    __SndResponse(client,CN_ENTERPASSIVE,paras);
    client->dchannel.ispasv = true;
    client->dchannel.slisten = lsn_sock;
    client->dchannel.ipaddr.s_addr =hostip;
    client->dchannel.port = port;
    return ret;
}

//this function used to deal with list message
static int __CmdListDeal(tagFtpClient* client,char *details)
{
	DIR* dirp;
	struct dirent* entry;
	struct stat s;
	int sock;
	if(client->dchannel.ispasv== false) //PORT MODE
	{
		sock = FtpConnect(&client->dchannel.ipaddr,client->dchannel.port);
		if(sock == -1)
		{
			__SndResponse(client,CN_OPENDATACONNECTIONFAILED,"open the data connection failed");
		}
		else
		{
			client->dchannel.sconnect = sock;
			__SndResponse(client,CN_OPENCONNECTION,"open ASCII mode data connection for directory list");
		}
	}
	else
	{
		sock = FtpAccept(client->dchannel.slisten,&client->dchannel.ipaddr,&client->dchannel.port);
		if(sock == -1)
		{
			__SndResponse(client,CN_OPENDATACONNECTIONFAILED,"open the data connection failed");
		}
		else
		{
			__SndResponse(client,CN_OPENCONNECTION,"open ASCII mode data connection for directory list");
			client->dchannel.saccept = sock;
		}
	}
	if(sock != -1)
	{
		dirp = opendir(client->curdir);
		if (dirp == NULL)
		{
			__SndResponse(client,CN_LOCALERR,"open dir err");
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
					__MkPath(client,entry->d_name,(char *)client->buf,client->buflen);
					if (stat((const char *)client->buf, &s) == 0)
					{
						if (s.st_mode & S_IFDIR)
							client->datalen = snprintf((char *)client->buf,client->buflen,\
									"drw-r--r-- 1 admin admin %d Jan 1 2000 %s\r\n", 0, entry->d_name);
						else
							client->datalen = snprintf((char *)client->buf,client->buflen,\
									"-rw-r--r-- 1 admin admin %d Jan 1 2000 %s\r\n",  s.st_size, entry->d_name);
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
		__SndResponse(client,CN_CLOSEDATACONNECTION,"CLOSE DATA CONNECTION");
	}
	__CloseDataChannel(client);
	return 0;
}
//this function used for the retr message
static int __CmdRetrDeal(tagFtpClient *client,char *para)
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

	__MkPath(client, para, file_name, CN_PATH_LENGTH);
	fd = open(file_name, O_RDONLY, 0);
	if (fd < 0)
	{
		__SndResponse(client,CN_FILEINVALID,"file unavailable");
		return 0;
	}
	if(client->dchannel.ispasv== false) //PORT MODE
	{
		sock = FtpConnect(&client->dchannel.ipaddr,client->dchannel.port);
		if(sock == -1)
		{
			__SndResponse(client,CN_OPENDATACONNECTIONFAILED,"open the data connection failed");
		}
		else
		{
			client->dchannel.sconnect = sock;
			__SndResponse(client,CN_OPENCONNECTION,"open ASCII mode data connection for directory list");
		}
	}
	else
	{
		sock = FtpAccept(client->dchannel.slisten,&client->dchannel.ipaddr,&client->dchannel.port);
		if(sock == -1)
		{
			__SndResponse(client,CN_OPENDATACONNECTIONFAILED,"open the data connection failed");
		}
		else
		{
			__SndResponse(client,CN_OPENCONNECTION,"open ASCII mode data connection for directory list");
			client->dchannel.saccept = sock;
		}
	}

	if(sock > 0)
	{
		while(1)
		{
			time1 = DjyGetSysTime();
			client->datalen = read(fd, client->buf, client->buflen);
			time2 = DjyGetSysTime();

			if(client->datalen > 0)
			{
				if(false == sendexact(sock, client->buf, client->datalen))
				{
					break; //maybe any send err happend
				}
				time3 = DjyGetSysTime();
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
		__SndResponse(client,CN_CLOSEDATACONNECTION,"close data connection");
		printf("DOWNLOAD:%s:size:%d nettime:%d(ms) fstime:%d(ms) totaltime:%d(ms)\r\n",\
				file_name,file_len,time_net/1000,time_fs/1000,time_all/1000);
	}
	close(fd);
	__CloseDataChannel(client);
	return 0;
}

//this function used for the stor message
static int __CmdStorDeal(tagFtpClient *client,char *para)
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

	__MkPath(client, para, file_name, CN_PATH_LENGTH);
	fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0);
	if (fd < 0)
	{
		__SndResponse(client,CN_FILEINVALID,"file unavailable");
		return 0;
	}
	if(client->dchannel.ispasv== false) //PORT MODE
	{
		sock = FtpConnect(&client->dchannel.ipaddr,client->dchannel.port);
		if(sock == -1)
		{
			__SndResponse(client,CN_OPENDATACONNECTIONFAILED,"open the data connection failed");
		}
		else
		{
			client->dchannel.sconnect = sock;
			__SndResponse(client,CN_OPENCONNECTION,"open ASCII mode data connection for directory list");
		}
	}
	else
	{
		sock = FtpAccept(client->dchannel.slisten,&client->dchannel.ipaddr,&client->dchannel.port);
		if(sock == -1)
		{
			__SndResponse(client,CN_OPENDATACONNECTIONFAILED,"open the data connection failed");
		}
		else
		{
			__SndResponse(client,CN_OPENCONNECTION,"open ASCII mode data connection for directory list");
			client->dchannel.saccept = sock;
		}
	}

	if(sock > 0)
	{
		while(1)
		{
			time1 = DjyGetSysTime();
			client->datalen=recv(sock, client->buf, client->buflen, 0);
			time2 = DjyGetSysTime();
			if(client->datalen > 0)
			{
				wlen = write(fd, client->buf, client->datalen);
				if(wlen != client->datalen)
				{
					printf("%s:write err:datalen:%d wlen:%d\r\n",__FUNCTION__,client->datalen,wlen);
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
			time3 = DjyGetSysTime();
			time_net += (u32)(time2-time1);
			time_fs +=(u32)(time3-time2);
			file_len += client->datalen;
		}
		time_all = time_net + time_fs;
		__SndResponse(client,CN_CLOSEDATACONNECTION,"close data connection");
		printf("UPLOAD:%s:size:%d nettime:%d(ms) fstime:%d(ms) totaltime:%d(ms)\r\n",\
				file_name,file_len,time_net/1000,time_fs/1000,time_all/1000);
	}
	close(fd);
	__CloseDataChannel(client);
	return 0;
}


//use this function to send the file size
static int __CmdSizeDeal(tagFtpClient* client,char *para)
{
	char file_name[CN_PATH_LENGTH];
	int file_size;
	char paras[32];

	__MkPath(client, para, file_name, CN_PATH_LENGTH);
	file_size = __FileSize((char *)file_name);
	if( file_size == -1)
	{
		snprintf(paras,32,"%s","file invalid");
	}
	else
	{
		snprintf(paras,32,"%d",file_size);
	}
	__SndResponse(client,CN_FILENOTVALID,paras);
	return 0;
}

//use this function cwd
static int __CmdCwdDeal(tagFtpClient* client,char *para)
{
	char file_name[CN_PATH_LENGTH];
	char paras[CN_PATH_LENGTH];
    __MkPath(client, para, file_name, CN_PATH_LENGTH);
    strncpy(client->curdir,file_name,CN_PATH_LENGTH);
    snprintf(paras,CN_PATH_LENGTH,"Changed to directory \"%s\"",client->curdir);
	__SndResponse(client,CN_FILECOMPLETE,paras);
	return 0;
}

//use this function cdup
static int __CmdCdupDeal(tagFtpClient* client,char *para)
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
	__SndResponse(client,CN_PATHBUILD,paras);
	return 0;
}
//use this function rest
static int __CmdRestDeal(tagFtpClient* client,char *para)
{
    __SndResponse(client,CN_CMDFAILED,"rest cmd execute failed");
    return 0;
}

//use this function mkd
static int __CmdMkdDeal(tagFtpClient* client,char *para)
{
    char file_name[CN_PATH_LENGTH];

    __MkPath(client, para, file_name, CN_PATH_LENGTH);
    if(mkdir((char *)file_name, 0) == -1)
    {
        __SndResponse(client,CN_FILEINVALID,"Mkd fail");

    }
    else
    {
        __SndResponse(client,CN_PATHBUILD,"Mkd success");
    }

    return 0;
}
//use this function delete
static int __CmdDeleDeal(tagFtpClient* client,char *para)
{
    char file_name[CN_PATH_LENGTH];

    __MkPath(client, para, file_name, CN_PATH_LENGTH);
    if(unlink((char *)file_name) == -1)
    {
        __SndResponse(client,CN_FILEINVALID,"Delete fail");
    }
    else
    {
        __SndResponse(client,CN_FILECOMPLETE,"Delete success");
    }

    return 0;
}

//use this function rmd
static int __CmdRmdDeal(tagFtpClient* client,char *para)
{
    char file_name[CN_PATH_LENGTH];

    __MkPath(client, para, file_name, CN_PATH_LENGTH);
    if(unlink((char *)file_name) == -1)
    {
        __SndResponse(client,550,"Rmd fail");
    }
    else
    {
        __SndResponse(client,257,"Rmd success");
    }

    return 0;
}


static int __msgdeal(tagFtpClient* client, char *buf)
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
	if(__MatchCmd(buf, "USER"))
	{
		// login correct
		result = __CmdUserDeal(client,parameter_ptr);
	}
	else if(__MatchCmd(buf, "PASS"))
	{
		result = __CmdPassDeal(client,parameter_ptr);
	}
	else if(__MatchCmd(buf, "LIST"))
	{
		result = __CmdListDeal(client,"detail");
	}
	else if(__MatchCmd(buf, "NLST"))
	{
		result = __CmdListDeal(client,"simple");
	}
	else if(__MatchCmd(buf, "PWD")|| __MatchCmd(buf, "XPWD"))
	{
		result = __CmdPwdDeal(client,parameter_ptr);
	}
	else if(__MatchCmd(buf, "TYPE"))
	{
		result = __CmdTypeDeal(client,parameter_ptr);
	}
	else if(__MatchCmd(buf, "PASV"))
	{
		result = __CmdPasvDeal(client,parameter_ptr);
	}
	else if(__MatchCmd(buf, "PORT"))
	{
	    result = __CmdPortDeal(client,parameter_ptr);
	}
	else if (__MatchCmd(buf, "RETR"))
	{
		result = __CmdRetrDeal(client,parameter_ptr);
	}
	else if (__MatchCmd(buf, "STOR"))
	{
		result = __CmdStorDeal(client,parameter_ptr);
    }
	else if(__MatchCmd(buf, "SIZE"))
	{
		result = __CmdSizeDeal(client,parameter_ptr);
	}
	else if(__MatchCmd(buf, "SYST"))
	{
		__SndResponse(client,CN_SYSTYPE,"DJYOS");
		result = 0;
	}
	else if(__MatchCmd(buf, "CWD"))
	{
		result = __CmdCwdDeal(client,parameter_ptr);
	}
	else if(__MatchCmd(buf, "CDUP"))
	{
		result = __CmdCdupDeal(client,parameter_ptr);
	}
	else if(__MatchCmd(buf, "REST"))
	{
	    result = __CmdRestDeal(client,parameter_ptr);
	}
	else if(__MatchCmd(buf, "MKD"))
	{
	    result = __CmdMkdDeal(client,parameter_ptr);
	}
	else if(__MatchCmd(buf, "DELE"))
	{
	    result = __CmdDeleDeal(client,parameter_ptr);
	}
	else if(__MatchCmd(buf, "RMD"))
	{
	    result = __CmdRmdDeal(client,parameter_ptr);
	}

	else if(__MatchCmd(buf, "QUIT"))
	{
		__SndResponse(client,CN_LOGOUT,"QUIT CMD,bye");
		result = -1;
	}
	else
	{
		__SndResponse(client,CN_CMDINVALID,"this cmd is not supported yet");
		result = 0;
	}
	return result;
}

//use this function to deal the connection
static bool_t __ClientEngine(tagFtpClient *client)
{
	time_t indextime;
	//ok, now use  the client processor to deal it
	//first we send the welcome message
	__SndResponse(client,CN_SERVERREADY,CN_FTP_WELCOMMSG);
	//then do the while loop to do the interactive
	while(1)
	{
		//receive messages from the command sock
		client->datalen = FtpRcvLine(client->cchannel.s,client->buf,client->buflen);
		if(client->datalen > 0)
		{
			if(gFtpdDebugSwitch)
			{
				indextime = time(NULL);
				printf("%s[%s]R:%s",CN_FTPD_INDEX,ctime(&indextime),(char *)client->buf);
			}
			if(__msgdeal(client,(char *)client->buf)==-1)
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
	__CloseDataChannel(client);
	closesocket(client->cchannel.s);
	return true;
}

//this is the ftp server main,always accept here
static ptu32_t ServerMain()
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
		printf("%s:socket err\r\n",__FUNCTION__);
		return 0;
	}
	if(0 != bind(sockserver, (struct sockaddr *)&local, addr_len))
	{
		printf("%s:bind err\r\n",__FUNCTION__);
		goto SERVER_EXIT;
	}
	if(0 != listen(sockserver, CN_FTP_LISTENMAX))
	{
		printf("%s:listen err\r\n",__FUNCTION__);
		goto SERVER_EXIT;
	}
	timeindex = time(NULL);
	printf("FTPD:DEAMON AT:%s %d Time:%s\r\n",inet_ntoa(local.sin_addr),ntohs(local.sin_port),ctime(&timeindex));
	client = malloc(sizeof(tagFtpClient));
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
		client->cchannel.s = FtpAccept(sockserver, &client->cchannel.ipaddr, &client->cchannel.port);
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
			printf("NewClient:IP:%s PORT:%d Time:%s\r\n",\
					inet_ntoa(client->cchannel.ipaddr),ntohs(client->cchannel.port),ctime(&timeindex));
			__ClientEngine(client);
		}
	}
	timeindex = time(NULL);
	printf("%s:FTP SERVER QUIT:TIME:%s\r\n",__FUNCTION__,ctime(&timeindex));
	free(client);

SERVER_EXIT:
	closesocket(sockserver);
	return 0;
}

struct ShellCmdTab  gFtpdDebug[] =
{
    {
        "ftpd",
		__FtpdDebug,
        "usage:ftpd subcmd [subpara]",
        "usage:ftpd subcmd [subpara]",
    }
};
#define CN_FtpdDebug_NUM  ((sizeof(gFtpdDebug))/(sizeof(struct ShellCmdTab)))
static struct ShellCmdRsc gFtpdDebugCmdRsc[CN_FtpdDebug_NUM];

//THIS IS PING MODULE FUNCTION
bool_t ServiceFtpdInit(ptu32_t para)
{
    u16 eventID;
    u16 evttID;

    evttID = Djy_EvttRegist(EN_CORRELATIVE, gFtpAcceptPrior, 0, 1,
            ServerMain,NULL, gFtpAcceptStack, "FTPD");
    if(evttID == CN_EVTT_ID_INVALID)
    {
        printf("%s:EVTT ERR\r\n",__FUNCTION__);
        goto EXIT_TFTPSERVEREVTT;
    }
    eventID = Djy_EventPop(evttID, NULL, 0, 0, 0, 0);
    if(eventID == CN_EVENT_ID_INVALID)
    {
        printf("%s:EVENT ERR\r\n",__FUNCTION__);
        goto EXIT_TFTPSERVEREVENT;
    }
    Sh_InstallCmd(gFtpdDebug,gFtpdDebugCmdRsc,CN_FtpdDebug_NUM);
    return true;

EXIT_TFTPSERVEREVENT:
    Djy_EvttUnregist(evttID);
EXIT_TFTPSERVEREVTT:
    return false;
}



