#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    externs32 ModuleInstall_EFS(const char *target, const char *source, u32 opt, void *config);
//    ModuleInstall_EFS(CFG_MOUNT_POINT, CFG_MOUNT_DEV, CFG_OPTIONS, CFG_FILE_BLOCK_SIZE);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"easyfilesystem"     //easy�ļ�ϵͳ
//parent:"filesystem"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:system                    //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                    //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                      //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium                    //��ʼ��ʱ������ѡֵ��early��medium��later��
                                      //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"filesystem"             //������������������������none����ʾ�������������
                                      //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                      //����������������������г����á�,���ָ�
//weakdependence:"none"               //��������������������������none����ʾ�������������
                                      //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                      //����������������������г����á�,���ָ�
//mutex:"none"                        //������������������������none����ʾ�������������
                                      //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef CFG_OPTIONS   //****�������Ƿ��Ѿ����ú�
#warning    efsfilesystem�������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,0,100,
#define CFG_OPTIONS             0       //"name",�ļ�ϵͳ�ļ���װ�߼�
#define CFG_FILE_BLOCK_SIZE     0
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#define CFG_MOUNT_POINT         ""      //"name",EFS�ļ�ϵͳ��װĿ¼
#define CFG_MOUNT_DEV           ""      //"name",EFS�ļ�ϵͳ�����豸·��
#endif//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure
