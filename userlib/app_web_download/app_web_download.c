#include "mongoose.h"
#include "cJSON.h"
#include "rw_msg_pub.h"
#include "Iboot_info.h"
#include <ntp.h>
#include "project_config.h"
#include <time.h>
#include "app_web_download.h"
//#include "../comm/comm_api.h"
//#ifndef free
//#define free(x) M_Free(x)
//#endif


//struct StDevUpgrade {
//    s8 host[100];
//    unsigned short port;
//};

struct StUpdData {
    s32 status;
    s8 *new_data;
    s32 new_len;
};

//struct StDevUpgrade gDevUpgrade;        //������������ַ


// ============================================================================
// ���ܣ���������ʱִ�еĻص�����ԭ�ͣ����ڴ���������������������
// ������data������������ݵ�ַ��len�����ݳ��ȣ�total���������ļ����������ȣ�timeout����ʱ
// ���أ�len -- ����������ݳ��ȣ�
// ��ע��
// ============================================================================
struct StDlFileData {
    volatile s32 is_break;
    volatile s32 status;    //0=�����У�1=����ɣ�-1=����
    volatile s32 is_firstpkg;
    volatile u32 timemark;
    volatile u32 timeout;
    volatile u32 file_size;
    volatile u32 mark_pos;
    fnSaveApp fun_net_do;
//  s8 url[512];
};

#define CN_STARTING         0
#define CN_DOWNLOADING      1
#define CN_PEER_CLOSE       2
#define CN_COMPLETED        3
#define CN_HTTP_ERROR       4
static void cb_http_download_handler(struct mg_connection *nc, s32 ev, void *ev_data)
{
    (void)ev_data;
//    struct http_message *hm = (struct http_message *) ev_data;
    struct StDlFileData *pUserData = nc->user_data;
    struct mbuf *io = &nc->recv_mbuf;
    s32 ret = 0;
    char *p = NULL;
    switch (ev) {
    case MG_EV_CONNECT:
        {
            pUserData->is_firstpkg = 1;
            pUserData->status = CN_DOWNLOADING;
    #if 0
            s32 opt = 0;
            //        opt = 1460*2;
            //        if(0 != setsockopt(nc->sock, SOL_SOCKET ,SO_SNDBUF,&opt, 4))
            //        {
            //            printf("error: Client:set client sndbuf failed!\r\n");
            //        }

            opt = 6 * 1024;
            if (0 != setsockopt(nc->sock, SOL_SOCKET, SO_RCVBUF, &opt, 4))
            {
                printf("Client: setsockopt failed!\n\r");
            }
    #endif
            if (pUserData->is_break) {
                nc->flags |= MG_F_CLOSE_IMMEDIATELY;
            }
//          pUserData->file_size = 0;
        }
        break;
    case MG_EV_RECV:
        {
            //printf("=== read: %d ===!\r\n", io->len);

            if (pUserData->is_firstpkg == 1)
            {
//              pUserData->file_size = 0;
//              pUserData->mark_pos = 0;
                pUserData->is_firstpkg = 0;
                if (0 == memcmp(io->buf, "HTTP/1.1 200", strlen("HTTP/1.1 200"))||
                    0 == memcmp(io->buf, "HTTP/1.1 206", strlen("HTTP/1.1 206")))
                {

                    printf("info: http download recv header ok!\r\n");
                    if ((p = strstr(io->buf, "\r\n\r\n")))
                    {
                        p = p + 4; //p pto data
                        //memset(&shm, 0, sizeof(struct http_message));
                        //mg_http_parse_headers(io->buf, p, p-io->buf, &shm);
                        //printf("shm->body.len=%d!\r\n", shm.body.len);
                        //pUserData->file_size = shm.body.len;
                        s8 *plen = (s8*)c_strnstr(io->buf, "Content-Length:", io->len);
                        if (!mg_ncasecmp(plen, "Content-Length:", 15))
                        {
                            if (pUserData->file_size == 0)
                                pUserData->file_size = atoi(plen+15);
                            printf("media file size: %d!\r\n", pUserData->file_size);
                        }
                        //ret = play_data(p, io->len - (p - io->buf), 1000);
                        if (pUserData->fun_net_do)
                        {
                            ret = pUserData->fun_net_do((u8 *)p, io->len - (p - io->buf), pUserData->file_size, 1000);
                        }
                        if (pUserData->file_size > 0 && io->len >= (size_t)(p - io->buf))
                        {
                            pUserData->mark_pos += io->len - (p - io->buf);
                        }
                        mbuf_remove(io, io->len);
                        pUserData->timemark = DJY_GetSysTime() / 1000;
                    }
                }
                else
                {
                    pUserData->status = CN_HTTP_ERROR;
                }
            }
            else {
                //ret = play_data(io->buf, io->len, 1000);
                if (pUserData->fun_net_do) {
                    ret = pUserData->fun_net_do((u8*)io->buf, io->len, pUserData->file_size, 1000);
                }
                if (io->len > 0 && ret > 0) {
                    pUserData->mark_pos += io->len;
                    mbuf_remove(io, ret);
                }
                pUserData->timemark = DJY_GetSysTime() / 1000;
            }
            //mbuf_remove(io, io->len);
            if (pUserData->is_break) {
                printf("info: is_break by man, close now!\r\n");
                nc->flags |= MG_F_CLOSE_IMMEDIATELY;
            }
            if (pUserData->file_size > 0 && pUserData->mark_pos >= pUserData->file_size) {
                printf("info: file_size full, close now!\r\n");
                nc->flags |= MG_F_CLOSE_IMMEDIATELY;
                printf("media download done: %d!\r\n", pUserData->mark_pos);
                pUserData->status = CN_COMPLETED;
            }
        }
        break;
    case MG_EV_CLOSE:
        {
            pUserData->status = CN_PEER_CLOSE;
            //printf("===============close socket=================!\r\n");
        }
        break;
    }
}

