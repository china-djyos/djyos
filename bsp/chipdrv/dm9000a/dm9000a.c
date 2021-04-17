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
// �ļ���     ��dm9000c.c
// ģ������: DM9000cep������������DJYOS����̫��Э��ʵ��Ҫ��ʵ�ֵ������ײ�����
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 18/09.2014
// =============================================================================
#include <sys/socket.h>

#include "dm9000a.h"
#include <shell.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"ethernet dm9000a"//dm9000a����
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:none                //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"lock","heap","tcpip"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_ETHERNET_DM9000A == false )
//#warning  " ethernet_dm9000a  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_ETHERNET_DM9000A    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure


struct DM9000_Dbg
{
    u32 IsrCnt;
    u32 IsrRcvCnt;
    u32 RcvTaskCnt;
    u32 RcvPkgCnt;
    u32 RxReadyErr;
    u32 RxHardOverCnt;
    u32 SndCnt;
    u32 ResetCnt;
};

static struct DM9000_Dbg dm9000dbg  = {0,0,0,0,0,0,0,0};

#ifndef DM9000_DBG
#define DM9000_DBG       printk
#endif

#define CN_PKG_MAX_LEN   1522
#define CN_DM9000NAME_LEN  32
typedef struct
{
    char   devname[CN_DM9000NAME_LEN];
    u8     devmac[CN_MACADDR_LEN];
    struct SemaphoreLCB rcvsync;
    struct MutexLCB     devsync;
    struct NetDev* handle;
    u32 irqno;
    bool_t (*clearextint)(u32 irqno);
    u16 *cmdaddr;
    u16 *dataddr;
}tagDm9000Dev;

u8  regread(tagDm9000Dev *dm9000,u16 reg)
{
    u16  data;
    *dm9000->cmdaddr = reg;
    data = *dm9000->dataddr;
    return (u8)data;
}
void  regwrite(tagDm9000Dev *dm9000,u16 reg,u16 data)
{
    *dm9000->cmdaddr = reg;
    *dm9000->dataddr = data;
    return;
}

static u8  regTab[]={0,1,2,3,4,5,6,7,8,9,10,254,255};
#define CN_REGLEN  sizeof(regTab)

void __showDm9000Reg(tagDm9000Dev *dm9000)
{
    int i;
    u8  value;
    for(i =0;i < CN_REGLEN;i++ )
    {
        value = regread(dm9000,regTab[i]);
        printk("reg:0x%02x:value:0x%02x\n\r",regTab[i],value);
    }

    return;
}

static bool_t __dm9000Probe(tagDm9000Dev *dm9000)
{
    u32 id_val;

    id_val  = regread(dm9000,DM9KS_VID_L);
    id_val |= regread(dm9000,DM9KS_VID_H) << 8;
    id_val |= regread(dm9000,DM9KS_PID_L) << 16;
    id_val |= regread(dm9000,DM9KS_PID_H) << 24;
    if (id_val == DM9KS_ID)
    {
        DM9000_DBG("dm9000 i/o:  id: 0x%x \r\n",id_val);
        return true;
    }
    else
    {
        DM9000_DBG("dm9000 not found !\r\n");
        return false;
    }
}
// =============================================================================
// ��  ��: дdm9000 PHY �Ĵ���
// ��  ��: phy_reg �� �Ĵ�����ַ
//       writedata �� д�������
// ��  ��: ��
// =============================================================================
static void __dm9000PhyWrite(tagDm9000Dev *dm9000,u8 phy_reg, u16 writedata)
{
    /* ����д�� PHY �Ĵ�����λ�� */
    regwrite(dm9000,DM9000_REG_EPAR, phy_reg | DM9000_PHY);

    /* ����д�� PHY �Ĵ�����ֵ */
    regwrite(dm9000,DM9000_REG_EPDRH, ( writedata >> 8 ) & 0xff);
    regwrite(dm9000,DM9000_REG_EPDRL, writedata & 0xff);

    regwrite(dm9000,DM9000_REG_EPCR, 0x0a);                       /* ������д�� PHY �Ĵ��� */
    while(regread(dm9000,DM9000_REG_EPCR) & 0x01); //todo ���˳�����  /* ��Ѱ�Ƿ�ִ�н��� */
    regwrite(dm9000,DM9000_REG_EPCR, 0x08);                       /* ���д������ */
}

static u16 __dm9000PhyRead(tagDm9000Dev *dm9000,int reg)
{
    u16 val;

    /* Fill the phyxcer register into REG_0C */
    regwrite(dm9000,DM9000_EPAR, DM9000_PHY | reg);
    regwrite(dm9000,DM9000_EPCR, 0xc);  /* Issue phyxcer read command */
    DJY_DelayUs(100);           /* Wait read complete */
    val = (regread(dm9000,DM9000_EPDRH) << 8) | regread(dm9000,DM9000_EPDRL);
    regwrite(dm9000,DM9000_EPCR, 0x8);  /* Clear phyxcer read command */

    return val;
}
// =============================================================================
// ��  ��: ��dm9000E���������λ
// ��������
// ���أ���
// =============================================================================
static  void __dm9000Reset(tagDm9000Dev *dm9000)
{

    if(Lock_MutexPend(&dm9000->devsync,CN_TIMEOUT_FOREVER))
    {
        regwrite(dm9000,DM9000_GPCR, (1<<0));            /* �� dm9000 ����������� */
        regwrite(dm9000,DM9000_GPCR,0);

        regwrite(dm9000,DM9000_NCR, DM9000_REG_RESET);
        do {
            DM9000_DBG("resetting the DM9000, 1st reset\r\n");
            DJY_DelayUs(25);                            /* Wait at least 20 us */
        } while (regread(dm9000,DM9000_NCR) & 1);

        regwrite(dm9000,DM9000_REG_NCR, 0x00);
        regwrite(dm9000,DM9000_REG_NCR, DM9000_REG_RESET);            /* �� dm9000 ����������� */
        do {
            DM9000_DBG("resetting the DM9000, 2nd reset\r\n");
            DJY_DelayUs(25); /* Wait at least 20 us */
        } while (regread(dm9000,DM9000_NCR) & 1);

        /* Check whether the ethernet controller is present */
        if ((regread(dm9000,DM9000_PIDL) != 0x0) ||
            (regread(dm9000,DM9000_PIDH) != 0x90))
            printf("ERROR: resetting DM9000 -> not responding.\r\n");

        dm9000dbg.ResetCnt ++;
        Lock_MutexPost(&dm9000->devsync);
    }
}


// =============================================================================
// ���ܣ����� dm9000 MAC �� �㲥 �� �ಥ �Ĵ���
// ��������
// ���أ���
// =============================================================================
static void dm9000_hash_table(tagDm9000Dev *dm9000)
{
    u8 i;
    u8 mac[6],*p;

    /* ���� ���� MAC λ�ã������ MyHardware */
    for(i = 0; i < 6; i++)
    {
        regwrite(dm9000,DM9000_REG_PAR + i, dm9000->devmac[i]);
    }

    if(1)
    {
        p   =mac;
        for(i = 0; i < 6; i++)
        {
            p[i] =regread(dm9000,DM9000_REG_PAR + i);
        }

        DM9000_DBG("DM_MAC:  %02X,%02X,%02X,%02X,%02X,%02X.\r\n",
                    p[0],p[1],p[2],p[3],p[4],p[5]);
    }

    for(i = 0; i < 8; i++)                              /* ��� �����ಥ���� */
    {
        regwrite(dm9000,DM9000_REG_MAR + i, 0x00);
    }

    regwrite(dm9000,DM9000_REG_MAR + 7, 0x80);                    /* ������ �㲥�� ���� */
}

