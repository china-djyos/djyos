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
// Copyright (c) 2014 ����Ȩ�����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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
//author:zhangqf
//date  :����4:43:13/2017��1��4��
//usage :
//-----------------------------------------------------------------------------

//standard includes
#include "ppp.h"

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <os.h>
//add your own specified header here
#include <sys/socket.h>
//we only make one task here:rcv then deal, then check the timeout
#include "iodev.h"
#include "osarch.h"


#include "../../tcpipconfig.h"
/*
 * FCS lookup table as calculated by genfcstab.
 */
static const u16 fcstab[256] = {
  0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
  0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
  0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
  0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
  0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
  0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
  0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
  0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
  0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
  0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
  0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
  0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
  0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
  0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
  0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
  0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
  0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
  0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
  0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
  0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
  0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
  0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
  0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
  0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
  0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
  0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
  0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
  0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
  0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
  0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
  0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
  0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};
/*
 * The basic PPP frame.
 */
#define PPP_HDRLEN      4       /* octets for standard ppp header */
#define PPP_FCSLEN      2       /* octets for FCS */
/*
 * Significant octet values.
 */
#define PPP_ALLSTATIONS 0xff    /* All-Stations broadcast address */
#define PPP_UI          0x03    /* Unnumbered Information */
#define PPP_FLAG        0x7e    /* Flag Sequence */
#define PPP_ESCAPE      0x7d    /* Asynchronous Control Escape */
#define PPP_TRANS       0x20    /* Asynchronous transparency modifier */
/*
 * Protocol field values.
 */
#define PPP_IP          0x21    /* Internet Protocol */
#define PPP_AT          0x29    /* AppleTalk Protocol */
#define PPP_VJC_COMP    0x2d    /* VJ compressed TCP */
#define PPP_VJC_UNCOMP  0x2f    /* VJ uncompressed TCP */
#define PPP_COMP        0xfd    /* compressed packet */
#define PPP_IPCP        0x8021  /* IP Control Protocol */
#define PPP_ATCP        0x8029  /* AppleTalk Control Protocol */
#define PPP_CCP         0x80fd  /* Compression Control Protocol */
#define PPP_LCP         0xc021  /* Link Control Protocol */
#define PPP_PAP         0xc023  /* Password Authentication Protocol */
#define PPP_LQR         0xc025  /* Link Quality Report protocol */
#define PPP_CHAP        0xc223  /* Cryptographic Handshake Auth. Protocol */
#define PPP_CBCP        0xc029  /* Callback Control Protocol */
/*
 * Values for FCS calculations.
 */
#define PPP_INITFCS     0xffff  /* Initial FCS value */
#define PPP_GOODFCS     0xf0b8  /* Good final FCS value */
#define PPP_FCS(fcs, c) (((fcs) >> 8) ^ fcstab[((fcs) ^ (c)) & 0xff])
/*
 *  CP (LCP, IPCP, etc.) codes.
 */
#define CONFREQ     1 /* Configuration Request */
#define CONFACK     2 /* Configuration Ack */
#define CONFNAK     3 /* Configuration Nak */
#define CONFREJ     4 /* Configuration Reject */
#define TERMREQ     5 /* Termination Request */
#define TERMACK     6 /* Termination Ack */
#define CODEREJ     7 /* Code Reject */
#define PROTOREJ    8 /* protocol reject */
#define ECHOREQ     9 /* echo request */
#define ECHOREP     10/* echo reply */
#define DISREQ      11/* discard request*/
//lcp options supported here
#define LCP_OPT_MRU     1
#define LCP_OPT_ACCM    2
#define LCP_OPT_AUTH    3
#define LCP_OPT_QUAL    4
#define LCP_OPT_MAGIC   5
#define LCP_OPT_PFC     7
#define LCP_OPT_ACFC    8
#define PPP_MTU         1500     /* Default MTU (size of Info field) */

#define CN_PPP_BUFLEN   1600
//usage:this data structure used for the send control
typedef struct
{
	u16      datalen;
	u16      buflen;
	u8       buf[CN_PPP_BUFLEN];
	mutex_t  mutex;   //used for the multi thread
	u16      fcs;
}tagPppSC;
//usage:use this data structure for the lcp negotiation  options
#define CN_OPTION_BUFLEN         64
typedef struct
{
	u32      mru;
	u32      accm;
	u32      macgic;
	u16      authcode;
	u8       authencode;
	u8       optionrej[CN_OPTION_BUFLEN];
	u8       rejlen;
	u8       optionnack[CN_OPTION_BUFLEN];
	u8       naklen;
	u8       optionack[CN_OPTION_BUFLEN];
	u8       acklen;
	u8       done;
}tagLcpPeerOptions; //when we receive the configure request, we use this data structure to decode
typedef struct
{
	u32      enmru:1;
	u32      enaccm:1;
	u32      enmagic:1;
	u32      enauth:1;
	u32      mru;
	u32      accm;
	u32      macgic;
	u16      authcode;
	u8       authencode;
	u8       id;
	u8       done;
	u8       buf[CN_OPTION_BUFLEN];
	u8       buflen;
	u8       datalen;
}tagLcpHostStat;
//usage:this data structure used for the lcp
typedef struct
{
	tagLcpPeerOptions   peer;    //the peer options
	tagLcpHostStat      host;    //the peer options
}tagLcp;
//usage:this data structure used for the auth
#define CN_NAME_LIMIT  32
typedef struct
{
	u8    id;
}tagAuth;
//usage:this data structure used for the ncp
typedef struct
{
	u32 enIp:1;
	u32 enDns:1;
	u32 enDnsBak:1;
	u32 enNbns:1;
	u32 enNbnsBak:1;
	u32 ip;
	u32 dns;
	u32 dnsbak;
	u32 nbns;
	u32 nbnsbak;
	u8  id;
	u8  buf[CN_OPTION_BUFLEN];
	u8  datalen;
	u8  buflen;
}tagNcp;
#define NCP_OPT_VJTCP 2
#define NCP_OPT_IP    3
#define NCP_OPT_DNS   0X81
#define NCP_OPT_NBNS  0X82
#define NCP_OPT_DNS2  0X83
#define NCP_OPT_NBNS2 0X84

//usage:this data struct used for the ip control
typedef struct
{
	//for the ip statistics
	u64                 ipflow_d;      //all the ip flow download:unites:bytes
	u64                 ipflow_u;      //all the ip flow upload:unites:bytes
}tagIp;

#pragma pack(1)
typedef struct
{
	u8   code;
	u8   id;
	u16  len;
}tagCHdr;
#define CN_PPPOS_CHDRSIZE   sizeof(tagCHdr)
//usage:we use this data structure to do the negotiation decode
typedef struct
{
	u8 type;
	u8 len;
	u8 v[0];
}tagPppOptions;
#define CN_PPPOS_OPTSIZE sizeof(tagPppOptions)
typedef struct
{
	u8  a;
	u8  c;
	u16 p;
}tagPppHdr;
#define CN_PPP_HDRSIZE  (sizeof(tagPppHdr))
#pragma pack()


//usage:use this header for the receive control
#define CN_PPP_RBUF  1600
//usage:use this function to decode the input data and check the frame we got
typedef enum
{
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
	EN_CHECK_FCS1,        //check the fcs1
	EN_CHECK_FCS2,        //check the fcs2
	EN_CHECK_UNKNOWN,     //illegal state
}enCheckStat;
typedef struct
{
	u8  data[CN_PPP_RBUF];
	u16 datalen;
	u16 drops;
	u16         fcs;
	enCheckStat state;
	tagCHdr     chdr;
	u16         proto;
	bool_t      inespace;
}tagPppRC;    //used for the receive control

//defines for the machine state
#define CN_LCP_TIME        (10*1000*mS)    //lcp  timeout
#define CN_AUTH_TIME       (10*1000*mS)    //auth timeout
#define CN_NCP_TIME        (10*1000*mS)    //ncp  timeout
#define CN_IP_TIME         (50*1000*mS)    //ip   timeout:if timeout will send the lcp echo
#define CN_MS_TIMEOUT      (5*1000*mS)     //each timeout,we will retrive it
#define CN_TIMEOUT_LIMIT   10              //timeout limittimes,if over,then will terminate and goto dead state
typedef enum
{
	EN_PPP_DEAD= 0,          //the link has not established yet
	EN_PPP_ESTABLISH,        //do the lcp negotiation
	EN_PPP_AUTH,             //do the authentication
	EN_PPP_NCP,              //do the net negotiation
	EN_PPP_NETWORK,          //do the normal net work
	EN_PPP_TERMINATE,        //do the normal shutdown
}enPppStat;
const char *gPppStatName[]={"DEAD","ESATBLISH","AUTH","NCP","NETWORK","TERMINAL"};
typedef struct
{
	enPppStat  stat;          //used for the machine state
	s64        timedead;      //the timeout time by the system
	u32        timeoutnum;    //continuing timeout times
	u32        timeoutlimit;  //continuing timeout times limit,if overlap,goto dead state
}tagPppMs;

