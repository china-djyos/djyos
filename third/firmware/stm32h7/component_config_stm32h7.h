
//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"stm32h7"//ԭ���̼���
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:third               //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:none                //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
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
#if ( CFG_MODULE_ENABLE_STM32H7 == false )
//#warning  " stm32h7  �������δ���ã�ʹ��Ĭ������"
//%$#@target = cmdline   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_STM32H7    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100
//%$#@enum,1,2,3,4,5,6,7,8
#define CFG_STM32_HAL_TIMER     6//����HAL��ʹ�õĶ�ʱ��
//%$#@string,1,16
//%$#@SYMBOL        ***�������ý��棬ֱ�Ӷ������
#define CFG_CPU_STM32H7
//%$#@select,1
#define     USE_HAL_DRIVER          //�Ƿ�ʹ��hal��
//%$#@select,1
//#define STM32H743xx          //ֻ��ѡ������һ��
//#define STM32H753xx
//#define STM32H750xx
//#define STM32H742xx
//#define STM32H745xx
//#define STM32H745xG
//#define STM32H755xx
//#define STM32H747xx
//#define STM32H747xG
//#define STM32H757xx
//#define STM32H7A3xx
//#define STM32H7A3xxQ
//#define STM32H7B3xx
//#define STM32H7B3xxQ
//#define STM32H7B0xx
//#define STM32H7B0xxQ
//#define STM32H735xx
//#define STM32H733xx
//#define STM32H730xx
//#define STM32H730xxQ
//#define STM32H725xx
//#define STM32H723xx
//%$#@free
#endif
//%$#@end configue  ****�������ý���
//%$#@exclude
//.\BSP;.\CMSIS;
//.\STM32H7xx_HAL_Driver\Src\stm32h7xx_hal_msp_template.c;
//.\STM32H7xx_HAL_Driver\Src\stm32h7xx_hal_timebase_rtc_alarm_template.c;
//.\STM32H7xx_HAL_Driver\Src\stm32h7xx_hal_timebase_rtc_wakeup_template.c;
//.\STM32H7xx_HAL_Driver\Src\stm32h7xx_hal_timebase_tim_template.c;
//%$#@end exclude

//%$#@API include path         ****APIͷ�ļ�·���б��÷ֺš�;���������Ա��ļ�����Ŀ¼Ϊ��ǰĿ¼��������ͨ�����Ӧ�ñ���ʱ��DIDE���Զ��ѱ��б��е�ͷ�ļ��ӵ� -I ����·����
//.\STM32H7xx_HAL_Driver\Inc;
//.\CMSIS\Include;
//.\CMSIS\Device\ST\STM32H7xx\Include;
//%$#@end API include path

//%$#@self include path         ****����������Ҫ��ͷ�ļ�·���б��÷ֺš�;���������Ա��ļ�����Ŀ¼Ϊ��ǰĿ¼��������ͨ��������뱾��ʱ��DIDE���Զ��ѱ��б��е�ͷ�ļ��ӵ� -I ����·����
//.\STM32H7xx_HAL_Driver\Inc;
//.\CMSIS\Include;
//%$#@end self include path

//@#$%component end configure
