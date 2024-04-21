#ifndef     _USER_MB_APP
#define     _USER_MB_APP

#include "mb.h"
#include "mb_m.h"
#include "mbconfig.h"
#include "mbframe.h"
#include "mbutils.h"
#include <project_config.h>

/* -----------------------Slave Defines -------------------------------------*/
#define S_DISCRETE_INPUT_START                    CFG_MB_S_DISCRETE_INPUT_START     
#define S_DISCRETE_INPUT_NDISCRETES               CFG_MB_S_DISCRETE_INPUT_NDISCRETES
#define S_COIL_START                              CFG_MB_S_COIL_START               
#define S_COIL_NCOILS                             CFG_MB_S_COIL_NCOILS              
#define S_REG_INPUT_START                         CFG_MB_S_REG_INPUT_START          
#define S_REG_INPUT_NREGS                         CFG_MB_S_REG_INPUT_NREGS          
#define S_REG_HOLDING_START                       CFG_MB_S_REG_HOLDING_START        
#define S_REG_HOLDING_NREGS                       CFG_MB_S_REG_HOLDING_NREGS        

// modbus״̬
enum eMBSlaveTransferMode
{
    MB_RTU_MODE = 0,
    MB_TCP_MODE,
    MB_ASCII_MODE,
};
bool_t ModuleInstall_ModbusSlave(UCHAR ucAddr, enum eMBSlaveTransferMode ucMode,
                    UCHAR ucNum, ULONG ulBaudrate, eMBParity eParity, USHORT usTCP_Port);
#endif
