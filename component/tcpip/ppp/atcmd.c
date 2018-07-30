//----------------------------------------------------
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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
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
//author:zhangqf
//date  :下午2:52:04/2017年3月22日
//usage :use this file to complete the at command
//-----------------------------------------------------------------------------
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <os.h>
#include <osarch.h>
//add your own specified header here
#include <shell.h>
#include "dbug.h"
#include "newshell.h"
#include "iodev.h"

static char pAtDevName[32]; //use this var to storage the at command device
//-----------------------------------------------------------------------------
//功能:use this function to set the default at command device name
//参数:
//返回:
//备注:
//作者:zhangqf@上午10:17:31/2017年3月23日
//-----------------------------------------------------------------------------
//bool_t  AtDefaultDevSet(char *name)
ADD_TO_SHELL_HELP(atdevset,"usage:atdevset devname");
ADD_TO_IN_SHELL bool_t  atdevset(char *name)
{
	if(NULL != name)
	{
		memset(pAtDevName,0,32);
		strncpy(pAtDevName,name,31);
	}
	return true;
}

const char *AtDevGetDefault(void)
{
	return pAtDevName;
}

//bool_t AtDefaultDevGetShell(char *para)
ADD_TO_SHELL_HELP(atdevget,"usage:atdevget");
ADD_TO_IN_SHELL bool_t atdevget(char *para)
{
    debug_printf("ATCOMMANDDEV","%s\n\r",AtDevGetDefault());
    return true;
}

//we use this for the at command
//usage:we use this function to deal the at result as the args format
static int __getpara(char *text,char *seperate,char *argv[],int argc)
{
	int result;
	char *s;
	int len,i;
	s = seperate;
	len = strlen(text);
	while(*s != '\0') //make all the charactor in text matching the seperate to 0
	{
		for(i =0;i<len;i++)
		{
			if(text[i]==*s)
			{
				text[i]='\0';
			}
		}
		s++;
	}
	//ok now check the para start
	result = 0;
	s = text;
	while(result <argc)
	{
		//jump the NULL
		while(*s == '\0')
		{
			s++;
		}
		//the s is the start
		if(s < (text + len))
		{
			argv[result] = s;
			result++;
			s = s+strlen(s);
		}
		else
		{
			break;
		}
	}
	return result;
}
//-----------------------------------------------------------------------------
//功能:use this function to do the at command and analyze
//参数:devname, the at device name,cmd:at command buf/buflen:storage the reply result(argc and argv in args mode)
//返回:
//备注:
//作者:zhangqf@上午10:51:51/2017年3月23日
//-----------------------------------------------------------------------------
#define CN_AT_LEN  128
//usage:send the at command to the serial device
int AtCmd(const char *devname,char *cmd,u8 *buf,int buflen,int argc,char *argv[])
{
	char   cmdbuf[CN_AT_LEN];
	int    result = -1;
	int    len = 0;
	int    lenleft;
	int    offset;
	ptu32_t dev;
	//open the at command device
	dev = iodevopen(devname);
	if(0 == dev)
	{
	    debug_printf("atcmd","%s:open %s dev failed \n\r",__FUNCTION__,devname);
	    goto EXIT_OPENFAILED;
	}
	//flush the device
	iodevflush(dev);
	//initialize the buf with the specified at command
	memset(cmdbuf,0,CN_AT_LEN);
	snprintf(cmdbuf,CN_AT_LEN,"%s\r",cmd);
	//write the command to the device
	len = strlen(cmdbuf);
	result = iodevwrite(dev,(u8 *)cmdbuf,(u32)len,CN_IODEV_WTIMEOUT);
	if(result != len)
	{
	    debug_printf("atcmd","%s:only write %d/%d to %s \n\r",__FUNCTION__,result,len,devname);
	    goto EXIT_WRITEFAILED;
	}

	Djy_EventDelay(1000*mS);//just wait for the echo back
	//if need the result,then we will wait for the timeout
	if((NULL != buf)&&(buflen>0))
	{
		//initialize the buf
		memset(buf,0,buflen);
		offset = 0;
		lenleft = buflen;
		while(1)
		{
			len = iodevread(dev,(u8 *)&buf[offset],lenleft, CN_IODEV_WTIMEOUT);
			if(len > 0)
			{
				offset += len;
				lenleft -= len;
				if(offset == buflen)
				{
					break;
				}
				else if(offset > buflen)
				{
				    debug_printf("atcmd","%s:read error--driver error\n\r",__FUNCTION__);
				}
				else
				{
					//do nothing
				}
				Djy_EventDelay(10*mS);//just wait for another package
			}
			else
			{
				break;
			}
		}
		result = offset;
		if(offset == 0)
		{
		    debug_printf("atcmd","%s:read %s error\n\r",__FUNCTION__,devname);
		    goto EXIT_READFAILED;
		}
		else
		{
			memset(&buf[offset],0,lenleft); //make the bufleft to zero
		}
		if((argc > 0)&&(NULL != argv))
		{
			result = __getpara((char *)buf,"\n\r",argv,argc);
			if(result <= 0)
			{
			    debug_printf("atcmd","%s:get para failed\n\r",__FUNCTION__);
			    goto EXIT_GETPARA;
			}
		}
	}
	//close the device and return
	iodevclose(dev);
	return result;

EXIT_GETPARA:
EXIT_READFAILED:
EXIT_WRITEFAILED:
	iodevclose(dev);
EXIT_OPENFAILED:
	return  result;
}


