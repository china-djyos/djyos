
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "cpu_peri.h"
#include "lan8720.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//  extern bool_t LAN8720_ResetInit(void);
//  LAN8720_RESET( );
//  LAN8720_ResetInit( );
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"ethernet phy lan8720"//RMII�ӿڵ�10/100M PHY
//parent:"tcpip"     //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"tcpip" //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_ETHERNET_PHY_LAN8720 == false )
//#warning  " ethernet_phy_lan8720  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_ETHERNET_PHY_LAN8720    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,255,
#define CFG_LAN8720_PHY_ADDRESS     0   //phy�������ַ��Ӳ��һ�����Ϊ 0
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
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

//extern uint32_t HAL_GetTick(void);
//bool_t lan8720Init(void)
//{
//    u16   phyreg = 0;
//    u32 delay = 0;
////  uint32_t tickstart = 0;
////  uint32_t tickNow ;
//    /*-------------------- PHY initialization and configuration ----------------*/
//    /* Put the PHY in reset mode */
//    if((GMAC_MdioW(CFG_LAN8720_PHY_ADDRESS, PHY_BCR, PHY_RESET)) != 1)
//    {
//        if(sg_NeedMacDriver == true)
//            printf("\r\n LAN8720������Ҫmac����");
//        return false;
//    }
//     DJY_EventDelay(CN_RESET_DELAY);
////   tickstart=HAL_GetTick();
//    do
//    {
//        GMAC_MdioR(CFG_LAN8720_PHY_ADDRESS, PHY_BSR, &phyreg);
//        if(delay > 1000)
//            return false;
//        else
//        {
//            DJY_DelayUs(1000);
//            delay++;
//        }
////      tickNow=HAL_GetTick();
////      if((tickNow - tickstart ) > 1000)
////          return false;
//
//    } while (((phyreg & PHY_LINKED_STATUS) != PHY_LINKED_STATUS));
//    /* Enable Auto-Negotiation */
//    if((GMAC_MdioW(CFG_LAN8720_PHY_ADDRESS, PHY_BCR, PHY_AUTONEGOTIATION)) != 1)
//    {
//      return false;
//    }
//    /* Get tick */
////  tickstart = HAL_GetTick();
//    delay = 0;
//    /* Wait until the auto-negotiation will be completed */
//    do
//    {
//        GMAC_MdioR(CFG_LAN8720_PHY_ADDRESS, PHY_BSR, &phyreg);
//        if(delay > 1000)
//            return false;
//        else
//        {
//            DJY_DelayUs(1000);
//            delay++;
//        }
////      tickNow=HAL_GetTick();
////      if((tickNow - tickstart ) > 10000)
////          return false;
//    } while (((phyreg & PHY_AUTONEGO_COMPLETE) != PHY_AUTONEGO_COMPLETE));
//
//    /* Read the result of the auto-negotiation */
//    if((GMAC_MdioR(CFG_LAN8720_PHY_ADDRESS, PHY_SR, &phyreg)) != 1)
//    {
//        return false;
//    }
//    return true;
//}

static lan8720_Object_t LAN8720_Obj;

/**
  * @brief  Register IO functions to component object
  * @param  pObj: device object  of LAN8742_Object_t.
  * @param  ioctx: holds device IO functions.
  * @retval LAN8742_STATUS_OK  if OK
  *         LAN8742_STATUS_ERROR if missing mandatory function
  */
int32_t  LAN8720_RegisterBusIO(lan8720_IOCtx_t *ioctx)
{
  if(!ioctx->ReadReg || !ioctx->WriteReg )
  {
    return -1;
  }

  LAN8720_Obj.IO.Init = ioctx->Init;
  LAN8720_Obj.IO.DeInit = ioctx->DeInit;
  LAN8720_Obj.IO.ReadReg = ioctx->ReadReg;
  LAN8720_Obj.IO.WriteReg = ioctx->WriteReg;

  return 0;
}

//��ȡPHY�Ĵ���ֵ
//regҪ��ȡ�ļĴ�����ַ
//����ֵ:0 ��ȡ�ɹ���-1 ��ȡʧ��
int32_t LAN8720_ReadPHY(u16 reg,u32 *regval)
{
    if(LAN8720_Obj.IO.ReadReg==NULL)
        return -1;
    if(LAN8720_Obj.IO.ReadReg(LAN8720_ADDR,reg,regval)!=0)
        return -1;
    return 0;
}

//��LAN8720ָ���Ĵ���д��ֵ
//reg:Ҫд��ļĴ���
//value:Ҫд���ֵ
//����ֵ:0 д��������-1 д��ʧ��
int32_t LAN8720_WritePHY(u16 reg,u16 value)
{
    u32 temp=value;
    if(LAN8720_Obj.IO.WriteReg==NULL)
        return -1;
    if(LAN8720_Obj.IO.WriteReg(LAN8720_ADDR,reg,temp)!=0)
        return -1;
    return 0;
}

//��LAN8720 Power Downģʽ
void LAN8720_EnablePowerDownMode(void)
{
    u32 readval=0;
    LAN8720_ReadPHY(LAN8720_BCR,&readval);
    readval|=LAN8720_BCR_POWER_DOWN;
    LAN8720_WritePHY(LAN8720_BCR,readval);
}

