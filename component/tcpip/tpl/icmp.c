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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������
// 3. ��������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ��������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
#include <sys/socket.h>

#include "../common/icmp.h"
#include "../common/ipV4.h"
#include "../common/ip.h"

#pragma pack(1)
typedef struct
{
    u8        type;        //type for icmp
    u8        code;        //code for icmp
    u16       chksum;      //pkg chksum
    u8        data[0];     //additional data for specified type and data
}tagIcmpHdr;

typedef struct
{
    u16 taskid;
    u16 seqno;
    u8  data[0];
}tagIcmpHdrEcho;

#pragma pack()

enum _EN_ICMP_TYPE
{
    EN_ICMP_ECHOREPLY = 0,
    EN_ICMP_DSTNOTREACH =3,
    EN_ICMP_SRCSHUTDOWN,
    EN_ICMP_RELOCATE,
    EN_ICMP_ECHOREQUEST =8,
    EN_ICMP_ROUTNOTICE,
    EN_ICMP_ROUTREQUEST,
    EN_ICMP_TIMEOUT,
    EN_ICMP_PARAERR,
    EN_ICMP_TIMEREQUEST,
    EN_ICMP_TIMEREPLY,
    EN_ICMP_ADDRMSKREQUEST = 17,
    EN_ICMP_ADDRMSKREPLY,
};
#define CN_ICMP_ECHOREPLY_CODE           0    //PING REPLY CODE
#define CN_ICMP_ECHOREQUEST_CODE         0    //PING REQUEST CODE


//the data used for request and answer info
typedef struct IcmpTask
{
    struct IcmpTask             *pre;     //pre  lst
    struct IcmpTask             *nxt;     //nxt  lst
    semp_t                       sync;     //when rcv, active it
    //the member used for specifying
    u8        type;                             //icmp type
    u8        code;                             //icmp code
    u8        *data;                            //task data:the data type and code need
}tagIcmpTask;

typedef struct
{
    u16     taskid;
    u16     seqno;
    u32     ipdst;
}tagIcmpTaskEcho;

static tagIcmpTask                *pgIcmpTaskLst;    //icmp queue
static struct MutexLCB         sgIcmpTaskSync;    //icmp queue sync
// =============================================================================
// �������ܣ�__Icmp_TaskAdd
//          add an task to the queue
// ���������task, the task to add
// ���������
// ����ֵ  ��true success while  false failed
// ˵��    :
// =============================================================================
bool_t __Icmp_TaskAdd(tagIcmpTask *task)
{
    task->sync = semp_init(1,0,NULL);
    if(mutex_lock(&sgIcmpTaskSync))
    {
        //create the semp sync
        task->pre = NULL;
        task->nxt = pgIcmpTaskLst;
        if(NULL != pgIcmpTaskLst)
        {
            pgIcmpTaskLst->pre = task;
        }
        pgIcmpTaskLst = task;
        mutex_unlock(&sgIcmpTaskSync);
        return true;
    }
    else
    {
        return false;
    }
}

