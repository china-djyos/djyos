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
#include <netdb.h>

#include "arp.h"
#include "rout.h"
#include "linkhal.h"

//first we should implement the device layer
static tagNetDev        *pNetDevQ        = NULL;
static struct MutexLCB  *pRoutMutex      = NULL;
static struct MutexLCB  gRoutMutexMem          ;

static tagRout          *pRoutDefaultV4  = NULL;   //this is the default  v4 rout
static tagRout          *pRoutLoop       = NULL;   //this is the loop rout
//this function used to get the specified dev
tagNetDev *__NetDevGet(const char *name)
{
    tagNetDev *result = NULL;
    tagNetDev *tmp = pNetDevQ;

    if(NULL == name)
    {
        result = pNetDevQ;
    }
    else
    {
        while(NULL != tmp)
        {
            if(0 == strcmp(tmp->name,name))
            {
                result = tmp;
                tmp = NULL;//end the search
            }
            else
            {
                tmp = tmp->nxt;
            }
        }
    }

    return result;
}
// =============================================================================
// FUNCTION   :find the name specified net device
// PARAMS IN  :name, the net device name,which specified by installation
// PARAMS OUT :NULL
// RETURN     :the name specified net device while NULL failed
// DESCRIPTION:if name is NULL then return the dev head
// =============================================================================
tagNetDev  *NetDevGet(const char  *name)
{
    tagNetDev *result = NULL;

    if(Lock_MutexPend(pRoutMutex, CN_TIMEOUT_FOREVER))
    {
        result = __NetDevGet(name);
        Lock_MutexPost(pRoutMutex);
    }
    return result;
}

ptu32_t NetDevHandle(const char *name)
{
	return (ptu32_t)NetDevGet(name);
}


//the device event listen hook module
static bool_t deveventhookfault(ptu32_t handle,enNetDevEvent event)
{
	switch(event)
	{
		case EN_NETDEVEVENT_LINKDOWN:
			printf("link is down\n\r");  //warning the app to do some notice
			break;
		case EN_NETDEVEVENT_LINKUP:
			printf("link is up\n\r"); //warning the app to do some notice
			break;
		case EN_NETDEVEVENT_IPGET:
			printf("ip get\n\r"); //warning the app to do some notice
			break;
		case EN_NETDEVEVENT_IPRELEASE:
			printf("ip release\n\r"); //warning the app to do some notice
			break;
		case EN_NETDEVEVENT_BROAD_OVER:
			//should shutdown the receive broad frame of the net device
			printf("broad strom has come,stop rcv the broadcast\n\r");
			break;
		case EN_NETDEVEVENT_BROAD_LACK:
			//should open the receive broad frame of the net device
			printf("broad strom has gone,rcv the broadcast\n\r");
			break;

		case EN_NETDEVEVENT_MULTI_OVER:
			//should shutdown the receive broad frame of the net device
			printf("MULTI strom has come,stop rcv the multi\n\r");
			break;
		case EN_NETDEVEVENT_MULTI_LACK:
			//should open the receive broad frame of the net device
			printf("MULTI strom has gone,rcv the multi\n\r");
			break;
		case EN_NETDEVEVENT_POINT_OVER:
			//should shutdown the receive broad frame of the net device
			printf("point strom has come,stop rcv the point\n\r");
			break;
		case EN_NETDEVEVENT_POINT_LACK:
			//should open the receive broad frame of the net device
			printf("point strom has gone,rcv the point\n\r");
			break;
		case EN_NETDEVEVENT_FLOW_OVER:
			//maybe some err happened to the device,should reset
			printf("flow over has come\n\r");
			break;
		case EN_NETDEVEVENT_FLOW_LACK:
			//the net work comes to normal
			printf("flow lack has COME,reset the net device\n\r");
			break;
		case EN_NETDEVEVENT_RESET:
			printf("dev reset\n\r"); //should do some log here
			break;
		default:
			printf("unknown event\n\r"); //should ignore here
			break;
	}
	return true;
}