//the following defines for the reg the sim card to the station
typedef bool_t (*fnModemReset)(void);
typedef struct
{
//	bool_t              pppmode;     //true for the ppp else for the debug mode(AT)
//	bool_t              regnetstop;  //check if we should continous register the net or not
//	u32                 regnettimes; //the times we register to the net
	bool_t              regnetstart; //which means we begin to start the reg
	bool_t              regnetok;
	bool_t              regnetauto;  //which means we begin to auto register the network
	fnModemReset        modemreset;
	fnAtRegNet          regnet;
}tagPppReg;  //used for the ppp register to the net control
typedef struct
{
	u64 fall;
	u64 fcserr;
}tagStatistics;
//usage:this data struct used for the ppp control
typedef struct
{
	void               *nxt;     //do the list
	ptu32_t             iofd;    //the io device fd
	ptu32_t             nfd;     //the net device fd
	tagLcp              lcp;     //used for the lcp
	tagAuth             auth;    //used for the auth
	tagNcp              ncp;     //used for the ncp
	tagIp               ip;      //used for the ip
	char                apn[CN_NAME_LIMIT];
	char                user[CN_NAME_LIMIT];
	char                passwd[CN_NAME_LIMIT];
	char                iodevname[CN_NAME_LIMIT];
	char                netdevname[CN_NAME_LIMIT];
	bool_t              framdebug;   //check if we should print the info receive or send
	tagPppRC            rc;       //receive data control block
	tagPppSC            sc;       //send frame control block
	tagPppMs            ms;       //ppp machine state control block
	tagPppReg           reg;      //used for the sim card register
	tagStatistics       num;

}tagPppCtrl;
//static tagPppCtrl gPppCtrl;       //now we define one here
static tagPppCtrl *pPppDevLst;     //this is the ppp dev list
static tagPppCtrl * __PppGet(char *name)
{
	tagPppCtrl *ppp = NULL;
	ppp =pPppDevLst;
	while(NULL != ppp)
	{
		if(0 == strcmp(ppp->netdevname,name))
		{
			break;
		}
		else
		{
			ppp = ppp->nxt;
		}
	}
	return ppp;
}
//usage:show the ppp state
static void pppstat(tagPppCtrl *ppp)
{
	printf("MODEMINFO:\n\r");
	printf("apn   :%s\n\r",ppp->apn);
	printf("user  :%s\n\r",ppp->user);
	printf("pass  :%s\n\r",ppp->passwd);
	printf("iodev :%s\n\r",ppp->iodevname);
	printf("netdev:%s\n\r",ppp->netdevname);
	//machine state
	printf("MS  :%s\n\r",gPppStatName[ppp->ms.stat]);
	printf("MSTO:%d\n\r",ppp->ms.timeoutnum);
	printf("MSTL:%d\n\r",ppp->ms.timeoutlimit);
	printf("MSTD:%lld\n\r",ppp->ms.timedead);
	//frame got
	printf("FRCV:%lld\n\r",ppp->num.fall);
	printf("FRER:%lld\n\r",ppp->num.fcserr);
	//IP FRAME GOT
	printf("IPD :%lld\n\r",ppp->ip.ipflow_d);
	printf("IPU :%lld\n\r",ppp->ip.ipflow_u);
	return;
}


//usage:we use this function to pass the receive info to the state machine
typedef struct __PppMsg
{
	tagPppCtrl        *ppp;    //ppp control block here
	u16                proto;  //frame proto here
	u16                len;    //data length
	u8                *data;   //data buffer
}tagPppMsg;

//usage:this function to regnet up by the machine
static bool_t __RegNetUp(tagPppCtrl *ppp)
{
	bool_t result = true;
	if(NULL != ppp->reg.regnet)
	{
		result = ppp->reg.regnet(ppp->iodevname,ppp->apn);
	}
	return result;
}

//-----------------------------------------------------------------------------
//����:we use this function to send data to the peer
//����:
//����:
//��ע:here we add the address control and the tail it self
//����:zhangqf@����4:04:11/2017��1��6��
//-----------------------------------------------------------------------------
static bool_t __PppMsgSnd(tagPppCtrl *ppp,u16 proto,tagCHdr *chdr,u8 *buf,u16 l,u32 accm)
{
	u8             *dst,*dstedge,*src;
	u8              c;
	u16             fcs,len;
	tagPppSC       *tx;
	tagPppHdr       hdr;

	tx = &ppp->sc;
	if(ppp->framdebug)
	{
		time_t printtime;
		printtime = time(NULL);
		if(NULL != chdr)
		{
			printf("[%s]snd:proto:0x%04x code:0x%02x id:0x%02x len:0x%04x\n\r",\
					ctime(&printtime),proto,chdr->code,chdr->id,htons(chdr->len));
		}
		else
		{
			printf("[%s]snd:proto:0x%04x len:0x%04x\n\r",\
					ctime(&printtime),proto,l);
		}
	}
	if(mutex_lock(tx->mutex))
	{
		//do some initialize;
		tx->datalen = 0;
		fcs = PPP_INITFCS;
		dst = tx->buf;
		dstedge = tx->buf + tx->buflen;
		//add the start flag
		c = PPP_FLAG;
		*dst++= c;
		//add the proto header
		hdr.a = PPP_ALLSTATIONS;
		hdr.c = PPP_UI;
		hdr.p = htons(proto);

		//now cpy use the accm and compute the fcs
		len = CN_PPP_HDRSIZE;
		src = (u8 *)&hdr;
		while((dst < dstedge)&&(len > 0))
		{
			c = *src++;
			len--;
			if((c == PPP_FLAG)||(c== PPP_ESCAPE)||\
			   ((c <0x20)&&((1<<c)&accm)))
			{
				*dst++ = PPP_ESCAPE;
				fcs = PPP_FCS(fcs,c);
				c ^= PPP_TRANS;
				*dst++= c;
			}
			else
			{
				*dst++=c;
				fcs = PPP_FCS(fcs,c);
			}
		}
		//add the code header
		if(NULL != chdr)
		{
			len = CN_PPPOS_CHDRSIZE;
			src = (u8 *)chdr;
			while((dst < dstedge)&&(len > 0))
			{
				c = *src++;
				len--;
				if((c == PPP_FLAG)||(c== PPP_ESCAPE)||\
				   ((c <0x20)&&((1<<c)&accm)))
				{
					*dst++ = PPP_ESCAPE;
					fcs = PPP_FCS(fcs,c);
					c ^= PPP_TRANS;
					*dst++= c;
				}
				else
				{
					*dst++=c;
					fcs = PPP_FCS(fcs,c);
				}
			}
		}
		//add the data
		len = l;
		src = (u8 *)buf;
		while((dst < dstedge)&&(len > 0))
		{
			c = *src++;
			len--;
			if((c == PPP_FLAG)||(c== PPP_ESCAPE)||\
			   ((c <0x20)&&((1<<c)&accm)))
			{
				*dst++ = PPP_ESCAPE;
				fcs = PPP_FCS(fcs,c);
				c ^= PPP_TRANS;
				*dst++= c;
			}
			else
			{
				*dst++=c;
				fcs = PPP_FCS(fcs,c);
			}
		}
		fcs = ~fcs;
		//add the fcs,at least 5 bytes left for the fcs and the stop flag
		if((dst+5)< dstedge)
		{
			len = 2;
			src = (u8 *)&fcs;
			while((dst < dstedge)&&(len > 0))
			{
				c = *src++;
				len--;
				if((c == PPP_FLAG)||(c== PPP_ESCAPE)||\
				   ((c <0x20)&&((1<<c)&accm)))
				{
					*dst++ = PPP_ESCAPE;
					c ^= PPP_TRANS;
					*dst++= c;
				}
				else
				{
					*dst++=c;
				}
			}
			//add the stop flag
			*dst++= PPP_FLAG;
			//compute the len, and send it to the device;
			len =(u16)(dst -tx->buf);
			iodevwrite(ppp->iofd,tx->buf,len,10*mS);
		}
		else
		{
			//buf maybe overlap
		}
		mutex_unlock(tx->mutex);
	}

	return true;
}
//here we create a ppp net device to the stack
#include <netdb.h>
#include <sys/socket.h>
//data flow in the loop
#define CN_PPP_MTU      4*1024        //4KB
//-----------------------------------------------------------------------------
//����:the linkoutloop call this function to pass the package to the stack
//����:
//����:
//��ע:
//����:zhangqf@����9:24:20/2016��12��29��
//-----------------------------------------------------------------------------
static bool_t __PppDevOut(ptu32_t dev,tagNetPkg *pkg,u32 framlen,u32 netdevtask)
{
    bool_t  result;
    tagNetPkg *tmp;
    u8 *buf;
    u8 *src;
    u8 *dst;
    u32 cpylen;
    tagPppCtrl *ppp;

    result = false;
    ppp = (tagPppCtrl*)NetDevPrivate(dev);

    if(ppp->ms.stat != EN_PPP_NETWORK)
    {
    	return result;   //if no ppp link got ,return false
    }
    buf = malloc(framlen);
    if(NULL != buf)
    {
    	cpylen = 0;
    	dst = buf;
        tmp = pkg;
        while(NULL != tmp)
        {
            src = (u8 *)(tmp->buf + tmp->offset);
            memcpy(dst, src, tmp->datalen);
            dst += tmp->datalen;
            cpylen += tmp->datalen;
            if(tmp->pkgflag & CN_PKLGLST_END)
            {
                tmp = NULL;
            }
            else
            {
                tmp = tmp->partnext;
            }
        }
        //send the buf to the ppp
        result = __PppMsgSnd(ppp,PPP_IP,NULL,buf,cpylen,ppp->lcp.peer.accm);
        free(buf);
        ppp->ip.ipflow_u += framlen;
    }

    return result;
}

