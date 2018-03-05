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
//author:zhangqf
//date  :����5:02:15/2016��12��28��
//usage :
//------------------------------------------------------

//standard includes
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <os.h>
//add your own specified header here
#include <sys/socket.h>
#include "linkhal.h"
#include "rout.h"
#include "arp.h"
#include "ip.h"
#include "tcpipconfig.h"

////////////////////////LINK LAYER///////////////////////////////////
//use this function to send the raw package for the device
//the handle is got by the install device or by name got
bool_t LinkSendRaw(tagNetDev *dev,tagNetPkg *pkg,u32 framlen,u32 devtask)
{
    bool_t result = false;
    if((NULL != dev) &&(NULL != dev->ifsend)&&(NULL != pkg))
    {
    	result = dev->ifsend((ptu32_t)dev,pkg,framlen,devtask);
    }
    return result;
}
//-----------------------------------------------------------------------------
//����:use this function to send data in buffer to the specified net device
//����:devhandle, the net device;
//     buf, the data buffer
//     len, the data len
//����:true success while false failed
//��ע:
//-----------------------------------------------------------------------------
bool_t LinkSendBufRaw(ptu32_t devhandle,u8 *buf, u32 len)
{
    bool_t result = false;
    tagNetDev *dev;
    tagNetPkg *pkg;

    pkg = PkgMalloc(len,CN_PKLGLST_END);
    if(NULL != pkg)
    {
        dev = (tagNetDev *)devhandle;
        if((NULL != dev) &&(NULL != dev->ifsend)&&(NULL != pkg))
        {
        	memcpy(pkg->buf+pkg->offset,buf,len);
        	pkg->datalen = len;
        	result = dev->ifsend((ptu32_t)dev,pkg,len,CN_IPDEV_NONE);
            TCPIP_DEBUG_INC(dev->fsnd);
            if(result == false)
            {
                TCPIP_DEBUG_INC(dev->fsnderr);
            }
        }
        PkgTryFreeLst(pkg);
    }
    return result;
}
//-----------------------------------------------------------------------------
//����:the ip layer use this function to send the ip frame to the link
//����:
//����:
//��ע:
//����:zhangqf@����7:48:16/2016��12��28��
//-----------------------------------------------------------------------------
bool_t LinkSend(tagRout *rout,tagNetPkg *pkg,u32 framlen,u32 devtask,u16 proto,\
                enum_ipv_t ver,ipaddr_t ip)
{
    bool_t result = false;
    tagNetDev *dev;

    if((NULL != rout) &&(NULL != rout->dev))
    {
        dev = rout->dev;
        if((NULL != dev->linkops)&&(NULL != dev->linkops->linkout))
        {
        	result = dev->linkops->linkout(rout,pkg,framlen,devtask,proto,ver,ip);
        }
        TCPIP_DEBUG_INC(dev->fsnd);
        TCPIP_DEBUG_INC(rout->outnum);
        if(result == false)
        {
            TCPIP_DEBUG_INC(dev->fsnderr);
            TCPIP_DEBUG_INC(rout->outerr);
        }
    }
    return result;
}
//-----------------------------------------------------------------------------
//����:the device thread or irq handler use this function to pass the package to the stack
//����:
//����:
//��ע:
//����:zhangqf@����7:59:30/2016��12��28��
//-----------------------------------------------------------------------------
bool_t LinkPost(ptu32_t devhandle,tagNetPkg *pkg)
{
    bool_t          result=false;
    tagNetDev      *dev;

    dev = (tagNetDev *)devhandle;
    TCPIP_DEBUG_INC(dev->frcv);
    if((NULL != dev)&&(NULL != pkg)&&(pkg->datalen > 0))
    {
    	if((NULL != dev->linkops)&&(NULL != dev->linkops->linkin))
    	{
    		result = dev->linkops->linkin(dev,pkg);
    	}
    }
    if(result == false)
    {
        TCPIP_DEBUG_INC(dev->frcverr);
    }
    return result;
}