// =============================================================================
// FUNCTION   :install an net device to the stack
// PARAMS IN  :para, the net device function paras as tagNetDevPara defined
// PARAMS OUT :NULL
// RETURN     :the dev handle
// DESCRIPTION:NO DUPLICATION NAME PERMITTED
// =============================================================================
ptu32_t NetDevInstall(tagNetDevPara *para)
{
    tagNetDev  *dev = NULL;
    tagLinkOps *ops;

    if(NULL == para)
    {
        return (ptu32_t)dev;
    }
    ops = LinkFindOps(para->iftype);

    if((NULL != ops)&&Lock_MutexPend(pRoutMutex, CN_TIMEOUT_FOREVER))
    {
        dev = __NetDevGet(para->name);
        if(NULL == dev)
        {
            dev = malloc(sizeof(tagNetDev));
            if(NULL != dev)
            {
                //fill the dev
                memset((void *)dev, 0, sizeof(tagNetDev));
                strncpy(dev->name,para->name,CN_TCPIP_NAMELEN);
                memcpy(dev->mac, para->mac, CN_MACADDR_LEN);
                dev->ifsend  = para->ifsend;
                dev->ifctrl  = para->ifctrl;
                dev->eventhook = deveventhookfault;
                dev->private = para->private;
                dev->iftype = para->iftype;
                dev->mtu= para->mtu;
                dev->devfunc= para->devfunc;
                //initialize the dev filter part
                dev->rfilter[EN_NETDEV_FLOW_POINT].en = false;
                dev->rfilter[EN_NETDEV_FLOW_POINT].overevent =EN_NETDEVEVENT_POINT_OVER;
                dev->rfilter[EN_NETDEV_FLOW_POINT].lackevent =EN_NETDEVEVENT_POINT_LACK;
                dev->rfilter[EN_NETDEV_FLOW_MULTI].en = false;
                dev->rfilter[EN_NETDEV_FLOW_MULTI].overevent =EN_NETDEVEVENT_MULTI_OVER;
                dev->rfilter[EN_NETDEV_FLOW_MULTI].lackevent =EN_NETDEVEVENT_MULTI_LACK;
                dev->rfilter[EN_NETDEV_FLOW_BROAD].en = false;
                dev->rfilter[EN_NETDEV_FLOW_BROAD].overevent =EN_NETDEVEVENT_BROAD_OVER;
                dev->rfilter[EN_NETDEV_FLOW_BROAD].lackevent =EN_NETDEVEVENT_BROAD_LACK;
                dev->rfilter[EN_NETDEV_FLOW_FRAME].en = false;
                dev->rfilter[EN_NETDEV_FLOW_FRAME].overevent =EN_NETDEVEVENT_FLOW_OVER;
                dev->rfilter[EN_NETDEV_FLOW_FRAME].lackevent =EN_NETDEVEVENT_FLOW_LACK;
                //fill the ops
                dev->linkops = ops;
                //add it to the dev chain
                dev->nxt = pNetDevQ;
                pNetDevQ = dev;
            }
            else
            {
                //no more mem for the dev, so failed
                printf("%s:failed--no mem\r\n",__FUNCTION__);
            }
        }
        else
        {
             //there has been a dev has the same name, so failed
            printf("%s:failed--corrupt name\r\n",__FUNCTION__);
        }
        Lock_MutexPost(pRoutMutex);
    }
    return (ptu32_t)dev;
}
// =============================================================================
// FUNCTION   :remove the name specified net device
// PARAMS IN  :name, the net device name,which specified by installation
// PARAMS OUT :NULL
// RETURN     :true success while false failed
// DESCRIPTION:
// =============================================================================
bool_t  NetDevUninstall(const char *name)
{
    tagNetDev  *dev = NULL;
    tagNetDev  *tmp;
    tagNetDev  *bak;
    tagRout    *rout;
    bool_t      result = false;

    if(Lock_MutexPend(pRoutMutex, CN_TIMEOUT_FOREVER))
    {
        tmp = pNetDevQ;
        bak = tmp;

        while(NULL != tmp)
        {
            if(0 == strcmp(tmp->name,name))
            {
            	dev = tmp;
                //bingo,we got it now
                //remove it from the dev chain
                if(tmp == pNetDevQ)
                {
                    //the head one
                    pNetDevQ = tmp->nxt;
                }
                else
                {
                    bak->nxt = tmp->nxt;
                }
                //free all the rout bind to the dev
                rout = dev->routq;
                while(NULL != rout)
                {
                    if(rout == pRoutDefaultV4)
                    {
                        pRoutDefaultV4 = NULL;
                    }
                    if(rout == pRoutLoop)
                    {
                    	pRoutLoop = NULL;
                    }

                    dev->routq = rout->nxt;
                    free((void *)rout);
                    rout = dev->routq;
                }
                //free the device memory now
                free((void *)tmp);
                result = true;
                break;
            }
            else
            {
                bak = tmp;
                tmp = tmp->nxt;
            }
        }
        Lock_MutexPost(pRoutMutex);
    }
    return result;
}
// =============================================================================
// FUNCTION   :register a hook to do the device event here
// PARAMS IN  :handle:the device handle returned by NetDevInstall
//			   devname:the name of the net device that has been installed
//			   hook:which will used to deal the net dev event
// PARAMS OUT :NULL
// RETURN     :true success while false failed
// DESCRIPTION:if handle NULL,then use the devname to search the net device
// =============================================================================
bool_t NetDevRegisterEventHook(ptu32_t handle,const char *devname,fnNetDevEventHook hook)
{
	bool_t result = false;
    tagNetDev *dev = NULL;

    if(Lock_MutexPend(pRoutMutex, CN_TIMEOUT_FOREVER))
    {
    	if(NULL != (void *)handle)
    	{
    		dev =(tagNetDev *)handle;
    	}
    	else
    	{
    		dev = __NetDevGet(devname);
    	}
    	if(NULL != dev)
    	{
    		dev->eventhook = hook;
    		result = true;
    	}
        Lock_MutexPost(pRoutMutex);
    }
    return result;
}
// =============================================================================
// FUNCTION   :Unregister the event hook  of the device
// PARAMS IN  :handle:the device handle returned by NetDevInstall
//			   devname:the name of the net device that has been installed
// PARAMS OUT :NULL
// RETURN     :true success while false failed
// DESCRIPTION:if handle NULL,then use the devname to search the net device
// =============================================================================
bool_t NetDevUnRegisterEventHook(ptu32_t handle,const char *devname)
{
	bool_t result = false;
    tagNetDev *dev = NULL;

    if(Lock_MutexPend(pRoutMutex, CN_TIMEOUT_FOREVER))
    {
    	if(NULL !=(void *) handle)
    	{
    		dev =(tagNetDev *)handle;
    	}
    	else
    	{
    		dev = __NetDevGet(devname);
    	}
    	if(NULL != dev)
    	{
    		dev->eventhook = NULL;
    		result = true;
    	}
        Lock_MutexPost(pRoutMutex);
    }
    return result;
}
// =============================================================================
// FUNCTION   :used to post a event to the net device
// PARAMS IN  :handle:the device handle returned by NetDevInstall
//			   devname:the name of the net device that has been installed
//			   event:the event to post
// PARAMS OUT :NULL
// RETURN     :true success while false failed
// DESCRIPTION:if handle NULL,then use the devname to search the net device
// =============================================================================
bool_t NetDevPostEvent(ptu32_t handle,const char *devname,enNetDevEvent event)
{
	bool_t result = false;
    tagNetDev *dev = NULL;
    fnNetDevEventHook hook= NULL;
    if(Lock_MutexPend(pRoutMutex, CN_TIMEOUT_FOREVER))
    {
    	if(NULL != (void *)handle)
    	{
    		dev =(tagNetDev *)handle;
    	}
    	else
    	{
    		dev = __NetDevGet(devname);
    	}
    	if(NULL != dev)
    	{
    		hook =dev->eventhook;
    	}
        Lock_MutexPost(pRoutMutex);
        if(NULL != hook)
        {
        	result = hook((ptu32_t)dev,event);
        }
    }
    return result;
}
// =============================================================================
// FUNCTION   :ctrl the specified netdev
// PARAMS IN  :name, the net device name,which specified by installation
//             cmd, the ctrl cmd. which defined by tagNetDevCmd
//             para,used by the cmd, and its type defined by cmd
// PARAMS OUT :NULL
// RETURN     :true success while false failed
// DESCRIPTION:
// =============================================================================
bool_t  NetDevCtrl(const char *name,enNetDevCmd cmd, ptu32_t para)
{
    bool_t      result = false;
    tagNetDev  *dev;

    dev = NetDevGet((const char *)name);
    if((NULL !=dev)&&(NULL != dev->ifctrl))
    {
        result = dev->ifctrl((ptu32_t)dev,cmd,para);
        if(result &&(cmd == EN_NETDEV_SETMAC))
        {
            memcpy(dev->mac,(u8 *)para,CN_MACADDR_LEN);
            //we also need to send arp  response message to update
            ArpInform(dev->name);
        }
    }
    else
    {
    	printf("%s:no dev matched or no ctrl function\n\r",__FUNCTION__);
    }
    return result;
}
//if you got the handle when install the net device, please use the handle
//it maybe more fast
bool_t  NetDevCtrlByHandle(ptu32_t handle,enNetDevCmd cmd, ptu32_t para)
{
    bool_t      result = false;
    tagNetDev  *dev;

    dev = (tagNetDev *)handle;
    if((NULL !=dev)&&(NULL != dev->ifctrl))
    {
        result = dev->ifctrl((ptu32_t)dev,cmd,para);
        if(result &&(cmd == EN_NETDEV_SETMAC))
        {
            memcpy(dev->mac,(u8 *)para,CN_MACADDR_LEN);
            //we also need to send arp  response message to update
            ArpInform(dev->name);
        }
    }
    return result;
}
// =============================================================================
// FUNCTION   :use this function to config the net device filter
// PARAMS IN  :
//             devname :the net device name
//             type    :the frame type, which defines by enNetDevFlowType
//             uflimit :the upper limit of the frame
//             lflimit :the lower limit of the frame
//             measuretime:during the time, if the flow is over, then triggle the corresponding event
//             enable :1 true while 0 false
// PARAMS OUT :
// RETURN     :true success while false failed
// DESCRIPTION:
// =============================================================================
bool_t NetDevFlowSet(const char *devname,enNetDevFlowType type,\
		             u32 llimit,u32 ulimit,u32 period,int enable)
{
	bool_t result = false;
    tagNetDev *dev = NULL;
	tagNetDevRcvFilter *filter;

    if(Lock_MutexPend(pRoutMutex, CN_TIMEOUT_FOREVER))
    {
		dev = __NetDevGet(devname);
    	if(NULL != dev)
    	{
    		filter = &dev->rfilter[type];
    		filter->period = period;
    		filter->fulimit =ulimit;
    		filter->fllimit =llimit;
    		filter->fcounter = 0;
    		filter->deadtime = DjyGetSysTime() + period;
    		filter->en = enable?1:0;
    		result = true;
    	}
        Lock_MutexPost(pRoutMutex);
    }
    return result;
}

