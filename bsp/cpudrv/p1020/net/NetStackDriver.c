//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
// =============================================================================

// ģ������: NetStackDriver.c
// ģ��汾: V1.00
// ������Ա: Administrator
// ����ʱ��: 10:12:46 AM/Aug 18, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================

#include "stdint.h"
#include "string.h"

#include "os.h"

#include "cpu_peri_tsec.h"
#include <sys/socket.h>

#define CN_HARD_BOARD            0x02
u8  sgNetHardMac[6] = {0x00,0x01, 0x02, 0x03, 0x04, CN_HARD_BOARD};
#define CN_PKG_LEN  1536
static struct NetDev *pgChkNetDev = NULL;
static u8  sgSndBuf[CN_PKG_LEN];
// =============================================================================
// �������ܣ�NetHard_Send
//          �����������ݰ�
// ���������netdev,ʹ�õ�����
//          pkg,�����͵����ݰ�
//          netdevtask,��Ҫ������ɵĹ���
// ���������
// ����ֵ  ��true���ͳɹ�  false����ʧ�ܡ�
// ˵��    �����ÿ����ķ�ʽ���ͣ���������ʹ��������
// =============================================================================
bool_t NetHard_Send(struct NetDev *netdev,struct NetPkg *pkg,u32 netdevtask)
{
    bool_t  result;
    struct NetPkg *tmp;
    u8 *src;
    u8 *dst;
    u32 sndlen;

    result = false;
    if((pgChkNetDev == netdev)&&(NULL != pkg))
    {
        sndlen = 0;
        tmp = pkg;
        //�������֮��ǵ��ͷ�
        sndlen = PkgListDataCopy(tmp,sgSndBuf);
//      while(NULL != tmp)
//      {
//          src = (u8 *)(tmp->buf + tmp->offset);
//          dst = (u8 *)(sgSndBuf + sndlen);
//          memcpy(dst, src, tmp->datalen);
//          sndlen += tmp->datalen;
//          tmp = PkgGetNextUnit(tmp);
//      }
        if(sndlen < 60)//С��60�İ����ǵ����
        {
            dst = (u8 *)(sgSndBuf + sndlen);
            memset(dst,0 ,60-sndlen);
            sndlen = 60;
        }
        Net_SendPacket(1,sgSndBuf,sndlen);
        result = true;
    }
    return result;
}

// =============================================================================
// �������ܣ�NetHard_RcvIntIsr
//          ������������ʱ���жϷ�����
// �����������
// ���������
// ����ֵ  ��
// ˵��    ��Ŀǰ������ѯ�ķ�ʽģ����ж�
// =============================================================================
void NetHard_RcvIntIsr(void)
{
    bool_t      newpkg;
    s32         rcvlen;
    u8          *rcvbuf;
    struct NetPkg *pkg;

    newpkg = true;
    pkg =NULL;
    while(1)
    {
        if(newpkg)
        {
            pkg =PkgMalloc(CN_PKG_LEN, CN_PKLGLST_END);
        }

        if(NULL != pkg)
        {
            rcvbuf = PkgGetCurrentBuffer(pkg);
//          rcvbuf = (u8 *)(pkg->buf + pkg->offset);

            rcvlen = Net_RecvPacket(1,rcvbuf);//��ѯ�������������ʱ
            if(0 != rcvlen)
            {
                PkgSetNextUnit(pkg,NULL);
                PkgSetDataLen(pkg, rcvlen);
//              pkg->datalen = rcvlen;
//              pkg->partnext = NULL;
                newpkg = true;
                NetDev_PostPkg(pgChkNetDev,pkg);
                PkgTryFreePart(pkg);
            }
            else
            {
                newpkg = false;
            }
        }
        DJY_EventDelay(1*mS);
    }
    return;
}


// =============================================================================
// �������ܣ�NetHard_AddNetDev
//          ��Э��ջ���һ�������豸
// �����������
// �����������
// ����ֵ  ��true�ɹ�  falseʧ�ܡ�
// ˵��    ��1,����豸
//        2,���·��
// =============================================================================
bool_t NetHard_AddNetDev(void)
{
    u16   evtt_id;
    bool_t  result;
    struct NetDevPara  devpara;

//��ʼ������
    extern bool_t module_tsec_init(s32 num,u8 last_mac);//eTSECx��ʼ��
    module_tsec_init(1,10);

    devpara.ifsend = NetHard_Send;
    devpara.iftype = EN_LINK_ETHERNET;
    memcpy(devpara.mac, sgNetHardMac,6);
    devpara.name = "TestDriver";
    devpara.Private = 0;
    devpara.linklen = 14;
    devpara.pkglen = 1500;
    devpara.devfunc = 0;

    result = false;
    pgChkNetDev = NetDev_Install(&devpara);
    if(pgChkNetDev != NULL)
    {
        result = true;
    }
    //��ӽ����߳�
    evtt_id = DJY_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS-2, 0, 1,
            (ptu32_t (*)(void))NetHard_RcvIntIsr,NULL, 0x100, "NetHard_RcvIsr");
    if (evtt_id != CN_EVTT_ID_INVALID)
    {
        evtt_id = DJY_EventPop(evtt_id, NULL, 0,0, 0, 0);
    }
    return result;
}