static bool_t __IpFrameDeal(tagPppMsg   *msg )
{
	//we should package the msg and put it to the stack
	u8 *src,*dst;
	u16 len;
	tagNetPkg *pkg;
	tagPppCtrl *ppp;

	src = msg->data;
	len = msg->len;
	ppp = msg->ppp;


	ppp->ip.ipflow_d += len;
	pkg = PkgMalloc(len,CN_PKLGLST_END);
	if(NULL != pkg)
	{
		dst = pkg->buf + pkg->offset;
		memcpy(dst,src,len);
		pkg->datalen = len;

		LinkPost(ppp->nfd,pkg);

		PkgTryFreePart(pkg);
	}


	return true;
}
//-----------------------------------------------------------------------------
//����:Install the ppp device
//����:
//����:
//��ע:
//����:zhangqf@����9:26:04/2016��12��29��
//-----------------------------------------------------------------------------
static ptu32_t __PppNetDevAdd(tagPppCtrl *ppp)
{
	ptu32_t         dev;
    tagNetDevPara   devpara;
    tagHostAddrV4   devaddr;
    //then we will register a loop device to the stack
    memset((void *)&devpara,0,sizeof(devpara));
    devpara.ifsend = __PppDevOut;
    devpara.iftype = EN_LINK_RAW;
    devpara.name = ppp->netdevname;
    devpara.private = (ptu32_t)ppp;
    devpara.mtu = CN_PPP_MTU;
    devpara.devfunc = CN_IPDEV_NONE;
    memcpy(devpara.mac,CN_MAC_BROAD,CN_MACADDR_LEN);
    dev = NetDevInstall(&devpara);
    if(0 == dev)
    {
    	goto EXIT_PPPDEV;
    }
    //here means we are successful
    //then we will create a loop rout to the stack
    devaddr.ip      = inet_addr("192.168.253.100");
    devaddr.gatway  = inet_addr("192.168.253.1");
    devaddr.submask = inet_addr("255.255.255.0");
    devaddr.dns     = inet_addr("192.168.253.1");
    devaddr.broad   = inet_addr("255.255.255.255");
    //loop could receive the broad mail,but never send the broad mail
    if(false == RoutCreate(ppp->netdevname,EN_IPV_4,(void *)&devaddr,CN_ROUT_NONE))
    {
    	goto EXIT_PPPROUT;
    }
    //here means we are successful
    RoutSetDefault(EN_IPV_4,devaddr.ip);
    return dev;

EXIT_PPPROUT:
	NetDevUninstall(ppp->netdevname);
EXIT_PPPDEV:
    return 0;
}
extern bool_t __AuthSndRequest(tagPppCtrl *ppp);
//-----------------------------------------------------------------------------
//����:we use this function to pass the receive data to the queue
//����:
//����:
//��ע:
//����:zhangqf@����10:10:36/2017��1��5��
//-----------------------------------------------------------------------------

//usage:use this function to reset the ppp machine state
static bool_t  __PppMsReset(tagPppCtrl *ppp,enPppStat state)
{
	ppp->ms.stat = state;
    //initialize the lcp options
	memset((void *)&ppp->lcp,0,sizeof(tagLcp));
	ppp->lcp.host.mru = PPP_MTU;
	ppp->lcp.host.enmru = 0;
	ppp->lcp.host.accm = 0;
	ppp->lcp.host.enaccm = 1;
	ppp->lcp.host.authcode = PPP_PAP;
	ppp->lcp.host.authencode = 0;
	ppp->lcp.host.enauth = 0;
	ppp->lcp.host.macgic = (u32)DjyGetSysTime();
	ppp->lcp.host.enmagic = 0;
	ppp->lcp.host.buflen = CN_OPTION_BUFLEN;
	ppp->lcp.host.id = 1;
	//initialize the auth
	memset((void *)&ppp->auth,0,sizeof(tagAuth));
	ppp->auth.id = 0x10;
	//initialize the ncp
	memset((void *)&ppp->ncp,0,sizeof(tagNcp));
	ppp->ncp.enIp = 1;
	ppp->ncp.enDns = 1;
	ppp->ncp.enDnsBak = 1;
	ppp->ncp.enNbns = 1;
	ppp->ncp.enNbnsBak = 1;
	ppp->ncp.id = 0x20;

	return true;
}

//we use this function to send the lcp request
static bool_t __LcpSndRequest(tagPppCtrl *ppp)
{

	u8                 *dst;
	tagPppOptions       opt;
	tagLcpHostStat     *host;
	u16                 v16;
	u32                 v32;
	tagCHdr             cdr;
	u32                 accm;

	host = &ppp->lcp.host;
	accm = 0xFFFFFFFF;

	//make the new request
	dst = host->buf;
	if(host->enaccm) //check the accm
	{
		opt.type = LCP_OPT_ACCM;
		opt.len = 6;
		memcpy(dst,&opt,sizeof(opt));
		dst += sizeof(opt);
		v32 = host->accm;
		v32 = htonl(v32);
		memcpy(dst,&v32,sizeof(v32));
		dst += sizeof(v32);
	}
	if(host->enauth) //check the auth
	{
		if(host->authcode == PPP_PAP)
		{
			opt.type = LCP_OPT_AUTH;
			opt.len = 4;
			memcpy(dst,&opt,sizeof(opt));
			dst += sizeof(opt);
			v16 = host->authcode;
			v16 = htonl(v16);
			memcpy(dst,&v16,sizeof(v16));
			dst += sizeof(v16);
		}
		else
		{
			opt.type = LCP_OPT_AUTH;
			opt.len = 5;
			memcpy(dst,&opt,sizeof(opt));
			dst += sizeof(opt);
			v16 = host->authcode;
			v16 = htonl(v16);
			memcpy(dst,&v16,sizeof(v16));
			dst += sizeof(v16);
			*dst++=host->authencode;
		}
	}
	if(host->enmagic) //CHECK THE MAGIC
	{
		opt.type = LCP_OPT_MAGIC;
		opt.len = 6;
		memcpy(dst,&opt,sizeof(opt));
		dst += sizeof(opt);
		v32 = host->macgic;
		v32 = htonl(v32);
		memcpy(dst,&v32,sizeof(v32));
		dst += sizeof(v32);
	}
	if(host->enmru) //CHECK THE MRU
	{
		opt.type = LCP_OPT_MRU;
		opt.len = 4;
		memcpy(dst,&opt,sizeof(opt));
		dst += sizeof(opt);
		v16 = host->mru;
		v16 = htonl(v16);
		memcpy(dst,&v16,sizeof(v16));
		dst += sizeof(v16);
	}
	host->datalen = dst - host->buf;
	cdr.code = CONFREQ;
	cdr.id = host->id;
	cdr.len = htons((host->datalen+sizeof(cdr)));
	__PppMsgSnd(ppp,PPP_LCP,&cdr,host->buf,host->datalen,accm);

	return true;
}

//use this function to send a echo request
static bool_t __LcpSendEchoReq(tagPppCtrl *ppp)
{
	u32     magic;
	tagCHdr cdr;
	u32     accm;

	accm = 0xffffffff;
	magic = ppp->lcp.peer.macgic;
	magic = htonl(magic);

	cdr.code = ECHOREQ;
	cdr.id = ppp->lcp.host.id;
	cdr.len = htons((sizeof(magic)+sizeof(cdr)));
	__PppMsgSnd(ppp,PPP_LCP,&cdr,(u8 *)&magic,sizeof(magic),accm);
	return true;
}

