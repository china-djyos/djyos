#ifndef _SCM6XX_UART_H_
#define _SCM6XX_UART_H_

#include "SCM6XX_ALL.h"

#define PARITY_NONE         0
#define PARITY_ODD           (UART_CTRL_PARITY_EN | UART_CTRL_PARITY_ODD)
#define PARITY_EVEN          (UART_CTRL_PARITY_EN | UART_CTRL_PARITY_EVEN)

#define UART_CLK 37500000UL
#define DEFAULT_BARDRATE   2400
   

#define BACKSP_KEY 0x08
#define RETURN_KEY 0x0D
#define DELETE_KEY 0x7F
#define ESC_KEY 0x1B

extern uint8_t UART_DataReady[3];
extern uint8_t UART_ThrEmpty[3] ;
extern UINT8 UART_DataReady[3];
extern UINT8 UART_ThrEmpty[3] ;


struct COMMUNICATION_FRM
{
    uint8_t Buf[30];          				//接收缓冲/发送缓冲
    uint8_t Len;                  		//接收长度字节/发生长度
    uint8_t Total_Len;                //接收总长度/发送总长度
    uint8_t Step;                     //接收步骤
};


/*  -------------------------------------------------------------------------------
 *   API
 *  -------------------------------------------------------------------------------
 */
void UART_Reset_Reg(SGCC_UART_TypeDef* UARTx);
void UART_Init(SGCC_UART_TypeDef* UARTx, UINT32 Baudrate, UINT32 Parity, UINT32 StopNum);
void UART_Enable_Int(SGCC_UART_TypeDef* UARTx,UINT32 IntMask);
void UART_Clear_Int_Flag(SGCC_UART_TypeDef* UARTx,UINT32 IntType);
UINT8 UART_Get_Char(SGCC_UART_TypeDef* UARTx);
void UART_Put_Char(SGCC_UART_TypeDef* UARTx, UINT8 ch);
UINT8 UART_Get_Flag_Status(SGCC_UART_TypeDef* UARTx, uint16_t UART_Flag);
void UART_Put_Str(SGCC_UART_TypeDef* UARTx, UINT8 *Str);
unsigned int UART_Get_Str(SGCC_UART_TypeDef* UARTx, UINT8 *buf);
INT32 UART_Printf(SGCC_UART_TypeDef* UARTx, const char *f, ...);


//extern void 		UART_Init (SGCC_UART_TypeDef* UARTx, uint32_t baudrate, uint32_t parity, uint32_t stopNum);
extern void 		UART_EnableIntr(SGCC_UART_TypeDef* UARTx,uint32_t intMask);
extern void 		UART_ClearIntr(SGCC_UART_TypeDef* UARTx,uint32_t intType);
extern uint8_t 	UART_GetChar(SGCC_UART_TypeDef* UARTx);
extern void 		UART_PutChar(SGCC_UART_TypeDef* UARTx, uint8_t ch);
extern uint8_t 	UART_GetFlagStatus(SGCC_UART_TypeDef* UARTx, uint16_t flag);
extern void 		UART_PutStr(SGCC_UART_TypeDef* UARTx, uint8_t *Str);
extern int 			UART_GetStr(SGCC_UART_TypeDef* UARTx, uint8_t *buf);
//extern int32_t 	UART_Printf(SGCC_UART_TypeDef* UARTx, const char *f, ...)  ;/* variable arguments */
#endif 



