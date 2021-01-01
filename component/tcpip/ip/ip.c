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
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然:
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。
// 3. 本软件作为都江堰操作系统的组成部分，未获事前取得的书面许可，不允许移植到非
//    都江堰操作系统环境下运行。

// 免责声明:本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
#include <sys/socket.h>
#include <netbsp.h>

#include "../common/link.h"
#include "../common/ipV4.h"
#include "../common/ip.h"
#include "../common/netdev.h"
#include "../common/netpkg.h"
#include "../component_config_tcpip.h"

typedef struct
{
   u32 rcvnum;
   u32 rcverr;
   u32 rcvcached;
   u32 sndnum;
   u32 snderr;
   u32 unknownsnd;
   u32 unknownrcv;
}tagIpDebug;
//usage: we use this module to create the proto install and uninstall
typedef struct __tagIpProto
{
    void *nxt;
    const char *name;
    u8          proto;
    fnIpProto   dealer;
    //the following for the debug here
    u32         rcv;
    u32         rcverr;
    u32         snd;
    u32         snderr;
}tagIpProto;
#define CN_PROTO_HASHLEN  7
typedef struct
{
    tagIpProto           *prototab[CN_PROTO_HASHLEN]; //TODO,check if need the mutex to protect
    tagIpDebug            debug;
}tagIp;
static tagIp  gIpCtrl;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

//add the proto to the hash tab
bool_t IpInstallProto(const char *name,u8 proto,fnIpProto handler)
{
    tagIpProto  *hash;
    int hashkey;
    hash = net_malloc(sizeof(tagIpProto));
    if(NULL != hash)
    {
        memset(hash,0,sizeof(tagIpProto));
        hash->proto = proto;
        hash->name = name;
        hash->dealer = handler;
        hash->nxt = NULL;
        hashkey = proto%CN_PROTO_HASHLEN;
        if(NULL == gIpCtrl.prototab[hashkey])
        {
            gIpCtrl.prototab[hashkey] = hash;
        }
        else
        {
            hash->nxt = gIpCtrl.prototab[hashkey];
            gIpCtrl.prototab[hashkey] = hash;
        }
    }
    return true;
}
//uninstall the hash to the hash tab--TODO,not support yet
bool_t IpUnInstallProto(const char *name,u8 proto,fnIpProto handler)
{
    return false;
}

#pragma GCC diagnostic pop

//find the the hash
tagIpProto *IpProtoFind(u8 proto)
{
    tagIpProto  *hash;
    int hashkey;
    hashkey = proto%CN_PROTO_HASHLEN;
    hash = gIpCtrl.prototab[hashkey];
    while(NULL !=  hash)
    {
        if(hash->proto == proto)
        {
            break;
        }
        else
        {
            hash = hash->nxt;
        }
    }
    return hash;
}

//usage:use this function to upload the package
bool_t IpProtoDeal(u8 proto,tagIpAddr *addr,struct NetPkg *pkglst,u32 devfunc)
{
    bool_t result = false;
    tagIpProto  *Transport;
    Transport = IpProtoFind(proto);
    if((NULL != Transport)&&(NULL != Transport->dealer))
    {
        TCPIP_DEBUG_INC(Transport->rcv);
        result =  Transport->dealer(addr,pkglst,devfunc);
        if(false == result)
        {
            TCPIP_DEBUG_INC(Transport->rcverr);
        }
    }
    return result;
}
//this function used to scan the arp item tab with a fixed cycle
//if the life of the item is less than the trigger level, then it will send a request
//if the life is zero, then it will be delete from the queue
//we scan the item by the hash value, so we will not block the sync too much
#define CN_IP_VERMASK    (0X0f)
static bool_t  __IpPushNew(struct NetDev *iface,struct NetPkg *pkg)
{
    u32           devfunc;
    u8            ipv;
    bool_t        ret;
    TCPIP_DEBUG_INC(gIpCtrl.debug.rcvnum);
    if((NULL != iface)&& (NULL!= pkg))
    {
        devfunc = NetDev_GetFunc(iface);
        ipv = *(u8 *)PkgGetCurrentBuffer(pkg);
//      ipv = *(u8 *)(pkg->buf + pkg->offset);
        ipv = (ipv >>4)&CN_IP_VERMASK;
        if(ipv == EN_IPV_4)
        {
            ret = IpV4Process(pkg,devfunc);
            if(ret == false)
            {
                TCPIP_DEBUG_INC(gIpCtrl.debug.rcverr);
            }
        }
        else
        {
            //do nothing here, unknown internet protocol version
        }
    }
    return ret;
}