//���أ�-1��������� -2�����س�ʱ
s32 WebDownload(s8 *host, s32 port, s8 *path,
                 fnSaveApp fSave, s32 timeout_ms)
{
//    struct StAliyunOssMgr *pOssMgr = &gOssMgr;
    struct StDlFileData userData;
    s8 GMT[60] = { 0 };
    u32 timestamp = 0;
    struct mg_connection *nc = 0;
    struct mg_mgr mgr;
    s8 *temp = 0;
    s32 ret = 0;
    u32 time_val = 0;
    s32 timezone;
    s32 resume = 0;
    bool_t finished=false;

//  if (!is_wifi_connected())
    //lst todo���������������ṩ�ж������Ƿ���ͨ�ĺ��� NG_ConnectIsOK

    temp = malloc(1024);
    if (temp == 0)
        return -1;
//    ntp_GetTimeStamp(&timestamp, timeout_ms);
//    //һ���������GTM_TIME����
//    gettimezone(&timezone);
//    timestamp += timezone*3600;
//    strftime(GMT, sizeof(GMT), "%a, %d %b %Y %H:%M:%S GMT", Time_LocalTime(&timestamp));
//  GTM_TIME(timestamp, GMT, sizeof(GMT));
    memset(&userData, 0, sizeof(struct StDlFileData));
    userData.fun_net_do = fSave;        //���ûص�����
    userData.mark_pos = 0;
    userData.file_size = 0;

    while ((userData.status != CN_COMPLETED) && (userData.status != CN_HTTP_ERROR))
    {

        mg_mgr_init(&mgr, NULL);

        memset(temp, 0, 1024);
        sprintf(temp, "%s:%d", host, port);
        printf("temp is %s\r\n",temp);
        nc = mg_connect(&mgr, temp, cb_http_download_handler);
        userData.status = CN_STARTING;
        if (nc == 0)
        {
            mg_mgr_free(&mgr);
            break ;
        }

        nc->user_data = &userData;

        mg_set_protocol_http_websocket(nc);
        userData.timeout = 5000;

        memset(temp, 0, 1024);
        sprintf(temp,"GET %s HTTP/1.1\r\n"
                    "Host: %s\r\n"
                    "User-Agent: Mongoose/6.15\r\n"
                    "Connection: keep-alive\r\n"
                    "Range: bytes=%d-\r\n\r\n", path, host,resume);
        mg_printf(nc, "%s", temp);
        userData.timemark = DJY_GetSysTime()/1000;
        while (1)
        {
            u32 tmark;
            mg_mgr_poll(&mgr, 500);
            if ((userData.status == CN_COMPLETED)||(userData.status == CN_HTTP_ERROR))
            {
                s32 statusbak = userData.status;
                mg_mgr_free(&mgr);
                userData.status = statusbak;
                break;
            }
            else if (userData.status == CN_PEER_CLOSE)
            {
                s32 statusbak = userData.status;
                if (userData.file_size == userData.mark_pos)
                    userData.status = CN_COMPLETED;
                else
                {
                    printf("server close,but file not completed,restart!\r\n");
                    resume = userData.mark_pos;
                }
                mg_mgr_free(&mgr);
                userData.status = statusbak;
                break;
            }
            else
            {
                s32 statusbak = userData.status;
                tmark = userData.timemark;    //�����в��ܺϲ������Ƕ��̲߳���
                time_val = DJY_GetSysTime()/1000 - tmark;
                if (time_val > userData.timeout)
                {
                    printf("down timeout! restart moment\r\n");
                    resume = userData.mark_pos;
                    mg_mgr_free(&mgr);
                    userData.status = statusbak;
                    break;
                }
            }
            DJY_EventDelay(20*1000);
        }
    }

    if(userData.file_size != userData.mark_pos)
    {
        printf("download size error\r\n");
        ret = -1;
    }

FUN_RET:
    if (temp) {
        free(temp);
        temp = 0;
    }
    return ret;
}


