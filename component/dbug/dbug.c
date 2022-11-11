#include<stdio.h>
#include"dbug.h"
#include"shell.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���

//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"debug information"//�������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:system              //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//grade:"none"                  //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"             //������������������������none����ʾ�������������
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_DEBUG_INFORMATION == false )
//#warning  " debug_information  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_DEBUG_INFORMATION    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure
//DJYOS����ϵͳprintf����Ĭ�ϴ�ӡ�ȼ�

#ifdef DEBUG
u32 debuglevel=CN_DEBUG;
#else
u32 debuglevel=CN_ERR;
#endif


// ============================================================================
// ���ܣ������豸�Ĵ�ӡ�ȼ�
// ������param���ַ�0 ~ 7����Ӧ�豸�Ĵ�ӡ�ȼ�
// ���أ�true -- �ɹ��� false -- ʧ��
// ��ע��
// ============================================================================
bool_t DbugSetPrintfLevel(u32 Level)
{
    if (Level <= CN_DEBUG)
    {
        debuglevel = Level;
        printf("Current device print level = %d\r\n", debuglevel);
        return true;
    }
    else
    {
        printf("device print level set fail, Current level = %d\r\n", debuglevel);
    }
    return false;
}

// ============================================================================
// ���ܣ�shell�����豸�Ĵ�ӡ�ȼ�
// ������param���ַ�0 ~ 7����Ӧ�豸�Ĵ�ӡ�ȼ�
// ���أ�true -- �ɹ��� false -- ʧ��
// ��ע��shellר��
// ============================================================================
bool_t ShellDbugSetPrintfLevel(char *param)
{
    u32 Level;
    Level = (u32)atoi(param);

    if (Level <= CN_DEBUG)
    {
        debuglevel = Level;
        printf("Current device print level = %d\r\n", debuglevel);
    }
    else
    {
        printf("device print level set fail, Current level = %d\r\n", debuglevel);
    }
    return true;
}

// ============================================================================
// ���ܣ���ȡ�豸�Ĵ�ӡ�ȼ�
// ��������
// ���أ���ǰ�Ĵ�ӡ�ȼ�
// ��ע��
// ============================================================================
u32 DbugGetPrintfLevel(void)
{
    return debuglevel;
}

ADD_TO_ROUTINE_SHELL(DebugLevel, ShellDbugSetPrintfLevel, "�޸��豸�Ĵ�ӡ�ȼ�����Χ(0 ~ 7)");
