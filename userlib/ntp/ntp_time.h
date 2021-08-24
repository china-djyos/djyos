#ifndef __NTP_TIME_H__
#define __NTP_TIME_H__

//通过ntp协议获取网络时间戳
s32 ntp_GetTimeStamp(u32 *ptimestamp, s32 timeout_ms);
int GetTimeStamp(unsigned int *timestamp_out, int timeout_ms);//函数被删，用ntp_GetTimeStamp
//函数被删除，代码修改方法参见app_web_download.c中。
char *GTM_TIME(unsigned int timestamp, char *buf, int len);
#endif
