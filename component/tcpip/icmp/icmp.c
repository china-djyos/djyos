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
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
#include <sys/socket.h>

#include "../common/icmp.h"
#include "../common/ipV4.h"
#include "../common/ip.h"
#include "../common/netpkg.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"icmp"        //icmp协议
//parent:"tcpip"       //填写该组件的父组件名字，none表示没有父组件
//attribute:system              //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"lock","heap","device file system"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_ICMP == false )
//#warning  " icmp  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_ICMP    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,,,
//%$#@enum,true,false,
//%$#@num,,,
//%$#@string,1,256,
//%$#@select
//%$#@free
#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束

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
    EN_ICMP_ECHOREPLY = 0,                  //收到回显应答包
    EN_ICMP_DSTNOTREACH =3,
    EN_ICMP_SRCSHUTDOWN,
    EN_ICMP_RELOCATE,
    EN_ICMP_ECHOREQUEST =8,                 //收到回显请求包
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
// 函数功能：__Icmp_TaskAdd
//          add an task to the queue
// 输入参数：task, the task to add
// 输出参数：
// 返回值  ：true success while  false failed
// 说明    :
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
// 函数功能：__Icmp_TaskDel
//          remove an task from the queue
// 输入参数：task, the task to remove
// 输出参数：
// 返回值  ：true success while  false failed
// 说明    :
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
// 函数功能：__Icmp_TaskEchoActive
// 输入参数：
//          ipsrc:pkg src
//          type: icmp type
//          code: icmp code
//          pkg:  icmp info pkg
// 输出参数：
// 返回值  ：
// 说明    :
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
// 函数功能：Icmp_EchoRequest
//          PING request
// 输入参数： ipdst, the ip of the host we will PING
//           data, additionnal data
//           len, additionnal data len
// 输出参数：
// 返回值  ：true , request success while false timeout
// 说明    :
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
//            PkgSetNextUnit(sndpkg,NULL);      //PkgMalloc已经设置好
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
// 函数功能：Icmp_MsgSnd
//          snd icmp msg
// 输入参数： ipsrc, the ip of the msg src
//        ipdst, the ip of the  msg dst
//        type, icmp type
//        code, icmp code
//        info, the info buf ptr
//        infolen, the len of the info
// 输出参数：
// 返回值  ：true , snd success while false failed
// 说明    :
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
//        PkgSetNextUnit(pkg2snd,NULL);      //PkgMalloc已经设置好
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
// 函数功能：__Icmp_EchoReply
//          deal the echo reply pkg
// 输入参数：ipsrc, pkg source host
//          ipdst, pkg dst host
//          type, icmp type
//          code, icmp code
//          pkg, info for the specified type and code
// 输出参数：
// 返回值  ：true succees while false failed
// 说明    :Find any task in the queue,if any the set it,otherwise do nothing
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
// 功能：收到ICMP请求的响应函数胡，将发送ICMP应答包
// 输入参数：ipsrc, pkg source host
//          ipdst, pkg dst host
//          type, icmp type
//          code, icmp code
//          pkg, info for the specified type and code
// 输出参数：
// 返回值  ：true , request success while false timeout
// 说明    :--todo, CPY PROCESS MAYBE TOO OLD
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
// 函数功能：Icmp_RecvProcess
//          Icmp协议数据包处理
// 输入参数：ipsrc,源IP
//          ipdst,目的IP
//          pkglst,icmp数据包,存在重组包的可能
//          devfunc,网卡已经做得工作
// 输出参数：
// 返回值  ：true数据包已经被缓存或者释放， false数据包没有被缓存或者释放失败
// 说明    :
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
// 函数功能：Icmp_Init
//          ICMP协议初始化函数
// 输入参数：para,暂时无意义
// 输出参数：
// 返回值  ：true 成功 false失败
// 说明    :1,注册进IP层
//       2,注册进TPL层
// =============================================================================
bool_t IcmpInit(void)
{
    bool_t result;
    result = IpInstallProto("icmp",IPPROTO_ICMP,__rcvdeal);
    return result;
}