//use this function to send a terminate request
static bool_t __LcpSendTerminateReq(tagPppCtrl *ppp)
{
	u32     magic;
	tagCHdr cdr;
	u32     accm;

	accm = 0xffffffff;
	magic = ppp->lcp.peer.macgic;
	magic = htonl(magic);

	cdr.code = TERMREQ;
	cdr.id = ppp->lcp.host.id;
	cdr.len = htons((sizeof(magic)+sizeof(cdr)));
	__PppMsgSnd(ppp,PPP_LCP,&cdr,(u8 *)&magic,sizeof(magic),accm);
	return true;
}
//deal the configure request
static bool_t __LcpDealCRQ(tagPppCtrl *ppp,u8 id,u8 *buf, u16 len)
{
	u8                 *src;
	u8                 *stop;
	tagPppOptions      *opt;
	tagLcpPeerOptions  *peer;
	u16                 v16;
	u32                 v32;
	tagPppOptions       mopt;
	tagCHdr             cdr;

	src = buf;
	stop = buf + len;
	peer = &ppp->lcp.peer;
	peer->naklen = 0;
	peer->rejlen = 0;
	peer->acklen = 0;
	//now decode the lcp options here
	while(src < stop)
	{
		opt = (tagPppOptions *)src;
		src += opt->len;
		switch (opt->type)
		{
			case LCP_OPT_MRU:
				memcpy(&v16,opt->v,sizeof(v16));
				v16 = ntohl(v16);
				if(v16 > PPP_MTU) //should advise it small
				{
					v16 = htons(PPP_MTU);
					mopt.type = opt->type;
					mopt.len = 4;
					memcpy(&peer->optionnack[peer->naklen],&mopt,CN_PPPOS_OPTSIZE);
					peer->naklen += CN_PPPOS_OPTSIZE;
					memcpy(&peer->optionnack[peer->naklen],&v16,sizeof(v16));
					peer->naklen += sizeof(v16);
				}
				else
				{
					peer->mru = v16;
					memcpy(&peer->optionack[peer->acklen],opt,opt->len);
					peer->acklen += opt->len;
				}
				break;
			case LCP_OPT_ACCM:
				memcpy(&v32,opt->v,sizeof(v32));
				v32 = ntohl(v32);
				peer->accm = v32;
				memcpy(&peer->optionack[peer->acklen],opt,opt->len);
				peer->acklen += opt->len;
				break;
			case LCP_OPT_AUTH:
				memcpy(&v16,opt->v,sizeof(v16));
				v16 = ntohs(v16);
				if(v16 != PPP_PAP)  //only md5 supported
				{
					v16 = htons(PPP_PAP);
					mopt.type = opt->type;
					mopt.len = 4;
					memcpy(&peer->optionnack[peer->naklen],&mopt,CN_PPPOS_OPTSIZE);
					peer->naklen += CN_PPPOS_OPTSIZE;
					memcpy(&peer->optionnack[peer->naklen],&v16,sizeof(v16));
					peer->naklen += sizeof(v16);
				}
				else
				{
					peer->authcode = v16;
					memcpy(&peer->optionack[peer->acklen],opt,opt->len);
					peer->acklen += opt->len;
				}
				break;
			case LCP_OPT_MAGIC:
				memcpy(&v32,opt->v,sizeof(v32));
				v32 = ntohl(v32);
				peer->macgic = v32;
				memcpy(&peer->optionack[peer->acklen],opt,opt->len);
				peer->acklen += opt->len;
				break;
			case LCP_OPT_PFC: //not supported yet
				memcpy(&peer->optionrej[peer->rejlen],opt,opt->len);
				peer->rejlen += opt->len;
				break;
			case LCP_OPT_ACFC://not supported yet
				memcpy(&peer->optionrej[peer->rejlen],opt,opt->len);
				peer->rejlen += opt->len;
				break;
			default:
				memcpy(&peer->optionrej[peer->rejlen],opt,opt->len);
				peer->rejlen += opt->len;
				break;
		}
	}
	u32 accm = 0xffff;
	//here we should do the machine state changed, and do the time out configure
	if(peer->rejlen > 0) //do the rej
	{
		cdr.code = CONFREJ;
		cdr.id = id;
		cdr.len = peer->rejlen + CN_PPPOS_CHDRSIZE;
		cdr.len = htons(cdr.len);
		__PppMsgSnd(ppp,PPP_LCP,&cdr,peer->optionrej,peer->rejlen,accm);
	}
	else if(peer->naklen > 0)//if no rej, then do the nack
	{
		cdr.code = CONFNAK;
		cdr.id = id;
		cdr.len = peer->naklen + CN_PPPOS_CHDRSIZE;
		cdr.len = htons(cdr.len);
		__PppMsgSnd(ppp,PPP_LCP,&cdr,peer->optionnack,peer->naklen,accm);
	}
	else //if no rej and nack, then we do the ack
	{
		cdr.code = CONFACK;
		cdr.id = id;
		cdr.len = peer->acklen + CN_PPPOS_CHDRSIZE;
		cdr.len = htons(cdr.len);
		__PppMsgSnd(ppp,PPP_LCP,&cdr,peer->optionack,peer->acklen,accm);
		peer->done = 1;
		if(ppp->lcp.host.done)
		{
			//if the peer has agree the negotiation,then we do the auth
			printf("LCP SUCCESS,GOTO AUTH\n\r");
			ppp->ms.timeoutnum = 0;
			ppp->ms.stat = EN_PPP_AUTH;
			__AuthSndRequest(ppp);
		}
		else
		{
			__LcpSndRequest(ppp);//redo the request
		}
	}
	return true;
}

//-----------------------------------------------------------------------------
//����:when we receive this frame type,it means it has agree all the options we supplied
//����:
//����:
//��ע:we should changge the stage and go on
//����:zhangqf@����10:12:07/2017��1��7��
//-----------------------------------------------------------------------------
static bool_t __LcpDealCACK(tagPppCtrl *ppp,u8 id,u8 *buf, u16 len)
{
	//i think all i want to do is to change the state,and do the next thing
	//maybe should send the pap or chap to the peer
	ppp->lcp.host.done = 1;
	ppp->lcp.host.id++;
	if(ppp->lcp.peer.done)//if the peer has agree the negotiation,then we do the auth
	{
		printf("LCP SUCCESS,GOTO AUTH\n\r");
		ppp->ms.timeoutnum = 0;
		ppp->ms.stat = EN_PPP_AUTH;
		ppp->ms.timedead = DjyGetSysTime() + (CN_MS_TIMEOUT<<ppp->ms.timeoutnum);
		__AuthSndRequest(ppp);
	}
	return true;
}
//usage:use this function to deal the lcp nack frame
static bool_t __LcpDealCNACK(tagPppCtrl *ppp,u8 id,u8 *buf, u16 len)
{
	u8                 *src;
	u8                 *stop;
	tagPppOptions      *opt;
	tagLcpHostStat     *host;
	u16                 v16;
	u32                 v32;

	src = buf;
	stop = buf + len;
	host = &ppp->lcp.host;
	//now decode the lcp options here
	while(src < stop)
	{
		opt = (tagPppOptions *)src;
		src += opt->len;
		switch (opt->type)
		{
			case LCP_OPT_MRU:
				memcpy(&v16,opt->v,sizeof(v16));
				v16 = ntohl(v16);
				host->mru = v16;
				break;
			case LCP_OPT_ACCM:
				memcpy(&v32,opt->v,sizeof(v32));
				v32 = ntohl(v32);
				host->accm = v32;
				break;
			case LCP_OPT_AUTH:
				memcpy(&v16,opt->v,sizeof(v16));
				v16 = ntohs(v16);
				host->authcode = v16;
				if(v16 != PPP_PAP)  //only md5 supported
				{
					host->authencode = opt->v[2];
				}
				break;
			case LCP_OPT_MAGIC:
				memcpy(&v32,opt->v,sizeof(v32));
				v32 = ntohl(v32);
				host->macgic = v32;
				break;
			default:
				break;
		}
	}
	host->id++;
	//we should resend the request here
	__LcpSndRequest(ppp);
	return true;
}
//-----------------------------------------------------------------------------
//����:we use this function to deal the reject options
//����:
//����:
//��ע:if we receive the nack,it means we should modify the options we send
//����:zhangqf@����11:37:16/2017��1��7��
//-----------------------------------------------------------------------------
static bool_t __LcpDealCREJ(tagPppCtrl *ppp,u8 id,u8 *buf, u16 len)
{
	u8                 *src;
	u8                 *stop;
	tagPppOptions      *opt;
	tagLcpHostStat  *host;

	src = buf;
	stop = buf + len;
	host = &ppp->lcp.host;
	//now decode the lcp options here
	while(src < stop)
	{
		opt = (tagPppOptions *)src;
		src += opt->len;
		switch (opt->type)
		{
			case LCP_OPT_MRU:
				host->enmru = 0;
				break;
			case LCP_OPT_ACCM:
				host->enaccm = 0;
				break;
			case LCP_OPT_AUTH:
				host->enauth = 0;
				break;
			case LCP_OPT_MAGIC:
				host->enmagic = 0;
				break;
			default:
				break;
		}
	}
	host->id++;
	//we should resend the request here
	__LcpSndRequest(ppp);
	return true;
}
//-----------------------------------------------------------------------------
//����:we use this function to deal the echo request
//����:
//����:
//��ע:just send the magic number we have received
//����:zhangqf@����11:37:16/2017��1��7��
//-----------------------------------------------------------------------------
static bool_t __LcpDealEchoReq(tagPppCtrl *ppp,u8 id,u8 *buf, u16 len)
{
	tagCHdr  cdr;

	cdr.code = ECHOREP;
	cdr.id = id;
	cdr.len = 6;
	__PppMsgSnd(ppp,PPP_LCP,&cdr,buf,len,ppp->lcp.peer.accm);
	return true;
}
//usage:we use this function to deal a lcp msg
static bool_t __LcpFrameDeal(tagPppMsg   *msg )
{
	tagCHdr      hdr;
	u8          *src;
	u16          len;
	src = msg->data;
	//now do the decode now
	memcpy(&hdr,src,CN_PPPOS_CHDRSIZE);
	hdr.len = htons(hdr.len);
	src += CN_PPPOS_CHDRSIZE;
	len = msg->len - CN_PPPOS_CHDRSIZE;
	//now decode the lcp options here
	switch(hdr.code)
	{
		case CONFREQ:
			__LcpDealCRQ(msg->ppp,hdr.id,src,len);
			break;
		case CONFACK:
			__LcpDealCACK(msg->ppp,hdr.id,src,len);
			break;
		case CONFNAK:
			__LcpDealCNACK(msg->ppp,hdr.id,src,len);
			break;
		case CONFREJ:
			__LcpDealCREJ(msg->ppp,hdr.id,src,len);
			break;
		case TERMREQ:
			//we should send the term ack and turn to the down state
			hdr.code = TERMACK;
			__PppMsgSnd(msg->ppp,PPP_LCP,&hdr,src,len,0xFFFFFFFF);
			if(msg->ppp->ms.stat == EN_PPP_NETWORK)
			{
				//here we call the uplayer that the link is down
        		NetDevPostEvent(msg->ppp->nfd,NULL,EN_NETDEVEVENT_IPRELEASE);
			}
			//goto dead
			msg->ppp->ms.stat = EN_PPP_DEAD;
			printf("LCP:TERMINATE!,GOTO DEAD\n\r");
			break;
		case TERMACK:
			//goto dead
			if(msg->ppp->ms.stat == EN_PPP_TERMINATE)
			{
				msg->ppp->ms.stat = EN_PPP_DEAD;
				printf("LCP:TERMINATE ACK!,GOTO DEAD\n\r");
			}
			break;
		case CODEREJ:
			break;
		case PROTOREJ:
			break;
		case ECHOREQ:
			__LcpDealEchoReq(msg->ppp,hdr.id,src,len);
			break;
		case ECHOREP:
			msg->ppp->lcp.host.id++;
			msg->ppp->ms.timeoutnum = 0;
			break;
		case DISREQ:
			break;
		default://unknow the code, we should send the reject code here
			hdr.code = CODEREJ;
			hdr.len = htons(msg->len);
			__PppMsgSnd(msg->ppp,PPP_LCP,&hdr,msg->data,msg->len,0xffffff);
			break;
	}
	return true;
}


