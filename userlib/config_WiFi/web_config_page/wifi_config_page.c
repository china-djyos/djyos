#include "mongoose.h"
#include "rw_pub.h"

/* http server的端口号，http 80重定向到真正的http web服务器8080端口 */
static const s8 *s_http_port = "8080";
//static struct mg_serve_http_opts s_http_server_opts;

#define CONF_SERVER_IP "192.168.0.253"

const static s8 web_netc[] = {
#include "web_netc.dat"
};

#include "cJSON.h"

volatile s32 exit_flag = 0;
static s8 ap_name[100];
static s8 ap_passwd[100];

/* 这个是返回扫码到所有热点信息的jason格式 */
#define AP_LIST "{\"AP\":[],\"APLEN\":\"0\",\"CS\":\"0\",\"MAC\":\"AAAA0007959D\",\"TYPE\":\"WS01\",\"PASS\":\"0477B68BFEEB375018FBBEE888D23002\"}"

typedef struct StJsonObj {
    s32 init_flag;
    s8 *pnew;
    s32 counter;
}StJsonObj;
struct StJsonObj gJsonObj;

/*
 * json的初始化，目的是wifi热点扫码后把所有热点信息打包成json返回给http quest
 * 这里先把初始化的字符串生产json树，然后再字符串序列号成字符串。可以直接copy字符串。
 */
static s32 JsonObjInit()
{
    cJSON *root = 0;
    s8 *pJsonStr = 0;
    struct StJsonObj *pJsonObj = &gJsonObj;

    if (pJsonObj->init_flag == 0) {
        atom_low_t atom_low = Int_LowAtomStart( );
        root = cJSON_Parse(AP_LIST);
        pJsonStr = cJSON_PrintUnformatted(root);
        pJsonObj->pnew = realloc(pJsonObj->pnew, strlen(pJsonStr) + 1);
        if (pJsonObj->pnew && pJsonStr) {
            strcpy(pJsonObj->pnew, pJsonStr);
            printf("%s\r\n", pJsonObj->pnew);
        }
        if (root) cJSON_Delete(root);
        if (pJsonStr) free(pJsonStr);
        pJsonObj->init_flag = 1;
        Int_LowAtomEnd(atom_low);
    }

    return 0;
}

/*
 * json卸载
 */
s32 JsonObjDeInit()
{
    struct StJsonObj *pJsonObj = &gJsonObj;
    atom_low_t atom_low = Int_LowAtomStart( );
    pJsonObj->init_flag = 0;
    if (pJsonObj->pnew) {
        free(pJsonObj->pnew);
        pJsonObj->pnew = 0;
    }
    Int_LowAtomEnd(atom_low);
    return 0;
}

/* 统计ap热点个数， 返回json 热点集信息时需要用到这个热点个数 */
s32 ap_list_counter()
{
    s32 counter = 0;
    struct StJsonObj *pJsonObj = &gJsonObj;
    atom_low_t atom_low = Int_LowAtomStart( );
    counter = pJsonObj->counter;
    Int_LowAtomEnd(atom_low);
    return counter;
}

