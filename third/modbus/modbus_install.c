#include <user_mb_app.h>
#include <user_mb_app_m.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

// =============================================================================
// ��������:modbusģ�鰲װ
// �������:��
// ����ֵ : true -- �ɹ���false -- ʧ��
// =============================================================================
bool_t ModuleInstall_Modbus(void)
{
    bool_t result = false;

#if CFG_MB_SLAVE_ENABLED == TRUE

    result = ModuleInstall_ModbusSlave(CFG_MB_S_ADDR, CFG_MB_S_TRANSFER_MODE, CFG_MB_S_PORT_NUM, 
                                        CFG_MB_S_PORT_BAUDRATE, CFG_MB_S_PORT_PARITY, CFG_MB_S_TCP_PORT_NUM);

#endif

#if CFG_MB_MASTER_ENABLED == TRUE

    result = ModuleInstall_ModbusMaster(CFG_MB_M_PORT_NUM, CFG_MB_M_PORT_BAUDRATE, CFG_MB_M_PORT_PARITY);

#endif

return result;
}
