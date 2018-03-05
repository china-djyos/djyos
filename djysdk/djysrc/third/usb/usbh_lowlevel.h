
#ifndef __USBH_LOWLEVEL_H__
#define __USBH_LOWLEVEL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <lock.h>
#include "./stm32_usb_host_library/core/inc/usbh_def.h"

#define HIGH_SPEED_HOST        1
#define FULL_SPEED_HOST        2

// USB 驱动层 BSP中实现
struct USBH_LowLevel
{
    USBH_StatusTypeDef (*Init) (USBH_HandleTypeDef *phost);
    USBH_StatusTypeDef (*DeInit) (USBH_HandleTypeDef *phost);
    USBH_StatusTypeDef (*Start) (USBH_HandleTypeDef *phost);
    USBH_StatusTypeDef (*Stop) (USBH_HandleTypeDef *phost);
    USBH_SpeedTypeDef (*GetSpeed) (USBH_HandleTypeDef *phost);
    USBH_StatusTypeDef (*ResetPort) (USBH_HandleTypeDef *phost);
    uint32_t (*GetLastXferSize) (USBH_HandleTypeDef *phost, uint8_t pipe);
    USBH_StatusTypeDef (*OpenPipe) (USBH_HandleTypeDef *phost, 
                        uint8_t pipe_num, uint8_t epnum, uint8_t dev_address,
                        uint8_t speed, uint8_t ep_type, uint16_t mps);
    USBH_StatusTypeDef (*ClosePipe) (USBH_HandleTypeDef *phost, uint8_t pipe);
    USBH_StatusTypeDef (*SubmitURB) (USBH_HandleTypeDef *phost, uint8_t pipe, 
                        uint8_t direction, uint8_t ep_type, uint8_t token, 
                        uint8_t* pbuff, uint16_t length, uint8_t do_ping);
    USBH_URBStateTypeDef (*GetURBState) (USBH_HandleTypeDef *phost, uint8_t pipe);
    USBH_StatusTypeDef (*DriverVBUS) (USBH_HandleTypeDef *phost, 
                        uint8_t state);
    USBH_StatusTypeDef (*SetToggle) (USBH_HandleTypeDef *phost, 
                        uint8_t pipe, uint8_t toggle);
    uint8_t (*GetToggle) (USBH_HandleTypeDef *phost, uint8_t pipe);
    USBH_StatusTypeDef (*EnAsync) (USBH_HandleTypeDef *phost, uint8_t pipe);
    USBH_StatusTypeDef (*DisAsync) (USBH_HandleTypeDef *phost, uint8_t pipe);
    USBH_StatusTypeDef (*Suspend) (USBH_HandleTypeDef *phost);
    USBH_StatusTypeDef (*Resume) (USBH_HandleTypeDef *phost);
    uint8_t bID;
    void *pMutex; // 驱动互斥
};

struct USBH_LowLlevel *USBH_LL_GetLL(u8 USBH_Type);
USBH_StatusTypeDef USBH_LL_EnAsync (USBH_HandleTypeDef *phost, uint8_t pipe);
USBH_StatusTypeDef USBH_LL_DisAsync (USBH_HandleTypeDef *phost, uint8_t pipe);
USBH_StatusTypeDef USBH_LL_Suspend (USBH_HandleTypeDef *pHost);
USBH_StatusTypeDef USBH_LL_Resume (USBH_HandleTypeDef *pHost);
#ifdef __cplusplus
}
#endif

#endif
