//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//�����Ƶ���ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"gd32e10x"//vendor firmware
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
#if ( CFG_MODULE_ENABLE_GD32E10X == false )
//#warning  " gd32e10x  �������δ���ã�ʹ��Ĭ������"
//%$#@target = cmdline   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_GD32E10X    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100
//%$#@enum,1,2,3,4,5,6,7,8
#define CFG_STM32_HAL_TIMER     6//����HAL��ʹ�õĶ�ʱ��
//%$#@string,1,16
//%$#@SYMBOL        ***�������ý��棬ֱ�Ӷ������
#define GD32E103R_START
#define CFG_CPU_GD32E10X
#define USE_STDPERIPH_DRIVER
#define GD32E10X
//%$#@select,1
#define     _RTE_          //RTE�Ƿ�ʹ�ÿ�
//%$#@select,1
//#define GD32_DUMMY         //ֻ��ѡ������һ��
#define GD32E10x
//%$#@free
#endif
//%$#@end configue  ****�������ý���
//%$#@exclude
//./GD32E10x_usbfs_library;
//./CMSIS/GD/GD32E10x/Source/ARM/startup_gd32e10x.s;
//./CMSIS/GD/GD32E10x/Source/IAR/startup_gd32e10x.s;
//%$#@end exclude

//%$#@API include path         ****APIͷ�ļ�·���б��÷ֺš�;���������Ա��ļ�����Ŀ¼Ϊ��ǰĿ¼��������ͨ�����Ӧ�ñ���ʱ��DIDE���Զ��ѱ��б��е�ͷ�ļ��ӵ� -I ����·����
//./;
//./CMSIS;
//./CMSIS/GD/GD32E10x/Include;
//./GD32E10x_standard_peripheral/Include;
//%$#@end API include path

//%$#@self include path         ****����������Ҫ��ͷ�ļ�·���б��÷ֺš�;���������Ա��ļ�����Ŀ¼Ϊ��ǰĿ¼��������ͨ��������뱾��ʱ��DIDE���Զ��ѱ��б��е�ͷ�ļ��ӵ� -I ����·����
//%$#@end self include path

//@#$%component end configure

