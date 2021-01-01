/*
 * AX88796.c
 *
 *  Created on: 2015��10��27��
 *      Author: zqf
 */

/*
 * AX88796_Module1.c
 *
 *  Created on: 2015��6��4��
 *      Author: zhangqf
 */

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <tcpip/pkg.h>
#include <tcpip/netdev.h>
#include <tcpip/tcpip_cdef.h>

#include "os.h"

#include "Ax88796Byte.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern tagAx88796Pri *Ax88796Install(tagAx88796Para *para);
//    tagAx88796Para Ax88796param;
//    u8 Ax88796Mac[6]           = {0xaa,0xbb,0xcc,0xdd,0xee,0x02};
//    Ax88796param.useirq         = CFG_AX88796_IRQ_USE;
//    Ax88796param.loopcycle      = CFG_AX88796_LOOP_CYCLE;
//    Ax88796param.irqno          = CFG_AX88796_IRQ_NO;
//    Ax88796param.mac            = &Ax88796Mac;
//    Ax88796param.name           = "phy_ax88796";
//    Ax88796param.devbase        = CFG_AX88796_DEV_BASE;
//    Ax88796param.stacksize      = CFG_AX88796_STACK_SIZE;
//    Ax88796param.fnRstDealer    = NULL;       //��Ҫ�������ȥʵ�֣��˴���ʱдΪNULL
//    Ax88796param.fnRcvDealer    = NULL;       //��Ҫ�������ȥʵ�֣��˴���ʱдΪNULL
//    Ax88796Install(&Ax88796param);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"ethernet AX88796"//���ϵ�localbus�ӿڵ�10/100M��̫��оƬ
//parent:"tcpip"     //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"lock","tcpip","heap"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_ETHERNET_AX88796 == false )
//#warning  " ethernet_AX88796  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_ETHERNET_AX88796    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,1000,1000000,
#define CFG_AX88796_LOOP_CYCLE                (1000*1000) //"looptime",����ѭ�����������յ�ʱ��
//%$#@enum,true,false,
#define CFG_AX88796_IRQ_USE                   (1)         //"ʹ���ж�",AX88796�Ƿ�ʹ���ж�
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#define CFG_AX88796_IRQ_NO                               //"����ʹ�õ��жϺ�",
#define CFG_AX88796_DEV_BASE                             //"���߲�����ַ",
#define CFG_AX88796_STACK_SIZE                           //"ջ��С",
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

#define CN_DEV_MONITOR_CYCLE    (5*1000*mS)  //if 5s no pkg comes. reset the dev
#define CN_TSND_TIMEWAIT        1000      //��λ��us,����������ȴ�ʱ��

//****************************************************************************/
//����laogu  http://www.laogu.com
//����Ĺ����Ǹ��ݶಥ��ַmulticast_address[6]��ֵ�������MAR0-MAR7������multicast_filter[8];
//������ָ����һ���ಥ��ַ������ж���ಥ��ַ����ÿ���ಥ��ַ���ɵ�multicast_filter[8]���Ϳ����ˣ�
//������ݶಥ��ַ 01:00:5e:00:00:01���ɵ� value1=multicast_filter[8];
//���ݶಥ��ַ     01:00:5e:00:00:02���ɵ� value2=multicast_filter[8];
//��ô���������ಥ��ַ���ɵ�multicast_filter[8]=value1  |  value2 ;������ֵ���
//�����״�����õ����Ҫ�������жಥ��ַ�����ݰ���MAR0--MAR7��������Ϊ0xff,����˵
//multicast_filter[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