// =============================================================================
// �������ܣ�__Icmp_TaskDel
//          remove an task from the queue
// ���������task, the task to remove
// ���������
// ����ֵ  ��true success while  false failed
// ˵��    :
// =============================================================================
bool_t __Icmp_TaskDel(tagIcmpTask *task)
{
    if(mutex_lock(&sgIcmpTaskSync))
    {
        if(task == pgIcmpTaskLst)
        {
            task = pgIcmpTaskLst;
            pgIcmpTaskLst = task->nxt;
            if(NULL != pgIcmpTaskLst)
            {
                pgIcmpTaskLst->pre = NULL;
            }
        }
        else
        {
            if(NULL == task->nxt)
            {
                task->pre->nxt = NULL;
            }
            else
            {
                task->pre->nxt = task->nxt;
                task->nxt->pre = task->pre;
            }
        }
        //del the semp sync
        semp_del(task->sync);
        mutex_unlock(&sgIcmpTaskSync);
        return true;
    }
    else
    {
        return false;
    }
}
// =============================================================================
// �������ܣ�__Icmp_TaskEchoActive
// ���������
//          ipsrc:pkg src
//          ipdst:pkg dst
//          type: icmp type
//          code: icmp code
//          pkg:  icmp info pkg
// ���������
// ����ֵ  ��
// ˵��    :
// =============================================================================
void __Icmp_TaskEchoActive(u32 ipsrc, u32 ipdst,u8 type, u8 code,tagNetPkg *pkg)
{
    tagIcmpTask         *tmp;
    tagIcmpTaskEcho     *taskdata;
    tagIcmpHdrEcho      *echodata;

    if(mutex_lock(&sgIcmpTaskSync))
    {
        tmp = pgIcmpTaskLst;
        echodata = (tagIcmpHdrEcho *)(pkg->buf + pkg->offset);
        while(NULL != tmp)
        {
            if((tmp->type == type)&&(tmp->code == code))
            {
                taskdata = (tagIcmpTaskEcho *)tmp->data;
                if((taskdata->ipdst == ipsrc)&&\
                   (taskdata->taskid ==echodata->taskid)&&\
                   (taskdata->seqno == echodata->seqno))
                {
                    semp_post(tmp->sync);
                    break;
                }
            }
            tmp =tmp->nxt;
        }
        mutex_unlock(&sgIcmpTaskSync);
    }
    return;
}
// =============================================================================
// �������ܣ�Icmp_EchoRequest
//          ping request
// ��������� ipdst, the ip of the host we will ping
//           data, additionnal data
//           len, additionnal data len
// ���������
// ����ֵ  ��true , request success while false timeout
// ˵��    :
// =============================================================================
bool_t Icmp_EchoRequest(u32 ipdst, u8 *data, int len,int timeout)
{
    bool_t               result;
    static u16           seqno = 0;
    u16                  pkglen;
    tagNetPkg            *sndpkg;
    tagIcmpHdr           *icmppkg;
    tagIcmpHdrEcho       *icmppkgecho;
    tagIcmpTask          icmptask;
    tagIcmpTaskEcho      icmptaskecho;

    result = false;
    if((NULL != data)&&(len > 0)&&(ipdst != INADDR_ANY))
    {
        pkglen = sizeof(tagIcmpHdr)+sizeof(tagIcmpHdrEcho)+len;
        sndpkg = PkgMalloc(pkglen,CN_PKLGLST_END);
        if(NULL != sndpkg)
        {
            seqno++;
            sndpkg->datalen = pkglen;
            sndpkg->partnext = NULL;
            //fill the snd pkg
            icmppkg = (tagIcmpHdr *)(sndpkg->buf + sndpkg->offset);
            icmppkg->type = EN_ICMP_ECHOREQUEST;
            icmppkg->code = CN_ICMP_ECHOREQUEST_CODE;
            icmppkg->chksum = 0;
            icmppkgecho = (tagIcmpHdrEcho *)(&icmppkg->data[0]);
            icmppkgecho->seqno = htons(seqno);
            icmppkgecho->taskid = htons(Djy_MyEvttId());
            memcpy(&icmppkgecho->data[0], data, len);
            //combin the task
            icmptaskecho.ipdst = ipdst;
            icmptaskecho.seqno = icmppkgecho->seqno;
            icmptaskecho.taskid = icmppkgecho->taskid;
            icmptask.type = EN_ICMP_ECHOREPLY;       //we expect the reply
            icmptask.code = CN_ICMP_ECHOREPLY_CODE;
            icmptask.data = (u8 *)&icmptaskecho;
            __Icmp_TaskAdd(&icmptask);
            //ok, the pkg has been commpletted. now we snd it
            IpSend(EN_IPV_4,(ptu32_t)INADDR_ANY,(ptu32_t)ipdst,sndpkg,pkglen,IPPROTO_ICMP,\
                    CN_IPDEV_ICMPOCHKSUM,&icmppkg->chksum);
            PkgTryFreePart(sndpkg);
            //just wait for the echo, if false, it must be timeout
            result = semp_pendtimeout(icmptask.sync, timeout);
            __Icmp_TaskDel(&icmptask);
        }
    }

    return result;
}

