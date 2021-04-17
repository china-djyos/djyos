
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include "dbug.h"
#include <shell.h>

#include "../../component_config_tcpip.h"

/** This is an example of a "SNTP" client (with socket API).
 *
 * For a list of some public NTP servers, see this link :
 * http://support.ntp.org/bin/view/Servers/NTPPoolServers
 *
 */

/**
 * SNTP_DEBUG: Enable debugging for SNTP.
 */
#ifndef SNTP_DEBUG
#define SNTP_DEBUG                  1
#endif

/** SNTP server port */
#ifndef SNTP_PORT
#define SNTP_PORT                   123
#endif


/** SNTP receive timeout - in milliseconds */
#ifndef SNTP_RECV_TIMEOUT
#define SNTP_RECV_TIMEOUT           (3000*mS)
#endif

/** SNTP update delay - in milliseconds */
#ifndef SNTP_UPDATE_DELAY
#define SNTP_UPDATE_DELAY           (10*1000*mS)
#endif

/** SNTP macro to change system time and/or the update the RTC clock */
#ifndef SNTP_SYSTEM_TIME
#define SNTP_SYSTEM_TIME(t)
#endif

/* SNTP protocol defines */
#define SNTP_MAX_DATA_LEN           48
#define SNTP_RCV_TIME_OFS           32
#define SNTP_LI_NO_WARNING          0x00
#define SNTP_VERSION               (4/* NTP Version 4*/<<3)
#define SNTP_MODE_CLIENT            0x03
#define SNTP_MODE_SERVER            0x04
#define SNTP_MODE_BROADCAST         0x05
#define SNTP_MODE_MASK              0x07

/* number of seconds between 1900 and 1970 */
#define DIFF_SEC_1900_1970         (2208988800)


#define PRINT_SNTPDEBUG(x)            printf

static int __SNTP_DealTime(time_t time)
{

    time_t   time_now;
    int      timezone;
    struct tm  ostime;
    struct timeval time_timeval;
    char *asct;
    char *ct;

    gettimezone(&timezone);
    time_now = time + timezone*3600;

    time_timeval.tv_sec = time_now;
    time_timeval.tv_usec = 0;
    settimeofday(&time_timeval,NULL);

    localtime_r(&time_now,&ostime);

    asct = asctime(&ostime);
    ct = ctime(&time_now);

    debug_printf("sntp","%s:ASCTIME:%s\n\r",__FUNCTION__,asct);
    debug_printf("sntp","%s:CTIME:%s\n\r",__FUNCTION__,ct);

    return 0;
}


/**
 * SNTP processing
 */
static void __SNTP_Process( time_t t)
{
    /* change system time and/or the update the RTC clock */
    __SNTP_DealTime(t);
}

/**
 * SNTP request
 */
static void __SNTP_Request(char *SntpServerAddress)
{
    int                sock;
    struct sockaddr_in to;
    int                tolen;
    int                size;
    int                timeout;
    u8               __SntpBuffer [SNTP_MAX_DATA_LEN];
    u8               sntp_response[SNTP_MAX_DATA_LEN];
    u32              sntp_server_address;
    u32              timestamp;
    time_t           t;

    /* initialize SNTP server address */
    sntp_server_address = inet_addr(SntpServerAddress);

    /* if we got a valid SNTP server address... */
    if (sntp_server_address!=0)
    {
        /* create new socket */
        sock = socket( AF_INET, SOCK_DGRAM, 0);
        if (sock>=0)
        {

            /* set recv timeout */
            timeout = SNTP_RECV_TIMEOUT;
            setsockopt( sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));

            /* prepare SNTP request */
            memset( __SntpBuffer, 0, sizeof(__SntpBuffer));
            __SntpBuffer[0] = SNTP_LI_NO_WARNING | SNTP_VERSION | SNTP_MODE_CLIENT;

            /* prepare SNTP server address */
            memset(&to, 0, sizeof(to));
            to.sin_family      = AF_INET;
            to.sin_port        = htons(SNTP_PORT);
            to.sin_addr.s_addr = sntp_server_address;

            /* send SNTP request to server */
            if (sendto( sock, __SntpBuffer, sizeof(__SntpBuffer), 0, (struct sockaddr *)&to, sizeof(to))>=0)
            {
                /* receive SNTP server response */
                tolen = sizeof(to);
                size  = recvfrom( sock, sntp_response, sizeof(sntp_response), 0, (struct sockaddr *)&to, (socklen_t *)&tolen);

                /* if the response size is good */
                if (size == SNTP_MAX_DATA_LEN)
                {
                    /* if this is a SNTP response... */
                    if (((sntp_response[0] & SNTP_MODE_MASK) == SNTP_MODE_SERVER) || ((sntp_response[0] & SNTP_MODE_MASK) == SNTP_MODE_BROADCAST))
                    {
                        /* extract GMT time from response */
                        memcpy( &timestamp, (sntp_response+SNTP_RCV_TIME_OFS), sizeof(timestamp));
                        t = (ntohl(timestamp) - DIFF_SEC_1900_1970);

                        /* do time processing */
                        __SNTP_Process(t);
                    }
                }
            }


            /* close the socket */
            closesocket(sock);
        }
    }
}

//bool_t SntpTimeSyncShell(char *param)
bool_t sntp(char *param)
{
    __SNTP_Request(param);
    return true;
}


//THIS IS SNTP MODULE FUNCTION
bool_t SNTP_ServiceSNTP_Init(void)
{
        return (TRUE);
}
ADD_TO_ROUTINE_SHELL(sntp,sntp,"usage:sntp ipaddress");


