//----------------------------------------------------
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
//author:zhangqf
//date  :下午4:43:13/2017年1月4日
//usage :
//-----------------------------------------------------------------------------
//some times i found that the code was too ugly,so i rewrite the new one,wish you
//like it
//standard includes
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <os.h>
#include <misc/misc.h>

#include <sys/socket.h>
#include <netdb.h>
#include <netbsp.h>
#include <arpa/inet.h>
//add your own specified header here
#include "iodev.h"
#include "osarch.h"
#include "ppp.h"
#include <shell.h>
#include "component_config_ppp.h"
#include "../common/netpkg.h"

#include "fcs.h"
#include "dbug.h"
#define CN_OPTION_BUFLEN         128             //this maybe big enough
#define CN_FRAME_BUFLEN          (PPP_MTU+32)    //long enough to storage the head and fcs
#define CN_FRAME_CACHELEN        64              //USED COPY TO OR FROM THE FRAME BUFFER
#define CN_FRAME_CACHECODELEN    (CN_FRAME_CACHELEN/2) //at least half could be encode
#define CN_FRAME_FCSLEN          2               //fcs code len
#define PPP_BITS(x)              (1<<x)

#pragma pack(1)
typedef struct {
    u8 code;
    u8 id;
    u16 len;
} tagCH;
#define CN_PPPOS_CHSIZE   sizeof(tagCH)
//usage:we use this data structure to do the negotiation decode
typedef struct {
    u8 type;
    u8 len;
    u8 v[0];
} tagOH;
#define CN_PPPOS_OPTSIZE sizeof(tagOH)
typedef struct {
    u8 a;
    u8 c;
    u16 p;
} tagPH;
#define CN_PPP_HDRSIZE  (sizeof(tagPH))
#pragma pack()

typedef struct {
    bool_t endebug;      //when true, then it will debug the io
    u32 sndframe;     //the data to send has been over the frame size
    u32 sndioerr; //the device used to send the data returned err(-1 or not equal)
    u32 rcvfcserr;    //how many frames received but fcs err
    u32 rcvframe;     //how many frames received
    s64 ipsndlen;     //how many ip bytes sent by the ppp
    s64 iprcvlen;     //how many ip bytes received by the ppp
    u32 termreq;      //terminate by terminate request get
    u32 timerst;      //the terminate by time out
} tagDB;
//use this function to define the ppp frame buffer
typedef struct {
    u8 buffer[CN_FRAME_BUFLEN];
    u8 cache[CN_FRAME_CACHELEN]; //use this cache to avoid the data encoded overflow
    u16 lenbuffer;
    u16 lencache;
} tagBuffer;
#define CN_PPP_OPT_MAXLEN  6   //maybe enough for all the use
typedef u8 tOPTBUFER[CN_PPP_OPT_MAXLEN];
#define CN_OPT_EN         (1<<0)   //when enable,you could find the opt
#define CN_OPT_ACK        (1<<1)   //when ack, then it could be ack
#define CN_OPT_NACK       (1<<2)   //when nack, then it could be set but not RO
#define CN_OPT_REJ        (1<<3)   //when REJ,then it could be set
#define CN_OPT_W          (1<<4)   //which means this item could be write,if not set,then you could not modified
typedef struct
{
    u8 status;
    tOPTBUFER buf;
} tagOptItem;   //one item
typedef struct
{
    u8 len;  //show how many option item there
    tagOptItem *tab;
} tagOpts;

//use this function to initialize all the options in lcp ncp
static void __OptItemInit(tagOptItem *item, u8 type, u8 *value, u8 len,
        u8 status)
{
    tagOH *opt;
    opt = (tagOH*) &item->buf;
    opt->type = type;
    opt->len = sizeof(tagOH) + len;
    memcpy(opt->v, value, len);
    item->status = status;
}
//find a specified item in the item table
static tagOptItem *__OptItemMatch(tagOpts *opts, u8 type)
{
    tagOptItem *ret = NULL;
    tagOH *opt = NULL;
    int i = 0;
    for (i = 0; i < opts->len; i++) {
        opt = (tagOH *) &opts->tab[i].buf;
        if ((type == opt->type) && (opts->tab[i].status & CN_OPT_EN)) {
            ret = &opts->tab[i];
        }
    }
    return ret;
}
//deal all the response action:ack nack reject
static void __OptRspDeal(tagOpts *opts, u8 *buf, u16 len, u8 status) {
    u8 *src;
    u8 *stop;
    tagOH *opt;
    tagOptItem *item = NULL;

    src = buf;
    stop = buf + len;
    while (src < stop) {
        opt = (tagOH *) src;
        src += opt->len;
        item = __OptItemMatch(opts, opt->type);
        if (NULL != item) {
            item->status |= status;
            __OptItemInit(item, opt->type, opt->v,
                    opt->len - sizeof(tagOH), item->status);
        }
    }
    return;
}

//usage:use this function to decode the input data and check the frame we got
typedef enum {
    EN_CHECK_START = 0,   //check 0x7e
    EN_CHECK_STATION,     //check 0xff
    EN_CHECK_CTRL,        //check 0x03
    EN_CHECK_PROTO_H,     //check the proto high
    EN_CHECK_PROTO_L,     //check the proto low
    EN_CHECK_CDR_CODE,    //check the code
    EN_CHECK_CDR_ID,      //check the id
    EN_CHECK_LEN1,        //check the len
    EN_CHECK_LEN2,        //check the len
    EN_CHECK_DATA,        //check the proto data
    EN_CHECK_FCS16_1,     //check the FCS161
    EN_CHECK_FCS16_2,     //check the FCS162
    EN_CHECK_UNKNOWN,     //illegal state
} enCheckStat;
typedef struct {
    tagOpts host;
    tagOpts peer;
    u8 status; //when done you could pick the magic accm auth out for the options
} tagLcp;
#define CN_CONF_HOSTDONE    (PPP_BITS(0))
#define CN_CONF_PEERDONE    (PPP_BITS(1))

typedef struct
{
    u8 status;
    tagOpts net;
} tagNcp;
//defines for the machine state
#define CN_LCP_TIME        (10*1000*mS)    //lcp  timeout
#define CN_AUTH_TIME       (10*1000*mS)    //auth timeout
#define CN_NCP_TIME        (10*1000*mS)    //ncp  timeout
#define CN_IP_TIME         (50*1000*mS)    //ip   timeout:if timeout will send the lcp echo
#define CN_TIMEOUT_LIMIT   10              //timeout limittimes,if over,then will terminate and goto dead state
typedef enum {
    EN_PPP_DEAD = 0,          //the link has not established yet
    EN_PPP_ESTABLISH,        //do the lcp negotiation
    EN_PPP_AUTH,             //do the authentication
    EN_PPP_NCP,              //do the net negotiation
    EN_PPP_NETWORK,          //do the normal net work
    EN_PPP_TERMINATE,        //do the normal shutdown
} enPppStat;
const char *gPppMsName[] = { "DEAD", "ESATBLISH", "AUTH", "NCP", "NETWORK",
        "TERMINAL" };
typedef struct {
    enPppStat stat;          //used for the machine state
    s64 timedead;      //the timeout time by the system
    u32 timeoutnum;    //continuing timeout times
    u32 timeoutlimit; //continuing timeout times limit,if overlap,goto dead state
} tagMS;
typedef struct
{
    tagCH *ch;    //point to ch position in the receive buffer
    u16 len;    //the frame data len:include the data
    void *data;  //point to the data position in the receive buffer
    u16 datalen;    //how many data we has get
    bool_t espace;  //tell you if in the espace state
    u16 proto;  //the frame proto type
    u16 fcs16;  //the fcs of the frame
    u8 state;  //tell you which state in
} tagRBC;  //byte receive decoder,decode the data to the receive buffer
//usage:THIS IS THE PPP CONTROL BLOCK HERE
#define CN_NAME_LIMIT  32
typedef struct {
    //configured by the api function
    char nameio[CN_NAME_LIMIT];
    char namenet[CN_NAME_LIMIT];
    char user[CN_NAME_LIMIT];
    char passwd[CN_NAME_LIMIT];
    char apn[CN_NAME_LIMIT];
    fnIO2PPP fnio2ppp;    //used to make the io to ppp data mode
    fnIORst fniorst;     //used to reset the io device
    fnNetDevEventHook fnevent;
    //defines will not be reset
    s32 fdio;    //the io device
    struct NetDev *fdnet;   //the net device
    tagDB debug;     //defines for the ppp debug
    tagRBC rbc;      //receive byte controller
    struct MutexLCB* sndmutex;      //do the multiple thread protect
    //the following member will be reset in machine state
    bool_t start;
    //defines for the receive and send buffer
    tagBuffer rcvbuf;
    tagBuffer sndbuf;
    u16 optionlen;
    u8 options[CN_OPTION_BUFLEN];
    tagMS ms;          //defines for the ppp machine station
    u8 idhost;   //when the peer ack/nack/rej will do the increment
    u8 idpeer;   //when we ack/nack/rej use this id to fill the chdr
    //defines for the negotiation of lcp auth ncp
    tagLcp lcp;
    tagNcp ncp;
    //this is picked out by the negotiation
    //picked out by the lcp
    u8 authtype;
    u16 authproto;
    u32 magichost;
    u32 magicpeer;
    u32 accmhost;
    u32 accmpeer;
    //picked out by the ncp
    u32 ipaddr;
    u32 dnsaddr;

    void *routwan;//for ppp,there is only wan router
} tagPPP;

static tagPPP *pPPPClient; //this is the only ppp client,not support multi client now
//usage:use this to reset the receive machine state
static void __RstRBC(tagPPP *ppp) {
    tagRBC *rbc;
    rbc = &ppp->rbc;
    rbc->state = EN_CHECK_START;
    rbc->proto = 0;
    rbc->ch = NULL;
    rbc->len = 0;
    rbc->data = NULL;
    rbc->datalen = 0;
    rbc->espace = false;
    ppp->rcvbuf.lenbuffer = 1;
    ppp->rcvbuf.buffer[0] = CN_PPP_FLAG;
    return;
}
typedef enum
{
    EN_LCP_OPT_MRU = 0,
    EN_LCP_OPT_ACCM,
    EN_LCP_OPT_AUTH,
    EN_LCP_OPT_QUAL,
    EN_LCP_OPT_MAGIC,
    EN_LCP_OPT_PFC,
    EN_LCP_OPT_ACFC,
    EN_LCP_OPT_POSLAST,
} enLcpOptPos;
typedef enum
{
    EN_NCP_OPT_IP = 0,
    EN_NCP_OPT_DNS,
    EN_NCP_OPT_DNSBAK,
    EN_NCP_OPT_NBNS,
    EN_NCP_OPT_NBNSBAK,
    EN_NCP_OPT_POSLAST,
} enNcpOptPos;
//usage:use this function to reset the ppp machine state
//      recover all the state to the initialized
static bool_t __MSRst(tagPPP *ppp) {

    u32 v32;
    u16 v16;
    tagOpts *opts;
    //reset all the ppp state here
    memset(&ppp->rcvbuf, 0, sizeof(tagBuffer));
    memset(&ppp->sndbuf, 0, sizeof(tagBuffer)); //clean the receive and send buffer
    ppp->optionlen = 0;
    memset(ppp->options, 0, CN_OPTION_BUFLEN);
    ppp->idhost = 1;
    ppp->idpeer = 1;
    //CLEAR THE TIMEOUT CONTROLLER
    memset(&ppp->ms, 0, sizeof(tagMS)); //clean the machine state
    ppp->ms.timeoutlimit = CN_TIMEOUT_LIMIT;
    //SET THE CONFIGURATION OPTIONS
    //SET THE LCP HOST
    opts = &ppp->lcp.host;
    memset(opts->tab, 0, sizeof(tagOptItem) * opts->len);
    //set the magic MRU auth protocol accm
    v16 = PPP_MTU;
    v16 = htons(v16);
    __OptItemInit(&opts->tab[EN_LCP_OPT_MRU], LCP_OPT_MRU, (u8 *) &v16,
            sizeof(v16),CN_OPT_EN | CN_OPT_W);
    v32 = (u32) DJY_GetSysTime();
    ppp->magichost = v32;
    v32 = htonl(v32);
    __OptItemInit(&opts->tab[EN_LCP_OPT_MAGIC], LCP_OPT_MAGIC, (u8 *) &v32,
            sizeof(v32), CN_OPT_EN | CN_OPT_W);
    v32 = 0;
    ppp->accmhost = v32;
    __OptItemInit(&opts->tab[EN_LCP_OPT_ACCM], LCP_OPT_ACCM, (u8 *) &v32,
            sizeof(v32),CN_OPT_EN | CN_OPT_W);
    //SET THE LCP PEER
    opts = &ppp->lcp.peer;
    memset(opts->tab, 0, sizeof(tagOptItem) * opts->len);
    //set the magic MRU auth protocol accm
    v16 = PPP_MTU;
    v16 = htons(v16);
    __OptItemInit(&opts->tab[EN_LCP_OPT_MRU], LCP_OPT_MRU, (u8 *) &v16,
            sizeof(v16), CN_OPT_EN | CN_OPT_W);
    v32 = (u32) DJY_GetSysTime();
    v32 = htonl(v32);
    __OptItemInit(&opts->tab[EN_LCP_OPT_MAGIC], LCP_OPT_MAGIC, (u8 *) &v32,
            sizeof(v32), CN_OPT_EN | CN_OPT_W);
    v32 = 0;
    __OptItemInit(&opts->tab[EN_LCP_OPT_ACCM], LCP_OPT_ACCM, (u8 *) &v32,
            sizeof(v32), CN_OPT_EN | CN_OPT_W);
    v16 = CN_PPP_PAP;
    v16 = htons(v16);
    __OptItemInit(&opts->tab[EN_LCP_OPT_AUTH], LCP_OPT_AUTH, (u8 *) &v16,
            sizeof(v16), CN_OPT_EN);
    //SET THE NCP
    opts = &ppp->ncp.net;
    memset(opts->tab, 0, sizeof(tagOptItem) * opts->len);
    //set the IP DNS DNSBAK NBNS NBNSBAK
    v32 = 0;
    __OptItemInit(&opts->tab[EN_NCP_OPT_IP], NCP_OPT_IP, (u8 *) &v32,
            sizeof(v32), CN_OPT_EN | CN_OPT_W);
    __OptItemInit(&opts->tab[EN_NCP_OPT_DNS], NCP_OPT_DNS, (u8 *) &v32,
            sizeof(v32),CN_OPT_EN | CN_OPT_W);
    __OptItemInit(&opts->tab[EN_NCP_OPT_DNSBAK], NCP_OPT_DNSBAK, (u8 *) &v32,
            sizeof(v32), CN_OPT_EN | CN_OPT_W);
    __OptItemInit(&opts->tab[EN_NCP_OPT_NBNS], NCP_OPT_NBNS, (u8 *) &v32,
            sizeof(v32), CN_OPT_EN | CN_OPT_W);
    __OptItemInit(&opts->tab[EN_NCP_OPT_NBNSBAK], NCP_OPT_NBNSBAK, (u8 *) &v32,
            sizeof(v32), CN_OPT_EN | CN_OPT_W);

    ppp->lcp.status = 0;
    ppp->ncp.status = 0;
    ppp->start = false;
    //reset the picked out member
    ppp->authtype = 0;
    ppp->authproto = CN_PPP_PAP;
    ppp->magicpeer = 0;
    ppp->accmpeer = 0;
    ppp->ipaddr = 0;
    ppp->dnsaddr = 0;
    ppp->ms.stat = EN_PPP_DEAD;
    return true;
}
//usage:use this function  to change the machine state
static void __ChangeMS(tagPPP *ppp, u8 newstate) {
    u8 bak;
    bak = ppp->ms.stat;
    ppp->ms.stat = newstate;
    ppp->ms.timedead = 0;
    ppp->ms.timeoutnum = 0;
    ppp->ms.timeoutlimit = CN_TIMEOUT_LIMIT;
    debug_printf("PPP","MSCHANGE:%s-->%s\n\r", gPppMsName[bak], gPppMsName[newstate]);
    return;
}
//usage:encode the data to send:return the of encode data length
static u16 __DataEncode(u8 *src, u16 len, u8 *dst, u32 accm) {
    u16 ret = 0;
    u8 c;
    while (len > 0) {
        c = *src++;
        len--;
        if ((c == CN_PPP_FLAG) || (c == CN_PPP_ESCAPE)
                || ((c < 0x20) && ((1 << c) & accm))) {
            *dst++ = CN_PPP_ESCAPE;
            c ^= CN_PPP_TRANS;
            *dst++ = c;
            ret += 2;
        }
        else {
            *dst++ = c;
            ret += 1;
        }
    }
    return ret;
}
//usage:use this function to send the message to the device,
//which must reorganize the  the frame here
//the ip and the lcp may use the function here, so we should do the multi task sync
static bool_t __IoDevOut(tagPPP *ppp, u16 proto, tagCH *chdr, u8 *buf, u16 l,
        u32 accm) {
    int datalen = 0;
    int len = 0;
    tagPH headpro;
    tagBuffer *txbuff;
    FCS16 fcs16;
    u16 fcsv;
    u8 c;
    u8 *dst;
    bool_t ret = false;

    if (Lock_MutexPend(ppp->sndmutex,CN_TIMEOUT_FOREVER)) {
        TCPIP_DEBUG_INC(ppp->debug.sndframe); //do the exit here
        txbuff = &ppp->sndbuf;
        memset(txbuff, 0, sizeof(tagBuffer));
        dst = txbuff->buffer;
        //first we should reorganize the frame to send here
        //fill and copy the protocol head
        headpro.a = CN_PPP_ALLSTATIONS;
        headpro.c = CN_PPP_UI;
        headpro.p = htons(proto);
        len = sizeof(headpro);
        memcpy(dst, &headpro, len);
        datalen += len;
        dst += len;
        //cpy the chr if need
        if (NULL != chdr) {
            len = sizeof(tagCH);
            memcpy(dst, (void *) chdr, len);
            datalen += len;
            dst += len;
        }
        //cpy the data to send
        if ((datalen + l + CN_FRAME_FCSLEN) < CN_FRAME_BUFLEN) {
            len = l;
            memcpy(dst, (void *) buf, len);
            datalen += len;
            dst += len;
        }
        else {
            TCPIP_DEBUG_INC(ppp->debug.sndioerr);
            goto EXIT_SND;
        }
        //now compute the frame checksum sequence
        FCS16_Init(&fcs16);
        FCS16_Update(&fcs16, txbuff->buffer, datalen);
        FCS16_Final(&fcs16, &fcsv);
        //cpy the fcs,low byte first
        len = CN_FRAME_FCSLEN;
        c = fcsv & 0xff;
        *dst++ = c;
        c = (fcsv >> 8) & 0xff;
        *dst++ = c;
        datalen += len;
        //file the send buffer length
        txbuff->lenbuffer = datalen;
        //if debug
        if (ppp->debug.endebug) {
            time_t logtime;
            logtime = time(NULL);
            debug_printf("PPP","[%s:S:%04d]7e ", ctime(&logtime), txbuff->lenbuffer);
            for (int i = 0; i < txbuff->lenbuffer; i++) {
                debug_printf("PPP","%02x ", txbuff->buffer[i]);
            }
            debug_printf("PPP","7e\n\r");
        }
        //we has reorganized the frame,so do the sending here,should do the accm compute
        //send the start flag here
        c = 0x7e;
        len = iodevwrite(ppp->fdio, &c, 1, CN_IODEV_WTIMEOUT);
        if (len != 1) {
            TCPIP_DEBUG_INC(ppp->debug.sndioerr); //do the exit here
            goto EXIT_SND;
        }
        dst = txbuff->buffer;
        while (datalen > 0) {
            len = datalen > CN_FRAME_CACHECODELEN ?CN_FRAME_CACHECODELEN :datalen;
            txbuff->lencache = __DataEncode(dst, len, txbuff->cache, accm);
            datalen -= len;
            dst += len;
            len = iodevwrite(ppp->fdio, txbuff->cache, txbuff->lencache,CN_IODEV_WTIMEOUT);
            if (len != txbuff->lencache) {
                TCPIP_DEBUG_INC(ppp->debug.sndioerr); //do the exit here
                goto EXIT_SND;
            }
        }
        //then send the stop flag
        c = 0x7e;
        len = iodevwrite(ppp->fdio, &c, 1, CN_IODEV_WTIMEOUT);
        if (len != 1) {
            TCPIP_DEBUG_INC(ppp->debug.sndioerr); //do the exit here
            goto EXIT_SND;
        }

        ret = true;
        EXIT_SND:
        Lock_MutexPost(ppp->sndmutex);
    }
    return ret;
}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