//usage:this is the link monitor item data structure
typedef struct __LinkRcvHookItem
{
	struct __LinkRcvHookItem *nxt;  //used to do the queue
	const char *      name;         //may be you want to specify an name here
	u16               proto;        //the listen  protocol you have set
	ptu32_t           devhandle;    //the listen device you have set
	s64               framenum;     //how many frames has listen
	fnLinkProtoDealer hook;         //the listen hook you have set
}tagLinkRcvHookItem;
typedef struct
{
	u32 itemtotal;
	s64 framenum;
	s64 frameunknown;
	tagLinkRcvHookItem *list;
	struct MutexLCB    *mutex;
}tagLinkRcvHookModule;
static tagLinkRcvHookModule  gLinkRcvHookModule;
//if you specified the special device, only the same device and the same proto frame
//will passed into the hook;if the devhandler is NULL,then any proto with the same proto
//will passed into the hook.
//you could use this function to register some link protocol to do the functions
bool_t LinkRegisterRcvHook(fnLinkProtoDealer hook, ptu32_t devhandle,u16 proto,const char *hookname)
{
	bool_t result = false;
	tagLinkRcvHookItem *item;

	if(NULL == hook)
	{
		return result;
	}
	item = malloc(sizeof(tagLinkRcvHookItem));
	if(NULL == item)
	{
		return result;
	}
	memset((void *)item,0,sizeof(tagLinkRcvHookItem));
	item->devhandle = devhandle;
	item->hook = hook;
	item->proto = proto;
	item->name = hookname;

	if(Lock_MutexPend(gLinkRcvHookModule.mutex,CN_TIMEOUT_FOREVER))
	{
		item->nxt = gLinkRcvHookModule.list;
		gLinkRcvHookModule.list = item;
		TCPIP_DEBUG_INC(gLinkRcvHookModule.itemtotal);
		result = true;

		Lock_MutexPost(gLinkRcvHookModule.mutex);
	}
	else
	{
		free(item);
	}
	return result;
}