/* 添加新ap热点信息 */
s32 ap_list_add(s8 *ssid)
{
    s32 i = 0;
    struct StJsonObj *pJsonObj = &gJsonObj;
    s32 arr_size = 0;
    cJSON *root = 0;
    cJSON *temp = 0;
    s8 *pJsonStr = 0;
    s8 str_dig[20] = "";

    JsonObjInit();

    atom_low_t atom_low = Int_LowAtomStart( );

    root = cJSON_Parse(pJsonObj->pnew);
    if (root) {
        cJSON*  arr = cJSON_GetObjectItem(root, "AP");
        if (arr) {
            arr_size = cJSON_GetArraySize(arr);
            //printf("arr_size=%d!\r\n", arr_size);
            for (i = 0; i < arr_size; i++) {
                temp = cJSON_GetArrayItem(arr, i);
                if (temp) temp = cJSON_GetObjectItem(temp, "AP");
                if (temp && strcmp(ssid, temp->valuestring) == 0) {
                    break;
                }
            }
            if (i >= arr_size) {
                temp = cJSON_CreateObject();
                if (temp) {
                    cJSON_AddItemToObject(temp, "AP", cJSON_CreateString(ssid));
                    cJSON_AddItemToArray(arr, temp);
                    //cJSON_AddItemToObject(temp, "APLEN", cJSON_CreateString(str_dig));
                    arr_size++;
                    pJsonObj->counter = arr_size;
                    sprintf(str_dig, "%d", arr_size);
                    cJSON_ReplaceItemInObject(root, "APLEN", cJSON_CreateString(str_dig));
                }
            }
        }
    }
    pJsonStr = cJSON_PrintUnformatted(root);

    pJsonObj->pnew = realloc(pJsonObj->pnew, strlen(pJsonStr) + 1);
    if (pJsonObj->pnew && pJsonStr) {
        strcpy(pJsonObj->pnew, pJsonStr);
        //printf("%s\r\n", pJsonObj->pnew);
    }

    if (root) cJSON_Delete(root);
    if (pJsonStr) free(pJsonStr);

    Int_LowAtomEnd(atom_low);

    return 0;
}

/*
 * 下面是过滤一些客户端，不过目前没使用。
 */
typedef struct StClientLog{
    s32 fd;
    u32 client_ip;
    u16 client_port;
    u32 timestampe;
}StClientLog;
struct StClientLog gArrClientLog[50];
s32 keep_once_ack_from_client(s32 fd)
{
    s32 i = 0;
    s32 ret = 0;
    u32 id_addr = 0;
    u16 port = 0;
    struct sockaddr_in sa_temp;
    s32 index_free = -1;
    if (fd<0) return -1;

    socklen_t slen = sizeof(sa_temp);
    memset(&sa_temp, 0, sizeof(sa_temp));

    ret = getpeername(fd, &sa_temp ,&slen);
    id_addr = (u32)sa_temp.sin_addr.s_addr;
    s8 *ip_str = inet_ntoa(sa_temp.sin_addr);
    port = htons(sa_temp.sin_port);

    if (ip_str) {
        printf("== client ip: %s, port=%d ==\r\n", ip_str, port);
    }
    u32 timemark = DJY_GetSysTime()/1000;
    for (i=0; i<sizeof(gArrClientLog)/sizeof(gArrClientLog[0]); i++)
    {
        if (id_addr!=0 && gArrClientLog[i].client_ip == id_addr) {
            if (timemark - gArrClientLog[i].timestampe < 5000/*ms*/) {
                printf("matched the client ip, and Do NOT send page back!\r\n");
                return -1;
            }
            else {
                //refresh the timestampe
                gArrClientLog[i].fd = fd;
                gArrClientLog[i].client_port = port;
                gArrClientLog[i].timestampe = timemark;
                printf("matched the client ip, time out, refresh and send page to client, index=%d, fd=%d, port=%d!\r\n", i, fd, port);
                return 1;
            }
        }
        if (gArrClientLog[i].client_ip != 0 && timemark - gArrClientLog[i].timestampe > 5000/*ms*/) {
            gArrClientLog[i].client_ip = 0;
            gArrClientLog[i].client_port = 0;
            gArrClientLog[i].fd = -1;
            gArrClientLog[i].timestampe = 0;
        }
        if (gArrClientLog[i].client_ip==0 && gArrClientLog[i].timestampe == 0 && -1 == index_free) {
            index_free = i;
        }
    }
    if (i == sizeof(gArrClientLog)/sizeof(gArrClientLog[0])) {
        //add item
        if (index_free<0) {
            printf(">>>>>>>>>>>>>>>error: index_free=%d!\r\n", index_free);
            index_free = 0;
        }
        gArrClientLog[index_free].fd = fd;
        gArrClientLog[index_free].client_port = port;
        gArrClientLog[index_free].timestampe = (u32)DJY_GetSysTime()/1000;
        gArrClientLog[index_free].client_ip = id_addr;
        printf("add new client ip, and send page to client, index=%d, fd=%d, port=%d!\r\n", index_free, fd, port);
        return 1;
    }

    return 0;
}


