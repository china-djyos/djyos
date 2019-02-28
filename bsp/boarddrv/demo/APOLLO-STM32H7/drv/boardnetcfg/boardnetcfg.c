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

#include <stdint.h>
#include <sys/socket.h>
#include "djyos.h"
#include "lan8720.h"
//#include "pcf8574.h"
#include "cpu_peri.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//  extern void ModuleInstall_InitNet( );
//    ModuleInstall_InitNet( );
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"netconfig"    //��������
//parent:"tcpip"                //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"cpu_peri_eth","lan8720","tcpip"     //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������������������������none����ʾ�������������
                                //����������������������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef CFG_STATIC_IP   //****�������Ƿ��Ѿ����ú�
#warning    ���������á��������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,0,100
//%$#@enum,true,false
#define CFG_STATIC_IP       true            //"IP����",true=ʹ�þ�̬IP��false=��̬IP
//%$#@string,1,16
#define CFG_NETCARD_NAME    "NUCLEO_H743_ETH"    //"������",
//%$#@string,7,15
#define CFG_MY_IPV4         "192.168.0.179" //"��̬IP",
#define CFG_MY_SUBMASK      "255.255.255.0" //"��������",
#define CFG_MY_GATWAY       "192.168.0.1"   //"����",
#define CFG_MY_DNS          "192.168.0.1"   //"DNS",
//%$#@select
//%$#@free
#endif
//%$#@end configue  ****�������ý���


static u8   gc_NetMac[CN_MACADDR_LEN] ={0x00,0x02,0x03,0x04,0x05,0x06};
static lan8720_IOCtx_t  LAN8720_IOCtx = {ETH_PHY_IO_Init,
                                         ETH_PHY_IO_DeInit,
                                         ETH_PHY_IO_WriteReg,
                                         ETH_PHY_IO_ReadReg};

__attribute__((weak)) void GetCpuSignature(void *buf,int len)
{
    //this is for the stm32f7
    vu8   *base;
    int i = 0;
    base = (vu8 *)0x1FF0F420;   //THE ADDRESS OF F7 F4 F1 IS NOT THE SAME
    for(i = 0;i < len;i++)
    {
        *((vu8 *)buf +i)=*(base+i);
    }
    return;
}

// HAL���е����˸ú���
void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOA_CLK_ENABLE();           //����GPIOAʱ��
    __HAL_RCC_GPIOB_CLK_ENABLE();           //����GPIOBʱ��
    __HAL_RCC_GPIOC_CLK_ENABLE();           //����GPIOCʱ��
    __HAL_RCC_GPIOG_CLK_ENABLE();           //����GPIOGʱ��
    __HAL_RCC_ETH1MAC_CLK_ENABLE();         //ʹ��ETH1 MACʱ��
    __HAL_RCC_ETH1TX_CLK_ENABLE();          //ʹ��ETH1����ʱ��
    __HAL_RCC_ETH1RX_CLK_ENABLE();          //ʹ��ETH1����ʱ��

    GPIO_Initure.Pin=GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_7;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //���츴��
    GPIO_Initure.Pull=GPIO_NOPULL;              //����������
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //����
    GPIO_Initure.Alternate=GPIO_AF11_ETH;       //����ΪETH����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);         //��ʼ��

    //PB11
    GPIO_Initure.Pin=GPIO_PIN_11;               //PB11
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);         //ʼ��

    //PC1,4,5
    GPIO_Initure.Pin=GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5; //PC1,4,5
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);         //��ʼ��

    //PG13,14
    GPIO_Initure.Pin=GPIO_PIN_13|GPIO_PIN_14;   //PG13,14
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);         //��ʼ��
}

bool_t PHY_Init(void)
{
    uint32_t duplex, speed = 0;
    int32_t PHYLinkState;
    ETH_MACConfigTypeDef MACConf;
    /* Set PHY IO functions */
//    Djy_DelayUs(500*1000);
//    PCF8574_Init();                         //��ʼ��PCF8574
    LAN8720_RegisterBusIO(&LAN8720_IOCtx);
    Djy_DelayUs(1000);
    PCF8574_WriteBit(ETH_RESET_IO,1);       //Ӳ����λ
    Djy_DelayUs(50*1000);
    PCF8574_WriteBit(ETH_RESET_IO,0);       //��λ����
    Djy_DelayUs(50*1000);
    LAN8720_ResetInit();
    PHYLinkState = LAN8720_GetLinkState();
    switch(PHYLinkState)
    {
        case LAN8720_STATUS_100MBITS_FULLDUPLEX:
            duplex = ETH_FULLDUPLEX_MODE;
            speed = ETH_SPEED_100M;
            break;
        case LAN8720_STATUS_100MBITS_HALFDUPLEX:
            duplex = ETH_HALFDUPLEX_MODE;
            speed = ETH_SPEED_100M;
                break;
        case LAN8720_STATUS_10MBITS_FULLDUPLEX:
            duplex = ETH_FULLDUPLEX_MODE;
            speed = ETH_SPEED_10M;
                break;
        case LAN8720_STATUS_10MBITS_HALFDUPLEX:
            duplex = ETH_HALFDUPLEX_MODE;
            speed = ETH_SPEED_10M;
                break;
        default:
              duplex = ETH_FULLDUPLEX_MODE;
              speed = ETH_SPEED_100M;
              break;
    }
    djybsp_eth_get_mac_config(&MACConf);
    MACConf.DuplexMode = duplex;
    MACConf.Speed = speed;
    djybsp_eth_set_mac_config( &MACConf);
    djybsp_eth_start();

    return true;
}

