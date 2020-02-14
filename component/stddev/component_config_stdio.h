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
//#if(CFG_STDIO_STDIOFILE == true)
//    extern s32 ModuleInstall_STDIO(const char *in,const char *out, const char *err);
//    ModuleInstall_STDIO(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME,CFG_STDIO_ERR_NAME);
//#endif
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"stdio"        //��д�����������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:system              //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:later               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"             //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_STDIO == false )
//#warning  " stdio  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_STDIO    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
#define CFG_STDIO_STDIN_MULTI      true         //"�Ƿ�֧�ֶ��������豸",
#define CFG_STDIO_STDOUT_FOLLOW    true         //"stdout�Ƿ����stdin",
#define CFG_STDIO_STDERR_FOLLOW    true         //"stderr�Ƿ����stdin",
#define CFG_STDIO_FLOAT_PRINT      true         //"֧�ָ����ӡ"
#define CFG_STDIO_STDIOFILE        true         //"֧�ֱ�׼IO�ļ�"
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#define CFG_STDIO_IN_NAME              "/dev/UART1"    //"��׼�����豸��",
#define CFG_STDIO_OUT_NAME             "/dev/UART1"    //"��׼����豸��",
#define CFG_STDIO_ERR_NAME             "/dev/UART1"    //"��׼err����豸��",
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
