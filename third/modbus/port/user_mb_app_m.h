#ifndef     _USER_MB_APP_M
#define     _USER_MB_APP_M
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mb_m.h"
#include "mbconfig.h"
#include "mbframe.h"
#include "mbutils.h"
/* -----------------------Master Defines -------------------------------------*/
#define M_DISCRETE_INPUT_START                    CFG_MB_M_DISCRETE_INPUT_START
#define M_DISCRETE_INPUT_NDISCRETES               CFG_MB_M_DISCRETE_INPUT_NDISCRETES
#define M_COIL_START                              CFG_MB_M_COIL_START
#define M_COIL_NCOILS                             CFG_MB_M_COIL_NCOILS
#define M_REG_INPUT_START                         CFG_MB_M_REG_INPUT_START
#define M_REG_INPUT_NREGS                         CFG_MB_M_REG_INPUT_NREGS
#define M_REG_HOLDING_START                       CFG_MB_M_REG_HOLDING_START
#define M_REG_HOLDING_NREGS                       CFG_MB_M_REG_HOLDING_NREGS

bool_t ModuleInstall_ModbusMaster(UCHAR ucNum, ULONG ulBaudrate, eMBParity eParity);
eMBErrorCode eMBMasterRegHoldingCB_Get(UCHAR ucSndAddr, UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs);
eMBErrorCode eMBMasterRegCoilsCB_Get(UCHAR ucSndAddr, UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils);
eMBErrorCode eMBMasterRegInputCB_Get(UCHAR ucSndAddr, UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs );
eMBErrorCode eMBMasterRegDiscreteCB_Get(UCHAR ucSndAddr, UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete );
#endif
