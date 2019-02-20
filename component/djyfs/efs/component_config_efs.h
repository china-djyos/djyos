#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//#include "filesystems.h"
//    externs32 s32 ModuleInstall_EFS(const char *target, u32 opt, void *config);
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
#define CFG_EFS_FILE_BLOCK_SIZE           0x1000                 //EFS�ļ�ϵͳ�õ�ý����С.at45���СΪ0x1000����������ý��������Ҫ�޸�
//%$#@enum,true,false,
//%$#@string,1,128,
#define CFG_EFS_MOUNT_POINT               "efs"      //"name",EFS�ļ�ϵͳ��װĿ¼
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#define CFG_EFS_INSTALL_OPTION        MS_INSTALLCREAT       //EFS�ļ�ϵͳ��װѡ�MS_INSTALLCREAT���ļ�ϵͳ������ʱ�����½���
#endif//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure
