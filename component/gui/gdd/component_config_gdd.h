#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern void ModuleInstall_Gdd_AND_Desktop(void);
//    ModuleInstall_Gdd_AND_Desktop();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"graphical decorate development(gdd)"//��д�����������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:system                          //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                          //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                            //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:later                           //��ʼ��ʱ������ѡֵ��early��medium��later��
                                            //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"graphical kernel","System:Message queue"//������������������������none����ʾ�������������
                                            //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                            //����������������������г����á�,���ָ�
//weakdependence:"HmiInput"                 //��������������������������none����ʾ�������������
                                            //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                            //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                            //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_GRAPHICAL_DECORATE_DEVELOPMENT(GDD) == false )
//#warning  " graphical_decorate_development(gdd)  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_GRAPHICAL_DECORATE_DEVELOPMENT(GDD)    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,800,
#define CFG_DESKTOP_WIDTH       0           //"������",����ߴ磨����������ȣ�0=��ʾ�����
#define CFG_DESKTOP_HEIGHT      0           //"����߶�",����ߴ磨���������߶ȣ�0=��ʾ���߶�
//%$#@enum,true,false,
//%$#@string,1,256,
#define CFG_DISPLAY_NAME        "DISPLAY_NAME"         //"��ʾ����",����bsp����ʾ������ģ�����õ���ʾ��������ͬ
#define CFG_DESKTOP_NAME        "DESKTOP_NAME"         //"������"
#define CFG_INPUTDEV_NAME       "INPUTDEV_NAME"         //"�����豸����",ʹ��bsp�������豸�����õ����֣��������豸�Ļ���ÿ���豸���ö��Ÿ���
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#define CFG_DESKTOP_FORMAT      CN_SYS_PF_RGB565    //"���ظ�ʽ",���洰�����ظ�ʽ��������gkernel.h�ж��壬һ��ʹ������ʾ����ͬ��ɫ
#define CFG_GRAY_BASE_COLOR     CN_COLOR_WHITE      //"�ҶȻ�ɫ",���ظ�ʽ��Ϊ�Ҷ�ʱ����Ҫ���õġ�������ɫ������gkernel.h���ҵ�������ɫ����
#define CFG_FILL_COLOR          CN_COLOR_BLUE       //"���ɫ",��������ʱ�����ɫ����888��ʽ������gkernel.h���ҵ�������ɫ����
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