//please refers the following function in the module-trim in proper place.
//make sure that that os kernel has been initialize ok and the interrupt system intialize ok
//make sure that that board net hardware has initialized and could work ok
void ModuleInstall_InitNet(void)   //static ip example
{
    //GET THE SIGNATURE OF THE DEVICE
    u32  signature[4];
    memset(signature,0,sizeof(signature));
    GetCpuSignature((u8 *)signature,sizeof(signature));
    printk("CPU SIGNATURE:%08X-%08X-%08X-%08X\n\r",signature[0],signature[1],signature[2],signature[3]);
    //use the signature as the mac address
    signature[0] = signature[1]+signature[2]+signature[3];
    memcpy(gc_NetMac,&signature[0],CN_MACADDR_LEN);
    gc_NetMac[0]=0x00;      //����mac�Ĺ涨����һ�ֽ�ĳλ��Ϊ1��ʾ�㲥�����鲥
    //install the net device you used,you could use more than one, but they
    //has different names and macs
    //use the corresponding net device install function you use
//    extern bool_t ModuleInstall_TcpIp(void);
//    ModuleInstall_TcpIp();

    extern bool_t ModuleInstall_ETH(const char *devname, u8 *mac,\
            bool_t loop,u32 loopcycle,\
            bool_t (*rcvHook)(u8 *buf, u16 len));
    ModuleInstall_ETH(CFG_NETCARD_NAME,gc_NetMac,false,1*mS,NULL);
    if(PHY_Init()==false)
    {
        return;
    }
#if CFG_STATIC_IP == 1
    tagHostAddrV4  ipv4addr;
    //we use the static ip we like
    memset((void *)&ipv4addr,0,sizeof(ipv4addr));
    ipv4addr.ip      = inet_addr(CFG_MY_IPV4);
    ipv4addr.submask = inet_addr(CFG_MY_SUBMASK);
    ipv4addr.gatway  = inet_addr(CFG_MY_GATWAY);
    ipv4addr.dns     = inet_addr(CFG_MY_DNS);
    ipv4addr.broad   = inet_addr("192.168.0.255");
    if(RoutCreate(CFG_NETCARD_NAME,EN_IPV_4,(void *)&ipv4addr,CN_ROUT_NONE))
    {
        printk("%s:CreateRout:%s:%s success\r\n",__FUNCTION__,CFG_NETCARD_NAME,inet_ntoa(ipv4addr.ip));
    }
    else
    {
        printk("%s:CreateRout:%s:%s failed\r\n",__FUNCTION__,CFG_NETCARD_NAME,inet_ntoa(ipv4addr.ip));
    }
    //��һ��·�ɣ��������������ã�����CPU ID �������������ַ�������ַ�� 192.168.1
    //WE WILL ADD A ROUT DIFFERENT FOR EACH DEVICE USE THE CPU SIGNATURE
    //USE THE NET:192.168.1.xx
    u8 value8 = 0;
    value8 = +((u8)signature[0]>>0)+((u8)signature[0]>>8)+((u8)signature[0]>>16)+((u8)signature[0]>>24);
    if((value8==0)||(value8==1)||(value8==255))
    {
        value8=253;
    }
    u32 value32 = 0;
    memset((void *)&ipv4addr,0,sizeof(ipv4addr));
    value32 = inet_addr("192.168.1.0");
    value32 = ntohl(value32);
    value32 =(value32&0xffffff00) + value8;
    ipv4addr.ip      = htonl(value32);
    ipv4addr.submask = inet_addr("255.255.255.0");
    ipv4addr.gatway  = inet_addr("192.168.1.1");
    ipv4addr.dns     = inet_addr("192.168.1.1");
    ipv4addr.broad   = inet_addr("192.168.1.255");
    if(RoutCreate(CFG_NETCARD_NAME,EN_IPV_4,(void *)&ipv4addr,CN_ROUT_NONE))
    {
        printk("%s:CreateRout:%s:%s success\r\n",__FUNCTION__,CFG_NETCARD_NAME,inet_ntoa(ipv4addr.ip));
    }
    else
    {
        printk("%s:CreateRout:%s:%s failed\r\n",__FUNCTION__,CFG_NETCARD_NAME,inet_ntoa(ipv4addr.ip));
    }

#else
    if(DhcpAddClientTask(CFG_NETCARD_NAME))
    {
       printk("%s:Add %s success\r\n",__FUNCTION__,CFG_NETCARD_NAME);
    }
    else
    {
        printk("%s:Add %s failed\r\n",__FUNCTION__,CFG_NETCARD_NAME);
    }

#endif
    //do the lan8720 chip set

//    bool_t ETH_RESE(void);
//    ETH_RESE( );//���ڸ�λ
//
//    bool_t lan8720Init(void);
//    lan8720Init( );

    return;
}

