//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//****�½����������copy component_config_template.txt�����ݵ����
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//  extern s32 ModuleInstall_Python(u32 stack_size);
//    ModuleInstall_Python(CFG_PYTHON_STACKSIZE);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"microPython113" //�����
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:third               //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable               //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:pre-main            //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"shell"            //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_MICROPYTHON113 == false )
//#warning  " myname �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_MICROPYTHON113    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,0x10000,
#define CFG_PYTHON_STACKSIZE    4096    //"����python�Ļ���stack�ߴ�"
#define CFG_PYTHON_HEAPSIZE     4096    //"����python�Ļ���heap�ߴ�"
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#@SYMBOL,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�ų��ļ��б����
//%$#@API include path         ****APIͷ�ļ�·���б��÷ֺš�;���������Ա��ļ�����Ŀ¼Ϊ��ǰĿ¼��������ͨ�����Ӧ�ñ���ʱ��DIDE���Զ��ѱ��б��е�ͷ�ļ��ӵ� -I ����·����
//./;./ports/djyos;${ProjDirPath}/libmicroPython_App_Debug/build;${ProjDirPath}/libmicroPython_App_Release/build;
//%$#@end API include path
//%$#@self include path         ****���������ͷ�ļ�·���б��÷ֺš�;���������Ա��ļ�����Ŀ¼Ϊ��ǰĿ¼��������ͨ��������뱾��ʱ��DIDE���Զ��ѱ��б��е�ͷ�ļ��ӵ� -I ����·����
//./include;
//%$#@end self include path
//%$#@library path             ****�г���������õĶ������ļ��б��÷ֺš�;���������Ա��ļ�����Ŀ¼Ϊ��ǰĿ¼��������ͨ���
//./ip/my_lib_name.a
//%$#@end library path

//@#$%component end configure

