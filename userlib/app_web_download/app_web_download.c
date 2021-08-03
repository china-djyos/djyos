#include "mongoose.h"
#include "cJSON.h"
#include "rw_msg_pub.h"
#include "Iboot_info.h"
#include "ntp/ntp_time.h"
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

//struct StDevUpgrade gDevUpgrade;        //升级服务器地址


// ============================================================================
// 功能：空中升级时执行的回调函数原型，用于处理网络下载下来的数据
// 参数：data：待处理的数据地址，len：数据长度，total：待升级文件的完整长度，timeout：超时
// 返回：len -- 处理完的数据长度；
// 备注：
// ============================================================================
struct StDlFileData {
    s32 media_type;//0: mp3, 1: wav
    volatile s32 is_break;
    volatile s32 status;
    volatile s32 is_start;
    volatile u32 timemark;
    volatile u32 timeout;
    volatile u32 body_size;
    volatile u32 mark_pos;
    fnTransAppToIboot fun_net_do;
    s8 url[512];
};


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
            pUserData->is_start = 1;
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
            pUserData->body_size = 0;
            pUserData->mark_pos = 0;
        }
        break;
    case MG_EV_RECV:
        {
            //printf("=== read: %d ===!\r\n", io->len);

            if (pUserData->is_start == 1) {
                pUserData->body_size = 0;
                pUserData->mark_pos = 0;
                pUserData->is_start = 0;
                if (0 == memcmp(io->buf, "HTTP/1.1 200", strlen("HTTP/1.1 200"))||
                    0 == memcmp(io->buf, "HTTP/1.1 206", strlen("HTTP/1.1 206"))) {

                    printf("info: http download recv header ok!\r\n");
                    if ((p = strstr(io->buf, "\r\n\r\n"))) {
                        p = p + 4; //p pto data
                        //memset(&shm, 0, sizeof(struct http_message));
                        //mg_http_parse_headers(io->buf, p, p-io->buf, &shm);
                        //printf("shm->body.len=%d!\r\n", shm.body.len);
                        //pUserData->body_size = shm.body.len;
                        s8 *plen = (s8*)c_strnstr(io->buf, "Content-Length:", io->len);
                        if (!mg_ncasecmp(plen, "Content-Length:", 15)) {
                            pUserData->body_size = atoi(plen+15);
                            printf("media body size: %d!\r\n", pUserData->body_size);
                        }
                        //ret = play_data(p, io->len - (p - io->buf), 1000);
                        if (pUserData->fun_net_do)
                        {
                            ret = pUserData->fun_net_do((u8 *)p, io->len - (p - io->buf), pUserData->body_size, 1000);
                        }
                        if (pUserData->body_size > 0 && io->len >= (size_t)(p - io->buf))
                        {
                            pUserData->mark_pos += io->len - (p - io->buf);
                        }
                        mbuf_remove(io, io->len);
                        pUserData->timemark = DJY_GetSysTime() / 1000;
                    }
                    pUserData->status = 0;
                }
                else {
                    pUserData->status = -1;
                }
            }
            else {
                //ret = play_data(io->buf, io->len, 1000);
                if (pUserData->fun_net_do) {
                    ret = pUserData->fun_net_do((u8*)io->buf, io->len, pUserData->body_size, 1000);
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
            if (pUserData->body_size > 0 && pUserData->mark_pos >= pUserData->body_size) {
                printf("info: body_size full, close now!\r\n");
                nc->flags |= MG_F_CLOSE_IMMEDIATELY;
                printf("media download done: %d!\r\n", pUserData->mark_pos);
            }
        }
        break;
    case MG_EV_CLOSE:
        {
            pUserData->status = 1;
            pUserData->body_size = 0;
            pUserData->mark_pos = 0;
            //printf("===============close socket=================!\r\n");
            if (pUserData->media_type==1) {//wav
//                WavDataEndPlay (); // TODO merlin 20191214
            }
            else if (pUserData->media_type==0) { //mp3
//                Mp3DataEndPlay (); // TODO merlin 20191214
            }
        }
        break;
    }
}

