// =============================================================================

// �ļ���     ��cpu_peri_rng.c
// ģ������: GMACģ��ײ�Ӳ��������ΪDJYIP��Э��ջ�ض�������
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 02/13.2017
// =============================================================================
#include "stdlib.h"
#include "cpu_peri.h"
#include "systime.h"
#include "board-config.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern u8 RNG_Init(void);
//    RNG_Init();
//%$#@end initcode  ****��ʼ���������
//%$#@describe      ****���������ʼ
//component name:"cpu onchip random"//CPU��mac����
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
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
#define CFG_RNG_NUM_MAX      65535         //"��������ֵ",
#define CFG_RNG_NUM_MIN      1024          //"�������Сֵ",
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

RNG_HandleTypeDef RNG_Handler;


//�õ������
//����ֵ:��ȡ���������
u32 RNG_Get_RandomNum(void)
{
    u32 randomnum;

    HAL_RNG_GenerateRandomNumber(&RNG_Handler,&randomnum);
    return randomnum;
}

//����[min,max]��Χ�������
int RNG_Get_RandomRange(int min,int max)
{
    u32 randomnum;

    HAL_RNG_GenerateRandomNumber(&RNG_Handler,&randomnum);
    return randomnum%(max-min+1)+min;
}

//��ʼ��RNG
u8 RNG_Init(void)
{
    u16 retry=0;

    RNG_Handler.Instance=RNG;
    HAL_RNG_DeInit(&RNG_Handler);
    HAL_RNG_Init(&RNG_Handler);//��ʼ��RNG
    while(__HAL_RNG_GET_FLAG(&RNG_Handler,RNG_FLAG_DRDY)==RESET&&retry<10000)//�ȴ�RNG׼������
    {
        retry++;
        DJY_DelayUs(10);
    }
    if(retry>=10000) return 1;//���������������������
    srand(RNG_Get_RandomRange(CFG_RNG_NUM_MIN,CFG_RNG_NUM_MAX));
    return 0;
}

void HAL_RNG_MspInit(RNG_HandleTypeDef *hrng)
{
    RCC_PeriphCLKInitTypeDef RNGClkInitStruct;

    //����RNGʱ��Դ��ѡ��PLL��ʱ��Ϊ400MHz
    RNGClkInitStruct.PeriphClockSelection=RCC_PERIPHCLK_RNG;    //����RNGʱ��Դ
    RNGClkInitStruct.RngClockSelection=RCC_RNGCLKSOURCE_PLL;    //RNGʱ��Դѡ��PLL
    HAL_RCCEx_PeriphCLKConfig(&RNGClkInitStruct);

    __HAL_RCC_RNG_CLK_ENABLE();//ʹ��RNGʱ��
}


