#ifndef __BOARD__H__
#define __BOARD__H__


#ifdef __cplusplus
extern "C" {
#endif

bool_t Board_UartGpioInit(u8 SerialNo);
bool_t Board_IicGpioInit(u8 I2Cx);
bool_t Board_EthGpioInit(void);
bool_t Board_SpiGpioInit(u8 SPIx);
void Board_UartHalfDuplexSend(u8 SerialNo);
u8   Board_CheckPortIsRS232(u8 SerialNo);
void Board_UartHalfDuplexRecv(u8 SerialNo);
bool_t Board_SpiCsCtrl(u8 SPIPort,u8 cs,u8 level);
static void Board_NORGpioInit (void );
void Board_Init(void);
bool_t LAN8720_RESET(void);

#ifdef __cplusplus
}
#endif
#endif