static void handle_index_page_call(struct mg_connection *nc, struct http_message *hm) {
    mg_printf(nc, "HTTP/1.1 200 OK\r\nConnection: close\r\nCache-Control: no-cache\r\nContent-type: text/html; charset=utf-8\r\nContent-Length: %d\r\n\r\n", sizeof(web_netc));
    mg_send(nc, web_netc, sizeof(web_netc));
}

static void handle_conwifi_call(struct mg_connection *nc, struct http_message *hm) {

    /* Get form variables */
    mg_get_http_var(&hm->query_string, "ssid", ap_name, sizeof(ap_name));
    mg_get_http_var(&hm->query_string, "pwd", ap_passwd, sizeof(ap_passwd));

    printf("info: Get Info, ssid=%s, pwd=%s!\r\n", ap_name, ap_passwd);
    /* Send headers */
    mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nConnection: close\r\nCache-Control: no-cache\r\nTransfer-Encoding: chunked\r\n\r\n");
    mg_printf_http_chunk(nc, "{ \"status\": %s }", "ok");
    mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */
    exit_flag = 1;
    nc->flags |= MG_F_SEND_AND_CLOSE;
}


/*
 * wifi web 真正处理的地方，处理枚举热点请求，main page返回等。
 */
static void ev_handler(struct mg_connection *nc, s32 ev, void *ev_data) {
    struct http_message *hm = (struct http_message *) ev_data;
    struct StJsonObj *pJsonObj = &gJsonObj;
    switch (ev) {
    case MG_EV_HTTP_REQUEST:
        if (mg_vcmp(&hm->uri, "/api/scan") == 0) {
            mg_printf(nc, "%s", "HTTP/1.0 200 OK\r\nCache-Control: no-cache\r\nContent-Type: application/json;charset:utf-8;\r\n\r\n");
            atom_low_t atom_low = Int_LowAtomStart( );
            if (pJsonObj->pnew) {
                mg_printf(nc, "%s", pJsonObj->pnew);
            }
            Int_LowAtomEnd(atom_low);
            nc->flags |= MG_F_SEND_AND_CLOSE;
        }
        else if (mg_vcmp(&hm->uri, "/api/info")==0) {
            handle_conwifi_call(nc, hm);
        }
        else if (mg_vcmp(&hm->uri, "/favicon.ico") == 0) {
            mg_printf(nc, "%s", "HTTP/1.0 200 OK\r\nConnection: close\r\nCache-Control: no-cache\r\nContent-Length: 0\r\n\r\n");
            nc->flags |= MG_F_SEND_AND_CLOSE;
        }
        else if (mg_vcmp(&hm->method, "GET") == 0 && mg_vcmp(&hm->uri, "/") == 0) {

//            if (keep_once_ack_from_client(nc->sock) > 0) {
//                printf("matched the fd, do nothing!\r\n");
//            }
//            else
            {
                printf(">>>>>>START INDEX PAGE>>>>>>\r\n");
                handle_index_page_call(nc, hm); /* Serve static content */
                printf("<<<<<<ENDED INDEX PAGE<<<<<<\r\n");
            }
            //mg_printf(nc, "%s", "HTTP/1.0 200 OK\r\nConnection: close\r\n\r\n\r\n");
            nc->flags |= MG_F_SEND_AND_CLOSE;
        }
        else {
            //mg_printf(nc, "%s", "HTTP/1.0 200 OK\r\nConnection: close\r\n\r\n\r\n");
            mg_http_send_error(nc, 204, NULL);
            nc->flags |= MG_F_SEND_AND_CLOSE;
        }
        break;
    default:
        break;
    }
}
/*
 * web启动，基于mongoose api。
 */
s32 web_server_event() {
    struct mg_mgr mgr;
    struct mg_connection *nc;
    struct mg_bind_opts bind_opts;
//    s32 i;
//    s8 *cp;
    const s8 *err_str;
    mg_mgr_init(&mgr, NULL);

    /* Set HTTP server options */
    memset(&bind_opts, 0, sizeof(bind_opts));
    bind_opts.error_string = &err_str;
    nc = mg_bind_opt(&mgr, s_http_port, ev_handler, bind_opts);
    if (nc == NULL) {
        fprintf(stderr, "Error starting server on port %s: %s\n", s_http_port,
            *bind_opts.error_string);
        exit(1);
    }
    mg_set_protocol_http_websocket(nc);

    while (exit_flag==0) {
        mg_mgr_poll(&mgr, 1000);
        DJY_EventDelay(20*1000);
    }
    mg_mgr_free(&mgr);
    return 0;
}


/*
 * 重定向服务器，正确端口是80, 重定向到8080。
 * 这里注意：必须使用重定向，不然当手机连接上wifi，不会自动弹出网页，启动重定向后才能弹出网页。（具体什么原因，不太清楚，应该跟手机端处理机制有关）
 */
const s8 *str_302_jump = "HTTP/1.1 302 Object Moved\r\nLocation: http://192.168.0.253:8080\r\nConnection: keep-alive\r\nCache-Control: no-cache\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: 0\r\n\r\n";
static void ev_redirect_handler(struct mg_connection *nc, s32 ev, void *ev_data) {
    struct http_message *hm = (struct http_message *) ev_data;
    struct mg_str *s = 0;
    switch (ev) {
    case MG_EV_HTTP_REQUEST:
        /* 过滤一些重定向的请求，因为手机经常发一堆数据过来，每个都响应导致socket不够用响应慢问题 */
        if ((mg_vcmp(&hm->method, "GET") == 0) && (s = mg_get_http_header(hm, "Host")) != NULL &&
                (strncmp(s->p, "www.apple.com", strlen("www.apple.com")) == 0 ||
                 strncmp(s->p, "captive.apple.com", strlen("captive.apple.com")) == 0
                 )){
            mg_printf(nc, "%s", str_302_jump);
            printf("info: matched iPhone handset, go redirect!\r\n");
        }
        /* 当手机第一次连接wifi热点， 手机都自动发http://www.xxx.xxx/generate_204请求  */
        else if ((mg_vcmp(&hm->method, "GET") == 0) && strncmp(hm->uri.p, "/generate_204", strlen("/generate_204"))==0) {
            //mg_http_send_redirect(nc, 302,
            //    mg_mk_str("http://192.168.0.253:8080"), mg_mk_str("Connection: close\r\nContent-Type: text/html; charset=utf-8\r\n"));
            mg_printf(nc, "%s", str_302_jump);
            printf("info: matched Android URL=\"/generate_204\" go redirect!\r\n");
        }
        /* http重定向响应 */
        else if ((mg_vcmp(&hm->method, "GET") == 0) &&
                (s = mg_get_http_header(hm, "Host")) != NULL &&
                strncmp(s->p, "www.google.com", strlen("www.google.com")) == 0){
            mg_printf(nc, "%s", str_302_jump);
            printf("info: matched Host=\"/www.google.com\" go redirect!\r\n");
        }
        /* 当手机第一次连接wifi热点， 99%以上的手机都自动发connectivitycheck请求来鉴权网络是否可以使用 */
        else if ((mg_vcmp(&hm->method, "GET") == 0) && (s = mg_get_http_header(hm, "Host")) != NULL &&
                        (strncmp(s->p, "connectivitycheck", strlen("connectivitycheck")) == 0
                         )){ //锤子手机 connectivitycheck.smartisan.com
                 mg_printf(nc, "%s", str_302_jump);
            printf("info: matched smartisan handset, go redirect!\r\n");
        }
        else { /* 其他情况返回404错误  */
            mg_http_send_error(nc, 404, NULL);
            //mg_printf(nc, "%s", "HTTP/1.0 200 OK\r\nConnection: close\r\n\r\n\r\n");
        }
        nc->flags |= MG_F_SEND_AND_CLOSE;
        break;
    default:
        break;
    }
}

/*
 * 重定向服务器，侦听端口80, 重定向到端口8080
 */
s32 redirect_server_event() {
    struct mg_mgr mgr;
    struct mg_connection *nc;
    struct mg_bind_opts bind_opts;
//    s32 i;
//    s8 *cp;
    const s8 *err_str;
    mg_mgr_init(&mgr, NULL);

    /* Set HTTP server options */
    memset(&bind_opts, 0, sizeof(bind_opts));
    bind_opts.error_string = &err_str;
    nc = mg_bind_opt(&mgr, "80", ev_redirect_handler, bind_opts);
    if (nc == NULL) {
        fprintf(stderr, "Error starting server on port %s: %s\n", s_http_port,
            *bind_opts.error_string);
        exit(1);
    }
    mg_set_protocol_http_websocket(nc);

    while (exit_flag==0) {
        mg_mgr_poll(&mgr, 1000);
        DJY_EventDelay(20*1000);
    }
    mg_mgr_free(&mgr);
    return 0;
}

static const s8 *s_listening_addr = "udp://192.168.0.253:53";


/* 重定向过滤域名 ，目前没启动 */
s8 *table_dns_passed[] = {
        "captive.apple.com",
        "www.apple.com",
        "connectivitycheck.platform.hicloud.com",
        "connect.rom.miui.com",
        "connectivitycheck.gstatic.com",
        "www.google.com",
};

/* 重定向过滤域名 ，目前没启动 */
s32 is_dns_passed(s8 *dns_name)
{
    return 1;

    s32 i = 0;

    if (dns_name==0) return 0;

    for (i=0; i<sizeof(table_dns_passed)/sizeof(table_dns_passed[0]); i++){
        if (0 == strcmp(table_dns_passed[i], dns_name)) break;
    }
    return (i < sizeof(table_dns_passed)/sizeof(table_dns_passed[0]));
}

static void ev_dns_handler(struct mg_connection *nc, s32 ev, void *ev_data) {
    struct mg_dns_message *msg;
    struct mg_dns_resource_record *rr;
    struct mg_dns_reply reply;
    s32 i;

    switch (ev) {
        case MG_DNS_MESSAGE: {
            struct mbuf reply_buf;
            mbuf_init(&reply_buf, 512);
            msg = (struct mg_dns_message *) ev_data;
            reply = mg_dns_create_reply(&reply_buf, msg);

            for (i = 0; i < msg->num_questions; i++) {
                s8 rname[256];
                rr = &msg->questions[i];
                mg_dns_uncompress_name(msg, &rr->name, rname, sizeof(rname) - 1);
                if (is_dns_passed(rname)){
                    //printf("Q type %d name %s\r\n", rr->rtype, rname);
                    if (rr->rtype == MG_DNS_A_RECORD) {
                        in_addr_t addr = inet_addr("192.168.0.253");
                        mg_dns_reply_record(&reply, rr, NULL, rr->rtype, 10, &addr, 4);
                    }
                }
            }
            mg_dns_send_reply(nc, &reply);
            nc->flags |= MG_F_SEND_AND_CLOSE;
            mbuf_free(&reply_buf);
            break;
        }
    }
}

/*
 * 域名服务器： 作用是欺骗手机客户端重定向到设备板上的server服务器来。
 */
s32 dns_server_event() {
    struct mg_mgr mgr;
    struct mg_connection *nc = 0;

    DJY_EventDelay(1000*1000);//10秒后才转发，免得web还没初始化就转发。

    mg_mgr_init(&mgr, NULL);
    nc = mg_bind(&mgr, s_listening_addr, ev_dns_handler);
    if (nc==0) {
        printf("error: dns_server_event->mg_bind, nc=0!\r\n");
        return 0;
    }

    mg_set_protocol_dns(nc);

    while (exit_flag==0) {
        mg_mgr_poll(&mgr, 1000);
        DJY_EventDelay(10*1000);
    }
    mg_mgr_free(&mgr);

    return 0;
}