u8 gAtRcvBuf[CN_AT_LEN];
//-----------------------------------------------------------------------------
//功能:use this function to do the at command
//参数:
//返回:
//备注:
//作者:zhangqf@上午10:54:07/2017年3月23日
//-----------------------------------------------------------------------------
//bool_t AtCmdShell(char *cmd)
ADD_TO_SHELL_HELP(atcmd,"usage:atcmd command");
ADD_TO_IN_SHELL bool_t atcmd(char *cmd)
{
	int result;
	int argc = 10;
	char *argv[10];
	if(NULL == pAtDevName)
	{
	    debug_printf("atcmd","atdevice not configure yet\n\r");
	    return true;
	}
	if(NULL == cmd)
	{
	    debug_printf("atcmd","atcommand NULL\n\r");
	    return true;
	}
	result = AtCmd(pAtDevName,cmd,gAtRcvBuf,CN_AT_LEN,argc,argv);
	if(result > 0)
	{
		for(int i = 0;i <result;i++)
		{
		    debug_printf("atcmd","args:%d:%s\n\r",i,argv[i]);
		}
	}
	return true;
}

//usage:use this function to check if the string is in the argv
//      if exit then return the position it in
static int strinargs(int argc,char *argv[],char *str)
{
	int result = -1;
	int i= 0;
	while(i < argc)
	{
		if(strstr(argv[i],str))
		{
			result = i;
			break;
		}
		else
		{
			i++;
		}
	}
	return result;
}

typedef struct
{
	const char *mcc_mnc;
	const char *apndefault;
}tagImsi;

tagImsi gAtcimi[]={\
		{"46000","cmnet"},\
		{"46002","cmnet"},\
		{"46004","cmnet"},\
		{"46007","cmnet"},\
		{"46001","3gnet"},\
		{"46006","3gnet"},\
		{"46009","3gnet"},\
		{"46003","ctnet"},\
		{"46005","ctnet"},\
		{"46011","ctlte"},\
//		{"46020","cmnet"},  //not support the tietong
};
#define CN_CIMI_SIZE  (sizeof(gAtcimi)/sizeof(tagImsi))