// =============================================================================
// �������ܣ�Icmp_MsgSnd
//          snd icmp msg
// ��������� ipsrc, the ip of the msg src
//        ipdst, the ip of the  msg dst
//        type, icmp type
//        code, icmp code
//        info, the info buf ptr
//        infolen, the len of the info
// ���������
// ����ֵ  ��true , snd success while false failed
// ˵��    :
// =============================================================================
bool_t Icmp_MsgSnd(u32 ipsrc, u32 ipdst, u8 type, u8 code, u8 *info, u16 infolen)
{
    bool_t            result;
    u16               pkglen;
    tagNetPkg         *pkg2snd;
    tagIcmpHdr        *hdr;
    u8                *src;
    u8                *dst;
    result = false;
    pkglen = sizeof(tagIcmpHdr)+infolen;
    pkg2snd = PkgMalloc(pkglen,CN_PKLGLST_END);
    if(NULL != pkg2snd)
    {
        pkg2snd->datalen = pkglen;
        pkg2snd->partnext = NULL;

        hdr = (tagIcmpHdr *)(pkg2snd->buf + pkg2snd->offset);
        hdr->type = type;
        hdr->code = code;
        hdr->chksum = 0;
        src = info;
        dst = &hdr->data[0];
        memcpy(dst, src, infolen);

        result = IpSend(EN_IPV_4,(ptu32_t)ipdst,(ptu32_t)ipsrc,pkg2snd,pkg2snd->datalen,IPPROTO_ICMP,\
                CN_IPDEV_ICMPOCHKSUM,&hdr->chksum);
        PkgTryFreePart(pkg2snd);
    }

    return result;
}

// =============================================================================
// �������ܣ�__Icmp_EchoReply
//          deal the echo reply pkg
// ���������ipsrc, pkg source host
//          ipdst, pkg dst host
//          type, icmp type
//          code, icmp code
//          pkg, info for the specified type and code
// ���������
// ����ֵ  ��true succees while false failed
// ˵��    :Find any task in the queue,if any the set it,otherwise do nothing
//       anyway, net_free the pkg in
// =============================================================================
bool_t __Icmp_EchoReply(u32 ipsrc, u32 ipdst,tagIcmpHdr *hdr,tagNetPkg *pkg)
{
    u8 type;
    u8 code;

    type = hdr->type;
    code = hdr->code;

    __Icmp_TaskEchoActive(ipsrc, ipdst, type, code, pkg);
    return true;
}
// =============================================================================
// �������ܣ�__Icmp_EchoRequest
//          deal the icmp request pkg
// ���������ipsrc, pkg source host
//          ipdst, pkg dst host
//          type, icmp type
//          code, icmp code
//          pkg, info for the specified type and code
// ���������
// ����ֵ  ��true , request success while false timeout
// ˵��    :--todo, CPY PROCESS MAYBE TOO OLD
// =============================================================================
bool_t __Icmp_EchoRequest(u32 ipsrc, u32 ipdst,tagIcmpHdr *hdr,tagNetPkg *pkglst)
{
    bool_t              result;
    u16                 pkglen;
    tagNetPkg           *pkg;
    tagNetPkg           *pkg2snd;
    u8                  *dst;
    u8                  *src;

    hdr->type = EN_ICMP_ECHOREPLY;
    hdr->code = CN_ICMP_ECHOREPLY_CODE;
    hdr->chksum = 0;
    pkglst->offset -= sizeof(tagIcmpHdr);
    pkglst->datalen +=sizeof(tagIcmpHdr);

    pkglen = 0;
    pkg = pkglst;
    while(NULL != pkg)
    {
        pkglen += pkg->datalen;
        pkg = pkg->partnext;
    }
    //net_malloc NEW PKG FOR SEND, BECAUSE THE OLD ONE MAY NOT ALIGNED
    pkg2snd = PkgMalloc(pkglen,CN_PKLGLST_END);
    if(NULL != pkg2snd)
    {
        //cpy the data to the new pkg
        pkglen = 0;
        pkg = pkglst;
        dst = (u8 *)(pkg2snd->buf + pkg2snd->offset);
        while(NULL != pkg)
        {
            pkglen = pkg->datalen;
            src = (u8 *)(pkg->buf + pkg->offset);
            memcpy(dst, src, pkglen);
            dst += pkglen;
            pkg2snd->datalen += pkglen;
            pkg = pkg->partnext;
        }

        hdr = (tagIcmpHdr *)(pkg2snd->buf + pkg2snd->offset);
        result = IpSend(EN_IPV_4,(ptu32_t)ipdst,(ptu32_t)ipsrc,pkg2snd,pkg2snd->datalen,IPPROTO_ICMP,\
                CN_IPDEV_ICMPOCHKSUM,&hdr->chksum);
        PkgTryFreePart(pkg2snd);
    }
    return  result;
}

