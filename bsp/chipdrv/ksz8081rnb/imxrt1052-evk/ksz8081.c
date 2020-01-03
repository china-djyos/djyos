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

// �ļ���     ��ksz8081.c
// ģ������: PHYоƬKSZ8081RNB��RT1052ƽ̨�µ�����
// ģ��汾: V1.00
// ������Ա: chj
// ����ʱ��: 05/15.2018
// =============================================================================
#include "ksz8081.h"
#include "fsl_phy.h"
#include "fsl_clock.h"
#include "board.h"
#include "board-config.h"
#include "cpu_peri_eth.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    #include "fsl_enet.h"
//    extern bool_t ksz8081_init(tagPhyMessage* phy);
//    tagPhyMessage phy;
//    phy.speed = CFG_KSZ8081_PHY_SPEED;
//    phy.duplex = CFG_KSZ8081_PHY_DUPLEX;
//    if(false == ksz8081_init (&phy))
//    {
//        printf("ksz8081_init failed!\r\n);
//        while(1);
//    }
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"ethernet phy ksz8081"//��̫��phy����
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"ethernet phy" //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_ETHERNET_PHY_KSZ8081 == false )
//#warning  " ethernet_phy_ksz8081  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_ETHERNET_PHY_KSZ8081    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,0,1
#define CFG_KSZ8081_PHY_SPEED           1//"speed",1 = 100M��0 = 10M
#define CFG_KSZ8081_PHY_DUPLEX          1//"duplex",1 = full duplex, 0 = half duplex
//%$#@string,1,10
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure


#define CORE_CLK_FREQ CLOCK_GetFreq(kCLOCK_AhbClk)
#define WTICH_ENET          ENET
#define PHY_CLK             (50*Mhz)
bool_t ksz8081_init(tagPhyMessage* phy)
{
    bool link=false;
    status_t status;
    phy_speed_t speed;
    phy_duplex_t duplex;

    EthOutClkToPhy(WTICH_ENET,PHY_CLK);
    status = PHY_Init(WTICH_ENET, KSZ8081_PHY_ADDR, CORE_CLK_FREQ);
    if (status != kStatus_Success)
    {
        return false;
    }
    PHY_GetLinkStatus(WTICH_ENET, KSZ8081_PHY_ADDR, &link);
    if (link)
    {
        /* Get the actual PHY link speed. */
        PHY_GetLinkSpeedDuplex(WTICH_ENET, KSZ8081_PHY_ADDR, &speed, &duplex);
        /* Change the MII speed and duplex for actual link status. */
        phy->speed = ((enet_mii_speed_t)speed==kENET_MiiSpeed100M?\
                                        CPU_ETH_SPEED_100M:CPU_ETH_SPEED_10M);
        phy->duplex = ((enet_mii_duplex_t)duplex==kENET_MiiFullDuplex?\
                                        CPU_ETH_FULL_DUPLEX:CPU_ETH_HALF_DUPLEX);
        return true;
    }
    return false;
}