static void cb_upgrade_ev_handler(struct mg_connection *nc, s32 ev, void *ev_data) {
    struct http_message *hm = (struct http_message *) ev_data;
    struct StUpdData *pQuestData = (struct StUpdData*)nc->user_data;
    s8 *p = 0;

    switch (ev) {
    case MG_EV_CONNECT:
        if (*(s32 *)ev_data != 0) {
            /*  fprintf(stderr, "connect() failed: %s\n", strerror(*(s32 *)ev_data));*/
            pQuestData->status = -1;
        }
        if (pQuestData->new_data) {
            free(pQuestData->new_data);
        }
        pQuestData->new_data = 0;
        pQuestData->new_len = 0;
        break;
    case MG_EV_HTTP_REPLY:
    {
        printf("hm->resp_code is %d\r\n",hm->resp_code);
        switch (hm->resp_code) {
        case 200:
            nc->flags |= MG_F_CLOSE_IMMEDIATELY;

            if (hm->body.len <= 0) {
                printf("hm->body.len %d\r\n",hm->body.len);
                if (pQuestData->status == 0) pQuestData->status = -1;
                break;
            }

            //process the whole body.
            pQuestData->status = -1;
            p = realloc(pQuestData->new_data, hm->body.len+1);
            if (p == 0) {
                printf("error: cb_ev_handler->realloc MG_EV_HTTP_REPLY, p==null!\r\n");
                break;
            }
            pQuestData->new_data = p;
            pQuestData->new_len = hm->body.len+1;
            memcpy(pQuestData->new_data, hm->body.p, hm->body.len);
            pQuestData->new_data[hm->body.len] = 0;

                pQuestData->status = 1;
            break;
        case 302:
            pQuestData->status = -302;
            break;
        case 401:
            pQuestData->status = -401;
            break;
        default:
            pQuestData->status = -1;
            break;
        }
        break;
    }
#if 1
    case MG_EV_HTTP_CHUNK:
    {
        nc->flags = MG_F_DELETE_CHUNK;
        //hex_dump("cb_http_upload_handler MG_EV_HTTP_CHUNK:", hm->body.p, hm->body.len);
        if (hm->body.len > 0) {
            p = realloc(pQuestData->new_data, pQuestData->new_len + hm->body.len);
            if (p == 0) {
                printf("error: cb_ev_handler->realloc MG_EV_HTTP_CHUNK, p==null!\r\n");
                break;
            }
            pQuestData->new_data = p;
            pQuestData->new_len = hm->body.len+1;
            memcpy(pQuestData->new_data, hm->body.p, hm->body.len);
            pQuestData->new_data[hm->body.len] = 0;
            pQuestData->status = 1;
        }
        else if (hm->body.len == 0) {//end flag
            if (pQuestData->status != 0) break;
            p = realloc(pQuestData->new_data, pQuestData->new_len+1);
            if (p == 0) {
                printf("error: cb_ev_handler->realloc MG_EV_HTTP_CHUNK, p==null!\r\n");
                break;
            }
            pQuestData->new_data = p;
            pQuestData->new_data[pQuestData->new_len] = 0;
            pQuestData->new_len += 1;
            pQuestData->status = 1;
        }
        break;
    }
#endif
    case MG_EV_CLOSE:
        break;
    default:
        break;
    }
}