// =============================================================================
// FUNCTION   :use this function to check the frame type
// PARAMS IN  :buf, the frame buffer
// PARAMS OUT :
// RETURN     :enNetDevFramType,which type of the frame
// DESCRIPTION:
// =============================================================================
enNetDevFramType NetDevFrameType(u8 *buf,u16 len)
{
	enNetDevFramType result = EN_NETDEV_FRAME_LAST;
	if((NULL != buf)&&(len > CN_MACADDR_LEN))
	{
		if(0 == memcmp(buf,CN_MAC_BROAD,CN_MACADDR_LEN))
		{
			result = EN_NETDEV_FRAME_BROAD;
		}
		else if(buf[0]&0x01)
		{
			result = EN_NETDEV_FRAME_MULTI;
		}
		else
		{
			result = EN_NETDEV_FRAME_POINT;
		}
	}
	return result;
}

// =============================================================================
// FUNCTION   :use this function to config the net device filter
// PARAMS IN  :
//             handle:the device handle,returned by NetDevInstall
//             type  :the frame type has received
// PARAMS OUT :
// RETURN     :true success while false failed
// DESCRIPTION:
// =============================================================================
//static void __NetDevFlowCheck(ptu32_t handle,tagNetDevRcvFilter *filter,s64 timenow)
//{
//	if(filter->enable)
//	{
//		if(timenow > filter->deadtime) //should check if the upper limit meets
//		{
//			//check the upper limit
//			if(filter->fulimit > filter->fcounter)
//			{
//				if(filter->uaction)
//				{
//					NetDevPostEvent(handle,NULL,filter->uactione);//post a gone message
//					filter->uaction = 0;
//				}
//			}
//			//check the lower limit
//			if(filter->fcounter < filter->fllimit)
//			{
//				if(!filter->laction)
//				{
//					filter->laction = 1;
//					NetDevPostEvent(handle,NULL,filter->lactionb);//post a begin message
//				}
//			}
//			filter->fcounter = 0;
//			filter->deadtime = timenow + filter->period;
//		}
//		else
//		{
//			//check the upper limit
//			if(filter->fulimit < filter->fcounter)
//			{
//				if(!filter->uaction)
//				{
//					filter->uaction = 1;
//					NetDevPostEvent(handle,NULL,filter->uactionb);//post a begin message
//				}
//			}
//			//check the lower limit
//			if(filter->fcounter > filter->fllimit)
//			{
//				if(filter->laction)
//				{
//					filter->laction = 0;
//					NetDevPostEvent(handle,NULL,filter->lactione);//post a end message
//				}
//			}
//		}
//	}
//}
//bool_t NetDevFlowCounter(ptu32_t handle,enNetDevFramType type)
//{
//	bool_t result = false;
//    tagNetDev *dev = NULL;
//	tagNetDevRcvFilter *filter;
//	s64 timenow;
//	u32 looptype;
//	if(NULL != (void *)handle)
//	{
//		dev = (tagNetDev *)handle;
//		if(type < EN_NETDEV_FRAME_LAST)
//		{
//			filter = &dev->rfilter[type];
//			if(filter->enable)
//			{
//				filter->fcounter ++;
//				filter->ftotal++;
//			}
//			//any data will inc the frame filter
//			if(type != EN_NETDEV_FLOW_FRAME)
//			{
//				filter = &dev->rfilter[EN_NETDEV_FLOW_FRAME];
//				if(filter->enable)
//				{
//					filter->fcounter ++;
//					filter->ftotal++;
//				}
//			}
//		}
//		//check all the filter
//		timenow = DjyGetSysTime();
//		for(looptype =0;looptype < EN_NETDEV_FLOW_LAST;looptype++)
//		{
//			filter = &dev->rfilter[looptype];
//			__NetDevFlowCheck(handle,filter,timenow);
//		}
//	}
//	return result;
//}
//THE OLD ONE IS NOT CORRECT, ONLY THE FRAME BETWEEN THE UPPER AND FLOOR LIMIT IS PROPER
static void __NetDevFlowCheck(ptu32_t handle,tagNetDevRcvFilter *filter,s64 timenow)
{
	if(filter->en)
	{
		if(timenow > filter->deadtime) //should check if the floor limit is reached
		{
			//check the upper limit
			if(filter->fllimit > filter->fcounter)
			{
				NetDevPostEvent(handle,NULL,filter->lackevent);//post a lack message
				filter->lackaction = true;
				filter->actiontimes++;
			}
			else
			{
				filter->lackaction = false;
			}
			filter->overaction = false;
			filter->fcounter = 0;
			filter->deadtime = timenow + filter->period;
		}
		else
		{
			//check the upper limit
			if(filter->fulimit < filter->fcounter)
			{
				filter->actiontimes++;
				filter->fcounter = 0;
				filter->overaction = true;
				NetDevPostEvent(handle,NULL,filter->overevent);//post a begin message
			}
		}
	}
}
bool_t NetDevFlowCounter(ptu32_t handle,enNetDevFramType type)
{
	bool_t result = false;
    tagNetDev *dev = NULL;
	tagNetDevRcvFilter *filter;
	s64 timenow;
	u32 looptype;
	if(NULL != (void *)handle)
	{
		dev = (tagNetDev *)handle;
		if(type < EN_NETDEV_FRAME_LAST)
		{
			filter = &dev->rfilter[type];
			if(filter->en)
			{
				filter->fcounter ++;
				filter->ftotal++;
			}
			//any data will inc the frame filter
			if(type != EN_NETDEV_FLOW_FRAME)
			{
				filter = &dev->rfilter[EN_NETDEV_FLOW_FRAME];
				if(filter->en)
				{
					filter->fcounter ++;
					filter->ftotal++;
				}
			}
		}
		//check all the filter
		timenow = DjyGetSysTime();
		for(looptype =0;looptype < EN_NETDEV_FLOW_LAST;looptype++)
		{
			filter = &dev->rfilter[looptype];
			__NetDevFlowCheck(handle,filter,timenow);
		}
	}
	return result;
}