//-----------------------------------------------------------------------------
//����:you could use this function to unregister the hook you got last time
//����:hook,the hook you registered;
//����:
//��ע:
//����:zhangqf@����7:45:38/2016��12��28��
//-----------------------------------------------------------------------------
bool_t LinkUnRegisterRcvHook(fnLinkProtoDealer hook, ptu32_t devhandle,u16 proto,const char *hookname)
{
	bool_t result = false;
	tagLinkRcvHookItem *item;
	tagLinkRcvHookItem *nxt;

	if(Lock_MutexPend(gLinkRcvHookModule.mutex,CN_TIMEOUT_FOREVER))
	{
		item = gLinkRcvHookModule.list;
		if((item->hook == hook)&&\
			(item->devhandle == devhandle)&&\
			(item->proto == proto)&&\
			(item->name == hookname))
		{
			//the first match
			gLinkRcvHookModule.list = item->nxt;
			free(item);
			TCPIP_DEBUG_DEC(gLinkRcvHookModule.itemtotal);
			result= true;
		}
		else
		{
			while(NULL != item->nxt)
			{
				nxt = item->nxt;
				if((nxt->hook == hook)&&\
					(nxt->devhandle == devhandle)&&\
					(nxt->proto == proto)&&\
					(nxt->name == hookname))
				{

					item->nxt = nxt->nxt;
					free(nxt);
					TCPIP_DEBUG_DEC(gLinkRcvHookModule.itemtotal);
					result= true;
					break;
				}
				else
				{
					item = nxt;
				}
			}
		}
		Lock_MutexPost(gLinkRcvHookModule.mutex);
	}

	return result;
}
//use this function to do hook protocol deal
bool_t __LinkHookPost(tagNetDev  *dev,tagNetPkg *pkg,u16 proto)
{
	bool_t  result = false;
	fnLinkProtoDealer hook = NULL;
	tagLinkRcvHookItem *item;

	//find the hook first
	if(Lock_MutexPend(gLinkRcvHookModule.mutex,CN_TIMEOUT_FOREVER))
	{
		item = gLinkRcvHookModule.list;
		if(dev->iftype == EN_LINK_ETHERNET)
		{
			//here we recover the link head for the ethernet(14 bytes here 6 macdst 6 macsrc 2 type)
			pkg->datalen += 14;
			pkg->offset -= 14;
		}
		while(NULL != item)
		{
			if((proto == item->proto)&&\
			  ((dev == (tagNetDev *)item->devhandle)||\
			   (NULL == (tagNetDev *)item->devhandle)))
			{
				hook = item->hook;
				TCPIP_DEBUG_INC(item->framenum);
				break;
			}
			item = item->nxt;
		}
		Lock_MutexPost(gLinkRcvHookModule.mutex);
	}

	if(NULL != hook)
	{
		result = hook((ptu32_t)dev,proto,pkg);
		TCPIP_DEBUG_INC(gLinkRcvHookModule.framenum);
	}
	else
	{
		TCPIP_DEBUG_INC(gLinkRcvHookModule.frameunknown);
	}
	return result;
}
//-----------------------------------------------------------------------------
//����:the link type like the ethernet 80211 link dealer call this func to pass the
//    the package to the tcpip stack
//����:
//����:
//��ע:
//����:zhangqf@����3:55:30/2016��12��29��
//-----------------------------------------------------------------------------
extern bool_t  IpPushPkg(tagNetDev *dev,tagNetPkg *pkg);
extern bool_t  ArpPushPkg(tagNetDev *dev,tagNetPkg *pkg);
bool_t Link2IP( tagNetDev  *dev,tagNetPkg *pkg,u16 proto)
{
    bool_t          result=false;
    //we analyze the ethernet header first, which type it has
    switch(proto)
    {
     //uptil now, we support the ipv4
        case EN_LINKPROTO_IPV4:
            result = IpPushPkg(dev,pkg);
            break;
        case EN_LINKPROTO_IPV6:
            result = IpPushPkg(dev,pkg);
            break;
        case EN_LINKPROTO_ARP:
            result = ArpPushPkg(dev,pkg);
            break;
        default:
        	result = __LinkHookPost(dev,pkg,proto);
            break;
    }
    return result;
}

//here define the link register and unregister here
typedef struct
{
	u8          iftype;
	const char *name;
	tagLinkOps  ops;
}tagLink;
static tagLink  *pLinkHal[EN_LINK_LAST];
//-----------------------------------------------------------------------------
//����:use this function to find a link operation
//����:
//����:
//��ע:
//����:zhangqf@����8:22:20/2016��12��28��
//-----------------------------------------------------------------------------
tagLinkOps *LinkFindOps(u8 type)
{
	tagLinkOps *result = NULL;
	if((type < EN_LINK_LAST)&&(NULL != pLinkHal[type]))
	{
		result = &pLinkHal[type]->ops;
	}
	return result;
}
//-----------------------------------------------------------------------------
//����:you could use this function to find a registered link type name
//����:type,which defines by enLinkType in socket.h
//����:
//��ע:
//����:zhangqf@����6:44:54/2017��1��3��
//-----------------------------------------------------------------------------
const char *LinkTypeName(u8 type)
{

	const char *name = "Unknown";
	if((type < EN_LINK_LAST)&&(NULL != pLinkHal[type]))
	{
		name = pLinkHal[type]->name;
	}
	return name;
}
//-----------------------------------------------------------------------------
//����:use this function to register a link here
//����:
//����:
//��ע:
//����:zhangqf@����8:25:38/2016��12��28��
//-----------------------------------------------------------------------------
bool_t LinkRegister(u8 type,const char *name,tagLinkOps *ops)
{
	bool_t result = false;
	tagLink *link = NULL;

	if((NULL != ops)&&(NULL != name)&&(type<EN_LINK_LAST)&&(NULL == pLinkHal[type]))
	{
		link = malloc(sizeof(tagLink));
		if(NULL != link)
		{
			link->iftype = type;
			link->name = name;
			link->ops = *ops;
			pLinkHal[type] = link;
			result = true;
		}
	}
	return result;
}