typedef enum
{
	EN_PAP_REQ = 1,
	EN_PAP_ACK,
	EN_PAP_NACK,
}enPapCode;
//usage:use this function to send our auth
bool_t __AuthSndRequest(tagPppCtrl *ppp)
{
	u8       len;
	tagCHdr  cdr;
	tagAuth  *auth;
	u8       *buf;
	u8       *dst;

	if(ppp->lcp.peer.authcode == PPP_PAP) //USE USERNAME AND PASSWD
	{
		auth = &ppp->auth;
		len = strlen(ppp->user) + strlen(ppp->passwd) + 2;
		buf = malloc(len);
		if(NULL != buf)
		{
			dst = buf;
			*dst++ = strlen(ppp->user);
			memcpy(dst,ppp->user,strlen(ppp->user));
			dst += strlen(ppp->user);

			*dst++ = strlen(ppp->passwd);
			memcpy(dst,ppp->passwd,strlen(ppp->passwd));
			dst += strlen(ppp->passwd);

			cdr.code = EN_PAP_REQ;
			cdr.id = auth->id;
			cdr.len = htons(len + CN_PPPOS_CHDRSIZE);
			__PppMsgSnd(ppp,PPP_PAP,&cdr,buf,len,ppp->lcp.peer.accm);
			//free the buf
			free(buf);
		}
	}

	return true;
}

static bool_t __NcpSndRequest(tagPppCtrl *ppp);
//usage:we use this function to deal a auth msg
static bool_t __AuthFrameDeal(tagPppMsg   *msg)
{
	tagCHdr      hdr;
	u8          *src;
	u16          len,proto;
	tagPppCtrl *ppp;

	ppp = msg->ppp;
	src = msg->data;
	proto = msg->proto;
	len = msg->len;
	//now do the decode now
	memcpy(&hdr,src,CN_PPPOS_CHDRSIZE);
	hdr.len = htons(hdr.len);
	src += CN_PPPOS_CHDRSIZE;
	len -= CN_PPPOS_CHDRSIZE;
	if((proto == PPP_PAP)&&(ppp->lcp.host.authcode == PPP_PAP))
	{
		//we support the pap mode
		if(hdr.code == EN_PAP_ACK)
		{
			//we has passed ,goto the ncp
			ppp->ms.timeoutnum = 0;
			ppp->ms.stat = EN_PPP_NCP;
			ppp->ms.timedead = DjyGetSysTime() + (CN_MS_TIMEOUT<<ppp->ms.timeoutnum);
			printf("AUTH SUCCESS,GOTO NCP\n\r");
			//send the ncp request
			__NcpSndRequest(ppp);
		}
		else if(hdr.code == EN_PAP_NACK)
		{
			ppp->ms.timeoutnum = 0;
			ppp->ms.stat = EN_PPP_TERMINATE;
			ppp->ms.timedead = DjyGetSysTime() + (CN_MS_TIMEOUT<<ppp->ms.timeoutnum);
			printf("AUTH FAILED,GOTO TERMINATE\n\r");
		}
		else
		{
			//other frame should be ignored
		}
	}
	else
	{
		//the chap not supported yet--TODO
	}
	return true;
}

//usage:we use this function to deal a ncp msg
static bool_t __NcpSndRequest(tagPppCtrl *ppp)
{

	u8                 *dst;
	tagPppOptions       opt;
	u32                 v32;
	tagCHdr             cdr;
	tagNcp             *ncp;

	ncp = &ppp->ncp;
	dst = ncp->buf;
	//make the new request
	if(ncp->enIp) //check the ip
	{
		opt.type = NCP_OPT_IP;
		opt.len = 6;
		memcpy(dst,&opt,sizeof(opt));
		dst += sizeof(opt);
		v32 = ncp->ip;
		memcpy(dst,&v32,sizeof(v32));
		dst += sizeof(v32);
	}
	if(ncp->enDns) //check the dns
	{
		opt.type = NCP_OPT_DNS;
		opt.len = 6;
		memcpy(dst,&opt,sizeof(opt));
		dst += sizeof(opt);
		v32 = ncp->dns;
		memcpy(dst,&v32,sizeof(v32));
		dst += sizeof(v32);
	}
	if(ncp->enDnsBak) //check the dns
	{
		opt.type = NCP_OPT_DNS2;
		opt.len = 6;
		memcpy(dst,&opt,sizeof(opt));
		dst += sizeof(opt);
		v32 = ncp->dnsbak;
		memcpy(dst,&v32,sizeof(v32));
		dst += sizeof(v32);
	}
	if(ncp->enNbns) //check the dns
	{
		opt.type = NCP_OPT_NBNS;
		opt.len = 6;
		memcpy(dst,&opt,sizeof(opt));
		dst += sizeof(opt);
		v32 = ncp->nbns;
		memcpy(dst,&v32,sizeof(v32));
		dst += sizeof(v32);
	}
	if(ncp->enNbnsBak) //check the dns
	{
		opt.type = NCP_OPT_NBNS2;
		opt.len = 6;
		memcpy(dst,&opt,sizeof(opt));
		dst += sizeof(opt);
		v32 = ncp->nbnsbak;
		memcpy(dst,&v32,sizeof(v32));
		dst += sizeof(v32);
	}
	ncp->datalen = dst- ncp->buf;
	cdr.code = CONFREQ;
	cdr.id = ncp->id;
	cdr.len = htons(ncp->datalen + CN_PPP_HDRSIZE);
	__PppMsgSnd(ppp,PPP_IPCP,&cdr,ncp->buf,ncp->datalen,ppp->lcp.peer.accm);
	return true;
}
static bool_t __NcpOptionModify(tagNcp *ncp,u8 *src,u16 len,bool_t rej)
{
	u8                 *stop;
	tagPppOptions      *opt;
	u32                 v32;
	stop = src + len;
	//now decode the ncp options here
	while(src < stop)
	{
		opt = (tagPppOptions *)src;
		src += opt->len;
		switch (opt->type)
		{
			case NCP_OPT_IP:
				if(rej)
				{
					ncp->enIp = 0;
				}
				else
				{
					memcpy(&v32,opt->v,sizeof(v32));
					ncp->ip = v32;
					ncp->enIp = 1;
				}
				break;
			case NCP_OPT_DNS:
				if(rej)
				{
					ncp->enDns = 0;
				}
				else
				{
					memcpy(&v32,opt->v,sizeof(v32));
					ncp->dns = v32;
					ncp->enDns =1;
				}
				break;
			case NCP_OPT_DNS2:
				if(rej)
				{
					ncp->enDnsBak = 0;
				}
				else
				{
					memcpy(&v32,opt->v,sizeof(v32));
					ncp->dnsbak = v32;
					ncp->enDnsBak = 1;
				}
				break;
			case NCP_OPT_NBNS:
				if(rej)
				{
					ncp->enNbns = 0;
				}
				else
				{
					memcpy(&v32,opt->v,sizeof(v32));
					ncp->nbns = v32;
					ncp->enNbns = 1;
				}
				break;
			case NCP_OPT_NBNS2:
				if(rej)
				{
					ncp->enNbnsBak = 0;
				}
				else
				{
					memcpy(&v32,opt->v,sizeof(v32));
					ncp->nbnsbak = v32;
					ncp->enNbnsBak=1;
				}
				break;
			default:
				break;
		}
	}
	ncp->id++;
	return true;
}

//when we got a ppp frame, we use this function to deal it
//we should know which proto and the data and datalen we got
static bool_t __NcpFrameDeal(tagPppMsg   *msg)
{
	tagCHdr      hdr;
	u8          *src;
	u16          len;
	tagPppCtrl *ppp;
	tagNcp       *ncp;

	ppp = msg->ppp;
	src = msg->data;
	//now do the decode now
	memcpy(&hdr,src,CN_PPPOS_CHDRSIZE);
	hdr.len = htons(hdr.len);
	src += CN_PPPOS_CHDRSIZE;
	len = msg->len -  CN_PPPOS_CHDRSIZE;
	//here we should check if it is a ack nack rej or something else
    ncp = &ppp->ncp;
	switch(hdr.code)
    {
		case CONFACK:
			//turn to another state
			ppp->ms.timeoutnum = 0;
			ppp->ms.stat = EN_PPP_NETWORK;
			//here we should create a net device and build a rout here
			//modify the rout
			RoutSetDefaultAddr(EN_IPV_4,ncp->ip,0xFFFFFFFF,ncp->ip,ncp->dns);
			printf("NCP SUCCESS!,GOTO NETWORK\n\r");
			//here we call the uplayer that the ip get here
    		NetDevPostEvent(ppp->nfd,NULL,EN_NETDEVEVENT_IPGET);
			break;
    	case CONFREQ:
    		if(len != 0)
    		{
    			//reject all
    			hdr.len = htons(hdr.len);
    			hdr.code = CONFREJ;
    			__PppMsgSnd(ppp,PPP_IPCP,&hdr,src,len,ppp->lcp.peer.accm);
    		}
    		else
    		{
    			//ack it
    			hdr.code = CONFACK;
    			hdr.len = htons(hdr.len);
    			__PppMsgSnd(ppp,PPP_IPCP,&hdr,NULL,0,ppp->lcp.peer.accm);
    		}
    		break;
    	case CONFNAK:
    		//modify the request and resend the request
    		__NcpOptionModify(ncp,src,len,false);
    		__NcpSndRequest(ppp);
    		break;
    	case CONFREJ:
    		//modify the request and resend the request
    		__NcpOptionModify(ncp,src,len,true);
    		__NcpSndRequest(ppp);
    		break;
    	default:
    		//send the code reject
			hdr.code = CODEREJ;
			hdr.len = htons(hdr.len);
			__PppMsgSnd(ppp,PPP_IPCP,&hdr,src,len,ppp->lcp.peer.accm);
    		break;
    }
	return true;
}
//-----------------------------------------------------------------------------
//����:this the ppp main task thread
//����:
//����:
//��ע:
//����:zhangqf@����10:50:16/2017��1��5��
//-----------------------------------------------------------------------------

static void __FrameDeal(tagPppMsg  *msg)
{
	tagCHdr        hdr;
	//we receive a message from the peer, we should deal it carefully
	if(msg->ppp->ms.stat > EN_PPP_DEAD)
	{
		switch(msg->proto)
		{
			case PPP_LCP://this is a lcp frame,so we should deal it with the LCP
				__LcpFrameDeal(msg);
				break;
			case PPP_PAP:
				__AuthFrameDeal(msg);
				break;
			case PPP_CHAP:
				__AuthFrameDeal(msg);
				break;
			case PPP_IPCP:
				__NcpFrameDeal(msg);
				break;
			case PPP_IP:
				__IpFrameDeal(msg);
				break;
			default:
				//maybe a unknown proto, we should send a protorej message to the peer
				hdr.code = PROTOREJ;
				hdr.id = msg->ppp->lcp.host.id;
				hdr.len = htons(msg->len);
				__PppMsgSnd(msg->ppp,PPP_LCP,&hdr,msg->data,msg->len,0xffffff);
				break;
		}
	}
}

//usage:this function used for the modem reset
static bool_t __PppDevReset(tagPppCtrl *ppp)
{
	if(NULL!= ppp->reg.modemreset)
	{
		ppp->reg.modemreset();
	}
	return true;
}
static void __CheckTimeout(tagPppCtrl    *ppp)
{
	u64            timenow;
	//maybe timeout, we should check if any ppp to do the timeout resend
	timenow = (u64)DjyGetSysTime();
	switch(ppp->ms.stat)
	{
		case EN_PPP_DEAD:
			//use the at command to do the link up
			__PppDevReset(ppp);
			break;
		case EN_PPP_ESTABLISH:
			//we should check if the config request timeout
			if(timenow > ppp->ms.timedead)
			{
				if(ppp->ms.timeoutnum < CN_TIMEOUT_LIMIT)
				{
					__LcpSndRequest(ppp);
					ppp->ms.timedead = timenow+ CN_LCP_TIME;
					ppp->ms.timeoutnum++;
				}
				else
				{
					ppp->ms.timeoutnum = 0;
					ppp->ms.stat = EN_PPP_TERMINATE;
					printf("LCP TIMEOUT\n\r");
				}
			}
			break;
		case EN_PPP_AUTH:
			//we should check if the auth timeout
			if(timenow > ppp->ms.timedead)
			{
				if(ppp->ms.timeoutnum < CN_TIMEOUT_LIMIT)
				{
					__AuthSndRequest(ppp);
					ppp->ms.timedead =timenow+ CN_AUTH_TIME;
					ppp->ms.timeoutnum++;
				}
				else
				{
					ppp->ms.timeoutnum = 0;
					ppp->ms.stat = EN_PPP_TERMINATE;
					printf("AUTH TIMEOUT\n\r");
				}
			}
			break;
		case EN_PPP_NCP:
			//we should check if the ncp timeout
			if(timenow > ppp->ms.timedead)
			{
				if(ppp->ms.timeoutnum < CN_TIMEOUT_LIMIT)
				{
					__NcpSndRequest(ppp);
					ppp->ms.timedead =timenow+ CN_NCP_TIME;
					ppp->ms.timeoutnum++;
				}
				else
				{
					ppp->ms.timeoutnum = 0;
					ppp->ms.stat = EN_PPP_TERMINATE;
					printf("NCP TIMEOUT\n\r");
				}
			}
			break;
		case EN_PPP_NETWORK:
			//here we should check if any echo request timeout
			if(timenow > ppp->ms.timedead)
			{
				if(ppp->ms.timeoutnum < CN_TIMEOUT_LIMIT)
				{
					__LcpSendEchoReq(ppp);
					ppp->ms.timedead =timenow+ CN_IP_TIME;
					ppp->ms.timeoutnum++;
				}
				else
				{
					ppp->ms.timeoutnum = 0;
					ppp->ms.stat = EN_PPP_TERMINATE;
					printf("IP TIMEOUT\n\r");
					//here we call the uplayer that the link is down
	        		NetDevPostEvent(ppp->nfd,NULL,EN_NETDEVEVENT_IPRELEASE);
				}
			}
			break;
		case EN_PPP_TERMINATE:
			//do the reset and shut down
			if(timenow > ppp->ms.timedead)
			{
				if(ppp->ms.timeoutnum < CN_TIMEOUT_LIMIT)
				{
					__LcpSendTerminateReq(ppp);
					ppp->ms.timedead =timenow+ CN_NCP_TIME;
					ppp->ms.timeoutnum++;
				}
				else
				{
					ppp->ms.timeoutnum = 0;
					printf("TERMINATE TIMEOUT\n\r");
					ppp->ms.stat = EN_PPP_DEAD;
				}
			}
			break;
		default:
			//do the reset and shut down
			break;
	}
}
////////////////////////THE FOLLOWING USED TO DECODE THE LINK FRAME////////////
//-----------------------------------------------------------------------------
//����:use this function to do the common proto inputs
//����:
//����:return how many data we has dealt
//��ע:
//����:zhangqf@����8:05:58/2017��1��4��
//-----------------------------------------------------------------------------
//usage:upload the frame
static void __FrameUpload(tagPppCtrl *ppp)
{
	tagPppMsg      msg;

	msg.ppp =ppp;
	msg.len = ppp->rc.datalen;
	msg.proto = ppp->rc.proto;
	msg.data =ppp->rc.data;
	__FrameDeal(&msg);
	return;
}
//usage:use this to reset the receive machine state
static bool_t __RcReset(tagPppRC *rc)
{
	rc->datalen = 0;
	rc->drops = 0;
	rc->state = EN_CHECK_START;
	rc->proto = 0;
	rc->chdr.code = 0;
	rc->chdr.id = 0;
	rc->chdr.len = 0;
	rc->fcs = PPP_INITFCS;
	rc->inespace = false;
	return true;
}
//usage:use this function to show the receive frame simple info
static bool_t __rcframe(tagPppRC *rc)
{
	time_t printtime;
	printtime = time(NULL);
	printf("[%s]rcv:proto:0x%04x code:0x%02x id:0x%02x len:0x%04x \n\r",\
			ctime(&printtime),rc->proto,rc->chdr.code,rc->chdr.id,rc->chdr.len);
	return true;
}