const char *pFilterItemName[EN_NETDEV_FLOW_LAST]=
{
	"BROAD",
	"POINT",
	"MULTI",
	"FRAME",
};
//we use this function to show the net device filter state
static bool_t NetDevFlowStat(char *name)
{
    bool_t      result = false;
    tagNetDev  *dev;
	tagNetDevRcvFilter *filter;
	u32                 type;

    dev = NetDevGet((const char *)name);
    if(NULL != dev)
    {
    	printf("NETDEVFLOW:UNIT FOR CYCLE AND DEADLINE IS US\n\r");
    	printf("%-6s%-3s%-3s%-3s%-9s%-9s%-9s%-9s%-9s%-9s%-9s\n\r",\
    			"Name","EN","LS","US","FC","FU","FL","ACTIONS","CYCLE","DEADLINE","FT");
    	for(type =0; type < EN_NETDEV_FLOW_LAST;type++)
    	{
    		filter = &dev->rfilter[type];
    		printf("%-6s%-3s%-3s%-3s%-8x %-8x %-8x %-8x %-8x %-8llx %-8llx\n\r",\
    				pFilterItemName[type],\
					filter->en==true?"Y":"N",\
					filter->lackaction?"Y":"N",\
					filter->overaction?"Y":"N",\
					filter->fcounter,\
					filter->fulimit,\
					filter->fllimit,\
					filter->actiontimes,\
					filter->period,\
					filter->deadtime,\
					filter->ftotal
					);
    	}
    	result =true;
    }
    return result;
}
// =============================================================================
// FUNCTION   :get the netdev private member
// PARAMS IN  :handle, the install returns handle
// PARAMS OUT :NULL
// RETURN     :the private data you installed
// DESCRIPTION:
// =============================================================================
bool_t NetDevPrivate(ptu32_t handle)
{
    ptu32_t result;
    result = ((tagNetDev*)handle)->private;
    return result;
}
// =============================================================================
// FUNCTION:Use this function to update the net rout malloced by the dhcp client
// PARA  IN:name, the net dev name
//          ver,you must specified the ip version
//          netaddr, a pointer to tagHostAddrV4 if ver is EN_IP_V4 else tagHostAddrV6
// PARA OUT:
// RETURN  :the corresponding rout
// INSTRUCT:
// =============================================================================
bool_t RoutUpdate(char *name,enum_ipv_t ver,void *netaddr)
{
    bool_t     result = false;
    tagNetDev *dev = NULL;
    tagRout   *rout;

    if(Lock_MutexPend(pRoutMutex, CN_TIMEOUT_FOREVER))
    {
        dev = __NetDevGet(name);
        rout = dev->routq;
        while(NULL != rout)
        {
            if(rout->pro & CN_ROUT_DHCP)
            {
                if((EN_IPV_4 == ver)&&(rout->ver == ver))
                {
                    rout->ipaddr.ipv4 = *(tagHostAddrV4 *)netaddr;
                    result = true;
                }
                break;
            }
            else
            {
            	rout = rout->nxt;
            }
        }
        Lock_MutexPost(pRoutMutex);
    }

    return result;
}