static const u32  CRC_32_Tbl[260]=
{0x00000000,0x077073096,0x0EE0E612C,0x0990951BA,0x0076DC419,0x0706AF48F,0x0E963A535,0x09E6495A3,0x00EDB8832,0x079DCB8A4,\
0x0E0D5E91E,0x097D2D988,0x009B64C2B,0x07EB17CBD,0x0E7B82D07,0x090BF1D91,0x01DB71064,0x06AB020F2,0x0F3B97148,0x084BE41DE,\
0x01ADAD47D,0x06DDDE4EB,0x0F4D4B551,0x083D385C7,0x0136C9856,0x0646BA8C0,0x0FD62F97A,0x08A65C9EC,0x014015C4F,0x063066CD9,\
0x0FA0F3D63,0x08D080DF5,0x03B6E20C8,0x04C69105E,0x0D56041E4,0x0A2677172,0x03C03E4D1,0x04B04D447,0x0D20D85FD,0x0A50AB56B,\
0x035B5A8FA,0x042B2986C,0x0DBBBC9D6,0x0ACBCF940,0x032D86CE3,0x045DF5C75,0x0DCD60DCF,0x0ABD13D59,0x026D930AC,0x051DE003A,\
0x0C8D75180,0x0BFD06116,0x021B4F4B5,0x056B3C423,0x0CFBA9599,0x0B8BDA50F,0x02802B89E,0x05F058808,0x0C60CD9B2,0x0B10BE924,\
0x02F6F7C87,0x058684C11,0x0C1611DAB,0x0B6662D3D,0x076DC4190,0x001DB7106,0x098D220BC,0x0EFD5102A,0x071B18589,0x006B6B51F,\
0x09FBFE4A5,0x0E8B8D433,0x07807C9A2,0x00F00F934,0x09609A88E,0x0E10E9818,0x07F6A0DBB,0x0086D3D2D,0x091646C97,0x0E6635C01,\
0x06B6B51F4,0x01C6C6162,0x0856530D8,0x0F262004E,0x06C0695ED,0x01B01A57B,0x08208F4C1,0x0F50FC457,0x065B0D9C6,0x012B7E950,\
0x08BBEB8EA,0x0FCB9887C,0x062DD1DDF,0x015DA2D49,0x08CD37CF3,0x0FBD44C65,0x04DB26158,0x03AB551CE,0x0A3BC0074,0x0D4BB30E2,\
0x04ADFA541,0x03DD895D7,0x0A4D1C46D,0x0D3D6F4FB,0x04369E96A,0x0346ED9FC,0x0AD678846,0x0DA60B8D0,0x044042D73,0x033031DE5,\
0x0AA0A4C5F,0x0DD0D7CC9,0x05005713C,0x0270241AA,0x0BE0B1010,0x0C90C2086,0x05768B525,0x0206F85B3,0x0B966D409,0x0CE61E49F,\
0x05EDEF90E,0x029D9C998,0x0B0D09822,0x0C7D7A8B4,0x059B33D17,0x02EB40D81,0x0B7BD5C3B,0x0C0BA6CAD,0x0EDB88320,0x09ABFB3B6,\
0x003B6E20C,0x074B1D29A,0x0EAD54739,0x09DD277AF,0x004DB2615,0x073DC1683,0x0E3630B12,0x094643B84,0x00D6D6A3E,0x07A6A5AA8,\
0x0E40ECF0B,0x09309FF9D,0x00A00AE27,0x07D079EB1,0x0F00F9344,0x08708A3D2,0x01E01F268,0x06906C2FE,0x0F762575D,0x0806567CB,\
0x0196C3671,0x06E6B06E7,0x0FED41B76,0x089D32BE0,0x010DA7A5A,0x067DD4ACC,0x0F9B9DF6F,0x08EBEEFF9,0x017B7BE43,0x060B08ED5,\
0x0D6D6A3E8,0x0A1D1937E,0x038D8C2C4,0x04FDFF252,0x0D1BB67F1,0x0A6BC5767,0x03FB506DD,0x048B2364B,0x0D80D2BDA,0x0AF0A1B4C,\
0x036034AF6,0x041047A60,0x0DF60EFC3,0x0A867DF55,0x0316E8EEF,0x04669BE79,0x0CB61B38C,0x0BC66831A,0x0256FD2A0,0x05268E236,\
0x0CC0C7795,0x0BB0B4703,0x0220216B9,0x05505262F,0x0C5BA3BBE,0x0B2BD0B28,0x02BB45A92,0x05CB36A04,0x0C2D7FFA7,0x0B5D0CF31,\
0x02CD99E8B,0x05BDEAE1D,0x09B64C2B0,0x0EC63F226,0x0756AA39C,0x0026D930A,0x09C0906A9,0x0EB0E363F,0x072076785,0x005005713,\
0x095BF4A82,0x0E2B87A14,0x07BB12BAE,0x00CB61B38,0x092D28E9B,0x0E5D5BE0D,0x07CDCEFB7,0x00BDBDF21,0x086D3D2D4,0x0F1D4E242,\
0x068DDB3F8,0x01FDA836E,0x081BE16CD,0x0F6B9265B,0x06FB077E1,0x018B74777,0x088085AE6,0x0FF0F6A70,0x066063BCA,0x011010B5C,\
0x08F659EFF,0x0F862AE69,0x0616BFFD3,0x0166CCF45,0x0A00AE278,0x0D70DD2EE,0x04E048354,0x03903B3C2,0x0A7672661,0x0D06016F7,\
0x04969474D,0x03E6E77DB,0x0AED16A4A,0x0D9D65ADC,0x040DF0B66,0x037D83BF0,0x0A9BCAE53,0x0DEBB9EC5,0x047B2CF7F,0x030B5FFE9,\
0x0BDBDF21C,0x0CABAC28A,0x053B39330,0x024B4A3A6,0x0BAD03605,0x0CDD70693,0x054DE5729,0x023D967BF,0x0B3667A2E,0x0C4614AB8,\
0x05D681B02,0x02A6F2B94,0x0B40BBE37,0x0C30C8EA1,0x05A05DF1B,0x02D02EF8D};

u16 CalcCRC32(u16 *temp)
{
    u32 CRC32=0xFFFFFFFF;
    u16 value;
    u8 DataBuff[6];
    int i;

    DataBuff[0] = temp[0]&0xff;
    DataBuff[1] = temp[1]&0xff;
    DataBuff[2] = temp[2]&0xff;
    DataBuff[3] = temp[3]&0xff;
    DataBuff[4] = temp[4]&0xff;
    DataBuff[5] = temp[5]&0xff;

    value = 0;

    CRC32=0xFFFFFFFF;
    for( i=0; i<6; ++i)
        CRC32 = CRC_32_Tbl[(CRC32^((u8 *)DataBuff)[i]) & 0xff] ^ (CRC32>>8);

    CRC32 = CRC32&0x00003F;

    if(CRC32&0x01)   value = value + 32;
    if(CRC32&0x02)   value = value + 16;
    if(CRC32&0x04)   value = value +  8;
    if(CRC32&0x08)   value = value +  4;
    if(CRC32&0x10)   value = value +  2;
    if(CRC32&0x20)   value = value +  1;

    return value;
}


//****************************************************************************/
// ��ȡ�����Ĵ���״ֵ̬������Ϊ8λ����
//****************************************************************************/
u8 Net_GetNetReg(tagAx88796Pri  *pri,u8 iReg)
{
    u16  *iAddr;
    u8    Result;
    u16   value;

    if(iReg&0x01)//���ַ
    {
        iAddr = (u16*)((ptu32_t)pri->devbase + (iReg&0xfe));
        value =*(u16 *)iAddr;
        Result = (u8)((value&0xFF00)>>8);
    }
    else
    {
        iAddr = (u16*)((u8 *)pri->devbase + iReg);
        value =*(u16 *)iAddr;
        Result = (u8)(value&0xFF);
    }
    return Result;
}


//****************************************************************************/
// д�Ĵ���������Ϊ8λ����
//****************************************************************************/
void Net_SetNetRegA(tagAx88796Pri  *pri,u8 iReg,u8 iValue)
{
    u8  *iAddr;
    iAddr = (u8*)((ptu32_t)pri->devbase + iReg);
    *(iAddr) = iValue;

    return ;
}

//****************************************************************************/
// ���üĴ���ֵ����8λ�Ĵ����Ķ�ȡ��д��״ֵ̬��ͬ
//****************************************************************************/
void SetRegMAC(tagAx88796Pri  *pri,u8 iReg,u8 iValue)
{
    u8  *iAddr;
    iAddr = (u8*)((ptu32_t)pri->devbase + iReg);
    *(iAddr) = iValue;

    return ;
}

//****************************************************************************/
// ��λ����оƬ��ͨ����ȡ��0x1F��ַ�����ɸ�λ����оƬ
//****************************************************************************/
void Net_ChipRST(tagAx88796Pri  *pri)
{
    Net_GetNetReg(pri,0x1f);
    DJY_DelayUs(5000);
}
//****************************************************************************/
// �������ж�
//****************************************************************************/
void Net_ClrIntStatus(tagAx88796Pri  *pri)
{
    Net_SetNetRegA(pri,PORT_INTSTAT,0xFF);     // ���жϱ�־
    return;
}

//****************************************************************************/
// ��ʼ������оƬ��Mac��ַ
//****************************************************************************/
void Net_ChipConfig(tagAx88796Pri  *pri)
{
    u8 data;
    Net_ChipRST(pri);                      // ��λоƬ
    Net_ClrIntStatus(pri);                 // ���жϱ�־

    //�����ж�:couter over,over write,transmit error,receive error, receive fram
    data = IM_CNTE|IM_OVWE|IM_TXEE|IM_RXEE|IM_PRXE;
    Net_SetNetRegA(pri,PORT_INTMASK,data);

    // ת��ҳ��0��ͬʱֹͣ�����������ԼĴ�����������
    Net_SetNetRegA(pri,PORT_CMD,CMD_NODMA|CMD_PAGE0|CMD_STOP); //page0/stop/complete remote DMA

    Net_SetNetRegA(pri,PORT_RBCR0,0x00);    // �������0 clear Remote u8 Count 0
    Net_SetNetRegA(pri,PORT_RBCR1,0x00);    // �������1 clear Remote u8 Count 1

    // ������ʼ��ʱ����Ҫ��ó��Ի�ģ�?
    Net_SetNetRegA(pri,0x0c,0xe0);         // ���տ��ƼĴ���monitor mode,no receive
    Net_SetNetRegA(pri,0x0d,0xe2);         // ���Ϳ��ƼĴ���loop back mode

}

