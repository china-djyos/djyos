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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
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
static tagCpuMsg gCpuMsgItem[CN_CPUMSG_ITEMNUM] DATA_BEFOREPRELOAD; //需要链接到逻辑很靠前的位置

//----向操作系统log cpu信息-----------------------------------------------------
//功能：执行该函数向系统log一条CPU的硬件信息
//参数：msgname,信息条目名字，比方CPU主频或者固件版本之类的
//    msg,log的信息，要保证该信息的存储是静态的不更改的
//    msglen,log的信息长度
//    type,参考enCpuMsgType的定义
//返回：true成功 否则失败
//备注：该函数可能在CPU的startup阶段就可能调用。因此对于那个分阶段加载的硬件体系下，该代码必须链接在preload之前
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
//----展示注册的CPU 信息------------------------------------------------------------
//功能：执行该函数将展示注册的CPU硬件信息
//参数：无意义
//返回：无
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

//----获取注册的CPU 信息------------------------------------------------------------
//功能：执行该函数将获取指定名字的cpu信息，比方固定名字的CPU签名
//参数：无
//返回：-1 失败，0 存储不足  >0 拷贝指定的CPU信息，存储不足时拷贝部分信息
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











