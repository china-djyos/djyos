#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//#if !defined (CFG_RUNMODE_BAREAPP)
//    extern bool_t ModuleInstall_UpdateIboot(void);
//    ModuleInstall_UpdateIboot( );
//#endif
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"loader"       //������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:system              //ѡ�third��system��bsp��user����������������IDE�з���
//select:required               //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:                   //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_LOADER == false )
//#warning  " loader  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_LOADER    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
#define CFG_UPDATEIBOOT_EN      false       //"�Ƿ�֧�����߸���Iboot"��
#define CFG_START_APP_IS_VERIFICATION      true       //"����appʱ�Ƿ�ִ��У�鹦��"��
//%$#@enum,CN_RUN_FORM_FILE,CN_DIRECT_RUN,
#define  CFG_APP_RUNMODE  CN_DIRECT_RUN     //"APP����ģʽ",CN_DIRECT_RUN=ֱ�Ӵ�flash�����У�CN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
//%$#@enum,VERIFICATION_NULL,VERIFICATION_CRC,VERIFICATION_MD5,VERIFICATION_SSL,
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL   //"APPУ�鷽��",
//%$#@num,0,100,
#define CFG_IBOOT_VERSION_SMALL       00        //"Iboot�汾��:��",xx.xx.__��APP����
#define CFG_IBOOT_VERSION_MEDIUM      00        //"Iboot�汾��:��",xx.__.xx��APP����
#define CFG_IBOOT_VERSION_LARGE       01        //"Iboot�汾��:��",__.xx.xx��APP����
//%$#@string,1,40,
#define CFG_IBOOT_UPDATE_NAME      "/yaf2/iboot.bin"           //"������ibootĬ�ϴ洢·��"
#define CFG_APP_UPDATE_NAME        "/yaf2/app.bin"            //"������appĬ�ϴ洢·��"
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,

#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

