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

#include "../common/icmp.h"
#include "../common/ipV4.h"
#include "../common/ip.h"
#include "../common/netpkg.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"icmp"        //icmpЭ��
//parent:"tcpip"       //��д������ĸ�������֣�none��ʾû�и����
//attribute:system              //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"lock","heap","device file system"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_ICMP == false )
//#warning  " icmp  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_ICMP    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,,,
//%$#@enum,true,false,
//%$#@num,,,
//%$#@string,1,256,
//%$#@select
//%$#@free
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure

#pragma pack(1)
struct IcmpHdr
{
    u8        type;        //type for icmp
    u8        code;        //code for icmp
    u16       chksum;      //pkg chksum
    u8        data[0];     //additional data for specified type and data
};

struct IcmpHdrEcho
{
    u16 taskid;
    u16 seqno;
    u8  data[0];
};

#pragma pack()

enum _EN_ICMP_TYPE
{
    EN_ICMP_ECHOREPLY = 0,                  //�յ�����Ӧ���
    EN_ICMP_DSTNOTREACH =3,
    EN_ICMP_SRCSHUTDOWN,
    EN_ICMP_RELOCATE,
    EN_ICMP_ECHOREQUEST =8,                 //�յ����������
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
struct IcmpTask
{
    struct IcmpTask             *pre;     //pre  lst
    struct IcmpTask             *nxt;     //nxt  lst
    struct SemaphoreLCB         *sync;     //when rcv, active it
    //the member used for specifying
    u8        type;                             //icmp type
    u8        code;                             //icmp code
    u8        *data;                            //task data:the data type and code need
};

struct IcmpTaskEcho
{
    u16     taskid;
    u16     seqno;
    u32     ipdst;
};

static struct IcmpTask        *pgIcmpTaskLst;    //icmp queue
static struct MutexLCB         sgIcmpTaskSync;    //icmp queue sync
// =============================================================================
// �������ܣ�__Icmp_TaskAdd
//          add an task to the queue
// ���������task, the task to add
// ���������
// ����ֵ  ��true success while  false failed
// ˵��    :
// =============================================================================
bool_t __Icmp_TaskAdd(struct IcmpTask *task)
{
    task->sync = Lock_SempCreate(1,0,CN_BLOCK_FIFO,NULL);
    if(Lock_MutexPend(&sgIcmpTaskSync,CN_TIMEOUT_FOREVER))
    {
        //create the semp sync
        task->pre = NULL;
        task->nxt = pgIcmpTaskLst;
        if(NULL != pgIcmpTaskLst)
        {
            pgIcmpTaskLst->pre = task;
        }
        pgIcmpTaskLst = task;
        Lock_MutexPost(&sgIcmpTaskSync);
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
bool_t __Icmp_TaskDel(struct IcmpTask *task)
{
    if(Lock_MutexPend(&sgIcmpTaskSync,CN_TIMEOUT_FOREVER))
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
        Lock_SempDelete(task->sync);
        Lock_MutexPost(&sgIcmpTaskSync);
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
//          type: icmp type
//          code: icmp code
//          pkg:  icmp info pkg
// ���������
// ����ֵ  ��
// ˵��    :
// =============================================================================
void __Icmp_TaskEchoActive(u32 ipsrc, u8 type, u8 code,struct NetPkg *pkg)
{
    struct IcmpTask         *tmp;
    struct IcmpTaskEcho     *taskdata;
    struct IcmpHdrEcho      *echodata;

    if(Lock_MutexPend(&sgIcmpTaskSync,CN_TIMEOUT_FOREVER))
    {
        tmp = pgIcmpTaskLst;
        echodata = (struct IcmpHdrEcho *)PkgGetCurrentBuffer(pkg);;
//      echodata = (struct IcmpHdrEcho *)(pkg->buf + pkg->offset);
        while(NULL != tmp)
        {
            if((tmp->type == type)&&(tmp->code == code))
            {
                taskdata = (struct IcmpTaskEcho *)tmp->data;
                if((taskdata->ipdst == ipsrc)&&\
                   (taskdata->taskid ==echodata->taskid)&&\
                   (taskdata->seqno == echodata->seqno))
                {
                    Lock_SempPost(tmp->sync);
                    break;
                }
            }
            tmp =tmp->nxt;
        }
        Lock_MutexPost(&sgIcmpTaskSync);
    }
    return;
}
// =============================================================================
// �������ܣ�Icmp_EchoRequest
//          PING request
// ��������� ipdst, the ip of the host we will PING
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
    struct NetPkg            *sndpkg;
    struct IcmpHdr           *icmppkg;
    struct IcmpHdrEcho       *icmppkgecho;
    struct IcmpTask          icmptask;
    struct IcmpTaskEcho      icmptaskecho;

    result = false;
    if((NULL != data)&&(len > 0)&&(ipdst != INADDR_ANY))
    {
        pkglen = sizeof(struct IcmpHdr)+sizeof(struct IcmpHdrEcho)+len;
        sndpkg = PkgMalloc(pkglen,CN_PKLGLST_END);
        if(NULL != sndpkg)
        {
            seqno++;
            PkgSetDataLen(sndpkg, pkglen);
//            PkgSetNextUnit(sndpkg,NULL);      //PkgMalloc�Ѿ����ú�
//          sndpkg->datalen = pkglen;
//          sndpkg->partnext = NULL;
            //fill the snd pkg
            icmppkg = (struct IcmpHdr *)PkgGetCurrentBuffer(sndpkg);
//          icmppkg = (struct IcmpHdr *)(sndpkg->buf + sndpkg->offset);
            icmppkg->type = EN_ICMP_ECHOREQUEST;
            icmppkg->code = CN_ICMP_ECHOREQUEST_CODE;
            icmppkg->chksum = 0;
            icmppkgecho = (struct IcmpHdrEcho *)(&icmppkg->data[0]);
            icmppkgecho->seqno = htons(seqno);
            icmppkgecho->taskid = htons(DJY_GetMyEvttId());
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
            result = Lock_SempPend(icmptask.sync, timeout);
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
    struct NetPkg         *pkg2snd;
    struct IcmpHdr        *hdr;
    u8                *src;
    u8                *dst;
    result = false;
    pkglen = sizeof(struct IcmpHdr)+infolen;
    pkg2snd = PkgMalloc(pkglen,CN_PKLGLST_END);
    if(NULL != pkg2snd)
    {
        PkgSetDataLen(pkg2snd, pkglen);
//        PkgSetNextUnit(pkg2snd,NULL);      //PkgMalloc�Ѿ����ú�
//      pkg2snd->datalen = pkglen;
//      pkg2snd->partnext = NULL;
        hdr = (struct IcmpHdr *)PkgGetCurrentBuffer(pkg2snd);
//      hdr = (struct IcmpHdr *)(pkg2snd->buf + pkg2snd->offset);
        hdr->type = type;
        hdr->code = code;
        hdr->chksum = 0;
        src = info;
        dst = &hdr->data[0];
        memcpy(dst, src, infolen);

        result = IpSend(EN_IPV_4,(ptu32_t)ipdst,(ptu32_t)ipsrc,pkg2snd,PkgGetDataLen(pkg2snd),IPPROTO_ICMP,\
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
bool_t __Icmp_EchoReply(u32 ipsrc, struct IcmpHdr *hdr,struct NetPkg *pkg)
{
    u8 type;
    u8 code;

    type = hdr->type;
    code = hdr->code;

    __Icmp_TaskEchoActive(ipsrc, type, code, pkg);
    return true;
}
// =============================================================================
// ���ܣ��յ�ICMP�������Ӧ��������������ICMPӦ���
// ���������ipsrc, pkg source host
//          ipdst, pkg dst host
//          type, icmp type
//          code, icmp code
//          pkg, info for the specified type and code
// ���������
// ����ֵ  ��true , request success while false timeout
// ˵��    :--todo, CPY PROCESS MAYBE TOO OLD
// =============================================================================
bool_t __Icmp_EchoRequest(u32 ipsrc, u32 ipdst,struct IcmpHdr *hdr,struct NetPkg *pkglst)
{
    bool_t              result=false;
    u16                 pkglen;
    struct NetPkg           *pkg;
    struct NetPkg           *pkg2snd;

    hdr->type = EN_ICMP_ECHOREPLY;
    hdr->code = CN_ICMP_ECHOREPLY_CODE;
    hdr->chksum = 0;
    PkgMoveOffsetDown(pkglst,sizeof(struct IcmpHdr));
//  pkglst->offset -= sizeof(struct IcmpHdr);
//  pkglst->datalen +=sizeof(struct IcmpHdr);

//  pkglen = 0;
    pkg = pkglst;
    pkglen = PkgListDatastatistics(pkg);
//  while(NULL != pkg)
//  {
//      pkglen += pkg->datalen;
//      pkg = PkgGetNextUnit(pkg);
//  }
    //net_malloc NEW PKG FOR SEND, BECAUSE THE OLD ONE MAY NOT ALIGNED
    pkg2snd = PkgMalloc(pkglen,CN_PKLGLST_END);
    if(NULL != pkg2snd)
    {
        //cpy the data to the new pkg
        pkglen = 0;
        pkg = pkglst;
        PkgCopyListToPkg(pkg, pkg2snd);
//      dst = (u8 *)(pkg2snd->buf + pkg2snd->offset);
//      while(NULL != pkg)
//      {
//          pkglen = pkg->datalen;
//          src = (u8 *)(pkg->buf + pkg->offset);
//          memcpy(dst, src, pkglen);
//          dst += pkglen;
//          pkg2snd->datalen += pkglen;
//          pkg = PkgGetNextUnit(pkg);
//      }

        hdr = (struct IcmpHdr *)PkgGetCurrentBuffer(pkg2snd);
//      hdr = (struct IcmpHdr *)(pkg2snd->buf + pkg2snd->offset);
        result = IpSend(EN_IPV_4,(ptu32_t)ipdst,(ptu32_t)ipsrc,pkg2snd,
                        PkgGetDataLen(pkg2snd),IPPROTO_ICMP,\
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
static bool_t __rcvdealv4(u32 ipsrc, u32 ipdst, struct NetPkg *pkglst, u32 devfunc)
{
    bool_t        result;
    struct IcmpHdr     *hdr;
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

    hdr = (struct IcmpHdr *)PkgGetCurrentBuffer(pkglst);
//  hdr = (struct IcmpHdr *)(pkglst->buf + pkglst->offset);
    PkgMoveOffsetUp(pkglst,sizeof(struct IcmpHdr));
//  pkglst->offset += sizeof(struct IcmpHdr);
//  pkglst->datalen -=sizeof(struct IcmpHdr);
    switch(hdr->type)
    {
        case EN_ICMP_ECHOREQUEST:
            result = __Icmp_EchoRequest(ipsrc,ipdst,hdr,pkglst);
            break;
        case EN_ICMP_ECHOREPLY:
            result = __Icmp_EchoReply(ipsrc,hdr,pkglst);
            break;
        default:
            break;
    }

    return result;
}

static bool_t __rcvdeal(tagIpAddr *addr,struct NetPkg *pkglst, u32 devfunc)
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