s32 DevUpgradeCommon(s8 *path, s8 *out_json, s32 len)
{
    struct StUpdData user_data;
    memset(&user_data, 0, sizeof(user_data));
//  struct StDevUpgrade *pDevMgr = &gDevUpgrade;
    s32 ret = 0;
    s8 *meta = 0;
    s8 *url = 0;
    struct mg_connection *nc = 0;
    struct mg_mgr mgr;


    mg_mgr_init(&mgr, NULL);

    meta = (s8*)malloc(1024);
    if (meta == 0) return -1;

    url = (s8*)malloc(1024);
    if (url == 0) return -1;

    user_data.new_len = 0;
    user_data.new_data = 0;
    user_data.status = 0;

    sprintf(meta,
        "Accept: */*\r\n"
        "Connection: keep-alive\r\n"
        "User-Agent: Mongoose/6.15\r\n"
        "Cache-Control: no-cache\r\n"
        "Content-Type: application/json\r\n");

//  sprintf(url, "http://%s%s", pDevMgr->host, path);
    sprintf(url, "http://%s%s", PRODUCT_OTA_ADDRESS, path);
    printf("url is %s\r\n", url);
    nc = mg_connect_http(&mgr, cb_upgrade_ev_handler, url, meta, 0);
    if (nc == 0) goto END_QUEST;
    nc->user_data = &user_data;

    u32 timemark = DJY_GetSysTime()/1000;
    u32 timeout_ms = 20000;   //TODO��ʱʱ��̫����
    while (user_data.status == 0) {
        if (DJY_GetSysTime()/1000-timemark > timeout_ms) {
            printf("DevGetCommon: timeout break!\r\n");
            break;
        }

        mg_mgr_poll(&mgr, 500);
        DJY_EventDelay(10*1000);
    }
    if (user_data.status == 1) {
        if (user_data.new_data)
        {
            s32 min = strlen(user_data.new_data) + 1;
            min = min < len ? min : len;
            memcpy(out_json, user_data.new_data, min);
            user_data.status = min;
        }
        else
        {
            user_data.status = 0;
        }
    }
    ret = user_data.status;
END_QUEST:
    mg_mgr_free(&mgr);
    if (meta) free(meta);
    if (url) free(url);
    if (user_data.new_data) {
        free(user_data.new_data);
        user_data.new_data = 0;
        user_data.new_len = 0;
    }
    return ret;
}

s32 DevUpgradeQuest(const s8 *serial_num, u8 *user, s8 *out_json, s32 len)
{
    s8 path[200] = { 0 };
    s8 VersionNum[13];
    s8 finger[16];
    memset(VersionNum, 0, sizeof(VersionNum));

    sprintf(VersionNum, "%d.%d.%d", PRODUCT_VERSION_LARGE,
                    PRODUCT_VERSION_MEDIUM, PRODUCT_VERSION_SMALL);

    memset(finger,0,sizeof(finger));
    Iboot_GetProductInfo(APP_HEAD_FINGER, finger, sizeof(finger));
    if (user == NULL)
        snprintf(path,sizeof(path),"/api/version?sn=%s&version=%s&finger=%s&apphead=?",
                serial_num, VersionNum, finger );
    else
        snprintf(path,sizeof(path),"/api/version?sn=%s&version=%s&finger=%s&%s&apphead=?",
                serial_num, VersionNum, finger, user );
    s32 ret = DevUpgradeCommon(path, out_json, len);
    return ret;
}

static u8 gserial_num[5];
static u8 gproduct_time[4];
static u8 gupgrade_url[256];
static  char gupgrade_version[20];
char *Get_Upgrade_version(void)
{
    return gupgrade_version;
}
//��ȡ�������·�������ʱ����������
s32 GetSrvUpgradeInfo(u8 *product_time, s32 prod_time_len, u8 *serial_num, s32 ser_num_len)
{
    if (gserial_num[0] == 0 || gproduct_time[0] == 0) return -1;

    if (serial_num && ((s32)sizeof(gserial_num) <= ser_num_len)) {
        memcpy(serial_num, gserial_num, sizeof(gserial_num));
//      *serial_num = gserial_num;
    }
    if (product_time && ((s32)sizeof(gproduct_time) <= prod_time_len)) {
        memcpy(product_time, gproduct_time, sizeof(gproduct_time));
    }
    memset(gserial_num, 0, sizeof(gserial_num));
    memset(gproduct_time, 0, sizeof(gproduct_time));
    return 0;
}