//usage:used to check the lte module
static bool_t checkmi(char *devname,int times)
{
	bool_t result = false;
	char *argv[6];
	int   argc;
	int   i = 0;
	int  position;

	//first we should check if the sim card inserted:at+cpin?
	    debug_printf("atcmd","checkcgmi:");
	for(i =0;i<times;i++)
	{
	    debug_printf("atcmd","%d->",i);
		memset(argv,0,sizeof(argv));
		memset(gAtRcvBuf,0,sizeof(gAtRcvBuf));
		argc = AtCmd(devname,"at+cgmi",gAtRcvBuf,CN_AT_LEN,6,argv);
		if(argc > 0)
		{
			position = strinargs(argc,argv,"OK");
			if((position != 0)&&(position != -1))
			{
				result = true;
				break;
			}
		}
	}
	if(result)
	{
	    debug_printf("atcmd",".:OK:%s\n\r",argv[position-1]);
	}
	else
	{
	    debug_printf("atcmd",".timeout!\n\r");
	}
	return result;
}
//usage:used to check the module type
static bool_t checkmm(char *devname,int times)
{
	bool_t result = false;
	char *argv[6];
	int   argc;
	int   i = 0;
	int  position;

	//first we should check if the sim card inserted:at+cpin?
	    debug_printf("atcmd","checkcgmm:");
	for(i =0;i<times;i++)
	{
	    debug_printf("atcmd","%d->",i);
		memset(argv,0,sizeof(argv));
		memset(gAtRcvBuf,0,sizeof(gAtRcvBuf));
		argc = AtCmd(devname,"at+cgmm",gAtRcvBuf,CN_AT_LEN,6,argv);
		if(argc > 0)
		{
			position = strinargs(argc,argv,"OK");
			if((position != 0)&&(position != -1))
			{
				result = true;
				break;
			}
		}
	}
	if(result)
	{
	    debug_printf("atcmd",".:OK:%s\n\r",argv[position-1]);
	}
	else
	{
	    debug_printf("atcmd",".timeout!\n\r");
	}
	return result;
}
//usage:used to check the module sn
static bool_t checksn(char *devname,int times)
{
	bool_t result = false;
	char *argv[6];
	int   argc;
	int   i = 0;
	int  position;

	//first we should check if the sim card inserted:at+cpin?
	    debug_printf("atcmd","checkcgsn:");
	for(i =0;i<times;i++)
	{
	    debug_printf("atcmd","%d->",i);
		memset(argv,0,sizeof(argv));
		memset(gAtRcvBuf,0,sizeof(gAtRcvBuf));
		argc = AtCmd(devname,"at+cgsn",gAtRcvBuf,CN_AT_LEN,6,argv);
		if(argc > 0)
		{
			position = strinargs(argc,argv,"OK");
			if((position != 0)&&(position != -1))
			{
				result = true;
				break;
			}
		}
	}
	if(result)
	{
	    debug_printf("atcmd",".:OK:%s\n\r",argv[position-1]);
	}
	else
	{
	    debug_printf("atcmd",".timeout!\n\r");
	}
	return result;
}
//usage:used to check the module sn
static bool_t checkmr(char *devname,int times)
{
	bool_t result = false;
	char *argv[6];
	int   argc;
	int   i = 0;
	int  position;

	//first we should check if the sim card inserted:at+cpin?
	debug_printf("atcmd","checkcgmr:");
	for(i =0;i<times;i++)
	{
	    debug_printf("atcmd","%d->",i);
		memset(argv,0,sizeof(argv));
		memset(gAtRcvBuf,0,sizeof(gAtRcvBuf));
		argc = AtCmd(devname,"at+cgmr",gAtRcvBuf,CN_AT_LEN,6,argv);
		if(argc > 0)
		{
			position = strinargs(argc,argv,"OK");
			if((position != 0)&&(position != -1))
			{
				result = true;
				break;
			}
		}
	}
	if(result)
	{
	    debug_printf("atcmd",".:OK:%s\n\r",argv[position-1]);
	}
	else
	{
	    debug_printf("atcmd",".timeout!\n\r");
	}
	return result;
}
//usage:used to check sim card mnc
static tagImsi* checkcimi(char *devname,int times,char *simapn)
{
	char *argv[6];
	int   argc;
	int   i = 0,tmp =0;
	int  position =-1;
	//find the mnc here
	tagImsi* result = NULL;
	//first we should check if the sim card inserted:at+cpin?
	    debug_printf("atcmd","checkcimi:");
	for(i =0;i<times;i++)
	{
	    debug_printf("atcmd","%d->",i);
		memset(argv,0,sizeof(argv));
		memset(gAtRcvBuf,0,sizeof(gAtRcvBuf));
		argc = AtCmd(devname,"at+cimi",gAtRcvBuf,CN_AT_LEN,6,argv);
		if(argc > 0)
		{
			position = strinargs(argc,argv,"OK");
			if((position != 0)&&(position != -1))
			{
				char mnc[6];
				memset(mnc,0,6);
				memcpy(mnc,argv[position-1],5);
				for(tmp =0; tmp <CN_CIMI_SIZE;tmp++)
				{
					if(0 == strcmp(mnc,gAtcimi[tmp].mcc_mnc))
					{
						result = &gAtcimi[tmp];
						if( (simapn==NULL) || (simapn[0]=='\0') )
						    debug_printf("atcmd",".:OK:cimi:%s apn:%s\n\r",argv[position-1],result->apndefault);
						else
						    debug_printf("atcmd",".:OK:cimi:%s apn:%s\n\r",argv[position-1],simapn);
						break;
					}
				}
				if(NULL == result)
				{
				    debug_printf("atcmd",".:OK:cimi:%s apn:%s\n\r",argv[position-1],"unknown");
				}
				break;
			}
		}
	}
	if((position == 0)||(position == -1))
	{
	    debug_printf("atcmd",".timeout!\n\r");
	}
	return result;
}
//usage:used to check if the simcard is inserted
static bool_t checkcpin(char *devname,int times)
{
	bool_t result = false;
	char *argv[6];
	int   argc;
	int   i = 0;

	//first we should check if the sim card inserted:at+cpin?
	    debug_printf("atcmd","checkcpin:");
	for(i =0;i<times;i++)
	{
	    debug_printf("atcmd","%d->",i);
		memset(argv,0,sizeof(argv));
		memset(gAtRcvBuf,0,sizeof(gAtRcvBuf));
		argc = AtCmd(devname,"at+cpin?",gAtRcvBuf,CN_AT_LEN,6,argv);
		if(argc > 0)
		{
			if(-1!=strinargs(argc,argv,"READY"))
			{
				result = true;
				break;
			}
		}
	}
	if(result)
	{
	    debug_printf("atcmd",".:OK\n\r");
	}
	else
	{
	    debug_printf("atcmd",".timeout!\n\r");
	}
	return result;
}