//usage:this is the receive machine state,continuing to decode the data received
//      if receive a good frame,then upload it to the upper layer
static void __DecodeData(tagPppCtrl *ppp,u8 *s,u16 l)
{
	u8              curchar;
	u8             *str;
	tagPppRC       *rc;

	rc =&ppp->rc;
	str = s;
	while (l-- > 0)
	{
		curchar = *str++;
		if(curchar == PPP_FLAG)
		{
			__RcReset(rc);
			rc->state =EN_CHECK_STATION;
			continue;
		}
		if((curchar == PPP_ESCAPE)&&(false == rc->inespace))
		{
			rc->inespace = true; //if it is the char espace
			continue;  //do another one
		}
		if(rc->inespace)
		{
			curchar ^= PPP_TRANS;
			rc->inespace = false;
			//maybe we should check whether it is in the accm
			//if it is not in the accm, we should go to the drop stat
		}
		//compute the fcs
		rc->fcs = PPP_FCS(rc->fcs, curchar);
		switch(rc->state)
		{
			case EN_CHECK_START: //only care about the start flag 0x7e
				if(curchar == PPP_FLAG)
				{
					rc->state = EN_CHECK_STATION;
				}
				else
				{
					//drops.here
				}
				break;
			case EN_CHECK_STATION:
				if(curchar == PPP_ALLSTATIONS)
				{
					rc->state = EN_CHECK_CTRL;//got the station here
				}
				else if(curchar == PPP_UI)
				{
					rc->state = EN_CHECK_PROTO_H; //the station has been compressed
				}
				else if(0 == (curchar &0x01))
				{
					rc->proto = curchar<<8;
					rc->state = EN_CHECK_PROTO_L; //the station and ui has been compressed
				}
				else
				{
					rc->proto = curchar;
					rc->state = EN_CHECK_CDR_CODE;
				}
				break;
			case EN_CHECK_CTRL:
				if(curchar == PPP_UI)
				{
					rc->state = EN_CHECK_PROTO_H; //the station has been compressed
				}
				else if(0 == (curchar &0x01))
				{
					rc->proto = curchar<<8;
					rc->state = EN_CHECK_PROTO_L; //the station and ui has been compressed
				}
				else
				{
					rc->proto = curchar;
					rc->state = EN_CHECK_CDR_CODE;
				}
				break;
			case EN_CHECK_PROTO_H:
				rc->proto = curchar<<8;
				rc->state = EN_CHECK_PROTO_L; //the station and ui has been compressed
				break;
			case EN_CHECK_PROTO_L:
				rc->proto |= curchar;
				rc->state = EN_CHECK_CDR_CODE; //the station and ui has been compressed
				break;
			case EN_CHECK_CDR_CODE:
				rc->chdr.code = curchar;
				rc->state = EN_CHECK_CDR_ID;
				rc->data[rc->datalen]= curchar;
				rc->datalen++;
				break;
			case EN_CHECK_CDR_ID:
				rc->chdr.id = curchar;
				rc->state = EN_CHECK_LEN1;
				rc->data[rc->datalen]= curchar;
				rc->datalen++;
				break;
			case EN_CHECK_LEN1:
				rc->chdr.len = curchar<<8;
				rc->state = EN_CHECK_LEN2;
				rc->data[rc->datalen]= curchar;
				rc->datalen++;
				break;
			case EN_CHECK_LEN2:
				rc->chdr.len |= curchar;
				rc->state = EN_CHECK_DATA;
				rc->data[rc->datalen]= curchar;
				rc->datalen++;
				if(rc->datalen == rc->chdr.len)
				{
					rc->state = EN_CHECK_FCS1; //no data following the chdr
				}
				if(rc->chdr.len > CN_PPP_MTU)
				{
					__RcReset(rc);//if big enough then do the reset
				}
				break;
			case EN_CHECK_DATA:
				rc->data[rc->datalen]= curchar;
				rc->datalen++;
				if(rc->datalen == rc->chdr.len)
				{
					rc->state = EN_CHECK_FCS1; //got all the data now
				}
				break;
			case EN_CHECK_FCS1:
				rc->state = EN_CHECK_FCS2;
				break;//just do the fcs
			case EN_CHECK_FCS2:
				if(ppp->framdebug)
				{
					__rcframe(rc);
				}
				TCPIP_DEBUG_INC(ppp->num.fall);
				if(rc->fcs == PPP_GOODFCS)
				{
					__FrameUpload(ppp);
				}
				else
				{
					TCPIP_DEBUG_INC(ppp->num.fcserr);
				}
				//we should do the reset here for another accept
				__RcReset(rc);
				break;
			default:
				break; //drops here
		} //end for the switch
	}//end for the while
	return;
}
//-----------------------------------------------------------------------------
//����:use this function to deal the ppp frames
//����:
//����:
//��ע:
//����:zhangqf@����7:14:31/2017��3��23��
//-----------------------------------------------------------------------------
ptu32_t __PppTask(void)
{
#define CN_READ_BUF 64
	u8  iobuf[CN_READ_BUF];
	int len;
	tagPppCtrl  *ppp;
	Djy_GetEventPara((ptu32_t *)&ppp,NULL);
	if(NULL == ppp)
	{
		printf("%s:para invalid\n\r",__FUNCTION__);
		goto EXIT_PARA;
	}
	//we here should do the at regnet
	while(1)
	{
		//here we're waiting for the at reg start command,which means
		if(ppp->reg.regnetauto)
		{
			//here we will do reset the device and do the reset
			__PppDevReset(ppp);
		}
		else
		{
			while(ppp->reg.regnetstart == false)
			{
				sleep(1);
			}
		}
		//use the at command to register the module
		ppp->reg.regnetok = __RegNetUp(ppp);
		ppp->reg.regnetstart = false;
		if(false == ppp->reg.regnetok)
		{
			NetDevPostEvent(ppp->nfd,NULL,EN_NETDEVEVENT_LINKDOWN);
			printf("%s:register 3G/4G failed\n\r",__FUNCTION__);
			continue; //wait for another register if need
		}
		else
		{
			NetDevPostEvent(ppp->nfd,NULL,EN_NETDEVEVENT_LINKUP);
			printf("%s:register 3G/4G success\n\r",__FUNCTION__);
		}
		//here we reset the machine state and do the ppp negotiation
		__PppMsReset(ppp,EN_PPP_ESTABLISH);
		ppp->iofd = iodevopen(ppp->iodevname);
		if(0 == ppp->iofd)
		{
			printf("%s:open %s failed\n\r",__FUNCTION__,ppp->iodevname);
			goto EXIT_OPENDEV;
		}
		//here we always do the receive here
		//do the receive state reset
		__RcReset(&ppp->rc);
		while(ppp->ms.stat != EN_PPP_DEAD)
		{
			len = iodevread(ppp->iofd,iobuf,CN_READ_BUF,0);
			if(len > 0)
			{
				__DecodeData(ppp,iobuf,len);
			}
			else
			{
			    Djy_EventDelay(1*mS); //do some wait here
			}
			__CheckTimeout(ppp);	  //check the machine state
		}
		//here close the device
		iodevclose(ppp->iofd);
		ppp->iofd = 0;
	}
EXIT_OPENDEV:
EXIT_PARA:
	return 0;
}


static void pppusage(void)
{
	printf("usage:\n\r\
-apn       apn\n\r\
-debug     on/off\n\r\
-dev       devname\n\r\
-user      user\n\r\
-passwd    passwd\n\r\
-mode      start/stop\n\r\
-help      help the uage\n\r");
	return;
}

static bool_t pppshell(char *param)
{
	int argc= 10;
	char *argv[10];
	int i = 0;
	tagPppCtrl *ppp;

	ppp = pPppDevLst;
	if(NULL == ppp)
	{
		printf("no ppp\n\r");
		return true;
	}
	argc = getargs(argc-1,&argv[1],param);
	if(argc == 0)
	{
		pppstat(ppp);
	}
	else
	{
		//do some set here
		i =1;
		argc++;
		while(i<argc)
		{
			if(0 == strcmp(argv[i],"-apn"))
			{
				i++;
				if(i < argc)
				{
					strncpy(ppp->apn,argv[i],CN_NAME_LIMIT);
					i++;
				}
			}
			else if(0 == strcmp(argv[i],"-dev"))
			{
				i++;
				if(i < argc)
				{
					strncpy(ppp->iodevname,argv[i],CN_NAME_LIMIT);
					i++;
				}
			}
			else if(0 == strcmp(argv[i],"-user"))
			{
				i++;
				if(i < argc)
				{
					strncpy(ppp->user,argv[i],CN_NAME_LIMIT);
					i++;
				}
			}
			else if(0 == strcmp(argv[i],"-passwd"))
			{
				i++;
				if(i < argc)
				{
					strncpy(ppp->passwd,argv[i],CN_NAME_LIMIT);
					i++;
				}
			}
			else if(0 == strcmp(argv[i],"-passwd"))
			{
				i++;
				if(i < argc)
				{
					strncpy(ppp->passwd,argv[i],CN_NAME_LIMIT);
					i++;
				}
			}
			else if(0 == strcmp(argv[i],"-debug"))
			{
				i++;
				if(i < argc)
				{
					if(0 == strcmp(argv[i],"start"))
					{
						ppp->framdebug = true;
					}
					else
					{
						ppp->framdebug = false;
					}
					i++;
				}
			}
			else if(0 == strcmp(argv[i],"-mode"))
			{
				i++;
				if(i < argc)
				{
					if(0 == strcmp(argv[i],"start"))
					{
						ppp->reg.regnetstart = true;
					}
					else
					{
						ppp->reg.regnetstart = false;
					}
					i++;
				}
			}
			else if(0 == strcmp(argv[i],"-help"))
			{
				pppusage();
				i++;
			}
			else
			{
				i++;
			}
		}
	}
	return true;
}