// =============================================================================
// FUNCTION:Use this function to create an rout
// PARA  IN:name, the net dev name
//          ver,you must specified the ip version
//          netaddr, a pointer to tagHostAddrV4 if ver is EN_IP_V4 else tagHostAddrV6
// PARA OUT:
// RETURN  :the corresponding rout
// INSTRUCT:
// =============================================================================
bool_t RoutCreate(const char *name,enum_ipv_t ver,void *netaddr,u32 pro)
{
    bool_t     result = false;
    tagNetDev *dev   = NULL;
    tagRout   *rout  = NULL;
    //we find the dev and check if any rout in the dev has the same addr
    if(Lock_MutexPend(pRoutMutex, CN_TIMEOUT_FOREVER))
    {
        dev = __NetDevGet(name);
        if(NULL != dev)
        {
            rout = dev->routq;
            while(NULL != rout)
            {
                if(rout->ver == ver)
                {
                    if((ver == rout->ver)&&(ver == EN_IPV_4)&&\
                       (rout->ipaddr.ipv4.ip == ((tagHostAddrV4 *)netaddr)->ip))
                    {
                        break;
                    }
                    else
                    {
                        rout = rout->nxt;
                    }
                }
                else
                {
                    rout = rout->nxt;
                }
            }
            if(NULL == rout)
            {
                //no rout matched. so we could add the new rout
                rout = (tagRout *) malloc(sizeof(tagRout));
                if((NULL != rout)&&(ver == EN_IPV_4))
                {
                	memset(rout,0,sizeof(tagRout));
                    //fill the net address and add it to the dev rout chain
                    rout->ver = EN_IPV_4;
                    rout->ipaddr.ipv4 = *(tagHostAddrV4 *)netaddr;
                    rout->pro = pro;
                    rout->func = dev->devfunc;
                    //add to the dew rout chain
                    rout->dev = dev;
                    rout->nxt = dev->routq;
                    dev->routq = rout;
                    if(rout->ipaddr.ipv4.ip == htonl(INADDR_LOOPBACK))
                    {
                        pRoutLoop = rout;   //this is the loop rout
                    }
                    result = true;
                }
            }
        }
        Lock_MutexPost(pRoutMutex);
    }
    return result;
}
// =============================================================================
// FUNCTION:please use this function to delete the rout
// PARA  IN:name. the net dev name
//          ver,you must specified the ip version
//          netaddr, a pointer to tagHostAddrV4 if ver is EN_IP_V4 else tagHostAddrV6
// PARA OUT:
// RETURN  :the corresponding rout
// INSTRUCT:
// =============================================================================
bool_t RoutDelete(const char *name,enum_ipv_t ver,ipaddr_t addr)
{
    bool_t     result = false;
    tagNetDev *dev   = NULL;
    tagRout   *rout  = NULL;
    tagRout   *pre   = NULL;

    //we find the dev and check if any rout in the dev has the same addr
    if(Lock_MutexPend(pRoutMutex, CN_TIMEOUT_FOREVER))
    {
        dev = __NetDevGet(name);
        if(NULL != dev)
        {
            rout = dev->routq;
            pre = rout;
            while(NULL != rout)
            {
                if((ver == rout->ver)&&(ver == EN_IPV_4)&&\
                   (rout->ipaddr.ipv4.ip == addr))
                {
                    //find the one, remove it from the chain and free it
                    if(rout == dev->routq)
                    {
                        dev->routq = rout->nxt;
                    }
                    else
                    {
                        pre->nxt = rout->nxt;
                    }
                    if(rout == pRoutDefaultV4)
                    {
                        pRoutDefaultV4 = NULL;
                    }
                    if(rout == pRoutLoop)
                    {
                    	pRoutLoop = NULL;
                    }
                    free((void *)rout);
                    break;
                }
                else
                {
                    pre = rout;
                    rout = rout->nxt;
                }
            }
        }
        Lock_MutexPost(pRoutMutex);
    }
    return result;
}
// =============================================================================
// FUNCTION:Use this function to modify the existed rout
// PARA  IN:name, the net dev name
//          ver,you must specified the ip version
//          netaddr, a pointer to tagHostAddrV4 if ver is EN_IP_V4 else tagHostAddrV6
// PARA OUT:
// RETURN  :the corresponding rout
// INSTRUCT:
// =============================================================================
bool_t RoutSet(const char *name,enum_ipv_t ver,ipaddr_t ipold,void *newaddr)
{
    bool_t     result = false;
    tagNetDev *dev   = NULL;
    tagRout   *rout  = NULL;
    tagHostAddrV4  *newhost;
    //we find the dev and check if any rout in the dev has the same addr
    if(Lock_MutexPend(pRoutMutex, CN_TIMEOUT_FOREVER))
    {
        dev = __NetDevGet(name);
        if(NULL != dev)
        {
            rout = dev->routq;
            while(NULL != rout)
            {
                if((rout->ver == ver)&&(ver == EN_IPV_4)&&\
                   (rout->ipaddr.ipv4.ip == ipold))
                {
                    break;
                }
                else
                {
                    rout = rout->nxt;
                }
            }
            if(NULL != rout)
            {
                //no rout matched. so we could add the new rout
                //fill the net address
                if(ver == EN_IPV_4)
                {
                    rout->ver = EN_IPV_4;
                    newhost = (tagHostAddrV4 *)newaddr;
                    if(newhost->broad != INADDR_ANY)
                    {
                        rout->ipaddr.ipv4.broad = newhost->broad;
                    }
                    if(newhost->dns != INADDR_ANY)
                    {
                        rout->ipaddr.ipv4.dns = newhost->dns;
                    }
                    if(newhost->gatway != INADDR_ANY)
                    {
                        rout->ipaddr.ipv4.gatway = newhost->gatway;
                    }
                    if(newhost->submask != INADDR_ANY)
                    {
                        rout->ipaddr.ipv4.submask = newhost->submask;
                    }
                    if(newhost->ip != INADDR_ANY)
                    {
                        rout->ipaddr.ipv4.ip = newhost->ip;
                    }
                    result = true;
                }
            }
        }
        Lock_MutexPost(pRoutMutex);
    }
    return result;
}
// =============================================================================
// FUNCTION:use this function to set the default rout
// PARA  IN: ip, the specified ip of the rout,if INADDR_ANY, then use the dynamic one
// PARA OUT:
// RETURN  :
// INSTRUCT:
// =============================================================================
bool_t RoutSetDefault(enum_ipv_t ver,ipaddr_t ip)
{
    bool_t     result = false;
    tagNetDev *dev   = NULL;
    tagRout   *rout  = NULL;
    //we find the dev and check if any rout in the dev has the same addr
    if(Lock_MutexPend(pRoutMutex, CN_TIMEOUT_FOREVER))
    {
        dev = __NetDevGet(NULL);
        while(NULL != dev)
        {
            rout = dev->routq;
            while(NULL != rout)
            {
                if((rout->ver == EN_IPV_4)&&(rout->ver == ver))
                {
                    if((ip == INADDR_ANY)&&(rout->pro&CN_ROUT_DHCP))
                    {
                        pRoutDefaultV4 = rout;
                        result = true;
                        rout = NULL;
                        dev = NULL;
                    }
                    else if(rout->ipaddr.ipv4.ip == ip)
                    {
                        pRoutDefaultV4 = rout;
                        result = true;
                        rout = NULL;
                        dev = NULL;  //stop the search
                    }
                    else
                    {
                        rout = rout->nxt;
                    }
                }
                else
                {
                    rout = rout->nxt;
                }
            }
            if(NULL != dev)
            {
                dev = dev->nxt;
            }
        }
        Lock_MutexPost(pRoutMutex);
    }
    return result;
}
// =============================================================================
// FUNCTION:use this function to get the default dns ip
// PARA  IN: ver,you must specified the ip version
// PARA OUT:ip, the specified ip buf
// RETURN  :the ip class
// INSTRUCT:
// =============================================================================
bool_t RoutDns(enum_ipv_t ver, ipaddr_t ip)
{
    bool_t result;
    if((ver == EN_IPV_4)&&(NULL != (u32 *)ip) &&(NULL != pRoutDefaultV4))
    {
        *(u32 *)ip = pRoutDefaultV4->ipaddr.ipv4.dns;
        result = true;
    }
    else
    {
        result = false;
    }

    return result;
}

