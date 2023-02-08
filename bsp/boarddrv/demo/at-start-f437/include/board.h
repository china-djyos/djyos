#ifndef __BOARD__H__
#define __BOARD__H__


#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool_t Board_UartGpioInit(u8 SerialNo);
void Board_Init(void);

#ifdef __cplusplus
}
#endif

#endif
