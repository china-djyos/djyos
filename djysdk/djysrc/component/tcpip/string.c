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
#include <sys/socket.h>

//this function is used to format the char string to the argc mode
//this function will changed the original string, used it carefully
int string2arg(int *argc, char *argv[],char *string)
{
	int argvlen = 0;
	int paramnum = 0;
	char *tmp = NULL;
	char bak;
	int len;

	argvlen = *argc;
	*argc = paramnum;
	if(NULL == string)
	{
		return 1;
	}

	//use the '\0' to replace the ' '
	len = strlen(string);
	tmp = string;
	while(tmp < (string + len))
	{
		if(*tmp == ' ')
		{
			*tmp = '\0';
		}
		tmp++;
	}
	bak = '\0';
	tmp = string;
	while(tmp < (string + len))
	{
		if((*tmp != '\0')&&(bak =='\0'))
		{
			if(paramnum < argvlen)
			{
				argv[paramnum] = tmp;
				paramnum++;
			}
		}
		bak = *tmp;
		tmp++;
	}
	*argc = paramnum;

	return 0;
}

//usage:use this function to change the string to the args
int getargs(int argc, char *argv[],char *string)
{
	int argvlen = 0;
	int paramnum = 0;
	char *tmp = NULL;
	char bak;
	int len;

	argvlen = argc;
	if(NULL == string)
	{
		return paramnum;
	}

	//use the '\0' to replace the ' '
	len = strlen(string);
	tmp = string;
	while(tmp < (string + len))
	{
		if(*tmp == ' ')
		{
			*tmp = '\0';
		}
		tmp++;
	}
	bak = '\0';
	tmp = string;
	while(tmp < (string + len))
	{
		if((*tmp != '\0')&&(bak =='\0'))
		{
			if(paramnum < argvlen)
			{
				argv[paramnum] = tmp;
				paramnum++;
			}
		}
		bak = *tmp;
		tmp++;
	}
	return paramnum;
}



//this is an function tools, used to make the string to mac address
//string must be xx-xx-xx-xx-xx-xx style
bool_t string2Mac(char *str,u8 *mac)
{
    bool_t result = false;
    int num = 0;
    int i =0;

    unsigned int data[CN_MACADDR_LEN];

    num = sscanf(str,"%x-%x-%x-%x-%x-%x",&data[0],&data[1],&data[2],&data[3],&data[4],&data[5]);
    if(num == CN_MACADDR_LEN)
    {
        result = true;
        //cpy the mac
        for(i=0;i<CN_MACADDR_LEN;i++)
        {
            mac[i] = (unsigned char)data[i];
        }
    }

    return result;
}
char* Mac2string(u8 *mac)
{
    int i =0;
    static char  str[30];

    unsigned int data[CN_MACADDR_LEN];

    for(i = 0;i <CN_MACADDR_LEN;i++)
    {
    	data[i] = (unsigned int)mac[i];
    }
    memset(str,0,30);
    sprintf(str,"%x-%x-%x-%x-%x-%x",data[0],data[1],data[2],data[3],data[4],data[5]);

    return str;
}