// =============================================================================
// FUNCTION:This function is used to compute the checksum
// PARA  IN:buf, which storage the data
//          len, the length of buf
//          chksum, the original checksum
//          done,if true which means do the carry-over
// PARA OUT:
// RETURN  :
// INSTRUCT:
// =============================================================================
u16 IpChksumSoft16(void *dataptr, int len,u16 chksum,bool_t done)
{
    u8 *pb = (u8 *)dataptr;
    u16 *ps, t = 0;
    u32 sum = 0;
    int odd = ((ptu32_t)pb & 1);
    u16 data16;

    /* Get aligned to u16 */
    if (odd && len > 0) {
        ((u8 *)&t)[1] = *pb++;
        len--;
    }

    /* Add the bulk of the data */
    ps = (u16 *)(void *)pb;
    while (len > 1) {
        sum += *ps++;
        len -= 2;
    }

    /* Consume left-over byte, if any */
    if (len > 0) {
        ((u8 *)&t)[0] = *(u8 *)ps;
    }

    /* Add end bytes */
    sum += t;

    /* Fold 32-bit sum to 16 bits
     calling this twice is propably faster than if statements... */
    sum = (sum>>16)+ (sum&0xffff);
    sum = (sum>>16)+ (sum&0xffff);
    data16 = (u16)sum;
    /* Swap if alignment was odd */
    if (odd) {
        data16 = htons(data16);
    }

    //add the sum before
    sum = chksum + data16;
    sum = (sum>>16)+ (sum&0xffff);
    sum = (sum>>16)+ (sum&0xffff);
    data16 = (u16)sum;

    if(done)
    {
        data16 = ~data16;
    }

    return data16;
}

// =============================================================================
// FUNCTION:This function is used to compute one package checksum
// PARA  IN:pkg, which storage the data
//          chksum, used to strorage the checksum
// PARA OUT:
// RETURN  :
// INSTRUCT:
// =============================================================================
void IpPkgChkSum(struct NetPkg *pkg,u16 *chksum,u16 sum)
{
    int                             chklen;
    u16                             result;
    void                            *chkbuf;

    chklen = PkgGetDataLen(pkg);
//  chklen = pkg->datalen;
    chkbuf = (u8 *)PkgGetCurrentBuffer(pkg);
//  chkbuf = (u8 *)(pkg->offset + pkg->buf);
    result = IpChksumSoft16((void *)chkbuf, chklen,sum, true);
    *chksum = result;
    return;
}
// =============================================================================
// FUNCTION:This function is used to compute one package list checksum
// PARA  IN:pkg, which storage the data
//          chksum, used to strorage the checksum
// PARA OUT:
// RETURN  :
// INSTRUCT:
// =============================================================================
void IpPkgLstChkSum(struct NetPkg *pkg,u16 *chksum,u16 sum)
{
    int                             chklen;
    u16                             chksumtmp;
    void                            *chkbuf;
    struct NetPkg                       *pkgtmp;

    chksumtmp = sum;
    pkgtmp = pkg;
    do{
        chklen = PkgGetDataLen(pkgtmp);
//      chklen = pkgtmp->datalen;
        chkbuf = (u8 *)PkgGetCurrentBuffer(pkgtmp);
//      chkbuf = (u8 *)(pkgtmp->offset + pkgtmp->buf);
        if((NULL == PkgGetNextUnit(pkgtmp))||PkgIsBufferEnd(pkgtmp))
        {
            chksumtmp = IpChksumSoft16((void *)chkbuf, chklen,chksumtmp, true);
            *chksum = chksumtmp;
            pkgtmp = NULL;
        }
        else
        {
            chksumtmp = IpChksumSoft16((void *)chkbuf, chklen,chksumtmp, false);
            pkgtmp = PkgGetNextUnit(pkgtmp);
        }
    }while(NULL != pkgtmp);
}
// =============================================================================
// FUNCTION:This function is used to make an interface for the transmit layer
// PARA  IN:ver:which means that EN_IPV_6 (which means ptu32_t is pointed to
//               16 byte array) or EN_IPV_4(which means ptu32_t is pointed to
//               an u32)
//          ipsrc, the local ip
//          ipdst, the detination ip
//          pkg,the transmit layer data, maybe an list
//          translen, the pkg (including the list) length
//          proto, the transmit layer proto
//          devtask, you may need the udp or tcp checksum
//          chksum, which storage the check sum
// PARA OUT:
// RETURN  :true success while false failed
// INSTRUCT:
// =============================================================================
bool_t IpSend(enum_ipv_t ver,ptu32_t ipsrc, ptu32_t ipdst, struct NetPkg *pkg,\
              u16 translen,u8 proto,u32  devtask, u16 *chksum)
{
    bool_t result;

    TCPIP_DEBUG_INC(gIpCtrl.debug.sndnum);

    tagIpProto  *hash;
    hash = IpProtoFind(proto);
    if(NULL != hash)
    {
        TCPIP_DEBUG_INC(hash->snd);
    }
    if(ver == EN_IPV_4)
    {
        result = IpV4Send((u32)ipsrc, (u32)ipdst, pkg,translen,proto,devtask, chksum);
        if(false == result)
        {
            TCPIP_DEBUG_INC(hash->snderr);
        }
    }
    else
    {
        result = false;   //not support yet
    }

    return result;
}
//extern bool_t  Ipv4Init(void);
// =============================================================================
// FUNCTION:this function is used to init the ip module
// PARA  IN:
// PARA OUT:
// RETURN  :true success while false failed
// INSTRUCT:if success, then the transmit layer could send message to the ip layer
//          and the link layer could post package to ipv4 layer,absolutely this engine
//          creates the ip deal engine
// =============================================================================
bool_t IpInit(void)
{
    bool_t ret = false;
//    Ipv4Init();
    memset((void *)&gIpCtrl,0,sizeof(gIpCtrl));
    ret= Link_PushRegister(EN_LINKPROTO_IPV4,__IpPushNew);
    return ret;
}

