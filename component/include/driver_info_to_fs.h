#ifndef __DRIVER_INFO_TO_FLASH_H__
#define __DRIVER_INFO_TO_FLASH_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

bool_t SetNameValueFS(char *name, char *val, int len);
bool_t GetNameValueFS(char *name, char *val, int len);
bool_t RmNameValueFS(char *name);

#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_WIFI_FAST_CONNECT_H__