// =============================================================================
//��  ��: ��ʼ��dm9000E������������ϣ����صļĴ������õ�
// ��������
// ���أ���
// =============================================================================
static void __dm9000HardInit(tagDm9000Dev *dm9000)
{
    u32 timeout = 1000;
    u8 io_mode;
    u16 lnk;

    regwrite(dm9000,DM9000_REG_GPR, DM9000_PHY_OFF);
    __dm9000Reset(dm9000);

    __dm9000PhyWrite(dm9000,0x04,0x00a1);
    __dm9000PhyWrite(dm9000,0x00,0x1200);

    io_mode = regread(dm9000,DM9000_ISR) >> 6;
    switch (io_mode) {
    case 0x0:  /* 16-bit mode */
        printf("DM9000: running in 16 bit mode\r\n");
        break;
    case 0x01:  /* 32-bit mode */
        printf("DM9000: running in 32 bit mode\r\n");
        break;
    case 0x02: /* 8 bit mode */
        printf("DM9000: running in 8 bit mode\r\n");
        break;
    default:
        /* Assume 8 bit mode, will probably not work anyway */
        printf("DM9000: Undefined IO-mode:0x%x\r\n", io_mode);
        break;
    }

    /* Program operating register, only internal phy supported */
    regwrite(dm9000,DM9000_NCR, 0x0);
    /* TX Polling clear */
    regwrite(dm9000,DM9000_TCR, 0);
    /* Less 3Kb, 200us */
    regwrite(dm9000,DM9000_BPTR, BPTR_BPHW(3) | BPTR_JPT_600US);
    /* Flow Control : High/Low Water */
    regwrite(dm9000,DM9000_FCTR, FCTR_HWOT(3) | FCTR_LWOT(8));
    /* SH FIXME: This looks strange! Flow Control */
    regwrite(dm9000,DM9000_FCR, 0x28);
    /* Special Mode */
    regwrite(dm9000,DM9000_SMCR, 0x00);
    /* clear TX status */
    regwrite(dm9000,DM9000_NSR, NSR_WAKEST | NSR_TX2END | NSR_TX1END);
    /* Clear interrupt status */
    regwrite(dm9000,DM9000_ISR, ISR_ROOS | ISR_ROS | ISR_PTS | ISR_PRS);

    regwrite(dm9000,DM9000_BUSCR, 0x6b);

    dm9000_hash_table(dm9000);

    /* Activate DM9000 */
    /* Enable TX/RX interrupt mask */
    regwrite(dm9000,DM9000_IMR, IMR_PAR);

    /* RX enable */
    regwrite(dm9000,DM9000_RCR, RCR_DIS_LONG | RCR_DIS_CRC | RCR_RXEN);

    regwrite(dm9000,DM9000_REG_GPR, DM9000_PHY_ON);  /* ���� PHY ����, ���� PHY */

    while(!(regread(dm9000,DM9000_NSR) & NSR_LINKST))
    {
        if(timeout == 0)
        {
            DM9000_DBG("DM9000 Link failed!\r\n");
            break;
        }
        timeout--;
        DJY_EventDelay(1000);
    }

    if(timeout)
    {
        DM9000_DBG("DM9000 Linked : ");
        lnk = __dm9000PhyRead(dm9000,17) >> 12;
        switch (lnk)
        {
        case 1:
            printf("10M half duplex ");
            break;
        case 2:
            printf("10M full duplex ");
            break;
        case 4:
            printf("100M half duplex ");
            break;
        case 8:
            printf("100M full duplex ");
            break;
        default:
            printf("unknown: %d ", lnk);
            break;
        }
        printf("\r\n");
    }
}

// =============================================================================
// �������ܣ�NetHard_Send
//          �����������ݰ�
// ���������netdev,ʹ�õ�����
//        pkg,�����͵����ݰ�
//        netdevtask,������Ҫ��ɵĹ���
// ���������
// ����ֵ  ��true���ͳɹ�  false����ʧ�ܡ�
// ˵��    �����ÿ����ķ�ʽ���ͣ���������ʹ��������
// =============================================================================
static bool_t __dm9000Snd(ptu32_t handle,struct NetPkg *pkg,u32 netdevtask)
{
    bool_t  result;
    struct NetPkg *tmp;
    u8 *mysrc;
    u16 sndlen;
    u16 i,u16Data,sndTimeout = 5000;
    tagDm9000Dev  *dm9000;

    result = false;
    if((0 != handle)&&(NULL != pkg))
    {
        dm9000 = (tagDm9000Dev  *)NetDev_GetPrivate(handle);
        sndlen = 0;
        tmp = pkg;
        //cout the len
        tmp = pkg;
        sndlen = 0;
        PkgFrameDatastatistics(tmp);
//      while(NULL != tmp)
//      {
//          sndlen +=tmp->datalen;
//          if(PkgIsBufferEnd(tmp))
//          {
//              tmp = NULL;
//          }
//          else
//          {
//              tmp = PkgGetNextUnit(tmp);
//          }
//      }
        if(Lock_MutexPend(&dm9000->devsync,CN_TIMEOUT_FOREVER))
        {
            /* ��� dm9000 �Ƿ��ڴ����У����ǵȴ�ֱ�����ͽ��� */
            while(regread(dm9000,DM9000_REG_TCR) & DM9000_TCR_SET)
            {
                DJY_DelayUs (5);
            }

            regwrite(dm9000,DM9000_ISR, IMR_PTM);
            //snd all the pkg
            tmp = pkg;
            //init the dm9000
            regwrite(dm9000,DM9000_TXPLH, (sndlen>>8) & 0x0ff);//���÷������ݳ���
            regwrite(dm9000,DM9000_TXPLL, sndlen & 0x0ff);

            *dm9000->cmdaddr = DM9000_REG_MWCMD;                    //�������ݻ��渳�����ݶ˿�

            while(NULL!= tmp)
            {
                sndlen = PkgGetDataLen(tmp);
//              sndlen = tmp->datalen;
                mysrc = PkgGetCurrentBuffer(pkg);
//              mysrc = (u8 *)(tmp->buf + tmp->offset);
                //��������
                i = sndlen;
                while(i > 1)
                {
                    u16Data = (*mysrc++);
                    u16Data += ((*mysrc++) << 8);
                    i -= 2;
                    *dm9000->dataddr = u16Data;
                }
                if(i > 0)
                {
                    u16Data = (u16)((*mysrc++) & 0x00FF);
                    *dm9000->dataddr = u16Data;
                }
                if(PkgIsBufferEnd(tmp))
                {
                    tmp = NULL;
                }
                else
                {
                    tmp = PkgGetNextUnit(tmp);
                }
            }
            //ok now start transfer;
            regwrite(dm9000,DM9000_REG_TCR, DM9000_TCR_SET);  /* ���д��� */

            while ( !(regread(dm9000,DM9000_NSR) & (NSR_TX1END | NSR_TX2END)) ||
                !(regread(dm9000,DM9000_ISR) & IMR_PTM) )
            {
                DJY_DelayUs(1);
                if (!sndTimeout)
                {
                    printk("transmission timeout\n");
                    break;
                }
                sndTimeout --;
            }
            regwrite(dm9000,DM9000_ISR, IMR_PTM); /* Clear Tx bit in ISR */

            result = true;
            dm9000dbg.SndCnt ++;
            Lock_MutexPost(&dm9000->devsync);
        }

    }
    return result;
}
// =============================================================================
// �������ܣ�NetHard_Rcv
//          ���������ݺ���
// �����������
// ���������
// ����ֵ  ��
// ˵��    ������������
// =============================================================================
static struct NetPkg *__dm9000RcvPkg(tagDm9000Dev *dm9000)
{
    s32   rx_length,i;
    u8    rx_ready;
    u16 rx_status;
    struct NetPkg *pkg=NULL;
    u16 *dst;

    if(Lock_MutexPend(&dm9000->devsync,CN_TIMEOUT_FOREVER))
    {
        rx_ready = regread(dm9000,DM9000_MRCMDX);      //�ȶ�ȡһ����Ч������,һ�����0
        rx_ready = regread(dm9000,DM9000_MRCMDX);      //������ȡ�������ݰ����ֽ�

        if(rx_ready == 1)                              //�ж����ֽ��Ƿ�Ϊ1��0
        {
            dm9000dbg.RcvTaskCnt ++;
            *dm9000->cmdaddr = DM9000_MRCMD;            //������ȡ���ݰ�����
            rx_status = *dm9000->dataddr;               //״̬�ֽ�
            rx_length = *dm9000->dataddr;               //���ݳ���
            rx_status &= 0xff;

            if((rx_length > 0) && (rx_length <= CN_PKG_MAX_LEN))
            {
                if(rx_length%2)
                    pkg =PkgMalloc(rx_length+1,0);
                else
                    pkg =PkgMalloc(rx_length,0);
            }
            if(pkg != NULL)
            {
                dst = (u16 *)PkgGetCurrentBuffer(pkg);
//              dst = (u16 *)(pkg->buf + pkg->offset);
                if(!(rx_status & 0xbf00) && (rx_length < 10000))//�ж������Ƿ����Ҫ��
                {
                    for(i=0; i<rx_length; i+=2)          //16λ����ת��Ϊ8λ���ݴ洢
                    {
                        *dst++ = *dm9000->dataddr;
                    }
                }
                PkgSetDataLen(pkg, rx_length);
//              pkg->datalen = rx_length;
            }
        }
        else if(rx_ready == 0)
        {
            pkg = NULL;     //δ���յ�����
        }
        else
        {
            dm9000dbg.RxReadyErr ++;
        }

        Lock_MutexPost(&dm9000->devsync);
    }


    return pkg;
}


#define CN_DM9000RCV_TIMEOUT   (1*mS)    //5 seconds timeout
static ptu32_t dm9000Rcv(void)
{
    u8  rcvstat;
    struct NetPkg *pkg;
    tagDm9000Dev *dm9000;

    DJY_GetEventPara((ptu32_t *)&dm9000,NULL);
    while(1)
    {
        Lock_SempPend(&dm9000->rcvsync,CN_DM9000RCV_TIMEOUT);
        while((pkg = __dm9000RcvPkg(dm9000))!= NULL)
        {
            dm9000dbg.RcvPkgCnt ++;
            Link_NetDevPush(dm9000->handle,pkg);
            PkgTryFreePart(pkg);
        }

        rcvstat = regread(dm9000,DM9000_REG_NSR);
        if(rcvstat &NSR_RXOV)
        {
            dm9000dbg.RxHardOverCnt ++;
            rcvstat = regread(dm9000,DM9000_REG_RSR);
            printk("RSR  :0X%02x\n\r",rcvstat);
            rcvstat = regread(dm9000,DM9000_REG_ROCR);
            printk("ROCR :0X%02x\n\r",rcvstat);

            __dm9000HardInit(dm9000);
        }
    }
    return 0;
}

__attribute__((weak)) bool_t ClearExtInt(u32 irqno)
{
    //�˺������Ӧ��CPUʵʵ��
}
u32  __dm9000Isr(ptu32_t para)
{
    tagDm9000Dev *dm9000;
    u8 IntStatus;
    dm9000 = (tagDm9000Dev *)para;

    dm9000dbg.IsrCnt ++;

    regwrite(dm9000,DM9000_REG_IMR , DM9000_IMR_OFF); //�ر� DM9000A �ж�

    IntStatus = regread(dm9000,DM9000_REG_ISR);   //ȡ���жϲ���ֵ
    regwrite(dm9000,DM9000_REG_ISR,IntStatus);    //���ж�
    if(IntStatus & ISR_PRS)                       //�����ж�
    {
        dm9000dbg.IsrRcvCnt ++;
        Lock_SempPost(&dm9000->rcvsync);
    }
    dm9000->clearextint(dm9000->irqno);               //���CPU�жϱ�־

    regwrite(dm9000,DM9000_REG_IMR , DM9000_IMR_SET); // ���� DM9000A �ж�

    return 0;
}


static bool_t __dm9000CreateDev(tagDm9000Dev *dm9000)
{
    bool_t res = false;
    u16 evttID;
    u16 eventID;
    struct NetDevPara  devpara;

    Lock_MutexCreate_s(&dm9000->devsync,NULL);
    Lock_SempCreate_s(&dm9000->rcvsync,1,0,CN_BLOCK_FIFO,NULL);
    evttID = DJY_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS-1, 0, 1,
                            (ptu32_t (*)(void))dm9000Rcv,NULL, 0x1000, dm9000->devname);
    if(evttID == CN_EVTT_ID_INVALID)
    {
        goto EVTT_FAILED;
    }

    eventID=DJY_EventPop(evttID, NULL, 0, (ptu32_t)dm9000, 0, 0);
    if(eventID == CN_EVENT_ID_INVALID)
    {
        goto EVENT_FAILED;
    }
    devpara.ifsend = __dm9000Snd;
    devpara.iftype = EN_LINK_ETHERNET;
    devpara.devfunc = 0x00;//COULD DO NO CRC
    memcpy(devpara.mac, dm9000->devmac,CN_MACADDR_LEN);
    devpara.name = dm9000->devname;
    devpara.mtu = CN_ETH_MTU;
    devpara.Private = (ptu32_t)dm9000;
    dm9000->handle = NetDev_Install(&devpara);

    if(0 == dm9000->handle)
    {
        goto DEV_FAILED;
    }

    //OK,NOW INSTALL THE INTERRUPT
    Int_Register(dm9000->irqno);
    Int_IsrConnect(dm9000->irqno,__dm9000Isr);
    Int_SetClearType(dm9000->irqno,CN_INT_CLEAR_AUTO);
    Int_SettoAsynSignal(dm9000->irqno);
    Int_ClearLine(dm9000->irqno);
    Int_SetIsrPara(dm9000->irqno,(ptu32_t)dm9000);
    Int_RestoreAsynLine(dm9000->irqno);

    res = true;
    return res;

