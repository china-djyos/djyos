

#ifndef __USB_H__
#define __USB_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "./stm32_usb_host_library/core/inc/usbh_def.h"

typedef struct DeviceUSB{
     USBH_HandleTypeDef *pController;
     struct DeviceUSB *pNext;
}tagDeviceUSB;

typedef struct _HOST_LIST{
    USBH_HandleTypeDef handle;
    struct _HOST_LIST *pNext;
    char *pName;
}tagHost;



s32 USBH_Query(void *pHost);
s32 USBH_QueryByName(char *pName);
s32 USBH_DeviceFromHub(USBH_HandleTypeDef *pHost);
USBH_HandleTypeDef *USBH_GetHandle(char *pName);
char *USBH_GetName(USBH_HandleTypeDef *pHandle);
s32 USBH_SetHostName(USBH_HandleTypeDef *pHandle, char *pName);
USBH_HandleTypeDef *USBH_NewHost(char *pName);
s32 USBH_FreeHost(USBH_HandleTypeDef *pHandle);
void USB_DeviceReset(u8 bHostID, u8 bDevID);
s32 USBH_Suspend(USBH_HandleTypeDef *pHost);
s32 USBH_Resume(USBH_HandleTypeDef *pHost);

extern void USBH_LL_Register(u8 bController); // BSP÷– µœ÷

#ifdef __cplusplus
}
#endif

#endif // __USB_H__
