
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "cpu_peri.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//  bool_t lan8720Init(void);
//  lan8720Init();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"lan8720"      //RMII�ӿڵ�10/100M PHY
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"cpu_peri_eth"     //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������������������������none����ʾ�������������
                                //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
//#ifndef CFG_VLAN_VALUE0         //****�������Ƿ��Ѿ����ú�
//#warning    lan8720�������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,0,255,
#define CFG_LAN8720_PHY_ADDRESS     0   //phy�������ַ��Ӳ��һ�����Ϊ 0
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
//#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

#define CN_RESET_DELAY      255000      //uS
static bool_t sg_NeedMacDriver = false;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
__attribute__((weak)) u8 GMAC_MdioR(u8 dev, u8 reg, u16 *value)
{
    sg_NeedMacDriver = true;
    return 0;
}

__attribute__((weak)) u8 GMAC_MdioW(u8 dev,u8 reg, u16 value)
{
    sg_NeedMacDriver = true;
    return 0;
}

#pragma GCC diagnostic pop

extern uint32_t HAL_GetTick(void);
bool_t lan8720Init(void)
{
    u16   phyreg = 0;
    u32 delay = 0;
//  uint32_t tickstart = 0;
//  uint32_t tickNow ;
    /*-------------------- PHY initialization and configuration ----------------*/
    /* Put the PHY in reset mode */
    if((GMAC_MdioW(CFG_LAN8720_PHY_ADDRESS, PHY_BCR, PHY_RESET)) != 1)
    {
        if(sg_NeedMacDriver == true)
            printf("\r\n LAN8720������Ҫmac����");
        return false;
    }
     Djy_EventDelay(CN_RESET_DELAY);
//   tickstart=HAL_GetTick();
    do
    {
        GMAC_MdioR(CFG_LAN8720_PHY_ADDRESS, PHY_BSR, &phyreg);
        if(delay > 1000)
            return false;
        else
        {
            Djy_DelayUs(1000);
            delay++;
        }
//      tickNow=HAL_GetTick();
//      if((tickNow - tickstart ) > 1000)
//          return false;

    } while (((phyreg & PHY_LINKED_STATUS) != PHY_LINKED_STATUS));
    /* Enable Auto-Negotiation */
    if((GMAC_MdioW(CFG_LAN8720_PHY_ADDRESS, PHY_BCR, PHY_AUTONEGOTIATION)) != 1)
    {
      return false;
    }
    /* Get tick */
//  tickstart = HAL_GetTick();
    delay = 0;
    /* Wait until the auto-negotiation will be completed */
    do
    {
        GMAC_MdioR(CFG_LAN8720_PHY_ADDRESS, PHY_BSR, &phyreg);
        if(delay > 1000)
            return false;
        else
        {
            Djy_DelayUs(1000);
            delay++;
        }
//      tickNow=HAL_GetTick();
//      if((tickNow - tickstart ) > 10000)
//          return false;
    } while (((phyreg & PHY_AUTONEGO_COMPLETE) != PHY_AUTONEGO_COMPLETE));

    /* Read the result of the auto-negotiation */
    if((GMAC_MdioR(CFG_LAN8720_PHY_ADDRESS, PHY_SR, &phyreg)) != 1)
    {
        return false;
    }
    return true;
}

