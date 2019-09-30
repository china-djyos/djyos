#ifndef __CPU_PERI_WIFI_FAST_CONNECT_H__
#define __CPU_PERI_WIFI_FAST_CONNECT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

#define FLASH_FAST_DATA_ADDR        (0x300000)

struct wlan_fast_connect
{
    unsigned char ssid[32];
    unsigned char bssid[6];
    unsigned char channel;
    unsigned char security;
    unsigned char psk[32];
};
typedef struct wlan_fast_connect wlan_fast_connect_t;

#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_WIFI_FAST_CONNECT_H__