//返回：-1：下载时候 -2：下载超时
s32 WebDownload(s8 *host, s32 port, s8 *path,
                 fnTransAppToIboot fdo, s32 timeout_ms)
{
//    struct StAliyunOssMgr *pOssMgr = &gOssMgr;
    struct StDlFileData userData;
    struct StDlFileData *pUserData = 0;
    s8 GMT[60] = { 0 };
    u32 timestamp = 0;
    struct mg_connection *nc = 0;
    struct mg_mgr mgr;
    s8 *temp = 0;
    s32 ret = 0;
    u32 time_val = 0;

//  printf ("WebDownload: %s!\r\n", path);

//  if (!is_wifi_connected())
    //lst todo：由网络管理组件提供判断网络是否连通的函数 NG_ConnectIsOK
    if(mhdr_get_station_status() != RW_EVT_STA_GOT_IP)
    {
        pUserData->url[0] = 0;      //网络断开
        return -1;
    }

    temp = malloc(1024);
    if (temp == 0) goto FUN_RET;
    memset(temp, 0, 1024);

    memset(&userData, 0, sizeof(struct StDlFileData));
    pUserData = &userData;
    pUserData->fun_net_do = fdo;        //设置回调函数

    GetTimeStamp(&timestamp, timeout_ms);
    GTM_TIME(timestamp, GMT, sizeof(GMT));

    mg_mgr_init(&mgr, NULL);

    memset(temp, 0, 1024);
    sprintf(temp, "%s:%d", host, port);
    printf("temp is %s\r\n",temp);
    nc = mg_connect(&mgr, temp, cb_http_download_handler);
    pUserData->status = 0;
    if (nc == 0)  goto MGR_FREE;

    nc->user_data = pUserData;

    mg_set_protocol_http_websocket(nc);

    memset(temp, 0, 1024);
    sprintf(temp,
        "GET %s HTTP/1.1\r\n"
        "Host: %s\r\n"
        "User-Agent: Mongoose/6.15\r\n"
        "Accept : */*\r\n"
        "Connection: keep-alive\r\n"
        "date: %s\r\n\r\n", path, host, GMT);
    mg_printf(nc, "%s", temp);
    pUserData->timemark = DJY_GetSysTime()/1000;
    pUserData->timeout = 20000;
    while (pUserData->status == 0)
    {
        mg_mgr_poll(&mgr, 500);
        time_val = DJY_GetSysTime()/1000 - pUserData->timemark;
        if (time_val > pUserData->timeout) {
            printf("==info: WebDownloadAndPlay break!==\r\n");
            break;
        }
//      if (media_is_stop()) {
//          pUserData->timemark = DJY_GetSysTime()/1000;
//      }
        DJY_EventDelay(10*1000);
    }

    if (time_val <= pUserData->timeout)
    {
        ret = -2;
    }

MGR_FREE:
    mg_mgr_free(&mgr);
    pUserData->url[0] = 0;

FUN_RET:
    if (temp) {
        free(temp);
        temp = 0;
    }
    return ret;
}