//usage:used to check if the net is registered
static bool_t  checkcgreg(char *devname,int times)
{
	bool_t result = false;
	char *argv[6];
	int   argc;
	int   i = 0;

	//first we should check if the sim card inserted:at+cpin?
	    debug_printf("atcmd","checkcreg:");
	for(i =0;i<times;i++)
	{
	    debug_printf("atcmd","%d->",i);
		memset(argv,0,sizeof(argv));
		memset(gAtRcvBuf,0,sizeof(gAtRcvBuf));
		argc = AtCmd(devname,"at+cgreg?",gAtRcvBuf,CN_AT_LEN,6,argv);
		if(argc > 0)
		{
			if((-1!=strinargs(argc,argv,",1"))||(-1!=strinargs(argc,argv,",5")))
			{
				result = true;
				break;
			}
		}
	}
	if(result)
	{
	    debug_printf("atcmd",".:OK\n\r");
	}
	else
	{
	    debug_printf("atcmd",".timeout!\n\r");
	}
	return result;
}
//usage:used to set the apn
static bool_t  setnetapn(char *devname,char *apn,int times)
{
	bool_t result = false;
	char *argv[6];
	int   argc;
	int   i = 0;

	//first we should check if the sim card inserted:at+cpin?
	    debug_printf("atcmd","setapn:");
	for(i =0;i<times;i++)
	{
	    debug_printf("atcmd","%d->",i);
		memset(argv,0,sizeof(argv));
		memset(gAtRcvBuf,0,sizeof(gAtRcvBuf));
		char cgdcont[64];
		memset(cgdcont,0,64);
		snprintf(cgdcont,63,"%s%s%s%s","at+cgdcont=1,\"ip\",","\"",apn,"\"");
		argc = AtCmd(devname,cgdcont,gAtRcvBuf,CN_AT_LEN,6,argv);
		if(argc > 0)
		{
			if((-1!=strinargs(argc,argv,"OK"))||(-1!=strinargs(argc,argv,"ok")))
			{
				result = true;
				break;
			}
		}
	}
	if(result)
	{
	    debug_printf("atcmd",".ready!\n\r");
	}
	else
	{
	    debug_printf("atcmd",".timeout!\n\r");
	}
	return result;
}

//usage:used to call the data connection
static bool_t  atdcall(char *devname,int times)
{
	bool_t result = false;
	char *argv[6];
	int   argc;
	int   i = 0;

	//first we should check if the sim card inserted:at+cpin?
	    debug_printf("atcmd","atdcall:");
	for(i =0;i<times;i++)
	{
	    debug_printf("atcmd","%d->",i);
		memset(argv,0,sizeof(argv));
		memset(gAtRcvBuf,0,sizeof(gAtRcvBuf));
		argc = AtCmd(devname,"atd*99#",gAtRcvBuf,CN_AT_LEN,6,argv);
		if(argc > 0)
		{
			if((-1!=strinargs(argc,argv,"CONNECT"))||(-1!=strinargs(argc,argv,"connect")))
			{
				result = true;
				break;
			}
		}
	}
	if(result)
	{
	    debug_printf("atcmd",".ready!\n\r");
	}
	else
	{
	    debug_printf("atcmd",".timeout!\n\r");
	}
	return result;
}