//usage:use this function to start the ppp dev
bool_t PppDevLinkStart(char *pppdevname)
{
	tagPppCtrl *ppp;
	ppp = __PppGet(pppdevname);
	if(NULL != ppp)
	{
		ppp->reg.regnetstart = true;
	}
	return true;
}
//usage:use this function to stop the ppp dev
bool_t PppDevLinkStop(char *pppdevname)
{
	tagPppCtrl *ppp;
	ppp = __PppGet(pppdevname);
	if(NULL != ppp)
	{
		ppp->reg.regnetstart = false;
	}
	return true;
}
//usage:use this function to check if the ppp dial is ok or not
//      this function is a block function here
bool_t PppDevLinkIsOk(char *pppdevname)
{
	tagPppCtrl *ppp;
	bool_t result = false;
	ppp = __PppGet(pppdevname);
	if((NULL != ppp)&&(ppp->ms.stat == EN_PPP_NETWORK))
	{
		result = true;
	}
	return result;
}
//usage:use this function to reset the ppp dev,as it create state
bool_t PppDevLinkRst(char *pppdevname)
{
	//we will do another  register and  ppp negotiation
	tagPppCtrl *ppp;
	ppp = __PppGet(pppdevname);
	if(NULL != ppp)
	{
		ppp->reg.regnetstart = false;
		ppp->ms.stat = EN_PPP_DEAD;
		//do the reset here
		__PppDevReset(ppp);
	}
	return true;
}
//usage:use this function to set the ppp auto register and auto negotiation
bool_t PppDevLinkAuto(char *pppdevname,bool_t mode)
{
	//we will do another  register and  ppp negotiation
	tagPppCtrl *ppp;
	ppp = __PppGet(pppdevname);
	if(NULL != ppp)
	{
		ppp->reg.regnetauto = mode;
		//do the reset here
	}
	return true;
}

bool_t PppDevLinkAutoShell(char *param)
{
	bool_t result = false;
	//we will do another  register and  ppp negotiation
	int argc=2;
	char *argv[2];
	string2arg(&argc,argv,param);
	if(argc == 2)
	{
		if(strcmp(argv[1],"auto"))
		{
			PppDevLinkAuto(argv[0],true);
		}
		else
		{
			PppDevLinkAuto(argv[0],false);
		}
		result = true;
	}
	else
	{
		result = false;
	}
	return result;
}

//usage:this is the ppp cmd for the os shell
struct ShellCmdTab  gPppDebug[] =
{
	{
		"ppp",
		pppshell,
		"usage:ppp [options/-help for the usage help]",
		"usage:ppp [options/-help for the usage help]",
	},
	{
		"pppreset",
		PppDevLinkRst,
		"usage:pppreset devname",
		"usage:pppreset devname",
	},
	{
		"pppstart",
		PppDevLinkStart,
		"usage:pppstart devname",
		"usage:pppstart devname",
	},
	{
		"pppstop",
		PppDevLinkStop,
		"usage:pppstop devname",
		"usage:pppstop devname",
	},
	{
		"pppmode",
		PppDevLinkAutoShell,
		"usage:pppmode auto/manual",
		"usage:pppmode auto/manual",
	}

};
#define CN_PPPDEBUG_NUM  ((sizeof(gPppDebug))/(sizeof(struct ShellCmdTab)))
static struct ShellCmdRsc gPppDebugCmdRsc[CN_PPPDEBUG_NUM];
//-----------------------------------------------------------------------------
//����:this is the ppp main function here
//����:
//����:
//��ע:
//����:zhangqf@����4:06:57/2017��1��5��
//-----------------------------------------------------------------------------
//usage:this is the new version to add the down and up event here
//the downhook is used to notify the user that the ppp
//first we should do install the ppp mode.
static u16  gPppEvttID = CN_EVTT_ID_INVALID;
static u16  gPppNum = 0;
bool_t PppInit(void)
{
	bool_t result = false;
	//initialize the at and io modem
    extern bool_t PppIoInit(ptu32_t para);
    PppIoInit(0);
    extern bool_t PppAtInit(ptu32_t para);
    PppAtInit(0);

    gPppEvttID = Djy_EvttRegist(EN_INDEPENDENCE, gPppTaskPrior, 0, gPppDevLimit,\
    		        __PppTask,NULL, gPppStackSize,(char *)pPppEvttName);
    if(gPppEvttID == CN_EVENT_ID_INVALID)
    {
    	return result;
    }
	//install the debug shell for the system
	result = Sh_InstallCmd(gPppDebug,gPppDebugCmdRsc,CN_PPPDEBUG_NUM);
    return result;
}
//add the device to the task list
static bool_t __PppAddTask(tagPppCtrl  *ppp)
{
	bool_t result = false;
	u16 eventID;

	if((NULL != ppp)&&(CN_EVTT_ID_INVALID != gPppEvttID))
	{
		eventID = Djy_EventPop(gPppEvttID, NULL, 0, (ptu32_t)ppp, 0, 0);
		if(CN_EVENT_ID_INVALID != eventID)
		{
			ppp->nxt = pPppDevLst;
			pPppDevLst = ppp;
			gPppNum++;
			result = true;
		}
	}
	return result;
}

//usage:the user call this function to create a ppp device
bool_t PppDevAdd(char *netdevname,char *iodevname,const char *user,const char *passwd,const char *apn,\
		         bool_t (*fnModemReset)(void),bool_t (*fnModemAtregnet)(char *devname,char *apn),\
				 fnNetDevEventHook eventhook)
{
	bool_t result = false;
	tagPppCtrl  *ppp;
	//malloc a ppp device rsc
	ppp = malloc(sizeof(tagPppCtrl));
	if(NULL == ppp)
	{
		printf("%s:Mem Err\n\r",__FUNCTION__);
		goto EXIT_PPPMEM;
	}
	memset(ppp,0,sizeof(tagPppCtrl));
	ppp->sc.buflen = CN_PPP_BUFLEN;     //send buffer
	ppp->sc.mutex = mutex_init(NULL);
	if(NULL == ppp->sc.mutex)
	{
		printf("%s:mutex Err\n\r",__FUNCTION__);
		goto EXIT_TXBUF_MUTEX;
	}
	//initialize the ppp control block and  add it to the ppp task
	if(NULL!=apn)
	{
		strncpy(ppp->apn,apn,CN_NAME_LIMIT);
	}
	if(NULL != user)
	{
		strncpy(ppp->user,user,CN_NAME_LIMIT);
	}
	if(NULL != passwd)
	{
		strncpy(ppp->passwd,passwd,CN_NAME_LIMIT);
	}
	if(NULL != iodevname)
	{
		strncpy(ppp->iodevname,iodevname,CN_NAME_LIMIT);
	}
	if(NULL != netdevname)
	{
		strncpy(ppp->netdevname,netdevname,CN_NAME_LIMIT);
	}
	ppp->reg.modemreset = fnModemReset;
	ppp->reg.regnet = fnModemAtregnet;
	ppp->reg.regnetauto = false;
	ppp->reg.regnetstart = false;
	//initialize the ppp internal member
	__PppMsReset(ppp,EN_PPP_DEAD);
	//add a netdevice to the stack
	ppp->nfd=__PppNetDevAdd(ppp);
	if(NULL == (void *)ppp->nfd)
	{
		printf("%s:NETDEV Err\n\r",__FUNCTION__);
		goto EXIT_NETDEV;
	}
	//add the event hook here
	if(NULL != eventhook)
	{
		NetDevRegisterEventHook(ppp->nfd,NULL,eventhook);
	}
	//add the device to the task list
	result = __PppAddTask(ppp);
	if(false == result)
	{
		printf("%s:Task Err\n\r",__FUNCTION__);
		goto EXIT_PPPTASK;
	}
	return result;

EXIT_PPPTASK:
EXIT_NETDEV:
	mutex_del(ppp->sc.mutex);
EXIT_TXBUF_MUTEX:
	free(ppp);
EXIT_PPPMEM:
	return false;
}





