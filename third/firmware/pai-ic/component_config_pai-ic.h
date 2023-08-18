
//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"PAI_IC"       //ԭ���̼���
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
#if ( CFG_MODULE_ENABLE_PAI_IC == false )
//#warning  " stm32f7  �������δ���ã�ʹ��Ĭ������"
//%$#@target = cmdline   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_PAI_IC    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100
//%$#@enum,1,2,3,4,5,6,7,8
//%$#@string,1,16
//%$#@SYMBOL        ***�������ý��棬ֱ�Ӷ������
//%$#@select,1
#define     PT32Z192Ex           //ֻ��ѡһ��
//#define     PT32Z192Cx
//%$#@free
#endif
//%$#@end configue  ****�������ý���
//%$#@exclude
//./CMSIS/Device/PT/PT32Z192xx/Source/Templates/system_pt32z192xx.c
//%$#@end exclude

//%$#@API include path         ****APIͷ�ļ�·���б��÷ֺš�;���������Ա��ļ�����Ŀ¼Ϊ��ǰĿ¼��������ͨ�����Ӧ�ñ���ʱ��DIDE���Զ��ѱ��б��е�ͷ�ļ��ӵ� -I ����·����
//.\PT32Z192xx_StdPeriph_Lib_Driver\cmsis;
//.\CMSIS\Device\PT\PT32Z192xx\Include;
//.\PT32Z192xx_StdPeriph_Lib_Driver\inc;
//.\CMSIS\Include;
//.\FocLib;.\kws_lib;
//%$#@end API include path

//%$#@self include path         ****����������Ҫ��ͷ�ļ�·���б��÷ֺš�;���������Ա��ļ�����Ŀ¼Ϊ��ǰĿ¼��������ͨ��������뱾��ʱ��DIDE���Զ��ѱ��б��е�ͷ�ļ��ӵ� -I ����·����
//.\CMSIS\Include;.\CMSIS\Device\PT\PT32Z192xx\Include;.\PT32Z192xx_StdPeriph_Lib_Driver\inc;
//%$#@end self include path

//%$#@library path             ****�г���������õ�.a���ļ��б��÷ֺš�;���������Ա��ļ�����Ŀ¼Ϊ��ǰĿ¼��������ͨ���
//./FocLib/libPtFocLib_20300130.lib;./kws_lib/libAiMath.lib;./kws_lib/libazpAuth.lib;./kws_lib/libKWSApp.lib;./kws_lib/libKWSModel.lib;
//%$#@end library path

//@#$%component end configure