//usage:use this functiont to make the device to the ppp mode
bool_t AtDial(char *devname,char *apn)
{
	bool_t result = false;

	char  *simapn;
	simapn = apn;
	debug_printf("atcmd","ATCMD CALL BEGIN:\n\r");
	//first we should check the module type
	result = checkmi(devname,32);
	if(result == false)
	{
		return result;
	}
	//check the mm
	result = checkmm(devname,32);
	if(result == false)
	{
		return result;
	}
	//check the mr
	result = checkmr(devname,32);
	if(result == false)
	{
		return result;
	}
	//check the sn
	result = checksn(devname,32);
	if(result == false)
	{
		return result;
	}
	//first we should check if the sim card inserted:at+cpin?
	result = checkcpin(devname,32);
	if(result == false)
	{
		return result;
	}
	//check the apn here
	if((NULL == simapn)||(simapn[0]=='\0'))
	{
		//do check our self
		tagImsi *imsi;
		imsi = checkcimi(devname,32,NULL);
		if(NULL == imsi)
		{
			result = false ;
			return result;
		}
		else
		{
			simapn = (char *)imsi->apndefault;
		}
	}
	else
	{
		checkcimi(devname,32,simapn);
	}
	//check if we has register the sim card to the carrieroperator
	result = checkcgreg(devname,32);
	if(result == false)
	{
		return result;
	}
	//OK,now set the apn to the carrieroperator
	result = setnetapn(devname,simapn,32);
	if(result == false)
	{
		return result;
	}
	//now we begin to atd(call the data service)
	result = atdcall(devname,32);
	if(result == false)
	{
		return result;
	}
	//for the end
	return result;
}
//static bool_t  AtDialShell(char *param)
ADD_TO_SHELL_HELP(atdial,"usage:atdial devname apn");
ADD_TO_IN_SHELL static bool_t  atdial(char *param)
{
	bool_t result = false;
	int argc =2;
	char *argv[2];
	char *devname;
	char *apn;
	argc = getargs(argc,argv,param);
	if(argc != 2)
	{
		return result;
	}
	devname = argv[0];
	apn = argv[1];
	result = AtDial(devname,apn);
	if(result == false)
	{
	    debug_printf("atcmd","AT DIAL:DEV:%s APN:%s failed\n\r",devname,apn);
	}
	else
	{
	    debug_printf("atcmd","AT DIAL:DEV:%s APN:%s success\n\r",devname,apn);
	}

	return result;

}
int AtGetSignal(const char *atdev)
{
	char *argv[6];
	int   argc;
	int   position =-1;
	u8    buf[32];
	int   result = -1;
	memset(argv,0,sizeof(argv));
	memset(buf,0,sizeof(buf));
	argc = AtCmd(atdev,"at+csq",buf,32,6,argv);
	if(argc > 0)
	{
		position = strinargs(argc,argv,"OK");
		if((position != 0)&&(position != -1))
		{
			sscanf(argv[position-1],"+CSQ: %d",&result);
		}
		else
		{
			result = -1;
		}
	}
	return result;
}
//static bool_t AtGetSignalShell(char *param)
ADD_TO_SHELL_HELP(atsignal,"usage:atsignal [devname](if not supplied, will use the default)");
ADD_TO_IN_SHELL static bool_t atsignal(char *param)
{
	const char *atdev;
	int signal;
	if(NULL == param)
	{
		atdev = pAtDevName;
	}
	else
	{
		atdev = param;
	}
	signal = AtGetSignal(atdev);
	if(signal ==-1)
	{
	    debug_printf("atcmd","%s:timeout or execute failed\n\r",__FUNCTION__);
	}
	else
	{
	    debug_printf("atcmd","%s:Signal Strenth:%d\n\r",__FUNCTION__,signal);
	}
	return true;
}
//usage:this is used for the at debeug
//struct ShellCmdTab  gAtDebugItem[] =
//{
//    {
//        "atcmd",
//		AtCmdShell,
//        "usage:atcmd command",
//        "usage:atcmd command",
//    },
//    {
//        "atdevset",
//		AtDefaultDevSet,
//        "usage:atdevset devname",
//        "usage:atdevset devname",
//    },
//    {
//        "atdevget",
//		AtDefaultDevGetShell,
//        "usage:atdevget",
//        "usage:atdevset",
//    },
//    {
//        "atdial",
//		AtDialShell,
//        "usage:atdial devname apn",
//        "usage:atdial devname apn",
//    },
//    {
//        "atsignal",
//		AtGetSignalShell,
//        "usage:atsignal [devname](if not supplied, will use the default)",
//        "usage:atsignal [devname](if not supplied, will use the default)",
//    }
//};
//#define CN_AtDebug_NUM  ((sizeof(gAtDebugItem))/(sizeof(struct ShellCmdTab)))
//static struct ShellCmdRsc gAtDebugCmdRsc[CN_AtDebug_NUM];
//-----------------------------------------------------------------------------
//功能:this is the ppp main function here
//参数:
//返回:
//备注:
//作者:zhangqf@下午4:06:57/2017年1月5日
//-----------------------------------------------------------------------------
//bool_t PppAtInit(ptu32_t para)
//{
//	bool_t result;
//	result = Sh_InstallCmd(gAtDebugItem,gAtDebugCmdRsc,CN_AtDebug_NUM);
//	return result;
//}



