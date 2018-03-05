//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

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
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#include <os.h>

#define CN_CPUMSG_MAGIC     0X1234
#define CN_CPUMSG_ITEMNUM   16
typedef struct
{
	const char     *msgname;
	const u8       *msg;
	u8              msglen;
	u8              msgtype;
	u16             magic;
}tagCpuMsg;
static tagCpuMsg gCpuMsgItem[CN_CPUMSG_ITEMNUM] DATA_BEFOREPRELOAD; //��Ҫ���ӵ��߼��ܿ�ǰ��λ��

//----�����ϵͳlog cpu��Ϣ-----------------------------------------------------
//���ܣ�ִ�иú�����ϵͳlogһ��CPU��Ӳ����Ϣ
//������msgname,��Ϣ��Ŀ���֣��ȷ�CPU��Ƶ���߹̼��汾֮���
//    msg,log����Ϣ��Ҫ��֤����Ϣ�Ĵ洢�Ǿ�̬�Ĳ����ĵ�
//    msglen,log����Ϣ����
//    type,�ο�enCpuMsgType�Ķ���
//���أ�true�ɹ� ����ʧ��
//��ע���ú���������CPU��startup�׶ξͿ��ܵ��á���˶����Ǹ��ֽ׶μ��ص�Ӳ����ϵ�£��ô������������preload֮ǰ
//---------------------------------------------------------------------------
CODE_BEFOREPRELOAD bool_t LogCpuMsg(const char *msgname, const void *msg,u8 msglen, enCpuMsgType type)
{
	u8 i =0;
	bool_t ret = false;
	for(i =0;i<CN_CPUMSG_ITEMNUM;i++)
	{
		if(gCpuMsgItem[i].magic != CN_CPUMSG_MAGIC)
		{
			//no same item,so create one
			gCpuMsgItem[i].msgname = msgname;
			gCpuMsgItem[i].msg = msg;
			gCpuMsgItem[i].msglen = msglen;
			gCpuMsgItem[i].msgtype = type;
			gCpuMsgItem[i].magic = CN_CPUMSG_MAGIC;
			ret = true;
			break;
		}
	}
	return ret;
}
//----չʾע���CPU ��Ϣ------------------------------------------------------------
//���ܣ�ִ�иú�����չʾע���CPUӲ����Ϣ
//������������
//���أ���
//------------------------------------------------------------------------------
bool_t ShowCpuInfo(char *param)
{
	u8 i =0;
	u8 num = 0;
	tagCpuMsg *item;
	u32  value;
	u8   v8;
	float point;
	printf("CPUINFO:\n\r");
	for(i =0;i<CN_CPUMSG_ITEMNUM;i++)
	{
		item = &gCpuMsgItem[i];
		if(item->magic == CN_CPUMSG_MAGIC)
		{
			switch (item->msgtype)
			{
				case EN_CPU_MSGTYPE_SIZE:
					value = 0;
					memcpy(&value,item->msg,item->msglen);
					if(value > 1024*1024)//MB
					{
						point = ((float)value)/(1024*1024);
						printf("%-12s:%f MB \n\r",item->msgname,point);
					}
					else if(value > 1024)//KB
					{
						point = ((float)value)/(1024);
						printf("%-12s:%f KB \n\r",item->msgname,point);
					}
					else
					{
						printf("%-12s:%d BYTES \n\r",item->msgname,value);
					}
					break;
				case EN_CPU_MSGTYPE_FREQUENCY:
					value = 0;
					memcpy(&value,item->msg,item->msglen);

					if(value > 1000*1000)//MHZ
					{
						point = ((float)value)/(1000*1000);
						printf("%-12s:%f MHZ \n\r",item->msgname,point);
					}
					else if(value > 1024)//KHZ
					{
						point = ((float)value)/(1000);
						printf("%-12s:%f KHZ \n\r",item->msgname,point);
					}
					else
					{
						printf("%-12s:%d HZ\n\r",item->msgname,value);
					}
					break;
				case EN_CPU_MSGTYPE_STRING:
					printf("%-12s:%s\n\r",item->msgname,item->msg);
					break;
				case EN_CPU_MSGTYPE_MULTIBYTES:
					printf("%-12s:",item->msgname);
					for(value = 0;value < item->msglen;value++)
					{
						v8 = *((u8 *)item->msg+value);
						printf("%02x ",v8);
					}
					printf("\n\r");
					break;
				default:
					break;
			}
			num++;
		}
	}
	if(num ==0)
	{
		printf("CPUINFO:NO MESSAGE REGISTERED YET\n\r");
	}
	else
	{
		printf("CPUINFO:%d MESSAGES REGISTERED\n\r",num);
	}
	return true;
}

//----��ȡע���CPU ��Ϣ------------------------------------------------------------
//���ܣ�ִ�иú�������ȡָ�����ֵ�cpu��Ϣ���ȷ��̶����ֵ�CPUǩ��
//��������
//���أ�-1 ʧ�ܣ�0 �洢����  >0 ����ָ����CPU��Ϣ���洢����ʱ����������Ϣ
//------------------------------------------------------------------------------
s32  GetCpuInfo(const char *name,void *buf, u8 len)
{
	u8 i =0;
	s32 ret = -1;
	tagCpuMsg *item;
	for(i =0;i<CN_CPUMSG_ITEMNUM;i++)
	{
		item = &gCpuMsgItem[i];
		if((item->magic == CN_CPUMSG_MAGIC)&&(0 == strcmp(item->msgname,name)))
		{
			//hit it
			if(len < item->msglen)
			{
				memcpy(buf,item->msg,len);
				ret = 0;
			}
			else
			{
				memcpy(buf,item->msg,item->msglen);
				ret = item->msglen;
			}
			break;
		}
	}
	return ret;
}











