#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_myname.h****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//   ModuleInstall_EFS(CFG_MOUNT_POINT, CFG_MOUNT_DEV, CFG_OPTIONS, CFG_FILE_BLOCK_SIZE);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"easyfilesystem"     //��д�����������
//parent:"filesystem"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:�������                   //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                         //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                      //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//grade:init                          //��ʼ��ʱ������ѡֵ��none��init��main��none��ʾ�����ʼ����
                                      //init��ʾ�ڵ���main֮ǰ��main��ʾ��main�����г�ʼ��
//dependence:"none"                   //������������������������none����ʾ�������������
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
#warning    easyfilesystem�������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,0,100,
#define CFG_OPTIONS             0       //"����ѡ��",EFS�ļ�ϵͳ�ļ���װ�߼�
#define CFG_FILE_BLOCK_SIZE     0       //"�ļ���С��(block)",�����ļ�ϵͳ�ļ���С�ߴ�
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#define CFG_MOUNT_POINT         ""      //"��װĿ¼",�����ļ�ϵͳ��װĿ¼
#define CFG_MOUNT_DEV           ""      //"�豸·�� ",�����ļ�ϵͳ�����豸·��
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure
