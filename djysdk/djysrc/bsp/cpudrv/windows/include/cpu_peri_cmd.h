//----------------------------------------------------
//Copyright (C), 2004-2011,  ������.
//��Ȩ���� (C), 2004-2011,   ������.
//����ģ��:ʱ��ģ��
//����:  ������
//�汾��V1.0.0
//�ļ�����: �������������
//����˵��:
//�޶���ʷ:
// 1. ����: 2012-10-09
//   ����:  ������
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __CPU_PERI_CMD_H__
#define __CPU_PERI_CMD_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <windows.h>

u32 cmd_int(ufast_t uart_int_line);
u32 WINAPI win32_scanf_pro( LPVOID lpParameter );
ptu32_t ModuleInstall_Cmd(ptu32_t para);

#ifdef __cplusplus
}
#endif

#endif /*__CPU_PERI_CMD_H__*/

