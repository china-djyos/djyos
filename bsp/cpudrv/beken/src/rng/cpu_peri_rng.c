// =============================================================================

// �ļ���     ��cpu_peri_rng.c
// ģ������: GMACģ��ײ�Ӳ��������ΪDJYIP��Э��ջ�ض�������
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 02/13.2017
// =============================================================================
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
#include "wlan_ui_pub.h"
#include "arm_arch.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern void ModuleInstall_random(void);
//    ModuleInstall_random();
//%$#@end initcode  ****��ʼ���������
//%$#@describe      ****���������ʼ
//component name:"cpu onchip random"//CPU�����������������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"             //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_RANDOM == false )
//#warning  " cpu_onchip_random  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header    //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_RANDOM    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,255,
//%$#@num,1000,65535,
//%$#@enum,true,false,
//%$#@string,1,32,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure
// =============================================================================

#define TRNG_BASE (0X00802480)
#define TRNG_CTRL (TRNG_BASE + 0*4)
#define TRNG_EN (0x01UL << 0)
#define TRNG_DATA ((TRNG_BASE + 1*4))

static void trang_active(u8 enable)
{
    u32 value;
    value = REG_READ(TRNG_CTRL);

    if(enable)
    {
        value |= TRNG_EN;
    }
    else
    {
        value &= ~TRNG_EN;
    }

    REG_WRITE(TRNG_CTRL,value);
}

u32 trng_get_random(void)
{
    trang_active(1);
    return REG_READ(TRNG_DATA);
}


u16 rand_port (void)
{
    u32 xx =  trng_get_random();
    u16 xx_h = xx>>16;
    u16 xx_l = xx&0xFFFF;
//    printf("xx=%d!\r\n", (int)(xx_h^xx_l));
    return (u16)(xx_h^xx_l);
}


void ModuleInstall_random(void)
{
  u32 crc,crc2;
  u8 i=8;
  trang_active(1);
  while(i--)
  {
      crc2 =trng_get_random();
      crc ^= crc2;

      printf("\n\r\n\r random_init NUm ====== 0x%x,0x%x\n\r\n\r",crc,crc2);
  }
  srand(crc);
}
