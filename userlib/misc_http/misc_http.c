#include "misc_http.h"
#include "mongoose.h"
#include "cJSON.h"
#include <stdlib.h>
int MiscParseData(MiscHttp_StMiscHttpObj *phttp_obj, char *buf, int len)
{
    int ret = 0;
    if (phttp_obj->parser) {
        ret = phttp_obj->parser(phttp_obj, buf, len);
    }
    return ret;
}

static void cb_misc_http_hdl(struct mg_connection *nc, int ev, void *ev_data)
{
    struct http_message *hm = (struct http_message *) ev_data;
    struct MiscHttp_StMiscHttpObj *phttp_obj = (struct MiscHttp_StMiscHttpObj *)nc->user_data;
    char *p = 0;
    int ret = 0;
    if (phttp_obj == 0) {
        printf("error: %s phttp_obj==NULL!\r\n", __FUNCTION__);
        return;
    }
    if (phttp_obj->is_break) {
        nc->flags |= MG_F_CLOSE_IMMEDIATELY;
        phttp_obj->status = HTTP_MISC_ERROR;
        printf("error: %s phttp_obj->is_break out!\r\n", __FUNCTION__);
        return;
    }
    switch (ev) {
    case MG_EV_CONNECT:
        if (*(int *)ev_data != 0) {
            printf("warning: %s MG_EV_CONNECT failed: %d!\r\n", __FUNCTION__, *(int *)ev_data);
            nc->flags |= MG_F_CLOSE_IMMEDIATELY;
            phttp_obj->status = HTTP_MISC_ERROR;
            break;
        }
        phttp_obj->status = HTTP_MISC_HEADER;
        break;
    case MG_EV_HTTP_REPLY:
        nc->flags |= MG_F_SEND_AND_CLOSE;

        if (hm->body.len > 0) {
            p = malloc(hm->body.len + 1);
            if (p == 0) {
                phttp_obj->status = HTTP_MISC_ERROR;
                goto END_FUN;
            }
            memcpy(p, hm->body.p, hm->body.len);
            p[hm->body.len] = 0;
            ret = MiscParseData(phttp_obj, p, hm->body.len+1);
            if (ret >= 0) {
                phttp_obj->status = HTTP_MISC_END;
            }
            else {
                phttp_obj->status = HTTP_MISC_ERROR;
            }
        }
        else {
            phttp_obj->status = HTTP_MISC_ERROR;
        }
        break;
    case MG_EV_CLOSE:
        if (phttp_obj && phttp_obj->status != HTTP_MISC_END) {
            phttp_obj->status = HTTP_MISC_ERROR;
        }
        break;
    }
END_FUN:
    if (p) {
        free(p);
    }
}

int MiscHttpInit(struct MiscHttp_StMiscHttpObj *http_obj, FUN_PARSER my_parser, void *user)
{
    if (http_obj) {
        memset(http_obj, 0, sizeof(*http_obj));
    }
    http_obj->parser = my_parser;
    http_obj->user = user;
    return 0;
}
int MiscHttpDeinit(struct MiscHttp_StMiscHttpObj *http_obj)
{
    if (http_obj) {
        memset(http_obj, 0, sizeof(*http_obj));
    }
    http_obj->parser = 0;
    http_obj->user = 0;
    return 0;
}

int MiscHttpBreak(MiscHttp_StMiscHttpObj *http_obj)
{
    if (http_obj) {
        http_obj->is_break = 1;
    }
    return 0;
}

int MiscHttpGet(MiscHttp_StMiscHttpObj *http_obj, char *url, int timeout_ms)
{
    int ret = 0;
    int timemark = 0;
    struct mg_connection *nc = 0;
    struct mg_mgr mgr;
    mg_mgr_init(&mgr, NULL);

    nc = mg_connect_http(&mgr, cb_misc_http_hdl, url, 0, 0);

    if (nc == 0) { ret = -1; goto END_FUN;  };

    nc->user_data = http_obj;

    timemark = DJY_GetSysTime()/1000;

    while (1) {
        mg_mgr_poll(&mgr, 500);
        if (DJY_GetSysTime()/1000 - timemark > timeout_ms) {
            ret = -1;
            break;
        }
        if (http_obj->status == HTTP_MISC_END) {
            ret = 0;
            break;
        }
        if (http_obj->status == HTTP_MISC_ERROR) {
            ret = -2;
            break;
        }
        DJY_EventDelay(10 * 1000);
    }


END_FUN:

    mg_mgr_free(&mgr);
    return ret;
}


int MiscHttpPostForm(MiscHttp_StMiscHttpObj *http_obj, char *url, char *boundary, struct MiscHttp_StNameValue *arr_content, int arr_cnts, int timeout_ms)
{
    int ret = 0;
    int i = 0;
    int timemark = 0;
    struct mg_connection *nc = 0;
    struct mg_mgr mgr;
    char *ptmp = 0;
    char *pcontent = 0;

    mg_mgr_init(&mgr, NULL);

    ptmp = malloc(1024);
    if (ptmp == 0) goto END_FUN;

    pcontent = malloc(2048);
    if (pcontent == 0) goto END_FUN;


    if (boundary) {
        boundary = "DJYOSFormBoundary7MA4YWxkTrZu0gW";
    }

    pcontent[0] = 0;
    sprintf(&pcontent[strlen(pcontent)], "--%s\r\n", boundary);
    for (i = 0; i < arr_cnts; i++) {
        sprintf(&pcontent[strlen(pcontent)], "Content-Disposition: form-data; name=\"%s\"\r\n\r\n%s\r\n", arr_content[i].name, arr_content[i].value);
        if(i != (arr_cnts-1))
        {
            sprintf(&pcontent[strlen(pcontent)], "--%s\r\n", boundary);
        }
    }
    sprintf(&pcontent[strlen(pcontent)], "--%s--", boundary);

    sprintf(ptmp, "Content-Type: multipart/form-data;boundary=%s\r\n", boundary);

    nc = mg_connect_http(&mgr, cb_misc_http_hdl, url, ptmp, pcontent);

    if (nc == 0) { ret = -1; goto END_FUN; };

    nc->user_data = http_obj;

    timemark = DJY_GetSysTime() / 1000;

    while (1) {
        mg_mgr_poll(&mgr, 500);
        if (DJY_GetSysTime() / 1000 - timemark > timeout_ms) {
            ret = -1;
            break;
        }
        if (http_obj->status == HTTP_MISC_END) {
            ret = 0;
            break;
        }
        if (http_obj->status == HTTP_MISC_ERROR) {
            ret = -2;
            break;
}
        DJY_EventDelay(10 * 1000);
    }


END_FUN:
    mg_mgr_free(&mgr);
    if (ptmp) free(ptmp);
    if (pcontent) free(pcontent);
    return ret;
}

int MiscHttpPostContent(MiscHttp_StMiscHttpObj *http_obj, char *url, char *add_header, char *content, int timeout_ms)
{
    int ret = 0;
//    int i = 0;
    int timemark = 0;
    struct mg_connection *nc = 0;
    struct mg_mgr mgr;

    mg_mgr_init(&mgr, NULL);

    if (content == 0 || content[0]==0) content = " ";//如果content为NULL， 也是设置一个字符，不然mg_connect_http会设置GET，而不是POST

    nc = mg_connect_http(&mgr, cb_misc_http_hdl, url, add_header, content);

    if (nc == 0) { ret = -1; goto END_FUN; };

    nc->user_data = http_obj;

    timemark = DJY_GetSysTime() / 1000;

    while (1) {
        mg_mgr_poll(&mgr, 500);
        if (DJY_GetSysTime() / 1000 - timemark > timeout_ms) {
            ret = -1;
            break;
        }
        if (http_obj->status == HTTP_MISC_END) {
            ret = 0;
            break;
        }
        if (http_obj->status == HTTP_MISC_ERROR) {
            ret = -2;
            break;
        }
        DJY_EventDelay(10 * 1000);
    }


END_FUN:
    mg_mgr_free(&mgr);
    return ret;
}

//下面是一个获取时间戳的示范，示范而已，实际上取时间戳应该用系统的ntp功能
#include "cJSON.h"
int do_timestamp(struct MiscHttp_StMiscHttpObj *pHttpObj, char *data, int len)
{
    int ret = -1;
    cJSON* cjson = 0;
    char str_timestamp[100] = { 0 };
    if (pHttpObj == 0 || data == 0 || len <= 0) return -1;

    cjson = cJSON_Parse(data);
    if (cjson && pHttpObj->user)
    {
        cJSON* sub = cJSON_GetObjectItem(cjson, "data");
        if (sub)
        {
            sub = cJSON_GetObjectItem(sub, "t");
            if (sub->valuestring) {
                memset(str_timestamp, 0, sizeof(str_timestamp));
                memcpy(str_timestamp, sub->valuestring, 10);
                *(unsigned int*)pHttpObj->user = atol(str_timestamp);
                ret = 0;
            }
            //printf("info: timestamp = %d !\r\n", *(unsigned int*)pHttpObj->user);
        }
    }
    if (cjson) {
        cJSON_Delete(cjson);
    }
    return ret;
}

int timestamp_http(unsigned int *timestamp, int timeout_ms)
{
    int ret = 0;
    MiscHttp_StMiscHttpObj http_obj;
//    misc_http_init(&http_obj, do_timestamp, timestamp
    MiscHttpInit(&http_obj, do_timestamp, timestamp);
//  ret = misc_http_get(&http_obj, "http://api.m.taobao.com/rest/api3.do?api=mtop.common.getTimestamp", timeout_ms);
    ret = MiscHttpGet(&http_obj, "http://api.m.taobao.com/rest/api3.do?api=mtop.common.getTimestamp", timeout_ms);
    if (ret >= 0) {
        printf("info: timestamp_http get successfully!\r\n");
    }
    else {
        printf("info: timestamp_http get failed!\r\n");
    }
//  misc_http_deinit(&http_obj);
    MiscHttpDeinit(&http_obj);
    return ret;
}



