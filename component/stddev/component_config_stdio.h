#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//���²����ڳ�ʼ��moduleʱȷ���������в��ɸ���
//��������˶�stdin�Լ�stdout���棬�����ʵ��uart��Telnet��������Ϊ�ն�ʹ�ã���
//�����û�������������豸�Զ��л���ע�⣬��Ϊ����ʱ��stdin�����ǿɶ�д�ġ�
#define CN_STDIO_STDOUT_FOLLOW      2       // stdout����stdin
#define CN_STDIO_STDERR_FOLLOW      4       // stderr����stdin
#define CN_STDIO_STDIN_MULTI        8       // ����stdin��Դ�����͵���uart��telnet

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern s32 ModuleInstall_STDIO(const char *in,const char *out, const char *err);
//    ModuleInstall_STDIO(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME,CFG_STDIO_ERR_NAME);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"stdio"        //��д�����������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:�������               //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//grade:main                    //��ʼ��ʱ������ѡֵ��none��init��main��none��ʾ�����ʼ����
                                //init��ʾ�ڵ���main֮ǰ��main��ʾ��main�����г�ʼ��
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
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#define CFG_STDIO_RUN_MODE            (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW) //"STDIOģʽ",
#define CFG_STDIO_IN_NAME              "/dev/UART1"//"�����豸��",
#define CFG_STDIO_OUT_NAME             "/dev/UART1"//"����豸��",
#define CFG_STDIO_ERR_NAME             "/dev/UART1"//"err����豸��",
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