//here we create a ppp net device to the stack
static bool_t __NetDevOut(struct NetDev *dev, struct NetPkg *pkg, u32 netdevtask)
{
    bool_t result;
    struct NetPkg *tmp;
    u8 *buf;
    u8 *dst;
    u32 cpylen;
    tagPPP *ppp;
    u32 framlen;

    framlen = PkgFrameDatastatistics(pkg);
    result = false;
    ppp = (tagPPP*) NetDev_GetPrivate(dev);
    if (ppp->ms.stat != EN_PPP_NETWORK) {
        return result;   //if no ppp link got ,return false
    }
    ppp->debug.ipsndlen += framlen;
    buf = net_malloc(framlen);
    if (NULL != buf) {
        cpylen = 0;
        dst = buf;
        tmp = pkg;
        cpylen = PkgFrameDataCopy(tmp,dst);
//        while (NULL != tmp) {
//            src = (u8 *) (tmp->buf + tmp->offset);
//            memcpy(dst, src, tmp->datalen);
//            dst += tmp->datalen;
//            cpylen += tmp->datalen;
////          if (tmp->pkgflag & CN_PKLGLST_END)
//            if(PkgIsBufferEnd(tmp))
//            {
//                tmp = NULL;
//            }
//            else {
//                tmp = PkgGetNextUnit(tmp);
//            }
//        }
        //send the buf to the ppp
        result = __IoDevOut(ppp, CN_PPP_IP, NULL, buf, cpylen,
                ppp->accmpeer);
        net_free(buf);
        ppp->debug.ipsndlen += framlen;
    }

    return result;
}

//usage:the deal model should recognize the chdr
static bool_t __IpDeal(tagPPP *ppp, tagCH *ch, u8 *data, int len) {
    //we should package the msg and put it to the stack
    u8 *dst;
    struct NetPkg *pkg;
    u16 lenframe;
    lenframe = len + sizeof(tagCH);
    ppp->debug.iprcvlen += lenframe;
    pkg = PkgMalloc(lenframe, CN_PKLGLST_END);
    if (NULL != pkg) {
        dst = PkgGetCurrentBuffer(pkg);
//      dst = pkg->buf + pkg->offset;
        memcpy(dst, (void *) ch, lenframe);
        PkgSetDataLen(pkg, lenframe);
//      pkg->datalen = lenframe;
        Link_NetDevPush(ppp->fdnet, pkg);
        PkgTryFreePart(pkg);
    }
    return true;
}
//usage:use this function to add a net device to the stack,at the same time we will
//      register a rout
static bool_t __NetDevAdd(tagPPP *ppp) {
    bool_t ret = false;
    struct NetDev* dev;
    struct NetDevPara devpara;
    //then we will register a loop device to the stack
    memset((void *) &devpara, 0, sizeof(devpara));
    devpara.ifsend = __NetDevOut;
    devpara.iftype = EN_LINK_RAW;
    devpara.name = ppp->namenet;
    devpara.Private =  ppp;
    devpara.mtu = PPP_MTU;
    devpara.devfunc = CN_IPDEV_NONE;
    memcpy(devpara.mac, CN_MAC_BROAD, CN_MACADDR_LEN);
    dev = NetDev_Install(&devpara);
    if (NULL == dev) {
        goto EXIT_PPPDEV;
    }
    //不可以这里设置dns, 因为ppp获取ip成功后同时获取到DNS才设置。
    //NetDev_SetDns(EN_IPV_4, dev, &ppp->dnsaddr, &ppp->dnsaddr);
    //NetDev_SetGateway(EN_IPV_4, dev, &ppp->dnsaddr);
    //add the event hook here
    if (NULL != ppp->fnevent) {
        NetDev_RegisterEventHook(dev, ppp->fnevent);
    }
    //here means we are successful
    ppp->fdnet = dev;
    ret = true;
    return ret;

EXIT_PPPDEV:
    return ret;
}


//use this function to send the ppp option configuration
//proto:ncp or lcp
//code:request ack nack rej,
//id:used for the tagCH
//tab: the option table
//len: how many item in the table
static bool_t __SndOptions(tagPPP *ppp, u16 proto, u8 code, u8 id,
        tagOptItem *tab, u8 tablen, u32 accm) {
    u8 *dst;
    tagCH ch;
    u16 len = 0;
    u8 i = 0;
    tagOptItem *item;
    tagOH *opt;
    dst = ppp->options;
    ppp->optionlen = len;
    //copy all the options to send
    for (i = 0; i < tablen; i++)
    {
        item = &tab[i];
        if ((0 == (item->status & CN_OPT_REJ)) && (item->status & CN_OPT_EN))
                {
            opt = (tagOH*) &item->buf;
            memcpy(dst, opt, opt->len);
            dst += opt->len;
            len += opt->len;
        }
    }
    ppp->optionlen = len;
    ch.code = code;
    ch.id = id;
    ch.len = htons((len+sizeof(ch)));
    __IoDevOut(ppp, proto, &ch, ppp->options, ppp->optionlen, accm);
    return true;
}
//use this function to send a command
static bool_t __LcpSndCmd(tagPPP *ppp, u8 code, u8 id, u8 *data, u16 len) {
    tagCH ch;
    ch.code = code;
    ch.id = id;
    ch.len = htons(len+sizeof(ch));
    __IoDevOut(ppp, CN_PPP_LCP, &ch, data, len, 0xFFFFFFFF);
    return true;
}

//copy the rej or unack option to the ppp options
static u8   __OptReqDeal(tagPPP *ppp,tagOpts *opts, u8 *buf, u16 len) {
    u8 *src;
    u8 *stop;
    tagOH *opt;
    tagOptItem *item = NULL;
    u8 *dst;
    u8 s;

    ppp->optionlen = 0;
    s = CN_OPT_ACK;
    src = buf;
    stop = buf + len;
    while (src < stop) {
        opt = (tagOH *) src;
        src += opt->len;
        item = __OptItemMatch(opts, opt->type);
        if (NULL != item) {
            if(0 == memcmp(item->buf,opt,opt->len)){//the same options,so just mark it
                item->status |= CN_OPT_ACK;  //HAS MODIFIED
            }
            else{//not the same
                if (item->status & CN_OPT_W) { //if could write we will accept it and do ack
                    item->status |= CN_OPT_NACK;  //HAS MODIFIED
                    __OptItemInit(item, opt->type, opt->v,
                            opt->len - sizeof(tagOH), item->status);
                }else{//could not write,so just modified
                    opt = (tagOH*) &item->buf;
                    if(0 == (s&CN_OPT_REJ)) //if has do the rej,then just ignore this time
                    {
                        s  = CN_OPT_NACK;
                        dst =&ppp->options[ppp->optionlen];
                        memcpy(dst,opt,opt->len);
                        ppp->optionlen += opt->len;
                    }
                }
            }
        }
        else{

            if(0 == (s&CN_OPT_REJ)) //if has do the rej,then just ignore this time
            {
                ppp->optionlen = 0;
            }
            s  = CN_OPT_REJ;
            dst =&ppp->options[ppp->optionlen];
            memcpy(dst,opt,opt->len);
            ppp->optionlen += opt->len;
        }
    }
    return s;
}
//usage:we use this function to deal a lcp msg
static bool_t __LcpDeal(tagPPP *ppp, tagCH *ch, u8 *data, u16 len) {
    //now decode the lcp options here
    tagOH *opt;
    tagLcp *lcp;
    tagOpts *opts;
    u16 proto;
    u8 code;
    u8 id;
    tagOptItem *item;

    u8 status = 0;
    lcp = &ppp->lcp;
    proto = CN_PPP_LCP;
    switch (ch->code) {
        case CONFREQ:
            ppp->idpeer = ch->id;
            id = ch->id;
            opts = &lcp->peer;
            status = __OptReqDeal(ppp,opts, data, len);
            if (status == CN_OPT_ACK) {
                //PICKED SOME OPTIONS THE PEERMODE
                u32 v32;
                u16 v16;
                item = __OptItemMatch(&ppp->lcp.peer,LCP_OPT_AUTH);  //picked out the auth type
                opt = (tagOH *)item->buf;
                memcpy(&v16,opt->v,sizeof(v16));
                ppp->authproto = ntohs(v16);
                item = __OptItemMatch(&ppp->lcp.peer,LCP_OPT_MAGIC);  //picked out the magic
                opt = (tagOH *)item->buf;
                memcpy(&v32,opt->v,sizeof(v32));
                ppp->magicpeer = ntohs(v32);
                item = __OptItemMatch(&ppp->lcp.peer,LCP_OPT_ACCM);  //picked out the accm
                opt = (tagOH *)item->buf;
                memcpy(&v32,opt->v,sizeof(v32));
                ppp->accmpeer = ntohs(v32);
                //accept all the options,send the ack here
                code = CONFACK;
                __LcpSndCmd(ppp, code, id,data,len);
                lcp->status |= CN_CONF_PEERDONE;
                if ((lcp->status & CN_CONF_HOSTDONE)){
                    __ChangeMS(ppp,EN_PPP_AUTH);
                }
            }
            else {
                if (status == CN_OPT_REJ) {
                    code = CONFREJ;//DO THE REJECT
                }
                else {
                    code = CONFNAK; //do the uack
                }
                __LcpSndCmd(ppp,code, id,ppp->options, ppp->optionlen);
            }
            break;
        case CONFACK:
            opts = &lcp->host;
            status = CN_OPT_ACK;
            ppp->idhost++;
            __OptRspDeal(opts, data, len, status);
            lcp->status |= CN_CONF_HOSTDONE;
            if ((lcp->status & CN_CONF_PEERDONE))
            {
                __ChangeMS(ppp,EN_PPP_AUTH);
            }
            break;
        case CONFNAK:
            opts = &lcp->host;
            status = CN_OPT_NACK;
            code = CONFREQ;
            ppp->idhost++;
            __OptRspDeal(opts, data, len, status);
            __SndOptions(ppp, proto, code, ppp->idhost, opts->tab, opts->len,
                    0Xffffffff);
            break;
        case CONFREJ:
            opts = &lcp->host;
            status = CN_OPT_REJ;
            code = CONFREQ;
            ppp->idhost++;
            __OptRspDeal(opts, data, len, status);
            __SndOptions(ppp, proto, code, ppp->idhost, opts->tab, opts->len,
                    0Xffffffff);
            break;
        case TERMREQ:
            ppp->idpeer = ch->id;
            //we should send the term ack and turn to the down state
            __LcpSndCmd(ppp, TERMACK, ppp->idpeer, data, len);
            if (ppp->ms.stat == EN_PPP_NETWORK) {
                //here we call the uplayer that the link is down
                if(NULL != ppp->routwan)
                {
                    RouterRemoveByHandle(ppp->routwan);
                    ppp->routwan = NULL;
                }
                NetDev_PostEvent(ppp->fdnet, EN_NETDEVEVENT_IPRELEASE);
            }
            __ChangeMS(ppp,EN_PPP_DEAD);
            TCPIP_DEBUG_INC(ppp->debug.termreq);
            break;
        case TERMACK:
            //goto dead
            __ChangeMS(ppp,EN_PPP_DEAD);
            break;
        case CODEREJ:
            break;
        case PROTOREJ:
            break;
        case ECHOREQ:
            ppp->idpeer = ch->id;
            __LcpSndCmd(ppp, ECHOREP, ppp->idpeer, data, len);
            break;
        case ECHOREP:
            ppp->idhost++;
            ppp->ms.timeoutnum = 0;
            break;
        case DISREQ:
            break;
        default:  //unknown the code
            break;
    }
    return true;
}

//usage:use this function to send our auth
static bool_t __AuthSndRequest(tagPPP *ppp) {
    u16 len;
    tagCH ch;
    u8 *dst;
    if (ppp->authproto == CN_PPP_PAP) //USE USERNAME AND PASSWD
    {
        len = strlen(ppp->user) + strlen(ppp->passwd) + 2;
        dst = ppp->options;
        if (len < CN_OPTION_BUFLEN) {
            *dst++ = strlen(ppp->user);
            memcpy(dst, ppp->user, strlen(ppp->user));
            dst += strlen(ppp->user);
            *dst++ = strlen(ppp->passwd);
            memcpy(dst, ppp->passwd, strlen(ppp->passwd));
            ppp->optionlen = len;
            dst += strlen(ppp->passwd);
            ch.code = CONFREQ;
            ch.id = ppp->idhost;
            ch.len = htons(len + CN_PPPOS_CHSIZE);
            __IoDevOut(ppp, CN_PPP_PAP, &ch, ppp->options, ppp->optionlen,
                    ppp->accmpeer);
        }
    }
    return true;
}
//usage:we use this function to deal a auth msg
static bool_t __PapDeal(tagPPP *ppp, tagCH *ch, u8 *data, u16 len) {
    //now do the decode now
    if (ch->code == CONFACK) {
        //we has passed ,goto the ncp
        __ChangeMS(ppp, EN_PPP_NCP);
    }
    else {
        __ChangeMS(ppp, EN_PPP_TERMINATE);
    }
    return true;
}
#pragma GCC diagnostic pop

static u32 ModemIP;
//when we got a ppp frame, we use this function to deal it
//we should know which proto and the data and datalen we got
static bool_t __NcpDeal(tagPPP *ppp, tagCH *ch, u8 *data, u16 len) {
    u8 status;
    tagNcp *ncp;
    tagOpts *opts;
    tagOptItem *item;
    tagOH  *opt;
    u32 subnet,ip,submask;
    tagRouterPara para;

    ncp = &ppp->ncp;
    opts = &ncp->net;
    ppp->idhost++;
    switch (ch->code) {
        case CONFREQ: //if we receive the request,just ack it
            ch->code = CONFACK;
            __IoDevOut(ppp,CN_PPP_IPCP,ch,data,len,ppp->accmpeer);
            break;
        case CONFACK:
            status = CN_OPT_ACK;
            __OptRspDeal(opts, data, len, status);
            //picked out the ip and dns here
            u32 v32;
            item = __OptItemMatch(&ppp->ncp.net,NCP_OPT_IP);  //picked out the auth type
            opt = (tagOH *)item->buf;
            memcpy(&v32,opt->v,sizeof(v32));
            ppp->ipaddr = v32;

            item = __OptItemMatch(&ppp->ncp.net,NCP_OPT_DNS);  //picked out the magic
            opt = (tagOH *)item->buf;
            memcpy(&v32,opt->v,sizeof(v32));
            ppp->dnsaddr = v32;

            ModemIP=ppp->ipaddr;
            //这里同时设置dns到网卡里
            NetDev_SetDns(EN_IPV_4, ppp->fdnet, &ppp->dnsaddr, &ppp->dnsaddr);
            NetDev_SetGateway(EN_IPV_4, ppp->fdnet, &ppp->dnsaddr);
//            RoutSetDefaultAddr(EN_IPV_4, ppp->ipaddr, 0xFFFFFFFF, ppp->ipaddr,ppp->dnsaddr);
            //turn to another state
            memset(&para,0,sizeof(para));
            if(NULL != ppp->routwan)
            {
                RouterRemoveByHandle(ppp->routwan);
            }

            memset(&para,0,sizeof(para));
            ip      = ppp->ipaddr;
            submask = INADDR_NONE;
//          hop     = ppp->dnsaddr;
//            dns     = ppp->dnsaddr;
//            dnsbak  = ppp->dnsaddr;

            subnet = INADDR_ANY;
            para.ver = EN_IPV_4;
            para.ifname = ppp->namenet;
            para.mask = &submask;
            para.net = &subnet;
            para.host = &ip;
//          para.hop = &hop;
//          para.dns = &dns;
//          para.dnsbak = &dnsbak;
            para.prior = CN_ROUT_PRIOR_UNI;
            para.flags = 0;

            ppp->routwan = RouterCreate(&para);
            __ChangeMS(ppp, EN_PPP_NETWORK);
            //here we call the uplayer that the ip get here
            NetDev_PostEvent(ppp->fdnet, EN_NETDEVEVENT_IPGET);
            break;
        case CONFNAK:
            //modify the request and resend the request
            status = CN_OPT_NACK;
            __OptRspDeal(opts, data, len, status);
            __SndOptions(ppp, CN_PPP_IPCP, CONFREQ, ppp->idhost, ncp->net.tab,
                    ncp->net.len, ppp->accmpeer);
            break;
        case CONFREJ:
            //modify the request and resend the request
            status = CN_OPT_REJ;
            __OptRspDeal(opts, data, len, status);
            __SndOptions(ppp, CN_PPP_IPCP, CONFREQ, ppp->idhost, ncp->net.tab,
                    ncp->net.len, ppp->accmpeer);
            break;
        default:
            break;
    }
    return true;
}
//usage:this function used for the ModemIP Get
char *Get_ModemIP(void)
{
    char *addr_str;
    struct in_addr addr;
    if(ModemIP == 0)
   {
        printf("There is currently no IP, and the dialing may not be successful");
        return NULL;
    }
    addr.s_addr = ModemIP;

    addr_str = inet_ntoa(addr);

    return addr_str;
}

//usage:use this function to deal the receive message here
static void __FrameDeal(tagPPP *ppp, u16 proto, tagCH *ch, u8 *data, u16 len) {
    //we receive a message from the peer, we should deal it carefully
    //if debug here
    if (ppp->debug.endebug) {
        time_t logtime;
        logtime = time(NULL);
        debug_printf("PPP","[%s:R:%04d]", ctime(&logtime),
                ppp->rcvbuf.lenbuffer);
        for (int i = 0; i < ppp->rcvbuf.lenbuffer; i++) {
            debug_printf("PPP","%02x ", ppp->rcvbuf.buffer[i]);
        }
        printf("\n\r");
    }
    if (ppp->ms.stat > EN_PPP_DEAD) {
        switch (proto) {
            case CN_PPP_LCP: //this is a lcp frame,so we should deal it with the LCP
                __LcpDeal(ppp, ch, data, len);
                break;
            case CN_PPP_PAP:
                __PapDeal(ppp, ch, data, len);
                break;
            case CN_PPP_CHAP:
                __PapDeal(ppp, ch, data, len);
                break;
            case CN_PPP_IPCP:
                __NcpDeal(ppp, ch, data, len);
                break;
            case CN_PPP_IP:
                __IpDeal(ppp, ch, data, len);
                break;
            default:
                //maybe a unknown proto, we should send a protorej message to the peer
                break;
        }
    }
}
static void __TimeoutCheck(tagPPP *ppp) {
    s64 timenow;
    u32 v32;
    //maybe timeout, we should check if any ppp to do the timeout resend
    timenow = DJY_GetSysTime();
    switch (ppp->ms.stat) {
        case EN_PPP_DEAD:
            break;
        case EN_PPP_ESTABLISH:
            //we should check if the config request timeout
            if (timenow > ppp->ms.timedead) {
                if (ppp->ms.timeoutnum < CN_TIMEOUT_LIMIT) {
                    __SndOptions(ppp, CN_PPP_LCP, CONFREQ, ppp->idhost,
                            ppp->lcp.host.tab, ppp->lcp.host.len,
                            0Xffffffff);
                    ppp->ms.timedead = timenow + CN_LCP_TIME;
                    ppp->ms.timeoutnum++;
                }
                else {
                    __ChangeMS(ppp, EN_PPP_TERMINATE);
                    TCPIP_DEBUG_INC(ppp->debug.timerst);
                }
            }
            break;
        case EN_PPP_AUTH:
            //we should check if the auth timeout
            if (timenow > ppp->ms.timedead) {
                if (ppp->ms.timeoutnum < CN_TIMEOUT_LIMIT) {
                    __AuthSndRequest(ppp);
                    ppp->ms.timedead = timenow + CN_AUTH_TIME;
                    ppp->ms.timeoutnum++;
                }
                else {
                    __ChangeMS(ppp, EN_PPP_TERMINATE);
                    TCPIP_DEBUG_INC(ppp->debug.timerst);
                }
            }
            break;
        case EN_PPP_NCP:
            //we should check if the ncp timeout
            if (timenow > ppp->ms.timedead) {
                if (ppp->ms.timeoutnum < CN_TIMEOUT_LIMIT) {
                    __SndOptions(ppp, CN_PPP_IPCP, CONFREQ, ppp->idhost,
                            ppp->ncp.net.tab, ppp->ncp.net.len,ppp->accmpeer);
                    ppp->ms.timedead = timenow + CN_NCP_TIME;
                    ppp->ms.timeoutnum++;
                }
                else {
                    __ChangeMS(ppp, EN_PPP_TERMINATE);
                    TCPIP_DEBUG_INC(ppp->debug.timerst);
                }
            }
            break;
        case EN_PPP_NETWORK:
            //here we should check if any echo request timeout
            if (timenow > ppp->ms.timedead) {
                if (ppp->ms.timeoutnum < CN_TIMEOUT_LIMIT) {
                    v32 = htonl(ppp->magichost);
                    __LcpSndCmd(ppp, ECHOREQ, ppp->idhost,(u8*)&v32,sizeof(v32));
                    ppp->ms.timedead = timenow + CN_IP_TIME;
                    ppp->ms.timeoutnum++;
                }
                else {
                    __ChangeMS(ppp, EN_PPP_TERMINATE);
                    //here we call the uplayer that the link is down
                    if(NULL != ppp->routwan)
                    {
                        RouterRemoveByHandle(ppp->routwan);
                        ppp->routwan = NULL;
                    }
                    NetDev_PostEvent(ppp->fdnet, EN_NETDEVEVENT_IPRELEASE);
                    TCPIP_DEBUG_INC(ppp->debug.timerst);
                }
            }
            break;
        case EN_PPP_TERMINATE:
            //do the reset and shut down
            if (timenow > ppp->ms.timedead) {
                if (ppp->ms.timeoutnum < CN_TIMEOUT_LIMIT) {
                    v32 = htonl(ppp->magichost);
                    __LcpSndCmd(ppp, TERMREQ, ppp->idhost, (u8*)&v32,sizeof(v32));
                    ppp->ms.timedead = timenow + CN_NCP_TIME;
                    ppp->ms.timeoutnum++;
                }
                else {
                    __ChangeMS(ppp, EN_PPP_DEAD);
                }
            }
            break;
        default:
            //do the reset and shut down
            break;
    }
}
////////////////////////THE FOLLOWING USED TO DECODE THE LINK FRAME////////////
//usage:this is the receive machine state,continuing to decode the data received
//      if receive a good frame,then upload it to the upper layer
static void __DataDecode(tagPPP *ppp, u8 *buf, u16 len) {
    tagRBC *rbc;
    u8 curchar;
    rbc = &ppp->rbc;
    u16 i;
    for (i = 0; i < len; i++) {
        curchar = buf[i];
        if ((rbc->state == EN_CHECK_START) && (curchar != CN_PPP_FLAG)) {
            continue;  //only care about the start flag:0x7e
        }
        if (curchar == CN_PPP_FLAG) {
            __RstRBC(ppp);
            rbc->state = EN_CHECK_STATION;
            continue;
        }
        if ((curchar == CN_PPP_ESCAPE) && (false == rbc->espace)) {
            rbc->espace = true; //if it is the char espace
            continue;
        }
        if (rbc->espace) {
            curchar ^= CN_PPP_TRANS;
            rbc->espace = false;
        }
        //all other bytes will be write to the receive buffer
        ppp->rcvbuf.buffer[ppp->rcvbuf.lenbuffer] = curchar;
        ppp->rcvbuf.lenbuffer++;
        switch (rbc->state) {
            case EN_CHECK_STATION:
                if (curchar == CN_PPP_ALLSTATIONS) {
                    rbc->state = EN_CHECK_CTRL; //got the station here
                }
                else if (curchar == CN_PPP_UI) {
                    rbc->state = EN_CHECK_PROTO_H; //the station has been compressed
                }
                else if (0 == (curchar & 0x01)) {
                    rbc->proto = curchar << 8;
                    rbc->state = EN_CHECK_PROTO_L; //the station and ui has been compressed
                }
                else {
                    rbc->proto = curchar;
                    rbc->state = EN_CHECK_CDR_CODE;
                }
                break;
            case EN_CHECK_CTRL:
                if (curchar == CN_PPP_UI) {
                    rbc->state = EN_CHECK_PROTO_H; //the station has been compressed
                }
                else if (0 == (curchar & 0x01)) {
                    rbc->proto = curchar << 8;
                    rbc->state = EN_CHECK_PROTO_L; //the station and ui has been compressed
                }
                else {
                    rbc->proto = curchar;
                    rbc->state = EN_CHECK_CDR_CODE;
                }
                break;
            case EN_CHECK_PROTO_H:
                rbc->proto = curchar << 8;
                rbc->state = EN_CHECK_PROTO_L; //the station and ui has been compressed
                break;
            case EN_CHECK_PROTO_L:
                rbc->proto |= curchar;
                rbc->state = EN_CHECK_CDR_CODE; //the station and ui has been compressed
                break;
            case EN_CHECK_CDR_CODE:
                rbc->ch = (tagCH *) &ppp->rcvbuf.buffer[ppp->rcvbuf.lenbuffer
                        - 1];
                rbc->state = EN_CHECK_CDR_ID;
                break;
            case EN_CHECK_CDR_ID:
                rbc->state = EN_CHECK_LEN1;
                break;
            case EN_CHECK_LEN1:
                rbc->len = curchar << 8;
                rbc->state = EN_CHECK_LEN2;
                break;
            case EN_CHECK_LEN2:
                rbc->len = (rbc->len & 0xff00) + curchar;
                rbc->data = (void *) &ppp->rcvbuf.buffer[ppp->rcvbuf.lenbuffer];
                if ((rbc->len > PPP_MTU) || (rbc->len < CN_PPPOS_CHSIZE)) {
                    __RstRBC(ppp); //if big enough then do the reset
                }
                else if (rbc->len == CN_PPPOS_CHSIZE) {
                    rbc->state = EN_CHECK_FCS16_1;
                }
                else {
                    rbc->state = EN_CHECK_DATA;
                }
                break;
            case EN_CHECK_DATA: //receive until we get enough data here
                rbc->datalen++;
                if (rbc->datalen == (rbc->len - CN_PPPOS_CHSIZE))
                        {
                    rbc->state = EN_CHECK_FCS16_1; //got all the data now
                }
                break;
            case EN_CHECK_FCS16_1:
                rbc->fcs16 = curchar;
                rbc->state = EN_CHECK_FCS16_2;
                break; //just do the FCS16
            case EN_CHECK_FCS16_2:
                rbc->fcs16 = rbc->fcs16 + (curchar << 8);
                //should check if the frame good here
                u16 fcs;
                FCS16 fcs16;
                FCS16_Init(&fcs16);
                //we would not and the start flag and the fcs to the fcs compute
                FCS16_Update(&fcs16, &ppp->rcvbuf.buffer[1],
                        ppp->rcvbuf.lenbuffer - 3);
                FCS16_Final(&fcs16, &fcs);
                TCPIP_DEBUG_INC(ppp->debug.rcvframe);
                if (fcs == rbc->fcs16) {
                    __FrameDeal(ppp, rbc->proto, rbc->ch, rbc->data,rbc->datalen);
                }
                else {
                    TCPIP_DEBUG_INC(ppp->debug.rcvfcserr); //bad frame
                }
                //we should do the reset here for another accept
                __RstRBC(ppp);
                break;
            default: //could never reach here
                break;
        } //end for the switch
    }
    return;
}
//del a ppp
static void __PppDevDel(tagPPP *ppp) {
    Lock_MutexDelete(ppp->sndmutex);
    net_free((void *) ppp->ncp.net.tab);
    net_free((void *) ppp->lcp.peer.tab);
    net_free((void *) ppp->lcp.host.tab);
    net_free((void *) ppp);
}
//use this function to deal with the ppp dial here
ptu32_t __TFTP_ClientMain(void) {
    tagPPP *ppp;
    DJY_GetEventPara((ptu32_t *) &ppp, NULL);
    if (NULL == ppp) {
        debug_printf("PPP","%s:para invalid\n\r", __FUNCTION__);
        return 0;
    }
    //add a net device here
    if (false == __NetDevAdd(ppp)) {
        debug_printf("PPP","%s:make the netdev:%s failed\n\r", __FUNCTION__, ppp->namenet);
        __PppDevDel(ppp);
        return 0;
    }
    pPPPClient = ppp;
    //we here should do the at regnet
    while (1) {
        //here we're waiting for the at reg start commadnd,which means
        while (ppp->start == false ) {
            DJY_EventDelay(1000*mS);
        }
        //first should do the reset here
        __MSRst(ppp);
        //use the at command to register the module
        if (NULL != ppp->fnio2ppp) {
            if (ppp->fnio2ppp(ppp->nameio, ppp->apn)) {
                NetDev_PostEvent(ppp->fdnet, EN_NETDEVEVENT_LINKUP);
                debug_printf("PPP","%s:CHANGE2PPPMODE SUCCESS\n\r", __FUNCTION__);
            }
            else {
                NetDev_PostEvent(ppp->fdnet, EN_NETDEVEVENT_LINKDOWN);
                debug_printf("PPP","%s:CHANGE2PPPMODE FAILED\n\r", __FUNCTION__);
                continue; //wait for another time
            }
        }
        //here we reset the machine state and do the ppp negotiation
        __ChangeMS(ppp, EN_PPP_ESTABLISH);
        ppp->fdio = iodevopen(ppp->nameio);
        if (0 == ppp->fdio) {
            debug_printf("PPP","%s:OPEN nameio:%s FAILED\n\r", __FUNCTION__, ppp->nameio);
            continue;
        }
        while (ppp->ms.stat != EN_PPP_DEAD) {
            ppp->rcvbuf.lencache = iodevread(ppp->fdio, ppp->rcvbuf.cache,
            CN_FRAME_CACHELEN, 0);
            if (ppp->rcvbuf.lencache > 0) {
                __DataDecode(ppp, ppp->rcvbuf.cache, ppp->rcvbuf.lencache);
            }
            else {
                DJY_EventDelay(10 * mS); //do some wait here
            }
            __TimeoutCheck(ppp);      //check the machine state
        }
        //here close the device
        iodevclose(ppp->fdio);
        ppp->fdio = 0;
    }
    return 0;
}
//usage:use this function to start the ppp dev
bool_t PppDevLinkStart(void) {
    if (NULL != pPPPClient) {
        pPPPClient->start = true;
    }
    return true;
}
//usage:use this function to stop the ppp dev
bool_t PppDevLinkStop(void) {
    if (NULL != pPPPClient) {
        pPPPClient->start = false;
    }
    return true;
}
//usage:use this function to check if the ppp dial is ok or not
//      this function is a block function here
bool_t PppDevLinkIsOk(void) {
    bool_t result = false;
    if ((NULL != pPPPClient) && (pPPPClient->ms.stat == EN_PPP_NETWORK)) {
        result = true;
    }
    return result;
}
//usage:use this function to reset the ppp dev,as it create state
bool_t PppDevLinkRst(void) {
    //we will do another  register and  ppp negotiation
    if (NULL != pPPPClient) {
        pPPPClient->start = false;
        pPPPClient->ms.stat = EN_PPP_DEAD;
        //do the reset here
        if (NULL != pPPPClient->fniorst) {
            pPPPClient->fniorst();
        }
    }
    return true;
}
//usage:this is the ppp cmd for the os shell

