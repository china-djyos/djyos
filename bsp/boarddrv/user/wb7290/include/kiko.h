/*
 * key.h
 *
 *  Created on: 2016Äê3ÔÂ14ÈÕ
 *      Author: zhangqf
 */

#ifndef _BOARDDRV_INCLUDE_KIKO_H_
#define _BOARDDRV_INCLUDE_KIKO_H_

void KO_Init(void);
//void KO_Write(u32 dwKO);
void KO_Write(u32 *p);
void KO_StartRly(u8 flag);
void CpuLedCtrl(u8 flag,u8 Num);
void KI_Init(void);
void KI_Read(u8 *buf);
u8 	GPS_Read(void);
u8   ZZYC_Read(void);
u8   QDJ1_Read(void);


#endif
