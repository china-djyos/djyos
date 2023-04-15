#ifndef __CPU_PERI_RTC_H__
#define __CPU_PERI_RTC_H__
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

// RTC
#ifdef __BIG_ENDIAN
#define rRTCCON    (*(volatile unsigned char *)0x57000043)  //RTC control
#define rTICNT     (*(volatile unsigned char *)0x57000047)  //Tick time count
#define rRTCALM    (*(volatile unsigned char *)0x57000053)  //RTC alarm control
#define rALMSEC    (*(volatile unsigned char *)0x57000057)  //Alarm second
#define rALMMIN    (*(volatile unsigned char *)0x5700005b)  //Alarm minute
#define rALMHOUR   (*(volatile unsigned char *)0x5700005f)  //Alarm Hour
#define rALMDATE   (*(volatile unsigned char *)0x57000063)  //Alarm date   //edited by junon
#define rALMMON    (*(volatile unsigned char *)0x57000067)  //Alarm month
#define rALMYEAR   (*(volatile unsigned char *)0x5700006b)  //Alarm year
#define rRTCRST    (*(volatile unsigned char *)0x5700006f)  //RTC round reset
#define rBCDSEC    (*(volatile unsigned char *)0x57000073)  //BCD second
#define rBCDMIN    (*(volatile unsigned char *)0x57000077)  //BCD minute
#define rBCDHOUR   (*(volatile unsigned char *)0x5700007b)  //BCD hour
#define rBCDDATE   (*(volatile unsigned char *)0x5700007f)  //BCD date  //edited by junon
#define rBCDDAY    (*(volatile unsigned char *)0x57000083)  //BCD day   //edited by junon
#define rBCDMON    (*(volatile unsigned char *)0x57000087)  //BCD month
#define rBCDYEAR   (*(volatile unsigned char *)0x5700008b)  //BCD year

#else //Little Endian
#define rRTCCON    (*(volatile unsigned char *)0x57000040)  //RTC control
#define rTICNT     (*(volatile unsigned char *)0x57000044)  //Tick time count
#define rRTCALM    (*(volatile unsigned char *)0x57000050)  //RTC alarm control
#define rALMSEC    (*(volatile unsigned char *)0x57000054)  //Alarm second
#define rALMMIN    (*(volatile unsigned char *)0x57000058)  //Alarm minute
#define rALMHOUR   (*(volatile unsigned char *)0x5700005c)  //Alarm Hour
#define rALMDATE   (*(volatile unsigned char *)0x57000060)  //Alarm date  // edited by junon
#define rALMMON    (*(volatile unsigned char *)0x57000064)  //Alarm month
#define rALMYEAR   (*(volatile unsigned char *)0x57000068)  //Alarm year
#define rRTCRST    (*(volatile unsigned char *)0x5700006c)  //RTC round reset
#define rBCDSEC    (*(volatile unsigned char *)0x57000070)  //BCD second
#define rBCDMIN    (*(volatile unsigned char *)0x57000074)  //BCD minute
#define rBCDHOUR   (*(volatile unsigned char *)0x57000078)  //BCD hour
#define rBCDDATE   (*(volatile unsigned char *)0x5700007c)  //BCD date  //edited by junon
#define rBCDDAY    (*(volatile unsigned char *)0x57000080)  //BCD day   //edited by junon
#define rBCDMON    (*(volatile unsigned char *)0x57000084)  //BCD month
#define rBCDYEAR   (*(volatile unsigned char *)0x57000088)  //BCD year
#endif  //RTC

#ifdef __cplusplus
}
#endif

#endif // __CPU_PERI_RTC_H__

