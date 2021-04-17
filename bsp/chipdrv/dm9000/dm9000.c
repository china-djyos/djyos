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
// �ļ���     ��dm9000.c
// ģ������: DM9000������������DJYOS����̫��Э��ʵ��Ҫ��ʵ�ֵ������ײ�����
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 18/09.2014
// =============================================================================

#include "stdint.h"
#include <tcpip/netdev.h>
#include "rout.h"
#include "dm9000.h"
#include "cpu_peri.h"
#include "stdio.h"
#include "string.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t DM9000_ModuleInit(ptu32_t para);
//    DM9000_ModuleInit(0);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"ethernet dm9000"//dm9000
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                     //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                  //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                    //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium                  //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                    //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"lock","heap","tcpip"//������������������������none����ʾ�������������
                                    //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                    //����������������������г����á�,���ָ�
//weakdependence:"none"             //��������������������������none����ʾ�������������
                                    //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                    //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                    //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_ETHERNET_DM9000 == false )
//#warning  " ethernet_dm9000  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_ETHERNET_DM9000    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#define CFG_DMA9000_ADDR          0x20000300//"��ַ��"��dm9000��ַ��
#define CFG_DMA9000_DATA          0x20000304//"���ݿ�"��dm9000���ݿ�
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

// =============================================================================
//#define DM_ADDR_PORT (*((volatile unsigned short *) 0x20000300)) //��ַ��
//#define DM_DATA_PORT (*((volatile unsigned short *) 0x20000304)) //���ݿ�

#define DM_ADDR_PORT (*((volatile unsigned short *) CFG_DMA9000_ADDR)) //��ַ��
#define DM_DATA_PORT (*((volatile unsigned short *) CFG_DMA9000_ADDR)) //���ݿ�

#define DM9000_DBG   printk
#define CN_MAC_SEMP_TIMEOUT    (100*mS)
#define CN_PHY_INTIT_TIMEOUT   (3000*mS)
// =============================================================================
#define CN_HARD_BOARD            0x02
static u8  sgNetHardMac[6] = {0x3C,0x97, 0x0E, 0x44, 0x07, 0XBD};
static u32  sgNetHardIpAddrMain = 0xC0A80100;
static u32  sgNetHardIpMsk = 0xFFFFFF00 ; //255.255.255.0
static u32  sgNetHardGateWay = 0xC0A80101; //192.168.1.1
static tagRoutTab  *ptNetRout = NULL;
static struct NetDev   *ptNetDev = NULL;
#define CN_PKG_MAX_LEN   1500
// =============================================================================

//дDM9000�Ĵ���
static void dm_reg_write(unsigned char reg, unsigned char data)
{
    DM_ADDR_PORT = reg;            //���Ĵ�����ַд����ַ�˿�
    DM_DATA_PORT = data;            //������д�����ݶ˿�
}

//��DM9000�Ĵ���
static unsigned char dm_reg_read(unsigned char reg)
{
    DM_ADDR_PORT = reg;
    return DM_DATA_PORT;             //�����ݴ����ݶ˿ڶ���
}

//��DM9000PHY�Ĵ���
static u16 dm_reg_read_phy(u16 reg)
{
    u16 data;
    dm_reg_write(DM9000_EPAR, reg|0x40);
    dm_reg_write(DM9000_EPCR, 0x0c);            //phy������
    data = dm_reg_read(DM9000_EPDRH);           //��phy
    data = (data<<8) | dm_reg_read(DM9000_EPDRL);
    DJY_DelayUs(20);
    dm_reg_write(DM9000_EPCR, 0x08);            //�����phy����
    return data;
}
//��DM9000PHY�Ĵ���д����
static void dm_reg_write_phy(u16 reg, u16 data)
{
    dm_reg_write(DM9000_EPAR, reg|0x40);
    dm_reg_write(DM9000_EPDRH, (data>>8)&0xff);
    dm_reg_write(DM9000_EPDRL, data&0xff);
    dm_reg_write(DM9000_EPCR, 0x0a);            //phyд����
    DJY_DelayUs(20);
    dm_reg_write(DM9000_EPCR, 0x08);            //���PHY������
}

// =============================================================================
// ���ܣ���ȡDM9000��ID����������ID����ȷ���򱨾�������ӡ��Ϣ���ú�������false����
//       ��ʼ������ʧ�ܣ�ģ��ֱ�ӷ���
// ��������
// ���أ�true,�ɹ�;false,ʧ��
// ˵�����ú������سɹ���ʵ��˵����CPU��Ӳ��֮���ͨ������
// =============================================================================
bool_t DM9000_Probe(void)
{
    unsigned int id_val;

    id_val  = dm_reg_read(DM9000_VIDL);
    id_val |= dm_reg_read(DM9000_VIDH) << 8;
    id_val |= dm_reg_read(DM9000_PIDL) << 16;
    id_val |= dm_reg_read(DM9000_PIDH) << 24;
    if (id_val == DM9000_ID)
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
// ���ܣ�DM9000PHYоƬ��λ��������Ϊ����Ӧģʽ
// ��������
// ���أ���
// =============================================================================
bool_t DM9000_PhyInit(void)
{
    u32 timeout = CN_PHY_INTIT_TIMEOUT;
    dm_reg_write(DM9000_GPCR,1);            //����GPIO0Ϊ���
    dm_reg_write(DM9000_GPR,0);             //�����ڲ�PHY
    DJY_DelayUs(3 * mS);

    dm_reg_write_phy(DM9000_BMCR,0x8000);
    DJY_DelayUs(30);
    while(dm_reg_read_phy(DM9000_BMCR) & 0x8000)
    {
        DJY_DelayUs(10*mS);
        timeout = timeout - 10*mS;
        if(timeout == 0)
        {
            DM9000_DBG("phy init failed !\r\n");
            return false;
        }
    }

    DM9000_DBG("Phy Reset Success!\r\n");

    dm_reg_write_phy(DM9000_BMCR,0x1200);//auto negotiation
    DJY_DelayUs(30);
    while(!(dm_reg_read_phy(DM9000_BMSR) & 0x20))
    {
        DJY_DelayUs(10*mS);
        timeout = timeout - 10*mS;
        if(timeout == 0)
        {
            DM9000_DBG("phy init failed !\r\n");
            return false;
        }
    }
    DM9000_DBG("auto negotiation completed!\r\n");

    while(!(dm_reg_read_phy(DM9000_BMSR) & 0x04))
    {
        DJY_DelayUs(10*mS);
        timeout = timeout - 10*mS;
        if(timeout == 0)
        {
            DM9000_DBG("phy init failed !\r\n");
            return false;
        }
    }
    DM9000_DBG("phy linked!\r\n");
    return true;
}

// =============================================================================
// ���ܣ�DM9000����оƬ��λ
// ��������
// ���أ���
// =============================================================================
void DM9000_reset(void)
{
    DM9000_DBG("resetting\n");
    dm_reg_write(DM9000_NCR, NCR_RST);
    DJY_DelayUs(3000);      /* delay 3ms */
    dm_reg_write(DM9000_NCR, 0x00);

    dm_reg_write(DM9000_NCR, NCR_RST);
    DJY_DelayUs(3000);      /* delay 3ms */
    dm_reg_write(DM9000_NCR, 0x00);
}

// =============================================================================
// ���ܣ�DM9000��̫������Ӳ����ʼ����������λ�������巢�ͺͽ���״̬������MAC��ַ��
//      ʹ�ܽ����ж���Ϣ�������������жϣ����������ж��ߣ�
// ��������
// ���أ�
// =============================================================================
bool_t DM9000_HardInit(void)
{
    dm_reg_write(DM9000_NCR,1);             //�����λDM9000
    DJY_DelayUs(30);                        //��ʱ����20��s
    dm_reg_write(DM9000_NCR,0);             //�����λλ

    dm_reg_write(DM9000_NCR,1);             //Ϊ��ȷ����λ��ȷ���ٴθ�λ
    DJY_DelayUs(30);
    dm_reg_write(DM9000_NCR,0);

    if(false == DM9000_PhyInit())                       //PHYоƬ��ʼ��
        return false;

    dm_reg_write(DM9000_NSR,0x2c);           //��TX״̬
    dm_reg_write(DM9000_ISR,0xf);            //���ж�״̬

    dm_reg_write(DM9000_RCR,0x39);           //����RX����
    dm_reg_write(DM9000_TCR,0);              //����TX����
    dm_reg_write(DM9000_BPTR,0x3f);
    dm_reg_write(DM9000_FCTR,0x3a);
    dm_reg_write(DM9000_FCR,0xff);
    dm_reg_write(DM9000_SMCR,0x00);

    dm_reg_write(DM9000_PAR+0,sgNetHardMac[0]);  //����MAC��ַ
    dm_reg_write(DM9000_PAR+1,sgNetHardMac[1]);
    dm_reg_write(DM9000_PAR+2,sgNetHardMac[2]);
    dm_reg_write(DM9000_PAR+3,sgNetHardMac[3]);
    dm_reg_write(DM9000_PAR+4,sgNetHardMac[4]);
    dm_reg_write(DM9000_PAR+5,sgNetHardMac[5]);

    dm_reg_write(DM9000_NSR,0x2c);           //�ٴ���TX״̬
    dm_reg_write(DM9000_ISR,0xf);            //�ٴ����ж�״̬

    dm_reg_write(DM9000_IMR,0x81);           //�򿪽��������ж�

    while(!(dm_reg_read(DM9000_NSR) & NSR_LINKST));

    DM9000_DBG("DM9000 Linked!\r\n");

    return true;
}

// =============================================================================
// ���ܣ�DM9000�жϳ�ʼ����DM9000�ж����Žӵ�CPU��GPIO���ţ�����轫����������Ϊ�ⲿ
//       �жϣ������ô�����ƽ��ͬʱ�������ж��ߵ��ж�ϵͳ
// ��������
// ����ֵ  ��
// =============================================================================
u32 DM9000_ISR_Handler(ptu32_t IntLine);
void DM9000_IntInit(void)
{
    //�����ⲿ����EINT7/GPF7
    pg_gpio_reg->GPFCON &= ~(3<<14);
    pg_gpio_reg->GPFCON |= (2<<14);     //����Ϊ����
    pg_gpio_reg->EXTINT0 &= ~(7<<28);
    pg_gpio_reg->EXTINT0  |= (4<<28);   //���������ش���
    pg_gpio_reg->EINTPEND |= (1<<7);
    pg_gpio_reg->EINTMASK &= (~(01<<7));

    //DM9000���ж���������ж��źŵ�CPU��EINT7
    Int_Register(cn_int_line_eint4_7);
    Int_IsrConnect(cn_int_line_eint4_7,DM9000_ISR_Handler);
    Int_SettoAsynSignal(cn_int_line_eint4_7);
    Int_ClearLine(cn_int_line_eint4_7);
    Int_RestoreAsynLine(cn_int_line_eint4_7);
}

// =============================================================================
// ���ܣ�DM9000�������ݰ������������ݰ����͵����������������ͣ���ɺ󷵻أ�������ѯ
//       �ķ�ʽ����
// ��������
// ����ֵ  ��
// =============================================================================
void DM9000_TxPacket(unsigned char *datas, int length)
{
    int i;

    dm_reg_write(DM9000_IMR, 0x80);                 //�ڷ������ݹ����н�ֹ�����ж�

    dm_reg_write(DM9000_TXPLH, (length>>8) & 0x0ff);//���÷������ݳ���
    dm_reg_write(DM9000_TXPLL, length & 0x0ff);

    DM_ADDR_PORT = DM9000_MWCMD;                    //�������ݻ��渳�����ݶ˿�

    //��������
    for(i=0;i<length;i+=2)
    {
//      DJY_DelayUs(50);
        DM_DATA_PORT = datas[i]|(datas[i+1]<<8);  //8λ����ת��Ϊ16λ�������
    }

    dm_reg_write(DM9000_TCR, 0x01);                 //�����ݷ��͵���̫����

    while((dm_reg_read(DM9000_NSR) & 0x0c) == 0)
    ;                                               //�ȴ����ݷ������

//  DJY_DelayUs(50);

    dm_reg_write(DM9000_NSR, 0x2c);                 //���TX״̬
    dm_reg_write(DM9000_IMR, 0x81);                 //��DM9000���������ж�
}

//static s64   timedriverstart;
//static s64   timedriverend;
//static u32   timedriverused;
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
bool_t DM9000_Send(struct NetDev *dev,struct NetPkg *pkg,u32 netdevtask)
{
    bool_t  result;
    struct NetPkg *tmp;
    u16 *mysrc;
    u16 sndlen;
    u16 i;
    atom_low_t atom;

    result = false;
    if((ptNetDev == dev)&&(NULL != pkg))
    {
        sndlen = 0;
        tmp = pkg;
        //cout the len
        tmp = pkg;
        sndlen = 0;
        while(NULL != tmp)
        {
            sndlen +=PkgGetDataLen(tmp);
//          sndlen +=tmp->datalen;

//          if(tmp->pkgflag&CN_PKLGLST_END)
            if(PkgIsBufferEnd(tmp))
            {
                tmp = NULL;
            }
            else
            {
                tmp = PkgGetNextUnit(tmp);
            }
        }
        atom = Int_LowAtomStart();
//        timedriverstart = DJY_GetSysTime();
        //snd all the pkg
        tmp = pkg;
        //init the dm9000
        dm_reg_write(DM9000_IMR, 0x80);                 //�ڷ������ݹ����н�ֹ�����ж�

        dm_reg_write(DM9000_TXPLH, (sndlen>>8) & 0x0ff);//���÷������ݳ���
        dm_reg_write(DM9000_TXPLL, sndlen & 0x0ff);

        DM_ADDR_PORT = DM9000_MWCMD;                    //�������ݻ��渳�����ݶ˿�


        while(NULL!= tmp)
        {
            sndlen = PkgGetDataLen(tmp);
//          sndlen = tmp->datalen;
            mysrc = (u16 *)PkgGetCurrentBuffer(tmp);
//          mysrc = (u16 *)(tmp->buf + tmp->offset);
            //��������
            for(i=0;i<sndlen;i+=2)
            {
                DM_DATA_PORT = *mysrc++;  //8λ����ת��Ϊ16λ�������
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
        dm_reg_write(DM9000_TCR, 0x01);                 //�����ݷ��͵���̫����

        while((dm_reg_read(DM9000_NSR) & 0x0c) == 0)
        ;                                               //�ȴ����ݷ������
        dm_reg_write(DM9000_NSR, 0x2c);                 //���TX״̬
        dm_reg_write(DM9000_IMR, 0x81);                 //��DM9000���������ж�

//        timedriverend = DJY_GetSysTime();
        Int_LowAtomEnd(atom);
//        timedriverused = (u32)(timedriverend- timedriverstart);
//        printk("%s:TimeUsed = %d\n\r",__FUNCTION__, timedriverused);
        result = true;
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
struct NetPkg *DM9000_RcvPacket(struct NetDev *netdev)
{
    s32   rx_length,i;
    u8    rx_ready;
    u16 rx_status;
    struct NetPkg *pkg=NULL;

    u16 *dst;
    atom_low_t atom;
    atom = Int_LowAtomStart();

    rx_ready = dm_reg_read(DM9000_MRCMDX);      //�ȶ�ȡһ����Ч������
    rx_ready = (unsigned char)DM_DATA_PORT;     //������ȡ�������ݰ����ֽ�

    if(rx_ready == 1)                           //�ж����ֽ��Ƿ�Ϊ1��0
    {
        DM_ADDR_PORT = DM9000_MRCMD;            //������ȡ���ݰ�����
        rx_status = DM_DATA_PORT;                          //״̬�ֽ�
        rx_length = DM_DATA_PORT;                          //���ݳ���
        rx_status &= 0xff;

        if((rx_length > 0) && (rx_length <= CN_PKG_MAX_LEN))
        {
            pkg =PkgMalloc(rx_length,0);
        }
        if(pkg != NULL)
        {
            dst = (u16 *)PkgGetCurrentBuffer(pkg);
//          dst = (u16 *)(pkg->buf + pkg->offset);
            if(!(rx_status & 0xbf00) && (rx_length < 10000))//�ж������Ƿ����Ҫ��
            {
                for(i=0; i<rx_length; i+=2)          //16λ����ת��Ϊ8λ���ݴ洢
                {
                    *dst++ = DM_DATA_PORT;
                }
            }
            PkgSetDataLen(pkg, rx_length);
//          pkg->datalen = rx_length;
        }
    }
    Int_LowAtomEnd(atom);

    return pkg;
}


static u16 sgDm9000RcvTaskID = CN_EVTT_ID_INVALID;
static struct MutexLCB *pDm9000RcvSync;
// =============================================================================
// ���ܣ�DM9000�жϷ������������ж�ʱ������NetDev_PostDataRcvSignal����ʾ��������
//       ��������Ϣ
// �������ж��ߺ�
// ���� ��
// =============================================================================
u32 DM9000_ISR_Handler(ptu32_t IntLine)
{
    u8 IntStatus;

    dm_reg_write(DM9000_IMR, 0x80);                 //���Ƚ�ֹ�����жϣ�CPU��OS�Ѿ�����

    if(pg_gpio_reg->EINTPEND & (1<<7))              //DM9000�ⲿ�жϱ�־
    {
        pg_gpio_reg->EINTPEND |= (1<<7);            //���ⲿ�жϱ�־

        IntStatus = dm_reg_read(DM9000_ISR);        //��ȡISR
        if(IntStatus & ISR_PRS)                     //�����ж�
        {
            Lock_SempPost(pDm9000RcvSync);
        }
        dm_reg_write(DM9000_ISR, IntStatus);       //���ж�
    }

    dm_reg_write(DM9000_IMR, 0x81);                 //�ָ������жϣ�CPU��OS�Ѿ�����

    return 0;
}


ptu32_t Dm9000Rcv(void)
{
    struct NetPkg *pkg;

    while(1)
    {
        Lock_SempPend(pDm9000RcvSync,CN_TIMEOUT_FOREVER);
        while((pkg = DM9000_RcvPacket(ptNetDev))!= NULL)
        {
            NetDev_PostPkg(ptNetDev,pkg);
            PkgTryFreePart(pkg);
        }
    }
    return 0;
}


bool_t Dm9000RcvTask()
{
    bool_t result = false;
    u16 evttID;
    u16 eventID;
    evttID = DJY_EvttRegist(EN_CORRELATIVE, CN_PRIO_CRITICAL, 0, 1,
        (ptu32_t (*)(void))Dm9000Rcv,NULL, 0x1000, "Dm900RcvTask");
    if (evttID != CN_EVTT_ID_INVALID)
    {
        eventID=DJY_EventPop(evttID, NULL, 0, 0, 0, 0);
        if(eventID != CN_EVENT_ID_INVALID)
        {
            result = true;
            sgDm9000RcvTaskID = evttID;
            pDm9000RcvSync = Lock_SempCreate(1,0,CN_BLOCK_FIFO,NULL);
        }
        else
        {
            DJY_EvttUnregist(evttID);
        }
    }
    return result;
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
bool_t DM9000_AddNetDev(void)
{
    bool_t  result;
    struct NetDevPara  devpara;
    tagHostIpv4Addr devaddr;

    devpara.ifsend = DM9000_Send;
    devpara.iftype = EN_LINK_ETHERNET;
    devpara.devfunc = 0x00;//0xFF;//�������Զ�������
    memcpy(devpara.mac, sgNetHardMac,6);
    devpara.name = "DM9000NetDriver";
    devpara.Private = 0;
    devpara.linklen = 14;
    devpara.pkglen = 1500;

    if(false == Dm9000RcvTask())
    {
        printk("%s:Create Rcv Task Failed\n\r",__FUNCTION__);
    }

    result = false;
    ptNetDev = NetDev_AddDev(&devpara);
    if(ptNetDev != NULL)
    {
//        devaddr.ip = sgNetHardIpAddrMain|CN_HARD_BOARD;
//        devaddr.gateway = sgNetHardGateWay;
//        devaddr.ipmsk = sgNetHardIpMsk;
        devaddr.ip = 0;
        devaddr.gateway = 0;
        devaddr.ipmsk = 0;
        ptNetRout = Rout_AddRout(ptNetDev, &devaddr);
        if(NULL != ptNetRout)
        {
            Dhcp_AddTask(ptNetRout);
            result = true;
        }
    }

    return result;
}

// =============================================================================
// ���ܣ�DM9000������DJYIP������ʼ������
// ������para
// ����ֵ  ��true�ɹ�  falseʧ�ܡ�
// =============================================================================
bool_t DM9000_ModuleInit(ptu32_t para)
{
    bool_t result = false;

    if(true == DM9000_Probe())  //�������ɹ�
    {
        if(true == DM9000_HardInit())
        {
            DM9000_IntInit();
            result = DM9000_AddNetDev();
        }
    }

    return result;
}