static u8 __hex2bin(u8 c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'F')
        return 10 + (c - 'A');
    if (c >= 'a' && c <= 'f')
        return 10 + (c - 'a');
    else
        return 0;
}

//-----------------------------------------------------------------------------
//���ܣ����apphead��ȷ�������̼��Ƿ�һ�£���Ҫ���ڴ������Ĺ̼��������ξ��ϣ��Լӿ�
//      �������̻��������������̣�ʹ���°汾ֻ�ӷ���������һ�Ρ�
//������head1,head2���������Ƚϵĳ���ͷָ��
//���أ�true = ��У���벢��У����һ�£�false = ûУ�����У���벻һ��
//------------------------------------------------------------------------------
bool_t CheckNew_AppHead(struct AppHead *head1,struct AppHead *head2)
{
    bool_t result = false;
    u8 *nflag,*oflag;
    nflag = head1->djy_flag;
    oflag = head2->djy_flag;

    if ((nflag == NULL) || (oflag==NULL))
        return false;
    //���APPͷ��־
    if((oflag[0] == 'd')&&(oflag[1] == 'j')&&(oflag[2] == 'y')
            &&(nflag[0] == 'd')&&(nflag[1] == 'j')&&(nflag[2] == 'y'))
    {
        if(head2->verification == head1->verification)  //У�鷽����ͬ�ſ��� true
        {
            if(head2->verification == VERIFICATION_CRC)   //CRC����4�ֽڵ�
                result = (memcmp(head2->verif_buf, head1->verif_buf, 4) == 0);
            else if(head2->verification == VERIFICATION_MD5)  //MD5����32�ֽڵ�
                result = (memcmp(head2->verif_buf, head1->verif_buf, 32) == 0);
            else
                result = false;
        }
        else
            result = false;
    }
    else
        result = false;
    return result;
}