// =============================================================================
// �������ܣ�Icmp_RecvProcess
//          IcmpЭ�����ݰ�����
// ���������ipsrc,ԴIP
//          ipdst,Ŀ��IP
//          pkglst,icmp���ݰ�,����������Ŀ���
//          devfunc,�����Ѿ����ù���
// ���������
// ����ֵ  ��true���ݰ��Ѿ�����������ͷţ� false���ݰ�û�б���������ͷ�ʧ��
// ˵��    :
// =============================================================================
static bool_t __rcvdealv4(u32 ipsrc, u32 ipdst, tagNetPkg *pkglst, u32 devfunc)
{
    bool_t        result;
    tagIcmpHdr     *hdr;
    u16            chksum16;

    result = true;
    if(0 == (devfunc&CN_IPDEV_ICMPICHKSUM))
    {
        //not check it yet, then we'd better got the chksum ourselves
        //if true the set the checksum true
        IpPkgLstChkSum(pkglst,&chksum16,0);
        if(0 != chksum16)
        {
            return result;
        }
    }

    hdr = (tagIcmpHdr *)(pkglst->buf + pkglst->offset);
    pkglst->offset += sizeof(tagIcmpHdr);
    pkglst->datalen -=sizeof(tagIcmpHdr);
    switch(hdr->type)
    {
        case EN_ICMP_ECHOREQUEST:
            result = __Icmp_EchoRequest(ipsrc,ipdst,hdr,pkglst);
            break;
        case EN_ICMP_ECHOREPLY:
            result = __Icmp_EchoReply(ipsrc,ipdst,hdr,pkglst);
            break;
        default:
            break;
    }

    return result;
}

static bool_t __rcvdeal(tagIpAddr *addr,tagNetPkg *pkglst, u32 devfunc)
{
    bool_t result = false;
    enum_ipv_t  ver;
    u32 ipsrc;
    u32 ipdst;
    if((NULL != addr)&&(NULL != pkglst))
    {
        ver = addr->ver;
        if(ver == EN_IPV_4)
        {
            ipsrc = addr->src.ipv4.s_addr;
            ipdst = addr->dst.ipv4.s_addr;
            result = __rcvdealv4(ipsrc,ipdst,pkglst,devfunc);
        }
    }
    return result;
}
// =============================================================================
// �������ܣ�Icmp_Init
//          ICMPЭ���ʼ������
// ���������para,��ʱ������
// ���������
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    :1,ע���IP��
//       2,ע���TPL��
// =============================================================================
bool_t IcmpInit(void)
{
    bool_t result;
    result = IpInstallProto("icmp",IPPROTO_ICMP,__rcvdeal);
    return result;
}
