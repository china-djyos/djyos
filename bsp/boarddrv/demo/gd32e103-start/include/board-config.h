#ifndef __BOARD_CONFIG_H__
#define __BOARD_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "cpu-optional.h"

#define DJY_BOARD       "EXPLORER-STM32F4"

#define Mhz             1000000
#define CN_CFG_MCLK     (168*Mhz)           //��Ƶ���ں�Ҫ�ã����붨��
#define CN_CFG_FCLK     CN_CFG_MCLK
#define CN_CFG_AHBCLK   CN_CFG_MCLK         //����ʱ��
#define CN_CFG_PCLK2    (CN_CFG_MCLK/2)     //��������ʱ��
#define CN_CFG_PCLK1    (CN_CFG_MCLK/4)     //��������ʱ��
#define CN_CFG_EXTCLK   (8*Mhz)             //�ⲿʱ��=8M

/*____���¶���tick����____*/
#define CN_CFG_TICK_US 1000  //tick�������usΪ��λ��
#define CN_CFG_TICK_HZ 1000  //�ں�ʱ��Ƶ�ʣ���λΪhz��
#define CN_USE_TICKLESS_MODE    (0U)
#if (!CN_USE_TICKLESS_MODE)
#define CN_CFG_FINE_US 0x00000186  //1/168M,tick����ʱ�����ڣ���uSΪ��λ��32λ������������С����ռ16λ����Ҳ������ticks�������65535uS
#define CN_CFG_FINE_HZ CN_CFG_MCLK  //tick����ʱ��Ƶ�ʣ���CN_CFG_FINE_US�ĵ�����
#else
#define CN_CFG_USE_USERTIMER        (0U)//�Ƿ�ʹ��LPTIMER��Ϊϵͳʱ��
#define CN_CFG_TIME_BASE_HZ         CN_CFG_MCLK//(32000U)//(8000U)
#if (!CN_CFG_USE_USERTIMER)
#define CN_CFG_USE_BYPASSTIMER      (0U)//0��ʾ��ʹ����·��ʱ����1��ʾʹ����·��ʱ��
#define CN_CFG_TIME_PRECISION       (500U)/*���ȵ�λ��US*/
#else
#define CN_CFG_USERTIMER_PRESC      (1U)//����ʹ��SYSTICK��ʱ������ָ���û���ʱ���ķ�Ƶ��
#define CN_CFG_FINE_US (0x1F4000U)  //1/32000,tick����ʱ�����ڣ���uSΪ��λ��32λ������������С����ռ16λ����Ҳ������ticks�������65535uS
#define CN_CFG_FINE_HZ (0x831U)  //tick����ʱ��Ƶ�ʣ���CN_CFG_FINE_US�ĵ���
#define CN_CFG_TIME_PRECISION       (200U)/*���ȵ�λ��US*/
#endif
#endif

//CPU�ܹ��������,��ѡ���߿��ܿ�ѡ�Ĳ�����������,�����С��,�ǿ�ѡ��,����������,
//��CPU�ֳ��̶�,���ֳ���BSP��arch.h�ļ��ж���
//�洢����С�˵�����
#define CN_CFG_LITTLE_ENDIAN        0
#define CN_CFG_BIG_ENDIAN           1
#define CN_CFG_BYTE_ORDER          CN_CFG_LITTLE_ENDIAN

#ifdef __cplusplus
}
#endif
#endif // __CPU_OPTIONAL_H__