//add some rout debug function
//for the debug
static void __RoutShow(tagNetDev *dev)
{
    tagRout       *rout;
    struct in_addr       addr;

    printf("***************************************************************\r\n");
    printf("Device:Name      :%s---LinkType:%s(%d)\r\n",dev->name,\
    		LinkTypeName(dev->iftype),dev->iftype);
    printf("Device:Mac       :%02x-%02x-%02x-%02x-%02x-%02x\r\n",\
            dev->mac[0],dev->mac[1],dev->mac[2],\
            dev->mac[3],dev->mac[4],dev->mac[5]);
    printf("Device:Property  :MTU= %d  Function=0x%08x\r\n",\
            dev->mtu,dev->devfunc);
    printf("Device:STAT      :snd = %d  snderr=%d\r\n",dev->fsnd,dev->fsnderr);
    printf("Device:STAT      :rcv = %d  rcverr=%d\r\n",dev->frcv,dev->frcverr);

    rout = dev->routq;
    while(NULL != rout)
    {
        //print net addr
        if(rout->ver == EN_IPV_4)
        {
            memcpy((void *)&addr,(void *)&rout->ipaddr.ipv4.ip,sizeof(addr));
            printf("IPV4  :ip        :%s(%s)\r\n",inet_ntoa(addr),\
                    rout->pro&CN_ROUT_DHCP?"dynamic":"static");
            memcpy((void *)&addr,(void *)&rout->ipaddr.ipv4.submask,sizeof(addr));
            printf("IPV4  :submask   :%s\r\n",inet_ntoa(addr));
            memcpy((void *)&addr,(void *)&rout->ipaddr.ipv4.gatway,sizeof(addr));
            printf("IPV4  :gateway   :%s\r\n",inet_ntoa(addr));
            memcpy((void *)&addr,(void *)&rout->ipaddr.ipv4.broad,sizeof(addr));
            printf("IPV4  :broad     :%s\r\n",inet_ntoa(addr));
            memcpy((void *)&addr,(void *)&rout->ipaddr.ipv4.dns,sizeof(addr));
            printf("IPV4  :dns       :%s\r\n",inet_ntoa(addr));
        }
        else
        {
            printf("Unknow Host ADDR\r\n");
        }

        printf("RoutSndNum:Total:0x%llx Err:0x%llx\n\r",rout->outnum,rout->outerr);
        printf("RoutRcvNum:Total:0x%llx Err:0x%llx\n\r",rout->innum,rout->inerr);

        rout = rout->nxt;
    }
}


