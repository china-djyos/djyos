

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


USBH_HandleTypeDef *USBH_HostHandle(u8 bController);
s32 USBH_DeviceQuery(u8 bIndex, tagDeviceUSB *pHead);
tagDeviceUSB *USBH_NewDevice(tagDeviceUSB *pHead, u8 bController);


#ifdef __cplusplus
}
#endif

#endif // __USB_H__
