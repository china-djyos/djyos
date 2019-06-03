#ifndef _SCM6XX_7816_H_
#define _SCM6XX_7816_H_

#include "SCM6XX_ALL.h"

//7816时钟 4000K
#define U7816_CLK  4000    
#define U7816_RTRIG 0

//通道定义
#define U7816_CH0       0
#define U7816_CH1       1

//ESAM通路电源控制
#define	ESAMPOWERON  	1//GPIO7 &= B0111_1111
#define	ESAMPOWEROFF 	0//GPIO7 |= B1000_0000


#define KEY_N	 0
#define KEY_P	 1


//U7816通讯缓冲区定义
#define u7816_buf_size	30


/*  -------------------------------------------------------------------------------
 *   API
 *  -------------------------------------------------------------------------------
 */
 
void U7816_Reset_Reg(void);
void U7816_Configure_BaudRate(UINT16 BaudRate);
void U7816_Configure_Data(UINT8 Tmod, UINT8 Diconv, UINT8 Par, UINT8 Txegt);
void U7816_Init(UINT16 BaudRate);
void U7816_Prepare_Rx(UINT8 RxLvl, UINT8 ErrWid, UINT8 Ch);
void U7816_Get_Chars(UINT8 *data, UINT16 len);
UINT8 U7816_Get_Char(UINT8 *data);
void U7816_Prepare_Tx(UINT8 Repeat, UINT8 Ch);
UINT8 U7816_Send_Byte(UINT8 Data);
void U7816_Enable_Int(UINT32 U7816_Int);
void U7816_ClearIntr(UINT32 U7816_Int);



#endif 

