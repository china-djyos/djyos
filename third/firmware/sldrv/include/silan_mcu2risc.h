#ifndef __SILAN_MCU2RISC_H__
#define __SILAN_MCU2RISC_H__

#include "silan_types.h"

#define MBOX_SIZE  16
u8 SendCmd(void);  
void mcu2risc_message(u8 cmd,u8 *para);
void mcu2dsp_message(u8 cmd,u8 *para);
extern u8 Flag_CMDACK_FAIL;

#endif
