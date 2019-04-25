#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern s32 ModuleInstall_EFS_Full(const char *target, u32 opt, void *config);
//    ModuleInstall_EFS_Full(CFG_EFS_FULL_MOUNT_POINT, CFG_EFS_INSTALL_OPTION, CFG_EFS_FULL_FILE_BLOCK_SIZE);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ+
//component name:"easyfullfilesystem"     //easy�ļ�ϵͳ
//parent:"filesystem"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:system                    //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                    //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                      //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                    //��ʼ��ʱ������ѡֵ��early��medium��later��
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
#ifndef CFG_EFS_FULL_MOUNT_POINT   //****�������Ƿ��Ѿ����ú�
#warning    efsfullfilesystem�������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,0,1000000000,
#define CFG_EFS_FULL_FILE_BLOCK_SIZE           4096                 //EFS�ļ���С����.
//%$#@enum,true,false,
//%$#@enum,16777216,256,0,
#define CFG_EFS_INSTALL_OPTION           16777216        //EFS�ļ�ϵͳ��װѡ�16777216:�ļ�ϵͳ������ʱ���½���256����ʽ���ļ�ϵͳ
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#define CFG_EFS_FULL_MOUNT_POINT               "efsfull"      //"name",EFS�ļ�ϵͳ��װĿ¼
#endif//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure
