#ifndef __NTP_TIME_H__
#define __NTP_TIME_H__

//ͨ��ntpЭ���ȡ����ʱ���
int GetTimeStamp(unsigned int *timestamp_out, int timeout_ms);
char *GTM_TIME(unsigned int timestamp, char *buf, int len);
#endif