static void __PppUsage(void) {
        debug_printf("PPP","usage:ppp help (print the usage)\n\r"
            "      ppp debug on/off (en or disen the io debug)\n\r"
            "      ppp status (show the current ppp state)\n\r");
    return;
}
//show the ppp state
static void __PppShow(tagPPP *ppp)
{
    debug_printf("PPP","cfg:nameio:%s namenet:%s user:%s passwd:%s apn:%s\n\r",
            (char *)ppp->nameio,(char *)ppp->namenet,(char *) ppp->user,(char *)ppp->passwd,(char *)ppp->apn);
    debug_printf("PPP","cfghook:io2pp:0x%08x iorst:0x%08x event:0x%08x\n\r",
            (u32) ppp->fnio2ppp, (u32) ppp->fniorst, (u32) ppp->fnevent);
    debug_printf("PPP","dev:fdio:0x%08x fdnet:0x%08x\n\r", ppp->fdio, (u32)ppp->fdnet);
    debug_printf("PPP","MS  :%s\n\r",gPppMsName[ppp->ms.stat]);
    debug_printf("PPP","MSTO:%d\n\r",ppp->ms.timeoutnum);
    debug_printf("PPP","MSTL:%d\n\r",ppp->ms.timeoutlimit);
    debug_printf("PPP","MSTD:%lld\n\r",ppp->ms.timedead);
    //show howmany frams we has get
    //IP FRAME GOT
    debug_printf("PPP","IPFRAME(bytes) R:%lld  S:%lld\n\r",ppp->debug.iprcvlen,ppp->debug.ipsndlen);
    debug_printf("PPP","IODRAME:RT:%d RERR:%d  ST:%d SERR:%d\n\r",\
            ppp->debug.rcvframe,ppp->debug.rcvfcserr,ppp->debug.sndframe,ppp->debug.sndioerr);
    debug_printf("PPP","TERREQUEST:%d  TIMEOUTNUM:%d\n\r",ppp->debug.termreq,ppp->debug.timerst);

}
//static bool_t __PppShellCmd(char *param)
bool_t ppp(char *param)
{
    int argc = 4;
    char*argv[4];
    if (NULL == param) {      //SHOW THE PPP USAGE
        __PppUsage();
    }
    else
    {
        string2arg(&argc, argv, param);
        if (0 == strcmp("debug", argv[0])) {
            if (0 == strcmp("on", argv[1])) {
                pPPPClient->debug.endebug = true;
            }
            else {
                pPPPClient->debug.endebug = false;
            }
        }
        else if (0 == strcmp("status", argv[0])) {
            if (NULL != pPPPClient) {
                __PppShow(pPPPClient);
            }
            else {
                debug_printf("PPP","No PPP Client Here\n\r");
            }
        }
        else {
            __PppUsage();
        }
    }
    return true;
}

