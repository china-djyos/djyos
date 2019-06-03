#ifndef _SCM6XX_RESET_H_
#define _SCM6XX_RESET_H_

#include "SCM6XX_ALL.h"

void RMU_Software_Reset_System(void);
void RMU_Software_Reset_Module(UINT8 module);
UINT8 RMU_Get_Reset_Status(UINT32 Rst_Status);
void RMU_Clear_Reset_Status(UINT32 Rst_Status);

#endif 