//show or config the rout
//0 -devname
//1 -oldip
//2 -newip
//3 -subnetmask
//4 -gatway
//5 -broad
//6 -dns
static bool_t RoutIfConfig(char *param)
{
    bool_t result;
    int   argc =7;
    char *argv[7];
    tagNetDev  *dev;

    tagHostAddrV4  hostaddr;
    ipaddr_t       ipold;

    result = false;
    string2arg(&argc,argv,param);

    hostaddr.broad = INADDR_ANY;
    hostaddr.dns = INADDR_ANY;
    hostaddr.gatway = INADDR_ANY;
    hostaddr.submask = INADDR_ANY;
    hostaddr.ip = INADDR_ANY;

    switch(argc)
    {
        case 7:
            hostaddr.dns = inet_addr(argv[6]);
            hostaddr.broad = inet_addr(argv[5]);
            hostaddr.gatway = inet_addr(argv[4]);
            hostaddr.submask = inet_addr(argv[3]);
            hostaddr.ip = inet_addr(argv[2]);
            ipold = inet_addr(argv[1]);
            result = RoutSet(argv[0],EN_IPV_4,ipold,&hostaddr);
            break;
        case 6:
            hostaddr.broad = inet_addr(argv[5]);
            hostaddr.gatway = inet_addr(argv[4]);
            hostaddr.submask = inet_addr(argv[3]);
            hostaddr.ip = inet_addr(argv[2]);
            ipold = inet_addr(argv[1]);
            result = RoutSet(argv[0],EN_IPV_4,ipold,&hostaddr);
            break;
        case 5:
            hostaddr.gatway = inet_addr(argv[4]);
            hostaddr.submask = inet_addr(argv[3]);
            hostaddr.ip = inet_addr(argv[2]);
            ipold = inet_addr(argv[1]);
            result = RoutSet(argv[0],EN_IPV_4,ipold,&hostaddr);
            break;
        case 4:
            hostaddr.submask = inet_addr(argv[3]);
            hostaddr.ip = inet_addr(argv[2]);
            ipold = inet_addr(argv[1]);
            result = RoutSet(argv[0],EN_IPV_4,ipold,&hostaddr);
            break;
        case 3:
            hostaddr.ip = inet_addr(argv[2]);
            ipold = inet_addr(argv[1]);
            result = RoutSet(argv[0],EN_IPV_4,ipold,&hostaddr);
            break;
        case 1:
            dev = NetDevGet((const char *)argv[0]);
            if(NULL != dev)
            {
                __RoutShow(dev);
                result = true;
            }
            break;
        case 0:
            dev = NetDevGet(NULL);
            while(NULL != dev)
            {
                __RoutShow(dev);
                dev = dev->nxt;
            }
            result = true;
            break;
        default:
            break;
    }

    return result;
}

//use this function to delete the net dev
static bool_t  NetDevRemove(char *param)
{
    bool_t result;

    result = NetDevUninstall(param);

    return result;
}
//use this function to set the netdev mac
static bool_t NetDevMacSet(char *param)
{
    u8   mac[CN_MACADDR_LEN];
    int  argc = 2;
    char *argv[2];
    bool_t result;

    result = false;
    string2arg(&argc,argv,param);
    if(argc == 2)
    {
        if(string2Mac(argv[1],mac))
        {
            result = NetDevCtrl(argv[0],EN_NETDEV_SETMAC,(ptu32_t)mac);
        }
    }
    return result;
}

//use this function to find a rout to output the ip package
//-----------------------------------------------------------------------------
//����:use this function to get the default rout
//����:ver,which means ipv4 or ipv6
//����:
//��ע:if no rout match the destination then we could use the default rout to send the package
//����:zhangqf@����5:48:47/2016��12��28��
//-----------------------------------------------------------------------------
tagRout *RoutGetDefault(enum_ipv_t ver)
{
	tagRout *result = NULL;
	if(ver == EN_IPV_4)
	{
		result = pRoutDefaultV4;
	}
	return result;
}

bool_t RoutSetDefaultAddr(enum_ipv_t ver,ipaddr_t ip,ipaddr_t mask,ipaddr_t gateway,ipaddr_t dns)
{
	bool_t result = false;
	if((NULL != pRoutDefaultV4)&&(ver = EN_IPV_4))
	{
		pRoutDefaultV4->ipaddr.ipv4.ip = ip;
		pRoutDefaultV4->ipaddr.ipv4.submask = mask;
		pRoutDefaultV4->ipaddr.ipv4.gatway = gateway;
		pRoutDefaultV4->ipaddr.ipv4.dns = dns;
		pRoutDefaultV4->ipaddr.ipv4.broad = (ip&mask)|(~mask);

		result = true;
	}
	return result;
}

//-----------------------------------------------------------------------------
//����:use this function to select a match rout,(same subnet)
//����:ver:means ipv4 or ipv6;ipaddr:which means the ipv4 address or the ipv6 address
//����:
//��ע:
//����:zhangqf@����5:53:32/2016��12��28��
//-----------------------------------------------------------------------------
tagRout *RoutMatch(enum_ipv_t ver,ipaddr_t ipaddr)
{
	tagRout   *result = NULL;
    tagRout   *tmp = NULL;
    tagNetDev *dev = NULL;
    u32        ip;
    if(Lock_MutexPend(pRoutMutex,CN_TIMEOUT_FOREVER))
    {
        if(ver == EN_IPV_4)
        {
        	ip = (u32)ipaddr;
            dev = __NetDevGet(NULL); //get the queue
            while(NULL != dev)       //loop all the dev to find an proper device
            {
                tmp = dev->routq;
                while(NULL != tmp)
                {
                    if(ver == tmp->ver)
                    {
                        if(ip == tmp->ipaddr.ipv4.ip) //host ip use the loop rout
                        {
                            result = pRoutLoop;
                            dev = NULL;   //stop the search
                            break;
                        }
                        else if((ip & tmp->ipaddr.ipv4.submask)==\
                                (tmp->ipaddr.ipv4.ip&tmp->ipaddr.ipv4.submask))
                        {
                            result = tmp;
                            dev = NULL;   //stop the search
                            break;
                        }
                        else
                        {

                        }
                    }
                    tmp = tmp->nxt;
                }
                if(NULL != dev)
                {
                    dev = dev->nxt;
                }
            }
        }
        Lock_MutexPost(pRoutMutex);
    }
    return   result;
}
//-----------------------------------------------------------------------------
//����:use this function to judge whether the ip is the host address
//����:
//����:true when the ip match the INADDR_ANY OR THE ROUT IP else false
//��ע:
//����:zhangqf@����6:07:57/2016��12��28��
//-----------------------------------------------------------------------------
bool_t RoutHostIp(enum_ipv_t ver,ipaddr_t ipaddr)
{
	bool_t     result = false;
	tagRout   *tmp = NULL;
	tagNetDev *dev = NULL;
	u32        ip;
	if(Lock_MutexPend(pRoutMutex,CN_TIMEOUT_FOREVER))
	{
	    if(ver == EN_IPV_4)
	    {
	    	ip = (u32)ipaddr;
	    	if(ip == INADDR_ANY)
	    	{
	    		result = true;
	    	}
	    	else  //check if any ip is match this
	    	{
		        dev = __NetDevGet(NULL); //get the queue
		        while(NULL != dev)       //loop all the dev to find an proper device
		        {
		            tmp = dev->routq;
		            while(NULL != tmp)
		            {
		                if(ver == tmp->ver)
		                {
		                    if(ip == tmp->ipaddr.ipv4.ip) //host ip use the loop rout
		                    {
		                        result = true;
		                        dev = NULL;   //stop the search
		                        break;
		                    }
		                    else
		                    {
		                    	//continue
		                    }
		                }
		                tmp = tmp->nxt;
		            }
		            if(NULL != dev)
		            {
		                dev = dev->nxt;
		            }
		        }
	    	}
	    }
	    Lock_MutexPost(pRoutMutex);
	}
	return   result;
}

//-----------------------------------------------------------------------------
//����:use this function to judge whether the ip is the host address
//����:
//����:true when the ip match the INADDR_BROADCAST OR THE ROUT IP else false
//��ע:
//����:zhangqf@����6:07:57/2016��12��28��
//-----------------------------------------------------------------------------
bool_t RoutHostTarget(enum_ipv_t ver,ipaddr_t ipaddr)
{
	bool_t     result = false;
	tagRout   *tmp = NULL;
	tagNetDev *dev = NULL;
	u32        ip;
	if(Lock_MutexPend(pRoutMutex,CN_TIMEOUT_FOREVER))
	{
	    if(ver == EN_IPV_4)
	    {
	    	ip = (u32)ipaddr;
	    	if(ip == INADDR_BROADCAST)
	    	{
	    		result = true;
	    	}
	    	else  //check if any ip is match this
	    	{
		        dev = __NetDevGet(NULL); //get the queue
		        while(NULL != dev)       //loop all the dev to find an proper device
		        {
		            tmp = dev->routq;
		            while(NULL != tmp)
		            {
		                if(ver == tmp->ver)
		                {
		                    if(ip == tmp->ipaddr.ipv4.ip) //host ip use the loop rout
		                    {
		                        result = true;
		                        dev = NULL;   //stop the search
		                        break;
		                    }
		                    else if(ip == tmp->ipaddr.ipv4.broad)
		                    {
		                        result = true;
		                        dev = NULL;   //stop the search
		                        break;
		                    }
		                    else if(ip == tmp->ipaddr.ipv4.multi)
		                    {
		                        result = true;
		                        dev = NULL;   //stop the search
		                        break;
		                    }
		                    else
		                    {
		                    	//continue
		                    }
		                }
		                tmp = tmp->nxt;
		            }
		            if(NULL != dev)
		            {
		                dev = dev->nxt;
		            }
		        }
	    	}
	    }
	    Lock_MutexPost(pRoutMutex);
	}
	return   result;
}
//-----------------------------------------------------------------------------
//����:use this function to check if the ip is in the subnet of binded to the specified device
//����:
//����:
//��ע:
//����:zhangqf@����2:45:01/2016��12��29��
//-----------------------------------------------------------------------------
bool_t RoutSubNet(tagNetDev *dev,enum_ipv_t ver,ipaddr_t ipaddr)
{
	bool_t     result = false;
	tagRout   *tmp = NULL;
	u32        ip;
	if(Lock_MutexPend(pRoutMutex,CN_TIMEOUT_FOREVER))
	{
	    if(ver == EN_IPV_4)
	    {
	    	ip = (u32)ipaddr;
            tmp = dev->routq;
            while(NULL != tmp)
            {
                if(ver == tmp->ver)
                {
                    if((ip&tmp->ipaddr.ipv4.submask)== \
                    	(tmp->ipaddr.ipv4.ip&tmp->ipaddr.ipv4.submask))
                    {
                        result = true;
                        dev = NULL;   //stop the search
                        break;
                    }
                }
                tmp = tmp->nxt;
            }
	    }
	    Lock_MutexPost(pRoutMutex);
	}
	return   result;
}


struct ShellCmdTab  gRoutDebug[] =
{
    {
        "ifconfig",
        RoutIfConfig,
        "usage:ifconfig [devname] [oldip] [newip] [subnetmask] [gatway] [broad] [dns]",
        "usage:ifconfig [devname] [oldip] [newip] [subnetmask] [gatway] [broad] [dns]",
    },
    {
        "netdevremove",
        NetDevRemove,
        "usage:netdevremove devname",
        "usage:netdevremove devname",
    },
    {
        "netdevmac",
        NetDevMacSet,
        "usage:netdevmac devname  mac0-mac1-mac2-mac3-mac4-mac5",
        "usage:netdevmac devname  mac0-mac1-mac2-mac3-mac4-mac5",
    },
    {
        "netdevflow",
		NetDevFlowStat,
        "usage:netdevflow",
        "usage:netdevflow",
    },
};
#define CN_ROUTDEBUG_NUM  ((sizeof(gRoutDebug))/(sizeof(struct ShellCmdTab)))
static struct ShellCmdRsc gRoutDebugCmdRsc[CN_ROUTDEBUG_NUM];
// =============================================================================
// FUNCTION   :this is the rout module initialize function
// PARAMS IN  :
// PARAMS OUT :
// RETURN     :
// DESCRIPTION:
// =============================================================================
bool_t RoutInit(void)
{
    bool_t result;
    pNetDevQ = NULL;
    pRoutLoop = NULL;
    pRoutDefaultV4 = NULL;

    pRoutMutex = Lock_MutexCreate_s(&gRoutMutexMem,NULL);
    if(NULL == pRoutMutex)
    {
        goto EXIT_MUTEX;
    }
    result = Sh_InstallCmd(gRoutDebug,gRoutDebugCmdRsc,CN_ROUTDEBUG_NUM);
    if(false == result)
    {
        goto EXIT_ROUTCMD;
    }
    result = true;
    return result;

EXIT_ROUTCMD:
    Lock_MutexDelete_s(pRoutMutex);
    pRoutMutex = NULL;
EXIT_MUTEX:
    result = false;
    return result;
}