//this is the initialize function for the ppp module
static u16 gPppEvttID = CN_EVTT_ID_INVALID;
#define     CN_PPP_TASKSTACKSIZE      0x800     //the ppp task stack size
#define     CN_PPP_TASKPRIOR          190       //the ppp task prior
#define     CN_PPP_DEVLIMIT           1         //how many ppp device could be add

bool_t PppInit(void) {
    bool_t result = false;
    //initialize the at and io modem
//    extern bool_t PppIoInit(ptu32_t para);
//    PppIoInit(0);
//    extern bool_t PppAtInit(ptu32_t para);
//    PppAtInit(0);
    gPppEvttID = DJY_EvttRegist(EN_INDEPENDENCE, CN_PPP_TASKPRIOR, 0, CN_PPP_DEVLIMIT,
            __TFTP_ClientMain, NULL, CN_PPP_TASKSTACKSIZE, "PPPCLIENT");
    if (gPppEvttID == CN_EVENT_ID_INVALID) {
        return result;
    }

    //install the debug shell for the system
        return (TRUE);

    return result;
}
//add the device to the task list
static bool_t __PppAddTask(tagPPP *ppp) {
    bool_t result = false;
    u16 eventID;

    if ((NULL != ppp) && (CN_EVTT_ID_INVALID != gPppEvttID)) {
        eventID = DJY_EventPop(gPppEvttID, NULL, 0, (ptu32_t) ppp, 0, 0);
        if (CN_EVENT_ID_INVALID != eventID) {
            pPPPClient = ppp;
            result = true;
        }
    }
    return result;
}

//just do the configuration and pop a task to do it
bool_t PppDevAdd(char *namenet, char *nameio, const char *user,
        const char *passwd, const char *apn, bool_t (*fnIoRst)(void),
        bool_t (*fnIo2Ppp)(char *devname, char *apn),
        fnNetDevEventHook eventhook) {
    bool_t result = false;
    tagPPP *ppp;
    //we use the dynamic memory for the ppp client
    ppp = net_malloc(sizeof(tagPPP));
    if (NULL == ppp) {
        debug_printf("PPP","%s:PPP Mem Err\n\r", __FUNCTION__);
        goto EXIT_PPPMEM;
    }
    memset(ppp, 0, sizeof(tagPPP));
    //net_malloc the opts for the lcp and ncp
    ppp->lcp.host.tab = net_malloc(EN_LCP_OPT_POSLAST * sizeof(tagOptItem));
    if (NULL == ppp->lcp.host.tab) {
        debug_printf("PPP","%s:PPP HOSTOPTS Mem Err\n\r", __FUNCTION__);
        goto EXIT_LCPHOSTOPTMEM;
    }
    ppp->lcp.host.len = EN_LCP_OPT_POSLAST;

    ppp->lcp.peer.tab = net_malloc(EN_LCP_OPT_POSLAST * sizeof(tagOptItem));
    if (NULL == ppp->lcp.peer.tab)
            {
        debug_printf("PPP","%s:PPP PEEROPTS Mem Err\n\r", __FUNCTION__);
        goto EXIT_LCPPEEROPTMEM;
    }
    ppp->lcp.peer.len = EN_LCP_OPT_POSLAST;

    ppp->ncp.net.tab = net_malloc(EN_NCP_OPT_POSLAST * sizeof(tagOptItem));
    if (NULL == ppp->ncp.net.tab)
            {
        debug_printf("PPP","%s:PPP PEEROPTS Mem Err\n\r", __FUNCTION__);
        goto EXIT_NCPOPTMEM;
    }
    ppp->ncp.net.len = EN_NCP_OPT_POSLAST;

    ppp->sndmutex = Lock_MutexCreate(NULL);
    if (NULL == ppp->sndmutex) {
        debug_printf("PPP","%s:mutex Err\n\r", __FUNCTION__);
        goto EXIT_TXBUF_MUTEX;
    }
    //initialize the ppp control block and  add it to the ppp task
    if(NULL != apn){
        strncpy(ppp->apn, apn, CN_NAME_LIMIT);
    }
    if(NULL != user){
        strncpy(ppp->user, user, CN_NAME_LIMIT);
    }
    if(NULL != passwd){
        strncpy(ppp->passwd, passwd, CN_NAME_LIMIT);
    }
    if(NULL != nameio){
        strncpy(ppp->nameio, nameio, CN_NAME_LIMIT);
    }
    if(NULL != namenet){
        strncpy(ppp->namenet, namenet, CN_NAME_LIMIT);
    }
    ppp->fnio2ppp = fnIo2Ppp;
    ppp->fniorst = fnIoRst;
    ppp->fnevent = eventhook;

    //pop a ppp virtual engine
    result = __PppAddTask(ppp);
    if (false == result) {
        debug_printf("PPP","%s:TASK ADD ERR\n\r", __FUNCTION__);
        goto EXIT_PPPTASK;
    }
    return result;

    EXIT_PPPTASK: Lock_MutexDelete(ppp->sndmutex);
    EXIT_TXBUF_MUTEX:
    net_free((void *) ppp->ncp.net.tab);
    EXIT_NCPOPTMEM:
    net_free((void *) ppp->lcp.peer.tab);
    EXIT_LCPPEEROPTMEM:
    net_free((void *) ppp->lcp.host.tab);
    EXIT_LCPHOSTOPTMEM:
    net_free((void *) ppp);
    EXIT_PPPMEM: return result;
}
ADD_TO_ROUTINE_SHELL(ppp,ppp,"usage:ppp [subcmd subparam]/help");

