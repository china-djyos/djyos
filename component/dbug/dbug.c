#include<stdio.h>
#include"dbug.h"
#include"shell.h"
//DJYOS����ϵͳĬ�ϵ��Դ�ӡ�ȼ�

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