//****************************************************************************/
// ������оƬ�Ĵ����������ã�Դmac��ַ
//****************************************************************************/
void Net_ChipStart(tagAx88796Pri  *pri)
{
    u16 mac[6],value;
    u8 marH,marL;
 //   Net_PhyWriteReg(pri,0x00,0x2300);
//  Net_DelayUs(1000);
    // ���ý��ջ�����ָ��Ĵ�����70��126�����ν���
    // ת��ҳ��0��ͬʱֹͣ�����������ԼĴ�����������
    Net_SetNetRegA(pri,PORT_CMD,CMD_NODMA|CMD_PAGE0|CMD_STOP); //page0/stop/complete remote DMA
    Net_SetNetRegA(pri,0x01,CHIP_PSTART);   // ������ʼҳ�Ĵ��������������ָ��
    Net_SetNetRegA(pri,0x02,CHIP_PSTOP);    // ������ֹҳ�Ĵ�����������ָֹ��
    Net_SetNetRegA(pri,0x03,CHIP_PSTART);   // ���ձ߽�ָ��Ĵ���������Ŀǰ����ָ��λ��
    Net_SetNetRegA(pri,0x04,CHIP_TSTART);   // ������ʼҳ��Ĵ��������巢����ʼָ��λ��
    Net_SetNetRegA(pri,0x05,0x00);          // BNRY
    Net_SetNetRegA(pri,0x06,0x00);          // TPSR
    // �������߹�����ʽ16λ���ر�loop backģʽ����ʼ��ָ�����
    Net_SetNetRegA(pri,0x0e, DCON_BSIZE1 | DCON_BUS_8 | DCON_LOOPBK_OFF);//Set dma 16 bite mode
    // ��ʼ��MAC��ַ����ҳ��1�Ĵ�����
    Net_SetNetRegA(pri,PORT_CMD,CMD_NODMA|CMD_PAGE1|CMD_STOP);//page1/stop/complete remote DMA
    Net_SetNetRegA(pri,0x01,pri->mac[0]);
    Net_SetNetRegA(pri,0x02,pri->mac[1]);
    Net_SetNetRegA(pri,0x03,pri->mac[2]);
    Net_SetNetRegA(pri,0x04,pri->mac[3]);
    Net_SetNetRegA(pri,0x05,pri->mac[4]);
    Net_SetNetRegA(pri,0x06,pri->mac[5]);
    // ��ʼ����ǰ��ʼָ�룬ָ����ջ���������һҳ
    Net_SetNetRegA(pri,0x07,CHIP_PSTART+1);//CURR;
// ��ʼ��MAR�Ĵ������鲥��ַ���˼Ĵ���
    Net_SetNetRegA(pri,0x08,0x00);//Mar0
    Net_SetNetRegA(pri,0x09,0x00);//Mar1
    Net_SetNetRegA(pri,0x0A,0x00);//Mar2
    Net_SetNetRegA(pri,0x0B,0x00);//Mar3
    Net_SetNetRegA(pri,0x0c,0x00);//Mar4
    Net_SetNetRegA(pri,0x0d,0x00);//Mar5
    Net_SetNetRegA(pri,0x0e,0x00);//Mar6
    Net_SetNetRegA(pri,0x0f,0x00);//Mar7
//����������MAC��������
    mac[0]=pri->mac[0];
    mac[1]=pri->mac[1];
    mac[2]=pri->mac[2];
    mac[3]=pri->mac[3];
    mac[4]=pri->mac[4];
    mac[5]=pri->mac[5];
    value = CalcCRC32((u16*)mac);
    marH = value / 8;
    marL = value % 8;
    marH = marH + 8;
    marL = (0x01 << marL);
    SetRegMAC(pri,marH,marL);       //����Ŀ���ַΪ��������MAC�鲥����
    //SetEhterNetRecMACReg(pri);        //������̫���ڽ���MAC���μĴ���,
    mac[0]=0x01;
    mac[1]=0x79;
    mac[2]=0x77;
    mac[3]=0x70;
    mac[4]=0xff;
    mac[5]=0xff;
    value = CalcCRC32((u16*)mac);
    marH = value / 8;
    marL = value % 8;
    marH = marH + 8;
    marL = (0x01 << marL);
    SetRegMAC(pri,marH,marL);       //����Ŀ���ַΪ��������MAC�鲥����
    // ת��ҳ��0��׼������ҳ��0�Ĵ���
    Net_SetNetRegA(pri,PORT_CMD,CMD_NODMA|CMD_PAGE0|CMD_STOP);  //page0/stop/complete remote DMA
    // ���ý��պͷ��Ϳ��ƼĴ���
    Net_SetNetRegA(pri,0x0c,0x0C);// ֻ�����鲥����           xuzian  2006-11-29 10:43����
    Net_SetNetRegA(pri,0x0d,0x00);// ��������
    Net_SetNetRegA(pri,PORT_CMD, CMD_NODMA | CMD_PAGE0 | CMD_START);  //start work
    Net_SetNetRegA(pri,0x07,0xFF); // ���жϱ�־
}

//****************************************************************************/
// �����ջ��������ݱ������ڴ�ȫ�ֱ�����
//****************************************************************************/
void Net_NICToPC(tagAx88796Pri *pri,u16 bnry,u16 length,u8 *Buffer,u16 flag)
{
    u16 i;
    u8* ioBufPort;
    u8 *tempbuf;

    Net_SetNetRegA(pri,PORT_CMD, CMD_NODMA | CMD_PAGE0 | CMD_START);
    Net_SetNetRegA(pri,0x09,bnry&0xFF);                        // �ý��ձ߽�ָ��
    Net_SetNetRegA(pri,0x08,0x00);
    Net_SetNetRegA(pri,0x0B,(length >>8) & 0xff);         // ������ݰ����?
    Net_SetNetRegA(pri,0x0a,length & 0xff);
    Net_SetNetRegA(pri,PORT_CMD, CMD_RREAD | CMD_START);   // �������������ȡ����״̬
    ioBufPort=(u8*)(pri->devbase + 0x10);
    tempbuf = Buffer;
    for(i=0;i<length;i++)
    {
        *tempbuf++=*(ioBufPort);  // ��ȡ��������������
    }

    Net_SetNetRegA(pri,PORT_CMD, CMD_NODMA | CMD_PAGE0 | CMD_START);
}

//****************************************************************************/
// �������ݽ��պ���
//****************************************************************************/
u16 Net_RecvPacket(tagAx88796Pri *pri,u8* buf,u16 buflen)
{
    register        u16 bnry,curr;
    register        u16 length;
    tagAx88796FH    RecFram,*ptRecFrame;

    if(pri->devflag & (CN_DEV_RERR|CN_DEV_ROVR))            // �������ֹͣ��������
        return 0;
    ptRecFrame = &RecFram;
    Net_SetNetRegA(pri,PORT_CMD,CMD_NODMA | CMD_PAGE0 | CMD_START);
    bnry=Net_GetNetReg(pri,0x03);    // ��ȡ���ջ������߽�ָ�룬ÿ����һ�����ģ�ָ���һ
    Net_SetNetRegA(pri,PORT_CMD, CMD_NODMA | CMD_PAGE1 | CMD_START);
    curr=Net_GetNetReg(pri,0x07);    // ��ȡ���ջ�������ǰָ�룬ÿ����һ�����ģ�ָ���һ
    Net_SetNetRegA(pri,PORT_CMD, CMD_NODMA | CMD_PAGE0 | CMD_START);
    if(curr==0)
        return 0;
    bnry++;
    if(bnry>=CHIP_PSTOP) bnry=CHIP_PSTART;//reset from pstop=pstart,form circuity structure

    if(bnry==curr)
    {
        return 0;   //NO New INPUT
    }
    Net_NICToPC(pri,bnry,4,(u8*)(ptRecFrame),0);  //Read Head INTO Memory

    if(((ptRecFrame->rstat & 0x01)==0)||(ptRecFrame->next>=CHIP_PSTOP)
        ||(ptRecFrame->next<CHIP_PSTART)||(ptRecFrame->uppByteCnt>0x06))//ERROR Message in Head 0x0600=1536 �֡Ϊ1522
    {
        Net_SetNetRegA(pri,PORT_CMD, CMD_NODMA | CMD_PAGE1 | CMD_STOP);   // �л���page1
        /*�����������ܰ��������ڲ����λ�������дָ��Խ�磬�������Ҫ���³�ʼ��дָ�룬�������Ĵ��뽫дָ�븳ֵ����ָ�룬�ᵼ�¶�ָ��ҲԽ��*/
        Net_SetNetRegA(pri,0x07,CHIP_PSTART+1);
        curr=Net_GetNetReg(pri,0x07);                                 // ��ȡ���ջ�������ǰָ��
        Net_SetNetRegA(pri,PORT_CMD, CMD_NODMA | CMD_PAGE0 | CMD_STOP);//�л���page0
        /*�����ڲ�DMA��������0*/
        Net_SetNetRegA(pri,0x0B,0);
        Net_SetNetRegA(pri,0x0A,0);
        bnry=curr-1;
        if(bnry<CHIP_PSTART)bnry=CHIP_PSTOP-1;
        Net_SetNetRegA(pri,0x03,bnry);                 //���ý��ջ������߽�ָ��
        Net_SetNetRegA(pri,PORT_CMD, CMD_NODMA | CMD_PAGE0 | CMD_START);//�л���page0
        return 0;
    }
    length=(ptRecFrame->uppByteCnt<<8)+ptRecFrame->lowByteCnt;

    if(length <= buflen)
    {
        Net_NICToPC(pri,bnry,length,buf,1);
    }
    else
    {
        length =0;
    }
    bnry=ptRecFrame->next-1;
    if(bnry<CHIP_PSTART)bnry=CHIP_PSTOP-1;
        Net_SetNetRegA(pri,0x03,bnry);                 //���ý��ջ������߽�ָ��
    return length;
}

//****************************************************************************/
// �����������д������
//****************************************************************************/
bool_t Net_PCToNIC(tagAx88796Pri *pri,u16 length,u8 *Buffer)
{

    bool_t      result = false;
    register    u16 i;
    register    u8 *addr;
    u8          cmdStat;
    u8          tsr;
    u32         timeout =0;

    Net_SetNetRegA(pri,PORT_CMD, CMD_NODMA | CMD_PAGE0 | CMD_START);
    cmdStat=Net_GetNetReg(pri,PORT_CMD);

    if(cmdStat & CMD_TXP)
        return result;

    Net_SetNetRegA(pri,0x08,00);
    Net_SetNetRegA(pri,0x09,CHIP_TSTART);                // �÷�����ʼָ��
    Net_SetNetRegA(pri,0x0a,length & 0xff);
    Net_SetNetRegA(pri,0x0b,(length >>8) & 0xff);        // ���÷������ݰ�����

    Net_SetNetRegA(pri,PORT_CMD, CMD_RWRITE | CMD_START); // ��������д����״̬

    addr = (u8 *)(pri->devbase+0x10);
    for(i=0;i<length;i++)
    {
        *addr  = *Buffer++;
    }
    // there are many diffrent codes about this part.
    Net_SetNetRegA(pri,PORT_TSTART, CHIP_TSTART);
    Net_SetNetRegA(pri,PORT_TCNTH,(length >> 8)& 0xff);
    Net_SetNetRegA(pri,PORT_TCNTL, length & 0xff);
    Net_SetNetRegA(pri,PORT_CMD, CMD_TXP | CMD_START);
    Net_SetNetRegA(pri,PORT_CMD, CMD_NODMA | CMD_PAGE0 | CMD_START);

    //�ȴ��������
    while(timeout < CN_TSND_TIMEWAIT)
    {
        tsr = Net_GetNetReg(pri,PORT_TSTAT);
        if(tsr&(TSTAT_OWC|TSTAT_ABT|TSTAT_COL))
        {
            result = true;
            pri->devflag |= CN_DEV_TERR;
            break;
        }
        else if(tsr |TSTAT_PTX)
        {
            result = true;
            break;
        }
        else
        {
            if(timeout == (CN_TSND_TIMEWAIT-1))
            {
                result = false;
                pri->devflag |= CN_DEV_TERR;
                printf("%s:TimeOut:Len:%d\n\r",__FUNCTION__,length);
                break;
            }
            else
            {
                timeout++;
                DJY_DelayUs(1); //ÿ����ʱ1us
            }

        }
    }

    return result;
}
//****************************************************************************/
// �������ݰ�����
//****************************************************************************/
static bool_t __Ax88796BufSnd(tagAx88796Pri *pri,u8* buf,u16 iLength)
{
    bool_t  result = false;
    if(Lock_MutexPend(pri->devsync,CN_TIMEOUT_FOREVER))
    {
        if((pri->devflag &(CN_DEV_TERR|CN_DEV_TOVR))||\
            (iLength > CN_FRAME_MAX)||(iLength < CN_FRAME_MIN)||(NULL == buf))
        {
            result = false;
        }
        else
        {
            result = Net_PCToNIC(pri,iLength,(u8*)buf);
        }

        Lock_MutexPost(pri->devsync);
    }

    return result;
}
//����88796�ĸ�λ
static bool_t __Ax88796Rst(tagAx88796Pri  *pri)
{
    atom_low_t  atom;

    atom = Int_LowAtomStart();
    //��Ӳ����λ
    if(NULL != pri->fnRstDealer)
    {
        pri->fnRstDealer((void *)pri);
    }
    //�������λ
    Net_ChipRST(pri);
    //����Ӳ������
    Net_ChipConfig(pri);
    Net_ChipStart(pri);
    //״̬���ĸ�λ
    pri->devflag =CN_DEV_OK;

    Int_LowAtomEnd(atom);

    return true;
}
// =============================================================================
// �������ܣ��������ݰ�
// ���������netdev,ʹ�õ�����
//        pkglst,�����͵����ݰ�����
//        netdevtask,������Ҫ��ɵĹ���
// ���������
// ����ֵ  ��true���ͳɹ�  false����ʧ�ܡ�
// ˵��    �����ÿ����ķ�ʽ���ͣ���������ʹ��������
// =============================================================================
bool_t Ax88796Snd(struct NetDev *dev,struct NetPkg *pkglst,u32 netdevtask)
{
    bool_t  result;
    struct NetPkg *tmp;
    u8 *src;
    u8 *dst;
    u8 *buf;
    u16 sndlen;
    tagAx88796Pri *pri;

    result = false;

    if((NULL == pkglst)||(netdevtask != CN_IPDEV_NONE)||(NULL ==dev))
    {
        printk("%s:Err Send Para!\n\r");
        return false;
    }

    pri = (tagAx88796Pri *)(NetDev_GetDevPrivate(dev));
    buf = pri->sndbuf;
    if(Lock_MutexPend(pri->devsync,CN_TIMEOUT_FOREVER))
    {
        dst = buf;
        //move all the package to the send buf
        tmp = pkglst;
        sndlen = PkgFrameDataCopy(tmp,dst);
//      sndlen = 0;
//      while(NULL != tmp)
//      {
//          src = (u8 *)(tmp->buf + tmp->offset);
//          memcpy((void *)dst,(void *)src,tmp->datalen);
//          sndlen +=tmp->datalen;
//          dst+=tmp->datalen;
//          if(PkgIsBufferEnd(tmp))
//          {
//              tmp = NULL;
//          }
//          else
//          {
//              tmp = PkgGetNextUnit(tmp);
//          }
//      }
        //ok, call the api to send the data
        result = Net_PCToNIC(pri,sndlen,buf);
        Lock_MutexPost(pri->devsync);
        result = true;

    }
    return result;
}
// =============================================================================
// ���ܣ��������������ݵ��豸�Ļ�����
// ������
// ���� ��
// =============================================================================
static u16  __DevRcvFunc(tagAx88796Pri  *pri)
{
    u16         len;

    if(Lock_MutexPend(pri->devsync,CN_TIMEOUT_FOREVER))
    {
        len = Net_RecvPacket(pri,pri->rcvbuf,CN_BUF_LEN);
        Lock_MutexPost(pri->devsync);
    }
    return len;
}

static ptu32_t __DevRcvMain(void)
{
    u16             len;
    rcvDealer       fnHook;
    u8              *src;
    u8              *dst;
    struct NetPkg       *pkg;
    tagAx88796Pri   *pri = NULL;
    DJY_GetEventPara((ptu32_t *)&pri,NULL);
    if(NULL != pri)
    {
        fnHook = pri->fnRcvDealer;
        while(1)
        {
            if(Lock_SempPend(pri->rcvsync,pri->loopcycle)) //һ���źŵƿ����ж������
            {
                while((len = __DevRcvFunc(pri))>0)
                {
                    if(NULL != fnHook) //���ݲ���Э��ջ����
                    {
                        fnHook(pri,pri->rcvbuf,len);
                    }
                    else  //��������Э��ջ
                    {
                        pkg = PkgMalloc(len,CN_PKLGLST_END);
                        if(NULL != pkg)
                        {
                            dst = PkgGetCurrentBuffer(pkg);
//                          dst = (u8 *)(pkg->offset+ pkg->buf);
                            src = (u8 *)pri->rcvbuf;
                            src += 4;  //ǰ�����ֽ�Ϊ��ͷ��Ϣ
                            memcpy((void *)dst,(void *)src, len);
                            PkgSetDataLen(pkg, len);
//                          pkg->datalen = len;
                            NetDev_PostPkg(pri->devhandle,pkg);
                            PkgTryFreePart(pkg);
                        }
                    }
                    pri->monitorRcv++;
                }
            }
        }
    }
    return 0;
}

//�����Ľ��ռ�������һ��ʱ����û�����ݰ����գ���λ����
static ptu32_t __DevRcvMonitor(void)
{
    tagAx88796Pri  *pri = NULL;
    u32             chippacks;

    DJY_GetEventPara((ptu32_t *)&pri,NULL);
    if(NULL == pri)
    {
        goto MONITOR_EXIT;
    }
    while(1)
    {
        chippacks = pri->monitorRcv;
        DJY_EventDelay(CN_DEV_MONITOR_CYCLE);
        if(Lock_MutexPend(pri->devsync,CN_TIMEOUT_FOREVER))
        {
            if(chippacks == pri->monitorRcv) //no pack rcved any more, reset the dev
            {
                __Ax88796Rst(pri);
            }
            Lock_MutexPost(pri->devsync);
        }
    }
MONITOR_EXIT:
    printk("%s:%s MonitorErr!\n\r",__FUNCTION__,pri->name);
    return 0;
}

static u16 __taskCreate(char *name,u32 prior,void *routine,u32 stacksize,ptu32_t para)
{

    u16 evttID;
    u16 eventID = CN_EVENT_ID_INVALID;
    evttID = DJY_EvttRegist(EN_CORRELATIVE, prior, 0, 1,\
                            (ptu32_t (*)(void))routine,NULL, stacksize, name);

    if (evttID != CN_EVTT_ID_INVALID)
    {
        eventID=DJY_EventPop(evttID, NULL, 0, (ptu32_t)para, 0, 0);
        if(eventID == CN_EVENT_ID_INVALID)
        {
            DJY_EvttUnregist(evttID);
        }
    }
    return eventID;
}
//�жϵĴ���̫��ª,Ӧ�ô�������ȵ��ж�--TODO
static u32 __Ax88796Isr(ptu32_t data)
{
    u8 isr;
    tagAx88796Pri *pri;

    pri = (tagAx88796Pri *)data;
    isr = Net_GetNetReg(pri,PORT_INTSTAT);
    Net_ClrIntStatus(pri);  //�����ж�

    if(isr & ISTAT_CNT)
    {
        pri->cnt++;
        __Ax88796Rst(pri);
    }
    if(isr& ISTAT_OVW)
    {
        pri->ovw++;
        __Ax88796Rst(pri);
    }
    if(isr&ISTAT_TXE)
    {
        pri->txe++;
        __Ax88796Rst(pri);
    }
    if(isr &ISTAT_RXE)
    {
        pri->rxe++;
        __Ax88796Rst(pri);
    }
    if(isr &ISTAT_PTX)
    {
        //�����ж�
    }
    if(isr&ISTAT_PRX)  //���յ�����
    {
        Lock_SempPost(pri->rcvsync);
    }

    return 0;
}