__attribute__((weak)) void OtaUserProcess(cJSON *dlparam)
{
}
__attribute__((weak)) bool_t Read_OtaAppHead(u8 *buf)
{
    return false;
}
// ============================================================================
// ���ܣ�����ota���󣬰��豸��Ϣ���͸�����������������������json��������json���������
//      ȫ�ֱ�����ֵ������ֵ�У�binURl�ֶΡ�finger�ֶ��ڱ������д����ˣ����������ֶΣ�
//      ���û��� OtaUserProcess �д���
// ������user:��������û���ӵ��ֶθ�ʽ�����ֶ�1=ֵ1&�ֶ�2=ֵ2��
//      SN:��Ʒ��SN�ţ�ע�Ⲣ��ָ��
// ���أ�0=���¹̼���-1=���¹̼�
// ��ע��
// ============================================================================
extern const struct AppHead Djy_App_Head;
s32 web_check_new_versions(u8 *user_param, u8 *SN)
{
    char buf[6] = {0};
    cJSON *cjson = NULL;
    cJSON*  results = 0;
    s32 ret;
    s8 *out_json = malloc(1024);
    if (out_json==0)
        return CN_NO_DOWNLOAD;
    printf("\r\n SN_Num   = %s,\r\n", SN);
    memset(gupgrade_url, 0, sizeof(gupgrade_url));
    memset(out_json, 0, 1024);
    //�������зǳ���Ҫ��iboot������Щ�ֶ��Ƿ���������Ƿ���²�Ʒ��Ϣ
    Iboot_SetSerial(buf);       //���struct IbootAppInfo��week�ֶ�
    Iboot_SetWeek(buf);         //���struct IbootAppInfo��serial�ֶ�
    //�ӷ�������ȡ�����ļ�����Ϣ����һ��json��
    if (DevUpgradeQuest((const s8 *)SN, user_param, out_json, 1024) <= 0)
    {
        free(out_json);
        return CN_NO_DOWNLOAD;
    }
    //todo��������ʵ���ݰ���д
    cjson = cJSON_Parse(out_json);  //����json��
    OtaUserProcess(cjson);          //weak�������û�������Ӧ�ó���������ʵ��
    if (cjson)
    {
        results = cJSON_GetObjectItem(cjson, "binURL");
        if (results)
        {
            if (strlen(results->valuestring) < 256)
            {
                strcpy((s8*)gupgrade_url, results->valuestring);
                results = cJSON_GetObjectItem(cjson, "apphead");
                if (results)
                {
                    u8 *p = results->valuestring;
                    u8 *newsthead = malloc(sizeof(struct AppHead) * 2);
                    u32 n;
                    for (n = 0; n < 256; n++)
                    {
                        newsthead[n] = (__hex2bin(p[2*n]) << 4) + __hex2bin(p[2*n+1]);
                    }
                    if (CheckNew_AppHead(&Djy_App_Head,newsthead) == true)
                    {
                        printf("firmware is running\r\n");
                        ret = CN_RUNNING_IS_CHECK;  //���������еĹ̼���ͬ
                    }
                    else
                    {
                        if(Read_OtaAppHead((newsthead+sizeof(struct AppHead))) == true)
                        {
                            if(CheckNew_AppHead((newsthead+sizeof(struct AppHead)),newsthead) == true)
                            {
                                ret = CN_READY_IS_CHECK;//�����غõĴ�����ͬ
                            }
                            else
                            {
                                ret = CN_NEAD_DOWNLOAD; //�����غõĺ������ܵĹ̼�����ͬ
                            }
                        }
                        else
                        {
                            ret = CN_NEAD_DOWNLOAD; //��ȡ���غõĹ̼�ʧ�ܣ����ܲ�����
                        }
                    }
                }
                else
                    ret = CN_NEAD_DOWNLOAD; //û��apphead�ֶΣ�����������
            }
            else
                ret = CN_NO_DOWNLOAD;   //��ַ������
        }
        else
            ret = CN_NO_DOWNLOAD;       //û���¹̼�

        results = cJSON_GetObjectItem(cjson, "finger");
        if (results)        //������Ϊ�˴����������ָ��
        {
            //15���ַ����ֱ���6�ֽ��ͺ���+4�ֽ��������+5�ֽ����к�
            //�ͺ����ǳ�ʼ��ʱ�Ѿ����úã������������á�
            Iboot_SetWeek(results->valuestring+6);
            Iboot_SetSerial(results->valuestring+6+4);  //ǰ6���ͺ��룬��4���������
        }

        cJSON_Delete(cjson);
    }

    if (out_json)
        free(out_json);
    return ret;
}

// ============================================================================
// ���ܣ����ع̼������ûص������洢��
// ������fSave����������ִ�еĻص����������ڴ���ͨ�������ȡ�������ݣ��ú�����Ҫ�û��Լ�
//           ʵ�֣�����ԭ�ͣ�int (*fnSaveApp)(u8 *buf,s32 len, int fsize, int timeout);
// ���أ�0���ɹ���-1������ʧ�ܣ�-2�����س�ʱ
// ��ע��
// ============================================================================
s32 web_upgrade_firmware(fnSaveApp fSave)
{
    s32 ret = -1;
    s8 *p1 = 0;
    s8 *p2 = 0;
    s8 domain[100] = { 0 };
    memset(domain, 0, sizeof(domain));
    p1 = 0;
    p2 = 0;
    if (gupgrade_url[0]) {
        p1 = (s8*)gupgrade_url;
        //������,���ӵ�ַ����ȡ���� www.djyos.com �ַ���
        if (strstr((s8*)gupgrade_url, "https://") == (s8*)gupgrade_url)
        {
            p1 = p1 + strlen("https://");
            p2 = strstr(p1, "/");
            if (p1 && p2 && p2>p1)
            {
                memcpy(domain, p1, p2 - p1);
            }
        }
        else if (strstr((s8*)gupgrade_url, "http://") == (s8*)gupgrade_url)
        {
            p1 = (s8*)&gupgrade_url[strlen("http://")];
            p2 = strstr(p1, "/");
            if (p1 && p2 && p2>p1)
            {
                memcpy(domain, p1, p2 - p1);
            }
        }

        if (domain[0] && p2)
        {
            if (fSave == NULL)
            {
                printf("web_up param error\r\n");
                return -1;
            }
            ret = WebDownload(domain, 80, p2, fSave, 600000);

            printf("=================WebDownload ret=%d=================!\r\n", ret);
        }
    }
    return ret;
}
