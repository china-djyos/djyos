#ifndef __BOARD_CONFIG_H__
#define __BOARD_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "cpu-optional.h"

#define DJY_BOARD        "topeet_imx6q"

/*____��Ӳ����ص�����____*/
#define Mhz 1000000
#define CN_CFG_MCLK (800*Mhz)  //��Ƶ���ں�Ҫ�ã����붨��
#define CN_CFG_HCLK (CN_CFG_MCLK/4)  //��������ʱ��
#define CN_CFG_PCLK (CN_CFG_MCLK/8)  //��������ʱ��
#define CN_CFG_TIMER_CLK CN_CFG_PCLK  //��ʱ��ʱ��Դ

/*____���¶���tick����____*/
#define CN_CFG_TICK_US 1000  //tick�������usΪ��λ��
#define CN_CFG_TICK_HZ 1000  //�ں�ʱ��Ƶ�ʣ���λΪhz��
#define CN_USE_TICKLESS_MODE    (0U)
#if (!CN_USE_TICKLESS_MODE)
#define CN_CFG_FINE_US 0x000007B  //1/532,tick����ʱ�����ڣ���uSΪ��λ��32λ������������С����ռ16λ����Ҳ������ticks�������65535uS
#define CN_CFG_FINE_HZ CN_CFG_MCLK  //tick����ʱ��Ƶ�ʣ���CN_CFG_FINE_US�ĵ���
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

//CPU�ܹ��������,��ѡ������������,�����С��,�ǿ�ѡ��,����������,
//��CPU�ֳ��̶�,���ֳ���BSP��arch_feature.h�ļ��ж���
//�洢����С�˵�����
#define CN_CFG_LITTLE_ENDIAN        0
#define CN_CFG_BIG_ENDIAN           1
#define CN_CFG_BYTE_ORDER          CN_CFG_LITTLE_ENDIAN

//LCD��������
#define CN_LCD_XSIZE        480     //LCD�������ؿ���.
#define CN_LCD_YSIZE        272     //LCD�������ظ߶�.
#define CN_LCD_PIXEL_FORMAT CN_SYS_PF_RGB565    //LCD��ɫ��ʽ.

#define  CN_LCD_DRV_FRAME_BUFFER_EN  //����ú���ʹ��FrameBuffer,����ֱ����Ļ���.
#undef  CN_LCD_DRV_REMOTE_EN        //����ú���LCD����ʹ��Զ����ʾ����

#ifdef __cplusplus
}
#endif
#endif // __BOARD_CONFIG_H__