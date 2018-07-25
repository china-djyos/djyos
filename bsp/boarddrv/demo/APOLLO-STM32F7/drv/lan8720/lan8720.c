
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "cpu_peri.h"
#include "stm32f7xx_hal_conf.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//  bool_t lan8720Init(void);
//  lan8720Init();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"lan8720"      /RMII�ӿڵ�10/100M PHY
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp���             //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                   //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//grade:init                    //��ʼ��ʱ������ѡֵ��none��init��main��none��ʾ�����ʼ����
                                //init��ʾ�ڵ���main֮ǰ��main��ʾ��main�����г�ʼ��
//dependence:"cpu_peri_eth"    //������������������������none����ʾ�������������
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
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
//#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

#define LAN8720_PHY_ADDRESS             0x00

extern u8 GMAC_MdioR(u8 dev,u8 reg, u16 *value);
extern u8 GMAC_MdioW(u8 dev,u8 reg, u16 value);
extern uint32_t HAL_GetTick(void);
bool_t lan8720Init(void)
{
    u16   phyreg = 0;
    uint32_t tickstart = 0;
    uint32_t tickNow ;
    /*-------------------- PHY initialization and configuration ----------------*/
    /* Put the PHY in reset mode */
    if((GMAC_MdioW(LAN8720_PHY_ADDRESS, PHY_BCR, PHY_RESET)) != 1)
    {
      /* Return false */
      return false;
    }
     Djy_EventDelay(PHY_RESET_DELAY*mS);
     tickstart=HAL_GetTick();
      do
      {
          GMAC_MdioR(LAN8720_PHY_ADDRESS, PHY_BSR, &phyreg);
          tickNow=HAL_GetTick();
          if((tickNow - tickstart ) > 1000)
              return false;

      } while (((phyreg & PHY_LINKED_STATUS) != PHY_LINKED_STATUS));
      /* Enable Auto-Negotiation */
      if((GMAC_MdioW(LAN8720_PHY_ADDRESS, PHY_BCR, PHY_AUTONEGOTIATION)) != 1)
      {
        return false;
      }
      /* Get tick */
      tickstart = HAL_GetTick();
      /* Wait until the auto-negotiation will be completed */
      do
      {
          GMAC_MdioR(LAN8720_PHY_ADDRESS, PHY_BSR, &phyreg);
          tickNow=HAL_GetTick();
          if((tickNow - tickstart ) > 10000)
              return false;
      } while (((phyreg & PHY_AUTONEGO_COMPLETE) != PHY_AUTONEGO_COMPLETE));

      /* Read the result of the auto-negotiation */
      if((GMAC_MdioR(LAN8720_PHY_ADDRESS, PHY_SR, &phyreg)) != 1)
      {
        return false;
      }
    return true;
}