//��װ�жϴ���
static void __Ax88796IntInstall(tagAx88796Pri  *pri)
{
    ufast_t  irqno;
    irqno = pri->irqno;
    //apply the interrupt and bind it to the isr
    if(true == Int_Register(irqno))
    {
        Int_SetClearType(irqno,CN_INT_CLEAR_AUTO);
        Int_IsrConnect(irqno,__Ax88796Isr);
        Int_SettoAsynSignal(irqno);
        Int_ClearLine(irqno);
        Int_RestoreAsynLine(irqno);
        Int_SetIsrPara(irqno,(ptu32_t)pri);
    }
}

//��ʼ����������������ӽ�Э��ջ��
tagAx88796Pri *Ax88796Install(tagAx88796Para *para)
{
    struct NetDevPara   devpara;
    tagAx88796Pri  *ax88796 = NULL;

    ax88796 = (tagAx88796Pri *)malloc(sizeof(tagAx88796Pri));
    if(NULL == ax88796)
    {
        goto INSTALL_END;
    }
    memset((void *)ax88796,0,sizeof(tagAx88796Pri));

    //�������õĳ�ʼ��
    ax88796->devbase = para->devbase;
    ax88796->stacksize = para->stacksize;
    ax88796->fnRstDealer = para->fnRstDealer;
    ax88796->fnRcvDealer = para->fnRcvDealer;
    ax88796->fnSndDealer = (sndDealer)__Ax88796BufSnd;
    memcpy((void *)ax88796->mac,(void *)para->mac,CN_MACADDR_LEN);
    memcpy((void *)ax88796->name,(void *)para->name,CN_NAME_LEN);
    //����һЩ��������
    ax88796->devsync = Lock_MutexCreate(NULL);
    if(NULL == ax88796->devsync)
    {
        goto DEVSYNC_FAILED;
    }
    ax88796->rcvsync = Lock_SempCreate(1,0,CN_BLOCK_FIFO,NULL);
    if(NULL == ax88796->rcvsync)
    {
        goto RCVSYNC_FAILED;
    }
    ax88796->taskRcvID = __taskCreate(ax88796->name,CN_PRIO_REAL+1,(void *)__DevRcvMain,\
            ax88796->stacksize, (ptu32_t)ax88796);
    ax88796->taskMonitorID = __taskCreate(NULL,CN_PRIO_RRS+1,(void *)__DevRcvMonitor,\
            ax88796->stacksize, (ptu32_t)ax88796);

    if((CN_EVENT_ID_INVALID == ax88796->taskMonitorID)||\
            (CN_EVENT_ID_INVALID == ax88796->taskRcvID))
    {
        goto TASK_FAILED;
    }
    //INSTALL THE IRQ
    if(para->useirq)
    {
        ax88796->irqno = para->irqno;
        ax88796->loopcycle = CN_TIMEOUT_FOREVER;
        __Ax88796IntInstall(ax88796);
    }
    else
    {
        ax88796->loopcycle = para->loopcycle;
    }

    //��ʼ�������豸���ݣ���ô���¸�λ������������
    __Ax88796Rst(ax88796);

    //��Э��ջע��������
    devpara.ifsend = Ax88796Snd;
    devpara.iftype = EN_LINK_ETHERNET;
    devpara.devfunc = CN_IPDEV_NONE;    //NO FUNC FOR THE DEV
    memcpy(devpara.mac, ax88796->mac,CN_MACADDR_LEN);
    devpara.name = ax88796->name;
    devpara.Private = (ptu32_t)ax88796;
    devpara.linklen = 14;
    devpara.pkglen = 1500;
    ax88796->devhandle = (void *) NetDev_Install(&devpara);
    if(NULL == ax88796->devhandle)
    {
        goto DEV_FAILED;
    }
    else
    {
        goto INSTALL_END;
    }
    //��װ���

DEV_FAILED:
    //DEL THE TASK HERE
TASK_FAILED:
    Lock_SempDelete(ax88796->rcvsync);
RCVSYNC_FAILED:
    Lock_MutexDelete(ax88796->devsync);
DEVSYNC_FAILED:
    free((void *)ax88796);
    ax88796 = NULL;
INSTALL_END:
    if(NULL == ax88796)
    {
        printf("%s:Install:%s Install Err\n\r",__FUNCTION__,para->name);
    }
    else
    {
        printf("%s:Install:%s Install OK\n\r",__FUNCTION__,para->name);
    }

    return ax88796;
}