void DjyWifi_StartScan(FUNC_2PARAM_PTR ind_cb);
void ScanApCallBack(void *arg, uint8_t vif_idx);
uint32_t DjyWifi_GetScanResult(struct sta_scan_res **scan_result);


/*
 * 热点扫码请求回调函数
 */
static s32 scan_flag = 0;
void ScanApCallBack(void *arg, uint8_t vif_idx)
{
    UNUSED(arg);
    UNUSED(vif_idx);
    s32 cnt = 0;
    s32 i = 0;
    struct sta_scan_res *scan_result = 0;
    cnt = DjyWifi_GetScanResult(&scan_result);
    //printf("cnt=%d\r\n", cnt);
    if(scan_result) {
        for (i = 0; i < cnt; i++) {
            ap_list_add(scan_result[i].ssid);
        }
    }
    if(scan_result) free(scan_result);
    scan_flag = 1;
}

/*
 * 热点扫描事件，定时扫描热点
 */
s32 scan_ap_event() {
    s32 secs = 5;
    while (1){
        scan_flag = 0;
        DjyWifi_StartScan(ScanApCallBack);
        while (scan_flag == 0) {
            DJY_EventDelay(100*1000);
        }
        if (ap_list_counter()>=6) {
            secs = 1000;
        }
        else if (ap_list_counter()>=4) {
            secs = 30;
        }
        else if (ap_list_counter()>=2) {
            secs = 10;
        }
        DJY_EventDelay(secs*1000*1000);
    }
    return 0;
}

/*
 * 获取用户通过web设定的wifi密码，wifi热点名信息。
 */
s32 GetNetInfo(s8 *ssid, s32 ssid_len, s8 *passwd, s32 pwd_len)
{
    s32 len = 0;
    if (ssid) {
        len = strlen(ap_name)+1;
        len = ssid_len < len ? ssid_len : len;
        memcpy(ssid, ap_name, len);
    }
    if (passwd) {
        len = strlen(ap_passwd)+1;
        len = pwd_len < len ? pwd_len : len;
        memcpy(passwd, ap_passwd, len);
    }
    return 0;
}


/*
 * 过滤用户功能，目前没启动。
 */
s32 delete_once_ack_from_client(s32 fd)
{
    s32 i = 0;
    for (i=0; i<sizeof(gArrClientLog)/sizeof(gArrClientLog[0]); i++)
    {
        if (fd == gArrClientLog[i].fd ) {
            gArrClientLog[i].client_ip = 0;
            gArrClientLog[i].client_port = 0;
            gArrClientLog[i].fd = -1;
            gArrClientLog[i].timestampe = 0;
        }
    }
    return 0;
}

/*
 * AP配网功能初始化
 */
bool_t web_config_event_init()
{
    u16 evt_scan = DJY_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS+2, 0, 0, scan_ap_event, 0, 0x1000, "scan_ap_event");
    if(evt_scan != CN_EVENT_ID_INVALID)
    {
        DJY_EventPop(evt_scan, NULL, 0, 0, 0, 0);
    }
    else
    {
        printf("\r\nError: evt_scan Event Start Fail!\r\n");
    }

    u16 evt_redirect = DJY_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 0, redirect_server_event, 0, 0x1000, "redirect_server_event");
    if(evt_redirect != CN_EVENT_ID_INVALID)
    {
        DJY_EventPop(evt_redirect, NULL, 0, 0, 0, 0);
    }
    else
    {
        printf("\r\nError: evt_evt_redirect Event Start Fail!\r\n");
    }

    u16 evt_dns = DJY_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 0, dns_server_event, 0, 0x1000, "dns_server_event");
    if(evt_dns != CN_EVENT_ID_INVALID)
    {
        DJY_EventPop(evt_dns, NULL, 0, 0, 0, 0);
    }
    else
    {
        printf("\r\nError: evt_dns Event Start Fail!\r\n");
    }

    web_server_event();

    return true;
}

