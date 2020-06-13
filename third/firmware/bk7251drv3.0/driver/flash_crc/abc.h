#ifndef __ABC_INCLUDE_H__
#define __ABC_INCLUDE_H__
#include "sys_config.h"
#if CFG_SUPPORT_DJYOS   //CK
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef unsigned char   BOOL;

u32 enc_data_my(u32 addr, u32 raw_data);

extern u32 coef0;
extern u32 coef1;
extern u32 coef2;
extern u32 coef3;
#endif

#endif
