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
//component name:"cpu_peri_rng" //CPU��mac����
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none",      //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������������������������none����ʾ�������������
                                //����������������������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header    //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef cpu_peri_rng   //****�������Ƿ��Ѿ����ú�
#warning    cpu_peri_rng�������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,0,255,
//%$#@num,1000,65535,
#define CFG_RNG_NUM_MAX      65535         //"��������ֵ",
#define CFG_RNG_NUM_MIN      1024          //"�������Сֵ",
//%$#@enum,true,false,
//%$#@string,1,32,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
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
        Djy_DelayUs(10);
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