//�ر�LAN8720 Power Downģʽ
void LAN8720_DisablePowerDownMode(void)
{
    u32 readval=0;
    LAN8720_ReadPHY(LAN8720_BCR,&readval);
    readval&=~LAN8720_BCR_POWER_DOWN;
    LAN8720_WritePHY(LAN8720_BCR,readval);
}

//����LAN8720����Э�̹���
void LAN8720_StartAutoNego(void)
{
    u32 readval=0;
    LAN8720_ReadPHY(LAN8720_BCR,&readval);
    readval|=LAN8720_BCR_AUTONEGO_EN;
    LAN8720_WritePHY(LAN8720_BCR,readval);
}

//ʹ�ܻز�ģʽ
void LAN8720_EnableLoopbackMode(void)
{
    u32 readval=0;
    LAN8720_ReadPHY(LAN8720_BCR,&readval);
    readval|=LAN8720_BCR_LOOPBACK;
    LAN8720_WritePHY(LAN8720_BCR,readval);
}

//�ر�LAN8720�Ļز�ģʽ
void LAN8720_DisableLoopbackMode(void)
{
    u32 readval=0;
    LAN8720_ReadPHY(LAN8720_BCR,&readval);
    readval&=~LAN8720_BCR_LOOPBACK;
    LAN8720_WritePHY(LAN8720_BCR,readval);
}

//ʹ���жϣ��ж�Դ��ѡ:LAN8720_ENERGYON_IT
//                     LAN8720_AUTONEGO_COMPLETE_IT
//                     LAN8720_REMOTE_FAULT_IT
//                     LAN8720_LINK_DOWN_IT
//                     LAN8720_AUTONEGO_LP_ACK_IT
//                     LAN8720_PARALLEL_DETECTION_FAULT_IT
//                     LAN8720_AUTONEGO_PAGE_RECEIVED_IT
void LAN8720_EnableIT(u32 interrupt)
{
    u32 readval=0;
    LAN8720_ReadPHY(LAN8720_IMR,&readval);
    readval|=interrupt;
    LAN8720_WritePHY(LAN8720_IMR,readval);
}

//�ر��жϣ��ж�Դ��ѡ:LAN8720_ENERGYON_IT
//                     LAN8720_AUTONEGO_COMPLETE_IT
//                     LAN8720_REMOTE_FAULT_IT
//                     LAN8720_LINK_DOWN_IT
//                     LAN8720_AUTONEGO_LP_ACK_IT
//                     LAN8720_PARALLEL_DETECTION_FAULT_IT
//                     LAN8720_AUTONEGO_PAGE_RECEIVED_IT
void LAN8720_DisableIT(u32 interrupt)
{
    u32 readval=0;
    LAN8720_ReadPHY(LAN8720_IMR,&readval);
    readval&=~interrupt;
    LAN8720_WritePHY(LAN8720_IMR,readval);
}

//����жϱ�־λ�����Ĵ���ISFR�Ϳ�����жϱ�־λ
void LAN8720_ClearIT(u32 interrupt)
{
    u32 readval=0;
    LAN8720_ReadPHY(LAN8720_ISFR,&readval);
}

//��ȡ�жϱ�־λ
//����ֵ��1 �жϱ�־λ��λ��
//        0 �жϱ�־λ����
u8 LAN8720_GetITStatus(u32 interrupt)
{
    u32 readval=0;
    u32 status=0;
    LAN8720_ReadPHY(LAN8720_ISFR,&readval);
    if(readval&interrupt) status=1;
    else status=0;
    return status;
}

//��ȡLAN8720������״̬
//����ֵ��LAN8720_STATUS_LINK_DOWN              ���ӶϿ�
//        LAN8720_STATUS_AUTONEGO_NOTDONE       �Զ�Э�����
//        LAN8720_STATUS_100MBITS_FULLDUPLEX    100Mȫ˫��
//        LAN8720_STATUS_100MBITS_HALFDUPLEX    100M��˫��
//        LAN8720_STATUS_10MBITS_FULLDUPLEX     10Mȫ˫��
//        LAN8720_STATUS_10MBITS_HALFDUPLEX     10M��˫��
u32 LAN8720_GetLinkState(void)
{
    u32 readval=0;

    //��ȡ���飬ȷ����ȡ��ȷ������
    LAN8720_ReadPHY(LAN8720_BSR,&readval);
    LAN8720_ReadPHY(LAN8720_BSR,&readval);

    //��ȡ����״̬(Ӳ�������ߵ����ӣ�����TCP��UDP��������ӣ�)
    if((readval&LAN8720_BSR_LINK_STATUS)==0)
        return LAN8720_STATUS_LINK_DOWN;

    //��ȡ�Զ�Э��״̬
    LAN8720_ReadPHY(LAN8720_BCR,&readval);
    if((readval&LAN8720_BCR_AUTONEGO_EN)!=LAN8720_BCR_AUTONEGO_EN)  //δʹ���Զ�Э��
    {
        if(((readval&LAN8720_BCR_SPEED_SELECT)==LAN8720_BCR_SPEED_SELECT)&&
                ((readval&LAN8720_BCR_DUPLEX_MODE)==LAN8720_BCR_DUPLEX_MODE))
            return LAN8720_STATUS_100MBITS_FULLDUPLEX;
        else if((readval&LAN8720_BCR_SPEED_SELECT)==LAN8720_BCR_SPEED_SELECT)
            return LAN8720_STATUS_100MBITS_HALFDUPLEX;
        else if((readval&LAN8720_BCR_DUPLEX_MODE)==LAN8720_BCR_DUPLEX_MODE)
            return LAN8720_STATUS_10MBITS_FULLDUPLEX;
        else
            return LAN8720_STATUS_10MBITS_HALFDUPLEX;
    }
    else                                                            //ʹ�����Զ�Э��
    {
        LAN8720_ReadPHY(LAN8720_PHYSCSR,&readval);
        if((readval&LAN8720_PHYSCSR_AUTONEGO_DONE)==0)
            return LAN8720_STATUS_AUTONEGO_NOTDONE;
        if((readval&LAN8720_PHYSCSR_HCDSPEEDMASK)==LAN8720_PHYSCSR_100BTX_FD)
            return LAN8720_STATUS_100MBITS_FULLDUPLEX;
        else if ((readval&LAN8720_PHYSCSR_HCDSPEEDMASK)==LAN8720_PHYSCSR_100BTX_HD)
            return LAN8720_STATUS_100MBITS_HALFDUPLEX;
        else if ((readval&LAN8720_PHYSCSR_HCDSPEEDMASK)==LAN8720_PHYSCSR_10BT_FD)
            return LAN8720_STATUS_10MBITS_FULLDUPLEX;
        else
            return LAN8720_STATUS_10MBITS_HALFDUPLEX;
    }
}


//����LAN8720������״̬
//����linkstate��LAN8720_STATUS_100MBITS_FULLDUPLEX 100Mȫ˫��
//               LAN8720_STATUS_100MBITS_HALFDUPLEX 100M��˫��
//               LAN8720_STATUS_10MBITS_FULLDUPLEX  10Mȫ˫��
//               LAN8720_STATUS_10MBITS_HALFDUPLEX  10M��˫��
void LAN8720_SetLinkState(u32 linkstate)
{

    u32 bcrvalue=0;
    LAN8720_ReadPHY(LAN8720_BCR,&bcrvalue);
    //�ر��������ã������Զ�Э�̣��ٶȺ�˫��
    bcrvalue&=~(LAN8720_BCR_AUTONEGO_EN|LAN8720_BCR_SPEED_SELECT|LAN8720_BCR_DUPLEX_MODE);
    if(linkstate==LAN8720_STATUS_100MBITS_FULLDUPLEX)       //100Mȫ˫��
        bcrvalue|=(LAN8720_BCR_SPEED_SELECT|LAN8720_BCR_DUPLEX_MODE);
    else if(linkstate==LAN8720_STATUS_100MBITS_HALFDUPLEX)  //100M��˫��
        bcrvalue|=LAN8720_BCR_SPEED_SELECT;
    else if(linkstate==LAN8720_STATUS_10MBITS_FULLDUPLEX)   //10Mȫ˫��
        bcrvalue|=LAN8720_BCR_DUPLEX_MODE;

    LAN8720_WritePHY(LAN8720_BCR,bcrvalue);
}

//��ʼ��LAN8720
int32_t LAN8720_ResetInit(void)
{
    u32 timeout=0;
    u32 regval=0;
    int32_t status=LAN8720_STATUS_OK;
    if(LAN8720_WritePHY(LAN8720_BCR,LAN8720_BCR_SOFT_RESET)>=0) //LAN8720�����λ
    {
        //�ȴ������λ���
        if(LAN8720_ReadPHY(LAN8720_BCR,&regval)>=0)
        {
            while(regval&LAN8720_BCR_SOFT_RESET)
            {
                if(LAN8720_ReadPHY(LAN8720_BCR,&regval)<0)
                {
                    status=LAN8720_STATUS_READ_ERROR;
                    break;
                }
                DJY_DelayUs(10*1000);
                timeout++;
                if(timeout>=LAN8720_TIMEOUT) break; //��ʱ����,5S
            }

        }
        else
        {
            status=LAN8720_STATUS_READ_ERROR;
        }
    }
    else
    {
        status=LAN8720_STATUS_WRITE_ERROR;
    }
    LAN8720_StartAutoNego();                //�����Զ�Э�̹���

    if(status==LAN8720_STATUS_OK)           //���ǰ��������������ʱ1s
        DJY_DelayUs(200*1000);                     //�ȴ�1s

    //�ȴ��������ӳɹ�
    timeout=0;
    while(LAN8720_GetLinkState()<=LAN8720_STATUS_LINK_DOWN)
    {
        DJY_DelayUs(10*1000);
        timeout++;
        if(timeout>=LAN8720_TIMEOUT)
        {
            status=LAN8720_STATUS_LINK_DOWN;
            break; //��ʱ����,5S
        }
    }
    return status;
}