//-----------------------------------------------------------------------------
//����:use this function to unregister ther link we registered
//����:
//����:
//��ע:
//����:zhangqf@����1:55:39/2016��12��29��
//-----------------------------------------------------------------------------
bool_t LinkUnRegister(u8 type,const char *name,tagLinkOps *ops)
{
	bool_t result = false;
	tagLink *link = NULL;

	if((NULL != ops)&&(NULL != name)&&(type<EN_LINK_LAST)&&(NULL == pLinkHal[type]))
	{
		link = pLinkHal[type];
		if((NULL != link)&&(0 == strcmp(link->name,name)))
		{
			free(link);
			pLinkHal[type] = NULL;
			result = true;
		}
	}
	return result;
}

//do the hook debug module
bool_t LinkHookShell(char *param)
{
	int i = 0;
	tagLinkRcvHookItem *item;

	printf("LinkHookModule\n\r");
	if(Lock_MutexPend(gLinkRcvHookModule.mutex,CN_TIMEOUT_FOREVER))
	{

		printf("Item:Total:%d  FrameRcvTotal:%lld FrameRcvUnknown:%lld\n\r",\
				gLinkRcvHookModule.itemtotal,gLinkRcvHookModule.framenum,gLinkRcvHookModule.frameunknown);

		printf("%-10s%-10s%-10s%-10s%-10s%-10s\n\r",\
				"Number","HookName","Protocol","Function","DevHandle","FrameNum");

		item = gLinkRcvHookModule.list;
		while(NULL != item)
		{
			printf("No.%-4d   %-8s  %-8x  %-8x  %-8x  %llx\n\r",\
					i++,item->name?item->name:"NULL",\
					item->proto,(u32)item->hook,\
					(u32)item->devhandle,item->framenum);
			item = item->nxt;
		}
		Lock_MutexPost(gLinkRcvHookModule.mutex);
	}
	return true;
}
//usage:use this shell to show all the link type supported
static bool_t LinkTypeShow(char *param)
{
	u8 i;
	tagLink *link;
	printf("%-4s%-10s\n\r","Type","Name");
	for(i = 0;i < EN_LINK_LAST;i++)
	{
		link = pLinkHal[i];
		if(NULL != link)
		{
			printf("%-4d%-10s\n\r",i,link->name);
		}
	}
	return true;
}
//usage:this data used for the debug here
struct ShellCmdTab  gLinkDebug[] =
{
    {
        "linkhook",
		LinkHookShell,
        "usage:linkhook",
        "usage:linkhook",
    },
    {
        "linkshow",
		LinkTypeShow,
        "usage:linkshow",
        "usage:linkshow",
    },

};
#define CN_LinkDebug_NUM  ((sizeof(gLinkDebug))/(sizeof(struct ShellCmdTab)))
static struct ShellCmdRsc gLinkDebugCmdRsc[CN_LinkDebug_NUM];

// =============================================================================
// FUNCTION   :this is the link module initialize function
// PARAMS IN  :
// PARAMS OUT :
// RETURN     :
// DESCRIPTION:uptils now, we do nothing here
// =============================================================================
bool_t LinkInit(void)
{
	//the gLinkHook module must be initialized
	memset((void *)&gLinkRcvHookModule,0,sizeof(gLinkRcvHookModule));
	gLinkRcvHookModule.mutex = Lock_MutexCreate(NULL);
	if(NULL == gLinkRcvHookModule.mutex)
	{
		goto EXIT_MUTEX;
	}
	gLinkRcvHookModule.itemtotal = 0;

	//add all the linktypes here
	extern bool_t LinkRawInit(ptu32_t para);
	LinkRawInit(0);
	extern bool_t LinkEthernetInit(ptu32_t para);
	LinkEthernetInit(0);
	Sh_InstallCmd(gLinkDebug,gLinkDebugCmdRsc,CN_LinkDebug_NUM);

	tcpipmemlog("linkhal",sizeof(tagLink),2);
	return true;

EXIT_MUTEX:
    return false;
}