s32 DoUpgradeJson(s8 *body, s32 len, s8 *outbuf, s32 outlen)
{
    s32 ret = 0;
    cJSON *cjson = 0;
    s8 *pJsonStr = 0;
    s8 *pnew = 0;
    if (body == 0 || len <= 0) return -1;
    pnew = malloc(len + 1);
    if (pnew == 0) return -1;
    memcpy(pnew, body, len);
    pnew[len] = 0;
    cJSON*  sub_item = 0;

    cjson = cJSON_Parse(body);
    if (cjson)
    {
        cJSON*  results = cJSON_GetObjectItem(cjson, "version");
        if (results)
        {
            sub_item = cJSON_GetObjectItem(results, "url");
            if (sub_item)
            {
                pJsonStr = cJSON_PrintUnformatted(cjson);
                ret = strlen(pJsonStr) + 1;
                if (outbuf == 0 || outlen < ret) {
                    ret = -2;
                    goto END_FUN;
                }
                strcpy(outbuf, pJsonStr);
                ret = 1;
            }
        }
    }

END_FUN:
    if (cjson) cJSON_Delete(cjson);
    if (pnew) free(pnew);
    if (pJsonStr) free(pJsonStr);
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

            if (DoUpgradeJson(pQuestData->new_data, pQuestData->new_len, pQuestData->new_data, pQuestData->new_len) > 0) {
                printf("pQuestData->new_data is %s\r\n", pQuestData->new_data);
                pQuestData->status = 1;
            }
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
//            pQuestData->new_data = p;
//            memcpy(&pQuestData->new_data[pQuestData->new_len], hm->body.p, hm->body.len);
//            pQuestData->new_len += hm->body.len;
            pQuestData->new_data = p;
            pQuestData->new_len = hm->body.len+1;
            memcpy(pQuestData->new_data, hm->body.p, hm->body.len);
            pQuestData->new_data[hm->body.len] = 0;

            if (DoUpgradeJson(pQuestData->new_data, pQuestData->new_len, pQuestData->new_data, pQuestData->new_len) > 0) {
                printf("pQuestData->new_data is %s\r\n", pQuestData->new_data);
                pQuestData->status = 1;
            }
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

            //process the whole body.
            pQuestData->status = -1;

            if (DoUpgradeJson(pQuestData->new_data, pQuestData->new_len, pQuestData->new_data, pQuestData->new_len) > 0) {
                printf("%s\r\n", pQuestData->new_data);
                pQuestData->status = 1;
            }
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

//static bool_t update_timeout=false;       //这个标志是没用的吧

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
    u32 timeout_ms = 20000;   //TODO超时时间太短了
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

s32 DevUpgradeQuest(const s8 *serial_num, u8 *branch, s8 *out_json, s32 len)
{
    s8 path[200] = { 0 };
    s8 VersionNum[13];
    s8 finger[16];
    memset(VersionNum, 0, sizeof(VersionNum));

    sprintf(VersionNum, "%d.%d.%d", PRODUCT_VERSION_LARGE,
                    PRODUCT_VERSION_MEDIUM, PRODUCT_VERSION_SMALL);
    printf("--------send VersionNumber = %s   ",VersionNum);

//    printf("ENTRY: %s!\r\n", __FUNCTION__);
//    if(mhdr_get_station_status() != RW_EVT_STA_GOT_IP)    //这是7251的函数，平台相关了
//        return -1;

    memset(finger,0,sizeof(finger));
    Iboot_GetAPP_ProductInfo(APP_HEAD_FINGER, finger, sizeof(finger));
//    sprintf(path, "/api/version?sn=%s&branch=%s&version=%s&fingerprint%s&data=%s",
//                                serial_num, branch, VersionNum, "QMNVLX20470000M", "NULL" );  //测试用的
    snprintf(path,sizeof(path),"/api/version?sn=%s&branch=%s&version=%s&finger=%s&data=%s", serial_num, branch, VersionNum, finger, "NULL" );

    s32 ret = DevUpgradeCommon(path, out_json, len);
    printf("====EXIT:ret is %d  %s!\r\n",ret,__FUNCTION__);
    return ret;
}

static u8 gserial_num[5];
static u8 gproduct_time[4];
static u8 gupgrade_url[256];
//获取服务器下发的生产时间和生产序号
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

// ============================================================================
// 功能：检查是否有新版本，把设备信息发送给服务器，服务器返回升级json，内含 bin 文件
//      的 URL。解析URL赋值给相关全局变量。
// 参数：branch:服务器上的升级分支，
//      SN:产品的SN号
// 返回：返回0表示有新版本，为-1则表示没有新版本,
// 备注：
// ============================================================================
s32 web_check_new_versions(u8 *branch, u8 *SN)
{
    memset(gupgrade_url, 0, sizeof(gupgrade_url));
    s32 ret = -1;
    s8 buf[30] = {0};
    cJSON *cjson = 0;
    cJSON *cjson_version = 0;
    cJSON *cjson_device = 0;
    s32 weeks = 0;
//    s8 VersionNum[13];
    cJSON*  results = 0;
    s8 *out_json = malloc(1024);
    if (out_json==0) return -1;
    printf("\r\n SN_Num   = %s,\r\n", SN);
    memset(out_json, 0, 1024);
    //从服务器拉取升级文件的信息
    if (DevUpgradeQuest((const s8 *)SN, branch, out_json, 1024) <= 0)
    {
        free(out_json);
        return ret;
    }
    //todo：根据真实数据包改写
    cjson = cJSON_Parse(out_json);
    if (cjson)
    {
        cjson_version = cJSON_GetObjectItem(cjson, "version");
        if (cjson_version) {
            results = cJSON_GetObjectItem(cjson_version, "url");
            if (results) {
                printf("==url==: %s!\r\n", results->valuestring);
                strcpy((s8*)gupgrade_url, results->valuestring);
                ret = 1;
            }
        }
        //device
        cjson_device = cJSON_GetObjectItem(cjson, "device");
        if (cjson_device)
        {
            results = cJSON_GetObjectItem(cjson_device, "weekday");
            if (results)
            {
                printf("==weekday==: %d!\r\n", results->valueint);
                weeks = results->valueint;
            }
            results = cJSON_GetObjectItem(cjson_device, "productNo");
            if (results)
            {
                printf("==productNo==: %s!\r\n", results->valuestring);
                memcpy(gserial_num, results->valuestring, sizeof(gserial_num));
            }
            results = cJSON_GetObjectItem(cjson_device, "produceTime");
            if (results)
            {
                printf("==produceTime==: %s!\r\n", results->valuestring);
                struct tm stm = { 0 };
                s32 cnt = sscanf(results->valuestring, "%d-%d-%d %d:%d:%d", &stm.tm_year, &stm.tm_mon, &stm.tm_mday, &stm.tm_hour, &stm.tm_min, &stm.tm_sec);
                if (cnt == 6)
                {
                    memset(buf, 0, sizeof(buf));
                    //sprintf(buf, "%04d%02d", stm.tm_year, weeks);
                    sprintf(buf, "%02d%02d", stm.tm_year%100, weeks);
                    printf("===>PRODUCT TIME: %s!\r\n", buf);
                    memcpy(gproduct_time, buf, sizeof(gproduct_time));
                    ret = 1;
                }
            }
        }
        cJSON_Delete(cjson);
    }

    if (out_json) free(out_json);
    if (gupgrade_url[0] != 0)
    {
        return 0;       //有新版本。
    }
    else
        return -1;
}

// ============================================================================
// 功能：空中升级
// 参数：fdo：空中升级执行的回调函数，用于处理通过网络获取到的数据，该函数需要用户自己实现，函数格式：
// 返回：0：成功。-1：下载失败，-2：下载超时
// 备注：
// ============================================================================

//lst todo : 升级分三步走，1、检查有没有新版本（download组件）；2、下载数据（download组件）
//          3、升级（app update组件）
s32 web_upgrade_firmware(fnTransAppToIboot fdo)
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
        //找域名
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
            if (fdo == NULL)
            {
                printf("web_up param error\r\n");
                return -1;
            }
            ret = WebDownload(domain, 80, p2, fdo, 5000);

            printf("===================WebDownload ret=%d========================!\r\n", ret);
        }
    }
    return ret;
}
