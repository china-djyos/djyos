#include "ntp_time.h"
#include "mongoose.h"

#define u32 unsigned int


struct tm *oss_gmtime(const time_t *time, int time_zone)
{
    static struct  tm result;
    s64 temp_time;
    if(time == NULL)
    {
        temp_time = __Rtc_Time(NULL);
        temp_time += ((s64)8*3600);
    }
    else
    {
        temp_time = *time + ((s64)time_zone*3600);
    }
    return Time_LocalTime_r(&temp_time, &result);
}

char *GTM_TIME(unsigned int timestamp, char *buf, int len)
{
    char GMT[100] = {0};

    time_t t = timestamp;
    strftime(GMT, sizeof(GMT), "%a, %d %b %Y %H:%M:%S GMT", oss_gmtime(&t, 0));
    //mg_strftime_patch(GMT);
    int min = strlen(GMT)+1;
    min = min < len ? min : len;
    memcpy(buf, GMT, min);
    return buf;
}

static unsigned int gNtpTickSecOffset = 0;
static unsigned int gNtpTickSecTimestamp = 0;


char *arr_ntp_server[] = {
        "ntp.ntsc.ac.cn",
        "cn.pool.ntp.org",
        "us.pool.ntp.org",
};
int get_ntp_tiemstamp(u32 *ptimestamp, char *ntp_domain, int timeout_ms);

int ntp_timestamp(u32 *ptimestamp, int timeout_ms)
{
    unsigned int timestamp = 0;
    static int index = 0;
    index++;
    index = index % (sizeof(arr_ntp_server)/sizeof(arr_ntp_server[0]));
    int status = get_ntp_tiemstamp(&timestamp, arr_ntp_server[index], timeout_ms);
    printf("info: get_ntp_tiemstamp, status:%d, index:%d, ntp_server:%s, timestamp=%d!\r\n", status, index, arr_ntp_server[index], timestamp);
    if (status >= 0) {
        *ptimestamp = timestamp;
    }
    return status;
}
int timestamp_http(unsigned int *timestamp, int timeout_ms);
int  GetTimeStamp(unsigned int *timestamp_out, int timeout_ms)
{
    static unsigned int cnts = 0;
    unsigned int cur_tick_sec  = 0;
    unsigned int timestamp = 0;
    int status = 0;
    if (gNtpTickSecOffset == 0 && gNtpTickSecTimestamp == 0) {
        //status = NetGetTimeStamp(&timestamp, timeout_ms);
        cnts++;
        if (cnts%3 == 0) {
            printf("info: timestamp_http get time!\r\n");
            status = timestamp_http(&timestamp, timeout_ms);
        }
        else {
            printf("info: ntp_timestamp get time!\r\n");
            status = ntp_timestamp(&timestamp, timeout_ms);
        }
        if (status >= 0 && timestamp) {
            *timestamp_out = timestamp;
            gNtpTickSecOffset = DJY_GetSysTime()/1000000;
            gNtpTickSecTimestamp = timestamp;
        }
        else {
            return -1;
        }
        printf("gNtpTickSecOffset = %d, gNtpTickSecTimestamp = %d!\r\n", gNtpTickSecOffset, gNtpTickSecTimestamp);
    }
    cur_tick_sec = DJY_GetSysTime()/1000000;
    *timestamp_out  = gNtpTickSecTimestamp + cur_tick_sec - gNtpTickSecOffset;
    //printf("Tick Offset: %d, TimeStamp: %d, RTC TIME: %d!\r\n", gNtpTickSecOffset, gNtpTickSecTimestamp, *timestamp_out);
    return 0;
}


int ClearTimeStamp()
{
    gNtpTickSecOffset = 0;
    gNtpTickSecTimestamp = 0;
    return 0;
}

int GetTimeHourMinute(int *hour, int *min)
{
    unsigned int timestamp_out = 0;
    unsigned int cur_tick_sec  = 0;
    cur_tick_sec = DJY_GetSysTime()/1000000;
    timestamp_out  = gNtpTickSecTimestamp + cur_tick_sec - gNtpTickSecOffset;
    time_t t = timestamp_out;
    struct tm *ptm = oss_gmtime(&t, 8);
    *hour = ptm->tm_hour;
    *min = ptm->tm_min;
    //printf("---->hour=%d, min=%d!\r\n", *hour, *min);
    //strftime(GMT, sizeof(GMT), "%a, %d %b %Y %H:%M:%S GMT", oss_gmtime(&t, 0));
    //printf("GetTimeHourMinute: %d, TimeStamp: %d, RTC TIME: %d!\r\n", gRtcTickSecOffset, gRtcTickSecTimestamp, timestamp_out);
    return 0;
}