DEV_FAILED:
    //do the unpop
EVENT_FAILED:
    DJY_EvttUnregist(evttID);
EVTT_FAILED:
    Lock_MutexDelete_s(&dm9000->devsync);
    Lock_SempDelete_s(&dm9000->rcvsync);
    return res;
}

static tagDm9000Dev *pDm9000;
//bool_t dm9000debuginfo(char* param)
bool_t dm9000(char* param)
{
    u8 rAddrH,rAddrL;

    printf("DM9000 IsrCnt = %d \r\n",       dm9000dbg.IsrCnt);
    printf("DM9000 IsrRcvCnt = %d \r\n",    dm9000dbg.IsrRcvCnt);
    printf("DM9000 RcvPkgCnt = %d \r\n",    dm9000dbg.RcvPkgCnt);
    printf("DM9000 RcvTaskCnt = %d \r\n",   dm9000dbg.RcvTaskCnt);
    printf("DM9000 RxReadyErr = %d \r\n",   dm9000dbg.RxReadyErr);
    printf("DM9000 RxHardOverCnt = %d \r\n",dm9000dbg.RxHardOverCnt);
    printf("DM9000 SndCnt = %d \r\n",       dm9000dbg.SndCnt);
    printf("DM9000 ResetCnt = %d \r\n", dm9000dbg.ResetCnt);

    rAddrH = regread(pDm9000,DM9000_MRRH);
    rAddrL = regread(pDm9000,DM9000_MRRL);

    printf("DM9000 Read Addr High = 0x%02x , Low = 0x%02x \r\n",rAddrH,rAddrL);
    return 1;
}
//bool_t dm9000reg(char *param)
bool_t dm9000reg(char *param)
{
    __showDm9000Reg(pDm9000);
    return 1;
}

//bool_t dm9000Reset(char *param)
bool_t dm9000reset(char *param)
{
    __dm9000HardInit(pDm9000);
    return 1;
}




//THIS FUNCTION USED BY USER TO INSTALL AN DM9000 DEV WITH THE SPECIFIED NAME AND MAC
bool_t Dm9000Install(tagDm9000Para *para)
{
    bool_t res = false;
    tagDm9000Dev *dm9000;

    dm9000 = malloc(sizeof(tagDm9000Dev));
    if(NULL == dm9000)
    {
        DM9000_DBG("%s:mem failed\n\r",__FUNCTION__);
        goto MEM_FAILED;
    }

    memset((void *)dm9000,0,sizeof(tagDm9000Dev));
    //cpy from the para
    memcpy((void *)dm9000->devname,(void *)para->devname,CN_DM9000NAME_LEN);
    memcpy((void *)dm9000->devmac,(void *)para->mac,CN_MACADDR_LEN);
    dm9000->cmdaddr = para->cmdaddr;
    dm9000->dataddr = para->dataddr;
    dm9000->irqno = para->irqno;
//    dm9000->clearextint = para->clearextint;
    dm9000->clearextint = ClearExtInt;

    //OK, NOW PROBE THE DM9000A DEVICE
    res = __dm9000Probe(dm9000);
    if(false == res)
    {
        DM9000_DBG("%s:probe failed\n\r",__FUNCTION__);
        goto PROBE_FAILED;
    }

    //OK,NOW HARD INITIALIZE
    __dm9000HardInit(dm9000);

    //NOW REGISTER IT FOR THE NETSTACK
    res = __dm9000CreateDev(dm9000);
    if(false == res)
    {
        DM9000_DBG("%s:Creaet Dm9000 Dev failed\n\r",__FUNCTION__);
        goto DEV_FAILED;
    }
    DM9000_DBG("%s:ISNTALL DM9000 DEV SUCCESS\n\r",__FUNCTION__);
    pDm9000 = dm9000;
    __showDm9000Reg(dm9000);
    return res;


DEV_FAILED:
PROBE_FAILED:
    free((void *)dm9000);
    dm9000 = NULL;

MEM_FAILED:
    return res;
}

bool_t shellDm9000Reg(char *param)
{
    __showDm9000Reg(pDm9000);
    return true;
}
ADD_TO_ROUTINE_SHELL(dm9000,dm9000,"dm9000 debug info");
ADD_TO_ROUTINE_SHELL(dm9000reg,dm9000reg,"print dm9000 reg");
ADD_TO_ROUTINE_SHELL(dm9000reset,dm9000reset,"reset dm9000");

