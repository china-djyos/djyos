#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�


//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern s32 ModuleInstall_STDIO(const char *in,const char *out, const char *err);
//    ModuleInstall_STDIO(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME,CFG_STDIO_ERR_NAME);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"stdio"        //��д�����������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:system              //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:later               //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"             //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������������������������none����ʾ�������������
                                //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef CFG_STDIO_RUN_MODE
#warning stdioģ��δ���ã�ʹ��Ĭ������
//%$#@num,0,100,
//%$#@enum,true,false,
#define CFG_STDIO_STDIN_MULTI      true    //"�Ƿ�֧�ֶ��������豸",
#define CFG_STDIO_STDOUT_FOLLOW    true    //"stdout�Ƿ����stdin",
#define CFG_STDIO_STDERR_FOLLOW    true    //"stderr�Ƿ����stdin",
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#define CFG_STDIO_IN_NAME              "/dev/UART1"    //"��׼�����豸��",
#define CFG_STDIO_OUT_NAME             "/dev/UART1"    //"��׼����豸��",
#define CFG_STDIO_ERR_NAME             "/dev/UART1"    //"��׼err����豸��",
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
//#define CFG_STDIO_RUN_MODE            (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW) //"STDIOģʽ",
