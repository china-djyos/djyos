
//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"mongoose"//webserver
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
#if ( CFG_MODULE_ENABLE_MONGOOSE == false )
//#warning  " webserver_goahead250  �������δ���ã�ʹ��Ĭ������"
//%$#@target = cmdline   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_MONGOOSE    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100
#define MG_MAX_DNS_QUESTIONS 8
#define MG_MAX_DNS_ANSWERS 8
//%$#@enum,true,false
//%$#@string,1,16
//%$#@select
//%$#@free
#endif
//%$#@end configue  ****�������ý���
//%$#@exclude
//./docs;./examples;./jni;./src;./test;./tools;
//%$#@end exclude

//%$#@API include path         ****APIͷ�ļ�·���б��÷ֺš�;���������Ա��ļ�����Ŀ¼Ϊ��ǰĿ¼��������ͨ�����Ӧ�ñ���ʱ��DIDE���Զ��ѱ��б��е�ͷ�ļ��ӵ� -I ����·����
//.;
//%$#@end API include path

//%$#@self include path         ****����������Ҫ��ͷ�ļ�·���б��÷ֺš�;���������Ա��ļ�����Ŀ¼Ϊ��ǰĿ¼��������ͨ��������뱾��ʱ��DIDE���Զ��ѱ��б��е�ͷ�ļ��ӵ� -I ����·����
//%$#@end self include path

//@#$%component end configure
//#define MG_ENABLE_SSL

