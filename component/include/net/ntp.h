#ifndef __NTP_TIME_H__
#define __NTP_TIME_H__

//ͨ��ntpЭ���ȡ����ʱ���
s32 ntp_GetTimeStamp(u32 *ptimestamp, s32 timeout_ms);
int GetTimeStamp(unsigned int *timestamp_out, int timeout_ms);//������ɾ����ntp_GetTimeStamp
//������ɾ���������޸ķ����μ�app_web_download.c�С�
char *GTM_TIME(unsigned int timestamp, char *buf, int len);
#endif
